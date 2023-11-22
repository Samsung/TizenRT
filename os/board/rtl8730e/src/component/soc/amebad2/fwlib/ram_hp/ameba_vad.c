/**
  ******************************************************************************
  * @file    ameba_vad.c
  * @author  Hank_Li
  * @version V1.0.0
  * @date    2021-02-02
  * @brief   This file contains all the functions prototypes for VAD.
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
#include "ameba_vad_pc.h"
#include "sysreg_lsys.h"
#include "sysreg_pll.h"
#include "sysreg_pmc.h"
#include "hal_platform.h"
#include "ameba_vad.h"

/**
  * @brief  Power up the VAD module. Must be called before other configurations.
  * @param  clock_source
  *         This parameter can be one of the following values:
  *            @arg VAD_OSC
  *            @arg VAD_XTAL
  */
void VAD_Power_Init(u32 clock_source)
{
	u32 reg_val = 0;
	if (clock_source == VAD_OSC) {
		reg_val = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		reg_val &= (~LSYS_BIT_CKSL_VAD);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, reg_val);

		reg_val = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_CLK);
		reg_val |= APBPeriph_OSC4M_FORCE4_CLOCK;
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_CLK, reg_val);
	} else {
		reg_val = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		reg_val |= (LSYS_BIT_CKSL_VAD);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, reg_val);
	}

	reg_val = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	reg_val &= (~LSYS_BIT_CKSL_VADM);
	reg_val |= LSYS_CKSL_VADM(0);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, reg_val);
}

/**
  * @brief  Set VAD 24bit
  */
void VAD_Set_Bit(void)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)(VAD_REG_BASE_S));
	} else {
		VAD = ((VAD_TypeDef *)(VAD_REG_BASE));
	}

	VAD->VAD_BUF_CTRL0 &= ~VAD_BIT_BIT_SEL;
}

/**
  * @brief  Initialize the ADC path of the codec to be used
  * @param  codec_index
  *         This parameter can be one of the following values:
  *            @arg VAD_CODEC_ZERO: use codec0
  *            @arg VAD_CODEC_TWO: use codec1
  *            @arg VAD_CODEC_THREE: use codec2
  *            @arg VAD_CODEC_FOUR: use codec3
  */
void VAD_ADC_Codec_Init(u32 codec_index)
{
	assert_param(codec_index <= VAD_CODEC_THREE);
	u32 reg_value = 0;

	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	reg_value = (VAD -> VAD_CODEC_CTRLx[codec_index]);
	reg_value &= (~(VAD_BIT_PC_ADC_x_AD_MIX_MUTE));
	reg_value  |= VAD_BIT_PC_ADC_x_DMIC_MIX_MUTE;
	reg_value  &= (~VAD_BIT_PC_ADC_x_AD_MUTE);
	VAD -> VAD_CODEC_CTRLx[codec_index] = reg_value;

	VAD -> VAD_CLK_CTRL |= VAD_BIT_PC_TCON_AD_ANA_CLK_SEL;//bit0->1 2M
	VAD -> VAD_CLK_CTRL |= VAD_BIT_PC_TCON_AD_ANA_EN;

	switch (codec_index) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_CLK_CTRL |= (VAD_BIT_PC_TCON_ADC_0_EN);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_CLK_CTRL |= (VAD_BIT_PC_TCON_ADC_1_EN);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_CLK_CTRL |= (VAD_BIT_PC_TCON_ADC_2_EN);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_CLK_CTRL |= (VAD_BIT_PC_TCON_ADC_3_EN);
		break;
	default:
		break;
	}
}

/**
  * @brief  Set VAD ADC Clock to PC_VAD
  */
void VAD_ADC_Clock_Enable(u32 clock_sel)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	if (clock_sel == CLK_FOR_PC_VAD_CODEC) {
		VAD -> VAD_BUF_CTRL0 |= VAD_BIT_ADC_ANALOG_CLK_SEL;
	} else {
		VAD -> VAD_BUF_CTRL0 &= ~VAD_BIT_ADC_ANALOG_CLK_SEL;

	}
}

