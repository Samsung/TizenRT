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

#ifndef OPUS_ENCODER_API_H
#define OPUS_ENCODER_API_H

#include <stdint.h>
#include <opus/opus.h>

#ifdef __cplusplus
extern "C" {
#endif

struct opus_enc_external_s {
	/*
	 * INPUT:
	 * Configures the application mode in the encoder. It will be used in opus_encoder_init() function.
	 * There are 3 values for selected: OPUS_APPLICATION_VOIP/OPUS_APPLICATION_AUDIO/OPUS_APPLICATION_RESTRICTED_LOWDELAY.
	 * OPUS_APPLICATION_VOIP : rocess signal for improved speech intelligibility.
	 * OPUS_APPLICATION_AUDIO : Favor faithfulness to the original input.
	 * OPUS_APPLICATION_RESTRICTED_LOWDELAY: Configure the minimum possible coding delay by disabling certain modes
	 * of operation.
	 */
	int32_t opus_application;

	/*
	 * INPUT:
	 * Configures the frame size in the encoder. It will be used in opus_encode() function.
	 * It supports value:2.5(25), 5, 10, 20, 40, 60, 80, 100, 120.(ms)
	 * For example: If value is 10, it means the expected frame can play 10 ms. And
	 * in the input PCM data, (inputChannels*inputSampleRate* 10/1000) samples will be encoded to the opus frame.
	 */
	int32_t frame_size_ms;

	/*
	 * INPUT:
	 * Configures the bitrate in the encoder. It means Bitrate in bits per second.
	 * Rates from 500 to 512000 bits per second are meaningful, as well as the special values #OPUS_AUTO and #OPUS_BITRATE_MAX.
	 * OPUS_BITRATE_MAX : can be used to cause the codec to use as much  rate as it can.
	 * OPUS_AUTO : it is determined based on the number of channels and the input sampling rate.
	 */
	int32_t bitrate_bps;

	/*
	 * INPUT:
	 * Configures the encoder's bandpass to a specific value.
	 * It has values : OPUS_AUTO/OPUS_BANDWIDTH_NARROWBAND/OPUS_BANDWIDTH_MEDIUMBAND/
	 * OPUS_BANDWIDTH_WIDEBAND/OPUS_BANDWIDTH_SUPERWIDEBAND/OPUS_BANDWIDTH_SUPERWIDEBAND
	 * OPUS_AUTO : encoder will automatically selecting the bandpass based on the available bitrate.
	 * Others will prevent the encoder from automatically selecting the bandpass.
	 * <dt>#OPUS_BANDWIDTH_NARROWBAND</dt>    <dd>4 kHz passband</dd>
	 * <dt>#OPUS_BANDWIDTH_MEDIUMBAND</dt>    <dd>6 kHz passband</dd>
	 * <dt>#OPUS_BANDWIDTH_WIDEBAND</dt>      <dd>8 kHz passband</dd>
	 * <dt>#OPUS_BANDWIDTH_SUPERWIDEBAND</dt><dd>12 kHz passband</dd>
	 * <dt>#OPUS_BANDWIDTH_FULLBAND</dt>     <dd>20 kHz passband</dd>
	 */
	int32_t band_width;

	/*
	 * INPUT:
	 * Configures the encoder's computational complexity.
	 * The supported range is 0-10 inclusive with 10 representing the highest complexity.
	 */
	int32_t complexity;

	/*
	 * INPUT:
	 * Pointer to the input buffer that contains the audio PCM data. It will be used in the opus_encode() function.
	 * The data is filled in such that the first bit transmitted is
	 * the most-significant bit (MSB) of the first array element.
	 */
	int16_t *pInputBuffer;

	/*
	 * INPUT:
	 * The actual size in bytes of the input buffer.
	 */
	int32_t inputBufferMaxLength;

	/*
	 * INPUT:
	 * Number of valid bytes in the input buffer, set by the calling function.
	 */
	int32_t inputBufferCurrentLength;

	/*
	 * INPUT/OUTPUT:
	 * Number of data in bytes used by the library, modified by each call to opus_frameEncode().
	 */
	int32_t inputBufferUsedLength;

	/*
	 * INPUT:
	 * Channel numbers of the input PCM audio. It will be used in the opus_encoder_init() function.
	 */
	int32_t inputChannels;

	/*
	 * INPUT:
	 * Sample rate of the input PCM audio. It will be used in the opus_encoder_init() function.
	 */
	int32_t inputSampleRate;

	/*
	 * INPUT: (but what is pointed to is an output)
	 * Pointer to the output buffer to hold the encoded Opus data.
	 * The output Opus data is composed of SYNC(4 bytes) +  frame length(4 bytes) + frame data.
	 */
	uint8_t *pOutputBuffer;

	/*
	 * INPUT:
	 * The actual size of the buffer in bytes.
	 * This variable is not used by the library, but is used by the
	 * console test application. This parameter could be deleted
	 * if this value was passed into these function.
	 */
	int32_t outputBufferMaxLength; // < 1500

	/*
	 * OUTPUT:
	 * Out: Size of the output data in bytes.
	 * Because the output Opus data is composed of SYNC(4 bytes) +  frame data Length(4 bytes) + frame data.
	 * The size is 8+ frame data Length.
	 */
	int32_t outputDataSize;
};

typedef struct opus_enc_external_s opus_enc_external_t;
typedef struct opus_enc_external_s *opus_enc_external_p;

/** Gets the size of internal OpusEncoder structure,which will be internally used.
  * @retval returns The size in bytes.
  */
uint32_t opus_encoderMemRequirements(void);

/** Initializes a previously allocated encoder memory pMem, by configuration in opus_enc_external_t.
  * The memory pointed to by pMem must be at least the size returned by opus_encoderMemRequirements().
  * @param [in] pExt <tt>opus_enc_external_t *</tt>: Encoder external structure for setting user configuration, pointer to input/output buffer.
  * @param [in] pMem :Encoder internal used memory, saving coniguration and so on.
  * @retval #OPUS_OK Success or @ref opus_errorcodes
  */
int32_t opus_initEncoder(opus_enc_external_t *pExt, void *pMem);

/** Uninitialize encoder resource.
  * Thread resource is allocated during opus_initEncoder().
  * @param [in] pMem :Decoder internal used memory.
  */
int32_t opus_uninitEncoder(void *pMem);

/** Reset a previously allocated encoder memory pMem.
  * The memory pointed to by pMem must be at least the size returned by opus_encoderMemRequirements().
  * @param [in] pMem :Encoder internal used memory, saving coniguration and so on.
  */
void opus_resetEncoder(void *pMem);

/** Encode the 16-bit PCM data to Opus frame data, and user can get output data from opus_enc_external_t struct.
  * The encode length can be calculated from frame_size_ms of opus_enc_external_t struct.
  * @param [in] pExt <tt>opus_enc_external_t *</tt>: Encoder external structure for setting user configuration, pointer to input/output buffer.
  * @param [in] pMem :Encoder internal used memory, saving coniguration and so on.
  * @retval #OPUS_OK Success or @ref opus_errorcodes
  */
int32_t opus_frameEncode(opus_enc_external_t *pExt, void *pMem);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* OPUS_ENCODER_API_H */

