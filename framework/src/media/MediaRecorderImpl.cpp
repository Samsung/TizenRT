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
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	std::unique_lock<std::mutex> lock(mCmdMtx);
	std::condition_variable syncCv;

	RecorderWorker& mrw = RecorderWorker::getWorker();

	recorder_result_t ret = RECORDER_OK;
	mrw.enQueue(&MediaRecorderImpl::createRecorder, shared_from_this(), std::ref(ret), std::ref(syncCv));
	meddbg("createRecorder enqueued. recorder: %x\n", &mRecorder);
	syncCv.wait(lock);

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::createRecorder(recorder_result_t& ret, std::condition_variable &syncCv)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != RECORDER_STATE_NONE) {
		meddbg("%s Fail : invalid state. recorder: %x\n", __func__, &mRecorder);
		LOG_STATE_DEBUG(mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync(syncCv);
	}

	mCurState = RECORDER_STATE_IDLE;
	notifySync(syncCv);
}

recorder_result_t MediaRecorderImpl::destroy()
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	RecorderWorker& mrw = RecorderWorker::getWorker();
	recorder_result_t ret = RECORDER_OK;
	std::condition_variable syncCv;

	std::unique_lock<std::mutex> lock(mCmdMtx);
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive. recorder: %x\n", &mRecorder);
		return RECORDER_ERROR_NOT_ALIVE;
	}

	mrw.enQueue(&MediaRecorderImpl::destroyRecorder, shared_from_this(), std::ref(ret), std::ref(syncCv));
	meddbg("destroyRecorder enqueued. recorder: %x\n", &mRecorder);
	syncCv.wait(lock);

	if (ret == RECORDER_OK && mRecorderObserver) {
		RecorderObserverWorker& row = RecorderObserverWorker::getWorker();
		mObserverQueue.clearQueue();
		row.enQueue(&MediaRecorderImpl::dequeueAndRunObserverCallback, shared_from_this());
		if (row.isSameThread()) {
			mRecorderObserver = nullptr;
		} else {
			row.enQueue(&MediaRecorderImpl::unsetRecorderObserver, shared_from_this(), std::ref(syncCv));
			syncCv.wait(lock);
		}
	}

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::unsetRecorderObserver(std::condition_variable &syncCv)
{
	mRecorderObserver = nullptr;
	notifySync(syncCv);
}

void MediaRecorderImpl::destroyRecorder(recorder_result_t& ret, std::condition_variable &syncCv)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != RECORDER_STATE_IDLE && mCurState != RECORDER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state. recorder: %x\n", __func__, &mRecorder);
		LOG_STATE_DEBUG(mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync(syncCv);
	}

	mCurState = RECORDER_STATE_NONE;
	notifySync(syncCv);
}

void MediaRecorderImpl::dequeueAndRunObserverCallback()
{
	if (!mObserverQueue.isEmpty()) {
		std::function<void()> run = mObserverQueue.deQueue();
		if (run != nullptr) {
			run();
		}
	}
}

recorder_result_t MediaRecorderImpl::prepare()
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	recorder_result_t ret = RECORDER_OK;
	std::condition_variable syncCv;

	std::unique_lock<std::mutex> lock(mCmdMtx);

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive. recorder: %x\n", &mRecorder);
		return RECORDER_ERROR_NOT_ALIVE;
	}
	mrw.enQueue(&MediaRecorderImpl::prepareRecorder, shared_from_this(), std::ref(ret), std::ref(syncCv));
	meddbg("prepareRecorder enqueued. recorder: %x\n", &mRecorder);
	syncCv.wait(lock);

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::prepareRecorder(recorder_result_t& ret, std::condition_variable &syncCv)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != RECORDER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state. recorder: %x\n", __func__, &mRecorder);
		LOG_STATE_DEBUG(mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync(syncCv);
	}

	if (!mOutputHandler.open()) {
		meddbg("open() failed. recorder: %x\n", &mRecorder);
		ret = RECORDER_ERROR_FILE_OPEN_FAILED;
		return notifySync(syncCv);
	}

	audio_manager_result_t result = AUDIO_MANAGER_SUCCESS;

	auto source = mOutputHandler.getDataSource();
	result = set_audio_stream_in(source->getChannels(), source->getSampleRate(), (pcm_format)source->getPcmFormat());
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_audio_stream_in failed : result : %d channel %d sample rate : %d format : %d. recorder: %x\n", result, \
			source->getChannels(), source->getSampleRate(), (pcm_format)source->getPcmFormat(), &mRecorder);
		mOutputHandler.close();
		ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync(syncCv);
	}

	mBuffSize = get_user_input_frames_to_byte(get_input_frame_count());

	if (mBuffSize == 0) {
		meddbg("Buffer size is too small size : %d. recorder: %x\n", mBuffSize, &mRecorder);
		mOutputHandler.close();
		ret = RECORDER_ERROR_INVALID_PARAM;
		return notifySync(syncCv);
	}
	medvdbg("mBuffer size : %d\n", mBuffSize);

	mBuffer = new unsigned char[mBuffSize];
	if (!mBuffer) {
		meddbg("Buffer alloc failed. recorder: %x\n", &mRecorder);
		ret = RECORDER_ERROR_OUT_OF_MEMORY;
		mOutputHandler.close();
		return notifySync(syncCv);
	}

	if (mDuration > 0) {
		mTotalFrames = mDuration * source->getSampleRate();
	} else if (mFileSize > 0) {
		mTotalFrames = get_user_input_bytes_to_frame(mFileSize);
	}

	mCurState = RECORDER_STATE_READY;

	notifySync(syncCv);
}

