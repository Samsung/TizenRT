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
#include <errno.h>
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
}

player_result_t MediaPlayerImpl::create()
{
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer create\n");

	PlayerWorker &mpw = PlayerWorker::getWorker();
	mpw.startWorker();

	mpw.enQueue(&MediaPlayerImpl::createPlayer, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	if (ret != PLAYER_OK) {
		mpw.stopWorker();
	}

	return ret;
}

void MediaPlayerImpl::createPlayer(player_result_t &ret)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != PLAYER_STATE_NONE) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		ret = PLAYER_ERROR_INVALID_STATE;
		return notifySync();
	}

	audio_manager_result_t result = init_audio_stream_out();
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Fail to initialize output audio stream : %d\n", result);
		ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	mCurState = PLAYER_STATE_IDLE;
	notifySync();
}

player_result_t MediaPlayerImpl::destroy()
{
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer destroy\n");

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::destroyPlayer, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	if (ret == PLAYER_OK) {
		if (mPlayerObserver) {
			PlayerObserverWorker &pow = PlayerObserverWorker::getWorker();
			pow.stopWorker();
			mPlayerObserver = nullptr;
		}

		mpw.stopWorker();
	}

	return ret;
}

void MediaPlayerImpl::destroyPlayer(player_result_t &ret)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != PLAYER_STATE_IDLE && mCurState != PLAYER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		ret = PLAYER_ERROR_INVALID_STATE;
		return notifySync();
	}

	mCurState = PLAYER_STATE_NONE;
	notifySync();
}

player_result_t MediaPlayerImpl::prepare()
{
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer prepare\n");

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::preparePlayer, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaPlayerImpl::preparePlayer(player_result_t &ret)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != PLAYER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		ret = PLAYER_ERROR_INVALID_STATE;
		return notifySync();
	}

	if (!mInputHandler.open()) {
		meddbg("MediaPlayer prepare fail : open fail\n");
		ret = PLAYER_ERROR_FILE_OPEN_FAILED;
		return notifySync();
	}

	auto source = mInputHandler.getInputDataSource();
	if (set_audio_stream_out(source->getChannels(), source->getSampleRate(),
							 source->getPcmFormat()) != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaPlayer prepare fail : set_audio_stream_out fail\n");
		ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	mBufSize = get_user_output_frames_to_byte(get_output_frame_count());
	if (mBufSize < 0) {
		meddbg("MediaPlayer prepare fail : get_output_frames_byte_size fail\n");
		ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	medvdbg("MediaPlayer mBuffer size : %d\n", mBufSize);

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

player_result_t MediaPlayerImpl::unprepare()
{
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer unprepare\n");

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::unpreparePlayer, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaPlayerImpl::unpreparePlayer(player_result_t &ret)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState == PLAYER_STATE_NONE || mCurState == PLAYER_STATE_IDLE || mCurState == PLAYER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		ret = PLAYER_ERROR_INVALID_STATE;
		return notifySync();
	}

	if (mBuffer) {
		delete[] mBuffer;
		mBuffer = nullptr;
	}
	mBufSize = 0;

	if (reset_audio_stream_out() != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaPlayer unprepare fail : reset_audio_stream_out fail\n");
		ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	mInputHandler.close();

	mCurState = PLAYER_STATE_IDLE;
	return notifySync();
}

player_result_t MediaPlayerImpl::start()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer start\n");

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::startPlayer, shared_from_this());

	return PLAYER_OK;
}

void MediaPlayerImpl::startPlayer()
{
	LOG_STATE_INFO(mCurState);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (mCurState != PLAYER_STATE_READY && mCurState != PLAYER_STATE_PAUSED) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		notifyObserver(PLAYER_OBSERVER_COMMAND_START_ERROR, PLAYER_ERROR_INVALID_STATE);
		return;
	}

	auto prevPlayer = mpw.getPlayer();
	auto curPlayer = shared_from_this();
	if (prevPlayer != curPlayer) {
		if (prevPlayer) {
			/** TODO Should be considered Audiofocus later **/
			prevPlayer->pausePlayer();
		}
		mpw.setPlayer(curPlayer);
	}

	mCurState = PLAYER_STATE_PLAYING;
	notifyObserver(PLAYER_OBSERVER_COMMAND_STARTED);
}

player_result_t MediaPlayerImpl::stop()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer stop\n");

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::stopPlayer, shared_from_this(), PLAYER_OK);

	return PLAYER_OK;
}

void MediaPlayerImpl::stopPlayer(player_result_t ret)
{
	LOG_STATE_INFO(mCurState);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (ret == PLAYER_OK && mCurState != PLAYER_STATE_PLAYING && mCurState != PLAYER_STATE_PAUSED) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		notifyObserver(PLAYER_OBSERVER_COMMAND_STOP_ERROR, PLAYER_ERROR_INVALID_STATE);
		return;
	}

	audio_manager_result_t result = stop_audio_stream_out();
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("stop_audio_stream_out failed ret : %d\n", result);
		if (ret == PLAYER_OK) {
			/* TODO : inform device control fail */
			notifyObserver(PLAYER_OBSERVER_COMMAND_STOP_ERROR, PLAYER_ERROR_INTERNAL_OPERATION_FAILED);
			return;
		}
	}

	mpw.setPlayer(nullptr);
	mCurState = PLAYER_STATE_READY;
	notifyObserver(PLAYER_OBSERVER_COMMAND_STOPPED);
}