/**
  * @brief  Initialize the ADC
  * @param  ADC_index
  *         This parameter can be one of the following values:
  *            @arg VAD_ADC_ZERO: use adc0
  *            @arg VAD_ADC_ONE: use adc1
  *            @arg VAD_ADC_TWO: use adc2
  *            @arg VAD_ADC_THREE: use adc3
  *            @arg VAD_ADC_FOUR: use adc4
  */
void VAD_ADC_Init(u32 ADC_index)
{
	assert_param(ADC_index <= VAD_ADC_FOUR);

	AUDIO_CODEC_SetMicBstChnMute(ADC_index + 1, MICIN, UNMUTE);
	AUDIO_CODEC_SetMicBstPowerMode(ADC_index + 1, LOWPOWER);
	AUDIO_CODEC_SetMicBstInputMode(ADC_index + 1, DIFF);
}



/**
  * @brief  Initialize the DMIC path of the codec to be used
  * @param  codec_index
  *         This parameter can be one of the following values:
  *            @arg VAD_CODEC_ZERO: use codec0
  *            @arg VAD_CODEC_TWO: use codec1
  *            @arg VAD_CODEC_THREE: use codec2
  *            @arg VAD_CODEC_FOUR: use codec3
  */
void VAD_DMIC_Codec_Init(u32 codec_index)
{
	assert_param(codec_index <= VAD_CODEC_THREE);

	u32 reg_value = 0;

	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}
	reg_value = (VAD -> VAD_CODEC_CTRLx[codec_index]);
	reg_value |= (VAD_BIT_PC_ADC_x_AD_MIX_MUTE);
	reg_value &= (~VAD_BIT_PC_ADC_x_DMIC_MIX_MUTE);
	reg_value &= (~VAD_BIT_PC_ADC_x_AD_MUTE);
	VAD -> VAD_CODEC_CTRLx[codec_index] = reg_value;

	VAD -> VAD_CLK_CTRL |= VAD_BIT_PC_TCON_DMIC_EN;
}

/**
  * @brief  delay for a period
  * @param  time_ms
  *         You can call this function when you need a delay， e.g, the dmic need a period of time
  *         to output stable signal, and you need to wait for a period in this situation
  */
void VAD_Delay(u32 time_ms)
{
	DelayMs(time_ms);
}

/**
  * @brief  start the codec
  * @param  codec_index
  *         This parameter can be one of the following values:
  *            @arg VAD_CODEC_ZERO: use codec0
  *            @arg VAD_CODEC_TWO: use codec1
  *            @arg VAD_CODEC_THREE: use codec2
  *            @arg VAD_CODEC_FOUR: use codec3
  */
void VAD_DMIC_Codec_Enable(u32 codec_index)
{
	assert_param(codec_index <= VAD_CODEC_THREE);

	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}
	switch (codec_index) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_CLK_CTRL |= (VAD_BIT_PC_TCON_DMIC_SRC_0_EN | VAD_BIT_PC_TCON_ADC_0_EN);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_CLK_CTRL |= (VAD_BIT_PC_TCON_DMIC_SRC_1_EN | VAD_BIT_PC_TCON_ADC_1_EN);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_CLK_CTRL |= (VAD_BIT_PC_TCON_DMIC_SRC_2_EN | VAD_BIT_PC_TCON_ADC_2_EN);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_CLK_CTRL |= (VAD_BIT_PC_TCON_DMIC_SRC_3_EN | VAD_BIT_PC_TCON_ADC_3_EN);
		break;
	default:
		break;
	}
}

/**
  * @brief  set the codec index and its related ADC data source
  * @param  codec_index
  *         This parameter can be one of the following values:
  *            @arg VAD_CODEC_ZERO: use codec0
  *            @arg VAD_CODEC_TWO: use codec1
  *            @arg VAD_CODEC_THREE: use codec2
  *            @arg VAD_CODEC_FOUR: use codec3
  * @param  adc_index
  *         This parameter can be one of the following values:
  *            @arg VAD_ADC_ZERO: use ADC0
  *            @arg VAD_ADC_ONE: use ADC1
  *            @arg VAD_ADC_TWO: use ADC2
  *            @arg VAD_ADC_THREE: use ADC3
  *            @arg VAD_ADC_FOUR: use ADC4
  */
