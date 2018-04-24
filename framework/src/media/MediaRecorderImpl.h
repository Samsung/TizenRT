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
#include "MediaQueue.h"
#include "RecorderObserverWorker.h"

using namespace std;

namespace media {
typedef enum observer_command_e
{
	OBSERVER_COMMAND_STARTED,
	OBSERVER_COMMAND_FINISHIED,
	OBSERVER_COMMAND_ERROR
} observer_command_t;

class MediaRecorderImpl : public enable_shared_from_this<MediaRecorderImpl>
{
public:
	MediaRecorderImpl();
	~MediaRecorderImpl();

	recorder_result_t create();
	recorder_result_t destroy();

	recorder_result_t prepare();
	recorder_result_t unprepare();
	recorder_result_t start();
	recorder_result_t pause();
	recorder_result_t stop();

	recorder_result_t getVolume();
	void setVolume(int vol);

	void setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource);
	recorder_state_t getState();
	void setState(recorder_state_t state);
	void setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer);
	
	void notifySync();
	void notifyObserver(observer_command_t cmd);
	void capture();

private:
	recorder_state_t mCurState;
	std::unique_ptr<stream::OutputDataSource> mOutputDataSource;
	std::shared_ptr<MediaRecorderObserverInterface> mRecorderObserver;

	unsigned char* mBuffer;
	unsigned int mBuffSize;
	mutex mCmdMtx; // command mutex
	std::condition_variable mSyncCv;
	int mCurVolume;
	int mId;
};
} // namespace media

#endif
