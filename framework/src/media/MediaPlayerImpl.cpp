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
#include <media/FocusManager.h>
#include "PlayerWorker.h"
#include "MediaPlayerImpl.h"

#include <debug.h>
#include <errno.h>
#include <stdarg.h>
#include "audio/audio_manager.h"

namespace media {

#define LOG_STATE_INFO(state) medvdbg("state at %s[line : %d] : %s\n", __func__, __LINE__, player_state_names[(state)])
#define LOG_STATE_DEBUG(state) meddbg("state at %s[line : %d] : %s\n", __func__, __LINE__, player_state_names[(state)])

MediaPlayerImpl::MediaPlayerImpl(MediaPlayer &player) : mPlayer(player)
{
	mPlayerObserver = nullptr;
	mCurState = PLAYER_STATE_NONE;
	mBuffer = nullptr;
	mBufSize = 0;
	stream_info_t *info;
	int ret = stream_info_create(STREAM_TYPE_MEDIA, &info);
	if (ret != OK) {
		meddbg("stream_info_create failed ret : %d\n", ret);
	}
	mStreamInfo = std::shared_ptr<stream_info_t>(info, [](stream_info_t *ptr) { stream_info_destroy(ptr); });
}

player_result_t MediaPlayerImpl::create()
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);

	PlayerWorker &mpw = PlayerWorker::getWorker();

	mpw.enQueue(&MediaPlayerImpl::createPlayer, shared_from_this(), std::ref(ret));
	meddbg("createPlayer enqueued. player: %x\n", &mPlayer);
	mSyncCv.wait(lock);

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::createPlayer(player_result_t &ret)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != PLAYER_STATE_NONE) {
		meddbg("%s Fail : invalid state mPlayer : %x\n", __func__, &mPlayer);
		LOG_STATE_DEBUG(mCurState);
		ret = PLAYER_ERROR_INVALID_STATE;
		return notifySync();
	}

	mCurState = PLAYER_STATE_IDLE;
	notifySync();
}

player_result_t MediaPlayerImpl::destroy()
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	PlayerWorker &mpw = PlayerWorker::getWorker();
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);

	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::destroyPlayer, shared_from_this(), std::ref(ret));
	meddbg("destroyPlayer enqueued. player: %x\n", &mPlayer);
	mSyncCv.wait(lock);

	if (ret == PLAYER_OK && mPlayerObserver) {
		PlayerObserverWorker &pow = PlayerObserverWorker::getWorker();
		mObserverQueue.clearQueue();
		pow.enQueue(&MediaPlayerImpl::dequeueAndRunObserverCallback, shared_from_this());
		if (pow.isSameThread()) {
			mPlayerObserver = nullptr;
		} else {
			pow.enQueue(&MediaPlayerImpl::unsetPlayerObserver, shared_from_this());
			mSyncCv.wait(lock);
		}
	}

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::unsetPlayerObserver()
{
	mPlayerObserver = nullptr;
	notifySync();
}

void MediaPlayerImpl::destroyPlayer(player_result_t &ret)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != PLAYER_STATE_IDLE && mCurState != PLAYER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state mPlayer : %x\n", __func__, &mPlayer);
		LOG_STATE_DEBUG(mCurState);
		ret = PLAYER_ERROR_INVALID_STATE;
		return notifySync();
	}

	mCurState = PLAYER_STATE_NONE;
	notifySync();
}

void MediaPlayerImpl::dequeueAndRunObserverCallback()
{
	if (!mObserverQueue.isEmpty()) {
		std::function<void()> run = mObserverQueue.deQueue();
		if (run != nullptr) {
			run();
		}
	}
}

