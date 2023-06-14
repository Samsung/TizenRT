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
PWRCFG_TypeDef km0_pwrmgt_config[] = {
//  	Module								Status
	//{PMC_BIT_PST_SLEP_EACK,			OFF},	//BIT1: ANA4M CLK status when PMC enter into sleep mode-->1:Enable 0:Disable
	{PMC_BIT_PST_SLEP_EBUS,			OFF},	//BIT2: platform clock status when PMC entro into sleep mode-->1:Enable 0:Gate
	{PMC_BIT_PST_SLEP_EMPM,			OFF},	//BIT3: LS system mem status when pmc enter sleep-->1:still in active 0:in DSLP
	{PMC_BIT_PST_SLEP_ERCK,			OFF},	//BIT4: ANA4M CLK status when PMC enter into sleep mode-->1:Enable 0:Disable
	{PMC_BIT_PST_SLEP_EPWM,			OFF},	//BIT8: SWR mode selection when PMC enter into sleep mode-->1:keeps at PWM mode 0:switch to PFM
	{PMC_MASK_PST_SLEP_VOLT,		0x0},	//BIT11:10: Sleep setting for SWR volt selection-->00:0.8v 01:0.9v 10/11:1.0v
	{PMC_MASK_PST_SLEP_XMD,		0x0},	//BIT13:12: Sleep setting for XTAL mode selection-->00:off 01:LPS 2/3:Normal
	{PMC_MASK_PST_SLEP_FBUS,		0x0},	//BIT15:14: Sleep setting for ls bus frequency selection-->00/01:ANA4M 10:XTAL 11:XTAL/2

	{0xFFFFFFFF,					OFF},	/* Table end */
};

PWRCFG_TypeDef aon_pwrmgt_config[] = {
//  	Module						Status
	{AON_BIT_PST_DSLP_LDHM,			OFF},	//BIT0: 1: Eanble AON LDO enter into sleep mode when PMC enter deep-sleep mode 0: Disable
	{AON_BIT_PST_DSLP_ERCK,			ON},	//BIT1: 1: Enable 100K clock for AON register access when PMC enter deep-sleep mode 0: Disable; SIC need set this bit on
	{AON_BIT_PST_DSLP_ERPC,			ON},	//BIT2: 1: Enable AON_RET power cut when PMC enter deep-sleep mode 0: Disable */
	{AON_BIT_PST_WAIT_XTAL,			OFF},	//BIT3: 1: When POF , polling xtal invalid signal and then run forward 0: not polling xtal invalid */
	{AON_BIT_RTC_XTAL_MODE,			ON},	//BIT4: Select xtal mode when xtal used for sdm calibration. 1: Normal 0: LPS
	{AON_BIT_SNOOZE_PDCK,			ON},	//BIT5: Select xtal auto pdck function for sdm calibration use xtal lps mode 1:Enable 0:Disable

	{0xFFFFFFFF,					OFF},	/* Table end */
};

