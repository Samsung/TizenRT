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

AUD_TypeDef	*AUD;

/**
  * @brief  Enable or disable audio codec IP.
  * @param  newstate: audio IP enable or disable.
  *			 This parameter can be one of the following values:
  *			   @arg ENABLE
  *			   @arg DISABLE
  * @return  None
  */
void AUDIO_CODEC_SetAudioIP(u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_AUDIO_CONTROL_0);
	if (newstate == ENABLE) {
		Tmp |= AUD_BIT_AUDIO_IP_EN;
	} else {
		Tmp &= ~AUD_BIT_AUDIO_IP_EN;
	}
	HAL_WRITE32(audio_base, CODEC_AUDIO_CONTROL_0, Tmp);
}


/**
  * @brief  Enable or disable I2S IP.
  * @param  i2s_sel: select I2S.
  *			 This parameter can be one of the following values:
  *			   @arg I2S0
  * @param  newstate: I2S IP enable or disable.
  *			 This parameter can be one of the following values:
  *			   @arg ENABLE
  *			   @arg DISABLE
  * @return  None
  */
void AUDIO_CODEC_SetI2SIP(u32 i2s_sel, u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (i2s_sel == I2S0) {
		Tmp = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_I2S_0_RST_N_REG;
		} else {
			Tmp &= ~AUD_BIT_I2S_0_RST_N_REG;
		}
		HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp);
	} else {
		return;
	}
}


/**
  * @brief  Select I2S master source.
  * @param  i2s_sel: select I2S.
  *			 This parameter can be one of the following values:
  *			   @arg I2S0
  * @param  src: source
  *          This parameter can be one of the following values:
  *            @arg INTERNAL_SPORT
  *            @arg EXTERNAL_I2S
  * @return  None
  */
void AUDIO_CODEC_SetI2SSRC(u32 i2s_sel, u32 src)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (i2s_sel == I2S0) {

		Tmp = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
		if (src == INTERNAL_SPORT) {
			Tmp &= ~AUD_BIT_I2S_0_MASTER_SEL;
		} else {
			Tmp |= AUD_BIT_I2S_0_MASTER_SEL;
		}
		HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp);
	} else {
		return;
	}

}


/**
  * @brief  Enable or disable adc/dac ananlog clock.
  * @param  newstate: enable or disable dac/adc analog clock.
  *			 This parameter can be one of the following values:
  *			   @arg ENABLE
  *			   @arg DISABLE
  * @return  None
  */
void AUDIO_CODEC_SetANAClk(u32 newstate)
{

	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);
	if (newstate == ENABLE) {
		Tmp |= AUD_BIT_DA_ANA_CLK_EN | AUD_BIT_AD_ANA_CLK_EN;
	} else {
		Tmp &= ~(AUD_BIT_DA_ANA_CLK_EN | AUD_BIT_AD_ANA_CLK_EN);
	}
	HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);
}

/**
  * @brief  Set i2s rx tdm mode.
  * @param  i2s_sel: select i2s: i2s0 or i2s1
  *			 This parameter can be one of the following values:
  *			   @arg I2S0
  * @param  tdmmode: select i2s rx tdm mode.
  *			 This parameter can be one of the following values:
  *			   @arg I2S_NOTDM
  *			   @arg I2S_TDM4
  * @return  None
  */
void AUDIO_CODEC_SetI2SRXTDM(u32 i2s_sel, u32 tdmmode)
{
	u32 Tmp1;
	u32 Tmp2;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (i2s_sel == I2S0) {

		Tmp1 = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
		Tmp1 &= ~(AUD_MASK_I2S_0_TDM_MODE_RX);
		Tmp2 = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL_1);

		if (tdmmode == I2S_NOTDM) {
			Tmp1 |= AUD_I2S_0_TDM_MODE_RX(0);
			Tmp2 &= ~(AUD_MASK_I2S_0_DATA_CH0_SEL_RX | AUD_MASK_I2S_0_DATA_CH1_SEL_RX | AUD_BIT_I2S_0_DATA_CH0_RX_DISABLE | AUD_BIT_I2S_0_DATA_CH1_RX_DISABLE);
			Tmp2 |= (AUD_I2S_0_DATA_CH0_SEL_RX(0) | AUD_I2S_0_DATA_CH1_SEL_RX(1) | AUD_BIT_I2S_0_DATA_CH2_RX_DISABLE | AUD_BIT_I2S_0_DATA_CH3_RX_DISABLE);
		} else {
			Tmp1 |= AUD_I2S_0_TDM_MODE_RX(1);
			Tmp2 &= ~(AUD_MASK_I2S_0_DATA_CH0_SEL_RX | AUD_MASK_I2S_0_DATA_CH1_SEL_RX | AUD_MASK_I2S_0_DATA_CH2_SEL_RX | AUD_MASK_I2S_0_DATA_CH3_SEL_RX |
					  AUD_BIT_I2S_0_DATA_CH0_RX_DISABLE | AUD_BIT_I2S_0_DATA_CH1_RX_DISABLE |  AUD_BIT_I2S_0_DATA_CH2_RX_DISABLE | AUD_BIT_I2S_0_DATA_CH3_RX_DISABLE);
			Tmp2 |= (AUD_I2S_0_DATA_CH0_SEL_RX(0) | AUD_I2S_0_DATA_CH1_SEL_RX(1) | AUD_I2S_0_DATA_CH2_SEL_RX(2) | AUD_I2S_0_DATA_CH3_SEL_RX(3));

		}

		HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp1);
		HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL_1, Tmp2);
	} else {
		return;
	}
}

/**
  * @brief  Default I2S initialization parameter.
  * @return  None
  */
void AUDIO_CODEC_I2S_StructInit(I2S_InitTypeDef *I2S_Struct)
{
	I2S_Struct->CODEC_SelI2STxSR = SR_48K;
	I2S_Struct->CODEC_SelI2STxWordLen = WL_16;
	I2S_Struct->CODEC_SelI2STxCHLen = CL_32;
	I2S_Struct->CODEC_SelI2STxDataFormat = DF_I2S;
	I2S_Struct->CODEC_SelI2SRxSR = SR_48K;
	I2S_Struct->CODEC_SelI2SRxWordLen = WL_16;
	I2S_Struct->CODEC_SelI2SRxCHLen = CL_32;
	I2S_Struct->CODEC_SelI2SRxDataFormat = DF_I2S;
	I2S_Struct->CODEC_SelRxI2STdm = I2S_NOTDM;
}

/**
  * @brief  Set I2S initialization parameter.
  * @param  i2s_sel: select i2s
  *			 This parameter can be one of the following values:
  *			   @arg I2S0
  * @param  type: audio work path.
  *			 This parameter can be one of the following values:
  *			   @arg DACPATH
  *			   @arg ADCPATH
  * @return  None
  */
void AUDIO_CODEC_SetI2SParameters(u32 i2s_sel, u32 path, I2S_InitTypeDef *I2S_Struct)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (i2s_sel == I2S0) {

		if (path == DACPATH) {
			Tmp = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
			Tmp &= ~(AUD_MASK_I2S_0_CH_LEN_SEL_TX | AUD_MASK_I2S_0_DATA_LEN_SEL_TX | AUD_MASK_I2S_0_DATA_FORMAT_SEL_TX | AUD_MASK_I2S_0_DATA_CH_SEL_TX);
			Tmp |= (AUD_I2S_0_CH_LEN_SEL_TX(I2S_Struct->CODEC_SelI2STxCHLen) | AUD_I2S_0_DATA_LEN_SEL_TX(I2S_Struct->CODEC_SelI2STxWordLen)
					| AUD_I2S_0_DATA_FORMAT_SEL_TX(I2S_Struct->CODEC_SelI2STxDataFormat));
			HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp);
		} else {
			Tmp = HAL_READ32(audio_base, CODEC_I2S_0_CONTROL);
			Tmp &= ~(AUD_MASK_I2S_0_CH_LEN_SEL_RX | AUD_MASK_I2S_0_DATA_LEN_SEL_RX | AUD_MASK_I2S_0_DATA_FORMAT_SEL_RX);
			Tmp |= (AUD_I2S_0_CH_LEN_SEL_RX(I2S_Struct->CODEC_SelI2SRxCHLen) | AUD_I2S_0_DATA_LEN_SEL_RX(I2S_Struct->CODEC_SelI2SRxWordLen)
					| AUD_I2S_0_DATA_FORMAT_SEL_RX(I2S_Struct->CODEC_SelI2SRxDataFormat));
			HAL_WRITE32(audio_base, CODEC_I2S_0_CONTROL, Tmp);
			AUDIO_CODEC_SetI2SRXTDM(I2S0, I2S_Struct->CODEC_SelRxI2STdm);
		}
	} else {
		return;
	}
}

/**
  * @brief  Select adc sample rate and source, all adc share souce and sample rate.
  * @param  src: source
  *          This parameter can be one of the following values:
  *            @arg SOURCE0
  *            @arg SOURCE1
  *            @arg SOURCE2
  * @param  sr: sample rate
  * @return  None
  */
