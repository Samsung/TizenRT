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
 * kernel/signal/sig_procmask.c
 *
 *   Copyright (C) 2007-2009, 2014 Gregory Nutt. All rights reserved.
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

#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <debug.h>
#include <sched.h>

#include <tinyara/arch.h>
#include <tinyara/wdog.h>
#include <tinyara/kmalloc.h>

#include "sched/sched.h"
#include "signal/signal.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sigprocmask
 *
 * Description:
 *   This function allows the calling process to examine and/or change its
 *   signal mask.  If the 'set' is not NULL, then it points to a set of
 *   signals to be used to change the currently blocked set.  The value of
 *   'how' indicates the manner in which the set is changed.
 *
 *   If there any pending unblocked signals after the call to sigprocmask(),
 *   those signals will be delivered before sigprocmask() returns.
 *
 *   If sigprocmask() fails, the signal mask of the process is not changed
 *   by this function call.
 *
 * Parameters:
 *   how - How the signal mast will be changed:
 *         SIG_BLOCK   - The resulting set is the union of the current set
 *                       and the signal set pointed to by 'set'.
 *         SIG_UNBLOCK - The resulting set is the intersection of the current
 *                       set and the complement of the signal set pointed to
 *                       by 'set'.
 *         SIG_SETMASK - The resulting set is the signal set pointed to by
 *                       'set'.
 *   set  - Location of the new signal mask
 *   oset - Location to store the old signal mask
 *
 * Return Value:
 *   0 (OK), or -1 (ERROR) if how is invalid.
 *
 * Assumptions:
 *
 ****************************************************************************/

int sigprocmask(int how, FAR const sigset_t *set, FAR sigset_t *oset)
{
	FAR struct tcb_s *rtcb = this_task();
	sigset_t oldsigprocmask;
	irqstate_t saved_state;
	int ret = OK;

	sched_lock();

	/* Return the old signal mask if requested */

	oldsigprocmask = rtcb->sigprocmask;
	if (oset) {
		*oset = oldsigprocmask;
	}

	/* Modify the current signal mask if so requested */

	if (set) {
		/* Some of these operations are non-atomic.  We need to protect
		 * ourselves from attempts to process signals from interrupts
		 */

		saved_state = enter_critical_section();

		/* Okay, determine what we are supposed to do */

		switch (how) {
		/* The resulting set is the union of the current set and the
		 * signal set pointed to by set.
		 */

		case SIG_BLOCK:
			rtcb->sigprocmask |= *set;
			break;

		/* The resulting set is the intersection of the current set and
		 * the complement of the signal set pointed to by _set.
		 */

		case SIG_UNBLOCK:
			rtcb->sigprocmask &= ~(*set);
			break;

		/* The resulting set is the signal set pointed to by set. */

		case SIG_SETMASK:
			rtcb->sigprocmask = *set;
			break;

		default:
			ret = ERROR;
			break;
		}

		leave_critical_section(saved_state);

		/* Now, process any pending signals that were just unmasked */

		sig_unmaskpendingsignal();
	}

	sched_unlock();
	return ret;
}
