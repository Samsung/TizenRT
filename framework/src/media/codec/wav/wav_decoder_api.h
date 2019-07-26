/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#ifndef WAV_DECODER_API_H
#define WAV_DECODER_API_H

#include "../../utils/rbs.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct audio_decoder_s;
typedef struct audio_decoder_s *audio_decoder_p;
typedef void *src_handle_t;

struct wav_dec_external_s {
	/*
	 * INPUT:
	 * Pointer to the input buffer that contains the wav data.
	 */
	uint8_t *pInputBuffer;

	/*
	 * INPUT:
	 * The actual size (in bytes) of the buffer.
	 */
	int32_t inputBufferMaxLength;

	/*
	 * INPUT:
	 * Number of valid bytes in the input buffer, that is the bytes of whole opus packet.
	 */
	int32_t inputBufferCurrentLength;

	/*
	 * INPUT:
	 * Number of requested output audio channels.
	 * Wav decoder conversions internally.
	 */
	int32_t desiredChannels;

	/*
	 * INPUT: (but what is pointed to is an output)
	 * Pointer to the output buffer to hold the 16-bit PCM audio samples.
	 * If the output is stereo, both left and right channels will be stored in this one buffer.
	 */
	int16_t *pOutputBuffer;

	/*
	 * INPUT:
	 * The actual size (in bytes) of the buffer.
	 */
	int32_t outputBufferMaxLength;

	/*
	 * OUTPUT:
	 * Sample rate in Hz of the output PCM data.
	 */
	int32_t samplingRate;

	/*
	 * OUTPUT:
	 * Size of the output frames, one frame consists of 16bit-PCM samples of all channels.
	 */
	int32_t outputFrameSize;
};

typedef struct wav_dec_external_s wav_dec_external_t;
typedef struct wav_dec_external_s *wav_dec_external_p;

int wav_init(audio_decoder_p decoder, void *dec_ext);
bool wav_check_type(rbstream_p rbsp);
bool wav_get_frame(rbstream_p mFp, ssize_t *offset, void *dec_mem, void *buffer, uint32_t *size);
int wav_decode_frame(wav_dec_external_p dec_ext, void *dec_mem, src_handle_t *resampler);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* WAV_DECODER_API_H */

