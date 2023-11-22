/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#include "ameba_soc.h"

static const char *TAG = "PMC";
u32 PMC_MemMode_BK[6];

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

/**
  * @brief  set power option for power save mode.
  * @param  pwrmgt_option: power save mode option.
  *          This parameter can be one or combinations of the following values:
  *            @arg BIT_LSYS_PST_NORM_EXTL: Enable XTAL when PMC exit from sleep mode0
  *            @arg BIT_LSYS_PST_NORM_EPSW: LS power keeps unchange when PMC exit from sleep mode0
  *            @arg BIT_LSYS_PST_DSLP_EACK: Enable ANA4M when PMC enter into deep sleep mode0
  *            @arg BIT_LSYS_PST_SLEP_XACT: XTAL keeps at active mode when PMC enter into sleep mode0
  *            @arg BIT_LSYS_PST_SLEP_EXTL:  Enable XTAL when PMC enter into sleep mode0
  *            @arg BIT_LSYS_PST_SLEP_ESWR: Enable SWR when PMC enter into sleep mode0
  *            @arg BIT_LSYS_PST_SLEP_EPWM: SWR keeps at PWM mode when PMC enter into sleep mode0
  *            @arg BIT_LSYS_PST_SLEP_DPSW: Keep LS power unchange when PMC enter into sleep mode0
  *            @arg BIT_LSYS_PST_SLEP_ERCK: Enable LS SYSON register clock when PMC enter into sleep mode0
  *            @arg BIT_LSYS_PST_SLEP_LDLM: LPLDO keeps at active mode when PMC enter into sleep mode
  *            @arg BIT_LSYS_PST_SLEP_EMPM: emory power control setting don't change when PMC enter into sleep mode
  *            @arg BIT_LSYS_PST_SLEP_EBUS: Enable platform clock when PMC entro into sleep mode
  *            @arg BIT_LSYS_PST_SLEP_EACK: Enable ANA4M CLK when PMC enter into sleep mode
  *            @arg BIT_LSYS_PST_SLEP_ESOC: power enable SOC platform when PMC enter into sleep mode (clock-gating)
  * @retval None
  */
void SOCPS_PWROption(u32 pwrmgt_option)
{
	HAL_WRITE32(PMC_BASE, SYSPMC_OPT, pwrmgt_option);
}

/**
  * @brief  get aon wake reason.
  * @param  None
  * @retval aon wake up reason
  *		This parameter can be one or combination of the following values:
  *		 @arg AON_BIT_RTC_ISR_EVT
  *		 @arg AON_BIT_GPIO_PIN3_WAKDET_EVT
  *		 @arg AON_BIT_GPIO_PIN2_WAKDET_EVT
  *		 @arg AON_BIT_GPIO_PIN1_WAKDET_EVT
  *		 @arg AON_BIT_GPIO_PIN0_WAKDET_EVT
  *		 @arg AON_BIT_TIM_ISR_EVT
  *		 @arg AON_BIT_BOR_ISR_EVT
  *		 @arg AON_BIT_CHIPEN_LP_ISR_EVT
  *		 @arg AON_BIT_CHIPEN_SP_ISR_EVT
  */
int SOCPS_AONWakeReason(void)
{
	int reason = 0;

	reason = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_AON_WAK_EVT);

	return reason;
}

/**
  * @brief  lower aon vol before sleep and raise vol after wake.
  * @param  status: enable or disable
  *          This parameter can be  one of the following values:
  *            @arg 1: ENABLE
  *            @arg 0: DISABLE
  */
static void OTP_Raise_AonVol(u32 status)
{
	if (SYSCFG_RLVersion() == SYSCFG_CUT_VERSION_A) {
		return;
	} else {
		u32 Aon_Vol;
		REGU_TypeDef *regu = REGU_BASE;

		if (status == ENABLE) {
			/* raise aon vol by 1 level before oprate otp */
			Aon_Vol = regu->REGU_AON_LDO_CTRL;
			regu->REGU_AON_LDO_CTRL = (Aon_Vol & ~(REGU_MASK_AONLDO_VOSEL_H)) | REGU_AONLDO_VOSEL_H(OTP_AON_VOL);

		} else {
			/* restore aon vol */
			Aon_Vol = regu->REGU_AON_LDO_CTRL;
			regu->REGU_AON_LDO_CTRL = (Aon_Vol & ~(REGU_MASK_AONLDO_VOSEL_H)) | REGU_AONLDO_VOSEL_H(NORMAL_AON_VOL);

		}
	}

}