player_result_t MediaPlayerImpl::prepare()
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;

	stream_focus_state_t streamState = getStreamFocusState();
	if (streamState == STREAM_FOCUS_STATE_RELEASED) {
		ret = PLAYER_ERROR_FOCUS_NOT_READY;
		meddbg("MediaPlayer prepare failed. ret: %d, player: %x\n", ret, &mPlayer);
		return ret;
	}

	std::unique_lock<std::mutex> lock(mCmdMtx);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::preparePlayer, shared_from_this(), std::ref(ret));
	meddbg("preparePlayer enqueued. player: %x\n", &mPlayer);
	mSyncCv.wait(lock);

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::preparePlayer(player_result_t &ret)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != PLAYER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state mPlayer : %x\n", __func__, &mPlayer);
		LOG_STATE_DEBUG(mCurState);
		ret = PLAYER_ERROR_INVALID_STATE;
		return notifySync();
	}

	if (!mInputHandler.open()) {
		meddbg("MediaPlayer prepare fail : open fail\n");
		ret = PLAYER_ERROR_FILE_OPEN_FAILED;
		return notifySync();
	}

	auto source = mInputHandler.getDataSource();
	if (set_audio_stream_out(source->getChannels(), source->getSampleRate(),
							 source->getPcmFormat(), mStreamInfo->id) != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaPlayer prepare fail : set_audio_stream_out fail\n");
		ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	audio_manager_result_t res = set_stream_out_policy(mStreamInfo->policy, mStreamInfo->id);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaPlayer prepare fail : set_stream_out_policy fail. res: %d\n", res);
		ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	mBufSize = get_output_card_buffer_size();
	if (mBufSize < 0) {
		meddbg("MediaPlayer prepare fail : get_output_frames_byte_size fail\n");
		ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	meddbg("MediaPlayer mBuffer size : %d\n", mBufSize);

	mBuffer = new unsigned char[mBufSize];
	if (!mBuffer) {
		meddbg("MediaPlayer prepare fail : mBuffer allocation fail\n");
		if (get_errno() == ENOMEM) {
			ret = PLAYER_ERROR_OUT_OF_MEMORY;
		} else {
			ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
		}
		return notifySync();
	}

	mCurState = PLAYER_STATE_READY;
	return notifySync();
}

player_result_t MediaPlayerImpl::prepareAsync()
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;

	stream_focus_state_t streamState = getStreamFocusState();
	if (streamState == STREAM_FOCUS_STATE_RELEASED) {
		ret = PLAYER_ERROR_FOCUS_NOT_READY;
		meddbg("MediaPlayer prepareAsync failed. ret: %d, player: %x\n", ret, &mPlayer);
		return ret;
	}

	std::unique_lock<std::mutex> lock(mCmdMtx);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::prepareAsyncPlayer, shared_from_this());

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return PLAYER_OK;
}

void MediaPlayerImpl::prepareAsyncPlayer()
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != PLAYER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state mPlayer : %x\n", __func__, &mPlayer);
		LOG_STATE_DEBUG(mCurState);
		notifyObserver(PLAYER_OBSERVER_COMMAND_ASYNC_PREPARED, PLAYER_ERROR_INVALID_STATE);
		return;
	}

	mCurState = PLAYER_STATE_PREPARING;

	if (!mInputHandler.doStandBy()) {
		meddbg("MediaPlayer prepare fail : doStandBy fail\n");
		notifyObserver(PLAYER_OBSERVER_COMMAND_ASYNC_PREPARED, PLAYER_ERROR_INTERNAL_OPERATION_FAILED);
		return;
	}
}

player_result_t MediaPlayerImpl::unprepare()
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::unpreparePlayer, shared_from_this(), std::ref(ret));
	meddbg("unpreparePlayer enqueued. player: %x\n", &mPlayer);
	mSyncCv.wait(lock);

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::unpreparePlayer(player_result_t &ret)
{
	LOG_STATE_INFO(mCurState);

	player_result_t errCode = unpreparePlayback();
	if (ret == PLAYER_OK && errCode != PLAYER_OK) {
		ret = errCode;
	}

	return notifySync();
}

player_result_t MediaPlayerImpl::unpreparePlayback(void)
{
	player_result_t ret = PLAYER_OK;
	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (mCurState == PLAYER_STATE_NONE || mCurState == PLAYER_STATE_IDLE || mCurState == PLAYER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state mPlayer : %x\n", __func__, &mPlayer);
		LOG_STATE_DEBUG(mCurState);
		return PLAYER_ERROR_INVALID_STATE;
	}

	if (reset_audio_stream_out(mStreamInfo->id) != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaPlayer unprepare fail : reset_audio_stream_out fail\n");
		return PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
	}

	mInputHandler.close();

	if (mBuffer) {
		delete[] mBuffer;
		mBuffer = nullptr;
	}
	mBufSize = 0;
	mCurState = PLAYER_STATE_IDLE;
	mpw.removePlayer(shared_from_this());
	return ret;
}

