/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#include "ameba_soc.h"

//static const char *TAG = "PMC";
typedef struct {
	u32 CTRL;
	u32 MAIR0;
	u32 MAIR1;
	u32 RBAR[MPU_MAX_REGION];
	u32 RLAR[MPU_MAX_REGION];
} MPU_BackUp_TypeDef;

typedef struct {
	u32 CPURegbackup_HP[25];
	u32 CPUPSP_HP;
	u32 PortSVC_Backup_HP;
	u32 NVICbackup_HP[9];
	u32 SYSTICKbackup_HP[4];
	u32 SCBbackup_HP[4];
	u32 IPCbackup_HP;
	u32 BASEPRI_backup_HP;
	u32 PRIMASK_backup_HP;
	u8 NVICIPbackup_HP[MAX_PERIPHERAL_IRQ_NUM];
	MPU_BackUp_TypeDef MPU_BK;
} CPU_BackUp_TypeDef;

CPU_BackUp_TypeDef PMC_BK;
u32 WakeEventFlag_HP = _FALSE;
extern SLEEP_ParamDef sleep_param;

VOID SOCPS_NVICBackup_HP(void)
{
	int i = 0;

	PMC_BK.SYSTICKbackup_HP[0] = SysTick->CTRL;
	PMC_BK.SYSTICKbackup_HP[1] = SysTick->LOAD;
	PMC_BK.SYSTICKbackup_HP[2] = SysTick->VAL;

	PMC_BK.NVICbackup_HP[0] = NVIC->ISER[0];
	PMC_BK.NVICbackup_HP[1] = NVIC->ISER[1];
	PMC_BK.NVICbackup_HP[2] = NVIC->ISER[2];
	PMC_BK.NVICbackup_HP[3] = NVIC->ICER[0];
	PMC_BK.NVICbackup_HP[4] = NVIC->ICER[1];
	PMC_BK.NVICbackup_HP[5] = NVIC->ICER[2];

	for (i = 0; i < MAX_PERIPHERAL_IRQ_NUM; i++) {
		PMC_BK.NVICIPbackup_HP[i] = NVIC->IPR[i];
	}

	PMC_BK.NVICbackup_HP[6] = NVIC->ISPR[0];
	PMC_BK.NVICbackup_HP[7] = NVIC->ISPR[1];
	PMC_BK.NVICbackup_HP[8] = NVIC->ISPR[2];

#if defined(__VTOR_PRESENT) && (__VTOR_PRESENT == 1)
	PMC_BK.SCBbackup_HP[0] = SCB->VTOR;
#endif
}

VOID SOCPS_NVICReFill_HP(VOID)
{
	int i = 0;

	/* Configure SysTick to interrupt at the requested rate. */
	SysTick->CTRL = PMC_BK.SYSTICKbackup_HP[0];
	SysTick->LOAD = PMC_BK.SYSTICKbackup_HP[1];
	SysTick->VAL = PMC_BK.SYSTICKbackup_HP[2];

	NVIC->ISPR[0] = PMC_BK.NVICbackup_HP[6];
	NVIC->ISPR[1] = PMC_BK.NVICbackup_HP[7];
	NVIC->ISPR[2] = PMC_BK.NVICbackup_HP[8];

	NVIC->ICER[0] = PMC_BK.NVICbackup_HP[3];
	NVIC->ICER[1] = PMC_BK.NVICbackup_HP[4];
	NVIC->ICER[2] = PMC_BK.NVICbackup_HP[5];

	for (i = 0; i < MAX_PERIPHERAL_IRQ_NUM; i++) {
		NVIC->IPR[i] = PMC_BK.NVICIPbackup_HP[i];
	}

	NVIC->ISER[0] = PMC_BK.NVICbackup_HP[0];
	NVIC->ISER[1] = PMC_BK.NVICbackup_HP[1];
	NVIC->ISER[2] = PMC_BK.NVICbackup_HP[2];

#if defined(__VTOR_PRESENT) && (__VTOR_PRESENT == 1)
	SCB->VTOR = PMC_BK.SCBbackup_HP[0];
#endif
}

VOID SOCPS_MPUBackup_HP(void)
{
	int i = 0;
	PMC_BK.MPU_BK.CTRL = MPU->CTRL;

	PMC_BK.MPU_BK.MAIR0 = MPU->MAIR0;
	PMC_BK.MPU_BK.MAIR1 = MPU->MAIR1;
	for (i = 0; i < MPU_MAX_REGION; i++) {
		MPU->RNR = i;
		PMC_BK.MPU_BK.RBAR[i] = MPU->RBAR;
		PMC_BK.MPU_BK.RLAR[i] = MPU->RLAR;
	}
}

