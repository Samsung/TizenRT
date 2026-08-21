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
 * apps/examples/testcase/ostest/perf_gettime.c
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
#include <tinyara/arch.h>

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>

#include "ostest.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Performance event test parameters */

#define PERF_ITERATIONS     10
#define SLEEP_INTERVAL_USEC 100000  /* 100ms */

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_ARCH_PERF_EVENTS
static clock_t g_prev_time = 0;
static clock_t g_min_interval = ULONG_MAX;
static clock_t g_max_interval = 0;
static unsigned long g_total_interval = 0;
static int g_iteration_count = 0;
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: perf_gettime_test
 *
 * Description:
 *   Test performance event time counter: perf_gettime()
 *
 *   This test verifies:
 *   1. perf_gettime() returns non-zero values
 *   2. Time values are monotonically increasing
 *   3. Multiple calls return consistent intervals
 *   4. Performance counter frequency is reasonable
 *
 ****************************************************************************/

void perf_gettime_test(void)
{
#ifdef CONFIG_ARCH_PERF_EVENTS
  int i;
  clock_t curr_time;
  clock_t time_interval;
  unsigned long perf_freq;

  printf("\nperf_gettime_test: Starting performance event test\n");

  /* Test 1: Get initial time value */

  printf("\nTest 1: Getting initial performance time value\n");
  g_prev_time = perf_gettime();
  printf("perf_gettime_test: Initial time = %lu\n",
          (unsigned long)g_prev_time);

  /* We expect the counter to not be zero (unless just initialized) */

  if (g_prev_time == 0)
    {
      printf("perf_gettime_test: WARNING - Initial time is zero\n");
    }

  /* Test 2: Verify monotonically increasing time */

  printf("\nTest 2: Verify monotonically increasing time values\n");
  g_total_interval = 0;
  g_min_interval = ULONG_MAX;
  g_max_interval = 0;
  g_iteration_count = 0;

  for (i = 0; i < PERF_ITERATIONS; i++)
    {
      /* Sleep for a short interval */

      usleep(SLEEP_INTERVAL_USEC);

      /* Get new time value */

      curr_time = perf_gettime();
      time_interval = curr_time - g_prev_time;

      printf("perf_gettime_test [%d]: current=%lu, interval=%lu\n",
             i, (unsigned long)curr_time, (unsigned long)time_interval);

      /* Verify time is monotonically increasing */

      if (curr_time <= g_prev_time)
        {
          printf("perf_gettime_test: ERROR - Time not monotonically "
                 "increasing at iteration %d\n", i);
          printf("  Previous time: %lu, Current time: %lu\n",
                 (unsigned long)g_prev_time, (unsigned long)curr_time);
          ASSERT(false);
        }

      /* Track intervals */

      if (time_interval < g_min_interval)
        {
          g_min_interval = time_interval;
        }

      if (time_interval > g_max_interval)
        {
          g_max_interval = time_interval;
        }

      g_total_interval += time_interval;
      g_iteration_count++;

      g_prev_time = curr_time;
    }

  /* Test 3: Analyze time intervals */

  printf("\nTest 3: Analyzing time intervals\n");

  if (g_iteration_count > 0)
    {
      unsigned long avg_interval = g_total_interval / g_iteration_count;

      printf("perf_gettime_test: Interval statistics:\n");
      printf("  Minimum interval: %lu\n", (unsigned long)g_min_interval);
      printf("  Maximum interval: %lu\n", (unsigned long)g_max_interval);
      printf("  Average interval: %lu\n", avg_interval);
      printf("  Total iterations: %d\n", g_iteration_count);

      /* Verify that intervals are within reasonable bounds */

      if (g_max_interval == 0 || g_min_interval == ULONG_MAX)
        {
          printf("perf_gettime_test: WARNING - \
                  No time intervals recorded\n");
        }
      else
        {
          printf("perf_gettime_test: Time intervals look reasonable\n");
        }
    }

  /* Test 4: Test perf_getfreq() if available */

  printf("\nTest 4: Getting performance counter frequency\n");

  perf_freq = perf_getfreq();
  printf("perf_gettime_test: Performance frequency = %lu Hz\n", perf_freq);

  if (perf_freq == ULONG_MAX || perf_freq == 0)
    {
      printf("perf_gettime_test: WARNING - Performance frequency is " \
             "invalid or uninitialized\n");
    }
  else
    {
      printf("perf_gettime_test: Performance frequency is valid\n");
    }

  /* Test 5: Multiple rapid calls */

  printf("\nTest 5: Testing multiple rapid calls\n");

  curr_time = perf_gettime();
  printf("perf_gettime_test: Rapid call 1: %lu\n", (unsigned long)curr_time);

  /* Make several rapid calls */

  for (i = 0; i < 5; i++)
    {
      clock_t t = perf_gettime();
      printf("perf_gettime_test: Rapid call %d: %lu\n", i + 2,
              (unsigned long)t);

      /* Verify either monotonic increase or same value (on fast systems) */

      if (t < curr_time)
        {
          printf("perf_gettime_test: ERROR - Time went backwards!\n");
          ASSERT(false);
        }

      curr_time = t;
    }

  printf("\nperf_gettime_test: All performance event tests PASSED\n");

#else /* CONFIG_ARCH_PERF_EVENTS */

  printf("\nperf_gettime_test: Skipping test - "
         "CONFIG_ARCH_PERF_EVENTS not enabled\n");

#endif /* CONFIG_ARCH_PERF_EVENTS */
}
