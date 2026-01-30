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

#include <common/bk_include.h>
#include "bk_icu.h"
#include "bk_drv_model.h"
#include "bk_uart.h"
#include "bk_arm_arch.h"
#include "bk_sys_ctrl.h"
#include "reset_reason.h"
#include <components/log.h>
#include "aon_pmu_hal.h"
#include "driver/gpio.h"

#define TAG "init"
#define DISPLAY_START_TYPE_STR 1

static uint32_t s_start_type;
static uint32_t s_misc_value_save;
static uint32_t s_mem_value_save;

// uint32_t bk_misc_get_reset_reason(void)
// {
// 	return s_start_type;
// }

void persist_memory_init(void)
{
	*((volatile uint32_t *)(PERSIST_MEMORY_ADDR)) = (uint32_t)CRASH_ILLEGAL_JUMP_VALUE;
}

uint32_t persist_memory_get(void)
{
	return (*((volatile uint32_t *)(PERSIST_MEMORY_ADDR)));
}

bool persist_memory_is_lost(void)
{
	if ((uint32_t)CRASH_ILLEGAL_JUMP_VALUE == persist_memory_get())
		return false;
	else
		return true;
}

static char *misc_get_start_type_str(uint32_t start_type)
{
#if DISPLAY_START_TYPE_STR
	switch (start_type) {
	case RESET_SOURCE_POWERON:
		return "power on";

	case RESET_SOURCE_REBOOT:
		return "software reboot";

	case RESET_SOURCE_WATCHDOG:
		return "interrupt watchdog";

	case RESET_SOURCE_DEEPPS_GPIO:
		return "deep sleep gpio";

	case RESET_SOURCE_DEEPPS_RTC:
		return "deep sleep rtc";

	case RESET_SOURCE_DEEPPS_TOUCH:
		return "deep sleep touch";

	case RESET_SOURCE_CRASH_ILLEGAL_JUMP:
		return "illegal jump";

	case RESET_SOURCE_CRASH_UNDEFINED:
		return "undefined";

	case RESET_SOURCE_CRASH_PREFETCH_ABORT:
		return "prefetch abort";

	case RESET_SOURCE_CRASH_DATA_ABORT:
		return "data abort";

	case RESET_SOURCE_CRASH_UNUSED:
		return "unused";

	case RESET_SOURCE_CRASH_ILLEGAL_INSTRUCTION:
		return "illegal instruction";

	case RESET_SOURCE_CRASH_MISALIGNED:
		return "misaligned";

	case RESET_SOURCE_CRASH_ASSERT:
		return "assert";

	case RESET_SOURCE_DEEPPS_USB:
		return "deep sleep usb";

	case RESET_SOURCE_SUPER_DEEP:
		return "super deep sleep";

	case RESET_SOURCE_NMI_WDT:
		return "nmi watchdog";

	case RESET_SOURCE_HARD_FAULT:
		return "hard fault";

	case RESET_SOURCE_MPU_FAULT:
		return "mpu fault";

	case RESET_SOURCE_BUS_FAULT:
		return "bus fault";

	case RESET_SOURCE_USAGE_FAULT:
		return "usage fault";

	case RESET_SOURCE_SECURE_FAULT:
		return "secure fault";

	case RESET_SOURCE_DEFAULT_EXCEPTION:
		return "default exception";

	case RESET_SOURCE_OTA_REBOOT:
		return "ota reboot";

	case RESET_SOURCE_BROWN_OUT:
		return "brown out";

	case RESET_SOURCE_FASTBOOT_POWER_ON:
		return "fastboot power on";

	case RESET_SOURCE_UNKNOWN:
	default:
		// Chip power on the value of start address may not always be 0
		// return "unknown";
		return "power on";
	}
#else
	return "";
#endif
}

void show_reset_reason(void)
{
	//BK_LOGI(TAG, "reason - %s\r\n", misc_get_start_type_str(s_start_type));
	BK_LOGI(TAG, "reason - %d\r\n", s_start_type);
	if(RESET_SOURCE_DEEPPS_GPIO == s_start_type)
	{
#if defined(CONFIG_DEEP_PS)
		BK_LOGI(TAG, "by gpio - %d\r\n", bk_misc_wakeup_get_gpio_num());
#else
#ifdef CONFIG_GPIO_WAKEUP_SUPPORT
		BK_LOGI(TAG, "by gpio - %d\r\n", bk_gpio_get_wakeup_gpio_id());
#endif
#endif
	}
	BK_LOGI(TAG, "regs - %x, %x, %x\r\n", s_start_type, s_misc_value_save, s_mem_value_save);
}

uint32_t reset_reason_deep_sleep_check(void)
{
	uint32_t misc_value = 0;

	if(s_misc_value_save != RESET_SOURCE_SUPER_DEEP)
		return misc_value;

	misc_value = aon_pmu_hal_get_wakeup_source();
	switch (misc_value)
	{
	case 0x1: // gpio
		misc_value = RESET_SOURCE_DEEPPS_GPIO;
		break;
	case 0x2: // rtc
		misc_value = RESET_SOURCE_DEEPPS_RTC;
		break;
	case 0x10: // touch
		misc_value = RESET_SOURCE_DEEPPS_TOUCH;
		break;
	case 0x20: // usb
		misc_value = RESET_SOURCE_DEEPPS_USB;
		break;

	default:
		misc_value = 0;
		break;
	}

	return misc_value;
}

uint32_t reset_reason_init(void)
{
	uint32_t misc_value;

	if (s_start_type) {
		return s_start_type;
	}

	misc_value = aon_pmu_hal_get_reset_reason();
	s_misc_value_save = misc_value;

	// check if wakeup from deep sleep
	misc_value = reset_reason_deep_sleep_check();
	if (misc_value) {
		s_start_type = misc_value;
	} else {
		s_start_type = s_misc_value_save;
	}
	bk_misc_set_reset_reason(RESET_SOURCE_POWERON);

	return s_start_type;
}

uint32_t bk_misc_get_reset_reason(void)
{
	uint32_t misc_value;

	if (s_start_type) {
		if (s_start_type == 0x7f)
		{
			s_start_type += 0x80;
		}
		
		return s_start_type;
	}

	misc_value = aon_pmu_hal_get_reset_reason();
	if (misc_value == 0x7f)
	{
		misc_value += 0x80;
	}
	

    return misc_value;
}

void bk_misc_set_reset_reason(uint32_t type)
{
#if defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
	if (type == RESET_SOURCE_POWERON || type == RESET_SOURCE_WATCHDOG) {
		// aon watchdog do not have other chance to write into r7b
		aon_pmu_hal_set_reset_reason(type, true);
	} else {
		aon_pmu_hal_set_reset_reason(type, true);
	}
#else
	aon_pmu_hal_set_reset_reason(type);
#endif
}


