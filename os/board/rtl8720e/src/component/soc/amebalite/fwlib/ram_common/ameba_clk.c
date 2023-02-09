/**
  ******************************************************************************
  * @file    ameba_clk.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware rom patch functions to manage clk
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
  * @brief  32K clock Enable,
  * @param  NA
  * @note NA
  */
void SDM32K_Enable(void)
{

	SDM_TypeDef *SDM = SDM_DEV;

	SDM->SDM_CTRL0 |= SDM_BIT_EN | SDM_BIT_RST | SDM_BIT_ALWAYS_CAL_EN | SDM_BIT_TIMER_CAL_EN;
}

/**
  * @brief  32K clock Timer auto calibration Enable,
  * @param  newstatus: can be one of the following values:
  *		 @arg ENABLE: open timer-calibration
  *		 @arg FALSE not open Timer-calibration
  * @note NA
  */
void SDM32K_TimerCalEnable(u32 newstatus)
{

	SDM_TypeDef *SDM = SDM_DEV;
	if (newstatus == ENABLE) {
		SDM->SDM_CTRL0 |= SDM_BIT_TIMER_CAL_EN;
	} else {
		SDM->SDM_CTRL0 &= ~SDM_BIT_TIMER_CAL_EN;
	}
}

/**
  * @brief  Get osc calibration result.
  * @param  Option: can be one of the following values:
  *		 @arg OSC4M_CAL_CLK
  *		 @arg AON100K_CAL_CLK
  *		 @arg AON128K_CAL_CLK
  * @note NA
  */
u32 OSC_CalResult_Get(u8 cal_clk)
{
	u32 temp = 0;

	/* start calibration */
	temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_AIP_CTRL0);
	temp &= ~LSYS_MASK_CK_CAL_SEL;
	temp |= (LSYS_CK_CAL_SEL(cal_clk) | LSYS_BIT_CK_CAL_START);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_AIP_CTRL0, temp);

	/* poll calibration complete */
	while (1) {
		temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_AIP_CTRL0);
		if ((temp & LSYS_BIT_CK_CAL_START) == 0) {
			break;
		}
	}

	/* read calibration result */
	temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_AIP_CTRL0);
	return LSYS_GET_CK_CAL_RPT(temp);
}

/* setbit/clearbit is: BIT0~BIT5 defined in BIT_AON_MASK_OSC32K_RCAL_5_0 */
VOID OSC131_R_Set(u32 setbit, u32 clearbit)
{
	REGU_TypeDef *regu = REGU_BASE;

	if (setbit) {
		regu->REGU_32KOSC |= REGU_RCAL(setbit);
	}

	if (clearbit) {
		regu->REGU_32KOSC &= ~REGU_RCAL(clearbit);
	}

	/* It takes 1ms to stable */
	if (SYSCFG_RLVersion() == SYSCFG_CUT_VERSION_A) {
		DelayMs(2);
	} else {
		DelayMs(1);
	}
}

/**
  * @brief  131.072K calibration
  * @param  max_delta: target_40m_counter-calibration_result, default 10
  * @note Here XTAL clock is used to count N cycles of calibration clock, the total used XTAL clock cycles can be read by this register, N= 8
  */
