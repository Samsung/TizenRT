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

#include <debug.h>
#include <media/MediaRecorder.h>
#include <media/MediaTypes.h>
#include "RecorderWorker.h"
#include "MediaRecorderImpl.h"
#include "audio/audio_manager.h"

namespace media {

#define LOG_STATE_INFO(state) medvdbg("state at %s[line : %d] : %s\n", __func__, __LINE__, recorder_state_names[(state)])
#define LOG_STATE_DEBUG(state) meddbg("state at %s[line : %d] : %s\n", __func__, __LINE__, recorder_state_names[(state)])

MediaRecorderImpl::MediaRecorderImpl(MediaRecorder &recorder) :
	mCurState(RECORDER_STATE_NONE),
	mRecorderObserver(nullptr),
	mRecorder(recorder),
	mBuffer(nullptr),
	mBuffSize(0),
	mDuration(0),
	mFileSize(0),
	mTotalFrames(0),
	mCapturedFrames(0),
	mStreamInfo(nullptr)
{
	medvdbg("MediaRecorderImpl::MediaRecorderImpl()\n");
}

recorder_result_t MediaRecorderImpl::create()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::create()\n");

	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.startWorker();

	recorder_result_t ret = RECORDER_OK;
	mrw.enQueue(&MediaRecorderImpl::createRecorder, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	if (ret != RECORDER_OK) {
		mrw.stopWorker();
	}

	return ret;
}

void MediaRecorderImpl::createRecorder(recorder_result_t& ret)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != RECORDER_STATE_NONE) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync();
	}

	mCurState = RECORDER_STATE_IDLE;
	notifySync();
}

recorder_result_t MediaRecorderImpl::destroy()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::destroy()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive\n");
		return RECORDER_ERROR_NOT_ALIVE;
	}

	recorder_result_t ret = RECORDER_OK;
	mrw.enQueue(&MediaRecorderImpl::destroyRecorder, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	if (ret == RECORDER_OK) {
		if (mRecorderObserver) {
			RecorderObserverWorker& row = RecorderObserverWorker::getWorker();
			row.stopWorker();
			mRecorderObserver = nullptr;
		}

		mrw.stopWorker();
	}

	return ret;
}

void MediaRecorderImpl::destroyRecorder(recorder_result_t& ret)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != RECORDER_STATE_IDLE && mCurState != RECORDER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync();
	}

	mCurState = RECORDER_STATE_NONE;
	notifySync();
}

recorder_result_t MediaRecorderImpl::prepare()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);

	medvdbg("MediaRecorderImpl::prepare()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive\n");
		return RECORDER_ERROR_NOT_ALIVE;
	}
	recorder_result_t ret = RECORDER_OK;
	mrw.enQueue(&MediaRecorderImpl::prepareRecorder, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::prepareRecorder(recorder_result_t& ret)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != RECORDER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync();
	}

	if (!mOutputHandler.open()) {
		meddbg("open() failed\n");
		ret = RECORDER_ERROR_FILE_OPEN_FAILED;
		return notifySync();
	}

	auto source = mOutputHandler.getDataSource();
	audio_manager_result_t result = set_audio_stream_in(source->getChannels(), source->getSampleRate(),
		(pcm_format)source->getPcmFormat());
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_audio_stream_in failed : result : %d channel %d sample rate : %d format : %d\n", result, \
			source->getChannels(), source->getSampleRate(), (pcm_format)source->getPcmFormat());
		mOutputHandler.close();
		ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	mBuffSize = get_user_input_frames_to_byte(get_input_frame_count());

	if (mBuffSize == 0) {
		meddbg("Buffer size is too small size : %d\n", mBuffSize);
		mOutputHandler.close();
		ret = RECORDER_ERROR_INVALID_PARAM;
		return notifySync();
	}
	medvdbg("mBuffer size : %d\n", mBuffSize);

	mBuffer = new unsigned char[mBuffSize];
	if (!mBuffer) {
		meddbg("mBuffer alloc failed\n");
		ret = RECORDER_ERROR_OUT_OF_MEMORY;
		mOutputHandler.close();
		return notifySync();
	}

	if (mDuration > 0) {
		mTotalFrames = mDuration * source->getSampleRate();
	} else if (mFileSize > 0) {
		mTotalFrames = get_user_input_bytes_to_frame(mFileSize);
	}

	mCurState = RECORDER_STATE_READY;
	notifySync();
}

