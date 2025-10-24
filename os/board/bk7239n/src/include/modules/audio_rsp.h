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

#ifndef __AUDIO_RSP_H__
#define __AUDIO_RSP_H__
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

/**
 * @brief RSP API
 * @defgroup RSP API group
 * @{
 */

/**
 * @brief     Init audio resample
 *
 * @param cfg audio resample configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_rsp_init(aud_rsp_cfg_t cfg);

/**
 * @brief     Deinit audio resample
 *
 * @param cfg audio resample configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_rsp_deinit(void);

/**
 * @brief     Process audio resample
 *
 * @param in_addr the address of data need to resample
 * @param in_len the address of data need to resample
 * @param out_addr the address of data output
 * @param out_len the length of data processed by resample
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_rsp_process(int16_t *in_addr, uint32_t *in_len, int16_t *out_addr, uint32_t *out_len);

/**
 * @}
 */

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__AUDIO_RSP_H__

