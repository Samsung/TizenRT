/*****************************************************************************
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
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "tinyara_wrapper.h"
#include "utils_scsc.h"
#include "utils_misc.h"

/**
 * wait_for_completion_timeout: - waits for completion of a task (w/timeout)
 * @x:  holds the state of this particular completion
 * @timeout:  timeout value in mili seconds
 *
 * This waits for either a completion of a specific task to be signaled or for a
 * specified timeout to expire. The timeout is in mili second. It is not
 * interruptible.
 *
 * Return: 0 if timed out, and positive if completed.
 */
unsigned long wait_for_completion_timeout(struct completion *x, unsigned long timeout_ms)
{
	struct timespec timeout;
	struct timespec current;
	int ret = 0;

	if (timeout_ms) {
		clock_gettime(CLOCK_REALTIME, &current);
		timeout.tv_sec = timeout_ms / 1000;
		timeout.tv_nsec = (timeout_ms % 1000) * 1000 * 1000;

		timeout.tv_sec = timeout.tv_sec + current.tv_sec;
		timeout.tv_nsec = timeout.tv_nsec + current.tv_nsec;
		if (timeout.tv_nsec > NSEC_PER_SEC) {
			timeout.tv_sec++;
			timeout.tv_nsec -= NSEC_PER_SEC;
		}
		while (sem_timedwait(&x->sem, &timeout) != 0) {
			ret = get_errno();
			if (ret == ETIMEDOUT) {
				ret = 1;
				break;
			}
		}
	} else {
		while (sem_wait(&x->sem) != 0) {
			sem_post(&x->sem);
		}
	}

	if (ret) {
		return 0;
	}

	return 1;
}

void wait_for_completion(struct completion *x)
{
	wait_for_completion_timeout(x, 0);
}

/**
 * wait_for_completion_interruptible: - waits for completion of a task (w/intr)
 * @x:  holds the state of this particular completion
 *
 * This waits for completion of a specific task to be signaled.
 *
 * Return: 0 if completed.
 */
int wait_for_completion_interruptible(struct completion *x)
{
	wait_for_completion_timeout(x, 0);
	return 0;
}

void complete(struct completion *x)
{
	sem_post(&x->sem);

}

void complete_all(struct completion *x)
{
	complete(x);
}

void init_completion(struct completion *x)
{
	sem_init(&(x->sem), 0, 0);
}

void reinit_completion(struct completion *x)
{
	init_completion(x);
}

void *copy_to_user(void *dest, const void *src, size_t n)
{
	memcpy(dest, src, n);
	return 0;
}

void *copy_from_user(void *dest, const void *src, size_t n)
{
	memcpy(dest, src, n);
	return 0;
}
