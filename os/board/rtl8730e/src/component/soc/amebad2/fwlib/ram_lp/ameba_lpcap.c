/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

static const char *TAG = "LPCAP";
u32 ap_sleep_timeout = 0xffffffff;
u8 ap_sleep_type;
u32 ap_pll_backup;
u32 APDslpEn;
// 0x1 for core 0, 0x3 for core 0/1
#define CORE_NUM 0x1
void ap_power_on_ctrl(void)
{
	CA32_TypeDef *ca32 = CA32_BASE;

	/* step 0: AP L1/L2 Active mode */
	ca32->CA32_SRAM_CTRL2 &= (~(CA32_MASK_CORE_SRAM_DS | CA32_BIT_L2_SRAM_RET));

	/* step 1: set ISO */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO, (HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO) | HSYS_BIT_ISO_HP_AP_TOP));
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO, (HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO) | (HSYS_ISO_HP_AP_CORE(0x3))));

	/* step 2: open top power */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC) | HSYS_BIT_PSW_HP_AP_TOP);
	DelayUs(50);
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC) | HSYS_BIT_PSW_HP_AP_TOP_2ND);

	/* step 3: open ap l2 cache ram power */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC) | HSYS_BIT_PSW_HP_AP_L2);

	/* step 4: open core 0/1 power */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC) | HSYS_PSW_HP_AP_CORE(CORE_NUM));
	DelayUs(50);
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC) | HSYS_PSW_HP_AP_CORE_2ND(CORE_NUM));

	/*delay until power stable*/
	DelayUs(500);

	/* step 5: release isolation */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO, (HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO) & (~(HSYS_ISO_HP_AP_CORE(CORE_NUM)))));

	/* step 6: release top isolation */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO, (HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO) & (~HSYS_BIT_ISO_HP_AP_TOP)));

	if (RRAM->APPLL_STATE == TRUE) {
		/* open ap PLL, Max frequency is 1.08G@0.9V; 1.48G@1.0V*/
		PLL_AP(ENABLE);
	}
	/* enable ap clock */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE) | HSYS_BIT_CKE_AP);

	/* enable ap fen */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_FEN, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_FEN) | HSYS_BIT_FEN_AP);

	ca32->CA32_C0_RST_CTRL |= (CA32_NCOREPORESET(CORE_NUM) | CA32_MASK_NCORERESET | CA32_BIT_NRESETSOCDBG | CA32_BIT_NL2RESET | CA32_BIT_NGICRESET);
}

void ap_power_off_ctrl(void)
{
	APLL_TypeDef *apll = APLL_BASE;
	CA32_TypeDef *ca32 = CA32_BASE;

	ap_pll_backup = APLL_GET_SDM_DIVN(apll->APLL_CTRL5);

	/* step 1: disable ap clock */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE) & (~HSYS_BIT_CKE_AP));

	/* step 2: disable ap fen */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_FEN, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_FEN) & (~HSYS_BIT_FEN_AP));

	if (RRAM->APPLL_STATE == TRUE) {
		PLL_AP(DISABLE);
	}
	/* step 4: set ISO */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO, (HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO) | HSYS_BIT_ISO_HP_AP_TOP));
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO, (HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO) | (HSYS_ISO_HP_AP_CORE(0x3))));

	/* step 5: close core 0/1 power */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC) & (~HSYS_MASK_PSW_HP_AP_CORE_2ND));
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC) & (~HSYS_MASK_PSW_HP_AP_CORE));

	/* step 6: close top power */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC) & (~HSYS_BIT_PSW_HP_AP_TOP_2ND));
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC) & (~HSYS_BIT_PSW_HP_AP_TOP));

	/* step 7: close ap l2 cache ram power */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC) & (~HSYS_BIT_PSW_HP_AP_L2));

	/* AP L1 Cache DS mode and L2 RET mode */
	ca32->CA32_SRAM_CTRL2 |= (CA32_MASK_CORE_SRAM_DS | CA32_BIT_L2_SRAM_RET);

}


void ap_power_gate(void)
{
	//u8 wait_cnt = 10;
	CA32_TypeDef *ca32 = CA32_BASE;

	if ((HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_FEN) & HSYS_BIT_FEN_AP) == 0) {
		RTK_LOGI(TAG, "AP PG Already\n");
		return;
	}

	/* check core0 WFI state */
	while (1) {
		if (ca32->CA32_C0_CPU_STATUS & CA32_STANDBYWFE_CORE0) {
			break;
		}
	}

	ap_power_off_ctrl();
	pmu_release_wakelock(PMU_AP_RUN);
	if (APDslpEn) {
		pmu_release_deepwakelock(PMU_AP_RUN);
	}

	RTK_LOGI(TAG, "CA7PG-\n");
}

void ap_power_on(void)
{
	if (HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_FEN) & HSYS_BIT_FEN_AP) {
		RTK_LOGI(TAG, "AP PW Already\n");
		return;
	}
	pmu_acquire_wakelock(PMU_AP_RUN);
	pmu_acquire_deepwakelock(PMU_AP_RUN);

	ap_power_on_ctrl();

	RTK_LOGI(TAG, "CA7PW-\n");
}

void ap_clk_gate_ctrl(void)
{
	CA32_TypeDef *ca32 = CA32_BASE;
	/* disable CA7 clock */
	//HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE) & (~HSYS_BIT_CKE_AP));

	if (RRAM->APPLL_STATE == TRUE) {
		/* disable CA7 PLL*/
		PLL_AP(DISABLE);
	}

	/* AP L1 Cache DS mode and L2 RET mode */
	ca32->CA32_SRAM_CTRL2 |= (CA32_MASK_CORE_SRAM_DS | CA32_BIT_L2_SRAM_RET);
}

