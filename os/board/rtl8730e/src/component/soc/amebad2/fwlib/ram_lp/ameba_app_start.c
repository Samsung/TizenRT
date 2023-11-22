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
#if defined ( __ICCARM__ )
#pragma section=".ram_image2.bss"
#pragma section="NOCACHE_DATA"

SECTION(".data") u8 *__bss_start__ = 0;
SECTION(".data") u8 *__bss_end__ = 0;
SECTION(".data") u8 *__ram_nocache_start__ = 0;
SECTION(".data") u8 *__ram_nocache_end__ = 0;
#endif

extern int main(void);
extern VOID SOCPS_WakeFromPG(VOID);
void app_section_init(void)
{
#if defined ( __ICCARM__ )
	__bss_start__               = (u8 *)__section_begin(".ram_image2.bss");
	__bss_end__                 = (u8 *)__section_end(".ram_image2.bss");
	__ram_nocache_start__       = (u8 *)__section_begin("NOCACHE_DATA");
	__ram_nocache_end__         = (u8 *)__section_end("NOCACHE_DATA");
#endif
}

void app_fault_handler(uint32_t mstack[], uint32_t pstack[], uint32_t lr_value, uint32_t fault_id)
{
	RTK_LOGA(TAG, "Hard Fault Patch (KM0)\r\n");
	if (fault_id == 0) {
		RTK_LOGA(TAG, "Hard Fault: \r\n");
	} else {
		RTK_LOGA(TAG, "Unsupported faults\r\n");
		while (1);
	}
	uint32_t *cstack;

	uint32_t *extra_regs = mstack;

	uint32_t regs[17] = {0x0};

	u8 is_psp = 0;

	enum {
		REG_EXCR = 0, REG_R4, REG_R5, REG_R6, REG_R7, REG_R8, REG_R9, REG_R10, REG_R11, \
		REG_R0, REG_R1, REG_R2, REG_R3, REG_R12, REG_LR, REG_EPC, REG_xPSR, REG_END
	};

	if (lr_value & BIT(3)) {		//Thread Mode
		if (lr_value & BIT(2)) { 	//PSP
			is_psp = 1;
		}
	}
	regs[REG_EXCR] = lr_value;
	/* MSP stack
	High addr -> |  xxx  | <--- &extra_regs[0] is mstack;
	  ^          |  R7   | <--- extra_regs[-1]
	  |	         |  R6   |
	  |	         |  ...  |
	Low addr  -> |  R4   | <--- extra_regs[-4]*/

	//point to R7, R8 ~ R11 were not saved.
	extra_regs--;
	for (int i = REG_R4; i <= REG_R7; i++) {
		regs[i] = extra_regs[-REG_R7 + i];
	}

	cstack = is_psp ? pstack : mstack;

	for (int i = REG_R0; i < REG_END; i++) {
		regs[i] = cstack[i - REG_R0];
	}

	crash_dump((uint32_t *)cstack[REG_EPC], cstack, regs);

	RTK_LOGA(TAG, "MSP     = 0x%08x\r\n", mstack);
	RTK_LOGA(TAG, "PSP     = 0x%08x\r\n", pstack);
	RTK_LOGA(TAG, "CONTROL = 0x%08x\r\n", __get_CONTROL());
	RTK_LOGA(TAG, "SVC priority: 	 %d\r\n", __NVIC_GetPriority(SVCall_IRQn));
	RTK_LOGA(TAG, "PendSVC priority:  %d\r\n", __NVIC_GetPriority(PendSV_IRQn));
	RTK_LOGA(TAG, "Systick priority:  %d\r\n", __NVIC_GetPriority(SysTick_IRQn));
	RTK_LOGA(TAG, "\r\n");
	/*crash dump for debug*/
	extern void INT_HardFault_C(u32 * stack);
	INT_HardFault_C(cstack);
}

void app_int_hardfault(void)
{
	__ASM volatile(
		"MRS 		R0, MSP					\n\t"
		"MRS 		R1, PSP					\n\t"
		"MOV 		R2, LR					\n\t" /* Third parameter is LR current value */
		"MOV 		R3, #0					\n\t"
		"PUSH 		{R4-R7}					\n\t"
		"B			app_fault_handler		\n\t"
	);
}

//set all KM0 rom & ram no-cachable, just flash cachable
//KM0 have 4 mpu entrys
//0x0000_0000	0x0001_7FFF	96K		KM0 ITCM ROM
//0x0008_0000	0x0008_3FFF	16K		KM0 DTCM ROM
//0x2300_0000	0x2301_0000	64K		KM0 SRAM
//0x2400_0000	0x2400_01FF	0.5K	KM0 Retention SRAM
u32 app_mpu_nocache_init(void)
{
	mpu_region_config mpu_cfg;
	u32 mpu_entry = 0;

	/* close rom cache */
	mpu_entry = mpu_entry_alloc();
	mpu_cfg.region_base = 0x00000000;
	mpu_cfg.region_size = 0x10000;
	mpu_cfg.xn = MPU_EXEC_ALLOW;
	mpu_cfg.ap = MPU_UN_PRIV_RW;
	mpu_cfg.sh = MPU_NON_SHAREABLE;
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_NC;
	mpu_region_cfg(mpu_entry, &mpu_cfg);

	/* set nocache region */
	mpu_entry = mpu_entry_alloc();
	mpu_cfg.region_base = (uint32_t)__ram_nocache_start__;
	mpu_cfg.region_size = __ram_nocache_end__ - __ram_nocache_start__;
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

VOID app_start_autoicg(VOID)
{
	u32 temp = 0;

	/*Enable LP platform auto ICG*/
	temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_LPLAT_CTRL);
	temp |= LSYS_BIT_PLFM_AUTO_ICG_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_LPLAT_CTRL, temp);

	/*Enable HP platform auto ICG*/
	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HPLAT_CTRL);
	temp |= HSYS_BIT_PLFM_AUTO_ICG_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HPLAT_CTRL, temp);
}

// The Main App entry point
void app_start(void)
{
	/* 1. Init heap region for printf (RTK_LOGx) */
	app_section_init();
	_memset((void *) __bss_start__, 0, (__bss_end__ - __bss_start__));
	os_heap_init();

	RTK_LOGI(TAG, "KM0 APP_START \n");
	/* 2. Redirect hardfault for debug */
	NewVectorTable[3] = (HAL_VECTOR_FUN)app_int_hardfault;
	/* 3. Update CPU clock */
	SystemCoreClockUpdate();
	RTK_LOGI(TAG, "KM0 CPU CLK: %d Hz \n", SystemCoreClock);

	/* 4. use for compensating PMC sleep time*/
	SYSTIMER_Init(); /* 0.2ms */

	//app_start_autoicg();

	/* 5. configure FreeRTOS interrupt*/
	__NVIC_SetVector(SVCall_IRQn, (u32)vPortSVCHandler);
	__NVIC_SetVector(PendSV_IRQn, (u32)xPortPendSVHandler);
	__NVIC_SetVector(SysTick_IRQn, (u32)xPortSysTickHandler);

	RTK_LOGI(TAG, "KM0 VTOR:0x%x \n", SCB->VTOR);
	/* 6. MPU init*/
	mpu_init();

	app_mpu_nocache_init();
	/* 7. Enter Main. */
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
	SOCPS_WakeFromPG,
	NULL,
};