void SOCPS_CLK_SwitchToLow(u32 status)
{
	u32 Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);

	if (status == ENABLE) {
		// swtich ADC & GPIO int to OSC4M from lbus
		// CTC todo, hipc done when off NP, SPIC seems no need
		Rtemp |= LSYS_BIT_CKSL_GPIO | LSYS_BIT_CKSL_ADC;
	} else {
		Rtemp &= ~(LSYS_BIT_CKSL_GPIO | LSYS_BIT_CKSL_ADC);
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);
}

SRAM_ONLY_TEXT_SECTION
VOID SOCPS_SleepCG(VOID)
{
	u32 nDeviceIdOffset = PMU_MAX;

	if (ps_config.km0_tickles_debug) {
		RTK_LOGD(TAG, "KM0CG \n");
	}

	//SOCPS_Hplat_OFF();

	/* exec sleep hook functions */
	nDeviceIdOffset = pmu_exec_sleep_hook_funs();
	if (nDeviceIdOffset != PMU_MAX) {
		//pmu_exec_wakeup_hook_funs(nDeviceIdOffset);
		if (ps_config.km0_tickles_debug) {
			RTK_LOGD(TAG, "DBG: KM0 Sleep CG blocked because Dev %x  busy\n", nDeviceIdOffset);
		}
		return;
	}

	/* switch chipen inti intr mode to wakeup system*/
	CHIPEN_WorkMode(CHIPEN_INT_RESET_MODE);

	/* switch IP clk to OSC4M, so that can wakeup system when need */
	SOCPS_CLK_SwitchToLow(ENABLE);

	OTP_Raise_AonVol(DISABLE);
	SOCPS_SleepCG_LIB();
	OTP_Raise_AonVol(ENABLE);

	/* switch IP clk to lsbus */
	SOCPS_CLK_SwitchToLow(DISABLE);

	/* sys req pfm mode when only km0 */
	SWR_PFM_MODE_Set(ENABLE);

	/* exec sleep hook functions */
	pmu_exec_wakeup_hook_funs(PMU_MAX);

	if (ps_config.km0_tickles_debug) {
		RTK_LOGD(TAG, "KM0CG- %x %x\n", HAL_READ32(PMC_BASE, WAK_STATUS0), HAL_READ32(PMC_BASE, WAK_STATUS1));
	}
}

/* keep power functions: UART/I2C/RTC/GPIO/Gtimer/REGU/ANAtimer */
/* close power functions: GDMA */
_OPTIMIZE_NONE_
SRAM_ONLY_TEXT_SECTION
VOID SOCPS_SleepPG(VOID)
{
	u32 nDeviceIdOffset = 0;//0

	if (ps_config.km0_tickles_debug) {
		RTK_LOGD(TAG, "SOCPS_SleepPG \n");
	}

	/* exec sleep hook functions */
	nDeviceIdOffset = pmu_exec_sleep_hook_funs();
	if (nDeviceIdOffset != PMU_MAX) {
		pmu_exec_wakeup_hook_funs(nDeviceIdOffset);

		if (ps_config.km0_tickles_debug) {
			RTK_LOGD(TAG, "DBG: KM0 Sleep PG blocked because Dev %x  busy\n", nDeviceIdOffset);
		}
		return;
	}

	//SOCPS_Hplat_OFF();

	/* switch chipen inti intr mode to wakeup system*/
	CHIPEN_WorkMode(CHIPEN_INT_RESET_MODE);

	/* switch IP clk to OSC4M, so that can wakeup system when need */
	SOCPS_CLK_SwitchToLow(ENABLE);

	OTP_Raise_AonVol(DISABLE);
	SOCPS_SleepPG_LIB();
	OTP_Raise_AonVol(ENABLE);

	/* switch IP clk to lsbus */
	SOCPS_CLK_SwitchToLow(DISABLE);

	/* exec sleep hook functions */
	pmu_exec_wakeup_hook_funs(PMU_MAX);
}

/**
  *  @brief set work modules/wake up event after sleep.
  *  @retval None
  */
