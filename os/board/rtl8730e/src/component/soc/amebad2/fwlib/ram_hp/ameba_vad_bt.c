/**
  ******************************************************************************
  * @file    ameba_vad_bt.c
  * @author  Xiaojuan_Li
  * @version V1.0.0
  * @date    2022-07-22
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
  * @brief  Enable VAD
  */
void BT_VAD_Start(void)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_VAD_CTRL);
	Tmp &= ~AUD_BIT_VAD_RESET;
	HAL_WRITE32(audio_base, CODEC_VAD_CTRL, Tmp);
}

/**
  * @brief  Set VAD 24bit
  */
void BT_VAD_SetBit()
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)(VAD_REG_BASE_S));
	} else {
		VAD = ((VAD_TypeDef *)(VAD_REG_BASE));
	}

	VAD->VAD_BUF_CTRL0 |= VAD_BIT_BIT_SEL;
}

/**
  * @brief  Enable VAD INITERRUPT
  */
void BT_VAD_ENIRQ(u32 mode)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_VAD_INTR_EN);
	Tmp &= ~AUD_MASK_VAD_INTR_EN;
	if (mode == VAD_INTERRUPT_RISING) {
		Tmp |= AUD_VAD_INTR_EN(0x01);
	} else {
		Tmp |= AUD_VAD_INTR_EN(0x10);
	}

	HAL_WRITE32(audio_base, CODEC_VAD_INTR_EN, Tmp);
}

/**
  * @brief  Disable VAD INITERRUPT
  */
void BT_VAD_DISIRQ(u32 mode)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_VAD_INTR_ST);
	Tmp &= ~AUD_MASK_VAD_INTR_ST;
	if (mode == VAD_INTERRUPT_RISING) {
		Tmp |= AUD_VAD_INTR_ST(0x01);
	} else {
		Tmp |= AUD_VAD_INTR_ST(0x10);
	}

	HAL_WRITE32(audio_base, CODEC_VAD_INTR_ST, Tmp);
}

/**
  * @brief  Set VAD threshold
  * @param  threshold
  *         This parameter can be one of the following recommended values:
  *            @arg 0x19: more sensitive to voice
  *            @arg 0x1E
  *            @arg 0x28
  *            @arg 0x3C: less sensitive to voice
  */
void BT_VAD_SetDetThr(u32 threshold)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_VAD_DET_THR);
	Tmp &= ~AUD_MASK_VAD_DET_THR;
	Tmp |= AUD_VAD_DET_THR(threshold);
	HAL_WRITE32(audio_base, CODEC_VAD_DET_THR, Tmp);

}

/**
  * @brief  Set VAD voice level
  * @param  voice_level
  *         This parameter can be one of the following recommended values:
  *            @arg 0x12C: signal level:-10 ~ -20dBFS
  *            @arg 0xFA: signal level:-10 ~ -30dBFS
  *            @arg 0xC8: signal level:-10 ~ -40BFS
  *            @arg 0x96: signal level:-10 ~ -50dBFS
  */
void BT_VAD_SetVoice(u32 voice_level)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_VAD_VOICE_LEVEL);
	Tmp &= ~AUD_MASK_VAD_VOICE_LEVEL;
	Tmp |= AUD_VAD_VOICE_LEVEL(voice_level);
	HAL_WRITE32(audio_base, CODEC_VAD_VOICE_LEVEL, Tmp);
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
void BT_VAD_SelSrc(u32 codec_index)
{
	u32 Tmp;
	u32 audio_base;

	if (TrustZone_IsSecure()) {
		audio_base = AUDIO_REG_BASE_S;
	} else {
		audio_base = AUDIO_REG_BASE;
	}

	Tmp = HAL_READ32(audio_base, CODEC_VAD_CTRL);
	Tmp &= ~AUD_MASK_VAD_EQ_SRC_SEL;
	Tmp |= AUD_VAD_EQ_SRC_SEL(codec_index);
	HAL_WRITE32(audio_base, CODEC_VAD_CTRL, Tmp);
}

/**
  * @brief  Configure VAD buf operation(single block)
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

void BT_VAD_Buf_move_one(u32 codec_index, u32 block_index, u32 transfer_mode)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)(VAD_REG_BASE_S));
	} else {
		VAD = ((VAD_TypeDef *)(VAD_REG_BASE));
	}

	VAD->VAD_BUF_CTRL0 |= VAD_BIT_BUF_SHARE_SRAM_ENABLE;

	/*enable sram blocks as needed. e.g., if block_num = 0(A), only 32k ram is needed. if block_num = 1(B), 64k ram is needed, and so on.*/
	VAD->VAD_BUF_CTRL0  &= ~VAD_MASK_W_SRAM_ADDRESS_SEL;
