/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#include "ameba_soc.h"

u32 PMC_MemMode_BK[6];

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
  * @brief  get wakepin index.
  * @param  None
  * @retval BIT(0): wakepin0, BIT(1): wakepin1, BIT(2): wakepin2, BIT(3): wakepin3
  */
int SOCPS_WakePinCheck(void)
{
	int WakePin = 0;

	WakePin = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_AON_WAK_EVT);
	WakePin = WakePin >> 4;

	return WakePin;
}

/**
  * @brief  set AON timer for wakeup.
  * @param  SDuration: sleep time, unit is ms
  * @note wakeup state:sleep PG & CG & deep sleep
  * @note This is 100KHz timer, max counter = 0xFFFFFFFF/100000/60 = 715min
  * @retval None
  */
void SOCPS_AONTimer(u32 SDuration)
{
	if (SDuration == 0) {
		return;
	}

	SDuration = SDuration * 100;

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_SET, AON_TIM_CNT_CUR(SDuration));
}

/**
  * @brief  enable AON timer interrupt.
  * @param  Status: interrupt status
  * @retval None
  */
void SOCPS_AONTimerINT_EN(u32 Status)
{
	u32 Rtemp = 0;

	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_IER);
	if (ENABLE == Status) {
		Rtemp |= AON_BIT_TIM_IE;
	} else {
		Rtemp &= (~AON_BIT_TIM_IE);

	}
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_IER, Rtemp);
}

/**
  * @brief  get current AON timer.
  * @param  None
  * @note wakeup state:sleep PG & CG & deep sleep
  * @retval The current Aon Timer counter, the unit is ms.
  */
u32 SOCPS_AONTimerGet(VOID)
{
	u32 Rtemp = 0;

	/* use 32K */
	Rtemp = AON_GET_TIM_CNT_CUR(HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_CNT));
	Rtemp = Rtemp / 100;
	return Rtemp;
}

/**
  * @brief  Clear aontimer interrupt.
  * @retval   None
  */
VOID SOC_AONTimerClearINT(void)
{
	u32 temp = 0;

	temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_ISR);
	temp |= AON_BIT_TIM_ISR;
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_TIM_ISR, temp);
}

/**
  * @brief  set REGU wakeup_pin (just on wakepin mux to 4 pads).
  * @param  PinMask: aon wakepin index
  *		This parameter can be one of the following values:
  *		 @arg WAKUP_0 :
  *		 @arg WAKUP_1 :
  *		 @arg WAKUP_2 :
  *		 @arg WAKUP_3 :
  * @param  Polarity: aon wakepin Polarity
  *		This parameter can be one of the following values:
  *		 @arg 1 : high wakeup
  *		 @arg 0 : low wakeup
  * @note wakeup state: sleep PG & CG & deep sleep
  * @retval None
  */
void SOCPS_SetWakepin(u32 PinIdx, u32 Polarity)
{
	u32 Rtemp = 0;

	/* disable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	Rtemp &= (~(AON_GPIO_WAKE_FEN(BIT(PinIdx))));
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);

	/* set polarity & internal PU/PD resistence */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	Rtemp &= ~(AON_GPIO_WAKDET_POLY(BIT(PinIdx))); /* clear polarity, set to low active */
	if (Polarity == 1) {
		Rtemp |= AON_GPIO_WAKDET_POLY(BIT(PinIdx)); /* if high active */
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);

	/* enable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	Rtemp |= (AON_GPIO_WAKE_FEN(BIT(PinIdx)));
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);
}

/**
  * @brief  set REGU wakeup_pin debounce cycle.
  * @param  Dbnc_cycle: wakeup_pin debounce cycle, unit is 100k/128k.
  *		This parameter must be set to a value in the 0-0x7FFF range.
  * @param  Status: enable or disable wakepin debounce function
  * @note Dbnc_cycle is valid when Status is set ENABLE.
  * @retval None
  */
void SOCPS_SetWakepinDebounce(u32 Dbnc_cycle, u32 Status)
{
	u32 Rtemp = 0;
	u32 FEN_backup = 0;
	assert_param(Dbnc_cycle <= 0x7FFF);

	/* disable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	FEN_backup = AON_GET_GPIO_WAKE_FEN(Rtemp);
	Rtemp &= (~AON_MASK_GPIO_WAKE_FEN);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);

	/* set gpio debounce cycle */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	if (ENABLE == Status) {
		Rtemp &= (~AON_MASK_GPIO_DBNC_CYC);
		Rtemp |= AON_GPIO_DBNC_CYC(Dbnc_cycle);
		Rtemp |= AON_BIT_GPIO_DBNC_FEN;
	} else {
		Rtemp &= (~AON_BIT_GPIO_DBNC_FEN);
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);

	/* enable wakepin */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL);
	Rtemp |= AON_GPIO_WAKE_FEN(FEN_backup);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_CTRL, Rtemp);
}

