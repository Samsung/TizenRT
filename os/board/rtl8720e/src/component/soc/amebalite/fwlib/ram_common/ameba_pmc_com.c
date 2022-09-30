/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"
IMAGE2_RAM_DATA_SECTION
u32 PMC_MemMode_BK[10];
IMAGE2_RAM_DATA_SECTION
u32 LBUS_CLK;
IMAGE2_RAM_DATA_SECTION
u32 PSRAM_CLK;
IMAGE2_RAM_DATA_SECTION
u32 SPIC_CLK;
IMAGE2_RAM_DATA_SECTION
u32 PLFM_CLK;
IMAGE2_RAM_DATA_SECTION
u32 HIFI_CLK;
IMAGE2_RAM_DATA_SECTION
MEMMode_TypeDef PMC_MemMode_Def[] = {
//		Module								MEM_SD					MEM_DS					MEM_LS
	{
		REG_CTRL_GRP_LRET_E0_CTRL0,		BIT_MEM_CTRL_LPS_SD_REV,	BIT_MEM_CTRL_LPS_DS_REV,	BIT_MEM_CTRL_LPS_LS_REV
	},	//retention sram
	{
		REG_CTRL_GRP_HSYS_E0_CTRL0,		BIT_MEM_CTRL_LPS_SD,		BIT_MEM_CTRL_LPS_DS,		BIT_MEM_CTRL_LPS_LS
	},	//hs sram E0
	{
		REG_CTRL_GRP_HSYS_E1_CTRL0,		BIT_MEM_CTRL_LPS_SD,		BIT_MEM_CTRL_LPS_DS,		BIT_MEM_CTRL_LPS_LS
	},	//hs sram E1
	{
		REG_CTRL_GRP_WLK4_E0_CTRL0,	BIT_MEM_CTRL_LPS_SD,		BIT_MEM_CTRL_LPS_DS,		BIT_MEM_CTRL_LPS_LS
	},	//wlk4 sram
	{
		REG_CTRL_GRP_WPON_E0_CTRL0,	BIT_MEM_CTRL_LPS_SD,		BIT_MEM_CTRL_LPS_DS,		BIT_MEM_CTRL_LPS_LS
	},	//wpon sram
	{
		REG_CTRL_GRP_BTONK4_E0_CTRL0,	BIT_MEM_CTRL_LPS_SD,		BIT_MEM_CTRL_LPS_DS,		BIT_MEM_CTRL_LPS_LS
	},	//bt on sram E0
	{
		REG_CTRL_GRP_BTONK4_E1_CTRL0,	BIT_MEM_CTRL_LPS_SD,		BIT_MEM_CTRL_LPS_DS,		BIT_MEM_CTRL_LPS_LS
	},	//bt on sram E1
	{
		REG_CTRL_GRP_BTOFFK4_E0_CTRL0,	BIT_MEM_CTRL_LPS_SD_REV,	BIT_MEM_CTRL_LPS_DS_REV,	BIT_MEM_CTRL_LPS_LS_REV
	},	//bt off sram E0
	{
		REG_CTRL_GRP_BTOFFK4_E1_CTRL0,	BIT_MEM_CTRL_LPS_SD_REV,	BIT_MEM_CTRL_LPS_DS_REV,	BIT_MEM_CTRL_LPS_LS_REV
	},	//bt off sram E1
	{
		REG_CTRL_GRP_HIFI5_E5_CTRL0,		BIT_MEM_CTRL_LPS_SD,		BIT_MEM_CTRL_LPS_DS,		BIT_MEM_CTRL_LPS_LS
	},	//HIFI5 shared-sram
};

