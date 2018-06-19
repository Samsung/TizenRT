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
#include "RecorderWorker.h"
#include "MediaRecorderImpl.h"
#include "audio/audio_manager.h"

namespace media {
MediaRecorderImpl::MediaRecorderImpl()
	: mCurState(RECORDER_STATE_NONE), mOutputDataSource(nullptr), mRecorderObserver(nullptr), mBuffer(nullptr), mBuffSize(0),
	mDuration(0), mTotalFrames(0), mCapturedFrames(0)
{
	medvdbg("MediaRecorderImpl::MediaRecorderImpl()\n");
	static int recorderId = 1;
	mId = recorderId++;
}

recorder_result_t MediaRecorderImpl::create()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::create()\n");

	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.startWorker();

	recorder_result_t ret = RECORDER_ERROR;
	mrw.enQueue(&MediaRecorderImpl::createRecorder, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	if (ret == RECORDER_ERROR) {
		mrw.stopWorker();
	}

	return ret;
}

void MediaRecorderImpl::createRecorder(recorder_result_t& ret)
{
	medvdbg("createRecorder mCurState : %d\n", (recorder_state_t)mCurState);

	if (mCurState != RECORDER_STATE_NONE) {
		meddbg("mCurState != RECORDER_STATE_NONE mCurState : %d\n", (recorder_state_t)mCurState);
		return notifySync();
	}

	audio_manager_result_t result = init_audio_stream_in();
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Fail to initialize input audio stream : %d\n", result);
		return notifySync();
	}

	mCurState = RECORDER_STATE_IDLE;
	ret = RECORDER_OK;
	notifySync();
}

recorder_result_t MediaRecorderImpl::destroy()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::destroy()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive\n");
		return RECORDER_ERROR;
	}

	recorder_result_t ret = RECORDER_ERROR;
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
	medvdbg("destroyRecorder mCurState : %d\n", (recorder_state_t)mCurState);

	if (mCurState != RECORDER_STATE_IDLE) {
		meddbg("mCurState != RECORDER_STATE_IDLE mCurState : %d\n", (recorder_state_t)mCurState);
		return notifySync();
	}

	mCurState = RECORDER_STATE_NONE;
	ret = RECORDER_OK;
	notifySync();
}

recorder_result_t MediaRecorderImpl::prepare()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);

	medvdbg("MediaRecorderImpl::prepare()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive\n");
		return RECORDER_ERROR;
	}
	recorder_result_t ret = RECORDER_ERROR;
	mrw.enQueue(&MediaRecorderImpl::prepareRecorder, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::prepareRecorder(recorder_result_t& ret)
{
	medvdbg("prepareRecorder mCurState : %d\n", (recorder_state_t)mCurState);

	if (mCurState != RECORDER_STATE_IDLE || mOutputDataSource == nullptr) {
		meddbg("prepare Failed mCurState: %d mOutputDataSource : %s\n", (recorder_state_t)mCurState, \
			(mOutputDataSource == nullptr ? "nullptr" : "not null"));
		return notifySync();
	}

	if (!mOutputDataSource->open()) {
		meddbg("open() failed\n");
		return notifySync();
	}

	audio_manager_result_t result = set_audio_stream_in(mOutputDataSource->getChannels(), mOutputDataSource->getSampleRate(),
		(pcm_format)mOutputDataSource->getPcmFormat());
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_audio_stream_in failed : result : %d channel %d sample rate : %d format : %d\n", result, \
			mOutputDataSource->getChannels(), mOutputDataSource->getSampleRate(), (pcm_format)mOutputDataSource->getPcmFormat());
		mOutputDataSource->close();
		return notifySync();
	}

	mBuffSize = get_input_frames_to_byte(get_input_frame_count());

	if (mBuffSize <= 0) {
		meddbg("Buffer size is too small size : %d\n", mBuffSize);
		mOutputDataSource->close();
		return notifySync();
	}
	medvdbg("mBuffer size : %d\n", mBuffSize);

	mBuffer = new unsigned char[mBuffSize];
	if (!mBuffer) {
		meddbg("mBuffer alloc failed\n");
		mOutputDataSource->close();
		return notifySync();
	}

	if (mDuration > 0) {
		mTotalFrames = mDuration * mOutputDataSource->getSampleRate();
	}
	
	mCurState = RECORDER_STATE_READY;
	ret = RECORDER_OK;
	notifySync();
}

