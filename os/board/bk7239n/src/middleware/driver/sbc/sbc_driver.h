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

#include <components/log.h>

#define SBC_TAG "sbc"
#define SBC_LOGI(...) BK_LOGI(SBC_TAG, ##__VA_ARGS__)
#define SBC_LOGW(...) BK_LOGW(SBC_TAG, ##__VA_ARGS__)
#define SBC_LOGE(...) BK_LOGE(SBC_TAG, ##__VA_ARGS__)
#define SBC_LOGD(...) BK_LOGD(SBC_TAG, ##__VA_ARGS__)

/**
 * @brief     SBC decoder memory initialize
 *
 * This API init the sbc decoder memory.
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
bk_err_t bk_sbc_decoder_mem_init(void);

/**
 * @brief     enable sbc decoder
 *
 * This API enable sbc decoder function.
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
bk_err_t bk_sbc_decoder_sbc_enable(void);

/**
 * @brief     get the sbc status to check whether is enable
 *
 * This API get the sbc status to check whether is enable.
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
bk_err_t bk_sbc_decoder_get_sbc_enable_status(void);

/**
 * @brief     get the configuration value of sbc decoder
 *
 * This API get the configuration value of sbc decoder.
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
bk_err_t bk_sbc_decoder_get_sbc_ctrl_value(void);

/**
 * @brief     clear sbc interrupt status
 *
 * This API clear sbc interrupt status.
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
bk_err_t bk_sbc_decoder_clear_interrupt_status(void);

/**
 * @brief     get sbc interrupt status
 *
 * This API get sbc interrupt status.
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
bk_err_t bk_sbc_decoder_get_interrupt_status(void);

/**
 * @brief     get the idle status of sbc decoder 
 *
 * This API check whether the sbc is in idle status.
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
bk_err_t bk_sbc_decoder_get_idle_status(void);

/**
 * @brief     the low bit byte that SBC decoder need to resolve 
 *
 * This API resolve the low bit byte.
 *
 *
 * @param
 *    - resl_value: the low bit value that need to resolve
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_set_res_bytel_value(uint32_t resl_value);

/**
 * @brief     the middle bit byte that SBC decoder need to resolve 
 *
 * This API resolve the middle bit byte.
 *
 *
 * @param
 *    - resl_value: the middle bit value that need to resolve
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_set_res_bytem_value(uint32_t resm_value);

/**
 * @brief     the high bit byte that SBC decoder need to resolve 
 *
 * This API resolve the high bit byte.
 *
 *
 * @param
 *    - resl_value: the high bit value that need to resolve
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_set_res_byteh_value(uint32_t resh_value);

/**
 * @brief     set sbc decoder bit number
 *
 * This API set sbc decoder bit number.
 *
 *
 * @param
 *    - bit_num: sbc decoder bit number
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_set_sbc_bit_num(uint32_t bit_num);

/**
 * @brief     set sbc decoder scale factor
 *
 * This API set sbc decoder scale factor.
 *
 *
 * @param
 *    - sf: sbc decoder scale factor
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_set_scale_factor(uint32_t sf);

/**
 * @brief     set sbc decoder level
 *
 * This API set sbc decoder level.
 *
 *
 * @param
 *    - level: sbc decoder level
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_set_sbc_level(uint32_t level);

/**
 * @brief     set sbc decoder resloved bit
 *
 * This API set sbc decoder resloved bit.
 *
 *
 * @param
 *    - res_bit: sbc decoder resolved bit
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sbc_decoder_set_sbc_res_bit(uint32_t res_bit);

/**
 * @brief     start sbc decoding
 *
 * This API enable the sbc decoder function.
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
bk_err_t bk_sbc_decoder_start_decode(void);

/**
 * @brief     get sbc decoder status
 *
 * This API get sbc decoder status to check whether is decoding.
 *
 *
 * @param
 *    - sbc: sbc decoder context pointer
 *
 *
 * @return
 *    - the status of sbc decoder enable
 */
uint32_t bk_sbc_decoder_get_decode_enable_value(void);

/**
 * @brief     get pcm data by sbc decoder
 *
 * This API get pcm data.
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
bk_err_t bk_sbc_decoder_get_pcm_data(void);


