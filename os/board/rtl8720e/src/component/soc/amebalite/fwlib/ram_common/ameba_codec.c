/**
  ******************************************************************************
  * @file    rtl8721dhp_audio.c
  * @author
  * @version V1.0.0
  * @date    2017-12-13
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Audio codec peripheral:
  *           - SPORT Initialization
  *           - SPORT parameters management
  *           - Data transfers configuration
  *           - GDMA configuration
  *
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
#include "ameba_vadbuf.h"
#include "sysreg_lsys.h"
#include "sysreg_pll.h"
#include "sysreg_pmc.h"
#include "hal_platform.h"

AUD_TypeDef	*AUD = AUD_SYS_BASE;

void AUDIO_CODEC_StructInit(CODEC_InitTypeDef *CODEC_InitStruct)
{
	CODEC_InitStruct->CODEC_SelI2STxSR = SR_48K;
	CODEC_InitStruct->CODEC_SelI2STxWordLen = WL_16;
	CODEC_InitStruct->CODEC_SelI2STxCHLen = CL_32;
	CODEC_InitStruct->CODEC_SelI2STxDataFormat = DF_I2S;
	CODEC_InitStruct->CODEC_SelI2SRxSR = SR_48K;
	CODEC_InitStruct->CODEC_SelI2SRxWordLen = WL_16;
	CODEC_InitStruct->CODEC_SelI2SRxCHLen = CL_32;
	CODEC_InitStruct->CODEC_SelI2SRxDataFormat = DF_I2S;
	CODEC_InitStruct->CODEC_SelRxI2STdm = I2S_NOTDM;
}

/**
  * @brief  Mute ADC input path.
  * @param  tdmmode: select adc input path channel
  *			 This parameter can be one of the following values:
  *			   @arg I2S_NOTDM
  *			   @arg I2S_TDM4
  * @param  NewState: mute or unmute option for ad channel
  *			 This parameter can be one of the following values:
  *			   @arg UNMUTE
  *			   @arg MUTE
  * @return  None
  */
static void AUDIO_CODEC_MuteADCInput(u32 NewState, u32 tdmmode)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (NewState == UNMUTE) {
		if ((tdmmode == I2S_NOTDM) || (tdmmode == I2S_TDM4)) {
			Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
			Tmp &= ~(AUD_BIT_ADC_0_AD_MIX_MUTE | AUD_MASK_ADC_0_AD_SRC_SEL);
			Tmp |=	AUD_ADC_0_AD_SRC_SEL(0) | AUD_BIT_ADC_0_DCHPF_EN;
			HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);

			Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
			Tmp &= ~(AUD_BIT_ADC_1_AD_MIX_MUTE | AUD_MASK_ADC_1_AD_SRC_SEL);
			Tmp |=	AUD_ADC_1_AD_SRC_SEL(1) | AUD_BIT_ADC_1_DCHPF_EN;
			HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);
		}

		if ((tdmmode == I2S_TDM4)) {
			Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
			Tmp &= ~(AUD_BIT_ADC_2_AD_MIX_MUTE | AUD_MASK_ADC_2_AD_SRC_SEL);
			Tmp |=	AUD_ADC_2_AD_SRC_SEL(2) | AUD_BIT_ADC_2_DCHPF_EN;
			HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);

			Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
			Tmp &= ~(AUD_BIT_ADC_3_AD_MIX_MUTE | AUD_MASK_ADC_3_AD_SRC_SEL);
			Tmp |=	AUD_ADC_3_AD_SRC_SEL(3) | AUD_BIT_ADC_3_DCHPF_EN;
			HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
		}

	} else {

		if ((tdmmode == I2S_NOTDM) || (tdmmode == I2S_TDM4)) {
			Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
			Tmp |= AUD_BIT_ADC_0_AD_MIX_MUTE;
			HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);

			Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
			Tmp |= AUD_BIT_ADC_1_AD_MIX_MUTE;
			HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);
		}

		if ((tdmmode == I2S_TDM4)) {
			Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
			Tmp |= AUD_BIT_ADC_2_AD_MIX_MUTE;
			HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);

			Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
			Tmp |= AUD_BIT_ADC_3_AD_MIX_MUTE;
			HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
		}
	}
}

/**
  * @brief  mute or unmute  dmic input path; if unmute: enable dchpg and select dmic src.
  * @param  tdmmode: select adc input path channel
  *			 This parameter can be one of the following values:
  *			   @arg I2S_NOTDM
  *			   @arg I2S_TDM4
  * @param  NewState: mute or unmute option for ad channel
  *			 This parameter can be one of the following values:
  *			   @arg UNMUTE
  *			   @arg MUTE
  * @return  None
  */
static void AUDIO_CODEC_MuteDmicInput(u32 NewState, u32 tdmmode)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (NewState == UNMUTE) {
		if ((tdmmode == I2S_NOTDM) || (tdmmode == I2S_TDM4)) {
			Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
			Tmp &= ~(AUD_BIT_ADC_0_DMIC_MIX_MUTE | AUD_MASK_ADC_0_DCHPF_FC_SEL | AUD_MASK_ADC_0_DCHPF_FC_SEL);
			Tmp |= AUD_BIT_ADC_0_DCHPF_EN | AUD_ADC_0_DCHPF_FC_SEL(7);
			HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);

			Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
			Tmp &= ~(AUD_BIT_ADC_1_DMIC_MIX_MUTE | AUD_MASK_ADC_1_DMIC_SRC_SEL | AUD_MASK_ADC_1_DCHPF_FC_SEL);
			Tmp |= ((AUD_ADC_1_DMIC_SRC_SEL(1)) | AUD_BIT_ADC_1_DCHPF_EN | (AUD_ADC_1_DCHPF_FC_SEL(7)));
			HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);
		}

		if ((tdmmode == I2S_TDM4)) {
			Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
			Tmp &= ~(AUD_BIT_ADC_2_DMIC_MIX_MUTE | AUD_MASK_ADC_2_DMIC_SRC_SEL | AUD_MASK_ADC_2_DCHPF_FC_SEL);
			Tmp |= (AUD_ADC_2_DMIC_SRC_SEL(2) | AUD_BIT_ADC_2_DCHPF_EN | (AUD_ADC_2_DCHPF_FC_SEL(7)));
			HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);

			Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
			Tmp &= ~(AUD_BIT_ADC_3_DMIC_MIX_MUTE | AUD_MASK_ADC_3_DMIC_SRC_SEL | AUD_MASK_ADC_3_DCHPF_FC_SEL);
			Tmp |= ((AUD_ADC_3_DMIC_SRC_SEL(3)) | AUD_BIT_ADC_3_DCHPF_EN | (AUD_ADC_3_DCHPF_FC_SEL(7)));
			HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
		}
	} else {

		if ((tdmmode == I2S_NOTDM) || (tdmmode == I2S_TDM4)) {
			Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
			Tmp |= AUD_BIT_ADC_0_DMIC_MIX_MUTE;
			HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);

			Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
			Tmp |= AUD_BIT_ADC_1_DMIC_MIX_MUTE;
			HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);
		}

		if ((tdmmode == I2S_TDM4)) {
			Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
			Tmp |= AUD_BIT_ADC_2_DMIC_MIX_MUTE;
			HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);

			Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
			Tmp |= AUD_BIT_ADC_3_DMIC_MIX_MUTE;
			HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
		}

	}
}

/**
  * @brief  Select i2s rx and i2s rx tdm mode
  * @param  i2s: select i2s: i2s0 or i2s1
  *			 This parameter can be one of the following values:
  *			   @arg I2S0
  * @param  tdmmode: select adc input path channel
  *			 This parameter can be one of the following values:
  *			   @arg I2S_NOTDM
  *			   @arg I2S_TDM4
  * @return  None
  */
