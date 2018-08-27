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
#include <debug.h>
#include <media/FileOutputDataSource.h>
#include "utils/MediaUtils.h"
#include "Encoder.h"
#include "StreamBuffer.h"
#include "StreamBufferReader.h"

#ifndef CONFIG_FILE_DATASOURCE_STREAM_BUFFER_SIZE
#define CONFIG_FILE_DATASOURCE_STREAM_BUFFER_SIZE 4096
#endif

#ifndef CONFIG_FILE_DATASOURCE_STREAM_BUFFER_THRESHOLD
#define CONFIG_FILE_DATASOURCE_STREAM_BUFFER_THRESHOLD 2048
#endif

namespace media {
namespace stream {

FileOutputDataSource::FileOutputDataSource(const std::string& dataPath)
	: OutputDataSource(), mDataPath(dataPath), mFp(nullptr)
{
}

FileOutputDataSource::FileOutputDataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat, const std::string& dataPath)
	: OutputDataSource(channels, sampleRate, pcmFormat), mDataPath(dataPath), mFp(nullptr)
{
}

FileOutputDataSource::FileOutputDataSource(const FileOutputDataSource& source) :
	OutputDataSource(source), mDataPath(source.mDataPath), mFp(source.mFp)
{
}

FileOutputDataSource& FileOutputDataSource::operator=(const FileOutputDataSource& source)
{
	OutputDataSource::operator=(source);
	return *this;
}

bool FileOutputDataSource::open()
{
	if (!getStreamBuffer()) {
		auto streamBuffer = StreamBuffer::Builder()
								.setBufferSize(CONFIG_FILE_DATASOURCE_STREAM_BUFFER_SIZE)
								.setThreshold(CONFIG_FILE_DATASOURCE_STREAM_BUFFER_THRESHOLD)
								.build();

		if (!streamBuffer) {
			meddbg("streamBuffer is nullptr!\n");
			return false;
		}

		setStreamBuffer(streamBuffer);
	}

	if (!mFp) {
		setAudioType(utils::getAudioTypeFromPath(mDataPath));

		switch (getAudioType()) {
		case AUDIO_TYPE_OPUS:
			setEncoder(std::make_shared<Encoder>(AUDIO_TYPE_OPUS, getChannels(), getSampleRate()));
			break;

		default:
			/* Don't set any encoder for unsupported formats */
			break;
		}

		mFp = fopen(mDataPath.c_str(), "wb");
		if (mFp) {
			medvdbg("file open success\n");
			start();
			return true;
		} else {
			meddbg("file open failed error : %d\n", errno);
			return false;
		}
	}

	medvdbg("file already exists\n");
	/** return true if mFp is not null, because it means it using now */
	return true;
}

bool FileOutputDataSource::close()
{
	if (mFp) {
		stop();

		int ret = fclose(mFp);
		if (ret == OK) {
			mFp = nullptr;
			medvdbg("close success!!\n");
			return true;
		} else {
			meddbg("close failed ret : %d error : %d\n", ret, errno);
			return false;
		}
	}
	meddbg("close failed, mFp is nullptr!!\n");
	return false;
}

bool FileOutputDataSource::isPrepare()
{
	if (mFp == nullptr) {
		meddbg("mFp is null\n");
		return false;
	}
	return true;
}

ssize_t FileOutputDataSource::onStreamBufferReadable(bool isFlush)
{
	size_t total = 0;
	unsigned char buf[1024];
	auto reader = getBufferReader();

	do {
		size_t size = sizeof(buf);
		size = reader->read(buf, size);
		size_t wlen = 0;
		while (wlen < size) {
			size_t temp = fwrite(buf + wlen, sizeof(unsigned char), size - wlen, mFp);
			if (!temp) {
				// Can not write data, error occurred.
				meddbg("fwrite failed!\n");
				return EOF;
			}
			wlen += temp;
		}

		total += wlen;
	} while (isFlush && (reader->sizeOfData() > 0));

	return (ssize_t) total;
}

FileOutputDataSource::~FileOutputDataSource()
{
	if (mFp) {
		close();
	}
}

} // namespace stream
} // namespace media
