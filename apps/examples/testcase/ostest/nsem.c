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
 * apps/examples/testcase/ostest/nsem.c
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
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>

#include "ostest.h"

#ifdef CONFIG_FS_NAMED_SEMAPHORES

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define SEM1_NAME "foo"
#define SEM2_NAME "bar"

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static FAR void *nsem_peer(void *parameter)
{
  FAR sem_t *sem1;
  FAR sem_t *sem2;

  /* Open semaphore 1.  This should have already been created by
   * nsem_test().
   */

  printf("nsem_peer: Open semaphore 1\n");
  sem1 = sem_open(SEM1_NAME, 0);
  if (sem1 == SEM_FAILED)
    {
      int errcode = errno;
      printf("nsem_peer: ERROR: sem_open(1) failed: %d\n", errcode);
      ASSERT(false);
      return NULL;
    }

  /* Open semaphore 2.  We will create that one */

  printf("nsem_peer: Create semaphore 2 with value == 0\n");
  sem2 = sem_open(SEM2_NAME, O_CREAT | O_EXCL, 0644, 0);
  if (sem1 == SEM_FAILED)
    {
      int errcode = errno;
      printf("nsem_peer: ERROR: sem_open(2) failed: %d\n", errcode);
      ASSERT(false);
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

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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
  if (sem1 == SEM_FAILED)
    {
      int errcode = errno;
      printf("nsem_peer: ERROR: sem_open(1) failed: %d\n", errcode);
      ASSERT(false);
      return;
    }

  /* Start the peer thread */

  printf("nsem_test: Starting peer peer\n");
  status = pthread_attr_init(&attr);
  if (status != OK)
    {
      printf("nsem_test: pthread_attr_init failed, status=%d\n",  status);
    }

  prio_min = sched_get_priority_min(SCHED_FIFO);
  prio_max = sched_get_priority_max(SCHED_FIFO);
  prio_mid = (prio_min + prio_max) / 2;

  sparam.sched_priority = (prio_mid + prio_max) / 2;
  status = pthread_attr_setschedparam(&attr, &sparam);
  if (status != OK)
    {
      printf("nsem_test: "
             "ERROR: pthread_attr_setschedparam failed, status=%d\n",
              status);
      ASSERT(false);
    }
  else
    {
      printf("nsem_test: Set peer priority to %d\n",  sparam.sched_priority);
    }

  status = pthread_create(&peer, &attr, nsem_peer, NULL);
  if (status != 0)
    {
      printf("nsem_test: ERROR: Peer thread creation failed: %d\n",  status);
      ASSERT(false);
      return;
    }

  /* Wait for the peer to post semaphore 1 */

  printf("nsem_test: Wait on semaphore 1\n");
  status = sem_wait(sem1);
  if (status < 0)
    {
      int errcode = errno;
      printf("nsem_test: ERROR: sem_wait(1) failed: %d\n",  errcode);
      ASSERT(false);
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
  if (sem2 == SEM_FAILED)
    {
      int errcode = errno;
      printf("nsem_test: ERROR: sem_open(2) failed: %d\n", errcode);
      ASSERT(false);
      pthread_cancel(peer);
      return;
    }

  /* Wait for the peer to post semaphore 2 */

  printf("nsem_test: Wait on semaphore 2\n");
  status = sem_wait(sem2);
  if (status < 0)
    {
      int errcode = errno;
      printf("nsem_test: ERROR: sem_wait(1) failed: %d\n",  errcode);
      ASSERT(false);
      pthread_cancel(peer);
      return;
    }

  /* Close and unlink semaphore 2 */

  printf("nsem_test: Close and unlink semaphore 2\n");
  sem_close(sem2);
  sem_unlink(SEM2_NAME);

#ifdef SDCC
  if (peer != (pthread_t)0)
    {
      pthread_join(peer, &result);
    }
#else
  if (peer != (pthread_t)0)
    {
      pthread_join(peer, NULL);
    }
#endif
}

#endif /* CONFIG_FS_NAMED_SEMAPHORES */