recorder_result_t MediaRecorderImpl::unprepare()
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	recorder_result_t ret = RECORDER_OK;
	std::condition_variable syncCv;

	std::unique_lock<std::mutex> lock(mCmdMtx);

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR_NOT_ALIVE;
	}
	mrw.enQueue(&MediaRecorderImpl::unprepareRecorder, shared_from_this(), std::ref(ret), std::ref(syncCv));
	meddbg("unprepareRecorder enqueued. recorder: %x\n", &mRecorder);
	syncCv.wait(lock);

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::unprepareRecorder(recorder_result_t& ret, std::condition_variable &syncCv)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState == RECORDER_STATE_NONE || mCurState == RECORDER_STATE_IDLE || mCurState == RECORDER_STATE_CONFIGURED) {
		meddbg("%s Fail : invalid state. recorder: %x\n", __func__, &mRecorder);
		LOG_STATE_DEBUG(mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync(syncCv);
	}
	audio_manager_result_t result = reset_audio_stream_in();
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("reset_audio_stream_in failed ret : %d recorder: %x\n", result, &mRecorder);
		ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync(syncCv);
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
	notifySync(syncCv);
}

recorder_result_t MediaRecorderImpl::reset()
{
	LOG_STATE_INFO(mCurState);
	meddbg("%s recorder: %x\n", __func__, &mRecorder);

	if (mCurState == RECORDER_STATE_READY || mCurState == RECORDER_STATE_RECORDING || mCurState == RECORDER_STATE_PAUSED || mCurState == RECORDER_STATE_PAUSED_BY_MUTE) {
		audio_manager_result_t result = reset_audio_stream_in();
		if (result != AUDIO_MANAGER_SUCCESS) {
			meddbg("reset_audio_stream_in failed ret : %d recorder: %x\n", result, &mRecorder);
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
	}

	mCurState = RECORDER_STATE_IDLE;

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::start()
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	recorder_result_t ret = RECORDER_OK;
	std::condition_variable syncCv;

	std::unique_lock<std::mutex> lock(mCmdMtx);
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR_NOT_ALIVE;
	}
	mrw.enQueue(&MediaRecorderImpl::startRecorder, shared_from_this(), std::ref(ret), std::ref(syncCv));
	syncCv.wait(lock);
	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::startRecorder(recorder_result_t& ret, std::condition_variable &syncCv)
{
	LOG_STATE_INFO(mCurState);

	if (mCurState != RECORDER_STATE_READY && mCurState != RECORDER_STATE_PAUSED && mCurState != RECORDER_STATE_PAUSED_BY_MUTE) {
		meddbg("%s Fail : invalid state. recorder: %x\n", __func__, &mRecorder);
		LOG_STATE_DEBUG(mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync(syncCv);
	}

	bool mute = false;
	audio_manager_result_t result = get_audio_stream_mute_state(STREAM_TYPE_VOICE_RECORD, &mute);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Failed to get mute status. res: %d\n",result);
		ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync(syncCv);
	}

	if (mute) {
		ret = RECORDER_ERROR_DEVICE_SUSPENDED;
		return notifySync(syncCv);
	}

	RecorderWorker& mrw = RecorderWorker::getWorker();
	meddbg("set Current Recorder!!. recorder: %x\n", &mRecorder);
	mrw.setCurrentRecorder(shared_from_this());

	AudioEventListener muteListener = std::bind(&MediaRecorderImpl::onMuteListener, shared_from_this());
	registerRecorderMuteListener(muteListener);

	mCurState = RECORDER_STATE_RECORDING;
	return notifySync(syncCv);
}

recorder_result_t MediaRecorderImpl::stop()
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	recorder_result_t ret = RECORDER_OK;
	std::condition_variable syncCv;

	std::unique_lock<std::mutex> lock(mCmdMtx);
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR_NOT_ALIVE;
	}

	mrw.enQueue(&MediaRecorderImpl::stopRecorder, shared_from_this(), std::ref(ret), std::ref(syncCv));
	syncCv.wait(lock);
	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::stopRecorder(recorder_result_t& ret, std::condition_variable &syncCv)
{
	LOG_STATE_INFO(mCurState);

	if (ret == RECORDER_OK && mCurState != RECORDER_STATE_RECORDING && mCurState != RECORDER_STATE_PAUSED && mCurState != RECORDER_STATE_PAUSED_BY_MUTE) {
		meddbg("%s Fail : invalid state. recorder: %x\n", __func__, &mRecorder);
		LOG_STATE_DEBUG(mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync(syncCv);
	}

	mOutputHandler.flush();

	audio_manager_result_t result = stop_audio_stream_in();
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("stop_audio_stream_in failed ret : %d. recorder: %x\n", result, &mRecorder);
		ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync(syncCv);
	}
	meddbg("Total record size : %lu. recorder: %x\n", get_user_input_frames_to_byte(mCapturedFrames), &mRecorder);

	mCurState = RECORDER_STATE_READY;

	RecorderWorker &mrw = RecorderWorker::getWorker();
	mrw.setCurrentRecorder(nullptr);

	unregisterRecorderMuteListener();

	return notifySync(syncCv);
}

