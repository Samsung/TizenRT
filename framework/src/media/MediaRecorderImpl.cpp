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
	std::unique_lock<std::mutex> lock(mCmdMtx);
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
	std::unique_lock<std::mutex> lock(mCmdMtx);

	medvdbg("MediaRecorderImpl::destroy()");

	if (mCurState != RECORDER_STATE_IDLE) {
		meddbg("MediaRecorderImpl::destroy() - mCurState != RECORDER_STATE_IDLE\n");
		return RECORDER_ERROR;
	}

	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.getQueue().enQueue(&MediaRecorderImpl::notifySync, shared_from_this());
	mSyncCv.wait(lock);

	if (mRecorderObserver) {
		RecorderObserverWorker& row = RecorderObserverWorker::getWorker();
		row.stopWorker();
		mRecorderObserver = nullptr;
	}

	mrw.stopWorker();

	stream::OutputDataSource * raw = mOutputDataSource.release();
	delete raw;

	mCurState = RECORDER_STATE_NONE;
	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::prepare()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);

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

	if (mBuffSize < 0) {
		meddbg("MediaRecorderImpl::prepare() - get_input_frames_byte_size(get_input_frame_count()) failed\n");
		return RECORDER_ERROR;
	}

	medvdbg("MediaRecorder mBuffer size : %d\n", mBuffSize);

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
	std::unique_lock<std::mutex> lock(mCmdMtx);

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

	if (reset_audio_stream_in() != AUDIO_MANAGER_SUCCESS) {
		meddbg("reset_audio_stream_in() != AUDIO_MANAGER_SUCCESS\n");
		return RECORDER_ERROR;
	}

	mCurState = RECORDER_STATE_IDLE;
	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::start()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::start()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.getQueue().enQueue(&RecorderWorker::startRecorder, &mrw, shared_from_this());

	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::stop()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::stop()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.getQueue().enQueue(&RecorderWorker::stopRecorder, &mrw, shared_from_this(), true);

	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::pause()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::pause()\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.getQueue().enQueue(&RecorderWorker::pauseRecorder, &mrw, shared_from_this());

	return RECORDER_OK;
}

int MediaRecorderImpl::getVolume()
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::getVolume()\n");

	if (mCurState == RECORDER_STATE_IDLE || mCurState == RECORDER_STATE_NONE) {
		meddbg("MediaRecorderImpl::getVolume() - mCurState == RECORDER_STATE_IDLE || mCurState == RECORDER_STATE_NONE\n");
		return -1;
	}

	return get_input_audio_volume();
}

recorder_result_t MediaRecorderImpl::setVolume(int vol)
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::setVolume(int vol)\n");

	if (mCurState == RECORDER_STATE_IDLE || mCurState == RECORDER_STATE_NONE) {
		meddbg("MediaRecorderImpl::setVolume(int vol) - mCurState == RECORDER_STATE_IDLE || mCurState == RECORDER_STATE_NONE\n");
		return RECORDER_ERROR;
	}

	int maxVolume = get_max_audio_volume();
	if (vol < 0 || vol > maxVolume) {
		meddbg("MediaRecorderImpl::setVolume(int vol) : incorrect vol range (recorder ranage 0 ~ %d)\n", maxVolume);
		return RECORDER_ERROR;
	}

	if (set_input_audio_volume(vol) != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaRecorderImpl::setVolume(int vol) : set_input_audio_volume(vol) != AUDIO_MANAGER_SUCCESS\n", maxVolume);
		return RECORDER_ERROR;
	}

	medvdbg("MediaRecorderImpl::setVolume is success(int vol)\n");
	return RECORDER_OK;
}

recorder_result_t MediaRecorderImpl::setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource)
{
	std::lock_guard<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource)\n");
	
	if (mCurState != RECORDER_STATE_IDLE) {
		meddbg("MediaRecorderImpl::setDataSource : mCurState != RECORDER_STATE_IDLE\n");
		return RECORDER_ERROR;
	}

	if (!dataSource) {
		meddbg("MediaRecorderImpl::setDataSource : datasource is nullptr\n");
		return RECORDER_ERROR;
	}

	mOutputDataSource = std::move(dataSource);	
	return RECORDER_OK;
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
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer)\n");
	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.getQueue().enQueue(&MediaRecorderImpl::notifySync, shared_from_this());
	mSyncCv.wait(lock);

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
			meddbg("MediaRecorderImpl::capture() - ret < 0\n");
		}
	} else {
		std::lock_guard<std::mutex> lock(mCmdMtx);
		meddbg("MediaRecorderImpl::startRecording() - enqueue stopRecorder : errro code = %d\n", frames);
		RecorderWorker& mrw = RecorderWorker::getWorker();
		mrw.getQueue().enQueue(&RecorderWorker::stopRecorder, &mrw, shared_from_this(), false);
	}
}

void MediaRecorderImpl::notifySync()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
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
}
} // namespace media
