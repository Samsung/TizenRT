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

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief    Get calibrated 32k clock frequency
 *
 * This API 26m digital clock measurement 32k analog clock.
 *
 * @return
 *    - alibrated 32k clock frequency value.
 */
double bk_rosc_32k_get_freq(void);

/**
 * @brief    Get calibrated 32k clock ppm
 *
 * This API tells the current clock ppm of rc32k.
 *
 * @return
 *    - alibrated 32k clock ppm value.
 */
bk_err_t bk_rosc_32k_get_ppm(void);

/**
 * @brief    Perform 32k frequency measurement
 *
 * This API 26m digital clock measurement 32k analog clock.
 *
 * @param count number of clock cycles
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_rosc_32k_ckest_prog(uint32_t count);

/**
 * @brief    Perform 32k frequency calibration
 *
 * This API start a thread to calibrate the rosc clock to 32K which will cost about 100ms to finish.
 * If the calibration fail, rc32k will be set to trig mode.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_rosc_32k_calib(void);

/**
 * @brief    Get tick diff for rtc tick compensation
 *
 * This API tells the current tick diff for compensation.
 *
 * @param tick current tick value
 *
 * @return
 *    - tick diff value.
 */
int64_t bk_rosc_32k_get_tick_diff(uint64_t tick);

/**
 * @brief    Get time diff for mac time compensation
 *
 * This API tells the current time diff for compensation.
 *
 * @return
 *    - time diff value.
 */
int64_t bk_rosc_32k_get_time_diff(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