static void AUDIO_CODEC_SetI2SRXTDM(u32 tdmmode)
{
	u32 Tmp1;
	u32 Tmp2;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp1 = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
	Tmp1 &= ~(AUD_MASK_I2S_0_TDM_MODE_RX);
	Tmp2 = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL_1);

	if (tdmmode == I2S_NOTDM) {
		Tmp1 |= AUD_I2S_0_TDM_MODE_RX(0);
		Tmp2 &= ~(AUD_MASK_I2S_0_DATA_CH0_SEL_RX | AUD_MASK_I2S_0_DATA_CH1_SEL_RX);
		Tmp2 |= (AUD_I2S_0_DATA_CH0_SEL_RX(0) | AUD_I2S_0_DATA_CH1_SEL_RX(1) | AUD_BIT_I2S_0_DATA_CH2_RX_DISABLE | AUD_BIT_I2S_0_DATA_CH3_RX_DISABLE);
	} else {
		Tmp1 |= AUD_I2S_0_TDM_MODE_RX(1);
		Tmp2 &= ~(AUD_MASK_I2S_0_DATA_CH0_SEL_RX | AUD_MASK_I2S_0_DATA_CH1_SEL_RX | AUD_MASK_I2S_0_DATA_CH2_SEL_RX | AUD_MASK_I2S_0_DATA_CH3_SEL_RX);
		Tmp2 |= (AUD_I2S_0_DATA_CH0_SEL_RX(0) | AUD_I2S_0_DATA_CH1_SEL_RX(1) | AUD_I2S_0_DATA_CH2_SEL_RX(2) | AUD_I2S_0_DATA_CH3_SEL_RX(3));
	}
	HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp1);
	HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL_1, Tmp2);
}

/**
  * @brief  Enable and select ADC channel and ADC filter channel;
  * @param  tdmmode: select adc input path channel
  *			 This parameter can be one of the following values:
  *			   @arg I2S_NOTDM
  *			   @arg I2S_TDM4
  * @return  None
  */
static void AUDIO_CODEC_SelADCChannel(u32 tdmmode)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (tdmmode == I2S_NOTDM) {
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);
		Tmp |= AUD_BIT_AD_0_EN | AUD_BIT_AD_1_EN | AUD_BIT_AD_0_FIFO_EN | AUD_BIT_AD_1_FIFO_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_2);
		Tmp |= AUD_BIT_AD_ANA_0_EN | AUD_BIT_AD_ANA_1_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_2, Tmp);
	} else {
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);
		Tmp |= AUD_BIT_AD_0_EN | AUD_BIT_AD_1_EN | AUD_BIT_AD_0_FIFO_EN | AUD_BIT_AD_1_FIFO_EN
			   | AUD_BIT_AD_2_EN | AUD_BIT_AD_3_EN | AUD_BIT_AD_2_FIFO_EN | AUD_BIT_AD_3_FIFO_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_2);
		Tmp |= AUD_BIT_AD_ANA_0_EN | AUD_BIT_AD_ANA_1_EN | AUD_BIT_AD_ANA_2_EN | AUD_BIT_AD_ANA_3_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_2, Tmp);
	}
}

/**
  * @brief  Enable and select DMIC channel and ADC filter channel;
  * @param  tdmmode: select dmic channel
  *			 This parameter can be one of the following values:
  *			   @arg I2S_NOTDM
  *			   @arg I2S_TDM4
  * @return  None
  */
static void AUDIO_CODEC_SelDmicChannel(u32 tdmmode)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (tdmmode == I2S_NOTDM) {

		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);
		Tmp |= AUD_BIT_AD_0_EN | AUD_BIT_AD_1_EN | AUD_BIT_AD_0_FIFO_EN | AUD_BIT_AD_1_FIFO_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_2);
		Tmp |= AUD_BIT_DMIC_0_EN | AUD_BIT_DMIC_1_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_2, Tmp);

	} else {

		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);
		Tmp |= AUD_BIT_AD_0_EN | AUD_BIT_AD_1_EN | AUD_BIT_AD_0_FIFO_EN | AUD_BIT_AD_1_FIFO_EN
			   | AUD_BIT_AD_2_EN | AUD_BIT_AD_3_EN | AUD_BIT_AD_2_FIFO_EN | AUD_BIT_AD_3_FIFO_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_2);
		Tmp |= AUD_BIT_DMIC_0_EN | AUD_BIT_DMIC_1_EN | AUD_BIT_DMIC_2_EN | AUD_BIT_DMIC_3_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_2, Tmp);
	}
}

void AUDIO_CODEC_VREF_Calib(void)
{
	if (SWR_Calib_Get_Value_AUD() == Swr_Calib_Nocalib) {
		return;
	} else {
		AUD->AUD_MICBST_CTL1 &= ~(AUD_MASK_LDO_TUNE);
		AUD->AUD_MICBST_CTL1 |= AUD_LDO_TUNE(SWR_Calib_Get_Value_AUD());
	}

}

/**
  * @brief Audio Codec LDO power on or power down.
  * @param  ldo_mode: LDO power on or power down
  *          This parameter can be one of the following values:
  *            @arg LDO_POWER_ON:
  *            @arg LDO_POWER_DOWN:
  * @return  None
  */
void AUDIO_CODEC_SetLDOMode(u32 ldo_mode)
{
	if (ldo_mode == LDO_POWER_ON) {
		AUD->AUD_ADDA_CTL |= AUD_BIT_POWADDACK;
		/*LDO power on*/
		AUD->AUD_MICBIAS_CTL0 |= (AUD_BIT_MBIAS_POW);
		AUD->AUD_MICBST_CTL1 |= AUD_BIT_LDO_POW | AUD_BIT_LDO_PREC;
		DelayMs(2);
		AUD->AUD_MICBST_CTL1 &= ~(AUD_BIT_LDO_PREC);
		DelayUs(10);
		AUD->AUD_MICBST_CTL1 |= AUD_BIT_LDO_POW_0P9V;
		DelayUs(990);
		AUD->RSVD0 &= ~AUD_BIT_LDO_CODEC_DRIVING_PATH;
		AUD->RSVD0 &= ~AUD_BIT_LDO_CODEC_DISCHARGE_PATH;
		AUDIO_CODEC_VREF_Calib();

	} else {
		AUD->AUD_ADDA_CTL &= ~AUD_BIT_POWADDACK;
		AUD->AUD_MICBST_CTL1 &= ~(AUD_BIT_LDO_POW_0P9V);
		DelayUs(10);
		AUD->AUD_MICBST_CTL1 &= ~(AUD_BIT_LDO_POW);
		AUD->AUD_MICBST_CTL1 |= AUD_BIT_LDO_PREC;
		AUD->RSVD0 |= AUD_BIT_LDO_CODEC_DISCHARGE_PATH;
		DelayMs(9);
		AUD->AUD_MICBST_CTL1 &= ~(AUD_BIT_LDO_PREC);
		AUD->RSVD0 &= ~AUD_BIT_LDO_CODEC_DISCHARGE_PATH;
		AUD->AUD_MICBIAS_CTL0 &= ~(AUD_BIT_MBIAS_POW);
	}
}

void AUDIO_CODEC_DisPAD(void)
{
	u32 Tmp;
	u32 pin_base;

	/*disable PAD*/
	Pinmux_Config(_PB_11, PINMUX_FUNCTION_AUDIO);
	Pinmux_Config(_PB_12, PINMUX_FUNCTION_AUDIO);
	Pinmux_Config(_PB_13, PINMUX_FUNCTION_AUDIO);
	Pinmux_Config(_PB_14, PINMUX_FUNCTION_AUDIO);
	Pinmux_Config(_PB_15, PINMUX_FUNCTION_AUDIO);
	Pinmux_Config(_PB_16, PINMUX_FUNCTION_AUDIO);
	Pinmux_Config(_PB_17, PINMUX_FUNCTION_AUDIO);
	Pinmux_Config(_PB_18, PINMUX_FUNCTION_AUDIO);
	Pinmux_Config(_PB_19, PINMUX_FUNCTION_AUDIO);

	if (TrustZone_IsSecure()) {
		pin_base = PINMUX_REG_BASE_S;
	} else {
		pin_base = PINMUX_REG_BASE;
	}

	Tmp = HAL_READ32(pin_base, REG_PAD_AUD_PAD_CTRL);
	Tmp &= ~PAD_MASK_GPIOE_IE;
	HAL_WRITE32(pin_base, REG_PAD_AUD_PAD_CTRL, Tmp);

}

