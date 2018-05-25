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
 * arch/arm/src/s5j/s5j_tickless.c
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
 * Tickless OS Support.
 *
 * When CONFIG_SCHED_TICKLESS is enabled, all support for timer interrupts
 * is suppressed and the platform specific code is expected to provide the
 * following custom functions.
 *
 *   void up_timer_initialize(void): Initializes the timer facilities.  Called
 *     early in the intialization sequence (by up_intialize()).
 *   int up_timer_gettime(FAR struct timespec *ts):  Returns the current
 *     time from the platform specific time source.
 *   int up_alarm_cancel(void):  Cancels the interval timer.
 *   int up_alarm_start(FAR const struct timespec *ts): Start (or re-starts)
 *     the interval timer.
 *
 * The RTOS will provide the following interfaces for use by the platform-
 * specific interval timer implementation:
 *
 *   void sched_timer_expiration(void):  Called by the platform-specific
 *     logic when the interval timer expires.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <time.h>
#include <debug.h>
#include <tinyara/arch.h>

#include "s5j_mct.h"
#include "s5j_clock.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct s5j_mct_priv_s *g_mct;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_convert_systimr
 *
 * Description:
 *   Convert the 64-bit system timer value to a standard struct ts
 *
 * Input Parameters:
 *   ts = Location to return the converted system timer value
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void s5j_convert_systimr(u64 usec, FAR struct timespec *ts)
{
	u32 sec;

	sec         = (u32)(usec / USEC_PER_SEC);
	ts->tv_sec  = sec;
	ts->tv_nsec = (usec - (sec * USEC_PER_SEC)) * NSEC_PER_USEC;
}

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
int up_timerisr(int irq, uint32_t *regs)
{
	struct timespec ts;
	u64 cnt_val;

	s5j_mct_clear_pending(g_mct, S5J_MCT_GLOBAL, MCT_G0);

	cnt_val = s5j_mct_global_get_frc_cnt(g_mct);

	s5j_convert_systimr(cnt_val, &ts);

	/* Process timer interrupt */
	sched_alarm_expiration(&ts);

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
	/* Initialize MCT */
	g_mct = s5j_mct_init(S5J_MCT_GLOBAL);

	DEBUGASSERT(g_mct);

	s5j_mct_global_set_frc_cnt(g_mct, 0);

	s5j_mct_global_start_frc(g_mct);

	(void)irq_attach(IRQ_MCT_G0, (xcpt_t)up_timerisr, NULL);

	/* Enable SysTick interrupts */
	s5j_mct_global_enable_int(g_mct, MCT_G0);


	/* And enable the timer interrupt */
	up_enable_irq(IRQ_MCT_G0);
}

int up_timer_gettime(FAR struct timespec *ts)
{
	u64 cnt_val;
	irqstate_t flags;

	DEBUGASSERT(g_mct && ts != NULL);

	/* Read the 64-bit frc */

	flags = irqsave();
	cnt_val = s5j_mct_global_get_frc_cnt(g_mct);
	irqrestore(flags);

	s5j_convert_systimr(cnt_val, ts);

	return OK;
}

int up_alarm_cancel(FAR struct timespec *ts)
{
	u64 cnt_val;
	irqstate_t flags;

	DEBUGASSERT(g_mct && ts != NULL);

	flags = irqsave();
	up_disable_irq(IRQ_MCT_G0);

	s5j_mct_global_disable_comp(g_mct, S5J_MCT_GLOBAL, MCT_G0);

	cnt_val = s5j_mct_global_get_frc_cnt(g_mct);

	up_enable_irq(IRQ_MCT_G0);
	irqrestore(flags);

	s5j_convert_systimr(cnt_val, ts);

	return OK;
}

int up_alarm_start(FAR const struct timespec *ts)
{
	u64 cnt_val;
	irqstate_t flags;

	DEBUGASSERT(g_mct && ts != NULL);

	cnt_val = (u64)ts->tv_sec * USEC_PER_SEC;
	cnt_val += (u64)ts->tv_nsec / NSEC_PER_USEC;

	flags = irqsave();
	up_disable_irq(IRQ_MCT_G0);

	s5j_mct_global_set_comp_cnt(g_mct, S5J_MCT_GLOBAL, MCT_G0, cnt_val);

	s5j_mct_global_enable_comp(g_mct, S5J_MCT_GLOBAL, MCT_G0);

	up_enable_irq(IRQ_MCT_G0);
	irqrestore(flags);

	return OK;
}
