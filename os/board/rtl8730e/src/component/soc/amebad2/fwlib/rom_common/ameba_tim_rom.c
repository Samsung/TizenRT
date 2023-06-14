/**
  ******************************************************************************
  * @file    rtl8721d_tim_rom.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Timer peripheral:
  *           - Initialization
  *           - TimeBase configuration and management
  *           - Intput Capture/Output Compare configuration and management
  *           - Interrupt management
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
  * @brief  Initializes the parameters in the TIM_InitStruct with its default value
  * @param  TIM_InitStruct: pointer to a RTIM_TimeBaseInitTypeDef structure which will be
  *         initialized.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void RTIM_TimeBaseStructInit(RTIM_TimeBaseInitTypeDef *TIM_InitStruct)
{
	TIM_InitStruct->TIM_Prescaler = 0x00;
	TIM_InitStruct->TIM_Period = 0xFFFF;

	TIM_InitStruct->TIM_UpdateEvent = ENABLE; /* UEV enable */
	TIM_InitStruct->TIM_UpdateSource = TIM_UpdateSource_Overflow;
	TIM_InitStruct->TIM_ARRProtection = ENABLE;

	TIM_InitStruct->TIM_SecureTimer = 0; /* 0: non-secure timer 1: secure timer */
}

/**
  * @brief  Initializes the TIM Time Base Unit peripheral according to
  *         the specified parameters in the TIM_TimeBaseInitStruct.
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @param  TIM_InitStruct: pointer to a TIM_TimeBaseInitTypeDef
  *         structure that contains the configuration information for the
  *         specified TIM peripheral.
  * @param  IrqNum: TIM IRQ num.This parameter can be TIMERx_IRQ
  *         where x can be 0-9 corresponding to TIM
  * @param  UserCB: ISR function handler. The type IRQ_FUN is defined
  *         as follows: typedef u32 (*IRQ_FUN)(void *Data);
  * @param  UserCBData: parameters of ISR function.
  * @retval  none
  */
__weak HAL_ROM_TEXT_SECTION
void RTIM_TimeBaseInit(RTIM_TypeDef *TIM, RTIM_TimeBaseInitTypeDef *TIM_InitStruct, IRQn_Type IrqNum, IRQ_FUN UserCB,
					   u32 UserCBData)
{
	u32 tmpCR = 0;
	u32 Status;

	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));
	assert_param(IS_TIM_UPDATE_SOURCE(TIM_InitStruct->TIM_UpdateSource));
	assert_param(IS_TIM_PSC(TIM_InitStruct->TIM_Prescaler));

	/* Check the secure attribute of basic timer */
	if (TIM_InitStruct->TIM_SecureTimer) {
		/* set this timer to be secure timer */
		TIM->RSVD0 |= TIM_BIT_SEC;
	}

	/* Reset Timer before init */
	/* 1. disable timer CNT */
	/* 2. close and clear all interrupt, or err interrupt will hanppen */
	RTIM_Cmd(TIM, DISABLE);

	TIM->DIER = 0x00;
	RTIM_INTClear(TIM);

	/* Set the Autoreload value */
	if (IS_TIM_40M_TIM(TIM)) {
		TIM->ARR = TIM_CNT(TIM_InitStruct->TIM_Period);
	} else {
		TIM->ARR = TIMBasic_CNT(TIM_InitStruct->TIM_Period);
	}

	/* Set the Prescaler value */
	if (IS_TIM_CCM_TIM(TIM)) {
		TIM->PSC = TIM_InitStruct->TIM_Prescaler;
	} else {
		TIM->PSC = 0;
	}

	/* get CR */
	tmpCR = TIM->CR;

	/* Set the ARR Preload Enable Bit */
	if (TIM_InitStruct->TIM_ARRProtection != DISABLE) {
		tmpCR |= TIM_BIT_ARPE;	//period will update when cnt = 0
	} else {
		tmpCR &= ~TIM_BIT_ARPE; //period will update immediatly
	}

	/* Set the URS Bit */
	if (TIM_InitStruct->TIM_UpdateSource == TIM_UpdateSource_Overflow) {
		tmpCR |= TIM_BIT_URS;
	} else {
		tmpCR &= ~TIM_BIT_URS;
	}

	/* Set the Update Disable Bit */
	if (TIM_InitStruct->TIM_UpdateEvent == DISABLE) {
		tmpCR |= TIM_BIT_UDIS;
	} else {
		tmpCR &= ~(TIM_BIT_UDIS);
	}

	/* set CR */
	TIM->CR = tmpCR;

	/* Initial TimerIRQHandle */
	if (UserCB) {
		InterruptRegister(UserCB, IrqNum, UserCBData, 3);
		InterruptEn(IrqNum, 3);
	}

	/* Generate an update event */
	/* 1) reload the Prescaler immediatly */
	/* 2) reload the ARP immediatly */
	/* 3) hadrware will clear this bit after reload, about 71.936 us (2*32k cycles)  */
	/* 4) UEV will reset counter, and counter will start from 0 */
	/* 5) gen a interrupt if use TIM_UpdateSource_Global */
	TIM->EGR = TIM_PSCReloadMode_Immediate;

	/* poll EGR UG done */
	while (1) {
		if (TIM->SR & TIM_BIT_UG_DONE) {
			break;
		}
	}

	/* Clear all flags: PWM: 0x3F007F, Capture: 0x3, Basic: 0x1 */
	Status = TIM->SR;
	TIM->SR = Status;
}