void MediaRecorderImpl::stopRecorderInternal(recorder_observer_command_e command, recorder_result_t ret)
{
	mOutputHandler.flush();

	mCurState = RECORDER_STATE_READY;

	RecorderWorker &mrw = RecorderWorker::getWorker();
	mrw.setCurrentRecorder(nullptr);

	audio_manager_result_t result = stop_audio_stream_in();
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("stop_audio_stream_in failed ret : %d. recorder: %x\n", result, &mRecorder);
	}
	meddbg("Total record size : %lu. recorder: %x\n", get_user_input_frames_to_byte(mCapturedFrames), &mRecorder);

 	unregisterRecorderMuteListener();

	notifyObserver(command, (recorder_error_t)ret);
}

recorder_result_t MediaRecorderImpl::pause()
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	recorder_result_t ret = RECORDER_OK;
	std::condition_variable syncCv;

	std::unique_lock<std::mutex> lock(mCmdMtx);
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR_NOT_ALIVE;
	}
	mrw.enQueue(&MediaRecorderImpl::pauseRecorder, shared_from_this(), std::ref(ret), true, std::ref(syncCv));
	syncCv.wait(lock);
	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::pauseRecorder(recorder_result_t& ret, bool notify, std::condition_variable &syncCv)
{
	LOG_STATE_INFO(mCurState);
	if (mCurState == RECORDER_STATE_PAUSED_BY_MUTE) {
		medvdbg("%s recorder muted and already paused. recorder: %x\n", __func__, &mRecorder);
		mCurState = RECORDER_STATE_PAUSED;
		if (notify) {
			notifySync(syncCv);
		}
		return;
	}

	if (mCurState != RECORDER_STATE_RECORDING) {
		meddbg("%s Fail : invalid state. recorder: %x\n", __func__, &mRecorder);
		LOG_STATE_DEBUG(mCurState);
		ret = RECORDER_ERROR_INVALID_STATE;
		if (notify) {
			notifySync(syncCv);
		}
		return;
	}

	audio_manager_result_t result = pause_audio_stream_in();
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("pause_audio_stream_in failed ret : %d. recorder: %x\n", result, &mRecorder);
		ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		if (notify) {
			notifySync(syncCv);
		}
		return;
	}

	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.setCurrentRecorder(nullptr);

	unregisterRecorderMuteListener();

	if (ret != RECORDER_ERROR_DEVICE_SUSPENDED) {
		mCurState = RECORDER_STATE_PAUSED;
	} else {
		mCurState = RECORDER_STATE_PAUSED_BY_MUTE;
	}

	if (notify) {
		notifySync(syncCv);
	}

	return;
}