recorder_result_t MediaRecorderImpl::unprepare()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR_NOT_ALIVE;
	}
	recorder_result_t ret = RECORDER_OK;
	mrw.enQueue(&MediaRecorderImpl::unprepareRecorder, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::unprepareRecorder(recorder_result_t& ret)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState == RECORDER_STATE_NONE || mCurState == RECORDER_STATE_IDLE || mCurState == RECORDER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync();
	}
	audio_manager_result_t result = reset_audio_stream_in();
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("reset_audio_stream_in failed ret : %d\n", result);
		ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	auto source = mOutputHandler.getDataSource();
	if (source->isPrepared()) {
		mOutputHandler.close();
	}

	if (mBuffer) {
		delete[] mBuffer;
		mBuffer = nullptr;
	}

	mBuffSize = 0;
	mDuration = 0;
	mFileSize = 0;
	mTotalFrames = 0;
	mCapturedFrames = 0;

	mCurState = RECORDER_STATE_IDLE;
	notifySync();
}

recorder_result_t MediaRecorderImpl::start()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::start()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR_NOT_ALIVE;
	}
	mrw.enQueue(&MediaRecorderImpl::startRecorder, shared_from_this());

	return RECORDER_OK;
}

void MediaRecorderImpl::startRecorder()
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != RECORDER_STATE_READY && mCurState != RECORDER_STATE_PAUSED) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		notifyObserver(RECORDER_OBSERVER_COMMAND_START_ERROR, RECORDER_ERROR_INVALID_STATE);
		return;
	}

	RecorderWorker& mrw = RecorderWorker::getWorker();
	auto prevRecorder = mrw.getCurrentRecorder();
	auto curRecorder = shared_from_this();

	if (prevRecorder != curRecorder) {
		if (prevRecorder) {
			/** TODO Should be considered Audiofocus later **/
			meddbg("stop prevRecorder\n");
			prevRecorder->pauseRecorder();
		}
		meddbg("set Current Recorder!!\n");
		mrw.setCurrentRecorder(curRecorder);
	}

	mCurState = RECORDER_STATE_RECORDING;
	notifyObserver(RECORDER_OBSERVER_COMMAND_STARTED);
}

recorder_result_t MediaRecorderImpl::stop()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::stop()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR_NOT_ALIVE;
	}

	mrw.enQueue(&MediaRecorderImpl::stopRecorder, shared_from_this(), RECORDER_OK);
	return RECORDER_OK;
}

void MediaRecorderImpl::stopRecorder(recorder_result_t ret)
{
	LOG_STATE_INFO(mCurState);

	if (ret == RECORDER_OK && mCurState != RECORDER_STATE_RECORDING && mCurState != RECORDER_STATE_PAUSED) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		notifyObserver(RECORDER_OBSERVER_COMMAND_STOP_ERROR, RECORDER_ERROR_INVALID_STATE);
		return;
	}

	mOutputHandler.flush();

	audio_manager_result_t result = stop_audio_stream_in();
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("stop_audio_stream_in failed ret : %d\n", result);
		if (ret == RECORDER_OK) {
			notifyObserver(RECORDER_OBSERVER_COMMAND_STOP_ERROR, RECORDER_ERROR_INTERNAL_OPERATION_FAILED);
		}
		return;
	}

	mCurState = RECORDER_STATE_READY;
	RecorderWorker &mrw = RecorderWorker::getWorker();
	mrw.setCurrentRecorder(nullptr);

	if (ret == RECORDER_OK) {
		notifyObserver(RECORDER_OBSERVER_COMMAND_FINISHIED);
	} else {
		notifyObserver(RECORDER_OBSERVER_COMMAND_STOP_ERROR, (recorder_error_t)ret);
	}
}