void VAD_ADC_Select(u32 codec_index, u32 adc_index)
{
	assert_param(codec_index <= VAD_CODEC_THREE);

	assert_param(adc_index <= VAD_ADC_FOUR);

	VAD_TypeDef *VAD;

	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	switch (codec_index) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_FILTER_CTRL |= (VAD_PC_ADC_SDM_DATA_ADC_0_SEL(adc_index));
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_FILTER_CTRL |= (VAD_PC_ADC_SDM_DATA_ADC_1_SEL(adc_index));
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_FILTER_CTRL |= (VAD_PC_ADC_SDM_DATA_ADC_2_SEL(adc_index));
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_FILTER_CTRL |= (VAD_PC_ADC_SDM_DATA_ADC_3_SEL(adc_index));
		break;
	default:
		break;
	}
}


/**
  * @brief  set the codec index and its related DMIC data source
  * @param  codec_index
  *         This parameter can be one of the following values:
  *            @arg VAD_CODEC_ZERO: use codec0
  *            @arg VAD_CODEC_TWO: use codec1
  *            @arg VAD_CODEC_THREE: use codec2
  *            @arg VAD_CODEC_FOUR: use codec3
  * @param  dmic_index
  *         This parameter can be one of the following values:
  *            @arg VAD_DMIC_ZERO: use DMIC0
  *            @arg VAD_DMIC_ZERO: use DMIC1
  *            @arg VAD_DMIC_TWO: use DMIC2
  *            @arg VAD_DMIC_THREE: use DMIC3
  *            @arg VAD_DMIC_FOUR: use DMIC4
  *            @arg VAD_DMIC_FIVE: use DMIC5
  *            @arg VAD_DMIC_SIX: use DMIC6
  *            @arg VAD_DMIC_SEVEN: use DMIC7
  */
void VAD_DMIC_Select(u32 codec_index, u32 dmic_index)
{

	assert_param(codec_index <= VAD_CODEC_THREE);

	assert_param(dmic_index <= VAD_DMIC_SEVEN);

	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	switch (codec_index) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_FILTER_CTRL |= (VAD_PC_DMIC_DATA_ADC_0_SEL(dmic_index));
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_FILTER_CTRL |= (VAD_PC_DMIC_DATA_ADC_1_SEL(dmic_index));
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_FILTER_CTRL |= (VAD_PC_DMIC_DATA_ADC_2_SEL(dmic_index));
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_FILTER_CTRL |= (VAD_PC_DMIC_DATA_ADC_3_SEL(dmic_index));
		break;
	default:
		break;
	}
}

/**
  * @brief  set the source for voice activity detection
  * @param  codec_index
  *         This parameter can be one of the following values:
  *            @arg VAD_CODEC_ZERO: use codec0
  *            @arg VAD_CODEC_TWO: use codec1
  *            @arg VAD_CODEC_THREE: use codec2
  *            @arg VAD_CODEC_FOUR: use codec3
  */
void VAD_Source_Select(u32 codec_index)
{
	assert_param(codec_index <= VAD_CODEC_THREE);

	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	VAD -> VAD_SEL_CTRL &= (~VAD_MASK_PC_VAD_IN_SEL);
	VAD -> VAD_SEL_CTRL |= VAD_PC_VAD_IN_SEL(codec_index);
}



/**
  * @brief  set the codec index and its related DMIC data source
  * @param  codec_index
  *         This parameter can be one of the following values:
  *            @arg VAD_CODEC_ZERO: use codec0
  *            @arg VAD_CODEC_TWO: use codec1
  *            @arg VAD_CODEC_THREE: use codec2
  *            @arg VAD_CODEC_FOUR: use codec3
  * @param  cut_fre_select
  *         This parameter can be one of the following values:
  *            @arg VAD_HPF_ZERO: cut frequency 0.3Hz
  *            @arg VAD_HPF_ONE: cut frequency 0.6Hz
  *            @arg VAD_HPF_TWO: cut frequency 1.2Hz
  *            @arg VAD_HPF_THREE: cut frequency 2.5Hz
  *            @arg VAD_HPF_FOUR: cut frequency 5Hz
  *            @arg VAD_HPF_FIVE: cut frequency 10Hz
  *            @arg VAD_HPF_SIX: cut frequency 20Hz
  *            @arg VAD_HPF_SEVEN: cut frequency 40Hz
  */
