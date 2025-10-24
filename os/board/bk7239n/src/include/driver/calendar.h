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
#include <driver/calendar_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the calendar driver
 *
 * This API init the resoure common:
 *   - Init calendar driver control memory
 *
 * @attention 1. This API should be called before any other calendar APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_calendar_driver_init(void);

/**
 * @brief     Deinit the calendar driver
 *
 * This API free all resource related to calendar and disable calendar.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_calendar_driver_deinit(void);

/**
 * @brief     Set the calendar init value
 *
 * @param init_val calendar init value
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_calendar_set_init_value(uint32_t init_val);

/**
 * @brief     Get the calendar time us
 *
 * @return the calendar time us
 */
uint64_t bk_calendar_get_time_us(void);

#ifdef __cplusplus
}
#endif

