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
#pragma section=".ram_image2.bss"

SECTION(".data") u8 *__bss_start__ = 0;
SECTION(".data") u8 *__bss_end__ = 0;
#endif

extern int main(void);
extern VOID SOCPS_WakeFromPG(VOID);
void app_section_init(void)
{
#if defined ( __ICCARM__ )
	__bss_start__               = (u8 *)__section_begin(".ram_image2.bss");
	__bss_end__                 = (u8 *)__section_end(".ram_image2.bss");
#endif
}

// The Main App entry point
void app_start(void)
{
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "KR4 APP_START \n");

	/*Exception Handler Init*/
	extern void freertos_risc_v_trap_handler(void);
	__csr_write(mtvec, freertos_risc_v_trap_handler);

	app_section_init();
	//_memset((void *) __bss_start__, 0, (__bss_end__ - __bss_start__));

	SystemCoreClockUpdate();
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "KR4 CPU CLK: %d Hz \n", SystemCoreClock);

	/*use for compensating PMC sleep time*/
	SYSTIMER_Init(); /* 0.2ms */

	/* low power pin dont need pinmap init again after wake from dslp */
	pinmap_init(); /* 1.7ms */


	/* configure FreeRTOS interrupt and heap region */
	os_heap_init();

	/* KR4 can not assign non-cache region which is less than 512M */
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
