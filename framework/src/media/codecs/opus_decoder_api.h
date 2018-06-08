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
	 * The data is filled in such that the first bit transmitted is
	 * the most-significant bit (MSB) of the first array element.
	 * The data in the pInputBuffer should be all decoded by opus_decode() one time, and no remained undecoded data.
	 * So should push a whole opus packet to this buffer, and set correct inputBufferCurrentLength.
	 * Here, a whole opus packet is composed of SYNC(4 bytes) +  frame data Length(4 bytes) + frame data.
	 */
	uint8_t *pInputBuffer;

	/*
	 * INPUT:
	 * The actual size of the buffer.
	 * This variable is not used by the library, but is used by the
	 * console test application. This parameter could be deleted
	 * if this value was passed into these function.
	 */
	int32_t inputBufferMaxLength;

	/*
	 * INPUT:
	 * Number of valid bytes in the input buffer, set by the calling
	 * function. After decoding the bitstream the library checks to
	 * see if it when past this value; it would be to prohibitive to
	 * check after every read operation. This value is not modified by
	 * the Opus library.
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
	 * This value is the bitrate in units of bits/second. IT
	 * is calculated using the number of bits consumed for the current frame,
	 * and then multiplying by the sampling_rate, divided by points in a frame.
	 * This value can changes frame to frame.
	 */
	int32_t bitRate;

	/*
	 * INPUT/OUTPUT:
	 * In: Inform decoder how much more room is available in the output buffer in int16 samples
	 * Out: Size of the output frame in 16-bit words.
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
	 * The actual size of the buffer.
	 * This variable is not used by the library, but is used by the
	 * console test application. This parameter could be deleted
	 * if this value was passed into these function.
	 */
	int32_t outputBufferMaxLength;
};

typedef struct opus_dec_external_s opus_dec_external_t;
typedef struct opus_dec_external_s *opus_dec_external_p;

/** Gets the size of internal OpusDecoder structure,which will be internally used.
  * @retval returns The size in bytes.
  */
uint32_t opus_decoderMemRequirements(void);

/** Initializes a previously allocated decoder memory pMem, by configuration in opus_dec_external_t.
  * The memory pointed to by pMem must be at least the size returned by opus_decoderMemRequirements().
  * @param [in] pExt <tt>opus_dec_external_t *</tt>: Decoder external structure for setting user configuration, pointer to input/output buffer and so on.
  * @param [in] pMem :Decoder internal used memory.
  * @retval #OPUS_OK Success or @ref opus_errorcodes
  */
int32_t opus_initDecoder(opus_dec_external_t *pExt, void *pMem);

/** Uninitialize decoder resource.
  * Thread resource is allocated during opus_initDecoder().
  * @param [in] pMem :Decoder internal used memory.
  */
int32_t opus_uninitDecoder(void *pMem);

/** Reset a previously allocated decoder memory pMem.
  * The memory pointed to by pMem must be at least the size returned by opus_decoderMemRequirements().
  * @param [in] pMem :Decoder internal used memory.
  */
void opus_resetDecoder(void *pMem);

/** Decode the Opus frame data to 16-bit PCM audio samples, and user can get output data from opus_enc_external_t struct.
  * The decoded data must be a whole packet. It is composed of SYNC(4 bytes) +  frame data Length(4 bytes) + frame data.
  * @param [in] pExt <tt>opus_dec_external_t *</tt>: Decoder external structure for setting user configuration, pointer to input/output buffer and so on.
  * @param [in] pMem :Decoder internal used memory.
  * @retval #OPUS_OK Success or @ref opus_errorcodes
  */
int32_t opus_frameDecode(opus_dec_external_t *pExt, void *pMem);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* OPUS_DECODER_API_H */

