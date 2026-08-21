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
 * apps/examples/testcase/ostest/pthread_exit.c
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
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include "ostest.h"

#ifdef CONFIG_SCHED_WAITPID

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define PRIORITY  100

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static FAR void *pthread_exit_thread(FAR void *parameter)
{
  unsigned me = (unsigned)pthread_self();

  printf("pthread_exit_thread %u: Sleeping for 10 second\n", me);
  sleep(5);
  printf("pthread_exit_thread %u: Still running...\n", me);
  sleep(5);
  printf("pthread_exit_thread %u: Exiting\n", me);
  return NULL;
}

static FAR void *pthread_exit_main(FAR void *arg)
{
  pthread_t child;
#ifdef SDCC
  pthread_attr_t attr;
#endif
  unsigned me = (unsigned)pthread_self();
  int ret;

  printf("pthread_exit_main %u: Starting pthread_exit_thread\n", me);
#ifdef SDCC
  pthread_attr_init(&attr);
  ret = pthread_create(&child, &attr, pthread_exit_thread, NULL);
#else
  ret = pthread_create(&child, NULL, pthread_exit_thread, NULL);
#endif
  if (ret != 0)
    {
      printf("Thread creation failed, return code %d", ret);
    }

  printf("pthread_exit_main %u: Sleeping for 5 seconds\n", me);
  fflush(stdout);
  sleep(5);

  printf("pthread_exit_main %u: Calling pthread_exit()\n", me);
  pthread_exit(NULL);

  printf("pthread_exit_main %u: ERROR:  Still running\n", me);
  exit(0);

  return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void pthread_exit_test(void)
{
  struct sched_param param;
  pthread_attr_t attr;
  pid_t pid;
  int ret;

  pthread_attr_init(&attr);
  param.sched_priority = PRIORITY;
  pthread_attr_setschedparam(&attr, &param);
  pthread_attr_setstacksize(&attr, STACKSIZE);
  ret = pthread_create(&pid, &attr, pthread_exit_main, NULL);
  if (ret < 0)
    {
      printf("pthread_exit_test:  ERROR pthread_create Failed\n");
    }
  else
    {
      printf("pthread_exit_test: Started pthread_exit_main at PID=%d\n",
             pid);
      if (pthread_join(pid, NULL) != 0)
        {
          printf("pthread_exit_test: ERROR Failed to join to terminate\n");
          ASSERT(false);
        };
    }
}

#endif /* CONFIG_SCHED_WAITPID */
