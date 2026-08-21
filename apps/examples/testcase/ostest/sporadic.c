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
 * apps/examples/testcase/ostest/sporadic.c
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

#include <assert.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/param.h>
#include <time.h>
#include <unistd.h>

#include "ostest.h"

#ifdef CONFIG_SCHED_SPORADIC

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* It is actually a better test without schedule locking because that
 * forces the scheduler into an uninteresting fallback mode.
 */

#undef sched_lock
#undef sched_unlock
#define sched_lock()
#define sched_unlock()

/****************************************************************************
 * Private Data
 ****************************************************************************/

static sem_t g_sporadic_sem;
static time_t g_start_time;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void my_mdelay(unsigned int milliseconds)
{
  struct timespec start;
  struct timespec cur;
  struct timespec diff;

  clock_gettime(CLOCK_MONOTONIC, &start);

  for (; ; )
    {
      clock_gettime(CLOCK_MONOTONIC, &cur);
      clock_timespec_subtract(&cur, &start, &diff);
      if (diff.tv_sec * 1000 + diff.tv_nsec / 1000000 > milliseconds)
        {
          break;
        }
    }
}

static void *nuisance_func(void *parameter)
{
  /* Synchronized start */

  while (sem_wait(&g_sporadic_sem) < 0);

  /* Sleep until we are cancelled */

  for (; ; )
    {
      /* Sleep gracefully for awhile */

      usleep(500 * 1000);

      /* Then hog some CPU time */

      my_mdelay(100);
    }

  return NULL;
}

static void *fifo_func(void *parameter)
{
  struct sched_param param;
  time_t last;
  time_t now;
  int ret;

  while (sem_wait(&g_sporadic_sem) < 0);

  last  = g_start_time;

  for (; ; )
    {
      do
        {
          sched_lock(); /* Just to exercise more logic */
          ret = sched_getparam(0, &param);
          if (ret < 0)
            {
              printf("ERROR: sched_getparam failed\n");
              ASSERT(false);
              return NULL;
            }

          now = time(NULL);
          sched_unlock();
        }
      while (now == last);

      sched_lock(); /* Just to exercise more logic */
      printf("%4lu FIFO:     %d\n",
             (unsigned long)(now - g_start_time), param.sched_priority);
      last = now;
      sched_unlock();
    }
}