void VAD_HPF_Init(u32 codec_index, u32 cut_fre_select)
{
	assert_param(codec_index <= VAD_CODEC_THREE);
	assert_param(cut_fre_select <= VAD_HPF_SEVEN);

	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	u32 reg_val = VAD -> VAD_CODEC_CTRLx[codec_index];

	reg_val &= (~VAD_MASK_PC_ADC_x_AD_HPF_COEF);
	reg_val |= VAD_PC_ADC_x_AD_HPF_COEF(cut_fre_select);
	reg_val |= VAD_BIT_PC_ADC_x_AD_DCHPF_EN;

	VAD -> VAD_CODEC_CTRLx[codec_index] = reg_val;
}

/**
  * @brief  enable interrupt, set interrupt routine and priority
  * @param  vad_interrupt_source
  *         This parameter can be one of the following values:
  *            @arg VAD_INTERRUPT_KM0: enable interrupt for KM0
  *            @arg VAD_INTERRUPT_KM4: enable interrupt for KM4
  *            @arg VAD_INTERRUPT_CA7: enable interrupt for CA7
  * @param  CallbackFunc
  *         This parameter should be a function written by the user
  * @param  IrqPriority
  *         This parameter is the priority of this interrupt
  */
void VAD_Interrupt_set(u32 vad_interrupt_source, IRQ_FUN CallbackFunc, u32 IrqPriority)
{
	assert_param(vad_interrupt_source <= VAD_INTERRUPT_KM0);

	switch (vad_interrupt_source) {
	case VAD_INTERRUPT_KM0:
		printf("CPU: KM0\n");
		break;
	case VAD_INTERRUPT_KM4:
		printf("CPU: KM4\n");
		break;
	case VAD_INTERRUPT_CA7:
		printf("CPU: CA7\n");
		break;
	default:
		break;
	}

	InterruptRegister(CallbackFunc, VADBT_OR_VADPC_IRQ, NULL, IrqPriority);
	InterruptEn(VADBT_OR_VADPC_IRQ, IrqPriority);
}


/**
  * @brief  Configure VAD buf operation(one block)
  * @param  codec_index
  *         This parameter can be one of the following values:
  *            @arg VAD_CODEC_ZERO: use codec0
  *            @arg VAD_CODEC_TWO: use codec1
  *            @arg VAD_CODEC_THREE: use codec2
  *            @arg VAD_CODEC_FOUR: use codec3
  * @param  block_index
  *         This parameter can be one of the following values:
  *            @arg VAD_BLOCK_A: use ram block A
  *            @arg VAD_BLOCK_B: use ram block B
  *            @arg VAD_BLOCK_C: use ram block C
  *            @arg VAD_BLOCK_D: use ram block D
  * @param  transfer_mode
  *         This parameter can be one of the follow values:
  *            @arg VAD_BLOCK_MODE: use block mode
  *            @arg VAD_INTERLEAVE_MODE: use interleave mode
  */
void VAD_Buf_move_one(u32 codec_index, u32 block_index, u32 transfer_mode)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	VAD -> VAD_BUF_CTRL0 |= VAD_BIT_BUF_SHARE_SRAM_ENABLE;

	/*enable sram blocks as needed. e.g., if block_num = 0(A), only 32k ram is needed. if block_num = 1(B), 64k ram is needed, and so on.*/
	VAD -> VAD_BUF_CTRL0 |= VAD_W_SRAM_ADDRESS_SEL(block_index);

	/*sram lock*/
	VAD -> VAD_BUF_CTRL0 |= VAD_BIT_BUF_LOCK;

	if (transfer_mode == VAD_INTERLEAVE_MODE) {
		VAD -> VAD_BUF_CTRL0 |= VAD_BIT_W_MODE_SEL;
	} else {
		VAD -> VAD_BUF_CTRL0 &= (~VAD_BIT_W_MODE_SEL);
	}

	/*disable all channels*/
	VAD -> VAD_BUF_CTRL0 &= (~(VAD_BIT_CH0_IN_VADBUF_EN | VAD_BIT_CH1_IN_VADBUF_EN | VAD_BIT_CH2_IN_VADBUF_EN | VAD_BIT_CH3_IN_VADBUF_EN));

	switch (codec_index) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index);
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index);
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index);
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index);
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

}

