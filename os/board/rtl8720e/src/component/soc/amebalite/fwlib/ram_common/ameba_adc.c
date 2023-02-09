/**
  ******************************************************************************
  * @file    ameba_adc.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Analog to Digital Convertor (ADC) peripheral:
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
  * @brief	 Initializes the parameters in the ADC_InitStruct with default values.
  * @param  ADC_InitStruct: pointer to a ADC_InitTypeDef structure that contains
  *         the configuration information for the ADC peripheral.
  * @retval  None
  */
void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct)
{
	u8 i;

	ADC_InitStruct->ADC_OpMode = ADC_SW_TRI_MODE;
	ADC_InitStruct->ADC_ClkDiv = ADC_CLK_DIV_12;
	ADC_InitStruct->ADC_RxThresholdLevel = 0;
	ADC_InitStruct->ADC_CvlistLen = ADC_CH_NUM - 1;
	ADC_InitStruct->ADC_ChanInType = 0;
	ADC_InitStruct->ADC_SpecialCh = 0xFF;
	ADC_InitStruct->ADC_ChIDEn = DISABLE;

	for (i = 0; i < ADC_InitStruct->ADC_CvlistLen + 1; i++) {
		ADC_InitStruct->ADC_Cvlist[i] = i;
	}
}

/**
  * @brief	 Initializes the ADC according to the specified parameters in ADC_InitStruct.
  * @param  ADC_InitStruct: pointer to a ADC_InitTypeDef structure that contains
  *         the configuration information for the ADC peripheral.
  * @retval  None
  */
void ADC_Init(ADC_InitTypeDef *ADC_InitStruct)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	CAPTOUCH_TypeDef *CapTouch = CAPTOUCH_DEV;
	u32 value = 0;
	u8 len, i;
	u8 FT_Type;
	u32 Temp;
	u32 FT_Type_Address = 0x7EB;

	assert_param(IS_ADC_MODE(ADC_InitStruct->ADC_OpMode));
	assert_param(IS_ADC_SAMPLE_CLK(ADC_InitStruct->ADC_ClkDiv));
	assert_param(ADC_InitStruct->ADC_CvlistLen < 16);

	EFUSE_PMAP_READ8(0, FT_Type_Address, &FT_Type, L25EOUTVOLTAGE);
	/* [2:0] Vref Selection */
	FT_Type &= 0x7;

	if (FT_Type == 0x3) {
		/* Set CT_ANA_ADC_REG0X_LPAD register with 3'b 011 */
		Temp = (u32)CapTouch->CT_ANA_ADC_REG0X_LPAD;
		Temp &= ~(0x7 << 8);
		Temp |= (0x3 << 8);
		CapTouch->CT_ANA_ADC_REG0X_LPAD = Temp;
	}

	/* Disable ADC, clear pending interrupt, clear FIFO */
	ADC_Cmd(DISABLE);

	adc->ADC_INTR_CTRL = 0;
	ADC_INTClear();
	ADC_ClearFIFO();

	/* Set clock divider */
	adc->ADC_CLK_DIV = ADC_CLK_DIV(ADC_InitStruct->ADC_ClkDiv);

	/* Set adc configuration*/
	value = adc->ADC_CONF & (~(ADC_MASK_CVLIST_LEN | ADC_MASK_OP_MOD));
	value |= ADC_OP_MOD(ADC_InitStruct->ADC_OpMode) | ADC_CVLIST_LEN(ADC_InitStruct->ADC_CvlistLen);
	adc->ADC_CONF = value;

	/* Set channel input type */
	adc->ADC_IN_TYPE = ADC_InitStruct->ADC_ChanInType;

	/* Set channel switch list */
	value = 0;
	len = ((ADC_InitStruct->ADC_CvlistLen + 1) > 8) ? 8 : (ADC_InitStruct->ADC_CvlistLen + 1);
	for (i = 0; i < len; i++) {
		value |= (u32)(ADC_InitStruct->ADC_Cvlist[i] << ADC_SHIFT_CHSW0(i));
	}
	adc->ADC_CHSW_LIST_0 = value;

	value = 0;
	if ((ADC_InitStruct->ADC_CvlistLen + 1) > 8) {
		for (i = 8; i < (ADC_InitStruct->ADC_CvlistLen + 1); i++) {
			value |= (u32)(ADC_InitStruct->ADC_Cvlist[i] << ADC_SHIFT_CHSW1(i));
		}
	}
	adc->ADC_CHSW_LIST_1 = value;

	/* Set particular channel */
	if (ADC_InitStruct->ADC_SpecialCh < ADC_CH_NUM) {
		ADC_INTConfig(ADC_BIT_IT_CHCV_END_EN, ENABLE);
		adc->ADC_IT_CHNO_CON = (u32)ADC_InitStruct->ADC_SpecialCh;
	}

	/* Set FIFO full level */
	adc->ADC_FULL_LVL = (u32)ADC_InitStruct->ADC_RxThresholdLevel;

	/* Set channel ID included in data or not */
	if (ADC_InitStruct->ADC_ChIDEn) {
		adc->ADC_DELAY_CNT |= ADC_BIT_CH_UNMASK;
	}
}

