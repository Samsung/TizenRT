/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/stm32/stm32_start.c
 * arch/arm/src/chip/stm32_start.c
 *
 *   Copyright (C) 2009, 2011-2014 Gregory Nutt. All rights reserved.
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

#include "stm32.h"
#include "stm32_gpio.h"

#ifdef CONFIG_ARCH_FPU
#include "nvic.h"
#endif

/****************************************************************************
 * Private Function prototypes
 ****************************************************************************/

#ifdef CONFIG_ARCH_FPU
static inline void stm32_fpuconfig(void);
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
#define showprogress(c) up_lowputc(c)
#else
#define showprogress(c)
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_ARMV7M_STACKCHECK
/* we need to get r10 set before we can allow instrumentation calls */

void __start(void) __attribute__((no_instrument_function));
#endif

/****************************************************************************
 * Name: stm32_fpuconfig
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
#ifdef CONFIG_ARM_CMNVECTOR

static inline void stm32_fpuconfig(void)
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

static inline void stm32_fpuconfig(void)
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
	regval |= ((3 << (2 * 10)) | (3 << (2 * 11)));
	putreg32(regval, NVIC_CPACR);
}

#endif

#else
#define stm32_fpuconfig()
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

#ifdef CONFIG_ARMV7M_STACKCHECK
	/* Set the stack limit before we attempt to call any functions */

	__asm__ volatile
	(
		"sub r10, sp, %0"
		:
		: "r"(CONFIG_IDLETHREAD_STACKSIZE - 64)
		:
	);
#endif

	/* Configure the UART so that we can get debug output as soon as possible */

	stm32_clockconfig();
	stm32_fpuconfig();
	stm32_lowsetup();
	stm32_gpioinit();
	showprogress('A');

	/* Clear .bss.  We'll do this inline (vs. calling memset) just to be
	 * certain that there are no issues with the state of global variables.
	 */

	for (dest = &_sbss; dest < &_ebss;) {
		*dest++ = 0;
	}

	showprogress('B');

	/* Move the initialized data section from his temporary holding spot in
	 * FLASH into the correct place in SRAM.  The correct place in SRAM is
	 * give by _sdata and _edata.  The temporary location is in FLASH at the
	 * end of all of the other read-only data (.text, .rodata) at _eronly.
	 */

	for (src = &_eronly, dest = &_sdata; dest < &_edata;) {
		*dest++ = *src++;
	}

	showprogress('C');

	/* Perform early serial initialization */

#ifdef USE_EARLYSERIALINIT
	up_earlyserialinit();
#endif
	showprogress('D');

	/* Initialize onboard resources */

	stm32_boardinitialize();
	showprogress('E');

	/* Then start TinyAra */

	showprogress('\r');
	showprogress('\n');

#ifdef CONFIG_STACK_COLORATION
	/* Set the IDLE stack to the coloration value and jump into os_start() */

	go_os_start((FAR void *)&_sidle_stack, CONFIG_IDLETHREAD_STACKSIZE);
#else
	/* Call os_start() */

	os_start();

	/* Shoulnd't get here */

	for (;;) ;
#endif
}