/**
  * @brief  Configure VAD buf operation(two block)
  * @param  codec_index_x
  *         This parameter can be one of the following values:
  *            @arg VAD_CODEC_ZERO: use codec0
  *            @arg VAD_CODEC_TWO: use codec1
  *            @arg VAD_CODEC_THREE: use codec2
  *            @arg VAD_CODEC_FOUR: use codec3
  * @param  block_index
  *         This parameter can be one of the following values:
  *            @arg VAD_BLOCK_AB: use ram block A and block B
  *            @arg VAD_BLOCK_BC: use ram block B and block C
  *            @arg VAD_BLOCK_CD: use ram block C and block D
  * @param  transfer_mode
  *         This parameter can be one of the follow values:
  *            @arg VAD_BLOCK_MODE: use block mode
  *            @arg VAD_INTERLEAVE_MODE: use interleave mode
  */
void VAD_Buf_move_two(u32 codec_index_0, u32 codec_index_1, u32 block_index, u32 transfer_mode)
{
	assert_param(codec_index_0 != codec_index_1);

	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	VAD -> VAD_BUF_CTRL0 |= VAD_BIT_BUF_SHARE_SRAM_ENABLE;

	switch (block_index) {
	case VAD_BLOCK_AB:
		VAD -> VAD_BUF_CTRL0 |= VAD_W_SRAM_ADDRESS_SEL(1);
		break;
	case VAD_BLOCK_BC:
		VAD -> VAD_BUF_CTRL0 |= VAD_W_SRAM_ADDRESS_SEL(2);
		break;
	case VAD_BLOCK_CD:
		VAD -> VAD_BUF_CTRL0 |= VAD_W_SRAM_ADDRESS_SEL(3);
		break;
	default:
		break;
	}

	/*sram lock*/
	VAD -> VAD_BUF_CTRL0 |= VAD_BIT_BUF_LOCK;

	if (transfer_mode == VAD_INTERLEAVE_MODE) {
		VAD -> VAD_BUF_CTRL0 |= VAD_BIT_W_MODE_SEL;
	} else {
		VAD -> VAD_BUF_CTRL0 &= (~VAD_BIT_W_MODE_SEL);
	}

	/*disable all channels*/
	VAD -> VAD_BUF_CTRL0 &= (~(VAD_BIT_CH0_IN_VADBUF_EN | VAD_BIT_CH1_IN_VADBUF_EN | VAD_BIT_CH2_IN_VADBUF_EN | VAD_BIT_CH3_IN_VADBUF_EN));

	switch (codec_index_0) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index);
		break;
	default:
		break;
	}

	switch (codec_index_1) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	default:
		break;
	}

	switch (codec_index_0) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

	switch (codec_index_1) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}
}


/**
  * @brief  Configure VAD buf operation(three block)
  * @param  codec_index_x
  *         This parameter can be one of the following values:
  *            @arg VAD_CODEC_ZERO: use codec0
  *            @arg VAD_CODEC_TWO: use codec1
  *            @arg VAD_CODEC_THREE: use codec2
  *            @arg VAD_CODEC_FOUR: use codec3
  * @param  block_index
  *         This parameter can be one of the following values:
  *            @arg VAD_BLOCK_ABC: use ram block A, block B and block C
  *            @arg VAD_BLOCK_BCD: use ram block B, block C and block D
  * @param  transfer_mode
  *         This parameter can be one of the follow values:
  *            @arg VAD_BLOCK_MODE: use block mode
  *            @arg VAD_INTERLEAVE_MODE: use interleave mode
  */
