/**
  ******************************************************************************
  * @file    rtl8721d_tim.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Timer peripheral:
  *           - Initialization
  *           - TimeBase configuration and management
  *           - Intput Capture/Output Compare configuration and management
  *           - Interrupt and DMA management
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

int TIMx_irq[TimerNum] = {
#if defined (ARM_CORE_CM0)
	NULL,
	NULL,
	TIMER2_IRQ,
	TIMER3_IRQ,
	TIMER4_IRQ,
	TIMER5_IRQ,
	TIMER6_IRQ,
	TIMER7_IRQ,
	NULL,
	NULL,
	TIMER10_IRQ,
	TIMER11_IRQ,
	TIMER12_IRQ,
	TIMER13_IRQ,
#else
	TIMER0_IRQ,
	TIMER1_IRQ,
	TIMER2_IRQ,
	TIMER3_IRQ,
	TIMER4_IRQ,
	TIMER5_IRQ,
	TIMER6_IRQ,
	TIMER7_IRQ,
	TIMER8_IRQ,
	TIMER9_IRQ,
	TIMER10_IRQ,
	TIMER11_IRQ,
	TIMER12_IRQ,
	TIMER13_IRQ,
#endif
};

u32 APBPeriph_TIMx[TimerNum] = {
	APBPeriph_TIM0,
	APBPeriph_TIM1,
	APBPeriph_TIM2,
	APBPeriph_TIM3,
	APBPeriph_TIM4,
	APBPeriph_TIM5,
	APBPeriph_TIM6,
	APBPeriph_TIM7,
	APBPeriph_TIM8,
	APBPeriph_TIM9,
	APBPeriph_TIM10,
	APBPeriph_TIM11,
	APBPeriph_TIM12,
	APBPeriph_TIM13
};

u32 APBPeriph_TIMx_CLOCK[TimerNum] = {
	APBPeriph_TIM0_CLOCK,
	APBPeriph_TIM1_CLOCK,
	APBPeriph_TIM2_CLOCK,
	APBPeriph_TIM3_CLOCK,
	APBPeriph_TIM4_CLOCK,
	APBPeriph_TIM5_CLOCK,
	APBPeriph_TIM6_CLOCK,
	APBPeriph_TIM7_CLOCK,
	APBPeriph_TIM8_CLOCK,
	APBPeriph_TIM9_CLOCK,
	APBPeriph_TIM10_CLOCK,
	APBPeriph_TIM11_CLOCK,
	APBPeriph_TIM12_CLOCK,
	APBPeriph_TIM13_CLOCK
};

RTIM_TypeDef *TIMx[TimerNum] = {
	TIM0,
	TIM1,
	TIM2,
	TIM3,
	TIM4,
	TIM5,
	TIM6,
	TIM7,
	TIM8,
	TIM9,
	TIM10,
	TIM11,
	TIM12,
	TIM13
};

RTIM_TypeDef *TIMx_S[TimerNum] = {
	TIM0_S,
	TIM1_S,
	TIM2_S,
	TIM3_S,
	TIM4_S,
	TIM5_S,
	TIM6_S,
	TIM7_S,
	TIM8_S,
	TIM9_S,
	TIM10_S,
	TIM11_S,
	TIM12_S,
	TIM13_S,
};

u32 TIM_IT_CCx[PWM_CHAN_MAX] = {
	TIM_IT_CC0,
	TIM_IT_CC1,
	TIM_IT_CC2,
	TIM_IT_CC3,
	TIM_IT_CC4,
	TIM_IT_CC5,
};

/**
  * @brief  Enables or Disables the TIM Update event(UEV).
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @param  NewState: new state of the TIM UDIS bit
  *          This parameter can be:ENABLE or DISABLE
  * @note
  *		- If NewState is ENABLE, Update Disable Bit is set, UEV disable and shadow registers keep their value.
  *		- If NewState is DISABLE, Update Disable Bit is clear, UEV enable and buffered registers are loaded with
  *		their preload values when UEV happen.
  * @retval None
  */
