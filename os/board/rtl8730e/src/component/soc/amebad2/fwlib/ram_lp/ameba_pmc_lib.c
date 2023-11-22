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
typedef struct {
	u32 CTRL;
	u32 MAIR0;
	u32 MAIR1;
	u32 RBAR[MPU_MAX_REGION];
	u32 RLAR[MPU_MAX_REGION];
} MPU_BackUp_TypeDef;

typedef struct {
	u32 MMU_ENTRY0_CTRL;			/*!< flash MMU entry_x control */
	u32 MMU_ENTRY0_STRADDR;		/*!< flash MMU entry_x virtual address start */
	u32 MMU_ENTRY0_ENDADDR;		/*!< flash MMU entry_x virtual address end */
	u32 MMU_ENTRY0_OFFSET;
	u32 MMU_ENTRY1_CTRL;			/*!< flash MMU entry_x control */
	u32 MMU_ENTRY1_STRADDR;		/*!< flash MMU entry_x virtual address start */
	u32 MMU_ENTRY1_ENDADDR;		/*!< flash MMU entry_x virtual address end */
	u32 MMU_ENTRY1_OFFSET;
} MMU_Backup_TypeDef;

typedef struct {
	u32 CPURegbackup[25];
	u32 NVICbackup[6];
	u32 SYSTICKbackup[4];
	u32 SCBbackup[4];
	u32 CPUPSP;
	u32 IPCbackup;
	u32 PortSVC_Backup;
	u32 NVICIPbackup[MAX_NVIC_IPR_NUM];
	MPU_BackUp_TypeDef MPU_BK;
	MMU_Backup_TypeDef MMU_BK;
} CPU_BackUp_TypeDef;

CPU_BackUp_TypeDef PMC_BK;
u32 WakeEventFlag = _FALSE;

MEMMode_TypeDef PMC_MemMode_Def[6] = {
//		Module								MEM_SD					MEM_DS					MEM_LS
	{
		REG_CTRL_LRET_E0_FTC_ULL_0,		CTRL_BIT_LRET_E0_SLP,	CTRL_BIT_LRET_E0_RET,	CTRL_BIT_LRET_E0_NAP
	},	//retention sram
	{
		REG_CTRL_HSYS_E0_FTC_ULL_0,		CTRL_BIT_HSYS_E0_SLP,	CTRL_BIT_HSYS_E0_RET,	CTRL_BIT_HSYS_E0_NAP
	},	//hs sram
	{
		REG_CTRL_WLK4_E0_FTC_ULL_0,		CTRL_BIT_WLK4_E0_SLP,	CTRL_BIT_WLK4_E0_RET,	CTRL_BIT_WLK4_E0_NAP
	},	//km4 wlan sram
	{
		REG_CTRL_BTONK4_E0_FTC_ULL_0,	CTRL_BIT_BTONK4_E0_SLP,	CTRL_BIT_BTONK4_E0_RET,	CTRL_BIT_BTONK4_E0_NAP
	},	//bt on sram
	{
		REG_CTRL_BTOFFK4_E0_FTC_ULL_0,	CTRL_BIT_BTOFFK4_E0_SLP, CTRL_BIT_BTOFFK4_E0_RET, CTRL_BIT_BTOFFK4_E0_NAP
	},	//bt off sram0
	{
		REG_CTRL_BTOFFK4_E1_FTC_ULL_0,	CTRL_BIT_BTOFFK4_E0_SLP, CTRL_BIT_BTOFFK4_E0_RET, CTRL_BIT_BTOFFK4_E0_NAP
	},	//bt off sram1
};

/**
  * @brief  set sram mem mode
  * @param  module: sram mudule
  *		This parameter can be one of the following values:
  *		 @arg REG_CTRL_LRET_E0_FTC_ULL_0 : Retention RAM 512Bytes
  *		 @arg REG_CTRL_HSYS_E0_FTC_ULL_0 : hs sys SRAM 256KB
  *		 @arg REG_CTRL_WLK4_E0_FTC_ULL_0 : Wifi & KM4 share SRAM
  *		 @arg REG_CTRL_BTONK4_E0_FTC_ULL_0 : Wifi OFF SRAM
  *		 @arg REG_CTRL_BTOFFK4_E0_FTC_ULL_0 : Wifi ON SRAM 8K
  * @param  mem_mode: mem mode
  *		This parameter can be one of the following values:
  *		 @arg MEM_ACT_MODE : active mode
  *		 @arg MEM_LS_MODE : light sleep mode
  *		 @arg MEM_DS_MODE : deep sleep mode
  *		 @arg MEM_SD_MODE : shut down mode
  * @retval None
  */
