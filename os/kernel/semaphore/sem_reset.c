/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * os/kernel/semaphore/sem_reset.c
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

#include <semaphore.h>
#include <sched.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/irq.h>

#include "semaphore/semaphore.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sem_reset
 *
 * Description:
 *   Reset a semaphore to a specific value.  This kind of operation is
 *   sometimes required for certain error handling conditions.
 *
 * Parameters:
 *   sem   - Semaphore descriptor to be reset
 *   count - The requested semaphore count
 *
 * Return Value:
 *   0 if successful.  Otherwise, -1 is returned and the errno value is set
 *   appropriately.
 *
 ****************************************************************************/

int sem_reset(FAR sem_t *sem, int16_t count)
{
	irqstate_t flags;

	if ((sem == NULL) || ((sem->flags & FLAGS_INITIALIZED) == 0) || (count < 0)) {
		set_errno(EINVAL);
		return ERROR;
	}

	/*
	 * Don't allow any context switches that may result from the following
	 * sem_post operations.
	 */
	sched_lock();

	/*
	 * Prevent any access to the semaphore by interrupt handlers while we
	 * are performing this operation.
	 */
	flags = enter_critical_section();

	/*
	 * A negative count indicates the negated number of threads that are
	 * waiting to take a count from the semaphore.  Loop here, handing
	 * out counts to any waiting threads.
	 */
	while (sem->semcount < 0 && count > 0) {
		/*
		 * Give out one counting, waking up one of the waiting threads
		 * and, perhaps, kicking off a lot of priority inheritance
		 * logic (REVISIT).
		 */
		DEBUGVERIFY(sem_post(sem));
		count--;
	}

	/*
	 * We exit the above loop with either (1) no threads waiting for the
	 * (i.e., with sem->semcount >= 0).  In this case, 'count' holds the
	 * the new value of the semaphore count.  OR (2) with threads still
	 * waiting but all of the semaphore counts exhausted:  The current
	 * value of sem->semcount is correct.
	 */
	if (sem->semcount >= 0) {
		sem->semcount = count;
	}

	/* Allow any pending context switches to occur now */
	leave_critical_section(flags);
	sched_unlock();
	return OK;
}