/**
  * @brief  Enable or Disable the ADC peripheral.
  * @param  NewState: new state of the ADC peripheral.
  *   			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_Cmd(u32 NewState)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	if (NewState != DISABLE) {
		adc->ADC_CONF |= ADC_BIT_ENABLE;
	} else {
		adc->ADC_CONF &= ~ADC_BIT_ENABLE;

		/* Need to clear FIFO, or there will be waste data in FIFO next time ADC is enable. Ameba-D A-cut bug */
		ADC_ClearFIFO();
	}
}

/**
  * @brief	 ENABLE/DISABLE  the ADC interrupt bits.
  * @param  ADC_IT: specifies the ADC interrupts to be configured.
  *         This parameter can be one or combinations of the following parameters:
  *            @arg ADC_BIT_IT_COMPRE_CHx_EN:	ADC channel x compare rising edge interrupt
  *                 where x can be 0-9 corresponding to channelx
  *            @arg ADC_BIT_IT_COMP_CHx_EN:		ADC channel x compare interrupt
  *                 where x can be 0-9 corresponding to channelx
  *            @arg ADC_BIT_IT_ERR_EN:			ADC error state interrupt
  *            @arg ADC_BIT_IT_DAT_OVW_EN:		ADC data overwritten interrupt
  *            @arg ADC_BIT_IT_FIFO_EMPTY_EN:	ADC FIFO empty interrupt
  *            @arg ADC_BIT_IT_FIFO_OVER_EN:	ADC FIFO overflow interrupt
  *            @arg ADC_BIT_IT_FIFO_FULL_EN:	ADC FIFO full interrupt
  *            @arg ADC_BIT_IT_CHCV_END_EN:		ADC particular channel conversion done interrupt
  *            @arg ADC_BIT_IT_CV_END_EN:		ADC conversion end interrupt
  *            @arg ADC_BIT_IT_CVLIST_END_EN:	ADC conversion list end interrupt
  * @param  NewState: ENABLE/DISABLE.
  * @retval  None
  */
void ADC_INTConfig(u32 ADC_IT, u32 NewState)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	if (NewState != DISABLE) {
		adc->ADC_INTR_CTRL |= ADC_IT;
	} else {
		adc->ADC_INTR_CTRL &= ~ADC_IT;
	}
}

/**
  * @brief	 Clear all the ADC interrupt pending bits.
  * @param  None
  * @retval  None
  * @note  This function can also used to clear raw status.
  */
void ADC_INTClear(void)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	/* Clear all the IT pending Bits */
	adc->ADC_INTR_STS = ADC_BIT_IT_ALL_STS;
}

/**
  * @brief  Clear specified ADC interrupt pending bits.
  * @param  ADC_IT: specifies the pending bits to be cleared.
  *   This parameter can be one or combinations of the following values:
  *            @arg ADC_BIT_IT_COMPRE_CHx_STS
  *                 where x can be 0-9 corresponding to channelx
  *            @arg ADC_BIT_IT_COMP_CHx_STS
  *                 where x can be 0-9 corresponding to channelx
  *            @arg ADC_BIT_IT_ERR_STS
  *            @arg ADC_BIT_IT_DAT_OVW_STS
  *            @arg ADC_BIT_IT_FIFO_EMPTY_STS
  *            @arg ADC_BIT_FIFO_OVER_STS
  *            @arg ADC_BIT_FIFO_FULL_STS
  *            @arg ADC_BIT_CHCV_END_STS
  *            @arg ADC_BIT_CV_END_STS
  *            @arg ADC_BIT_CVLIST_END_STS
  * @retval  None
  */
void ADC_INTClearPendingBits(u32 ADC_IT)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	adc->ADC_INTR_STS = ADC_IT;
}

