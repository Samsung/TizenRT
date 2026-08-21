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
 * apps/examples/testcase/ostest/semtimed.c
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
#include <sched.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "ostest.h"

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

static sem_t sem;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void *poster_func(void *parameter)
{
  int status;

  /* Wait for one second, then post the semaphore */

  printf("poster_func: Waiting for 1 second\n");
  sleep(1);

  printf("poster_func: Posting\n");
  status = sem_post(&sem);
  if (status != OK)
    {
      printf("poster_func: ERROR: sem_post failed\n");
      ASSERT(false);
    }

  return NULL;
}

static void ostest_gettime(struct timespec *tp)
{
  int status;

  status = clock_gettime(CLOCK_REALTIME, tp);
  if (status != OK)
    {
      printf("ostest_gettime: ERROR: clock_gettime failed\n");
      ASSERT(false);
    }
  else if (tp->tv_sec < 0 || tp->tv_nsec < 0 ||
           tp->tv_nsec >= 1000 * 1000 * 1000)
    {
      printf("ostest_gettime: ERROR: clock_gettime returned bogus time\n");
      ASSERT(false);
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void semtimed_test(void)
{
  pthread_t poster_thread = (pthread_t)0;
#ifdef SDCC
  pthread_addr_t result;
#endif
  struct sched_param sparam;
  struct timespec abstime;
  struct timespec before;
  struct timespec after;
  int prio_min;
  int prio_max;
  int prio_mid;
  int errcode;
  pthread_attr_t attr;
  int status;

  printf("semtimed_test: Initializing semaphore to 0\n");
  status = sem_init(&sem, 0, 0);
  if (status != OK)
    {
      printf("semtimed_test: ERROR: sem_init failed\n");
      ASSERT(false);
    }

  /* First,
   * make sure that the timeout expires if the semaphore is never posted
   */

  ostest_gettime(&before);

  abstime.tv_sec  = before.tv_sec + 2;
  abstime.tv_nsec = before.tv_nsec;

  printf("semtimed_test: Waiting for two second timeout\n");
  status  = sem_timedwait(&sem, &abstime);
  errcode = errno;

  ostest_gettime(&after);

  if (status == OK)
    {
      printf("semtimed_test: ERROR: sem_timedwait succeeded\n");
      ASSERT(false);
    }
  else
    {
      if (errcode == ETIMEDOUT)
        {
          printf("semtimed_test: PASS: first test returned timeout\n");
        }
      else
        {
          printf("semtimed_test: ERROR: sem_timedwait failed with: %d\n",
                  errcode);
          ASSERT(false);
        }
    }

  printf("BEFORE: (%jd sec, %ld nsec)\n",
          (intmax_t)before.tv_sec, before.tv_nsec);
  printf("AFTER:  (%jd sec, %ld nsec)\n",
          (intmax_t)after.tv_sec, after.tv_nsec);

  /* Now make sure that the time wait returns successfully if the semaphore
   * is posted
   */

  /* Start a poster thread.  It will wait 1 seconds and post the semaphore */

  printf("semtimed_test: Starting poster thread\n");
  status = pthread_attr_init(&attr);
  if (status != OK)
    {
      printf("semtimed_test: ERROR: pthread_attr_init failed, status=%d\n",
              status);
      ASSERT(false);
    }

  prio_min = sched_get_priority_min(SCHED_FIFO);
  prio_max = sched_get_priority_max(SCHED_FIFO);
  prio_mid = (prio_min + prio_max) / 2;

  sparam.sched_priority = (prio_mid + prio_max) / 2;
  status = pthread_attr_setschedparam(&attr, &sparam);
  if (status != OK)
    {
      printf("semtimed_test: ERROR: "
             "pthread_attr_setschedparam failed, status=%d\n",  status);
      ASSERT(false);
    }
  else
    {
      printf("semtimed_test: Set thread 1 priority to %d\n",
              sparam.sched_priority);
    }

  printf("semtimed_test: Starting poster thread 3\n");
  status = pthread_attr_init(&attr);
  if (status != 0)
    {
      printf("semtimed_test: ERROR: pthread_attr_init failed, status=%d\n",
              status);
      ASSERT(false);
    }

  sparam.sched_priority = (prio_min + prio_mid) / 2;
  status = pthread_attr_setschedparam(&attr, &sparam);
  if (status != OK)
    {
      printf("semtimed_test: "
             "ERROR pthread_attr_setschedparam failed, status=%d\n",
              status);
      ASSERT(false);
    }
  else
    {
      printf("semtimed_test: Set thread 3 priority to %d\n",
              sparam.sched_priority);
    }

  status = pthread_create(&poster_thread, &attr, poster_func, NULL);
  if (status != 0)
    {
      printf("semtimed_test: ERROR: Poster thread creation failed: %d\n",
              status);
      ASSERT(false);
      sem_destroy(&sem);
      return;
    }

  /* Up to two seconds for the semaphore to be posted */

  ostest_gettime(&before);

  abstime.tv_sec  = before.tv_sec + 2;
  abstime.tv_nsec = before.tv_nsec;

  printf("semtimed_test: Waiting for two second timeout\n");
  status  = sem_timedwait(&sem, &abstime);
  errcode = errno;

  ostest_gettime(&after);

  if (status < 0)
    {
      printf("semtimed_test: ERROR: sem_timedwait failed with: %d\n",
              errcode);
      ASSERT(false);
    }
  else
    {
      printf("semtimed_test: PASS: sem_timedwait succeeded\n");
    }

  printf("BEFORE: (%jd sec, %ld nsec)\n",
          (intmax_t)before.tv_sec, before.tv_nsec);
  printf("AFTER:  (%jd sec, %ld nsec)\n",
          (intmax_t)after.tv_sec, after.tv_nsec);

  /* Clean up detritus left by the pthread */

#ifdef SDCC
  if (poster_thread != (pthread_t)0)
    {
      pthread_join(poster_thread, &result);
    }
#else
  if (poster_thread != (pthread_t)0)
    {
      pthread_join(poster_thread, NULL);
    }
#endif

  sem_destroy(&sem);
}