void MediaRecorderImpl::onMuteListener()
{
	if (mCurState == RECORDER_STATE_RECORDING) {
		recorder_result_t ret = RECORDER_ERROR_DEVICE_SUSPENDED;
		std::condition_variable syncCv;
		pauseRecorder(ret, false, syncCv);
	}
}

recorder_result_t MediaRecorderImpl::getVolume(uint8_t *vol)
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	std::unique_lock<std::mutex> lock(mCmdMtx);
	std::condition_variable syncCv;

	if (vol == nullptr) {
		meddbg("The given argument is invalid. recorder: %x\n", &mRecorder);
		return RECORDER_ERROR_INVALID_PARAM;
	}

	recorder_result_t ret = RECORDER_OK;
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		ret = RECORDER_ERROR_NOT_ALIVE;
		return ret;
	}

	mrw.enQueue(&MediaRecorderImpl::getRecorderVolume, shared_from_this(), vol, std::ref(ret), std::ref(syncCv));
	meddbg("getRecorderVolume enqueued. recorder: %x\n", &mRecorder);
	syncCv.wait(lock);

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::getRecorderVolume(uint8_t *vol, recorder_result_t &ret, std::condition_variable &syncCv)
{
	medvdbg("getRecorderVolume\n");

	if (get_input_audio_gain(vol) != AUDIO_MANAGER_SUCCESS) {
		meddbg("get_output_audio_volume() is failed, ret = %d. recorder: %x\n", ret, &mRecorder);
		ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync(syncCv);
	}

	notifySync(syncCv);
}

recorder_result_t MediaRecorderImpl::getMaxVolume(uint8_t *vol)
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	std::unique_lock<std::mutex> lock(mCmdMtx);
	std::condition_variable syncCv;

	if (vol == nullptr) {
		meddbg("The given argument is invalid. recorder: %x\n", &mRecorder);
		return RECORDER_ERROR_INVALID_PARAM;
	}

	recorder_result_t ret = RECORDER_OK;
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		ret = RECORDER_ERROR_NOT_ALIVE;
		return ret;
	}

	mrw.enQueue(&MediaRecorderImpl::getRecorderMaxVolume, shared_from_this(), vol, std::ref(ret), std::ref(syncCv));
	meddbg("getRecorderMaxVolume enqueued. recorder: %x\n", &mRecorder);
	syncCv.wait(lock);

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::getRecorderMaxVolume(uint8_t *vol, recorder_result_t &ret, std::condition_variable &syncCv)
{
	medvdbg("getRecorderMaxVolume\n");

	if (get_max_audio_volume(vol) != AUDIO_MANAGER_SUCCESS) {
		meddbg("get_max_audio_volume() is failed, ret = %d. recorder: %x\n", ret, &mRecorder);
		ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		return notifySync(syncCv);
	}

	notifySync(syncCv);
}

recorder_result_t MediaRecorderImpl::setVolume(uint8_t vol)
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	recorder_result_t ret = RECORDER_OK;
	std::condition_variable syncCv;

	std::unique_lock<std::mutex> lock(mCmdMtx);

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR_NOT_ALIVE;
	}

	mrw.enQueue(&MediaRecorderImpl::setRecorderVolume, shared_from_this(), vol, std::ref(ret), std::ref(syncCv));
	meddbg("setRecorderVolume enqueued. recorder: %x\n", &mRecorder);
	syncCv.wait(lock);

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::setRecorderVolume(uint8_t vol, recorder_result_t& ret, std::condition_variable &syncCv)
{
	medvdbg("setRecorderVolume\n");

	audio_manager_result_t result = set_input_audio_gain(vol);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_input_audio_volume failed vol : %d ret : %d. recorder: %x\n", vol, result, &mRecorder);
		if (result == AUDIO_MANAGER_DEVICE_NOT_SUPPORT) {
			ret = RECORDER_ERROR_DEVICE_NOT_SUPPORTED;
		} else {
			ret = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
		}
		return notifySync(syncCv);
	}

	medvdbg("setVolume is success vol : %d\n", vol);
	notifySync(syncCv);
}