recorder_result_t MediaRecorderImpl::pause()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::pause()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR_NOT_ALIVE;
	}
	mrw.enQueue(&MediaRecorderImpl::pauseRecorder, shared_from_this());

	return RECORDER_OK;
}

void MediaRecorderImpl::pauseRecorder()
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != RECORDER_STATE_RECORDING) {
		meddbg("%s Fail : invalid state\n", __func__);
		LOG_STATE_DEBUG(mCurState);
		notifyObserver(RECORDER_OBSERVER_COMMAND_PAUSE_ERROR, RECORDER_ERROR_INVALID_STATE);
		return;
	}

	audio_manager_result_t result = pause_audio_stream_in();
	if (result != AUDIO_MANAGER_SUCCESS) {
		notifyObserver(RECORDER_OBSERVER_COMMAND_PAUSE_ERROR, RECORDER_ERROR_INTERNAL_OPERATION_FAILED);
		meddbg("pause_audio_stream_in failed ret : %d\n", result);
		return;
	}

	mCurState = RECORDER_STATE_PAUSED;
	notifyObserver(RECORDER_OBSERVER_COMMAND_PAUSED);
}

recorder_result_t MediaRecorderImpl::getVolume(uint8_t *vol)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::getVolume()\n");

	if (vol == nullptr) {
		meddbg("The given argument is invalid.\n");
		return RECORDER_ERROR_INVALID_PARAM;
	}

	recorder_result_t ret = RECORDER_OK;
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		ret = RECORDER_ERROR_NOT_ALIVE;
		return ret;
	}

	mrw.enQueue(&MediaRecorderImpl::getRecorderVolume, shared_from_this(), vol, std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::getRecorderVolume(uint8_t *vol, recorder_result_t &ret)
{
	medvdbg("getRecorderVolume\n");

	if (get_input_audio_gain(vol) != AUDIO_MANAGER_SUCCESS) {
		meddbg("get_output_audio_volume() is failed, ret = %d\n", ret);
		ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	notifySync();
}

recorder_result_t MediaRecorderImpl::getMaxVolume(uint8_t *vol)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::getMaxVolume()\n");

	if (vol == nullptr) {
		meddbg("The given argument is invalid.\n");
		return RECORDER_ERROR_INVALID_PARAM;
	}

	recorder_result_t ret = RECORDER_OK;
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		ret = RECORDER_ERROR_NOT_ALIVE;
		return ret;
	}

	mrw.enQueue(&MediaRecorderImpl::getRecorderMaxVolume, shared_from_this(), vol, std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::getRecorderMaxVolume(uint8_t *vol, recorder_result_t &ret)
{
	medvdbg("getRecorderMaxVolume\n");

	if (get_max_audio_volume(vol) != AUDIO_MANAGER_SUCCESS) {
		meddbg("get_max_audio_volume() is failed, ret = %d\n", ret);
		ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync();
	}

	notifySync();
}

recorder_result_t MediaRecorderImpl::setVolume(uint8_t vol)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("setVolume\n");

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR_NOT_ALIVE;
	}

	recorder_result_t ret = RECORDER_OK;
	mrw.enQueue(&MediaRecorderImpl::setRecorderVolume, shared_from_this(), vol, std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::setRecorderVolume(uint8_t vol, recorder_result_t& ret)
{
	medvdbg("setRecorderVolume\n");

	audio_manager_result_t result = set_input_audio_gain(vol);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_input_audio_volume failed vol : %d ret : %d\n", vol, result);
		if (result == AUDIO_MANAGER_DEVICE_NOT_SUPPORT) {
			ret = RECORDER_ERROR_DEVICE_NOT_SUPPORTED;
		} else {
			ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		}
		return notifySync();
	}

	medvdbg("setVolume is success vol : %d\n", vol);
	notifySync();
}

recorder_result_t MediaRecorderImpl::setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("setDataSource\n");

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive\n");
		return RECORDER_ERROR_NOT_ALIVE;
	}

	recorder_result_t ret = RECORDER_OK;
	std::shared_ptr<stream::OutputDataSource> sharedDataSource = std::move(dataSource);
	mrw.enQueue(&MediaRecorderImpl::setRecorderDataSource, shared_from_this(), sharedDataSource, std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::setRecorderDataSource(std::shared_ptr<stream::OutputDataSource> dataSource, recorder_result_t& ret)
{
	if (mCurState != RECORDER_STATE_IDLE) {
		meddbg("setDataSource failed mCurState : %d\n", (recorder_state_t)mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync();
	}

	if (!dataSource) {
		meddbg("DataSource is nullptr\n");
		ret = RECORDER_ERROR_INVALID_PARAM;
		return notifySync();
	}

	dataSource->setRecorder(shared_from_this());
	mOutputHandler.setRecorder(shared_from_this());
	mOutputHandler.setOutputDataSource(dataSource);
	mCurState = RECORDER_STATE_CONFIGURED;

	notifySync();
}

recorder_state_t MediaRecorderImpl::getState()
{
	medvdbg("getState() mCurState : %d\n", (recorder_state_t)mCurState);
	return mCurState;
}

recorder_result_t MediaRecorderImpl::setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("setObserver\n");

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive");
		return RECORDER_ERROR_NOT_ALIVE;
	}

	mrw.enQueue(&MediaRecorderImpl::setRecorderObserver, shared_from_this(), observer);
	mSyncCv.wait(lock);

	return RECORDER_OK;
}

