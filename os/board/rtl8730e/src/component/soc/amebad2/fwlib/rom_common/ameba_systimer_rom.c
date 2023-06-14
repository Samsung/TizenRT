/**
  ******************************************************************************
  * @file    rtl8721d_systimer_rom.c
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

/**
  * @brief  Init TIM0 as systimer.
  * @param  none
  * @note called by rom code when boot, user dont use it.
  */
HAL_ROM_TEXT_SECTION
void SYSTIMER_Init(void)
{
	RTIM_TimeBaseInitTypeDef TIM_InitStruct;

	RTIM_TimeBaseStructInit(&TIM_InitStruct);

	TIM_InitStruct.TIM_Idx = 0;
	TIM_InitStruct.TIM_Prescaler = 0;
	TIM_InitStruct.TIM_Period = 0xFFFFFFFF;

	TIM_InitStruct.TIM_UpdateEvent = ENABLE; /* UEV enable */
	TIM_InitStruct.TIM_UpdateSource = TIM_UpdateSource_Overflow;
	TIM_InitStruct.TIM_ARRProtection = ENABLE;

	RTIM_TimeBaseInit(TIM0, &TIM_InitStruct, 0, (IRQ_FUN) NULL, (u32)NULL);
	RTIM_Cmd(TIM0, ENABLE);
}

/**
  * @brief  Get TIM0 tick, unit is 31us.
  * @param  none
  */
HAL_ROM_TEXT_SECTION
u32 SYSTIMER_TickGet(void)
{
	return RTIM_GetCount(TIM0);
}

/**
  * @brief  Get TIM0 pass tick.
  * @param  none
  */
HAL_ROM_TEXT_SECTION
u32 SYSTIMER_GetPassTick(u32 start)
{
	u32 current = 0;
	u32 passed = 0;

	current = RTIM_GetCount(TIM0);

	if (current >= start) {
		passed = current - start;
	} else {
		passed = 0xFFFFFFFF - (start - current);
	}

	return passed;
}

/**
  * @brief  Get TIM0 pass time, unit is ms.
  * @param  start: start time stamp (ms)
  * @note resolution is 31us
  */
HAL_ROM_TEXT_SECTION
u32 SYSTIMER_GetPassTime(u32 start)
{
	u32 current = 0;
	u32 passed = 0;

	current = RTIM_GetCount(TIM0);

	if (current >= start) {
		passed = current - start;
	} else {
		passed = 0xFFFFFFFF - (start - current);
	}

	/* ms */
	passed = (((passed & 0xFFFF8000) >> 15) * 1000) + (((passed & 0x7FFF) * 1000) >> 15);

	return passed;
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
