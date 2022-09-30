/**
  ******************************************************************************
  * @file    ameba_comparator.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Comparator peripheral:
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
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

/**
  * @brief	 Initializes the parameters in the CMP_InitStruct with default value.
  * @param CMP_InitStruct: pointer to a CMP_InitTypeDef structure that contains
  *         the configuration information for the Comparator peripheral.
  * @retval None
  */
void CMP_StructInit(CMP_InitTypeDef *CMP_InitStruct)
{
	u8 i;

	for (i = 0; i < COMP_CH_NUM; i++) {
		CMP_InitStruct->CMP_ChanCtrl[i].CMP_ChIndex = i;
		CMP_InitStruct->CMP_ChanCtrl[i].CMP_Ref0 = 0;		// Vref0 = 0.1*0=0v
		CMP_InitStruct->CMP_ChanCtrl[i].CMP_Ref1 = 0x1F;		//  Vref1 = 0.1*31=3.1v
		CMP_InitStruct->CMP_ChanCtrl[i].CMP_WakeType = COMP_WK_NONE;
		CMP_InitStruct->CMP_ChanCtrl[i].CMP_WakeSysCtrl = COMP_WITHIN_REF0_AND_REF1;
		CMP_InitStruct->CMP_ChanCtrl[i].CMP_WakeADCCtrl = COMP_WITHIN_REF0_AND_REF1;
	}
}

/**
  * @brief	 Initializes the Comparator according to the specified parameters in CMP_InitStruct.
  * @param  CMP_InitStruct: pointer to a CMP_InitTypeDef structure that contains
  *         the configuration for the Comparator peripheral.
  * @retval None
  */
void CMP_Init(CMP_InitTypeDef *CMP_InitStruct)
{
	u8 i, index;
	u32 tmp, val_ch = 0;

	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	CMP_CHTypeDef *compChan = CMP_InitStruct->CMP_ChanCtrl;

	CMP_Cmd(DISABLE);

	tmp = comparator->COMP_INTR_CTRL;

	for (i = 0; i < COMP_CH_NUM; i++) {
		index = compChan[i].CMP_ChIndex;

		val_ch |= (u32)(index << COMP_SHIFT_CHSW(i));

		comparator->COMP_REF_CHx[index] = (u32)(COMP_REF0_CHx(compChan[i].CMP_Ref0) | COMP_REF1_CHx(compChan[i].CMP_Ref1));

		if (compChan[i].CMP_WakeType & COMP_WK_ADC) {
			tmp &= ~ COMP_MASK_WK_ADC_CTRL(index);
			tmp |= (compChan[i].CMP_WakeADCCtrl << COMP_SHIFT_WK_ADC_CTRL(index)) | \
				   COMP_WK_ADC_EN(index);
		}

		if (compChan[i].CMP_WakeType & COMP_WK_SYS) {
			tmp &= ~ COMP_MASK_WK_SYS_CTRL(index);
			tmp |= (compChan[i].CMP_WakeSysCtrl << COMP_SHIFT_WK_SYS_CTRL(index)) | \
				   COMP_WK_SYS_EN(index);
		}
	}

	comparator->COMP_INTR_CTRL = tmp;
	comparator->COMP_CHSW_LIST = val_ch;

	comparator->COMP_LPSD |= COMP_BIT_LPSD_1;
	comparator->COMP_AUTO_SHUT = COMP_BIT_AUTO_SHUT;
}

/**
  * @brief  Enable or Disable the Comparator peripheral.
  * @param  NewState: new state of the Comparator peripheral.
  *   			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CMP_Cmd(u32 NewState)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	if (NewState != DISABLE) {
		comparator->COMP_EN_TRIG |= COMP_BIT_ENABLE;
	} else {
		comparator->COMP_EN_TRIG &= ~COMP_BIT_ENABLE;
	}
}

/**
  * @brief  Indicate the Comparator is busy or not .
  * @param  None
  * @retval   busy status value:
  *           - 1: Busy
  *           - 0: Not Busy.
  */
u32 CMP_Busy(void)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	if (comparator->COMP_BUSY_STS & COMP_BIT_BUSY_STS) {
		return 1;
	} else {
		return 0;
	}
}

/**
  * @brief  ENABLE/DISABLE  the comparator's interrupt bits.
  * @param  Cmp_IT: specifies the interrupt bits to be configured.
  *     This parameter can be one or combinations of the following values:
  *     @arg COMP_IT_CTN_VS: Rising edge interrupt of hysteresis cmp result at continuous mode.
  *     @arg COMP_IT_CTN_VH: Rising edge interrupt of vref1 cmp result at continuous mode.
  *     @arg COMP_IT_CTN_VL: Rising edge interrupt of vref0 cmp result at continuous mode.
  * @param NewState: ENABLE or DISABLE
  * @note None
  * @retval None
  */