player_result_t MediaPlayerImpl::reset()
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	LOG_STATE_INFO(mCurState);

	if (mCurState == PLAYER_STATE_READY || mCurState == PLAYER_STATE_PLAYING || mCurState == PLAYER_STATE_PAUSED || mCurState == PLAYER_STATE_COMPLETING) {
		mInputHandler.close();
		if (reset_audio_stream_out(mStreamInfo->id) != AUDIO_MANAGER_SUCCESS) {
			meddbg("MediaPlayer reset fail : reset_audio_stream_out fail\n");
		}
		
		if (mBuffer) {
			delete[] mBuffer;
			mBuffer = nullptr;
		}
		mBufSize = 0;
	}

	mCurState = PLAYER_STATE_IDLE;

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return PLAYER_OK;
}

player_result_t MediaPlayerImpl::start()
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;

	stream_focus_state_t streamState = getStreamFocusState();
	if (streamState == STREAM_FOCUS_STATE_RELEASED) {
		ret = PLAYER_ERROR_FOCUS_NOT_READY;
		meddbg("MediaPlayer start failed. ret: %d, player: %x\n", ret, &mPlayer);
		return ret;
	}

	std::unique_lock<std::mutex> lock(mCmdMtx);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::startPlayer, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);
	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::startPlayer(player_result_t &ret)
{
	LOG_STATE_INFO(mCurState);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (mCurState != PLAYER_STATE_READY && mCurState != PLAYER_STATE_PAUSED &&
		mCurState != PLAYER_STATE_COMPLETED && mCurState != PLAYER_STATE_PLAYING && mCurState != PLAYER_STATE_COMPLETING) {
		meddbg("%s Fail : invalid state mPlayer : %x\n", __func__, &mPlayer);
		LOG_STATE_DEBUG(mCurState);
		ret = PLAYER_ERROR_INVALID_STATE;
		return notifySync();
	}

	if (mCurState == PLAYER_STATE_PLAYING || mCurState == PLAYER_STATE_COMPLETING) {
		meddbg("Already Playing, Ignore Start\n");
		return notifySync();
	}

	if (mCurState == PLAYER_STATE_COMPLETED) {
		mInputHandler.stop();
		mInputHandler.seekTo(0);
		mInputHandler.start();
	}

	audio_manager_result_t res;

	if (mCurState == PLAYER_STATE_PAUSED) {
		auto source = mInputHandler.getDataSource();
		if (set_audio_stream_out(source->getChannels(), source->getSampleRate(),
								 source->getPcmFormat(), mStreamInfo->id) != AUDIO_MANAGER_SUCCESS) {
			meddbg("MediaPlayer startPlayer fail : set_audio_stream_out fail\n");
			ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
			return notifySync();
		}
		res = set_stream_out_policy(mStreamInfo->policy, mStreamInfo->id);
		if (res != AUDIO_MANAGER_SUCCESS) {
			meddbg("MediaPlayer startPlayer fail : set_stream_out_policy fail. res: %d\n", res);
			ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
			return notifySync();
		}
	}

	res = set_output_audio_mixer(mStreamInfo->id);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaPlayer prepare fail : set_output_audio_mixer fail. ret: %d\n", res);
		ret = (res == AUDIO_MANAGER_DEVICE_NOT_SUPPORT) ? PLAYER_ERROR_DEVICE_NOT_SUPPORTED : PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	res = set_output_stream_volume(mStreamInfo->policy);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaPlayer prepare fail : set_output_stream_volume fail. ret: %d\n", res);
		ret = (res == AUDIO_MANAGER_DEVICE_NOT_SUPPORT) ? PLAYER_ERROR_DEVICE_NOT_SUPPORTED : PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	res = set_audio_stream_mute_from_json(mStreamInfo->policy);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaPlayer prepare fail : set_audio_stream_mute_from_json fail. policy: %d, ret: %d\n", mStreamInfo->policy, res);
		ret = (res == AUDIO_MANAGER_DEVICE_NOT_SUPPORT) ? PLAYER_ERROR_DEVICE_NOT_SUPPORTED : PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	mpw.addPlayer(shared_from_this());
	FocusManager &fm = FocusManager::getFocusManager();
	FocusManager::FocusLossListener playerFocusLossListener = std::bind(&MediaPlayerImpl::onFocusLossListener, shared_from_this());
	fm.registerPlayerFocusLossListener(playerFocusLossListener, mStreamInfo->id);
	mCurState = PLAYER_STATE_PLAYING;
	return notifySync();
}