void RTIM_UpdateDisableConfig(RTIM_TypeDef *TIM, u32 NewState)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));

	if (NewState != DISABLE) {
		/* Set the Update Disable Bit */
		TIM->CR |= TIM_BIT_UDIS;
	} else {
		/* Reset the Update Disable Bit */
		TIM->CR &= ~TIM_BIT_UDIS;
	}
}

/**
  * @brief  Enables or disables TIM peripheral Preload register on ARR.
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @param  NewState: new state of the TIM peripheral Preload register
  *          This parameter can be: ENABLE or DISABLE.
  * @note
  *		- DISABLE: TIM_ARR register is not buffered, and shadow register will update immediately
  *		- ENABLE: TIM_ARR register is buffered, and shadow register will update after overflow
  * @retval None
  */
void RTIM_ARRPreloadConfig(RTIM_TypeDef *TIM, u32 NewState)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));

	if (NewState != DISABLE) {
		/* Set the ARR Preload Bit */
		TIM->CR |= TIM_BIT_ARPE;
	} else {
		/* Reset the ARR Preload Bit */
		TIM->CR &= ~TIM_BIT_ARPE;
	}
}

/**
  * @brief  Configures the TIM Update Request Interrupt source.
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @param  TIM_UpdateSource: specifies the Update source.
  *          This parameter can be one of the following values:
  *            @arg TIM_UpdateSource_Global: Source of update is the counter
  *                 overflow or the setting of UG bit.
  *            @arg TIM_UpdateSource_Overflow: Source of update is counter overflow.
  * @retval None
  */
void RTIM_UpdateRequestConfig(RTIM_TypeDef *TIM, u32 TIM_UpdateSource)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));
	assert_param(IS_TIM_UPDATE_SOURCE(TIM_UpdateSource));

	if (TIM_UpdateSource != TIM_UpdateSource_Global) {
		/* Set the URS Bit */
		TIM->CR |= TIM_BIT_URS;
	} else {
		/* Reset the URS Bit */
		TIM->CR &= (u32)~TIM_BIT_URS;
	}
}

/**
  * @brief  Configures the TIM Prescaler.
  * @param  TIM: where x can be  8 or 9 to select the TIM peripheral.
  * @param  Prescaler: specifies the Prescaler Register value,which can be a number in 0~0xFF range for TIM9 and 0~0xFFFF range for TIM8.
  * @param  TIM_PSCReloadMode: specifies the TIM Prescaler Reload mode
  *          This parameter can be one of the following values:
  *            @arg TIM_PSCReloadMode_Update: The Prescaler is loaded at the update event.
  *            @arg TIM_PSCReloadMode_Immediate: The Prescaler is loaded immediatly.
  * @retval None
  */
void RTIM_PrescalerConfig(RTIM_TypeDef *TIM, u32 Prescaler, u32 TIM_PSCReloadMode)
{
	/* Check the parameters */
	assert_param(IS_TIM_40M_TIM(TIM));
	assert_param(IS_TIM_PSC(Prescaler));
	assert_param(IS_TIM_PRESCALER_RELOAD(TIM_PSCReloadMode));

	/* Set the Prescaler value */
	TIM->PSC = Prescaler;
	/* Set or reset the UG Bit */
	TIM->EGR = TIM_PSCReloadMode;
}

/**
  * @brief  Configures the TIM event to be generate by software.
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @param  TIM_EventSource: specifies the event source.
  *          This parameter can be one or more of the following values @ref TIM_Event_Generation_definitons
  * @note
  *		- TIM0~9 have Timer update Event source TIM_EventSource_Update
  *		- TIM9 have Timer Capture Compare 1 Event source TIM_EventSource_CC0
  *		- TIM8 has 6 Timer Capture Compare x Event source TIM_EventSource_CC0-5.
  * @retval None
  */