void AUDIO_CODEC_SetADCSRSrc(u32 src, u32 sr)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_5);
	Tmp &= ~(AUD_MASK_ADC_0_FS_SRC_SEL | AUD_MASK_ADC_1_FS_SRC_SEL | AUD_MASK_ADC_2_FS_SRC_SEL | AUD_MASK_ADC_3_FS_SRC_SEL);
	Tmp |= AUD_ADC_0_FS_SRC_SEL(src) | AUD_ADC_2_FS_SRC_SEL(src) | AUD_ADC_2_FS_SRC_SEL(src) | AUD_ADC_3_FS_SRC_SEL(src);
	HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_5, Tmp);

	if (src == SOURCE0) {
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_0;
		Tmp |= AUD_SAMPLE_RATE_0(sr);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);
	} else if (src == SOURCE1) {
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_1;
		Tmp |= AUD_SAMPLE_RATE_1(sr);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);
	} else {
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_2;
		Tmp |= AUD_SAMPLE_RATE_2(sr);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);
	}
}

/**
  * @brief  Select dac sample rate and source .
  * @param  src: source
  *          This parameter can be one of the following values:
  *            @arg SOURCE0
  *            @arg SOURCE1
  *            @arg SOURCE2
  * @param  sr: sample rate
  * @return  None
  */
void AUDIO_CODEC_SetDACSRSrc(u32 src, u32 sr)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_5);
	Tmp &= ~ AUD_MASK_DAC_L_FS_SRC_SEL;
	Tmp |= AUD_DAC_L_FS_SRC_SEL(src);
	HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_5, Tmp);

	if (src == SOURCE0) {
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_0;
		Tmp |= AUD_SAMPLE_RATE_0(sr);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);
	} else if (src == SOURCE1) {
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_1;
		Tmp |= AUD_SAMPLE_RATE_1(sr);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);
	} else {
		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_4);
		Tmp &= ~ AUD_MASK_SAMPLE_RATE_2;
		Tmp |= AUD_SAMPLE_RATE_2(sr);
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_4, Tmp);
	}
}

/**
  * @brief  Disable audio pad according to the work path.
  * @param  path: audio work path.
  *			 This parameter can be one of the following values:
  *			   @arg PAD_DACL
  *			   @arg PAD_MIC1
  *			   @arg PAD_MIC2
  *			   @arg PAD_MIC3
  * @return  None
  */
void AUDIO_CODEC_DisPAD(u32 path)
{
	u32 Tmp;
	u32 pin_base;

	if (TrustZone_IsSecure()) {
		pin_base = PINMUX_REG_BASE_S;
	} else {
		pin_base = PINMUX_REG_BASE;
	}

	Tmp = HAL_READ32(pin_base, REG_PAD_AUD_PAD_CTRL);

	switch (path) {
	case PAD_DACL:

		Pinmux_Config(_PB_16, PINMUX_FUNCTION_AUDIO);
		Pinmux_Config(_PB_17, PINMUX_FUNCTION_AUDIO);
		Tmp &= ~(BIT(5) | BIT(6));
		break;

	case PAD_MIC1:

		Pinmux_Config(_PB_11, PINMUX_FUNCTION_AUDIO);
		Pinmux_Config(_PB_12, PINMUX_FUNCTION_AUDIO);
		Pinmux_Config(_PB_15, PINMUX_FUNCTION_AUDIO);
		Tmp &= ~(BIT(0) | BIT(1) | BIT(4));
		break;

	case PAD_MIC2:

		Pinmux_Config(_PB_13, PINMUX_FUNCTION_AUDIO);
		Pinmux_Config(_PB_14, PINMUX_FUNCTION_AUDIO);
		Pinmux_Config(_PB_15, PINMUX_FUNCTION_AUDIO);
		Tmp &= ~(BIT(4) | BIT(3) | BIT(2));
		break;

	case PAD_MIC3:

		Pinmux_Config(_PB_18, PINMUX_FUNCTION_AUDIO);
		Pinmux_Config(_PB_19, PINMUX_FUNCTION_AUDIO);
		Pinmux_Config(_PB_15, PINMUX_FUNCTION_AUDIO);
		Tmp &= ~(BIT(4) | BIT(7) | BIT(8));
		break;
	}

	HAL_WRITE32(pin_base, REG_PAD_AUD_PAD_CTRL, Tmp);
}

/**
  * @brief  VREF calibration.
  * @return  None
  */
static void AUDIO_CODEC_VREF_Calib(void)
{
	if (TrustZone_IsSecure()) {
		AUD = AUD_SYS_BASE_S;
	} else {
		AUD = AUD_SYS_BASE;
	}

	if (SWR_Calib_Get_Value_AUD() == Swr_Calib_Nocalib) {
		return;
	} else {
		AUD->AUD_MICBST_CTL1 &= ~(AUD_MASK_LDO_TUNE);
		AUD->AUD_MICBST_CTL1 |= AUD_LDO_TUNE(SWR_Calib_Get_Value_AUD());
	}
}

/**
  * @brief Audio Codec LDO power on or power down.
  * @param  powermode: LDO power on or power down
  *          This parameter can be one of the following values:
  *            @arg POWER_ON:
  *            @arg POWER_DOWN:
  * @return  None
  */
void AUDIO_CODEC_SetLDOMode(u32 powermode)
{
	if (TrustZone_IsSecure()) {
		AUD = AUD_SYS_BASE_S;
	} else {
		AUD = AUD_SYS_BASE;
	}

	if (powermode == POWER_ON) {
		AUD->AUD_ADDA_CTL |= AUD_BIT_POWADDACK;
		/*LDO power on*/
		AUD->RSVD0 &= ~AUD_BIT_LDO_CODEC_DISCHARGE_PATH;
		AUD->AUD_MICBIAS_CTL0 |= (AUD_BIT_MBIAS_POW);
		AUD->AUD_MICBST_CTL1 |= AUD_BIT_LDO_POW | AUD_BIT_LDO_PREC;
		DelayMs(2);
		AUD->AUD_MICBST_CTL1 &= ~(AUD_BIT_LDO_PREC);
		DelayUs(10);
		AUD->AUD_MICBST_CTL1 |= AUD_BIT_LDO_POW_0P9V;
		DelayUs(990);
		AUD->RSVD0 &= ~AUD_BIT_LDO_CODEC_DRIVING_PATH;
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

/**
  * @brief  Enable or disable per AD and AD fifo channel clock.
  * @param  ad_chn: select ad channel.
  *			 This parameter can be one of the following values:
  *			   @arg ADCHN1
  *			   @arg ADCHN2
  *			   @arg ADCHN3
  *			   @arg ADCHN4
  * @param  newstate: enable or disable per ad channel.
  *			 This parameter can be one of the following values:
  *			   @arg ENABLE
  *			   @arg DISABLE
  * @return  None
  */
void AUDIO_CODEC_EnableADC(u32 ad_chn, u32 newstate)   //enable adc
{

	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);

	switch (ad_chn) {

	case ADCHN1:
		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_AD_0_EN | AUD_BIT_AD_0_FIFO_EN;
		} else {
			Tmp &= ~(AUD_BIT_AD_0_EN | AUD_BIT_AD_0_FIFO_EN);
		}
		break;
	case ADCHN2:

		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_AD_1_EN | AUD_BIT_AD_1_FIFO_EN;
		} else {
			Tmp &= ~(AUD_BIT_AD_1_EN | AUD_BIT_AD_1_FIFO_EN);
		}
		break;
	case ADCHN3:

		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_AD_2_EN | AUD_BIT_AD_2_FIFO_EN;
		} else {
			Tmp &= ~(AUD_BIT_AD_2_EN | AUD_BIT_AD_2_FIFO_EN);
		}
		break;
	case ADCHN4:

		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_AD_3_EN | AUD_BIT_AD_3_FIFO_EN;
		} else {
			Tmp &= ~(AUD_BIT_AD_3_EN | AUD_BIT_AD_3_FIFO_EN);
		}
		break;
	default:
		break;

	}


	HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);
}

/**
  * @brief  Set the gain of ADC digital volume.
  * @param  adc_sel: select adc number.
  *          This parameter can be one of the following values:
  *            @arg ADC1
  *            @arg ADC2
  *            @arg ADC3
  *            @arg ADC4
  * @param  gain: the value of gain.
  *          This parameter can be -17.625dB~48dB in 0.375dB step
  *            @arg 0x00 -17.625dB
  *            @arg 0xaf 48dBS
  * @retval None
  */
void AUDIO_CODEC_SetADCVolume(u32 adc_sel, u32 gain)
{

	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	switch (adc_sel) {
	case ADC1:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_1);
		Tmp &= ~AUD_MASK_ADC_0_AD_GAIN;
		Tmp |= AUD_ADC_0_AD_GAIN(gain) ;
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_1, Tmp);
		break;
	case ADC2:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_1);
		Tmp &= ~AUD_MASK_ADC_1_AD_GAIN;
		Tmp |= AUD_ADC_1_AD_GAIN(gain) ;
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_1, Tmp);
		break;
	case ADC3:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_1);
		Tmp &= ~AUD_MASK_ADC_2_AD_GAIN;
		Tmp |= AUD_ADC_2_AD_GAIN(gain) ;
		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_1, Tmp);
		break;
	case ADC4:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_1);
		Tmp &= ~AUD_MASK_ADC_3_AD_GAIN;
		Tmp |= AUD_ADC_3_AD_GAIN(gain) ;
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_1, Tmp);
		break;

	default:
		break;
	}
}

