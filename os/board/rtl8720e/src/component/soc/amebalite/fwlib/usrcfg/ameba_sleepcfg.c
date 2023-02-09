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
PWRCFG_TypeDef np_pwrmgt_config[] = {
//  	Module						Status
	{PMC_BIT_PST_SLEP_EBUS,			OFF},	//BIT2: platform clock(clk_sram) status when PMC entro into sleep mode-->1:Enable 0:Gate
	{PMC_BIT_PST_SLEP_ERCK,			ON},	//BIT4: ANA4M(lpon) CLK status when PMC enter into sleep mode-->1:Enable 0:Disable; SIC need set this bit on
	{PMC_BIT_PST_SLEP_EPWM,			OFF},	//BIT8: SWR mode selection when PMC enter into sleep mode-->1:keeps at PWM mode 0:switch to PFM
	{PMC_BIT_PST_SLEP_LDOM,			OFF},	//BIT9: LDO_core mode selection when PMC enter into sleep mode-->1:keeps at normal mode 0:switch to standby
	{PMC_MASK_PST_SLEP_XMD,		0x0},	//BIT13:12: Sleep setting for XTAL mode selection-->00:off 01:LPS 2/3:Normal; VAD need set XTAL lps or normal mode

	{0xFFFFFFFF,					OFF},	/* Table end */
};

PWRCFG_TypeDef np_aon_pwrmgt_config[] = {
//  	Module						Status
	{AON_BIT_PST_DSLP_LDHM,			OFF},	//BIT0: 1: Eanble AON LDO enter into sleep mode when PMC enter deep-sleep mode 0: Disable
	{AON_BIT_PST_DSLP_ERCK,			ON},	//BIT1: 1: Enable 100K clock for AON register access when PMC enter deep-sleep mode 0: Disable; SIC need set this bit on
	{AON_BIT_PST_WAIT_XTAL,			OFF},	//BIT3: 1: When POF , polling xtal invalid signal and then run forward 0: not polling xtal invalid */
	{AON_BIT_RTC_XTAL_MODE,		ON},	//BIT4: Select xtal mode when xtal used for sdm calibration. 1: Normal 0: LPS
	{AON_BIT_SNOOZE_PDCK,			ON},	//BIT5: Select xtal auto pdck function for sdm calibration use xtal lps mode 1:Enable 0:Disable

	{0xFFFFFFFF,					OFF},	/* Table end */
};


/*Each Module can only be ON by one of the three masks*/
WakeEvent_TypeDef sleep_wevent_config[] = {
//  	Module							Group				Msk_NP				Msk_AP				Msk_DSP
	{WAKE_SRC_VAD,							1,					OFF,				OFF,				OFF},
	{WAKE_SRC_AON_WAKEPIN,				1,					OFF,				OFF,				OFF},
	{WAKE_SRC_AON_TIM,						1,					OFF,				OFF,				OFF},
	{WAKE_SRC_PWR_DOWN,					0,					OFF,				OFF,				OFF},
	{WAKE_SRC_BOR,							0,					OFF,				OFF,				OFF},
	{WAKE_SRC_ADC_COMP,					0,					OFF,				OFF,				OFF},
	{WAKE_SRC_ADC,							0,					OFF,				OFF,				OFF},
	{WAKE_SRC_AON_RTC,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_SPI1,							0,					OFF,				OFF,				OFF},
	{WAKE_SRC_SPI0,							0,					OFF,				OFF,				OFF},
	{WAKE_SRC_CTOUCH,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_GPIOB,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_GPIOA,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_UART_LOG,					0,					OFF,				ON,				OFF},
	{WAKE_SRC_UART3,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_UART2,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_UART1,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_UART0,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_Timer7,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_Timer6,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_Timer5,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_Timer4,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_Timer3,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_Timer2,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_Timer1,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_Timer0,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_WDG0,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_BT_WAKE_HOST,				0,					OFF,				OFF,				OFF},
	{WAKE_SRC_DSP_WAKE,					0,					OFF,				OFF,				OFF},
	{WAKE_SRC_AP_WAKE,						0,					OFF,				OFF,				OFF},
	{WAKE_SRC_WIFI_FTSR_MAILBOX,			0,					ON,				OFF,				OFF},
	{WAKE_SRC_WIFI_FISR_FESR,				0,					ON,				OFF,				OFF},

	{0xFFFFFFFF,							0,					OFF,				OFF,				OFF},	/* Table end */
};

//TODO
/* light-sleep: out-circut=on, out-val=keep, mem-arry=on */
/* deep-sleep: out-circut=off, out-val=not keep, mem-arry=on, suggest to use this */
PWRCFG_TypeDef sleep_sram_config[] = {
//  	Module							Status
	{REG_CTRL_GRP_LRET_E0_CTRL0,		BIT_MEM_CTRL_LPS_DS_REV},	//retention sram
	{REG_CTRL_GRP_HSYS_E0_CTRL0,		BIT_MEM_CTRL_LPS_DS},	//hs sram E0
	{REG_CTRL_GRP_HSYS_E1_CTRL0,		BIT_MEM_CTRL_LPS_DS},	//hs sram E1
	{REG_CTRL_GRP_WLK4_E0_CTRL0,		BIT_MEM_CTRL_LPS_DS},	//wlk4 sram
	{REG_CTRL_GRP_WPON_E0_CTRL0,		BIT_MEM_CTRL_LPS_DS},	//wpon sram
	{REG_CTRL_GRP_BTONK4_E0_CTRL0,		BIT_MEM_CTRL_LPS_DS},	//bt on sram E0
	{REG_CTRL_GRP_BTONK4_E1_CTRL0,		BIT_MEM_CTRL_LPS_DS},	//bt on sram E1
	{REG_CTRL_GRP_BTOFFK4_E0_CTRL0,		BIT_MEM_CTRL_LPS_DS_REV},	//bt off sram E0
	{REG_CTRL_GRP_BTOFFK4_E1_CTRL0,		BIT_MEM_CTRL_LPS_DS_REV},	//bt off sram E1
	{REG_CTRL_GRP_HIFI5_E5_CTRL0,		BIT_MEM_CTRL_LPS_DS},	//HIFI5 DTCM
	{0xFFFFFFFF,							OFF},	/* Table end */
};

/* if X can wakeup dsleep, it can wakeup dstandby & sleep */
/* if X can wakeup dstandby, it can wakeup sleep */
PWRCFG_TypeDef dsleep_aon_wevent_config[] = {
//  	Module								Status
	{0xFFFFFFFF,							OFF},	/* Table end */
};

u8 aon_wakepin[2] = {
	_PA_0, /* wakepin 0 */
	_PA_1, /* wakepin 1 */
};

/* you should set dsleep_wevent_config.BIT_GPIO_WAKE_STS ON if you use wakepin */
/* can be used by sleep mode (power-gating) & deep sleep mode */
WAKEPIN_TypeDef sleep_wakepin_config[] = {
//  wakepin		Status		Polarity
	{WAKUP_0,		OFF,			1},	/* wakeup_0 config */
	{WAKUP_1,		OFF,			1},	/* wakeup_1 config */

	{0xFFFFFFFF,	OFF,			0},	/* Table end */
};

PSCFG_TypeDef ps_config = {
	.np_tickles_debug = FALSE,
//	.np_pll_off = TRUE,
//	.np_audio_vad_on = FALSE,
//	.np_config_ddr = WB_PSRAM_SlEEP_Mode,
//	.np_osc4m_close = FALSE,
};
#endif
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