void MediaRecorderImpl::setRecorderObserver(std::shared_ptr<MediaRecorderObserverInterface> observer)
{
	medvdbg("setRecorderObserver\n");

	RecorderObserverWorker& row = RecorderObserverWorker::getWorker();

	if (mRecorderObserver) {
		medvdbg("stopWorker\n");
		row.stopWorker();
	}

	if (observer) {
		medvdbg("startWorker\n");
		row.startWorker();
	}

	mRecorderObserver = observer;
	notifySync();
}

bool MediaRecorderImpl::isRecording()
{
	bool ret = false;
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorder isRecording\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return ret;
	}

	/* Wait for other commands to complete. */
	mrw.enQueue([&]() {
		if (getState() == RECORDER_STATE_RECORDING) {
			ret = true;
		}
		notifySync();
	});
	mSyncCv.wait(lock);

	return ret;
}

recorder_result_t MediaRecorderImpl::setDuration(int second)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);

	medvdbg("MediaRecorderImpl::setDuration()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive\n");
		return RECORDER_ERROR_NOT_ALIVE;
	}

	recorder_result_t ret = RECORDER_OK;
	mrw.enQueue(&MediaRecorderImpl::setRecorderDuration, shared_from_this(), second, std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::setRecorderDuration(int second, recorder_result_t& ret)
{
	medvdbg("setRecorderDuration mCurState : %d\n", (recorder_state_t)mCurState);

	if (mCurState != RECORDER_STATE_CONFIGURED) {
		meddbg("setRecorderDuration Failed mCurState: %d\n", (recorder_state_t)mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync();
	}

	if (second > 0) {
		medvdbg("second is greater than zero, set limit : %d seconds\n", second);
		mDuration = second;
	} else {
		medvdbg("second is smaller than or equal to zero, set unlimit\n");
		mDuration = 0;
	}
	mFileSize = 0;

	notifySync();
}

recorder_result_t MediaRecorderImpl::setFileSize(int byte)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::setFileSize()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive\n");
		return RECORDER_ERROR_NOT_ALIVE;
	}
	recorder_result_t ret = RECORDER_OK;
	mrw.enQueue(&MediaRecorderImpl::setRecorderFileSize, shared_from_this(), byte, std::ref(ret));
	mSyncCv.wait(lock);
	return ret;
}

