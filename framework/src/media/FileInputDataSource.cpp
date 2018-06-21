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

#include <media/FileInputDataSource.h>
#include "utils/MediaUtils.h"
#include "Decoder.h"

namespace media {
namespace stream {

FileInputDataSource::FileInputDataSource() : InputDataSource(), mDataPath(""), mFp(nullptr)
{
}

FileInputDataSource::FileInputDataSource(const std::string &dataPath)
	: InputDataSource(), mDataPath(dataPath), mFp(nullptr)
{
}

FileInputDataSource::FileInputDataSource(const FileInputDataSource &source)
	: InputDataSource(source), mDataPath(source.mDataPath), mFp(source.mFp)
{
}

FileInputDataSource &FileInputDataSource::operator=(const FileInputDataSource &source)
{
	InputDataSource::operator=(source);
	return *this;
}

FileInputDataSource::~FileInputDataSource()
{
	close();
}

bool FileInputDataSource::open()
{
	if (!mFp) {
		setAudioType(utils::getAudioTypeFromPath(mDataPath));

		switch (getAudioType()) {
		case AUDIO_TYPE_MP3:
		case AUDIO_TYPE_AAC:
		case AUDIO_TYPE_OPUS:
			setDecoder(std::make_shared<Decoder>(getChannels(), getSampleRate()));
			break;
		case AUDIO_TYPE_FLAC:
			/* To be supported */
			break;
		default:
			/* Don't set any decoder for unsupported formats */
			break;
		}

		mFp = fopen(mDataPath.c_str(), "rb");
		if (mFp) {
			medvdbg("file open success\n");
			return true;
		} else {
			meddbg("file open failed error : %d\n", errno);
			return false;
		}
	}

	/** return true if mFp is not null, because it means it using now */
	return true;
}

bool FileInputDataSource::close()
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

bool FileInputDataSource::isPrepare()
{
	if (mFp == nullptr) {
		meddbg("mFp is null\n");
		return false;
	}
	return true;
}

ssize_t FileInputDataSource::read(unsigned char *buf, size_t size)
{
	size_t rlen = 0;
	size_t readSize = size;
	std::shared_ptr<Decoder> decoder = getDecoder();

	if (!buf) {
		meddbg("buf is nullptr, hence return EOF\n");
		return EOF;
	}

	if (decoder) {
		size_t frames = size;
		rlen = getDecodeFrames(buf, &frames);
		medvdbg("decode frame : %d/%d\n", rlen, size);
		if (rlen == size) {
			return rlen;
		}

		/* Set buf offset */
		buf += rlen;
		/* Calculate available space in 'buf' */
		readSize -= rlen;

		if (readSize > decoder->getAvailSpace()) {
			readSize = decoder->getAvailSpace();
		}
	}

	size_t readRet = fread(buf, sizeof(unsigned char), readSize, mFp);
	medvdbg("read size : %d\n", readRet);
	if (readRet == 0) {
		/* fread returned 0 */
		/* If file position reaches end of file, it's a normal case, we returns rlen */
		if (feof(mFp)) {
			medvdbg("eof!! stop!\n");
			return rlen;
		}

		/* Otherwise, an error occurred, we also returns error */
		return EOF;
	}

	if (decoder) {
		if (!decoder->pushData(buf, readRet)) {
			meddbg("decode push data failed!\n");
			return EOF;
		}

		size_t frames = size - rlen;
		rlen += getDecodeFrames(buf, &frames);
		medvdbg("decode frame ; %d/%d\n", rlen, size);
		return rlen;
	}

	return readRet;
}

int FileInputDataSource::readAt(long offset, int origin, unsigned char *buf, size_t size)
{
	if (fseek(mFp, offset, origin) != 0) {
		meddbg("FileInputDataSource::readAt : fail to seek\n");
		return -1;
	}

	return fread(buf, sizeof(unsigned char), size, mFp);
}

} // namespace stream
} // namespace media
