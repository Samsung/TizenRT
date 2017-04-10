/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
/*****************************************************************************
 * arch/arm/src/artik053/src/artik053_boot.c
 *
 *   Copyright (C) 2009, 2011, 2013, 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *           Laurent Latil <laurent@latil.nom.fr>
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

/*****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <debug.h>
#include <assert.h>

#include "up_arch.h"

/*****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/*****************************************************************************
 * Public Functions Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Data
 ****************************************************************************/

/*****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_gpio_initialize
 *
 * Description:
 *  Expose board dependent GPIOs
 ****************************************************************************/
static void board_gpio_initialize(void)
{
	/* List of available GPIO IDs */

	int artik053_gpio_list[] = {
		30,	/* ARTIK_A053_XGPIO1 */
		31,	/* ARTIK_A053_XGPIO2 */
		32,	/* ARTIK_A053_XGPIO3 */
		37,	/* ARTIK_A053_XGPIO8 */
		38,	/* ARTIK_A053_XGPIO9 */
		39,	/* ARTIK_A053_XGPIO10 */
		40,	/* ARTIK_A053_XGPIO11 */
		41,	/* ARTIK_A053_XGPIO12 */
		42,	/* ARTIK_A053_XGPIO13 */
		43,	/* ARTIK_A053_XGPIO14 */
		44,	/* ARTIK_A053_XGPIO15 */
		45,	/* ARTIK_A053_XGPIO16 */
		46,	/* ARTIK_A053_XGPIO17 */
		47,	/* ARTIK_A053_XGPIO18 */
		48,	/* ARTIK_A053_XGPIO19 */
		49,	/* ARTIK_A053_XGPIO20 */
		50,	/* ARTIK_A053_XGPIO21 */
		51,	/* ARTIK_A053_XGPIO22 */
		52,	/* ARTIK_A053_XGPIO23 */
		53,	/* ARTIK_A053_XGPIO24 */
		54,	/* ARTIK_A053_XGPIO25 */
		55,	/* ARTIK_A053_XGPIO26 */
		57,	/* ARTIK_A053_XEINT0 */
		58,	/* ARTIK_A053_XEINT1 */
		59	/* ARTIK_A053_XEINT2 */
	};

	int i;
	int count = sizeof(artik053_gpio_list) / sizeof(artik053_gpio_list[0]);

	for (i = 0; i < count; i++) {
		up_create_gpio(artik053_gpio_list[i]);
	}
}

/*****************************************************************************
 * Public Functions
 ****************************************************************************/

static void artik053_clear_bootcount(void)
{
#ifdef CONFIG_ARTIK053_BOOT_COUNTS_ADDR
	/*
	 * As BL2 sets up a watchdog before it jumps to secondary OS,
	 * we should disable the watchdog to prevent it from barking.
	 *
	 * FIXME: use watchdog driver rather than accessing SFR directly
	 */
	putreg32(0, 0x80030000);

	/* then, clear the boot count */
	putreg32(0, CONFIG_ARTIK053_BOOT_COUNTS_ADDR);
#endif
}

/****************************************************************************
 * Name: s5j_board_initialize
 *
 * Description:
 *   All S5J architectures must provide the following entry point. This entry
 *   point is called early in the initialization -- after all memory has been
 *   configured and mapped but before any devices have been initialized.
 *
 *   This function must perform low level initialization including:
 *
 *   - Initialization of board-specific memory resources (e.g., SDRAM)
 *   - Configuration of board-specific resources (GPIOs, LEDs, etc).
 *   - Setup of the console UART. This UART done early so that the serial
 *     console is available for debugging very early in the boot sequence.
 *
 *   Special precautions must be taken if .data/.bss lie in SRAM. In that
 *   case, the boot logic cannot initialize .data or .bss. The function
 *   must then:
 *
 *   - Take precautions to assume that logic does not access any global
 *     data that might lie in SDRAM.
 *   - Call the function arm_data_initialize() as soon as SDRAM has been
 *     properly configured for use.
 *
 ****************************************************************************/
void s5j_board_initialize(void)
{
#ifdef CONFIG_S5J_SFLASH
	s5j_sflash_init();
#endif
}

#ifdef CONFIG_BOARD_INITIALIZE
/****************************************************************************
 * Name: board_initialize
 *
 * Description:
 *   If CONFIG_BOARD_INITIALIZE is selected, then an additional
 *   initialization call will be performed in the boot-up sequence to a
 *   function called board_initialize().  board_initialize() will be
 *   called immediately after up_initialize() is called and just before the
 *   initial application is started.  This additional initialization phase
 *   may be used, for example, to initialize board-specific device drivers.
 *
 ****************************************************************************/
void board_initialize(void)
{
	artik053_clear_bootcount();

	/* Perform app-specific initialization here instaed of from the TASH. */
	board_app_initialize();

#ifdef CONFIG_SCSC_WLAN
	slsi_driver_initialize();
#endif

#ifdef CONFIG_S5J_PWM
	board_pwm_setup();
#endif

	board_gpio_initialize();
}
#endif /* CONFIG_BOARD_INITIALIZE */
