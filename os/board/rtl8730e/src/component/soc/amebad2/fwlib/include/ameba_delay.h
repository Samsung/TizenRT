/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_DELAY_H_
#define _AMEBA_DELAY_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup DELAY
  * @brief DELAY driver modules
  * @{
  */

/** @addtogroup DELAY
  * @verbatim
  *****************************************************************************************
  * Delay
  *****************************************************************************************
  * 	- DelayUs
  * 	- DelayMs
  * 	- DelayNop
  *
  *****************************************************************************************
  * Sys Timer
  *****************************************************************************************
  * 	- TIM0 is used as systimer, so TIM0 can not be used for other purpose
  * 	- init when boot in rom code
  *	- resolution is 31us
  *	- you can get timer tick use SYSTIMER_TickGet, every tick is 31us
  *	- you can get passing time use SYSTIMER_GetPassTime in ms
  *
  *****************************************************************************************
  * @endverbatim
  */

/** @defgroup DELAY_Exported_Functions DELAY Exported Functions
  * @{
  */
_LONG_CALL_ void SYSTIMER_Init(void);
_LONG_CALL_ u32 SYSTIMER_TickGet(void);
_LONG_CALL_ u32 SYSTIMER_GetPassTick(u32 start);
_LONG_CALL_ u32 SYSTIMER_GetPassTime(u32 start);
_LONG_CALL_ void DelayUs(u32 us);
_LONG_CALL_ void DelayMs(u32 ms);
/** @} */

/** @} */
/** @} */
_LONG_CALL_ void DelayNop(u32 count);
#define HalDelayUs DelayUs

extern u32 RBSS_UDELAY_FLAG;
extern u32 RBSS_UDELAY_CLK;

#ifdef __cplusplus
}
#endif

#endif//_AMEBA_DELAY_H_