void SOCPS_SetMemMode(u32 module, u32 mem_mode)
{
	u32 Rtemp = 0;
	u32 index = 0;
	for (index = 0; index < 6; index++) {
		if (module == PMC_MemMode_Def[index].Module) {
			break;
		}
	}

	if (6 == index) {
		return;
	}
	//RTK_LOGD(TAG, "mem module idx = %d mode = %d\n", index, mem_mode);
	Rtemp = HAL_READ32(SYSTEM_MEM_CTRL_BASE, module);
	if (MEM_ACT_MODE == mem_mode) {
		Rtemp &= (~(PMC_MemMode_Def[index].MEM_SD | PMC_MemMode_Def[index].MEM_DS | PMC_MemMode_Def[index].MEM_LS));
	} else if (MEM_LS_MODE == mem_mode) {
		Rtemp |= PMC_MemMode_Def[index].MEM_LS;
	} else if (MEM_DS_MODE == mem_mode) {
		Rtemp |= PMC_MemMode_Def[index].MEM_DS;
	} else if (MEM_SD_MODE == mem_mode) {
		Rtemp |= PMC_MemMode_Def[index].MEM_SD;
	}

	HAL_WRITE32(SYSTEM_MEM_CTRL_BASE, module, Rtemp);
	//RTK_LOGD(TAG, "mem set %x = %x\n", module, HAL_READ32(SYSTEM_MEM_CTRL_BASE, module));
}

/**
  * @brief  resume sram mem active mode
  * @param  module: sram mudule
  *		This parameter can be one of the following values:
  *		 @arg REG_CTRL_LRET_E0_FTC_ULL_0 : Retention RAM 512Bytes
  *		 @arg REG_CTRL_HSYS_E0_FTC_ULL_0 : hs sys SRAM 256KB
  *		 @arg REG_CTRL_WLK4_E0_FTC_ULL_0 : Wifi & KM4 share SRAM
  *		 @arg REG_CTRL_BTONK4_E0_FTC_ULL_0 : Wifi OFF SRAM
  *		 @arg REG_CTRL_BTOFFK4_E0_FTC_ULL_0 : Wifi ON SRAM 8K
  * @retval None
  */
void SOCPS_ResMemMode(u32 module)
{
	u32 Rtemp = 0;
	u32 index = 0;
	for (index = 0; index < 6; index++) {
		if (module == PMC_MemMode_Def[index].Module) {
			break;
		}
	}

	if (6 == index) {
		return;
	}

	//RTK_LOGD(TAG, "mem module resume idx = %d\n", index);
	Rtemp = HAL_READ32(SYSTEM_MEM_CTRL_BASE, module);
	Rtemp &= (~(PMC_MemMode_Def[index].MEM_SD | PMC_MemMode_Def[index].MEM_DS | PMC_MemMode_Def[index].MEM_LS));
	HAL_WRITE32(SYSTEM_MEM_CTRL_BASE, module, Rtemp);
}

VOID SOCPS_SWRLDO_Suspend(u32 new_status)
{
	u32 Temp;
	REGU_TypeDef *regu_ctrl = REGU_BASE;

	/* suspend */
	if (new_status == ENABLE) {
		/* set AON LDO sleep mode, voltage default is 0x09=0.899V */
		Temp = regu_ctrl->REGU_AON_LDO_CTRL;
		Temp |= REGU_BIT_EN_SLEEP_L; /* 11uA */
		regu_ctrl->REGU_AON_LDO_CTRL = Temp;
	} else {
		/* set AON LDO active mode, voltage default is 0x09=0.899V */
		Temp = regu_ctrl->REGU_AON_LDO_CTRL;
		Temp &= (~REGU_BIT_EN_SLEEP_L); /* 11uA */
		regu_ctrl->REGU_AON_LDO_CTRL = Temp;
	}
}

void SOCPS_vWFSSVCHandler(void)
{
	asm volatile("mov r0, %0\n"::"r"(PMC_BK.CPUPSP):"r0");
	asm volatile
	(
		"ldmia r0!, {r4-r7}				\n" /* Pop the high registers. */
		"mov r8, r4						\n"
		"mov r9, r5						\n"
		"mov r10, r6					\n"
		"mov r11, r7					\n"
		"ldmia r0!, {r4-r7}				\n" /* Pop low registers.  */
		"msr psp, r0					\n" /* Remember the new top of stack for the task. */
		"mov r1, r14					\n" /* OR R14 with 0x0d. */
		"movs r0, #0x0d					\n"
#if defined ( __ICCARM__ )
		"orrs r1, r1, r0						\n"
#else
		"orr r1, r0						\n"
#endif
		"bx r1							\n"
	);
}