/**
  * @brief  set sram mem mode
  * @param  module: sram mudule
  * @param  mem_mode: mem mode
  *		This parameter can be one of the following values:
  *		 @arg MEM_ACT_MODE : active mode
  *		 @arg MEM_LS_MODE : light sleep mode
  *		 @arg MEM_DS_MODE : deep sleep mode
  *		 @arg MEM_SD_MODE : shut down mode
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_SetMemMode(u32 module, u32 mem_mode)
{
	u32 Rtemp = 0;
	u32 temp = 0;
	u32 index = 0;
	for (index = 0; index < 10; index++) {
		if (module == PMC_MemMode_Def[index].Module) {
			break;
		}
	}

	if (10 == index) {
		return;
	}

	Rtemp = HAL_READ32(SYSTEM_MEM_CTRL_BASE, module);
	Rtemp &= (~CTRL_MASK_GRP_x_PWR_LPS);
	Rtemp |= CTRL_GRP_x_PWR_LPS(mem_mode);
	HAL_WRITE32(SYSTEM_MEM_CTRL_BASE, module, Rtemp);
	//DBG_8195A("mem set 0x%x = 0x%x\n", module, HAL_READ32(SYSTEM_MEM_CTRL_BASE, module));

	if ((module == REG_CTRL_GRP_BTOFFK4_E0_CTRL0) || (module == REG_CTRL_GRP_BTOFFK4_E1_CTRL0)) {
		temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PLAT_CTRL);
		temp |= LSYS_BIT_SHARE_BT_MEM;
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_PLAT_CTRL, temp);

	} else if (module == REG_CTRL_GRP_WLK4_E0_CTRL0) {
		temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PLAT_CTRL);
		temp |= LSYS_BIT_SHARE_WL_MEM;
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_PLAT_CTRL, temp);
	}

}

//TODO
/**
  * @brief  set power option for power save mode.
  * @param  pwrmgt_option: power save mode option.
  *          This parameter can be one or combinations of the following values:
  *            @arg PMC_PST_NORM_XMD: Enable XTAL when PMC exit from sleep mode
  *            @arg PMC_PST_SLEP_XMD: Enable XTAL when PMC enter into sleep mode
  *            @arg PMC_BIT_PST_SLEP_LDOM: Sleep setting for core LDO mode selection
  *            @arg PMC_BIT_PST_SLEP_EPWM: Sleep setting for SWR mode selection
  *            @arg PMC_BIT_PST_SLEP_ERCK: Enable lpon CLK when PMC enter into sleep mode
  *            @arg PMC_BIT_PST_SLEP_EBUS: Enable platform clock when PMC entro into sleep mode
  *            @arg PMC_BIT_PST_SLEP_ESOC: power enable SOC platform when PMC enter into sleep mode (clock-gating)
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_PWROption(u32 pwrmgt_option)
{
	HAL_WRITE32(PMC_BASE, SYSPMC_OPT, pwrmgt_option);
}


/**
  * @brief  set REGU wakeup_pin (just on wakepin mux to 2 pads).
  * @param  PinMask: aon wakepin index
  *		This parameter can be one of the following values:
  *		 @arg WAKUP_0 :
  *		 @arg WAKUP_1 :
  * @param  Polarity: aon wakepin Polarity
  *		This parameter can be one of the following values:
  *		 @arg 1 : high wakeup
  *		 @arg 0 : low wakeup
  * @note wakeup state: sleep PG & CG & deep sleep
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_SetWakepin(u32 PinIdx, u32 Polarity)
{
	u32 Rtemp = 0;

	/* disable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_GPIO_CTRL);
	Rtemp &= (~(AON_GPIO_WAKE_FEN(BIT(PinIdx))));
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_GPIO_CTRL, Rtemp);

	/* set polarity & internal PU/PD resistence */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_GPIO_CTRL);
	Rtemp &= ~(AON_GPIO_WAKDET_POLY(BIT(PinIdx))); /* clear polarity, set to low active */
	if (Polarity == 1) {
		Rtemp |= AON_GPIO_WAKDET_POLY(BIT(PinIdx)); /* if high active */
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_GPIO_CTRL, Rtemp);

	/* enable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_GPIO_CTRL);
	Rtemp |= (AON_GPIO_WAKE_FEN(BIT(PinIdx)));
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_GPIO_CTRL, Rtemp);
}


/**
  * @brief  set REGU wakeup_pin debounce cycle.
  * @param  Dbnc_cycle: wakeup_pin debounce cycle, unit is 100k/128k.
  *		This parameter must be set to a value in the 0-0x7FFF range.
  * @param  Status: enable or disable wakepin debounce function
  * @note Dbnc_cycle is valid when Status is set ENABLE.
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_SetWakepinDebounce(u32 Dbnc_cycle, u32 Status)
{
	u32 Rtemp = 0;
	u32 FEN_backup = 0;
	assert_param(Dbnc_cycle <= 0x7FFF);

	/* disable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_GPIO_CTRL);
	FEN_backup = AON_GET_GPIO_WAKE_FEN(Rtemp);
	Rtemp &= (~AON_MASK_GPIO_WAKE_FEN);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_GPIO_CTRL, Rtemp);

	/* set gpio debounce cycle */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_GPIO_CTRL);
	if (ENABLE == Status) {
		Rtemp &= (~AON_MASK_GPIO_DBNC_CYC);
		Rtemp |= AON_GPIO_DBNC_CYC(Dbnc_cycle);
		Rtemp |= AON_BIT_GPIO_DBNC_FEN;
	} else {
		Rtemp &= (~AON_BIT_GPIO_DBNC_FEN);
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_GPIO_CTRL, Rtemp);

	/* enable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_GPIO_CTRL);
	Rtemp |= AON_GPIO_WAKE_FEN(FEN_backup);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_GPIO_CTRL, Rtemp);
}

/**
  * @brief  get wakepin index.
  * @param  None
  * @retval BIT(0): wakepin0, BIT(1): wakepin1
  */
IMAGE2_RAM_TEXT_SECTION
int SOCPS_WakePinCheck(void)
{
	int WakePin = 0;

	WakePin = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_AON_WAK_EVT);
	WakePin = WakePin >> 4;

	return WakePin;
}


/**
  * @brief  Clear wakepin interrupt.
  * @retval   None
  */
