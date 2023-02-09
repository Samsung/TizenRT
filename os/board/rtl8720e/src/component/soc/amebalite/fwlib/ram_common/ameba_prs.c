/**
  ******************************************************************************
  * @file    ameba_prs.c
  * @author
  * @version V1.0.0
  * @date    2021-11-25
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Peripheral Reflex Systerm (PRS) peripheral:
  *           - Initialization and Configuration
  *           - Analog configuration
  *           - Mode configuration
  *           - Interrupts and flags management
  *
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

/**
  * @brief  Initializes the parameters in the PRS_InitStruct with default values.
  * @param  PRS_InitStruct: pointer to a PRS_InitTypeDef structure that contains
  *         the configuration information for the PRS.
  * @retval  None
  */
void PRS_StructInit(PRS_InitTypeDef *PRS_InitStruct)
{
	PRS_InitStruct->PRS_ProducerType = PRS_SRC_GPIO;
	PRS_InitStruct->PRS_ConsumerType = PRS_DST_TIM_EN | PRS_DST_PWM_TRIG;
	PRS_InitStruct->PRS_TimerOfIn = ENABLE;
	PRS_InitStruct->PRS_ReverseSig = DISABLE;
	PRS_InitStruct->PRS_DebounceCnt = 0x0;
}

/**
  * @brief  Initializes the PRS according to the specified parameters in PRS_InitStruct.
  * @param  PRS_InitStruct: pointer to a PRS_InitTypeDef structure that contains
  *         the configuration information for the PRS.
  * @retval  None
  */
void PRS_Init(PRS_InitTypeDef *PRS_InitStruct)
{
	PRS_SrcConfig(PRS_InitStruct->PRS_ProducerType);
	PRS_DstConfig(PRS_InitStruct->PRS_ConsumerType);
	PRS_TimOfInCmd(PRS_InitStruct->PRS_TimerOfIn);
	PRS_ReverseCmd(PRS_InitStruct->PRS_ReverseSig);
	PRS_SetDbcCnt(PRS_InitStruct->PRS_DebounceCnt);
}

/**
  * @brief  Specify the source signal type of PRS.
  * @param  SrcType: Specified signal source.
  *   			This parameter can be a value of @ref PRS_Producer_Signal_Type_Definitions.
  * @retval None
  */
void PRS_SrcConfig(u8 SrcType)
{
	assert_param(IS_PRS_PRODUCER_TYPE(SrcType));

	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL);
	Temp &= ~LSYS_MASK_PRS_SRC_SEL;
	Temp |= LSYS_PRS_SRC_SEL(SrcType);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL, Temp);
}

/**
  * @brief  Specify the destination signal type of PRS.
  * @param  DstType: Specified signal destination.
  *   			This parameter can be a value or combinations of @ref PRS_Consumer_Signal_Type_Definitions.
  * @retval None
  */
void PRS_DstConfig(u8 DstType)
{
	assert_param(IS_PRS_CONSUMER_TYPE(DstType));

	if (DstType == PRS_DST_NONE) {
		u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL);
		Temp &= ~(LSYS_BIT_PRS_OUT_PWM_TRIG | LSYS_BIT_PRS_OUT_TIM_EN);
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL, Temp);
		return;
	}

	if (DstType & PRS_DST_TIM_EN) {
		u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL);
		Temp |= LSYS_BIT_PRS_OUT_TIM_EN;
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL, Temp);
	}

	if (DstType & PRS_DST_PWM_TRIG) {
		u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL);
		Temp |= LSYS_BIT_PRS_OUT_PWM_TRIG;
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL, Temp);
	}
}

/**
  * @brief Enable or disable basic timer overflow input.
  * @param NewState: can be ENABLE or DISABLE.
  * @retval None.
  */
void PRS_TimOfInCmd(u32 NewState)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL);

	if (NewState != DISABLE) {
		Temp |= LSYS_BIT_PRS_IN_TIM_OF;
	} else {
		Temp &= ~LSYS_BIT_PRS_IN_TIM_OF;
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL, Temp);
}

/**
  * @brief Enable or disable signal_reverse function in circuit.
  * @param  NewState: can be one of the following value:
  *			@arg ENABLE: Reverse source signal.
  *			@arg DISABLE: Do not reverse source signal.
  * @retval  None.
  */
void PRS_ReverseCmd(u32 NewState)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL);
	Temp &= ~LSYS_BIT_PRS_RVS_EN;

	if (NewState != DISABLE) {
		Temp |= LSYS_BIT_PRS_RVS_EN;
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL, Temp);
}

/**
  * @brief  Set debounce counter under XTAL40M to realize signal deglitch.
  * @param  Dbc_Cnt: Debounce counter to be set.
  *	@note Dbc_Cnt = 0 means debounce function is disabled.
  * @retval None
  */
void PRS_SetDbcCnt(u16 Dbc_Cnt)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL);
	Temp &= ~LSYS_MASK_PRS_DBC_CNT;
	Temp |= LSYS_PRS_DBC_CNT(Dbc_Cnt);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_PRS_CTRL, Temp);
}

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor *****END OF FILE****/