u32 OSC131K_Calibration(u32 ppm_limit)
{
	u32 temp = 0;
	u32 cal_n = 0;
	u32 rsel_bit_index = 0;
	u32 delta = 0;
	u32 min_delta = 0xFFFFFFFF;
	u32 cur_ppm = 0;
	u32 min_delta_r = 0;
	u32 target_40m_counter = 2441; //cal_rpt=8*40Mhz/fclk_osc131k
	u32 clearbit = 0;
	REGU_TypeDef *regu = REGU_BASE;

	/* Step1: Power on aon 128kHz OSC: 0x4200_8100[0]=1'b1 */
	/* Clear 4m calibration parameter first */
	regu->REGU_32KOSC &= ~REGU_MASK_RCAL;
	while (1) {
		temp = regu->REGU_POWER_CTRL;
		if (temp & REGU_BIT_OSC32K_OK) {
			break;
		}
	}

	/* Step2: Adjust R_CAL. Enter the following loop: each loop decides one bit of 4800002c[13:8]-RCAL, i.e. the first loop decides RCAL[5], the second loop decides RCAL[4] ¡­, suppose the current loop is N (N=1..6) */
	/* Loop Step1: Set RCAL[6-N] = 0x1 */
	/* Loop Step2: Wait some time for clock stable (this wait time decide by SW) */
	/* Loop Step3: Enable Calibration: 0x4800_2840 = 0x8200_0000 */
	/* Loop Step2: Poll 0x4800_2840[31] = 0x0, read calibration result: 0x4800_2840[15:0] */
	/* Loop Step3: */
	/* If(frequency is in the allowed deviation range) */
	/* 	Calibration will end successfully */
	/* Else if (frequency counter > target frequency counter) */ /* (frequency > target frequency) */
	/* 	RCAL[6-N] = 0x0 */
	/* Else */
	/* 	RCAL[6-N] keep 1. */
	/* If (N=6) */
	/* 	Calibration will end, and if the last loop frequency is OK , then calibration is OK, otherwise calibration is fail */
	/* Else */
	/* 	Go to Loop step1 */
	for (cal_n = 1; cal_n <= 6; cal_n++) {
		rsel_bit_index = 6 - cal_n;
		OSC131_R_Set(BIT(rsel_bit_index), clearbit);

		/* read calibration result */
		temp = OSC_CalResult_Get(AON128K_CAL_CLK);

		if (temp < target_40m_counter) {
			delta = target_40m_counter - temp;
			clearbit = BIT(rsel_bit_index);
		} else {
			delta = temp - target_40m_counter;
			clearbit = 0;
		}

		if (delta < min_delta) {
			min_delta = delta;
			min_delta_r = regu->REGU_32KOSC;
			min_delta_r &= REGU_MASK_RCAL;
		}

		//DBG_8195A("[CAL131K]:cal_n %d delta:%d \n", cal_n, delta);
	}

	/* the last one is not the best one */
	if (delta > min_delta) {
		regu->REGU_32KOSC &= ~REGU_MASK_RCAL;
		regu->REGU_32KOSC |= min_delta_r;

		/* It takes 1ms to stable */
		if (SYSCFG_RLVersion() == SYSCFG_CUT_VERSION_A) {
			DelayMs(2);

		} else {
			DelayMs(1);
		}

		/* read calibration result */
		temp = OSC_CalResult_Get(AON128K_CAL_CLK);

		if (temp < target_40m_counter) {
			delta = target_40m_counter - temp;
		} else {
			delta = temp - target_40m_counter;
		}
	}

	cur_ppm = delta * 1000000 / target_40m_counter;
	DBG_8195A("[CAL131K]: delta:%d target:%d PPM: %d PPM_Limit:%d \n", delta, target_40m_counter, cur_ppm, ppm_limit);

	if (cur_ppm >= ppm_limit) {
		DBG_8195A("[CAL131K]: !!! cal fail !!! PPM: %d PPM_Limit:%d \n", cur_ppm, ppm_limit);
		if (SYSCFG_RLVersion() != SYSCFG_CUT_VERSION_A) {
			assert_param(0);
		}
	}

	return TRUE;
}

/* bitnum is: 0~7 defined in REGU_MASK_FREQ_R_SEL */
VOID OSC4M_R_Set(u32 setbit, u32 clearbit)
{
	REGU_TypeDef *regu = REGU_BASE;

	if (setbit) {
		regu->REGU_4MOSC0 |= REGU_FREQ_R_SEL(setbit);
	}

	if (clearbit) {
		regu->REGU_4MOSC0 &= ~REGU_FREQ_R_SEL(clearbit);
	}

	/* It takes 2us to stable */
	DelayUs(2);
}

/* bitnum is: 0~3 defined in REGU_MASK_VCM_SEL_H/L */
VOID OSC4M_VCM_Set(u32 value)
{
	u32 temp;

	REGU_TypeDef *regu = REGU_BASE;
	temp = regu->REGU_4MOSC0;

	temp &= ~(REGU_MASK_VCM_SEL_L | REGU_MASK_VCM_SEL_H);
	temp |= (REGU_VCM_SEL_L(value) | REGU_VCM_SEL_H(value));
	regu->REGU_4MOSC0 = temp;


	/* It takes 2us to stable */
	DelayUs(2);
}

