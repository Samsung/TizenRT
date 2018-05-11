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

#include <media/DataSource.h>
#include <debug.h>

namespace media {
DataSource::DataSource()
	: mChannels(2)
	, mSampleRate(16000)
	, mPcmFormat(AUDIO_FORMAT_TYPE_S16_LE)
{
	medvdbg("DataSource::DataSource()\n");
}

DataSource::DataSource(unsigned short channels, unsigned int sampleRate, int pcmFormat)
	: mChannels(channels)
	, mSampleRate(sampleRate)
	, mPcmFormat(pcmFormat)
{
	medvdbg("DataSource::DataSource(unsigned short channels, unsigned int sampleRate, int pcmFormat)\n");
}

DataSource::DataSource(const DataSource& source)
	: mChannels(source.mChannels)
	, mSampleRate(source.mSampleRate)
	, mPcmFormat(source.mPcmFormat)
{
}

DataSource& DataSource::operator=(const DataSource& source)
{
	mChannels = source.mChannels;
	mSampleRate = source.mSampleRate;
	mPcmFormat = source.mPcmFormat;
	return *this;
}

unsigned char DataSource::getChannels()
{
	return mChannels;
}

unsigned int DataSource::getSampleRate()
{
	return mSampleRate;
}

int DataSource::getPcmFormat()
{
	return mPcmFormat;
}

bool DataSource::setChannels(unsigned char channels)
{
	if (channels == 0) {
		return false;
	}

	mChannels = channels;
	return true;
}

bool DataSource::setSampleRate(unsigned int sampleRate)
{
	if (sampleRate == 0) {
		return false;
	}

	mSampleRate = sampleRate;
	return true;
}

bool DataSource::setPcmFormat(int pcmFormat)
{
	switch (pcmFormat) {
	case AUDIO_FORMAT_TYPE_S8:
	case AUDIO_FORMAT_TYPE_S16_LE:
	case AUDIO_FORMAT_TYPE_S32_LE:
		mPcmFormat = pcmFormat;
		return true;
	default:
		return false;
	}
}

DataSource::~DataSource()
{
	medvdbg("DataSource::~DataSource()\n");
}
} // namespace media
