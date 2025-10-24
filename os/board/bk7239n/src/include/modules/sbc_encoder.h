// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "sbc_encoder_types.h"

#pragma once

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


/**
 * @brief  SBC encoder initialzie
 * @param  sbc          SBC encoder context pointer
 * @param  sample_rate  sample rate
 * @param  num_channels number of channels
 * @return error code, @see SBC_ENCODER_ERROR_CODE
 */
int32_t sbc_encoder_init(SbcEncoderContext *sbc, int32_t sample_rate, int32_t num_channels);

/**
 * @brief  SBC encoder parameters config
 * @param  sbc SBC encoder context pointer
 * @param  cmd @see SBC_ENCODER_CTRL_CMD
 * @param  arg the argument or result address for the cmd
 * @return error code, @see SBC_ENCODER_ERROR_CODE
 */
int32_t sbc_encoder_ctrl(SbcEncoderContext *sbc, uint32_t cmd, uint32_t arg);

/**
 * @brief  SBC encoder encode one frame
 * @param  sbc SBC encoder context pointer
 * @param  pcm input PCM samples to be encoded,
 *         the number of input PCM samples MUST be sbc->pcm_length !!!
 * @return encoded buffer length by encoder if no error ocurs,
 *         else error code (always small than 0) will be return, @see SBC_ENCODER_ERROR_CODE
 *         the output encoded buffer refer to sbc->stream.
 */
int32_t sbc_encoder_encode(SbcEncoderContext *sbc, const int16_t *pcm);

/**
 * @brief  Get sample rate by index
 * @param  idx sample rate index, 0:16000, 1:32000, 2:44100, 3:48000
 * @return sample rate
 */
uint16_t sbc_comm_sample_rate_get(uint32_t idx);

/**
 * @brief  CRC8 calculation
 * @param  data  data buffer to do calculation
 * @param  len   data buffer length in bits
 * @return CRC8 value
 */
uint8_t  sbc_common_crc8(const uint8_t *data, uint32_t len);

/**
 * @brief  SBC bit allocation calculate for both encoder and decoder
 * @param  sbc SBC common context pointer
 * @return NULL
 */
void sbc_common_bit_allocation(SbcCommonContext *sbc);


#ifdef __cplusplus
}
#endif//__cplusplus