void RTIM_GenerateEvent(RTIM_TypeDef *TIM, u32 TIM_EventSource)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));
	assert_param((IS_HP_TIM_EVENT_SOURCE(TIM_EventSource) || IS_LP_TIM_EVENT_SOURCE(TIM_EventSource)));

	/* Set the event sources */
	TIM->EGR = TIM_EventSource;
}

/**
  * @brief  Sets the TIM Autoreload Register(TIM_ARR) value to change period
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @param  Autoreload: specifies the Autoreload register new value.
  *			To TIM0~7 this value can be a number in 0~0xFFFFFFFF range.
  *			To TIM8~9 this value can be a number in 0~0xFFFF range.
  * @retval None
  */
void RTIM_ChangePeriodImmediate(RTIM_TypeDef *TIM, u32 Autoreload)
{
	u32 syncphase;

	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));

	/* Reset the ARR Preload Bit */
	/* period will update immediatly */
	TIM->CR &= ~TIM_BIT_ARPE;

	/* Set the Autoreload Register value */
	if (IS_TIM_PWM_TIM(TIM)) {
		/* Ensure phasesync value is less than arr value*/
		for (int i = 0; i < 5; i++) {
			syncphase = TIM->PSYNCx[i] & TIM_MASK_SYNCPHASEx;
			assert_param(syncphase <= Autoreload);
		}
		TIM->ARR = TIM_CNT(Autoreload);
	} else if (IS_TIM_INPULSE_TIM(TIM)) {
		TIM->ARR = TIM_CNT(Autoreload);
	} else {
		TIM->ARR = TIMBasic_CNT(Autoreload);
	}

	/* Generate an update event */
	/* 1) reload the Prescaler immediatly */
	/* 2) reload the ARR immediatly */
	/* 3) hadrware will clear this bit after reload  */
	/* 4) UEV will reset counter, and counter will start from 0 */
	/* 5) gen a interrupt if use TIM_UpdateSource_Global */
	TIM->EGR = TIM_PSCReloadMode_Immediate;

	/* poll EGR UG done */
	while (1) {
		if (TIM->SR & TIM_BIT_UG_DONE) {
			break;
		}
	}
}

/**
  * @brief  Sets the TIM Autoreload Register(TIM_ARR) value to change period with protection
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @param  Autoreload: specifies the Autoreload register new value.
  *			To TIM0~7 this value can be a number in 0~0xFFFFFFFF range.
  *			To TIM8~9 this value can be a number in 0~0xFFFF range.
  * @retval None
  */
void RTIM_ChangePeriod(RTIM_TypeDef *TIM, u32 Autoreload)
{
	u32 syncphase;

	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));

	/* Set the Autoreload Register value */
	if (IS_TIM_PWM_TIM(TIM)) {
		/* Ensure phasesync value is less than arr value*/
		for (int i = 0; i < 5; i++) {
			syncphase = TIM->PSYNCx[i] & TIM_MASK_SYNCPHASEx;
			assert_param(syncphase <= Autoreload);
		}

		TIM->ARR = TIM_CNT(Autoreload);
	} else if (IS_TIM_INPULSE_TIM(TIM)) {
		TIM->ARR = TIM_CNT(Autoreload);
	} else {
		TIM->ARR = TIMBasic_CNT(Autoreload);
	}
}

/**
  * @brief  Reset timer, counter will start from 0
  * @param  TIM: where x can be 0-9 to select the TIM peripheral.
  * @retval None
  */
void RTIM_Reset(RTIM_TypeDef *TIM)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_TIM(TIM));

	/* Generate an update event */
	/* 1) reload the Prescaler immediatly */
	/* 2) reload the ARR immediatly */
	/* 3) hadrware will clear this bit after reload  */
	/* 4) UEV will reset counter, and counter will start from 0 */
	/* 5) gen a interrupt if use TIM_UpdateSource_Global */
	TIM->EGR = TIM_PSCReloadMode_Immediate;

	/* poll EGR UG done */
	while (1) {
		if (TIM->SR & TIM_BIT_UG_DONE) {
			break;
		}
	}
}

