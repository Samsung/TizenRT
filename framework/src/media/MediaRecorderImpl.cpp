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
	medvdbg("MediaRecorderImpl::create()\n");

	RecorderWorker& mrw = RecorderWorker::getWorker();
	mrw.startWorker();

	recorder_result_t ret = RECORDER_ERROR;
	mrw.getQueue().enQueue(&MediaRecorderImpl::createRecorder, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	if (ret == RECORDER_ERROR) {
		mrw.stopWorker();
	}

	return ret;
}

void MediaRecorderImpl::createRecorder(recorder_result_t& ret)
{
	medvdbg("MediaRecorderImpl::createRecorder(recorder_result_t& ret");

	if (mCurState != RECORDER_STATE_NONE) {
		meddbg("MediaRecorderImpl::createRecorder(recorder_result_t& ret) - mCurState != RECORDER_STATE_NONE\n");
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
		meddbg("MediaRecorderImpl::destroy() - (mrw.isAlive() == false)");
		return RECORDER_ERROR;
	}

	recorder_result_t ret = RECORDER_ERROR;
	mrw.getQueue().enQueue(&MediaRecorderImpl::destroyRecorder, shared_from_this(), std::ref(ret));
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
	medvdbg("MediaRecorderImpl::destroyRecorder(recorder_result_t& ret)\n");

	if (mCurState != RECORDER_STATE_IDLE) {
		meddbg("MediaRecorderImpl::destroyRecorder(recorder_result_t& ret) - mCurState != RECORDER_STATE_IDLE\n");
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
		meddbg("MMediaRecorderImpl::prepare()- (mrw.isAlive() == false)\n");
		return RECORDER_ERROR;
	}
	recorder_result_t ret = RECORDER_ERROR;
	mrw.getQueue().enQueue(&MediaRecorderImpl::prepareRecorder, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::prepareRecorder(recorder_result_t& ret)
{
	medvdbg("MediaRecorderImpl::prepareRecorder(recorder_result_t& ret)\n");

	if (mCurState != RECORDER_STATE_IDLE || mOutputDataSource == nullptr) {
		meddbg("MediaRecorderImpl::prepareRecorder(recorder_result_t& ret) - " \
			"mCurState != RECORDER_STATE_IDLE || mOutputDataSource == nullptr\n");
		return notifySync();
	}

	if (set_audio_stream_in(mOutputDataSource->getChannels(), mOutputDataSource->getSampleRate(),
		(pcm_format)mOutputDataSource->getPcmFormat()) != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaRecorderImpl::prepareRecorder(recorder_result_t& ret) - " \
			"set_audio_stream_in(mOutputDataSource->getChannels(), mOutputDataSource->getSampleRate()," \
			"(pcm_format)mOutputDataSource->getPcmFormat()) != AUDIO_MANAGER_SUCCESS\n");
		return notifySync();
	}

	if (!mOutputDataSource->open()) {
		meddbg("MediaRecorderImpl::prepareRecorder(recorder_result_t& ret) - mOutputDataSource->open() failed\n");
		return notifySync();
	}

	mBuffSize = get_input_frames_byte_size(get_input_frame_count());

	if (mBuffSize <= 0) {
		meddbg("MediaRecorderImpl::prepareRecorder(recorder_result_t& ret) - (mBuffSize <= 0)\n");
		return notifySync();
	}

	medvdbg("MediaRecorderImpl::prepareRecorder(recorder_result_t& ret) - MediaRecorder mBuffer size : %d\n", mBuffSize);

	mBuffer = new unsigned char[mBuffSize];
	if (!mBuffer) {
		meddbg("MediaRecorderImpl::prepareRecorder(recorder_result_t& ret) - mBuffer alloc failed\n");
		return notifySync();
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
	mrw.getQueue().enQueue(&MediaRecorderImpl::unprepareRecorder, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::unprepareRecorder(recorder_result_t& ret)
{
	medvdbg("MediaRecorderImpl::unprepareRecorder(recorder_result_t& ret)\n");

	if (mCurState == RECORDER_STATE_NONE || mCurState == RECORDER_STATE_IDLE) {
		meddbg("MediaRecorderImpl::unprepareRecorder(recorder_result_t& ret) - " \
			"mCurState == RECORDER_STATE_NONE || mCurState == RECORDER_STATE_IDLE\n");
		return notifySync();
	}

	if (mBuffer) {
		delete[] mBuffer;
		mBuffer = nullptr;
	}

	mBuffSize = 0;

	if (mOutputDataSource->isPrepare()) {
		mOutputDataSource->close();
	}

	if (reset_audio_stream_in() != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaRecorderImpl::unprepareRecorder(recorder_result_t& ret) - " \
			"reset_audio_stream_in() != AUDIO_MANAGER_SUCCESS\n");
		return notifySync();
	}

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
	mrw.getQueue().enQueue(&MediaRecorderImpl::startRecorder, shared_from_this());

	return RECORDER_OK;
}

void MediaRecorderImpl::startRecorder()
{
	medvdbg("MediaRecorderImpl::startRecorder()\n");

	if (mCurState != RECORDER_STATE_READY && mCurState != RECORDER_STATE_PAUSED) {
		notifyObserver(OBSERVER_COMMAND_ERROR);
		meddbg("MediaRecorderImpl::startRecorder() - " \
			"mCurState != RECORDER_STATE_READY && mCurState != RECORDER_STATE_PAUSED\n");
		return;
	}

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (mrw.getCurrentRecorder() != nullptr) {
		mrw.getCurrentRecorder()->pauseRecorder();
	}

	mrw.setCurrentRecorder(shared_from_this());
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
	mrw.getQueue().enQueue(&MediaRecorderImpl::stopRecorder, shared_from_this(), true);

	return RECORDER_OK;
}

void MediaRecorderImpl::stopRecorder(bool completed)
{
	medvdbg("MediaRecorderImpl::stopRecorder(bool completed)\n");

	if ((mCurState != RECORDER_STATE_RECORDING && mCurState != RECORDER_STATE_PAUSED) || completed != true) {
		notifyObserver(OBSERVER_COMMAND_ERROR);
		meddbg("MediaRecorderImpl::stopRecorder(bool completed) - " \
			"((mCurState != RECORDER_STATE_RECORDING && mCurState != RECORDER_STATE_PAUSED) || completed != true)\n");
		return;
	}

	RecorderWorker& mrw = RecorderWorker::getWorker();
	mCurState = RECORDER_STATE_READY;
	notifyObserver(OBSERVER_COMMAND_FINISHIED);
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
	mrw.getQueue().enQueue(&MediaRecorderImpl::pauseRecorder, shared_from_this());

	return RECORDER_OK;
}

void MediaRecorderImpl::pauseRecorder()
{
	medvdbg("MediaRecorderImpl::pauseRecorder()\n");

	if (mCurState != RECORDER_STATE_RECORDING) {
		notifyObserver(OBSERVER_COMMAND_ERROR);
		meddbg("MediaRecorderImpl::pauseRecorder() - (mCurState != RECORDER_STATE_RECORDING)\n");
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

	mrw.getQueue().enQueue(&MediaRecorderImpl::getRecorderVolume, shared_from_this(), std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::getRecorderVolume(int& ret)
{
	medvdbg("MediaRecorderImpl::getVolumePlayer()\n");

	if (mCurState == RECORDER_STATE_IDLE || mCurState == RECORDER_STATE_NONE) {
		meddbg("MediaRecorderImpl::getVolume() - mCurState == RECORDER_STATE_IDLE || mCurState == RECORDER_STATE_NONE\n");
		return notifySync();
	}

	ret = get_input_audio_volume();
	notifySync();
}

recorder_result_t MediaRecorderImpl::setVolume(int vol)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::setVolume(int vol)\n");

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		return RECORDER_ERROR;
	}

	recorder_result_t ret = RECORDER_ERROR;
	mrw.getQueue().enQueue(&MediaRecorderImpl::setRecorderVolume, shared_from_this(), vol, std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::setRecorderVolume(int vol, recorder_result_t& ret)
{
	medvdbg("MediaRecorderImpl::setVolumeRecorder(int vol, recorder_result_t& ret)\n");

	if (mCurState == RECORDER_STATE_IDLE || mCurState == RECORDER_STATE_NONE) {
		meddbg("MediaRecorderImpl::setVolumeRecorder(int vol, recorder_result_t& ret) - " \
			"mCurState == RECORDER_STATE_IDLE || mCurState == RECORDER_STATE_NONE\n");
		return notifySync();
	}

	int maxVolume = get_max_audio_volume();
	if (vol < 0 || vol > maxVolume) {
		meddbg("MediaRecorderImpl::setVolumeRecorder(int vol, recorder_result_t& ret) - "\
			"incorrect vol range (recorder ranage 0 ~ %d)\n", maxVolume);
		return notifySync();
	}

	if (set_input_audio_volume(vol) != AUDIO_MANAGER_SUCCESS) {
		meddbg("MediaRecorderImpl::setVolumeRecorder(int vol, recorder_result_t& ret) : " \
			"set_input_audio_volume(vol) != AUDIO_MANAGER_SUCCESS\n");
		return notifySync();
	}

	medvdbg("MediaRecorderImpl::setVolume is success(int vol)\n");
	ret = RECORDER_OK;
	notifySync();
}

recorder_result_t MediaRecorderImpl::setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource)\n");

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("MediaRecorderImpl::setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource)" \
			" - (mrw.isAlive() == false)");
		return RECORDER_ERROR;
	}

	recorder_result_t ret = RECORDER_ERROR;
	std::shared_ptr<stream::OutputDataSource> sharedDataSource = std::move(dataSource);
	mrw.getQueue().enQueue(&MediaRecorderImpl::setRecorderDataSource, shared_from_this(), sharedDataSource, std::ref(ret));
	mSyncCv.wait(lock);

	return ret;
}

void MediaRecorderImpl::setRecorderDataSource(std::shared_ptr<stream::OutputDataSource> dataSource, recorder_result_t& ret)
{
	if (mCurState != RECORDER_STATE_IDLE) {
		meddbg("MediaRecorderImpl::setDataSource : mCurState != RECORDER_STATE_IDLE\n");
		return notifySync();
	}

	if (!dataSource) {
		meddbg("MediaRecorderImpl::setDataSource : datasource is nullptr\n");
		return notifySync();
	}

	mOutputDataSource = dataSource;
	ret = RECORDER_OK;
	notifySync();
}

recorder_state_t MediaRecorderImpl::getState()
{
	medvdbg("MediaRecorderImpl::getState()\n");
	return mCurState;
}

recorder_result_t MediaRecorderImpl::setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer)
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	medvdbg("MediaRecorderImpl::setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer)\n");

	RecorderWorker& mrw = RecorderWorker::getWorker();
	if (!mrw.isAlive()) {
		meddbg("MediaRecorderImpl::setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer)" \
			" - (mrw.isAlive() == false)");
		return RECORDER_ERROR;
	}

	mrw.getQueue().enQueue(&MediaRecorderImpl::setRecorderObserver, shared_from_this(), observer);
	mSyncCv.wait(lock);

	return RECORDER_OK;
}

void MediaRecorderImpl::setRecorderObserver(std::shared_ptr<MediaRecorderObserverInterface> observer)
{
	medvdbg("MediaRecorderImpl::notifySync()\n");

	RecorderObserverWorker& row = RecorderObserverWorker::getWorker();

	if (mRecorderObserver) {
		row.stopWorker();
	}

	if (observer) {
		row.startWorker();
	}

	mRecorderObserver = observer;
	notifySync();
}

void MediaRecorderImpl::capture()
{
	medvdbg("MediaRecorderImpl::capture()\n");
	int frames = start_audio_stream_in(mBuffer, get_input_frame_count());

	if (frames > 0) {
		int size = get_input_frames_byte_size(frames);

		int ret = 0;
		while (size > 0) {
			int written = mOutputDataSource->write(mBuffer + ret, size);
			printf("written = %d\n", written);
			if (written <= 0) {
				meddbg("MediaRecorderImpl::capture() - written <= 0\n");
				break;
			}
			size -= written;
			ret += written;
		}
	} else {
		std::lock_guard<std::mutex> lock(mCmdMtx);
		meddbg("MediaRecorderImpl::capture() -  (frames < 0)\n");
		RecorderWorker& mrw = RecorderWorker::getWorker();
		mrw.getQueue().enQueue(&MediaRecorderImpl::stopRecorder, shared_from_this(), false);
	}
}

void MediaRecorderImpl::notifySync()
{
	std::unique_lock<std::mutex> lock(mCmdMtx);
	mSyncCv.notify_one();
}

void MediaRecorderImpl::notifyObserver(observer_command_t cmd)
{
	medvdbg("MediaRecorderImpl::notifyObserver(observer_command_t cmd)\n");
	if (mRecorderObserver) {
		RecorderObserverWorker& row = RecorderObserverWorker::getWorker();
		switch (cmd) {
		case OBSERVER_COMMAND_STARTED: {
			medvdbg("MediaRecorderImpl::notifyObserver(observer_command_t cmd) - OBSERVER_COMMAND_STARTED\n");
			row.getQueue().enQueue(&MediaRecorderObserverInterface::onRecordStarted, mRecorderObserver, mId);
		} break;
		case OBSERVER_COMMAND_PAUSED: {
			medvdbg("MediaRecorderImpl::notifyObserver(observer_command_t cmd) - OBSERVER_COMMAND_PAUSED\n");
			row.getQueue().enQueue(&MediaRecorderObserverInterface::onRecordPaused, mRecorderObserver, mId);
		} break;
		case OBSERVER_COMMAND_FINISHIED: {
			medvdbg("MediaRecorderImpl::notifyObserver(observer_command_t cmd) - OBSERVER_COMMAND_FINISHIED\n");
			row.getQueue().enQueue(&MediaRecorderObserverInterface::onRecordFinished, mRecorderObserver, mId);
		} break;
		case OBSERVER_COMMAND_ERROR: {
			medvdbg("MediaRecorderImpl::notifyObserver(observer_command_t cmd) - OBSERVER_COMMAND_ERROR\n");
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