recorder_result_t MediaRecorderImpl::setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource)
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	std::unique_lock<std::mutex> lock(mCmdMtx);
	std::condition_variable syncCv;

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive. recorder: %x\n", &mRecorder);
		return RECORDER_ERROR_NOT_ALIVE;
	}

	recorder_result_t ret = RECORDER_OK;
	std::shared_ptr<stream::OutputDataSource> sharedDataSource = std::move(dataSource);
	mrw.enQueue(&MediaRecorderImpl::setRecorderDataSource, shared_from_this(), sharedDataSource, std::ref(ret), std::ref(syncCv));
	meddbg("setRecorderDataSource enqueued. recorder: %x\n", &mRecorder);
	syncCv.wait(lock);

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::setRecorderDataSource(std::shared_ptr<stream::OutputDataSource> dataSource, recorder_result_t& ret, std::condition_variable &syncCv)
{
	if (mCurState != RECORDER_STATE_IDLE && mCurState != RECORDER_STATE_CONFIGURED) {
		meddbg("setDataSource failed mCurState : %d. recorder: %x\n", (recorder_state_t)mCurState, &mRecorder);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync(syncCv);
	}

	if (!dataSource) {
		meddbg("DataSource is nullptr. recorder: %x\n", &mRecorder);
		ret = RECORDER_ERROR_INVALID_PARAM;
		return notifySync(syncCv);
	}

	dataSource->setRecorder(shared_from_this());
	mOutputHandler.setRecorder(shared_from_this());
	mOutputHandler.setOutputDataSource(dataSource);
	mCurState = RECORDER_STATE_CONFIGURED;

	notifySync(syncCv);
}

recorder_state_t MediaRecorderImpl::getState()
{
	medvdbg("getState() mCurState : %d\n", (recorder_state_t)mCurState);
	return mCurState;
}

recorder_result_t MediaRecorderImpl::setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer)
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	recorder_result_t ret = RECORDER_OK;
	std::unique_lock<std::mutex> lock(mCmdMtx);
	std::condition_variable syncCv;

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive. recorder: %x", &mRecorder);
		return RECORDER_ERROR_NOT_ALIVE;
	}

	mrw.enQueue(&MediaRecorderImpl::setRecorderObserver, shared_from_this(), observer, std::ref(ret), std::ref(syncCv));
	meddbg("setRecorderObserver enqueued. recorder: %x\n", &mRecorder);
	syncCv.wait(lock);

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::setRecorderObserver(std::shared_ptr<MediaRecorderObserverInterface> observer, recorder_result_t& ret, std::condition_variable &syncCv)
{
	medvdbg("setRecorderObserver\n");

	if (mRecorderObserver) {
		meddbg("Observer already set. Invalid operation. recorder: %x\n", &mRecorder);
		ret = RECORDER_ERROR_INVALID_OPERATION;
	}

	mRecorderObserver = observer;
	notifySync(syncCv);
}

bool MediaRecorderImpl::isRecording()
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	bool ret = false;
	std::unique_lock<std::mutex> lock(mCmdMtx);
	std::condition_variable syncCv;
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return ret;
	}

	/* Wait for other commands to complete. */
	mrw.enQueue([&]() {
		if (getState() == RECORDER_STATE_RECORDING) {
			ret = true;
		}
		notifySync(syncCv);
	});
	meddbg("getState() enqueued. recorder: %x\n", &mRecorder);
	syncCv.wait(lock);

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

recorder_result_t MediaRecorderImpl::setDuration(int second)
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	std::unique_lock<std::mutex> lock(mCmdMtx);
	std::condition_variable syncCv;

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive. recorder: %x\n", &mRecorder);
		return RECORDER_ERROR_NOT_ALIVE;
	}

	recorder_result_t ret = RECORDER_OK;
	mrw.enQueue(&MediaRecorderImpl::setRecorderDuration, shared_from_this(), second, std::ref(ret), std::ref(syncCv));
	meddbg("setRecorderDuration enqueued. recorder: %x\n", &mRecorder);
	syncCv.wait(lock);

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::setRecorderDuration(int second, recorder_result_t& ret, std::condition_variable &syncCv)
{
	medvdbg("setRecorderDuration mCurState : %d\n", (recorder_state_t)mCurState);

	if (mCurState != RECORDER_STATE_CONFIGURED) {
		meddbg("setRecorderDuration Failed mCurState: %d. recorder: %x\n", (recorder_state_t)mCurState, &mRecorder);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync(syncCv);
	}

	if (second > 0) {
		medvdbg("second is greater than zero, set limit : %d seconds\n", second);
		mDuration = second;
	} else {
		medvdbg("second is smaller than or equal to zero, set unlimit\n");
		mDuration = 0;
	}
	mFileSize = 0;

	notifySync(syncCv);
}