player_result_t MediaPlayerImpl::stop()
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;

	stream_focus_state_t streamState = getStreamFocusState();
	if (streamState == STREAM_FOCUS_STATE_RELEASED) {
		ret = PLAYER_ERROR_FOCUS_NOT_READY;
		meddbg("MediaPlayer stop failed. ret: %d, player: %x\n", ret, &mPlayer);
		return ret;
	}

	std::unique_lock<std::mutex> lock(mCmdMtx);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::stopPlayer, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);
	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::stopPlayer(player_result_t &ret)
{
	LOG_STATE_INFO(mCurState);
	ret = stopPlayback(false);

	FocusManager &fm = FocusManager::getFocusManager();
	fm.unregisterPlayerFocusLossListener(mStreamInfo->id);
	return notifySync();
}

player_result_t MediaPlayerImpl::stopPlayback(bool drain)
{
	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (mCurState != PLAYER_STATE_PLAYING && mCurState != PLAYER_STATE_PAUSED &&
		mCurState != PLAYER_STATE_COMPLETED && mCurState != PLAYER_STATE_READY && mCurState != PLAYER_STATE_COMPLETING) {
		meddbg("%s Fail : invalid state mPlayer : %x\n", __func__, &mPlayer);
		LOG_STATE_DEBUG(mCurState);
		return PLAYER_ERROR_INVALID_STATE;
	}

	if (mCurState == PLAYER_STATE_READY) {
		meddbg("Currently in Stop State, Ignore Stop\n");
		return PLAYER_OK;
	}

	if (mCurState == PLAYER_STATE_COMPLETED) {
		mCurState = PLAYER_STATE_READY;
		meddbg("Playback already Finished, Ignore Stop\n");
		return PLAYER_OK;
	}

	audio_manager_result_t result = stop_audio_stream_out(mStreamInfo->id, drain);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("stop_audio_stream_out failed ret : %d\n", result);
		return PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
	}

	mCurState = PLAYER_STATE_READY;
	mpw.removePlayer(shared_from_this());

	return PLAYER_OK;
}

void MediaPlayerImpl::stopPlaybackInternal(bool drain)
{
	if (mCurState != PLAYER_STATE_PLAYING) {
		return;
	}

	mCurState = PLAYER_STATE_READY;

	PlayerWorker &mpw = PlayerWorker::getWorker();
	mpw.removePlayer(shared_from_this());

	audio_manager_result_t result = stop_audio_stream_out(mStreamInfo->id, drain);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("stop_audio_stream_out failed ret : %d\n", result);
	}

	FocusManager &fm = FocusManager::getFocusManager();
	fm.unregisterPlayerFocusLossListener(mStreamInfo->id);

	notifyObserver(PLAYER_OBSERVER_COMMAND_PLAYBACK_ERROR, PLAYER_ERROR_INTERNAL_OPERATION_FAILED);
}

player_result_t MediaPlayerImpl::pause()
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;

	stream_focus_state_t streamState = getStreamFocusState();
	if (streamState == STREAM_FOCUS_STATE_RELEASED) {
		ret = PLAYER_ERROR_FOCUS_NOT_READY;
		meddbg("MediaPlayer pause failed. ret: %d, player: %x\n", ret, &mPlayer);
		return ret;
	}

	std::unique_lock<std::mutex> lock(mCmdMtx);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::pausePlayer, shared_from_this(), std::ref(ret), true);
	mSyncCv.wait(lock);
	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::pausePlayer(player_result_t &ret, bool notify)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState == PLAYER_STATE_PLAYING || mCurState == PLAYER_STATE_COMPLETING) {
		audio_manager_result_t result = pause_audio_stream_out(mStreamInfo->id);
		if (result != AUDIO_MANAGER_SUCCESS) {
			meddbg("pause_audio_stream_in failed ret : %d\n", result);
			ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
			if (notify) {
				notifySync();
			}
			return;
		}

		result = reset_audio_stream_out(mStreamInfo->id);
		if (result != AUDIO_MANAGER_SUCCESS) {
			meddbg("reset_audio_stream_out failed, stream_id: %lu, ret : %d\n", mStreamInfo->id, result);
			ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
			if (notify) {
				notifySync();
			}
			return;
		}

		FocusManager &fm = FocusManager::getFocusManager();
		fm.unregisterPlayerFocusLossListener(mStreamInfo->id);

		PlayerWorker &mpw = PlayerWorker::getWorker();
		mpw.removePlayer(shared_from_this());

		mCurState = PLAYER_STATE_PAUSED;
		if (notify) {
			notifySync();
		}
		return;
	}

	if (mCurState == PLAYER_STATE_PAUSED) {
		medvdbg("Currently in Paused State, Ignore Pause\n");
		return notifySync();
	}

	if (mCurState == PLAYER_STATE_COMPLETED) {
		medvdbg("Playback already Finished, Ignore Pause\n");
		return notifySync();
	}

	meddbg("%s Fail : invalid state mPlayer : %x\n", __func__, &mPlayer);
	LOG_STATE_DEBUG(mCurState);

	ret = PLAYER_ERROR_INVALID_STATE;
	return notifySync();
}