VOID SOCPS_MPUReFill_HP(VOID)
{
	int i = 0;
	MPU->CTRL = 0;
	MPU->MAIR0 = PMC_BK.MPU_BK.MAIR0;
	MPU->MAIR1 = PMC_BK.MPU_BK.MAIR1;
	for (i = 0; i < MPU_MAX_REGION; i++) {
		MPU->RNR = i;
		MPU->RBAR = PMC_BK.MPU_BK.RBAR[i];
		MPU->RLAR = PMC_BK.MPU_BK.RLAR[i];
	}
	MPU->CTRL = PMC_BK.MPU_BK.CTRL;
}

void SOCPS_vWFSSVCHandler_HP(void)
{
	asm volatile("mov r0, %0\n"::"r"(PMC_BK.CPUPSP_HP):"r0");

	asm volatile
	(
		"ldmia r0!, {r4-r7}				\n" /* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
		"ldmia r0!, {r8-r11}				\n"
		"msr psp, r0						\n" /* Restore the task stack pointer. */
		"orr r14, r14, #0xc					      \n"
	);
}

_OPTIMIZE_NONE_
VOID SOCPS_WakeFromPG_HP(VOID)
{
	u32 Rtemp;

	//RTK_LOGD(TAG, "SOCPS_WakeFromPG_HP\n");

	/* Need Modify */
	//irq_table_init(MSP_RAM_HP);

	//ns cache already enabled in wakefrom_pg in bootloader, secure world keep off
	/* we should Cache_Flush when we wake */
	//Cache_Enable(DISABLE);
	//Cache_Enable(ENABLE);

	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG);
	Rtemp &= (~LSYS_BIT_BOOT_WAKE_FROM_PS_HS);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG, Rtemp);

	/* Make PendSV, CallSV and SysTick the same priroity as the kernel. */
	/* uint8_t  SHP[12]: Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
	/* SCB->SHP[8~11] */
	HAL_WRITE32(0, 0xE000ED20, 0xF0F00000);
	PMC_BK.PortSVC_Backup_HP = (u32)NewVectorTable[11];
	NewVectorTable[11] = (HAL_VECTOR_FUN)SOCPS_vWFSSVCHandler_HP;

	/* push cpu register into stack based on SOCPS_vWFSSVCHandler */
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 4))) = (PMC_BK.CPURegbackup_HP[16] | 0x1000000);     //PSR
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 8))) = PMC_BK.CPURegbackup_HP[15];     //PC
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 12))) = PMC_BK.CPURegbackup_HP[14];     //LR
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 16))) = PMC_BK.CPURegbackup_HP[12];     //R12
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 20))) = PMC_BK.CPURegbackup_HP[3];      //R3
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 24))) = PMC_BK.CPURegbackup_HP[2];      //R2
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 28))) = PMC_BK.CPURegbackup_HP[1];      //R1
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 32))) = PMC_BK.CPURegbackup_HP[0];      //R0
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 36))) = PMC_BK.CPURegbackup_HP[11];     //R11
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 40))) = PMC_BK.CPURegbackup_HP[10];     //R10
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 44))) = PMC_BK.CPURegbackup_HP[9];      //R9
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 48))) = PMC_BK.CPURegbackup_HP[8];      //R8
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 52))) = PMC_BK.CPURegbackup_HP[7];      //R7
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 56))) = PMC_BK.CPURegbackup_HP[6];      //R6
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 60))) = PMC_BK.CPURegbackup_HP[5];      //R5
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP_HP - 64))) = PMC_BK.CPURegbackup_HP[4];      //R4
	PMC_BK.CPUPSP_HP = PMC_BK.CPUPSP_HP - 64; //PSP

	asm volatile(
		" cpsie i				\n" /* Globally enable interrupts. */
		" svc 0					\n" /* System call to start first task. */
		" nop					\n"
	);
}


