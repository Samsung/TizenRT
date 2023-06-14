/**
  ******************************************************************************
  * @file    rtl8721dhp_pmc_ram.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the soc power management circut:
  *           - wakeup timer
  *           - wakeup pin
  *           - sleep option
  *           - sleep mode
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

/**
  * @brief  set REGU wakeup_pin debounce cycle.
  * @param  Dbnc_cycle: wakeup_pin debounce cycle, unit is 100k/128k.
  *		This parameter must be set to a value in the 0-0x7FFF range.
  * @param  Status: enable or disable wakepin debounce function
  * @note Dbnc_cycle is valid when Status is set ENABLE.
  * @retval None
  */
void SOCPS_SetWakepinDebounce_HP(u32 Dbnc_cycle, u32 Status)
{
	u32 Rtemp = 0;
	u32 FEN_backup = 0;
	assert_param(Dbnc_cycle <= 0x7FFF);

	/* disable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	FEN_backup = AON_GET_GPIO_WAKE_FEN(Rtemp);
	Rtemp &= (~AON_MASK_GPIO_WAKE_FEN);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);

	/* set gpio debounce cycle */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	if (ENABLE == Status) {
		Rtemp &= (~AON_MASK_GPIO_DBNC_CYC);
		Rtemp |= AON_GPIO_DBNC_CYC(Dbnc_cycle);
		Rtemp |= AON_BIT_GPIO_DBNC_FEN;
	} else {
		Rtemp &= (~AON_BIT_GPIO_DBNC_FEN);
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);

	/* enable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	Rtemp |= AON_GPIO_WAKE_FEN(FEN_backup);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);
}

/**
  * @brief  set REGU wakeup_pin (just on wakepin mux to 4 pads).
  * @param  PinMask: aon wakepin index
  *		This parameter can be one of the following values:
  *		 @arg WAKUP_0 :
  *		 @arg WAKUP_1 :
  *		 @arg WAKUP_2 :
  *		 @arg WAKUP_3 :
  * @param  Polarity: aon wakepin Polarity
  *		This parameter can be one of the following values:
  *		 @arg 1 : high wakeup
  *		 @arg 0 : low wakeup
  * @note wakeup state: sleep PG & CG & deep sleep
  * @retval None
  */
void SOCPS_SetWakepin_HP(u32 PinIdx, u32 Polarity)
{
	u32 Rtemp = 0;

	/* disable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	Rtemp &= (~(AON_GPIO_WAKE_FEN(BIT(PinIdx))));
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);

	/* set polarity & internal PU/PD resistence */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	Rtemp &= ~(AON_GPIO_WAKDET_POLY(BIT(PinIdx))); /* clear polarity, set to low active */
	if (Polarity == 1) {
		Rtemp |= AON_GPIO_WAKDET_POLY(BIT(PinIdx)); /* if high active */
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);

	/* enable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	Rtemp |= (AON_GPIO_WAKE_FEN(BIT(PinIdx)));
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);
}

/**
  * @brief  Clear wakepin interrupt.
  * @retval   None
  */
VOID SOC_WakePinClearINT_HP(u32 wakepin)
{
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_WAKE_ISR, wakepin);
}

/**
  * @brief  set AON timer for wakeup.
  * @param  SDuration: sleep time, unit is ms
  * @note wakeup state:sleep PG & CG & deep sleep
  * @note This is 100KHz timer, max counter = 0xFFFFFFFF/100000/60 = 715min
  * @retval None
  */
void SOCPS_AONTimer_HP(u32 SDuration)
{
	if (SDuration == 0) {
		return;
	}

	SDuration = SDuration * 100;

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_SET, AON_TIM_CNT_CUR(SDuration));
}

/**
  * @brief  enable AON timer interrupt.
  * @param  Status: interrupt status
  * @retval None
  */
void SOCPS_AONTimerINT_EN_HP(u32 Status)
{
	u32 Rtemp = 0;

	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_IER);
	if (ENABLE == Status) {
		Rtemp |= AON_BIT_TIM_IE;
	} else {
		Rtemp &= (~AON_BIT_TIM_IE);

	}
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_IER, Rtemp);
}

/**
  * @brief  Clear aontimer interrupt.
  * @retval   None
  */
VOID SOC_AONTimerClearINT_HP(void)
{
	u32 temp = 0;

	temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_ISR);
	temp |= AON_BIT_TIM_ISR;
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_ISR, temp);
}


