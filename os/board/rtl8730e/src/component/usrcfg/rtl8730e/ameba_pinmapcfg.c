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
	//Pin Name		Func PU/PD			Slp PU/PD
	{_PA_0,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PA_1,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PA_2,			GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA pull up 10k
	{_PA_3,			GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA pull up 10k
	{_PA_4,			GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA pull up 4.7k
	{_PA_5,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// SDA pull up 4.7k & pull down 9.1k
	{_PA_6,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PA_7,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PA_8,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PA_9,			GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA pull up 10k
	{_PA_10,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA pull up 10k
	{_PA_11,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PA_12,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PA_13,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// RTK SWD_DATA
	{_PA_14,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// RTK SWD_CLK
	{_PA_15,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// SDA pull down 4.7k
	{_PA_16,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL},		// RTK QA board need pull up
	{_PA_17,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// NA
	{_PA_18,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// NA
	{_PA_19,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// NA
	{_PA_20,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// NA
	{_PA_21,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// NA
	{_PA_22,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL},		// SDA AMP-RESET
	{_PA_23,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// SDA pull down 4.7k
	{_PA_24,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA pull up 100k
	{_PA_25,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA pull up 10k
	{_PA_26,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PA_27,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PA_28,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA pull up 4.7k
	{_PA_29,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA pull up 4.7k
	{_PA_30,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// NA
	{_PA_31,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// NA
	{_PB_0,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PB_1,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PB_2,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// NA
	{_PB_3,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// SDA NPU SPI0_MISO
	{_PB_4,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// SDA NPU SPI0_MOSI
	{_PB_5,			GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA NPU SPI0_CS pull up 4.7k
	{_PB_6,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// SDA NPU SPI0_CLK
	{_PB_7,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PB_8,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PB_9,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PB_10,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL},		// SDA AMP-I2S
	{_PB_11,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA AILite+W LCD PWN
	{_PB_12,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PB_13,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PB_14,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PB_15,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PB_16,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PB_17,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// RTL8730ELH
	{_PB_18,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// RTL8730ELH
	{_PB_19,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// NA
	{_PB_20,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// SDA pull down 10k
	{_PB_21,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL},		// SDA AMP-I2S
	{_PB_22,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA pull up 10k; RTK TM_DIS
	{_PB_23,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// RTK UART_LOG_RXD
	{_PB_24,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// RTK UART_LOG_TXD internal pull up 80k
	{_PB_25,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// SDA Ext-Flash SPI1_CLK
	{_PB_26,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA Ext-Flash SPI1_CS pull up 4.7k
	{_PB_27,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA Ext-Flash SPI1_MISO
	{_PB_28,		GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA Ext-Flash SPI0_MOSI
	{_PB_29,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL},		// SDA pull up 4.7k
	{_PB_30,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL},		// SDA pull up 4.7k
	{_PB_31,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN},		// SDA pull down 4.7k
	{_PC_0,			GPIO_PuPd_UP,			GPIO_PuPd_UP},			// SDA pull up 4.7k
	{_PC_1,			GPIO_PuPd_UP,			GPIO_PuPd_UP},			// RTK Int-Flash SPI_FLASH_DATA
	{_PC_2,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN},		// RTK Int-Flash SPI_FLASH_CLK pull down for PG or 50uA
	{_PC_3,			GPIO_PuPd_UP,			GPIO_PuPd_UP},			// RTK Int-Flash SPI_FLASH_DATA
	{_PC_4,			GPIO_PuPd_UP,			GPIO_PuPd_UP},			// RTK Int-Flash SPI_FLASH_DATA
	{_PC_5,			GPIO_PuPd_UP,			GPIO_PuPd_UP},			// RTK Int-Flash SPI_FLASH_DATA
	{_PC_6,			GPIO_PuPd_UP,			GPIO_PuPd_UP},			// RTK Int-Flash SPI_FLASH_CS
	{_PNC,			GPIO_PuPd_KEEP,			GPIO_PuPd_KEEP},		// table end
};
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