void AUDIO_CODEC_Init(u32 application, CODEC_InitTypeDef *CODEC_InitStruct)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	/*Enable audio IP*/
	Tmp = HAL_READ32(audio_base, CODEC_AUDIO_CONTROL_0);
	Tmp |= AUD_BIT_AUDIO_IP_EN;
	HAL_WRITE32(audio_base, CODEC_AUDIO_CONTROL_0, Tmp);

	AUDIO_CODEC_DisPAD();

	/*DAC source0,sample rate*/
	Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_5);
	Tmp &= ~ AUD_MASK_DAC_L_FS_SRC_SEL;
	Tmp |= AUD_DAC_L_FS_SRC_SEL(0);
	HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_5, Tmp);

	/*ADC source1, sample rate*/
	Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_5);
	Tmp &= ~(AUD_MASK_ADC_0_FS_SRC_SEL | AUD_MASK_ADC_1_FS_SRC_SEL);
	Tmp |= AUD_ADC_0_FS_SRC_SEL(1) | AUD_ADC_1_FS_SRC_SEL(1) | AUD_ADC_2_FS_SRC_SEL(1) | AUD_ADC_3_FS_SRC_SEL(1);
	HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_5, Tmp);

	/*Enable I2S0*/
	Tmp = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
	Tmp |= AUD_BIT_I2S_0_RST_N_REG;
	HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp);

	/* ================= CODEC initialize ======================== */
	if (application == APP_DAAD_LPBK) {
		/*Enable loopback mode*/
		Tmp = HAL_READ32(audio_base, CODEC_AUDIO_CONTROL_0);
		Tmp |= AUD_BIT_DAAD_LPBK_EN;
		HAL_WRITE32(audio_base, CODEC_AUDIO_CONTROL_0, Tmp);

		/*CODEC: DAC part*/
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);
		Tmp |= AUD_BIT_DA_L_EN | AUD_BIT_MOD_L_EN | AUD_BIT_DA_ANA_CLK_EN | AUD_BIT_DA_FIFO_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_0);
		Tmp &= ~AUD_BIT_DAC_L_DA_SRC_SEL;
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_0, Tmp);

		/*I2S Select and config: DAC part*/
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_0;
		Tmp |= AUD_SAMPLE_RATE_0(CODEC_InitStruct->CODEC_SelI2STxSR);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
		Tmp &= ~(AUD_MASK_I2S_0_CH_LEN_SEL_TX | AUD_MASK_I2S_0_DATA_LEN_SEL_TX | AUD_MASK_I2S_0_DATA_FORMAT_SEL_TX | AUD_MASK_I2S_0_DATA_CH_SEL_TX);
		Tmp |= (AUD_I2S_0_CH_LEN_SEL_TX(CODEC_InitStruct->CODEC_SelI2STxCHLen) | AUD_I2S_0_DATA_LEN_SEL_TX(CODEC_InitStruct->CODEC_SelI2STxWordLen)
				| AUD_I2S_0_DATA_FORMAT_SEL_TX(CODEC_InitStruct->CODEC_SelI2STxDataFormat));
		HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp);

		/*I2S Select and config: ADC part*/
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_1;
		Tmp |= AUD_SAMPLE_RATE_1(CODEC_InitStruct->CODEC_SelI2SRxSR);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
		Tmp &= ~(AUD_MASK_I2S_0_CH_LEN_SEL_RX | AUD_MASK_I2S_0_DATA_LEN_SEL_RX | AUD_MASK_I2S_0_DATA_FORMAT_SEL_RX);
		Tmp |= (AUD_I2S_0_CH_LEN_SEL_RX(CODEC_InitStruct->CODEC_SelI2SRxCHLen) | AUD_I2S_0_DATA_LEN_SEL_RX(CODEC_InitStruct->CODEC_SelI2SRxWordLen)
				| AUD_I2S_0_DATA_FORMAT_SEL_RX(CODEC_InitStruct->CODEC_SelI2SRxDataFormat));
		HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp);

		/*CODEC: ADC part*/
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);
		Tmp |= AUD_BIT_AD_ANA_CLK_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);

		AUDIO_CODEC_SetI2SRXTDM(CODEC_InitStruct->CODEC_SelRxI2STdm);
		AUDIO_CODEC_MuteADCInput(DISABLE, CODEC_InitStruct->CODEC_SelRxI2STdm);
		AUDIO_CODEC_SelADCChannel(CODEC_InitStruct->CODEC_SelRxI2STdm);
	}

	/*default: HPO enable single out*/
	if (application == APP_LINE_OUT) {

		/*DAC part:需放在analog配置之前以避免pop音问题*/
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);
		Tmp |= AUD_BIT_DA_L_EN | AUD_BIT_MOD_L_EN | AUD_BIT_DA_ANA_CLK_EN | AUD_BIT_DA_FIFO_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_0);
		Tmp &= ~AUD_BIT_DAC_L_DA_SRC_SEL;
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_0, Tmp);

		/*I2S Select and config*/
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_0;
		Tmp |= AUD_SAMPLE_RATE_0(CODEC_InitStruct->CODEC_SelI2STxSR);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
		Tmp &= ~(AUD_MASK_I2S_0_CH_LEN_SEL_TX | AUD_MASK_I2S_0_DATA_LEN_SEL_TX | AUD_MASK_I2S_0_DATA_FORMAT_SEL_TX | AUD_MASK_I2S_0_DATA_CH_SEL_TX);
		Tmp |= (AUD_I2S_0_CH_LEN_SEL_TX(CODEC_InitStruct->CODEC_SelI2STxCHLen) | AUD_I2S_0_DATA_LEN_SEL_TX(CODEC_InitStruct->CODEC_SelI2STxWordLen)
				| AUD_I2S_0_DATA_FORMAT_SEL_TX(CODEC_InitStruct->CODEC_SelI2STxDataFormat));
		HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp);

		/*analog*/
		AUD->AUD_ADDA_CTL &= ~(AUD_BIT_DAC_CKXSEL);
		AUD->AUD_ADDA_CTL |= AUD_BIT_DAC_CKXEN | AUD_BIT_DAC_POW;

		AUD->AUD_LO_CTL &= ~(AUD_MASK_LO_M | AUD_BIT_LO_SE);
		AUD->AUD_LO_CTL |= AUD_LO_M(2) | AUD_BIT_LO_POW;

		/*differential output: depop flow*/
		AUD->AUD_LO_CTL |= AUD_BIT_LO_OPPDP;
		AUD->AUD_ADDA_CTL |= AUD_BIT_DPRAMP_POW;
		DelayUs(100);
		AUD->AUD_ADDA_CTL |= AUD_BIT_DPRAMP_ENRAMP;
		DelayMs(500);
		AUD->AUD_LO_CTL &= ~(AUD_BIT_LO_OPPDP);
		DelayUs(100);
		AUD->AUD_ADDA_CTL &= ~(AUD_BIT_DPRAMP_POW | AUD_BIT_DPRAMP_ENRAMP);
		DelayMs(1);
		AUD->AUD_LO_CTL |= AUD_BIT_LO_MDP | AUD_BIT_LO_OPNDP;
		AUD->AUD_ADDA_CTL |= AUD_BIT_DPRAMP_POW;
		DelayUs(100);
		AUD->AUD_ADDA_CTL |= AUD_BIT_DPRAMP_ENRAMP;
		DelayMs(500);
		AUD->AUD_LO_CTL &= ~(AUD_BIT_LO_MDP | AUD_BIT_LO_OPNDP);
		DelayUs(100);
		AUD->AUD_ADDA_CTL &= ~(AUD_BIT_DPRAMP_POW | AUD_BIT_DPRAMP_ENRAMP);
		AUD->AUD_LO_CTL &= ~(AUD_BIT_LO_ENDP);

	}

	if (application == APP_DMIC_IN) {
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_3);
		Tmp &= ~ AUD_MASK_DMIC1_CLK_SEL;
		Tmp |= AUD_BIT_DMIC1_CLK_EN | AUD_DMIC1_CLK_SEL(DMIC_2P5M);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_3, Tmp);
		/* Dmic delay: steady time*/
		DelayMs(200);

		Tmp = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
		Tmp &= ~(AUD_MASK_I2S_0_CH_LEN_SEL_RX | AUD_MASK_I2S_0_DATA_LEN_SEL_RX | AUD_MASK_I2S_0_DATA_FORMAT_SEL_RX);
		Tmp |= (AUD_I2S_0_CH_LEN_SEL_RX(CODEC_InitStruct->CODEC_SelI2SRxCHLen) | AUD_I2S_0_DATA_LEN_SEL_RX(CODEC_InitStruct->CODEC_SelI2SRxWordLen)
				| AUD_I2S_0_DATA_FORMAT_SEL_RX(CODEC_InitStruct->CODEC_SelI2SRxDataFormat));
		HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_1;
		Tmp |= AUD_SAMPLE_RATE_1(CODEC_InitStruct->CODEC_SelI2SRxSR);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);

		AUDIO_CODEC_SetI2SRXTDM(CODEC_InitStruct->CODEC_SelRxI2STdm);
		AUDIO_CODEC_MuteDmicInput(DISABLE, CODEC_InitStruct->CODEC_SelRxI2STdm);
		AUDIO_CODEC_SelDmicChannel(CODEC_InitStruct->CODEC_SelRxI2STdm);
	}

	if (application == APP_AMIC_IN) {/*Normal mode: 默认2amic*/
		/*Micbias enable*/
		AUD->AUD_MICBIAS_CTL0 |= AUD_BIT_MICBIAS1_POW;
		/*Micbias power: 2channel*/
		AUD->AUD_MICBST_CTL0 |= AUD_BIT_MICBST_POWL | AUD_BIT_MICBST_POWR;
		/*DTSDM Enable*/
		AUD->AUD_ADDA_CTL |= AUD_BIT_DTSDM_CKXEN | AUD_BIT_DTSDM_POW_L | AUD_BIT_DTSDM_POW_R;
		/*default:5 amic differential mode*/
		AUD->AUD_MICBST_CTL0 |= AUD_BIT_MICBST_ENDFL | AUD_BIT_MICBST_ENDFR;
		AUD->AUD_MICBST_CTL1 |= AUD_BIT_MICBST3_ENDF;
		/*micbst gain select: default 30dB*/
		AUD->AUD_MICBST_CTL0 &= ~(AUD_MASK_MICBST_GSELL | AUD_MASK_MICBST_GSELR);
		AUD->AUD_MICBST_CTL0 |= (AUD_MICBST_GSELL(MICBST_GAIN_30DB) | AUD_MICBST_GSELR(MICBST_GAIN_30DB));
		/*unmute amic enable*/
		AUD->AUD_MICBST_CTL0 &= ~(AUD_MASK_MICBST_MUTE_L | AUD_MASK_MICBST_MUTE_R);
		AUD->AUD_MICBST_CTL0 |= ((AUD_MICBST_MUTE_L(2)) | (AUD_MICBST_MUTE_R(2)));
		Tmp = HAL_READ32(audio_base, CODEC_AUDIO_CONTROL_1); //0x00
		Tmp |= AUD_BIT_CKX_MICBIAS_EN;
		HAL_WRITE32(audio_base, CODEC_AUDIO_CONTROL_1, Tmp);
		AUD->AUD_MICBIAS_CTL0 &= ~(AUD_MASK_MICBIAS1_VSET);
		AUD->AUD_MICBIAS_CTL0 |= AUD_MICBIAS1_VSET(7);
		/*ADC part*/
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);
		Tmp |= AUD_BIT_AD_ANA_CLK_EN | AUD_BIT_DA_ANA_CLK_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);


		Tmp = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
		Tmp &= ~(AUD_MASK_I2S_0_CH_LEN_SEL_RX | AUD_MASK_I2S_0_DATA_LEN_SEL_RX | AUD_MASK_I2S_0_DATA_FORMAT_SEL_RX);
		Tmp |= (AUD_I2S_0_CH_LEN_SEL_RX(CODEC_InitStruct->CODEC_SelI2SRxCHLen) | AUD_I2S_0_DATA_LEN_SEL_RX(CODEC_InitStruct->CODEC_SelI2SRxWordLen)
				| AUD_I2S_0_DATA_FORMAT_SEL_RX(CODEC_InitStruct->CODEC_SelI2SRxDataFormat));
		HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_1;
		Tmp |= AUD_SAMPLE_RATE_1(CODEC_InitStruct->CODEC_SelI2SRxSR);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);

		AUDIO_CODEC_SetI2SRXTDM(CODEC_InitStruct->CODEC_SelRxI2STdm);
		AUDIO_CODEC_MuteADCInput(DISABLE, CODEC_InitStruct->CODEC_SelRxI2STdm);
		AUDIO_CODEC_SelADCChannel(CODEC_InitStruct->CODEC_SelRxI2STdm);
	}

	if (application == APP_LINE_IN) {
		/*Micbias enable*/
		AUD->AUD_MICBIAS_CTL0 |= AUD_BIT_MICBIAS1_POW;
		/*Micbias power cut enable: 5channel*/
		AUD->AUD_MICBST_CTL0 |= AUD_BIT_MICBST_POWL | AUD_BIT_MICBST_POWR;
		/*DTSDM Enable*/
		AUD->AUD_ADDA_CTL |= AUD_BIT_DTSDM_CKXEN | AUD_BIT_DTSDM_POW_L | AUD_BIT_DTSDM_POW_R;
		/*default: 2 amic differential mode*/
		AUD->AUD_MICBST_CTL0 |= AUD_BIT_MICBST_ENDFL | AUD_BIT_MICBST_ENDFR;
		/*micbst gain select: dufault 0dB*/
		AUD->AUD_MICBST_CTL0 &= ~(AUD_MASK_MICBST_GSELL | AUD_MASK_MICBST_GSELR);
		AUD->AUD_MICBST_CTL0 |= (AUD_MICBST_GSELL(MICBST_GAIN_0DB) | AUD_MICBST_GSELR(MICBST_GAIN_0DB));
		/*unmute linein enable*/
		AUD->AUD_MICBST_CTL0 &= ~ AUD_MASK_MICBST_MUTE_L | AUD_MASK_MICBST_MUTE_R;
		AUD->AUD_MICBST_CTL0 |= AUD_MICBST_MUTE_L(1) | AUD_MICBST_MUTE_R(1);
		/*ADC part*/
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);
		Tmp |= AUD_BIT_AD_ANA_CLK_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_1;
		Tmp |= AUD_SAMPLE_RATE_1(CODEC_InitStruct->CODEC_SelI2SRxSR);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
		Tmp &= ~(AUD_MASK_I2S_0_CH_LEN_SEL_RX | AUD_MASK_I2S_0_DATA_LEN_SEL_RX | AUD_MASK_I2S_0_DATA_FORMAT_SEL_RX);
		Tmp |= (AUD_I2S_0_CH_LEN_SEL_RX(CODEC_InitStruct->CODEC_SelI2SRxCHLen) | AUD_I2S_0_DATA_LEN_SEL_RX(CODEC_InitStruct->CODEC_SelI2SRxWordLen)
				| AUD_I2S_0_DATA_FORMAT_SEL_RX(CODEC_InitStruct->CODEC_SelI2SRxDataFormat));
		HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp);

		AUDIO_CODEC_SetI2SRXTDM(I2S_NOTDM);
		AUDIO_CODEC_MuteADCInput(DISABLE, I2S_NOTDM);
		AUDIO_CODEC_SelADCChannel(I2S_NOTDM);
	}

	/*default: HPO enable single out*/
	if (application == APP_PDM_OUT) {

		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_0;
		Tmp |= AUD_SAMPLE_RATE_0(CODEC_InitStruct->CODEC_SelI2STxSR);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);
		Tmp |= AUD_BIT_DA_L_EN | AUD_BIT_MOD_L_EN | AUD_BIT_DA_ANA_CLK_EN | AUD_BIT_DA_FIFO_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_0);
		Tmp &= ~AUD_BIT_DAC_L_DA_SRC_SEL;  //I2S_L
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_0, Tmp);
	}
}

