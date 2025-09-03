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
 * os/board/stm32f407-disc1/src/stm32_userleds.c
 *
 *   Copyright (C) 2011 Gregory Nutt. All rights reserved.
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
#include <stdbool.h>
#include <debug.h>

#include <arch/board/board.h>

#include "chip.h"
#include "up_arch.h"
#include "up_internal.h"
#include "stm32.h"

#ifndef CONFIG_ARCH_LEDS

/****************************************************************************
 * Definitions
 ****************************************************************************/

/* CONFIG_DEBUG_LEDS enables debug output from this file (needs CONFIG_DEBUG
 * with CONFIG_DEBUG_VERBOSE too)
 */

#ifdef CONFIG_DEBUG_LEDS
#define leddbg  lldbg
#define ledvdbg llvdbg
#else
#define leddbg(x...)
#define ledvdbg(x...)
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* This array maps an LED number to GPIO pin configuration */

static uint32_t g_ledcfg[BOARD_NLEDS] = {
	GPIO_LED1, GPIO_LED2, GPIO_LED3, GPIO_LED4
};

/****************************************************************************
 * Private Function Protototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/


/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/


/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_ledinit
 ****************************************************************************/

void stm32_ledinit(void)
{
	/* Configure LED1-4 GPIOs for output */

	stm32_configgpio(GPIO_LED1);
	stm32_configgpio(GPIO_LED2);
	stm32_configgpio(GPIO_LED3);
	stm32_configgpio(GPIO_LED4);
}

/****************************************************************************
 * Name: stm32_setled
 ****************************************************************************/

void stm32_setled(int led, bool ledon)
{
	if ((unsigned)led < BOARD_NLEDS) {
		stm32_gpiowrite(g_ledcfg[led], ledon);
	}
}

/****************************************************************************
 * Name: stm32_setleds
 ****************************************************************************/

void stm32_setleds(uint8_t ledset)
{
	stm32_gpiowrite(GPIO_LED1, (ledset & BOARD_LED1_BIT) == 0);
	stm32_gpiowrite(GPIO_LED2, (ledset & BOARD_LED2_BIT) == 0);
	stm32_gpiowrite(GPIO_LED3, (ledset & BOARD_LED3_BIT) == 0);
	stm32_gpiowrite(GPIO_LED4, (ledset & BOARD_LED4_BIT) == 0);
}


#endif							/* !CONFIG_ARCH_LEDS */