/**
  * @brief  Enables or disables the specified TIM peripheral.
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @param  NewState: new state of the TIM peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void RTIM_Cmd(RTIM_TypeDef *TIM, u32 NewState)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));

	if (NewState != DISABLE) {
		/* Enable the TIM Counter, dont do this if timer is RUN */
		if ((TIM->EN & TIM_BIT_CEN) == 0) {
			TIM->EN = TIM_BIT_CNT_START;
		}

		/* poll if cnt is running, 3*32k cycles */
		while (1) {
			if (TIM->EN & TIM_BIT_CEN) {
				break;
			}
		}
	} else {
		/* Disable the TIM Counter, dont do this if timer is not RUN */
		/* this action need sync to 32k domain for 100us */
		if (TIM->EN & TIM_BIT_CEN) {
			TIM->EN = TIM_BIT_CNT_STOP;
		}

		/* poll if cnt is running, aout 100us */
		while (1) {
			if ((TIM->EN & TIM_BIT_CEN) == 0) {
				break;
			}
		}
	}
}

/**
  * @brief  DeInit the specified TIM peripheral,
  *	including disable TIM, close and clear interrupt.
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void RTIM_DeInit(RTIM_TypeDef *TIM)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));

	/* disable */
	RTIM_Cmd(TIM, DISABLE);

	/* close and clear all interrupt */
	RTIM_INTClear(TIM);
	TIM->DIER = 0x00;
}

/**
  * @brief  Gets the TIM Counter value.
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @retval Counter Register value
  */
__weak HAL_ROM_TEXT_SECTION
u32 RTIM_GetCount(RTIM_TypeDef *TIM)
{
	u32 TimerCountOld;
	u32 TimerCountNew;
	u32 TimerRDCnt;

	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));

	TimerRDCnt = 0;
	TimerCountOld = TIM->CNT;
	while (1) {
		TimerCountNew = TIM->CNT;

		if (TimerCountOld == TimerCountNew) {
			return (u32)TimerCountOld;
		} else {
			TimerRDCnt++;
			TimerCountOld = TimerCountNew;

			if (TimerRDCnt >= 2) {
				return (u32)TimerCountOld;
			}
		}
	}
}

