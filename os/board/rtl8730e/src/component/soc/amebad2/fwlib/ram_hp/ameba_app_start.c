/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"
#include "ameba_system.h"

static const char *TAG = "APP";
#if defined(CONFIG_EXAMPLE_CM_BACKTRACE) && CONFIG_EXAMPLE_CM_BACKTRACE
#include "cm_backtrace/example_cm_backtrace.h"
#endif

#if defined ( __ICCARM__ )
#pragma section=".ram_image2.bss"
#pragma section="NOCACHE_DATA"
#pragma section=".psram.bss"

SECTION(".data") u8 *__bss_start__ = 0;
SECTION(".data") u8 *__bss_end__ = 0;
SECTION(".data") u8 *__ram_nocache_start__ = 0;
SECTION(".data") u8 *__ram_nocache_end__ = 0;
SECTION(".data") u8 *__psram_bss_start__ = 0;
SECTION(".data") u8 *__psram_bss_end__ = 0;
#endif

extern int main(void);
extern u32 GlobalDebugEnable;
void NS_ENTRY BOOT_IMG3(void);
void app_init_psram(void);
void app_section_init(void)
{
#if defined ( __ICCARM__ )
	__bss_start__               = (u8 *)__section_begin(".ram_image2.bss");
	__bss_end__                 = (u8 *)__section_end(".ram_image2.bss");
	__ram_nocache_start__       = (u8 *)__section_begin("NOCACHE_DATA");
	__ram_nocache_end__         = (u8 *)__section_end("NOCACHE_DATA");
	__ram_nocache_end__ = (u8 *)(((((u32)__ram_nocache_end__ - 1) >> 5) + 1) << 5); //32-byte aligned
#endif
}

