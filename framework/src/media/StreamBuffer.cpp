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

namespace media {
namespace stream {

StreamBuffer::StreamBuffer()
	: mObserver(nullptr)
	, mRingBuf(nullptr)
	, mEOS(false)
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

} // namespace stream
} // namespace media