VOID SOCPS_WakePinClearINT(u32 wakepin)
{
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_GPIO_WAKE_ISR, wakepin);
}


/**
  * @brief  set AON timer for wakeup.
  * @param  SDuration: sleep time, unit is ms
  * @note wakeup state:sleep PG & CG & deep sleep
  * @note This is 100KHz timer, max counter = 0xFFFFFFFF/100000/60 = 715min
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_AONTimer(u32 SDuration)
{
	if (SDuration == 0) {
		return;
	}

	SDuration = SDuration * 100;

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_TIM_SET, AON_TIM_CNT_CUR(SDuration));
}

/**
  * @brief  get current AON timer.
  * @param  None
  * @note wakeup state:sleep PG & CG & deep sleep
  * @retval The current Aon Timer counter, the unit is ms.
  */
IMAGE2_RAM_TEXT_SECTION
u32 SOCPS_AONTimerGet(VOID)
{
	u32 Rtemp = 0;

	/* use 32K */
	Rtemp = AON_GET_TIM_CNT_CUR(HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_TIM_CNT));
	Rtemp = Rtemp / 100;
	return Rtemp;
}


/**
  * @brief  enable AON timer interrupt.
  * @param  Status: interrupt status
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_AONTimerINT_EN(u32 Status)
{
	u32 Rtemp = 0;

	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_TIM_IER);
	if (ENABLE == Status) {
		Rtemp |= AON_BIT_TIM_IE;
	} else {
		Rtemp &= (~AON_BIT_TIM_IE);

	}
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_TIM_IER, Rtemp);
}

/**
  * @brief  Clear aontimer interrupt.
  * @retval   None
  */
VOID SOCPS_AONTimerClearINT(void)
{
	u32 temp = 0;

	temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_TIM_ISR);
	temp |= AON_BIT_TIM_ISR;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_TIM_ISR, temp);
}


/**
  * @brief  get aon wake reason.
  * @param  None
  * @retval aon wake up reason
  *		This parameter can be one or combination of the following values:
  *		 @arg AON_BIT_RTC_ISR_EVT
  *		 @arg AON_BIT_GPIO_PIN1_WAKDET_EVT
  *		 @arg AON_BIT_GPIO_PIN0_WAKDET_EVT
  *		 @arg AON_BIT_TIM_ISR_EVT
  *		 @arg AON_BIT_BOR_ISR_EVT
  *		 @arg AON_BIT_CHIPEN_LP_ISR_EVT
  *		 @arg AON_BIT_CHIPEN_SP_ISR_EVT
  */
IMAGE2_RAM_TEXT_SECTION
int SOCPS_AONWakeReason(void)
{
	int reason = 0;

	reason = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_AON_WAK_EVT);

	return reason;
}


/**
  * @brief  set np wake up event mask0.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_SetNPWakeEvent_MSK0(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;

	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, WAK_MASK0_NP);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, WAK_MASK0_NP, WakeEvent);
}

/**
  * @brief  set np wake up event mask1.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_SetNPWakeEvent_MSK1(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;

	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, WAK_MASK1_NP);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, WAK_MASK1_NP, WakeEvent);
}

/**
  * @brief  set np wake up event.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_SetNPWakeEvent(u32 Option, u32 Group, u32 NewStatus)
{
	if (Group) {
		SOCPS_SetNPWakeEvent_MSK1(Option, NewStatus);
	} else {
		SOCPS_SetNPWakeEvent_MSK0(Option, NewStatus);
	}
}

/**
  * @brief  set ap wake up event mask0.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_SetAPWakeEvent_MSK0(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;

	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, WAK_MASK0_AP);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, WAK_MASK0_AP, WakeEvent);
}

/**
  * @brief  set ap wake up event mask1.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_SetAPWakeEvent_MSK1(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;

	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, WAK_MASK1_AP);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, WAK_MASK1_AP, WakeEvent);
}

/**
  * @brief  set ap wake up event.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_SetAPWakeEvent(u32 Option, u32 Group, u32 NewStatus)
{
	if (Group) {
		SOCPS_SetAPWakeEvent_MSK1(Option, NewStatus);
	} else {
		SOCPS_SetAPWakeEvent_MSK0(Option, NewStatus);
	}
}

/**
  * @brief  set dsp wake up event mask0.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_SetDSPWakeEvent_MSK0(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;

	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, WAK_MASK0_DSP);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, WAK_MASK0_DSP, WakeEvent);
}

/**
  * @brief  set dsp wake up event mask1.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_SetDSPWakeEvent_MSK1(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;

	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, WAK_MASK1_DSP);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, WAK_MASK1_DSP, WakeEvent);
}

/**
  * @brief  set dsp wake up event.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_SetDSPWakeEvent(u32 Option, u32 Group, u32 NewStatus)
{
	if (Group) {
		SOCPS_SetDSPWakeEvent_MSK1(Option, NewStatus);
	} else {
		SOCPS_SetDSPWakeEvent_MSK0(Option, NewStatus);
	}
}


/**
  *  @brief set work modules/wake up event after deepsleep.
  *  @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_deepsleepInit(void)
{
	static u32 npdslp_pwrmgt_config_val;

	npdslp_pwrmgt_config_val = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_PMC_OPT);
	for (int i = 0;;) {
		/*  Check if search to end */
		if (np_aon_pwrmgt_config[i].Module == 0xFFFFFFFF) {
			break;
		}

		if (np_aon_pwrmgt_config[i].Status == ON) {
			npdslp_pwrmgt_config_val |= np_aon_pwrmgt_config[i].Module;
		} else {
			npdslp_pwrmgt_config_val &= ~np_aon_pwrmgt_config[i].Module;
		}

		i++;
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_PMC_OPT, npdslp_pwrmgt_config_val);

	DBG_PRINTF(MODULE_PMC, LEVEL_INFO, "REG_AON_PMC_OPT 0x%x\n", HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_PMC_OPT));
}


