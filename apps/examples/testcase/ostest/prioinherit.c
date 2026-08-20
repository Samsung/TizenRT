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
 * apps/examples/testcase/ostest/prioinherit.c
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef CONFIG_SIM_WALLTIME_SLEEP
#  include <nuttx/arch.h>
#endif

#include <sys/wait.h>

#include "ostest.h"

#if defined(CONFIG_PRIORITY_INHERITANCE) && !defined(CONFIG_DISABLE_PTHREAD)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_SEM_PREALLOCHOLDERS
#  define CONFIG_SEM_PREALLOCHOLDERS 0
#endif

/* If resources were configured for lots of holders, then run 3 low priority
 * threads.  Otherwise, just one.
 */

#if CONFIG_SEM_PREALLOCHOLDERS > 3
#  define NLOWPRI_THREADS 3
#else
#  define NLOWPRI_THREADS 1
#endif

#define NHIGHPRI_THREADS 1

#define NUMBER_OF_COMPETING_THREADS     3
#define COMPETING_THREAD_START_PRIORITY 170
#define PRIORIY_SPREED                  10
#define CONFIG_DEFAULT_TASK_STACKSIZE   1024

/****************************************************************************
 * Private Types
 ****************************************************************************/

enum thstate_e
{
  NOTSTARTED = 0,
  RUNNING,
  WAITING,
  DONE
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static sem_t g_sem;
static volatile enum thstate_e g_middlestate;
static volatile enum thstate_e g_highstate[NHIGHPRI_THREADS];
static volatile enum thstate_e g_lowstate[NLOWPRI_THREADS];
static volatile int g_priority_tracking[NUMBER_OF_COMPETING_THREADS];
static int g_highpri;
static int g_medpri;
static int g_lowpri;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sleep_and_display
 ****************************************************************************/

static void sleep_and_display(int n, int us)
{
  struct sched_param sparam;

  us /= 100;

  do
    {
      int status = sched_getparam(getpid(), &sparam);

      if (status != 0)
        {
          printf("priority_inheritance: ERROR sched_getparam failed\n");
          ASSERT(false);
        }

      if (us == 0 || g_priority_tracking[n] != sparam.sched_priority)
        {
          if (g_priority_tracking[n] == 0)
            {
              printf("priority_inheritance: "
                     "Task%1d initial priority is:%d\n",
                     n, sparam.sched_priority);
            }
          else
            {
              printf("priority_inheritance: "
                     "Task%1d priority was:%d is:%d\n",
                     n, g_priority_tracking[n], sparam.sched_priority);
            }

          g_priority_tracking[n] = sparam.sched_priority;
          FFLUSH();
        }

      usleep(100);
      us--;
    }
  while (us > 0);
}

/****************************************************************************
 *
 ****************************************************************************/

static int adversary(int argc, FAR char *argv[])
{
  int index        = atoi(argv[1]);
  int inital_delay = atoi(argv[2]);
  int hold_delay   = atoi(argv[3]);

  sleep_and_display(index, inital_delay);
  printf("priority_inheritance: "
         "%s Started, waiting %d uS to take count\n", argv[0], inital_delay);
  sem_wait(&g_sem);
  sleep_and_display(index,  hold_delay);
  sem_post(&g_sem);
  printf("priority_inheritance: %s Posted\n", argv[0]);
  sleep_and_display(index, 0);
  return 0;
}

/****************************************************************************
 * Name: nhighpri_waiting
 ****************************************************************************/

static int nhighpri_waiting(void)
{
  int n = 0;
  int i;

  for (i = 0; i < NHIGHPRI_THREADS; i++)
    {
      if (g_highstate[i] == WAITING)
        {
          n++;
        }
    }

  return n;
}

/****************************************************************************
 * Name: nhighpri_running
 ****************************************************************************/

static int nhighpri_running(void)
{
  int n = 0;
  int i;

  for (i = 0; i < NHIGHPRI_THREADS; i++)
    {
      if (g_highstate[i] != DONE)
        {
          n++;
        }
    }

  return n;
}

/****************************************************************************
 * Name: highpri_thread
 ****************************************************************************/

static FAR void *highpri_thread(FAR void *parameter)
{
  int threadno = (int)((intptr_t)parameter);
  int ret;

  g_highstate[threadno - 1] = RUNNING;

  printf("highpri_thread-%d: Started\n", threadno);
  FFLUSH();
  sleep(1);

  printf("highpri_thread-%d: Calling sem_wait()\n", threadno);
  g_highstate[threadno - 1] = WAITING;
  ret                     = sem_wait(&g_sem);
  g_highstate[threadno - 1] = DONE;

  if (ret != 0)
    {
      printf("highpri_thread-%d: "
             "ERROR sem_take failed: %d\n", threadno, ret);
      ASSERT(false);
    }
  else if (g_middlestate == RUNNING)
    {
      printf("highpri_thread-%d: SUCCESS midpri_thread is still running!\n",
             threadno);
    }
  else
    {
      printf("highpri_thread-%d: ERROR --  "
             "midpri_thread has already exited!\n",
             threadno);
      ASSERT(false);
    }

  sem_post(&g_sem);
  printf("highpri_thread-%d: Okay... I'm done!\n", threadno);
  FFLUSH();
  return NULL;
}

/****************************************************************************
 * Name: hog_cpu
 ****************************************************************************/

static inline void hog_cpu(void)
{
#ifdef CONFIG_SIM_WALLTIME_SLEEP
  /* The simulator doesn't have any mechanism to do asynchronous pre-emption
   * (basically because it doesn't have any interrupts/asynchronous events).
   * The simulator does "fake" a timer interrupt in up_idle() -- the idle
   * thread that only executes when nothing else is running.  In the
   * simulator, we cannot suspend the middle priority task, or we wouldn't
   * have the test that we want.  So, we have no option but to pump the fake
   * clock here by calling up_idle().  Sigh!
   */

  up_idle();
#else
  /* On real platforms with a real timer interrupt, we really can hog the
   * CPU.  When the sleep() goes off in priority_inheritance(), it will
   * wake up and start the high priority thread.
   */

  volatile int i;
  for (i = 0; i < INT_MAX; i++);
#endif
}

/****************************************************************************
 * Name: medpri_thread
 ****************************************************************************/

static FAR void *medpri_thread(FAR void *parameter)
{
  printf("medpri_thread: Started ... I won't let go of the CPU!\n");
  g_middlestate = RUNNING;
  FFLUSH();

  /* The following loop will completely block lowpri_thread from running.
   * UNLESS priority inheritance is working.  In that case, its priority
   * will be boosted.
   */

  while (nhighpri_running() > 0)
    {
      hog_cpu();
    }

  printf("medpri_thread: Okay... I'm done!\n");
  FFLUSH();
  g_middlestate = DONE;
  return NULL;
}

/****************************************************************************
 * Name: lowpri_thread
 ****************************************************************************/

static FAR void *lowpri_thread(FAR void *parameter)
{
  FAR void *retval = (FAR void *)-1;
  struct sched_param sparam;
  int threadno = (int)((intptr_t)parameter);
  int expected;
  int count;
  int policy;
  int ret;
  int nwaiting = 0;
  int i;

  g_lowstate[threadno - 1] = RUNNING;
  printf("lowpri_thread-%d: Started\n", threadno);

  ret = pthread_getschedparam(pthread_self(), &policy, &sparam);
  if (ret != 0)
    {
      printf("lowpri_thread-%d: ERROR pthread_getschedparam failed: %d\n",
             threadno, ret);
      ASSERT(false);
    }
  else
    {
      printf("lowpri_thread-%d: initial priority: %d\n", threadno,
          sparam.sched_priority);
      if (sparam.sched_priority != g_lowpri)
        {
          printf("               ERROR should have been %d\n", g_lowpri);
          ASSERT(false);
        }
    }

  g_lowstate[threadno - 1] = WAITING;
  ret = sem_wait(&g_sem);
  if (ret != 0)
    {
      printf("lowpri_thread-%d: ERROR sem_take failed: %d\n", threadno, ret);
      ASSERT(false);
    }
  else
    {
      /* Hang on to the thread until the middle priority thread runs */

      while (g_middlestate == NOTSTARTED &&
             nhighpri_waiting() < NHIGHPRI_THREADS)
        {
          printf("lowpri_thread-%d: Waiting for the midle pri task to run\n",
                 threadno);
          printf("    g_middlestate:  %d\n", (int)g_middlestate);
          for (i = 0; i < NHIGHPRI_THREADS; i++)
            {
              printf("    g_highstate[%d]: %d\n", i, (int)g_highstate[i]);
            }

          printf("    I still have a count on the semaphore\n");
          sem_enumholders(&g_sem);
          FFLUSH();
          sleep(1);
        }

      /* Account for all of the semaphore counts.  At any given time if there
       * are 'n' running high prioity tasks, then the semaphore count should
       * be '-n'
       */

      sched_lock(); /* Needs to be atomic */
      ret      = sem_getvalue(&g_sem, &count);
      nwaiting = nhighpri_waiting();
      sched_unlock();

      if (ret < 0)
        {
          printf("lowpri_thread-%d: ERROR sem_getvalue failed: %d\n",
                 threadno, errno);
          ASSERT(false);
        }

      printf("lowpri_thread-%d: Sem count: %d, No. highpri thread: %d\n",
             threadno, count, nwaiting);

      /* The middle priority task is running, let go of the semaphore */

      if (g_middlestate == RUNNING && nwaiting == -count)
        {
          /* Good.. the middle priority task is still running and the counts
           * are okay.
           */

          retval = NULL;
        }
      else
        {
          /* If the sem count is positive, then there all of the higher
           * priority threads should have already completed.
           */

          printf("lowpri_thread-%d: %s the middle priority task has already"
                 " exitted!\n",
                 threadno, count >= 0 ? "SUCCESS" : "ERROR");
          ASSERT(count >= 0);
          printf("               g_middlestate:  %d sem count=%d\n",
                 (int)g_middlestate, count);
          for (i = 0; i < NHIGHPRI_THREADS; i++)
            {
              printf("               g_highstate[%d]: %d\n",
                     i, (int)g_highstate[i]);
            }
        }
    }

  ret = pthread_getschedparam(pthread_self(), &policy, &sparam);
  sem_enumholders(&g_sem);
  sem_post(&g_sem);
  if (ret != 0)
    {
      printf("lowpri_thread-%d: ERROR pthread_getschedparam failed: %d\n",
             threadno, ret);
      ASSERT(false);
    }
  else
    {
      if (nwaiting > 0)
        {
          expected = g_highpri;
        }
      else
        {
          expected = g_lowpri;
        }

      printf("lowpri_thread-%d: %s priority before sem_post: %d\n",
             threadno,
             sparam.sched_priority != expected ? "ERROR" : "SUCCESS",
             sparam.sched_priority);

      if (sparam.sched_priority != expected)
        {
          printf("               ERROR should have been %d\n", expected);
          ASSERT(false);
        }
    }

  ret = pthread_getschedparam(pthread_self(), &policy, &sparam);
  if (ret != 0)
    {
      printf("lowpri_thread-%d: ERROR pthread_getschedparam failed: %d\n",
             threadno, ret);
      ASSERT(false);
    }
  else
    {
      printf("lowpri_thread-%d: %s final priority: %d\n",
             threadno,
             sparam.sched_priority != g_lowpri ? "ERROR" : "SUCCESS",
             sparam.sched_priority);

      if (sparam.sched_priority != g_lowpri)
        {
          printf("               ERROR should have been %d\n", g_lowpri);
          ASSERT(false);
        }
    }

  sem_enumholders(&g_sem);

  printf("lowpri_thread-%d: Okay... I'm done!\n", threadno);
  FFLUSH();
  g_lowstate[threadno - 1] = DONE;
  return retval;
}
#endif /* CONFIG_PRIORITY_INHERITANCE && !CONFIG_DISABLE_PTHREAD */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: priority_inheritance
 ****************************************************************************/

void priority_inheritance(void)
{
#if defined(CONFIG_PRIORITY_INHERITANCE) && !defined(CONFIG_DISABLE_PTHREAD)
  pthread_t lowpri[NLOWPRI_THREADS];
  pthread_t medpri;
  pthread_t highpri[NHIGHPRI_THREADS];
  pthread_addr_t result;
  pthread_attr_t attr;
  struct sched_param sparam;
  int my_pri;
  int status;
  int i;
  pid_t pids[NUMBER_OF_COMPETING_THREADS];
  char args[3][32];
  FAR char *argv[4];
  char name[32];
  int priority;
  int restoration_result;

  printf("priority_inheritance: Started\n");

  g_middlestate = NOTSTARTED;
  for (i = 0; i < NHIGHPRI_THREADS; i++) g_highstate[i] = NOTSTARTED;
  for (i = 0; i < NLOWPRI_THREADS; i++)  g_lowstate[i]  = NOTSTARTED;

  status = sched_getparam(getpid(), &sparam);
  if (status != 0)
    {
      printf("priority_inheritance: ERROR sched_getparam failed\n");
      ASSERT(false);
      sparam.sched_priority = PTHREAD_DEFAULT_PRIORITY;
    }

  my_pri = sparam.sched_priority;

  g_highpri = sched_get_priority_max(SCHED_FIFO);
  g_lowpri = sched_get_priority_min(SCHED_FIFO);
  g_medpri = my_pri - 1;

  sem_init(&g_sem, 0, NLOWPRI_THREADS);
  sem_setprotocol(&g_sem, SEM_PRIO_INHERIT);

  dump_nfreeholders("priority_inheritance:");

  /* Start the low priority threads */

  for (i = 0; i < NLOWPRI_THREADS; i++)
    {
      int threadno = i + 1;
      printf("priority_inheritance: "
             "Starting lowpri_thread-%d (of %d) at %d\n",
             threadno, NLOWPRI_THREADS, g_lowpri);
      status = pthread_attr_init(&attr);
      if (status != 0)
        {
          printf("priority_inheritance: "
                 "ERROR pthread_attr_init failed, status=%d\n",
                 status);
          ASSERT(false);
        }

      sparam.sched_priority = g_lowpri;
      status = pthread_attr_setschedparam(&attr, &sparam);
      if (status != OK)
        {
          printf("priority_inheritance: "
                 "ERROR pthread_attr_setschedparam failed, status=%d\n",
                 status);
          ASSERT(false);
        }
      else
        {
          printf("priority_inheritance: "
                 "Set lowpri_thread-%d priority to %d\n",
                 threadno, sparam.sched_priority);
        }

      status = pthread_create(&lowpri[i], &attr, lowpri_thread,
                              (FAR void *)((uintptr_t)threadno));
      if (status != 0)
        {
          printf("priority_inheritance: "
                 "ERROR pthread_create failed, status=%d\n", status);
          ASSERT(false);
        }
    }

  printf("priority_inheritance: Waiting...\n");
  sleep(2);
  dump_nfreeholders("priority_inheritance:");

  /* Start the medium priority thread */

  printf("priority_inheritance: Starting medpri_thread at %d\n", g_medpri);
  status = pthread_attr_init(&attr);
  if (status != 0)
    {
      printf("priority_inheritance: "
             "ERROR pthread_attr_init failed, status=%d\n", status);
      ASSERT(false);
    }

  sparam.sched_priority = g_medpri;
  status = pthread_attr_setschedparam(&attr, &sparam);
  if (status != OK)
    {
      printf("priority_inheritance: "
             "ERROR pthread_attr_setschedparam failed, status=%d\n",
              status);
      ASSERT(false);
    }
  else
    {
      printf("priority_inheritance: Set medpri_thread priority to %d\n",
             sparam.sched_priority);
    }

  FFLUSH();

  status = pthread_create(&medpri, &attr, medpri_thread, NULL);
  if (status != 0)
    {
      printf("priority_inheritance: "
             "ERROR pthread_create failed, status=%d\n",
             status);
      ASSERT(false);
    }

  printf("priority_inheritance: Waiting...\n");
  sleep(1);
  dump_nfreeholders("priority_inheritance:");

  /* Start the high priority threads */

  for (i = 0; i < NHIGHPRI_THREADS; i++)
    {
      int threadno = i + 1;
      printf("priority_inheritance: "
             "Starting highpri_thread-%d (of %d) at %d\n",
             threadno, NHIGHPRI_THREADS, g_highpri);
      status = pthread_attr_init(&attr);
      if (status != 0)
        {
          printf("priority_inheritance: "
                 "ERROR pthread_attr_init failed, status=%d\n", status);
          ASSERT(false);
        }

      sparam.sched_priority = g_highpri - i;
      status = pthread_attr_setschedparam(&attr, &sparam);
      if (status != OK)
        {
          printf("priority_inheritance: "
                 "ERROR pthread_attr_setschedparam failed, status=%d\n",
                 status);
          ASSERT(false);
        }
      else
        {
          printf("priority_inheritance: "
                 "Set highpri_thread-%d priority to %d\n",
                 threadno, sparam.sched_priority);
        }

      FFLUSH();

      status = pthread_create(&highpri[i], &attr, highpri_thread,
                              (FAR void *)((uintptr_t)threadno));
      if (status != 0)
        {
          printf("priority_inheritance: "
                 "ERRROR pthread_create failed, status=%d\n", status);
          ASSERT(false);
        }
    }

  dump_nfreeholders("priority_inheritance:");
  FFLUSH();

  /* Wait for all thread instances to complete */

  for (i = 0; i < NHIGHPRI_THREADS; i++)
    {
      printf("priority_inheritance: "
             "Waiting for highpri_thread-%d to complete\n", i + 1);
      FFLUSH();
      pthread_join(highpri[i], &result);
      dump_nfreeholders("priority_inheritance:");
    }

  printf("priority_inheritance: Waiting for medpri_thread to complete\n");
  FFLUSH();
  pthread_join(medpri, &result);
  dump_nfreeholders("priority_inheritance:");

  for (i = 0; i < NLOWPRI_THREADS; i++)
    {
      printf("priority_inheritance: "
             "Waiting for lowpri_thread-%d to complete\n", i + 1);
      FFLUSH();
      pthread_join(lowpri[i], &result);
      dump_nfreeholders("priority_inheritance:");
    }

  /* Perform restoration test */

  printf("priority_inheritance: Restoration Test:\n");

  sem_init(&g_sem, 0, 1);
  priority = COMPETING_THREAD_START_PRIORITY;
  argv[0] = args[0];
  argv[1] = args[1];
  argv[2] = args[2];
  argv[3] = NULL;

  for (i = 0; i < NUMBER_OF_COMPETING_THREADS; i++)
    {
      g_priority_tracking[i] = 0;
      snprintf(name, sizeof(name), "Task%1d", i);
      snprintf(args[0], sizeof(args[0]), "%d", i);
      snprintf(args[1], sizeof(args[1]), "%d", i * 10000);
      snprintf(args[2], sizeof(args[2]), "%d", i == 0 ? 100000 : 1000);

      pids[i] = task_create(name, priority, CONFIG_DEFAULT_TASK_STACKSIZE,
                            adversary, argv);
      priority += PRIORIY_SPREED;
    }

  priority = COMPETING_THREAD_START_PRIORITY;
  restoration_result = 0;
  for (i = 0; i < NUMBER_OF_COMPETING_THREADS; i++)
    {
      printf("priority_inheritance: "
             "Waiting for Task-%d to complete\n", i);

      waitpid(pids[i], &status, 0);
      if (priority != g_priority_tracking[i])
        {
          printf("priority_inheritance: "
                 "Task-%d Priority is %d, and was not restored to %d\n",
                  i, g_priority_tracking[i], priority);

          restoration_result |= 1 << i;
        }

      priority += PRIORIY_SPREED;
    }

  if (restoration_result != 0)
    {
      printf("priority_inheritance: ERROR: FAIL Priorities were not "
             "correctly restored.\n");
      ASSERT(false);
    }
  else
    {
      printf("priority_inheritance: PASSED Priority were correctly"
             " restored.\n");
    }

  printf("priority_inheritance: Finished\n");
  sem_destroy(&g_sem);
  dump_nfreeholders("priority_inheritance:");
  FFLUSH();
#endif /* CONFIG_PRIORITY_INHERITANCE && !CONFIG_DISABLE_PTHREAD */
}
