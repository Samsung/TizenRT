// Copyright 2021-2022 Beken
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
#include <driver/aon_wdt_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief AON_WDT API
 * @defgroup bk_api_aon_wdt AON_WDT API group
 * @{
 */

/**
 * @brief   Get watchdog period
 *    - return 0 means close aon_wdt
 *
 * @return
 *    - aon_wdt period.
 *    - period 0 means aon_wdt closed.
 */
uint32_t bk_aon_wdt_get_period(void);

/**
 * @brief  set watchdog period
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aon_wdt_set_period(uint32_t timeout_ms);

/**
 * @brief   Stop the AON_WDT
 *
 * This API stop the AON_WDT:
 *   - Reset all configuration of AON_WDT to default value
 *   - Power down the AON_WDT
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aon_wdt_stop(void);

/**
 * @brief     Feed the AON_WDT
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aon_wdt_feed(void);

/**
 * @brief   Get feed watchdog time
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
uint32_t bk_aon_wdt_get_feed_time(void);

/**
 * @brief  set feed watchdog time
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aon_wdt_set_feed_time(uint32_t feed_time);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