recorder_result_t MediaRecorderImpl::unprepare()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR;
	}
	recorder_result_t ret = RECORDER_ERROR;
	mrw.enQueue(&MediaRecorderImpl::unprepareRecorder, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::unprepareRecorder(recorder_result_t& ret)
{
	medvdbg("unprepareRecorder mCurState : %d\n", (recorder_state_t)mCurState);

	if (mCurState == RECORDER_STATE_NONE || mCurState == RECORDER_STATE_IDLE) {
		meddbg("unprepare Failed : %d\n", (recorder_state_t)mCurState);
		return notifySync();
	}
	audio_manager_result_t result = reset_audio_stream_in();
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("reset_audio_stream_in failed ret : %d\n", result);
		return notifySync();
	}

	if (mOutputDataSource->isPrepare()) {
		mOutputDataSource->close();
	}

	if (mBuffer) {
		delete[] mBuffer;
		mBuffer = nullptr;
	}

	mBuffSize = 0;
	mDuration = 0;
	mTotalFrames = 0;
	mCapturedFrames = 0;
	
	mCurState = RECORDER_STATE_IDLE;
	ret = RECORDER_OK;
	notifySync();
}

recorder_result_t MediaRecorderImpl::start()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::start()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR;
	}
	mrw.enQueue(&MediaRecorderImpl::startRecorder, shared_from_this());

	return RECORDER_OK;
}

void MediaRecorderImpl::startRecorder()
{
	medvdbg("startRecorder() mCurState : %d\n", (recorder_state_t)mCurState);

	if (mCurState != RECORDER_STATE_READY && mCurState != RECORDER_STATE_PAUSED) {
		notifyObserver(OBSERVER_COMMAND_ERROR);
		meddbg("startRecorder Failed mCurState : %d\n", (recorder_state_t)mCurState);
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
	notifyObserver(OBSERVER_COMMAND_STARTED);
}

recorder_result_t MediaRecorderImpl::stop()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::stop()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR;
	}
	mrw.enQueue(&MediaRecorderImpl::stopRecorder, shared_from_this(), true);

	return RECORDER_OK;
}

void MediaRecorderImpl::stopRecorder(bool completed)
{
	medvdbg("stopRecorder mCurState : %d\n", (recorder_state_t)mCurState);

	if ((mCurState != RECORDER_STATE_RECORDING && mCurState != RECORDER_STATE_PAUSED)) {
		notifyObserver(OBSERVER_COMMAND_ERROR);
		meddbg("stopRecorder Failed mCurState : %d\n", (recorder_state_t)mCurState);
		return;
	}
	
	audio_manager_result_t result = stop_audio_stream_in();
	if (result != AUDIO_MANAGER_SUCCESS) {
		notifyObserver(OBSERVER_COMMAND_ERROR);
		meddbg("stop_audio_stream_in failed ret : %d\n", result);
		return;
	}
	
	mCurState = RECORDER_STATE_READY;
	if (completed) {
		notifyObserver(OBSERVER_COMMAND_FINISHIED);
	} else {
		notifyObserver(OBSERVER_COMMAND_ERROR);
	}
	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.setCurrentRecorder(nullptr);
}

recorder_result_t MediaRecorderImpl::pause()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::pause()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR;
	}
	mrw.enQueue(&MediaRecorderImpl::pauseRecorder, shared_from_this());

	return RECORDER_OK;
}

void MediaRecorderImpl::pauseRecorder()
{
	medvdbg("pauseRecorder() mCurState : %d\n", (recorder_state_t)mCurState);

	if (mCurState != RECORDER_STATE_RECORDING) {
		notifyObserver(OBSERVER_COMMAND_ERROR);
		meddbg("pause Failed mCurState : %d\n", (recorder_state_t)mCurState);
		return;
	}
		
	audio_manager_result_t result = pause_audio_stream_in();
	if (result != AUDIO_MANAGER_SUCCESS) {
		notifyObserver(OBSERVER_COMMAND_ERROR);
		meddbg("pause_audio_stream_in failed ret : %d\n", result);
		return;
	}
	
	mCurState = RECORDER_STATE_PAUSED;
	notifyObserver(OBSERVER_COMMAND_PAUSED);
}

int MediaRecorderImpl::getVolume()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::getVolume()\n");

	int ret = -1;
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return ret;
	}

	mrw.enQueue(&MediaRecorderImpl::getRecorderVolume, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::getRecorderVolume(int& ret)
{
	medvdbg("getRecorderVolume\n");

	ret = get_input_audio_volume();
	notifySync();
}

recorder_result_t MediaRecorderImpl::setVolume(int vol)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("setVolume\n");

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR;
	}

	recorder_result_t ret = RECORDER_ERROR;
	mrw.enQueue(&MediaRecorderImpl::setRecorderVolume, shared_from_this(), vol, std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::setRecorderVolume(int vol, recorder_result_t& ret)
{
	medvdbg("setRecorderVolume\n");

	int maxVolume = get_max_audio_volume();
	if (vol < 0 || vol > maxVolume) {
		meddbg("Volume is out of range vol : %d (recorder ranage 0 ~ %d)\n", vol, maxVolume);
		return notifySync();
	}

	audio_manager_result_t result = set_input_audio_volume((uint8_t)vol);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_input_audio_volume failed vol : %d ret : %d\n", vol, result);
		return notifySync();
	}

	medvdbg("setVolume is success vol : %d\n", vol);
	ret = RECORDER_OK;
	notifySync();
}

