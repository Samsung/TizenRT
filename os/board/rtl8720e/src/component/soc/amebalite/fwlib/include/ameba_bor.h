/**
  ******************************************************************************
  * @file    ameba_bor.h
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the functions prototypes for the bor.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _AMEBA_BOR_H_
#define _AMEBA_BOR_H_

/** @addtogroup BOR
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * functions:
  * 	- set bor mode
  * 	- bor rising and falling threshold set for reset or interrupt mode
  * 	- bor debounce timer set
  * 	- bor interrupt clear
  *
  *****************************************************************************************
  * How to use BOR
  *****************************************************************************************
  *      To use the BOR mode, the following steps are mandatory:
  *
  *      1. Choose bor mode using the follwoing functions.
  *			BOR_ModeSet(u32 Option);
  *
  *      2. If Choose bor Interrupt mode, register bor Interrupt handler using the follwoing functions.
  *			InterruptRegister() : register bor Interrupt handler
  *			InterruptEn() : enable bor Interrupt
  *
  *****************************************************************************************
  * @endverbatim
  */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup BOR
  * @brief BOR driver modules
  * @{
  */

/* Exported constants ------------------------------------------------------------*/
/** @defgroup BOR_Exported_Constants BOR Exported Constants
  * @{
  */

/** @defgroup BOR_Mode
  * @{
  */
#define	BOR_RESET							((u32)0x00000000)
#define	BOR_INTR							((u32)0x00000001)
/**
  * @}
  */

/** @defgroup  BOR_Debounce_Threshold
  * @{
  */
#define IS_BOR_DBNC_THRES(DBNC)    ((DBNC) <= 0x7)
/**
  * @}
  */

/** @defgroup BOR_Threshold_RISE
  * @{
  */
#define IS_BOR_TH_RISE(TYPE)	((TYPE) <= 0x1F)
/**
  * @}
  */

/** @defgroup BOR_Threshold_FALL
  * @{
  */
#define IS_BOR_TH_FALL(TYPE)		((TYPE) <= 0x1F)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup BOR_Exported_Functions BOR Exported Functions
  * @{
  */

VOID BOR_ThresholdSet(u32 Thres_Fall, u32 Thres_Rise);
void BOR_Enable(u32 newstatus);
VOID BOR_DbncSet(u32 Dbnc_Value);
VOID BOR_ClearINT(void);
VOID BOR_ModeSet(u32 Option);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif //_AMEBA_BOR_H_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/