void MediaPlayerImpl::onFocusLossListener()
{
	if (mCurState == PLAYER_STATE_PLAYING || mCurState == PLAYER_STATE_COMPLETING) {
		player_result_t ret = PLAYER_OK;
		pausePlayer(ret, false);
		meddbg("Internal pause done. player: %x\n", &mPlayer);
	}
}

player_result_t MediaPlayerImpl::getVolume(uint8_t *vol)
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);

	if (vol == nullptr) {
		meddbg("The given argument is invalid.\n");
		return PLAYER_ERROR_INVALID_PARAMETER;
	}

	PlayerWorker &mpw = PlayerWorker::getWorker();

	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::getPlayerVolume, shared_from_this(), vol, std::ref(ret));
	meddbg("getPlayerVolume enqueued. player: %x\n", &mPlayer);
	mSyncCv.wait(lock);

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::getPlayerVolume(uint8_t *vol, player_result_t &ret)
{
	medvdbg("MediaPlayer Worker : getVolume\n");
	audio_manager_result_t res = get_output_stream_volume(vol, mStreamInfo->policy);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("get_output_stream_volume() is failed, res = %d\n", res);
		// ToDo: Lets think if some other error type is required or not.
		ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
	}

	notifySync();
}

player_result_t MediaPlayerImpl::getMaxVolume(uint8_t *vol)
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);

	if (vol == nullptr) {
		meddbg("The given argument is invalid.\n");
		return PLAYER_ERROR_INVALID_PARAMETER;
	}

	PlayerWorker &mpw = PlayerWorker::getWorker();

	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::getPlayerMaxVolume, shared_from_this(), vol, std::ref(ret));
	meddbg("getPlayerMaxVolume enqueued. player: %x\n", &mPlayer);
	mSyncCv.wait(lock);

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::getPlayerMaxVolume(uint8_t *vol, player_result_t &ret)
{
	medvdbg("MediaPlayer Worker : getMaxVolume\n");
	audio_manager_result_t res = get_max_audio_volume(vol);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("get_max_audio_volume() is failed, res = %d\n", res);
		ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
	}

	notifySync();
}

player_result_t MediaPlayerImpl::setVolume(uint8_t vol)
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;

	stream_focus_state_t streamState = getStreamFocusState();
	if (streamState == STREAM_FOCUS_STATE_RELEASED) {
		ret = PLAYER_ERROR_FOCUS_NOT_READY;
		meddbg("MediaPlayer setVolume failed. ret: %d, player: %x\n", ret, &mPlayer);
		return ret;
	}

	std::unique_lock<std::mutex> lock(mCmdMtx);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::setPlayerVolume, shared_from_this(), vol, std::ref(ret));
	meddbg("setPlayerVolume enqueued. player: %x\n", &mPlayer);
	mSyncCv.wait(lock);

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::setPlayerVolume(uint8_t vol, player_result_t &ret)
{
	medvdbg("MediaPlayer Worker : setVolume %d\n", vol);

	audio_manager_result_t result = set_output_audio_volume(vol, mStreamInfo->policy);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_output_audio_volume failed vol : %d ret : %d\n", vol, result);
		if (result == AUDIO_MANAGER_DEVICE_NOT_SUPPORT) {
			ret = PLAYER_ERROR_DEVICE_NOT_SUPPORTED;
		} else {
			ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
		}
		return notifySync();
	}

	medvdbg("MediaPlayer setVolume success\n");
	ret = PLAYER_OK;
	return notifySync();
}

