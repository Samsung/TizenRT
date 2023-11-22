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


/**
  * @brief  PLL divider set
  * @param  Sportx: sport index, 0/1/2/3
			div: Division
  */
void PLL_I2S_Div(int Sportx, u32 div)
{
	assert_param(div >= 1);
	assert_param(div <= 8);

	u32 RegValue = 0;
	RegValue = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SPORT_CLK);

	switch (Sportx) {
	case 0:
		RegValue &= ~(LSYS_MASK_CKD_SPORT0);
		RegValue |= LSYS_CKD_SPORT0(div - 1);
		break;
	case 1:
		RegValue &= ~(LSYS_MASK_CKD_SPORT1);
		RegValue |= LSYS_CKD_SPORT1(div - 1);
		break;
	case 2:
		RegValue &= ~(LSYS_MASK_CKD_SPORT2);
		RegValue |= LSYS_CKD_SPORT2(div - 1);
		break;
	case 3:
		RegValue &= ~(LSYS_MASK_CKD_SPORT3);
		RegValue |= LSYS_CKD_SPORT3(div - 1);
		break;
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SPORT_CLK, RegValue);
}

/**
  * @brief  Enable or disable I2S1(98.304M Hz)
  * @param  NewState: DISABLE/ENABLE
  * @note   PLL I2S1 disable should not power off BandGap unlsess neither ADC nor APPLL uses the BandGap.
  */