/**
  * @brief  Fills each TIM_CCInitStruct member with its default value.
  * @param  TIM_CCInitStruct: pointer to a TIM_CCInitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void RTIM_CCStructInit(TIM_CCInitTypeDef *TIM_CCInitStruct)
{
	/* Set the default configuration */
	TIM_CCInitStruct->TIM_CCMode = TIM_CCMode_PWM;
	TIM_CCInitStruct->TIM_CCPolarity = TIM_CCPolarity_High;
	TIM_CCInitStruct->TIM_OCProtection = TIM_OCPreload_Enable;
	TIM_CCInitStruct->TIM_OCPulse = 0xFFF;
}

/**
  * @brief  Initializes the TIM Channel 0-5 according to the specified parameters in
  *         the TIM_CCInitStruct.
  * @param  TIM: where x can be 8/9, to select the TIM peripheral.
  * @param  TIM_CCInitStruct: pointer to a TIM_CCInitTypeDef structure that contains
  *         the configuration information for the specified TIM peripheral.
  * @param  TIM_Channel: the channel need to be initialized,
  *		which can be one of the following parameters @ref TIM_Channel_definitions
  * @note
  *		- TIM9 only has 1 channel: TIM_Channel_0
  *		- TIM8 has 6 channels:  TIM_Channel_0-5.
  * @retval None
  */
void RTIM_CCxInit(RTIM_TypeDef *TIM, TIM_CCInitTypeDef *TIM_CCInitStruct, u16 TIM_Channel)
{
	/* Check the parameters */
	assert_param(IS_TIM_CCM_TIM(TIM));
	assert_param(IS_TIM_CC_MODE(TIM_CCInitStruct->TIM_CCMode));
	assert_param(IS_TIM_CC_POLARITY(TIM_CCInitStruct->TIM_CCPolarity));
	assert_param(IS_TIM_OCPRELOAD_STATE(TIM_CCInitStruct->TIM_OCProtection));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));

	u32 Status;

	/* Reset the CCMR Bit */
	TIM->CCRx[TIM_Channel] = 0;

	/* Write to TIM CCMR */
	if (IS_TIM_PWM_TIM(TIM)) {
		TIM->CCRx[TIM_Channel] = (TIM_CCInitStruct->TIM_CCPolarity |
								   TIM_CCInitStruct->TIM_OCProtection |
								   TIM_CCInitStruct->TIM_CCMode |
								   TIM_CCInitStruct->TIM_OCPulse);
	} else if (IS_TIM_INPULSE_TIM(TIM)) {
		TIM->CCRx[TIM_Channel] = (TIM_CCInitStruct->TIM_CCPolarity |
								   TIM_CCInitStruct->TIM_ICPulseMode);
	}

	/* Generate an update event */
	/* 1) reload the CCRx immediatly */
	/* 2) hadrware will clear this bit after reload, about 71.936 us (2*32k cycles)  */
	/* 3) UEV will reset counter, and counter will start from 0 */
	/* 4) gen a interrupt if use TIM_UpdateSource_Global */
	TIM->EGR = TIM_PSCReloadMode_Immediate;

	/* poll EGR UG done */
	while (1) {
		if (TIM->SR & TIM_BIT_UG_DONE) {
			break;
		}
	}

	Status = TIM->SR;
	TIM->SR = Status;
}

/**
  * @brief  Initializes the TIM Channel 0-5 CCmode.
  * @param  TIM: where x can be 8, to select the TIM peripheral.
  * @param  TIM_Channel: the channel need to be set mode,
  *		which can be one of the following parameters @ref TIM_Channel_definitions.
  * @param  TIM_CCMode: CCx working mode which can be one of the following parameters:
  *		@arg TIM_CCMode_PWM
  *		@arg TIM_CCMode_Inputcapture
  * @note
  *		- TIM9 only has 1 channel: TIM_Channel_0
  *		- TIM8 has 6 channels:  TIM_Channel_0-5.
  * @retval None
  */
