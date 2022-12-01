/****************************************************************************
 * arch/arm/src/stm32l4/stm32l4_start.c
 *
 *   Copyright (C) 2009, 2011-2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

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
#include "nvic.h"

#include "stm32l4.h"
#include "stm32l4_gpio.h"
#include "stm32l4_start.h"
#include "hal/stm32l4xx_hal_interface.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Memory Map ***************************************************************/
/*
 * 0x0800:0000 - Beginning of the internal FLASH.   Address of vectors.
 *               Mapped as boot memory address 0x0000:0000 at reset.
 * 0x080f:ffff - End of flash region (assuming the max of 2MiB of FLASH).
 * 0x1000:0000 - Start of internal SRAM2
 * 0x2000:0000 - Start of internal SRAM and start of .data (_sdata)
 *             - End of .data (_edata) and start of .bss (_sbss)
 *             - End of .bss (_ebss) and bottom of idle stack
 *             - _ebss + CONFIG_IDLETHREAD_STACKSIZE = end of idle stack,
 *               start of heap. NOTE that the ARM uses a decrement before
 *               store stack so that the correct initial value is the end of
 *               the stack + 4;
 */

#define SRAM2_START  STM32L4_SRAM2_BASE
#define SRAM2_END    (SRAM2_START + STM32L4_SRAM2_SIZE)

#define IDLE_STACK ((uintptr_t)&_sidle_stack + CONFIG_IDLETHREAD_STACKSIZE - 4)
#define HEAP_BASE  ((uintptr_t)&_sint_heap_start)

/* g_idle_topstack: The idle task stack starts at the end of BSS _ebss and
 * at _sidle_stack and is of size CONFIG_IDLETHREAD_STACKSIZE.
 * The IDLE thread is the thread that the system boots on and, eventually,
 * becomes the IDLE, do nothing task that runs only when there is nothing
 * else to run.  The heap continues from there until the end of memory.
 * g_idle_topstack is a read-only variable the provides this computed
 * address.
 */

const uintptr_t g_idle_topstack = HEAP_BASE;

/****************************************************************************
 * Private Function prototypes
 ****************************************************************************/

#ifdef CONFIG_ARCH_FPU
static inline void stm32l4_fpuconfig(void);
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: showprogress
 *
 * Description:
 *   Print a character on the UART to show boot status.
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG
#  define showprogress(c) up_lowputc(c)
#else
#  define showprogress(c)
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_ARMV7M_STACKCHECK
/* we need to get r10 set before we can allow instrumentation calls */

void __start(void) __attribute__ ((no_instrument_function));
#endif

/****************************************************************************
 * Name: stm32l4_fpuconfig
 *
 * Description:
 *   Configure the FPU.  Relative bit settings:
 *
 *     CPACR:  Enables access to CP10 and CP11
 *     CONTROL.FPCA: Determines whether the FP extension is active in the
 *       current context:
 *     FPCCR.ASPEN:  Enables automatic FP state preservation, then the
 *       processor sets this bit to 1 on successful completion of any FP
 *       instruction.
 *     FPCCR.LSPEN:  Enables lazy context save of FP state. When this is
 *       done, the processor reserves space on the stack for the FP state,
 *       but does not save that state information to the stack.
 *
 *  Software must not change the value of the ASPEN bit or LSPEN bit while either:
 *   - the CPACR permits access to CP10 and CP11, that give access to the FP
 *     extension, or
 *   - the CONTROL.FPCA bit is set to 1
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_FPU
#ifndef CONFIG_ARMV7M_LAZYFPU

static inline void stm32l4_fpuconfig(void)
{
	uint32_t regval;

	/* Set CONTROL.FPCA so that we always get the extended context frame
	 * with the volatile FP registers stacked above the basic context.
	 */

	regval = getcontrol();
	regval |= (1 << 2);
	setcontrol(regval);

	/* Ensure that FPCCR.LSPEN is disabled, so that we don't have to contend
	 * with the lazy FP context save behaviour.  Clear FPCCR.ASPEN since we
	 * are going to turn on CONTROL.FPCA for all contexts.
	 */

	regval = getreg32(NVIC_FPCCR);
	regval &= ~((1 << 31) | (1 << 30));
	putreg32(regval, NVIC_FPCCR);

	/* Enable full access to CP10 and CP11 */

	regval = getreg32(NVIC_CPACR);
	regval |= ((3 << (2 * 10)) | (3 << (2 * 11)));
	putreg32(regval, NVIC_CPACR);
}