_OPTIMIZE_NONE_
VOID SOCPS_SleepPG_RAM(VOID)
{
	//asm volatile("SLPPG_WAKEUP_POINT:\n");
	asm volatile("MOV %0, r0\n"	:"=r"(PMC_BK.CPURegbackup_HP[0])::"memory");
	asm volatile("MOV %0, r1\n"	:"=r"(PMC_BK.CPURegbackup_HP[1])::"memory");
	asm volatile("MOV %0, r2\n"	:"=r"(PMC_BK.CPURegbackup_HP[2])::"memory");
	asm volatile("MOV %0, r3\n"	:"=r"(PMC_BK.CPURegbackup_HP[3])::"memory");
	asm volatile("MOV %0, r4\n"	:"=r"(PMC_BK.CPURegbackup_HP[4])::"memory");
	asm volatile("MOV %0, r5\n"	:"=r"(PMC_BK.CPURegbackup_HP[5])::"memory");
	asm volatile("MOV %0, r6\n"	:"=r"(PMC_BK.CPURegbackup_HP[6])::"memory");
	asm volatile("MOV %0, r7\n"	:"=r"(PMC_BK.CPURegbackup_HP[7])::"memory");
	asm volatile("MOV %0, r8\n"	:"=r"(PMC_BK.CPURegbackup_HP[8])::"memory");
	asm volatile("MOV %0, r9\n"	:"=r"(PMC_BK.CPURegbackup_HP[9])::"memory");
	asm volatile("MOV %0, r10\n":"=r"(PMC_BK.CPURegbackup_HP[10])::"memory");
	asm volatile("MOV %0, r11\n":"=r"(PMC_BK.CPURegbackup_HP[11])::"memory");
	asm volatile("MOV %0, r12\n":"=r"(PMC_BK.CPURegbackup_HP[12])::"memory");
	asm volatile("MOV %0, r13\n":"=r"(PMC_BK.CPURegbackup_HP[13])::"memory");//PSP
	asm volatile("MOV %0, r14\n" :"=r"(PMC_BK.CPURegbackup_HP[14])::"memory");
	asm volatile("MOV %0, pc\n" :"=r"(PMC_BK.CPURegbackup_HP[15])::"memory");
	asm volatile("MRS %0, PSR\n":"=r"(PMC_BK.CPURegbackup_HP[16])::"memory");
	asm volatile("NOP");
	asm volatile("NOP");


	/* Enable low power mode:  */
	if (WakeEventFlag_HP != _TRUE) {
		//PMC_BK.CPURegbackup_HP[14] = PMC_BK.CPURegbackup_HP[15];
		PMC_BK.CPUPSP_HP = PMC_BK.CPURegbackup_HP[13];

		WakeEventFlag_HP = _TRUE;

		//asm volatile("cpsid i" : : : "memory");
		asm volatile("cpsid f" : : : "memory");

		asm volatile("wfe");
		asm volatile("wfe");

		//asm volatile("cpsie i" : : : "memory");
		asm volatile("cpsie f" : : : "memory");
	}

}

_OPTIMIZE_NONE_
VOID SOCPS_SleepPG_LIB(VOID)
{
	/*Backup KM4 IPC configuration*/
	PMC_BK.IPCbackup_HP = IPCNP_DEV->IPC_IMR;

	PMC_BK.BASEPRI_backup_HP = __get_BASEPRI();
	PMC_BK.PRIMASK_backup_HP = __get_PRIMASK();

	/* backup registgers */
	SOCPS_NVICBackup_HP();
	SOCPS_MPUBackup_HP();

	sleep_param.sleep_type = SLEEP_PG;
	ipc_send_message(IPC_NP_TO_LP, IPC_N2L_TICKLESS_INDICATION, (PIPC_MSG_STRUCT)&sleep_param);
	DCache_Disable();

	SOCPS_SleepPG_RAM();

	/* disable all int until exit idle task */
	__asm volatile("cpsid i");

	SOCPS_NVICReFill_HP();
	SOCPS_MPUReFill_HP();

	NewVectorTable[11] = (HAL_VECTOR_FUN)PMC_BK.PortSVC_Backup_HP;

	/*Refill KM4 IPC configuration*/
	IPCNP_DEV->IPC_IMR = PMC_BK.IPCbackup_HP;
	/* ReFill registers */
	__set_PRIMASK(PMC_BK.PRIMASK_backup_HP);
	__set_BASEPRI(PMC_BK.BASEPRI_backup_HP);

	WakeEventFlag_HP = _FALSE;

}

VOID SOCPS_DeepSleep(VOID)
{
	DCache_CleanInvalidate(0xFFFFFFFF, 0xFFFFFFFF);

	ipc_send_message(IPC_NP_TO_LP, IPC_N2L_TICKLESS_INDICATION, (PIPC_MSG_STRUCT)&sleep_param);
	asm volatile("cpsid i" : : : "memory");
	asm volatile("cpsid f" : : : "memory");

	asm volatile("wfe");
	asm volatile("wfe");

	asm volatile("cpsie i" : : : "memory");
	asm volatile("cpsie f" : : : "memory");
}

void SOCPS_SleepCG_LIB(void)
{

	sleep_param.sleep_type = SLEEP_CG;
	ipc_send_message(IPC_NP_TO_LP, IPC_N2L_TICKLESS_INDICATION, (PIPC_MSG_STRUCT)&sleep_param);

	asm volatile("wfe");
	asm volatile("wfe");
}
/**
  * @brief  set np wake up event mask0.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
void SOCPS_SetNPWakeEvent_MSK0_HP(u32 Option, u32 NewStatus)
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
void SOCPS_SetNPWakeEvent_MSK1_HP(u32 Option, u32 NewStatus)
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

