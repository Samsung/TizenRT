/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/init.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "up_internal.h"

#include "cache.h"
#ifdef CONFIG_ARCH_FPU
#include "nvic.h"
#endif


/****************************************************************************
 * Name: _start
 *
 * Description:
 *   This is the reset entry point.
 *
 ****************************************************************************/

void __start(void)
{
	//prvSetupFPU();
	//os_start();
	app_start();
}
