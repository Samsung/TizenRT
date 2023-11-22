/**
  ******************************************************************************
  * @file    rtl8721dlp_sleepcfg.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of pin control:
  *           - sleep wakeup event
  *           - sleep power management
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

#ifdef CONFIG_SOC_PS_MODULE

/*wakeup attribute can be set to WAKEUP_NULL/WAKEUP_LP/WAKEUP_NP/WAKEUP_AP*/
WakeEvent_TypeDef sleep_wevent_config[] = {
//  Module									wakeup
	{WAKE_SRC_nFIQOUT1_OR_nIRQOUT1,			WAKEUP_NULL},
	{WAKE_SRC_nFIQOUT0_OR_nIRQOUT0,			WAKEUP_NULL},
	{WAKE_SRC_BT_WAKE_HOST,					WAKEUP_NULL},
	{WAKE_SRC_AON_WAKEPIN,					WAKEUP_LP},
	{WAKE_SRC_WDG4,							WAKEUP_NULL},
	{WAKE_SRC_WDG3,							WAKEUP_NULL},
	{WAKE_SRC_WDG2,							WAKEUP_NULL},
	{WAKE_SRC_WDG1,							WAKEUP_NULL},
	{WAKE_SRC_UART2,						WAKEUP_NULL},
	{WAKE_SRC_UART1,						WAKEUP_NULL},
	{WAKE_SRC_UART0,						WAKEUP_NULL},
	{WAKE_SRC_SPI1,							WAKEUP_NULL},
	{WAKE_SRC_SPI0,							WAKEUP_NULL},
	{WAKE_SRC_USB_OTG,						WAKEUP_NULL},
	{WAKE_SRC_IPC_AP,						WAKEUP_AP},
	{WAKE_SRC_IPC_NP,						WAKEUP_NP},
	{WAKE_SRC_VADBT_OR_VADPC,				WAKEUP_NULL},
	{WAKE_SRC_PWR_DOWN,						WAKEUP_LP},
	{WAKE_SRC_BOR,							WAKEUP_NULL},
	{WAKE_SRC_ADC_COMP,						WAKEUP_NULL},
	{WAKE_SRC_ADC,							WAKEUP_NULL},
	{WAKE_SRC_CTOUCH,						WAKEUP_NULL},
	{WAKE_SRC_RTC,							WAKEUP_NULL},
	{WAKE_SRC_GPIOC,						WAKEUP_NULL},
	{WAKE_SRC_GPIOB,						WAKEUP_NULL},
	{WAKE_SRC_GPIOA,						WAKEUP_NULL},
	{WAKE_SRC_UART_LOG,						WAKEUP_NULL},
	{WAKE_SRC_Timer7,						WAKEUP_NULL},
	{WAKE_SRC_Timer6,						WAKEUP_NP},
	{WAKE_SRC_Timer5,						WAKEUP_NULL},
	{WAKE_SRC_Timer4,						WAKEUP_NULL},
	{WAKE_SRC_Timer3,						WAKEUP_NULL},
	{WAKE_SRC_Timer2,						WAKEUP_NULL},
	{WAKE_SRC_Timer1,						WAKEUP_NULL},
	{WAKE_SRC_Timer0,						WAKEUP_NULL},
	{WAKE_SRC_WDG0,							WAKEUP_NULL},
	{WAKE_SRC_AP_WAKE,						WAKEUP_NULL},
	{WAKE_SRC_NP_WAKE,						WAKEUP_NULL},
	{WAKE_SRC_AON_TIM,						WAKEUP_NULL},
	{WAKE_SRC_WIFI_FTSR_MAILBOX,			WAKEUP_LP},
	{WAKE_SRC_WIFI_FISR_FESR,				WAKEUP_LP},

	{0xFFFFFFFF,							WAKEUP_NULL},	/* Table end */
};

/* can be used by sleep mode & deep sleep mode */
/* config can be set to DISABLE_WAKEPIN/HIGH_LEVEL_WAKEUP/LOW_LEVEL_WAKEUP */
WAKEPIN_TypeDef sleep_wakepin_config[] = {
//  wakepin			config
	{WAKEPIN_0,		DISABLE_WAKEPIN},	/* WAKEPIN_0 corresponding to _PB_21 */
	{WAKEPIN_1,		DISABLE_WAKEPIN},	/* WAKEPIN_1 corresponding to _PB_22  */
	{WAKEPIN_2,		DISABLE_WAKEPIN},	/* WAKEPIN_2 corresponding to _PB_23  */
	{WAKEPIN_3,		DISABLE_WAKEPIN},	/* WAKEPIN_3 corresponding to _PB_24  */

	{0xFFFFFFFF,	DISABLE_WAKEPIN},	/* Table end */
};

PSCFG_TypeDef ps_config = {
	.km0_tickles_debug = TRUE,/* if open WIFI FW, should close it, or beacon will lost in WOWLAN */
	.km0_pg_enable = FALSE,
	.km0_pll_off = TRUE,
	.km0_audio_vad_on = FALSE,
#if defined(CONFIG_CLINTWOOD ) && CONFIG_CLINTWOOD
	.km0_config_psram = FALSE, /* if device enter sleep mode or not, false for keep active */
	.km0_sleep_withM4 = FALSE,
#else
	.km0_config_psram = TRUE, /* if device enter sleep mode or not, false for keep active */
	.km0_sleep_withM4 = TRUE,
#endif
	.keep_OSC4M_on = FALSE,
	.xtal_mode_in_sleep = XTAL_OFF,
	.swr_mode_in_sleep = SWR_PFM,
};

UARTCFG_TypeDef uart_config[4] = {
	/* UART0 */
	{
		.LOW_POWER_RX_ENABLE = DISABLE, /*Enable low power RX*/
	},
	/* UART1 */
	{
		.LOW_POWER_RX_ENABLE = DISABLE,
	},
	/* UART2 */
	{
		.LOW_POWER_RX_ENABLE = DISABLE,
	},
	/* UART3_BT */
	{
		.LOW_POWER_RX_ENABLE = DISABLE,
	},
};
#endif
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