/**
  * @brief  enter deep sleep mode, patch of SOCPS_DeepSleep.
  * @retval None
  *
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_DeepSleep_RAM(void)
{
	u32 Rtemp = 0;
	REGU_TypeDef *regu = REGU_BASE;

	DBG_8195A("DSLP \n");

	Systick_Cmd(DISABLE);
	__disable_irq();

	/* Enable low power mode */
	FLASH_DeepPowerDown(ENABLE);

	/* shutdown analog LDO: about 143uA */
	regu->REGU_LDO_3318_CTRL |= REGU_BIT_PD_REGU_L_3318;

	/* shutdown psram pad */
	Rtemp = HAL_READ32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1);
	Rtemp &= ~PAD_BIT_DDR_PWDPADN_2REGU;
	HAL_WRITE32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1, Rtemp);

	/* set power mode */
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	Rtemp |= PMC_BIT_PMEN_DSLP;
	Rtemp &= ~PMC_BIT_PMEN_SLEP;
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

	/* Wait CHIP enter deep sleep mode */
	__WFI();
}

/**
  * @brief  PMC patch for fixing hw bug
  * @param  None
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
static void SOCPS_PMC_Patch(void)
{
	u32 Rtemp = 0;

	/* PMC patch1 for lbus clk switch to xtal when system exit sleep mode */
	Rtemp = HAL_READ32(PMC_BASE, PMC_PATCH_GRP0_0);
	Rtemp &= ~PMC_MASK_PATCH_GRP0_SADDR;
	Rtemp |= PMC_PATCH_GRP0_SADDR(0x38);
	Rtemp &= ~PMC_MASK_PATCH_GRP0_INSTR0;
	Rtemp |= PMC_PATCH_GRP0_INSTR0(0x24101);
	Rtemp &= ~PMC_MASK_PATCH_GRP0_INSTR1_5_0;
	Rtemp |= PMC_PATCH_GRP0_INSTR1_5_0(0x4);
	HAL_WRITE32(PMC_BASE, PMC_PATCH_GRP0_0, Rtemp);

	Rtemp = HAL_READ32(PMC_BASE, PMC_PATCH_GRP0_1);
	Rtemp &= ~PMC_MASK_PATCH_GRP0_INSTR1_17_6;
	Rtemp |= PMC_PATCH_GRP0_INSTR1_17_6(0x900);
	HAL_WRITE32(PMC_BASE, PMC_PATCH_GRP0_1, Rtemp);

	/* PMC patch2 for aon_ldo mode switch when system enter/exit sleep mode, for saving 15uA */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_PMC_ADDR01);
	Rtemp &= ~AON_MASK_PMC_PATCH_ADDR1;
	Rtemp |= AON_PMC_PATCH_ADDR1(0x10D);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_PMC_ADDR01, Rtemp);

	Rtemp = HAL_READ32(PMC_BASE, PMC_PATCH_GRP1_0);
	Rtemp &= ~PMC_MASK_PATCH_GRP1_SADDR;
	Rtemp |= PMC_PATCH_GRP1_SADDR(0x9A);
	Rtemp &= ~PMC_MASK_PATCH_GRP1_INSTR0;
	Rtemp |= PMC_PATCH_GRP1_INSTR0(0x809c);
	HAL_WRITE32(PMC_BASE, PMC_PATCH_GRP1_0, Rtemp);

	Rtemp = HAL_READ32(PMC_BASE, PMC_PATCH_GRP4_0);
	Rtemp &= ~PMC_MASK_PATCH_GRP4_SADDR;
	Rtemp |= PMC_PATCH_GRP4_SADDR(0x9c);
	Rtemp &= ~PMC_MASK_PATCH_GRP4_INSTR0;
	Rtemp |= PMC_PATCH_GRP4_INSTR0(0x20151);
	Rtemp &= ~PMC_MASK_PATCH_GRP4_INSTR1_5_0;
	Rtemp |= PMC_PATCH_GRP4_INSTR1_5_0(0x10);
	HAL_WRITE32(PMC_BASE, PMC_PATCH_GRP4_0, Rtemp);

	Rtemp = HAL_READ32(PMC_BASE, PMC_PATCH_GRP4_1);
	Rtemp &= ~PMC_MASK_PATCH_GRP4_INSTR1_17_6;
	Rtemp |= PMC_PATCH_GRP4_INSTR1_17_6(0x822);
	Rtemp &= ~PMC_MASK_PATCH_GRP4_INSTR2;
	Rtemp |= PMC_PATCH_GRP4_INSTR2(0x809b);
	HAL_WRITE32(PMC_BASE, PMC_PATCH_GRP4_1, Rtemp);

	Rtemp = HAL_READ32(PMC_BASE, PMC_PATCH_GRP2_0);
	Rtemp &= ~PMC_MASK_PATCH_GRP2_SADDR;
	Rtemp |= PMC_PATCH_GRP2_SADDR(0x22);
	Rtemp &= ~PMC_MASK_PATCH_GRP2_INSTR0;
	Rtemp |= PMC_PATCH_GRP2_INSTR0(0x805A);
	HAL_WRITE32(PMC_BASE, PMC_PATCH_GRP2_0, Rtemp);

	Rtemp = HAL_READ32(PMC_BASE, PMC_PATCH_GRP3_0);
	Rtemp &= ~PMC_MASK_PATCH_GRP3_SADDR;
	Rtemp |= PMC_PATCH_GRP3_SADDR(0x5A);
	Rtemp &= ~PMC_MASK_PATCH_GRP3_INSTR0;
	Rtemp |= PMC_PATCH_GRP3_INSTR0(0x20150);
	Rtemp &= ~PMC_MASK_PATCH_GRP3_INSTR1_5_0;
	Rtemp |= PMC_PATCH_GRP3_INSTR1_5_0(0x21);
	HAL_WRITE32(PMC_BASE, PMC_PATCH_GRP3_0, Rtemp);

	Rtemp = HAL_READ32(PMC_BASE, PMC_PATCH_GRP3_1);
	Rtemp &= ~PMC_MASK_PATCH_GRP3_INSTR1_17_6;
	Rtemp |= PMC_PATCH_GRP3_INSTR1_17_6(0x801);
	Rtemp &= ~PMC_MASK_PATCH_GRP3_INSTR2;
	Rtemp |= PMC_PATCH_GRP3_INSTR2(0x8023);
	HAL_WRITE32(PMC_BASE, PMC_PATCH_GRP3_1, Rtemp);
}