/**
  * @brief  Set HPF mode and select HPF FC
  * @param  adc_sel: select adc channel.
  *			 This parameter can be one of the following values:
  *			   @arg ADC1
  *			   @arg ADC2
  *			   @arg ADC3
  *			   @arg ADC4
  * @param  fc: select high pass filter fc
  *			 This parameter can be one of the following values:
  *			   @arg 0��5e10^-3 Fs
  *			   @arg 1��2.5e10^-3 Fs
  *			   @arg 2��1.125e10^-3 Fs
  *			   @arg 3: 6.25e10^-4 Fs
  *			   @arg 4: 3.125e10^-4 Fs
  *			   @arg 5: 1.5625e10^-4 Fs
  *			   @arg 6: 7.8125e10^-5 Fs
  *			   @arg 7: 3.90625e10^-5 Fs
  * @param  newstate: enable or disable per ad channel HPF mode
  *			 This parameter can be one of the following values:
  *			   @arg ENABLE
  *			   @arg DISABLE
  * @return  None
  */
void AUDIO_CODEC_SetADCHPF(u32 adc_sel, u32 fc, u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	switch (adc_sel) {
	case ADC1:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
		if (newstate == ENABLE) {
			Tmp &= ~(AUD_MASK_ADC_0_DCHPF_FC_SEL);
			Tmp |= AUD_BIT_ADC_0_DCHPF_EN | AUD_ADC_0_DCHPF_FC_SEL(fc);
		} else {
			Tmp &= ~AUD_BIT_ADC_0_DCHPF_EN;
		}
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);

		break;

	case ADC2:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
		if (newstate == ENABLE) {
			Tmp &= ~(AUD_MASK_ADC_1_DCHPF_FC_SEL);
			Tmp |= AUD_BIT_ADC_1_DCHPF_EN | AUD_ADC_1_DCHPF_FC_SEL(fc);
		} else {
			Tmp &= ~AUD_BIT_ADC_1_DCHPF_EN;
		}
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);

		break;

	case ADC3:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
		if (newstate == ENABLE) {
			Tmp &= ~(AUD_MASK_ADC_2_DCHPF_FC_SEL);
			Tmp |= AUD_BIT_ADC_2_DCHPF_EN | AUD_ADC_2_DCHPF_FC_SEL(fc);
		} else {
			Tmp &= ~AUD_BIT_ADC_2_DCHPF_EN;
		}
		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);

		break;

	case ADC4:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
		if (newstate == ENABLE) {
			Tmp &= ~(AUD_MASK_ADC_3_DCHPF_FC_SEL);
			Tmp |= AUD_BIT_ADC_3_DCHPF_EN | AUD_ADC_3_DCHPF_FC_SEL(fc);
		} else {
			Tmp &= ~AUD_BIT_ADC_3_DCHPF_EN;
		}
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);

		break;

	default:
		break;
	}
}

/**
  * @brief  Set ADC ASRC mode.
  * @param  adc_sel: select adc channel.
  *          This parameter can be one of the following values:
  *            @arg ADC1
  *            @arg ADC2
  *            @arg ADC3
  *            @arg ADC4
  * @param  sr: the value of sample rate.
  * @param  newstate: the value of tdm mode.
  *			 This parameter can be one of the following values:
  *			   @arg DISABLE
  *			   @arg ENABLE
  * @retval None
  */
void AUDIO_CODEC_SetADCASRC(u32 adc_sel, u32 sr, u32 newstate)
{

	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
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

	Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_6);

	switch (adc_sel) {
	case ADC1:

		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_ADC_0_ASRC_EN;
		} else {
			Tmp &= ~AUD_BIT_ADC_0_ASRC_EN;
		}
		break;

	case ADC2:

		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_ADC_1_ASRC_EN;
		} else {
			Tmp &= ~AUD_BIT_ADC_1_ASRC_EN;
		}
		break;

	case ADC3:

		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_ADC_2_ASRC_EN;
		} else {
			Tmp &= ~AUD_BIT_ADC_2_ASRC_EN;
		}

		break;

	case ADC4:

		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_ADC_3_ASRC_EN;
		} else {
			Tmp &= ~AUD_BIT_ADC_3_ASRC_EN;
		}

		break;

	default:
		break;
	}

	HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_6, Tmp);
}

/**
  * @brief  Mute ADC path.
  * @param  adc_sel: select adc channel.
  *			 This parameter can be one of the following values:
  *			   @arg ADC1
  *			   @arg ADC2
  *			   @arg ADC3
  *			   @arg ADC4
  * @param  newstate: mute or unmute option for ad channel
  *			 This parameter can be one of the following values:
  *			   @arg UNMUTE
  *			   @arg MUTE
  * @return  None
  */
void AUDIO_CODEC_SetADCMute(u32 adc_sel, u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	switch (adc_sel) {
	case ADC1:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
		if (newstate == MUTE) {
			Tmp |= AUD_BIT_ADC_0_AD_MUTE;
		} else {
			Tmp &= ~AUD_BIT_ADC_0_AD_MUTE;
		}
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);

		break;
	case ADC2:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
		if (newstate == MUTE) {
			Tmp |=	AUD_BIT_ADC_1_AD_MUTE;
		} else {
			Tmp &= ~AUD_BIT_ADC_1_AD_MUTE;
		}
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);

		break;
	case ADC3:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
		if (newstate == MUTE) {
			Tmp |= AUD_BIT_ADC_2_AD_MUTE;
		} else {
			Tmp &= ~AUD_BIT_ADC_2_AD_MUTE;
		}
		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);

		break;
	case ADC4:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
		if (newstate == MUTE) {
			Tmp |= AUD_BIT_ADC_3_AD_MUTE;
		} else {
			Tmp &= ~AUD_BIT_ADC_3_AD_MUTE;
		}
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);

		break;
	default:
		break;
	}
}


/**
  * @brief  Set per adc mix mute ad input path to mute or unmute.
  * @param  ad_chn: select adc input path channel
  *			 This parameter can be one of the following values:
  *			   @arg ADC1
  *			   @arg ADC2
  *			   @arg ADC3
  *			   @arg ADC4
  * @param  type: adc path input select
  *			 This parameter can be one of the following values:
  *			   @arg DMIC
  *			   @arg ANAAD
  * @param  newstate: mute or unmute option for ad channel
  *			 This parameter can be one of the following values:
  *			   @arg UNMUTE
  *			   @arg MUTE
  * @return  None
  */
void AUDIO_CODEC_SetADCMixMute(u32 adc_num, u32 type,  			      u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	switch (adc_num) {
	case ADC1:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
		if (newstate == UNMUTE) {
			if (type == DMIC) {
				Tmp &= ~AUD_BIT_ADC_0_DMIC_MIX_MUTE;
			} else {
				Tmp &= ~AUD_BIT_ADC_0_AD_MIX_MUTE;
			}
		} else {
			if (type == DMIC) {
				Tmp |= AUD_BIT_ADC_0_DMIC_MIX_MUTE;
			} else {
				Tmp |= AUD_BIT_ADC_0_AD_MIX_MUTE;
			}
		}
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);

		break;

	case ADC2:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);

		if (newstate == UNMUTE) {
			if (type == DMIC) {
				Tmp &= ~AUD_BIT_ADC_1_DMIC_MIX_MUTE;
			} else {
				Tmp &= ~AUD_BIT_ADC_1_AD_MIX_MUTE;
			}
		} else {
			if (type == DMIC) {
				Tmp |= AUD_BIT_ADC_1_DMIC_MIX_MUTE;
			} else {
				Tmp |= AUD_BIT_ADC_1_AD_MIX_MUTE;
			}
		}

		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);

		break;
	case ADC3:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);

		if (newstate == UNMUTE) {
			if (type == DMIC) {
				Tmp &= ~AUD_BIT_ADC_2_DMIC_MIX_MUTE;
			} else {
				Tmp &= ~AUD_BIT_ADC_2_AD_MIX_MUTE;
			}
		} else {
			if (type == DMIC) {
				Tmp |= AUD_BIT_ADC_2_DMIC_MIX_MUTE;
			} else {
				Tmp |= AUD_BIT_ADC_2_AD_MIX_MUTE;
			}
		}

		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);

		break;
	case ADC4:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);

		if (newstate == UNMUTE) {
			if (type == DMIC) {
				Tmp &= ~AUD_BIT_ADC_3_DMIC_MIX_MUTE;
			} else {
				Tmp &= ~AUD_BIT_ADC_3_AD_MIX_MUTE;
			}
		} else {
			if (type == DMIC) {
				Tmp |= AUD_BIT_ADC_3_DMIC_MIX_MUTE;
			} else {
				Tmp |= AUD_BIT_ADC_3_AD_MIX_MUTE;
			}
		}
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);

		break;
	default:
		break;
	}

}

