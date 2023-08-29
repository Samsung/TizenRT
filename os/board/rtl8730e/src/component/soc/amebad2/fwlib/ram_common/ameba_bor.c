/**
  ******************************************************************************
  * @file    ameba_bor.c
  * @author
  * @version V1.0.0
  * @date    2017-10-16
  * @brief   This file contains all the functions prototypes for the bor firmware
  *             library, including the following functionalities of bor controller:
  *           - mode set
  *           - threshold set
  *           - Debounce time set
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

/**
  * @brief Set BOR rising and falling threshold.
  * @param Thres_Fall: BOR falling threshold.
  *        This parameter can be one of the following values:
  *          @arg 0xA: 2.90
  *          @arg 0xB: 2.86
  *          @arg 0xC: 2.81
  *          @arg 0xD: 2.76
  *          @arg 0xE: 2.72
  *          @arg 0xF: 2.67
  *          @arg 0x10: 2.63
  * @param Thres_Rise: BOR rising threshold.
  *        This parameter can be one of the following values:
  *          @arg 0x8: 2.98
  *          @arg 0x9: 2.92
  *          @arg 0xA: 2.87
  *          @arg 0xB: 2.83
  *          @arg 0xC: 2.78
  *          @arg 0xD: 2.74
  *          @arg 0xE: 2.69
  * @note To make chip work normally, BOR_TH_LOW0 and BOR_TH_HIGH0 are not supported.
  * @retval None
  */
VOID BOR_ThresholdSet(u32 Thres_Fall, u32 Thres_Rise)
{
	u32 temp = 0;
	REGU_TypeDef *regu = REGU_BASE;
	assert_param(IS_BOR_TH_FALL(Thres_Fall));
	assert_param(IS_BOR_TH_RISE(Thres_Rise));

	temp = regu->REGU_AON_BOR_CTRL;
	temp &= (~(REGU_MASK_REG_BOD_R_4_0 | REGU_MASK_REG_BOD_F_4_0));
	temp |= (REGU_REG_BOD_F_4_0(Thres_Fall) | \
			 REGU_REG_BOD_R_4_0(Thres_Rise));
	regu->REGU_AON_BOR_CTRL = temp;
}

/**
  * @brief  BOD Enable,
  * @param  newstatus: can be one of the following values:
  *		 @arg ENABLE: enable bod.
  *		 @arg DISABLE: disable bod.
  * @note NA
  */
void BOR_Enable(u32 newstatus)
{

	REGU_TypeDef *regu = REGU_BASE;

	if (newstatus == ENABLE) {
		regu->REGU_AON_BOR_CTRL |= REGU_BIT_REG_BOD_R_5;
	} else {
		regu->REGU_AON_BOR_CTRL &= ~REGU_BIT_REG_BOD_R_5;
	}
}

/**
  * @brief  Set BOR debounce cycle.
  * @param  Dbnc_Value: debounce cycle, in unit of ANA4M clock cycles.
  *            @arg 000: 0us
  *            @arg 001: 100us
  *            @arg 010: 500us
  *            @arg 011: 1000us
  *            @arg 100: 2000us
  *            @arg 101: 4000us
  *            @arg 110: 8000us
  *            @arg 111: 16000us
  * @retval   None
  */
VOID BOR_DbncSet(u32 Dbnc_Value)
{
	u32 temp = 0;
	assert_param(IS_BOR_DBNC_THRES(Dbnc_Value));
	temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_BOR_CTRL);
	temp &= (~AON_MASK_BOR_TDBC);
	temp |= AON_BOR_TDBC(Dbnc_Value);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_BOR_CTRL, temp);
}

/**
  * @brief  Clear BOR interrupt.
  * @retval   None
  */
VOID BOR_ClearINT(void)
{
	u32 temp = 0;

	temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_BOR_ISR);
	temp |= AON_BIT_BOR_ISR;
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_BOR_ISR, temp);
}

/**
  * @brief  Enable BOR.
  * @param  Option: BOR mode.
  *          This parameter can be one of the following values:
  *            @arg BOR_RESET: BOR reset mode
  *            @arg BOR_INTR: BOR interrupt mode
  * @retval   None
  */
VOID BOR_ModeSet(u32 Option)
{
	u32 temp = 0;

	if (BOR_INTR == Option) {
		temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_BOR_CTRL);
		temp |= AON_BIT_BOR_INTR_MODE;
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_BOR_CTRL, temp);

		/* Please set wake event in sleep_wevent_config[] in ameba_sleepcfg.c */
//		temp = HAL_READ32(PMC_BASE, WAK_MASK0_LP);
//		temp |= PMC_WAK_LP_IMR_31_0(BOR_WAKE_LP_IMR);
//		HAL_WRITE32(PMC_BASE, WAK_MASK0_LP, temp);
	} else {
		temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_BOR_CTRL);
		temp &= ~AON_BIT_BOR_INTR_MODE;
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_BOR_CTRL, temp);
	}
}
