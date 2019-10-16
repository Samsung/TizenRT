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
#include <stdlib.h>
#include <debug.h>
#include <media/FileOutputDataSource.h>
#include <media/MediaUtils.h>

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
	if (!mFp) {
		mFp = fopen(mDataPath.c_str(), "wb");
		if (!mFp) {
			meddbg("file open failed error : %d\n", errno);
			return false;
		}

		setAudioType(utils::getAudioTypeFromPath(mDataPath));
		switch (getAudioType()) {
		case AUDIO_TYPE_WAVE:
			if (!utils::createWavHeader(mFp)) {
				meddbg("wav header create failed\n");
				if (fclose(mFp) == OK) {
					mFp = nullptr;
				} else {
					meddbg("file close failed error : %d\n", errno);
				}
				return false;
			}
			break;
		default:
			/* Don't set any encoder for unsupported formats */
			break;
		}
	} else {
		medvdbg("file already exists\n");
		/** return true if mFp is not null, because it means it using now */
	}
	return true;
}

bool FileOutputDataSource::close()
{
	switch (getAudioType()) {
		case AUDIO_TYPE_WAVE: {
			fflush(mFp);
			long ret = ftell(mFp);
			if (ret < 0) {
				meddbg("file size could not be found errno : %d\n", errno);
				break;
			}
			unsigned int fileSize = (unsigned int)ret;
			if (!utils::writeWavHeader(mFp, getChannels(), getSampleRate(), getPcmFormat(), fileSize)) {
				meddbg("wav header write to failed\n");
			}
			break;
		}
		default:
			/* Don't set any encoder for unsupported formats */
			break;
	}
	if (mFp) {
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

bool FileOutputDataSource::isPrepared()
{
	if (mFp == nullptr) {
		return false;
	}
	return true;
}

ssize_t FileOutputDataSource::write(unsigned char *buf, size_t size)
{
	if (size == 0) {
		return 0;
	}

	if (!isPrepared()) {
		return EOF;
	}

	if (buf == nullptr) {
		return EOF;
	}

	return fwrite(buf, sizeof(unsigned char), size, mFp);
}

FileOutputDataSource::~FileOutputDataSource()
{
	if (isPrepared()) {
		close();
	}
}

} // namespace stream
} // namespace media