u32 app_mpu_nocache_check(u32 mem_addr)
{
	mpu_region_config mpu_cfg;

	mpu_cfg.region_base = (uint32_t)__ram_nocache_start__;
	mpu_cfg.region_size = __ram_nocache_end__ - __ram_nocache_start__;

	if ((mem_addr >= mpu_cfg.region_base) && (mem_addr < (mpu_cfg.region_base + mpu_cfg.region_size))) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/*KM4 have 4 secure mpu entrys & 8 nonsecure mpu entrys*/
u32 app_mpu_nocache_init(void)
{
	mpu_region_config mpu_cfg;
	u32 mpu_entry = 0;

	mpu_entry = mpu_entry_alloc();

	mpu_cfg.region_base = (uint32_t)__ram_nocache_start__;
	mpu_cfg.region_size = __ram_nocache_end__ - __ram_nocache_start__;

	mpu_cfg.xn = MPU_EXEC_ALLOW;
	mpu_cfg.ap = MPU_UN_PRIV_RW;
	mpu_cfg.sh = MPU_NON_SHAREABLE;
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_NC;
	if (mpu_cfg.region_size >= 32) {
		mpu_region_cfg(mpu_entry, &mpu_cfg);
	}

	/* close rom_ns cache */
	mpu_entry = mpu_entry_alloc();
	mpu_cfg.region_base = 0x1E000;
	mpu_cfg.region_size = 0x54000 - 0x1E000;
	mpu_cfg.xn = MPU_EXEC_ALLOW;
	mpu_cfg.ap = MPU_UN_PRIV_RW;
	mpu_cfg.sh = MPU_NON_SHAREABLE;
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_NC;
	mpu_region_cfg(mpu_entry, &mpu_cfg);

	/* set 512Byte retention ram no-cache */
	mpu_entry = mpu_entry_alloc();
	mpu_cfg.region_base = 0x23020000;
	mpu_cfg.region_size = 0x200;
	mpu_cfg.xn = MPU_EXEC_ALLOW;
	mpu_cfg.ap = MPU_UN_PRIV_RW;
	mpu_cfg.sh = MPU_NON_SHAREABLE;
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_NC;
	mpu_region_cfg(mpu_entry, &mpu_cfg);
	return 0;
}

#if defined (__GNUC__)
/* Add This for C++ support to avoid compile error */
void _init(void) {}
#endif

void app_int_memfault(void)
{
	__ASM volatile(
		"MRS 		R0, MSP					\n\t"
		"MRS 		R1, PSP					\n\t"
		"MOV 		R2, LR					\n\t" /* Third parameter is LR current value */
		"MOV 		R3, #3					\n\t"
		"PUSH 		{R4-R11}				\n\t"
		"B			app_fault_handler		\n\t"
	);
}

void app_int_busfault(void)
{
	__ASM volatile(
		"MRS 		R0, MSP					\n\t"
		"MRS 		R1, PSP					\n\t"
		"MOV 		R2, LR					\n\t" /* Third parameter is LR current value */
		"MOV 		R3, #2					\n\t"
		"PUSH 		{R4-R11}				\n\t"
		"B			app_fault_handler		\n\t"
	);
}

void app_int_usagefault(void)
{
	__ASM volatile(
		"MRS 		R0, MSP					\n\t"
		"MRS 		R1, PSP					\n\t"
		"MOV 		R2, LR					\n\t" /* Third parameter is LR current value */
		"MOV 		R3, #1					\n\t"
		"PUSH 		{R4-R11}				\n\t"
		"B			app_fault_handler		\n\t"
	);
}

void app_fault_handler(uint32_t mstack[], uint32_t pstack[], uint32_t lr_value, uint32_t fault_id)
{
	RTK_LOGA(TAG, "Hard Fault Patch (Non-secure)\r\n");

	uint32_t *cstack;

	uint32_t *extra_regs = mstack;

	uint32_t regs[17] = {0x0};

	u8 is_psp = 0;

	enum {
		REG_EXCR = 0, REG_R4, REG_R5, REG_R6, REG_R7, REG_R8, REG_R9, REG_R10, REG_R11, \
		REG_R0, REG_R1, REG_R2, REG_R3, REG_R12, REG_LR, REG_EPC, REG_xPSR, REG_END
	};

	/* EXC_RETURN.S, 1: Registers have been pushed to a Secure stack
	                 0: Registers have been pushed to a Non-Secure stack */

	/* 1 Non-Secure world cannot dump registers about exception from secure world. */
	if (lr_value & EXC_RETURN_S) {
		RTK_LOGA(TAG, "Exception taken from Secure to Non-secure.\nSecure stack is used to store context."
				 "It can not be dumped from non-secure side for security reason!!!\n");
		while (1);
		/* 2 Non-secure world hander exception from non-secure world!*/
	} else {
		if ((lr_value & EXC_RETURN_MODE) && (lr_value & EXC_RETURN_SPSEL)) {
			is_psp = 1;
		}
	}

	regs[REG_EXCR] = lr_value;
	/* MSP stack
	High addr -> |  xxx  | <--- &extra_regs[0] is mstack;
	  ^          |  R11  | <--- extra_regs[-1]
	  |	         |  R10  |
	  |	         |  ...  |
	Low addr  -> |  R4   | <--- extra_regs[-8]*/

	extra_regs--; //point to R11
	for (int i = REG_R4; i <= REG_R11; i++) {
		regs[i] = extra_regs[-REG_R11 + i];
	}

	cstack = is_psp ? pstack : mstack;

	for (int i = REG_R0; i < REG_END; i++) {
		regs[i] = cstack[i - REG_R0];
	}

	crash_dump((uint32_t *)cstack[REG_EPC], cstack, regs);

	extern void INT_HardFault_C(uint32_t mstack[], uint32_t pstack[], uint32_t lr_value, uint32_t fault_id);
	INT_HardFault_C(mstack, pstack, lr_value, fault_id);
}

#if 0
static VOID aontimer_dslp_handler(void)
{
	SOCPS_AONTimerClearINT();
	RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, DISABLE);
}

static VOID rtc_dslp_handler(void)
{
	RTC_AlarmClear();
	RTC_DayIntClear();
	RTC_WakeupClear();
}

static VOID wakepin_dslp_handler(void)
{
	u32 pinidx;
	pinidx = SOCPS_WakePinCheck();
	SOCPS_WakePinClearINT(pinidx);
}

static VOID dslp_wake_handler(void)
{
	u32 BootReason;

	BootReason = SOCPS_AONWakeReason();
	RTK_LOGI(TAG, "DSLP WAKE REASON: %x \n", BootReason);

	if (BootReason & AON_BIT_TIM_ISR_EVT) {
		//RTK_LOGI(TAG, "dslp from aontimer\n");
		RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, ENABLE);
		//SOCPS_AONTimerINT_EN_HP(ENABLE);
		InterruptRegister((IRQ_FUN)aontimer_dslp_handler, AON_TIM_IRQ, NULL, 3);
		InterruptEn(AON_TIM_IRQ, 3);
	}

	if (BootReason & AON_BIT_RTC_ISR_EVT) {
		InterruptRegister((IRQ_FUN)rtc_dslp_handler, RTC_IRQ, NULL, 3);
		InterruptEn(RTC_IRQ, 3);
	}

	if (BootReason & (AON_BIT_GPIO_PIN0_WAKDET_EVT | AON_BIT_GPIO_PIN1_WAKDET_EVT | AON_BIT_GPIO_PIN2_WAKDET_EVT | AON_BIT_GPIO_PIN3_WAKDET_EVT)) {
		InterruptRegister((IRQ_FUN)wakepin_dslp_handler, AON_WAKEPIN_IRQ, NULL, 3);
		InterruptEn(AON_WAKEPIN_IRQ, 3);
	}
}
#endif

