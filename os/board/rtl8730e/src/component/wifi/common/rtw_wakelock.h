/**
  ******************************************************************************
  * @file    rtw_wakelock.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */


#ifndef __RTW_WAKELOCK_H_
#define __RTW_WAKELOCK_H_

/**
 * @brief  This function indicates that the WLAN needs to stay on which means cannot go into power saving mode.
 * @return  None
 * @note
 */
void	rtw_acquire_wakelock(void);

/**
 * @brief  This function indicates that the WLAN does not need to stay on which means can go into power saving mode.
 * @return  None
 * @note
 */
void	rtw_release_wakelock(void);

void rtw_wakelock_timeout(uint32_t timeout);

#endif