recorder_result_t MediaRecorderImpl::setFileSize(int byte)
{
	meddbg("%s recorder: %x\n", __func__, &mRecorder);
	std::unique_lock<std::mutex> lock(mCmdMtx);
	std::condition_variable syncCv;
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive. recorder: %x\n", &mRecorder);
		return RECORDER_ERROR_NOT_ALIVE;
	}
	recorder_result_t ret = RECORDER_OK;
	mrw.enQueue(&MediaRecorderImpl::setRecorderFileSize, shared_from_this(), byte, std::ref(ret), std::ref(syncCv));
	meddbg("setRecorderFileSize enqueued. recorder: %x\n", &mRecorder);
	syncCv.wait(lock);

	meddbg("%s returned. recorder: %x\n", __func__, &mRecorder);
	return ret;
}

void MediaRecorderImpl::setRecorderFileSize(int byte, recorder_result_t& ret, std::condition_variable &syncCv)
{
	medvdbg("setRecorderFileSize mCurState : %d\n", (recorder_state_t)mCurState);
	if (mCurState != RECORDER_STATE_CONFIGURED) {
		meddbg("setRecorderFileSize Failed mCurState: %d. recorder: %x\n", (recorder_state_t)mCurState, &mRecorder);
		ret = RECORDER_ERROR_INVALID_STATE;
		return notifySync(syncCv);
	}

	if (byte > 0) {
		medvdbg("byte is greater than zero, set limit : %d bytes\n", byte);
		mFileSize = byte;
	} else {
		medvdbg("byte is smaller than or equal to zero, set unlimit\n");
		mFileSize = 0;
	}
	mDuration = 0;

	notifySync(syncCv);
}

void MediaRecorderImpl::capture()
{
	medvdbg("MediaRecorderImpl::capture()\n");
	unsigned int frameSize = get_input_frame_count();
	recorder_result_t res = RECORDER_OK;

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
			meddbg("Too huge value : %d, set 0 to prevent overflow. recorder: %x\n", mCapturedFrames, &mRecorder);
		}

		int ret = 0;
		int size = get_user_input_frames_to_byte(frames);

		while (size > 0) {
			int written = mOutputHandler.write(mBuffer + ret, size);
			medvdbg("written : %d size : %d frames : %d\n", written, size, frames);
			medvdbg("mCapturedFrames : %ld totalduration : %d mTotalFrames : %ld\n", mCapturedFrames, mDuration, mTotalFrames);
			/* For Error case, we stop Capture */
			if (written == EOF) {
				std::lock_guard<std::mutex> lock(mCmdMtx);
				meddbg("MediaRecorderImpl::capture() failed : errno : %d written : %d. recorder: %x\n", errno, written, &mRecorder);
				RecorderWorker& mrw = RecorderWorker::getWorker();
				res = RECORDER_ERROR_INTERNAL_OPERATION_FAILED;
				mrw.enQueue(&MediaRecorderImpl::stopRecorderInternal, shared_from_this(), RECORDER_OBSERVER_COMMAND_STOPPED, res);
				break;
			}

			/* It finished Successfully refer to file size or frame numbers*/
			if ((written == 0) || (mTotalFrames == mCapturedFrames)) {
				std::lock_guard<std::mutex> lock(mCmdMtx);
				medvdbg("File write Ended\n");
				RecorderWorker& mrw = RecorderWorker::getWorker();
				mrw.enQueue(&MediaRecorderImpl::stopRecorderInternal, shared_from_this(), RECORDER_OBSERVER_COMMAND_FINISHIED, res);
				break;
			}
			size -= written;
			ret += written;
		}
	} else if (frames == AUDIO_MANAGER_DEVICE_SUSPENDED) {
		std::lock_guard<std::mutex> lock(mCmdMtx);
		meddbg("Too small frames : %d, audio device suspended. recorder: %x\n", frames, &mRecorder);
		RecorderWorker& mrw = RecorderWorker::getWorker();
		res = RECORDER_ERROR_DEVICE_SUSPENDED;
		mrw.enQueue(&MediaRecorderImpl::stopRecorderInternal, shared_from_this(), RECORDER_OBSERVER_COMMAND_STOPPED, res);
	} else if (frames == AUDIO_MANAGER_DEVICE_DEAD) {
		std::lock_guard<std::mutex> lock(mCmdMtx);
		meddbg("audio device dead. recorder: %x\n", &mRecorder);
		RecorderWorker& mrw = RecorderWorker::getWorker();
		res = RECORDER_ERROR_DEVICE_DEAD;
		mrw.enQueue(&MediaRecorderImpl::stopRecorderInternal, shared_from_this(), RECORDER_OBSERVER_COMMAND_STOPPED, res);
	} else {
		std::lock_guard<std::mutex> lock(mCmdMtx);
		meddbg("Too small frames : %d. recorder: %x\n", frames, &mRecorder);
		RecorderWorker& mrw = RecorderWorker::getWorker();
		res = RECORDER_ERROR_INVALID_PARAM;
		mrw.enQueue(&MediaRecorderImpl::stopRecorderInternal, shared_from_this(), RECORDER_OBSERVER_COMMAND_STOPPED, res);
	}
}

