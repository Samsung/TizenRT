/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/armv8-m/up_sigdeliver.c
 *
 *   Copyright (C) 2009-2010, 2013-2014 Gregory Nutt. All rights reserved.
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
#include <sched.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <arch/board/board.h>

#include "sched/sched.h"
#include "up_internal.h"
#include "up_arch.h"

#ifdef CONFIG_TASK_SCHED_HISTORY
#include <tinyara/debug/sysdbg.h>
#endif

#ifndef CONFIG_DISABLE_SIGNALS

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_sigdeliver
 *
 * Description:
 *   This is the a signal handling trampoline.  When a signal action was
 *   posted.  The task context was mucked with and forced to branch to this
 *   location with interrupts disabled.
 *
 ****************************************************************************/

void up_sigdeliver(void)
{
	struct tcb_s *rtcb = this_task();
	uint32_t regs[XCPTCONTEXT_REGS];
	sig_deliver_t sigdeliver;

	/* Save the errno.  This must be preserved throughout the signal handling
	 * so that the user code final gets the correct errno value (probably
	 * EINTR).
	 */

	int saved_errno = rtcb->pterrno;

	board_led_on(LED_SIGNAL);

	svdbg("rtcb=%p sigdeliver=%p sigpendactionq.head=%p\n", rtcb, rtcb->xcp.sigdeliver, rtcb->sigpendactionq.head);
	ASSERT(rtcb->xcp.sigdeliver != NULL);

	/* Save the real return state on the stack. */

	up_copyfullstate(regs, rtcb->xcp.regs);
	regs[REG_PC] = rtcb->xcp.saved_pc;
#ifdef CONFIG_ARMV8M_USEBASEPRI
	regs[REG_BASEPRI] = rtcb->xcp.saved_basepri;
#else
	regs[REG_PRIMASK] = rtcb->xcp.saved_primask;
#endif
	regs[REG_XPSR] = rtcb->xcp.saved_xpsr;
#ifdef CONFIG_BUILD_PROTECTED
	regs[REG_LR] = rtcb->xcp.saved_lr;
#endif
	regs[REG_EXC_RETURN] = rtcb->xcp.saved_exec_ret;

	/* Get a local copy of the sigdeliver function pointer. We do this so that
	 * we can nullify the sigdeliver function pointer in the TCB and accept
	 * more signal deliveries while processing the current pending signals.
	 */

	sigdeliver = (sig_deliver_t)rtcb->xcp.sigdeliver;

	/* Then restore the task interrupt state */

#ifdef CONFIG_ARMV8M_USEBASEPRI
	irqrestore((uint8_t)regs[REG_BASEPRI]);
#else
	irqrestore((uint16_t)regs[REG_PRIMASK]);
#endif

	/* Deliver the signal */

	sigdeliver(rtcb);

	/* Output any debug messages BEFORE restoring errno (because they may
	 * alter errno), then disable interrupts again and restore the original
	 * errno that is needed by the user logic (it is probably EINTR).
	 */

	svdbg("Resuming\n");
	(void)irqsave();
	rtcb->pterrno = saved_errno;

	/* Then restore the correct state for this thread of
	 * execution.
	 */

	/* handle new signal only after finishing current signal processing */
	rtcb->xcp.sigdeliver = NULL;

	board_led_off(LED_SIGNAL);

#ifdef CONFIG_TASK_SCHED_HISTORY
	/* Save the task name which will be scheduled */
	save_task_scheduling_status(rtcb);
#endif
	up_fullcontextrestore(regs);
}

#endif							/* !CONFIG_DISABLE_SIGNALS */