void CMP_INTConfig(u32 Cmp_IT, u32 NewState)
{
	/* Check the parameters */
	assert_param(IS_COMP_IT(Cmp_IT));

	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	if (NewState != DISABLE) {
		/* Enable the Interrupt sources */
		comparator->COMP_INTR_CTRL |= COMP_IT_CONFIG(Cmp_IT);
	} else {
		/* Disable the Interrupt sources */
		comparator->COMP_INTR_CTRL &= ~COMP_IT_CONFIG(Cmp_IT);
	}
}

/**
  * @brief  Get value of Comparator Wakeup ADC/SYS Status Register .
  * @param  None
  * @retval Current Comparator Wakeup ADC/SYS and cmp result's rising edge interrupt status
  */
u32 CMP_GetISR(void)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	return comparator->COMP_WK_STS;
}

/**
  * @brief  Clear pending bit in Comparator Wakeup ADC/SYS Status Register.
  * @param  Cmp_IT: specifies the interrupt bits to be cleared.
  *     This parameter can be one or combinations of the following values:
  *     @arg COMP_IT_CTN_VS: Rising edge interrupt of hysteresis cmp result at continuous mode.
  *     @arg COMP_IT_CTN_VH: Rising edge interrupt of vref1 cmp result at continuous mode.
  *     @arg COMP_IT_CTN_VL: Rising edge interrupt of vref0 cmp result at continuous mode.
  *     @arg COMP_WK_SYS_EN(3): Channel3 wakeup system
  *     @arg COMP_WK_SYS_EN(2): Channel2 wakeup system
  *     @arg COMP_WK_SYS_EN(1): Channel1 wakeup system
  *     @arg COMP_WK_SYS_EN(0): Channel0 wakeup system
  *     @arg COMP_WK_ADC_EN(3): Channel3 wakeup ADC
  *     @arg COMP_WK_ADC_EN(2): Channel2 wakeup ADC
  *     @arg COMP_WK_ADC_EN(1): Channel1 wakeup ADC
  *     @arg COMP_WK_ADC_EN(0): Channel0 wakeup ADC
  * @retval   None
  */
void CMP_INTClearPendingBit(u32 Cmp_IT)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	comparator->COMP_WK_STS = Cmp_IT;
}

/**
  * @brief  Get the comparison result .
  * @param  channel : the Comparator channel index
  * @retval  The comparison result of specified channel index. The return value can be:
  *		-0: when Vin < Vref0
  *		-2: when Vin > Vref0 & Vin < Vref1
  *		-3: when Vin > Vref1
  */
u32 CMP_GetCompStatus(u8 channel)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	u32 value = (comparator->COMP_CH_STS & COMP_MASK_CH_STS(channel)) >> \
				COMP_SHIFT_CH_STS(channel);

	return value;
}

/**
  * @brief  Get the channel list index of the last used channel.
  * @param  None.
  * @retval  The channel list index of the last used channel. Not channel ID.
  */
u32 CMP_GetLastChan(void)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	return comparator->COMP_LAST_CH;
}

/**
  * @brief  Reset the channel switch to default state.
  * @param  None
  * @retval  None
  */
void CMP_ResetCSwList(void)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	comparator->COMP_RST_LIST = COMP_BIT_RST_LIST;
	comparator->COMP_RST_LIST = 0;
}

/**
  * @brief  Enable or disable automatic shut after the analog conversion is done.
  * @param  NewState: can be one of the following value:
  *			@arg ENABLE: The analog module and mux will be disabled automatically after the analog conversion is done.
  *			@arg DISABLE: The analog module and mux will not be disabled.
  * @retval  None.
  */
void CMP_AutoShutCmd(u32 NewState)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	if (NewState != DISABLE) {
		comparator->COMP_AUTO_SHUT |= COMP_BIT_AUTO_SHUT;
	} else {
		comparator->COMP_AUTO_SHUT &= ~COMP_BIT_AUTO_SHUT;
	}
}

/**
  * @brief  Controls the automatic channel swtich enabled or disabled in Comparator Automatic Mode.
  * @param  NewState: can be one of the following value:
  *			@arg ENABLE: Enable the automatic channel switch.
  *				When setting this bit, an automatic channel switch starts from the first channel in the channel switch list.
  *			@arg DISABLE:  Disable the automatic channel switch.
  *				If an automatic channel switch is in progess, writing 0 will terminate the automatic channel switch.
  * @retval  None.
  */
