/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"
#include "arch_timer.h"

#ifdef CONFIG_PLATFORM_TIZENRT_OS
#define vGetGenericTimerFreq()		((u32)PLL_GetHBUSClk() / 2)
#endif /* CONFIG_PLATFORM_TIZENRT_OS */

#define DelayCheck()						\
	do {							\
		__asm__ __volatile__("nop;");	\
	} while (0)

/**
  * @brief  Delay x us.
  * @param  us: delay time, unit is us
  */
void DelayUs(u32 us)
{
	uint64_t cntvct;

	/*check if arch timer is still close, open it */
	if ((SYSCFG_CHIPType_Get() == CHIP_TYPE_RTLSIM) || !(arm_arch_timer_check_enable())) {
		arm_arch_timer_enable(pdTRUE);
	}

	cntvct = (uint64_t)(arm_arch_timer_count() + vGetGenericTimerFreq() * us / MHZ_TICK_CNT);

	while (arm_arch_timer_count() < cntvct) {
		DelayCheck();
	}
	__DSB();
}

/**
  * @brief  Delay x ms.
  * @param  ms: delay time, unit is ms
  */
void DelayMs(u32 ms)
{
	DelayUs(ms * 1000);
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
