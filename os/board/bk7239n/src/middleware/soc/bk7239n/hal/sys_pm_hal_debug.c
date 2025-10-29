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

#include "sys_pm_hal_debug.h"
#include "sys_ll.h"

#if defined(CONFIG_PM_HAL_DEBUG)

static const uint8_t s_sys_regs[] = {
	8, 9, 0xa, 0xe, 0xf, 0x10, 0x12, 0x20, 0x21, 0x22, 0x23,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
	0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e
};

#define SYS_REG_NUM (sizeof(s_sys_regs) / sizeof(s_sys_regs[0]))
static uint32_t s_sys_regs_before_sleep[SYS_REG_NUM] = {0};
static uint32_t s_sys_regs_after_sleep[SYS_REG_NUM] = {0};

void sys_hal_debug_dump_sys_regs(void)
{
}

void sys_hal_debug_get_sys_regs(uint32_t *regs)
{
	uint8_t reg_id;

	for (int i = 0; i < SYS_REG_NUM; i++) {
		reg_id = s_sys_regs[i];

		if (reg_id >= 0x40) {
			regs[i] = sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (reg_id << 2));
		} else {
			regs[i] = REG_READ(SOC_SYS_REG_BASE + (reg_id << 2));
		}
		PM_HAL_LOGD("sys_r0x%02x: %x\r\n", reg_id, regs[i]);
	}
}

void sys_hal_debug_get_sys_regs_before_sleep(void)
{
	sys_hal_debug_get_sys_regs(s_sys_regs_before_sleep);
}

void sys_hal_debug_get_sys_regs_after_waked(void)
{
	sys_hal_debug_get_sys_regs(s_sys_regs_after_sleep);
}

void sys_hal_debug_check_sys_regs(void)
{
	uint8_t reg_id;

	for (int i = 0; i < SYS_REG_NUM; i++) {
		reg_id = s_sys_regs[i];

		if (s_sys_regs_before_sleep[i] != s_sys_regs_after_sleep[i]) {
			PM_HAL_LOGD("sys_r0x%02x mismatch: %x != %x\r\n",
				reg_id, s_sys_regs_before_sleep[i], s_sys_regs_after_sleep[i]);
		}
	}
}

void sys_hal_debug_gpio_up(int id)
{
	GPIO_UP(id);
}

void sys_hal_debug_gpio_down(int id)
{
	GPIO_DOWN(id);
}

#endif
