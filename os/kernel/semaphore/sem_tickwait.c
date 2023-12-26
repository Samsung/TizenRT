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
 * kernel/semaphore/sem_tickwait.c
 *
 *   Copyright (C) 2011, 2013-2014 Gregory Nutt. All rights reserved.
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
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/wdog.h>

#include "sched/sched.h"
#include "clock/clock.h"
#include "semaphore/semaphore.h"

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
 * Name: sem_tickwait
 *
 * Description:
 *   This function is a lighter weight version of sem_timedwait().  It is
 *   non-standard and intended only for use within the RTOS.
 *
 * Parameters:
 *   sem     - Semaphore object
 *   start   - The system time that the delay is relative to.  If the
 *             current time is not the same as the start time, then the
 *             delay will be adjust so that the end time will be the same
 *             in any event.
 *   delay   - Ticks to wait from the start time until the semaphore is
 *             posted.  If ticks is zero, then this function is equivalent
 *             to sem_trywait().
 *
 * Return Value:
 *   Zero (OK) is returned on success.
 *   On failure, -1 (ERROR) is returned and the errno
 *   is set appropriately:
 *
 *   ETIMEDOUT The semaphore could not be locked before the specified timeout
 *             (delay) expired.
 *   ENOMEM    Out of memory
 *
 ****************************************************************************/


int sem_tickwait(FAR sem_t *sem, clock_t start, uint32_t delay)
{
	FAR struct tcb_s *rtcb = this_task();
	irqstate_t flags;
	clock_t elapsed;
	int errcode = OK;
	int ret = ERROR;

	DEBUGASSERT(sem != NULL && up_interrupt_context() == false && rtcb->waitdog == NULL);

	/* Create a watchdog.  We will not actually need this watchdog
	 * unless the semaphore is unavailable, but we will reserve it up
	 * front before we enter the following critical section.
	 */

	rtcb->waitdog = wd_create();
	if (!rtcb->waitdog) {
		set_errno(ENOMEM);
		return ret;
	}

	/* We will disable interrupts until we have completed the semaphore
	 * wait.  We need to do this (as opposed to just disabling pre-emption)
	 * because there could be interrupt handlers that are asynchronously
	 * posting semaphores and to prevent race conditions with watchdog
	 * timeout.  This is not too bad because interrupts will be re-
	 * enabled while we are blocked waiting for the semaphore.
	 */

	flags = enter_critical_section();
	/* Try to take the semaphore without waiting. */

	ret = sem_trywait(sem);
	if (ret == OK) {
		/* We got it! */

		goto success_with_irqdisabled;
	}

	/* We will have to wait for the semaphore.  Make sure that we were provided
	 * with a valid timeout.
	 */

	if (delay == 0) {
		/* Return the errno from sem_trywait() */
		errcode = get_errno();
		goto errout_with_irqdisabled;
	}

	/* Adjust the delay for any time since the delay was calculated */

	elapsed = clock_systimer() - start;
	if (/* elapsed >= (UINT32_MAX / 2) || */ elapsed >= delay) {
		/* Already timedout as elapsed time is more than required delay */
		errcode =  ETIMEDOUT;
		goto errout_with_irqdisabled;
	}

	delay -= elapsed;

	/* Start the watchdog with interrupts still disabled */

	(void)wd_start(rtcb->waitdog, delay, (wdentry_t)sem_timeout, 1, getpid());

	/* Now perform the blocking wait */

	ret = sem_wait(sem);
	if (ret != OK) {
		/* Return the errorcode set either by sem_wait() or sem_timeout() */
		errcode = get_errno();
	}

	/* Stop the watchdog timer */

	wd_cancel(rtcb->waitdog);

	/* We can now restore interrupts and delete the watchdog */

	/* Success exits */

success_with_irqdisabled:

	/* Error exits */

errout_with_irqdisabled:
	leave_critical_section(flags);
	wd_delete(rtcb->waitdog);
	rtcb->waitdog = NULL;
	/* some functions (sem_trywait()) inside wd_delete() may fail and set the
	 * errorno. this would override the any errors set above. In recursice
	 * error scenario, catch the first error and set it as errorno.
	 */
	if (errcode != OK) {
		set_errno(errcode);
	}
	return ret;
}
