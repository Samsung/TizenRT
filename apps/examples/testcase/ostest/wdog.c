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
 * apps/examples/testcase/ostest/wdog.c
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
#include <tinyara/wdog.h>
#include <tinyara/spinlock.h>

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <syslog.h>
#include <unistd.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define WDOGTEST_RAND_ITER           1024
#define WDOGTEST_THREAD_NR           (CONFIG_SMP_NCPUS * 4)
#define WDOGTEST_TOLERENT_TICK       10

#define wdtest_assert(x)             _ASSERT(x, __FILE__, __LINE__)

#define wdtest_printf(...)           printf(__VA_ARGS__)

#define wdtest_delay(delay_ns)       usleep(delay_ns / 1000 + 1)

/****************************************************************************
 * Private Type
 ****************************************************************************/

typedef struct wdtest_param_s
{
  FAR struct wdog_s *wdog;
  clock_t            interval;
  uint64_t           callback_cnt;
  clock_t            triggered_tick;
} wdtest_param_t;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_FLAT
static void wdtest_callback(wdparm_t param)
{
  FAR wdtest_param_t *wdtest_param = (FAR wdtest_param_t *)param;

  /* Record the system tick at which the callback was triggered */

  wdtest_param->triggered_tick  = clock_systime_ticks();

  /* Increment the callback count */

  wdtest_param->callback_cnt   += 1;
}

static void wdtest_checkdelay(clock_t diff, clock_t delay_tick)
{
  /* Ensure the watchdog trigger time is not earlier than expected. */

  wdtest_assert(diff - delay_tick >= 0);

  /* If the timer latency exceeds the tolerance, print a warning. */

  if (diff - delay_tick > WDOGTEST_TOLERENT_TICK)
    {
      wdtest_printf("WARNING: wdog latency ticks %lld "
                    "(> %u may indicate timing error)\n",
                    (long long)diff - delay_tick,
                    WDOGTEST_TOLERENT_TICK);
    }
}

static void wdtest_once(FAR struct wdog_s *wdog, FAR wdtest_param_t *param,
                        clock_t delay_ns)
{
  uint64_t   cnt;
  clock_t    diff;
  clock_t    wdset_tick;
  irqstate_t flags;
  clock_t    delay_tick = NSEC2TICK(delay_ns);

  wdtest_printf("wdtest_once %lld ns\n", (long long)delay_ns);

  /* Save the current callback count. */

  cnt        = param->callback_cnt;

  /* Enter a critical section to prevent interruptions. */

  flags = enter_critical_section();

  /* Record the current system tick before setting the watchdog. */

  wdset_tick = clock_systime_ticks();

  wdtest_assert(wd_start(wdog, delay_tick, wdtest_callback,
                         (wdparm_t)param) == OK);

  leave_critical_section(flags);

  /* Wait until the callback is triggered exactly once. */

  while (cnt + 1 != param->callback_cnt)
    {
      wdtest_delay(delay_ns);
    }

  /* Check if the delay is within the acceptable tolerance. */

  diff = param->triggered_tick - wdset_tick;

  wdtest_checkdelay(diff, delay_tick);
}

static void wdtest_rand(FAR struct wdog_s *wdog, FAR wdtest_param_t *param,
                        clock_t rand_ns)
{
  uint64_t   cnt;
  int        idx;
  clock_t    delay_ns;
  clock_t    wdset_tick;
  clock_t    delay_tick;
  clock_t    diff;
  irqstate_t flags = 0;

  /* Perform multiple iterations with random delays. */

  for (idx = 0; idx < WDOGTEST_RAND_ITER; idx++)
    {
      cnt = param->callback_cnt;

      /* Generate a random delay within the specified range. */

      delay_ns = rand() % rand_ns;
      delay_tick = NSEC2TICK(delay_ns);

      /* Enter critical section if the callback count is odd. */

      if (cnt % 2)
        {
          flags = enter_critical_section();
        }

      wdset_tick = clock_systime_ticks();
      wdtest_assert(wd_start(wdog, delay_tick, wdtest_callback,
                             (wdparm_t)param) == 0);
      if (cnt % 2)
        {
          leave_critical_section(flags);
        }

      /* Decide to wait for the callback or cancel the watchdog. */

      if (delay_ns % 2)
        {
          /* Wait for the callback. */

          while (cnt + 1 != param->callback_cnt)
            {
              wdtest_delay(delay_ns);
            }

          /* Check the delay if the callback count is odd. */

          if (cnt % 2)
            {
              diff = param->triggered_tick - wdset_tick;
              wdtest_checkdelay(diff, delay_tick);
            }
        }
      else
        {
          wd_cancel(wdog);
        }
    }
}

static void wdtest_callback_recursive(wdparm_t param)
{
  FAR wdtest_param_t *wdtest_param = (FAR wdtest_param_t *)param;
  clock_t             interval     = wdtest_param->interval;

  wdtest_param->callback_cnt   += 1;
  wdtest_param->triggered_tick  = clock_systime_ticks();

  wd_start(wdtest_param->wdog, interval,
           wdtest_callback_recursive, param);
}