void SOCPS_SleepInit(void)
{

	int i = 0;
	static u32 km0cg_pwrmgt_config_val;

	/* shrink process time */
	if (km0cg_pwrmgt_config_val != 0) {
		for (i = 0; i < 6; i++) {
			if (sleep_sram_config[i].Module == 0xFFFFFFFF) {
				break;
			}
			HAL_WRITE32(SYSTEM_MEM_CTRL_BASE, sleep_sram_config[i].Module, PMC_MemMode_BK[i]);
		}

		goto end;
	}

	/*power management setting*/
	km0cg_pwrmgt_config_val = HAL_READ32(PMC_BASE, SYSPMC_OPT);

	if (ps_config.keep_OSC4M_on) {
		km0cg_pwrmgt_config_val |= PMC_BIT_PST_SLEP_ERCK;
	} else {
		km0cg_pwrmgt_config_val &= ~PMC_BIT_PST_SLEP_ERCK;
	}

	if (ps_config.swr_mode_in_sleep == SWR_PWM) {
		km0cg_pwrmgt_config_val |= PMC_BIT_PST_SLEP_EPWM;
	} else {
		km0cg_pwrmgt_config_val &= ~PMC_BIT_PST_SLEP_EPWM;
	}

	km0cg_pwrmgt_config_val &= (~PMC_MASK_PST_SLEP_XMD);
	km0cg_pwrmgt_config_val |= PMC_PST_SLEP_XMD(ps_config.xtal_mode_in_sleep);

	HAL_WRITE32(PMC_BASE, SYSPMC_OPT, km0cg_pwrmgt_config_val);
	RTK_LOGI(TAG, "SYSPMC_OPT %x\n", HAL_READ32(PMC_BASE, SYSPMC_OPT));

	/* hs sram option init */
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

		if (sleep_wakepin_config[i].config != DISABLE_WAKEPIN) {
			SOCPS_SetWakepin(sleep_wakepin_config[i].wakepin, sleep_wakepin_config[i].config);
		}

		i++;
	}
end:
	;
}

/**
  *  @brief Get dslp wake status.
	* @param none:

	* @retval DslpWake status.
	* 		   @arg TRUE: boot from deep sleep
	* 		   @arg FALSE: boot from power on
  */
u32 SOCPS_DsleepWakeStatusGet(void)
{
	u32 Temp = BOOT_Reason();

	if (Temp & AON_BIT_RSTF_DSLP) {
		return TRUE;
	} else {
		return FALSE;
	}
}

u32 LPWNP_INTHandler(UNUSED_WARN_DIS void *Data)
{
	UNUSED(Data);
	RTK_LOGI(TAG, "LP WAKE NP HANDLER %x %x\n",
			 HAL_READ32(PMC_BASE, WAK_STATUS0), HAL_READ32(PMC_BASE, WAK_STATUS1));

	InterruptDis(NP_WAKE_IRQ);
	np_resume();
	return TRUE;
}

u32 LPWAP_INTHandler(UNUSED_WARN_DIS void *Data)
{
	UNUSED(Data);

	HAL_WRITE8(SYSTEM_CTRL_BASE_LP, REG_LSYS_AP_STATUS_SW,
			   HAL_READ8(SYSTEM_CTRL_BASE_LP, REG_LSYS_AP_STATUS_SW) | LSYS_BIT_AP_RUNNING);

	RTK_LOGI(TAG, "LP WAKE AP HANDLER %x %x\n",
			 HAL_READ32(PMC_BASE, WAK_STATUS0), HAL_READ32(PMC_BASE, WAK_STATUS1));

	InterruptDis(AP_WAKE_IRQ);

	ap_resume();
	return TRUE;
}

/**
  *  @brief set wake event.
  *  @retval None
  */