void CMP_AutoCSwCmd(u32 NewState)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	if (NewState != DISABLE) {
		comparator->COMP_AUTOSW_EN = COMP_BIT_AUTOSW_EN;
		comparator->COMP_EN_TRIG |= COMP_BIT_EN_TRIG;
	} else {
		comparator->COMP_AUTOSW_EN = 0;
		comparator->COMP_EN_TRIG &= ~COMP_BIT_EN_TRIG;
	}
}

/**
  * @brief  Enabled or disabled software-trigger channel switch in Comparator Software-Trigger Mode.
  * @param  NewState: can be one of the following value:
  *			@arg ENABLE: Enable the software-trigger channel switch.
  *				When setting this bit, do one channel switch in the channel switch list.
  *			@arg DISABLE:  Disable the software-trigger channel switch.
  * @retval  None.
  */
void CMP_SwTrigCmd(u32 NewState)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	if (NewState != DISABLE) {
		comparator->COMP_EN_TRIG |= COMP_BIT_EN_TRIG;
	} else {
		comparator->COMP_EN_TRIG &= ~COMP_BIT_EN_TRIG;
	}
}

/**
  * @brief	 Initialize the trigger timer when in Comparator Timer-Trigger Mode.
  * @param  Tim_Idx: The timer index would be used to make the Comparator module do a comparison.
  * @param  PeriodMs: Indicate the period of trigger timer.
  * @param  NewState: can be one of the following value:
  *			@arg ENABLE: Enable the Comparator timer trigger mode.
  *			@arg DISABLE: Disable the Comparator timer trigger mode.
  * @retval  None.
  * @note  Used in Comparator Timer-Trigger Mode
  */
void CMP_TimerTrigCmd(u8 Tim_Idx, u32 PeriodMs, u32 NewState)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	RTIM_TimeBaseInitTypeDef TIM_InitStruct;

	comparator->COMP_EXT_TRIG_TIMER_SEL = Tim_Idx;

	if (NewState != DISABLE) {
		RTIM_TimeBaseStructInit(&TIM_InitStruct);
		TIM_InitStruct.TIM_Idx = Tim_Idx;
		TIM_InitStruct.TIM_Period = (PeriodMs * 32768) / 1000 / 2; //ms to tick

		RTIM_TimeBaseInit(TIMx[Tim_Idx], &TIM_InitStruct, TIMx_irq[Tim_Idx], (IRQ_FUN)NULL, (u32)NULL);
		RTIM_Cmd(TIMx[Tim_Idx], ENABLE);

		comparator->COMP_EXT_TRIG_CTRL = COMP_BIT_EXT_WK_TIMER;
	} else {
		RTIM_Cmd(TIMx[Tim_Idx], DISABLE);
		comparator->COMP_EXT_TRIG_CTRL = 0;
	}
}

/**
  * @brief Enable or disable continuous sample function in autoswitch mode.
  * @param  NewState: can be one of the following value:
  *			@arg ENABLE: Enable continuous sample function.
  *			@arg DISABLE: Disable continuous sample function.
  * @note Continuous sample function can only take effect in autoswitch mode with autoshut disabled meanwhile.
  * @retval  None.
  */
void CMP_ContinousCmd(u32 NewState)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	if (NewState != DISABLE) {
		comparator->COMP_CONTINUOUS_CTRL_0 |= COMP_BIT_CONTINUOUS_EN;
	} else {
		comparator->COMP_CONTINUOUS_CTRL_0 &= ~COMP_BIT_CONTINUOUS_EN;
	}
}

/**
  * @brief Enable or disable hysteresis circuit function in continuous function.
  * @param  NewState: can be one of the following value:
  *			@arg ENABLE: Enable hysteresis circuit function.
  *			@arg DISABLE: Disable hysteresis circuit function.
  * @note Hysteresis circuit function can only take effect in continuous function.
  * @retval  None.
  */
