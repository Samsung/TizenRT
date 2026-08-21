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
 * apps/examples/testcase/ostest/posixtimer.c
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
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include <tinyara/signal.h>

#include "ostest.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

#define MY_TIMER_SIGNAL SIGRTMIN
#define SIGVALUE_INT    42

/****************************************************************************
 * Private Data
 ****************************************************************************/

static int g_nsigreceived = 0;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void timer_expiration(int signo, FAR const siginfo_t *info)
{
  /* Check signo */

  if (signo != MY_TIMER_SIGNAL)
    {
      printf("timer_expiration: ERROR expected signo=%d\n",
             MY_TIMER_SIGNAL);
      ASSERT(false);
    }

  /* Check siginfo */

  if (info->si_value.sival_int != SIGVALUE_INT)
    {
      printf("timer_expiration: ERROR sival_int=%d expected %d\n",
              info->si_value.sival_int, SIGVALUE_INT);
      ASSERT(false);
    }
  else
    {
      printf("timer_expiration: sival_int=%d\n" , info->si_value.sival_int);
    }

  if (info->si_signo != MY_TIMER_SIGNAL)
    {
      printf("timer_expiration: ERROR expected si_signo=%d, got=%d\n",
               MY_TIMER_SIGNAL, info->si_signo);
      ASSERT(false);
    }

  if (info->si_code == SI_TIMER)
    {
      printf("timer_expiration: si_code=%d (SI_TIMER)\n" , info->si_code);
    }
  else
    {
      printf("timer_expiration: ERROR si_code=%d, expected SI_TIMER=%d\n",
             info->si_code, SI_TIMER);
      ASSERT(false);
    }

  g_nsigreceived++;
  printf("timer_expiration: g_nsigreceived=%d\n", g_nsigreceived);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void timer_test(void)
{
  sigset_t           set;
  siginfo_t          info;
  struct sigevent    notify;
  struct itimerspec  timer;
  timer_t            timerid;
  int                status;
  int                i;

  /* Block the timer signal so that it stays pending until we explicitly
   * wait for it via sigwaitinfo().
   */

  sigemptyset(&set);
  sigaddset(&set, MY_TIMER_SIGNAL);

  printf("timer_test: Masking signal %d\n", MY_TIMER_SIGNAL);

  status = sigprocmask(SIG_BLOCK, &set, NULL);
  if (status != OK)
    {
      printf("timer_test: ERROR sigprocmask failed, status=%d\n",
              status);
      ASSERT(false);
    }

  /* Create the POSIX timer */

  printf("timer_test: Creating timer\n");

  notify.sigev_notify            = SIGEV_SIGNAL;
  notify.sigev_signo             = MY_TIMER_SIGNAL;
  notify.sigev_value.sival_int   = SIGVALUE_INT;
#ifdef CONFIG_SIG_EVTHREAD
  notify.sigev_notify_function   = NULL;
  notify.sigev_notify_attributes = NULL;
#endif

  status = timer_create(CLOCK_REALTIME, &notify, &timerid);
  if (status != OK)
    {
      printf("timer_test: ERROR timer_create failed, errno=%d\n", errno);
      ASSERT(false);
      goto errorout;
    }

  /* Start the POSIX timer */

  printf("timer_test: Starting timer\n");

  timer.it_value.tv_sec     = 2;
  timer.it_value.tv_nsec    = 0;
  timer.it_interval.tv_sec  = 2;
  timer.it_interval.tv_nsec = 0;

  status = timer_settime(timerid, 0, &timer, NULL);
  if (status != OK)
    {
      printf("timer_test: ERROR timer_settime failed, errno=%d\n", errno);
      ASSERT(false);
      goto errorout;
    }

  /* Synchronously wait for the timer signal via sigwaitinfo(). */

  for (i = 0; i < 5; i++)
    {
      printf("timer_test: Waiting on sigwaitinfo\n");
      FFLUSH();
      status = sigwaitinfo(&set, &info);
      if (status < 0)
        {
          printf("timer_test: ERROR sigwaitinfo failed, errno=%d\n", errno);
          ASSERT(false);
        }
      else
        {
          printf("timer_test: sigwaitinfo() returned signo=%d\n", status);
          timer_expiration(status, &info);
        }
    }

errorout:

  /* Delete the timer */

  printf("timer_test: Deleting timer\n");
  status = timer_delete(timerid);
  if (status != OK)
    {
      printf("timer_test: ERROR timer_delete failed, errno=%d\n", errno);
      ASSERT(false);
    }

  /* Restore the signal mask */

  sigprocmask(SIG_UNBLOCK, &set, NULL);

  printf("timer_test: done\n");
  FFLUSH();
}
