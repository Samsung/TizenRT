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
 * apps/examples/testcase/ostest/smp_call.c
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
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>

#include <tinyara/sched.h>

#if defined(CONFIG_SMP) && defined(CONFIG_BUILD_FLAT)

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct smp_call_data_s g_call_data;
static cpu_set_t g_cpuset;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int smp_call_func(void *arg)
{
  FAR sem_t *psem = arg;

  sem_post(psem);
  return OK;
}

static void wdg_wdentry(wdparm_t arg)
{
  nxsched_smp_call_init(&g_call_data, smp_call_func, (FAR void *)arg);
  nxsched_smp_call_async(g_cpuset, &g_call_data);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void smp_call_test(void)
{
  struct smp_call_data_s call_data;
  sem_t sem;
  int cpucnt;
  int cpu;
  int value;
  int status;
  struct wdog_s wdog =
    {
      0
    };

  printf("smp_call_test: Test start\n");

  sem_init(&sem, 0, 0);
  nxsched_smp_call_init(&call_data, smp_call_func, &sem);

  for (cpu = 0; cpu < CONFIG_SMP_NCPUS; cpu++)
    {
      printf("smp_call_test: Call cpu %d, nowait\n", cpu);

      nxsched_smp_call_single_async(cpu, &call_data);

      status = sem_wait(&sem);
      if (status != 0)
        {
          printf("smp_call_test: Check smp call error\n");
          ASSERT(false);
        }

      printf("smp_call_test: Call cpu %d, wait\n", cpu);

      nxsched_smp_call_single(cpu, smp_call_func, &sem);

      sem_getvalue(&sem, &value);
      if (value != 1)
        {
          printf("smp_call_test: Check smp call wait error\n");
          ASSERT(false);
        }

      nxsem_reset(&sem, 0);
    }

  printf("smp_call_test: Call multi cpu, nowait\n");

  sched_getaffinity(0, sizeof(cpu_set_t), &g_cpuset);
  cpucnt = CPU_COUNT(&g_cpuset);

  nxsched_smp_call_async(g_cpuset, &call_data);

  for (cpu = 0; cpu < cpucnt; cpu++)
    {
      status = sem_wait(&sem);
      if (status != 0)
        {
          printf("smp_call_test: Check smp call error\n");
          ASSERT(false);
        }
    }

  printf("smp_call_test: Call in interrupt, wait\n");

  memset(&wdog, 0, sizeof(wdog));
  wd_start(&wdog, 0, wdg_wdentry, (wdparm_t)&sem);
  for (cpu = 0; cpu < cpucnt; cpu++)
    {
      status = sem_wait(&sem);
      if (status != 0)
        {
          printf("smp_call_test: smp call in interrupt error\n");
          ASSERT(false);
        }
    }

  printf("smp_call_test: Call multi cpu, wait\n");

  nxsched_smp_call(g_cpuset, smp_call_func, &sem);

  sem_getvalue(&sem, &value);
  if (value != cpucnt)
    {
      printf("smp_call_test: Check smp call wait error\n");
      ASSERT(false);
    }

  sem_destroy(&sem);

  printf("smp_call_test: Test success\n");
}
#endif