player_result_t MediaPlayerImpl::setDataSource(std::unique_ptr<stream::InputDataSource> source)
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	std::shared_ptr<stream::InputDataSource> sharedDataSource = std::move(source);
	mpw.enQueue(&MediaPlayerImpl::setPlayerDataSource, shared_from_this(), sharedDataSource, std::ref(ret));
	meddbg("setPlayerDataSource enqueued. player: %x\n", &mPlayer);
	mSyncCv.wait(lock);

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::setPlayerDataSource(std::shared_ptr<stream::InputDataSource> source, player_result_t &ret)
{
	if (mCurState != PLAYER_STATE_IDLE && mCurState != PLAYER_STATE_CONFIGURED &&
		mCurState != PLAYER_STATE_COMPLETED) {
		meddbg("%s Fail : invalid state mPlayer : %x\n", __func__, &mPlayer);
		LOG_STATE_DEBUG(mCurState);
		ret = PLAYER_ERROR_INVALID_STATE;
		return notifySync();
	}

	if (mCurState == PLAYER_STATE_COMPLETED) {
		player_result_t errCode = unpreparePlayback();
		if (ret == PLAYER_OK && errCode != PLAYER_OK) {
			ret = errCode;
			return notifySync();
		}
	}

	if (!source) {
		meddbg("MediaPlayer setDataSource fail : invalid argument. DataSource should not be nullptr\n");
		ret = PLAYER_ERROR_INVALID_PARAMETER;
		return notifySync();
	}

	mInputHandler.setPlayer(shared_from_this());
	mInputHandler.setInputDataSource(source);
	mCurState = PLAYER_STATE_CONFIGURED;

	return notifySync();
}

player_result_t MediaPlayerImpl::setObserver(std::shared_ptr<MediaPlayerObserverInterface> observer)
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;
	std::unique_lock<std::mutex> lock(mCmdMtx);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::setPlayerObserver, shared_from_this(), observer, std::ref(ret));
	meddbg("setPlayerObserver enqueued. player: %x\n", &mPlayer);
	mSyncCv.wait(lock);

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::setPlayerObserver(std::shared_ptr<MediaPlayerObserverInterface> observer, player_result_t &ret)
{
	medvdbg("setPlayerObserver\n");

	if (mPlayerObserver) {
		meddbg("Observer already set. Invalid operation. player: %x\n", &mPlayer);
		ret = PLAYER_ERROR_INVALID_OPERATION;
	}

	mPlayerObserver = observer;
	notifySync();
}

player_result_t MediaPlayerImpl::setStreamInfo(std::shared_ptr<stream_info_t> stream_info)
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::setPlayerStreamInfo, shared_from_this(), stream_info, std::ref(ret));
	meddbg("setPlayerStreamInfo enqueued. player: %x\n", &mPlayer);
	mSyncCv.wait(lock);

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::setPlayerStreamInfo(std::shared_ptr<stream_info_t> stream_info, player_result_t &ret)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != PLAYER_STATE_IDLE && mCurState != PLAYER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state mPlayer : %x\n", __func__, &mPlayer);
		LOG_STATE_DEBUG(mCurState);
		ret = PLAYER_ERROR_INVALID_STATE;
		return notifySync();
	}
	if (stream_info->policy == STREAM_TYPE_VOICE_RECORD) {
		meddbg("MediaPlayer setStreamInfo fail : invalid argument. Stream Type should not be VOICE_RECORD\n");
		ret = PLAYER_ERROR_INVALID_PARAMETER;
		return notifySync();
	}
	mStreamInfo = stream_info;
	notifySync();
}

stream_focus_state_t MediaPlayerImpl::getStreamFocusState(void)
{
	FocusManager &fm = FocusManager::getFocusManager();
	return fm.getStreamFocusState(mStreamInfo->id);
}

bool MediaPlayerImpl::isPlaying()
{
	meddbg("%s player: %x\n", __func__, &mPlayer);
	bool ret = false;
	std::unique_lock<std::mutex> lock(mCmdMtx);
	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		return ret;
	}

	/* Wait for other commands to complete. */
	mpw.enQueue([&]() {
		if (getState() == PLAYER_STATE_PLAYING || getState() == PLAYER_STATE_COMPLETING) {
			ret = true;
		}
		notifySync();
	});
	meddbg("getState() enqueued. player: %x\n", &mPlayer);
	mSyncCv.wait(lock);

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

player_result_t MediaPlayerImpl::setLooping(bool loop)
{
	meddbg("%s loop: %d player: %x\n", __func__, loop, &mPlayer);
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);

	PlayerWorker &mpw = PlayerWorker::getWorker();

	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::setPlayerLooping, shared_from_this(), loop, std::ref(ret));
	meddbg("setPlayerLooping enqueued. player: %x\n", &mPlayer);
	mSyncCv.wait(lock);

	meddbg("%s returned. player: %x\n", __func__, &mPlayer);
	return ret;
}