/**
  * @brief  Clear wakepin interrupt.
  * @retval   None
  */
VOID SOC_WakePinClearINT(u32 wakepin)
{
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_GPIO_WAKE_ISR, wakepin);
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

SRAM_ONLY_TEXT_SECTION
VOID SOCPS_SleepCG(VOID)
{
	u32 nDeviceIdOffset = PMU_MAX;

	if (ps_config.km0_tickles_debug) {
		DBG_8195A("KM0CG \n");
	}

	//SOCPS_Hplat_OFF();

	/* exec sleep hook functions */
	nDeviceIdOffset = pmu_exec_sleep_hook_funs();
	if (nDeviceIdOffset != PMU_MAX) {
		//pmu_exec_wakeup_hook_funs(nDeviceIdOffset);
		if (ps_config.km0_tickles_debug) {
			DBG_8195A("DBG: KM0 Sleep CG blocked because Dev %x  busy\n", nDeviceIdOffset);
		}
		return;
	}

	/* switch chipen inti intr mode to wakeup system*/
	CHIPEN_WorkMode(CHIPEN_INT_RESET_MODE);

	OTP_Raise_AonVol(DISABLE);
	SOCPS_SleepCG_LIB();
	OTP_Raise_AonVol(ENABLE);

	/* sys req pfm mode when only km0 */
	SWR_PFM_MODE_Set(ENABLE);

	/* exec sleep hook functions */
	pmu_exec_wakeup_hook_funs(PMU_MAX);

	if (ps_config.km0_tickles_debug) {
		DBG_8195A("KM0CG- %x %x\n", HAL_READ32(PMC_BASE, WAK_STATUS0), HAL_READ32(PMC_BASE, WAK_STATUS1));
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
		DBG_8195A("SOCPS_SleepPG \n");
	}

	/* exec sleep hook functions */
	nDeviceIdOffset = pmu_exec_sleep_hook_funs();
	if (nDeviceIdOffset != PMU_MAX) {
		pmu_exec_wakeup_hook_funs(nDeviceIdOffset);

		if (ps_config.km0_tickles_debug) {
			DBG_8195A("DBG: KM0 Sleep PG blocked because Dev %x  busy\n", nDeviceIdOffset);
		}
		return;
	}

	//SOCPS_Hplat_OFF();

	/* switch chipen inti intr mode to wakeup system*/
	CHIPEN_WorkMode(CHIPEN_INT_RESET_MODE);

	OTP_Raise_AonVol(DISABLE);
	SOCPS_SleepPG_LIB();
	OTP_Raise_AonVol(ENABLE);

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
	static u32 sleep_wevent_config_val[3][2];
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

	for (i = 0;;) {
		/*  Check if search to end */
		if (km0_pwrmgt_config[i].Module == 0xFFFFFFFF) {
			break;
		}

		if (km0_pwrmgt_config[i].Status == ON) {
			km0cg_pwrmgt_config_val |= km0_pwrmgt_config[i].Module;
		} else {
			km0cg_pwrmgt_config_val &= ~km0_pwrmgt_config[i].Module;
		}

		if (km0_pwrmgt_config[i].Module == PMC_MASK_PST_SLEP_VOLT) {
			km0cg_pwrmgt_config_val &= (~PMC_MASK_PST_SLEP_VOLT);
			km0cg_pwrmgt_config_val |= PMC_PST_SLEP_VOLT(km0_pwrmgt_config[i].Status);
		}

		if (km0_pwrmgt_config[i].Module == PMC_MASK_PST_SLEP_XMD) {
			km0cg_pwrmgt_config_val &= (~PMC_MASK_PST_SLEP_XMD);
			km0cg_pwrmgt_config_val |= PMC_PST_SLEP_XMD(km0_pwrmgt_config[i].Status);
		}

		if (km0_pwrmgt_config[i].Module == PMC_MASK_PST_SLEP_FBUS) {
			km0cg_pwrmgt_config_val &= (~PMC_MASK_PST_SLEP_FBUS);
			km0cg_pwrmgt_config_val |= PMC_PST_SLEP_FBUS(km0_pwrmgt_config[i].Status);
		}

		i++;
	}

	HAL_WRITE32(PMC_BASE, SYSPMC_OPT, km0cg_pwrmgt_config_val);
	DBG_8195A("SYSPMC_OPT %x\n", HAL_READ32(PMC_BASE, SYSPMC_OPT));

	/*wake event setting*/
	for (i = 0;;) {
		/*  Check if search to end */
		if (sleep_wevent_config[i].Module == 0xFFFFFFFF) {
			break;
		}

		if (sleep_wevent_config[i].Group == 1) {
			if (sleep_wevent_config[i].Msk_Lp == ON) {
				sleep_wevent_config_val[0][1] |= sleep_wevent_config[i].Module;
			} else {
				sleep_wevent_config_val[0][1] &= ~sleep_wevent_config[i].Module;
			}
		}

		if (sleep_wevent_config[i].Group == 0) {
			if (sleep_wevent_config[i].Msk_Lp == ON) {
				sleep_wevent_config_val[0][0] |= sleep_wevent_config[i].Module;
			} else {
				sleep_wevent_config_val[0][0] &= ~sleep_wevent_config[i].Module;
			}
		}
		i++;
	}

	sleep_wevent_config_val[1][0] = HAL_READ32(PMC_BASE, WAK_MASK0_NP);
	sleep_wevent_config_val[1][1] = HAL_READ32(PMC_BASE, WAK_MASK1_NP);
	sleep_wevent_config_val[2][0] = HAL_READ32(PMC_BASE, WAK_MASK0_AP);
	sleep_wevent_config_val[2][1] = HAL_READ32(PMC_BASE, WAK_MASK1_AP);

	if ((sleep_wevent_config_val[0][0] & sleep_wevent_config_val[1][0]) | \
		(sleep_wevent_config_val[0][0] & sleep_wevent_config_val[2][0]) | \
		(sleep_wevent_config_val[1][0] & sleep_wevent_config_val[2][0]) | \
		(sleep_wevent_config_val[0][1] & sleep_wevent_config_val[1][1]) | \
		(sleep_wevent_config_val[0][1] & sleep_wevent_config_val[2][1]) | \
		(sleep_wevent_config_val[1][1] & sleep_wevent_config_val[2][1])) {
		DBG_8195A("Warning: Wakeevent set conflict !!!\n");
	}

	if (sleep_wevent_config_val[1][0] | sleep_wevent_config_val[1][1]) {
		sleep_wevent_config_val[0][0] |= WAKE_SRC_NP_WAKE;
		DBG_8195A("NP wake event %x %x\n", HAL_READ32(PMC_BASE, WAK_MASK0_NP), HAL_READ32(PMC_BASE, WAK_MASK1_NP));
	}

	if (sleep_wevent_config_val[2][0] | sleep_wevent_config_val[2][1]) {
		sleep_wevent_config_val[0][0] |= WAKE_SRC_AP_WAKE;
		DBG_8195A("AP wake event %x %x\n", HAL_READ32(PMC_BASE, WAK_MASK0_AP), HAL_READ32(PMC_BASE, WAK_MASK1_AP));
	}

	SOCPS_SetLPWakeEvent_MSK0(sleep_wevent_config_val[0][0], ENABLE);
	SOCPS_SetLPWakeEvent_MSK1(sleep_wevent_config_val[0][1], ENABLE);

	DBG_8195A("LP wake event %x %x\n", HAL_READ32(PMC_BASE, WAK_MASK0_LP), HAL_READ32(PMC_BASE, WAK_MASK1_LP));

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

		if (sleep_wakepin_config[i].Status == ON) {
			SOCPS_SetWakepin(sleep_wakepin_config[i].wakepin, sleep_wakepin_config[i].Polarity);
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
	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "LP WAKE NP HANDLER %x %x\n",
			   HAL_READ32(PMC_BASE, WAK_STATUS0), HAL_READ32(PMC_BASE, WAK_STATUS1));

	InterruptDis(NP_WAKE_IRQ);
	np_resume();
	return TRUE;
}