void VAD_Buf_move_three(u32 codec_index_0, u32 codec_index_1, u32 codec_index_2, u32 block_index, u32 transfer_mode)
{
	assert_param(codec_index_0 != codec_index_1);
	assert_param(codec_index_0 != codec_index_2);
	assert_param(codec_index_1 != codec_index_2);

	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	VAD -> VAD_BUF_CTRL0 |= VAD_BIT_BUF_SHARE_SRAM_ENABLE;

	switch (block_index) {
	case VAD_BLOCK_ABC:
		VAD -> VAD_BUF_CTRL0 |= VAD_W_SRAM_ADDRESS_SEL(2);
		break;
	case VAD_BLOCK_BCD:
		VAD -> VAD_BUF_CTRL0 |= VAD_W_SRAM_ADDRESS_SEL(3);
		break;
	default:
		break;
	}

	/*sram lock*/
	VAD -> VAD_BUF_CTRL0 |= VAD_BIT_BUF_LOCK;

	if (transfer_mode == VAD_INTERLEAVE_MODE) {
		VAD -> VAD_BUF_CTRL0 |= VAD_BIT_W_MODE_SEL;
	} else {
		VAD -> VAD_BUF_CTRL0 &= (~VAD_BIT_W_MODE_SEL);
	}

	/*disable all channels*/
	VAD -> VAD_BUF_CTRL0 &= (~(VAD_BIT_CH0_IN_VADBUF_EN | VAD_BIT_CH1_IN_VADBUF_EN | VAD_BIT_CH2_IN_VADBUF_EN | VAD_BIT_CH3_IN_VADBUF_EN));

	switch (codec_index_0) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index);
		break;
	default:
		break;
	}

	switch (codec_index_1) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	default:
		break;
	}

	switch (codec_index_2) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	default:
		break;
	}


	switch (codec_index_0) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

	switch (codec_index_1) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

	switch (codec_index_2) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}
}


/**
  * @brief  Configure VAD buf operation(four block)
  * @param  codec_index
  *         This parameter can be one of the following values:
  *            @arg VAD_CODEC_ZERO: use codec0
  *            @arg VAD_CODEC_TWO: use codec1
  *            @arg VAD_CODEC_THREE: use codec2
  *            @arg VAD_CODEC_FOUR: use codec3
  * @param  block_index
  *         This parameter can be one of the following values:
  *            @arg VAD_BLOCK_ABCD: use ram block A, block B, block C and block D
  * @param  transfer_mode
  *         This parameter can be one of the follow values:
  *            @arg VAD_BLOCK_MODE: use block mode
  *            @arg VAD_INTERLEAVE_MODE: use interleave mode
  */
void VAD_Buf_move_four(u32 codec_index_0, u32 codec_index_1, u32 codec_index_2, u32 codec_index_3, u32 block_index, u32 transfer_mode)
{
	assert_param(codec_index_0 != codec_index_1);
	assert_param(codec_index_0 != codec_index_2);
	assert_param(codec_index_1 != codec_index_2);
	assert_param(codec_index_1 != codec_index_2);
	assert_param(codec_index_2 != codec_index_3);

	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}


	VAD -> VAD_BUF_CTRL0 |= VAD_BIT_BUF_SHARE_SRAM_ENABLE;

	VAD -> VAD_BUF_CTRL0 |= VAD_W_SRAM_ADDRESS_SEL(3);

	/*sram lock*/
	VAD -> VAD_BUF_CTRL0 |= VAD_BIT_BUF_LOCK;

	if (transfer_mode == VAD_INTERLEAVE_MODE) {
		VAD -> VAD_BUF_CTRL0 |= VAD_BIT_W_MODE_SEL;
	} else {
		VAD -> VAD_BUF_CTRL0 &= (~VAD_BIT_W_MODE_SEL);
	}

	/*disable all channels*/
	VAD -> VAD_BUF_CTRL0 &= (~(VAD_BIT_CH0_IN_VADBUF_EN | VAD_BIT_CH1_IN_VADBUF_EN | VAD_BIT_CH2_IN_VADBUF_EN | VAD_BIT_CH3_IN_VADBUF_EN));

	switch (codec_index_0) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index);
		break;
	default:
		break;
	}

	switch (codec_index_1) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	default:
		break;
	}

	switch (codec_index_2) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	default:
		break;
	}

	switch (codec_index_3) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index + 3);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index + 3);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index + 3);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD -> VAD_BUF_CTRL0 |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index + 3);
		break;
	default:
		break;
	}

	switch (codec_index_0) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

	switch (codec_index_1) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

	switch (codec_index_2) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

	switch (codec_index_3) {
	case VAD_CODEC_ZERO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD -> VAD_BUF_CTRL0 |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}
}