void MediaPlayerImpl::setPlayerLooping(bool loop, player_result_t &ret)
{
	medvdbg("setPlayerLooping\n");
	if (mCurState != PLAYER_STATE_IDLE && mCurState != PLAYER_STATE_CONFIGURED) {
		meddbg("setLooping failed, Player not created!\n");
		LOG_STATE_DEBUG(mCurState);
		ret = PLAYER_ERROR_INVALID_STATE;
	}
	mInputHandler.setLoop(loop);
	notifySync();
}

player_state_t MediaPlayerImpl::getState()
{
	medvdbg("MediaPlayer getState\n");
	return mCurState;
}

void MediaPlayerImpl::notifySync()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	mSyncCv.notify_one();
}

void MediaPlayerImpl::notifyObserver(player_observer_command_t cmd, ...)
{
	va_list ap;
	va_start(ap, cmd);
	medvdbg("%s cmd : %d\n", __func__, cmd);

	if (mPlayerObserver != nullptr) {
		switch (cmd) {
		case PLAYER_OBSERVER_COMMAND_FINISHED:
			mObserverQueue.enQueue(&MediaPlayerObserverInterface::onPlaybackFinished, mPlayerObserver, std::ref(mPlayer));
			break;
		case PLAYER_OBSERVER_COMMAND_PLAYBACK_ERROR:
			mObserverQueue.enQueue(&MediaPlayerObserverInterface::onPlaybackError, mPlayerObserver, std::ref(mPlayer), (player_error_t)va_arg(ap, int));
			break;
		case PLAYER_OBSERVER_COMMAND_BUFFER_OVERRUN:
		case PLAYER_OBSERVER_COMMAND_BUFFER_UNDERRUN:
		case PLAYER_OBSERVER_COMMAND_BUFFER_UPDATED:
		case PLAYER_OBSERVER_COMMAND_BUFFER_STATECHANGED:
			/* TODO These need to be handled by Framework Level */
			break;
		case PLAYER_OBSERVER_COMMAND_BUFFER_DATAREACHED: {
			medvdbg("OBSERVER_COMMAND_BUFFER_DATAREACHED\n");
			unsigned char *data = va_arg(ap, unsigned char *);
			size_t size = va_arg(ap, size_t);
			// Call observer directly, not via PlayerWorker.
			// Because data buffer would be released after this function returned.
			mPlayerObserver->onPlaybackBufferDataReached(mPlayer, data, size);
		} break;
		case PLAYER_OBSERVER_COMMAND_ASYNC_PREPARED:
			player_error_t error = (player_error_t)va_arg(ap, int);
			if (error != PLAYER_ERROR_NONE) {
				mCurState = PLAYER_STATE_CONFIGURED;
			}
			mObserverQueue.enQueue(&MediaPlayerObserverInterface::onAsyncPrepared, mPlayerObserver, std::ref(mPlayer), error);
			break;
		}

		if (cmd == PLAYER_OBSERVER_COMMAND_FINISHED || cmd == PLAYER_OBSERVER_COMMAND_PLAYBACK_ERROR 
			|| cmd == PLAYER_OBSERVER_COMMAND_ASYNC_PREPARED) {
			PlayerObserverWorker &pow = PlayerObserverWorker::getWorker();
			pow.enQueue(&MediaPlayerImpl::dequeueAndRunObserverCallback, shared_from_this());
		}
	}

	va_end(ap);
}

