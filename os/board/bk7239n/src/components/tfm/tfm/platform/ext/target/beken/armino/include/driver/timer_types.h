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
//

#pragma once

#include <driver/hal/hal_timer_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Timer APIs
 * @addtogroup bk_api_timer Timer API group
 * @{
 */

/**
 * @brief Timer defines
 * @defgroup bk_api_timer_defs macos
 * @ingroup bk_api_timer
 * @{
 */

#define BK_ERR_TIMER_ID              (BK_ERR_TIMER_BASE - 1) /**< timer id is invalid */
#define BK_ERR_TIMER_NOT_INIT        (BK_ERR_TIMER_BASE - 2) /**< timer driver not init */
#define BK_ERR_TIMER_IS_RUNNING      (BK_ERR_TIMER_BASE - 3) /**< timer id is running */
#define BK_ERR_TIMER_ID_ON_DEFCONFIG     (BK_ERR_TIMER_BASE - 4) /**< timer id is error */

/**
 * @brief Timer interrupt service routine
 */
typedef void (*timer_isr_t)(timer_id_t timer_id);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

