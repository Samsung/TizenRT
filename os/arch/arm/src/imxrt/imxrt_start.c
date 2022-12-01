/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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
 * os/arch/arm/src/imxrt/imxrt_start.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
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

#include "cache.h"
#ifdef CONFIG_ARCH_FPU
#include "nvic.h"
#endif

#include "imxrt_clockconfig.h"
#include "up_mpuinit.h"
#include "imxrt_start.h"
#include "imxrt_gpio.h"
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
#include "chip/imxrt102x_config.h"
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#include "chip/imxrt105x_config.h"
#else
#error Unrecognized i.MX RT architecture
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Memory Map ***************************************************************/
/* 0x2020:0000 - Start of on-chip RAM (OCRAM) and start of .data (_sdata)
 *             - End of .data (_edata) and start of .bss (_sbss)
 *             - End of .bss (_ebss) and start of idle stack (_sidle_stack)
 *             - _sidle_stack + CONFIG_IDLETHREAD_STACKSIZE = end of idle stack,
 *             - _sint_heap_start start of heap. NOTE that the ARM uses a
 *               decrement before store stack so that the correct initial value
 *               is the end of the stack + 4;
 * 0x2027:ffff - End of OCRAM and end of heap (assuming 512Kb OCRAM)
 *
 * NOTE:  This assumes that all internal RAM is configured for OCRAM (vs.
 * ITCM or DTCM).  The RAM that holds .data and .bss is called the "Primary
 * RAM".  Many other configurations are possible, including configurations
 * where the primary ram is in external memory.  Those are not considered
 * here.
 */

#define IDLE_STACK ((uintptr_t)&_sidle_stack + CONFIG_IDLETHREAD_STACKSIZE - 4)
#define HEAP_BASE  ((uintptr_t)&_sint_heap_start)

/****************************************************************************
 * Public data
 ****************************************************************************/

const uintptr_t g_idle_topstack = (uintptr_t)&_sidle_stack + CONFIG_IDLETHREAD_STACKSIZE;

/****************************************************************************
 * Private Function prototypes
 ****************************************************************************/

#ifdef CONFIG_ARCH_FPU
static inline void imxrt_fpuconfig(void);
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_ARMV7M_STACKCHECK
/* we need to get r10 set before we can allow instrumentation calls */

void __start(void) __attribute__((no_instrument_function));
#endif

/****************************************************************************
 * Name: imxrt_fpuconfig
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
 *  Software must not change the value of the ASPEN bit or LSPEN bit while
 *  either:
 *
 *   - the CPACR permits access to CP10 and CP11, that give access to the FP
 *     extension, or
 *   - the CONTROL.FPCA bit is set to 1
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_FPU
#ifndef CONFIG_ARMV7M_LAZYFPU

static inline void imxrt_fpuconfig(void)
{
	uint32_t regval;

	/* Set CONTROL.FPCA so that we always get the extended context frame
	 * with the volatile FP registers stacked above the basic context.
	 */

	regval = getcontrol();
	regval |= (1 << 2);
	setcontrol(regval);

	/* Ensure that FPCCR.LSPEN is disabled, so that we don't have to contend
	 * with the lazy FP context save behavior.  Clear FPCCR.ASPEN since we
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

static inline void imxrt_fpuconfig(void)
{
	uint32_t regval;

	/* Clear CONTROL.FPCA so that we do not get the extended context frame
	 * with the volatile FP registers stacked in the saved context.
	 */

	regval = getcontrol();
	regval &= ~(1 << 2);
	setcontrol(regval);

	/* Ensure that FPCCR.LSPEN is disabled, so that we don't have to contend
	 * with the lazy FP context save behavior.  Clear FPCCR.ASPEN since we
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
#define imxrt_fpuconfig()
#endif

/****************************************************************************
 * Name: imxrt_tcmenable
 *
 * Description:
 *   Enable/disable tightly coupled memories.  Size of tightly coupled
 *   memory regions is controlled by GPNVM Bits 7-8.
 *
 ****************************************************************************/