void MediaPlayerImpl::notifyAsync(player_event_t event)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != PLAYER_STATE_PREPARING) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		return notifyObserver(PLAYER_OBSERVER_COMMAND_ASYNC_PREPARED, PLAYER_ERROR_INVALID_STATE);
	}

	switch (event) {
	case PLAYER_EVENT_SOURCE_PREPARED: {
		// Input handler has been opened successfully by InputHandler::doStandBy().
		// Now setup audio manager and notify player observer the result.
		auto source = mInputHandler.getDataSource();
		if (set_audio_stream_out(source->getChannels(), source->getSampleRate(),
								 source->getPcmFormat(), mStreamInfo->id) != AUDIO_MANAGER_SUCCESS) {
			meddbg("MediaPlayer prepare fail : set_audio_stream_out fail\n");
			return notifyObserver(PLAYER_OBSERVER_COMMAND_ASYNC_PREPARED, PLAYER_ERROR_INTERNAL_OPERATION_FAILED);
		}

		mBufSize = get_user_output_frames_to_byte(get_output_frame_count(mStreamInfo->id), mStreamInfo->id);
		if (mBufSize < 0) {
			meddbg("MediaPlayer prepare fail : get_user_output_frames_to_byte fail\n");
			return notifyObserver(PLAYER_OBSERVER_COMMAND_ASYNC_PREPARED, PLAYER_ERROR_INTERNAL_OPERATION_FAILED);
		}

		medvdbg("MediaPlayer mBuffer size : %d\n", mBufSize);

		mBuffer = new unsigned char[mBufSize];
		if (!mBuffer) {
			meddbg("MediaPlayer prepare fail : mBuffer allocation fail\n");
			if (get_errno() == ENOMEM) {
				return notifyObserver(PLAYER_OBSERVER_COMMAND_ASYNC_PREPARED, PLAYER_ERROR_OUT_OF_MEMORY);
			}
			return notifyObserver(PLAYER_OBSERVER_COMMAND_ASYNC_PREPARED, PLAYER_ERROR_INTERNAL_OPERATION_FAILED);
		}

		mCurState = PLAYER_STATE_READY;
		return notifyObserver(PLAYER_OBSERVER_COMMAND_ASYNC_PREPARED, PLAYER_ERROR_NONE);
	}

	case PLAYER_EVENT_SOURCE_OPEN_FAILED:
		return notifyObserver(PLAYER_OBSERVER_COMMAND_ASYNC_PREPARED, PLAYER_ERROR_FILE_OPEN_FAILED);
	}
}

void MediaPlayerImpl::playback(std::chrono::milliseconds timeout, uint8_t playback_idx)
{
	medvdbg("timeout: %lld, playback_idx: %d\n", timeout, playback_idx);
	float outputSampleRateRatio = get_output_sample_rate_ratio(mStreamInfo->id);
	outputSampleRateRatio = (outputSampleRateRatio >= 1.0f ? outputSampleRateRatio : 1);
	unsigned int framesToRead = get_card_output_bytes_to_frame(mBufSize) / outputSampleRateRatio;
	unsigned int bufferSize = get_user_output_frames_to_byte(framesToRead, mStreamInfo->id);

	// ToDo: Handle underrun properly in future when we support streaming player
	ssize_t num_read = mInputHandler.read(mBuffer, (int)bufferSize, timeout);
	medvdbg("num_read : %d player : %x\n", num_read, &mPlayer);
	if (num_read > 0) {
		int ret = start_audio_stream_out(mBuffer, get_user_output_bytes_to_frame((unsigned int)num_read, mStreamInfo->id), playback_idx, mStreamInfo->id);
		if (ret < 0) {
			PlayerWorker &mpw = PlayerWorker::getWorker();
			switch (ret) {
			case AUDIO_MANAGER_XRUN_STATE:
				meddbg("AUDIO_MANAGER_XRUN_STATE\n");
				mpw.enQueue(&MediaPlayerImpl::stopPlaybackInternal, shared_from_this(), false);
				break;
			default:
				meddbg("audio manager error : %d\n", ret);
				mpw.enQueue(&MediaPlayerImpl::stopPlaybackInternal, shared_from_this(), false);
				break;
			}
		}
	} else if (num_read == 0) {
		mCurState = PLAYER_STATE_COMPLETING;
	} else {
		/*@ToDo: It is not possible for num_read to be negative according to code in InputHandler read() API.*/
		meddbg("InputDatasource read error\n");
		PlayerWorker &mpw = PlayerWorker::getWorker();
		mpw.enQueue(&MediaPlayerImpl::stopPlaybackInternal, shared_from_this(), false);
	}
}

void MediaPlayerImpl::playbackFinished()
{
	meddbg("%s player: %x\n", __func__, &mPlayer);

	audio_manager_result_t result = stop_audio_stream_out(mStreamInfo->id, true);
	if (result == AUDIO_MANAGER_EAGAIN){
		return;
	}
	if (result == AUDIO_MANAGER_SUCCESS){
		mCurState = PLAYER_STATE_COMPLETED;
		notifyObserver(PLAYER_OBSERVER_COMMAND_FINISHED);
	} else {
		mCurState = PLAYER_STATE_READY;
		notifyObserver(PLAYER_OBSERVER_COMMAND_PLAYBACK_ERROR, PLAYER_ERROR_INTERNAL_OPERATION_FAILED);
	}

	PlayerWorker &mpw = PlayerWorker::getWorker();
	mpw.removePlayer(shared_from_this());

	FocusManager &fm = FocusManager::getFocusManager();
	fm.unregisterPlayerFocusLossListener(mStreamInfo->id);
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
