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
#include <sched.h>
#include <pthread.h>
#include "MediaWorker.h"

namespace media {

MediaWorker::MediaWorker() :
	mStacksize(PTHREAD_STACK_DEFAULT),
	mPriority(100),
	mThreadName("MediaWorker"),
	mIsRunning(false),
	mRefCnt(0),
	mWorkerThread(0),
	mInsideThreadFunc(false),
	mMAX_START_THREAD_WAIT_COUNT(10)
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
	++mRefCnt;
	meddbg("%s::startWorker() - increase RefCnt : %d\n", mThreadName, mRefCnt);
	if (mRefCnt == 1) {
		unsigned short attemptCount = 0;
		/* Thread is marked to exit but not exited yet, wait for thread to exit and then create again */
		while (attemptCount < mMAX_START_THREAD_WAIT_COUNT && mIsRunning == false && mInsideThreadFunc == true) {
			medwdbg("%s::startWorker() - wait for existing thread to finish\n", mThreadName);
			usleep(500000);
			attemptCount++;
		}
		if (attemptCount >= mMAX_START_THREAD_WAIT_COUNT && mIsRunning == false && mInsideThreadFunc == true) {
			meddbg("%s::startWorker() - existing thread is stuck. Abort creating new thread\n", mThreadName);
			return;
		}
		int ret;
		struct sched_param sparam;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, mStacksize);
		sparam.sched_priority = mPriority;
		pthread_attr_setschedparam(&attr, &sparam);
		attr.affinity = 1 << 0;
		mIsRunning = true;
		ret = pthread_create(&mWorkerThread, &attr, MediaWorker::mediaLooper, this);
		if (ret != OK) {
			meddbg("Fail to create worker thread, return value : %d\n", ret);
			--mRefCnt;
			mIsRunning = false;
			return;
		}
		pthread_setname_np(mWorkerThread, mThreadName);
	}
}

void MediaWorker::stopWorker()
{
	std::unique_lock<std::mutex> lock(mRefMtx);
	if (mRefCnt > 0) {
		--mRefCnt;
	}
	meddbg("%s::stopWorker() - decrease RefCnt : %d\n", mThreadName, mRefCnt);
	if (mRefCnt <= 0) {
		pthread_t currentThreadId = pthread_self();
		/* when current thread and join thread same, join API return immdiately */
		/* directly mIsRunning = false; is done for immediate exit without dequeue operation and then exit */
		if (mWorkerThread == currentThreadId) { 
			mIsRunning = false;
			meddbg("%s::stopWorker() - setting exit condition of mWorkerthread\n", mThreadName);
		} else {
			std::atomic<bool> &refBool = mIsRunning;
			mWorkerQueue.enQueue([&refBool]() {
				refBool = false;
			});
			pthread_join(mWorkerThread, NULL);
			meddbg("%s::stopWorker() - mWorkerthread exited\n", mThreadName);
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

void *MediaWorker::mediaLooper(void *arg)
{
	auto worker = static_cast<MediaWorker *>(arg);
	worker->mInsideThreadFunc = true;
	medvdbg("MediaWorker : mediaLooper\n");

	while (worker->mIsRunning) {
		while (worker->processLoop()) {
			if (!(worker->mWorkerQueue.isEmpty())) {
				break;
			}
			pthread_yield();
		}

		std::function<void()> run = worker->deQueue();
		medvdbg("MediaWorker : deQueue\n");
		if (run != nullptr) {
			run();
		}
	}
	worker->mInsideThreadFunc = false;
	return NULL;
}

bool MediaWorker::isAlive()
{
	std::unique_lock<std::mutex> lock(mRefMtx);
	return mRefCnt == 0 ? false : true;
}

bool MediaWorker::isSameThread()
{
	pthread_t currentThreadId = pthread_self();
	return mWorkerThread == currentThreadId;
}

void MediaWorker::clearQueue(void)
{
	mWorkerQueue.clearQueue();
}
} // namespace media