static inline void imxrt_tcmenable(void)
{
	uint32_t regval;

	ARM_DSB();
	ARM_ISB();

	/* Enabled/disabled ITCM */

#ifdef CONFIG_ARMV7M_ITCM
	regval = NVIC_TCMCR_EN | NVIC_TCMCR_RMW | NVIC_TCMCR_RETEN;
#else
	regval = getreg32(NVIC_ITCMCR);
	regval &= ~NVIC_TCMCR_EN;
#endif
	putreg32(regval, NVIC_ITCMCR);

	/* Enabled/disabled DTCM */

#ifdef CONFIG_ARMV7M_DTCM
	regval = NVIC_TCMCR_EN | NVIC_TCMCR_RMW | NVIC_TCMCR_RETEN;
#else
	regval = getreg32(NVIC_DTCMCR);
	regval &= ~NVIC_TCMCR_EN;
#endif
	putreg32(regval, NVIC_DTCMCR);

	ARM_DSB();
	ARM_ISB();

#ifdef CONFIG_ARMV7M_ITCM
	/* Copy TCM code from flash to ITCM */

#warning Missing logic
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_ARMV7M_DTCM
/****************************************************************************
 * Name: imxrt_configure_dtcm
 *
 * Description:
 *   Make configuration to use the entire 512KB FlexRAM as DTCM
 *
 ****************************************************************************/
static void imxrt_configure_dtcm(void)
{
	/* Configure FlexRAM banks for DTCM and 64KB OCRAM */
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
	/* It is mandatory to have at least 32KB as OCRAM to boot the board */
	IOMUXC_GPR->GPR17 |= IOMUXC_GPR_GPR17_FLEXRAM_BANK_CFG(0x00005aaa);
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
	/* It is mandatory to have at least 64KB as OCRAM to boot the board */
	IOMUXC_GPR->GPR17 |= IOMUXC_GPR_GPR17_FLEXRAM_BANK_CFG(0x55aaaaaa);
#endif
	IOMUXC_GPR->GPR16 |= IOMUXC_GPR_GPR16_FLEXRAM_BANK_CFG_SEL(0x1);

	/* Configure and enable DTCM for 256KB */
	IOMUXC_GPR->GPR16 &= ~IOMUXC_GPR_GPR16_INIT_DTCM_EN_MASK;
	IOMUXC_GPR->GPR14 &= ~IOMUXC_GPR_GPR14_CM7_CFGDTCMSZ_MASK;
	IOMUXC_GPR->GPR14 |= IOMUXC_GPR_GPR14_CM7_CFGDTCMSZ(0x9);
	IOMUXC_GPR->GPR16 |= IOMUXC_GPR_GPR16_INIT_DTCM_EN_MASK;

	/* Disable ITCM */
	IOMUXC_GPR->GPR16 &= ~IOMUXC_GPR_GPR16_INIT_ITCM_EN_MASK;
	IOMUXC_GPR->GPR14 &= ~IOMUXC_GPR_GPR14_CM7_CFGITCMSZ_MASK;
	IOMUXC_GPR->GPR14 |= IOMUXC_GPR_GPR14_CM7_CFGITCMSZ(0x0);
}

#else
/****************************************************************************
 * Name: imxrt_configure_ocram
 *
 * Description:
 *   Make configuration to use the entire 512KB FlexRAM as OCRAM
 *
 ****************************************************************************/
static void imxrt_configure_ocram(void)
{
	/* Configure FlexRAM banks for OCRAM*/
	IOMUXC_GPR->GPR17 |= IOMUXC_GPR_GPR17_FLEXRAM_BANK_CFG(0x55555555);
	IOMUXC_GPR->GPR16 |= IOMUXC_GPR_GPR16_FLEXRAM_BANK_CFG_SEL(0x1);

	/* Disable DTCM */
	IOMUXC_GPR->GPR16 &= ~IOMUXC_GPR_GPR16_INIT_DTCM_EN_MASK;
	IOMUXC_GPR->GPR14 &= ~IOMUXC_GPR_GPR14_CM7_CFGDTCMSZ_MASK;
	IOMUXC_GPR->GPR14 |= IOMUXC_GPR_GPR14_CM7_CFGDTCMSZ(0x0);

	/* Disable ITCM */
	IOMUXC_GPR->GPR16 &= ~IOMUXC_GPR_GPR16_INIT_ITCM_EN_MASK;
	IOMUXC_GPR->GPR14 &= ~IOMUXC_GPR_GPR14_CM7_CFGITCMSZ_MASK;
	IOMUXC_GPR->GPR14 |= IOMUXC_GPR_GPR14_CM7_CFGITCMSZ(0x0);
}
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
	const uint32_t *src;
	uint32_t *dest;

#ifdef CONFIG_ARMV7M_DTCM
	imxrt_configure_dtcm();
#else
	imxrt_configure_ocram();
#endif

#ifdef CONFIG_ARMV7M_STACKCHECK
	/* Set the stack limit before we attempt to call any functions */

	__asm__ volatile("sub r10, sp, %0" : : "r"(CONFIG_IDLETHREAD_STACKSIZE - 64) : );
#endif

	/* Clear .bss.  We'll do this inline (vs. calling memset) just to be
	 * certain that there are no issues with the state of global variables.
	 */

	for (dest = &_sbss; dest < &_ebss;) {
		*dest++ = 0;
	}

	/* Move the initialized data section from his temporary holding spot in
	 * FLASH into the correct place in OCRAM.  The correct place in OCRAM is
	 * give by _sdata and _edata.  The temporary location is in FLASH at the
	 * end of all of the other read-only data (.text, .rodata) at _eronly.
	 */

	for (src = &_eronly, dest = &_sdata; dest < &_edata;) {
		*dest++ = *src++;
	}

	/* Copy any necessary code sections from FLASH to RAM.  The correct
	 * destination in OCRAM is given by _sramfuncs and _eramfuncs.  The
	 * temporary location is in flash after the data initialization code
	 * at _framfuncs.  This should be done before imxrt_clockconfig() is
	 * called (in case it has some dependency on initialized C variables).
	 */

#ifdef CONFIG_ARCH_RAMFUNCS
	for (src = &_framfuncs, dest = &_sramfuncs; dest < &_eramfuncs;) {
		*dest++ = *src++;
	}
#endif

	/* Configure the UART so that we can get debug output as soon as possible */

	imxrt_clockconfig();
	imxrt_fpuconfig();
	imxrt_lowsetup();

	/* Enable/disable tightly coupled memories */

	imxrt_tcmenable();

	/* Initialize onboard resources */

	imxrt_boardinitialize();

#ifdef  CONFIG_ARMV7M_MPU
	/* Configure the MPU to permit user-space access to its FLASH and RAM (for
	 * CONFIG_BUILD_PROTECTED) or to manage cache properties in external
	 * memory regions.
	 */

	up_mpuinitialize();
#endif

	/* Enable I- and D-Caches */

	arch_dcache_writethrough();
	arch_enable_icache();
	arch_enable_dcache();

	/* Perform early serial initialization */

#ifdef USE_EARLYSERIALINIT
	up_earlyserialinit();
#endif

	/* Then start TinyARA */

#ifdef CONFIG_STACK_COLORATION
	/* Set the IDLE stack to the coloration value and jump into os_start() */

	go_os_start((FAR void *)&_sidle_stack, CONFIG_IDLETHREAD_STACKSIZE);
#else
	/* Call os_start() */

	os_start();

	/* Shouldn't get here */

	for (;;) ;
#endif
}
