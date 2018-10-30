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
/***********************************************************************
 * examples/kernel_sample/nsem.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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
 ***********************************************************************/

/***********************************************************************
 * Included Files
 ***********************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <sched.h>
#include <errno.h>

#include "kernel_sample.h"

#ifdef CONFIG_FS_NAMED_SEMAPHORES

/***********************************************************************
 * Pre-processor Definitions
 ***********************************************************************/

#define SEM1_NAME "foo"
#define SEM2_NAME "bar"

#ifndef NULL
# define NULL (void*)0
#endif

/***********************************************************************
 * Private Data
 ***********************************************************************/

/***********************************************************************
 * Private Functions
 ***********************************************************************/

static FAR void *nsem_peer(void *parameter)
{
	FAR sem_t *sem1;
	FAR sem_t *sem2;

	/* Open semaphore 1.  This should have already been created by
	 * nsem_test().
	 */

	printf("nsem_peer: Open semaphore 1\n");
	sem1 = sem_open(SEM1_NAME, 0);
	if (sem1 == (FAR sem_t *)ERROR) {
		int errcode = errno;
		printf("nsem_peer: ERROR: sem_open(1) failed: %d\n", errcode);
		return NULL;
	}

	/* Open semaphore 2.  We will create that one */

	printf("nsem_peer: Create semaphore 2 with value == 0\n");
	sem2 = sem_open(SEM2_NAME, O_CREAT | O_EXCL, 0644, 0);
	if (sem1 == (FAR sem_t *)ERROR) {
		int errcode = errno;
		printf("nsem_peer: ERROR: sem_open(2) failed: %d\n", errcode);
		return NULL;
	}

	/* Post and discard semaphore 1 */

	printf("nsem_peer: Post, close, and unlink semaphore 1\n");
	sem_post(sem1);
	sem_close(sem1);
	sem_unlink(SEM1_NAME);

	/* Now post and close semaphore 2 */

	printf("nsem_peer: Post and close semaphore 2\n");
	sem_post(sem2);
	sem_close(sem2);
	return NULL;
}

/***********************************************************************
 * Public Functions
 ***********************************************************************/

void nsem_test(void)
{
	pthread_t peer = (pthread_t)0;
#ifdef SDCC
	pthread_addr_t result;
#endif
	FAR sem_t *sem1;
	FAR sem_t *sem2;
	struct sched_param sparam;
	int prio_min;
	int prio_max;
	int prio_mid;
	pthread_attr_t attr;
	int status;

	/* Open semaphore 2.  We will create that one */

	printf("nsem_test: Create semaphore 1 with value == 0\n");
	sem1 = sem_open(SEM1_NAME, O_CREAT | O_EXCL, 0644, 0);
	if (sem1 == (FAR sem_t *)ERROR) {
		int errcode = errno;
		printf("nsem_peer: ERROR: sem_open(1) failed: %d\n", errcode);
		return;
	}

	/* Start the peer thread */

	printf("nsem_test: Starting peer peer\n");
	status = pthread_attr_init(&attr);
	if (status != OK) {
		printf("nsem_test: pthread_attr_init failed, status=%d\n",  status);
	}

	prio_min = sched_get_priority_min(SCHED_FIFO);
	prio_max = sched_get_priority_max(SCHED_FIFO);
	prio_mid = (prio_min + prio_max) / 2;

	sparam.sched_priority = (prio_mid + prio_max) / 2;
	status = pthread_attr_setschedparam(&attr, &sparam);
	if (status != OK) {
		printf("nsem_test: ERROR: pthread_attr_setschedparam failed, status=%d\n",  status);
	} else {
		printf("nsem_test: Set peer priority to %d\n",  sparam.sched_priority);
	}

	status = pthread_create(&peer, &attr, nsem_peer, NULL);
	if (status != 0) {
		printf("nsem_test: ERROR: Peer thread creation failed: %d\n",  status);
		return;
	}

	/* Wait for the peer to post semaphore 1 */

	printf("nsem_test: Wait on semaphore 1\n");
	status = sem_wait(sem1);
	if (status < 0) {
		int errcode = errno;
		printf("nsem_test: ERROR: sem_wait(1) failed: %d\n",  errcode);
		pthread_cancel(peer);
		return;
	}

	/* Close sem1.  It should already have been unlinked by the nsem_peer */

	printf("nsem_test: Close semaphore 1\n");
	sem_close(sem1);

	/* Open semaphore 2.  This should have already been created by
	 * nsem_peer().
	 */

	printf("nsem_test: Open semaphore 2\n");
	sem2 = sem_open(SEM2_NAME, 0);
	if (sem2 == (FAR sem_t *)ERROR) {
		int errcode = errno;
		printf("nsem_test: ERROR: sem_open(2) failed: %d\n", errcode);
		pthread_cancel(peer);
		return;
	}

	/* Wait for the peer to post semaphore 2 */

	printf("nsem_test: Wait on semaphore 2\n");
	status = sem_wait(sem2);
	if (status < 0) {
		int errcode = errno;
		printf("nsem_test: ERROR: sem_wait(1) failed: %d\n",  errcode);
		pthread_cancel(peer);
		return;
	}

	/* Close and unlink semaphore 2 */

	printf("nsem_test: Close and unlink semaphore 2\n");
	sem_close(sem2);
	sem_unlink(SEM2_NAME);

#ifdef SDCC
	if (peer != (pthread_t)0) {
		pthread_join(peer, &result);
	}
#else
	if (peer != (pthread_t)0) {
		pthread_join(peer, NULL);
	}
#endif
}

#endif /* CONFIG_FS_NAMED_SEMAPHORES */