player_result_t MediaPlayerImpl::pause()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer pause\n");

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::pausePlayer, shared_from_this());

	return PLAYER_OK;
}

void MediaPlayerImpl::pausePlayer()
{
	LOG_STATE_INFO(mCurState);

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (mCurState != PLAYER_STATE_PLAYING) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		notifyObserver(PLAYER_OBSERVER_COMMAND_PAUSE_ERROR, PLAYER_ERROR_INVALID_STATE);
		return;
	}

	audio_manager_result_t result = pause_audio_stream_out();
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("pause_audio_stream_in failed ret : %d\n", result);
		notifyObserver(PLAYER_OBSERVER_COMMAND_PAUSE_ERROR, PLAYER_ERROR_INTERNAL_OPERATION_FAILED);
		return;
	}

	auto prevPlayer = mpw.getPlayer();
	auto curPlayer = shared_from_this();
	if (prevPlayer == curPlayer) {
		mpw.setPlayer(nullptr);
	}
	mCurState = PLAYER_STATE_PAUSED;
	notifyObserver(PLAYER_OBSERVER_COMMAND_PAUSED);
}

player_result_t MediaPlayerImpl::getVolume(uint8_t *vol)
{
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer getVolume\n");

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
	mSyncCv.wait(lock);

	return ret;
}

void MediaPlayerImpl::getPlayerVolume(uint8_t *vol, player_result_t &ret)
{
	medvdbg("MediaPlayer Worker : getVolume\n");
	if (get_output_audio_volume(vol) != AUDIO_MANAGER_SUCCESS) {
		meddbg("get_output_audio_volume() is failed, ret = %d\n", ret);
		ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
	}

	notifySync();
}

player_result_t MediaPlayerImpl::getMaxVolume(uint8_t *vol)
{
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer getMaxVolume\n");

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
	mSyncCv.wait(lock);

	return ret;
}

void MediaPlayerImpl::getPlayerMaxVolume(uint8_t *vol, player_result_t &ret)
{
	medvdbg("MediaPlayer Worker : getMaxVolume\n");
	if (get_max_audio_volume(vol) != AUDIO_MANAGER_SUCCESS) {
		meddbg("get_max_audio_volume() is failed, ret = %d\n", ret);
		ret = PLAYER_ERROR_INTERNAL_OPERATION_FAILED;
	}

	notifySync();
}

