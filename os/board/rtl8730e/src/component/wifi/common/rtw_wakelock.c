/**
  ******************************************************************************
  * @file    rtw_wakelock.c
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include <basic_types.h>
#include <os_wrapper.h>
#if CONFIG_PLATFORM_TIZENRT_OS
#include "ameba_tizenrt_pmu.h"
#else
#include <ameba_pmu.h>
#endif //CONFIG_PLATFORM_TIZENRT_OS

void rtw_acquire_wakelock(void)
{
#if CONFIG_PLATFORM_TIZENRT_OS
	/* TIZEN_RT_NOT_REQUIRED */
#else
	if (pmu_yield_os_check()) {
		pmu_acquire_wakelock(PMU_WLAN_DEVICE);
	}
#endif //CONFIG_PLATFORM_TIZENRT_OS
}

void rtw_release_wakelock(void)
{
#if CONFIG_PLATFORM_TIZENRT_OS
	/* TIZEN_RT_NOT_REQUIRED */
#else
	if (pmu_yield_os_check()) {
		pmu_release_wakelock(PMU_WLAN_DEVICE);
	}
#endif //CONFIG_PLATFORM_TIZENRT_OS
}

void rtw_wakelock_timeout(u32 timeoutms)
{
#if CONFIG_PLATFORM_TIZENRT_OS
  /* TIZEN_RT_NOT_REQUIRED */
#else
	if (pmu_yield_os_check()) {
		pmu_set_sysactive_time(timeoutms);
	}
#endif //CONFIG_PLATFORM_TIZENRT_OS
}

