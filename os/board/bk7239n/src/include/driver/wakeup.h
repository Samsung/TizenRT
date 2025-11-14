// Copyright 2020-2025 Beken
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
#include <driver/wakeup_types.h>
#include <sys_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/* @brief Overview about this API header
 *
 */

/**
 * @brief WAKEUP API
 * @defgroup bk_api_wakeup WAKEUP API group
 * @{
 */

/**
 * @brief     Init the WAKEUP driver
 *
 * This API init the resoure common:
 *   - Init WAKEUP driver control memory
 *   - Enable WAKEUP interrupt
 *
 * @attention 1. This API should be called before any other WAKEUP APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_wakeup_driver_init(void);

/**
 * @brief     Deinit the WAKEUP driver
 *
 * This API free all resource related to WAKEUP and disable the interrupt.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_wakeup_driver_deinit(void);

/**
 * @brief     Enable the specific wakeup source
 *
 * This API enable the input WAKEUP source.
 *
 * @param
 * -type:     wakeup source type
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_wakeup_driver_source_set(wakeup_source_t type);

/**
 * @brief     Disable the specific wakeup source
 *
 * This API disable the input WAKEUP source.
 *
 * @param
 * -type:     wakeup source type
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_wakeup_driver_source_clr(wakeup_source_t type);

/**
 * @brief     Register WAKEUP callback function
 *
 * This API register cb function for the input WAKEUP source.
 *
 * @param
 * -type:     wakeup source type
 * -wkup_cfg: wakeup callback function and args
 * -src_cfg:  wakeup source configuration function and args
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_wakeup_register_cb(wakeup_source_t type, wakeup_cb_conf_t wkup_cfg, wakeup_cb_conf_t src_cfg);

/**
 * @brief     Unregister WAKEUP callback function
 *
 * This API unregister cb function for the input WAKEUP source.
 *
 * @param
 * -wakeup_source_t:wakeup source type
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_wakeup_unregister_cb(wakeup_source_t type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

