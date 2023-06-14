/**
  ******************************************************************************
  * @file    rtl8721d_delay_rom.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the systimer & delay:
  *           - SYSTIMER_GetPassTime
  *           - SYSTIMER_TickGet
  *           - DelayUs
  *           - DelayMs
  *           - DelayNop
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

HAL_ROM_BSS_SECTION u32 RBSS_UDELAY_FLAG;
HAL_ROM_BSS_SECTION u32 RBSS_UDELAY_CLK;

/**
  * @brief  Delay x nops.
  * @param  count: nop number
  * @note for KM4(default): HF=YES MVE=YES, code in ITCM, 7 cycles per loop. For 300M CPU clock, one loop cost: (1/300)*7=0.023us
  * @note for KM4: HF=NO MVE=YES, code in ICache, 6 cycles per loop. For 300M CPU clock, one loop cost: (1/300)*6=0.02us
  * @note for KM4 FPGA: 50M CPU clock, one loop cost: (1/50)*7=0.14us
  * @note for KM0: HF=NO, 5 cycles per loop. For 20M CPU clock, one loop cost: (1/20)*5=0.25us
  */
__weak HAL_ROM_TEXT_SECTION
__attribute__((noinline)) void DelayNop(u32 count)
{
	asm volatile(".align 3					\n"
				 "1:						\n"
				 "sub	%[count], #1		\n"
				 "cmp	%[count], #0		\n"
				 "nop					\n"
				 "bne		1b			\n"
				 :[count]"+r"(count)
				 ::"cc");
}

/**
  * @brief  Delay x us.
  * @param  us: delay time, unit is us
  */
__weak HAL_ROM_TEXT_SECTION
void DelayUs(u32 us)
{
	u32 WaitCount = 0;
	u32 CpuClk = CPU_ClkGet() / 1000000;
	u32 CPUID = SYS_CPUID();

	if (CPUID == LP_CPU_ID) {
		WaitCount = (us * CpuClk) / 5; /* There are 5 instructions in every DelayNop loop, one loop = 5/CpuClk us */
	} else if (CPUID == NP_CPU_ID) {
		WaitCount = (us * CpuClk) / 7; /* There are 7 instructions in every DelayNop loop, one loop = 7/CpuClk us */
	}

	/* ROM uses low CPU clock delay in case of clock error */
	if (RBSS_UDELAY_FLAG == 0) {
		u32 Temp = 0;
		u32 Clk[] = {26, 100};
		u32 Div[] = {5, 7};

		if (RBSS_UDELAY_CLK == 0) {
			Temp = Clk[CPUID];
		} else {
			Temp = RBSS_UDELAY_CLK;
		}
		WaitCount = (us * Temp) / Div[CPUID];
	}

	if (WaitCount == 0) {
		return;
	}

	DelayNop(WaitCount);

	return;
}

/**
  * @brief  Delay x ms.
  * @param  ms: delay time, unit is ms
  */
__weak HAL_ROM_TEXT_SECTION
void DelayMs(u32 ms)
{
	DelayUs(ms * 1000);
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