player_result_t MediaPlayerImpl::setVolume(uint8_t vol)
{
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer setVolume\n");

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::setPlayerVolume, shared_from_this(), vol, std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaPlayerImpl::setPlayerVolume(uint8_t vol, player_result_t &ret)
{
	medvdbg("MediaPlayer Worker : setVolume %d\n", vol);

	audio_manager_result_t result = set_output_audio_volume(vol);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_input_audio_volume failed vol : %d ret : %d\n", vol, result);
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
	player_result_t ret = PLAYER_OK;

	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer setDataSource\n");

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	std::shared_ptr<stream::InputDataSource> sharedDataSource = std::move(source);
	mpw.enQueue(&MediaPlayerImpl::setPlayerDataSource, shared_from_this(), sharedDataSource, std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaPlayerImpl::setPlayerDataSource(std::shared_ptr<stream::InputDataSource> source, player_result_t &ret)
{
	if (mCurState != PLAYER_STATE_IDLE) {
		meddbg("MediaPlayerImpl::setDataSource : mCurState != PLAYER_STATE_IDLE\n");
		ret = PLAYER_ERROR_INVALID_STATE;
		return notifySync();
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
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaPlayer setObserver\n");

	PlayerWorker &mpw = PlayerWorker::getWorker();
	if (!mpw.isAlive()) {
		meddbg("PlayerWorker is not alive\n");
		return PLAYER_ERROR_NOT_ALIVE;
	}

	mpw.enQueue(&MediaPlayerImpl::setPlayerObserver, shared_from_this(), observer);
	mSyncCv.wait(lock);

	return PLAYER_OK;
}

void MediaPlayerImpl::setPlayerObserver(std::shared_ptr<MediaPlayerObserverInterface> observer)
{
	PlayerObserverWorker &pow = PlayerObserverWorker::getWorker();

	if (mPlayerObserver) {
		pow.stopWorker();
	}

	if (observer) {
		pow.startWorker();
	}

	mPlayerObserver = observer;
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

	if (mPlayerObserver != nullptr) {
		PlayerObserverWorker &pow = PlayerObserverWorker::getWorker();
		switch (cmd) {
		case PLAYER_OBSERVER_COMMAND_STARTED:
			pow.enQueue(&MediaPlayerObserverInterface::onPlaybackStarted, mPlayerObserver, mPlayer);
			break;
		case PLAYER_OBSERVER_COMMAND_FINISHIED:
			pow.enQueue(&MediaPlayerObserverInterface::onPlaybackFinished, mPlayerObserver, mPlayer);
			break;
		case PLAYER_OBSERVER_COMMAND_PLAYBACK_ERROR:
			pow.enQueue(&MediaPlayerObserverInterface::onPlaybackError, mPlayerObserver, mPlayer, (player_error_t)va_arg(ap, int));
			break;
		case PLAYER_OBSERVER_COMMAND_START_ERROR:
			pow.enQueue(&MediaPlayerObserverInterface::onStartError, mPlayerObserver, mPlayer, (player_error_t)va_arg(ap, int));
			break;
		case PLAYER_OBSERVER_COMMAND_STOP_ERROR:
			pow.enQueue(&MediaPlayerObserverInterface::onStopError, mPlayerObserver, mPlayer, (player_error_t)va_arg(ap, int));
			break;
		case PLAYER_OBSERVER_COMMAND_PAUSE_ERROR:
			pow.enQueue(&MediaPlayerObserverInterface::onPauseError, mPlayerObserver, mPlayer, (player_error_t)va_arg(ap, int));
			break;
		case PLAYER_OBSERVER_COMMAND_PAUSED:
			pow.enQueue(&MediaPlayerObserverInterface::onPlaybackPaused, mPlayerObserver, mPlayer);
			break;
		case PLAYER_OBSERVER_COMMAND_STOPPED:
			pow.enQueue(&MediaPlayerObserverInterface::onPlaybackStopped, mPlayerObserver, mPlayer);
			break;
		case PLAYER_OBSERVER_COMMAND_BUFFER_OVERRUN:
			pow.enQueue(&MediaPlayerObserverInterface::onPlaybackBufferOverrun, mPlayerObserver, mPlayer);
			break;
		case PLAYER_OBSERVER_COMMAND_BUFFER_UNDERRUN:
			pow.enQueue(&MediaPlayerObserverInterface::onPlaybackBufferUnderrun, mPlayerObserver, mPlayer);
			break;
		case PLAYER_OBSERVER_COMMAND_BUFFER_UPDATED:
			pow.enQueue(&MediaPlayerObserverInterface::onPlaybackBufferUpdated, mPlayerObserver, mPlayer, (size_t)va_arg(ap, size_t));
			break;
		case PLAYER_OBSERVER_COMMAND_BUFFER_STATECHANGED:
			pow.enQueue(&MediaPlayerObserverInterface::onPlaybackBufferStateChanged, mPlayerObserver, mPlayer, (buffer_state_t)va_arg(ap, int));
			break;
		case PLAYER_OBSERVER_COMMAND_BUFFER_DATAREACHED: {
			medvdbg("OBSERVER_COMMAND_BUFFER_DATAREACHED\n");
			unsigned char *data = va_arg(ap, unsigned char *);
			size_t size = va_arg(ap, size_t);
			// Call observer directly, not via PlayerWorker.
			// Because data buffer would be released after this function returned.
			mPlayerObserver->onPlaybackBufferDataReached(mPlayer, data, size);
		} break;
		}
	}

	va_end(ap);
}

void MediaPlayerImpl::playback()
{
	ssize_t num_read = mInputHandler.read(mBuffer, (int)mBufSize);
	medvdbg("num_read : %d\n", num_read);
	if (num_read > 0) {
		int ret = start_audio_stream_out(mBuffer, get_user_output_bytes_to_frame((unsigned int)num_read));
		if (ret < 0) {
			notifyObserver(PLAYER_OBSERVER_COMMAND_PLAYBACK_ERROR, PLAYER_ERROR_INTERNAL_OPERATION_FAILED);
			PlayerWorker &mpw = PlayerWorker::getWorker();
			switch (ret) {
			case AUDIO_MANAGER_XRUN_STATE:
				meddbg("AUDIO_MANAGER_XRUN_STATE\n");
				mpw.enQueue(&MediaPlayerImpl::stopPlayer, shared_from_this(), PLAYER_ERROR_INTERNAL_OPERATION_FAILED);
				break;
			default:
				meddbg("audio manager error : %d\n", ret);
				mpw.enQueue(&MediaPlayerImpl::stopPlayer, shared_from_this(), PLAYER_ERROR_INTERNAL_OPERATION_FAILED);
				break;
			}
		}
	} else if (num_read == 0) {
		notifyObserver(PLAYER_OBSERVER_COMMAND_FINISHIED);
		stop();
	} else {
		meddbg("InputDatasource read error\n");
		notifyObserver(PLAYER_OBSERVER_COMMAND_PLAYBACK_ERROR, PLAYER_ERROR_INTERNAL_OPERATION_FAILED);
		PlayerWorker &mpw = PlayerWorker::getWorker();
		mpw.enQueue(&MediaPlayerImpl::stopPlayer, shared_from_this(), PLAYER_ERROR_INVALID_OPERATION);
	}
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
