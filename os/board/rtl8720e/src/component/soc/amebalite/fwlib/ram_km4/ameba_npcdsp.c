/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"
u32 DSPSleepTick = 0;
static u32 dsp_sleep_type;


u32 dsp_status_on(void)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKE_GRP1);

	if (Temp & APBPeriph_HIFI_CLOCK) {
		return 1;
	} else {
		return 0;
	}
}

void dsp_power_off(void)
{
	if (!dsp_status_on()) {
		return;
	}

	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "DSPOFF\n");

	/* Amebalite_TODO poll DSP clock gate */

	RCC_PeriphClockCmd(APBPeriph_HIFI, APBPeriph_HIFI_CLOCK, DISABLE);

	pmu_release_wakelock(PMU_DSP_RUN);
}

void dsp_power_on(void)
{
	if (dsp_status_on()) {
		return;
	}

	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "DSPON\n");

	pmu_acquire_wakelock(PMU_DSP_RUN);

	RCC_PeriphClockCmd(APBPeriph_HIFI, APBPeriph_HIFI_CLOCK, ENABLE);

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_HIFI_CTRL1, 0x8100000);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_HIFI_CTRL0, HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_HIFI_CTRL0) | 0x4F);
}

void dsp_clock_gate(void)
{
	if (!dsp_status_on()) {
		return;
	}

	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "DSPCG\n");

	/* poll DSP sleep status */
#ifdef AMEBALITE_TODO
	u32 temp = 0;

	while (1) {
		temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PLAT_STATUS);	/*get KR4 sleep status*/
		if (temp & LSYS_BIT_KR4_SLP_MODE) {
			break;
		}
	}
#endif

	RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_HIFI_CLOCK, DISABLE);

	pmu_release_wakelock(PMU_DSP_RUN);
	pmu_release_wakelock(PMU_OS);
}


void dsp_clock_on(void)
{
	if (dsp_status_on()) {
		return;
	}

	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "DSPCW\n");

	/* enable DSP clock */
	RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_HIFI_CLOCK, ENABLE);
}



void dsp_resume(void)
{
	if (dsp_status_on()) {
		return;
	}

	pmu_acquire_wakelock(PMU_DSP_RUN);

	dsp_power_on();
}

u32 NPWDSP_INTHandler(UNUSED_WARN_DIS void *Data)
{
	UNUSED(Data);
	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "NP WAKE DSP HANDLER %x %x\n",
			   HAL_READ32(PMC_BASE, WAK_STATUS0), HAL_READ32(PMC_BASE, WAK_STATUS1));

	InterruptDis(DSP_WAKE_IRQ);
	dsp_resume();
	return TRUE;
}


u32 dsp_suspend(u32 type)
{
	UNUSED(type);

	u32 ret = _SUCCESS;
	u32 sleep_wevent_config_val[2] = {0};

	SLEEP_ParamDef *sleep_param;
	u32 duration = 0;
	sleep_param = (SLEEP_ParamDef *)ipc_get_message(IPC_DSP_TO_KM4, IPC_D2M_TICKLESS_INDICATION);

	if (sleep_param != NULL) {
		duration = sleep_param->sleep_time;
	}

	if (duration > 0) {
		/* used for resume delay */
	}

	sleep_wevent_config_val[1] = HAL_READ32(PMC_BASE, WAK_MASK1_DSP);
	sleep_wevent_config_val[0] = HAL_READ32(PMC_BASE, WAK_MASK0_DSP);

	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "dsp_suspend %x %x\n", sleep_wevent_config_val[0], sleep_wevent_config_val[1]);
	if ((sleep_wevent_config_val[0] | sleep_wevent_config_val[1])) {
		InterruptRegister(NPWDSP_INTHandler, DSP_WAKE_IRQ, (u32)PMC_BASE, INT_PRI3);
		InterruptEn(DSP_WAKE_IRQ, INT_PRI3);
	}

	if (type == SLEEP_CG) {
		dsp_clock_gate();
	} else {
		dsp_power_off();
	}

	return ret;

}


void dsp_tickless_ipc_int(UNUSED_WARN_DIS VOID *Data, UNUSED_WARN_DIS u32 IrqStatus, UNUSED_WARN_DIS u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);
	SLEEP_ParamDef *psleep_param;

	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "dsp_tickless_ipc_int\n");

	psleep_param = (SLEEP_ParamDef *)ipc_get_message(IPC_DSP_TO_KM4, IPC_D2M_TICKLESS_INDICATION);
	DCache_Invalidate((u32)psleep_param, sizeof(SLEEP_ParamDef));

	//TODO: set dlps
	if (psleep_param->dlps_enable) {
		RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, ENABLE);
		if (psleep_param->sleep_time) {
			SOCPS_AONTimerClearINT();
			SOCPS_AONTimer(psleep_param->sleep_time);
			SOCPS_AONTimerINT_EN(ENABLE);
		}
		SOCPS_DeepSleep_RAM();
	}

	dsp_sleep_type = psleep_param->sleep_type;

	switch (psleep_param->sleep_type) {
	case SLEEP_PG:
		if (_SUCCESS == dsp_suspend(SLEEP_PG)) {
		}
		break;
	case SLEEP_CG:
		if (_SUCCESS == dsp_suspend(SLEEP_CG)) {
			pmu_set_sysactive_timeFromISR(2);
		}
		break;
	default:
		DBG_8195A("unknow sleep type\n");
	}

	DSPSleepTick = SYSTIMER_TickGet();
	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "T:%d, tms:%d\r\n", DSPSleepTick, (((DSPSleepTick & 0xFFFF8000) / 32768) * 1000 + ((DSPSleepTick & 0x7FFF) * 1000) / 32768));
}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   ipc_km4cdsp_table[] = {
	{IPC_USER_DATA, 	dsp_tickless_ipc_int,	(VOID *) NULL, IPC_DSP_TO_KM4, IPC_D2M_TICKLESS_INDICATION, IPC_RX_FULL},
};