void ap_clk_wake_ctrl(void)
{
	CA32_TypeDef *ca32 = CA32_BASE;

	if (RRAM->APPLL_STATE == TRUE) {
		/* open CA7 PLL*/
		PLL_AP(ENABLE);
	}

	/* AP L1/L2 Active mode */
	ca32->CA32_SRAM_CTRL2 &= (~(CA32_MASK_CORE_SRAM_DS | CA32_BIT_L2_SRAM_RET));
	/* enable CA7 clock */
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE, HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE) | HSYS_BIT_CKE_AP);
}

void ap_clock_gate(void)
{
#if 0
	u8 wait_cnt = 10;
	CA32_TypeDef *ca32 = CA32_BASE;
	if ((HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE) & HSYS_BIT_CKE_AP) == 0) {
		RTK_LOGI(TAG, "AP CG Already\n");
		return;
	}
	/* poll CA7 clock gate, check WFI state? */
	while (wait_cnt) {
		if ((ca32->CA32_C0_CPU_STATUS & CA32_MASK_STANDBYWFI) == CA32_MASK_STANDBYWFI) {
			break;
		} else {
			wait_cnt--;
			DelayUs(1);
		}
	}
#endif
	/* since CA7 will be blocked even if interrupt happens, so still do clock gate here*/
	ap_clk_gate_ctrl();
	pmu_release_wakelock(PMU_AP_RUN);
	if (APDslpEn) {
		pmu_release_deepwakelock(PMU_AP_RUN);
	}

	RTK_LOGI(TAG, "CA7CG-\n");

}

void ap_clock_on(void)
{
	if (HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE) & HSYS_BIT_CKE_AP) {
		RTK_LOGI(TAG, "AP CW Already\n");
		return;
	}
	pmu_acquire_wakelock(PMU_AP_RUN);
	pmu_acquire_deepwakelock(PMU_AP_RUN);

	ap_clk_wake_ctrl();

	RTK_LOGI(TAG, "CA7CW-\n");
}


void ap_resume(void)
{
	int cnt = 0;
	/* check km4 state, km4 need be active when CA7 run*/
	if (!np_status_on()) {
		RTK_LOGI(TAG, "wake km4\n");
		InterruptDis(NP_WAKE_IRQ);
		np_resume();
	}

	if (HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE) & HSYS_BIT_CKE_AP) {
		RTK_LOGI(TAG, "already clk on\n");
		return;
	}
	pmu_acquire_wakelock(PMU_AP_RUN);

	/* check km4 state, km4 should be active before CA7 run*/
	while (1) {
		if (HAL_READ8(SYSTEM_CTRL_BASE_LP, REG_LSYS_NP_STATUS_SW) & LSYS_BIT_NP_RUNNING) {
			break;
		}
		DelayUs(100);

		/* wait for 1s */
		if (cnt++ > 10000) {
			return;
		}
	}

	if (ap_sleep_type == SLEEP_CG) {
		ap_clock_on();
	} else {
		ap_power_on();
	}
}



u32 ap_suspend(u32 type)
{
	UNUSED(type);

	u32 ret = _SUCCESS;
	SLEEP_ParamDef *sleep_param;
	u32 duration = 0;

	if (!np_status_on()) {
		RTK_LOGI(TAG, "NP is not on\n");
		return 0;
	}

	sleep_param = (SLEEP_ParamDef *)ipc_get_message(IPC_AP_TO_LP, IPC_A2L_TICKLESS_INDICATION);

	if (sleep_param != NULL) {
		duration = sleep_param->sleep_time;
	}

	if (duration > 0) {
		/* used for resume delay */
		ap_sleep_timeout = xTaskGetTickCount() + duration;
	}


	if (type == SLEEP_CG) {
		ap_clock_gate();
	} else {
		ap_power_gate();
	}

	/*clean ap wake pending interrupt*/
	NVIC_ClearPendingIRQ(AP_WAKE_IRQ);
	InterruptEn(AP_WAKE_IRQ, 5);

	return ret;
}

void ap_tickless_ipc_int(UNUSED_WARN_DIS VOID *Data, UNUSED_WARN_DIS u32 IrqStatus, UNUSED_WARN_DIS u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);

	SLEEP_ParamDef *psleep_param;

	DBG_INFO_MSG_ON(MODULE_KM4);

	psleep_param = (SLEEP_ParamDef *)ipc_get_message(IPC_AP_TO_LP, IPC_A2L_TICKLESS_INDICATION);
	DCache_Invalidate((u32)psleep_param, sizeof(SLEEP_ParamDef));

	ap_sleep_type = psleep_param->sleep_type;
	if (psleep_param->dlps_enable) {
		APDslpEn = TRUE;
	} else {
		APDslpEn = FALSE;
	}

	switch (psleep_param->sleep_type) {
	case SLEEP_PG:
		if (_SUCCESS == ap_suspend(SLEEP_PG)) {
			pmu_set_sysactive_time(2);
			pmu_set_sleep_type(SLEEP_PG);
		}
		break;
	case SLEEP_CG:
		if (_SUCCESS == ap_suspend(SLEEP_CG)) {
			pmu_set_sysactive_time(2);
			pmu_set_sleep_type(SLEEP_CG);
		}
		break;

	default:
		RTK_LOGW(TAG, "unknow sleep type\n");
	}
}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE ipc_aptickless_table = {
	.USER_MSG_TYPE = IPC_USER_DATA,
	.Rxfunc = ap_tickless_ipc_int,
	.RxIrqData = (VOID *) NULL,
	.Txfunc = IPC_TXHandler,
	.TxIrqData = (VOID *) NULL,
	.IPC_Direction = IPC_AP_TO_LP,
	.IPC_Channel = IPC_A2L_TICKLESS_INDICATION

};

