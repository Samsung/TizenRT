

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
#include "pm_debug.h"
// #include "bk_pm_internal_api.h"

#define PM_DEBUG_CTRL_STATE                    (8)
#define PM_DEBUG_SYS_REG_BASE                  (SOC_SYSTEM_REG_BASE)
#define PM_DEBUG_PMU_REG_BASE                  (SOC_AON_PMU_REG_BASE)


static uint8_t  s_debug_en               = 0;

static uint32_t s_before_low_vol_pd     =  0;
static uint32_t s_before_low_vol_lpo    =  0;
static uint32_t s_before_low_vol_psram  =  0;
static uint32_t s_after_low_vol_pd      =  0;
static uint32_t s_after_low_vol_lpo     =  0;
static uint32_t s_after_low_vol_psram   =  0;


static void pm_pmu_dump(void);


/*=========================DEBUG/TEST CTRL START========================*/
void pm_debug_dump()
{
	pm_power_dump();

	pm_core_dump();

	pm_pmu_dump();

	pm_debug_module_state();
}

uint32_t pm_debug_mode()
{
	return s_debug_en;
}

void pm_debug_ctrl(uint32_t debug_en)
{
	s_debug_en = debug_en;

	if (debug_en == PM_DEBUG_CTRL_STATE)
		pm_debug_dump();
}

static void pm_pmu_dump(void)
{
	uint32_t pd = REG_READ(PM_DEBUG_SYS_REG_BASE+0x10*4);
	uint32_t lpo = REG_READ(PM_DEBUG_PMU_REG_BASE+0x41*4);
	pm_wakeup_source_e wkup_src = bk_pm_exit_low_vol_wakeup_source_get();

	LOGI("pm power,pmu[0x%x][0x%x][%d],[0x%x][0x%x][0x%x],[0x%x][0x%x][0x%x]\r\n", pd, lpo, wkup_src,
											s_before_low_vol_pd, s_before_low_vol_lpo, s_before_low_vol_psram,
											s_after_low_vol_pd, s_after_low_vol_lpo, s_after_low_vol_psram);
}
/*=========================DEBUG/TEST CTRL END========================*/

/*=========================DEBUG HOOK START========================*/
void pm_debug_lowvol_sleep_hook(void)
{
	s_before_low_vol_pd  = REG_READ(PM_DEBUG_SYS_REG_BASE+0x10*4);
	s_before_low_vol_lpo = REG_READ(PM_DEBUG_PMU_REG_BASE+0x41*4);
	s_before_low_vol_psram = REG_READ(PM_DEBUG_SYS_REG_BASE+0x4d*4);
}

void pm_debug_low_vol_wakeup_hook(void)
{
	s_after_low_vol_pd  = REG_READ(PM_DEBUG_SYS_REG_BASE+0x10*4);
	s_after_low_vol_lpo = REG_READ(PM_DEBUG_PMU_REG_BASE+0x41*4);
	s_after_low_vol_psram = REG_READ(PM_DEBUG_SYS_REG_BASE+0x4d*4);
}
/*=========================DEBUG HOOK END========================*/