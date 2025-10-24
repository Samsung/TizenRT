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


#pragma once
#include <common/bk_include.h>
#include <driver/sbc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief SBC API
 * @defgroup sbc.h SBC API group
 * @{
 */

/**
 * @brief     SBC decoder decode one frame
 *
 * This API decode one frame by sbc decoder.
 *
 *
 * @param
 *    - sbc: sbc decoder context pointer;
 *    - data: buffer to be decoded;
 *    - length: the length of input buffer;
 *
 *
 * Usage example:
 *
 *    sbcdecodercontext_t sbc_decoder;
 *	  bk_sbc_decoder_frame_decode(&sbc_decoder, sbc_data, 512);
 *
 *
 * @return
 *    - consumed: buffer length by decoder if no error ocurs, else error code (always small than 0) will be return.
 *
 * @attention 1. the output PCM data please refer to the follows variables:
 *    - sbc->pcm_sample: means output PCM data address
 *    - sbc->pcm_length: means output PCM data length in sample
 *    - sbc->channel_number: means output PCM data channels
 */
bk_err_t bk_sbc_decoder_frame_decode(sbcdecodercontext_t *sbc, const uint8_t *data, uint32_t length);

/**
 * @brief     SBC bit allocation calculate for both encoder and decoder
 *
 * This API calculate sbc bit allocation.
 *
 *
 * @param
 *    - sbc: sbc decoder context pointer
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_bit_allocation(sbccommoncontext_t *sbc);

/**
 * @brief     SBC decoder initialize
 *
 * This API init the sbc decoder function.
 *
 *
 * @param
 *    - sbc: sbc decoder context pointer
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_init(sbcdecodercontext_t *sbc);

/**
 * @brief     SBC decoder deinit
 *
 * This API deinit the sbc decoder function.
 *
 *
 * @param
 *    - None
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_deinit(void);

/**
 * @brief     enable/disable sbc interrupt
 *
 * This API enable or disable sbc interrupt:
 *    - reigster interrupt service handle
 *    - enable or disable sbc interrupt
 *
 *
 * @param
 *    - enable: enable —— 1, disable —— 0
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_interrupt_enable(bool enable);

/**
 * @brief     enable/disable msbc decoder
 *
 * This API enable/disable msbc decoder.
 *
 *
 * @param
 *    - enable: enable —— 1, disable —— 0
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_support_msbc(bool enable);

/**
 * @brief     Register sbc decoder isr
 *
 * This API register sbc decoder isr.
 *
 *
 * @param
 *    - isr: sbc decoder isr callback;
 *    - param: sbc decoder isr callback parameter;
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_register_sbc_isr(sbc_decoder_isr_t isr, void *param);

/**
 * @}
 */
 
#ifdef __cplusplus
 }
#endif


