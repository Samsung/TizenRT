/**
  ******************************************************************************
  * @file    rtl8721dlp_intfcfg.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities:
  *           - uart mbed function config
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"


#if defined(CONFIG_FTL_ENABLED)
#define FTL_MEM_CUSTEM		1
#if FTL_MEM_CUSTEM == 0
#error "You should allocate flash sectors to for FTL physical map as following, then set FTL_MEM_CUSTEM to 1. For more information, Please refer to Application Note, FTL chapter. "
#else
							
#endif
#endif

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