VOID SOCPS_NVICBackup(void)
{
	int i = 0;

	PMC_BK.SYSTICKbackup[0] = SysTick->CTRL;
	PMC_BK.SYSTICKbackup[1] = SysTick->LOAD;
	PMC_BK.SYSTICKbackup[2] = SysTick->VAL;

	PMC_BK.NVICbackup[0] = NVIC->ISER[0];
	PMC_BK.NVICbackup[1] = NVIC->ISER[1];
	PMC_BK.NVICbackup[2] = NVIC->ICER[0];
	PMC_BK.NVICbackup[3] = NVIC->ICER[1];

	for (i = 0; i < MAX_NVIC_IPR_NUM; i++) {
		PMC_BK.NVICIPbackup[i] = NVIC->IPR[i];
	}

	PMC_BK.NVICbackup[4] = NVIC->ISPR[0];
	PMC_BK.NVICbackup[5] = NVIC->ISPR[1];

#if (__VTOR_PRESENT == 1)
	PMC_BK.SCBbackup[0] = SCB->VTOR;
#endif
}

VOID SOCPS_NVICReFill(VOID)
{
	int i = 0;

	/* Configure SysTick to interrupt at the requested rate. */
	SysTick->CTRL = PMC_BK.SYSTICKbackup[0];
	SysTick->LOAD = PMC_BK.SYSTICKbackup[1];
	SysTick->VAL = PMC_BK.SYSTICKbackup[2];

	NVIC->ISPR[0] = PMC_BK.NVICbackup[4];
	NVIC->ISPR[1] = PMC_BK.NVICbackup[5];
	NVIC->ICER[0] = PMC_BK.NVICbackup[2];
	NVIC->ICER[1] = PMC_BK.NVICbackup[3];

	for (i = 0; i < MAX_NVIC_IPR_NUM; i++) {
		NVIC->IPR[i] = PMC_BK.NVICIPbackup[i];
	}

	NVIC->ISER[0] = PMC_BK.NVICbackup[0];
	NVIC->ISER[1] = PMC_BK.NVICbackup[1];

#if (__VTOR_PRESENT == 1)
	SCB->VTOR = PMC_BK.SCBbackup[0];
#endif
}

VOID SOCPS_MPUBackup(void)
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

VOID SOCPS_MPUReFill(VOID)
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

SRAM_ONLY_TEXT_SECTION
VOID SOCPS_WakeFromPG(VOID)
{
	//km4_flash_highspeed_resume(FALSE);

	if (ps_config.km0_tickles_debug) {
		RTK_LOGD(TAG, "SOCPS_WakeFromPG \n");
	}

	/* Make PendSV, CallSV and SysTick the same priroity as the kernel. */
	/* uint8_t  SHP[12]: Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
	/* SCB->SHP[8~11] */
	HAL_WRITE32(0, 0xE000ED20, 0xF0F00000);

	PMC_BK.PortSVC_Backup = (u32)NewVectorTable[11];
	NewVectorTable[11] = (HAL_VECTOR_FUN)SOCPS_vWFSSVCHandler;

	/* push cpu register into stack based on SOCPS_vWFSSVCHandler */
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 4))) = (PMC_BK.CPURegbackup[16] | 0x1000000);     //PSR
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 8))) = PMC_BK.CPURegbackup[15];     //PC
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 12))) = PMC_BK.CPURegbackup[14];     //LR
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 16))) = PMC_BK.CPURegbackup[12];     //R12
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 20))) = PMC_BK.CPURegbackup[3];      //R3
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 24))) = PMC_BK.CPURegbackup[2];      //R2
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 28))) = PMC_BK.CPURegbackup[1];      //R1
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 32))) = PMC_BK.CPURegbackup[0];      //R0
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 36))) = PMC_BK.CPURegbackup[7];     //R11
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 40))) = PMC_BK.CPURegbackup[6];     //R10
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 44))) = PMC_BK.CPURegbackup[5];      //R9
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 48))) = PMC_BK.CPURegbackup[4];      //R8
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 52))) = PMC_BK.CPURegbackup[11];      //R7
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 56))) = PMC_BK.CPURegbackup[10];      //R6
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 60))) = PMC_BK.CPURegbackup[9];      //R5
	(* ((volatile unsigned long *)(PMC_BK.CPUPSP - 64))) = PMC_BK.CPURegbackup[8];      //R4
	PMC_BK.CPUPSP = PMC_BK.CPUPSP - 64; //PSP

	asm volatile(
		" cpsie i				\n" /* Globally enable interrupts. */
		" svc 0					\n" /* System call to start first task. */
		" nop					\n"
	);
}