static FAR void *sporadic_func(FAR void *parameter)
{
  struct sched_param param;
  time_t last;
  time_t now;
  int prio = 0;
  int ret;

  while (sem_wait(&g_sporadic_sem) < 0);

  last  = g_start_time;

  for (; ; )
    {
      do
        {
          sched_lock(); /* Just to exercise more logic */
          ret = sched_getparam(0, &param);
          if (ret < 0)
            {
              printf("ERROR: sched_getparam failed\n");
              ASSERT(false);
              return NULL;
            }

          now = time(NULL);
          sched_unlock();
        }
      while (now == last && prio == param.sched_priority);

      sched_lock(); /* Just to exercise more logic */
      printf("%4lu SPORADIC: %d->%d\n",
             (unsigned long)(now - g_start_time), prio,
             param.sched_priority);
      prio = param.sched_priority;
      last = now;
      sched_unlock();
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void sporadic_test(void)
{
  pthread_t nuisance_thread = (pthread_t)0;
  pthread_t sporadic_thread = (pthread_t)0;
  pthread_t fifo_thread = (pthread_t)0;
#ifdef SDCC
  pthread_addr_t result;
#endif
  FAR void *result;
  struct sched_param myparam;
  struct sched_param sparam;
  int prio_min;
  int prio_max;
  int prio_low;
  int prio_mid;
  int prio_high;
  pthread_attr_t attr;
  int ret;

#if CONFIG_SCHED_SPORADIC_MAXREPL < 5
  printf("sporadic_test: CONFIG_SCHED_SPORADIC_MAXREPL is small: %d\n",
         CONFIG_SCHED_SPORADIC_MAXREPL);
  printf("  -- There will some errors in the replenishment interval\n");
#endif

  printf("sporadic_test: Initializing semaphore to 0\n");
  sem_init(&g_sporadic_sem, 0, 0);

  prio_min  = sched_get_priority_min(SCHED_FIFO);
  prio_max  = sched_get_priority_max(SCHED_FIFO);

  prio_low  = prio_min + ((prio_max - prio_min) >> 2);
  prio_mid  = (prio_min + prio_max) >> 1;
  prio_high = prio_max - ((prio_max - prio_min) >> 2);

  /* Temporarily set our priority to prio_high + 2 */

  ret = sched_getparam(0, &myparam);
  if (ret != OK)
    {
      printf("sporadic_test: ERROR: sched_getparam failed, ret=%d\n", ret);
      ASSERT(false);
    }

  sparam.sched_priority = prio_high + 2;
  ret = sched_setparam(0, &sparam);
  if (ret != OK)
    {
      printf("sporadic_test: ERROR: sched_setparam failed, ret=%d\n", ret);
      ASSERT(false);
    }

  ret = pthread_attr_init(&attr);
  if (ret != OK)
    {
      printf("sporadic_test: ERROR: pthread_attr_init failed, ret=%d\n",
             ret);
      ASSERT(false);
    }

  /* This semaphore will prevent anything from running until we are ready */

  sched_lock();
  sem_init(&g_sporadic_sem, 0, 0);

  /* Start a FIFO thread at the highest priority (prio_max + 1) */

  printf("sporadic_test: Starting FIFO thread at priority %d\n", prio_mid);

  ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  if (ret != OK)
    {
      printf("sporadic_test: ERROR: pthread_attr_setschedpolicy failed, "
             "ret=%d\n",
             ret);
      ASSERT(false);
    }

  sparam.sched_priority = prio_high + 1;
  ret = pthread_attr_setschedparam(&attr, &sparam);
  if (ret != OK)
    {
      printf("sporadic_test: ERROR: pthread_attr_setschedparam failed, "
             "ret=%d\n",
             ret);
      ASSERT(false);
    }

  ret = pthread_create(&nuisance_thread, &attr, nuisance_func, NULL);
  if (ret != 0)
    {
      printf("sporadic_test: ERROR: FIFO thread creation failed: %d\n",
             ret);
      ASSERT(false);
    }

  /* Start a FIFO thread at the middle priority */

  sparam.sched_priority = prio_mid;
  ret = pthread_attr_setschedparam(&attr, &sparam);
  if (ret != OK)
    {
      printf("sporadic_test: ERROR: pthread_attr_setschedparam failed, "
             "ret=%d\n",
             ret);
      ASSERT(false);
    }

  ret = pthread_create(&fifo_thread, &attr, fifo_func, NULL);
  if (ret != 0)
    {
      printf("sporadic_test: ERROR: FIFO thread creation failed: %d\n",
             ret);
      ASSERT(false);
    }

  /* Start a sporadic thread, with the following parameters: */

  printf("sporadic_test: Starting sporadic thread at priority "
         "%d (hi) %d (lo)\n",
         prio_high, prio_low);

  ret = pthread_attr_setschedpolicy(&attr, SCHED_SPORADIC);
  if (ret != OK)
    {
      printf("sporadic_test: ERROR: pthread_attr_setschedpolicy failed, "
             "ret=%d\n",
             ret);
      ASSERT(false);
    }

  sparam.sched_priority               = prio_high;
  sparam.sched_ss_low_priority        = prio_low;
  sparam.sched_ss_repl_period.tv_sec  = 5;
  sparam.sched_ss_repl_period.tv_nsec = 0;
  sparam.sched_ss_init_budget.tv_sec  = 2;
  sparam.sched_ss_init_budget.tv_nsec = 0;
  sparam.sched_ss_max_repl            = CONFIG_SCHED_SPORADIC_MAXREPL;

  ret = pthread_attr_setschedparam(&attr, &sparam);
  if (ret != OK)
    {
      printf("sporadic_test: ERROR: pthread_attr_setsched param failed, "
             "ret=%d\n",
             ret);
      ASSERT(false);
    }

  ret = pthread_create(&sporadic_thread, &attr, sporadic_func,
                       (pthread_addr_t)1);
  if (ret != 0)
    {
      printf("sporadic_test: ERROR: sporadic thread creation failed: %d\n",
             ret);
      ASSERT(false);
    }

  g_start_time = time(NULL);

  sem_post(&g_sporadic_sem);
  sem_post(&g_sporadic_sem);
  sem_post(&g_sporadic_sem);

  /* Wait a while then kill the FIFO thread */

  sleep(15);
  ret = pthread_cancel(fifo_thread);
  pthread_join(fifo_thread, &result);

  /* Wait a bit longer then kill the nuisance thread */

  sleep(10);
  ret = pthread_cancel(nuisance_thread);
  pthread_join(nuisance_thread, &result);

  /* Wait a bit longer then kill the sporadic thread */

  sleep(10);
  ret = pthread_cancel(sporadic_thread);
  pthread_join(sporadic_thread, &result);
  sched_unlock();

  printf("sporadic_test: Done\n");
  sem_destroy(&g_sporadic_sem);

  ret = sched_setparam(0, &myparam);
  if (ret != OK)
    {
      printf("sporadic_test: ERROR: sched_setparam failed, ret=%d\n", ret);
      ASSERT(false);
    }
}

#endif /* CONFIG_SCHED_SPORADIC */
