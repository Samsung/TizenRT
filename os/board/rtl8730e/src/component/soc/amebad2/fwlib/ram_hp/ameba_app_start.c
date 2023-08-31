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

#define CONFIG_CM_BACKTRACE 1

#if defined(CONFIG_CM_BACKTRACE) && CONFIG_CM_BACKTRACE
extern void cm_backtrace_fault(uint32_t fault_handler_sp, uint32_t fault_handler_lr);
extern void cm_backtrace_init(const char *firmware_name, const char *hardware_ver, const char *software_ver);
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

void app_fault_handler(uint32_t mstack[], uint32_t pstack[], uint32_t lr_value, uint32_t fault_id)
{
#if defined(CONFIG_CM_BACKTRACE) && CONFIG_CM_BACKTRACE
	UNUSED(fault_id);

	DBG_8195A("\r\nHard Fault Patch (Non-secure)\r\n");

	u8 IsPstack = 0;

	/* EXC_RETURN.S, 1: Registers have been pushed to a Secure stack
	                 0: Registers have been pushed to a Non-Secure stack */
	if (lr_value & BIT_EXCRETURN_S) {				//Taken from S
		DBG_8195A("\nException taken from Secure to Non-secure.\nSecure stack is used to store context."
				  "It can not be dumped from non-secure side for security reason!!!\n");

		while (1);
	} else {										//Taken from NS
		if (lr_value & BIT_EXCRETURN_MODE) {		//Thread Mode
			if (lr_value & BIT_EXCRETURN_SPSEL) {	//PSP
				IsPstack = 1;
			}
		}
	}

	cm_backtrace_fault(IsPstack ? (uint32_t)pstack : (uint32_t)mstack, lr_value);
	while (1);
#else
	UNUSED(mstack);
	UNUSED(pstack);
	UNUSED(lr_value);
	UNUSED(fault_id);
#endif
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
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "DSLP WAKE REASON: %x \n", BootReason);

	if (BootReason & AON_BIT_TIM_ISR_EVT) {
		//DBG_8195A("dslp from aontimer\n");
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

	/* enable non-secure cache */
	Cache_Enable(ENABLE);

	app_section_init();
	_memset((void *) __bss_start__, 0, (__bss_end__ - __bss_start__));

	irq_table_init(MSP_RAM_HP_NS); /* NS Vector table init */
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "VTOR: %x, VTOR_NS:%x\n", SCB->VTOR, SCB_NS->VTOR);

	pinmap_init();

	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "KM4 APP START \n");

	cmse_address_info_t cmse_address_info = cmse_TT((void *)app_start);
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "IMG2 SECURE STATE: %d\n", cmse_address_info.flags.secure);

#if defined (CONFIG_TRUSTZONE_EN) && (CONFIG_TRUSTZONE_EN == 1U)
	BOOT_IMG3();
#endif

	BootReason = BOOT_Reason();
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "KM4 BOOT REASON: %x \n", BootReason);

	/* For reference only, users can modify the function if need */
	/* Attention, the handler is needed to clear NVIC pending int and ip int in dslp flow */
	if ((BootReason & AON_BIT_RSTF_DSLP) != 0) {
		//dslp_wake_handler();
		//depends on user's request
	}

	SystemCoreClockUpdate();
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "KM4 CPU CLK: %d Hz \n", SystemCoreClock);

	XTAL_INIT();

	if (SYSCFG_CHIPType_Get() != CHIP_TYPE_FPGA) {

		if (((BOOT_Reason()) & AON_BIT_RSTF_DSLP) == FALSE) {

			OSC131K_Calibration(30000); /* PPM=30000=3% *//* 7.5ms */
		}
		OSC4M_Calibration(30000);
		XTAL_PDCK();
	}
	/* configure FreeRTOS interrupt and heap region */
	os_heap_init();
	__NVIC_SetVector(SVCall_IRQn, (u32)(VOID *)vPortSVCHandler);
	__NVIC_SetVector(PendSV_IRQn, (u32)(VOID *)xPortPendSVHandler);
	__NVIC_SetVector(SysTick_IRQn, (u32)(VOID *)xPortSysTickHandler);

	cm_backtrace_init("target_img2", "HW v1.0", "SW v1.0");
	FaultPatch_register(app_fault_handler);

#if defined (__GNUC__)
	extern void __libc_init_array(void);
	/* Add This for C++ support */
	__libc_init_array();
#endif

	// force SP align to 8 byte not 4 byte (initial SP is 4 byte align)
	__asm(
		"mov r0, sp\n"
		"bic r0, r0, #7\n"
		"mov sp, r0\n"
	);

	mpu_init();
	app_mpu_nocache_init();

	flash_layout_init();

	main(); /* project/xxxx/src/main.c */
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

