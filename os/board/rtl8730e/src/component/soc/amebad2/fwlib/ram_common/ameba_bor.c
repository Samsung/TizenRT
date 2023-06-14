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
  * @brief  Set BOR rising and falling threshold.
  * @param  Thres_Fall: BOR falling threshold.
  *          This parameter can be a number between 0 and 0x1F:
  *            @arg 00000: 3.3333
  *            @arg 00001: 3.2877
  *            @arg 00010: 3.2432
  *            @arg 00011: 3.2000
  *            @arg 00100: 3.1579
  *            @arg 00101: 3.1035
  *            @arg 00110: 3.0508
  *            @arg 00111: 3.0000
  *            @arg 01000: 2.9508
  *            @arg 01001: 2.9032
  *            @arg 01010: 2.8571
  *            @arg 01011: 2.8125
  *            @arg 01100: 2.7692
  *            @arg 01101: 2.7169
  *            @arg 01110: 2.6666
  *            @arg 01111: 2.6182
  *            @arg 10000: 2.5623
  *            @arg 10001: 2.5087
  *            @arg 10010: 2.4574
  *            @arg 10011: 2.4080
  *            @arg 10100: 2.3606
  *            @arg 10101: 2.3151
  *            @arg 10110: 2.2713
  *            @arg 10111: 2.2154
  *            @arg 11000: 2.1621
  *            @arg 11001: 2.1114
  *            @arg 11010: 2.0630
  *            @arg 11011: 2.0168
  *            @arg 11100: 1.9672
  *            @arg 11101: 1.9098
  *            @arg 11110: 1.8556
  *            @arg 11111: 1.8045
  * @param  Thres_Rise: BOR rising threshold.
  *          This parameter can be a number between 0 and 0x1F. The value description is the same as above.
  * @note   For chip to work normally, BOR_TH_LOW0 and BOR_TH_HIGH0 are not supported.
  * @retval   None
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
