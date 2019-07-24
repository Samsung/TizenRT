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

#ifndef __MEDIA_STREAMBUFFER_H
#define __MEDIA_STREAMBUFFER_H

#include <memory>
#include <mutex>
#include <condition_variable>
#include "utils/rb.h"

namespace media {
namespace stream {

class BufferObserverInterface;

class StreamBuffer : public std::enable_shared_from_this<StreamBuffer>
{
public:
	class Builder
	{
	public:
		Builder();
		Builder &setBufferSize(size_t bufferSize);
		Builder &setThreshold(size_t threshold);
		std::shared_ptr<StreamBuffer> build();

	private:
		size_t mBufferSize;
		size_t mThreshold;
	};

	StreamBuffer(size_t bufferSize, size_t threshold);
	virtual ~StreamBuffer();
	/**
	 * Initialize stream buffer with specific buffer size.
	 * A ring buffer would be allocated and initialized in this function.
	 */
	bool init(size_t size);
	/**
	 * Reset stream buffer
	 * Data in stream buffer would be cleared.
	 */
	bool reset();
	/**
	 * Register observer for listening stream buffer state.
	 */
	void setObserver(BufferObserverInterface *observer);
	std::mutex &getMutex() { return mMutex; }
	std::condition_variable &getCondv() { return mCondv; }

public:
	enum class State {
		OVERRUN,
		UNDERRUN,
		UPDATED,
	};
	/**
	 * Notify stream buffer state to observer, when stream buffer state changed.
	 */
	void notifyObserver(State st, ...);
	/**
	 * Copy data from stream buffer without change.
	 * And we can give an offset where start to copy.
	 */
	size_t copy(unsigned char *buf, size_t size, size_t offset = 0);
	/**
	 * Read(pop) data from stream buffer.
	 */
	size_t read(unsigned char *buf, size_t size);
	/**
	 * Write(push) data into stream buffer.
	 */
	size_t write(unsigned char *buf, size_t size);
	/**
	 * Get bytes of data available in stream buffer.
	 */
	size_t sizeOfData();
	/**
	 * Get bytes of space available in stream buffer.
	 */
	size_t sizeOfSpace();
	/**
	 * Set the end-of-stream flag.
	 */
	void setEndOfStream();
	/**
	 * Check if the end-of-stream flag was set.
	 */
	bool isEndOfStream();
	size_t getBufferSize() { return mBufferSize; }
	size_t getThreshold() { return mThreshold; }

private:
	std::mutex mMutex;
	std::condition_variable mCondv;
	BufferObserverInterface *mObserver;
	rb_t mRingBuf;
	bool mEOS;
	size_t mBufferSize;
	size_t mThreshold;
};

} // namespace stream
} // namespace media

#endif