recorder_result_t MediaRecorderImpl::setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("setDataSource\n");

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive\n");
		return RECORDER_ERROR;
	}

	recorder_result_t ret = RECORDER_ERROR;
	std::shared_ptr<stream::OutputDataSource> sharedDataSource = std::move(dataSource);
	mrw.enQueue(&MediaRecorderImpl::setRecorderDataSource, shared_from_this(), sharedDataSource, std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::setRecorderDataSource(std::shared_ptr<stream::OutputDataSource> dataSource, recorder_result_t& ret)
{
	if (mCurState != RECORDER_STATE_IDLE) {
		meddbg("setDataSource failed mCurState : %d\n", (recorder_state_t)mCurState);
		return notifySync();
	}

	if (!dataSource) {
		meddbg("DataSource is nullptr\n");
		return notifySync();
	}

	mOutputDataSource = dataSource;
	ret = RECORDER_OK;
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
		return RECORDER_ERROR;
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

recorder_result_t MediaRecorderImpl::setDuration(int second)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);

	medvdbg("MediaRecorderImpl::setDuration()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("Worker is not alive\n");
		return RECORDER_ERROR;
	}

	recorder_result_t ret = RECORDER_ERROR;
	mrw.enQueue(&MediaRecorderImpl::setRecorderDuration, shared_from_this(), second, std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::setRecorderDuration(int second, recorder_result_t& ret)
{
	medvdbg("setRecorderDuration mCurState : %d\n", (recorder_state_t)mCurState);

	if (mCurState != RECORDER_STATE_IDLE) {
		meddbg("setRecorderDuration Failed mCurState: %d\n", (recorder_state_t)mCurState);
		return notifySync();
	}
	if (second > 0) {
		medvdbg("second is greater than zero, set limit : %d\n", second);
		mDuration = second;
	}

	ret = RECORDER_OK;
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
		int size = get_input_frames_to_byte(frames);
		
		while (size > 0) {
			int written = mOutputDataSource->write(mBuffer + ret, size);
			medvdbg("written : %d size : %d frames : %d\n", written, size, frames);
			medvdbg("mCapturedFrames : %ld totalduration : %d mTotalFrames : %ld\n", mCapturedFrames, mDuration, mTotalFrames);
			/* For Error case, we stop Capture */
			if (written == EOF) {
				meddbg("MediaRecorderImpl::capture() failed : errno : %d written : %d\n", errno, written);
				RecorderWorker& mrw = RecorderWorker::getWorker();
				mrw.enQueue(&MediaRecorderImpl::stopRecorder, shared_from_this(), false);
				break;
			}

			/* It finished Successfully refer to file size or frame numbers*/
			if ((written == 0) || (mTotalFrames == mCapturedFrames)) {
				medvdbg("File write Ended\n");
				RecorderWorker& mrw = RecorderWorker::getWorker();
				mrw.enQueue(&MediaRecorderImpl::stopRecorder, shared_from_this(), true);
				break;
			}
			size -= written;
			ret += written;
		}
	} else {
		std::lock_guard<std::mutex> lock(mCmdMtx);
		meddbg("Too small frames : %d\n", frames);
		RecorderWorker& mrw = RecorderWorker::getWorker();
		mrw.enQueue(&MediaRecorderImpl::stopRecorder, shared_from_this(), false);
	}
}

void MediaRecorderImpl::notifySync()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	mSyncCv.notify_one();
}

void MediaRecorderImpl::notifyObserver(observer_command_t cmd)
{
	medvdbg("notifyObserver cmd : %d\n", cmd);
	if (mRecorderObserver) {
		RecorderObserverWorker& row = RecorderObserverWorker::getWorker();
		switch (cmd) {
		case OBSERVER_COMMAND_STARTED: {
			medvdbg("OBSERVER_COMMAND_STARTED\n");
			row.enQueue(&MediaRecorderObserverInterface::onRecordStarted, mRecorderObserver, mId);
		} break;
		case OBSERVER_COMMAND_PAUSED: {
			medvdbg("OBSERVER_COMMAND_PAUSED\n");
			row.enQueue(&MediaRecorderObserverInterface::onRecordPaused, mRecorderObserver, mId);
		} break;
		case OBSERVER_COMMAND_FINISHIED: {
			medvdbg("OBSERVER_COMMAND_FINISHIED\n");
			row.enQueue(&MediaRecorderObserverInterface::onRecordFinished, mRecorderObserver, mId);
		} break;
		case OBSERVER_COMMAND_ERROR: {
			medvdbg("OBSERVER_COMMAND_ERROR\n");
			row.enQueue(&MediaRecorderObserverInterface::onRecordError, mRecorderObserver, mId);
		} break;
		}
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
