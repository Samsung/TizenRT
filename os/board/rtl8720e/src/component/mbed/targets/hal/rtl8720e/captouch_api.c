/** mbed Microcontroller Library
  ******************************************************************************
  * @file    captouch_api.c
  * @author
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for CAPTOUCH.
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
#include "captouch_api.h"
#include "PinNames.h"
#include "pinmap.h"

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @defgroup MBED_CAPTOUCH
 *  @brief    MBED_CAPTOUCH driver modules.
 *  @{
 */

/** @defgroup MBED_CAPTOUCH_Exported_Types MBED_CAPTOUCH Exported Types
  * @{
  */

static const PinMap PinMap_CTC[] = {
	{PB_5,			CapTouch_0,		PINMUX_FUNCTION_AUXIN},
	{PB_4,			CapTouch_1,		PINMUX_FUNCTION_AUXIN},
	{PB_3,			CapTouch_2,		PINMUX_FUNCTION_AUXIN},
	{PB_2,			CapTouch_3,		PINMUX_FUNCTION_AUXIN},
	{PB_1,			CapTouch_4,		PINMUX_FUNCTION_AUXIN},
	{PA_31,			CapTouch_5,		PINMUX_FUNCTION_AUXIN},
	{PA_30,			CapTouch_6,		PINMUX_FUNCTION_AUXIN},
	{PA_29,			CapTouch_7,		PINMUX_FUNCTION_AUXIN},
	{PA_28,			CapTouch_8,		PINMUX_FUNCTION_AUXIN},
	{NC,			NC,				0}
};

IRQn_Type CTOUCH_IrqNum = CTOUCH_IRQ;

typedef void (*ctc_irq_handler)(u8 ch);

/**
  * @}
  */

/** @defgroup MBED_CAPTOUCH_Exported_Functions MBED_CAPTOUCH Exported Functions
  * @{
  */

/**
  * @brief  Register captouch interrupt handler.
  * @param  obj: Captouch object defined in application software.
  * @retval 0
  */
static u32 captouch_irq_handler(void *data)
{
	captouch_t *obj = (captouch_t *)data;

	u8 ch;
	u32 IntStatus;
	ctc_irq_handler handler_press;
	ctc_irq_handler handler_release;

	IntStatus = CapTouch_GetISR(CAPTOUCH_DEV);
	CapTouch_INTClearPendingBit(CAPTOUCH_DEV, IntStatus);

	for (ch = 0; ch < CT_CHANNEL_NUM; ch++) {
		if (IntStatus & CT_CHX_PRESS_INT(ch)) {
			DBG_8195A("Key	%x press \n", ch);

			if (obj->irq_handler_press) {
				handler_press = (ctc_irq_handler)obj->irq_handler_press;
				handler_press(ch);
			}
		}

		if (IntStatus & CT_CHX_RELEASE_INT(ch)) {
			DBG_8195A("Key	%x release \n", ch);

			if (obj->irq_handler_release) {
				handler_release = (ctc_irq_handler)obj->irq_handler_release;
				handler_release(ch);
			}
		}
	}

	/* User do not need care the Noise interrupt */
	if (IntStatus & CT_BIT_OVER_P_NOISE_TH_INTR) {
		CapTouch_Cmd(CAPTOUCH_DEV, DISABLE);
		CapTouch_Cmd(CAPTOUCH_DEV, ENABLE);
	}

	return 0;

}

/**
  * @brief  Initialize the captouch device.
  * @param  obj: Captouch object defined in application software.
  * @retval none
  */