//	VAD ->VAD_BUF_CTRL0 |= VAD_W_SRAM_ADDRESS_SEL(block_index);

	/* for VAD_BT*/
	VAD ->VAD_BUF_CTRL0 |= VAD_BIT_BUF_CTRL_INPUT_SEL;
	VAD ->VAD_BUF_CTRL0 &= (~VAD_BIT_ADC_ANALOG_CLK_SEL);


	switch (block_index) {
	case VAD_BLOCK_B:
		VAD->VAD_BUF_CTRL0  |= VAD_W_SRAM_ADDRESS_SEL(1);
		break;
	case VAD_BLOCK_C:
		VAD->VAD_BUF_CTRL0  |= VAD_W_SRAM_ADDRESS_SEL(2);
		break;
	case VAD_BLOCK_D:
		VAD->VAD_BUF_CTRL0  |= VAD_W_SRAM_ADDRESS_SEL(3);
		break;
	default:
		break;
	}

	/*sram lock*/
	VAD->VAD_BUF_CTRL0 |= VAD_BIT_BUF_LOCK;

	if (transfer_mode == VAD_INTERLEAVE_MODE) {
		VAD->VAD_BUF_CTRL0  |= VAD_BIT_W_MODE_SEL;
	} else {
		VAD->VAD_BUF_CTRL0  &= (~VAD_BIT_W_MODE_SEL);
	}

	/*disable all channels*/
	VAD->VAD_BUF_CTRL0  &= (~(VAD_BIT_CH0_IN_VADBUF_EN | VAD_BIT_CH1_IN_VADBUF_EN | VAD_BIT_CH2_IN_VADBUF_EN | VAD_BIT_CH3_IN_VADBUF_EN));

	switch (codec_index) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index);
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index);
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index);
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index);
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

}

/**
  * @brief  Configure VAD buf operation(double block)
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
void BT_VAD_Buf_move_two(u32 codec_index_0, u32 codec_index_1, u32 block_index, u32 transfer_mode)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)(VAD_REG_BASE_S));
	} else {
		VAD = ((VAD_TypeDef *)(VAD_REG_BASE));
	}

	assert_param(codec_index_0 != codec_index_1);

	VAD->VAD_BUF_CTRL0  |= VAD_BIT_BUF_SHARE_SRAM_ENABLE;

	/*enable sram blocks as needed. e.g., if block_num = 0(A), only 32k ram is needed. if block_num = 1(B), 64k ram is needed, and so on.*/
	VAD->VAD_BUF_CTRL0  &= ~VAD_MASK_W_SRAM_ADDRESS_SEL;

	/* for VAD_BT*/
	VAD ->VAD_BUF_CTRL0 |= VAD_BIT_BUF_CTRL_INPUT_SEL;
	VAD ->VAD_BUF_CTRL0 &= (~VAD_BIT_ADC_ANALOG_CLK_SEL);

	switch (block_index) {
	case VAD_BLOCK_AB:
		VAD->VAD_BUF_CTRL0  |= VAD_W_SRAM_ADDRESS_SEL(1);
		break;
	case VAD_BLOCK_BC:
		VAD->VAD_BUF_CTRL0  |= VAD_W_SRAM_ADDRESS_SEL(2);
		break;
	case VAD_BLOCK_CD:
		VAD->VAD_BUF_CTRL0  |= VAD_W_SRAM_ADDRESS_SEL(3);
		break;
	default:
		break;
	}

	/*sram lock*/
	VAD->VAD_BUF_CTRL0  |= VAD_BIT_BUF_LOCK;


	if (transfer_mode == VAD_INTERLEAVE_MODE) {
		VAD->VAD_BUF_CTRL0  |= VAD_BIT_W_MODE_SEL;
	} else {
		VAD->VAD_BUF_CTRL0  &= (~VAD_BIT_W_MODE_SEL);
	}

	/*disable all channels*/
	VAD->VAD_BUF_CTRL0  &= (~(VAD_BIT_CH0_IN_VADBUF_EN | VAD_BIT_CH1_IN_VADBUF_EN | VAD_BIT_CH2_IN_VADBUF_EN | VAD_BIT_CH3_IN_VADBUF_EN));

	switch (codec_index_0) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index);
		break;
	default:
		break;
	}

	switch (codec_index_1) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	default:
		break;
	}

	switch (codec_index_0) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

	switch (codec_index_1) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}
}