void SOCPS_WakeEvent_Init(void)
{
	u32 i;
	u32 sleep_wevent_config_val[3][2] = {0};
	u32 group = 0;
	u32 wakebit = 0;

	/*wake event setting*/
	for (i = 0;;) {
		/*  Check if search to end */
		if (sleep_wevent_config[i].Module == 0xFFFFFFFF) {
			break;
		}

		/* decide group */
		if ((sleep_wevent_config[i].Module & WAKE_MASK1_CHECK) == WAKE_MASK1_CHECK) {
			group = 1;
			wakebit = sleep_wevent_config[i].Module & (~WAKE_MASK1_CHECK);
		} else {
			group = 0;
			wakebit = sleep_wevent_config[i].Module;
		}

		if (sleep_wevent_config[i].wakeup == WAKEUP_LP) {
			sleep_wevent_config_val[0][group] |= wakebit;
		}

		if (sleep_wevent_config[i].wakeup == WAKEUP_NP) {
			sleep_wevent_config_val[1][group] |= wakebit;
		}

		if (sleep_wevent_config[i].wakeup == WAKEUP_AP) {
			sleep_wevent_config_val[2][group] |= wakebit;
		}
		i++;
	}

	SOCPS_SetNPWakeEvent_MSK0(sleep_wevent_config_val[1][0], ENABLE);
	SOCPS_SetNPWakeEvent_MSK1(sleep_wevent_config_val[1][1], ENABLE);
	RTK_LOGI(TAG, "NP wake event: %x %x\n", sleep_wevent_config_val[1][0], sleep_wevent_config_val[1][1]);

	if ((sleep_wevent_config_val[1][0] | sleep_wevent_config_val[1][1])) {
		InterruptRegister(LPWNP_INTHandler, NP_WAKE_IRQ, (u32)PMC_BASE, INT_PRI_MIDDLE);
		/* enable when suspend KM4 */
		//InterruptEn(NP_WAKE_IRQ, 10);

		/* open NP wake in LP*/
		sleep_wevent_config_val[0][0] |= WAKE_SRC_NP_WAKE;
	}

	SOCPS_SetAPWakeEvent_MSK0(sleep_wevent_config_val[2][0], ENABLE);
	SOCPS_SetAPWakeEvent_MSK1(sleep_wevent_config_val[2][1], ENABLE);
	RTK_LOGI(TAG, "AP wake event %x %x\n", sleep_wevent_config_val[2][0], sleep_wevent_config_val[2][1]);

	if ((sleep_wevent_config_val[2][0] | sleep_wevent_config_val[2][1])) {
		InterruptRegister(LPWAP_INTHandler, AP_WAKE_IRQ, (u32)PMC_BASE, INT_PRI_MIDDLE);
		//InterruptEn(AP_WAKE_IRQ, 5);

		/* open AP wake in LP*/
		sleep_wevent_config_val[0][0] |= WAKE_SRC_AP_WAKE;
	}

	SOCPS_SetLPWakeEvent_MSK0(sleep_wevent_config_val[0][0], ENABLE);
	SOCPS_SetLPWakeEvent_MSK1(sleep_wevent_config_val[0][1], ENABLE);
	RTK_LOGI(TAG, "LP wake event %x %x\n", HAL_READ32(PMC_BASE, WAK_MASK0_LP), HAL_READ32(PMC_BASE, WAK_MASK1_LP));

}

/**
  * @brief  set lp wake up event mask0.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
void SOCPS_SetLPWakeEvent_MSK0(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;

	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, WAK_MASK0_LP);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, WAK_MASK0_LP, WakeEvent);
}

/**
  * @brief  set lp wake up event mask1.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
void SOCPS_SetLPWakeEvent_MSK1(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;

	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, WAK_MASK1_LP);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, WAK_MASK1_LP, WakeEvent);
}

/**
  * @brief  set lp wake up event.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
void SOCPS_SetLPWakeEvent(u32 Option, u32 Group, u32 NewStatus)
{
	if (Group) {
		SOCPS_SetLPWakeEvent_MSK1(Option, NewStatus);
	} else {
		SOCPS_SetLPWakeEvent_MSK0(Option, NewStatus);
	}
}

/**
  * @brief  set np wake up event mask0.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
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
void SOCPS_SetAPWakeEvent(u32 Option, u32 Group, u32 NewStatus)
{
	if (Group) {
		SOCPS_SetAPWakeEvent_MSK1(Option, NewStatus);
	} else {
		SOCPS_SetAPWakeEvent_MSK0(Option, NewStatus);
	}
}

/**
  * @brief  clear all of wake_pin interrupt(Max. 4 wake_pin).
  * @retval None
  *
  */
void SOCPS_ClearWakePin(void)
{
#ifdef 	AMEBAD2_TODO

	u32 Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_WAKE_GPIO_CTRL2);

	Rtemp |= (BIT_AON_MASK_GPIO_WAKE_EVENT << BIT_AON_SHIFT_GPIO_WAKE_EVENT);//0x0F000000;
	Rtemp &= (~(BIT_AON_MASK_GPIO_WAKE_PUPD_EN << BIT_AON_SHIFT_GPIO_WAKE_PUPD_EN)); /* close internal PU/PD resistence */

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_WAKE_GPIO_CTRL2, Rtemp);
#endif
}

/**
  *  @brief set power cut stable time.
  *  @retval None
  */
void SOCPS_SetPowerCut_Time(u32 time)
{
	u32 temp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	temp &= ~PMC_MASK_STL_PDSOC;

	temp |= PMC_STL_PDSOC(time);
	//set 0 for 8*2^5 = 256us
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, temp);
}

