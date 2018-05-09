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

#include "RecorderWorker.h"

namespace media {

RecorderWorker::RecorderWorker() : mRefCnt(0), mIsRunning(false)
{
	medvdbg("RecorderWorker::RecorderWorker()\n");
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

int RecorderWorker::entry()
{
	medvdbg("RecorderWorker::entry()\n");

	while (mIsRunning) {
		if (mCurRecorder && (mCurRecorder->getState() == RECORDER_STATE_RECORDING)) {
			mCurRecorder->capture();

			if (mWorkerQueue.isEmpty()) {
				continue;
			}
		}

		std::function<void()> run = mWorkerQueue.deQueue();
		medvdbg("RecorderWorker::entry() - pop Queue\n");
		if (run != nullptr) {
			run();
		}
	}
	return 0;
}

recorder_result_t RecorderWorker::startWorker()
{
	std::unique_lock<std::mutex> lock(mRefMtx);
	increaseRef();
	medvdbg("RecorderWorker::startWorker() - increase RefCnt : %d\n", mRefCnt);
	if (mRefCnt == 1) {
		mIsRunning = true;
		mWorkerThread = std::thread(std::bind(&RecorderWorker::entry, this));
	}

	return RECORDER_OK;
}

void RecorderWorker::stopWorker()
{
	std::unique_lock<std::mutex> lock(mRefMtx);
	decreaseRef();
	medvdbg("RecorderWorker::stopWorker() - decrease RefCnt : %d\n", mRefCnt);
	if (mRefCnt <= 0) {
		if (mWorkerThread.joinable()) {
			std::atomic<bool> &refBool = mIsRunning;
			mWorkerQueue.enQueue([&refBool]() {
				refBool = false;
			});
			mWorkerThread.join();
			medvdbg("RecorderWorker::stopWorker() - mWorkerthread exited\n");
		}
	}
}

std::shared_ptr<MediaRecorderImpl> RecorderWorker::getCurrentRecorder()
{
	return mCurRecorder;
}

void RecorderWorker::setCurrentRecorder(std::shared_ptr<MediaRecorderImpl> mr)
{
	mCurRecorder = mr;
}

MediaQueue& RecorderWorker::getQueue()
{
	return mWorkerQueue;
}

void RecorderWorker::increaseRef()
{
	mRefCnt++;
}

void RecorderWorker::decreaseRef()
{
	if (mRefCnt > 0) {
		mRefCnt--;
	}
}

bool RecorderWorker::isAlive()
{
	std::unique_lock<std::mutex> lock(mRefMtx);
	return mRefCnt == 0 ? false : true ;
}
} // namespace media