/**
  *  @brief set work modules/wake up event after sleep.
  *  @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_sleepInit(void)
{

	int i = 0;
	static u32 sleep_wevent_config_val[3][2];
	static u32 npcg_pwrmgt_config_val;
	u32 KR4_is_NP = 0;

	KR4_is_NP = LSYS_GET_KR4_IS_NP(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1));

	SOCPS_PMC_Patch();

	/* np_pwrmgt_config option setting */
	npcg_pwrmgt_config_val = HAL_READ32(PMC_BASE, SYSPMC_OPT);
	for (i = 0;;) {
		/*  Check if search to end */
		if (np_pwrmgt_config[i].Module == 0xFFFFFFFF) {
			break;
		}

		if (np_pwrmgt_config[i].Status == ON) {
			npcg_pwrmgt_config_val |= np_pwrmgt_config[i].Module;
		} else {
			npcg_pwrmgt_config_val &= ~np_pwrmgt_config[i].Module;
		}

		if (np_pwrmgt_config[i].Module == PMC_MASK_PST_SLEP_XMD) {
			npcg_pwrmgt_config_val &= (~PMC_MASK_PST_SLEP_XMD);
			npcg_pwrmgt_config_val |= PMC_PST_SLEP_XMD(np_pwrmgt_config[i].Status);
		}

		i++;
	}

	HAL_WRITE32(PMC_BASE, SYSPMC_OPT, npcg_pwrmgt_config_val);
	DBG_PRINTF(MODULE_PMC, LEVEL_INFO, "SYSPMC_OPT %x\n", HAL_READ32(PMC_BASE, SYSPMC_OPT));

	/*wake event setting*/
	for (i = 0;;) {
		/*  Check if search to end */
		if (sleep_wevent_config[i].Module == 0xFFFFFFFF) {
			break;
		}

		u32 wevent_msk = 0;
		u32 group = sleep_wevent_config[i].Group;

		if (group == 1 || group == 0) {

			//NP wakeevent msk setting
			if (KR4_is_NP) {
				wevent_msk = sleep_wevent_config[i].Msk_KR4;
			} else {
				wevent_msk = sleep_wevent_config[i].Msk_KM4;
			}

			if (wevent_msk == ON) {
				sleep_wevent_config_val[0][group] |= sleep_wevent_config[i].Module;
			} else {
				sleep_wevent_config_val[0][group] &= ~sleep_wevent_config[i].Module;
			}

			//AP wakeevent msk setting
			if (KR4_is_NP) {
				wevent_msk = sleep_wevent_config[i].Msk_KM4;
			} else {
				wevent_msk = sleep_wevent_config[i].Msk_KR4;
			}

			if (wevent_msk == ON) {
				sleep_wevent_config_val[1][group] |= sleep_wevent_config[i].Module;
			} else {
				sleep_wevent_config_val[1][group] &= ~sleep_wevent_config[i].Module;
			}

			//DSP wakeevent msk setting
			wevent_msk = sleep_wevent_config[i].Msk_DSP;
			if (wevent_msk == ON) {
				sleep_wevent_config_val[2][group] |= sleep_wevent_config[i].Module;
			} else {
				sleep_wevent_config_val[2][group] &= ~sleep_wevent_config[i].Module;
			}
		}

		i++;
	}

	SOCPS_SetAPWakeEvent_MSK0(sleep_wevent_config_val[1][0], ENABLE);
	SOCPS_SetAPWakeEvent_MSK1(sleep_wevent_config_val[1][1], ENABLE);
	SOCPS_SetDSPWakeEvent_MSK0(sleep_wevent_config_val[2][0], ENABLE);
	SOCPS_SetDSPWakeEvent_MSK1(sleep_wevent_config_val[2][1], ENABLE);

	if ((sleep_wevent_config_val[0][0] & sleep_wevent_config_val[1][0]) | \
		(sleep_wevent_config_val[0][0] & sleep_wevent_config_val[2][0]) | \
		(sleep_wevent_config_val[1][0] & sleep_wevent_config_val[2][0]) | \
		(sleep_wevent_config_val[0][1] & sleep_wevent_config_val[1][1]) | \
		(sleep_wevent_config_val[0][1] & sleep_wevent_config_val[2][1]) | \
		(sleep_wevent_config_val[1][1] & sleep_wevent_config_val[2][1])) {
		DBG_PRINTF(MODULE_PMC, LEVEL_ERROR, "Warning: Wakeevent set conflict !!!\n");
	}

	if (sleep_wevent_config_val[1][0] | sleep_wevent_config_val[1][1]) {
		sleep_wevent_config_val[0][0] |= WAKE_SRC_AP_WAKE;
		DBG_PRINTF(MODULE_PMC, LEVEL_INFO, "AP wake event %x %x\n", HAL_READ32(PMC_BASE, WAK_MASK0_AP), HAL_READ32(PMC_BASE, WAK_MASK1_AP));
	}

	if (sleep_wevent_config_val[2][0] | sleep_wevent_config_val[2][1]) {
		sleep_wevent_config_val[0][0] |= WAKE_SRC_DSP_WAKE;
		DBG_PRINTF(MODULE_PMC, LEVEL_INFO, "DSP wake event %x %x\n", HAL_READ32(PMC_BASE, WAK_MASK0_DSP), HAL_READ32(PMC_BASE, WAK_MASK1_DSP));
	}

	SOCPS_SetNPWakeEvent_MSK0(sleep_wevent_config_val[0][0], ENABLE);
	SOCPS_SetNPWakeEvent_MSK1(sleep_wevent_config_val[0][1], ENABLE);

	DBG_PRINTF(MODULE_PMC, LEVEL_INFO, "NP wake event %x %x\n", HAL_READ32(PMC_BASE, WAK_MASK0_NP), HAL_READ32(PMC_BASE, WAK_MASK1_NP));

	/* sleep sram option init
	currently lite no need to config sram, default all sram enter DS mode when system sleep */
	for (i = 0;;) {
		/*  Check if search to end */
		if (sleep_sram_config[i].Module == 0xFFFFFFFF) {
			break;
		}

		SOCPS_SetMemMode(sleep_sram_config[i].Module, sleep_sram_config[i].Status);
		PMC_MemMode_BK[i] = HAL_READ32(SYSTEM_MEM_CTRL_BASE, sleep_sram_config[i].Module);

		i++;
	}

	/* set aon wake pin */
	/* clear all wakeup pin first, and then enable by config */
	for (i = 0;;) {
		/*  Check if search to end */
		if (sleep_wakepin_config[i].wakepin == 0xFFFFFFFF) {
			break;
		}

		if (sleep_wakepin_config[i].Status == ON) {
			SOCPS_SetWakepin(sleep_wakepin_config[i].wakepin, sleep_wakepin_config[i].Polarity);
		}

		i++;
	}
}

