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
#include "Decoder.h"
#include <debug.h>

#define PV_SUCCESS 0
#define PV_FAILURE -1

namespace media {

Decoder::Decoder(unsigned short channels, unsigned int sampleRate)
	: mChannels(channels)
	, mSampleRate(sampleRate)
{
#ifdef CONFIG_AUDIO_CODEC
	memset(&mPlayer, 0, sizeof(pv_player_t));
	if (pv_player_init(&mPlayer, CONFIG_AUDIO_CODEC_RINGBUFFER_SIZE, nullptr, nullptr, nullptr, nullptr) !=
		PV_SUCCESS) {
		meddbg("Error! pv_player_init failed!\n");
	}
#endif
}

Decoder::Decoder(const Decoder *source)
{
#ifdef CONFIG_AUDIO_CODEC
	mPlayer = source->mPlayer;
#endif
}

Decoder::~Decoder()
{
#ifdef CONFIG_AUDIO_CODEC
	if (pv_player_finish(&mPlayer) != PV_SUCCESS) {
		meddbg("Error! pv_player_finish failed!\n");
	}
#endif
}

size_t Decoder::pushData(unsigned char *buf, size_t size)
{
#ifdef CONFIG_AUDIO_CODEC
	size_t rmax = getAvailSpace();
	if (size > rmax) {
		meddbg("Error!! data is larger than rmax\n");
		size = rmax;
	}

	return pv_player_pushdata(&mPlayer, buf, size);
#endif
	return 0;
}

/**
 * @brief   Get decoded PCM sample frames
 * @remarks
 * @param   buf:  pointer to buffer for output
 * @param   size: input/output parameter, in bytes.
 *                input-size means the capability of 'buf', and also be the requested size for output.
 *                output-size means the real output data size.
 * @param   sampleRate:  Sample rate (Hz) of the output PCM samples
 * @param   channels:  Number of channels of the output PCM samples
 * @return  true  - output some data to 'buf', and output-size in the range of (0, input-size];
 *          false - no output, and need push more audio data for decoding.
 * @see
 */
bool Decoder::getFrame(unsigned char *buf, size_t *size, unsigned int *sampleRate, unsigned short *channels)
{
#ifdef CONFIG_AUDIO_CODEC
	/*
	 * Need to get the enough data to parse data format.
	 */
	if (mPlayer.audio_type == AUDIO_TYPE_UNKNOWN) {
		mPlayer.audio_type = pv_player_get_audio_type(&mPlayer);

		if (!mConfig(mPlayer.audio_type)) {
			meddbg("Error! mConfig() failed!\n");
			return false;
		}
	}

	*size = pv_player_get_frames(&mPlayer, buf, *size, sampleRate, channels);
	if (*size == 0) {
		return false;
	}

	return true;
#endif
	return false;
}

bool Decoder::empty()
{
#ifdef CONFIG_AUDIO_CODEC
	return pv_player_dataspace_is_empty(&mPlayer);
#endif
	return false;
}

size_t Decoder::getAvailSpace()
{
#ifdef CONFIG_AUDIO_CODEC
	return rb_avail(mPlayer.rbsp->rbp);
#endif
	return 0;
}

#ifdef CONFIG_AUDIO_CODEC
bool Decoder::mConfig(int audioType)
{
	/* To-do: Below buffer size and channel count must be calculated correctly. */
	static uint8_t inputBuf[4096];
	static int16_t outputBuf[4096];

	switch (audioType) {
	case AUDIO_TYPE_MP3: {
		tPVMP3DecoderExternal mp3_ext = {0};
		mp3_ext.equalizerType = flat;
		mp3_ext.crcEnabled = false;
		mp3_ext.pInputBuffer = inputBuf;
		mp3_ext.pOutputBuffer = outputBuf;
		mp3_ext.outputFrameSize = sizeof(outputBuf) / sizeof(int16_t);

		if (pv_player_init_decoder(&mPlayer, audioType, &mp3_ext) != PV_SUCCESS) {
			meddbg("Error! pv_player_init_decoder failed!\n");
			return false;
		}
		break;
	}

	case AUDIO_TYPE_AAC: {
		tPVMP4AudioDecoderExternal aac_ext = {0};
		aac_ext.outputFormat = OUTPUTFORMAT_16PCM_INTERLEAVED;
		aac_ext.desiredChannels = mChannels;
		aac_ext.pInputBuffer = inputBuf;
		aac_ext.pOutputBuffer = outputBuf;
		aac_ext.aacPlusEnabled = 1;

		if (pv_player_init_decoder(&mPlayer, audioType, &aac_ext) != PV_SUCCESS) {
			meddbg("Error! pv_player_init_decoder failed!\n");
			return false;
		}
		break;
	}

#ifdef CONFIG_CODEC_LIBOPUS
	case AUDIO_TYPE_OPUS: {
		opus_dec_external_t opus_ext = {0};
		opus_ext.pInputBuffer = inputBuf;
		opus_ext.inputBufferMaxLength = sizeof(inputBuf);
		opus_ext.pOutputBuffer = outputBuf;
		opus_ext.outputBufferMaxLength = sizeof(outputBuf);
		opus_ext.desiredSampleRate = mSampleRate;
		opus_ext.desiredChannels = mChannels;

		if (pv_player_init_decoder(&mPlayer, audioType, &opus_ext) != PV_SUCCESS) {
			meddbg("Error! pv_player_init_decoder failed!\n");
			return false;
		}
		break;
	}
#endif

	default:
		meddbg("Error! Not supported audio format: %d\n", audioType);
		return false;
	}

	return true;
}
#endif

} // namespace media
