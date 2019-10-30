/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include "BufferInputDataSource.h"
#include <stdio.h>
#include <string.h>
#include <media/MediaTypes.h>

BufferInputDataSource::BufferInputDataSource()
	: mFp(nullptr)
	, mSrcBuf(nullptr)
	, mSrcSize(0)
{

}

bool BufferInputDataSource::isPrepared()
{
	return mFp && mSrcBuf && mSrcSize;
}

bool BufferInputDataSource::open()
{
	if (!isPrepared()) {
		mFp = fopen("/rom/over_16000.mp3", "rb");
		mSrcSize = 2048;
		mSrcBuf = new unsigned char[mSrcSize];
		// Application can specify all audio informations (audioType/sampleRate/channels/pcmFormat),
		// Otherwise, player will identify them from data stream during prepareAsync().
	}
	return isPrepared();
}

bool BufferInputDataSource::close()
{
	delete[] mSrcBuf;
	return fclose(mFp);
}

ssize_t BufferInputDataSource::read(unsigned char *buf, size_t size)
{
	if (mSrcSize >= size) {
		size_t ret = fread(mSrcBuf, sizeof(unsigned char), size, mFp);
		if (ret != 0) {
			memcpy(buf, mSrcBuf, size);
		}
		return ret;
	}

	size_t used = 0;
	while (used < size) {
		size_t len = size - used > mSrcSize ? mSrcSize : size - used;
		size_t ret = fread(mSrcBuf, sizeof(unsigned char), len, mFp);
		if (ret == 0) {
			return used;
		} else {
			memcpy(buf + used, mSrcBuf, len);
			used += ret;
		}
	}

	return used;
}