/**
  * @brief  Set amic in to single end mode.
  * @param  None
  * @return  None
  */
void AUDIO_CODEC_SetAmicInSE(u32 Amic_sel)
{

	if (Amic_sel == AMIC1) {
		AUD->AUD_MICBST_CTL0 &= ~(AUD_BIT_MICBST_ENDFL);
	} else if (Amic_sel == AMIC2) {
		AUD->AUD_MICBST_CTL0 &= ~(AUD_BIT_MICBST_ENDFR);
	} else {
		AUD->AUD_MICBST_CTL1 &= ~(AUD_BIT_MICBST3_ENDF);
	}
}

/**
  * @brief  Set line in to single end mode. Only amic1 and amic2 support linein.
  * @param  None
  * @return  None
  */
void AUIDO_CODEC_SetLineInSE(u32 Linein_sel)
{

	if (Linein_sel == LINEIN1) {
		AUD->AUD_MICBST_CTL0 &= ~(AUD_BIT_MICBST_ENDFL);
	} else if (Linein_sel == LINEIN2) {
		AUD->AUD_MICBST_CTL0 &= ~(AUD_BIT_MICBST_ENDFR);
	}
}

/**
  * @brief  mute or unmute per ad channel.
  * @param  ADC_sel: select amic channel
  *			 This parameter can be one of the following values:
  *			   @arg ADC1
  *			   @arg ADC2
  *			   @arg ADC3
  *			   @arg ADC4
  * @param  NewState: mute or unmute option for ad channel
  *			 This parameter can be one of the following values:
  *			   @arg DISABLE: unmute
  *			   @arg ENABLE: mute
  * @return  None
  */