/**
  * @brief  ENABLE/DISABLE  the TIM's interrupt bits.
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @param  TIM_IT: specifies the interrupt bit to configure.
  *   This parameter can be any combination of the following values:
  *     @arg TIM_IT_Update: TIM update Interrupt source (TIM0-9)
  *     @arg TIM_IT_CC0: TIM Capture Compare 0 Interrupt source (TIM8/TIM9)
  *     @arg TIM_IT_CC1: TIM Capture Compare 1 Interrupt source (TIM8)
  *     @arg TIM_IT_CC2: TIM Capture Compare 2 Interrupt source (TIM8)
  *     @arg TIM_IT_CC3: TIM Capture Compare 3 Interrupt source (TIM8)
  *     @arg TIM_IT_CC4: TIM Capture Compare 4 Interrupt source (TIM8)
  *     @arg TIM_IT_CC5: TIM Capture Compare 5 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE0: TIM Update Event 0 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE1: TIM Update Event 1 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE2: TIM Update Event 2 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE3: TIM Update Event 3 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE4: TIM Update Event 4 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE5: TIM Update Event 5 Interrupt source (TIM8)
  * @param NewState: ENABLE or DISABLE
  * @note
  *   - TIM0-7 can generate only an TIM_IT_Update interrupt.
  *   - TIM9 can generate TIM_IT_Update and TIM_IT_CC0 interrupt.
  *   - TIM8 can generate all interrupts.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void RTIM_INTConfig(RTIM_TypeDef *TIM, u32 TIM_IT, u32 NewState)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));
	assert_param(IS_TIM_IT(TIM_IT));

	if (NewState != DISABLE) {
		/* Enable the Interrupt sources */
		TIM->DIER |= TIM_IT;
	} else {
		/* Disable the Interrupt sources */
		TIM->DIER &= ~TIM_IT;
	}
}

/**
  * @brief  Clears the TIM's all interrupt pending bits.
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void RTIM_INTClear(RTIM_TypeDef *TIM)
{
	u32 CounterIndex = 0;
	u32 Status = TIM->SR;

	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));

	/* Clear the all IT pending Bits */
	TIM->SR = Status;

	/* make sure write ok, because bus delay */
	while (1) {
		CounterIndex++;
		if (CounterIndex >= 300) {
			break;
		}

		if (((TIM->SR) & 0xFFFFFF) == 0) {
			break;
		}
	}
}

/**
  * @brief  Clears the TIM's interrupt pending bits.
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @param  TIM_IT: specifies the pending bit to clear.
  *   This parameter can be any combination of the following values:
  *     @arg TIM_IT_Update: TIM update Interrupt source (TIM0-9)
  *     @arg TIM_IT_CC0: TIM Capture Compare 0 Interrupt source (TIM8/TIM9)
  *     @arg TIM_IT_CC1: TIM Capture Compare 1 Interrupt source (TIM8)
  *     @arg TIM_IT_CC2: TIM Capture Compare 2 Interrupt source (TIM8)
  *     @arg TIM_IT_CC3: TIM Capture Compare 3 Interrupt source (TIM8)
  *     @arg TIM_IT_CC4: TIM Capture Compare 4 Interrupt source (TIM8)
  *     @arg TIM_IT_CC5: TIM Capture Compare 5 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE0: TIM Update Event 0 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE1: TIM Update Event 1 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE2: TIM Update Event 2 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE3: TIM Update Event 3 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE4: TIM Update Event 4 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE5: TIM Update Event 5 Interrupt source (TIM8)
  * @note
  *   - TIM0-7 can generate only an TIM_IT_Update interrupt.
  *   - TIM9 can generate TIM_IT_Update and TIM_IT_CC0 interrupt.
  *   - TIM8 can generate all interrupts.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void RTIM_INTClearPendingBit(RTIM_TypeDef *TIM, u32 TIM_IT)
{
	u32 CounterIndex = 0;

	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));
	assert_param(IS_TIM_IT(TIM_IT));

	/* Clear the IT pending Bit */
	TIM->SR = TIM_IT;

	/* make sure write ok, because bus delay */
	while (1) {
		CounterIndex++;
		if (CounterIndex >= 300) {
			break;
		}

		if (((TIM->SR) & TIM_IT) == 0) {
			break;
		}
	}
}

