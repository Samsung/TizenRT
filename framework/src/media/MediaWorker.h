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

#ifndef __MEDIA_MEIDAWORKER_H
#define __MEDIA_MEIDAWORKER_H

#include <thread>
#include <mutex>
#include <atomic>

#include "MediaQueue.h"

using namespace std;

namespace media {
class MediaWorker
{
public:
	MediaQueue& getQueue();

protected:
	MediaWorker();
	virtual ~MediaWorker();

	virtual int entry() = 0;
	void increaseRef();
	void decreaseRef();

protected:
	int mRefCnt;
	std::atomic<bool> mIsRunning;
	std::thread mWorkerThread;
	MediaQueue mWorkerQueue;   // worker queue
	std::mutex mRefMtx; // reference cnt mutex
};
} // namespace media

#endif