void AUDIO_CODEC_MuteAmicIn(u32 NewState, u32 Amic_sel)
{
	if (NewState == UNMUTE) {
		if (Amic_sel == AMIC1) {
			AUD->AUD_MICBST_CTL0 &= ~AUD_MASK_MICBST_MUTE_L;
			AUD->AUD_MICBST_CTL0 |= AUD_MICBST_MUTE_L(2);
		} else if (Amic_sel == AMIC2) {
			AUD->AUD_MICBST_CTL0 &= ~AUD_MASK_MICBST_MUTE_R;
			AUD->AUD_MICBST_CTL0 |= AUD_MICBST_MUTE_R(2);
		} else {
			AUD->AUD_MICBST_CTL1 &= ~AUD_BIT_MICBST3_MUTE;
		}
	} else {
		if (Amic_sel == AMIC1) {
			AUD->AUD_MICBST_CTL0 |= AUD_MASK_MICBST_MUTE_L;
		} else if (Amic_sel == AMIC2) {
			AUD->AUD_MICBST_CTL0 |= AUD_MASK_MICBST_MUTE_R;
		} else {
			AUD->AUD_MICBST_CTL1 |= AUD_BIT_MICBST3_MUTE;
		}
	}
}

/**
  * @brief  mute or unmute per dmic channel.
  * @param  ADC_sel: select amic channel
  *			 This parameter can be one of the following values:
  *			   @arg DMIC1
  *			   @arg DMIC2
  *			   @arg DMIC3
  *			   @arg DMIC4
  * @param  NewState: mute or unmute option for ad channel
  *			 This parameter can be one of the following values:
  *			   @arg DISABLE: unmute
  *			   @arg ENABLE: mute
  * @return  None
  */
void AUDIO_CODEC_MuteDmicIn(u32 NewState, u32 Dmic_sel)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (NewState == UNMUTE) {

		switch (Dmic_sel) {
		case DMIC1:
			Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
			Tmp &= ~(AUD_BIT_ADC_0_DMIC_MIX_MUTE | AUD_MASK_ADC_0_DCHPF_FC_SEL | AUD_MASK_ADC_0_DCHPF_FC_SEL);
			Tmp |= AUD_BIT_ADC_0_DCHPF_EN | AUD_ADC_0_DCHPF_FC_SEL(7);
			HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);
			break;

		case DMIC2:
			Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
			Tmp &= ~(AUD_BIT_ADC_1_DMIC_MIX_MUTE | AUD_MASK_ADC_1_DMIC_SRC_SEL | AUD_MASK_ADC_1_DCHPF_FC_SEL);
			Tmp |= ((AUD_ADC_1_DMIC_SRC_SEL(1)) | AUD_BIT_ADC_1_DCHPF_EN | (AUD_ADC_1_DCHPF_FC_SEL(7)));
			HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);

			break;
		case DMIC3:
			Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
			Tmp &= ~(AUD_BIT_ADC_2_DMIC_MIX_MUTE | AUD_MASK_ADC_2_DMIC_SRC_SEL | AUD_MASK_ADC_2_DCHPF_FC_SEL);
			Tmp |= (AUD_ADC_2_DMIC_SRC_SEL(2) | AUD_BIT_ADC_2_DCHPF_EN | (AUD_ADC_2_DCHPF_FC_SEL(7)));
			HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);


			break;
		case DMIC4:
			Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
			Tmp &= ~(AUD_BIT_ADC_3_DMIC_MIX_MUTE | AUD_MASK_ADC_3_DMIC_SRC_SEL | AUD_MASK_ADC_3_DCHPF_FC_SEL);
			Tmp |= ((AUD_ADC_3_DMIC_SRC_SEL(3)) | AUD_BIT_ADC_3_DCHPF_EN | (AUD_ADC_3_DCHPF_FC_SEL(7)));
			HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
			break;
		default:
			break;
		}
	} else {
		switch (Dmic_sel) {
		case DMIC1:
			Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
			Tmp |= AUD_BIT_ADC_0_DMIC_MIX_MUTE;
			HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);
			break;
		case DMIC2:
			Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
			Tmp |= AUD_BIT_ADC_1_DMIC_MIX_MUTE;
			HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);

			break;
		case DMIC3:
			Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
			Tmp |= AUD_BIT_ADC_2_DMIC_MIX_MUTE;
			HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);

			break;
		case DMIC4:
			Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
			Tmp |= AUD_BIT_ADC_3_DMIC_MIX_MUTE;
			HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
			break;
		default:
			break;
		}
	}

}


/**
  * @brief  mute or unmute per ad channel.
  * @param  LINEIN1: select amic channel
  *			 This parameter can be one of the following values:
  *			   @arg LINEIN1
  *			   @arg LINEIN2
  * @param  NewState: mute or unmute option for ad channel
  *			 This parameter can be one of the following values:
  *			   @arg DISABLE: unmute
  *			   @arg ENABLE: mute
  * @return  None
  */
void AUDIO_CODEC_MuteLineIn(u32 NewState, u32 Linein_sel)
{
	if (NewState == UNMUTE) {
		if (Linein_sel == LINEIN1) {
			AUD->AUD_MICBST_CTL0 &= ~ AUD_MASK_MICBST_MUTE_L;
			AUD->AUD_MICBST_CTL0 |= AUD_MICBST_MUTE_L(1);
		} else if (Linein_sel == LINEIN2) {
			AUD->AUD_MICBST_CTL0 &= ~ AUD_MASK_MICBST_MUTE_R;
			AUD->AUD_MICBST_CTL0 |= AUD_MICBST_MUTE_R(1);
		}
	} else {
		if (Linein_sel == LINEIN1) {
			AUD->AUD_MICBST_CTL0 |= AUD_MASK_MICBST_MUTE_L;
		} else if (Linein_sel == LINEIN2) {
			AUD->AUD_MICBST_CTL0 |= AUD_MASK_MICBST_MUTE_R;
		}
	}
}

/**
  * @brief  Set codec MICBST gain.
  * @param  Amic_sel: amic select
  *          This parameter can be one of the following values:
  *            @arg AMIC1
  *            @arg AMIC2
  *            @arg AMIC3
  *            @arg AMIC4
  * @param  gain: ADC channel micbst gain select
  *          This parameter can be one of the following values:
  *            @arg MICBST_GAIN_0DB: 0dB
  *            @arg MICBST_Gain_5dB: 5dB
  *            ...
  *            @arg MICBST_Gain_40dB: 40dB
  * @note ADC digital volume is 0dB~+40dB in 5dB step.
  * @return  None
  */
