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
 * apps/examples/testcase/ostest/sighand.c
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
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <tinyara/signal.h>

#include "ostest.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define WAKEUP_SIGNAL SIGRTMIN
#define SIGVALUE_INT  42

/****************************************************************************
 * Private Data
 ****************************************************************************/

static sem_t sem1;
static sem_t sem2;
static bool sigreceived = false;
static bool thread1exited = false;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_SCHED_HAVE_PARENT
static void death_of_child(int signo, siginfo_t *info, void *ucontext)
{
  /* Use of printf in a signal handler is NOT safe! It can cause deadlocks!
   * Also, signals are not queued by NuttX.  As a consequence, some
   * notifications will get lost (or the info data can be overwrittedn)!
   * Because POSIX  does not require signals to be queued, I do not think
   * that this is a bug (the overwriting is a bug, however).
   */

  if (info)
    {
      printf("death_of_child: PID %d received signal=%d code=%d "
             "errno=%d pid=%d status=%d\n",
             getpid(), signo, info->si_code, info->si_errno,
             info->si_pid, info->si_status);
    }
  else
    {
      printf("death_of_child: PID %d received signal=%d (no info?)\n",
             getpid(), signo);
    }
}
#endif

static void wakeup_action(int signo, siginfo_t *info, void *ucontext)
{
  sigset_t oldset;
  sigset_t allsigs;
  int status;

  /* Use of printf in a signal handler is NOT safe! It can cause deadlocks! */

  sigreceived = true;

  /* Check signo */

  if (signo != WAKEUP_SIGNAL)
    {
      ASSERT(false);
    }

  /* Check siginfo */

  if (info->si_value.sival_int != SIGVALUE_INT)
    {
      ASSERT(false);
    }

  if (info->si_signo != WAKEUP_SIGNAL)
    {
      ASSERT(false);
    }

  /* Check sigprocmask */

  sigfillset(&allsigs);
  status = sigprocmask(SIG_SETMASK, NULL, &oldset);
  if (status != OK)
    {
      ASSERT(false);
    }

  if (!sigset_isequal(&oldset, &allsigs))
    {
      ASSERT(false);
    }
}

