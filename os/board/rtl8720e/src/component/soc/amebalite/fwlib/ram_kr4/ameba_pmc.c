/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#include "ameba_soc.h"
#include "ameba_plic.h"

u32 WakeEventFlag = _FALSE;
extern SLEEP_ParamDef sleep_param;

#if defined( __riscv_32e ) && ( __riscv_32e == 1 )
#define configMAX_REGS_NUMBER 16
#else
#define configMAX_REGS_NUMBER 32
#endif

#if __riscv_xlen == 32
#define configMAX_PMPCFG_NUMBER 4
#else
#define configMAX_PMPCFG_NUMBER 2
#endif

#define configMAX_PMPADDR_NUMBER 16

struct ctxRISCVBackup {
	BaseType_t xReg[ configMAX_REGS_NUMBER ];
	BaseType_t xCSRMstatus;
	BaseType_t xCSRMie;
	BaseType_t xCSRMtvec;
	BaseType_t xCSRMepc;
};

struct ctxMtimerBackup {
	uint64_t ulldifftick;
	uint64_t ullCounter;
	uint64_t ullCompare;
};

struct ctxPLICBackup {
	uint32_t xThreshold;
	uint32_t xInterruptEnable[ PLIC_INT_NUMBER / 32 + 1];
	uint32_t xInterruptPrio[ PLIC_INT_NUMBER ];
};

#if (portUSING_MPU_WRAPPERS == 1)
struct ctxPMPBackup {
	BaseType_t xPmpAddrs[ configMAX_PMPADDR_NUMBER ];
	BaseType_t xPmpCfgs[ configMAX_PMPCFG_NUMBER ];
};
#endif

struct ctxPlatformBackup {
	u32 IPCbackup;
	struct ctxRISCVBackup ctxRISCV;
	struct ctxMtimerBackup ctxMtimer;
	struct ctxPLICBackup ctxPLIC;
#if (portUSING_MPU_WRAPPERS == 1)
	struct ctxPMPBackup ctxPMP;
#endif
};
struct ctxPlatformBackup ctxBackup;

extern uint64_t ullNextTime;
extern const size_t uxTimerIncrementsForOneTick;
extern volatile uint64_t *pullMachineTimerCompareRegister;
extern volatile uint64_t *pullMachineTimerCounterRegister;
extern uint32_t const ullMachineTimerCompareRegisterBase;

extern void vPortBackupRegs(BaseType_t *pxGPRBaseAddr, BaseType_t *pxCSRBaseAddr);
extern void vPortRestoreRegs(BaseType_t *pxGPRBaseAddr, BaseType_t *pxCSRBaseAddr);
extern void vPortBackupPmp(BaseType_t *pxPmpBaseAddr);
extern void vPortRestorePmp(BaseType_t *pxPmpBaseAddr);

static uint64_t vPortGetCurMtimer(void)
{
	uint64_t ullcurrenttick = 0ULL;
	volatile uint64_t *const pulTime = (volatile uint64_t *const)(configMTIME_BASE_ADDRESS);

	ullcurrenttick = (uint64_t)(*pulTime);

	return ullcurrenttick;
}

void vPortBackupMtimer(struct ctxMtimerBackup *ctxMtimer)
{
	volatile uint32_t ulHartId;
	volatile uint64_t ullcurrenttick = 0ULL;

	__asm volatile("csrr %0, mhartid" : "=r"(ulHartId));
	pullMachineTimerCompareRegister  = (volatile uint64_t *)(ullMachineTimerCompareRegisterBase + (ulHartId * sizeof(uint64_t)));

	ullcurrenttick = vPortGetCurMtimer();

	if (*pullMachineTimerCompareRegister > ullcurrenttick) {
		ctxMtimer->ulldifftick = *pullMachineTimerCompareRegister - ullcurrenttick;
	} else {
		ctxMtimer->ulldifftick = 0;
	}
}

