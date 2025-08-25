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
#include "board_pins.h"

/* KM0 without KM4 ON: 4.30mA */
/* KM0 CG without KM4 ON: 135uA */
/* DSLP(RTC, KEYSCAN OFF, TOUCH OFF): 2uA under power supply, but 7uA under current meter */
/* DSLP(RTC+KEYSCAN, TOUCH OFF): 3uA under power supply */
/* DSLP(RTC+KEYSCAN+TOUCH): 3.9uA under power supply (peak is 3.7mA) */
const PMAP_TypeDef pmap_func[] = {
	PWR_PIN_00,
	PWR_PIN_01,
	PWR_PIN_02,
	PWR_PIN_03,
	PWR_PIN_04,
	PWR_PIN_05,
	PWR_PIN_06,
	PWR_PIN_07,
	PWR_PIN_08,
	PWR_PIN_09,
	PWR_PIN_10,
	PWR_PIN_11,
	PWR_PIN_12,
	PWR_PIN_13,
	PWR_PIN_14,
	PWR_PIN_15,
	PWR_PIN_16,
	PWR_PIN_17,
	PWR_PIN_18,
	PWR_PIN_19,
	PWR_PIN_20,
	PWR_PIN_21,
	PWR_PIN_22,
	PWR_PIN_23,
	PWR_PIN_24,
	PWR_PIN_25,
	PWR_PIN_26,
	PWR_PIN_27,
	PWR_PIN_28,
	PWR_PIN_29,
	PWR_PIN_30,
	PWR_PIN_31,
	PWR_PIN_32,
	PWR_PIN_33,
	PWR_PIN_34,
	PWR_PIN_35,
	PWR_PIN_36,
	PWR_PIN_37,
	PWR_PIN_38,
	PWR_PIN_39,
	PWR_PIN_40,
	PWR_PIN_41,
	PWR_PIN_42,
	PWR_PIN_43,
	PWR_PIN_44,
	PWR_PIN_45,
	PWR_PIN_46,
	PWR_PIN_47,
	PWR_PIN_48,
	PWR_PIN_49,
	PWR_PIN_50,
	PWR_PIN_51,
	PWR_PIN_52,
	PWR_PIN_53,
	PWR_PIN_54,
	PWR_PIN_55,
	PWR_PIN_56,
	PWR_PIN_57,
	PWR_PIN_58,
	PWR_PIN_59,
	PWR_PIN_60,
	PWR_PIN_61,
	PWR_PIN_62,
	PWR_PIN_63,
	PWR_PIN_64,
	PWR_PIN_65,
	PWR_PIN_66,
	PWR_PIN_67,
	PWR_PIN_68,
	PWR_PIN_69,
	PWR_PIN_70,
	PWR_PIN_71,
};

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