void RTIM_CCRxMode(RTIM_TypeDef *TIM, u16 TIM_Channel, u32 TIM_CCMode)
{
	u32 tmpccmr = TIM->CCRx[TIM_Channel];

	/* Check the parameters */
	assert_param(IS_TIM_PWM_TIM(TIM));
	assert_param(IS_TIM_CC_MODE(TIM_CCMode));

	tmpccmr &= ~TIM_BIT_CCxM;
	tmpccmr |= TIM_CCMode;

	/* Write to TIM CCMR */
	TIM->CCRx[TIM_Channel] = tmpccmr;
}

/**
  * @brief  Sets the TIM Capture Compare X register value
  * @param  TIM: where x can be 8, to select the TIM peripheral.
  * @param  Compare: the value specifies pulsewidth, which is in the 0x00~0xFFFF range.
  *					Duty cycle = Compare / (ARR+1).
  * @param  TIM_Channel: the channel to be set,
  *		which can be one of the following parameters @ref TIM_Channel_definitions.
  * @retval None
  * @note
  *		- CCRx=0 will give 0% cycle pwm pulse.
  *		- CCRx>=TIM_Period there will be 100% pwm pulse.
  *		- TIM8 has 6 channels:  TIM_Channel_0-5.
  */
void RTIM_CCRxSet(RTIM_TypeDef *TIM, u32 Compare, u16 TIM_Channel)
{
	u32 PulseWidth = 0;
	u32 tmpccmr = 0;

	/* Check the parameters */
	assert_param(IS_TIM_PWM_TIM(TIM));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));
	assert_param(IS_TIM_CC_PULSEWIDTH(Compare));

	/* CCRx=0 will give 0% cycle pwm pulse */
	PulseWidth = Compare;

	tmpccmr = TIM->CCRx[TIM_Channel];

	/* reset Compare val */
	tmpccmr &= ~TIM_CCMode_CCR;

	/* Set the Capture CompareX Register value */
	tmpccmr |= (u32)PulseWidth;

	/* set CCMR */
	TIM->CCRx[TIM_Channel] = tmpccmr;
}

/**
  * @brief  Gets the TIM Capture Compare X register value.
  * @param  TIM: where x can be 8/9, to select the TIM peripheral.
  * @param  TIM_Channel: the channel to be read,
  *		which can be one of the following parameters @ref TIM_Channel_definitions.
  * @retval  Capture Compare x Register value.
  * @note	If you want to get pulse width of PWM, remember to plus 1 to
  *			the retval of this function.
  *		- TIM9 only has 1 channel:  TIM_Channel_0
  *		- TIM8 has 6 channels:  TIM_Channel_0-5.
  */
u32 RTIM_CCRxGet(RTIM_TypeDef *TIM, u16 TIM_Channel)
{
	/* Check the parameters */
	assert_param(IS_TIM_CCM_TIM(TIM));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));

	/* Get the Capture Compare x Register value */
	return (TIM->CCRx[TIM_Channel] & TIM_CCMode_CCR);
}

/**
  * @brief  Enables or disables the TIM peripheral Preload register on CCRx.
  * @param  TIM: where x can be 8, to select the TIM peripheral.
  * @param  TIM_OCProtection: new state of the TIM peripheral Preload register
  *          This parameter can be one of the following values:
  *            @arg TIM_OCPreload_Enable: value is loaded in the active register at each update event.
  *            @arg TIM_OCPreload_Disable: new value is taken in account immediately
  * @param  TIM_Channel: the channel need to be set,
  *		which can be one of the following parameters @ref TIM_Channel_definitions
  * @note
  *		- TIM8 has 6 channels:  TIM_Channel_0-5.
  * @retval None
  */