/**
  * @brief  Choose the codec to do voice activity detection
  * @param  codec_index
  *         This parameter can be one of the following values:
  *            @arg VAD_CODEC_ZERO: use codec0
  *            @arg VAD_CODEC_TWO: use codec1
  *            @arg VAD_CODEC_THREE: use codec2
  *            @arg VAD_CODEC_FOUR: use codec3
  */
void VAD_Codec_Select(u8 codec_index)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}
	VAD -> VAD_SEL_CTRL &= (~VAD_MASK_PC_VAD_IN_SEL);
	VAD -> VAD_SEL_CTRL |= VAD_PC_VAD_IN_SEL(codec_index);
}

/**
  * @brief  Enable VAD
  */
void VAD_Start(void)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	/*enable VAD*/
	VAD -> VAD_CLK_CTRL |= VAD_BIT_PC_TCON_VAD_EN;
}

/**
  * @brief  use "pitch" as the criterion to detect voice activity
  * @param  type
  *         This parameter can be one of the following values:
  *            @arg VAD_HOLD: the flag will keep 1 once voice activity is detected
  *            @arg VAD_INSTANT: the flag will keeps changing with the voice activity
  */
void VAD_Pitch_Set(u32 type)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	if (type == VAD_INSTANT) {
		VAD -> VAD_SEL_CTRL &= (~VAD_MASK_PC_SEL_VAD_FLAG);
		VAD -> VAD_SEL_CTRL |= VAD_PC_SEL_VAD_FLAG(1);
	} else {
		VAD -> VAD_SEL_CTRL &= (~VAD_MASK_PC_SEL_VAD_FLAG);
		VAD -> VAD_SEL_CTRL |= VAD_PC_SEL_VAD_FLAG(0);
	}
}


/**
  * @brief  use "energy" as the criterion to detect voice activity
  * @param  type
  *         This parameter can be one of the following values:
  *            @arg VAD_HOLD: the flag will keep 1 once voice activity is detected
  *            @arg VAD_INSTANT: the flag will keeps changing with the voice activity
  */
void VAD_Energy_Set(u32 type)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	if (type == VAD_INSTANT) {
		VAD -> VAD_SEL_CTRL &= (~VAD_MASK_PC_SEL_VAD_FLAG);
		VAD -> VAD_SEL_CTRL |= VAD_PC_SEL_VAD_FLAG(1);
	} else {
		VAD -> VAD_SEL_CTRL &= (~VAD_MASK_PC_SEL_VAD_FLAG);
		VAD -> VAD_SEL_CTRL |= VAD_PC_SEL_VAD_FLAG(0);
	}
	VAD -> VAD_PITCH_DET_CTRL2 |= VAD_BIT_PITCH_DET_ENERGY_MODE_MANUAL_EN;
}

/**
  * @brief  use "abs value" as the criterion to detect voice activity
  * @param  type
  *         This parameter can be one of the following values:
  *            @arg VAD_HOLD: the flag will keep 1 once voice activity is detected
  *            @arg VAD_INSTANT: the flag will keeps changing with the voice activity
  */
void VAD_Abs_Set(u32 type)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	if (type == VAD_INSTANT) {
		VAD -> VAD_SEL_CTRL &= (~VAD_MASK_PC_SEL_VAD_FLAG);
		VAD -> VAD_SEL_CTRL |= VAD_PC_SEL_VAD_FLAG(4);
	} else {
		VAD -> VAD_SEL_CTRL &= (~VAD_MASK_PC_SEL_VAD_FLAG);
		VAD -> VAD_SEL_CTRL |= VAD_PC_SEL_VAD_FLAG(5);
	}
}

/**
  * @brief  set the threshold of absolute energy
  * @param  type
  *         This parameter should be set between 0 and 8191
  */