void MediaRecorderImpl::setRecorderFileSize(int byte, recorder_result_t& ret)
{
	medvdbg("setRecorderFileSize mCurState : %d\n", (recorder_state_t)mCurState);
	if (mCurState != RECORDER_STATE_CONFIGURED) {
		meddbg("setRecorderFileSize Failed mCurState: %d\n", (recorder_state_t)mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync();
	}

	if (byte > 0) {
		medvdbg("byte is greater than zero, set limit : %d bytes\n", byte);
		mFileSize = byte;
	} else {
		medvdbg("byte is smaller than or equal to zero, set unlimit\n");
		mFileSize = 0;
	}
	mDuration = 0;

	notifySync();
}

void MediaRecorderImpl::capture()
{
	medvdbg("MediaRecorderImpl::capture()\n");
	unsigned int frameSize = get_input_frame_count();

	if (mTotalFrames > 0) {
		int64_t remainFrames = mTotalFrames - mCapturedFrames;
		/* almost reaches size limit, adjust frame size before request capture to audio_manager */
		if (remainFrames < frameSize) {
			frameSize = remainFrames;
		}
	}

	int frames = start_audio_stream_in(mBuffer, frameSize);
	if (frames > 0) {
		mCapturedFrames += frames;
		if (mCapturedFrames > INT_MAX) {
			mCapturedFrames = 0;
			meddbg("Too huge value : %d, set 0 to prevent overflow\n", mCapturedFrames);
		}

		int ret = 0;
		int size = get_user_input_frames_to_byte(frames);

		while (size > 0) {
			int written = mOutputHandler.write(mBuffer + ret, size);
			medvdbg("written : %d size : %d frames : %d\n", written, size, frames);
			medvdbg("mCapturedFrames : %ld totalduration : %d mTotalFrames : %ld\n", mCapturedFrames, mDuration, mTotalFrames);
			/* For Error case, we stop Capture */
			if (written == EOF) {
				meddbg("MediaRecorderImpl::capture() failed : errno : %d written : %d\n", errno, written);
				RecorderWorker& mrw = RecorderWorker::getWorker();
				mrw.enQueue(&MediaRecorderImpl::stopRecorder, shared_from_this(), RECORDER_ERROR_INTERNAL_OPERATION_FAILED);
				break;
			}

			/* It finished Successfully refer to file size or frame numbers*/
			if ((written == 0) || (mTotalFrames == mCapturedFrames)) {
				medvdbg("File write Ended\n");
				RecorderWorker& mrw = RecorderWorker::getWorker();
				mrw.enQueue(&MediaRecorderImpl::stopRecorder, shared_from_this(), RECORDER_OK);
				break;
			}
			size -= written;
			ret += written;
		}
	} else {
		std::lock_guard<std::mutex> lock(mCmdMtx);
		meddbg("Too small frames : %d\n", frames);
		RecorderWorker& mrw = RecorderWorker::getWorker();
		mrw.enQueue(&MediaRecorderImpl::stopRecorder, shared_from_this(), RECORDER_ERROR_INVALID_PARAM);
	}
}

void MediaRecorderImpl::notifySync()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	mSyncCv.notify_one();
}

