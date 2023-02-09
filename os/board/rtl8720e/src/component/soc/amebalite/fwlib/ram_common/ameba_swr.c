/**
  ******************************************************************************
  * @file    ameba_swr.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the swr:
  *           - swr_core
  *           - swr_mem
  *           - swr_audio
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
  * @brief  Enable or disable SWR_MEN.
  * @param  NewState: DISABLE/ENABLE
  * @retval SWR operation status, 0 operation OK, 1 operation fail.
  */
u32 SWR_MEM(u32 NewState)
{
	SWR_TypeDef *swr = SWR2_BASE;
	u32 Rtemp;
	/*timeout value according to SWR_MASK_STD_L1 of SWR_ON_CTRL0*/
	u32 swr_timeout = 100000;

	if (NewState == ENABLE) {
		Rtemp = swr->SWR_OFF_CTRL1;
		Rtemp &= (~SWR_MASK_VOFB_SEL);
		Rtemp |= SWR_VOFB_SEL_(0x1);
		swr->SWR_OFF_CTRL1 = Rtemp;

		swr->SWR_ON_CTRL0 &= (~SWR_MASK_STD_L1);
		swr->REGU_POWER_CTRL |= SWR_BIT_POW_BG;

		DelayUs(300);

		swr->REGU_POWER_CTRL |= SWR_BIT_POW_SW;

		// Wait ready
		while (!(swr->REGU_POWER_CTRL & SWR_BIT_PRDY_SWR)) {
			swr_timeout--;
			if (swr_timeout == 0) {
				return 1;
			}
		}
		return 0;
	} else {
		swr->REGU_POWER_CTRL &= (~SWR_BIT_POW_SW);
		swr->REGU_POWER_CTRL &= (~SWR_BIT_POW_BG);
		return 0;
	}
}

/**
  * @brief  Get mem swr mode.
  * @retval swr mode.
  *   swr mode shows as follows:
  *            - SWR_PWM : PWM mode
  *            - SWR_PFM : PFM mode
  */
u32 SWR_MEM_Mode_Get(void)
{
	SWR_TypeDef *swr = SWR2_BASE;

	return ((swr->REGU_POWER_CTRL & SWR_BIT_MODE_L) >> 18);
}


/**
  * @brief  Switch SWR_MEN mode.
  * @param  SWR_Mode: SWR_PWM/SWR_PFM
  * @retval SWR operation status, 0 operation OK, 1 operation fail.
  */
u32 SWR_MEM_Mode_Set(u32 SWR_Mode)
{
	SWR_TypeDef *swr = SWR2_BASE;
	u32 Rtemp;
	/*mode switch 100us*/
	u32 swr_timeout = 10000;

	assert_param(IS_SWR_MODE(SWR_Mode));

	if (SWR_PWM == SWR_Mode) {
		if (SWR_PWM == SWR_MEM_Mode_Get()) {
			return 0;
		}
		swr->SWR_ON_CTRL1 |= SWR_BIT_PWM_FORCE;

		DelayUs(100);

		Rtemp = swr->SWR_ON_CTRL0;
		Rtemp |= SWR_BIT_FPWM_L1;
		Rtemp &= (~SWR_MASK_REG_ZCDC_H);
		Rtemp |= SWR_REG_ZCDC_H(0x2);
		swr->SWR_ON_CTRL0 = Rtemp;

		DelayUs(100);

		swr->SWR_ON_CTRL1 &= (~SWR_BIT_PWM_FORCE);

		while (!(swr->REGU_POWER_CTRL & SWR_BIT_MODE_L)) {
			swr_timeout--;
			if (swr_timeout == 0) {
				return 1;
			}
		}
		return 0;
	} else {
		if (SWR_PFM == SWR_MEM_Mode_Get()) {
			return 0;
		}

		Rtemp = swr->SWR_ON_CTRL0;
		Rtemp &= (~SWR_BIT_FPWM_L1);
		Rtemp &= (~SWR_MASK_REG_ZCDC_H);
		Rtemp |= SWR_REG_ZCDC_H(0x1);
		swr->SWR_ON_CTRL0 = Rtemp;

		while (swr->REGU_POWER_CTRL & SWR_BIT_MODE_L) {
			swr_timeout--;
			if (swr_timeout == 0) {
				return 1;
			}
		}
		return 0;
	}
}

