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
#include <media/BufferInputDataSource.h>

namespace media {
namespace stream {

BufferInputDataSource::BufferInputDataSource(OnBufferInputDataReached callback)
	: InputDataSource(), mCallback(callback)
{
}

BufferInputDataSource::BufferInputDataSource(const BufferInputDataSource &source)
	: InputDataSource(source), mCallback(source.mCallback)
{
}

BufferInputDataSource &BufferInputDataSource::operator=(const BufferInputDataSource &source)
{
	InputDataSource::operator=(source);
	return *this;
}

bool BufferInputDataSource::open()
{
	return true;
}

bool BufferInputDataSource::close()
{
	return true;
}

bool BufferInputDataSource::isPrepare()
{
	return true;
}

ssize_t BufferInputDataSource::read(unsigned char *buf, size_t size)
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

int BufferInputDataSource::seek(long offset, int origin)
{
	//TODO - Implement BufferInputDataSource::seek
	meddbg("BufferInputDataSource::seek is not implemented\n");
	return -1;
}

BufferInputDataSource::~BufferInputDataSource()
{
	close();
}
} // namespace stream
} // namespace media
