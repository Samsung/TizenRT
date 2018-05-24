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
#include "MediaWorker.h"

namespace media {
	

MediaWorker::MediaWorker() : mIsRunning(false), mRefCnt(0)
{
	medvdbg("MediaWorker::MediaWorker()\n");
}
MediaWorker::~MediaWorker()
{
	medvdbg("MediaWorker::~MediaWorker()\n");
}

void MediaWorker::startWorker()
{
	std::unique_lock<std::mutex> lock(mRefMtx);
	increaseRef();
	medvdbg("MediaWorker::startWorker() - increase RefCnt : %d\n", mRefCnt);
	if (mRefCnt == 1) {
		mIsRunning = true;
		mWorkerThread = std::thread(std::bind(&MediaWorker::mediaLooper, this));
	}
}

void MediaWorker::stopWorker()
{
	std::unique_lock<std::mutex> lock(mRefMtx);
	decreaseRef();
	medvdbg("MediaWorker::stopWorker() - decrease RefCnt : %d\n", mRefCnt);
	if (mRefCnt <= 0) {
		if (mWorkerThread.joinable()) {
			std::atomic<bool> &refBool = mIsRunning;
			mWorkerQueue.enQueue([&refBool]() {
				refBool = false;
			});
			mWorkerThread.join();
			medvdbg("MediaWorker::stopWorker() - mWorkerthread exited\n");
		}
	}
}

std::function<void()> MediaWorker::deQueue()
{
	return mWorkerQueue.deQueue();
}

bool MediaWorker::processLoop()
{
	return false;
}

int MediaWorker::mediaLooper()
{
	medvdbg("MediaWorker : mediaLooper\n");

	while (mIsRunning) {
		while (processLoop() && mWorkerQueue.isEmpty());

		std::function<void()> run = deQueue();
		medvdbg("MediaWorker : deQueue\n");
		if (run != nullptr) {
			run();
		}
	}
	return 0;
}

void MediaWorker::increaseRef()
{
	mRefCnt++;
}

void MediaWorker::decreaseRef()
{
	if (mRefCnt > 0) {
		mRefCnt--;
	}
}

bool MediaWorker::isAlive()
{
	std::unique_lock<std::mutex> lock(mRefMtx);
	return mRefCnt == 0 ? false : true ;
}
} // namespace media