/*Each Module can only be ON by one of the three masks*/
WakeEvent_TypeDef sleep_wevent_config[] = {
//  	Module							Group				Msk_Lp				Msk_Hp				Msk_Ap
	{WAKE_SRC_nFIQOUT1_OR_nIRQOUT1,			1,				OFF,				OFF,				OFF},
	{WAKE_SRC_nFIQOUT0_OR_nIRQOUT0,			1,				OFF,				OFF,				OFF},
	{WAKE_SRC_BT_WAKE_HOST,					1,				OFF,				OFF,				OFF},
	{WAKE_SRC_AON_WAKEPIN,					1,				ON,				OFF,				OFF},
	{WAKE_SRC_WDG4,							1,				OFF,				OFF,				OFF},
	{WAKE_SRC_WDG3,							1,				OFF,				OFF,				OFF},
	{WAKE_SRC_WDG2,							1,				OFF,				OFF,				OFF},
	{WAKE_SRC_WDG1,							1,				OFF,				OFF,				OFF},
	{WAKE_SRC_UART2,						1,				OFF,				OFF,				OFF},
	{WAKE_SRC_UART1,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_UART0,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_SPI1,							0,				OFF,				OFF,				OFF},
	{WAKE_SRC_SPI0,							0,				OFF,				OFF,				OFF},
	{WAKE_SRC_USB_OTG,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_IPC_AP,						0,				OFF,				OFF,				ON},
	{WAKE_SRC_IPC_NP,						0,				OFF,				ON,				OFF},
	{WAKE_SRC_VADBT_OR_VADPC,				0,				OFF,				OFF,				OFF},
	{WAKE_SRC_PWR_DOWN,						0,				ON,				OFF,				OFF},
	{WAKE_SRC_BOR,							0,				OFF,				OFF,				OFF},
	{WAKE_SRC_ADC_COMP,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_ADC,							0,				OFF,				OFF,				OFF},
	{WAKE_SRC_CTOUCH,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_RTC,							0,				OFF,				OFF,				OFF},
	{WAKE_SRC_GPIOC,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_GPIOB,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_GPIOA,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_UART_LOG,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_Timer7,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_Timer6,						0,				OFF,				ON,				OFF},
	{WAKE_SRC_Timer5,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_Timer4,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_Timer3,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_Timer2,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_Timer1,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_Timer0,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_WDG0,							0,				OFF,				OFF,				OFF},
	{WAKE_SRC_AP_WAKE,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_NP_WAKE,						0,				OFF,				OFF,				OFF},
	{WAKE_SRC_AON_TIM,						0,				ON,				OFF,				OFF},
	{WAKE_SRC_WIFI_FTSR_MAILBOX,			0,				ON,				OFF,				OFF},
	{WAKE_SRC_WIFI_FISR_FESR,				0,				ON,				OFF,				OFF},

	{0xFFFFFFFF,							0,				OFF,				OFF,				OFF},	/* Table end */
};

/* light-sleep: out-circut=on, out-val=keep, mem-arry=on */
/* deep-sleep: out-circut=off, out-val=not keep, mem-arry=on, suggest to use this */
PWRCFG_TypeDef sleep_sram_config[] = {
//  	Module										Status
	{REG_CTRL_HSYS_E0_FTC_ULL_0,	MEM_DS_MODE},
	{REG_CTRL_WLK4_E0_FTC_ULL_0,	MEM_DS_MODE},
	{REG_CTRL_BTOFFK4_E0_FTC_ULL_0,	MEM_DS_MODE},
	{REG_CTRL_BTOFFK4_E1_FTC_ULL_0,	MEM_DS_MODE},
	{0xFFFFFFFF,							OFF},	/* Table end */
};

/* if X can wakeup dsleep, it can wakeup dstandby & sleep */
/* if X can wakeup dstandby, it can wakeup sleep */
PWRCFG_TypeDef dsleep_aon_wevent_config[] = {
//  	Module									Status

	{0xFFFFFFFF,								OFF},	/* Table end */
};

u8 aon_wakepin[4] = {
	_PB_21, /* wakepin 0 */
	_PB_22, /* wakepin 1 */
	_PB_23, /* wakepin 2 */
	_PB_24, /* wakepin 3 */
};

/* you should set dsleep_wevent_config.BIT_GPIO_WAKE_STS ON if you use wakepin */
/* can be used by sleep mode & deep sleep mode */
WAKEPIN_TypeDef sleep_wakepin_config[] = {
//  wakepin		Status		Polarity
	{WAKUP_0,		OFF,			1},	/* wakeup_0 config */
	{WAKUP_1,		OFF,			1},	/* wakeup_1 config */
	{WAKUP_2,		OFF,			1},	/* wakeup_2 config */
	{WAKUP_3,		OFF,			1},	/* wakeup_3 config */

	{0xFFFFFFFF,	OFF,			0},	/* Table end */
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
};

UARTCFG_TypeDef uart_config[4] = {
	/* HS UART--> UART0 */
	{
		.LOW_POWER_RX_ENABLE = DISABLE, /*Enable low power RX*/
	},
	/* BT UART--> UART1 */
	{
		.LOW_POWER_RX_ENABLE = DISABLE,
	},
	/* Log UART--> UART2 */
	{
		.LOW_POWER_RX_ENABLE = DISABLE,
	},
	/* LPUART-->UART3 */
	{
		.LOW_POWER_RX_ENABLE = DISABLE,
	},
};
#endif
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