void vPortRestoreMtimer(struct ctxMtimerBackup *ctxMtimer)
{
	volatile uint32_t ulHartId;
	volatile uint64_t ullcurrenttick = 0ULL;

	__asm volatile("csrr %0, mhartid" : "=r"(ulHartId));
	pullMachineTimerCompareRegister  = (volatile uint64_t *)(ullMachineTimerCompareRegisterBase + (ulHartId * sizeof(uint64_t)));

	ullcurrenttick = vPortGetCurMtimer();

	*pullMachineTimerCompareRegister = ullcurrenttick + ctxMtimer->ulldifftick;

	ullNextTime = *pullMachineTimerCompareRegister + uxTimerIncrementsForOneTick;
}

#ifdef AMEBALITE_TODO
/* Mtimer is stopped when risc-v cpu powers down, this function backups
 * current values
 */
void vPortBackupMtimer(struct ctxMtimerBackup *ctxMtimer)
{
	ctxMtimer->ullCounter = *pullMachineTimerCounterRegister;
	ctxMtimer->ullCompare = *pullMachineTimerCompareRegister;
}

/* This funtion resume mtimer counter value with elapsed time. */
void vPortRestoreMtimer(struct ctxMtimerBackup *ctxMtimer)
{
	/* currently mtimer clock is gated, counterreg cannot write */
	*pullMachineTimerCounterRegister = ctxMtimer->ullCounter;
	*pullMachineTimerCompareRegister = ctxMtimer->ullCompare;
}
#endif

/* This funtion backups plic status. */
void vPortBackupPlic(struct ctxPLICBackup *pCtxPLIC)
{
	UBaseType_t x;

	pCtxPLIC->xThreshold = plic_get_threshold();

	for (x = (UBaseType_t) 0; x <= (UBaseType_t)(PLIC_INT_NUMBER / 32); x++) {
		pCtxPLIC->xInterruptEnable[ x ] = plic_get_enable(x);
	}

	for (x = (UBaseType_t) 0; x < (UBaseType_t) PLIC_INT_NUMBER; x++) {
		pCtxPLIC->xInterruptPrio[ x ] = plic_get_irq_priority(x);
	}
}

/* This function resume plic. */
void vPortRestorePlic(struct ctxPLICBackup *pCtxPLIC)
{
	UBaseType_t x;

	plic_set_threshold(pCtxPLIC->xThreshold);

	for (x = (UBaseType_t) 0; x <= (UBaseType_t)(PLIC_INT_NUMBER / 32); x++) {
		plic_set_enable(x, pCtxPLIC->xInterruptEnable[ x ]);
	}

	for (x = (UBaseType_t) 0; x < (UBaseType_t) PLIC_INT_NUMBER; x++) {
		plic_set_irq_priority(x, pCtxPLIC->xInterruptPrio[ x ]);
	}

}

/* This function is called from Reset_Handler(), then return back to
 * ctxRestore.ctxRISCV.xCSRMepc.
 */
IMAGE2_RAM_TEXT_SECTION
void vSystemResume(void)
{
	vPortRestoreRegs(ctxBackup.ctxRISCV.xReg, &ctxBackup.ctxRISCV.xCSRMstatus);
}

/* User can add platform related power management code in this function to
 * power off the system. This funtion will excutes again after system
 * recovery, but goes different if-else branch, xSystemPowerStatus is used
 * to distinguish that.
 */
IMAGE2_RAM_TEXT_SECTION
void vPortSystemPowerOff(VOID)
{
	u32 Rtemp = 0;
	u32 KR4_is_NP = LSYS_GET_KR4_IS_NP(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1));

	ctxBackup.IPCbackup = IPC_IERGet(IPCKR4_DEV);	/*Backup KR4 IPC configuration*/

#if (portUSING_MPU_WRAPPERS == 1)
	vPortBackupPmp((BaseType_t *) &ctxBackup.ctxPMP);
