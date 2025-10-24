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
#include <driver/hal/hal_wdt_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief WDT defines
 * @defgroup bk_api_wdt_defs macos
 * @ingroup bk_api_wdt
 * @{
 */
#define BK_ERR_WDT_DRIVER_NOT_INIT    (BK_ERR_WDT_BASE - 1) /**< WDT driver not init */
#define BK_ERR_WDT_NOT_INIT           (BK_ERR_WDT_BASE - 2) /**< WDT not init */
#define BK_ERR_WDT_INVALID_PERIOD     (BK_ERR_WDT_BASE - 3) /**< WDT invalid period */
#define BK_ERR_WDT_DEBUG_DISABLED     (BK_ERR_WDT_BASE - 4) /**< WDT disabled via cmdline flag */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