IMAGE2_RAM_TEXT_SECTION
void LOGUART_WaitTxComplete(void)
{
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;
	u32 lsr;

	/* Wait for LogUart print out */
	while (1) {
		lsr = UARTLOG->LOGUART_UART_LSR;

		if ((lsr & LOGUART_BIT_TP1F_EMPTY) && (lsr & LOGUART_BIT_TP2F_EMPTY) && (lsr & LOGUART_BIT_TP3F_EMPTY)  && \
			(lsr & LOGUART_BIT_TP4F_EMPTY) && (!(lsr & LOGUART_BIT_RPF_DRDY)) && (lsr & LOGUART_BIT_TX_EMPTY)) {
			break;
		}

		DelayUs(100);
	}

	/* delay at least 12 cycles of one bit time to make sure the last data is completely out of tx fifo, 1.5Mbps(xtal40M) is 12/1.5M = 8us */
	DelayUs(8);
}

IMAGE2_RAM_TEXT_SECTION
void SOCPS_OSC4M_CTRL(u8 open)
{
	u32 Rtemp;

	if (open) {
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKE_GRP1);
		Rtemp |= APBPeriph_LOGUART_CLOCK;
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKE_GRP1, Rtemp);
	} else {

		LOGUART_WaitTxComplete();

		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKE_GRP1);
		Rtemp &= ~APBPeriph_LOGUART_CLOCK;
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKE_GRP1, Rtemp);
	}
}

