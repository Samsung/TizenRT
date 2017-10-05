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
 * kernel/task/task_setcanceltype.c
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

#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>

#include "sched/sched.h"
#include "task/task.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: task_setcanceltype
 *
 * Description:
 *   The task_setcanceltype() function atomically both sets the calling
 *   thread's cancelability type to the indicated type and returns the
 *   previous cancelability type at the location referenced by oldtype
 *   Legal values for type are TASK_CANCEL_DEFERRED and
 *   TASK_CANCEL_ASYNCHRONOUS.
 *
 *   The cancelability state and type of any newly created threads,
 *   including the thread in which main() was first invoked, are
 *   TASK_CANCEL_ENABLE and TASK_CANCEL_DEFERRED respectively.
 *
 ****************************************************************************/

int task_setcanceltype(int type, FAR int *oldtype)
{
	FAR struct tcb_s *tcb = this_task();
	int ret = OK;

	/* Suppress context changes for a bit so that the flags are stable. (the
	 * flags should not change in interrupt handling).
	 */

	sched_lock();

	/* Return the current type if so requrested */

	if (oldtype != NULL) {
		if ((tcb->flags & TCB_FLAG_CANCEL_DEFERRED) != 0) {
			*oldtype = TASK_CANCEL_DEFERRED;
		} else {
			*oldtype = TASK_CANCEL_ASYNCHRONOUS;
		}
	}

	/* Set the new cancellation type */

	if (type == TASK_CANCEL_ASYNCHRONOUS) {
		/* Clear the deferred cancellation bit */

		tcb->flags &= ~TCB_FLAG_CANCEL_DEFERRED;

		/* If we just switched from deferred to asynchronous type and if a
		 * cancellation is pending, then exit now.
		 */

		if ((tcb->flags & TCB_FLAG_CANCEL_PENDING) != 0 && (tcb->flags & TCB_FLAG_NONCANCELABLE) == 0) {
			tcb->flags &= ~TCB_FLAG_CANCEL_PENDING;

			/* Exit according to the type of the thread */

#ifndef CONFIG_DISABLE_PTHREAD
			if ((tcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD) {
				pthread_exit(PTHREAD_CANCELED);
			} else
#endif
			{
				exit(EXIT_FAILURE);
			}
		}
	} else if (type == TASK_CANCEL_DEFERRED) {
		/* Set the deferred cancellation type */

		tcb->flags |= TCB_FLAG_CANCEL_DEFERRED;
	} else {
		ret = EINVAL;
	}

	sched_unlock();
	return ret;
}