void AUDIO_CODEC_SetMicBstGain(u32 Amic_sel, u32 gain)
{
	if (Amic_sel == AMIC1) {
		AUD->AUD_MICBST_CTL0 &= ~(AUD_MASK_MICBST_GSELL);
		AUD->AUD_MICBST_CTL0 |= AUD_MICBST_GSELL(gain) ;
	} else if (Amic_sel == AMIC2) {
		AUD->AUD_MICBST_CTL0 &= ~(AUD_MASK_MICBST_GSELR);
		AUD->AUD_MICBST_CTL0 |= AUD_MICBST_GSELR(gain);
	} else {
		AUD->AUD_MICBST_CTL1 &= ~(AUD_MASK_MICBST3_GSEL);
		AUD->AUD_MICBST_CTL1 |= AUD_MICBST3_GSEL(gain);
	}
}

/**
  * @brief  Set codec DMIC clk.
  * @param  clk: dmic clk select
  *          This parameter can be one of the following values:
  *            @arg DMIC_5M
  *            @arg DMIC_2P5M
  *            @arg DMIC_1P25M
  *            @arg DMIC_625K
  *            @arg DMIC_312P5K
  *            @arg DMIC_769P2K
  * @return  None
  */
void AUDIO_CODEC_SelDmicClk(u32 clk)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_3);
	Tmp &= ~ AUD_MASK_DMIC1_CLK_SEL;
	Tmp |= AUD_BIT_DMIC1_CLK_EN | AUD_DMIC1_CLK_SEL(clk);
	HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_3, Tmp);
}

/**
  * @brief  Set codec PDM clk.
  * @param  clk: PDM clk select
  *          This parameter can be one of the following values:
  *            @arg PDM_5M
  *            @arg PDM_2P5M
  *            @arg PDM_6P67M
  * @param  NewState: pdm enable or disable
  *			 This parameter can be one of the following values:
  *			   @arg DISABLE
  *			   @arg ENABLE

  * @return  None
  */
void AUDIO_CODEC_SetPDM(u32 NewState, u32 clk)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (NewState == ENABLE) {
		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_1);
		Tmp |= AUD_BIT_DAC_L_PDM_EN;
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_1, Tmp);

		Tmp = HAL_READ32(audio_base, CODEC_AUDIO_CONTROL_1);
		Tmp &= ~ AUD_MASK_PDM_CLK_SEL;
		Tmp |= AUD_PDM_CLK_SEL(clk);
		HAL_WRITE32(audio_base, CODEC_AUDIO_CONTROL_1, Tmp);
	} else {
		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_1);
		Tmp &= ~AUD_BIT_DAC_L_PDM_EN;
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_1, Tmp);
	}
}

/**
  * @brief  Set the gain of ADC digital volume.
  * @param  tdmmode: the value of tdm mode.
  *          This parameter can be one of the following values:
  *            @arg I2S_NOTDM
  *            @arg I2S_TDM4
  *            @arg I2S_TDM6
  *            @arg I2S_TDM8
  * @param  Gain: the value of gain.
  *          This parameter can be -17.625dB~48dB in 0.375dB step
  *            @arg 0x00 -17.625dB
  *            @arg 0xaf 48dB
  * @retval None
  */
void AUDIO_CODEC_SetADCVolume(u32 tdmmode, u32 Gain)
{

	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_1);
	Tmp &= ~(0xff << 0);
	Tmp |= (Gain << 0);

	if (tdmmode == I2S_NOTDM) {
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_1, Tmp);
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_1, Tmp);
	} else {
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_1, Tmp);
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_1, Tmp);
		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_1, Tmp);
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_1, Tmp);
	}
}

/**
  * @brief  Set ADC ASRC MODE.
  * @param  NewState: the value of tdm mode.
  *			 This parameter can be one of the following values:
  *			   @arg DISABLE
  *			   @arg ENABLE
  * @param  tdmmode: the value of tdm mode.
  *          This parameter can be one of the following values:
  *            @arg I2S_NOTDM
  *            @arg I2S_TDM4
  *            @arg I2S_TDM6
  *            @arg I2S_TDM8
  * @param  sr: the value of sample rate.
  * @retval None
  */
void AUDIO_CODEC_SetADCASRC(u32 NewState, u32 tdmmode, u32 sr)
{

	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (NewState == ENABLE) {
		if (tdmmode == I2S_NOTDM) {
			Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_6);
			Tmp |= AUD_BIT_ADC_0_ASRC_EN | AUD_BIT_ADC_1_ASRC_EN;
			HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_6, Tmp);

		} else {
			Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_6);
			Tmp |= AUD_BIT_ADC_0_ASRC_EN | AUD_BIT_ADC_1_ASRC_EN | AUD_BIT_ADC_2_ASRC_EN | AUD_BIT_ADC_3_ASRC_EN;
			HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_6, Tmp);

		}

		if (sr == SR_96K || sr == SR_88P2K) {
			Tmp = HAL_READ32(audio_base, CODEC_ASRC_CONTROL_2);
			Tmp &= ~AUD_MASK_ASRC_RATE_SEL_RX_0;
			Tmp |= AUD_ASRC_RATE_SEL_RX_0(1);
			HAL_WRITE32(audio_base, CODEC_ASRC_CONTROL_2, Tmp);
		} else {
			Tmp = HAL_READ32(audio_base, CODEC_ASRC_CONTROL_2);
			Tmp &= ~AUD_MASK_ASRC_RATE_SEL_RX_0;
			Tmp |= AUD_ASRC_RATE_SEL_RX_0(0);
			HAL_WRITE32(audio_base, CODEC_ASRC_CONTROL_2, Tmp);
		}
	} else {
		if (tdmmode == I2S_NOTDM) {
			Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_6);
			Tmp &= ~(AUD_BIT_ADC_0_ASRC_EN | AUD_BIT_ADC_1_ASRC_EN);
			HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_6, Tmp);

		} else {
			Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_6);
			Tmp &= ~(AUD_BIT_ADC_0_ASRC_EN | AUD_BIT_ADC_1_ASRC_EN | AUD_BIT_ADC_2_ASRC_EN | AUD_BIT_ADC_3_ASRC_EN);
			HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_6, Tmp);

		}
	}
}


/**
  * @brief  Set the gain of DAC digital volume.
  * @param  Gain: the value of gain.
  *          This parameter can be -62.5dB~0dB in 0.375dB step
  *            @arg 0x00 -65.625dB
  *            @arg 0xaf 0dB
  * @retval None
  */
void AUDIO_CODEC_SetDACVolume(u32 Gain)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_0);
	Tmp &= ~ AUD_MASK_DAC_L_DA_GAIN;
	Tmp |= AUD_DAC_L_DA_GAIN(Gain);
	HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_0, Tmp);

}

/**
  * @brief  Enable or disable DAC ASRC mode and select asrc rate.
  *			 This parameter can be one of the following values:
  *			   @arg DISABLE
  *			   @arg ENABLE
  * @retval None
  */
void AUDIO_CODEC_SetDACASRC(u32 NewState, u32 sr)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (NewState == ENABLE) {
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_6);
		Tmp |= AUD_BIT_DAC_L_ASRC_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_6, Tmp);

		if (sr == SR_192K || sr == SR_176P4K) {
			Tmp = HAL_READ32(audio_base, CODEC_ASRC_CONTROL_0);
			Tmp &= ~AUD_MASK_ASRC_RATE_SEL_TX;
			Tmp |= AUD_ASRC_RATE_SEL_TX(2);
			HAL_WRITE32(audio_base, CODEC_ASRC_CONTROL_0, Tmp);

		} else if (sr == SR_96K || sr == SR_88P2K) {
			Tmp = HAL_READ32(audio_base, CODEC_ASRC_CONTROL_0);
			Tmp &= ~AUD_MASK_ASRC_RATE_SEL_TX;
			Tmp |= AUD_ASRC_RATE_SEL_TX(1);
			HAL_WRITE32(audio_base, CODEC_ASRC_CONTROL_0, Tmp);

		} else {
			Tmp = HAL_READ32(audio_base, CODEC_ASRC_CONTROL_0);
			Tmp &= ~AUD_MASK_ASRC_RATE_SEL_TX;
			Tmp |= AUD_ASRC_RATE_SEL_TX(0);
			HAL_WRITE32(audio_base, CODEC_ASRC_CONTROL_0, Tmp);
		}

	} else {
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_6);
		Tmp &= ~AUD_BIT_DAC_L_ASRC_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_6, Tmp);

	}
}