/**
  * @brief  Set psram clock Source.
  * @param  Source: This parameter can be one of the following values:
  *		 @arg BIT_LSYS_CKSL_PSRAM_CPUPLL
  *		 @arg BIT_LSYS_CKSL_PSRAM_DSPPLL
  *		 @arg BIT_LSYS_CKSL_PSRAM_LBUS
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_PSRAM_ClkSet(u8 Source)
{
	u32 cur_src = LSYS_GET_CKSL_PSRAM(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0));

	if (Source != cur_src) {
		if ((Source == BIT_LSYS_CKSL_PSRAM_LBUS) &&
			((cur_src == BIT_LSYS_CKSL_PSRAM_CPUPLL) || (cur_src == BIT_LSYS_CKSL_PSRAM_DSPPLL))) {
			//psram clk switch from pll to xtal
			PSRAM_AutoGating(DISABLE, 1, 16);
		} else if ((cur_src == BIT_LSYS_CKSL_PSRAM_LBUS) &&
				   ((Source == BIT_LSYS_CKSL_PSRAM_CPUPLL) || (Source == BIT_LSYS_CKSL_PSRAM_DSPPLL))) {
			//psram clk switch from xtal to pll
			PSRAM_AutoGating(DISABLE, 10, 3);
		}

		RCC_PeriphClockSource_PSRAM(Source);

		if ((Source == BIT_LSYS_CKSL_PSRAM_LBUS) &&
			((cur_src == BIT_LSYS_CKSL_PSRAM_CPUPLL) || (cur_src == BIT_LSYS_CKSL_PSRAM_DSPPLL))) {
			//psram clk switch from pll to xtal
			PSRAM_AutoGating(ENABLE, 10, 3);
		} else if ((cur_src == BIT_LSYS_CKSL_PSRAM_LBUS) &&
				   ((Source == BIT_LSYS_CKSL_PSRAM_CPUPLL) || (Source == BIT_LSYS_CKSL_PSRAM_DSPPLL))) {
			//psram clk switch from xtal to pll
			PSRAM_AutoGating(ENABLE, 1, 16);
		}
	}
}


IMAGE2_RAM_TEXT_SECTION
void SOCPS_Voltage_Switch(u8 sleep)
{
	/* active is 1.0V, need switch voltage */
	if (SWR_CORE_Vol_Get() == 0x01) {
		if (sleep) {
			/* enter SLP: reduce voltage to LDO@0.9V SWR@PWM */
			SWR_Calib_ReduceVoltage();
		} else {
			/* exit SLP: raise voltage to LDO@1.0V SWR@PWM */
			SWR_Calib_RaiseVoltage();
		}
	}
}


/**
  * @brief  before NP enter CG/PG, NP should config some modules for power save
  * @param  type: CG/PG
  * @retval None
  */
