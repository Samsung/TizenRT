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

#include <tinyara/config.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <debug.h>

#include "OutputStreamBuffer.h"

#ifndef CONFIG_STREAM_BUFFER_SIZE_DEFAULT
#define CONFIG_STREAM_BUFFER_SIZE_DEFAULT 4096
#endif

#ifndef CONFIG_STREAM_BUFFER_THRESHOLD_DEFAULT
#define CONFIG_STREAM_BUFFER_THRESHOLD_DEFAULT CONFIG_STREAM_BUFFER_SIZE_DEFAULT
#endif

#if (CONFIG_STREAM_BUFFER_THRESHOLD_DEFAULT == 0)
#error "CONFIG_STREAM_BUFFER_THRESHOLD_DEFAULT should be nonzero!"
#endif

namespace media {
namespace stream {

std::shared_ptr<OutputStreamBuffer> OutputStreamBuffer::create(size_t bufferSize, size_t threshold)
{
	medvdbg("OutputStreamBuffer::create(%d,%d)\n", bufferSize, threshold);

	if (bufferSize == 0) {
		bufferSize = CONFIG_STREAM_BUFFER_SIZE_DEFAULT;
	}

	if (threshold == 0) {
		threshold = CONFIG_STREAM_BUFFER_THRESHOLD_DEFAULT;
	}

	if (threshold > bufferSize) {
		threshold = bufferSize;
	}

	std::shared_ptr<OutputStreamBuffer> instance(new OutputStreamBuffer(bufferSize, threshold));
	if (instance->init(bufferSize)) {
		return instance;
	}

	meddbg("OutputStreamBuffer::create(%d,%d), init failed!\n", bufferSize, threshold);
	return nullptr;
}

OutputStreamBuffer::OutputStreamBuffer(size_t bufferSize, size_t threshold)
	: mBufferSize(bufferSize)
	, mThreshold(threshold)
{
}

OutputStreamBuffer::~OutputStreamBuffer()
{
}

} // namespace stream
} // namespace media