/**
  * @brief  Enable or disable analog LDO.
  * @param  NewState: DISABLE/ENABLE
  * @retval operation status, 0 operation OK, 1 operation fail.
  */
u32 LDO_PSRAM(u32 NewState)
{
	REGU_TypeDef *regu = REGU_BASE;
	u32 Rtemp;
	u8 Value;

	/*autoload ANA LDO 1.8v from otp*/
	Value = SWR_Calib_Get_Value(ANA_LDO_180);
	if (Value != Swr_Calib_Nocalib) {
		Rtemp = regu->REGU_LDO_3318_CTRL;
		Rtemp &= ~(REGU_MASK_VOADJ_L_3318);
		Rtemp |= REGU_VOADJ_L_3318(Value);
		regu->REGU_LDO_3318_CTRL = Rtemp;
	}

	Rtemp = regu->REGU_LDO_3318_CTRL;
	if (NewState == ENABLE) {
		if ((Rtemp & REGU_BIT_PD_REGU_L_3318) == 0) {
			return 0;
		}

		regu->REGU_LDO_3318_CTRL = Rtemp & (~REGU_BIT_PD_REGU_L_3318);

#ifdef AMEBALITE_TODO /* 200us only for test, wait a proper num from DD */
#endif
		/* wait LDO power stable, 47uF 800us, 100uF 1.3ms. 4.7 or 10uF should be choosed */
		DelayUs(200);
		return 0;
	} else {
		regu->REGU_LDO_3318_CTRL = Rtemp | REGU_BIT_PD_REGU_L_3318;
		return 0;
	}
}

/**
  * @brief  Get psram swr mode.
  * @retval swr mode.
  *   swr mode shows as follows:
  *            - SWR_PWM : PWM mode
  *            - SWR_PFM : PFM mode
  */
u32 SWR_PSRAM_Mode_Get(void)
{
	SWR_TypeDef *swr = SWR1_BASE;

	return ((swr->REGU_POWER_CTRL & SWR_BIT_MODE_L) >> 18);
}

/**
  * @brief  Switch SWR_PSRAM mode.
  * @param  SWR_Mode: SWR_PWM/SWR_PFM
  * @retval SWR operation status, 0 operation OK, 1 operation fail.
  */
u32 SWR_PSRAM_Mode_Set(u32 SWR_Mode)
{
	SWR_TypeDef *swr = SWR1_BASE;
	u32 Rtemp;
	/*mode switch 100us*/
	u32 swr_timeout = 10000;

	assert_param(IS_SWR_MODE(SWR_Mode));

	if (SWR_PWM == SWR_Mode) {
		if (SWR_PWM == SWR_PSRAM_Mode_Get()) {
			return 0;
		}
		swr->SWR_ON_CTRL1 |= SWR_BIT_PWM_FORCE;

		DelayUs(100);

		Rtemp = swr->SWR_ON_CTRL0;
		Rtemp |= SWR_BIT_FPWM_L1;
		Rtemp &= (~SWR_MASK_REG_ZCDC_H);
		Rtemp |= SWR_REG_ZCDC_H(0x2);
		swr->SWR_ON_CTRL0 = Rtemp;

		DelayUs(100);

		swr->SWR_ON_CTRL1 &= (~SWR_BIT_PWM_FORCE);

		while (!(swr->REGU_POWER_CTRL & SWR_BIT_MODE_L)) {
			swr_timeout--;
			if (swr_timeout == 0) {
				return 1;
			}
		}
		return 0;
	} else {
		if (SWR_PFM == SWR_PSRAM_Mode_Get()) {
			return 0;
		}

		Rtemp = swr->SWR_ON_CTRL0;
		Rtemp &= (~SWR_BIT_FPWM_L1);
		Rtemp &= (~SWR_MASK_REG_ZCDC_H);
		Rtemp |= SWR_REG_ZCDC_H(0x1);
		swr->SWR_ON_CTRL0 = Rtemp;

		while (swr->REGU_POWER_CTRL & SWR_BIT_MODE_L) {
			swr_timeout--;
			if (swr_timeout == 0) {
				return 1;
			}
		}
		return 0;
	}
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
