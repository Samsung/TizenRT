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
 * apps/examples/testcase/ostest/rmutex.c
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

#include "ostest.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define NTHREADS    3
#define NLOOPS      3
#define NRECURSIONS 3

static pthread_mutex_t mut;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

static void thread_inner(int id, int level)
{
  int status;
  if (level < NRECURSIONS)
    {
      /* Take the mutex */

      printf("thread_inner[%d, %d]: Locking\n", id, level);
      status = pthread_mutex_lock(&mut);
      if (status != 0)
        {
          printf("thread_inner[%d, %d]: "
                 "ERROR pthread_mutex_lock failed: %d\n",
                  id, level, status);
          ASSERT(false);
        }

      printf("thread_inner[%d, %d]: Locked\n", id, level);

      /* Try-lock already locked recursive mutex. */

      status = pthread_mutex_trylock(&mut);
      if (status != 0)
        {
          printf("thread_inner[%d, %d]: "
                 "ERROR pthread_mutex_trylock failed: %d\n",
                  id, level, status);
          ASSERT(false);
        }
      else
        {
          /* Unlock the try-lock. */

          status = pthread_mutex_unlock(&mut);
          if (status != 0)
            {
              printf("thread_inner[%d, %d]: ERROR "
                     "pthread_mutex_unlock after try-lock failed: %d\n",
                      id, level, status);
              ASSERT(false);
            }
        }

      /* Give the other threads a chance */

      pthread_yield();
      thread_inner(id, level + 1);
      pthread_yield();

      /* Unlock the mutex */

      printf("thread_inner[%d, %d]: Unlocking\n", id, level);
      status = pthread_mutex_unlock(&mut);
      if (status != 0)
        {
          printf("thread_inner[%d, %d]: "
                 "ERROR pthread_mutex_unlock failed: %d\n",
                 id, level, status);
          ASSERT(false);
        }

      printf("thread_inner[%d, %d]: Unlocked\n", id, level);
      pthread_yield();
    }
}

static FAR void *thread_outer(FAR void *parameter)
{
  int i;

  printf("thread_outer[%d]: Started\n", (int)((intptr_t)parameter));
  for (i = 0; i < NLOOPS; i++)
    {
      printf("thread_outer[%d]: Loop %d\n", (int)((intptr_t)parameter), i);
      thread_inner((int)((intptr_t)parameter), 0);
    }

  printf("thread_outer[%d]: Exiting\n", (int)((intptr_t)parameter));
  pthread_exit(NULL);
  return NULL; /* Non-reachable -- needed for some compilers */
}

void recursive_mutex_test(void)
{
  pthread_t thread[NTHREADS];
#ifdef SDCC
  pthread_addr_t result[NTHREADS];
  pthread_attr_t attr;
#endif
  pthread_mutexattr_t mattr;
  int type;
  int status;
  int i;

  /* Initialize the mutex attributes */

  pthread_mutexattr_init(&mattr);
  status = pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
  if (status != 0)
    {
      printf("recursive_mutex_test: "
             "ERROR pthread_mutexattr_settype failed, status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_mutexattr_gettype(&mattr, &type);
  if (status != 0)
    {
      printf("recursive_mutex_test: "
             "ERROR pthread_mutexattr_gettype failed, status=%d\n", status);
      ASSERT(false);
    }

  if (type != PTHREAD_MUTEX_RECURSIVE)
    {
      printf("recursive_mutex_test: "
             "ERROR pthread_mutexattr_gettype return type=%d\n", type);
      ASSERT(false);
    }

  /* Initialize the mutex */

  printf("recursive_mutex_test: Initializing mutex\n");
  status = pthread_mutex_init(&mut, &mattr);
  if (status != 0)
    {
      printf("recursive_mutex_test: "
             "ERROR pthread_mutex_init failed, status=%d\n", status);
      ASSERT(false);
    }

  /* Start the threads -- all at the same, default priority */

  for (i = 0; i < NTHREADS; i++)
    {
      printf("recursive_mutex_test: Starting thread %d\n", i + 1);
#ifdef SDCC
      pthread_attr_init(&attr);
      status = pthread_create(&thread[i], &attr, thread_outer,
                             (pthread_addr_t)((uintptr_t)i + 1));
#else
      status = pthread_create(&thread[i], NULL, thread_outer,
                             (pthread_addr_t)((uintptr_t)i + 1));
#endif
      if (status != 0)
        {
          printf("recursive_mutex_test: ERROR thread#%d creation: %d\n",
                 i + 1, status);
          ASSERT(false);
        }
    }

  /* Wait for all; of the threads to complete */

  for (i = 0; i < NTHREADS; i++)
    {
      printf("recursive_mutex_test: Waiting for thread %d\n", i + 1);
#ifdef SDCC
      pthread_join(thread[i], &result[i]);
#else
      pthread_join(thread[i], NULL);
#endif
    }

  printf("recursive_mutex_test: Complete\n");
  pthread_mutex_destroy(&mut);
}