/**
  *  @brief set wake event.
  *  @retval None
  */
void SOCPS_WakeEvent_Init(void)
{
	u32 i;
	u32 sleep_wevent_config_val[2] = {0};

	/*wake event setting*/
	for (i = 0;;) {
		/*  Check if search to end */
		if (sleep_wevent_config[i].Module == 0xFFFFFFFF) {
			break;
		}

		if (sleep_wevent_config[i].Group == 1) {
			if (sleep_wevent_config[i].Msk_Np == ON) {
				sleep_wevent_config_val[1] |= sleep_wevent_config[i].Module;
			} else {
				sleep_wevent_config_val[1] &= ~sleep_wevent_config[i].Module;
			}
		}

		if (sleep_wevent_config[i].Group == 0) {
			if (sleep_wevent_config[i].Msk_Np == ON) {
				sleep_wevent_config_val[0] |= sleep_wevent_config[i].Module;
			} else {
				sleep_wevent_config_val[0] &= ~sleep_wevent_config[i].Module;
			}
		}
		i++;
	}

	SOCPS_SetNPWakeEvent_MSK0(sleep_wevent_config_val[0], ENABLE);
	SOCPS_SetNPWakeEvent_MSK1(sleep_wevent_config_val[1], ENABLE);
	DBG_8195A("wake event: %x %x \r\n", sleep_wevent_config_val[0], sleep_wevent_config_val[1]);

	if ((sleep_wevent_config_val[0] | sleep_wevent_config_val[1])) {
		InterruptRegister(LPWNP_INTHandler, NP_WAKE_IRQ, (u32)PMC_BASE, INT_PRI_MIDDLE);
		/* enable when suspend KM4 */
		//InterruptEn(NP_WAKE_IRQ, 10);
	}
}

