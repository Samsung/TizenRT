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
 * apps/examples/testcase/ostest/restart.c
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
#include <sched.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ostest.h"

#ifndef CONFIG_BUILD_KERNEL

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define PRIORITY    100
#define NARGS         3

/****************************************************************************
 * Private Data
 ****************************************************************************/

static char * const g_argv[NARGS + 1] =
{
  "This is argument 1",
  "Argument 2 here",
  "Lastly, the 3rd argument",
  NULL
};

#ifndef CONFIG_DISABLE_ENVIRON
static const char g_varname[] = "VarName";
static const char g_varvalue[] = "VarValue";
#endif

static uint8_t g_restartstep;

static sem_t g_sem;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int restart_main(int argc, char *argv[])
{
#ifndef CONFIG_DISABLE_ENVIRON
  char *actual;
#endif
  int i;

  printf("restart_main: Started with argc=%d\n", argc);

  /* Verify passed arguments */

  if (argc != NARGS + 1)
    {
      printf("restart_main: ERROR: Expected argc=%d got argc=%d\n",
             NARGS + 1, argc);
      ASSERT(false);
    }

  for (i = 0; i <= NARGS; i++)
    {
      printf("restart_main: argv[%d]=\"%s\"\n", i, argv[i]);
      if (i > 0 && strcmp(argv[i], g_argv[i - 1]) != 0)
        {
          printf("restart_main: ERROR: "
                 "Expected argv[%d]=\"%s\" got \"%s\"\n",
                 i, argv[i], g_argv[i - 1]);
          ASSERT(false);
        }
    }

#ifndef CONFIG_DISABLE_ENVIRON
  actual = getenv(g_varname);
  if (actual)
    {
      if (strcmp(actual, g_varvalue) == 0)
        {
          printf("restart_main: Variable=%s has value=%s\n", g_varname,
                 g_varvalue);
        }
      else
        {
          printf("restart_main: ERROR Variable=%s has the wrong value\n",
                 g_varname);
          printf("restart_main:       found=%s expected=%s\n",
                 actual, g_varvalue);
          ASSERT(false);
        }
    }
  else
    {
      printf("restart_main: ERROR: Variable=%s has no value\n", g_varname);
      ASSERT(false);
    }
#endif

  /* Were we restarted? */

  switch (g_restartstep)
    {
      case 0:
        for (; ; )
          {
            sleep(2);
            printf("restart_main: I am still here\n");
          }
        break;
      case 1:
        if (sem_wait(&g_sem) != 0)
          {
            printf("restart_main: ERROR thread sem_wait failed\n");
            ASSERT(false);
          }
        break;
      default:
        break;
    }

  return 0; /* Won't get here unless we were restarted */
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void restart_test(void)
{
  int ret;

  g_restartstep = 0;

  /* Start the children and wait for first one to complete */

  printf("\nTest task_restart()\n");

  /* Set up an environment variables */

#ifndef CONFIG_DISABLE_ENVIRON
  printf("restart_main: setenv(%s, %s, TRUE)\n", g_varname, g_varvalue);
  setenv(g_varname, g_varvalue, TRUE);  /* Variable1=GoodValue1 */
#endif

  /* Initialize global variables */

  g_restartstep = 0;
  sem_init(&g_sem, 0, 0);

  /* Start the task */

  ret = task_create("ostest", PRIORITY, STACKSIZE, restart_main, g_argv);
  if (ret < 0)
    {
      printf("restart_main: ERROR Failed to start restart_main\n");
      ASSERT(false);
    }
  else
    {
      pid_t pid = ret;

      printf("restart_main: Started restart_main at PID=%d\n", pid);

      /* Wait a bit and restart the task */

      sleep(5);

      g_restartstep = 1;

      ret = task_restart(pid);
      if (ret < 0)
        {
          printf("restart_main:  ERROR: task_restart failed\n");
          ASSERT(false);
        }

      /* Start the task wait for a semaphore */

      printf("restart_main: Started restart_main at PID=%d\n", pid);

      /* Wait a bit and restart the task */

      sleep(5);

      g_restartstep = 2;

      ret = task_restart(pid);
      if (ret < 0)
        {
          printf("restart_main:  ERROR: task_restart failed\n");
          ASSERT(false);
        }

      sleep(1);
    }

  sem_destroy(&g_sem);

  printf("restart_main: Exiting\n");
}

#endif /* !CONFIG_BUILD_KERNEL */
