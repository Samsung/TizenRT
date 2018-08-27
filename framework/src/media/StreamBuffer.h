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

struct rb_s;

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
		virtual ~Builder();
		Builder &setBufferSize(size_t bufferSize);
		Builder &setThreshold(size_t threshold);
		std::shared_ptr<StreamBuffer> build();

	private:
		size_t mBufferSize;
		size_t mThreshold;
	};

	StreamBuffer(size_t bufferSize, size_t threshold);
	virtual ~StreamBuffer();

	bool init(size_t size);
	bool reset();
	std::mutex &getMutex() { return mMutex; }
	std::condition_variable &getCondv() { return mCondv; }
	void setObserver(BufferObserverInterface *observer);

public:
	enum class State {
		OVERRUN,
		UNDERRUN,
		UPDATED,
	};
	void notifyObserver(State st, ...);
	size_t read(unsigned char *buf, size_t size);
	size_t write(unsigned char *buf, size_t size);
	size_t sizeOfData();
	size_t sizeOfSpace();
	void setEndOfStream();
	bool isEndOfStream();
	size_t getBufferSize() { return mBufferSize; }
	size_t getThreshold() { return mThreshold; }

private:
	std::mutex mMutex;
	std::condition_variable mCondv;
	BufferObserverInterface *mObserver;
	struct rb_s *mRingBuf;
	bool mEOS;
	size_t mBufferSize;
	size_t mThreshold;
};

} // namespace stream
} // namespace media

#endif