/**
  * @brief  Select per adc analog ad source.
  *			 This parameter can be one of the following values:
  *			   @arg ADCHN1
  *			   @arg ADCHN2
  *			   @arg ADCHN3
  *			   @arg ADCHN4
  * @param  amic_num: select per analog adc channel src.
  *			 This parameter can be one of the following values:
  *			   @arg AMIC1
  *			   @arg AMIC2
  *			   @arg AMIC3
  * @return  None
  */
void AUDIO_CODEC_SetADCANASrc(u32 ad_chn, u32 amic_num)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	switch (ad_chn) {
	case ADCHN1:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_0_AD_SRC_SEL;
		Tmp |=	AUD_ADC_0_AD_SRC_SEL(amic_num - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);
		break;

	case ADCHN2:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_1_AD_SRC_SEL;
		Tmp |=	AUD_ADC_1_AD_SRC_SEL(amic_num - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);
		break;

	case ADCHN3:

		Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_2_AD_SRC_SEL;
		Tmp |=	AUD_ADC_2_AD_SRC_SEL(amic_num - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);
		break;

	case ADCHN4:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_3_AD_SRC_SEL;
		Tmp |=	AUD_ADC_3_AD_SRC_SEL(amic_num - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
		break;

	default:
		break;
	}
}


/**
  * @brief  Enable or disable per adc ananlog ad filter.
  * @param  adc_num: select ad ana channel
  *			 This parameter can be one of the following values:
  *			   @arg ADC1
  *			   @arg ADC2
  *			   @arg ADC3
  *			   @arg ADC4
  * @param  newstate: enable or disable per ad ana channel.
  *			 This parameter can be one of the following values:
  *			   @arg ENABLE
  *			   @arg DISABLE
  * @return  None
  */
void AUDIO_CODEC_SetADCANAFilter(u32 adc_num, u32 newstate)
{

	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_2);

	switch (adc_num) {

	case ADC1:
		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_AD_ANA_0_EN;
		} else {
			Tmp &= ~AUD_BIT_AD_ANA_0_EN;
		}
		break;

	case ADC2:
		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_AD_ANA_1_EN;
		} else {
			Tmp &= ~AUD_BIT_AD_ANA_1_EN;
		}
		break;

	case ADC3:
		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_AD_ANA_2_EN;
		} else {
			Tmp &= ~AUD_BIT_AD_ANA_2_EN;
		}
		break;

	case ADC4:
		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_AD_ANA_3_EN;
		} else {
			Tmp &= ~AUD_BIT_AD_ANA_3_EN;
		}
		break;

	default:
		break;

	}

	HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_2, Tmp);
}

/**
  * @brief Audio Codec micbias power on or power down.
  * @param  micbias_mode: micbias power on or power down
  *          This parameter can be one of the following values:
  *            @arg POWER_ON:
  *            @arg POWER_DOWN:
  * @return  None
  */
void AUDIO_CODEC_SetMicBiasPowerMode(u32 powermode)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		AUD = AUD_SYS_BASE_S;
		audio_base = AUDIO_REG_BASE_S;
	} else {
		AUD = AUD_SYS_BASE;
		audio_base = AUDIO_REG_BASE;
	}

	if (powermode == POWER_ON) {
		AUD->AUD_MICBIAS_CTL0 |= AUD_BIT_MICBIAS1_POW;
		AUD->AUD_MICBIAS_CTL0 &= ~ AUD_MASK_MICBIAS1_COMP;
		AUD->AUD_MICBIAS_CTL0 |= AUD_MICBIAS1_COMP(1);
		Tmp = HAL_READ32(audio_base, CODEC_AUDIO_CONTROL_1);
		Tmp |= AUD_BIT_CKX_MICBIAS_EN;
		HAL_WRITE32(audio_base, CODEC_AUDIO_CONTROL_1, Tmp);
		AUD->AUD_MICBIAS_CTL0 &= ~(AUD_MASK_MICBIAS1_VSET);
		AUD->AUD_MICBIAS_CTL0 |= AUD_MICBIAS1_VSET(7);
	} else {
		AUD->AUD_MICBIAS_CTL0 &= ~ AUD_BIT_MICBIAS1_POW;
	}

}

/**
  * @brief  Set micbst power on or down.
  * @param  amic_num: select amic channel
  *			 This parameter can be one of the following values:
  *			   @arg AMIC1
  *			   @arg AMIC2
  *			   @arg AMIC3
  * @param  powermode: power on or down mode
  *			 This parameter can be one of the following values:
  *			   @arg NORMALPOWER
  *			   @arg SHUTDOWN
  * @return  None
  */
void AUDIO_CODEC_SetMicBstPowerMode(u32 amic_num, u32 powermode)
{
	if (TrustZone_IsSecure()) {
		AUD = AUD_SYS_BASE_S;
	} else {
		AUD = AUD_SYS_BASE;
	}

	if (powermode == NORMALPOWER) {
		if (amic_num == AMIC1) {
			AUD->AUD_MICBST_CTL0 |= AUD_BIT_MICBST_POWL;
			AUD->AUD_MBIAS_CTL1 &= ~AUD_MASK_MBIAS_ISEL_MICBST_L;
			AUD->AUD_MBIAS_CTL1 |= AUD_MBIAS_ISEL_MICBST_L(2);
			AUD->AUD_ADDA_CTL |= AUD_BIT_DTSDM_CKXEN | AUD_BIT_DTSDM_POW_L;
			AUD->AUD_MBIAS_CTL1 &= ~(AUD_MASK_MBIAS_ISEL_DTSDM_L | AUD_MASK_MBIAS_ISEL_DTSDM_INT1_L);
			AUD->AUD_MBIAS_CTL1 |= AUD_MBIAS_ISEL_DTSDM_L(2) | AUD_MBIAS_ISEL_DTSDM_INT1_L(4);
		} else if (amic_num == AMIC2) {
			AUD->AUD_MICBST_CTL0 |= AUD_BIT_MICBST_POWR;
			AUD->AUD_MBIAS_CTL1 &= ~AUD_MASK_MBIAS_ISEL_MICBST_R;
			AUD->AUD_MBIAS_CTL1 |= AUD_MBIAS_ISEL_MICBST_R(2);
			AUD->AUD_ADDA_CTL |= AUD_BIT_DTSDM_CKXEN | AUD_BIT_DTSDM_POW_R;
			AUD->AUD_MBIAS_CTL1 &= ~(AUD_MASK_MBIAS_ISEL_DTSDM_R | AUD_MASK_MBIAS_ISEL_DTSDM_INT1_R);
			AUD->AUD_MBIAS_CTL1 |= AUD_MBIAS_ISEL_DTSDM_R(2) | AUD_MBIAS_ISEL_DTSDM_INT1_R(4);
		} else {
			AUD->AUD_MICBST_CTL1 |= AUD_BIT_MICBST3_POW;
			AUD->AUD_MBIAS_CTL2 &= ~AUD_MASK_MBIAS_ISEL_MICBST3;
			AUD->AUD_MBIAS_CTL2 |= AUD_MBIAS_ISEL_MICBST3(2);
			AUD->AUD_DTS_CTL |= AUD_BIT_DTSDM3_CKXEN | AUD_BIT_DTSDM3_POW;
			AUD->AUD_MBIAS_CTL0 &= ~(AUD_MASK_MBIAS_ISEL_DTSDM3 | AUD_MASK_MBIAS_ISEL_DTSDM3_INT1);
			AUD->AUD_MBIAS_CTL0 |= AUD_MBIAS_ISEL_DTSDM3(2) | AUD_MBIAS_ISEL_DTSDM3_INT1(4);
		}
	} else {
		if (amic_num == AMIC1) {
			AUD->AUD_MICBST_CTL0 &= ~ AUD_BIT_MICBST_POWL;
			AUD->AUD_ADDA_CTL &= ~AUD_BIT_DTSDM_POW_L;
		} else if (amic_num == AMIC2) {
			AUD->AUD_MICBST_CTL0 &= ~ AUD_BIT_MICBST_POWR;
			AUD->AUD_ADDA_CTL &= ~AUD_BIT_DTSDM_POW_R;
		} else {
			AUD->AUD_MICBST_CTL1 &= ~ AUD_BIT_MICBST3_POW;
			AUD->AUD_DTS_CTL &= ~AUD_BIT_DTSDM3_POW;
		}
	}
}

/**
  * @brief  Set per micbst mute or unmute.
  * @param  amic_sel: select amic channel
  *			 This parameter can be one of the following values:
  *			   @arg AMIC1
  *			   @arg AMIC2
  *			   @arg AMIC3
   * @param  type: mute control
  *			 This parameter can be one of the following values:
  *			   @arg MICIN
  *			   @arg LINEIN: amic3 not support
  * @param  newstate: mute or unmute option for ad channel
  *			 This parameter can be one of the following values:
  *			   @arg UNMUTE: unmute
  *			   @arg MUTE: mute
  * @return  None
  */
