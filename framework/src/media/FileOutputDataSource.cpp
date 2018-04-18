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

#include <media/FileOutputDataSource.h>

namespace media {
namespace stream {

FileOutputDataSource::FileOutputDataSource(const std::string& dataPath) 
	: OutputDataSource(), mDataPath(dataPath)
{	
}

FileOutputDataSource::FileOutputDataSource(unsigned short channels, unsigned int sampleRate, int pcmFormat, const std::string& dataPath)
	: OutputDataSource(channels, sampleRate, pcmFormat), mDataPath(dataPath)
{
}

FileOutputDataSource::FileOutputDataSource(const FileOutputDataSource& source) : OutputDataSource(source)
{
}

FileOutputDataSource& FileOutputDataSource::operator=(const FileOutputDataSource& source)
{
	OutputDataSource::operator=(source);
	return *this;
}

bool FileOutputDataSource::open()
{
	mFp = fopen(mDataPath.c_str(), "w+");
	return isPrepare();
}

void FileOutputDataSource::close()
{
	fclose(mFp);
	mFp = nullptr;
}

bool FileOutputDataSource::isPrepare()
{
	return (mFp != nullptr);
}

size_t FileOutputDataSource::write(unsigned char* buf, size_t size)
{
	return fwrite(buf, sizeof(unsigned char), size, mFp);
}

FileOutputDataSource::~FileOutputDataSource()
{
}
} // namespace stream
} // namespace media
