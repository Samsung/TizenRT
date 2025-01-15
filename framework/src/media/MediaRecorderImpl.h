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

#ifndef __MEDIA_MEDIARECORDERIMPL_H
#define __MEDIA_MEDIARECORDERIMPL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <functional>
#include <iostream>
#include <fstream>

#include <tinyalsa/tinyalsa.h>
#include <media/OutputDataSource.h>
#include "OutputHandler.h"
#include "MediaQueue.h"
#include "RecorderObserverWorker.h"
#include "media/stream_info.h"

using namespace std;

namespace media {
/**
 * @brief current state of MediaRecorder
 * @details @b #include <media/MediaRecorder.h>
 * @since TizenRT v2.0
 */
typedef enum recorder_state_e {
	/** MediaRecorder object was created */
	RECORDER_STATE_NONE,
	/** MediaRecorder worker object was created */
	RECORDER_STATE_IDLE,
	/** MediaRecorder datasource configured */
	RECORDER_STATE_CONFIGURED,
	/** MediaRecorder ready to record */
	RECORDER_STATE_READY,
	/** MediaRecorder do recording */
	RECORDER_STATE_RECORDING,
	/** MediaRecorder pause to record */
	RECORDER_STATE_PAUSED
} recorder_state_t;

const char *const recorder_state_names[] = {
	"RECORDER_STATE_NONE",
	"RECORDER_STATE_IDLE",
	"RECORDER_STATE_CONFIGURED",
	"RECORDER_STATE_READY",
	"RECORDER_STATE_RECORDING",
	"RECORDER_STATE_PAUSED",
};

typedef enum recorder_observer_command_e {
	RECORDER_OBSERVER_COMMAND_STARTED,
	RECORDER_OBSERVER_COMMAND_PAUSED,
	RECORDER_OBSERVER_COMMAND_FINISHIED,
	RECORDER_OBSERVER_COMMAND_STOPPED,
	RECORDER_OBSERVER_COMMAND_START_ERROR,
	RECORDER_OBSERVER_COMMAND_PAUSE_ERROR,
	RECORDER_OBSERVER_COMMAND_STOP_ERROR,
	RECORDER_OBSERVER_COMMAND_BUFFER_OVERRUN,
	RECORDER_OBSERVER_COMMAND_BUFFER_UNDERRUN,
	RECORDER_OBSERVER_COMMAND_BUFFER_DATAREACHED,
} recorder_observer_command_t;

class MediaRecorderImpl : public enable_shared_from_this<MediaRecorderImpl>
{
public:
	MediaRecorderImpl(MediaRecorder& recorder);
	~MediaRecorderImpl();

	recorder_result_t create();
	recorder_result_t destroy();
	recorder_result_t prepare();
	recorder_result_t unprepare();
	recorder_result_t reset();

	recorder_result_t start();
	recorder_result_t pause();
	recorder_result_t stop();

	recorder_result_t getVolume(uint8_t *vol);
	recorder_result_t getMaxVolume(uint8_t *vol);
	recorder_result_t setVolume(uint8_t vol);
	recorder_result_t setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource);
	recorder_state_t getState();
	recorder_result_t setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer);
	bool isRecording();
	recorder_result_t setDuration(int second);
	recorder_result_t setFileSize(int byte);
	void notifySync();
	void notifyObserver(recorder_observer_command_t cmd, ...);
	void capture();

private:
	void createRecorder(recorder_result_t& ret);
	void destroyRecorder(recorder_result_t& ret);
	void prepareRecorder(recorder_result_t& ret);
	void unprepareRecorder(recorder_result_t& ret);
	void startRecorder();
	void pauseRecorder();
	void stopRecorder(recorder_result_t ret);
	void getRecorderVolume(uint8_t *vol, recorder_result_t& ret);
	void getRecorderMaxVolume(uint8_t *vol, recorder_result_t& ret);
	void setRecorderVolume(uint8_t vol, recorder_result_t& ret);
	void setRecorderObserver(std::shared_ptr<MediaRecorderObserverInterface> observer);
	void setRecorderDataSource(std::shared_ptr<stream::OutputDataSource> dataSource, recorder_result_t& ret);
	void setRecorderDuration(int second, recorder_result_t& ret);
	void setRecorderFileSize(int byte, recorder_result_t& ret);

private:
	std::atomic<recorder_state_t> mCurState;
	stream::OutputHandler mOutputHandler;
	std::shared_ptr<MediaRecorderObserverInterface> mRecorderObserver;

	MediaRecorder& mRecorder;
	unsigned char* mBuffer;
	int mBuffSize;
	mutex mCmdMtx; // command mutex
	std::condition_variable mSyncCv;
	int mDuration;
	int mFileSize;
	uint32_t mTotalFrames;
	uint32_t mCapturedFrames;
	std::shared_ptr<stream_info_t> mStreamInfo;
};
} // namespace media

#endif