/**
  * @brief  Checks whether the specified TIM flag is set or not.
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @param  TIM_FLAG: specifies the TIM interrupt source to check.
  *      This parameter can be one of the following values:
  *     @arg TIM_IT_Update: TIM update Interrupt source (TIM0-9)
  *     @arg TIM_IT_CC0: TIM Capture Compare 0 Interrupt source (TIM8/TIM9)
  *     @arg TIM_IT_CC1: TIM Capture Compare 1 Interrupt source (TIM8)
  *     @arg TIM_IT_CC2: TIM Capture Compare 2 Interrupt source (TIM8)
  *     @arg TIM_IT_CC3: TIM Capture Compare 3 Interrupt source (TIM8)
  *     @arg TIM_IT_CC4: TIM Capture Compare 4 Interrupt source (TIM8)
  *     @arg TIM_IT_CC5: TIM Capture Compare 5 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE0: TIM Update Event 0 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE1: TIM Update Event 1 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE2: TIM Update Event 2 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE3: TIM Update Event 3 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE4: TIM Update Event 4 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE5: TIM Update Event 5 Interrupt source (TIM8)
  * @note
  *   - TIM0-7 can generate only an TIM_IT_Update interrupt.
  *   - TIM9 can generate TIM_IT_Update and TIM_IT_CC0 interrupt.
  *   - TIM8 can generate all interrupts.
  * @retval  _TRUE: the specified TIM flag is set
  *              _FALSE: the specified TIM flag is not set.
  */
__weak HAL_ROM_TEXT_SECTION
u32 RTIM_GetFlagStatus(RTIM_TypeDef *TIM, u32 TIM_FLAG)
{
	u32 bitstatus = _FALSE;

	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));
	assert_param(IS_TIM_GET_IT(TIM_FLAG));


	if ((TIM->SR & TIM_FLAG) != 0) {
		bitstatus = _TRUE;
	} else {
		bitstatus = _FALSE;
	}

	return bitstatus;
}

/**
  * @brief  Checks whether the TIM interrupt has occurred or not.
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @param  TIM_IT: specifies the TIM interrupt source to check.
  *      This parameter can be one of the following values:
  *     @arg TIM_IT_Update: TIM update Interrupt source (TIM0-9)
  *     @arg TIM_IT_CC0: TIM Capture Compare 0 Interrupt source (TIM8/TIM9)
  *     @arg TIM_IT_CC1: TIM Capture Compare 1 Interrupt source (TIM8)
  *     @arg TIM_IT_CC2: TIM Capture Compare 2 Interrupt source (TIM8)
  *     @arg TIM_IT_CC3: TIM Capture Compare 3 Interrupt source (TIM8)
  *     @arg TIM_IT_CC4: TIM Capture Compare 4 Interrupt source (TIM8)
  *     @arg TIM_IT_CC5: TIM Capture Compare 5 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE0: TIM Update Event 0 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE1: TIM Update Event 1 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE2: TIM Update Event 2 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE3: TIM Update Event 3 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE4: TIM Update Event 4 Interrupt source (TIM8)
  *     @arg TIM_IT_UIE5: TIM Update Event 5 Interrupt source (TIM8)
  * @note
  *   - TIM0-7 can generate only an TIM_IT_Update interrupt.
  *   - TIM9 can generate TIM_IT_Update and TIM_IT_CC0 interrupt.
  *   - TIM8 can generate all interrupts.
  *
  * @retval _TRUE: The specified TIM interrupt has occurred
  *              _FALSE: The specified TIM interrupt has not occurred.
  */
__weak HAL_ROM_TEXT_SECTION
u32 RTIM_GetINTStatus(RTIM_TypeDef *TIM, u32 TIM_IT)
{
	u32 bitstatus = _FALSE;
	u32 itstatus = 0x0, itenable = 0x0;

	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));
	assert_param(IS_TIM_GET_IT(TIM_IT));

	itstatus = TIM->SR & TIM_IT;
	itenable = TIM->DIER & TIM_IT;

	if ((itstatus != 0) && (itenable != 0)) {
		bitstatus = _TRUE;
	} else {
		bitstatus = _FALSE;
	}

	return bitstatus;
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
