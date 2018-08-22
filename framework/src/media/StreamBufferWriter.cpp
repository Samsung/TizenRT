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
#include <stdio.h>
#include <assert.h>
#include <debug.h>

#include "StreamBuffer.h"
#include "StreamBufferWriter.h"

namespace media {
namespace stream {

StreamBufferWriter::StreamBufferWriter(std::shared_ptr<StreamBuffer> stream)
	: mStream(stream)
{
	assert(mStream);
}

size_t StreamBufferWriter::write(unsigned char *buf, size_t size, bool sync)
{
	medvdbg("size %lu sync %c\n", size, sync ? 'Y' : 'N');
	std::unique_lock<std::mutex> lock(mStream->getMutex());

	size_t wlen = 0;

	if (sync) {
		while (wlen < size) {
			// Streaming may be stopped (EOS was set)
			if (mStream->isEndOfStream()) {
				// Don't need to write anymore
				medvdbg("EOS break\n");
				break;
			}

			// Write data into stream as much as possible
			size_t temp = mStream->write(buf + wlen, size - wlen);
			mStream->notifyObserver(StreamBuffer::State::UPDATED, (ssize_t) temp);
			wlen += temp;
			if (wlen < size) {
				medvdbg("written %lu/%lu\n", wlen, size);
				// There's not enough space
				// Notify observer, shouldn't be blocked.
				mStream->notifyObserver(StreamBuffer::State::OVERRUN);
				// Reader may be waiting for more data, so it's necessary to notify after writing.
				mStream->getCondv().notify_one();
				// Then wait notification from reader.
				mStream->getCondv().wait(lock);
			}
		}
	} else {
		wlen = mStream->write(buf, size);
		mStream->notifyObserver(StreamBuffer::State::UPDATED, (ssize_t) wlen);
	}

	// Reader may be waiting for more data, so it's necessary to notify after writing.
	mStream->getCondv().notify_one();

	medvdbg("written %lu\n", wlen);
	return wlen;
}

size_t StreamBufferWriter::sizeOfSpace()
{
	std::lock_guard<std::mutex> lock(mStream->getMutex());
	return mStream->sizeOfSpace();
}

void StreamBufferWriter::setEndOfStream()
{
	std::lock_guard<std::mutex> lock(mStream->getMutex());

	// Set EOS flag in stream.
	mStream->setEndOfStream();

	// Reader may be waiting for more data, so it's necessary to notify.
	mStream->getCondv().notify_one();
}

} // namespace stream
} // namespace media
