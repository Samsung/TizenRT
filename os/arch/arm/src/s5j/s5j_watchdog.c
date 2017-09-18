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
 * arch/arm/src/s5j/s5j_watchdog.c
 *
 *   Copyright (C) 2014-2016 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * The Samsung sample code has a BSD compatible license that requires this
 * copyright notice:
 *
 *   Copyright (c) 2016 Samsung Electronics, Inc.
 *   All rights reserved.
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
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <string.h>

#include "up_arch.h"

#include "s5j_watchdog.h"
#include <arch/irq.h>
#include <chip.h>
#include <tinyara/clock.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_watchdog_disable
 *
 * Description:
 *   Disable the watchdog timer. The S5J always boots with the watchdog
 *   timer enabled at timeout of 10 - 20 seconds by the second stage boot
 *   loader to detect any boot failure. So, the watchdog timer must be
 *   disabled as part of the start up logic.
 *
 ****************************************************************************/
void s5j_watchdog_disable(void)
{
	unsigned int wtcon = (getreg32(S5J_WDT_WTCON) & ~WTCON_WATCHDOG_EN) | WTCON_WATCHDOG_DIS;

	putreg32(wtcon, S5J_WDT_WTCON);
}

/****************************************************************************
 * Name: s5j_watchdog_enable
 *
 * Description:
 *   Enable watchdog operation.
 *   Should be correctly configured before enabling.
 *
 ****************************************************************************/
void s5j_watchdog_enable(void)
{
	unsigned int wtcon = (getreg32(S5J_WDT_WTCON) & ~WTCON_WATCHDOG_EN) | WTCON_WATCHDOG_EN;

	putreg32(wtcon, S5J_WDT_WTCON);
}

/****************************************************************************
 * Name: s5j_watchdog_reset_disable
 *
 * Description:
 *   When WD timer expires, it can issue HW reset.
 *   This function disables reset feature.
 *   Watchdog will be reloaded with value written in reload register.
 *   and continue its operation.
 *
 ****************************************************************************/
void s5j_watchdog_reset_disable(void)
{
	unsigned int wtcon = (getreg32(S5J_WDT_WTCON) & ~WTCON_RESET_EN) | WTCON_RESET_DIS;

	putreg32(wtcon, S5J_WDT_WTCON);
}

/****************************************************************************
 * Name: s5j_watchdog_reset_enable
 *
 * Description:
 *   When WD timer expires, it can issue HW reset.
 *   This function enables reset feature.
 *
 ****************************************************************************/
void s5j_watchdog_reset_enable(void)
{
	unsigned int wtcon = (getreg32(S5J_WDT_WTCON) & ~WTCON_RESET_EN) | WTCON_RESET_EN;

	putreg32(wtcon, S5J_WDT_WTCON);
}

/****************************************************************************
 * Name: s5j_watchdog_irq_disable
 *
 * Description:
 *   When WD timer expires, it can issue interrupt.
 *   This function disables reset feature.
 *
 ****************************************************************************/
void s5j_watchdog_irq_disable(void)
{
	unsigned int wtcon = (getreg32(S5J_WDT_WTCON) & ~WTCON_IRQ_EN) | WTCON_IRQ_DIS;

	putreg32(wtcon, S5J_WDT_WTCON);
}

/****************************************************************************
 * Name: s5j_watchdog_irq_enable
 *
 * Description:
 *   When WD timer expires, it can issue interrupt.
 *   This function enables reset feature.
 *
 ****************************************************************************/
void s5j_watchdog_irq_enable(void)
{
	unsigned int wtcon = (getreg32(S5J_WDT_WTCON) & ~WTCON_IRQ_EN) | WTCON_IRQ_EN;

	putreg32(wtcon, S5J_WDT_WTCON);
}

/****************************************************************************
 * Name: s5j_watchdog_clk_set
 *
 * Description:
 *   WD timer clock can be defined by prescaler and divider.
 *   WD frequency can be calculated by next equation:
 *   WD_CLK = CLK_IN/((prescaler+1)*divider)
 *   Where:
 *      CLK_IN - input clock frequency (by default 26MHz),
 *      prescaler - 8 bit value 0~255,
 *      divider - 2 bit division factor,
 *		    encoding 0 - 16, 1 - 32, 2 - 64, 3 - 128 division ratios.
 *
 ****************************************************************************/
void s5j_watchdog_clk_set(unsigned int prescaler, unsigned int divider)
{
	unsigned int wtcon = getreg32(S5J_WDT_WTCON);

	wtcon &= ~(WTCON_PRESCALER(0xFF) | WTCON_CLOCK(3));
	wtcon |= WTCON_CLOCK(divider) | WTCON_PRESCALER(prescaler);

	putreg32(wtcon, S5J_WDT_WTCON);
}

/****************************************************************************
 * Name: s5j_watchdog_set_reload_val
 *
 * Description:
 *   When WD timer expires, if reset is disabled, will be reloaded with value
 *   defined by this function call.
 *
 ****************************************************************************/
void s5j_watchdog_set_reload_val(unsigned int reload_val)
{
	putreg32(reload_val, S5J_WDT_WTDAT);
}

/****************************************************************************
 * Name: s5j_watchdog_get_curr
 *
 * Description:
 *   Function s5j_watchdog_get_curr returns current WD counter value.
 ****************************************************************************/
unsigned int s5j_watchdog_get_curr(void)
{
	return getreg32(S5J_WDT_WTCNT);
}

/****************************************************************************
 * Name: s5j_watchdog_set_curr
 *
 * Description:
 *   Function s5j_watchdog_set_curr sets immediately current WD counter value.
 *   Use this function to set initial WD timer value before running operation.
 ****************************************************************************/
void s5j_watchdog_set_curr(unsigned int curr_val)
{
	putreg32(curr_val, S5J_WDT_WTCNT);
}

/****************************************************************************
 * Name: s5j_watchdog_clear_int
 *
 * Description:
 *   Function s5j_watchdog_clear_int clears pending interrupt flag.
 *   Used after WD IRQ service routine completes its operation.
 ****************************************************************************/
void s5j_watchdog_clear_int(void)
{
	putreg32(0xffffffff, S5J_WDT_WTCLRINT);
}
