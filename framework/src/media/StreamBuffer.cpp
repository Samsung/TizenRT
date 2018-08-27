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
#include <cstring>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <debug.h>

#include <media/BufferObserverInterface.h>

#include "StreamBuffer.h"
#include "StreamBufferReader.h"
#include "StreamBufferWriter.h"
#include "utils/rb.h"

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

StreamBuffer::StreamBuffer(size_t bufferSize, size_t threshold)
	: mObserver(nullptr), mRingBuf(nullptr), mEOS(false), mBufferSize(bufferSize), mThreshold(threshold)
{
}

StreamBuffer::~StreamBuffer()
{
	if (mRingBuf) {
		rb_free(mRingBuf);
		delete mRingBuf;
		mRingBuf = nullptr;
	}
}

bool StreamBuffer::init(size_t size)
{
	assert(!mRingBuf);
	mRingBuf = new rb_t;

	assert(mRingBuf);
	return rb_init(mRingBuf, size);
}

bool StreamBuffer::reset()
{
	bool ret;

	assert(mRingBuf);
	ret = rb_reset(mRingBuf);

	mEOS = false;
	return ret;
}

size_t StreamBuffer::read(unsigned char *buf, size_t size)
{
	assert(mRingBuf);
	return rb_read(mRingBuf, buf, size);
}

size_t StreamBuffer::write(unsigned char *buf, size_t size)
{
	assert(mRingBuf);
	return rb_write(mRingBuf, buf, size);
}

size_t StreamBuffer::sizeOfSpace()
{
	assert(mRingBuf);
	return rb_avail(mRingBuf);
}

size_t StreamBuffer::sizeOfData()
{
	assert(mRingBuf);
	return rb_used(mRingBuf);
}

void StreamBuffer::setEndOfStream()
{
	mEOS = true;
}

bool StreamBuffer::isEndOfStream()
{
	return mEOS;
}

void StreamBuffer::setObserver(BufferObserverInterface *observer)
{
	mObserver = observer;
}

void StreamBuffer::notifyObserver(State st, ...)
{
	if (mObserver) {
		switch (st) {
		case State::OVERRUN:
			mObserver->onBufferOverrun();
			break;
		case State::UNDERRUN:
			mObserver->onBufferUnderrun();
			break;
		case State::UPDATED: {
			va_list ap;
			va_start(ap, st);
			ssize_t change = va_arg(ap, ssize_t);
			va_end(ap);
			mObserver->onBufferUpdated(change, sizeOfData());
			break;
		}
		}
	}
}

StreamBuffer::Builder::Builder()
	: mBufferSize(CONFIG_STREAM_BUFFER_SIZE_DEFAULT), mThreshold(CONFIG_STREAM_BUFFER_THRESHOLD_DEFAULT)
{
}

StreamBuffer::Builder::~Builder()
{
}

StreamBuffer::Builder &StreamBuffer::Builder::setBufferSize(size_t bufferSize)
{
	if (bufferSize == 0) {
		mBufferSize = CONFIG_STREAM_BUFFER_SIZE_DEFAULT;
	} else {
		mBufferSize = bufferSize;
	}

	return *this;
}

StreamBuffer::Builder &StreamBuffer::Builder::setThreshold(size_t threshold)
{
	if (threshold == 0) {
		mThreshold = CONFIG_STREAM_BUFFER_THRESHOLD_DEFAULT;
	} else {
		mThreshold = threshold;
	}

	return *this;
}

std::shared_ptr<StreamBuffer> StreamBuffer::Builder::build()
{
	if (mThreshold > mBufferSize) {
		mThreshold = mBufferSize;
	}

	auto instance = std::make_shared<StreamBuffer>(mBufferSize, mThreshold);
	if (instance->init(mBufferSize)) {
		return instance;
	}

	meddbg("init failed! mBufferSize %u, mThreshold:%u\n", mBufferSize, mThreshold);
	return nullptr;
}

} // namespace stream
} // namespace media
