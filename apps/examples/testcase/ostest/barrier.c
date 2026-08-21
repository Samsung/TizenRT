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
 * apps/examples/testcase/ostest/barrier.c
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
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "ostest.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define HALF_SECOND 500000L

/****************************************************************************
 * Private Data
 ****************************************************************************/

static pthread_barrier_t barrier;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: barrier_func
 ****************************************************************************/

static void *barrier_func(void *parameter)
{
  int id  = (int)((intptr_t)parameter);
  int status;

  printf("barrier_func: Thread %d started\n",  id);
  usleep(HALF_SECOND);

  /* Wait at the barrier until all threads are synchronized. */

  printf("barrier_func: Thread %d calling pthread_barrier_wait()\n",
         id);
  FFLUSH();
  status = pthread_barrier_wait(&barrier);
  if (status == 0)
    {
      printf("barrier_func: Thread %d, back with "
             "status=0 (I am not special)\n",
             id);
    }
  else if (status == PTHREAD_BARRIER_SERIAL_THREAD)
    {
      printf("barrier_func: Thread %d, back with "
             "status=PTHREAD_BARRIER_SERIAL_THREAD (I AM SPECIAL)\n",
             id);
    }
  else
    {
      printf("barrier_func: ERROR thread %d could not get semaphore value\n",
             id);
      ASSERT(false);
    }

  FFLUSH();

  usleep(HALF_SECOND);
  printf("barrier_func: Thread %d done\n",  id);
  FFLUSH();
  return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: barrier_test
 ****************************************************************************/

void barrier_test(void)
{
  pthread_t barrier_thread[CONFIG_TESTING_OSTEST_NBARRIER_THREADS];
  pthread_addr_t result;
  pthread_attr_t attr;
  pthread_barrierattr_t barrierattr;
  int status;
  int i;

  printf("barrier_test: Initializing barrier\n");

  /* Create the barrier */

  status = pthread_barrierattr_init(&barrierattr);
  if (status != OK)
    {
      printf("barrier_test: pthread_barrierattr_init failed, status=%d\n",
             status);
    }

  status = pthread_barrier_init(&barrier, &barrierattr,
                                CONFIG_TESTING_OSTEST_NBARRIER_THREADS);
  if (status != OK)
    {
      printf("barrier_test: pthread_barrier_init failed, status=%d\n",
             status);
    }

  /* Start CONFIG_TESTING_OSTEST_NBARRIER_THREADS thread instances */

  status = pthread_attr_init(&attr);
  if (status != OK)
    {
      printf("barrier_test: pthread_attr_init failed, status=%d\n",
              status);
    }

  for (i = 0; i < CONFIG_TESTING_OSTEST_NBARRIER_THREADS; i++)
    {
      status = pthread_create(&barrier_thread[i], &attr, barrier_func,
                              (pthread_addr_t)((uintptr_t)i));
      if (status != 0)
        {
          printf("barrier_test: ERROR thread %d create, status=%d\n",
                 i, status);
          printf("barrier_test: Test aborted with waiting threads\n");
          ASSERT(false);
          goto abort_test;
        }
      else
        {
          printf("barrier_test: Thread %d created\n", i);
        }
    }

  FFLUSH();

  /* Wait for all thread instances to complete */

  for (i = 0; i < CONFIG_TESTING_OSTEST_NBARRIER_THREADS; i++)
    {
      status = pthread_join(barrier_thread[i], &result);
      if (status != 0)
        {
          printf("barrier_test: ERROR thread %d join, status=%d\n",
                 i, status);
          ASSERT(false);
        }
      else
        {
          printf("barrier_test: Thread %d completed with result=%p\n",
                 i, result);
        }
    }

  /* Destroy the barrier */

abort_test:
  status = pthread_barrier_destroy(&barrier);
  if (status != OK)
    {
      printf("barrier_test: pthread_barrier_destroy failed, status=%d\n",
             status);
    }

  status = pthread_barrierattr_destroy(&barrierattr);
  if (status != OK)
    {
      printf("barrier_test: pthread_barrierattr_destroy failed, status=%d\n",
             status);
    }

  FFLUSH();
}
