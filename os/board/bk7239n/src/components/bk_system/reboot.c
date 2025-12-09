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
#include <components/log.h>
#include <common/bk_err.h>
#include <components/system.h>
#include <driver/wdt.h>
#include "bk_misc.h"
#include "reset_reason.h"
#include "bk_wifi_types.h"
#include "bk_wifi.h"
#include "aon_pmu_driver.h"
#include "wdt_driver.h"
#include <modules/pm.h>
#include "flash_types.h"
#include <tinyara/reboot_reason.h>

#define TAG "sys"



void bk_reboot_ex(uint32_t reset_reason)
{
#if defined(CONFIG_SYS_CPU0)
	static uint32_t entry_cnt = 0;
	if(entry_cnt == 0)	//first time come here, or force reboot:avoid these codes cause system abnormal.
	{
		entry_cnt++;

		/* Only write reset reason if not already set by higher-level code */
		uint32_t current_reason = bk_misc_get_reset_reason();
		if ((current_reason == REBOOT_REASON_INITIALIZED || current_reason == 0) && 
		    (reset_reason < RESET_SOURCE_UNKNOWN)) {
			bk_misc_set_reset_reason(reset_reason);
		}


		// BK_LOGI(TAG, "bk_reboot\r\n");
		delay_ms(100); //add delay for bk_writer BEKEN_DO_REBOOT cmd
		bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT,PM_CPU_FRQ_60M);
#if defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT)
		{
			/* 
			 * NOTES:special requirements
			 * Some customers system reboot, needs to reserve the network time,
			 * maybe after reboot, the network can't work at once.
			 * so before reboot, save the network time to flash.
			 */
			extern bk_err_t aon_rtc_enter_reboot(void);
			aon_rtc_enter_reboot();
		}
#endif

		// BK_LOGI(TAG, "wdt reboot\r\n");
		rtos_disable_int();
	}
	bk_flash_set_line_mode(FLASH_LINE_MODE_TWO);
	bk_wdt_force_reboot();
#endif //#if CONFIG_SYS_CPU0
}

void bk_reboot(void)
{
	bk_reboot_ex(REBOOT_REASON_INITIALIZED);
}

void bk_reboot_reset_reason(void)
{
#if defined(CONFIG_SYS_CPU0)
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT,PM_CPU_FRQ_60M);
	rtos_disable_int();

	bk_flash_set_line_mode(FLASH_LINE_MODE_TWO);
	bk_wdt_force_reboot();
#endif //#if CONFIG_SYS_CPU0
}
