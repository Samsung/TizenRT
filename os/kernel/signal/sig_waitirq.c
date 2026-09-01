/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * kernel/signal/sig_waitirq.c
 *
 *   Copyright (C) 2016 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sched.h>
#include <errno.h>
#include <tinyara/arch.h>
#include <tinyara/wdog.h>

#include "signal/signal.h"

/****************************************************************************
 * Pre-processor Definitions
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
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sig_waitirq
 *
 * Description:
 *   This function is called when an error event has occurred and a signal
 *   wait (in sigwaitinfo/sigtimedwait) must be terminated with an error.
 *   It unblocks the task so that it can handle the error condition.
 *
 *   This is analogous to sem_waitirq() for semaphore waits and
 *   mq_waitirq() for message queue waits.
 *
 * Parameters:
 *   wtcb    - A pointer to the TCB of the task that is waiting for a
 *             signal, but has been interrupted instead.
 *   signo   - The signal number to store in the unblock info.  This is
 *             typically a sentinel value like SIG_WAIT_CANCELED or
 *             SIG_WAIT_TIMEOUT.
 *   code    - The si_code to store in the unblock info (e.g. SI_USER).
 *   errcode - The error code to set for the thread (e.g. ECANCELED).
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   Called from within a critical section (interrupts disabled).
 *
 ****************************************************************************/

void sig_waitirq(FAR struct tcb_s *wtcb, uint8_t signo,
                 uint8_t code, int errcode)
{
	/* It is possible that an interrupt/context switch beat us to the punch
	 * and already changed the task's state.
	 */

	if (wtcb->task_state == TSTATE_WAIT_SIG) {
		/* If a watchdog timer was started by sigtimedwait(), cancel it so
		 * that it does not fire and overwrite the sigunbinfo we set below.
		 * We do NOT delete it here — sigtimedwait() will handle the deletion
		 * when the task wakes up and resumes execution.
		 */

		if (wtcb->waitdog != NULL) {
			wd_cancel(wtcb->waitdog);
		}

		/* Clear the sigwaitmask so that the task does not think it is
		 * still waiting for a signal.
		 */

		wtcb->sigwaitmask = NULL_SIGNAL_SET;

		/* Set the unblock info from the caller-provided parameters.
		 * This allows the caller to specify the reason for the unblock
		 * (cancellation, timeout, etc.).
		 */

		wtcb->sigunbinfo.si_signo = signo;
		wtcb->sigunbinfo.si_code = code;
		wtcb->sigunbinfo.si_value.sival_int = 0;
#ifdef CONFIG_SCHED_HAVE_PARENT
		wtcb->sigunbinfo.si_pid = 0;
		wtcb->sigunbinfo.si_status = OK;
#endif

		/* Mark the errno value for the thread. */

		wtcb->pterrno = errcode;

		/* Restart the task. */

		up_unblock_task(wtcb);
	}
}