/**
  * @brief  Set adc power on or down and select power mode.
  * @param  adc_sel: select amic channel
  *			 This parameter can be one of the following values:
  *			   @arg ADC1
  *			   @arg ADC2
  *			   @arg ADC3
  *			   @arg ADC4
  * @param  powermode: normal or low power mode
  *			 This parameter can be one of the following values:
  *			   @arg NormalPower
  *			   @arg LowPower
  *			   @arg Shutdown
  * @return  None
  */
void AUDIO_CODEC_SetADCMode(u32 adc_sel, u32 powermode)
{
	if (powermode == NORMALPOWER) {
		if (adc_sel == ADC1) {
			AUD->AUD_MICBST_CTL0 |= AUD_BIT_MICBST_POWL;
			AUD->AUD_ADDA_CTL |= AUD_BIT_DTSDM_CKXEN | AUD_BIT_DTSDM_POW_L;
			AUD->AUD_MBIAS_CTL1 &= ~(AUD_MASK_MBIAS_ISEL_DTSDM_L | AUD_MASK_MBIAS_ISEL_DTSDM_INT1_L | AUD_MASK_MBIAS_ISEL_MICBST_L);
			AUD->AUD_MBIAS_CTL1 |= AUD_MBIAS_ISEL_DTSDM_L(6) | AUD_MBIAS_ISEL_DTSDM_INT1_L(6) | AUD_MBIAS_ISEL_MICBST_L(6);
		} else if (adc_sel == ADC2) {
			AUD->AUD_MICBST_CTL0 |= AUD_BIT_MICBST_POWR;
			AUD->AUD_ADDA_CTL |= AUD_BIT_DTSDM_CKXEN | AUD_BIT_DTSDM_POW_R;
			AUD->AUD_MBIAS_CTL1 &= ~(AUD_MASK_MBIAS_ISEL_DTSDM_R | AUD_MASK_MBIAS_ISEL_DTSDM_INT1_R | AUD_MASK_MBIAS_ISEL_MICBST_R);
			AUD->AUD_MBIAS_CTL1 |= AUD_MBIAS_ISEL_DTSDM_R(6) | AUD_MBIAS_ISEL_DTSDM_INT1_R(6) | AUD_MBIAS_ISEL_MICBST_R(6);
		} else {
			AUD->AUD_MICBST_CTL1 |= AUD_BIT_MICBST3_POW;
			AUD->AUD_DTS_CTL |= AUD_BIT_DTSDM3_CKXEN | AUD_BIT_DTSDM3_POW;
			AUD->AUD_MBIAS_CTL0 &= ~AUD_MASK_MBIAS_ISEL_DTSDM3;
			AUD->AUD_MBIAS_CTL0 |= AUD_MBIAS_ISEL_DTSDM3(6);
			AUD->AUD_MBIAS_CTL2 &= ~AUD_MASK_MBIAS_ISEL_MICBST3;
			AUD->AUD_MBIAS_CTL2 |= AUD_MBIAS_ISEL_MICBST3(6);
		}
	} else if (powermode == LOWPOWER) {
		AUD->AUD_DTS_CTL |= AUD_BIT_LPMODE_EN;
		AUD->AUD_MICBIAS_CTL0 &= ~AUD_BIT_MICBIAS1_POW;

		if (adc_sel == ADC1) {
			AUD->AUD_MICBST_CTL0 |= AUD_BIT_MICBST_POWL;
			AUD->AUD_ADDA_CTL |= AUD_BIT_DTSDM_CKXEN | AUD_BIT_DTSDM_POW_L;
			AUD->AUD_MBIAS_CTL1 &= ~(AUD_MASK_MBIAS_ISEL_DTSDM_L | AUD_MASK_MBIAS_ISEL_DTSDM_INT1_L | AUD_MASK_MBIAS_ISEL_MICBST_L);
			AUD->AUD_MBIAS_CTL1 |= AUD_MBIAS_ISEL_DTSDM_L(0) | AUD_MBIAS_ISEL_DTSDM_INT1_L(0) | AUD_MBIAS_ISEL_MICBST_L(0);
		} else if (adc_sel == ADC2) {
			AUD->AUD_MICBST_CTL0 |= AUD_BIT_MICBST_POWR;
			AUD->AUD_ADDA_CTL |= AUD_BIT_DTSDM_CKXEN | AUD_BIT_DTSDM_POW_R;
			AUD->AUD_MBIAS_CTL1 &= ~(AUD_MASK_MBIAS_ISEL_DTSDM_R | AUD_MASK_MBIAS_ISEL_DTSDM_INT1_R | AUD_MASK_MBIAS_ISEL_MICBST_R);
			AUD->AUD_MBIAS_CTL1 |= AUD_MBIAS_ISEL_DTSDM_R(0) | AUD_MBIAS_ISEL_DTSDM_INT1_R(0) | AUD_MBIAS_ISEL_MICBST_R(0);
		} else {
			AUD->AUD_MICBST_CTL1 |= AUD_BIT_MICBST3_POW;
			AUD->AUD_DTS_CTL |= AUD_BIT_DTSDM3_CKXEN | AUD_BIT_DTSDM3_POW;
			AUD->AUD_MBIAS_CTL0 &= ~(AUD_MASK_MBIAS_ISEL_DTSDM3 | AUD_MASK_MBIAS_ISEL_DTSDM3_INT1);
			AUD->AUD_MBIAS_CTL0 |= AUD_MBIAS_ISEL_DTSDM3(1) | AUD_MBIAS_ISEL_DTSDM3_INT1(1);
			AUD->AUD_MBIAS_CTL2 &= ~AUD_MASK_MBIAS_ISEL_MICBST3;
			AUD->AUD_MBIAS_CTL2 |= AUD_MBIAS_ISEL_MICBST3(1);
		}
	} else {
		if (adc_sel == ADC1) {
			/*Micbias power cut/Micbst power control/DTSDM enable*/
			/*NOTE: AUD_BIT_DTSDM_CKXEN暂时采用Enable状态，以单独控制ADC1 OR ADC2的disable状态*/
			AUD->AUD_MICBST_CTL0 &= ~ AUD_BIT_MICBST_POWL;
			AUD->AUD_ADDA_CTL &= ~(AUD_BIT_DTSDM_POW_L);
		} else if (adc_sel == ADC2) {
			AUD->AUD_MICBST_CTL0 &= ~ AUD_BIT_MICBST_POWR;
			AUD->AUD_ADDA_CTL &= ~(AUD_BIT_DTSDM_POW_R);
		} else {
			AUD->AUD_MICBST_CTL1 &= ~ AUD_BIT_MICBST3_POW;
			AUD->AUD_DTS_CTL &= ~(AUD_BIT_DTSDM3_POW);
		}
	}
}

/**
  * @brief  Set Line out Poweron single-end mode.
  * @return  None
  */
void AUDIO_CODEC_SetLOSE(void)
{
	AUD->AUD_LO_CTL |= AUD_BIT_LO_POW;
	AUD->AUD_LO_CTL &= ~(AUD_BIT_LO_ENDP);
	AUD->AUD_LO_CTL |= AUD_BIT_LO_POW;
	AUD->AUD_LO_CTL &= ~(AUD_MASK_LO_M);
	AUD->AUD_LO_CTL |= AUD_LO_M(2);
	AUD->AUD_LO_CTL |= AUD_BIT_LO_SE;
}

/**
  * @brief  DAC Testtone mode.
  * @param  fc: the value of test frequency.
  *          This parameter can be one of the following values:
  *          @arg fc
  *          frequency= (fs/192)*(fc+1)Hz
  * @param  NewState: the status of testtone mode.
  *          This parameter can be the following values:
  *            @arg ENABLE
  *            @arg DISABLE
  * @retval None
  */

void AUDIO_CODEC_SelTesttone(u32 NewState, u32 fc)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_0);
	Tmp |= AUD_BIT_DAC_L_DA_SRC_SEL;
	HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_0, Tmp);

	if (NewState == ENABLE) {

		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_0);
		Tmp &= ~AUD_MASK_DAC_L_TEST_FC_SEL;
		Tmp |= AUD_BIT_DAC_L_TEST_TONE_EN | (AUD_DAC_L_TEST_FC_SEL(fc)) | AUD_BIT_DAC_L_DA_SRC_SEL;
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_0, Tmp);

	} else {

		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_0);
		Tmp &= ~(AUD_BIT_DAC_L_TEST_TONE_EN | AUD_BIT_DAC_L_DA_SRC_SEL);
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_0, Tmp);
	}
}