void RTIM_OCxPreloadConfig(RTIM_TypeDef *TIM, u32 TIM_OCProtection, u16 TIM_Channel)
{
	u32 tmpccmr = 0;

	/* Check the parameters */
	assert_param(IS_TIM_PWM_TIM(TIM));
	assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCProtection));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));

	tmpccmr = TIM->CCRx[TIM_Channel];

	/* Reset the OC1PE Bit */
	tmpccmr &= ~(TIM_BIT_OCxPE);

	/* Enable or Disable the Output Compare Preload feature */
	tmpccmr |= TIM_OCProtection;

	/* Write to TIM CCMR1 register */
	TIM->CCRx[TIM_Channel] = tmpccmr;
}

/**
  * @brief  Configures the TIM channel x polarity.
  * @param  TIM: where x can be 8, to select the TIM peripheral.
  * @param  TIM_OCPolarity: specifies the OCx Polarity
  *          This parameter can be one of the following values:
  *            @arg TIM_CCPolarity_High: Output Compare active high
  *            @arg TIM_CCPolarity_Low: Output Compare active low
  * @retval None
  * @note
  *		-TIM8 has CCR0-5
  *		-TIM9 has CCR0
  *		-TIM0-7 dont have CCMR
  */
void RTIM_CCxPolarityConfig(RTIM_TypeDef *TIM, u32 TIM_OCPolarity, u16 TIM_Channel)
{
	u32 tmpccmr = 0;

	/* Check the parameters */
	assert_param(IS_TIM_PWM_TIM(TIM));
	assert_param(IS_TIM_CC_POLARITY(TIM_OCPolarity));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));

	tmpccmr = TIM->CCRx[TIM_Channel];

	/* Set or Reset the CCxP Bit */
	tmpccmr &= ~(TIM_BIT_CCxP);
	tmpccmr |= TIM_OCPolarity;

	/* Write to TIM CCER register */
	TIM->CCRx[TIM_Channel] = tmpccmr;
}

/**
  * @brief  Enables or disables the TIM Capture Compare Channel x.
  * @param  TIM: where x can be 8/9, to select the TIM peripheral.
  * @param  TIM_Channel: specifies the TIM Channel
  *          This parameter can be one of the following values @ref TIM_Channel_definitions
  * @param  TIM_CCx: specifies the TIM Channel CCxE bit new state.
  *          This parameter can be one of the following values:
  *		@arg TIM_CCx_Enable
  *		@arg TIM_CCx_Disable
  * @note
  *		- TIM8 has 6 channels:  TIM_Channel_0-5.
  * @retval None
  */
void RTIM_CCxCmd(RTIM_TypeDef *TIM, u16 TIM_Channel, u32 TIM_CCx)
{
	u32 tmpccmr = TIM->CCRx[TIM_Channel];

	/* Check the parameters */
	assert_param(IS_TIM_CCM_TIM(TIM));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));
	assert_param(IS_TIM_CCX(TIM_CCx));

	tmpccmr &= ~TIM_BIT_CCxE;
	tmpccmr |= TIM_CCx;

	/* Set or reset the CCxE Bit */
	TIM->CCRx[TIM_Channel] =  tmpccmr;
}

/**
  * @brief  Set the TIM's One Pulse Mode (output one pulse PWM mode).
  * @param  TIM: where x can be 8 to select the TIM peripheral.
  * @param  TIM_OPMode: specifies the OPM Mode to be used.
  *          This parameter can be one of the following values:
  *            @arg TIM_OPMode_Single
  *            @arg TIM_OPMode_Repetitive
  * @param  TrigerPolarity: specifies the OPM Mode Triger Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_OPMode_ETP_positive
  *            @arg TIM_OPMode_ETP_negative
  *            @arg TIM_OPMode_ETP_bothedge
  * @note  You must select  TIM_OPMode_Single if you want to set One Pluse Mode,
  *		which makes the counter stop automatically at the next UEV.
  * @retval None
  */
