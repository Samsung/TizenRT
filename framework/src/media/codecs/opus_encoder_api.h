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
	 * Configures the application mode in the encoder.
	 * It will be used in opus_encoder_init() function.
	 * Below application mode is selectable:
	 * OPUS_APPLICATION_VOIP : process signal for improved speech intelligibility.
	 * OPUS_APPLICATION_AUDIO : Favor faithfulness to the original input.
	 * OPUS_APPLICATION_RESTRICTED_LOWDELAY: Configure the minimum possible coding
	 * delay by disabling certain modes of operation.
	 */
	int32_t applicationMode;

	/*
	 * INPUT:
	 * Configures the frame size in the encoder. It will be used in opus_encode().
	 * It supports value: 5, 10, 20, 40, 60, 80, 100, 120. (in ms)
	 * Number of (inputChannels*inputSampleRate* frameSizeMS/1000) samples
	 * will be encoded to one opus packet as a transporting frame.
	 */
	int32_t frameSizeMS;

	/*
	 * INPUT:
	 * Configures the bitrate in the encoder. It means Bitrate in bits per second.
	 * Rates from 500 to 512000 bits per second are meaningful,
	 * as well as the special values 'OPUS_AUTO' and 'OPUS_BITRATE_MAX'.
	 * OPUS_BITRATE_MAX : can be used to cause the codec to use as much rate as it can.
	 * OPUS_AUTO : it is determined based on the number of channels and the input sampling rate.
	 */
	int32_t bitrate;

	/*
	 * INPUT:
	 * Configures the encoder's bandpass to a specific value.
	 * Below items are available:
	 * OPUS_AUTO : encoder will automatically select the bandpass based on the available bitrate.
	 * OPUS_BANDWIDTH_NARROWBAND     4 kHz passband
	 * OPUS_BANDWIDTH_MEDIUMBAND     6 kHz passband
	 * OPUS_BANDWIDTH_WIDEBAND       8 kHz passband
	 * OPUS_BANDWIDTH_SUPERWIDEBAND 12 kHz passband
	 * OPUS_BANDWIDTH_FULLBAND      20 kHz passband
	 */
	int32_t bandWidth;

	/*
	 * INPUT:
	 * Configures the encoder's computational complexity.
	 * The supported range is 0-10 inclusive with 10 representing the highest complexity.
	 */
	int32_t complexity;

	/*
	 * INPUT:
	 * Pointer to the input buffer that contains the audio PCM data to be encoded.
	 */
	int16_t *pInputBuffer;

	/*
	 * INPUT:
	 * The actual size (in bytes) of the input buffer.
	 */
	int32_t inputBufferMaxLength;

	/*
	 * INPUT:
	 * Number of valid bytes in the input buffer, set by the calling function.
	 */
	int32_t inputBufferCurrentLength;

	/*
	 * INPUT:
	 * Channel numbers of the input PCM audio. It will be used in the opus_encoder_init().
	 */
	int32_t inputChannels;

	/*
	 * INPUT:
	 * Sample rate of the input PCM audio. It will be used in the opus_encoder_init().
	 */
	int32_t inputSampleRate;

	/*
	 * INPUT: (but what is pointed to is an output)
	 * Pointer to the output buffer to hold the encoded Opus data.
	 * The output Opus data is composed of SYNC(4 bytes) +  frame length(4 bytes) + payload.
	 */
	uint8_t *pOutputBuffer;

	/*
	 * INPUT:
	 * The actual size of the buffer in bytes.
	 */
	int32_t outputBufferMaxLength;

	/*
	 * OUTPUT:
	 * Size of the output data in bytes.
	 * Because the output Opus data is composed of SYNC(4 bytes) +  frame length(4 bytes) + payload.
	 */
	int32_t outputDataSize;
};

typedef struct opus_enc_external_s opus_enc_external_t;
typedef struct opus_enc_external_s *opus_enc_external_p;

/** Gets the size of internal OpusEncoder structure,which will be internally used.
  * @retval returns The size in bytes.
  */
uint32_t opus_encoderMemRequirements(void);

/** Initializes encoder context memory, as per configuration in opus_enc_external_t.
  * The memory pointed to by pMem must be at least the size returned by opus_encoderMemRequirements().
  * @param [in] pExt: Encoder external structure, which is user setting.
  * @param [in] pMem: Encoder internal used memory, user allocated.
  * @return OPUS_OK on success, otherwise, return error codes, see opus_errorcodes
  */
int32_t opus_initEncoder(opus_enc_external_t *pExt, void *pMem);

/** Uninitialize encoder resource.
  * Thread resource is allocated during opus_initEncoder().
  * @param [in] pMem: Encoder internal used memory.
  * @return OPUS_OK on success, otherwise, return error codes, see opus_errorcodes
  */
int32_t opus_uninitEncoder(void *pMem);

/** Reset encoder context memory.
  * The memory pointed to by pMem must be at least the size returned by opus_encoderMemRequirements().
  * @param [in] pMem: Encoder internal used memory.
  */
void opus_resetEncoder(void *pMem);

/** Encode the 16-bit PCM data to Opus frame data, stored in opus_enc_external_t struct.
  * @param [in/out] pExt: Decoder external structure, input frame data and hold output data.
  * @param [in] pMem: Encoder internal used memory, saving encoder context info.
  * @return OPUS_OK on success, otherwise, return error codes, see opus_errorcodes
  */
int32_t opus_frameEncode(opus_enc_external_t *pExt, void *pMem);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* OPUS_ENCODER_API_H */

