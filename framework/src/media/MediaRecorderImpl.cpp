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

#include <media/MediaRecorder.h>
#include "RecorderWorker.h"
#include "MediaRecorderImpl.h"
#include "audio/audio_manager.h"

namespace media {
MediaRecorderImpl::MediaRecorderImpl()
	: mCurState(RECORDER_STATE_NONE), mOutputDataSource(nullptr), mRecorderObserver(nullptr), mBuffer(nullptr), mBuffSize(0)
{
	medvdbg("MediaRecorderImpl::MediaRecorderImpl()\n");
	static int recorderId = 1;
	mId = recorderId++;
}

recorder_result_t MediaRecorderImpl::create()
{
	unique_lock<mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::create()");

	if (mCurState != RECORDER_STATE_NONE) {
		meddbg("MediaRecorderImpl::create() - mCurState != RECORDER_STATE_NONE\n");
		return RECORDER_ERROR;
	}

	RecorderWorker& mrw = RecorderWorker::getWorker();

	mrw.startWorker();
	mrw.getQueue().enQueue(&MediaRecorderImpl::notifySync, shared_from_this());
	mSyncCv.wait(lock);

	mCurState = RECORDER_STATE_IDLE;
	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::destroy()
{
	unique_lock<mutex> lock(mCmdMtx);

	medvdbg("MediaRecorderImpl::destroy()");

	if (mCurState != RECORDER_STATE_IDLE) {
		meddbg("MediaRecorderImpl::destroy() - mCurState != RECORDER_STATE_IDLE\n");
		return RECORDER_ERROR;
	}

	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.getQueue().enQueue(&MediaRecorderImpl::notifySync, shared_from_this());
	mSyncCv.wait(lock);
	mrw.stopWorker();

	notifyObserver(OBSERVER_COMMAND_FINISHIED);
	mCurState = RECORDER_STATE_NONE;

	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::prepare()
{
	unique_lock<mutex> lock(mCmdMtx);

	medvdbg("MediaRecorderImpl::prepare()\n");
	if (mCurState != RECORDER_STATE_IDLE || mOutputDataSource == nullptr) {
		meddbg("MediaRecorderImpl::prepare() - mCurState != RECORDER_STATE_IDLE || mOutputDataSource == nullptr\n");
		return RECORDER_ERROR;
	}

	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.getQueue().enQueue(&MediaRecorderImpl::notifySync, shared_from_this());
	mSyncCv.wait(lock);

	if (set_audio_stream_in(mOutputDataSource->getChannels(), mOutputDataSource->getSampleRate(),
							(pcm_format)mOutputDataSource->getPcmFormat()) != AUDIO_MANAGER_SUCCESS) {

		meddbg("MediaRecorderImpl::prepare() - set_audio_stream_in() != AUDIO_MANAGER_SUCCESS\n");
		return RECORDER_ERROR;
	}

	if (!mOutputDataSource->open()) {
		meddbg("MediaRecorderImpl::prepare() - mOutputDataSource->open() failed\n");
		return RECORDER_ERROR;
	}

	mBuffSize = get_input_frames_byte_size(get_input_frame_count());	
	mBuffer = new unsigned char[mBuffSize];
	if (!mBuffer) {
		meddbg("MediaRecorderImpl::prepare() - mBuffer alloc failed\n");
		return RECORDER_ERROR;
	}

	mCurState = RECORDER_STATE_READY;
	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::unprepare()
{
	unique_lock<mutex> lock(mCmdMtx);

	medvdbg("MediaRecorderImpl::unprepare()\n");
	if (mCurState == RECORDER_STATE_NONE || mCurState == RECORDER_STATE_IDLE) {
		meddbg("MediaRecorderImpl::unprepare() - mCurState == RECORDER_STATE_NONE || mCurState == RECORDER_STATE_IDLE\n");
		return RECORDER_ERROR;
	}

	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.getQueue().enQueue(&MediaRecorderImpl::notifySync, shared_from_this());
	mSyncCv.wait(lock);

	if (mBuffer) {
		delete[] mBuffer;
		mBuffer = nullptr;
	}

	mBuffSize = 0;

	if (mOutputDataSource->isPrepare()) {
		mOutputDataSource->close();
	}

	reset_audio_stream_in();

	mCurState = RECORDER_STATE_IDLE;
	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::start()
{
	lock_guard<mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::start()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.getQueue().enQueue(&RecorderWorker::startRecorder, &mrw, shared_from_this());

	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::stop()
{
	lock_guard<mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::stop()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.getQueue().enQueue(&RecorderWorker::stopRecorder, &mrw, shared_from_this(), true);

	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::pause()
{
	lock_guard<mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::pause()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.getQueue().enQueue(&RecorderWorker::pauseRecorder, &mrw, shared_from_this());

	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::getVolume()
{
	lock_guard<mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::getVolume()\n");
	return RECORDER_OK;
}

void MediaRecorderImpl::setVolume(int vol)
{
	lock_guard<mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::setVolume(int vol)\n");
	mCurVolume = vol;
}

void MediaRecorderImpl::setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource)
{
	lock_guard<mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::setDataSource(OutputDataSource *_OutputDataSource)\n");
	mOutputDataSource = std::move(dataSource);
}

recorder_state_t MediaRecorderImpl::getState()
{
	medvdbg("MediaRecorderImpl::getState()\n");
	return mCurState;
}

void MediaRecorderImpl::setState(recorder_state_t state)
{
	medvdbg("MediaRecorderImpl::setState(recorder_state_t state)\n");
	mCurState = state;
}

void MediaRecorderImpl::setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer)
{
	unique_lock<mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer)\n");
	RecorderObserverWorker& row = RecorderObserverWorker::getWorker();
	row.startWorker();
	mRecorderObserver = observer;
}

void MediaRecorderImpl::capture()
{
	int frames = start_audio_stream_in(mBuffer, get_input_frame_count());

	if (frames > 0) {
		int size = get_input_frames_byte_size(frames);
		int ret = mOutputDataSource->write(mBuffer, size);

		if (ret < 0) {
			meddbg("MediaRecorderImpl::startRecording - ret < 0\n");
		}
	} else {
		lock_guard<mutex> lock(mCmdMtx);
		meddbg("MediaRecorderImpl::startRecording() - enqueue stopRecorder : errro code = %d\n", frames);
		RecorderWorker& mrw = RecorderWorker::getWorker();
		mrw.getQueue().enQueue(&RecorderWorker::stopRecorder, &mrw, shared_from_this(), false);
	}
}

void MediaRecorderImpl::notifySync()
{
	medvdbg("MediaRecorderImpl::notifySync()\n");
	mSyncCv.notify_one();
}

void MediaRecorderImpl::notifyObserver(observer_command_t cmd)
{
	medvdbg("MediaRecorderImpl::notifyObserver(observer_command_t cmd)\n");
	if (mRecorderObserver) {
		RecorderObserverWorker& row = RecorderObserverWorker::getWorker();
		switch (cmd) {
		case OBSERVER_COMMAND_STARTED: {
			row.getQueue().enQueue(&MediaRecorderObserverInterface::onRecordStarted, mRecorderObserver, mId);
		} break;
		case OBSERVER_COMMAND_FINISHIED: {
			row.getQueue().enQueue(&MediaRecorderObserverInterface::onRecordFinished, mRecorderObserver, mId);
		} break;
		case OBSERVER_COMMAND_ERROR: {
			row.getQueue().enQueue(&MediaRecorderObserverInterface::onRecordError, mRecorderObserver, mId);
		} break;
		}
	}
}

MediaRecorderImpl::~MediaRecorderImpl()
{
	medvdbg("MediaRecorderImpl::~MediaRecorderImpl()\n");
	if (mCurState > RECORDER_STATE_IDLE) {
		if (unprepare() != RECORDER_OK) {
			meddbg("MediaRecorderImpl::~MediaRecorderImpl() - unprepare() != RECORDER_OK\n");
		}
	}

	if (mCurState == RECORDER_STATE_IDLE) {
		if (destroy() != RECORDER_OK) {
			meddbg("MediaRecorderImpl::~MediaRecorderImpl() - destroy() != RECORDER_OK\n");
		}
	}

	RecorderObserverWorker& row = RecorderObserverWorker::getWorker();
	row.stopWorker();
}
} // namespace media
