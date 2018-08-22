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
#include "StreamBufferReader.h"

namespace media {
namespace stream {

StreamBufferReader::StreamBufferReader(std::shared_ptr<StreamBuffer> stream)
	: mStream(stream)
{
	assert(mStream);
}

size_t StreamBufferReader::read(unsigned char *buf, size_t size, bool sync)
{
	medvdbg("size %lu sync %c\n", size, sync ? 'Y' : 'N');
	std::unique_lock<std::mutex> lock(mStream->getMutex());

	size_t rlen = 0;

	if (sync) {
		while (rlen < size) {
			// Read data from stream as much as possible
			size_t temp = mStream->read(buf + rlen, size - rlen);
			mStream->notifyObserver(StreamBuffer::State::UPDATED, -((ssize_t) temp));
			rlen += temp;
			if (rlen < size) {
				// There's not enough data
				if (mStream->isEndOfStream()) {
					// End of stream, break reading
					medvdbg("EOS break\n");
					break;
				}

				medvdbg("read %lu/%lu\n", rlen, size);
				// Notify observer, shouldn't be blocked.
				mStream->notifyObserver(StreamBuffer::State::UNDERRUN);
				// Writer may be waiting for more spaces, so it's necessary to notify after reading.
				mStream->getCondv().notify_one();
				// Then wait notification from writer.
				mStream->getCondv().wait(lock);
			}
		}

		assert(rlen == size || mStream->isEndOfStream());
	} else {
		rlen = mStream->read(buf, size);
		mStream->notifyObserver(StreamBuffer::State::UPDATED, -((ssize_t) rlen));
	}

	// Writer may be waiting for more spaces, so it's necessary to notify after reading.
	mStream->getCondv().notify_one();

	medvdbg("read %lu\n", rlen);
	return rlen;
}

size_t StreamBufferReader::sizeOfData()
{
	std::lock_guard<std::mutex> lock(mStream->getMutex());
	return mStream->sizeOfData();
}

bool StreamBufferReader::isEndOfStream()
{
	std::lock_guard<std::mutex> lock(mStream->getMutex());
	return mStream->isEndOfStream();
}

} // namespace stream
} // namespace media
