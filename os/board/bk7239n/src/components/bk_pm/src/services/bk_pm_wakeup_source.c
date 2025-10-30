// Copyright 2021-2025 Beken
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
#include <modules/pm.h>
#include <sys_types.h>
#include "sys_driver.h"
#include "aon_pmu_driver.h"
#include <os/mem.h>
#include "pm_debug.h"
#include "int_types.h"
#include "pm_wakeup_source.h"
#include "pm_sleep.h"

/*=====================DEFINE SECTION START=====================*/
#define PM_WAKEUP_SOURCE_MARK                                (WAKEUP_SOURCE_MARK)

/*=====================DEFINE SECTION END=====================*/

/*=====================VARIABLE SECTION START=================*/
static uint32_t s_pm_wakeup_source                          = 0;
static pm_wakeup_source_e s_pm_exit_low_vol_wakeup_source   = PM_WAKEUP_SOURCE_INT_NONE;
static pm_wakeup_source_e s_pm_exit_deepsleep_wakeup_source = PM_WAKEUP_SOURCE_INT_NONE;

static touch_wakeup_param_t s_touch_wakeup_param;
static uint64_t s_normal_sleep_wakeup_irq                   = 0;
static icu_int_src_t s_normal_sleep_wakeup_irq_id           = INT_SRC_NONE;
/*=====================VARIABLE SECTION END=================*/

/*================FUNCTION DECLARATION SECTION START========*/

/*================FUNCTION DECLARATION SECTION END========*/

bk_err_t bk_pm_wakeup_source_set(pm_wakeup_source_e wakeup_source, void *source_param)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_pm_wakeup_source |= 0x1 << wakeup_source;
	if (source_param == NULL)
	{
#if CONFIG_WAKEUP
		bk_wakeup_driver_source_set(wakeup_source);
#endif
		GLOBAL_INT_RESTORE();
		return BK_OK;
	}
	switch (wakeup_source)
	{
	case PM_WAKEUP_SOURCE_INT_GPIO:
		break;

	case PM_WAKEUP_SOURCE_INT_RTC:
		break;

	case PM_WAKEUP_SOURCE_INT_SYSTEM_WAKE: // wifi/bt
		break;

	case PM_WAKEUP_SOURCE_INT_USBPLUG:
		break;

	case PM_WAKEUP_SOURCE_INT_TOUCHED:
		os_memcpy(&s_touch_wakeup_param, (touch_wakeup_param_t *)source_param, sizeof(touch_wakeup_param_t));
		break;

	default:
		break;
	}
	GLOBAL_INT_RESTORE();
	return BK_OK;
}

pm_wakeup_source_e bk_pm_wakeup_source_get(void)
{
	return s_pm_wakeup_source;
}

void pm_touched_wakeup_low_voltage()
{
	sys_drv_touch_wakeup_enable(s_touch_wakeup_param.touch_channel);
}

void pm_rtc_wakeup_deep_sleep()
{
	aon_pmu_drv_set_wakeup_source(WAKEUP_SOURCE_INT_RTC);
}

void pm_touched_wakeup_deep_sleep()
{
}

bk_err_t pm_wakeup_from_deepsleep_handle()
{
	uint32_t pmu_state = 0;
	if (aon_pmu_drv_reg_get(PMU_REG2) & BIT(BIT_SLEEP_FLAG_DEEP_SLEEP))
	{
		pmu_state = 0;
		pmu_state = aon_pmu_drv_reg_get(PMU_REG2);
		pmu_state &= ~(BIT(BIT_SLEEP_FLAG_DEEP_SLEEP));
		aon_pmu_drv_reg_set(PMU_REG2, pmu_state);
	}
	return BK_OK;
}

void pm_deep_sleep_wakeup_source_set()
{
	uint32_t pmu_state = 0;
	if (aon_pmu_drv_reg_get(PMU_REG2) & BIT(BIT_SLEEP_FLAG_DEEP_SLEEP))
	{
		pmu_state = 0;
		pmu_state = aon_pmu_drv_reg_get(PMU_REG0x71);
		pmu_state = (pmu_state >> 20) & PM_WAKEUP_SOURCE_MARK;

		switch (pmu_state)
		{
		case 0x1: // gpio
			bk_misc_set_reset_reason(RESET_SOURCE_DEEPPS_GPIO);
			s_pm_exit_deepsleep_wakeup_source = PM_WAKEUP_SOURCE_INT_GPIO;
			break;
		case 0x2: // rtc
			bk_misc_set_reset_reason(RESET_SOURCE_DEEPPS_RTC);
			s_pm_exit_deepsleep_wakeup_source = PM_WAKEUP_SOURCE_INT_RTC;
			break;
		case 0x10: // bk7256 use touch and bk7236 use usb
			bk_misc_set_reset_reason(RESET_SOURCE_DEEPPS_TOUCH);
			s_pm_exit_deepsleep_wakeup_source = PM_WAKEUP_SOURCE_INT_TOUCHED;
			break;
		case 0x20: // touch
			bk_misc_set_reset_reason(RESET_SOURCE_DEEPPS_TOUCH);
			s_pm_exit_deepsleep_wakeup_source = PM_WAKEUP_SOURCE_INT_TOUCHED;
			break;
		default:
			s_pm_exit_deepsleep_wakeup_source = PM_WAKEUP_SOURCE_INT_NONE;
			break;
		}
	}
}