/**
  * @brief  select ADC path decimation source
  * @param  ADC_sel: select amic channel
  *			 This parameter can be one of the following values:
  *			   @arg ADC1
  *			   @arg ADC2
  *			   @arg ADC3
  *			   @arg ADC4
  * @return  None
  */
void AUDIO_CODEC_MusicOutToADC(u32 ADC_sel)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (ADC_sel == ADC1) {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
		Tmp &= ~(AUD_MASK_ADC_0_DECI_SRC_SEL);
		Tmp |=	AUD_ADC_0_DECI_SRC_SEL(2);
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);
	} else if (ADC_sel == ADC2) {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
		Tmp &= ~(AUD_MASK_ADC_1_DECI_SRC_SEL);
		Tmp |=	AUD_ADC_1_DECI_SRC_SEL(2);
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);
	} else if (ADC_sel == ADC3) {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
		Tmp &= ~(AUD_MASK_ADC_2_DECI_SRC_SEL);
		Tmp |=	AUD_ADC_2_DECI_SRC_SEL(2);
		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);
	} else {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
		Tmp &= ~(AUD_MASK_ADC_3_DECI_SRC_SEL);
		Tmp |=	AUD_ADC_3_DECI_SRC_SEL(2);
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
	}
}

/**
  * @brief  Select analog adc source for per ad channel corresponding to AMIC.
  * @param  ad_chn: select amic channel corresponding to ADC channel
  *			 This parameter can be one of the following values:
  *			   @arg ADCHN1
  *			   @arg ADCHN2
  *			   @arg ADCHN3
  *			   @arg ADCHN4
  * @param  adc_sel: select per adc channel ad src.
  *			 This parameter can be one of the following values:
  *			   @arg ADC1
  *			   @arg ADC2
  *			   @arg ADC3
  * @return  None
  */
void AUDIO_CODEC_SelADCSrcToAmic(u32 ad_chn, u32 adc_sel)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (ad_chn == ADCHN1) {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_0_AD_SRC_SEL;
		Tmp |= AUD_ADC_0_AD_SRC_SEL(adc_sel - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);

	} else if (ad_chn == ADCHN2) {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_1_AD_SRC_SEL;
		Tmp |= AUD_ADC_1_AD_SRC_SEL(adc_sel - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);

	} else if (ad_chn == ADCHN3) {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_2_AD_SRC_SEL;
		Tmp |= AUD_ADC_2_AD_SRC_SEL(adc_sel - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);

	} else {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_3_AD_SRC_SEL;
		Tmp |= AUD_ADC_3_AD_SRC_SEL(adc_sel - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);

	}

}


/**
  * @brief  Select adc source for per dmic.
  * @param  amic_sel: select amic channel corresponding to ADC channel
  *			 This parameter can be one of the following values:
  *			   @arg DMIC1
  *			   @arg DMIC2
  *			   @arg DMIC3
  *			   @arg DMIC4
  * @param  adc_sel: select per adc channel ad src.
  *			 This parameter can be one of the following values:
  *			   @arg ADC1
  *			   @arg ADC2
  *			   @arg ADC3
  *			   @arg ADC4
  * @return  None
  */
void AUDIO_CODEC_SelADCSrcToDmic(u32 dmic_sel, u32 adc_sel)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (dmic_sel == DMIC1) {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_0_DMIC_SRC_SEL;
		Tmp |= AUD_ADC_0_DMIC_SRC_SEL(adc_sel - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);

	} else if (dmic_sel == DMIC2) {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_1_DMIC_SRC_SEL;
		Tmp |= AUD_ADC_1_DMIC_SRC_SEL(adc_sel - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);

	} else if (dmic_sel == DMIC3) {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_2_DMIC_SRC_SEL;
		Tmp |= AUD_ADC_2_DMIC_SRC_SEL(adc_sel - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);

	} else {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_3_DMIC_SRC_SEL;
		Tmp |= AUD_ADC_3_DMIC_SRC_SEL(adc_sel - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
	}
}

/**
  * @brief  mute or unmute dac channel.
  * @param  NewState: mute or unmute option for ad channel
  *			 This parameter can be one of the following values:
  *			   @arg DISABLE: unmute
  *			   @arg ENABLE: mute
  */
void AUDIO_CODEC_MuteDAC(u32 NewState)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (NewState == UNMUTE) {
		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_1);
		Tmp &= ~AUD_BIT_DAC_L_DA_MUTE;
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_1, Tmp);
	} else {
		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_1);
		Tmp |= AUD_BIT_DAC_L_DA_MUTE;
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_1, Tmp);
	}
}

/**  * @brief  Select I2S master source.
  * @param  src: source
  *          This parameter can be one of the following values:
  *            @arg INTERNAL_SPORT
  *            @arg EXTERNAL_I2S
  * @return  None
  */
void AUDIO_CODEC_SelI2SSRC(u32 src)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
	if (src == INTERNAL_SPORT) {
		Tmp &= ~AUD_BIT_I2S_0_MASTER_SEL;
	} else {
		Tmp |= AUD_BIT_I2S_0_MASTER_SEL;
	}
	HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp);

}

/**  * @brief  De-initialize codec peripheral.
  * @param  application: application mode
  *          This parameter can be one of the following values:
  *            @arg APP_AMIC_IN: analog microphone in
  *            @arg APP_DMIC_IN: digital microphone in
  *            @arg APP_LINE_IN: line in
  *            @arg APP_LINE_OUT: line out
  * @return  None
  */
void AUDIO_CODEC_DeInit(u32 application)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	/* ================= CODEC initialize ======================== */
	/*default: Disable HPO single out*/
	if (application == APP_LINE_OUT) {
		AUD->AUD_LO_CTL &= ~AUD_BIT_LO_POW;
		AUD->AUD_ADDA_CTL &= ~ AUD_BIT_DPRAMP_POW;
		/*Disable DAC*/
		AUD->AUD_ADDA_CTL &= ~AUD_BIT_DAC_POW;
		AUD->AUD_LO_CTL |= AUD_MASK_LO_M;
		AUD->AUD_LO_CTL &= ~AUD_BIT_LO_POW;
	}

	if (application == APP_AMIC_IN) {
		/*Disable Micbias*/
		AUD->AUD_MICBIAS_CTL0 &= ~AUD_BIT_MICBIAS1_POW;
		/*Disable Micbias power: 5channel*/
		AUD->AUD_MICBST_CTL0 &= ~(AUD_BIT_MICBST_POWL | AUD_BIT_MICBST_POWR);
		AUD->AUD_MICBST_CTL1 |= AUD_BIT_MICBST3_POW;
		/*Disable DTSDM*/
		AUD->AUD_ADDA_CTL &= ~(AUD_BIT_DTSDM_CKXEN | AUD_BIT_DTSDM_POW_L | AUD_BIT_DTSDM_POW_R);
		AUD->AUD_DTS_CTL &= ~(AUD_BIT_DTSDM3_CKXEN | AUD_BIT_DTSDM3_POW);
	}

	if (application == APP_LINE_IN) {
		/*Disable Micbias*/
		AUD->AUD_MICBIAS_CTL0 &= ~AUD_BIT_MICBIAS1_POW;
		/*Disable Micbias power cut: 2channel*/
		AUD->AUD_MICBST_CTL0 &= ~(AUD_BIT_MICBST_POWL | AUD_BIT_MICBST_POWR);
		/*Disable DTSDM*/
		AUD->AUD_ADDA_CTL &= ~(AUD_BIT_DTSDM_CKXEN | AUD_BIT_DTSDM_POW_L | AUD_BIT_DTSDM_POW_R);
	}

	if (application == APP_DMIC_IN) {
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_3);
		Tmp &= ~ AUD_BIT_DMIC1_CLK_EN;
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_3, Tmp);
	}
}
/******************* (C) COPYRIGHT 2017 Realtek Semiconductor *****END OF FILE****/


