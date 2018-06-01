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

#include <media/FileInputDataSource.h>
#include "utils/MediaUtils.h"
#include <debug.h>
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

FileInputDataSource::FileInputDataSource(const FileInputDataSource &source) : InputDataSource(source)
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
			setDecoder(std::make_shared<Decoder>());
			break;
		case AUDIO_TYPE_OPUS:
			/* To be supported */
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

size_t FileInputDataSource::read(unsigned char *buf, size_t size)
{
	size_t ret;
	size_t readSize = size;
	std::shared_ptr<Decoder> decoder = getDecoder();

	if (!buf) {
		meddbg("buf is nullptr, hence return 0\n");
		return 0;
	}

	if (decoder) {
		size_t frames = 0;
		/* TODO Currently it just return frames which decoded only ONCE if available */
		ret = getDecodeFrames(buf, &frames);
		medvdbg("decode frame : %d\n", frames);
		if (ret > 0) {
			return ret;
		}

		if (readSize > decoder->getAvailSpace()) {
			readSize = decoder->getAvailSpace();
		}
	}

	ret = fread(buf, sizeof(unsigned char), readSize, mFp);
	/* If file position reaches end of file, return 0 */
	if (feof(mFp)) {
		medvdbg("eof!! stop!\n");
		return 0;
	}

	medvdbg("read size : %d\n", ret);
	if (ret == 0) {
		return EOF;
	}

	if (decoder) {
		if (!decoder->pushData(buf, ret)) {
			return EOF;
		}
		size_t frames = 0;
		if (getDecodeFrames(buf, &frames)) {
			return frames;
		}
		return EOF;
	}
	return ret;
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