// The Main App entry point
void app_start(void)
{
	u32 BootReason;
	/* 1. enable non-secure cache */
	Cache_Enable(ENABLE);

	/* 2. Init heap region for printf (RTK_LOGx) */
	app_section_init();
	_memset((void *) __bss_start__, 0, (__bss_end__ - __bss_start__));
	os_heap_init();
	/* 3. Initialize Non-secure vector table and retarget partly exception handler function. */
	irq_table_init(MSP_RAM_HP_NS); /* NS Vector table init */
	/* Add redefine fault handler to vector table */
	NewVectorTable[4] = (HAL_VECTOR_FUN)app_int_memfault;
	NewVectorTable[5] = (HAL_VECTOR_FUN)app_int_busfault;
	NewVectorTable[6] = (HAL_VECTOR_FUN)app_int_usagefault;

	RTK_LOGI(TAG, "VTOR: %x, VTOR_NS:%x\n", SCB->VTOR, SCB_NS->VTOR);

	/* 4. low power pin dont need pinmap init again after wake from dslp*/
	pinmap_init();

	RTK_LOGI(TAG, "KM4 APP START \n");
	/* 5. Confirm CPU secure state*/
	cmse_address_info_t cmse_address_info = cmse_TT((void *)app_start);
	RTK_LOGI(TAG, "IMG2 SECURE STATE: %d\n", cmse_address_info.flags.secure);
	/* 6. Load secure image*/
#if defined (CONFIG_TRUSTZONE_EN) && (CONFIG_TRUSTZONE_EN == 1U)
	BOOT_IMG3();
#endif
	/* 7. Get Boot reason. */
	BootReason = BOOT_Reason();
	RTK_LOGI(TAG, "KM4 BOOT REASON: %x \n", BootReason);

	/* For reference only, users can modify the function if need */
	/* Attention, the handler is needed to clear NVIC pending int and ip int in dslp flow */
	if ((BootReason & AON_BIT_RSTF_DSLP) != 0) {
		//dslp_wake_handler();
		//depends on user's request
	}
	/* 8. Update CPU clk. */
	SystemCoreClockUpdate();
	RTK_LOGI(TAG, "KM4 CPU CLK: %d Hz \n", SystemCoreClock);

	/* 9. Init XTAL*/
	XTAL_INIT();

	if (SYSCFG_CHIPType_Get() != CHIP_TYPE_FPGA) {

		if (((BOOT_Reason()) & AON_BIT_RSTF_DSLP) == FALSE) {

			OSC131K_Calibration(30000); /* PPM=30000=3% *//* 7.5ms */
		}
		OSC4M_Calibration(30000);
		XTAL_PDCK();
	}
	/* 10. configure FreeRTOS interrupt*/
	__NVIC_SetVector(SVCall_IRQn, (u32)(VOID *)vPortSVCHandler);
	__NVIC_SetVector(PendSV_IRQn, (u32)(VOID *)xPortPendSVHandler);
	__NVIC_SetVector(SysTick_IRQn, (u32)(VOID *)xPortSysTickHandler);

#if defined (__GNUC__)
	extern void __libc_init_array(void);
	/* Add This for C++ support */
	__libc_init_array();
#endif
	/*11. MPU init*/
	mpu_init();
	app_mpu_nocache_init();

	/*12. Enter main*/
	main();
}

IMAGE2_VALID_PATTEN_SECTION
const u8 RAM_IMG2_VALID_PATTEN[20] = {
	'R', 'T', 'K', 'W', 'i', 'n', 0x0, 0xff,
	(FW_VERSION & 0xff), ((FW_VERSION >> 8) & 0xff),
	(FW_SUBVERSION & 0xff), ((FW_SUBVERSION >> 8) & 0xff),
	(FW_CHIP_ID & 0xff), ((FW_CHIP_ID >> 8) & 0xff),
	(FW_CHIP_VER),
	(FW_BUS_TYPE),
	(FW_INFO_RSV1),
	(FW_INFO_RSV2),
	(FW_INFO_RSV3),
	(FW_INFO_RSV4)
};

IMAGE2_ENTRY_SECTION
RAM_START_FUNCTION Img2EntryFun0 = {
	app_start,
	NULL,//BOOT_RAM_WakeFromPG,
	(u32)NewVectorTable
};

