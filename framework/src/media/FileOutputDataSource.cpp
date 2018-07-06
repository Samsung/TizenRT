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
#include <media/FileOutputDataSource.h>
#include "utils/MediaUtils.h"
#include "Encoder.h"

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

ssize_t FileOutputDataSource::write(unsigned char* buf, size_t size)
{
	if (!isPrepare()) {
		return (ssize_t)EOF;
	}

	if (!buf) {
		meddbg("buf is nullptr, hence return EOF\n");
		return (ssize_t)EOF;
	}

	std::shared_ptr<Encoder> encoder = getEncoder();

	size_t wlen = 0;
	while (wlen < size) {
		if (encoder) {
			// Push data as much as possible
			size_t pushed = encoder->pushData(buf + wlen, size - wlen);
			if (pushed == 0) {
				meddbg("Can not push data! Error occurred during encoding!\n");
				break;
			}

			wlen += pushed;

			// Encode data and write to file.
			while (1) {
				// Reuse 'wlen' bytes free space in 'buf'.
				// Encoded data size is usually smaller than origin PCM data size.
				size_t ret = wlen;
				if (!encoder->getFrame(buf, &ret)) {
					// Need push more data
					break;
				}

				size_t written = fwrite(buf, sizeof(unsigned char), ret, mFp);
				medvdbg("written size: %d\n", written);
				if (written != ret) {
					meddbg("Can not write all!\n");
					break;
				}
			}
		} else {
			// Write origin data to file
			wlen += fwrite(buf + wlen, sizeof(unsigned char), size - wlen, mFp);
			medvdbg("written size : %d\n", wlen);
			break;
		}
	}

	return wlen;
}

FileOutputDataSource::~FileOutputDataSource()
{
	close();
}
} // namespace stream
} // namespace media