void RTIM_SetOnePulseOutputMode(RTIM_TypeDef *TIM, u32 TIM_OPMode, u32 TrigerPolarity)
{
	/* Check the parameters */
	assert_param(IS_TIM_ONE_PULSE_TIM(TIM));
	assert_param(IS_TIM_OPM_MODE(TIM_OPMode));
	assert_param(IS_TIM_OPM_ETP_MODE(TrigerPolarity));

	/* Reset the OPM & ETP Bit */
	TIM->CR &= (u32)~(TIM_BIT_OPM | TIM_MASK_ETP);

	/* Configure the OPM Mode */
	TIM->CR |= TIM_OPMode | TrigerPolarity;
}

/**
  * @brief  Set the TIM's One Pulse Mode (output one pulse PWM mode).
  * @param  TIM: where x can be 8 to select the TIM peripheral.
  * @param  TIM_Channel: the channel to be set,
  *		which can be one of the following parameters @ref TIM_Channel_definitions.
  * @param  DefaultLevel: specifies the OPM Mode Default Level.
  *          This parameter can be one of the following values:
  *            @arg TIMPWM_DefaultLevel_High
  *            @arg TIMPWM_DefaultLevel_Low
  * @note  You must select  TIM_OPMode_Single if you want to set One Pluse Mode,
  *		which makes the counter stop automatically at the next UEV.
  * @retval None
  */
void RTIM_SetOnePulseDefaultLevel(RTIM_TypeDef *TIM, u16 TIM_Channel, u32 DefaultLevel)
{
	/* Check the parameters */
	assert_param(IS_TIM_ONE_PULSE_TIM(TIM));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));
	assert_param(IS_TIMPWM_DefaultLevel(DefaultLevel));

	if (DefaultLevel == TIMPWM_DefaultLevel_High) {
		TIM->CCRx[TIM_Channel] |= TIM_BIT_OPM_DLx;
	} else {
		TIM->CCRx[TIM_Channel] &= ~(TIM_BIT_OPM_DLx);
	}
}

/**
  * @brief  Sets the TIM Phase Sync X value
  * @param  TIM: where x can be 8, to select the TIM peripheral.
  * @param  TIM_Channel: the channel to be set,
  *		which can be one of the following parameters @ref TIM_Channel_definitions.
  * @param  TIM_SyncPhase: the phase sync value compared to CNT
  * @retval None
  * @note
  *		- TIM_SyncPhase should always smaller than ARR value
  */
void RTIM_PSyncxSet(RTIM_TypeDef *TIM, u16 TIM_Channel, u32 TIM_SyncPhase)
{
	u32 tmppsync = 0;
	u32 arr = 0;

	/* Check the parameters */
	assert_param(IS_TIM_PWM_TIM(TIM));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));

	tmppsync = TIM->PSYNCx[TIM_Channel];
	arr = TIM->ARR;

	/* Ensure phasesync value is less than arr value*/
	assert_param(TIM_SyncPhase <= arr);

	tmppsync &= ~(TIM_MASK_SYNCPHASEx);
	tmppsync |= TIM_SYNCPHASEx(TIM_SyncPhase);

	TIM->PSYNCx[TIM_Channel] =  tmppsync;
}

/**
  * @brief  Gets the TIM Phase Sync X value
  * @param  TIM: where x can be 8, to select the TIM peripheral.
  * @param  TIM_Channel: the channel to be read,
  *		which can be one of the following parameters @ref TIM_Channel_definitions.
  * @retval  TIM Phase Sync X value
  */
u32 RTIM_PSyncxGet(RTIM_TypeDef *TIM, u16 TIM_Channel)
{
	/* Check the parameters */
	assert_param(IS_TIM_PWM_TIM(TIM));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));

	return TIM_SYNCPHASEx(TIM->PSYNCx[TIM_Channel]);
}

/**
  * @brief  Sets the TIM Phase Sync X direction
  * @param  TIM: where x can be 8, to select the TIM peripheral.
  * @param  TIM_Channel: the channel to be set,
  *		which can be one of the following parameters @ref TIM_Channel_definitions.
  * @param  TIM_SyncDir: TIMPWM_PSync_Delay/TIMPWM_PSync_Ahead
  * @retval None
  */