void MediaRecorderImpl::notifySync(std::condition_variable &syncCv)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	syncCv.notify_one();
}

void MediaRecorderImpl::notifyObserver(recorder_observer_command_t cmd, ...)
{
	medvdbg("notifyObserver cmd : %d\n", cmd);
	if (mRecorderObserver) {
		va_list ap;
		va_start(ap, cmd);

		switch (cmd) {
		case RECORDER_OBSERVER_COMMAND_FINISHIED: {
			medvdbg("RECORDER_OBSERVER_COMMAND_FINISHIED\n");
			mObserverQueue.enQueue(&MediaRecorderObserverInterface::onRecordFinished, mRecorderObserver, std::ref(mRecorder));
		} break;
		case RECORDER_OBSERVER_COMMAND_STOPPED: {
			medvdbg("RECORDER_OBSERVER_COMMAND_STOPPED\n");
			recorder_error_t errCode = (recorder_error_t)va_arg(ap, int);
			mObserverQueue.enQueue(&MediaRecorderObserverInterface::onRecordStopped, mRecorderObserver, std::ref(mRecorder), errCode);
		} break;
		case RECORDER_OBSERVER_COMMAND_BUFFER_OVERRUN: {
			medvdbg("RECORDER_OBSERVER_COMMAND_BUFFER_OVERRUN\n");
			mObserverQueue.enQueue(&MediaRecorderObserverInterface::onRecordBufferOverrun, mRecorderObserver, std::ref(mRecorder));
		} break;
		case RECORDER_OBSERVER_COMMAND_BUFFER_UNDERRUN: {
			medvdbg("RECORDER_OBSERVER_COMMAND_BUFFER_UNDERRUN\n");
			mObserverQueue.enQueue(&MediaRecorderObserverInterface::onRecordBufferUnderrun, mRecorderObserver, std::ref(mRecorder));
		} break;
		case RECORDER_OBSERVER_COMMAND_BUFFER_DATAREACHED: {
			medvdbg("RECORDER_OBSERVER_COMMAND_BUFFER_DATAREACHED\n");
			unsigned char *data = va_arg(ap, unsigned char *);
			size_t size = va_arg(ap, size_t);
			std::shared_ptr<unsigned char> autodata(data, [](unsigned char *p){ delete[] p; });
			mObserverQueue.enQueue(&MediaRecorderObserverInterface::onRecordBufferDataReached, mRecorderObserver, std::ref(mRecorder), autodata, size);
		} break;
		}

		RecorderObserverWorker& row = RecorderObserverWorker::getWorker();
		row.enQueue(&MediaRecorderImpl::dequeueAndRunObserverCallback, shared_from_this());

		va_end(ap);
	}
}

MediaRecorderImpl::~MediaRecorderImpl()
{
	medvdbg("MediaRecorderImpl::~MediaRecorderImpl()\n");
	if (mCurState > RECORDER_STATE_IDLE) {
		if (unprepare() != RECORDER_OK) {
			meddbg("unprepare failed. recorder: %x\n", &mRecorder);
		}
	}

	if (mCurState == RECORDER_STATE_IDLE) {
		if (destroy() != RECORDER_OK) {
			meddbg("destroy failed. recorder: %x\n", &mRecorder);
		}
	}
}
} // namespace media
