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

/* KM0 without KM4 ON: 4.30mA */
/* KM0 CG without KM4 ON: 135uA */
/* DSLP(RTC, KEYSCAN OFF, TOUCH OFF): 2uA under power supply, but 7uA under current meter */
/* DSLP(RTC+KEYSCAN, TOUCH OFF): 3uA under power supply */
/* DSLP(RTC+KEYSCAN+TOUCH): 3.9uA under power supply (peak is 3.7mA) */
const PMAP_TypeDef pmap_func[] = {
//Pin Name		Func PU/PD					Slp PU/PD
	{_PA_0,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_1,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_2,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_3,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_4,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_5,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_6,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_7,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_8,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_9,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PA_10,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PA_11,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PA_12,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_13,			GPIO_PuPd_UP,			GPIO_PuPd_UP},			 //
	{_PA_14,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_15,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_16,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 	// QA board need pull up
	{_PA_17,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_18,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PA_19,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_20,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_21,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_22,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PA_23,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PA_24,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PA_25,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_26,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_27,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_28,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PA_29,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PA_30,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PA_31,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PB_0,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PB_1,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PB_2,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PB_3,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PB_4,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PB_5,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PB_6,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PB_7,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PB_8,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PB_9,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PB_10,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PB_11,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PB_12,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PB_13,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PB_14,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PB_15,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PB_16,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PB_17,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PB_18,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PB_19,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PB_20,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},			 //
	{_PB_21,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 // boot sel
	{_PB_22,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PB_23,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PB_24,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 // PB24 internal pull high(80k)
	{_PB_25,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PB_26,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PB_27,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PB_28,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PB_29,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PB_30,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PB_31,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //
	{_PC_0,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PC_1,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PC_2,			GPIO_PuPd_UP,				GPIO_PuPd_DOWN},			 //flash clk pull down for PG or 50uA
	{_PC_3,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PC_4,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PC_5,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PC_6,			GPIO_PuPd_UP,				GPIO_PuPd_UP},			 //
	{_PNC,			GPIO_PuPd_KEEP,				GPIO_PuPd_KEEP},			 //table end
};
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
