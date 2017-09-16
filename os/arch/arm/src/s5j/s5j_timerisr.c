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
 * arch/arm/src/s5j/s5j_timerisr.c
 *
 *   Copyright (C) 2009, 2014 Gregory Nutt. All rights reserved.
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
#include <time.h>
#include <tinyara/arch.h>
#include <arch/board/board.h>
#include "up_arch.h"

#include "chip.h"
#include "s5j_rtc.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/*
 * The desired timer interrupt frequency is provided by the definition
 * CLK_TCK (see include/time.h).  CLK_TCK defines the desired number of
 * system clock ticks per second.  That value is a user configurable setting
 * that defaults to 100 (100 ticks per second = 10 MS interval).
 *
 * The timer counts at the rate SYSCLK_FREQUENCY as defined in the board.h
 * header file.
 */
#define SYSTICK_RELOAD ((SYSCLK_FREQUENCY / CLK_TCK) - 1)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  up_timerisr
 *
 * Description:
 *   The timer ISR will perform a variety of services for various portions
 *   of the systems.
 *
 ****************************************************************************/
int up_timerisr(int irq, FAR void *context, FAR void *arg)
{
	/* Clear interrupt pending */
	putreg32(RTC_INTP_TIMETIC0, S5J_RTC_INTP);

	/* Process timer interrupt */
	sched_process_timer();

	return 0;
}

/****************************************************************************
 * Function:  up_timer_initialize
 *
 * Description:
 *   This function is called during start-up to initialize
 *   the timer interrupt.
 *
 ****************************************************************************/
void up_timer_initialize(void)
{
	/* OSC_CON[16] should be set to 1, so that RTC uses XRTCXTO as srcclk */
	modifyreg32(0x800A0554, 0x0, 1 << 16);

	/* Configure the RTC timetick to generate periodic interrupts */
	modifyreg32(S5J_RTC_RTCCON, RTC_RTCCON_TICKEN0_ENABLE, 0);
	putreg32(SYSTICK_RELOAD, S5J_RTC_TICCNT0);
	modifyreg32(S5J_RTC_RTCCON, RTC_RTCCON_TICCKSEL0_MASK, RTC_RTCCON_TICKEN0_ENABLE | RTC_RTCCON_TICCKSEL0_32768HZ);

	/* Attach the timer interrupt vector */
	irq_attach(IRQ_TOP_RTC_TIC, up_timerisr, NULL);

	/* Enable the timer interrupt */
	up_enable_irq(IRQ_TOP_RTC_TIC);
}