/**
  * @brief	 Get ADC interrupt status.
  * @param  None
  * @retval  Current Interrupt Status, each bit of this value represents one
  *		interrupt status which is as follows:
  *            @arg ADC_BIT_IT_COMPRE_CHx_STS
  *                 where x can be 0-9 corresponding to channelx
  *            @arg ADC_BIT_IT_COMP_CHx_STS
  *                 where x can be 0-9 corresponding to channelx
  *            @arg ADC_BIT_IT_ERR_STS
  *            @arg ADC_BIT_IT_DAT_OVW_STS
  *            @arg ADC_BIT_IT_FIFO_EMPTY_STS
  *            @arg ADC_BIT_FIFO_OVER_STS
  *            @arg ADC_BIT_FIFO_FULL_STS
  *            @arg ADC_BIT_CHCV_END_STS
  *            @arg ADC_BIT_CV_END_STS
  *            @arg ADC_BIT_CVLIST_END_STS
  */
u32 ADC_GetISR(void)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	return adc->ADC_INTR_STS;
}

/**
  * @brief  Get ADC raw interrupt status.
  * @param  None
  * @retval  Current Raw Interrupt Status, each bit of this value represents one
  *		raw interrupt status which is as follows:
  *            @arg ADC_BIT_IT_COMPRE_CHx_RAW_STS
  *                 where x can be 0-9 corresponding to channelx
  *            @arg ADC_BIT_IT_COMP_CHx_RAW_STS
  *                 where x can be 0-9 corresponding to channelx
  *            @arg ADC_BIT_IT_ERR_RAW_STS
  *            @arg ADC_BIT_IT_DAT_OVW_RAW_STS
  *            @arg ADC_BIT_IT_FIFO_EMPTY_RAW_STS
  *            @arg ADC_BIT_IT_FIFO_OVER_RAW_STS
  *            @arg ADC_BIT_IT_FIFO_FULL_RAW_STS
  *            @arg ADC_BIT_IT_CHCV_END_RAW_STS
  *            @arg ADC_BIT_IT_CV_END_RAW_STS
  *            @arg ADC_BIT_IT_CVLIST_END_RAW_STS
  */
u32 ADC_GetRawISR(void)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	return adc->ADC_INTR_RAW_STS;
}

/**
  * @brief  Get the number of valid entries in ADC receive FIFO.
  * @param  None.
  * @retval  The number of valid entries in receive FIFO.
  */
u32 ADC_GetRxCount(void)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	return ADC_GET_FLR(adc->ADC_FLR);
}

/**
  * @brief  Get the last ADC used channel.
  * @param  None.
  * @retval  The last ADC used channel index.
  */
u32 ADC_GetLastChan(void)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	return ADC_GET_LAST_CH(adc->ADC_LAST_CH);
}

/**
  * @brief  Get comparison result of ADC channel.
  * @param  ADC_Channel: The channel index
  * @retval  The comparison result of specified ADC channel.
  */
u32 ADC_GetCompStatus(u8 ADC_Channel)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	u32 value = (adc->ADC_COMP_STS & ADC_MASK_COMP_STS_CH(ADC_Channel)) \
				>> ADC_SHIFT_COMP_STS_CH(ADC_Channel);

	return  value;
}

/**
  * @brief  Set ADC channel threshold and criteria for comparison.
  * @param  ADC_channel: can be a value of @ref ADC_Chn_Selection as following:
  *				@arg ADC_CH0
  *				@arg ADC_CH1
  *				@arg ADC_CH2
  *				@arg ADC_CH3
  *				@arg ADC_CH4
  *				@arg ADC_CH5
  *				@arg ADC_CH6
  *				@arg ADC_CH7
  *				@arg ADC_CH8
  *				@arg ADC_CH9
  * @param  CompThresH:  the higher threshold of channel for ADC automatic comparison
  * @param  CompThresH:  the lower threshold of channel for ADC automatic comparison
  * @param  CompCtrl:  This parameter can be a value of @ref ADC_Compare_Control_Definitions as following:
  *		 		@arg ADC_COMP_SMALLER_THAN_THL: less than the lower threshold
  *		 		@arg ADC_COMP_GREATER_THAN_THH: greater than the higher threshod
  *		 		@arg ADC_COMP_WITHIN_THL_AND_THH: between the lower and higher threshod
  *		 		@arg ADC_COMP_OUTSIDE_THL_AND_THH: out the range of the higher and lower threshod
  * @retval None
  */
