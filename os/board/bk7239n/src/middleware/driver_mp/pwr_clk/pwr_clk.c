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
#include <driver/pwr_clk.h>
#if CONFIG_MAILBOX
#include <driver/mailbox_channel.h>
#endif
#include <modules/pm.h>
#include "sys_driver.h"
#if CONFIG_PSRAM
#include <driver/psram.h>
#endif
#include <os/mem.h>
#include "sys_types.h"
#include <driver/aon_rtc.h>
#include <os/os.h>
#include <driver/psram.h>
#include <components/system.h>
#include <common/bk_kernel_err.h>
#include "aon_pmu_hal.h"
#if CONFIG_WDT_EN
#include "wdt_driver.h"
#endif

static pm_lpo_src_e s_lpo_src = PM_LPO_SRC_X32K;

int bk_clk_32k_customer_config_set(pm_lpo_src_e lpo_src)
{
	s_lpo_src = lpo_src;
	return 0;
}

/* Flash version for early boot (in FLASH) */
__FLASH_BOOT_CODE pm_lpo_src_e bk_clk_32k_customer_config_get_flash(void)
{
#if CONFIG_LPO_MP_A_FORCE_USE_EXT32K
	uint32_t chip_id = aon_pmu_hal_get_chipid();
	if ((chip_id & PM_CHIP_ID_MASK) == (PM_CHIP_ID_MP_A & PM_CHIP_ID_MASK))
	{
		return PM_LPO_SRC_X32K;
	}
	else
	{
		#if CONFIG_AUTO_DETECT_32K_SOURCE
			return s_lpo_src;
		#else
			#if CONFIG_EXTERN_32K
				return PM_LPO_SRC_X32K;
			#elif CONFIG_LPO_SRC_26M32K
				return PM_LPO_SRC_DIVD;
			#else
				return PM_LPO_SRC_ROSC;
			#endif //CONFIG_EXTERN_32K
		#endif //CONFIG_AUTO_DETECT_32K_SOURCE
	}
#else
#if CONFIG_AUTO_DETECT_32K_SOURCE
	return s_lpo_src;
#else
	#if CONFIG_EXTERN_32K
		return PM_LPO_SRC_X32K;
	#elif CONFIG_LPO_SRC_26M32K
		return PM_LPO_SRC_DIVD;
	#else
		return PM_LPO_SRC_ROSC;
	#endif
#endif//CONFIG_AUTO_DETECT_32K_SOURCE
#endif//CONFIG_LPO_MP_A_FORCE_USE_EXT32K
	return PM_LPO_SRC_ROSC;
}

pm_lpo_src_e bk_clk_32k_customer_config_get(void)
{
#if CONFIG_LPO_MP_A_FORCE_USE_EXT32K
	uint32_t chip_id = aon_pmu_hal_get_chipid();
	if ((chip_id & PM_CHIP_ID_MASK) == (PM_CHIP_ID_MP_A & PM_CHIP_ID_MASK))
	{
		return PM_LPO_SRC_X32K;
	}
	else
	{
		#if CONFIG_AUTO_DETECT_32K_SOURCE
			return s_lpo_src;
		#else
			#if CONFIG_EXTERN_32K
				return PM_LPO_SRC_X32K;
			#elif CONFIG_LPO_SRC_26M32K
				return PM_LPO_SRC_DIVD;
			#else
				return PM_LPO_SRC_ROSC;
			#endif //CONFIG_EXTERN_32K
		#endif //CONFIG_AUTO_DETECT_32K_SOURCE
	}
#else
#if CONFIG_AUTO_DETECT_32K_SOURCE
	return s_lpo_src;
#else
	#if CONFIG_EXTERN_32K
		return PM_LPO_SRC_X32K;
	#elif CONFIG_LPO_SRC_26M32K
		return PM_LPO_SRC_DIVD;
	#else
		return PM_LPO_SRC_ROSC;
	#endif
#endif//CONFIG_AUTO_DETECT_32K_SOURCE
#endif//CONFIG_LPO_MP_A_FORCE_USE_EXT32K
	return PM_LPO_SRC_ROSC;
}