#endif
	vPortBackupPlic(&ctxBackup.ctxPLIC);
	vPortBackupMtimer(&ctxBackup.ctxMtimer);
	vPortBackupRegs(ctxBackup.ctxRISCV.xReg, &ctxBackup.ctxRISCV.xCSRMstatus);

	if (WakeEventFlag != _TRUE) {
		WakeEventFlag = _TRUE;

		/* ISB() DMB() */
		__asm volatile("fence.i" ::: "memory");
		__asm volatile("fence rw,rw" ::: "memory");

		/* mask KR4 irqs */
		Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
		Rtemp |= PMC_BIT_KR4_IRQ_MASK;
		HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

		if (__csr_read(mip)) {
			DBG_PRINTF(MODULE_KR4, LEVEL_ERROR, "pending int\n");
			goto resume;
		}

		if (KR4_is_NP) {
			DBG_PRINTF(MODULE_KR4, LEVEL_INFO, "R4G NP\n");
			//SOCPS_NP_suspend_config(SLEEP_PG);
			SOCPS_PLL_open(DISABLE);
			DCache_Clean(0xFFFFFFFF, 0xFFFFFFFF);
			Cache_Enable(DISABLE);

			if (ps_config.np_config_ddr == APM_PSRAM_SlEEP_Mode || ps_config.np_config_ddr == WB_PSRAM_SlEEP_Mode) {
				/* psram device enter half sleep mode */
				set_psram_suspend_and_restore(DISABLE);
			}

			/* close CPU */
			Rtemp = HAL_READ32(PMC_BASE, SYSPMC_OPT);
			Rtemp &= ~PMC_BIT_PST_SLEP_ESOC;
			HAL_WRITE32(PMC_BASE, SYSPMC_OPT, Rtemp);

			/* set power mode */
			Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
			Rtemp |= PMC_BIT_PMEN_SLEP;
			Rtemp &= ~PMC_BIT_PMEN_DSLP;
			HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

		} else {
			DBG_PRINTF(MODULE_KR4, LEVEL_INFO, "R4G AP\n");
			ipc_send_message(IPC_KR4_TO_KM4, IPC_R2M_TICKLESS_INDICATION, (PIPC_MSG_STRUCT)&sleep_param);
		}
		__DSB();
		/* it will take 6.5us to enter clock gating mode after register set */
		__WFI();

		if (KR4_is_NP) {
			if (ps_config.np_config_ddr == APM_PSRAM_SlEEP_Mode || ps_config.np_config_ddr == WB_PSRAM_SlEEP_Mode) {
				set_psram_suspend_and_restore(ENABLE);
			}

			Cache_Enable(ENABLE);

			//SOCPS_NP_resume_config(SLEEP_PG);
			SOCPS_PLL_open(ENABLE);
		}


	} else {
		__asm volatile("fence rw,rw" ::: "memory");
		__asm volatile("fence.i" ::: "memory");

		if (KR4_is_NP) {
			//SOCPS_NP_resume_config(SLEEP_PG);
			SOCPS_PLL_open(ENABLE);
		}

		DBG_PRINTF(MODULE_KR4, LEVEL_INFO, "R4GW NP\n");

		vPortRestorePlic(&ctxBackup.ctxPLIC);
		vPortRestoreMtimer(&ctxBackup.ctxMtimer);
#if (portUSING_MPU_WRAPPERS == 1)
		vPortRestorePmp((BaseType_t *) &ctxBackup.ctxPMP);
#endif

		IPC_IERSet(IPCKR4_DEV, ctxBackup.IPCbackup);   /*Refill KR4 IPC configuration */
	}

	if (HAL_READ32(PMC_BASE, SYSPMC_CTRL) & PMC_BIT_PMEN_SLEP) {
		/* clear sleep request */
		Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
		Rtemp &= ~PMC_BIT_PMEN_SLEP;
		HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);
	}

