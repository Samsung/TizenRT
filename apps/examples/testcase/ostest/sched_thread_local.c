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
 * apps/examples/testcase/ostest/sched_thread_local.c
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
#include <tinyara/tls.h>

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ostest.h"

#ifdef CONFIG_SCHED_THREAD_LOCAL

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define INIT_VALUE 6
#define TEST_THREADS 3

/****************************************************************************
 * Private Data
 ****************************************************************************/

__thread short g_tls_short = INIT_VALUE;
__thread int g_tls_int = INIT_VALUE;
__thread long long g_tls_lld = -INIT_VALUE;

long long g_tls_variables[TEST_THREADS][3];

static pthread_barrier_t g_barrier;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void *thread_func(void *parameter)
{
  int id = (int)((intptr_t)parameter);
  short value_short = g_tls_short;
  int value_int = g_tls_int;
  long long value_lld = g_tls_lld;
  int status;

  /* Wait at the g_barrier until all threads are synchronized. */

  status = pthread_barrier_wait(&g_barrier);
  if (status != 0 && status != PTHREAD_BARRIER_SERIAL_THREAD)
    {
      printf("sched_thread_local_test: ERROR thread %d pthread_barrier_wait "
             "failed\n", id);
      ASSERT(false);
    }

  printf("thread_func[%d]: Thread Started\n", id);

  printf("thread_func[%d]: g_tls_short (at 0x%p) initial value = %d\n",
         id, &g_tls_short, value_short);
  printf("thread_func[%d]: g_tls_int (at 0x%p) initial value = %d\n",
         id, &g_tls_int, value_int);
  printf("thread_func[%d]: g_tls_lld (at 0x%p) initial value = %lld\n",
         id, &g_tls_lld, value_lld);

  if (value_short != INIT_VALUE)
    {
      printf("thread_func[%d]: "
             "ERROR value_short value for this thread is different than the "
             "expected initial value (%d): %d\n",
             id, INIT_VALUE, value_short);
      ASSERT(false);
    }

  if (value_int != INIT_VALUE)
    {
      printf("thread_func[%d]: "
             "ERROR value_int value for this thread is different than the "
             "expected initial value (%d): %d\n",
             id, INIT_VALUE, value_int);
      ASSERT(false);
    }

  if (value_lld != -INIT_VALUE)
    {
      printf("thread_func[%d]: "
             "ERROR value_lld value for this thread is different than the "
             "expected initial value (%d): %lld\n",
             id, -INIT_VALUE, value_lld);
      ASSERT(false);
    }

  printf("thread_func[%d]: setting value_short (at 0x%p) to %d\n",
         id, &g_tls_short, value_short + id);

  g_tls_short = value_short + id;
  g_tls_variables[id][0] = g_tls_short;

  printf("thread_func[%d]: setting value_int (at 0x%p) to %d\n",
         id, &g_tls_int, value_int + id);

  g_tls_int = value_int + id;
  g_tls_variables[id][1] = g_tls_int;

  printf("thread_func[%d]: setting value_lld (at 0x%p) to %lld\n",
         id, &g_tls_lld, value_lld - id);

  g_tls_lld = value_lld - id;
  g_tls_variables[id][2] = g_tls_lld;

  printf("thread_func[%d]: Thread done\n", id);
  return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void sched_thread_local_test(void)
{
  pthread_t thread[TEST_THREADS] =
    {
      0
    };

  int i;
  int status;

  printf("sched_thread_local_test: g_tls_int value is: %d\n", g_tls_int);

  /* Create the g_barrier */

  status = pthread_barrier_init(&g_barrier, NULL, TEST_THREADS);
  if (status != OK)
    {
      printf("sched_thread_local_test: pthread_barrier_init failed, "
             "status=%d\n", status);
    }

  for (i = 0; i < TEST_THREADS; i++)
    {
      /* Start three thread instances */

      printf("sched_thread_local_test: Starting waiter thread %d\n", i);

      status = pthread_create(&thread[i], NULL,
                              thread_func, (pthread_addr_t)(intptr_t)i);
      if (status != 0)
        {
          printf("sched_thread_local_test: ERROR: "
                 "Thread %d creation failed: %d\n", i, status);
          ASSERT(false);
        }
    }

  for (i = 0; i < TEST_THREADS; i++)
    {
      if (thread[i] != (pthread_t)0)
        {
          pthread_join(thread[i], NULL);
        }
    }

  for (i = 0; i < TEST_THREADS; i++)
    {
      printf("sched_thread_local_test: "
             "g_tls_variables[thread_%d][g_tls_short] = %lld\n",
             i, g_tls_variables[i][0]);
      if (g_tls_variables[i][0] != INIT_VALUE + i)
        {
          printf("sched_thread_local_test: ERROR: "
                 "g_tls_variables[thread_%d][g_tls_short] = %lld\n",
                 i, g_tls_variables[i][0]);
          ASSERT(false);
        }

      printf("sched_thread_local_test: "
             "g_tls_variables[thread_%d][g_tls_int] = %lld\n",
             i, g_tls_variables[i][1]);
      if (g_tls_variables[i][1] != INIT_VALUE + i)
        {
          printf("sched_thread_local_test: ERROR: "
                 "g_tls_variables[thread_%d][g_tls_int] = %lld\n",
                 i, g_tls_variables[i][1]);
          ASSERT(false);
        }

      printf("sched_thread_local_test: "
             "g_tls_variables[thread_%d][g_tls_lld] = %lld\n",
             i, g_tls_variables[i][2]);
      if (g_tls_variables[i][2] != -INIT_VALUE - i)
        {
          printf("sched_thread_local_test: ERROR: "
                 "g_tls_variables[thread_%d][g_tls_lld] = %lld\n",
                 i, g_tls_variables[i][2]);
          ASSERT(false);
        }
    }

  status = pthread_barrier_destroy(&g_barrier);
  if (status != OK)
    {
      printf("sched_thread_local_test: pthread_barrier_destroy failed, "
             "status=%d\n", status);
    }
}

#endif /* CONFIG_SCHED_THREAD_LOCAL */