/**
  * @brief  OSC4M calibration
  * @param  ppm_limit: target_40m_counter-calibration_result, default 10
  * @note NA
  */
u32 OSC4M_Calibration(u32 ppm_limit)
{
	u32 temp = 0;
	u32 cal_n = 0;
	u32 rsel_bit_index = 0;
	u32 delta = 0;
	u32 min_delta = 0xFFFFFFFF;
	u32 cur_ppm = 0;
	u32 min_delta_r = 0;
	u32 target_40m_counter = 320; //cal_rpt=32*40Mhz/fclk_osc4m
	u32 clearbit = 0;
	REGU_TypeDef *regu = REGU_BASE;
	/* Step1: Power on aon 4MHz OSC: 0x4200_8100[2]=1'b1 */
	regu->REGU_POWER_CTRL |= REGU_BIT_POW_4MOSC;
	//DBG_8195A("regu->REGU_4MOSC0=0x%x\n",(regu->REGU_4MOSC0));

	/* Step2: Clear 4m calibration parameter first */
	regu->REGU_4MOSC0 &= (u16)(~(REGU_MASK_VCM_SEL_L | REGU_MASK_VCM_SEL_H));
	//DBG_8195A("regu->REGU_4MOSC0=0x%x\n",(regu->REGU_4MOSC0));

	/* Step3: Adjust V_H_SEL & V_L_SEL. Enter the following loop: suppose the current loop is N (N=1..4) */
	/* Loop Step1: Set REGU_VCM_SEL_L[4-N] = 0x1, REGU_VCM_SEL_H[4-N] = 0x1 */
	/* Loop Step2: Enable Calibration, read calibration result*/
	/* Loop Step3: */
	/* If(frequency is in the allowed deviation range)  */
	/* 	Calibration will end successfully */
	/* Else if (frequency counter > target frequency counter) */ /* (frequency < target frequency) */
	/* 	REGU_VCM_SEL_L/H[4-N] = 0x0 */
	/* Else  */
	/* 	REGU_VCM_SEL_L/H[4-N] keep 1. */
	for (cal_n = 5; cal_n <= 9; cal_n++) {

		OSC4M_VCM_Set(cal_n);

		/* read calibration result */
		temp = OSC_CalResult_Get(OSC4M_CAL_CLK);
		//DBG_8195A("[CAL4M]:cal_n %d H:0x%x L:0x%x result:%d ", cal_n,REGU_GET_VCM_SEL_H(regu->REGU_4MOSC0),REGU_GET_VCM_SEL_L(regu->REGU_4MOSC0),temp);

		if (temp < target_40m_counter) {
			delta = target_40m_counter - temp;
			clearbit = 0;
		} else if (temp > target_40m_counter) {
			delta = temp - target_40m_counter;
			clearbit = BIT(rsel_bit_index);
		} else {
			delta = 0;
			goto cal_end;
		}

		if (delta < min_delta) {
			min_delta = delta;
			min_delta_r = regu->REGU_4MOSC0;
			min_delta_r &= (REGU_MASK_VCM_SEL_L | REGU_MASK_VCM_SEL_H | REGU_MASK_FREQ_R_SEL);
		}
		//DBG_8195A("delta:%d min_delta:%d min_delta_r:0x%x \n", delta,min_delta,min_delta_r);
	}

	/* the last one is not the best one */
	if (delta > min_delta) {
		temp = regu->REGU_4MOSC0;
		temp &= ~(REGU_MASK_VCM_SEL_L | REGU_MASK_VCM_SEL_H);
		temp |= min_delta_r;
		regu->REGU_4MOSC0 = temp;
	}
	//DBG_8195A("[CAL4M]:hL last one: 0x%x 0x%x temp:%x \n", REGU_GET_VCM_SEL_H(regu->REGU_4MOSC0),REGU_GET_VCM_SEL_L(regu->REGU_4MOSC0),temp);
	/* Step4: Adjust R_SEL. Enter the following loop: suppose the current loop is N (N=1..8) */
	/* Clear 4m calibration parameter first */
	/* Loop Step1: Set REGU_FREQ_R_SEL[8-N] = 0x1 */
	/* Loop Step2: Enable Calibration, read calibration result*/
	/* Loop Step3: */
	/* If(frequency is in the allowed deviation range)  */
	/* 	Calibration will end successfully */
	/* Else if (frequency counter > target frequency counter) */ /* (frequency < target frequency) */
	/* 	REGU_FREQ_R_SEL[8-N] = 0x0 */
	/* Else  */
	/* 	REGU_FREQ_R_SEL[8-N] keep 1. */
	clearbit = 0;
	regu->REGU_4MOSC0 &= (u16)(~REGU_MASK_FREQ_R_SEL);
	for (cal_n = 1; cal_n <= 8; cal_n++) {
		rsel_bit_index = 8 - cal_n;
		OSC4M_R_Set(BIT(rsel_bit_index), clearbit);

		/* read calibration result */
		temp = OSC_CalResult_Get(OSC4M_CAL_CLK);
		//DBG_8195A("[CAL4M]:cal_n %d R:0x%x result:%d ", cal_n,REGU_GET_FREQ_R_SEL(regu->REGU_4MOSC0), temp, target_40m_counter, delta);

		if (temp < target_40m_counter) {
			delta = target_40m_counter - temp;
			clearbit = 0;
		} else if (temp > target_40m_counter) {
			delta = temp - target_40m_counter;
			clearbit = BIT(rsel_bit_index);
		}  else {
			delta = 0;
			goto cal_end;
		}

		if (delta < min_delta) {
			min_delta = delta;
			min_delta_r = regu->REGU_4MOSC0;
			min_delta_r &= REGU_MASK_FREQ_R_SEL;
		}
		//DBG_8195A("delta:%d \n", delta);
	}

	/* the last one is not the best one */
	if (delta > min_delta) {
		temp = regu->REGU_4MOSC0;
		temp &= ~REGU_MASK_FREQ_R_SEL;
		temp |= min_delta_r;
		regu->REGU_4MOSC0 = temp;

		/* It takes 2us to stable */
		DelayUs(2) ;

		/* read calibration result */
		temp = OSC_CalResult_Get(OSC4M_CAL_CLK);
		//DBG_8195A("OSC4M_Calibration:cal_n %d result:%d target:%d delta:%d\n", cal_n, temp, target_40m_counter, delta);

		if (temp < target_40m_counter) {
			delta = target_40m_counter - temp;
		} else {
			delta = temp - target_40m_counter;
		}
	}
	//DBG_8195A("[CAL4M]:R last one: 0x%x H:0x%x L:0x%x \n",REGU_GET_FREQ_R_SEL(regu->REGU_4MOSC0),REGU_GET_VCM_SEL_H(regu->REGU_4MOSC0),REGU_GET_VCM_SEL_H(regu->REGU_4MOSC0));

cal_end:
	cur_ppm = delta * 1000000 / target_40m_counter;
	DBG_8195A("[CAL4M]: delta:%d target:%d PPM: %d PPM_Limit:%d \n", delta, target_40m_counter, cur_ppm, ppm_limit);

	if (cur_ppm >= ppm_limit) {
		DBG_8195A("[CAL4M]: PPM: %d PPM_Limit:%d \n", cur_ppm, ppm_limit);
		assert_param(0);
	}

	return TRUE;
}

