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
#include <tinyara/clock.h>
#include <tinyara/arch.h>
#include <arch/board/board.h>
#include "up_arch.h"

#include "chip.h"
#include "s5j_mct.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifndef CONFIG_S5J_MCT
#  error SYSTICK depends the MCT Driver. Should be enable MCT in chip menu.
#endif

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
	s5j_mct_ack_irq((FAR struct s5j_mct_priv_s *)arg);

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
#define TICK_INTERVAL (USEC_PER_SEC / CLK_TCK)
	FAR struct s5j_mct_priv_s *mct =
			s5j_mct_init(S5J_MCT_CHANNEL3);
	irqstate_t flags = irqsave();

	if (mct) {
		s5j_mct_disableint(mct);

		/* Configure the timetick to generate periodic interrupts */
		s5j_mct_setperiod(mct, TICK_INTERVAL);

		/* use interval mode */
		s5j_mct_setmode(mct, false);

		/* Attach the timer interrupt vector */
		s5j_mct_setisr(mct, up_timerisr, mct);

		/* Enable the timer interrupt */
		s5j_mct_enableint(mct);

		s5j_mct_enable(mct);
	}

#ifdef CONFIG_SYSVIEW_HWTIMER
	FAR struct s5j_mct_priv_s *mct2 =
			s5j_mct_init(S5J_MCT_CHANNEL2);

	if (mct2) {
		/* Configure the timetick to generate periodic interrupts */
		s5j_mct_setperiod(mct2, USEC_PER_SEC);

		/* use interval mode */
		s5j_mct_setmode(mct2, false);

		s5j_mct_enable(mct2);
	}
#endif

	irqrestore(flags);
#undef TICK_INTERVAL
}

#ifdef CONFIG_SYSVIEW_HWTIMER
/****************************************************************************
 * Function:  up_timer_get_current_ticks
 *
 * Description:
 *   This function allows you to get real time ticks per one tick period.
 *
 ****************************************************************************/
unsigned int up_timer_get_current_ticks(void)
{
	FAR struct s5j_mct_priv_s *mct =
			s5j_mct_init(S5J_MCT_CHANNEL2);

	return s5j_mct_gettick(mct);
}
#endif
