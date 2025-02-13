/** mbed Microcontroller Library
  ******************************************************************************
  * @file    wdt_api.c
  * @author
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for WDG.
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
#include "ameba_soc.h"
#include "wdt_api.h"
#include "cmsis.h"

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @defgroup MBED_WDG
 *  @brief    MBED_WDG driver modules.
 *  @{
 */

/** @defgroup MBED_WDG_Exported_Types MBED_WDG Exported Types
  * @{
  */
WDG_TypeDef *WDGArray[AMEBASMART_WDGMAX] = {WDG2_DEV, WDG4_DEV};
IRQn_Type WdgIrqNum[AMEBASMART_WDGMAX] = {WDG2_IRQ, WDG4_IRQ};

/**
  * @}
  */

/** @defgroup MBED_WDG_Exported_Functions MBED_WDG Exported Functions
  * @{
  */



void WDG_Init(WDG_TypeDef *WDG, WDG_InitTypeDef *WDG_InitStruct)
{
	u32 prescaler = 0;

	assert_param(IS_WDG_ALL_PERIPH(WDG));

	if (IS_IWDG_PERIPH(WDG)) {
		prescaler = 0x63;
	} else {
		prescaler = 0x5D;

	}

	WDG_Wait_Busy(WDG);

	/*Enable Register access*/
	WDG->WDG_MKEYR = WDG_ACCESS_EN;

	if (WDG_InitStruct->EIMOD) {
		WDG->WDG_CR = WDG_BIT_EIE | WDG_EICNT(WDG_InitStruct->EICNT);
	} else {
		WDG->WDG_CR = 0;
	}

	u16 time = WDG_InitStruct->Timeout/1000*348;

	WDG->WDG_RLR = WDG_PRER(prescaler) | WDG_RELOAD(time);
	WDG->WDG_WINR = WDG_InitStruct->Window;

	/*Disable Register access*/
	WDG->WDG_MKEYR = 0xFFFF;

}

/**
 * @brief  Initialize the watchdog, including time and early interrupt settings.
 * @param  WDG_id which WDT to use
 * @param  timeout_ms: Timeout value of watchdog timer in units of ms.
 * @retval none
 * @note  By default, watchdog will reset the whole system once timeout occurs.
 */
int watchdog_init(int WDG_id, uint32_t timeout_ms)
{
	if (WDG_id < 0 || WDG_id >= AMEBASMART_WDGMAX) {
		return -1;
	}
	WDG_InitTypeDef WDG_InitStruct;

	WDG_StructInit(&WDG_InitStruct);
	WDG_InitStruct.Timeout = timeout_ms;
	/*set early int to 100ms if timeout is bigger than 100ms*/
	WDG_InitStruct.EICNT = timeout_ms > 100 ? 100 : timeout_ms / 2;
	WDG_InitStruct.EIMOD     = ENABLE;

	WDG_Init(WDGArray[WDG_id], &WDG_InitStruct);
	return OK;
}

/**
 * @brief  Enable the watchdog and it starts to count.
 * @param  WDG_id which WDT to use
 * @retval none
 */
void watchdog_start(int WDG_id)
{
	WDG_Enable(WDGArray[WDG_id]);
}

/**
 * @brief  Disable the watchdog and it stops counting.
 * @param  none
 * @retval none
 * @note  Once watchdog is enabled, it cannot be disabled by software.
 */
void watchdog_stop(void)
{
	dbg("Ameba-D2 not support watchdog_stop function!\n");
}

/**
 * @brief  Refresh count of the watchdog in avoidance of WDT timeout.
 * @param  WDG_id which WDT to use
 * @retval none
 */
void watchdog_refresh(int WDG_id)
{
	WDG_Refresh(WDGArray[WDG_id]);
}

/**
 * @brief  Enable eraly interrupt and register a watchdog timer timeout interrupt handler.
 *        The interrupt handler will be called at a programmable time prior to watchdog timeout, for users to prepare for reset
 * @param  WDG_id which WDT to use
 * @param  handler: WDT timeout interrupt callback function.
 * @param  id: WDT timeout interrupt callback parameter.
 * @retval none
 */
void watchdog_irq_init(int WDG_id, wdt_irq_handler handler, uint32_t id)
{
	WDG_ClearINT(WDGArray[WDG_id], WDG_BIT_EIC);
	InterruptRegister((IRQ_FUN)handler, WdgIrqNum[WDG_id], (u32)id, INT_PRI_HIGH);
	InterruptEn(WdgIrqNum[WDG_id], INT_PRI_HIGH);
	WDG_INTConfig(WDGArray[WDG_id], WDG_BIT_EIE, ENABLE);
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