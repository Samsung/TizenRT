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
#if defined (ARM_CORE_CM4)
WDG_TypeDef *WDGDev = WDG2_DEV;
IRQn_Type WdgIrqNum = WDG2_IRQ;
#elif defined (ARM_CORE_CA32)
WDG_TypeDef *WDGDev = WDG4_DEV;
IRQn_Type WdgIrqNum = WDG4_IRQ;
#endif

/**
  * @}
  */

/** @defgroup MBED_WDG_Exported_Functions MBED_WDG Exported Functions
  * @{
  */

/**
 * @brief  Initialize the watchdog, including time and early interrupt settings.
 * @param  timeout_ms: Timeout value of watchdog timer in units of ms.
 * @retval none
 * @note  By default, watchdog will reset the whole system once timeout occurs.
 */
void watchdog_init(uint32_t timeout_ms)
{
	WDG_InitTypeDef WDG_InitStruct;

	WDG_StructInit(&WDG_InitStruct);
	WDG_InitStruct.Timeout = timeout_ms;
	/*set early int to 100ms if timeout is bigger than 100ms*/
	WDG_InitStruct.EICNT = timeout_ms > 100 ? 100 : timeout_ms / 2;
	WDG_InitStruct.EIMOD     = ENABLE;

	WDG_Init(WDGDev, &WDG_InitStruct);
}

/**
 * @brief  Enable the watchdog and it starts to count.
 * @param  none
 * @retval none
 */
void watchdog_start(void)
{
	WDG_Enable(WDGDev);
}

/**
 * @brief  Disable the watchdog and it stops counting.
 * @param  none
 * @retval none
 * @note  Once watchdog is enabled, it cannot be disabled by software.
 */
void watchdog_stop(void)
{
	DBG_8195A("Ameba-D2 not support watchdog_stop function!\n");
	assert_param(0);
}

/**
 * @brief  Refresh count of the watchdog in avoidance of WDT timeout.
 * @param  none
 * @retval none
 */
void watchdog_refresh(void)
{
	WDG_Refresh(WDGDev);
}

/**
 * @brief  Enable eraly interrupt and register a watchdog timer timeout interrupt handler.
 *        The interrupt handler will be called at a programmable time prior to watchdog timeout, for users to prepare for reset
 * @param  handler: WDT timeout interrupt callback function.
 * @param  id: WDT timeout interrupt callback parameter.
 * @retval none
 */
void watchdog_irq_init(wdt_irq_handler handler, uint32_t id)
{
	WDG_ClearINT(WDGDev, WDG_BIT_EIC);
	InterruptRegister((IRQ_FUN)handler, WdgIrqNum, (u32)id, 3);
	InterruptEn(WdgIrqNum, 3);
	WDG_INTConfig(WDGDev, WDG_BIT_EIE, ENABLE);
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