/**
  * @brief  enter sleep power gate mode.
  * @retval None
  *
  */
SRAM_ONLY_TEXT_SECTION
_OPTIMIZE_NONE_
void SOCPS_SleepCG_RAM(VOID)
{
	u32 Rtemp = 0;

	/* mask lp irqs */
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	Rtemp |= PMC_BIT_LP_IRQ_MASK;
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

	for (int32_t x = 0; x <= MAX_PERIPHERAL_IRQ_NUM / 32; x++) {
		Rtemp = NVIC->ISPR[x];
		if (NVIC->ISER[x] & Rtemp) {
			goto resume;
		}
	}

	FLASH_DeepPowerDown(ENABLE);//120us

	/* don't close CPU*/
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_OPT);
	Rtemp |= (PMC_BIT_PST_SLEP_ESOC);
	HAL_WRITE32(PMC_BASE, SYSPMC_OPT, Rtemp);

	/* set power mode */
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	Rtemp |= PMC_BIT_PMEN_SLEP;
	Rtemp &= ~PMC_BIT_PMEN_DSLP;
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

	/* it will take 6.5us to enter clock gating mode after register set */
	__WFI();

	/* wakeup */
	FLASH_DeepPowerDown(DISABLE);//257


resume:
	/*unmask lp irqs*/
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	Rtemp &= (~PMC_BIT_LP_IRQ_MASK);
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);
}

VOID SOCPS_SleepCG_LIB(VOID)
{
	WakeEventFlag = _TRUE;

	//Enable low power mode
	SOCPS_SleepCG_RAM();
}

/**
  * @brief  enter sleep power gate mode.
  * @retval None
  *
  */
_OPTIMIZE_NONE_
SRAM_ONLY_TEXT_SECTION
void SOCPS_SleepPG_RAM(VOID)
{
	//asm volatile("SLPPG_WAKEUP_POINT:\n");
	//asm volatile("NOP");
	asm volatile("MOV %0, r0\n"	:"=r"(PMC_BK.CPURegbackup[0])::"memory");
	asm volatile("MOV %0, r1\n"	:"=r"(PMC_BK.CPURegbackup[1])::"memory");
	asm volatile("MOV %0, r2\n"	:"=r"(PMC_BK.CPURegbackup[2])::"memory");
	asm volatile("MOV %0, r3\n"	:"=r"(PMC_BK.CPURegbackup[3])::"memory");
	asm volatile("MOV %0, r4\n"	:"=r"(PMC_BK.CPURegbackup[4])::"memory");
	asm volatile("MOV %0, r5\n"	:"=r"(PMC_BK.CPURegbackup[5])::"memory");
	asm volatile("MOV %0, r6\n"	:"=r"(PMC_BK.CPURegbackup[6])::"memory");
	asm volatile("MOV %0, r7\n"	:"=r"(PMC_BK.CPURegbackup[7])::"memory");
	asm volatile("MOV %0, r8\n"	:"=r"(PMC_BK.CPURegbackup[8])::"memory");
	asm volatile("MOV %0, r9\n"	:"=r"(PMC_BK.CPURegbackup[9])::"memory");
	asm volatile("MOV %0, r10\n":"=r"(PMC_BK.CPURegbackup[10])::"memory");
	asm volatile("MOV %0, r11\n":"=r"(PMC_BK.CPURegbackup[11])::"memory");
	asm volatile("MOV %0, r12\n":"=r"(PMC_BK.CPURegbackup[12])::"memory");
	asm volatile("MOV %0, r13\n":"=r"(PMC_BK.CPURegbackup[13])::"memory");//PSP
	asm volatile("MOV %0, r14\n" :"=r"(PMC_BK.CPURegbackup[14])::"memory");
	asm volatile("MRS %0, PSR\n":"=r"(PMC_BK.CPURegbackup[16])	::"memory");

	asm volatile("MOV %0, pc\n" :"=r"(PMC_BK.CPURegbackup[15])::"memory");
	asm volatile("NOP");
	asm volatile("NOP");

	PMC_BK.CPUPSP = PMC_BK.CPURegbackup[13];

	/* Enable low power mode:  */
	if (WakeEventFlag != _TRUE) {
		WakeEventFlag = _TRUE;

		/* when read the value of pc, the value returned will increase by 4 */
		PMC_BK.CPURegbackup[15] -= 4;
		/* it will take 6.5us to enter clock gating mode after register set */
		__asm("	WFI");
	}
}