_OPTIMIZE_NONE_
IMAGE2_RAM_TEXT_SECTION
void SOCPS_AP_suspend_config(u32 type, u32 Protection)
{
	(void)type;

	u32 Rtemp = 0;
	u32 PreState_irq = 0;

	if (Protection) {
		PreState_irq = irq_disable_save();
	}

	PSRAM_CLK = LSYS_GET_CKSL_PSRAM(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0));
	SOCPS_PSRAM_ClkSet(BIT_LSYS_CKSL_PSRAM_LBUS);

	/* PLL:
	1. switch modules that used PLL to XTAL
	2. PLL off */
	/*switch lbus clk to xtal*/
	LBUS_CLK = LSYS_GET_CKSL_LBUS(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0));
	if (LSYS_GET_CKSL_LBUS(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0))) {
		//current lbus clk is xtal
	} else {
		//current lbus clk is osc4m
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0);
		Rtemp |= LSYS_BIT_CKSL_LBUS;
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0, Rtemp);
	}

	/* 1. SPIC clock source switch to lbus clk */
	SPIC_CLK = LSYS_GET_CKSL_SPIC(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0));
	if (SYSCFG_CHIPType_Get() == CHIP_TYPE_FPGA) {
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0);
		Rtemp &= ~(LSYS_MASK_CKSL_SPIC);
		Rtemp |= LSYS_CKSL_SPIC(BIT_LSYS_CKSL_SPIC_LBUS);
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0, Rtemp);
	} else {
		/* switch clock to XTAL, disable dummy cycle and diable cal */
		FLASH_ClockSwitch(BIT_LSYS_CKSL_SPIC_LBUS, 0);
	}

	/* 2. hifi clock source switch to lbus clk */
	HIFI_CLK = LSYS_GET_CKSL_HIFI(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0));
	if (!HIFI_CLK) {
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0);
		Rtemp |= LSYS_BIT_CKSL_HIFI;
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0, Rtemp);
	}

	PLFM_CLK = LSYS_GET_CKSL_PLFM(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0));
	CPU_ClkSet(CLK_CPU_LBUS);

	PLL_CPU(DISABLE);
	PLL_DSP(DISABLE);
	PLL_BG(DISABLE);

	/* add 5mA load for 0.9v ldo, can decrease LPS SWR@PWM mode power */
	REGU_TypeDef *REGU = REGU_BASE;
	Rtemp = REGU->REGU_LDO_1209_CTRL;
	Rtemp &= ~REGU_MASK_REG_DMYLOAD_L_1209;
	REGU->REGU_LDO_1209_CTRL = Rtemp;

	if (Protection) {
		irq_enable_restore(PreState_irq);
	}
}


/**
  * @brief  after NP exit CG/PG, NP should config some modules
  * @param  type: CG/PG
  * @retval None
  */
_OPTIMIZE_NONE_
IMAGE2_RAM_TEXT_SECTION
void SOCPS_AP_resume_config(u32 type, u32 Protection)
{
	(void)type;

	u32 Rtemp = 0;
	u32 PreState_irq = 0;

	if (Protection) {
		PreState_irq = irq_disable_save();
	}

	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKE_GRP1);
	Rtemp &= ~APBPeriph_PLL_CLOCK;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKE_GRP1, Rtemp);

	/* open PLL */
	PLL_BG(ENABLE);
	PLL_CPU(ENABLE);
	PLL_DSP(ENABLE);

	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKE_GRP1);
	Rtemp |= APBPeriph_PLL_CLOCK;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKE_GRP1, Rtemp);

	//TRNG need pll, so after open pll, should init trng again
	RCC_PeriphClockCmd(APBPeriph_TRNG, APBPeriph_TRNG_CLOCK, ENABLE);

	/* switch to PLL */
	/* 1. Plaform clock source switch to pll clk */
	CPU_ClkSet(PLFM_CLK);

	/* 2. SPIC clock source switch to pll clk */
	if (SYSCFG_CHIPType_Get() == CHIP_TYPE_FPGA) {
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0);
		Rtemp &= ~(LSYS_MASK_CKSL_SPIC);
		Rtemp |= LSYS_CKSL_SPIC(SPIC_CLK);
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0, Rtemp);
	} else {
		FLASH_ClockSwitch(SPIC_CLK, 0);
	}

	/* 3. hifi clock source switch to pll clk */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0);
	Rtemp &= ~(LSYS_BIT_CKSL_HIFI);
	Rtemp |= LSYS_CKSL_HIFI(HIFI_CLK);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0, Rtemp);

	/* 4. psram clock source switch to pll */
	SOCPS_PSRAM_ClkSet(PSRAM_CLK);

	if (Protection) {
		irq_enable_restore(PreState_irq);
	}
}


IMAGE2_RAM_TEXT_SECTION
void SOCPS_NP_suspend_and_resume(u32 type, u8 enable)
{
	u32 Rtemp;
	RRAM_TypeDef *rram = RRAM_DEV;

	if (enable) {
		SOCPS_OSC4M_CTRL(ENABLE);

		/* LBUS clock source switch to XTAL40M */
		if (LSYS_GET_CKSL_LBUS(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0))) {
			//current lbus clk is xtal
		} else {
			//current lbus clk is osc4m
			Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0);
			Rtemp |= LSYS_BIT_CKSL_LBUS;
			HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0, Rtemp);
		}

		if (SYSCFG_OTP_FlashDSleepEn()) {
			FLASH_DeepPowerDown(DISABLE);
			/* to fix hw bug: between flash user read and auto read wrap, need single auto read, otherwise SPIC may return error */
			Rtemp = HAL_READ32(SPI_FLASH_BASE, 0);
		}

		SOCPS_Voltage_Switch(DISABLE);

		if (rram->PMC_CORE_ROLE_Flag == PMC_CORE_ROLE_SINGLE) {
			SOCPS_AP_resume_config(type, DISABLE);
		}

	} else {
		if (rram->PMC_CORE_ROLE_Flag == PMC_CORE_ROLE_SINGLE) {
			SOCPS_AP_suspend_config(type, DISABLE);
		}

		SOCPS_Voltage_Switch(ENABLE);

		SOCPS_OSC4M_CTRL(DISABLE);

		FLASH_DeepPowerDown(ENABLE);
	}
}

