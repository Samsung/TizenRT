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

#include "MediaQueue.h"

namespace media {
MediaQueue::MediaQueue()
{
}
MediaQueue::~MediaQueue()
{
}

std::function<void()> MediaQueue::deQueue()
{
	std::unique_lock<std::mutex> lock(mQueueMtx);
	if (mQueueData.empty()) {
		mQueueCv.wait(lock);
	}

	auto data = std::move(mQueueData.front());
	mQueueData.pop();
	return data;
}

bool MediaQueue::isEmpty()
{
	std::unique_lock<std::mutex> lock(mQueueMtx);
	return mQueueData.empty();
}
} // namespace media