/**
  * @brief  OSC4M Init
  * @param  NA
  * @note NA
  */
void OSC4M_Init(void)
{

}
/**
  * @brief  XTAL Init
  * @note This is the special setting of amebalite. Special settings are required because the XTAL default parameter is not optimal.
  */
void XTAL_INIT(void)
{
	XTAL_TypeDef *xtal = XTAL_BASE;

	xtal->XTAL_ANAPAR_XTAL_ON_0 = 0x008103FF; /*04h*/
	xtal->XTAL_ANAPAR_XTAL_ON_1 = 0xFC3B9D80; /*08h*/
	xtal->XTAL_ANAPAR_XTAL_ON_2 = 0x00000BE9; /*0ch*/
	xtal->XTAL_ANAPAR_XTAL_OFF_0 = 0xCC0FFF90; /*10h*/
	xtal->XTAL_ANAPAR_XTAL_OFF_1 = 0x00000068; /*14h*/

	xtal->XTAL_FEN = 0x00000004; /*1ch*/
	xtal->XTAL_ANAPAR_XTAL_PDCK = 0x000DFFD0; /*20h*/
	xtal->XTAL_ANAPAR_XTAL_AAC = 0xC7FEFE04; /*24h*/
	xtal->XTAL_ANAPAR_XTAL_AAC_ON_1 = 0x0000003C; /*28h*/
	xtal->XTAL_ANAPAR_XTAL_MODE_DEC_ON_0 = 0x988365E2; /*2ch*/

}