pm_wakeup_source_e bk_pm_deep_sleep_wakeup_source_get()
{
	return s_pm_exit_deepsleep_wakeup_source;
}

pm_wakeup_source_e bk_pm_exit_low_vol_wakeup_source_get()
{
	return s_pm_exit_low_vol_wakeup_source;
}

bk_err_t bk_pm_exit_low_vol_wakeup_source_set()
{
	uint32_t pmu_state = 0;
	if (aon_pmu_drv_reg_get(PMU_REG2) & BIT(BIT_SLEEP_FLAG_LOW_VOLTAGE))
	{
		pmu_state = 0;
		pmu_state = aon_pmu_drv_reg_get(PMU_REG0x71);
		pmu_state = (pmu_state >> 20) & PM_WAKEUP_SOURCE_MARK;

		switch (pmu_state)
		{
		case 0x1: // gpio
			s_pm_exit_low_vol_wakeup_source = PM_WAKEUP_SOURCE_INT_GPIO;
			break;
		case 0x2: // rtc
			s_pm_exit_low_vol_wakeup_source = PM_WAKEUP_SOURCE_INT_RTC;
			break;
		case 0x4: // WIFI wakeup
			s_pm_exit_low_vol_wakeup_source = PM_WAKEUP_SOURCE_INT_WIFI;
			break;
		case 0x8: // bk7256 use usb and bk7236 use BT wakeup
			s_pm_exit_low_vol_wakeup_source = PM_WAKEUP_SOURCE_INT_BT;
			break;
		case 0x10: // bk7256 use touch and bk7236 use usb wakeup
			s_pm_exit_low_vol_wakeup_source = PM_WAKEUP_SOURCE_INT_TOUCHED;
			break;
		case 0x20: // touch
			s_pm_exit_low_vol_wakeup_source = PM_WAKEUP_SOURCE_INT_TOUCHED;
			break;
		default:
			s_pm_exit_low_vol_wakeup_source = PM_WAKEUP_SOURCE_INT_NONE;
			break;
		}
	}
	return BK_OK;
}

bk_err_t bk_pm_exit_low_vol_wakeup_source_clear()
{
	/*clear the wakeup source*/
	uint32_t pmu_state = 0;
	pmu_state = aon_pmu_drv_reg_get(PMU_REG0x43);
	pmu_state |= (0x1 << 17);
	aon_pmu_drv_reg_set(PMU_REG0x43, pmu_state);
	s_pm_exit_low_vol_wakeup_source = PM_WAKEUP_SOURCE_INT_NONE;
	return BK_OK;
}

__attribute__((section(".iram")))  bk_err_t bk_pm_sleep_wakeup_reason_clear()
{
	s_normal_sleep_wakeup_irq    = 0;
	s_normal_sleep_wakeup_irq_id = 0;
	return BK_OK;
}

__attribute__((section(".iram")))  bk_err_t bk_pm_sleep_wakeup_reason_set(uint64_t wakeup_irq)
{
	s_normal_sleep_wakeup_irq = wakeup_irq;
	for(int i = 0; i < INT_SRC_NONE; i++)
	{
		if(wakeup_irq & (1ULL << i))
		{
			s_normal_sleep_wakeup_irq_id = i;
		}
	}
	return BK_OK;
}

bk_pm_wakeup_reason_e bk_pm_sleep_wakeup_reason_get()
{
	if(s_normal_sleep_wakeup_irq_id != INT_SRC_NONE)
	{
		/*Debug*/
		//LOGI("NS wakeup irq: %d,0x%llx", s_normal_sleep_wakeup_irq_id,s_normal_sleep_wakeup_irq);
	}
	switch (s_normal_sleep_wakeup_irq_id)
	{
		case INT_SRC_ANA_GPIO:
			return BK_PM_WAKEUP_GPIO;
		case INT_SRC_ANA_RTC:
			return BK_PM_WAKEUP_HW_TIMER;
		case INT_SRC_MAC_GENERAL:
			return BK_PM_WAKEUP_WIFI;
		case INT_SRC_BT:
			return BK_PM_WAKEUP_BLE;
		case INT_SRC_UART1:
			return BK_PM_WAKEUP_UART_CONSOLE;
		case INT_SRC_UART2:
			return BK_PM_WAKEUP_UART_TTYS2;
		default:
			return BK_PM_WAKEUP_UNKNOWN;
			break;
	}
	return BK_PM_WAKEUP_UNKNOWN;
}
