/**
  ******************************************************************************
  * @file    rtl8721dlp_pinmapcfg.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of pin control:
  *           - pinmux
  *           - active pad pull up & pull down
  *           - sleep pad pull up & pull down
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
#include <board_pins.h>

/* KM0 without KM4 ON: 4.30mA */
/* KM0 CG without KM4 ON: 135uA */
/* DSLP(RTC, KEYSCAN OFF, TOUCH OFF): 2uA under power supply, but 7uA under current meter */
/* DSLP(RTC+KEYSCAN, TOUCH OFF): 3uA under power supply */
/* DSLP(RTC+KEYSCAN+TOUCH): 3.9uA under power supply (peak is 3.7mA) */

PMAP_TypeDef *pmap_func  = pin_table;
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
