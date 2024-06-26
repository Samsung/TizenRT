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
 *  arch/arm/src/stm32/stm32_idle.c
 *
 *   Copyright (C) 2011-2012 Gregory Nutt. All rights reserved.
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

#include <arch/board/board.h>
#include <tinyara/config.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/pm/pm.h>

#include <arch/irq.h>

#include "chip.h"
#include "stm32_pm.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Does the board support an IDLE LED to indicate that the board is in the
 * IDLE state?
 */

#if defined(CONFIG_ARCH_LEDS) && defined(LED_IDLE)
#define BEGIN_IDLE() board_led_on(LED_IDLE)
#define END_IDLE()   board_led_off(LED_IDLE)
#else
#define BEGIN_IDLE()
#define END_IDLE()
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_PM
/****************************************************************************
 * Name: up_pm_board_sleep
 *
 * Description:
 *   Perform IDLE state power management.
 *
 * Input Parameters:
 *   wakeuphandler - The wakeuphandler function that must be called after each board wakeup.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void up_pm_board_sleep(void (*wakeuphandler)(clock_t, pm_wakeup_reason_code_t))
{
	irqstate_t flags;
	flags = irqsave();
	/* MCU-specific power management logic */
	(void)stm32_pmstandby();
	irqrestore(flags);
}
#else
#define up_pm_board_sleep(wakeuphandler)
#endif

/****************************************************************************
 * Name: up_idle
 *
 * Description:
 *   up_idle() is the logic that will be executed when their is no other
 *   ready-to-run task.  This is processor idle time and will continue until
 *   some interrupt occurs to cause a context switch from the idle task.
 *
 *   Processing in this state may be processor-specific. e.g., this is where
 *   power management operations might be performed.
 *
 ****************************************************************************/

void up_idle(void)
{
#if defined(CONFIG_SUPPRESS_INTERRUPTS) || defined(CONFIG_SUPPRESS_TIMER_INTS)
	/* If the system is idle and there are no timer interrupts, then process
	 * "fake" timer interrupts. Hopefully, something will wake up.
	 */

	sched_process_timer();
#else
	/* NOTE:  There is an STM32F107 errata that is fixed by the following
	 * workaround:
	 *
	 * "2.17.11 Ethernet DMA not working after WFI/WFE instruction
	 *  Description
	 *  If a WFI/WFE instruction is executed to put the system in sleep mode
	 *    while the Ethernet MAC master clock on the AHB bus matrix is ON and all
	 *    remaining masters clocks are OFF, the Ethernet DMA will be not able to
	 *    perform any AHB master accesses during sleep mode."
	 *
	 *  Workaround
	 *    Enable DMA1 or DMA2 clocks in the RCC_AHBENR register before
	 *    executing the WFI/WFE instruction."
	 *
	 * Here the workaround is just to avoid SLEEP mode for the connectivity
	 * line parts if Ethernet is enabled.  The errate recommends a  more
	 * general solution:  Enabling DMA1/2 clocking in stm32f10xx_rcc.c if the
	 * STM32107 Ethernet peripheral is enabled.
	 */
	/* set core to WFI */
#if !defined(CONFIG_STM32_CONNECTIVITYLINE) || !defined(CONFIG_STM32_ETHMAC)
#if !(defined(CONFIG_DEBUG_SYMBOLS) && defined(CONFIG_STM32_DISABLE_IDLE_SLEEP_DURING_DEBUG))
	BEGIN_IDLE();
	asm("WFI");
	END_IDLE();
#endif
#endif
#endif
}
