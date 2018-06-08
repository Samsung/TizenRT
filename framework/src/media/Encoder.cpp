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

#define PV_SUCCESS 0
#define PV_FAILURE -1

namespace media {

Encoder::Encoder(audio_type_t audio_type)
{
#ifdef CONFIG_AUDIO_CODEC
	switch (audio_type) {

#ifdef CONFIG_CODEC_LIBOPUS
	case AUDIO_TYPE_OPUS: {
		memset(&mEncoder, 0, sizeof(aud_encoder_t));

		static uint8_t outputBuf[1024];
		static int16_t inputBuf[1920*2];  // inputSampleRate / 1000 * frame_size_ms * 2

		opus_enc_external_t ext = {0};
		// params for opus encoding
		ext.applicationMode = OPUS_APPLICATION_AUDIO;
		ext.frameSizeMS = 100;
		ext.bitrate = 16000;
		ext.bandWidth = OPUS_AUTO;
		ext.complexity = 5;

		// params for streaming
		ext.pOutputBuffer = outputBuf;
		ext.outputBufferMaxLength = sizeof(outputBuf);
		ext.pInputBuffer = inputBuf;
		ext.inputBufferMaxLength = sizeof(inputBuf);

		// params about PCM source
		ext.inputChannels = 2; // channels
		ext.inputSampleRate = 16000; // sample rate

		// Initialize encoder
		if (aud_encoder_init(&mEncoder, CONFIG_AUDIO_CODEC_RINGBUFFER_SIZE, AUDIO_TYPE_OPUS, &ext) != PV_SUCCESS) {
			meddbg("Error! aud_encoder_init failed!\n");
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
	if (aud_encoder_finish(&mEncoder) != PV_SUCCESS) {
		meddbg("Error! aud_encoder_finish failed!\n");
	}
#endif
}

size_t Encoder::pushData(unsigned char *buf, size_t size)
{
#ifdef CONFIG_AUDIO_CODEC
	return aud_encoder_pushdata(&mEncoder, buf, size);
#endif
	return 0;
}

bool Encoder::getFrame(unsigned char *buf, size_t *size)
{
#ifdef CONFIG_AUDIO_CODEC
	int len = *size;
	len = aud_encoder_getframe(&mEncoder, (void *) buf, len);

	if (len <= 0) {
		medvdbg("Error! aud_encoder_getframe failed!\n");
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
	return aud_encoder_dataspace_is_empty(&mEncoder);
#endif
	return false;
}

size_t Encoder::getAvailSpace()
{
#ifdef CONFIG_AUDIO_CODEC
	return aud_encoder_dataspace(&mEncoder);
#endif
	return 0;
}

} // namespace media
