/******************************************************************
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

#ifndef __MEDIA_STREAMOUTPUTDATASOURCE_H
#define __MEDIA_STREAMOUTPUTDATASOURCE_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include "StreamBuffer.h"

namespace media {
namespace stream {

class OutputStreamBuffer
	: public StreamBuffer
{
public:
	static std::shared_ptr<OutputStreamBuffer> create(size_t bufferSize = 0, size_t threshold = 0);

	OutputStreamBuffer() = delete;
	virtual ~OutputStreamBuffer();

	size_t getBufferSize() { return mBufferSize; }
	size_t getThreshold() { return mThreshold; }

private:
	OutputStreamBuffer(size_t bufferSize, size_t threshold);

private:
	size_t mBufferSize;
	size_t mThreshold;
};

} // namespace stream
} // namespace media

#endif
