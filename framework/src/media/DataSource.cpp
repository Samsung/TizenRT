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
	, mAudioType(AUDIO_TYPE_INVALID)
{
	medvdbg("DataSource::DataSource()\n");
}

DataSource::DataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat)
	: mChannels(channels)
	, mSampleRate(sampleRate)
	, mPcmFormat(pcmFormat)
	, mAudioType(AUDIO_TYPE_INVALID)
{
	medvdbg("DataSource::DataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat)\n");
}

DataSource::DataSource(const DataSource& source)
	: mChannels(source.mChannels)
	, mSampleRate(source.mSampleRate)
	, mPcmFormat(source.mPcmFormat)
	, mAudioType(source.mAudioType)
{
}

DataSource& DataSource::operator=(const DataSource& source)
{
	mChannels = source.mChannels;
	mSampleRate = source.mSampleRate;
	mPcmFormat = source.mPcmFormat;
	mAudioType = source.mAudioType;
	return *this;
}

unsigned int DataSource::getChannels()
{
	return mChannels;
}

unsigned int DataSource::getSampleRate()
{
	return mSampleRate;
}

audio_format_type_t DataSource::getPcmFormat()
{
	return mPcmFormat;
}

void DataSource::setChannels(unsigned int channels)
{
	mChannels = channels;
}

void DataSource::setSampleRate(unsigned int sampleRate)
{
	mSampleRate = sampleRate;
}

void DataSource::setPcmFormat(audio_format_type_t pcmFormat)
{
	mPcmFormat = pcmFormat;
}

void DataSource::setAudioType(audio_type_t audioType)
{
	mAudioType = audioType;
}

audio_type_t DataSource::getAudioType()
{
	return mAudioType;
}

DataSource::~DataSource()
{
	medvdbg("DataSource::~DataSource()\n");
}
} // namespace media
