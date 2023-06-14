/**
  ******************************************************************************
  * @file    rtl8721d_backup_reg.h
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the 16bytes backup registers
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _RTL8721D_BACKUP_REG_H_
#define _RTL8721D_BACKUP_REG_H_

/** @addtogroup AmebaD_Platform
  * @{
  */

/** @defgroup BKUP_REG
  * @brief BKUP_REG driver modules
  * @{
  */

/** @addtogroup BKUP_REG
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * backup register size:
  *		-16bytes (4 dwords)
  *
  * usage:
  *		- user can use this registers to save some data before reset happens
  *
  * backup register can not be reset by following functions:
  *		- cpu reset
  *		- system reset
  *		- soc sleep mode
  *
  * backup register will be reset by following functions:
  *		- soc deep sleep mode
  *		- soc power down reset
  *		- soc power off
  *
  * system defined bits (other bits are reserved for user):
  *		- dword0[0]: system reset
  *		- dword0[1]: watchdog reset
  *		- dword0[2]: BOR2 HW temp bit
  *		- dword0[3]: this is SW set bit before reboot, for uart download
  *		- dword0[4]: this is SW set bit before reboot, for uart download debug
  *		- dword0[5]: this is SW set bit before reboot, for rtc init indication, not used now
  *		- dword0[6]: BOR2 HW temp bit
  *		- dword0[7]: 1: enable bor2 detection;  0: disable
  *
  *****************************************************************************************
  * how to use
  *****************************************************************************************
  *		BKUP_Write: write a dword backup register
  *		BKUP_Read: read a dword backup register
  *		BKUP_Set: set 1 to some bits of backup register
  *		BKUP_Clear: set 0 to some bits of backup register
  *****************************************************************************************
  * @endverbatim
  */

/**************************************************************************//**
 * @defgroup AMEBAD_BACKUP_REG
 * @{
 * @brief AMEBAD_BACKUP_REG Register Declaration
 * @note 16 Bytes total
 *****************************************************************************/
typedef struct {
	__IO uint32_t DWORD[4]; /* 0xE0 */
} BACKUP_REG_TypeDef;
/** @} */

/* Exported constants --------------------------------------------------------*/

/** @defgroup BKUP_REG_Exported_Constants BKUP_REG Exported Constants
  * @{
  */

/** @defgroup BKUP_REG_Index
  * @{
  */
#define BKUP_REG0				((u32)0x00000000)	/*!< Bit[10,9] is used by system */
#define BKUP_REG1				((u32)0x00000001)	/*!< all bits can be used by user */
#define BKUP_REG2				((u32)0x00000002)	/*!< all bits can be used by user */
#define BKUP_REG3				((u32)0x00000003)	/*!< all bits can be used by user */
#define IS_BKUP_REG(IDX)	(((IDX) == BKUP_REG0) || \
							((IDX) == BKUP_REG1) ||\
							((IDX) == BKUP_REG2) ||\
							((IDX) == BKUP_REG3))
/**
  * @}
  */
/**
  * @}
  */


/** @defgroup BKUP_REG_Exported_Functions BKUP_REG Exported Functions
  * @{
  */
_LONG_CALL_ void BKUP_Write(u32 DwordIdx, u32 WriteVal);
_LONG_CALL_ u32 BKUP_Read(u32 DwordIdx);
_LONG_CALL_ void BKUP_Set(u32 DwordIdx, u32 BitMask);
_LONG_CALL_ void BKUP_Clear(u32 DwordIdx, u32 BitMask);
/**
  * @}
  */

/* Registers Definitions --------------------------------------------------------*/
/**************************************************************************//**
 * @defgroup BKUP_REG_Register_Definitions BKUP_REG Register Definitions
 * @{
 *****************************************************************************/

/**************************************************************************//**
 * @defgroup BKUP_REG_WDORD0 REG_LP_BOOT_REASON0
 * @{
 *****************************************************************************/
#define BKUP_BIT_UARTBURN_DEBUG				BIT(10)	/*!<  this is SW set bit before reboot, for uart download debug */
#define BKUP_BIT_UARTBURN_BOOT				BIT(9)	/*!<  this is SW set bit before reboot, for uart download */
/** @} */
/** @} */

/**
  * @}
  */

/**
  * @}
  */

#endif //_RTL8710B_BACKUP_REG_H_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