void AUDIO_CODEC_SetMicBstChnMute(u32 amic_sel, u32 type, u32 newstate)
{
	if (TrustZone_IsSecure()) {
		AUD = AUD_SYS_BASE_S;
	} else {
		AUD = AUD_SYS_BASE;
	}

	if (newstate == UNMUTE) {
		if (amic_sel == AMIC1) {
			AUD->AUD_MICBST_CTL0 &= ~AUD_MASK_MICBST_MUTE_L;
			AUD->AUD_MICBST_CTL0 |= AUD_MICBST_MUTE_L(type);
		} else if (amic_sel == AMIC2) {
			AUD->AUD_MICBST_CTL0 &= ~AUD_MASK_MICBST_MUTE_R;
			AUD->AUD_MICBST_CTL0 |= AUD_MICBST_MUTE_R(type);
		} else {
			AUD->AUD_MICBST_CTL1 &= ~AUD_BIT_MICBST3_MUTE;
			if (type == LINEIN) {
				DBG_8195A("\n AMIC3 NOT SUPPORT LINEIN MODE\n");
				return;
			}
		}
	} else {
		if (amic_sel == AMIC1) {
			AUD->AUD_MICBST_CTL0 |= AUD_MASK_MICBST_MUTE_L;
		} else if (amic_sel == AMIC2) {
			AUD->AUD_MICBST_CTL0 |= AUD_MASK_MICBST_MUTE_R;
		} else {
			AUD->AUD_MICBST_CTL1 |= AUD_BIT_MICBST3_MUTE;
		}
	}
}

/**
  * @brief  Set micbst single-end or differential mode.
   * @param  amic_num: select amic channel
  *			 This parameter can be one of the following values:
  *			   @arg AMIC1
  *			   @arg AMIC2
  *			   @arg AMIC3
  * @param  mode: micbst single-end or differential mode
  *			 This parameter can be one of the following values:
  *			   @arg SINGLE
  *			   @arg DIFF
  * @return  None
  */
void AUDIO_CODEC_SetMicBstInputMode(u32 amic_num, u32 mode)
{
	if (TrustZone_IsSecure()) {
		AUD = AUD_SYS_BASE_S;
	} else {
		AUD = AUD_SYS_BASE;
	}

	if (amic_num == AMIC1) {
		if (mode == SINGLE) {
			AUD->AUD_MICBST_CTL0 &= ~AUD_BIT_MICBST_ENDFL;
		} else {
			AUD->AUD_MICBST_CTL0 |= AUD_BIT_MICBST_ENDFL;
		}
	} else if (amic_num == AMIC2) {
		if (mode == SINGLE) {
			AUD->AUD_MICBST_CTL0 &= ~AUD_BIT_MICBST_ENDFR;
		} else {
			AUD->AUD_MICBST_CTL0 |= AUD_BIT_MICBST_ENDFR;
		}
	} else {
		if (mode == SINGLE) {
			AUD->AUD_MICBST_CTL1 &= ~AUD_BIT_MICBST3_ENDF;
		} else {
			AUD->AUD_MICBST_CTL1 |= AUD_BIT_MICBST3_ENDF;
		}
	}
}

/**
  * @brief  Set micbst gain.
  * @param  amic_sel: amic select
  *          This parameter can be one of the following values:
  *            @arg AMIC1
  *            @arg AMIC2
  *            @arg AMIC3
  * @param  gain: ADC channel micbst gain select
  *          This parameter can be one of the following values:
  *            @arg MICBST_GAIN_0DB: 0dB
  *            @arg MICBST_Gain_5dB: 5dB
  *            ...
  *            @arg MICBST_Gain_40dB: 40dB
  * @note ADC digital volume is 0dB~+40dB in 5dB step.
  * @return  None
  */
void AUDIO_CODEC_SetMicBstGain(u32 amic_sel, u32 gain)
{
	if (TrustZone_IsSecure()) {
		AUD = AUD_SYS_BASE_S;
	} else {
		AUD = AUD_SYS_BASE;
	}

	if (amic_sel == AMIC1) {
		AUD->AUD_MICBST_CTL0 &= ~(AUD_MASK_MICBST_GSELL);
		AUD->AUD_MICBST_CTL0 |= AUD_MICBST_GSELL(gain) ;
	} else if (amic_sel == AMIC2) {
		AUD->AUD_MICBST_CTL0 &= ~(AUD_MASK_MICBST_GSELR);
		AUD->AUD_MICBST_CTL0 |= AUD_MICBST_GSELR(gain);
	} else {
		AUD->AUD_MICBST_CTL1 &= ~(AUD_MASK_MICBST3_GSEL);
		AUD->AUD_MICBST_CTL1 |= AUD_MICBST3_GSEL(gain);
	}
}

/**
  * @brief  Enable per dimc channel filter.
  * @param  adc_num: select ad ana channel
  *			 This parameter can be one of the following values:
  *			   @arg ADC1
  *			   @arg ADC2
  *			   @arg ADC3
  *			   @arg ADC4
  * @param  newstate: enable or disable per ad ana channel.
  *			 This parameter can be one of the following values:
  *			   @arg ENABLE
  *			   @arg DISABLE
  * @return  None
  */
void AUDIO_CODEC_SetADCDmicFilter(u32 adc_num, u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_2);

	switch (adc_num) {

	case ADC1:
		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_DMIC_0_EN;
		} else {
			Tmp &= ~AUD_BIT_DMIC_0_EN;
		}
		break;

	case ADC2:
		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_DMIC_1_EN;
		} else {
			Tmp &= ~AUD_BIT_DMIC_1_EN;
		}
		break;

	case ADC3:
		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_DMIC_2_EN;
		} else {
			Tmp &= ~AUD_BIT_DMIC_2_EN;
		}
		break;

	case ADC4:
		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_DMIC_3_EN;
		} else {
			Tmp &= ~AUD_BIT_DMIC_3_EN;
		}
		break;

	default:
		break;

	}

	HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_2, Tmp);
}

/**
  * @brief  Enable and select DMIC clk.
  * @param  clk: dmic clk select
  *          This parameter can be one of the following values:
  *            @arg DMIC_5M
  *            @arg DMIC_2P5M
  *            @arg DMIC_1P25M
  *            @arg DMIC_625K
  *            @arg DMIC_312P5K
  *            @arg DMIC_769P2K
  * @param  newstate: dmic clock enable or disable
  *			 This parameter can be one of the following values:
  *			   @arg DISABLE
  *			   @arg ENABLE
  * @return  None
  */
void AUDIO_CODEC_SetDmicClk(u32 clk, u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_3);

	if (newstate == ENABLE) {
		Tmp &= ~AUD_MASK_DMIC1_CLK_SEL;
		Tmp |= AUD_BIT_DMIC1_CLK_EN | AUD_DMIC1_CLK_SEL(clk);
	} else {
		Tmp &= ~AUD_BIT_DMIC1_CLK_EN;
	}

	HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_3, Tmp);

}

/**
  * @brief  mute or unmute  dmic input path.
  * @param  dmic_num: select adc input path channel
  *			 This parameter can be one of the following values:
  *			   @arg ADCHN1
  *			   @arg ADCHN2
  *			   @arg ADCHN3
  *			   @arg ADCHN4
  * @param  newstate: mute or unmute option for per DMIC mix.
  *			 This parameter can be one of the following values:
  *			   @arg DMIC1
  *			   @arg DMIC2
  *			   @arg DMIC3
  *			   @arg DMIC4
  * @return  None
  */
void AUDIO_CODEC_SetDmicSrc(u32 ad_chn, u32 dmic_num)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	switch (ad_chn) {
	case ADCHN1:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_0_DMIC_SRC_SEL;
		Tmp |= AUD_ADC_0_DMIC_SRC_SEL(dmic_num - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);

		break;
	case ADCHN2:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_1_DMIC_SRC_SEL;
		Tmp |= AUD_ADC_1_DMIC_SRC_SEL(dmic_num - 1);
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);

		break;
	case ADCHN3:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_2_DMIC_SRC_SEL;
		Tmp |= (AUD_ADC_2_DMIC_SRC_SEL(dmic_num - 1));
		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);

		break;
	case ADCHN4:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_3_DMIC_SRC_SEL;
		Tmp |= ((AUD_ADC_3_DMIC_SRC_SEL(dmic_num - 1)));
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
		break;

	}
}

/**
* @brief  Set DAC source.
* @param  i2s_sel: select i2s.
*		   This parameter can be one of the following values:
*			 @arg I2S0
* @param  dac_l_src: dac l source.
*		   This parameter can be one of the following values:
*			 @arg I2SL
*			 @arg TESTTONE
* @param  dac_r_src: not support.
*/
void AUDIO_CODEC_SetDACSrc(u32 i2s_sel, u32 dac_l_src, u32 dac_r_src)
{

	u32 Tmp;
	u32 audio_base;
	(void)dac_r_src;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (i2s_sel == I2S0) {

		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_0);
		if (dac_l_src == I2SL) {
			Tmp &= ~AUD_BIT_DAC_L_DA_SRC_SEL;
		} else {
			Tmp |= AUD_BIT_DAC_L_DA_SRC_SEL;
		}
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_0, Tmp);
	} else {
		return;
	}

}

