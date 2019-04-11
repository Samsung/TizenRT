/******************************************************************
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

#ifndef OPUS_DECODER_API_H
#define OPUS_DECODER_API_H

#include <stdint.h>
#include <opus/opus.h>

#ifdef __cplusplus
extern "C" {
#endif

struct opus_dec_external_s {
	/*
	 * INPUT:
	 * Pointer to the input buffer that contains the encoded Opus data.
	 * The data in the pInputBuffer should be a whole opus packet, which consists
	 * of SYNC(4 bytes) +  Frame Length(4 bytes) + payload.
	 * So make sure to set correct inputBufferCurrentLength.
	 * All input data is decoded by opus_decode() at one time.
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
	 *  Opus lib can do stereo-to-mono or mono-to-stereo conversions internally.
	 */
	int32_t desiredChannels;

	/*
	 * INPUT:
	 * Desired sampling rate decoded from the bitstream, in units of samples/second.
	 * Opus lib can do resample internally.
	 * Opus supported sample rate: 8000/12000/16000/24000/48000
	 */
	int32_t desiredSampleRate;

	/*
	 * OUTPUT:
	 * This value is the bitrate in units of bits/second.
	 * It's calculated using the number of bits consumed for the current frame,
	 * and then multiplying by the sampling_rate, divided by points in a frame.
	 * This value can changes frame to frame.
	 */
	int32_t bitRate;

	/*
	 * OUTPUT:
	 * Size of the output frames, one frame consists of 16bit-PCM samples of all channels.
	 */
	int32_t outputFrameSize;

	/*
	 * INPUT: (but what is pointed to is an output)
	 * Pointer to the output buffer to hold the 16-bit PCM audio samples.
	 * If the output is stereo, both left and right channels will be stored
	 * in this one buffer.
	 */
	int16_t *pOutputBuffer;

	/*
	 * INPUT:
	 * The actual size (in bytes) of the buffer.
	 */
	int32_t outputBufferMaxLength;
};

typedef struct opus_dec_external_s opus_dec_external_t;
typedef struct opus_dec_external_s *opus_dec_external_p;

/** Gets the size of internal OpusDecoder structure, which will be internally used.
  * @retval returns The size in bytes.
  */
uint32_t opus_decoderMemRequirements(void);

/** Initializes decoder context memory, as per configuration in opus_dec_external_t.
  * The memory pointed to by pMem must be at least the size returned by opus_decoderMemRequirements().
  * @param [in] pExt: Decoder external structure, which is user setting.
  * @param [in] pMem: Decoder internal used memory, user allocated.
  * @return OPUS_OK on success, otherwise, return error codes, see opus_errorcodes
  */
int32_t opus_initDecoder(opus_dec_external_t *pExt, void *pMem);

/** Uninitialize decoder resource.
  * Thread resource is allocated during opus_initDecoder().
  * @param [in] pMem: Decoder internal used memory.
  * @return OPUS_OK on success, otherwise, return error codes, see opus_errorcodes
  */
int32_t opus_uninitDecoder(void *pMem);

/** Reset decoder context memory.
  * The memory pointed to by pMem must be at least the size returned by opus_decoderMemRequirements().
  * @param [in] pMem: Decoder internal used memory.
  */
void opus_resetDecoder(void *pMem);

/** Decode one Opus frame to 16-bit PCM samples, stored in opus_enc_external_t struct.
  * @param [in/out] pExt: Decoder external structure, input frame data and hold output data.
  * @param [in] pMem: Decoder internal used memory.
  * @return OPUS_OK on success, otherwise, return error codes, see opus_errorcodes
  */
int32_t opus_frameDecode(opus_dec_external_t *pExt, void *pMem);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* OPUS_DECODER_API_H */