void ADC_SetComp(u8 ADC_channel, u16 CompThresH, u16 CompThresL, u8 CompCtrl)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	u32 value;

	assert_param(IS_ADC_CHN_SEL(ADC_channel));
	assert_param(IS_ADC_VALID_COMP_TH(CompThresH));
	assert_param(IS_ADC_VALID_COMP_TH(CompThresL));
	assert_param(IS_ADC_COMP_CRITERIA(CompCtrl));

	/* Set ADC channel threshold for comparison */
	adc->ADC_COMP_TH_CHx[ADC_channel] =
		(u32)(ADC_COMP_TH_H_CHx(CompThresH) | ADC_COMP_TH_L_CHx(CompThresL));

	/* Set ADC channel comparison criteria */
	value = adc->ADC_COMP_CTRL;
	value &= ~ ADC_MASK_COMP_CTRL_CH(ADC_channel);
	value |= (CompCtrl << ADC_SHIFT_COMP_CTRL_CH(ADC_channel));
	adc->ADC_COMP_CTRL = value;

	/* Enable comparison interrupt */
	ADC_INTConfig(ADC_IT_COMP_CH_EN(ADC_channel), ENABLE);
	ADC_INTConfig(ADC_IT_COMPRE_CH_EN(ADC_channel), ENABLE);
}

/**
  * @brief  Reset the channel switch to default state.
  * @param  None
  * @retval  None
  */
void ADC_ResetCSwList(void)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	adc->ADC_RST_LIST = ADC_BIT_RST_LIST;
	adc->ADC_RST_LIST = 0;
}

/**
  * @brief  Determine ADC FIFO is readable or not.
  * @param  None.
  * @retval ADC FIFO is readable or not:
  *        - 0: Not readable
  *        - 1: Readable
  */
u32 ADC_Readable(void)
{
	u32 Status = ADC_GetStatus();
	u32 Readable = (((Status & ADC_BIT_FIFO_EMPTY) == 0) ? 1 : 0);

	return Readable;
}

/**
  * @brief  Read data from ADC receive FIFO .
  * @param  None
  * @retval  The conversion data.
  */
u16 ADC_Read(void)
{
	u16 value = (u16)ADC_GET_DATA_GLOBAL(ADC->ADC_DATA_GLOBAL);

	return value;
}

/**
  * @brief  Continuous Read data in auto mode.
  * @param  pBuf: pointer to buffer to keep sample data
  * @param  len: the number of sample data to be read
  * @retval  None.
  */
void ADC_ReceiveBuf(u16 *pBuf, u32 len)
{
	u32 i = 0;

	ADC_ClearFIFO();

	ADC_AutoCSwCmd(ENABLE);

	for (i = 0; i < len; i++) {
		while (ADC_Readable() == 0);
		pBuf[i] = ADC_Read();
	}
	ADC_AutoCSwCmd(DISABLE);
}

/**
  * @brief  Clear ADC FIFO.
  * @param  None
  * @retval  None
  */
void ADC_ClearFIFO(void)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	adc->ADC_CLR_FIFO = ADC_BIT_CLR_FIFO;
	while (adc->ADC_FLR != 0);
	adc->ADC_CLR_FIFO = 0;
}

/**
  * @brief Get ADC status.
  * @param  None
  * @retval  Current status,each bit of this value represents one status which is as follows:
  *
  *		bit 2 : ADC_BIT_FIFO_EMPTY  ADC FIFO Empty.
  *			- 0: FIFO in ADC is not empty
  *			- 1: FIFO in ADC is empty
  *
  *		bit 1 : ADC_BIT_FIFO_FULL_REAL  ADC FIFO real full flag.
  *			- 0: FIFO in ADC is not full real
  *			- 1: FIFO in ADC is full real
  *
  *		bit 0 : ADC_BIT_BUSY_STS  ADC busy Flag.
  *			- 0: The ADC is ready
  *			- 1: The ADC is busy
  */
u32 ADC_GetStatus(void)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	return adc->ADC_BUSY_STS;
}

/**
  * @brief  Control the ADC module to do a conversion. Used as a start-convert event which is controlled by software.
  * @param  NewState: can be one of the following value:
  *			@arg ENABLE: Enable the analog module and analog mux. And then start a new channel conversion.
  *			@arg DISABLE:  Disable the analog module and analog mux.
  * @retval  None.
  * @note  1. Every time this bit is set to 1, ADC module would switch to a new channel and do one conversion.
  *			    Every time a conversion is done, software MUST clear this bit manually.
  *		  2. Used in Sotfware Trigger Mode
  */
void ADC_SWTrigCmd(u32 NewState)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	u8 div = adc->ADC_CLK_DIV;
	u8 sync_time[7] = {2, 4, 8, 12, 16, 32, 64};

	if (NewState != DISABLE) {
		adc->ADC_SW_TRIG = ADC_BIT_SW_TRIG;
	} else {
		adc->ADC_SW_TRIG = 0;
	}

	/* Wait 2 clock to sync signal */
	DelayUs(sync_time[div]);
}