resume:
	if (HAL_READ32(PMC_BASE, SYSPMC_CTRL) & PMC_BIT_KR4_IRQ_MASK) {
		/* unmask kr4 irqs */
		Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
		Rtemp &= (~PMC_BIT_KR4_IRQ_MASK);
		HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);
	}

}


IMAGE2_RAM_TEXT_SECTION
VOID SOCPS_WakeFromPG(VOID)
{
	u32 Rtemp;

	/* disable boot from ps */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_BOOT_CFG);
	Rtemp &= ~LSYS_BIT_BOOT_WAKE_FROM_PS_LS;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_BOOT_CFG, Rtemp);

	vSystemResume();
}

/* keep power functions: UART/I2C/RTC/GPIO/Gtimer/REGU/ANAtimer ?*/
/* close power functions: GDMA */
_OPTIMIZE_NONE_
IMAGE2_RAM_TEXT_SECTION
VOID SOCPS_SleepPG(VOID)
{
	u32 nDeviceIdOffset = PMU_MAX;//0
	u32 Rtemp = 0;

	Img2EntryFun0.RamWakeupFun = SOCPS_WakeFromPG;

	/* exec sleep hook functions */
	nDeviceIdOffset = pmu_exec_sleep_hook_funs();
	if (nDeviceIdOffset != PMU_MAX) {
		pmu_exec_wakeup_hook_funs(nDeviceIdOffset);
		DBG_8195A("Oops: Sleep Fail %x !!!!!\n", nDeviceIdOffset);
		return;
	}

	/* enable boot from ps */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_BOOT_CFG);
	Rtemp |= LSYS_BIT_BOOT_WAKE_FROM_PS_LS;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_BOOT_CFG, Rtemp);

	WakeEventFlag = _FALSE;

	vPortSystemPowerOff();

	/* disable boot from ps */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_BOOT_CFG);
	Rtemp &= ~LSYS_BIT_BOOT_WAKE_FROM_PS_LS;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_BOOT_CFG, Rtemp);

	/* exec sleep hook functions */
	pmu_exec_wakeup_hook_funs(PMU_MAX);

	DBG_8195A("R4GW- %x %x\n", HAL_READ32(PMC_BASE, WAK_STATUS0), HAL_READ32(PMC_BASE, WAK_STATUS1));

}


IMAGE2_RAM_TEXT_SECTION
VOID SOCPS_SleepCG(VOID)
{
	u32 nDeviceIdOffset = PMU_MAX;
	u32 Rtemp;
	u32 KR4_is_NP = 0;

	DBG_PRINTF(MODULE_KR4, LEVEL_INFO, "KR4CG\n");

	/* exec sleep hook functions */
	nDeviceIdOffset = pmu_exec_sleep_hook_funs();
	if (nDeviceIdOffset != PMU_MAX) {
		pmu_exec_wakeup_hook_funs(nDeviceIdOffset);
		DBG_8195A("Oops: Sleep Fail %x !!!!!\n", nDeviceIdOffset);
		return;
	}

	/* mask KR4 irqs */
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	Rtemp |= PMC_BIT_KR4_IRQ_MASK;
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

	if (__csr_read(mip)) {
		DBG_PRINTF(MODULE_KR4, LEVEL_ERROR, "pending int\n");
		goto resume;
	}

	KR4_is_NP = LSYS_GET_KR4_IS_NP(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1));
	if (KR4_is_NP) {
		//SOCPS_NP_suspend_config(SLEEP_CG);
		SOCPS_PLL_open(DISABLE);

		DCache_Clean(0xffffffff, 0xffffffff);
		DCache_Disable();

		if (ps_config.np_config_ddr == APM_PSRAM_SlEEP_Mode || ps_config.np_config_ddr == WB_PSRAM_SlEEP_Mode) {
			/* psram device enter half sleep mode */
			set_psram_suspend_and_restore(DISABLE);
		}

		/* don't close CPU*/
		Rtemp = HAL_READ32(PMC_BASE, SYSPMC_OPT);
		Rtemp |= (PMC_BIT_PST_SLEP_ESOC);
		HAL_WRITE32(PMC_BASE, SYSPMC_OPT, Rtemp);

		/* set power mode */
		Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
		Rtemp |= PMC_BIT_PMEN_SLEP;
		Rtemp &= ~PMC_BIT_PMEN_DSLP;
		HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);
	} else {
		DBG_PRINTF(MODULE_KR4, LEVEL_INFO, "R4C AP\n");
		DCache_Clean((u32)&sleep_param, sizeof(SLEEP_ParamDef));
		ipc_send_message(IPC_KR4_TO_KM4, IPC_R2M_TICKLESS_INDICATION, (PIPC_MSG_STRUCT)&sleep_param);
	}
	__DSB();
	/* it will take 6.5us to enter clock gating mode after register set */
	__WFI();

	if (KR4_is_NP) {
		if (ps_config.np_config_ddr == APM_PSRAM_SlEEP_Mode || ps_config.np_config_ddr == WB_PSRAM_SlEEP_Mode) {
			set_psram_suspend_and_restore(ENABLE);
		}

		DCache_Enable();

		//SOCPS_NP_resume_config(SLEEP_CG);
		SOCPS_PLL_open(ENABLE);
	} else {
		DBG_PRINTF(MODULE_KR4, LEVEL_INFO, "R4CW AP\n");
	}

	if (HAL_READ32(PMC_BASE, SYSPMC_CTRL) & PMC_BIT_PMEN_SLEP) {
		/* clear sleep request */
		Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
		Rtemp &= ~PMC_BIT_PMEN_SLEP;
		HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);
	}