/**
  * @brief  Configure VAD buf operation(double block)
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
void BT_VAD_Buf_move_three(u32 codec_index_0, u32 codec_index_1, u32 codec_index_2, u32 block_index, u32 transfer_mode)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)(VAD_REG_BASE_S));
	} else {
		VAD = ((VAD_TypeDef *)(VAD_REG_BASE));
	}

	assert_param(codec_index_0 != codec_index_1);
	assert_param(codec_index_0 != codec_index_2);
	assert_param(codec_index_1 != codec_index_2);

	VAD->VAD_BUF_CTRL0  |= VAD_BIT_BUF_SHARE_SRAM_ENABLE;

	/*enable sram blocks as needed. e.g., if block_num = 0(A), only 32k ram is needed. if block_num = 1(B), 64k ram is needed, and so on.*/
	VAD->VAD_BUF_CTRL0  &= ~VAD_MASK_W_SRAM_ADDRESS_SEL;

	/* for VAD_BT*/
	VAD ->VAD_BUF_CTRL0 |= VAD_BIT_BUF_CTRL_INPUT_SEL;
	VAD ->VAD_BUF_CTRL0 &= (~VAD_BIT_ADC_ANALOG_CLK_SEL);

	switch (block_index) {
	case VAD_BLOCK_ABC:
		VAD->VAD_BUF_CTRL0  |= VAD_W_SRAM_ADDRESS_SEL(2);
		break;
	case VAD_BLOCK_BCD:
		VAD->VAD_BUF_CTRL0  |= VAD_W_SRAM_ADDRESS_SEL(3);
		break;
	default:
		break;
	}

	/*sram lock*/
	VAD->VAD_BUF_CTRL0  |= VAD_BIT_BUF_LOCK;

	if (transfer_mode == VAD_INTERLEAVE_MODE) {
		VAD->VAD_BUF_CTRL0  |= VAD_BIT_W_MODE_SEL;
	} else {
		VAD->VAD_BUF_CTRL0  &= (~VAD_BIT_W_MODE_SEL);
	}

	/*disable all channels*/
	VAD->VAD_BUF_CTRL0  &= (~(VAD_BIT_CH0_IN_VADBUF_EN | VAD_BIT_CH1_IN_VADBUF_EN | VAD_BIT_CH2_IN_VADBUF_EN | VAD_BIT_CH3_IN_VADBUF_EN));

	switch (codec_index_0) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index);
		break;
	default:
		break;
	}

	switch (codec_index_1) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	default:
		break;
	}

	switch (codec_index_2) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	default:
		break;
	}


	switch (codec_index_0) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

	switch (codec_index_1) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

	switch (codec_index_2) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}
}


/**
  * @brief  Configure VAD buf operation(triple block)
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
void BT_VAD_Buf_move_four(u32 codec_index_0, u32 codec_index_1, u32 codec_index_2, u32 codec_index_3, u32 block_index, u32 transfer_mode)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	assert_param(codec_index_0 != codec_index_1);
	assert_param(codec_index_0 != codec_index_2);
	assert_param(codec_index_0 != codec_index_3);
	assert_param(codec_index_1 != codec_index_2);
	assert_param(codec_index_1 != codec_index_3);
	assert_param(codec_index_2 != codec_index_3);

	VAD->VAD_BUF_CTRL0  |= VAD_BIT_BUF_SHARE_SRAM_ENABLE;
	VAD->VAD_BUF_CTRL0  |= VAD_W_SRAM_ADDRESS_SEL(3);

	/* for VAD_BT*/
	VAD->VAD_BUF_CTRL0 |= VAD_BIT_BUF_CTRL_INPUT_SEL;
	VAD ->VAD_BUF_CTRL0 &= (~VAD_BIT_ADC_ANALOG_CLK_SEL);

	/*sram lock*/
	VAD->VAD_BUF_CTRL0  |= VAD_BIT_BUF_LOCK;

	if (transfer_mode == VAD_INTERLEAVE_MODE) {
		VAD->VAD_BUF_CTRL0  |= VAD_BIT_W_MODE_SEL;
	} else {
		VAD->VAD_BUF_CTRL0  &= (~VAD_BIT_W_MODE_SEL);
	}


	/*disable all channels*/
	VAD->VAD_BUF_CTRL0  &= (~(VAD_BIT_CH0_IN_VADBUF_EN | VAD_BIT_CH1_IN_VADBUF_EN | VAD_BIT_CH2_IN_VADBUF_EN | VAD_BIT_CH3_IN_VADBUF_EN));

	switch (codec_index_0) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index);
		break;
	default:
		break;
	}

	switch (codec_index_1) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index + 1);
		break;
	default:
		break;
	}

	switch (codec_index_2) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index + 2);
		break;
	default:
		break;
	}

	switch (codec_index_3) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC0_IN_BUF_ADD_BLOCK(block_index + 3);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC1_IN_BUF_ADD_BLOCK(block_index + 3);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC2_IN_BUF_ADD_BLOCK(block_index + 3);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  &= (~(VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK));
		VAD->VAD_BUF_CTRL0  |= VAD_CODEC3_IN_BUF_ADD_BLOCK(block_index + 3);
		break;
	default:
		break;
	}

	switch (codec_index_0) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

	switch (codec_index_1) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

	switch (codec_index_2) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}

	switch (codec_index_3) {
	case VAD_CODEC_ZERO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH0_IN_VADBUF_EN);
		break;
	case VAD_CODEC_ONE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH1_IN_VADBUF_EN);
		break;
	case VAD_CODEC_TWO:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH2_IN_VADBUF_EN);
		break;
	case VAD_CODEC_THREE:
		VAD->VAD_BUF_CTRL0  |= (VAD_BIT_CH3_IN_VADBUF_EN);
		break;
	default:
		break;
	}
}

