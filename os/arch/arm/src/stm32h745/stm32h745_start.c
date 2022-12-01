/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/stm32h745/stm32h745_start.c
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
#include <tinyara/mm/heap_regioninfo.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/init.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "up_internal.h"
#include "nvic.h"

#include <stm32h7xx_hal.h>
#include <system_stm32h745.h>
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

//#define SRAM2_START  STM32L4_SRAM2_BASE
//#define SRAM2_END    (SRAM2_START + STM32L4_SRAM2_SIZE)

#define IDLE_STACK ((uintptr_t)&_sidle_stack + CONFIG_IDLETHREAD_STACKSIZE - 4)
#define HEAP_BASE  ((uintptr_t)&_sint_heap_start)

/* g_idle_topstack: The idle task stack starts at the end of BSS _ebss and
 * at _sidle_stack and is of size CONFIG_IDLETHREAD_STACKSIZE.
 * The IDLE thread is the thread that the system boots on and, eventually,
 * becomes the IDLE, do nothing task that runs only when there is nothing
 * else to run. The heap continues from there until the end of memory.
 * g_idle_topstack is a read-only variable the provides this computed
 * address.
 */

uint8_t _header[] __attribute__((section(".header"))) = {
        [0 ... 15] = 0xFF /* 12 bytes for header and 4 bytes for checksum */
};


const uintptr_t g_idle_topstack = HEAP_BASE;

/****************************************************************************
 * Private Function prototypes
 ****************************************************************************/

#ifdef CONFIG_ARCH_FPU
static inline void stm32h745_fpuconfig(void);
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

static inline void stm32h745_fpuconfig(void)
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

static inline void stm32h745_fpuconfig(void)
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
#  define stm32h745_fpuconfig()
#endif

/****************************************************************************
 * Default MPU setting
 ****************************************************************************/
static void stm32h745_mpu_config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct = {0};

	/* Disables the MPU */
	HAL_MPU_Disable();

	/** Initializes and configures the Region and the memory to be protected
	*/
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress = 0x0;
	MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
	MPU_InitStruct.SubRegionDisable = 0x87;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	/* Enables the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}


static void stm32h745_os_heap_init(void)
{

}
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
extern HAL_StatusTypeDef HAL_Init(void);
void __start(void)
{
	const uint32_t *src;
	uint32_t *dest;
	volatile uint32_t delay=0xFFFFFF;
	int option_value;

	/* Add delay to wait for M4 System clock completed 10msec */
	stm32h745_get_boot_control(1, &option_value);
	while((delay--) && (option_value == 1))
	{
		__NOP();
	}

	/* Clear .bss.  We'll do this inline (vs. calling memset) just to be
	 * certain that there are no issues with the state of global variables.
	 */
	for (dest = &_sbss; dest < &_ebss; )
	{
		*dest++ = 0;
	}

	/* Move the initialized data section from his temporary holding spot in
	 * FLASH into the correct place in SRAM.  The correct place in SRAM is
	 * give by _sdata and _edata.  The temporary location is in FLASH at the
	 * end of all of the other read-only data (.text, .rodata) at _eronly.
	 */
	for (src = &_eronly, dest = &_sdata; dest < &_edata; )
	{
		*dest++ = *src++;
	}

#ifdef CONFIG_ARCH_RAMFUNCS
	for (src = &_framfuncs, dest = &_sramfuncs; dest < &_eramfuncs;)
	{
		*dest++ = *src++;
	}
#endif

	/* clear axi sram */
	for(dest = (uint32_t *)0x24000000; dest < (uint32_t *)(0x24000000 + (512 * 1024)); )
	{
		*dest++ = 0;
	}

	SystemInit();
	stm32h745_mpu_config();
	HAL_Init();
	stm32h745_os_heap_init();
#ifdef CONFIG_ARMV7M_STACKCHECK
	/* Set the stack limit before we attempt to call any functions */
	__asm__ volatile ("sub r10, sp, %0" : : "r" (CONFIG_IDLETHREAD_STACKSIZE - 64) : );
#endif

	stm32h745_fpuconfig();

#ifdef USE_EARLYSERIALINIT
	up_earlyserialinit();
#endif
	showprogress('S');
	showprogress('T');
	showprogress('M');
	showprogress('3');
	showprogress('2');
	showprogress('H');
	showprogress('7');
	showprogress('\r');
	showprogress('\n');

#ifdef CONFIG_STACK_COLORATION
	/* Set the IDLE stack to the coloration value and jump into nx_start() */

	go_os_start((FAR void *)&_sidle_stack, CONFIG_IDLETHREAD_STACKSIZE);
#else
	/* Call os_start() */
	os_start();

	/* Shoulnd't get here */

	for (; ; );
#endif
}