resume:
	if (HAL_READ32(PMC_BASE, SYSPMC_CTRL) & PMC_BIT_KR4_IRQ_MASK) {
		/* unmask kr4 irqs */
		Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
		Rtemp &= (~PMC_BIT_KR4_IRQ_MASK);
		HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

	}

	/* exec sleep hook functions */
	pmu_exec_wakeup_hook_funs(PMU_MAX);

	DBG_PRINTF(MODULE_KR4, LEVEL_INFO, "KR4CG- %x %x\n", HAL_READ32(PMC_BASE, WAK_STATUS0), HAL_READ32(PMC_BASE, WAK_STATUS1));
}


VOID SOCPS_DeepSleep(VOID)
{
	DCache_CleanInvalidate(0xFFFFFFFF, 0xFFFFFFFF);

	ipc_send_message(IPC_KR4_TO_KM4, IPC_R2M_TICKLESS_INDICATION, (PIPC_MSG_STRUCT)&sleep_param);

	taskENTER_CRITICAL();
	Systick_Cmd(DISABLE);

	__WFI();

	taskEXIT_CRITICAL();
	Systick_Cmd(ENABLE);
}


/**
  * @brief  KM4 wake KR4 by IPC
  * @param  None
  * @note  ipc_msg_temp.msg represents who wakes up KR4
  		* ipc_msg_temp.msg = 0: FW wakeup KR4
  * @retval None
  */
IMAGE2_RAM_TEXT_SECTION
void SOCPS_KM4WKR4_ipc_int(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);

	PIPC_MSG_STRUCT	ipc_msg_temp = (PIPC_MSG_STRUCT)ipc_get_message(IPC_KM4_TO_KR4, IPC_M2R_WAKE_AP);

	DBG_8195A("%s \n", __func__);

	u32 type = ipc_msg_temp->msg;

	if (type == FW_NPWAP_IPC) {
		DBG_8195A("FW wakeup KR4 via IPC \n");
	} else if (type == TIMER_NPWAP_IPC) {
		DBG_8195A("TIMER wakeup KR4 via IPC \n");
	}

}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   ipc_KM4WKR4_table[] = {
	{IPC_USER_DATA, 	SOCPS_KM4WKR4_ipc_int,	(VOID *) NULL, IPC_KM4_TO_KR4, IPC_M2R_WAKE_AP, IPC_RX_FULL},
};
