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
 * kernel/semaphore/sem_waitirq.c
 *
 *   Copyright (C) 2007-2010, 2014 Gregory Nutt. All rights reserved.
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

#include <sched.h>
#include <errno.h>
#include <tinyara/arch.h>

#include "semaphore/semaphore.h"

#ifdef CONFIG_SEMAPHORE_HISTORY
#include <tinyara/debug/sysdbg.h>
#endif

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
 * Name: sem_waitirq
 *
 * Description:
 *   This function is called when either:
 *
 *   1. A signal is received by a task that is waiting on a semaphore.
 *      According to the POSIX spec, "...the calling thread shall not return
 *      from the call to [sem_wait] until it either locks the semaphore or
 *      the call is interrupted by a signal."
 *   2. From logic associated with sem_timedwait().  This function is called
 *      when the timeout elapses without receiving the semaphore.
 *
 * Parameters:
 *   wtcb    - A pointer to the TCB of the task that is waiting on a
 *             semphaphore, but has received a signal or timeout instead.
 *   errcode - EINTR if the semaphore wait was awakened by a signal;
 *             ETIMEDOUT if awakened by a timeout
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

void sem_waitirq(FAR struct tcb_s *wtcb, int errcode)
{
	irqstate_t saved_state;

	/* Disable interrupts.  This is necessary (unfortunately) because an
	 * interrupt handler may attempt to post the semaphore while we are
	 * doing this.
	 */

	saved_state = enter_critical_section();

	/* It is possible that an interrupt/context switch beat us to the punch
	 * and already changed the task's state.
	 */

	if (wtcb->task_state == TSTATE_WAIT_SEM) {
		sem_t *sem = wtcb->waitsem;
		DEBUGASSERT(sem != NULL && sem->semcount < 0);

		/* Restore the correct priority of all threads that hold references
		 * to this semaphore.
		 */

		sem_canceled(wtcb, sem);

		/* And increment the count on the semaphore.  This releases the count
		 * that was taken by sem_post().  This count decremented the semaphore
		 * count to negative and caused the thread to be blocked in the first
		 * place.
		 */

		sem->semcount++;

		/* Indicate that the semaphore wait is over. */

		wtcb->waitsem = NULL;

#ifdef CONFIG_SEMAPHORE_HISTORY
		save_semaphore_history(sem, (void *)wtcb, SEM_ACQUIRE);
#endif

		/* Mark the errno value for the thread. */

		wtcb->pterrno = errcode;

		/* Restart the task. */

		up_unblock_task(wtcb);
	}

	/* Interrupts may now be enabled. */

	leave_critical_section(saved_state);
}
