/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/s5j_boot.c
 *
 *   Copyright (C) 2009-2010, 2014-2015 Gregory Nutt. All rights reserved.
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

#include "up_arch.h"
#include "up_internal.h"

#include <chip.h>
#include "s5j_watchdog.h"
#include "arm.h"
#ifdef CONFIG_ARMV7M_MPU
#include "mpu.h"
#endif
#include "cache.h"
#include "fpu.h"

/****************************************************************************
 * Public Data
 ****************************************************************************/
extern uint32_t _vector_start;
extern uint32_t _vector_end;


/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
void up_copyvectorblock(void)
{
	uint32_t *src = (uint32_t *)&_vector_start;
	uint32_t *end = (uint32_t *)&_vector_end;
	uint32_t *dest = (uint32_t *)VECTOR_BASE;

	while (src < end) {
		*dest++ = *src++;
	}
}

#ifdef CONFIG_ARMV7M_MPU
int s5j_mpu_initialize(void)
{
#if CONFIG_ARCH_CHIP_S5JT200
	/*
	 * Vector Table	0x02020000	0x02020FFF	4
	 * Reserved		0x02021000	0x020217FF	2
	 * BL1			0x02021800	0x020237FF	8
	 * TizenRT		0x02023800	0x0210FFFF	946(WBWA)
	 * WIFI			0x02110000	0x0215FFFF	320(NCNB)
	 */

	/* Region 0, Set read only for memory area */
	mpu_priv_flash(0x0, 0x80000000);

	/* Region 1, for ISRAM(0x02020000++1280KB, RW-WBWA */
	mpu_user_intsram_wb(S5J_IRAM_PADDR, S5J_IRAM_SIZE);

	/* Region 2, wifi driver needs non-$(0x02110000++320KB, RW-NCNB */
	mpu_priv_noncache(S5J_IRAM_PADDR + ((4 + 2 + 8 + 946) * 1024), (320 * 1024));

	/* Region 3, for FLASH area, default to set WBWA */
	mpu_user_intsram_wb(S5J_FLASH_PADDR, S5J_FLASH_SIZE);

	/* region 4, for Sflash Mirror area to be read only */
	mpu_priv_flash(S5J_FLASH_MIRROR_PADDR, S5J_FLASH_MIRROR_SIZE);

	/* Region 5, for SFR area read/write, strongly-ordered */
	mpu_priv_stronglyordered(S5J_PERIPHERAL_PADDR, S5J_PERIPHERAL_SIZE);

	/*
	 * Region 6, for vector table,
	 * set the entire high vector region as read-only.
	 */
	mpu_priv_flash(S5J_IRAM_MIRROR_PADDR, S5J_IRAM_MIRROR_SIZE);

#endif
	mpu_control(true);
	return 0;
}
#endif

void arm_boot(void)
{
	up_copyvectorblock();

	/* Disable the watchdog timer */
#ifdef CONFIG_S5J_WATCHDOG
	s5j_watchdog_disable();
#endif

#ifdef CONFIG_ARMV7R_MEMINIT
	/* Initialize the .bss and .data sections as well as RAM functions
	 * now after RAM has been initialized.
	 *
	 * NOTE that if SDRAM were supported, this call might have to be
	 * performed after returning from tms570_board_initialize()
	 */
	arm_data_initialize();
#endif

#ifdef CONFIG_ARMV7M_MPU
	s5j_mpu_initialize();
	arch_enable_icache();
	arch_enable_dcache();
#endif

	s5j_clkinit();

#ifdef USE_EARLYSERIALINIT
	up_earlyserialinit();
#endif

	/*
	 * Perform board-specific initialization. This must include:
	 *
	 * - Initialization of board-specific memory resources (e.g., SDRAM)
	 * - Configuration of board specific resources (GPIOs, LEDs, etc).
	 *
	 * NOTE: we must use caution prior to this point to make sure that
	 * the logic does not access any global variables that might lie
	 * in SDRAM.
	 */
	s5j_board_initialize();

#ifdef CONFIG_STACK_COLORATION
	/*
	 * This function makes idle stack colored.
	 * Do not call any function between this and os_start.
	 */

	go_os_start((void *)&_sidle_stack, CONFIG_IDLETHREAD_STACKSIZE);
#else
	os_start();
#endif
}