/**
  * @brief  Enable or disable da and da fifo channel clock.
  * @param  channel: select dac channel.
  *			 This parameter can be one of the following values:
  *			   @arg DAC_L
  * @param  newstate: enable or disable da channel.
  *			 This parameter can be one of the following values:
  *			   @arg ENABLE
  *			   @arg DISABLE
  * @return  None
  */
void AUDIO_CODEC_EnableDAC(u32 channel, u32 newstate)
{

	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (channel == DAC_L) {

		Tmp = HAL_READ32(audio_base, CODEC_CLOCK_CONTROL_1);

		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_DA_L_EN | AUD_BIT_DA_FIFO_EN | AUD_BIT_MOD_L_EN;
		} else {
			Tmp &= ~(AUD_BIT_DA_L_EN | AUD_BIT_DA_FIFO_EN | AUD_BIT_MOD_L_EN);
		}
		HAL_WRITE32(audio_base, CODEC_CLOCK_CONTROL_1, Tmp);
	} else {
		return;
	}

}

/**
  * @brief  Mute DAC path.
  * @param  channel: select dac channel.
  *			 This parameter can be one of the following values:
  *			   @arg DAC_L
  * @param  newstate: mute or unmute option for da channel
  *			 This parameter can be one of the following values:
  *			   @arg UNMUTE: unmute
  *			   @arg MUTE: mute
  */
void AUDIO_CODEC_SetDACMute(u32 channel, u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (channel == DAC_L) {

		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_1);
		if (newstate == UNMUTE) {
			Tmp &= ~AUD_BIT_DAC_L_DA_MUTE;
		} else {
			Tmp |= AUD_BIT_DAC_L_DA_MUTE;
		}
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_1, Tmp);
	} else {
		return;
	}
}

/**
  * @brief  Set the gain of DAC digital volume.
  * @param  channel: the value of dac path.
  *          This parameter can be one of the following values:
  *            @arg 0: DAC_L
  * @param  gain: the value of gain.
  *          This parameter can be -62.5dB~0dB in 0.375dB step
  *            @arg 0x00 -65.625dB
  *            @arg 0xaf 0dB
  * @retval None
  */
void AUDIO_CODEC_SetDACVolume(u32 channel, u32 gain)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (channel == DAC_L) {

		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_0);
		Tmp &= ~ AUD_MASK_DAC_L_DA_GAIN;
		Tmp |= AUD_DAC_L_DA_GAIN(gain);
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_0, Tmp);
	} else {
		return;
	}
}

/**
  * @brief  Set DAC HPF enable.
  * @param  channel: the value of dac path.
  *          This parameter can be one of the following values:
  *            @arg 0: DAC_L
  * @param  newstate: enable or disable per ad channel HPF mode
  *			 This parameter can be one of the following values:
  *			   @arg ENABLE
  *			   @arg DISABLE
  * @return  None
  */
void AUDIO_CODEC_SetDACHPF(u32 channel, u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (channel == DAC_L) {

		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_1);
		if (newstate == ENABLE) {
			Tmp |= AUD_BIT_DAC_L_DAHPF_EN;
		} else {
			Tmp &= ~AUD_BIT_DAC_L_DAHPF_EN;
		}
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_1, Tmp);
	} else {
		return;
	}
}


/**
  * @brief  Enable or disable DAC ASRC mode and select asrc rate.
  * @param  sr: the dac sample rate.
  * @param  newstate:
  *			 This parameter can be one of the following values:
  *			   @arg DISABLE
  *			   @arg ENABLE
  * @retval None
  */
void AUDIO_CODEC_SetDACASRC(u32 sr, u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (newstate == ENABLE) {
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
  * @brief  Set DAC power mode.
  * @param  channel: the value of dac path.
  *          This parameter can be one of the following values:
  *            @arg 0: DAC_L
  * @param  newstate: enable or disable dac power.
  *			 This parameter can be one of the following values:
  *			   @arg POWER_ON
  *			   @arg POWER_DOWN
  */
void AUDIO_CODEC_SetDACPowerMode(u32 channel, u32 powermode)
{
	if (TrustZone_IsSecure()) {
		AUD = AUD_SYS_BASE_S;
	} else {
		AUD = AUD_SYS_BASE;
	}

	if (channel == DAC_L) {

		if (powermode == POWER_ON) {
			AUD->AUD_ADDA_CTL |= AUD_BIT_DAC_CKXEN | AUD_BIT_DAC_POW;
			AUD->AUD_ADDA_CTL &= ~AUD_BIT_DAC_CKXSEL;
			AUD->AUD_MBIAS_CTL0 &= ~(AUD_MASK_MBIAS_ISEL_DAC);
			AUD->AUD_MBIAS_CTL0 |= AUD_MBIAS_ISEL_DAC(2);
		} else {
			AUD->AUD_ADDA_CTL &= ~(AUD_BIT_DAC_CKXEN | AUD_BIT_DAC_POW);
		}
	} else {
		return;
	}

}

/**
  * @brief  Set lineout power mode.
  * @param  channel: the value of dac path.
  *          This parameter can be one of the following values:
  *            @arg 0: DAC_L
  * @param  newstate: enable or disable dac power.
  *			 This parameter can be one of the following values:
  *			   @arg POWER_ON
  *			   @arg POWER_DOWN
  */
void AUDIO_CODEC_SetLineOutPowerMode(u32 channel, u32 powermode)
{
	if (TrustZone_IsSecure()) {
		AUD = AUD_SYS_BASE_S;
	} else {
		AUD = AUD_SYS_BASE;
	}

	if (channel == DAC_L) {
		if (powermode == POWER_ON) {
			AUD->AUD_LO_CTL |= AUD_BIT_LO_POW;
			AUD->AUD_LO_CTL &= ~AUD_BIT_LO_ENDP;
			AUD->AUD_MBIAS_CTL0 &= ~AUD_MASK_MBIAS_ISEL_LO;
			AUD->AUD_MBIAS_CTL0 |= AUD_MBIAS_ISEL_LO(2);

		} else {
			AUD->AUD_LO_CTL &= ~AUD_BIT_LO_POW;
		}
	} else {
		return;
	}
}

/**
  * @brief  Set Line out single-end or differential mode.
  * @param  channel: the value of dac path.
  *          This parameter can be one of the following values:
  *            @arg 0: DAC_L
  * @param  mode:
  *			 This parameter can be one of the following values:
  *			   @arg SINGLE
  *			   @arg DIFF
  * @return  None
  */
void AUDIO_CODEC_SetLineOutMode(u32 channel, u32 mode)
{
	if (TrustZone_IsSecure()) {
		AUD = AUD_SYS_BASE_S;
	} else {
		AUD = AUD_SYS_BASE;
	}

	if (channel == DAC_L) {
		if (mode == SINGLE) {
			AUD->AUD_LO_CTL |= AUD_BIT_LO_SE;
		} else {
			AUD->AUD_LO_CTL &= ~AUD_BIT_LO_SE;
		}
	} else {
		return;
	}
}

/**
  * @brief  Set Line out mute control: dac in or analog in.
  * @param  channel: the value of dac path.
  *          This parameter can be one of the following values:
  *            @arg 0: DAC_L
  * @param  type: dac in or analog in.
  *			 This parameter can be one of the following values:
  *			   @arg DACIN
  *			   @arg ANALOGIN
  * @param  newstate:
  *			 This parameter can be one of the following values:
  *			   @arg UNMUTE
  *			   @arg MUTE
  * @return  None
  */
void AUDIO_CODEC_SetLineOutMute(u32 channel, u32 type, u32 newstate)
{
	if (TrustZone_IsSecure()) {
		AUD = AUD_SYS_BASE_S;
	} else {
		AUD = AUD_SYS_BASE;
	}

	if (channel == DAC_L) {

		if (newstate == UNMUTE) {
			AUD->AUD_LO_CTL &= ~AUD_MASK_LO_M;
			AUD->AUD_LO_CTL |= AUD_LO_M(type);
		} else {
			AUD->AUD_LO_CTL |= AUD_MASK_LO_M;
		}
	} else {
		return;
	}
}


/**
  * @brief  Enable and select or disalbe PDM clk.
  * @param  channel: the value of dac path.
  *          This parameter can be one of the following values:
  *            @arg 0: DAC_L
  * @param  clk: PDM clk select
  *          This parameter can be one of the following values:
  *            @arg PDM_5M
  *            @arg PDM_2P5M
  *            @arg PDM_6P67M
  * @param  newstate: pdm enable or disable
  *			 This parameter can be one of the following values:
  *			   @arg DISABLE
  *			   @arg ENABLE
  * @return  None
  */
void AUDIO_CODEC_SetPDMClk(u32 channel, u32 clk, u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (channel == DAC_L) {

		if (newstate == ENABLE) {
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
	} else {
		return;
	}
}


/**
  * @brief  Set and select testtone mode.
  * @param  fc: the value of test frequency.
  *          This parameter can be one of the following values:
  *          @arg fc
  *          frequency= (fs/192)*(fc+1)Hz
  * @param  newstate: the status of testtone mode.
  *          This parameter can be the following values:
  *            @arg ENABLE
  *            @arg DISABLE
  * @retval None
  */
void AUDIO_CODEC_SetTesttone(u32 fc, u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (newstate == ENABLE) {
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
  * @brief  Set daad loopback mode.
  * @param  newstate: the status of testtone mode.
  *          This parameter can be the following values:
  *            @arg ENABLE
  *            @arg DISABLE
  * @retval None
  */
void AUDIO_CODEC_SetDAADLPBK(u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_AUDIO_CONTROL_0);

	if (newstate == ENABLE) {
		Tmp |= AUD_BIT_DAAD_LPBK_EN;
	} else {
		Tmp &= ~AUD_BIT_DAAD_LPBK_EN;
	}
	HAL_WRITE32(audio_base, CODEC_AUDIO_CONTROL_0, Tmp);

}

/**
  * @brief  Set adda loopback mode.
  * @param  newstate: the status of testtone mode.
  *          This parameter can be the following values:
  *            @arg ENABLE
  *            @arg DISABLE
  * @retval None
  */
void AUDIO_CODEC_SetADDALPBK(u32 newstate)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_AUDIO_CONTROL_0);
	if (newstate == ENABLE) {
		Tmp |= AUD_BIT_ADDA_LPBK_EN;
	} else {
		Tmp &= ~AUD_BIT_ADDA_LPBK_EN;
	}
	HAL_WRITE32(audio_base, CODEC_AUDIO_CONTROL_0, Tmp);

}

/**
  * @brief  select ADC path decimation source
  * @param  adc_sel: select adc channel
  *			 This parameter can be one of the following values:
  *			   @arg ADC1
  *			   @arg ADC2
  *			   @arg ADC3
  *			   @arg ADC4
  * @param  type: select decimation source
  *			 This parameter can be one of the following values:
  *			   @arg AUDIO_ADC
  *			   @arg ANC
  *			   @arg MUSIC
  *			   @arg ANCANDMUSIC
  * @return  None
  */
void AUDIO_CODEC_SetADCDecimationSrc(u32 adc_sel, u32 type)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (adc_sel == ADC1) {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
		Tmp &= ~(AUD_MASK_ADC_0_DECI_SRC_SEL);
		Tmp |=	AUD_ADC_0_DECI_SRC_SEL(type);
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);
	} else if (adc_sel == ADC2) {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
		Tmp &= ~(AUD_MASK_ADC_1_DECI_SRC_SEL);
		Tmp |=	AUD_ADC_1_DECI_SRC_SEL(type);
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);
	} else if (adc_sel == ADC3) {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
		Tmp &= ~(AUD_MASK_ADC_2_DECI_SRC_SEL);
		Tmp |=	AUD_ADC_2_DECI_SRC_SEL(type);
		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);
	} else {
		Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
		Tmp &= ~(AUD_MASK_ADC_3_DECI_SRC_SEL);
		Tmp |=	AUD_ADC_3_DECI_SRC_SEL(type);
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
	}
}

