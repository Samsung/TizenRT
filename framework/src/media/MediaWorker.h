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

#ifndef __MEDIA_MEDIAWORKER_HPP
#define __MEDIA_MEDIAWORKER_HPP

#include <sys/types.h>
#include <atomic>
#include <mutex>

#include "MediaQueue.h"

namespace media {
class MediaWorker
{
public:
	MediaWorker();
	virtual ~MediaWorker();

	void startWorker();
	void stopWorker();

	template <typename _Callable, typename... _Args>
	void enQueue(_Callable &&__f, _Args &&... __args) {
		mWorkerQueue.enQueue(__f, __args...);
	}
	std::function<void()> deQueue();
	bool isAlive();
	bool isSameThread();
	void clearQueue(void);

protected:
	long mStacksize;
	int mPriority;
	const char *mThreadName;
	virtual bool processLoop();

private:
	static void *mediaLooper(void *);

	MediaQueue mWorkerQueue;
	std::atomic<bool> mIsRunning;
	int mRefCnt;
	pthread_t mWorkerThread;
	std::mutex mRefMtx;
	std::atomic<bool> mInsideThreadFunc;
	unsigned short mMAX_START_THREAD_WAIT_COUNT;
};
} // namespace media
#endif
