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

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <debug.h>
#include <media/BufferOutputDataSource.h>
#include "MediaRecorderImpl.h"
#include "StreamBuffer.h"
#include "StreamBufferReader.h"

#ifndef CONFIG_BUFFER_DATASOURCE_STREAM_BUFFER_SIZE
#define CONFIG_BUFFER_DATASOURCE_STREAM_BUFFER_SIZE 4096
#endif

#ifndef CONFIG_BUFFER_DATASOURCE_STREAM_BUFFER_THRESHOLD
#define CONFIG_BUFFER_DATASOURCE_STREAM_BUFFER_THRESHOLD 1
#endif

namespace media {
namespace stream {
BufferOutputDataSource::BufferOutputDataSource() :
	OutputDataSource(),
	mIsPrepare(false)
{
}

BufferOutputDataSource::BufferOutputDataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat) :
	OutputDataSource(channels, sampleRate, pcmFormat),
	mIsPrepare(false)
{
}

BufferOutputDataSource::BufferOutputDataSource(const BufferOutputDataSource &source) :
	OutputDataSource(source),
	mIsPrepare(false)
{
}

BufferOutputDataSource &BufferOutputDataSource::operator=(const BufferOutputDataSource &source)
{
	OutputDataSource::operator=(source);
	return *this;
}

bool BufferOutputDataSource::open()
{
	mIsPrepare = true;
	return true;
}

bool BufferOutputDataSource::close()
{
	mIsPrepare = false;
	return true;
}

bool BufferOutputDataSource::isPrepare()
{
	return mIsPrepare;
}

ssize_t BufferOutputDataSource::write(unsigned char *buf, size_t size)
{
	if (size == 0) {
		return 0;
	}

	if (!isPrepare()) {
		return EOF;
	}

	if (buf == nullptr) {
		return EOF;
	}

	auto buffer = new unsigned char[size];
	memcpy(buffer, buf, size);
	auto recorder = getRecorder();
	if (recorder) {
		recorder->notifyObserver(RECORDER_OBSERVER_COMMAND_BUFFER_DATAREACHED, buffer, size);
	}
	// DO NOT `delete[]`, `buffer` will be managed in std::shared_ptr<> and released automatically!

	return size;
}

BufferOutputDataSource::~BufferOutputDataSource()
{
	if (isPrepare()) {
		close();
	}
}

} // namespace stream
} // namespace media
