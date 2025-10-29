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
#include <driver/pwr_clk.h>
#include <driver/psram.h>
#include <os/mem.h>
#if CONFIG_WDT_EN
#include "wdt_driver.h"
#endif

#include "pm_psram.h"
#include "pm_debug.h"


static uint32_t s_pm_psram_ctrl_state     = 0;
#if (CONFIG_CPU_CNT > 1)
static uint32_t s_pm_cp1_psram_malloc_count_state       = 0;
#endif

bk_err_t bk_pm_module_vote_psram_ctrl(pm_power_psram_module_name_e module,pm_power_module_state_e power_state)
{
	bk_err_t ret = BK_OK;
	GLOBAL_INT_DECLARATION();
	//BK_LOGD(NULL, "%s %d %d 0x%x\r\n",__func__, module, power_state,s_pm_psram_ctrl_state);
    if(power_state == PM_POWER_MODULE_STATE_ON)//power on
    {
        GLOBAL_INT_DISABLE();
        s_pm_psram_ctrl_state |= 0x1 << (module);
        GLOBAL_INT_RESTORE();
		ret = bk_psram_init();
		if(ret != BK_OK)
		{
			LOGE("Psram_I err0:%d",ret);
			bk_psram_deinit();
			ret = bk_psram_init();
			if(ret != BK_OK)
			{
				LOGE("Psram_I err1:%d",ret);
				bk_psram_deinit();
				ret = bk_psram_init();
				if(ret != BK_OK)
				{
					LOGE("Psram_I err2:%d",ret);
					#if CONFIG_WDT_EN
					bk_wdt_force_reboot();//try 3 times, if fail ,reboot.
					#endif
				}
			}
		}
	}
    else //power down
    {
		if(s_pm_psram_ctrl_state&(0x1 << (module)))
		{
			GLOBAL_INT_DISABLE();
			s_pm_psram_ctrl_state &= ~(0x1 << (module));
			GLOBAL_INT_RESTORE();
			if(0x0 == s_pm_psram_ctrl_state)
			{
				bk_psram_deinit();
			}
		}
	}
	return BK_OK;
}

bk_err_t pm_cp1_psram_malloc_count_state_set(uint32_t value)
{
	#if (CONFIG_CPU_CNT > 1)
	s_pm_cp1_psram_malloc_count_state = value;
	#endif
	return BK_OK;
}

bk_err_t pm_cp1_psram_malloc_state_get()
{
	#if (CONFIG_CPU_CNT > 1)
	s_pm_cp1_psram_malloc_count_state = bk_pm_get_cp1_psram_malloc_count();
	#endif
	return BK_OK;
}
#define bk_psram_heap_get_used_count() 0
bk_err_t pm_psram_malloc_state_and_power_ctrl()
{
#if CONFIG_PSRAM_AS_SYS_MEMORY
	uint32_t cp0_psram_malloc_count = 0;
	uint32_t cp1_psram_malloc_count = 0;
	/*get the cp1 psram malloc count*/
	#if (CONFIG_CPU_CNT > 1)
	cp1_psram_malloc_count = s_pm_cp1_psram_malloc_count_state;
	if (pm_debug_mode() == 64)
	{
		if(s_pm_cp1_psram_malloc_count_state > 0)
		{
			LOGI("CP1 psram malloc count[%d] > 0\r\n",cp1_psram_malloc_count);
			LOGI("Power consumption will get higher,free them\r\n");

			bk_pm_dump_cp1_psram_malloc_info();
		}
	}
	#endif
	/*get the cp0 psram malloc count*/
	cp0_psram_malloc_count = bk_psram_heap_get_used_count();
	if (pm_debug_mode() == 64)
	{
		if(cp0_psram_malloc_count > 0)
		{
			LOGI("CP0 psram malloc count[%d] > 0\r\n",cp0_psram_malloc_count);
			LOGI("Power consumption will get higher,free them\r\n");

			bk_psram_heap_get_used_state();
		}
	}
	if((cp0_psram_malloc_count == 0)&&(cp1_psram_malloc_count == 0))
	{
		//bk_pm_module_vote_psram_ctrl(PM_POWER_PSRAM_MODULE_NAME_AS_MEM,PM_POWER_MODULE_STATE_OFF);
		//bk_pm_module_vote_power_ctrl(POWER_SUB_MODULE_NAME_BAKP_PSRAM, PM_POWER_MODULE_STATE_OFF);
	}

#endif
	return BK_OK;
}

void pm_debug_psram()
{
	uint32_t cp0_psram_malloc_count = 0;

	/*get the cp1 psram malloc count*/
	#if (CONFIG_CPU_CNT > 1)
	uint32_t cp1_psram_malloc_count = s_pm_cp1_psram_malloc_count_state;

	if(cp1_psram_malloc_count > 0)
	{
		LOGI("CP1 psram malloc count[%d] > 0\r\n",cp1_psram_malloc_count);
		LOGI("Power consumption will get higher, please free them\r\n");
		bk_pm_dump_cp1_psram_malloc_info();
	}
	#endif

	/*get the cp0 psram malloc count*/
	cp0_psram_malloc_count = bk_psram_heap_get_used_count();
	if(cp0_psram_malloc_count > 0)
	{
		LOGI("CP0 psram malloc count[%d] > 0\r\n",cp0_psram_malloc_count);
		LOGI("power consumption will get higher,free them\r\n");
		bk_psram_heap_get_used_state();
	}
}

void pm_debug_psram_state()
{
	LOGI("pm_psram:0x%x 0x%x\r\n",s_pm_psram_ctrl_state,bk_psram_heap_init_flag_get());
}

