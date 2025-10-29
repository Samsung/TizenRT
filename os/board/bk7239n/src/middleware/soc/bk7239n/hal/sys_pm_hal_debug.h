// Copyright 2022-2025 Beken
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

#include <soc/soc.h>

#define PM_HAL_LOGD HAL_LOGD

#define LV_DEBUG_GPIO 18

#if defined(CONFIG_PM_HAL_DEBUG)
void sys_hal_debug_get_sys_regs_before_sleep(void);
void sys_hal_debug_get_sys_regs_after_waked(void);
void sys_hal_debug_dump_sys_regs(void);
void sys_hal_debug_check_sys_regs(void);
void sys_hal_debug_gpio_up(int id);
void sys_hal_debug_gpio_down(int id);
#else
#define sys_hal_debug_get_sys_regs_before_sleep()
#define sys_hal_debug_get_sys_regs_after_waked()
#define sys_hal_debug_dump_sys_regs()
#define sys_hal_debug_check_sys_regs()
#define sys_hal_debug_gpio_up(id)
#define sys_hal_debug_gpio_down(id)
#endif