void CMP_HystCmd(u32 NewState)
{
	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	u32 reg_chsw = comparator->COMP_CHSW_LIST;

	if (NewState != DISABLE) {
		/* check whether in continuous mode */
		assert_param(comparator->COMP_CONTINUOUS_CTRL_0 & COMP_BIT_CONTINUOUS_EN);
		assert_param(comparator->COMP_AUTOSW_EN & COMP_BIT_AUTOSW_EN);
		assert_param((comparator->COMP_AUTO_SHUT & COMP_BIT_AUTO_SHUT) == 0);
		assert_param(COMP_GET_CHSW_0(reg_chsw) == COMP_GET_CHSW_1(reg_chsw));
		assert_param(COMP_GET_CHSW_1(reg_chsw) == COMP_GET_CHSW_2(reg_chsw));
		assert_param(COMP_GET_CHSW_2(reg_chsw) == COMP_GET_CHSW_3(reg_chsw));

		comparator->COMP_CONTINUOUS_CTRL_0 |= COMP_BIT_HYST_EN;
	} else {
		comparator->COMP_CONTINUOUS_CTRL_0 &= ~COMP_BIT_HYST_EN;
	}
}

/**
  * @brief	Select specified comparison result as ZCD output.
  * @param  Src_Type: Comparison result type.
  *     This parameter can be one of the following values:
  *			@arg COMP_ZCD_SRC_NONE: None.
  *			@arg COMP_ZCD_SRC_VHOUT: ZCD out is from high level Vref1 cmp.
  *			@arg COMP_ZCD_SRC_VLOUT: ZCD out is from low level Vref0 cmp.
  *			@arg COMP_ZCD_SRC_VSOUT: ZCD out is from Hysteresis cmp.
  *	@note Make sure hysteresis function is enabled when select COMP_ZCD_SRC_VSOUT.
  * @retval  None.
  */
void CMP_SetZcdSrc(u32 Src_Type)
{
	/* Check the parameters */
	assert_param(IS_COMP_ZCD_SRC_TYPE(Src_Type));

	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	u32 temp = comparator->COMP_CONTINUOUS_CTRL_0;
	temp &= ~ COMP_MASK_ZCD_SEL;
	temp |= COMP_ZCD_SEL(Src_Type);
	comparator->COMP_CONTINUOUS_CTRL_0 = temp;

}

/**
  * @brief	Set debounce counter for comparison output of different types.
  * @param  Dbc_Type: Indicate the comparison type.
  *     This parameter can be one of the following values:
  *			@arg COMP_DBC_VS: Debounce result of hysteresis cmp.
  *			@arg COMP_DBC_VH: Debounce result of high level Vref1 cmp.
  *			@arg COMP_DBC_VL: Debounce result of lov level Vref0 cmp.
  * @param  Dbc_Cnt: Debounce counter to be set.
  *	@note Dbc_Cnt = 0 means debounce function is disabled.
  * @retval  None.
  */
void CMP_SetDbcCnt(u8 Dbc_Type, u32 Dbc_Cnt)
{
	/* Check the parameters */
	assert_param(IS_COMP_DBC_TYPE(Dbc_Type));

	CMP_TypeDef *comparator = CMP_DEV;

	if (TrustZone_IsSecure()) {
		comparator = CMP_DEV_S;
	}

	/* Set debounce counter for Hysterisis comparison result S_out */
	if (COMP_DBC_VS == Dbc_Type) {
		/* Check the parameters */
		assert_param(IS_COMP_VS_BC_CNT(Dbc_Cnt));

		u32 temp = comparator->COMP_CONTINUOUS_CTRL_0;
		temp &= ~ COMP_MASK_VS_DBC_CNT;
		temp |= COMP_VS_DBC_CNT(Dbc_Cnt);
		comparator->COMP_CONTINUOUS_CTRL_0 = temp;
		/* Set debounce counter for Vref1 comparison result H_out */
	} else if (COMP_DBC_VH == Dbc_Type) {
		/* Check the parameters */
		assert_param(IS_COMP_VH_BC_CNT(Dbc_Cnt));

		u32 temp = comparator->COMP_CONTINUOUS_CTRL_1;
		temp &= ~ COMP_MASK_VH_DBC_CNT;
		temp |= COMP_VH_DBC_CNT(Dbc_Cnt);
		comparator->COMP_CONTINUOUS_CTRL_1 = temp;
		/* Set debounce counter for Vref0 comparison result L_out */
	} else if (COMP_DBC_VL == Dbc_Type) {
		/* Check the parameters */
		assert_param(IS_COMP_VL_BC_CNT(Dbc_Cnt));

		u32 temp = comparator->COMP_CONTINUOUS_CTRL_1;
		temp &= ~ COMP_MASK_VL_DBC_CNT;
		temp |= COMP_VL_DBC_CNT(Dbc_Cnt);
		comparator->COMP_CONTINUOUS_CTRL_1 = temp;
	}

}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