void MediaRecorderImpl::notifyObserver(recorder_observer_command_t cmd, ...)
{
	medvdbg("notifyObserver cmd : %d\n", cmd);
	if (mRecorderObserver) {
		va_list ap;
		va_start(ap, cmd);

		RecorderObserverWorker& row = RecorderObserverWorker::getWorker();
		switch (cmd) {
		case RECORDER_OBSERVER_COMMAND_STARTED: {
			medvdbg("RECORDER_OBSERVER_COMMAND_STARTED\n");
			row.enQueue(&MediaRecorderObserverInterface::onRecordStarted, mRecorderObserver, mRecorder);
		} break;
		case RECORDER_OBSERVER_COMMAND_PAUSED: {
			medvdbg("RECORDER_OBSERVER_COMMAND_PAUSED\n");
			row.enQueue(&MediaRecorderObserverInterface::onRecordPaused, mRecorderObserver, mRecorder);
		} break;
		case RECORDER_OBSERVER_COMMAND_FINISHIED: {
			medvdbg("RECORDER_OBSERVER_COMMAND_FINISHIED\n");
			row.enQueue(&MediaRecorderObserverInterface::onRecordFinished, mRecorderObserver, mRecorder);
		} break;
		case RECORDER_OBSERVER_COMMAND_START_ERROR: {
			medvdbg("RECORDER_OBSERVER_COMMAND_START_ERROR\n");
			recorder_error_t errCode = (recorder_error_t)va_arg(ap, int);
			row.enQueue(&MediaRecorderObserverInterface::onRecordStartError, mRecorderObserver, mRecorder, errCode);
		} break;
		case RECORDER_OBSERVER_COMMAND_PAUSE_ERROR: {
			medvdbg("RECORDER_OBSERVER_COMMAND_PAUSE_ERROR\n");
			recorder_error_t errCode = (recorder_error_t)va_arg(ap, int);
			row.enQueue(&MediaRecorderObserverInterface::onRecordPauseError, mRecorderObserver, mRecorder, errCode);
		} break;
		case RECORDER_OBSERVER_COMMAND_STOP_ERROR: {
			medvdbg("RECORDER_OBSERVER_COMMAND_STOP_ERROR\n");
			recorder_error_t errCode = (recorder_error_t)va_arg(ap, int);
			row.enQueue(&MediaRecorderObserverInterface::onRecordStopError, mRecorderObserver, mRecorder, errCode);
		} break;
		case RECORDER_OBSERVER_COMMAND_BUFFER_OVERRUN: {
			medvdbg("RECORDER_OBSERVER_COMMAND_BUFFER_OVERRUN\n");
			row.enQueue(&MediaRecorderObserverInterface::onRecordBufferOverrun, mRecorderObserver, mRecorder);
		} break;
		case RECORDER_OBSERVER_COMMAND_BUFFER_UNDERRUN: {
			medvdbg("RECORDER_OBSERVER_COMMAND_BUFFER_UNDERRUN\n");
			row.enQueue(&MediaRecorderObserverInterface::onRecordBufferUnderrun, mRecorderObserver, mRecorder);
		} break;
		case RECORDER_OBSERVER_COMMAND_BUFFER_DATAREACHED: {
			medvdbg("RECORDER_OBSERVER_COMMAND_BUFFER_DATAREACHED\n");
			unsigned char *data = va_arg(ap, unsigned char *);
			size_t size = va_arg(ap, size_t);
			std::shared_ptr<unsigned char> autodata(data, [](unsigned char *p){ delete[] p; });
			row.enQueue(&MediaRecorderObserverInterface::onRecordBufferDataReached, mRecorderObserver, mRecorder, autodata, size);
		} break;
		}

		va_end(ap);
	}
}

MediaRecorderImpl::~MediaRecorderImpl()
{
	medvdbg("MediaRecorderImpl::~MediaRecorderImpl()\n");
	if (mCurState > RECORDER_STATE_IDLE) {
		if (unprepare() != RECORDER_OK) {
			meddbg("unprepare failed\n");
		}
	}

	if (mCurState == RECORDER_STATE_IDLE) {
		if (destroy() != RECORDER_OK) {
			meddbg("destroy failed\n");
		}
	}
}
} // namespace media
