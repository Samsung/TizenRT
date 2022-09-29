/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

#if defined ( __ICCARM__ )
#pragma section=".ram.bss"
#pragma section=".rom.bss"
#pragma section=".ram.start.table"
#pragma section=".ram_image1.bss"
#pragma section=".ram_image2.entry"

BOOT_RAM_RODATA_SECTION u8 *__image2_entry_func__ = 0;
//BOOT_RAM_RODATA_SECTION u8* __image1_bss_start__ = 0;
//BOOT_RAM_RODATA_SECTION u8* __image1_bss_end__ = 0;
#endif
extern void prvSetupFPU(void);

BOOT_RAM_TEXT_SECTION
void BOOT_InitGP(void)
{
//# Initialize global pointer
	asm volatile(".option push								\n"
				 ".option norelax							\n"
				 "1:										\n"
				 "auipc gp, % pcrel_hi(__global_pointer$)	\n"
				 "addi  gp, gp, % pcrel_lo(1b)				\n"
				 ".option pop								\n"
				);
}

BOOT_RAM_TEXT_SECTION
PRAM_START_FUNCTION BOOT_SectionInit(void)
{
#if defined ( __ICCARM__ )
	// only need __bss_start__, __bss_end__
	__image2_entry_func__		= (u8 *)__section_begin(".ram_image2.entry");
	//__image1_bss_start__		= (u8*)__section_begin(".ram_image1.bss");
	//__image1_bss_end__			= (u8*)__section_end(".ram_image1.bss");
#endif
	return (PRAM_START_FUNCTION)__image2_entry_func__;
}

BOOT_RAM_TEXT_SECTION
void BOOT_CPU_init(void)
{
	/* modify memory layout cache attribute */
	Cache_Attribute_MCCA();
	/*no need to flush icache because 0x0000_0000~0x3FFF_FFFF and 0x6000_0000 ~ 0x7FFF_FFFF is cacheable */
	DCache_Enable();

	BOOT_InitGP();
	prvSetupFPU();
	__DSB();
}

BOOT_RAM_TEXT_SECTION
VOID BOOT_WakeFromPG(VOID)
{
	PRAM_START_FUNCTION Image2EntryFun;
	/* CPU shall be init at the entrance of the function */
	BOOT_CPU_init();

	Image2EntryFun = BOOT_SectionInit();

	if (ps_config.np_config_ddr == APM_PSRAM_SlEEP_Mode || ps_config.np_config_ddr == WB_PSRAM_SlEEP_Mode) {
		if (LSYS_GET_KR4_IS_NP(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1))) {
			set_psram_suspend_and_restore(ENABLE);
		}
	}
	Image2EntryFun->RamWakeupFun();
}

//3 Image 1
void BOOT_Image1(void)
{
	PRAM_START_FUNCTION Image2EntryFun;
	STDLIB_ENTRY_TABLE *prom_stdlib_export_func;
	/* CPU shall be init at the entrance of the function */
	BOOT_CPU_init();

	Image2EntryFun = (PRAM_START_FUNCTION)__image2_entry_func__;
	prom_stdlib_export_func = (STDLIB_ENTRY_TABLE *)__rom_stdlib_text_start__;

	/* Clear ROM bss*/
	_memset((void *) __rom_bss_start__, 0, (__rom_bss_end__ - __rom_bss_start__));

	/* Clear application bss */
	_memset((void *) __sbss_start__, 0, (__sbss_end__ - __sbss_start__));
	_memset((void *) __bss_start__, 0, (__bss_end__ - __bss_start__));

	RBSS_UDELAY_CLK = CPU_ClkGet() / 1000000;

	/* Initial Global Variable */
	BOOT_ROM_InitDebugFlg();

	if (Boot_Log_En && !SYSCFG_OTP_DisBootLog()) {
		LOG_MASK_MODULE(MODULE_BOOT, LEVEL_INFO, ENABLE);
	}
	LOG_MASK_MODULE(MODULE_BOOT, LEVEL_ERROR, ENABLE);
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "KR4 BOOT UP \n");

	/* init stdlib bss and load stdlib data */
	prom_stdlib_export_func->stdlib_init();

	/* Get flash_init_para address for KR4 */
	_memcpy((void *)&flash_init_para, (const void *)HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_FLASH_PARA_ADDR), sizeof(FLASH_InitTypeDef));

	/* goto IMG2 */
	Image2EntryFun->RamStartFun();

	return;
}

IMAGE1_ENTRY_SECTION
RAM_FUNCTION_START_TABLE RamStartTable = {
	.RamStartFun = NULL,
	.RamWakeupFun = BOOT_WakeFromPG,
	.RamPatchFun0 = NULL,
	.RamPatchFun1 = NULL,
	.RamPatchFun2 = NULL,
	.FlashStartFun = BOOT_Image1,
	.Img1ValidCode = NULL,
};