void PLL_I2S_98P304M(u32 NewState)
{
	PLL_TypeDef *pll = PLL_BASE;

	if (NewState == ENABLE) {
		//avoid repeated enabling operations
		if ((pll->PLL_STATE) & PLL_BIT_CKRDY_I2S1) {
			return ;
		}
		//Check BandGap power on
		if ((pll->PLL_AUX_BG & PLL_BG_POW_MASK) == 0) {
			pll->PLL_AUX_BG |= (PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
			DelayUs(20);
			pll->PLL_AUX_BG |= PLL_BIT_POW_I;
			DelayUs(140);
		}

		// erc enable
		pll->PLL_I2SPLL1_CTRL0 |= PLL_BIT_IPLL1_POW_ERC;
		DelayUs(1);

		// PLL power on
		pll->PLL_PS |= PLL_BIT_POW_CKGEN;
		pll->PLL_I2SPLL1_CTRL0 |= PLL_BIT_IPLL1_POW_PLL;
		pll->PLL_I2SPLL1_CTRL0 |= PLL_BIT_IPLL1_POW_PLL | PLL_BIT_IPLL1_CK_EN | PLL_BIT_IPLL1_CK_EN_D4;

		// Select I2S1
		pll->PLL_I2SPLL1_CTRL1 &= ~PLL_MASK_IPLL1_CK_OUT_SEL;
		pll->PLL_I2SPLL1_CTRL1 |= PLL_IPLL1_CK_OUT_SEL(14);

		// Wait ready
		while (!(pll->PLL_STATE & PLL_BIT_CKRDY_I2S1));

	} else {

		pll->PLL_I2SPLL1_CTRL0 &= ~ PLL_BIT_IPLL1_POW_PLL;
		pll->PLL_I2SPLL1_CTRL0 &= ~ PLL_BIT_IPLL1_POW_ERC;
		pll->PLL_AUX_BG &= ~(PLL_BIT_POW_BG | PLL_BIT_POW_I | PLL_BIT_POW_MBIAS);

	}
}

/**
  * @brief  Enable or disable I2S2(45.158M Hz)
  * @param  NewState: DISABLE/ENABLE
  * @note   PLL I2S2 disable should not power off BandGap unlsess neither ADC nor APPLL uses the BandGap.
  */
void PLL_I2S_45P158M(u32 NewState)
{
	PLL_TypeDef *pll = PLL_BASE;

	if (NewState == ENABLE) {
		//avoid repeated enabling operations
		if ((pll->PLL_STATE) & PLL_BIT_CKRDY_I2S2) {
			return ;
		}

		//Check BandGap power on
		if ((pll->PLL_AUX_BG & PLL_BG_POW_MASK) == 0) {
			pll->PLL_AUX_BG |= (PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
			DelayUs(20);

			pll->PLL_AUX_BG |=  PLL_BIT_POW_I;
			DelayUs(140);
		}

		// erc enable
		pll->PLL_I2SPLL2_CTRL0 |= PLL_BIT_IPLL2_POW_ERC;
		DelayUs(1);

		// PLL power on
		pll->PLL_PS |= PLL_BIT_POW_CKGEN;
		pll->PLL_I2SPLL2_CTRL0 |= PLL_BIT_IPLL2_POW_PLL | PLL_BIT_IPLL2_CK_EN | PLL_BIT_IPLL2_CK_EN_D4;

		// Select I2S1
		pll->PLL_I2SPLL2_CTRL1 &= ~PLL_MASK_IPLL2_CK_OUT_SEL;
		pll->PLL_I2SPLL2_CTRL1 |= PLL_IPLL2_CK_OUT_SEL(12);

		// Wait ready
		while (!(pll->PLL_STATE & PLL_BIT_CKRDY_I2S2));
	} else {

		pll->PLL_I2SPLL2_CTRL0 &= ~ PLL_BIT_IPLL2_POW_PLL;
		pll->PLL_I2SPLL2_CTRL0 &= ~ PLL_BIT_IPLL2_POW_ERC;
		pll->PLL_AUX_BG &= ~(PLL_BIT_POW_BG | PLL_BIT_POW_I | PLL_BIT_POW_MBIAS);

	}
}

/**
  * @brief  Enable or disable (24.576MHz)
  * @param  NewState: DISABLE/ENABLE
  * @note   PLL I2S2 disable should not power off BandGap unlsess neither ADC nor APPLL uses the BandGap.
  */
void PLL_I2S_24P576M(u32 NewState)
{
	PLL_TypeDef *pll = PLL_BASE;

	if (NewState == ENABLE) {
		//avoid repeated enabling operations
		if ((pll->PLL_STATE) & PLL_BIT_CKRDY_I2S1) {
			return ;
		}

		//Check BandGap power on
		if ((pll->PLL_AUX_BG & PLL_BG_POW_MASK) == 0) {
			pll->PLL_AUX_BG |= (PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
			DelayUs(20);
			pll->PLL_AUX_BG |= PLL_BIT_POW_I;
			DelayUs(140);
		}

		// erc enable
		pll->PLL_I2SPLL1_CTRL0 |= PLL_BIT_IPLL1_POW_ERC;
		DelayUs(1);

		// PLL power on
		pll->PLL_PS |= PLL_BIT_POW_CKGEN;
		pll->PLL_I2SPLL1_CTRL0 |= PLL_BIT_IPLL1_POW_PLL | PLL_BIT_IPLL1_CK_EN | PLL_BIT_IPLL1_CK_EN_D4;

		pll->PLL_I2SPLL1_CTRL1 &= ~PLL_MASK_IPLL1_CK_OUT_SEL;
		pll->PLL_I2SPLL1_CTRL1 |= PLL_IPLL1_CK_OUT_SEL(14);

		// Wait ready
		while (!(pll->PLL_STATE & PLL_BIT_CKRDY_I2S1));

	} else {

		pll->PLL_I2SPLL1_CTRL0 &= ~ PLL_BIT_IPLL1_POW_PLL;
		pll->PLL_I2SPLL1_CTRL0 &= ~ PLL_BIT_IPLL1_POW_ERC;
		pll->PLL_AUX_BG &= ~(PLL_BIT_POW_BG | PLL_BIT_POW_I | PLL_BIT_POW_MBIAS);

	}
}



/**
  * @brief  I2S1 PLL output adjust by ppm.
  * @param  ppm: ~1.55ppm per FOF step
  * @param  action: slower or faster or reset
  *          This parameter can be one of the following values:
  *            @arg PLL_AUTO: reset to auto 98.304M
  *            @arg PLL_FASTER: pll clock faster than 98.304M
  *            @arg PLL_SLOWER: pll clock slower than 98.304M
  */
float PLL_I2S_98P304M_ClkTune(float ppm, u32 action)
{

	u32 F0F_new;
	assert_param(ppm <= 1000);
	float real_ppm = 0;

	PLL_BASE -> PLL_I2SPLL1_CTRL1 &= (~PLL_MASK_IPLL1_DIVN_SDM);
	PLL_BASE -> PLL_I2SPLL1_CTRL1 |= (PLL_IPLL1_DIVN_SDM(7));

	if (action == PLL_FASTER) {
		F0F_new = 5269 + (u32)(ppm / 1.55);
	} else if (action == PLL_SLOWER) {
		F0F_new = 5269 - (u32)(ppm / 1.55);
	} else {
		F0F_new = 5269;
	}

	PLL_BASE -> PLL_I2SPLL1_CTRL3 &= (~PLL_MASK_IPLL1_F0F_SDM);
	PLL_BASE -> PLL_I2SPLL1_CTRL3 |= (PLL_IPLL1_F0F_SDM(F0F_new));

	PLL_BASE -> PLL_I2SPLL1_CTRL3 &= (~PLL_MASK_IPLL1_F0N_SDM);
	PLL_BASE -> PLL_I2SPLL1_CTRL3 |= (PLL_IPLL1_F0N_SDM(6));

	PLL_BASE ->	PLL_I2SPLL1_CTRL1 |= (PLL_BIT_IPLL1_TRIG_FREQ);
	PLL_BASE ->	PLL_I2SPLL1_CTRL1 &= (~PLL_BIT_IPLL1_TRIG_FREQ);

	real_ppm = (double)((double)F0F_new - (double)5269) * (double)1000000 / (double)8192 / (double)8 / (double)((double)9 + ((double)6 + (double)5269 /
			   (double)8192) / (double)8);

	return real_ppm;

}


/**
  * @brief  I2S2 PLL output adjust by ppm.
  * @param  ppm: ~1.69ppm per FOF step
  * @param  action: slower or faster or reset
  *          This parameter can be one of the following values:
  *            @arg PLL_AUTO: reset to auto 45.1584M
  *            @arg PLL_FASTER: pll clock faster than 45.1584M
  *            @arg PLL_SLOWER: pll clock slower than 45.1584M
  */
float PLL_I2S_45P158M_ClkTune(float ppm, u32 action)
{

	u32 F0F_new;
	float real_ppm = 0;
	assert_param(ppm <= 1000);

	PLL_BASE -> PLL_I2SPLL2_CTRL1 &= (~PLL_MASK_IPLL2_DIVN_SDM);
	PLL_BASE -> PLL_I2SPLL2_CTRL1 |= (PLL_IPLL2_DIVN_SDM(7));


	if (action == PLL_FASTER) {
		F0F_new = 2076 + (u32)(ppm / 1.69);
	} else if (action == PLL_SLOWER) {
		F0F_new = 2076 - (u32)(ppm / 1.69);
	} else {
		F0F_new = 2076;
	}

	PLL_BASE -> PLL_I2SPLL2_CTRL3 &= (~PLL_MASK_IPLL2_F0F_SDM);
	PLL_BASE -> PLL_I2SPLL2_CTRL3 |= (PLL_IPLL2_F0F_SDM(F0F_new));

	PLL_BASE -> PLL_I2SPLL2_CTRL3 &= (~PLL_MASK_IPLL2_F0N_SDM);
	PLL_BASE -> PLL_I2SPLL2_CTRL3 |= (PLL_IPLL2_F0N_SDM(0));

	PLL_BASE ->	PLL_I2SPLL2_CTRL1 |= (PLL_BIT_IPLL2_TRIG_FREQ);
	PLL_BASE ->	PLL_I2SPLL2_CTRL1 &= (~PLL_BIT_IPLL2_TRIG_FREQ);

	real_ppm = (double)((double)F0F_new - (double)2076) * (double)1000000 / (double)8192 / (double)8 / (double)((double)9 + ((double)0 + (double)2076 /
			   (double)8192) / (double)8);
	return real_ppm;

}

/**
  * @brief  Set PLL clock.
  * @param  PllClk: This parameter is used to set PLL frequency, for example PllClk=600000000 means 600MHz
  */
void PLL_NP_ClkSet(u32 PllClk)
{
	PLL_TypeDef *PLL = (PLL_TypeDef *)PLL_BASE;
	u32 Div, FoF, FoN, Temp;
	u32 XtalClk = XTAL_ClkGet();

	//PllClk = Div * XtalClk + (FoN + FoF >> 13) / 8 * XtalClk;
	Div = PllClk / XtalClk;

	Temp = PllClk - Div * XtalClk; //FoN represents [0, XtalClk)
	FoN = Temp / (XtalClk >> 3);

	Temp = Temp - FoN * (XtalClk >> 3); //FoF represents [0, XtalClk/8)
	FoF = (u32)(((u64)Temp << 13) / (XtalClk >> 3));

	Temp = PLL->PLL_NPPLL_CTRL1;
	Temp &= ~PLL_MASK_NPLL_DIVN_SDM;
	Temp |= PLL_NPLL_DIVN_SDM(Div - 2);
	PLL->PLL_NPPLL_CTRL1 = Temp;

	Temp = PLL->PLL_NPPLL_CTRL3;
	Temp &= ~(PLL_MASK_NPLL_F0F_SDM | PLL_MASK_NPLL_F0N_SDM);
	Temp |= PLL_NPLL_F0F_SDM(FoF) | PLL_NPLL_F0N_SDM(FoN);
	PLL->PLL_NPPLL_CTRL3 = Temp;

	DelayUs(1);

	PLL->PLL_NPPLL_CTRL1 |= PLL_BIT_NPLL_TRIG_FREQ;
	PLL->PLL_NPPLL_CTRL1 &= ~PLL_BIT_NPLL_TRIG_FREQ;
	/*frequency adjustment: 40M wait 20us, 26M wait 31us*/
	DelayUs(60);


}

/**
  * @brief  Enable or disable NP PLL 600M.
  * @param  NewState: DISABLE/ENABLE
  * @note  NP PLL disable should not power off BandGap, since BandGap power supply to ADC, and ADC may in use
  * @note  BandGap power consumption: 100~200uA
  */
void PLL_NP(u32 NewState)
{
	PLL_TypeDef *pll = PLL_BASE;

	if (NewState == ENABLE) {
		//Check BandGap power on
		if ((pll->PLL_AUX_BG & PLL_BG_POW_MASK) == 0) {
			pll->PLL_AUX_BG |= (PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
			DelayUs(20);
			pll->PLL_AUX_BG &= (~PLL_BIT_POW_I);
			DelayUs(140);
		}

		// erc enable
		pll->PLL_NPPLL_CTRL0 |= PLL_BIT_NPLL_POW_ERC;
		DelayUs(1);

		// PLL power on
		pll->PLL_PS |= PLL_BIT_POW_CKGEN;
		pll->PLL_NPPLL_CTRL0 |= PLL_BIT_NPLL_POW_PLL;
		pll->PLL_NPPLL_CTRL0 |= (PLL_BIT_NPLL_CK_EN | PLL_BIT_NPLL_CK_EN_D4);

		// Wait ready
		while (!(pll->PLL_STATE & PLL_BIT_CKRDY_NP));
	} else {
		pll->PLL_NPPLL_CTRL0 &= ~(PLL_BIT_NPLL_CK_EN | PLL_BIT_NPLL_CK_EN_D4);
		pll->PLL_NPPLL_CTRL0 &= ~PLL_BIT_NPLL_POW_PLL;
		pll->PLL_PS &= ~PLL_BIT_POW_CKGEN;

		pll->PLL_NPPLL_CTRL0 &= ~PLL_BIT_NPLL_POW_ERC;

		pll->PLL_AUX_BG &= ~(PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
		pll->PLL_AUX_BG |= PLL_BIT_POW_I;

	}
}

/**
  * @brief  Set PLL clock.
  * @note  only the oscillation frequency from 800M to 1.48G are guaranteed.
  * @param  PllClk: This parameter is used to set PLL frequency, for example PllClk=600000000 means 600MHz
  */
void PLL_AP_ClkSet(u32 PllClk)
{
	APLL_TypeDef *apll = APLL_BASE;

	u32 Rtemp;
	u32 XtalClk = XTAL_ClkGet();

	u32 Div = PllClk / XtalClk - 2;

	assert_param(IS_AP_PLL_SDM_DIVN(Div));

	// set SDM divn
	Rtemp = apll->APLL_CTRL5;
	Rtemp &= ~APLL_MASK_SDM_DIVN;
	Rtemp |= APLL_SDM_DIVN(Div);
	apll->APLL_CTRL5 = Rtemp;

}

/**
  * @brief  Enable or disable AP PLL
  * @param  NewState: DISABLE/ENABLE
  * @note  AP PLL disable should not power off BandGap, since BandGap power supply to ADC, and ADC may in use
  * @note  BandGap power consumption: 100~200uA
  */
void PLL_AP(u32 NewState)
{
	APLL_TypeDef *apll = APLL_BASE;
	PLL_TypeDef *pll = PLL_BASE;

	if (NewState == ENABLE) {

		//Check BandGap power on
		if ((pll->PLL_AUX_BG & PLL_BG_POW_MASK) == 0) {
			pll->PLL_AUX_BG |= (PLL_BIT_POW_BG | PLL_BIT_POW_I | PLL_BIT_POW_MBIAS);
			DelayUs(160);
		}

		//power cut, mbias enable
		apll->APLL_CTRL0 |= (APLL_BIT_POW_MBIAS | APLL_BIT_POW_POWER_CUT);

		DelayUs(1);

		//pll power on, PI function enable
		apll->APLL_CTRL0 |= (APLL_BIT_POW_PLL | APLL_BIT_POW_PI);

		// Wait ready
		while (!(apll->APLL_CTRL1 & APLL_BIT_CK_RDY));
	} else {
		//pll power off
		apll->APLL_CTRL0 &= ~(APLL_BIT_POW_PLL | APLL_BIT_POW_PI);

		//power off
		apll->APLL_CTRL0 &= ~(APLL_BIT_POW_MBIAS | APLL_BIT_POW_POWER_CUT);
	}
}

/**
  * @brief  AP PLL clock adjust.
  * @param  Option: AP PLL SDM div.
  * @note  AP_PLL_CLK =  (Option+2)*40M, so option=0x1C -->1.2G; option=0x17 -->1G
  * @note  Option can be set from 18 to 33, because only the oscillation frequency from 800M to 1.4G are guaranteed.
  */
void PLL_AP_ClkTune(u32 Option)
{
	u32 Rtemp;
	APLL_TypeDef *apll = APLL_BASE;
	assert_param(IS_AP_PLL_SDM_DIVN(Option));

	//pll power off
	apll->APLL_CTRL0 &= ~(APLL_BIT_POW_PLL | APLL_BIT_POW_PI);

	DelayUs(1);

	// set SDM divn
	Rtemp = apll->APLL_CTRL5;
	Rtemp &= ~APLL_MASK_SDM_DIVN;
	Rtemp |= APLL_SDM_DIVN(Option);
	apll->APLL_CTRL5 = Rtemp;

	DelayUs(1);

	//pll power on, PI function enable
	apll->APLL_CTRL0 |= (APLL_BIT_POW_PLL | APLL_BIT_POW_PI);

	// Wait ready
	while (!(apll->APLL_CTRL1 & APLL_BIT_CK_RDY));
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
