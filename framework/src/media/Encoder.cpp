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
#include <string.h>
#include "Encoder.h"
#include <debug.h>

#define MAX_PACKET_SIZE 1024
#define MSEC_PER_SECOND 1000

namespace media {

Encoder::Encoder(audio_type_t audio_type, unsigned short channels, unsigned int sampleRate)
	: inputBuf(nullptr)
	, outputBuf(nullptr)
{
#ifdef CONFIG_AUDIO_CODEC
	switch (audio_type) {

#ifdef CONFIG_CODEC_LIBOPUS
	case AUDIO_TYPE_OPUS: {
		memset(&mEncoder, 0, sizeof(audio_encoder_t));

		opus_enc_external_t ext = {0};

		// params for opus encoding
		#if defined(CONFIG_OPUS_APPLICATION_VOIP)
		ext.applicationMode = OPUS_APPLICATION_VOIP;
		#elif defined(CONFIG_OPUS_APPLICATION_AUDIO)
		ext.applicationMode = OPUS_APPLICATION_AUDIO;
		#elif defined(CONFIG_OPUS_APPLICATION_RESTRICTED_LOWDELAY)
		ext.applicationMode = OPUS_APPLICATION_RESTRICTED_LOWDELAY;
		#else
		ext.applicationMode = OPUS_APPLICATION_AUDIO;
		#endif
		ext.complexity = CONFIG_OPUS_ENCODE_COMPLEXITY;
		ext.frameSizeMS = CONFIG_OPUS_ENCODE_FRAMESIZE;
		ext.bitrate = CONFIG_OPUS_ENCODE_BITRATE;
		ext.bandWidth = OPUS_AUTO;

		// allocate memory resource
		size_t inSamples = sampleRate * channels * ext.frameSizeMS / MSEC_PER_SECOND;
		inputBuf = new signed short[inSamples];
		outputBuf = new unsigned char[MAX_PACKET_SIZE];

		// params for streaming
		ext.pOutputBuffer = outputBuf;
		ext.outputBufferMaxLength = sizeof(unsigned char) * MAX_PACKET_SIZE;
		ext.pInputBuffer = inputBuf;
		ext.inputBufferMaxLength = sizeof(signed short) * inSamples;

		// params about PCM source
		ext.inputChannels = channels;
		ext.inputSampleRate = sampleRate;

		// Initialize encoder
		if (audio_encoder_init(&mEncoder, CONFIG_AUDIO_CODEC_RINGBUFFER_SIZE, AUDIO_TYPE_OPUS, &ext) != AUDIO_ENCODER_OK) {
			meddbg("Error! audio_encoder_init failed!\n");
		}
		break;
	}
#endif

	default:
		break;
	}
#endif
}

Encoder::Encoder(const Encoder *source)
{
#ifdef CONFIG_AUDIO_CODEC
	mEncoder = source->mEncoder;
#endif
}

Encoder::~Encoder()
{
#ifdef CONFIG_AUDIO_CODEC
	if (audio_encoder_finish(&mEncoder) != AUDIO_ENCODER_OK) {
		meddbg("Error! audio_encoder_finish failed!\n");
	}

	delete[] inputBuf;
	delete[] outputBuf;
#endif
}

size_t Encoder::pushData(unsigned char *buf, size_t size)
{
#ifdef CONFIG_AUDIO_CODEC
	return audio_encoder_pushdata(&mEncoder, buf, size);
#endif
	return 0;
}

bool Encoder::getFrame(unsigned char *buf, size_t *size)
{
#ifdef CONFIG_AUDIO_CODEC
	int len = *size;
	len = audio_encoder_getframe(&mEncoder, (void *) buf, len);

	if (len <= 0) {
		medvdbg("Error! audio_encoder_getframe failed!\n");
		return false;
	}

	*size = len;
	return true;
#endif
	return false;
}

bool Encoder::empty()
{
#ifdef CONFIG_AUDIO_CODEC
	return audio_encoder_dataspace_is_empty(&mEncoder);
#endif
	return false;
}

size_t Encoder::getAvailSpace()
{
#ifdef CONFIG_AUDIO_CODEC
	return audio_encoder_dataspace(&mEncoder);
#endif
	return 0;
}

} // namespace media
