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

#include <debug.h>

#include <media/FileOutputDataSource.h>

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
		mFp = fopen(mDataPath.c_str(), "w");
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
	if (!buf) {
		meddbg("buf is nullptr, hence return EOF\n");
		return EOF;
	}
	
	size_t ret;
	ret = fwrite(buf, sizeof(unsigned char), size, mFp);
	if (ret == 0 && ferror(mFp)) {
		meddbg("Error : %d\n", errno);
		return EOF;
	}
	return ret;
}

FileOutputDataSource::~FileOutputDataSource()
{
	close();
}
} // namespace stream
} // namespace media