u32 LPWAP_INTHandler(UNUSED_WARN_DIS void *Data)
{
	UNUSED(Data);

	HAL_WRITE8(SYSTEM_CTRL_BASE_LP, REG_LSYS_AP_STATUS_SW,
			   HAL_READ8(SYSTEM_CTRL_BASE_LP, REG_LSYS_AP_STATUS_SW) | LSYS_BIT_AP_RUNNING);

	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "LP WAKE AP HANDLER %x %x\n",
			   HAL_READ32(PMC_BASE, WAK_STATUS0), HAL_READ32(PMC_BASE, WAK_STATUS1));

	InterruptDis(AP_WAKE_IRQ);

	ap_resume();
	return TRUE;
}

/**
  *  @brief set wake event.
  *  @retval None
  */
void SOCPS_AP_WakeEvent_Init(void)
{
	u32 i;
	u32 sleep_wevent_config_val[2] = {0};

	/*wake event setting*/
	for (i = 0;;) {
		/*  Check if search to end */
		if (sleep_wevent_config[i].Module == 0xFFFFFFFF) {
			break;
		}

		if (sleep_wevent_config[i].Group == 1) {
			if (sleep_wevent_config[i].Msk_Ap == ON) {
				sleep_wevent_config_val[1] |= sleep_wevent_config[i].Module;
			} else {
				sleep_wevent_config_val[1] &= ~sleep_wevent_config[i].Module;
			}
		}

		if (sleep_wevent_config[i].Group == 0) {
			if (sleep_wevent_config[i].Msk_Ap == ON) {
				sleep_wevent_config_val[0] |= sleep_wevent_config[i].Module;
			} else {
				sleep_wevent_config_val[0] &= ~sleep_wevent_config[i].Module;
			}
		}
		i++;
	}

	SOCPS_SetAPWakeEvent_MSK0(sleep_wevent_config_val[0], ENABLE);
	SOCPS_SetAPWakeEvent_MSK1(sleep_wevent_config_val[1], ENABLE);

	DBG_8195A("ap_suspend %x %x\n", sleep_wevent_config_val[0], sleep_wevent_config_val[1]);
	if ((sleep_wevent_config_val[0] | sleep_wevent_config_val[1])) {
		InterruptRegister(LPWAP_INTHandler, AP_WAKE_IRQ, (u32)PMC_BASE, INT_PRI_MIDDLE);
		//InterruptEn(AP_WAKE_IRQ, 5);
	}
}


/**
  *  @brief set deepsleep Option.
  *  @retval None
  */
void SOCPS_deepsleepInit(void)
{
	static u32 dslp_pwrmgt_config_val;

	dslp_pwrmgt_config_val = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_PMC_OPT);
	for (int i = 0;;) {
		/*  Check if search to end */
		if (aon_pwrmgt_config[i].Module == 0xFFFFFFFF) {
			break;
		}

		if (aon_pwrmgt_config[i].Status == ON) {
			dslp_pwrmgt_config_val |= aon_pwrmgt_config[i].Module;
		} else {
			dslp_pwrmgt_config_val &= ~aon_pwrmgt_config[i].Module;
		}

		i++;
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_PMC_OPT, dslp_pwrmgt_config_val);

	DBG_8195A("REG_AON_PMC_OPT 0x%x\n", HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_PMC_OPT));
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