static FAR void *waiter_main(FAR void *arg)
{
  sigset_t set;
  struct sigaction act;
  struct sigaction oact;
  int status;

  printf("waiter_main: Waiter started\n");

  printf("waiter_main: Unmasking signal %d\n" , WAKEUP_SIGNAL);
  sigemptyset(&set);
  sigaddset(&set, WAKEUP_SIGNAL);
  status = sigprocmask(SIG_UNBLOCK, &set, NULL);
  if (status != OK)
    {
      printf("waiter_main: ERROR sigprocmask failed, status=%d\n",
              status);
      ASSERT(false);
    }

  printf("waiter_main: Registering signal handler\n");
  act.sa_sigaction = wakeup_action;
  act.sa_flags  = SA_SIGINFO;

  sigfillset(&act.sa_mask);
  sigdelset(&act.sa_mask, WAKEUP_SIGNAL);

  status = sigaction(WAKEUP_SIGNAL, &act, &oact);
  if (status != OK)
    {
      printf("waiter_main: ERROR sigaction failed, status=%d\n" , status);
      ASSERT(false);
    }

#ifndef SDCC
  printf("waiter_main: oact.sigaction=%p oact.sa_flags=%x "
         "oact.sa_mask=" SIGSET_FMT "\n",
          oact.sa_sigaction, oact.sa_flags, SIGSET_ELEM(&oact.sa_mask));
#endif

  /* Take the semaphore */

  printf("waiter_main: Waiting on semaphore\n");
  FFLUSH();

  status = sem_wait(&sem1);
  if (status != 0)
    {
      int error = errno;
      if (error == EINTR)
        {
          printf("waiter_main: sem_wait() successfully interrupted by "
                 "signal\n");
        }
      else
        {
          printf("waiter_main: ERROR sem_wait failed, errno=%d\n" , error);
          ASSERT(false);
        }
    }
  else
    {
      printf("waiter_main: ERROR awakened with no error!\n");
      ASSERT(false);
    }

  /* Detach the signal handler */

  act.sa_handler = SIG_DFL;
  sigaction(WAKEUP_SIGNAL, &act, &oact);

  printf("waiter_main: done\n");
  FFLUSH();

  thread1exited = true;
  return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void sighand_test(void)
{
#ifdef CONFIG_SCHED_HAVE_PARENT
  struct sigaction act;
  struct sigaction oact;
#endif
  struct sched_param param;
  pthread_attr_t attr;
  union sigval sigvalue;
  pid_t waiterpid;
  sigset_t oset;
  sigset_t set;
  int status;

  printf("sighand_test: Initializing semaphore to 0\n");
  sem_init(&sem1, 0, 0);
  sem_init(&sem2, 0, 0);

#ifdef CONFIG_SCHED_HAVE_PARENT
  printf("sighand_test: Unmasking SIGCHLD\n");

  sigemptyset(&set);
  sigaddset(&set, SIGCHLD);
  status = sigprocmask(SIG_UNBLOCK, &set, NULL);
  if (status != OK)
    {
      printf("sighand_test: ERROR sigprocmask failed, status=%d\n",
              status);
      ASSERT(false);
    }

  printf("sighand_test: Registering SIGCHLD handler\n");
  act.sa_sigaction = death_of_child;
  act.sa_flags  = SA_SIGINFO;

  sigfillset(&act.sa_mask);
  sigdelset(&act.sa_mask, SIGCHLD);

  status = sigaction(SIGCHLD, &act, &oact);
  if (status != OK)
    {
      printf("waiter_main: ERROR sigaction failed, status=%d\n" , status);
      ASSERT(false);
    }
#endif

  /* Start waiter thread  */

  sigemptyset(&set);
  sigaddset(&set, WAKEUP_SIGNAL);
  status = sigprocmask(SIG_BLOCK, &set, &oset);
  if (status != OK)
    {
      printf("sighand_test: ERROR sigprocmask failed, status=%d\n",
              status);
      ASSERT(false);
    }

  printf("sighand_test: Starting waiter task\n");
  status = sched_getparam (0, &param);
  if (status != OK)
    {
      printf("sighand_test: ERROR sched_getparam() failed\n");
      ASSERT(false);
      param.sched_priority = PTHREAD_DEFAULT_PRIORITY;
    }

  pthread_attr_init(&attr);
  pthread_attr_setschedparam(&attr, &param);
  pthread_attr_setstacksize(&attr, STACKSIZE);
  status = pthread_create(&waiterpid, &attr, waiter_main, NULL);
  if (status != 0)
    {
      printf("sighand_test: ERROR failed to start waiter_main\n");
      ASSERT(false);
    }
  else
    {
      printf("sighand_test: Started waiter_main pid=%d\n", waiterpid);
    }

  /* Wait a bit */

  FFLUSH();
  sleep(2);

  /* Then signal the waiter thread. */

  printf("sighand_test: Signaling pid=%d with signo=%d sigvalue=%d\n",
         waiterpid, WAKEUP_SIGNAL, SIGVALUE_INT);

  sigvalue.sival_int = SIGVALUE_INT;
  status = sigqueue(waiterpid, WAKEUP_SIGNAL, sigvalue);
  if (status != OK)
    {
      printf("sighand_test: ERROR sigqueue failed\n");
      ASSERT(false);
      pthread_cancel(waiterpid);
    }

  /* Wait a bit */

  FFLUSH();
  status = sleep(2);
  while (status)
    {
      status = sleep(status);
    }

  /* Then check the result */

  if (!thread1exited)
    {
      printf("sighand_test: ERROR waiter task did not exit\n");
      ASSERT(false);
    }

  if (!sigreceived)
    {
      printf("sighand_test: ERROR signal handler did not run\n");
      ASSERT(false);
    }

  /* Detach the signal handler */

#ifdef CONFIG_SCHED_HAVE_PARENT
  act.sa_handler = SIG_DFL;
  sigaction(SIGCHLD, &act, &oact);
#endif

  printf("sighand_test: done\n");
  sigprocmask(SIG_SETMASK, &oset, NULL);
  FFLUSH();
  sem_destroy(&sem2);
  sem_destroy(&sem1);
}
