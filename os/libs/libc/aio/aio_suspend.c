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
 * libs/libc/aio/aio_suspend.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sched.h>
#include <signal.h>
#include <time.h>
#include <aio.h>
#include <assert.h>
#include <errno.h>

#ifdef CONFIG_FS_AIO

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: aio_suspend
 *
 * Description:
 *   The aio_suspend() function suspends the calling thread until at least
 *   one of the asynchronous I/O operations referenced by the 'list' argument
 *   has completed, until a signal interrupts the function, or, if 'timeout'
 *   is not NULL, until the time interval specified by 'timeout' has passed.
 *   If any of the aiocb structures in the list correspond to completed
 *   asynchronous I/O operations (that is, the error status for the
 *   operation is not equal to EINPROGRESS) at the time of the call, the
 *   function returns without suspending the calling thread.
 *
 *   Each aiocb structure pointed to must have been used in initiating an
 *   asynchronous I/O request via aio_read(), aio_write(), or lio_listio().
 *   This array may contain NULL pointers, which are ignored. If this
 *   array contains pointers that refer to aiocb structures that have not
 *   been used in submitting asynchronous I/O, the effect is undefined.
 *
 * Input Parameters:
 *   list    - An array of pointers to asynchronous I/O control blocks.
 *   nent    - The number of elements in the array.
 *   aiocbp  - A pointer to an array
 *   timeout - If not NULL, this parameter is pointer to a timespec
 *             structure that determines a timeout on the operation.  If
 *             the time referred to timeout passes before any of the I/O
 *             operations referenced by list are completed, then
 *             aio_suspend() returns with an error.
 *
 * Returned Value:
 *   If the aio_suspend() function returns after one or more asynchronous
 *   I/O operations have completed, the function returns zero. Otherwise,
 *   the function returns a value of -1 and sets errno to indicate the
 *   error.  The application may determine which asynchronous I/O completed
 *   by scanning the associated error and return status using aio_error()
 *   and aio_return(), respectively.
 *
 *   The aio_suspend() function will fail if:
 *
 *     EAGAIN - No asynchronous I/O indicated in the list referenced by
 *              list completed in the time interval indicated by timeout.
 *     EINTR  - A signal interrupted the aio_suspend() function.
 *
 ****************************************************************************/

int aio_suspend(FAR const struct aiocb *const list[], int nent,
                FAR const struct timespec *timeout)
{
	sigset_t set;
	FAR const struct aiocb *const *entry;
	struct timespec elapsed;
	struct timespec mark;
	struct timespec remaining;
	FAR struct timespec *rel;
	int ret;

	DEBUGASSERT(list);

	sigemptyset(&set);
	sigaddset(&set, SIGPOLL);

	/* Work from a private copy of the (relative) timeout so the
	 * remaining interval can be recomputed after each wake-up.
	 */

	if (timeout != NULL) {
		remaining = *timeout;
		rel = &remaining;
	} else {
		rel = NULL;
	}

	for (;;) {
		/* Check each entry in the list.  Break out of the loop if any
		 * entry has completed.
		 */

		for (entry = list; entry < list + nent; entry++) {
			/* Check if the I/O has completed */

			if (*entry != NULL &&
			    (*entry)->aio_result != -EINPROGRESS) {
				/* Yes, return success */

				return OK;
			}
		}

		/* No listed request has completed yet: wait for the next
		 * completion notification.  The AIO worker queues one
		 * SIGPOLL to this process for every completion it posts
		 * (os/fs/aio/aio_signal.c), independent of the per-aiocb
		 * sigev_notify selection.  A SIGPOLL that was already
		 * pending -- queued by an earlier completion of this or a
		 * previous list, and unconsumed because the process keeps
		 * SIGPOLL blocked -- satisfies sigtimedwait() immediately
		 * but says nothing about the requests in this list.  Such
		 * a spurious wake-up must not be returned as success:
		 * re-scan and wait again instead (TD-KERNEL-02).  Waiting
		 * in a loop until a listed entry is actually complete is
		 * also what the documented behavior of this function
		 * promises.
		 */

		if (rel != NULL) {
			clock_gettime(CLOCK_REALTIME, &mark);
		}

		ret = sigtimedwait(&set, NULL, rel);
		if (ret < 0) {
			/* Timed out (EAGAIN) or interrupted by a delivered
			 * signal (EINTR): sigtimedwait() has set errno.
			 */

			return ERROR;
		}

		if (rel != NULL) {
			clock_gettime(CLOCK_REALTIME, &elapsed);

			elapsed.tv_sec  -= mark.tv_sec;
			elapsed.tv_nsec -= mark.tv_nsec;
			if (elapsed.tv_nsec < 0) {
				elapsed.tv_nsec += 1000000000L;
				elapsed.tv_sec  -= 1;
			}

			remaining.tv_sec  -= elapsed.tv_sec;
			remaining.tv_nsec -= elapsed.tv_nsec;
			if (remaining.tv_nsec < 0) {
				remaining.tv_nsec += 1000000000L;
				remaining.tv_sec  -= 1;
			}

			if (remaining.tv_sec < 0 ||
			    (remaining.tv_sec == 0 &&
			     remaining.tv_nsec <= 0)) {
				/* Time expired without seeing a completed
				 * entry in the scan above.
				 */

				set_errno(EAGAIN);
				return ERROR;
			}
		}
	}

	return ERROR; /* NOTREACHED */
}

#endif /* CONFIG_FS_AIO */
