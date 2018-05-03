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

#include "RecorderObserverWorker.h"

namespace media {
once_flag RecorderObserverWorker::mOnceFlag;

RecorderObserverWorker::RecorderObserverWorker() : mRefCnt(0), mIsRunning(false)
{
}
RecorderObserverWorker::~RecorderObserverWorker()
{
}

RecorderObserverWorker& RecorderObserverWorker::getWorker()
{
	static RecorderObserverWorker worker;
	return worker;
}

int RecorderObserverWorker::entry()
{
	medvdbg("RecorderObserverWorker::entry()\n");

	while (mIsRunning) {
		std::function<void()> run = mObserverQueue.deQueue();
		medvdbg("RecorderObserverWorker::entry() - pop Queue\n");
		if (run != nullptr) {
			run();
		}
	}
	return 0;
}

recorder_result_t RecorderObserverWorker::startWorker()
{
	std::unique_lock<std::mutex> lock(mRefMtx);
	increaseRef();

	medvdbg("RecorderObserverWorker::startWorker() - increase RefCnt : %d\n", mRefCnt);

	if (mRefCnt == 1) {
		mIsRunning = true;
		mWorkerThread = std::thread(std::bind(&RecorderObserverWorker::entry, this));
	}

	return RECORDER_OK;
}

void RecorderObserverWorker::stopWorker()
{
	std::unique_lock<std::mutex> lock(mRefMtx);
	decreaseRef();

	medvdbg("RecorderObserverWorker::stopWorker() - decrease RefCnt : %d\n", mRefCnt);

	if (mRefCnt <= 0) {

		if (mWorkerThread.joinable()) {
			std::atomic<bool> &refBool = mIsRunning;
			mObserverQueue.enQueue([&refBool]() {
				refBool = false;
			});
			mWorkerThread.join();
			medvdbg("RecorderObserverWorker::stopWorker() - mWorkerthread exited\n");
		}
	}
}

MediaQueue& RecorderObserverWorker::getQueue()
{
	return mObserverQueue;
}

void RecorderObserverWorker::increaseRef()
{
	mRefCnt++;
}

void RecorderObserverWorker::decreaseRef()
{
	if (mRefCnt > 0) {
		mRefCnt--;
	}
}
} // namespace media