void VAD_Abs_Thre(u16 thre)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	VAD -> VAD_PITCH_DET_CTRL6 &= (~VAD_MASK_PITCH_DET_ST_ABS_THR);
	VAD -> VAD_PITCH_DET_CTRL6 |= (thre);
}

/**
  * @brief  set the threshold value in energy mode
  * @param  thr:  the accumulated time when thr keeps checking check time= REG * 4ms
  *         This parameter should be set between 0 and 2047
  */
void VAD_Energy_keep_Thre(u16 thr)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	VAD -> VAD_PITCH_DET_CTRL2 &= (~VAD_MASK_PITCH_DET_ENERGY_KEEP_THR);
	VAD -> VAD_PITCH_DET_CTRL2 |= (VAD_PITCH_DET_ENERGY_KEEP_THR(thr));

}

/**
  * @brief  set the threshold and higher threshole for peak search
  * @param  thr1
  *         This parameter should be set between 0 and 1023
  * @param  thr2
  *         This parameter should be set between 0 and 1023, and it should be larger than thr1
  */
void VAD_Energy_Search_Thre(u16 thr1, u16 thr2)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	VAD -> VAD_PITCH_DET_CTRL4 &= (~VAD_MASK_PITCH_DET_PEAK_SEARCH_THR);
	VAD -> VAD_PITCH_DET_CTRL4 |= (VAD_PITCH_DET_PEAK_SEARCH_THR(thr1));

	VAD -> VAD_PITCH_DET_CTRL4 &= (~VAD_MASK_PITCH_DET_PEAK_SEARCH_THR2);
	VAD -> VAD_PITCH_DET_CTRL4 |= (VAD_PITCH_DET_PEAK_SEARCH_THR2(thr2));

}

/**
  * @brief  set the default threshold of onset detection
  * @param  thr
  *         This parameter should be set between 0 and 31
  */
void VAD_Det_OD_Default_Thre(u16 thr)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	VAD -> VAD_PITCH_DET_CTRL3 &= (~VAD_MASK_PITCH_DET_OD_THR_DEFAULT);
	VAD -> VAD_PITCH_DET_CTRL3 |= (VAD_PITCH_DET_OD_THR_DEFAULT(thr));
}

/**
  * @brief  set the threshold of majority vote to determine whether the speech features appear range
  * @param  thr
  *         This parameter should be set between 0 and 31
  */
void VAD_Det_MV_Thre(u16 thr)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	VAD -> VAD_PITCH_DET_CTRL2 &= (~VAD_MASK_PITCH_DET_MV_THR);
	VAD -> VAD_PITCH_DET_CTRL2 |= (VAD_PITCH_DET_MV_THR(thr));
}

/**
  * @brief  set the threshold for checking the potential energy variation in voice
  * @param  thr
  *         This parameter should be set between 0 and 7
  */
void VAD_Det_Energy_CHK_Thre(u16 thr)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	VAD -> VAD_PITCH_DET_CTRL2 &= (~VAD_MASK_PITCH_DET_ENERGY_CHK_THR);
	VAD -> VAD_PITCH_DET_CTRL2 |= (VAD_PITCH_DET_ENERGY_CHK_THR(thr));

}

/**
  * @brief  set the energy mode threshold of onset detection
  * @param  thr
  *         This parameter should be set between 0 and 31
  */
void VAD_Det_OD_Energy_Thre(u16 thr)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	VAD -> VAD_PITCH_DET_CTRL3 &= (~VAD_MASK_PITCH_DET_OD_THR_ENERGY);
	VAD -> VAD_PITCH_DET_CTRL3 |= (VAD_PITCH_DET_OD_THR_ENERGY(thr));

}

/**
  * @brief  set the threshold to determine the validation of short_term energy range
  * @param  thr
  *         This parameter should be set between 0 and 8191
  */
void VAD_Pitch_Det_Thr_ST_Valid(u16 thr)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	VAD -> VAD_PITCH_DET_CTRL6 &= (~VAD_MASK_PITCH_DET_THR_ST_VALID);
	VAD -> VAD_PITCH_DET_CTRL6 |= (VAD_PITCH_DET_THR_ST_VALID(thr));
}


