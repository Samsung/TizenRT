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
 * apps/examples/testcase/ostest/schedlock.c
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

/* Scheduler lock test consists of two test cases that use 2 threads with
 * high (H) and low (L) priorities.  The H sets variable to predefined value
 * while L tries to clear the variable.  The H manipulates with with thread
 * priorities while scheduler is locked.  The test is pass if variable set by
 * H is still set to predefined value after thread priority is changed.
 *
 *   Test case 1. The H thread locks scheduler and increase priority of L
 *                to the H+1 priority. The test case passes if H is still
 *                running after priority of L is increased.
 *   Test case 2. The H thread locks scheduler and decreases it's own
 *                priority to L-1. The test case passes if H is still
 *                running after it's priority is decreased.
 *
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sched.h>
#include <stdint.h>

#ifdef CONFIG_SIM_WALLTIME_SLEEP
#  include <tinyara/arch.h>
#endif

#include <sys/wait.h>

#include "ostest.h"

static pthread_t g_lowpri;
static pthread_t g_highpri;
static volatile bool g_locked;
static volatile bool g_pass;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: highpri_thread
 ****************************************************************************/

static FAR void *highpri_thread(FAR void *parameter)
{
  struct sched_param param;
  int policy;
  bool self = (bool)(uintptr_t)parameter;
  pthread_t thread = self ? 0 : g_lowpri;

  usleep(100);

  pthread_getschedparam(0, &policy, &param);
  if (self)
    {
      param.sched_priority -= 2;
    }
  else
    {
      param.sched_priority += 2;
    }

  sched_lock();

  g_locked = true;

  pthread_setschedprio(thread, param.sched_priority);

  /* Test pass if g_locked was not cleared by lowpri thread while scheduler
   * is locked
   */

  g_pass = g_locked;

  sched_unlock();

  return NULL;
}

/****************************************************************************
 * Name: lowpri_thread
 ****************************************************************************/

static FAR void *lowpri_thread(FAR void *parameter)
{
  /* Wait until highpri thread starts the scheduler lock test */

  while (!g_locked)
    {
#ifdef CONFIG_SIM_WALLTIME_SLEEP
      /* The simulator doesn't have any mechanism to do asynchronous
       * pre-emption (basically because it doesn't have any
       * interrupts/asynchronous events).  The simulator does "fake" a timer
       * interrupt in up_idle() -- the idle thread that only executes when
       * nothing else is running.  In the simulator, we cannot suspend the
       * middle priority task, or we wouldn't have the test that we want.
       * So, we have no option but to pump the fake clock here by calling
       * up_idle().  Sigh!
       */

      up_idle();
#endif
    }

  g_locked = false;

  return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sched_lock_test
 ****************************************************************************/

void sched_lock_test(void)
{
  int i;

  for (i = 0; i < 2; i++)
    {
      pthread_attr_t attr;
      struct sched_param sparam;
      int status;
      int highprio;
      int lowprio;
#ifdef CONFIG_SMP
      cpu_set_t cpu_mask;

      CPU_ZERO(&cpu_mask);
      CPU_SET(0, &cpu_mask);
#endif

      status = sched_getparam(0, &sparam);
      if (status != 0)
        {
          printf("sched_lock: ERROR sched_getparam failed\n");
          ASSERT(false);
          sparam.sched_priority = PTHREAD_DEFAULT_PRIORITY;
        }

      highprio = sparam.sched_priority - 2;
      lowprio = highprio - 1;

      /* Start the low priority thread */

      printf("sched_lock: Starting lowpri_thread at %d\n", lowprio);
      status = pthread_attr_init(&attr);
      if (status != 0)
        {
          printf("sched_lock: ERROR pthread_attr_init failed, status=%d\n",
                 status);
          ASSERT(false);
        }

      sparam.sched_priority = lowprio;
      status = pthread_attr_setschedparam(&attr, &sparam);
      if (status != OK)
        {
          printf("sched_lock: "
                 "ERROR pthread_attr_setschedparam failed, status=%d\n",
                  status);
          ASSERT(false);
        }
      else
        {
          printf("sched_lock: Set lowpri_thread priority to %d\n",
                 sparam.sched_priority);
        }

#ifdef CONFIG_SMP
      pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpu_mask);
#endif

      FFLUSH();

      status = pthread_create(&g_lowpri, &attr, lowpri_thread, NULL);
      if (status != 0)
        {
          printf("sched_lock: ERROR pthread_create failed, status=%d\n",
                 status);
          ASSERT(false);
        }

      /* Start the high priority thread */

      printf("sched_lock: Starting highpri_thread at %d\n", highprio);
      status = pthread_attr_init(&attr);
      if (status != 0)
        {
          printf("sched_lock: ERROR pthread_attr_init failed, status=%d\n",
                 status);
          ASSERT(false);
        }

      sparam.sched_priority = highprio;
      status = pthread_attr_setschedparam(&attr, &sparam);
      if (status != OK)
        {
          printf("sched_lock: "
                 "ERROR pthread_attr_setschedparam failed, status=%d\n",
                  status);
          ASSERT(false);
        }
      else
        {
          printf("sched_lock: Set highpri_thread priority to %d\n",
                 sparam.sched_priority);
        }

#ifdef CONFIG_SMP
      pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpu_mask);
#endif

      FFLUSH();

      status = pthread_create(&g_highpri, &attr, highpri_thread,
                              (FAR void *)(uintptr_t)(i == 0));
      if (status != 0)
        {
          printf("sched_lock: ERROR pthread_create failed, status=%d\n",
                 status);
          ASSERT(false);
        }

      printf("sched_lock: Waiting...\n");
      sleep(1);

      pthread_join(g_highpri, NULL);
      pthread_join(g_lowpri, NULL);

      if (!g_pass)
        {
          printf("sched_lock: ERROR: FAIL pre-emption occurred "
                 "while scheduler was locked.\n");
          ASSERT(false);
        }
      else
        {
          printf("sched_lock: PASSED No pre-emption occurred "
                 "while scheduler was locked.\n");
        }
    }

  printf("sched_lock: Finished\n");
  FFLUSH();
}
