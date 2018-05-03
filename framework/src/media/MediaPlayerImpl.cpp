/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <media/MediaPlayer.h>
#include "PlayerWorker.h"
#include "MediaPlayerImpl.h"

#include <debug.h>
#include "audio/audio_manager.h"

namespace media {
MediaPlayerImpl::MediaPlayerImpl()
{
	mPlayerObserver = nullptr;
	mCurState = PLAYER_STATE_NONE;
	mBuffer = nullptr;
	mBufSize = 0;
	mInputDataSource = nullptr;

	static int playerId = 1;
	mId = playerId++;
}

player_result_t MediaPlayerImpl::create()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer create\n");
	if (mCurState != PLAYER_STATE_NONE) {
		meddbg("MediaPlayer create fail : wrong state\n");
		return PLAYER_ERROR;
	}

	PlayerWorker& mpw = PlayerWorker::getWorker();
	mpw.startWorker();
	mpw.getQueue().enQueue(&MediaPlayerImpl::notifySync, shared_from_this());
	mSyncCv.wait(lock);

	mCurState = PLAYER_STATE_IDLE;
	return PLAYER_OK;
}

player_result_t MediaPlayerImpl::destroy()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer destroy\n");
	if (mCurState != PLAYER_STATE_IDLE) {
		meddbg("MediaPlayer destroy fail : wrong state\n");
		return PLAYER_ERROR;
	}

	PlayerWorker& mpw = PlayerWorker::getWorker();
	mpw.getQueue().enQueue(&MediaPlayerImpl::notifySync, shared_from_this());
	mSyncCv.wait(lock);
	mpw.stopWorker();

	if (mPlayerObserver) {
		PlayerObserverWorker& pow = PlayerObserverWorker::getWorker();
		pow.stopWorker();
		mPlayerObserver = nullptr;
	}

	mCurState = PLAYER_STATE_NONE;
	return PLAYER_OK;
}

player_result_t MediaPlayerImpl::prepare()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer prepare\n");
	if (mCurState != PLAYER_STATE_IDLE) {
		meddbg("MediaPlayer prepare fail : wrong state\n");
		return PLAYER_ERROR;
	}

	PlayerWorker& mpw = PlayerWorker::getWorker();
	mpw.getQueue().enQueue(&MediaPlayerImpl::notifySync, shared_from_this());
	mSyncCv.wait(lock);

	if (mInputDataSource == nullptr) {
		meddbg("MediaPlayer prepare fail : mInputDataSource is not set\n");
		return PLAYER_ERROR;
	}

	if (!mInputDataSource->open()) {
		meddbg("MediaPlayer prepare fail : file open fail\n");
		return PLAYER_ERROR;
	}

	if (set_audio_stream_out(mInputDataSource->getChannels(), mInputDataSource->getSampleRate(),
							 mInputDataSource->getPcmFormat()) != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaPlayer prepare fail : set_audio_stream_out fail\n");
		return PLAYER_ERROR;
	}

	mBufSize = get_output_frames_byte_size(get_output_frame_count());
	if (mBufSize < 0) {
		meddbg("MediaPlayer prepare fail : get_output_frames_byte_size fail\n");
		return PLAYER_ERROR;
	}

	medvdbg("MediaPlayer mBuffer size : %d\n", mBufSize);

	mBuffer = new unsigned char[mBufSize];
	if (!mBuffer) {
		meddbg("MediaPlayer prepare fail : mBuffer allocation fail\n");
		return PLAYER_ERROR;
	}

	mCurState = PLAYER_STATE_READY;
	return PLAYER_OK;
}

player_result_t MediaPlayerImpl::unprepare()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer unprepare\n");
	if (mCurState == PLAYER_STATE_NONE || mCurState == PLAYER_STATE_IDLE) {
		meddbg("MediaPlayer unprepare fail : wrong state\n");
		return PLAYER_ERROR;
	}

	PlayerWorker& mpw = PlayerWorker::getWorker();
	mpw.getQueue().enQueue(&MediaPlayerImpl::notifySync, shared_from_this());
	mSyncCv.wait(lock);

	if (mBuffer) {
		delete[] mBuffer;
		mBuffer = nullptr;
	}
	mBufSize = 0;

	if (reset_audio_stream_out() != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaPlayer unprepare fail : reset_audio_stream_out fail\n");
		return PLAYER_ERROR;
	}

	mInputDataSource->close();

	mCurState = PLAYER_STATE_IDLE;
	return PLAYER_OK;
}

player_result_t MediaPlayerImpl::start()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer start\n");
	if (mCurState == PLAYER_STATE_NONE || mCurState == PLAYER_STATE_IDLE) {
		meddbg("MediaPlayer unprepare fail : wrong state\n");
		return PLAYER_ERROR;
	}

	PlayerWorker& mpw = PlayerWorker::getWorker();
	mpw.getQueue().enQueue(&PlayerWorker::startPlayer, &mpw, shared_from_this());

	return PLAYER_OK;
}