/**
  * @brief  Controls the automatic channel switch enabled or disabled.
  * @param  NewState: can be one of the following value:
  *		@arg ENABLE: Enable the automatic channel switch.
  *			When setting this bit, an automatic channel switch starts from the first channel in the channel switch list.
  *		@arg DISABLE:  Disable the automatic channel switch.
  *			If an automatic channel switch is in progess, writing 0 will terminate the automatic channel switch.
  * @retval  None.
  * @note  Used in Automatic Mode
  */
void ADC_AutoCSwCmd(u32 NewState)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	u8 div = adc->ADC_CLK_DIV;
	u8 sync_time[7] = {2, 4, 8, 12, 16, 32, 64};

	if (NewState != DISABLE) {
		adc->ADC_AUTO_CSW_CTRL = ADC_BIT_AUTO_CSW_EN;
	} else {
		adc->ADC_AUTO_CSW_CTRL = 0;
	}

	/* Wait 2 clock to sync signal */
	DelayUs(sync_time[div]);
}

/**
  * @brief	Initialize the trigger timer when in ADC Timer-Trigger Mode.
  * @param  Tim_Idx: The timer index would be used to make ADC module do a conversion.
  * @note   This para should be 0-7
  * @param  PeriodMs: Indicate the period of trigger timer.
  * @param  NewState: can be one of the following value:
  *			@arg ENABLE: Enable the ADC timer trigger mode.
  *			@arg DISABLE: Disable the ADC timer trigger mode.
  * @retval  None.
  * @note  Used in Timer-Trigger Mode
  */
void ADC_TimerTrigCmd(u8 Tim_Idx, u32 PeriodMs, u32 NewState)
{
	ADC_TypeDef	*adc = ADC;

	if (TrustZone_IsSecure()) {
		adc = ADC_S;
	}

	RTIM_TimeBaseInitTypeDef TIM_InitStruct;

	assert_param(IS_ADC_VALID_TIM(Tim_Idx));

	adc->ADC_EXT_TRIG_TIMER_SEL = Tim_Idx;

	if (NewState != DISABLE) {
		RTIM_TimeBaseStructInit(&TIM_InitStruct);
		TIM_InitStruct.TIM_Idx = Tim_Idx;
		TIM_InitStruct.TIM_Period = (PeriodMs * 32768) / 1000 / 2; //ms to tick

		RTIM_TimeBaseInit(TIMx[Tim_Idx], &TIM_InitStruct, TIMx_irq[Tim_Idx], (IRQ_FUN)NULL, (u32)NULL);
		RTIM_Cmd(TIMx[Tim_Idx], ENABLE);
	} else {
		RTIM_Cmd(TIMx[Tim_Idx], DISABLE);
	}
}

/**
  * @brief  Get voltage value in mV.
  * @param  chan_data: ADC conversion data.
  * @retval ADC voltage value in mV.
  */
s32 ADC_GetVoltage(u16 chan_data)
{
	u16 K_A, K_B, K_C;
	s64 ka, kb, kc;
	u8 EfuseBuf[6];
	u32 AdcCalAddr = 0x704;
	s32 ch_vol;

	for (u8 index = 0; index < 6; index++) {
		EFUSE_PMAP_READ8(0, AdcCalAddr + index, EfuseBuf + index, L25EOUTVOLTAGE);
	}
	K_A = EfuseBuf[1] << 8 | EfuseBuf[0];
	K_B = EfuseBuf[3] << 8 | EfuseBuf[2];
	K_C = EfuseBuf[5] << 8 | EfuseBuf[4];

	if (K_A == 0xFFFF) {
		K_A = 0xFF22;
	}

	if (K_B == 0xFFFF) {
		K_B = 0x6E39;
	}

	if (K_C == 0xFFFF) {
		K_C = 0xFD58;
	}

	ka = (K_A & BIT15) ? -(0x10000 - K_A) : (K_A & 0x7FFF);
	kb = (K_B & 0xFFFF);
	kc = (K_C & BIT15) ? -(0x10000 - K_C) : (K_C & 0x7FFF);

	ch_vol = (ka * chan_data * chan_data) / 67108864 + (kb * chan_data) / 32768 + kc / 64;

	return ch_vol;

}

/**
  * @brief Get internal R resistance of V33 channels(CH0~CH5) in divided mode.
  * @param none.
  * @retval Internal R resistance value in Kohm.
  */
u32 ADC_GetInterR(void)
{
	u32 AdcResAddr = 0x7EC;
	u8 r_offset;

	EFUSE_PMAP_READ8(0, AdcResAddr, &r_offset, L25EOUTVOLTAGE);

	return ((u32)r_offset + 400);
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
