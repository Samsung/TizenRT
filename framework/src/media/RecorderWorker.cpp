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

#include <tinyara/config.h>
#include <debug.h>
#include "RecorderWorker.h"

#ifndef CONFIG_MEDIA_RECORDER_STACKSIZE
#define CONFIG_MEDIA_RECORDER_STACKSIZE 4096
#endif
#ifndef CONFIG_MEDIA_RECORDER_THREAD_PRIORITY
#define CONFIG_MEDIA_RECORDER_THREAD_PRIORITY 100
#endif

namespace media {

RecorderWorker::RecorderWorker()
{
	medvdbg("RecorderWorker::RecorderWorker()\n");
	mThreadName = "RecorderWorker";
	mStacksize = CONFIG_MEDIA_RECORDER_STACKSIZE;
	mPriority = CONFIG_MEDIA_RECORDER_THREAD_PRIORITY;
}
RecorderWorker::~RecorderWorker()
{
	medvdbg("RecorderWorker::~RecorderWorker()\n");
}

RecorderWorker& RecorderWorker::getWorker()
{
	static RecorderWorker worker;
	return worker;
}

bool RecorderWorker::processLoop()
{
	if (mCurRecorder && (mCurRecorder->getState() == RECORDER_STATE_RECORDING)) {
		mCurRecorder->capture();
		return true;
	}

	return false;
}

std::shared_ptr<MediaRecorderImpl> RecorderWorker::getCurrentRecorder()
{
	return mCurRecorder;
}

void RecorderWorker::setCurrentRecorder(std::shared_ptr<MediaRecorderImpl> mr)
{
	mCurRecorder = mr;
}
} // namespace media