void RTIM_PSyncxDir(RTIM_TypeDef *TIM, u16 TIM_Channel, u8 TIM_SyncDir)
{
	u32 tmppsync = 0;

	/* Check the parameters */
	assert_param(IS_TIM_PWM_TIM(TIM));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));
	assert_param(IS_TIMPWM_PSync_Dir(TIM_SyncDir));

	tmppsync = TIM->PSYNCx[TIM_Channel];

	if (TIM_SyncDir == TIMPWM_PSync_Delay) {
		tmppsync &= ~(TIM_BIT_SYNCDIRx);
	} else {
		tmppsync |= TIM_BIT_SYNCDIRx;
	}

	TIM->PSYNCx[TIM_Channel] =  tmppsync;
}

/**
  * @brief  Enables or disables the preload function of TIM phase sync register.
  * @param  TIM: where x can be 8, to select the TIM peripheral.
  * @param  TIM_Channel: the channel need to be set,
  *		which can be one of the following parameters @ref TIM_Channel_definitions
  * @param  TIM_OCProtection: TIMPWM_PSyncPreload_Enable/TIMPWM_PSyncPreload_Disable
  *            @arg TIMPWM_PSyncPreload_Enable: value is loaded in the active register at each update event.
  *            @arg TIMPWM_PSyncPreload_Disable: new value is taken in account immediately
  * @retval None
  */
void RTIM_PSyncxPreloadConfig(RTIM_TypeDef *TIM, u16 TIM_Channel, u32 TIM_PSyncProtection)
{
	u32 tmppsync = 0;

	/* Check the parameters */
	assert_param(IS_TIM_PWM_TIM(TIM));
	assert_param(IS_TIMPWM_PPRELOAD_STATE(TIM_PSyncProtection));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));

	tmppsync = TIM->PSYNCx[TIM_Channel];

	/* Reset the OC1PE Bit */
	if (TIM_PSyncProtection == TIMPWM_PSyncPreload_Enable) {
		tmppsync |= TIM_BIT_SYNCPEx;
	} else {
		tmppsync &= ~(TIM_BIT_SYNCPEx);
	}

	TIM->PSYNCx[TIM_Channel] = tmppsync;
}

/**
  * @brief  Enables or disables the TIM8 Phase Sync function.
  * @param  TIM: where x can be 8, to select the TIM peripheral.
  * @param  TIM_Channel: specifies the TIM Channel
  *          This parameter can be one of the following values @ref TIM_Channel_definitions
  * @param  NewState: ENABLE/DISABLE.
  * @retval None
  */
void RTIM_PSyncxCmd(RTIM_TypeDef *TIM, u16 TIM_Channel, u8 NewState)
{
	u32 tmppsync = 0;

	/* Check the parameters */
	assert_param(IS_TIM_PWM_TIM(TIM));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));

	tmppsync = TIM->PSYNCx[TIM_Channel];

	if (NewState != DISABLE) {
		tmppsync |= TIM_BIT_SYNCENx;
	} else {
		tmppsync &= ~(TIM_BIT_SYNCENx);
	}

	TIM->PSYNCx[TIM_Channel] =  tmppsync;
}

/**
  * @brief  Gets the TIM Channel Counter value.
  * @param  TIM: where x can be 8 to select the TIM peripheral.
  * @param  TIM_Channel: specifies the TIM Channel
  *          This parameter can be one of the following values @ref TIM_Channel_definitions
  * @retval Counter Register value
  */
u32 RTIM_GetChannelCountx(RTIM_TypeDef *TIM, u16 TIM_Channel)
{
	u32 TimerCountOld;
	u32 TimerCountNew;
	u32 TimerRDCnt;

	/* Check the parameters */
	assert_param(IS_TIM_PWM_TIM(TIM));
	assert_param(IS_TIM_CHANNEL(TIM_Channel));

	TimerRDCnt = 0;
	TimerCountOld = TIM->PHASECNTx[TIM_Channel];
	while (1) {
		TimerCountNew = TIM->PHASECNTx[TIM_Channel];

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
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