/**
  * @brief  Set DAC path zero detection function.
  * @param  channel: select dac channel.
  *			 This parameter can be one of the following values:
  *			   @arg DACL
  *			   @arg DACR��not support
  * @param  type: zero detection function select
  *			 This parameter can be one of the following values:
  *			   @arg IME: immediate change
  *			   @arg ZDET_IME: zero detection & immediate change
  *			   @arg ZDET_STEP: zdet & step
  *			   @arg ZDET_TIMEOUT: zdet & timeout step
  */
void AUDIO_CODEC_SetDACZDET(u32 channel, u32 type)
{

	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (channel == DAC_L) {
		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_1);
		Tmp &= ~AUD_MASK_DAC_L_DA_ZDET_FUNC;
		Tmp |= AUD_DAC_L_DA_ZDET_FUNC(type);
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_1, Tmp);
	} else {
		return;
	}
}

/**
  * @brief  Set DAC path zero detection time out select.
  * @param  channel: select dac channel.
  *			 This parameter can be one of the following values:
  *			   @arg DACL
  *			   @arg DACRL: not support
  * @param  time_out: zero detection time out select select
  *			 This parameter can be one of the following values:
  *			   @arg 0: 1024*16 samples
  *			   @arg 1: 1024*32 samples
  *			   @arg 2: 1024*64 samples
  *			   @arg 3: 64 samples
  */
void AUDIO_CODEC_SetDACZDETTimeOut(u32 channel, u32 time_out)
{

	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	if (channel == DAC_L) {

		Tmp = HAL_READ32(audio_base, CODEC_DAC_L_CONTROL_1);
		Tmp &= ~AUD_MASK_DAC_L_DA_ZDET_TOUT;
		Tmp |= AUD_DAC_L_DA_ZDET_TOUT(time_out);
		HAL_WRITE32(audio_base, CODEC_DAC_L_CONTROL_1, Tmp);

	} else {
		return;
	}
}


/**
  * @brief  Set ADC path zero detection function.
  * @param  channel: select dac channel.
  *			 This parameter can be one of the following values:
  *			   @arg ADC1
  *			   @arg ADC2
  *			   @arg ADC3
  *			   @arg ADC4
  * @param  type: zero detection function select
  *			 This parameter can be one of the following values:
  *			   @arg IME: immediate change
  *			   @arg ZDET_IME: zero detection & immediate change
  *			   @arg ZDET_STEP: zdet & step
  *			   @arg ZDET_TIMEOUT: zdet & timeout step
  */
void AUDIO_CODEC_SetADCZDET(u32 adc_sel, u32 type)
{

	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	switch (adc_sel) {
	case ADC1:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_0_AD_ZDET_FUNC;
		Tmp |= AUD_ADC_0_AD_ZDET_FUNC(type);
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);
		break;

	case ADC2:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_1_AD_ZDET_FUNC;
		Tmp |= AUD_ADC_1_AD_ZDET_FUNC(type);
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);
		break;

	case ADC3:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_2_AD_ZDET_FUNC;
		Tmp |= AUD_ADC_2_AD_ZDET_FUNC(type);
		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);
		break;

	case ADC4:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_3_AD_ZDET_FUNC;
		Tmp |= AUD_ADC_3_AD_ZDET_FUNC(type);
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
		break;
	default:
		break;
	}
}


/**
  * @brief	Set ADC path zero detection function.
  * @param	channel: select dac channel.
  * 		 This parameter can be one of the following values:
  * 		   @arg ADC1
  * 		   @arg ADC2
  * 		   @arg ADC3
  * 		   @arg ADC4
  * @param	time_out: zero detection time out select select
  * 		 This parameter can be one of the following values:
  * 		   @arg 0: 1024*16 samples
  * 		   @arg 1: 1024*32 samples
  * 		   @arg 2: 1024*64 samples
  * 		   @arg 3: 64 samples
  */
void AUDIO_CODEC_SetADCZDETTimeOut(u32 adc_sel, u32 time_out)
{

	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	switch (adc_sel) {
	case ADC1:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_0_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_0_AD_ZDET_TOUT;
		Tmp |= AUD_ADC_0_AD_ZDET_TOUT(time_out);
		HAL_WRITE32(audio_base, CODEC_ADC_0_CONTROL_0, Tmp);
		break;

	case ADC2:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_1_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_1_AD_ZDET_TOUT;
		Tmp |= AUD_ADC_1_AD_ZDET_TOUT(time_out);
		HAL_WRITE32(audio_base, CODEC_ADC_1_CONTROL_0, Tmp);
		break;

	case ADC3:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_2_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_2_AD_ZDET_TOUT;
		Tmp |= AUD_ADC_2_AD_ZDET_TOUT(time_out);
		HAL_WRITE32(audio_base, CODEC_ADC_2_CONTROL_0, Tmp);
		break;

	case ADC4:
		Tmp = HAL_READ32(audio_base, CODEC_ADC_3_CONTROL_0);
		Tmp &= ~AUD_MASK_ADC_3_AD_ZDET_TOUT;
		Tmp |= AUD_ADC_3_AD_ZDET_TOUT(time_out);
		HAL_WRITE32(audio_base, CODEC_ADC_3_CONTROL_0, Tmp);
		break;
	default:
		break;
	}
}


/**
  * @brief	Audio codec record flow for test.
  */