player_result_t MediaPlayerImpl::stop()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer stop\n");
	if (mCurState == PLAYER_STATE_NONE || mCurState == PLAYER_STATE_IDLE) {
		meddbg("MediaPlayer stop fail : wrong state\n");
		return PLAYER_ERROR;
	}

	PlayerWorker& mpw = PlayerWorker::getWorker();
	mpw.getQueue().enQueue(&PlayerWorker::stopPlayer, &mpw, shared_from_this());

	return PLAYER_OK;
}

player_result_t MediaPlayerImpl::pause()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer pause\n");
	if (mCurState == PLAYER_STATE_NONE || mCurState == PLAYER_STATE_IDLE) {
		meddbg("MediaPlayer pause fail : wrong state\n");
		return PLAYER_ERROR;
	}

	PlayerWorker& mpw = PlayerWorker::getWorker();
	mpw.getQueue().enQueue(&PlayerWorker::pausePlayer, &mpw, shared_from_this());

	return PLAYER_OK;
}

int MediaPlayerImpl::getVolume()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer getVolume\n");
	if (mCurState == PLAYER_STATE_NONE) {
		meddbg("MediaPlayer getVolume fail : wrong state\n");
		return -1;
	}

	return get_output_audio_volume();
}

player_result_t MediaPlayerImpl::setVolume(int vol)
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer setVolume\n");
	if (mCurState == PLAYER_STATE_NONE) {
		meddbg("MediaPlayer setVolume fail : wrong state\n");
		return PLAYER_ERROR;
	}

	int vol_max = get_max_audio_volume();
	if (vol < 0 || vol > vol_max) {
		meddbg("MediaPlayer setVolume fail : invalid argument. volume level should be 0(Min) ~ %d(Max)\n", vol_max);
		return PLAYER_ERROR;
	}

	if (set_output_audio_volume(vol) != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaPlayer setVolume fail : audio manager failed\n", vol_max);
		return PLAYER_ERROR;
	}
	
	medvdbg("MediaPlayer setVolume success\n");
	return PLAYER_OK;
}

player_result_t MediaPlayerImpl::setDataSource(std::unique_ptr<stream::InputDataSource> source)
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer setDataSource\n");
	if (!source) {
		meddbg("MediaPlayer setDataSource fail : invalid argument. DataSource should not be nullptr\n");
		return PLAYER_ERROR;
	}

	mInputDataSource = std::move(source);
	return PLAYER_OK;
}

void MediaPlayerImpl::setObserver(std::shared_ptr<MediaPlayerObserverInterface> observer)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer setObserver\n");
	PlayerWorker& mpw = PlayerWorker::getWorker();
	mpw.getQueue().enQueue(&MediaPlayerImpl::notifySync, shared_from_this());
	mSyncCv.wait(lock);
	PlayerObserverWorker& pow = PlayerObserverWorker::getWorker();
	pow.startWorker();
	mPlayerObserver = observer;
}

player_state_t MediaPlayerImpl::getState()
{
	medvdbg("MediaPlayer getState\n");
	return mCurState;
}

player_result_t MediaPlayerImpl::seekTo(int msec)
{
	medvdbg("MediaPlayer seekTo %d msec\n", msec);
	// TODO : implementation of seekTo
	return PLAYER_OK;
}

void MediaPlayerImpl::notifySync()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	mSyncCv.notify_one();
}

void MediaPlayerImpl::notifyObserver(player_observer_command_t cmd)
{
	if (mPlayerObserver != nullptr) {
		PlayerObserverWorker& pow = PlayerObserverWorker::getWorker();
		switch (cmd) {
		case PLAYER_OBSERVER_COMMAND_STARTED:
			pow.getQueue().enQueue(&MediaPlayerObserverInterface::onPlaybackStarted, mPlayerObserver, mId);
			break;
		case PLAYER_OBSERVER_COMMAND_FINISHIED:
			pow.getQueue().enQueue(&MediaPlayerObserverInterface::onPlaybackFinished, mPlayerObserver, mId);
			break;
		case PLAYER_OBSERVER_COMMAND_ERROR:
			pow.getQueue().enQueue(&MediaPlayerObserverInterface::onPlaybackError, mPlayerObserver, mId);
			break;
		}
	}
}

int MediaPlayerImpl::playback(int size)
{
	return start_audio_stream_out(mBuffer, get_output_bytes_frame_count(size));
}

MediaPlayerImpl::~MediaPlayerImpl()
{
	player_result_t ret;

	if (mCurState > PLAYER_STATE_IDLE) {
		ret = unprepare();
		if (ret != PLAYER_OK) {
			meddbg("~MediaPlayer fail : unprepare fail\n");
		}
	}

	if (mCurState == PLAYER_STATE_IDLE) {
		ret = destroy();
		if (ret != PLAYER_OK) {
			meddbg("~MediaPlayer fail : destroy fail\n");
		}
	}
}
} // namespace media