static void wdtest_recursive(FAR struct wdog_s *wdog,
                             FAR wdtest_param_t *param,
                             clock_t delay_ns,
                             unsigned int times)
{
  uint64_t cnt;
  clock_t  wdset_tick;
  irqstate_t flags;

  wdtest_printf("wdtest_recursive %lldns\n", (long long)delay_ns);

  cnt = param->callback_cnt;

  param->wdog = wdog;
  param->interval = NSEC2TICK(delay_ns);

  wdtest_assert(param->interval >= 0);

  flags = enter_critical_section();

  wdset_tick = clock_systime_ticks();

  wdtest_assert(wd_start(param->wdog, param->interval,
                         wdtest_callback_recursive,
                         (wdparm_t)param) == OK);

  leave_critical_section(flags);

  wdtest_delay(times * delay_ns);

  wd_cancel(param->wdog);

  wdtest_printf("recursive wdog triggered %llu times, elapsed tick %lld\n",
                (unsigned long long)(param->callback_cnt - cnt),
                (long long)(param->triggered_tick - wdset_tick));
}

static void wdog_test_run(FAR wdtest_param_t *param)
{
  uint64_t             cnt;
  clock_t              rest;
  clock_t              delay;
  struct wdog_s        test_wdog =
  {
    0
  };

  /* Wrong arguments of the wd_start */

  wdtest_assert(wd_start(NULL, 0, NULL, (wdparm_t)NULL) != OK);
  wdtest_assert(wd_start(NULL, 0, wdtest_callback, (wdparm_t)NULL) != OK);
  wdtest_assert(wd_start(NULL, -1, NULL, (wdparm_t)NULL) != OK);
  wdtest_assert(wd_start(NULL, -1, wdtest_callback, (wdparm_t)NULL) != OK);
  wdtest_assert(wd_start(&test_wdog, 0, NULL, (wdparm_t)NULL) != OK);
  wdtest_assert(wd_start(&test_wdog, -1, NULL, (wdparm_t)NULL) != OK);
  wdtest_assert(wd_start(&test_wdog, -1, wdtest_callback, (wdparm_t)NULL)
                != OK);

  /* Wrong arguments of the wd_cancel */

  wdtest_assert(wd_cancel(NULL) != OK);
  wdtest_assert(wd_cancel(&test_wdog) != OK);

  /* Delay = 0 */

  wdtest_once(&test_wdog, param, 0);

  /* Delay > 0, small */

  wdtest_once(&test_wdog, param, 1);
  wdtest_once(&test_wdog, param, 10);
  wdtest_once(&test_wdog, param, 100);
  wdtest_once(&test_wdog, param, 1000);
  wdtest_once(&test_wdog, param, 10000);

  /* Delay > 0, middle 100us */

  wdtest_once(&test_wdog, param, 100000);
  wdtest_once(&test_wdog, param, 1000000);

  /* Delay > 0, maximum */

  cnt = param->callback_cnt;

  /* Maximum */

  delay = CLOCK_MAX >> 2;
  delay -= 1;
  wdtest_assert(wd_start(&test_wdog, delay,
                         wdtest_callback, (wdparm_t)param) == OK);

  /* Sleep for 1s */

  wdtest_delay(USEC_PER_SEC);

  /* Testing wd_gettime */

  wdtest_assert(wd_gettime(NULL) == 0);
  wdtest_assert(wd_cancel(NULL) != 0);

  /* Ensure watchdog not alarmed */

  wdtest_assert(cnt == param->callback_cnt);

  rest = wd_gettime(&test_wdog);

  wdtest_assert(rest < delay);

  wdtest_assert(wd_cancel(&test_wdog) == 0);

  wdtest_printf("wd_start with maximum delay, cancel OK, rest %lld\n",
                (long long)rest);

  /* Recursive wdog delay from 1000us to 10000us */

  wdtest_recursive(&test_wdog, param, 1000000, 100);
  wdtest_recursive(&test_wdog, param, 10000000, 10);

  /* Random delay ~12us */

  wdtest_rand(&test_wdog, param, 12345);

  /* Finally, cancel the wdog. */

  wd_cancel(&test_wdog);
}

/* Multi threaded */

static FAR void *wdog_test_thread(FAR void *param)
{
  wdog_test_run(param);
  return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void wdog_test(void)
{
  unsigned int   thread_id;
  pthread_attr_t attr;
  pthread_t      pthreads[WDOGTEST_THREAD_NR];
  wdtest_param_t params[WDOGTEST_THREAD_NR] =
    {
      0
    };

  printf("wdog_test start...\n");

  wdtest_assert(pthread_attr_init(&attr) == 0);

  /* Create wdog test thread */

  for (thread_id = 0; thread_id < WDOGTEST_THREAD_NR; thread_id++)
    {
      wdtest_assert(pthread_create(&pthreads[thread_id], &attr,
                                   wdog_test_thread, &params[thread_id])
                                   == 0);
    }

  for (thread_id = 0; thread_id < WDOGTEST_THREAD_NR; thread_id++)
    {
      pthread_join(pthreads[thread_id], NULL);
    }

  wdtest_assert(pthread_attr_destroy(&attr) == 0);

  printf("wdog_test end...\n");
}
#endif
