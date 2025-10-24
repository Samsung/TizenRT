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
#include <driver/wdt_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/* @brief Overview about this API header
 *
 */

/**
 * @brief WDT API
 * @defgroup bk_api_wdt WDT API group
 * @{
 */

/**
 * @brief     Init the WDT driver
 *
 * This API init the resoure common:
 *   - Init WDT driver control memory
 *
 * @attention 1. This API should be called before any other WDT APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_wdt_driver_init(void);

/**
 * @brief     Deinit the WDT driver
 *
 * This API free all resource related to WDT and power down WDT.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_wdt_driver_deinit(void);

/**
 * @brief     Start the WDT
 *
 * This API start the WDT:
 *  - Power up the WDT
 *  - Init the watch dog timer period
 *
 * @param timeout_ms WDT period
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_WDT_DRIVER_NOT_INIT: WDT driver not init
 *    - BK_ERR_WDT_INVALID_PERIOD: WDT invalid period
 *    - others: other errors.
 */
bk_err_t bk_wdt_start(uint32_t timeout_ms);

/**
 * @brief     Stop the WDT
 *
 * This API stop the WDT:
 *   - Reset all configuration of WDT to default value
 *   - Power down the WDT
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
__attribute__((section(".itcm_sec_code"))) bk_err_t bk_wdt_stop(void);

/**
 * @brief     Feed the WDT
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_wdt_feed(void);

/**
 * @brief   Get feed watchdog time
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
uint32_t bk_wdt_get_feed_time(void);

/**
 * @brief  set feed watchdog time
 *
 * @return
 *    - NULL
 *
 */
void bk_wdt_set_feed_time(uint32_t dw_set_time);

/**
 * @brief   Get wdt driver init flag
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bool bk_wdt_is_driver_inited(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

