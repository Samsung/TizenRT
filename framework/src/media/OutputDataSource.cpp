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

#include <media/OutputDataSource.h>
#include "Encoder.h"

namespace media {
namespace stream {
OutputDataSource::OutputDataSource()
	: DataSource(), mAudioType(AUDIO_TYPE_INVALID), mEncoder(nullptr)
{
}

OutputDataSource::OutputDataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat)
	: DataSource(channels, sampleRate, pcmFormat), mAudioType(AUDIO_TYPE_INVALID), mEncoder(nullptr)
{
}

OutputDataSource::OutputDataSource(const OutputDataSource& source)
	: DataSource(source), mAudioType(source.mAudioType), mEncoder(source.mEncoder)
{
}

OutputDataSource& OutputDataSource::operator=(const OutputDataSource& source)
{
	DataSource::operator=(source);
	return *this;
}

OutputDataSource::~OutputDataSource()
{
}

void OutputDataSource::setEncoder(std::shared_ptr<Encoder> encoder)
{
	mEncoder = encoder;
}

const std::shared_ptr<Encoder> OutputDataSource::getEncoder()
{
	return mEncoder;
}

void OutputDataSource::setAudioType(audio_type_t audioType)
{
	mAudioType = audioType;
}

audio_type_t OutputDataSource::getAudioType()
{
	return mAudioType;
}



} // namespace stream
} // namespace media