void AUDIO_CODEC_Record(u32 i2s_sel, u32 type, I2S_InitTypeDef *I2S_InitStruct)
{
	AUDIO_CODEC_SetAudioIP(ENABLE);
	AUDIO_CODEC_SetI2SIP(i2s_sel, ENABLE);
	AUDIO_CODEC_SetI2SSRC(i2s_sel, INTERNAL_SPORT);
	AUDIO_CODEC_SetADCSRSrc(SOURCE1, I2S_InitStruct->CODEC_SelI2SRxSR);
	AUDIO_CODEC_SetANAClk(ENABLE);
	AUDIO_CODEC_SetI2SParameters(i2s_sel, ADCPATH, I2S_InitStruct);

	if (I2S_InitStruct->CODEC_SelRxI2STdm == I2S_NOTDM || I2S_InitStruct->CODEC_SelRxI2STdm == I2S_TDM4) {
		AUDIO_CODEC_EnableADC(ADCHN1, ENABLE);
		AUDIO_CODEC_EnableADC(ADCHN2, ENABLE);
		AUDIO_CODEC_SetADCVolume(ADC1, 0X2F);
		AUDIO_CODEC_SetADCVolume(ADC2, 0X2F);
		AUDIO_CODEC_SetADCHPF(ADC1, 3, ENABLE);
		AUDIO_CODEC_SetADCHPF(ADC2, 3, ENABLE);
		AUDIO_CODEC_SetADCMute(ADC1, UNMUTE);
		AUDIO_CODEC_SetADCMute(ADC2, UNMUTE);
	}

	if (I2S_InitStruct->CODEC_SelRxI2STdm == I2S_TDM4) {
		AUDIO_CODEC_EnableADC(ADCHN3, ENABLE);
		AUDIO_CODEC_EnableADC(ADCHN4, ENABLE);
		AUDIO_CODEC_SetADCVolume(ADC3, 0X2F);
		AUDIO_CODEC_SetADCVolume(ADC4, 0X2F);
		AUDIO_CODEC_SetADCHPF(ADC3, 3, ENABLE);
		AUDIO_CODEC_SetADCHPF(ADC4, 3, ENABLE);
		AUDIO_CODEC_SetADCMute(ADC3, UNMUTE);
		AUDIO_CODEC_SetADCMute(ADC4, UNMUTE);
	}

	if (type == APP_DAAD_LPBK) {

		if (I2S_InitStruct->CODEC_SelRxI2STdm == I2S_NOTDM || I2S_InitStruct->CODEC_SelRxI2STdm == I2S_TDM4) {
			AUDIO_CODEC_SetADCMixMute(ADC1, ANAAD, UNMUTE);
			AUDIO_CODEC_SetADCMixMute(ADC2, ANAAD, UNMUTE);
			AUDIO_CODEC_SetADCANAFilter(ADC1, ENABLE);
			AUDIO_CODEC_SetADCANAFilter(ADC2, ENABLE);
		}

		if (I2S_InitStruct->CODEC_SelRxI2STdm == I2S_TDM4) {
			AUDIO_CODEC_SetADCMixMute(ADC3, ANAAD, UNMUTE);
			AUDIO_CODEC_SetADCMixMute(ADC4, ANAAD, UNMUTE);
			AUDIO_CODEC_SetADCANAFilter(ADC3, ENABLE);
			AUDIO_CODEC_SetADCANAFilter(ADC4, ENABLE);
		}
	}


	if (type == APP_AMIC_RECORD) {

		AUDIO_CODEC_SetLDOMode(POWER_ON);
		AUDIO_CODEC_SetMicBiasPowerMode(POWER_ON);

		if (I2S_InitStruct->CODEC_SelRxI2STdm == I2S_NOTDM || I2S_InitStruct->CODEC_SelRxI2STdm == I2S_TDM4) {

			AUDIO_CODEC_DisPAD(PAD_MIC1);
			AUDIO_CODEC_DisPAD(PAD_MIC2);

			AUDIO_CODEC_SetADCMixMute(ADC1, ANAAD, UNMUTE);
			AUDIO_CODEC_SetADCMixMute(ADC2, ANAAD, UNMUTE);

			AUDIO_CODEC_SetADCANAFilter(ADC1, ENABLE);
			AUDIO_CODEC_SetADCANAFilter(ADC2, ENABLE);

			AUDIO_CODEC_SetADCANASrc(ADCHN1, AMIC1);
			AUDIO_CODEC_SetADCANASrc(ADCHN2, AMIC2);

			AUDIO_CODEC_SetMicBstPowerMode(AMIC1, NORMALPOWER);
			AUDIO_CODEC_SetMicBstPowerMode(AMIC2, NORMALPOWER);

			AUDIO_CODEC_SetMicBstChnMute(AMIC1, MICIN, UNMUTE);
			AUDIO_CODEC_SetMicBstChnMute(AMIC2, MICIN, UNMUTE);

			AUDIO_CODEC_SetMicBstInputMode(AMIC1, DIFF);
			AUDIO_CODEC_SetMicBstInputMode(AMIC2, DIFF);

			AUDIO_CODEC_SetMicBstGain(AMIC1, MICBST_GAIN_0DB);
			AUDIO_CODEC_SetMicBstGain(AMIC2, MICBST_GAIN_0DB);
		}

		if (I2S_InitStruct->CODEC_SelRxI2STdm == I2S_TDM4) {

			AUDIO_CODEC_DisPAD(PAD_MIC3);
			AUDIO_CODEC_SetADCMixMute(ADC3, ANAAD, UNMUTE);
			AUDIO_CODEC_SetADCANAFilter(ADC3, ENABLE);
			AUDIO_CODEC_SetADCANASrc(ADCHN3, AMIC3);
			AUDIO_CODEC_SetMicBstPowerMode(AMIC3, NORMALPOWER);
			AUDIO_CODEC_SetMicBstChnMute(AMIC3, MICIN, UNMUTE);
			AUDIO_CODEC_SetMicBstInputMode(AMIC3, DIFF);
			AUDIO_CODEC_SetMicBstGain(AMIC3, MICBST_GAIN_0DB);
		}

	}

	if (type == APP_DMIC_RECORD) {

		AUDIO_CODEC_SetDmicClk(DMIC_2P5M, ENABLE);

		if (I2S_InitStruct->CODEC_SelRxI2STdm == I2S_NOTDM || I2S_InitStruct->CODEC_SelRxI2STdm == I2S_TDM4) {
			AUDIO_CODEC_SetADCMixMute(ADC1, DMIC, UNMUTE);
			AUDIO_CODEC_SetADCMixMute(ADC2, DMIC, UNMUTE);
			AUDIO_CODEC_SetADCDmicFilter(ADC1, ENABLE);
			AUDIO_CODEC_SetADCDmicFilter(ADC2, ENABLE);
			AUDIO_CODEC_SetDmicSrc(ADCHN1, DMIC1);
			AUDIO_CODEC_SetDmicSrc(ADCHN2, DMIC2);
		}

		if (I2S_InitStruct->CODEC_SelRxI2STdm == I2S_TDM4) {
			AUDIO_CODEC_SetADCMixMute(ADC3, DMIC, UNMUTE);
			AUDIO_CODEC_SetADCMixMute(ADC4, DMIC, UNMUTE);
			AUDIO_CODEC_SetADCDmicFilter(ADC3, ENABLE);
			AUDIO_CODEC_SetADCDmicFilter(ADC4, ENABLE);
			AUDIO_CODEC_SetDmicSrc(ADCHN3, DMIC3);
			AUDIO_CODEC_SetDmicSrc(ADCHN4, DMIC4);
		}
	}
}

/**
  * @brief	Audio codec playback flow for test.
  */
void AUDIO_CODEC_Playback(u32 i2s_sel, u32 type, I2S_InitTypeDef *I2S_InitStruct)
{
	AUDIO_CODEC_SetAudioIP(ENABLE);
	AUDIO_CODEC_SetI2SIP(i2s_sel, ENABLE);
	AUDIO_CODEC_SetI2SSRC(i2s_sel, INTERNAL_SPORT);
	AUDIO_CODEC_SetI2SParameters(i2s_sel, DACPATH, I2S_InitStruct);
	AUDIO_CODEC_SetDACSRSrc(SOURCE0, I2S_InitStruct->CODEC_SelI2STxSR);
	AUDIO_CODEC_EnableDAC(DAC_L, ENABLE);
	AUDIO_CODEC_SetDACHPF(DAC_L, ENABLE);
	AUDIO_CODEC_SetDACMute(DAC_L, UNMUTE);
	AUDIO_CODEC_SetDACVolume(DAC_L, 0XAF);

	if (type == APP_DAAD_LPBK) {

		AUDIO_CODEC_SetDACSrc(i2s_sel, I2SL, (uint32_t)NULL);
		AUDIO_CODEC_SetANAClk(ENABLE);
	}

	if (type == APP_LINE_OUT) {

		AUDIO_CODEC_SetDACSrc(i2s_sel, I2SL, (uint32_t)NULL);
		AUDIO_CODEC_SetANAClk(ENABLE);
		AUDIO_CODEC_SetLDOMode(POWER_ON);
		AUDIO_CODEC_DisPAD(PAD_DACL);
		AUDIO_CODEC_SetDACPowerMode(DAC_L, POWER_ON);
		AUDIO_CODEC_SetLineOutPowerMode(DAC_L, POWER_ON);
		AUDIO_CODEC_SetLineOutMode(DAC_L, DIFF);
		AUDIO_CODEC_SetLineOutMute(DAC_L, DACIN, UNMUTE);
	}

	if (type == APP_PDM_OUT) {
		AUDIO_CODEC_SetDACSrc(i2s_sel, I2SL, (uint32_t)NULL);
		AUDIO_CODEC_SetPDMClk(DAC_L, PDM_5M, ENABLE);
	}

	if (type == APP_TESTTONE) {
		AUDIO_CODEC_SetDACSrc(i2s_sel, TESTTONE, (uint32_t)NULL);
		AUDIO_CODEC_SetTesttone(3, ENABLE);
	}
}



