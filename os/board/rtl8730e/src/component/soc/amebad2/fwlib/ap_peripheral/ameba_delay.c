/**
  ******************************************************************************
  * @file    rtl8721d_delay.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the systimer & delay:
  *           - DelayUs
  *           - DelayMs
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"
#include "arch_timer.h"

#if defined ( __ICCARM__ )
#define DelayCheck()						\
	do {							\
		__asm volatile("nop;");	\
	} while (0)
#else
#define DelayCheck()						\
	do {							\
		__asm__ __volatile__("nop;");	\
	} while (0)
#endif
/**
  * @brief  Delay x us.
  * @param  us: delay time, unit is us
  */
void DelayUs(u32 us)
{
	uint64_t cntvct;

	/*check if arch timer is still close, open it */
	if ((SYSCFG_CHIPType_Get() == CHIP_TYPE_RTLSIM) || !(arm_arch_timer_check_enable())) {
		arm_arch_timer_enable(1);
	}

	if (SYSCFG_CHIPType_Get() == CHIP_TYPE_FPGA) {
		cntvct = (uint64_t)(arm_arch_timer_count() + 12.5 * us); //12.5M clock
	} else {
		cntvct = (uint64_t)(arm_arch_timer_count() + 50 * us); //50M clock
	}

	while (arm_arch_timer_count() < cntvct) {
		DelayCheck();
	}
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
