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

#include "hal_config.h"
#include "calendar_hw.h"
#include "calendar_ll.h"
#include <driver/hal/hal_calendar_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	calendar_hw_t *hw;
	calendar_unit_t id;
} calendar_hal_t;

#define calendar_hal_enable(hal) calendar_ll_enable((hal)->hw)
#define calendar_hal_disable(hal) calendar_ll_disable((hal)->hw)
#define calendar_hal_set_init_value(hal, val) calendar_ll_set_init_value((hal)->hw, val)
#define calendar_hal_reset_config_to_default(hal) calendar_ll_reset_config_to_default((hal)->hw)
#define calendar_hal_get_second_cnt(hal) calendar_ll_get_second_cnt((hal)->hw)
#define calendar_hal_get_us_cnt(hal) calendar_ll_get_us_cnt((hal)->hw)

bk_err_t calendar_hal_init(calendar_hal_t *hal);

#if CFG_HAL_DEBUG_CALENDAR
void calendar_struct_dump(void);
#else
#define calendar_struct_dump()
#endif

#ifdef __cplusplus
}
#endif

