/** mbed Microcontroller Library
  ******************************************************************************
  * @file    analogin_api.c
  * @author
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for ADC.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "objects.h"
#include "PinNames.h"
#include "analogin_api.h"
#include "pinmap.h"

/** @defgroup Ameba_Mbed_API
  * @{
  */

/** @defgroup MBED_ADC
 *  @brief    MBED_ADC driver modules.
 *  @{
 */

/* group: MBED_ADC Exported Types */
/* Since all the members in the group are static which will not be extracted to the doxygen doc,
  original comment has been deleted to avoid there is nothing displayed under the group. */

static const PinMap PinMap_ADC[] = {
	{AD_0,			ADC_CH0,		0},
	{AD_1,			ADC_CH1,		0},
	{AD_2,			ADC_CH2,		0},
	{AD_3,			ADC_CH3,		0},
	{AD_4,			ADC_CH4,		0},
	{AD_5,			ADC_CH5,		0},
	{VBAT_MEAS,		ADC_CH6,		0},
	{NC,			NC,				0}
};
/* end of group  */

/** @defgroup MBED_ADC_Exported_Functions MBED_ADC Exported Functions
  * @{
  */

/**
  * @brief  Initialize the ADC device, including clock, function and ADC registers.
  * @param  obj: ADC object defined in application software.
  * @param  pin: ADC PinName according to pinmux spec.
  * @retval none
  */
void analogin_init(analogin_t *obj, PinName pin)
{
	ADC_InitTypeDef ADC_InitStruct;
	uint32_t adc_idx;

	adc_idx = pinmap_peripheral(pin, PinMap_ADC);
	DBG_8195A("analogin_init [%x:%x ]\n", pin, adc_idx);
	assert_param(adc_idx != NC);

	/* Enable ADC clock and function */
	RCC_PeriphClockCmd(APBPeriph_CTC, APBPeriph_CTC_CLOCK, ENABLE);
	RCC_PeriphClockCmd(APBPeriph_ADC, APBPeriph_ADC_CLOCK, ENABLE);

	/* Set ADC channel Number and channel Pin */
	obj->adc_idx = adc_idx;
	obj->adc_pin = pin;

	if (obj->adc_pin != VBAT_MEAS) {
		/* Initialize ADC pin */
		Pinmux_Config(obj->adc_pin, PINMUX_FUNCTION_CAPTOUCH);
		PAD_PullCtrl(obj->adc_pin, PullNone);
		PAD_SleepPullCtrl(obj->adc_pin, PullNone);// avoid immediate wakeup once gated
		/* Disable digital path input */
		PAD_InputCtrl(obj->adc_pin, DISABLE);
	}

	/* Initialize ADC */
	ADC_StructInit(&ADC_InitStruct);
	ADC_InitStruct.ADC_CvlistLen = 0;
	ADC_Init(&ADC_InitStruct);

	ADC_Cmd(ENABLE);
}

/**
  * @brief  Read data from the specified ADC channel FIFO.
  * @param  obj: ADC object defined in application software.
  * @return ADC channel data(float).
  */
float analogin_read(analogin_t *obj)
{
	float value;
	uint32_t AnalogDatFull = 0xFFF;
	uint8_t  ChIdx = obj->adc_idx;
	uint32_t data;

	/* Set channel index into channel switch list*/
	ADC->ADC_CHSW_LIST_0 = ChIdx;

	/* Clear FIFO */
	ADC_ClearFIFO();

	/* SW trigger to sample */
	ADC_SWTrigCmd(ENABLE);
	while (ADC_Readable() == 0);
	ADC_SWTrigCmd(DISABLE);

	data = ADC_Read();
	value = (float)(ADC_GET_DATA_GLOBAL(data)) / (float)(AnalogDatFull);

	return (float)value;
}

/**
  * @brief  Read data from the specified ADC channel FIFO.
  * @param  obj: ADC object defined in application software.
  * @return 16b ADC channel data(int).
  */
uint16_t analogin_read_u16(analogin_t *obj)
{
	uint8_t  ChIdx = obj->adc_idx;
	uint32_t data;

	/* Set channel index into channel switch list*/
	ADC->ADC_CHSW_LIST_0 = ChIdx;

	/* Clear FIFO */
	ADC_ClearFIFO();

	/* SW trigger to sample */
	ADC_SWTrigCmd(ENABLE);
	while (ADC_Readable() == 0);
	ADC_SWTrigCmd(DISABLE);

	data = ADC_Read();

	return (uint16_t)ADC_GET_DATA_GLOBAL(data);
}

/**
  * @brief  Deinitialize the ADC device, including clock, function and ADC registers.
  * @param  obj: ADC object defined in application software.
  * @retval none
  */
void  analogin_deinit(analogin_t *obj)
{
	/* Enable digital path input */
	PAD_InputCtrl(obj->adc_pin, ENABLE);

	/* Clear ADC Status */
	ADC_INTClear();

	/* Disable ADC  */
	ADC_Cmd(DISABLE);
}

/**
  * @brief Get channel voltage in mV according to conversion data from normal channel(0-5 & 7-9).
  * @param adc_data: ADC conversion data from normal channel.
  * @return Normal channel voltage in mV.
  */
uint32_t analogin_voltage_norm(uint32_t adc_data)
{
	return ADC_GetVoltage(adc_data);
}

/**
  * @brief Get channel voltage in mV according to conversion data from VBAT channel(6).
  * @param adc_data: ADC conversion data from VBAT channel.
  * @return VBAT channel voltage in mV.
  */
uint32_t analogin_voltage_vbat(uint32_t adc_data)
{
	return ADC_GetVBATVoltage(adc_data);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
