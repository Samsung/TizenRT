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
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * apps/testing/ostest/timedmutex.c
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

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "ostest.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

static pthread_mutex_t g_mutex;
static bool g_running;
static int g_result;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void *thread_func(FAR void *parameter)
{
	struct timespec ts;
	int status;

	printf("pthread:  Started\n");
	g_running = true;

	for (;;) {
		printf("pthread:  Waiting for lock or timeout\n");

		/* Get the current time */

		status = clock_gettime(CLOCK_REALTIME, &ts);
		if (status < 0) {
			int errcode = errno;
			fprintf(stderr, "pthread: "
			        "ERROR clock_gettime() failed: %d\n", errcode);
			ASSERT(false);
			g_result = errcode;
			break;
		}

		/* Get a time two seconds in the future (we presume that we cannot be
		 * blocked for two seconds here!)
		 */

		ts.tv_sec += 2;

		/* Now wait until either we get the lock or until the timeout occurs */

		status = pthread_mutex_timedlock(&g_mutex, &ts);
		if (status != 0) {
			if (status == ETIMEDOUT) {
				printf("pthread:  Got the timeout.  Terminating\n");
			} else {
				fprintf(stderr, "pthread: "
				        "ERROR pthread_mutex_timedlock() failed: %d\n",
				        status);
				ASSERT(false);
			}

			g_result = status;
			break;
		}

		printf("pthread:  Got the lock\n");

		/* Release the lock and wait a bit in case the main thread wants it. */

		pthread_mutex_unlock(&g_mutex);
		usleep(500 * 1000);
	}

	g_running = false;
	return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void timedmutex_test(void)
{
	pthread_t thread;
#ifdef SDCC
	pthread_addr_t result;
#endif
	int status;

	/* Initialize the mutex */

	printf("mutex_test: Initializing mutex\n");
	pthread_mutex_init(&g_mutex, NULL);

	/* Lock the mutex */

	status = pthread_mutex_lock(&g_mutex);
	if (status != OK) {
		fprintf(stderr, "mutex_test: ERROR Failed to get mutex: %d\n", status);
		ASSERT(false);
		goto errout_with_mutex;
	}

	/* Start a thread */

	printf("mutex_test: Starting thread\n");
#ifdef SDCC
	pthread_attr_init(&attr);
	status = pthread_create(&thread, &attr, thread_func, (pthread_addr_t)0);
#else
	status = pthread_create(&thread, NULL, thread_func, (pthread_addr_t)0);
#endif
	if (status != 0) {
		fprintf(stderr, "mutex_test: ERROR in thread creation: %d\n", status);
		ASSERT(false);
		goto errout_with_lock;
	}

	/* Wait a bit to assure that the thread gets a chance to start */

	usleep(500 * 1000);

	/* Then unlock the mutex.  This should wake up the pthread. */

	printf("mutex_test: Unlocking\n");
	pthread_mutex_unlock(&g_mutex);

	/* Wait a bit to assure that the thread gets a chance to lock the mutex at
	 * least once (it may probably loop and retake the mutex several times)
	 */

	usleep(500 * 1000);

	/* The re-lock the mutex.  The pthread should now be waiting for the lock
	 * or a timeout.
	 */

	status = pthread_mutex_lock(&g_mutex);
	if (status != OK) {
		fprintf(stderr, "mutex_test: ERROR Failed to get mutex: %d\n", status);
		ASSERT(false);
		goto errout_with_mutex;
	}

	/* The pthread should timeout in two seconds.  Let's wait four.  At that
	 * time, the pthread should no longer be running.
	 */

	sleep(4);

	/* The pthread should no longer be running and it should have terminated
	 * because of EAGAIN.
	 */

	if (g_running) {
		fprintf(stderr, "mutex_test: ERROR: The pthread is still running!\n");
		ASSERT(false);
	} else if (g_result != ETIMEDOUT) {
		fprintf(stderr, "mutex_test: ERROR: Result was not ETIMEDOUT: %d\n",
		        g_result);
		ASSERT(false);
	} else {
		printf("mutex_test: PASSED\n");
	}

	/* Let's reap any join droppings left from the pthread */

#ifdef SDCC
	pthread_join(thread, &result);
#else
	pthread_join(thread, NULL);
#endif

errout_with_lock:
	pthread_mutex_unlock(&g_mutex);
errout_with_mutex:
	pthread_mutex_destroy(&g_mutex);
}
