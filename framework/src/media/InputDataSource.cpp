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
#include <media/InputDataSource.h>
#include "Decoder.h"

namespace media {
namespace stream {

InputDataSource::InputDataSource() : DataSource(), mAudioType(AUDIO_TYPE_INVALID)
{
}

InputDataSource::InputDataSource(const InputDataSource &source) : DataSource(source), mAudioType(source.mAudioType)
{
}

InputDataSource &InputDataSource::operator=(const InputDataSource &source)
{
	DataSource::operator=(source);
	return *this;
}

InputDataSource::~InputDataSource()
{
}

void InputDataSource::setDecoder(std::shared_ptr<Decoder> decoder)
{
	mDecoder = decoder;
}

const std::shared_ptr<Decoder> InputDataSource::getDecoder()
{
	return mDecoder;
}

void InputDataSource::setAudioType(audio_type_t audioType)
{
	mAudioType = audioType;
}

audio_type_t InputDataSource::getAudioType()
{
	return mAudioType;
}

size_t InputDataSource::getDecodeFrames(unsigned char *buf, size_t *size)
{
	unsigned int sampleRate = 0;
	unsigned short channels = 0;

	if (mDecoder->getFrame(buf, size, &sampleRate, &channels)) {
		/* TODO set configuration should be removed when we finish implement header parser */
		setSampleRate(sampleRate);
		setChannels(channels);
		medvdbg("size : %u samplerate : %d channels : %d\n", size, sampleRate, channels);
		return *size;
	}

	return 0;
}

} // namespace stream
} // namespace media
