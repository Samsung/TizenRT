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

#ifndef __AUDIO_NS_H__
#define __AUDIO_NS_H__
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @brief NS API
 * @defgroup Audio API group
 * @{
 */

/**
 * @brief     Init audio noise suppression
 *
 * @param frame_size_20ms audio frame size of 20ms
 * @param samp_rate audio sample rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_ns_init(int     frame_size_20ms, int samp_rate);

/**
 * @brief     Deinit audio noise suppression
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_ns_deinit(void);

/**
 * @brief     Process audio data
 *
 * @param in_addr the input and output address of data need to process, data is mono and 16 bits
 *
 * @return
 *    - 0: noise/silence
 *    - 1: speech.
 */
int bk_aud_ns_process(int16_t *in_addr);

/**
 * @brief     Set suppression value
 *
 * @param value the value of start
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_ns_set_suprs(int value);

/**
 * @}
 */

#ifdef  __cplusplus
}
#endif

#endif//__AUDIO_NS_H__

