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

#include <media/FileInputDataSource.h>
#include "utils/MediaUtils.h"

namespace media {
namespace stream {

FileInputDataSource::FileInputDataSource() :
	InputDataSource(),
	mDataPath(""),
	mFp(nullptr)
{
}

FileInputDataSource::FileInputDataSource(const std::string &dataPath) :
	InputDataSource(),
	mDataPath(dataPath),
	mFp(nullptr)
{
}

FileInputDataSource::FileInputDataSource(const FileInputDataSource &source) :
	InputDataSource(source),
	mDataPath(source.mDataPath),
	mFp(source.mFp)
{
}

FileInputDataSource &FileInputDataSource::operator=(const FileInputDataSource &source)
{
	InputDataSource::operator=(source);
	return *this;
}

bool FileInputDataSource::open()
{
	if (!mFp) {
		unsigned int channel;
		unsigned int sampleRate;
		audio_format_type_t pcmFormat;
		audio_type_t audioType;

		mFp = fopen(mDataPath.c_str(), "rb");
		if (!mFp) {
			medvdbg("file open failed error : %d\n", errno);
			return false;
		}

		audioType = utils::getAudioTypeFromPath(mDataPath);
		setAudioType(audioType);
		switch (audioType) {
		case AUDIO_TYPE_MP3:
		case AUDIO_TYPE_AAC:
			if (!utils::header_parsing(mFp, audioType, &channel, &sampleRate, NULL)) {
				medvdbg("header parsing failed\n");
				return false;
			}
			setSampleRate(sampleRate);
			setChannels(channel);
			break;
		case AUDIO_TYPE_WAVE:
			if (!utils::header_parsing(mFp, audioType, &channel, &sampleRate, &pcmFormat)) {
				medvdbg("header parsing failed\n");
				return false;
			}
			if (fseek(mFp, WAVE_HEADER_LENGTH, SEEK_SET) != 0) {
				medvdbg("file seek failed error\n");
				return false;
			}
			setSampleRate(sampleRate);
			setChannels(channel);
			setPcmFormat(pcmFormat);
			break;
		case AUDIO_TYPE_FLAC:
			/* To be supported */
			break;
		default:
			/* Don't set any decoder for unsupported formats */
			break;
		}

		return true;
	}

	/** return true if mFp is not null, because it means it using now */
	return true;
}

bool FileInputDataSource::close()
{
	bool ret = true;
	if (mFp) {
		if (fclose(mFp) == OK) {
			mFp = nullptr;
			medvdbg("close success!!\n");
		} else {
			meddbg("close failed ret : %d error : %d\n", ret, errno);
			ret = false;
		}
	} else {
		meddbg("close failed, mFp is nullptr!!\n");
		ret = false;
	}

	return ret;
}

bool FileInputDataSource::isPrepare()
{
	if (mFp == nullptr) {
		return false;
	}
	return true;
}

ssize_t FileInputDataSource::read(unsigned char *buf, size_t size)
{
	if (!isPrepare()) {
		meddbg("%s[line : %d] Fail : FileInputDataSource is not prepared\n", __func__, __LINE__);
		return EOF;
	}

	if (buf == nullptr) {
		meddbg("%s[line : %d] Fail : buf is nullptr\n", __func__, __LINE__);
		return EOF;
	}

	size_t rlen = fread(buf, sizeof(unsigned char), size, mFp);
	medvdbg("read size : %d\n", rlen);
	if (rlen == 0) {
		/* If file position reaches end of file, it's a normal case, we returns 0 */
		if (feof(mFp)) {
			medvdbg("eof!!!\n");
			return 0;
		}

		/* Otherwise, an error occurred, we also returns error */
		return EOF;
	}

	return rlen;
}

FileInputDataSource::~FileInputDataSource()
{
	if (isPrepare()) {
		close();
	}
}

} // namespace stream
} // namespace media
