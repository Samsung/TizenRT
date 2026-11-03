/*
 * Copyright (C) 2020 Realtek Semiconductor Corp.  All Rights Reserved.
 */

/* Standard includes. */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <tinyara/config.h>
/* Scheduler includes. */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include "FreeRTOS.h"
#include "task.h"     /* RTOS task related API prototypes. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */
#endif

#include "ameba_soc.h"

#ifndef CONFIG_PLATFORM_TIZENRT_OS
extern int main(void);
extern void arm_gic_set_CUTVersion(uint32_t CUTVersion);
#endif

#if defined (__GNUC__)
/* Add This for C++ support to avoid compile error */
void _init(void) {}
#endif

#ifdef CONFIG_PLATFORM_TIZENRT_OS
/* The heap of TizenRT on NuttX 12.13.0 comes from up_allocate_heap()
 * (os/arch/arm/src/amebasmart/amebasmart_allocateheap.c).
 */
#endif

/*
 * Starts all the other tasks, then starts the scheduler.
 */
void app_start(void)
{
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	/* GIC need know current Cut Version */
	arm_gic_set_CUTVersion(SYSCFG_RLVersion());
#endif

	RTK_LOG_MASK(LEVEL_ERROR, 0xFFFFFFFF);

	pinmap_init();
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined (__GNUC__)
	extern void __libc_init_array(void);
	/* Add This for C++ support */
	__libc_init_array();
#endif
#endif

#ifndef CONFIG_PLATFORM_TIZENRT_OS
	/* Configure the hardware ready to run the demo. */
	prvSetupHardware();
#endif

	/* Get flash_init_para info for AP */
	_memcpy((void *)&flash_init_para, (const void *)HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FLASH_PARA_ADDR), sizeof(FLASH_InitTypeDef));

	// flash_layout_init();

#ifndef CONFIG_PLATFORM_TIZENRT_OS
	main();
#endif
#if defined( __ICCARM__ )
	extern void __iar_data_init3();
	__iar_data_init3();
#endif
}
