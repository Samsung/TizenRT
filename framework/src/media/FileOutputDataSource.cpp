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
#include "utils/MediaUtils.h"
#include "Encoder.h"

namespace media {
namespace stream {

FileOutputDataSource::FileOutputDataSource(const std::string& dataPath)
	: OutputDataSource(), mDataPath(dataPath), mFp(nullptr)
{
}

FileOutputDataSource::FileOutputDataSource(unsigned short channels, unsigned int sampleRate, int pcmFormat, const std::string& dataPath)
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
	if (mFp) {
		medvdbg("file already exists\n");
		return false;
	}

	setAudioType(utils::getAudioTypeFromPath(mDataPath));

	switch (getAudioType()) {
		case AUDIO_TYPE_OPUS:
			setEncoder(std::make_shared<Encoder>(AUDIO_TYPE_OPUS));
			break;

		default:
			/* Don't set any decoder for unsupported formats */
			break;
	}

	mFp = fopen(mDataPath.c_str(), "w");
	if (mFp) {
		medvdbg("file open success\n");
		return true;
	} else {
		medvdbg("file open failed\n");
		return false;
	}
}

bool FileOutputDataSource::close()
{
	if (mFp && fclose(mFp) != EOF) {
		mFp = nullptr;
		return true;
	}

	return false;
}

bool FileOutputDataSource::isPrepare()
{
	return (mFp != nullptr);
}

size_t FileOutputDataSource::write(unsigned char* buf, size_t size)
{
	size_t wlen = 0;
	if (!isPrepare()) {
		return wlen;
	}

	std::shared_ptr<Encoder> encoder = getEncoder();

	while (wlen < size) {
		if (encoder) {
			// Push data as much as possible
			wlen += encoder->pushData(buf + wlen, size - wlen);

			// Get encoded data and save to file.
			// Actually, it's an encoding process.
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
				assert(written == ret);
			}
		} else {
			wlen = fwrite(buf + wlen, sizeof(unsigned char), size - wlen, mFp);
			medvdbg("written size : %d\n", wlen);
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
