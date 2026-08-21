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
 * apps/examples/testcase/ostest/sem.c
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
#include <semaphore.h>
#include <sched.h>
#include <stdio.h>

#include "ostest.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

static sem_t sem;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void *waiter_func(void *parameter)
{
  int id  = (int)((intptr_t)parameter);
  int status;
  int value;

  printf("waiter_func: Thread %d Started\n",  id);

  /* Take the semaphore */

  status = sem_getvalue(&sem, &value);
  if (status < 0)
    {
      printf("waiter_func: "
             "ERROR thread %d could not get semaphore value\n",  id);
      ASSERT(false);
    }
  else
    {
      printf("waiter_func: "
             "Thread %d initial semaphore value = %d\n",  id, value);
    }

  printf("waiter_func: Thread %d waiting on semaphore\n",  id);
  status = sem_wait(&sem);
  if (status != 0)
    {
      printf("waiter_func: ERROR thread %d sem_wait failed\n",  id);
      ASSERT(false);
    }

  printf("waiter_func: Thread %d awakened\n",  id);

  status = sem_getvalue(&sem, &value);
  if (status < 0)
    {
      printf("waiter_func: "
             "ERROR thread %d could not get semaphore value\n",  id);
      ASSERT(false);
    }
  else
    {
      printf("waiter_func: "
             "Thread %d new semaphore value = %d\n",  id, value);
    }

  printf("waiter_func: Thread %d done\n",  id);
  return NULL;
}

static void *poster_func(void *parameter)
{
  int id  = (int)((intptr_t)parameter);
  int status;
  int value;

  printf("poster_func: Thread %d started\n",  id);

  /* Take the semaphore */

  do
    {
      status = sem_getvalue(&sem, &value);
      if (status < 0)
        {
          printf("poster_func: "
                 "ERROR thread %d could not get semaphore value\n",  id);
          ASSERT(false);
        }
      else
        {
          printf("poster_func: "
                 "Thread %d semaphore value = %d\n",  id, value);
        }

      if (value < 0)
        {
          printf("poster_func: Thread %d posting semaphore\n",  id);
          status = sem_post(&sem);
          if (status != 0)
            {
              printf("poster_func: ERROR thread %d sem_wait failed\n",  id);
              ASSERT(false);
            }

          pthread_yield();

          status = sem_getvalue(&sem, &value);
          if (status < 0)
            {
              printf("poster_func: "
                     "ERROR thread %d could not get semaphore value\n",  id);
              ASSERT(false);
            }
          else
            {
              printf("poster_func: "
                     "Thread %d new semaphore value = %d\n",  id, value);
            }
        }
    }
  while (value < 0);

  printf("poster_func: Thread %d done\n",  id);
  return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void sem_test(void)
{
  pthread_t waiter_thread1 = (pthread_t)0;
  pthread_t waiter_thread2 = (pthread_t)0;
  pthread_t poster_thread = (pthread_t)0;
#ifdef SDCC
  pthread_addr_t result;
#endif
  struct sched_param sparam;
  int prio_min;
  int prio_max;
  int prio_mid;
  pthread_attr_t attr;
  int status;

  printf("sem_test: Initializing semaphore to 0\n");
  sem_init(&sem, 0, 0);

  /* Start two waiter thread instances */

  printf("sem_test: Starting waiter thread 1\n");
  status = pthread_attr_init(&attr);
  if (status != OK)
    {
      printf("sem_test: pthread_attr_init failed, status=%d\n",  status);
    }

  prio_min = sched_get_priority_min(SCHED_FIFO);
  prio_max = sched_get_priority_max(SCHED_FIFO);
  prio_mid = (prio_min + prio_max) / 2;

  sparam.sched_priority = (prio_mid + prio_max) / 2;
  status = pthread_attr_setschedparam(&attr, &sparam);
  if (status != OK)
    {
      printf("sem_test: ERROR: "
             "pthread_attr_setschedparam failed, status=%d\n",  status);
      ASSERT(false);
    }
  else
    {
      printf("sem_test: "
             "Set thread 1 priority to %d\n",  sparam.sched_priority);
    }

  status = pthread_create(&waiter_thread1, &attr,
                          waiter_func, (pthread_addr_t)1);
  if (status != 0)
    {
      printf("sem_test: ERROR: "
             "Thread 1 creation failed: %d\n",  status);
      ASSERT(false);
    }

  printf("sem_test: Starting waiter thread 2\n");
  status = pthread_attr_init(&attr);
  if (status != 0)
    {
      printf("sem_test: ERROR: "
             "pthread_attr_init failed, status=%d\n",  status);
      ASSERT(false);
    }

  sparam.sched_priority = prio_mid;
  status = pthread_attr_setschedparam(&attr, &sparam);
  if (status != OK)
    {
      printf("sem_test: ERROR: "
             "pthread_attr_setschedparam failed, status=%d\n",  status);
      ASSERT(false);
    }
  else
    {
      printf("sem_test: "
             "Set thread 2 priority to %d\n",  sparam.sched_priority);
    }

  status = pthread_create(&waiter_thread2, &attr,
                          waiter_func, (pthread_addr_t)2);
  if (status != 0)
    {
      printf("sem_test: ERROR: Thread 2 creation failed: %d\n",  status);
      ASSERT(false);
    }

  /* Make sure waiter_thread1 and waiter_thread2 in sem_wait */

  do
    {
      sem_getvalue(&sem, &status);
      usleep(10 * 1000L);
    }
  while (status != -2);

  printf("sem_test: Starting poster thread 3\n");
  status = pthread_attr_init(&attr);
  if (status != 0)
    {
      printf("sem_test: ERROR: "
             "pthread_attr_init failed, status=%d\n",  status);
      ASSERT(false);
    }

  sparam.sched_priority = (prio_min + prio_mid) / 2;
  status = pthread_attr_setschedparam(&attr, &sparam);
  if (status != OK)
    {
      printf("sem_test: "
             "pthread_attr_setschedparam failed, status=%d\n",  status);
    }
  else
    {
      printf("sem_test: Set thread 3 priority to %d\n",
              sparam.sched_priority);
    }

  status = pthread_create(&poster_thread, &attr,
                           poster_func, (pthread_addr_t)3);
  if (status != 0)
    {
      printf("sem_test: ERROR: Thread 3 creation failed: %d\n",  status);
      printf("          Canceling waiter threads\n");
      ASSERT(false);

      pthread_cancel(waiter_thread1);
      pthread_cancel(waiter_thread2);
    }

#ifdef SDCC
  if (waiter_thread1 != (pthread_t)0)
    {
      pthread_join(waiter_thread1, &result);
    }

  if (waiter_thread2 != (pthread_t)0)
    {
      pthread_join(waiter_thread2, &result);
    }

  if (poster_thread != (pthread_t)0)
    {
      pthread_join(poster_thread, &result);
    }
#else
  if (waiter_thread1 != (pthread_t)0)
    {
      pthread_join(waiter_thread1, NULL);
    }

  if (waiter_thread2 != (pthread_t)0)
    {
      pthread_join(waiter_thread2, NULL);
    }

  if (poster_thread != (pthread_t)0)
    {
      pthread_join(poster_thread, NULL);
    }
#endif

  sem_destroy(&sem);
}