void captouch_init(captouch_t *obj)
{
	u32 ch = 0;

	CapTouch_InitTypeDef Touch_InitStruct;

	/* enable ADC function */
	RCC_PeriphClockCmd(APBPeriph_CTC, APBPeriph_CTC_CLOCK, ENABLE);
	RCC_PeriphClockCmd(APBPeriph_ADC, APBPeriph_ADC_CLOCK, ENABLE);

	/* Load HAL initial data structure default value */
	Touch_InitStruct.CT_DebounceEn = 1;
	Touch_InitStruct.CT_SampleCnt = 6;
	Touch_InitStruct.CT_ScanInterval = 60;
	Touch_InitStruct.CT_ETCStep = 1;
	Touch_InitStruct.CT_ETCFactor = 4;
	Touch_InitStruct.CT_ETCScanInterval = 3;

	for (ch = 0; ch < CT_CHANNEL_NUM; ch++) {
		if (obj->CT_Channel[ch].CT_CHEnable == ENABLE) {
			Touch_InitStruct.CT_Channel[ch].CT_CHEnable = obj->CT_Channel[ch].CT_CHEnable;
			Touch_InitStruct.CT_Channel[ch].CT_DiffThrehold = obj->CT_Channel[ch].CT_DiffThrehold;
			Touch_InitStruct.CT_Channel[ch].CT_MbiasCurrent = obj->CT_Channel[ch].CT_MbiasCurrent;
			Touch_InitStruct.CT_Channel[ch].CT_ETCNNoiseThr = obj->CT_Channel[ch].CT_ETCNNoiseThr;
			Touch_InitStruct.CT_Channel[ch].CT_ETCPNoiseThr = obj->CT_Channel[ch].CT_ETCPNoiseThr;

			pinmap_pinout(PinMap_CTC[ch].pin, PinMap_CTC);
			PAD_InputCtrl(PinMap_CTC[ch].pin, DISABLE);
		}
	}

	CapTouch_Init(CAPTOUCH_DEV, &Touch_InitStruct);
	CapTouch_Cmd(CAPTOUCH_DEV, ENABLE);
	CapTouch_INTConfig(CAPTOUCH_DEV, CT_ALL_INT_EN, ENABLE);

	/* Register interrupt Callback function */
	InterruptRegister((IRQ_FUN) captouch_irq_handler, CTOUCH_IrqNum, (u32)obj, 5);
	InterruptEn(CTOUCH_IrqNum, 5);
}


/**
  * @brief  Deinitialize the captouch device.
  * @param  obj: Captouch object defined in application software.
  * @retval none
  */
void captouch_deinit(captouch_t *obj)
{
	/* To avoid gcc warnings */
	(void) obj;

	u32 ch = 0;

	InterruptDis(CTOUCH_IrqNum);
	InterruptUnRegister(CTOUCH_IrqNum);
	CapTouch_INTConfig(CAPTOUCH_DEV, CT_ALL_INT_EN, DISABLE);

	for (ch = 0; ch < CT_CHANNEL_NUM; ch++) {
		if (obj->CT_Channel[ch].CT_CHEnable == ENABLE) {
			PAD_InputCtrl(PinMap_CTC[ch].pin, ENABLE);
		}
	}

	CapTouch_Cmd(CAPTOUCH_DEV, DISABLE);
}

/**
  * @brief  Enable specified captouch channel.
  * @param  obj: Captouch object defined in application software.
  * @param  Channel: Specified captouch channel index, which can be 0~3.
  * @retval none
  */
void captouch_ch_enable(captouch_t *obj, u8 Channel)
{
	/* To avoid gcc warnings */
	(void) obj;

	CapTouch_ChCmd(CAPTOUCH_DEV, Channel, ENABLE);
}

/**
  * @brief  Disable specified captouch channel.
  * @param  obj: Captouch object defined in application software.
  * @param  Channel: Specified captouch channel index, which can be 0~3.
  * @retval none
  */
void captouch_ch_disable(captouch_t *obj, u8 Channel)
{
	/* To avoid gcc warnings */
	(void) obj;

	CapTouch_ChCmd(CAPTOUCH_DEV, Channel, DISABLE);
}

/**
  * @brief  Set captouch scan interval.
  * @param  obj: Captouch object defined in application software.
  * @param  Interval: Captouch scan interval in units of ms.
  * @retval none
  */
void captouch_set_scan_interval(captouch_t *obj, u32 Interval)
{
	/* To avoid gcc warnings */
	(void) obj;

	CapTouch_SetScanInterval(CAPTOUCH_DEV, Interval);
}

/**
  * @brief  Read baseline data from specified captouch channel.
  * @param  obj: Captouch object defined in application software.
  * @param  Channel: Specified captouch channel index, which can be 0~3.
  * @return Baseline data.
  */
u32 captouch_get_ch_baseline(captouch_t *obj, u8 Channel)
{
	/* To avoid gcc warnings */
	(void) obj;

	return CapTouch_GetChBaseline(CAPTOUCH_DEV, Channel);
}

/**
  * @brief  Read average data from specified captouch channel.
  * @param  obj: Captouch object defined in application software.
  * @param  Channel: Specified captouch channel index, which can be 0~3.
  * @return Average data.
  */
u32 captouch_get_ch_data(captouch_t *obj, u8 Channel)
{
	/* To avoid gcc warnings */
	(void) obj;

	return CapTouch_GetChAveData(CAPTOUCH_DEV, Channel);
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
