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

#include <stdio.h>
#include <debug.h>
#include <media/BufferOutputDataSource.h>

namespace media {
namespace stream {

BufferOutputDataSource::BufferOutputDataSource(OnBufferDataReached callback)
	: OutputDataSource(), mCallback(callback)
{
}

BufferOutputDataSource::BufferOutputDataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat, OnBufferDataReached callback)
	: OutputDataSource(channels, sampleRate, pcmFormat), mCallback(callback)
{
}

BufferOutputDataSource::BufferOutputDataSource(const BufferOutputDataSource& source) :
	OutputDataSource(source), mCallback(source.mCallback)
{
}

BufferOutputDataSource& BufferOutputDataSource::operator=(const BufferOutputDataSource& source)
{
	OutputDataSource::operator=(source);
	return *this;
}

bool BufferOutputDataSource::open()
{
	return true;
}

bool BufferOutputDataSource::close()
{
	return true;
}

bool BufferOutputDataSource::isPrepare()
{
	return true;
}

ssize_t BufferOutputDataSource::write(unsigned char* buf, size_t size)
{
	if (!buf) {
		meddbg("buf is nullptr, hence return EOF\n");
		return EOF;
	}
	if (size <= 0) {
		meddbg("size is 0 or less, hence return EOF\n");
		return EOF;
	}
	ssize_t ret;
	ret = mCallback(buf, size);
	//TODO - The return value of Callback will be modified.
	return ret;
}

BufferOutputDataSource::~BufferOutputDataSource()
{
	close();
}
} // namespace stream
} // namespace media
