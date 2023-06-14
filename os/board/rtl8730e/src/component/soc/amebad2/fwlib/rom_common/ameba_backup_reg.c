/**
  ******************************************************************************
  * @file    rtl8721d_backup_reg.c
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

#include "ameba_soc.h"

/**
  * @brief  backup register dwrod write
  * @param  DwordIdx: dword index of backup register
  *   This parameter can be any of the following values:
  *		 @arg BKUP_REG0~BKUP_REG3
  * @param  WriteVal: backup data write to backup register
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void BKUP_Write(u32 DwordIdx, u32 WriteVal)
{
	assert_param(IS_BKUP_REG(DwordIdx));

	BACKUP_REG->DWORD[DwordIdx] = WriteVal;
}

/**
  * @brief  backup register dwrod read
  * @param  DwordIdx: dword index of backup register
  *   This parameter can be any of the following values:
  *		 @arg BKUP_REG0~BKUP_REG3
  * @retval backup data read from backup register
  */
__weak HAL_ROM_TEXT_SECTION
u32 BKUP_Read(u32 DwordIdx)
{
	assert_param(IS_BKUP_REG(DwordIdx));

	return BACKUP_REG->DWORD[DwordIdx];
}

/**
  * @brief  set some bits of backup register
  * @param  DwordIdx: dword index of backup register
  *   This parameter can be any of the following values:
  *		 @arg BKUP_REG0~BKUP_REG3
  * @param  BitMask: bitmask to be set
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void BKUP_Set(u32 DwordIdx, u32 BitMask)
{
	assert_param(IS_BKUP_REG(DwordIdx));

	BACKUP_REG->DWORD[DwordIdx] |= BitMask;
}

/**
  * @brief  clear some bits of backup register
  * @param  DwordIdx: dword index of backup register
  *   This parameter can be any of the following values:
  *		 @arg BKUP_REG0~BKUP_REG3
  * @param  BitMask: bitmask to be clear
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void BKUP_Clear(u32 DwordIdx, u32 BitMask)
{
	assert_param(IS_BKUP_REG(DwordIdx));

	BACKUP_REG->DWORD[DwordIdx] &= ~BitMask;
}


/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
