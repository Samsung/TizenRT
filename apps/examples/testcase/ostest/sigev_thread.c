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
 * apps/examples/testcase/ostest/sigev_thread.c
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
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "ostest.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

#define MY_TIMER_SIGNAL SIGRTMIN
#define SIGVALUE_INT    42

/****************************************************************************
 * Private Data
 ****************************************************************************/

static sem_t g_sigev_thread_sem;
static int g_value_received;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void sigev_thread_callback(union sigval value)
{
  int sival_int = value.sival_int;

  printf("sigev_thread_callback: Received value %d\n" , sival_int);

  g_value_received = sival_int;
  sem_post(&g_sigev_thread_sem);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void sigev_thread_test(void)
{
  struct sigevent notify;
  struct itimerspec timer;
  timer_t timerid;
  int status;

  printf("sigev_thread_test: Initializing semaphore to 0\n");
  sem_init(&g_sigev_thread_sem, 0, 0);
  g_value_received = -1;

  /* Create the POSIX timer */

  printf("sigev_thread_test: Creating timer\n");

  notify.sigev_notify            = SIGEV_THREAD;
  notify.sigev_signo             = MY_TIMER_SIGNAL;
  notify.sigev_value.sival_int   = SIGVALUE_INT;
  notify.sigev_notify_function   = sigev_thread_callback;
  notify.sigev_notify_attributes = NULL;

  status = timer_create(CLOCK_REALTIME, &notify, &timerid);
  if (status != OK)
    {
      printf("sigev_thread_test: "
             "ERROR timer_create failed, errno=%d\n", errno);
      ASSERT(false);
      goto errorout;
    }

  /* Start the POSIX timer */

  printf("sigev_thread_test: Starting timer\n");

  timer.it_value.tv_sec     = 2;
  timer.it_value.tv_nsec    = 0;
  timer.it_interval.tv_sec  = 2;
  timer.it_interval.tv_nsec = 0;

  status = timer_settime(timerid, 0, &timer, NULL);
  if (status != OK)
    {
      printf("sigev_thread_test: "
             "ERROR timer_settime failed, errno=%d\n", errno);
      ASSERT(false);
      goto errorout;
    }

  /* Take the semaphore */

  printf("sigev_thread_test: Waiting on semaphore\n");

  do
    {
      status = sem_wait(&g_sigev_thread_sem);
      if (status < 0)
        {
          int error = errno;
          if (error == EINTR)
            {
              printf("sigev_thread_test: sem_wait() interrupted by "
                     "signal\n");
            }
          else
            {
              printf("sigev_thread_test: ERROR sem_wait failed, errno=%d\n",
                     error);
              ASSERT(false);
              goto errorout;
            }
        }
    }
  while (status < 0);

  printf("sigev_thread_test: Awakened with no error!\n");

  /* Check sigval */

  printf("sigev_thread_test: g_value_received=%d\n", g_value_received);
  if (g_value_received != SIGVALUE_INT)
    {
      printf("sigev_thread_callback: ERROR sival_int=%d expected %d\n",
             g_value_received, SIGVALUE_INT);
      ASSERT(false);
    }

errorout:
  sem_destroy(&g_sigev_thread_sem);

  /* Then delete the timer */

  printf("sigev_thread_test: Deleting timer\n");
  status = timer_delete(timerid);
  if (status != OK)
    {
      printf("sigev_thread_test: "
             "ERROR timer_create failed, errno=%d\n", errno);
      ASSERT(false);
    }

  printf("sigev_thread_test: Done\n");
}
