// Copyright 2020-2022 Beken
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

#define BK_ERR_AON_WDT_INVALID_PERIOD     (BK_ERR_AON_WDT_BASE - 1) /*< AON_WDT invalid period */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