void XTAL_AACK(void)
{
	u32 temp;
	u32 xtal_mode;
	XTAL_TypeDef *xtal = XTAL_BASE;

	//XTAL must be high performance mode
	xtal_mode = XTAL_GET_MODE_DEC(xtal->XTAL_ANAPAR_XTAL_MODE_DEC_ON_1);
	if (xtal_mode != XTAL_HP_MODE) {
		DBG_8195A("XTAL is not in HP mode:%x\n", xtal_mode);
	}

	//open clk for acc
	xtal->XTAL_ANAPAR_XTAL_ON_1 |= XTAL_BIT_EN_XTAL_DRV_DIGI;

	//reset_n_aac

	xtal->XTAL_FEN &= ~XTAL_BIT_FEN_XAACK;
	xtal->XTAL_FEN |= XTAL_BIT_FEN_XAACK;

	//only K as one set of GM values
	xtal->XTAL_ANAPAR_XTAL_AAC_ON_1 |= (XTAL_MASK_PK_START_AR | XTAL_MASK_PK_END_AR);

	xtal->XTAL_ANAPAR_XTAL_ON_2 |= XTAL_MASK_AAC_MODE;

	//EN_XTAL_AAC_PKDET
	xtal->XTAL_ANAPAR_XTAL_OFF_0 |= XTAL_BIT_EN_XTAL_AAC_PKDET;
	DelayUs(50);
	//EN_XTAL_AAC_DIGI
	xtal->XTAL_ANAPAR_XTAL_AAC |= XTAL_BIT_EN_XTAL_AAC_DIGI;
	DelayUs(50);
	//EN_XTAL_AAC_GM
	xtal->XTAL_ANAPAR_XTAL_OFF_0 |= XTAL_BIT_EN_XTAL_AAC_GM;
	DelayUs(50);

	while (1) {
		temp = xtal->XTAL_ANAPAR_XTAL_OFF_0 ;
		if (temp & XTAL_BIT_EN_XTAL_AAC_GM) {
			break;
		} else {
			xtal->XTAL_ANAPAR_XTAL_OFF_0 |= XTAL_BIT_EN_XTAL_AAC_GM;
		}
	}

}

void XTAL_PDCK(void)

{
	XTAL_TypeDef *xtal = XTAL_BASE;

	//reset_n_aac

	xtal->XTAL_FEN &= ~XTAL_BIT_FEN_XPDCK;
	xtal->XTAL_FEN |= XTAL_BIT_FEN_XPDCK;


	//EN_XTAL_PDCK_DIGI
	xtal->XTAL_ANAPAR_XTAL_PDCK |= XTAL_BIT_EN_XTAL_PDCK_DIGI;

}

/**
  * @brief  Set CPU clock Source.
  * @param  Source: This parameter can be one of the following values:
  *		 @arg CLK_CPU_DPLL
  *		 @arg CLK_CPU_MPLL
  *		 @arg CLK_CPU_LBUS
   *		 @arg CLK_CPU_XTAL
  */
void CPU_ClkSet_NonOS(u32 Source)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0);

	/* 1. Select CPU clock source */
	if (Source != LSYS_GET_CKSL_PLFM(Temp)) {
		Temp &= ~(LSYS_MASK_CKSL_PLFM);
		Temp |=  LSYS_CKSL_PLFM(Source);
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0, Temp);

		__DSB();
		__NOP();
	}

	/* 2. update for DelayUs */
	RBSS_UDELAY_CLK = CPU_ClkGet() / MHZ_TICK_CNT;
}


/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
