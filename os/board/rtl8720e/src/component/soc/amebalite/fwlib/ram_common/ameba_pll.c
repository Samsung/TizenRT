/**
  ******************************************************************************
  * @file    rtl8721d_pll.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the NCO clock:
  *           - NCO32k
  *           - NCO8M
  *           - EXT32k Disable/Enable
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

#ifdef AMEBALITE_TODO
/**
  * @brief  PLL divider set
  * @param  div: Division
  */
void PLL_I2S_Div(u32 div)
{
	assert_param(div >= 3);

	u32 RegValue = 0;
	RegValue = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKD_GRP1);
	RegValue &= ~(LSYS_MASK_CKD_I2S_PLL);
	RegValue |= LSYS_CKD_I2S_PLL(div - 1);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKD_GRP1, RegValue);
}
#endif

/**
  * @brief  CPU PLL open/close
  * @param  status: ENABLE/DISABLE
  */
NON_DRAM_TEXT_SECTION
void PLL_CPU(u32 status)
{
	PLL_TypeDef *PLL = (PLL_TypeDef *)PLL_BASE;
	if (status == DISABLE) {
		/* close CPU PLL */
		PLL->PLL_CPUPLL_CTRL0 &= ~(PLL_BIT_CPUPLL_CK_EN | PLL_BIT_CPUPLL_CK_EN_D4);
		PLL->PLL_CPUPLL_CTRL0 &= ~(PLL_BIT_CPUPLL_POW_PLL | PLL_BIT_CPUPLL_POW_DIV_IN);
		PLL->PLL_CPUPLL_PS &= (~(PLL_BIT_CPUPLL_EN_CK400M | PLL_BIT_CPUPLL_EN_CK400M_PS));
		PLL->PLL_CPUPLL_PS &= ~PLL_BIT_CPUPLL_POW_CKGEN;

		PLL->PLL_CPUPLL_CTRL0 &= ~PLL_BIT_CPUPLL_POW_ERC;
	} else {
		/* open CPU PLL */
		// CPUPLL erc enable
		PLL->PLL_CPUPLL_CTRL0 |= PLL_BIT_CPUPLL_POW_ERC;
		DelayUs(1);

		// CPUPLL power on
		PLL->PLL_CPUPLL_PS |= PLL_BIT_CPUPLL_EN_CK400M | PLL_BIT_CPUPLL_EN_CK400M_PS;
		PLL->PLL_CPUPLL_PS |= PLL_BIT_CPUPLL_POW_CKGEN;
		PLL->PLL_CPUPLL_CTRL0 |= (PLL_BIT_CPUPLL_POW_PLL | PLL_BIT_CPUPLL_POW_DIV_IN);
		PLL->PLL_CPUPLL_CTRL0 |= (PLL_BIT_CPUPLL_CK_EN | PLL_BIT_CPUPLL_CK_EN_D4);

		// Wait ready
		while (!(PLL->PLL_STATE & PLL_BIT_CPU_PLL_RDY));
	}
}

/**
  * @brief  DSP PLL open/close
  * @param  status: ENABLE/DISABLE
  */
NON_DRAM_TEXT_SECTION
void PLL_DSP(u32 status)
{
	PLL_TypeDef *PLL = (PLL_TypeDef *)PLL_BASE;

	if (status == DISABLE) {
		PLL->PLL_PERIPLL_CTRL0 &= ~(PLL_BIT_PERIPLL_CK_EN | PLL_BIT_PERIPLL_CK_EN_D4);
		PLL->PLL_PERIPLL_CTRL0 &= ~(PLL_BIT_PERIPLL_POW_PLL | PLL_BIT_PERIPLL_POW_DIV_IN);
		PLL->PLL_PERIPLL_PS &= (~(PLL_BIT_PERIPLL_EN_CK400M | PLL_BIT_PERIPLL_EN_CK400M_PS));
		PLL->PLL_PERIPLL_PS &= ~PLL_BIT_PERIPLL_POW_CKGEN;

		PLL->PLL_PERIPLL_CTRL0 &= ~PLL_BIT_PERIPLL_POW_ERC;

	} else {
		// PERIPLL erc enable
		PLL->PLL_PERIPLL_CTRL0 |= PLL_BIT_PERIPLL_POW_ERC;
		DelayUs(1);

		// PERIPLL power on
		PLL->PLL_PERIPLL_PS |= PLL_BIT_PERIPLL_EN_CK400M | PLL_BIT_PERIPLL_EN_CK400M_PS;
		PLL->PLL_PERIPLL_PS |= PLL_BIT_PERIPLL_POW_CKGEN;
		PLL->PLL_PERIPLL_CTRL0 |= (PLL_BIT_PERIPLL_POW_PLL | PLL_BIT_PERIPLL_POW_DIV_IN);
		PLL->PLL_PERIPLL_CTRL0 |= (PLL_BIT_PERIPLL_CK_EN | PLL_BIT_PERIPLL_CK_EN_D4);

		// Wait ready
		while (!(PLL->PLL_STATE & PLL_BIT_DSP_PLL_RDY));
	}
}

/**
  * @brief  PLL bandgap open/close
  when two plls are closed, need close PLL BG; when need to open pll, need open PLL BG first
  * @param  status: ENABLE/DISABLE
  */
NON_DRAM_TEXT_SECTION
void PLL_BG(u32 status)
{
	PLL_TypeDef *PLL = (PLL_TypeDef *)PLL_BASE;

	if (status == DISABLE) {
		/* if CPU PLL/ PERI PLL/ADC  all power off, need to close BandGap */
		PLL->PLL_AUX_BG &= ~(PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
		PLL->PLL_AUX_BG |= PLL_BIT_POW_I;
	} else {
		//Check BandGap power on
		if ((PLL->PLL_AUX_BG & PLL_BG_POW_MASK) == 0) {
			PLL->PLL_AUX_BG |= (PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
			DelayUs(20);
			PLL->PLL_AUX_BG &= (~PLL_BIT_POW_I);
			DelayUs(140);
		}
	}
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