/* keep power functions: UART/I2C/RTC/GPIO/Gtimer/REGU/ANAtimer */
/* close power functions: GDMA */
_OPTIMIZE_NONE_
SRAM_ONLY_TEXT_SECTION
VOID SOCPS_SleepPG_LIB(VOID)
{
	u32 Rtemp;

	/* enable boot from ps */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG);
	Rtemp |= LSYS_BIT_BOOT_WAKE_FROM_PS_LS;
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG, Rtemp);

	WakeEventFlag = _FALSE;
	/*Backup KM0 IPC configuration*/
	PMC_BK.IPCbackup = IPCLP_DEV->IPC_IMR;

	/* backup registgers */
	SOCPS_NVICBackup();
	SOCPS_MPUBackup();

	FLASH_DeepPowerDown(ENABLE);

	/* clear all Dcache, for LP sram size may change with chip */
	//DCache_CleanInvalidate(0xFFFFFFFF, 0xFFFFFFFF);
	DCache_Disable();

	/* close CPU */
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_OPT);
	Rtemp &= ~PMC_BIT_PST_SLEP_ESOC;
	HAL_WRITE32(PMC_BASE, SYSPMC_OPT, Rtemp);

	/* mask lp irqs */
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	Rtemp |= PMC_BIT_LP_IRQ_MASK;
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

	/* set power mode */
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	Rtemp |= PMC_BIT_PMEN_SLEP | PMC_BIT_POF_WAIT_SLEP;
	Rtemp &= ~PMC_BIT_PMEN_DSLP;
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

	SOCPS_SleepPG_RAM();

	/* for cpu can't enter wfi mode case when there are pending isr on NVIC */
	/* do not CPU */
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_OPT);
	Rtemp |= PMC_BIT_PST_SLEP_ESOC;
	HAL_WRITE32(PMC_BASE, SYSPMC_OPT, Rtemp);

	/* enable lp irqs */
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	Rtemp &= (~PMC_BIT_LP_IRQ_MASK);
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

	/* set power mode */
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	Rtemp &= ~PMC_BIT_PMEN_SLEP;
	Rtemp &= ~PMC_BIT_PMEN_DSLP;
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

	/* soc sleep need it, or wakeup will fail */
	FLASH_DeepPowerDown(DISABLE);

	if (ps_config.km0_tickles_debug) {
		RTK_LOGD(TAG, "SOCPS_SleepPG wakeup \n");
	}
	/* disable boot from ps */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG);
	Rtemp &= ~LSYS_BIT_BOOT_WAKE_FROM_PS_LS;
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG, Rtemp);

	NewVectorTable[11] = (HAL_VECTOR_FUN)PMC_BK.PortSVC_Backup;

	/*Refill KM4 IPC configuration*/
	IPCLP_DEV->IPC_IMR = PMC_BK.IPCbackup;
	if (ps_config.km0_tickles_debug) {
		Rtemp = HAL_READ32(PMC_BASE, WAK_STATUS0);
		RTK_LOGD(TAG, "PG wake event %x %x\n", Rtemp,
				 HAL_READ32(PMC_BASE, WAK_STATUS1));
	}

	/* ReFill registers */
	SOCPS_NVICReFill();
	SOCPS_MPUReFill();

}

/**
  * @brief  enter deep sleep mode, patch of SOCPS_DeepSleep.
  * @retval None
  *
  */
SRAM_ONLY_TEXT_SECTION
void SOCPS_DeepSleep_RAM(void)
{
	u32 Rtemp = 0;

	RTK_LOGI(TAG, "M0DS \n");

	/* Enable low power mode */
	FLASH_DeepPowerDown(ENABLE);

	/* set power mode */
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	Rtemp |= PMC_BIT_PMEN_DSLP;
	Rtemp &= ~PMC_BIT_PMEN_SLEP;
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

	/* Wait CHIP enter deep sleep mode */
	__WFI();
}