#else

static inline void stm32l4_fpuconfig(void)
{
	uint32_t regval;

	/* Clear CONTROL.FPCA so that we do not get the extended context frame
	 * with the volatile FP registers stacked in the saved context.
	 */

	regval = getcontrol();
	regval &= ~(1 << 2);
	setcontrol(regval);

	/* Ensure that FPCCR.LSPEN is disabled, so that we don't have to contend
	 * with the lazy FP context save behaviour.  Clear FPCCR.ASPEN since we
	 * are going to keep CONTROL.FPCA off for all contexts.
	 */

	regval = getreg32(NVIC_FPCCR);
	regval &= ~((1 << 31) | (1 << 30));
	putreg32(regval, NVIC_FPCCR);

	/* Enable full access to CP10 and CP11 */

	regval = getreg32(NVIC_CPACR);
	regval |= ((3 << (2*10)) | (3 << (2*11)));
	putreg32(regval, NVIC_CPACR);
}

#endif

#else
#  define stm32l4_fpuconfig()
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: _start
 *
 * Description:
 *   This is the reset entry point.
 *
 ****************************************************************************/

void __start(void)
{
	const uint32_t *src;
	uint32_t *dest;

	/* Clear .bss.  We'll do this inline (vs. calling memset) just to be
	 * certain that there are no issues with the state of global variables.
	 */

	for (dest = &_sbss; dest < &_ebss; ) {
		*dest++ = 0;
	}
	/* Move the initialized data section from his temporary holding spot in
	 * FLASH into the correct place in SRAM.  The correct place in SRAM is
	 * give by _sdata and _edata.  The temporary location is in FLASH at the
	 * end of all of the other read-only data (.text, .rodata) at _eronly.
	 */

	for (src = &_eronly, dest = &_sdata; dest < &_edata; ) {
		*dest++ = *src++;
	}

	SystemInit();
	HAL_Init();
	up_sys_timer_initialize();

#ifdef CONFIG_ARMV7M_STACKCHECK
	/* Set the stack limit before we attempt to call any functions */

	__asm__ volatile ("sub r10, sp, %0" : : "r" (CONFIG_IDLETHREAD_STACKSIZE - 64) : );
#endif

#ifdef CONFIG_STM32L4_SRAM2_INIT
	/* The SRAM2 region is parity checked, but upon power up, it will be in
	 * a random state and probably invalid with respect to parity, potentially
	 * generating faults if accessed.  If elected, we will write zeros to the
	 * memory, forcing the parity to be set to a valid state.
	 * NOTE:  this is optional because this may be inappropriate, especially
	 * if the memory is being used for it's battery backed purpose.  In that
	 * case, the first-time initialization needs to be performed by the board
	 * under application-specific circumstances.  On the other hand, if we're
	 * using this memory for, say, additional heap space, then this is handy.
	 */
	for (dest = (uint32_t *)SRAM2_START; dest < (uint32_t *)SRAM2_END; ) {
		*dest++ = 0;
	}
#endif

#if 1 //Dale KIM - LL Driver
	SystemClock_Config();
	stm32l4_fpuconfig();
	ST_USART2_UART_Init();
	stm32l4_clockconfig();
#else
	stm32l4_fpuconfig();
	stm32l4_lowsetup();
#endif
	stm32l4_gpioinit();
	showprogress('A');

#ifdef USE_EARLYSERIALINIT
	/* Perform early serial initialization */
	up_earlyserialinit();
#endif
	showprogress('B');
	showprogress('C');
	showprogress('D');

	/* For Systick enable */
	//up_sys_timer_initialize();

	/* Initialize onboard resources */
	//board_initialize();
	showprogress('E');

	/* Then start NuttX */
	showprogress('\r');
	showprogress('\n');

#ifdef CONFIG_STACK_COLORATION
	/* Set the IDLE stack to the coloration value and jump into nx_start() */

	go_os_start((FAR void *)&_sidle_stack, CONFIG_IDLETHREAD_STACKSIZE);
#else
	/* Call os_start() */
	up_sys_timer_deinitialize();
	os_start();

	/* Shoulnd't get here */

	for (; ; );
#endif
}
