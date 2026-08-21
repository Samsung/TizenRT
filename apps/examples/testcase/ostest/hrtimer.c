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
 * apps/examples/testcase/ostest/hrtimer.c
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
#include <tinyara/hrtimer.h>

#include <assert.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>

#include "ostest.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* HRTimer test constants */

#define HRTIMER_TEST_RAND_ITER (1024 * 2)             /* Random delay iterations */
#define HRTIMER_TEST_CSECTION  1024                   /* Critical section iterations */
#define HRTIMER_TEST_THREAD_NR (CONFIG_SMP_NCPUS * 8) /* Test threads */

/* Tolerable latency for timer expiration
 *
 * QEMU is a virtual platform, vCPUs can be preempted by any
 * high priority thread. This can cause the timer to be triggered
 * later than expected. This is especially true on QEMU because it
 * has a lot of overhead. The timer resolution is also less precise
 * than on real hardware. Using a larger latency ensures that tests
 * do not fail due to timing inaccuracies.
 *
 * On real hardware (verified on the a2g-tc397-5v-tft board), this
 * latency can be reduced to less than 5 ns because timers are precise
 * and deterministic.
 */

#define HRTIMER_TEST_TOLERENT_LATENCY (10 * NSEC_PER_MSEC)

/* Helper macro for nanosecond delay */

#define hrtimer_test_ndelay(delay_ns) usleep((delay_ns) / 1000 + 1)

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* Structure for HRTimer test tracking */

typedef struct hrtimer_test_s
{
  struct hrtimer_s  timer;     /* HRTimer instance */
  spinlock_t        lock;      /* Spinlock for thread-safe operations */
  volatile uint64_t timestamp; /* Previous timestamp in nanoseconds */
  volatile uint64_t count;     /* Number of timer expirations */
  uint64_t          period;    /* Expected period between expirations */
  volatile uint8_t  state;     /* Test state (used for synchronization tests) */
} hrtimer_test_t;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: hrtimer_test_callback_oneshot
 *
 * Description:
 *   One-shot timer callback function. This callback is invoked when a
 *   one-shot HRTimer expires. It records the exact timestamp and increments
 *   the expiration counter.
 *
 * Input Parameters:
 *   timer      - Pointer to the HRTimer instance
 *   expired_ns - Time when the timer expired (unused)
 *
 * Returned Value:
 *   0 (oneshot hrtimer)
 *
 ****************************************************************************/

static uint64_t hrtimer_test_callback_oneshot(FAR const hrtimer_t *timer,
                                              uint64_t expired_ns)
{
  FAR hrtimer_test_t *param = (FAR hrtimer_test_t *)timer;

  /* Record the exact timestamp when the callback was triggered */

  param->timestamp = clock_systime_nsec();

  /* Increment the callback count */

  param->count++;

  return 0;
}

/****************************************************************************
 * Name: hrtimer_test_checkdelay
 *
 * Description:
 *   Check timer delay against tolerance threshold. This function verifies
 *   that the actual timer expiration time is within the acceptable
 *   tolerance.
 *   It warns if the delay exceeds the threshold.
 *
 * Input Parameters:
 *   timestamp - Actual expiration timestamp (nanoseconds)
 *   expected  - Expected expiration timestamp (nanoseconds)
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

static void hrtimer_test_checkdelay(uint64_t timestamp, uint64_t expected)
{
  int64_t diff = timestamp - expected;

  /* Ensure the hrtimer trigger time is not earlier than expected. */

  ASSERT(diff >= 0);

  /* If the timer latency exceeds the tolerance, print a warning. */

  if (diff > HRTIMER_TEST_TOLERENT_LATENCY)
    {
      printf("hrtimer_test: [WARNING] hrtimer latency %" PRId64
             " ns is too late (> %u ns)\n", diff,
             (unsigned)HRTIMER_TEST_TOLERENT_LATENCY);
    }
}

/****************************************************************************
 * Name: hrtimer_test_oneshot
 *
 * Description:
 *   Test one-shot HRTimer functionality. This function tests a one-shot
 *   HRTimer by starting it with a specified delay and verifying that it
 *   expires at the expected time.
 *
 * Input Parameters:
 *   param - Test context structure containing timer and state information
 *   delay - Delay in nanoseconds for the one-shot timer
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

static void hrtimer_test_oneshot(FAR hrtimer_test_t *param, uint64_t delay)
{
  uint64_t       count;
  uint64_t       now;
  FAR hrtimer_t *timer = &param->timer;

  printf("hrtimer_test_oneshot: delay = %" PRIu64 " ns\n", delay);

  /* Save the current callback count and system time */

  count = param->count;
  now = clock_systime_nsec();

  /* Start the one-shot timer */

  ASSERT(hrtimer_start(timer, hrtimer_test_callback_oneshot,
                       delay + now, HRTIMER_MODE_ABS) == OK);

  /* Wait until the callback is triggered exactly once */

  while (count + 1 != param->count)
    {
      hrtimer_test_ndelay(delay);
    }

  /* Verify the delay is within acceptable tolerance */

  hrtimer_test_checkdelay(param->timestamp, now + delay);

  /* Cancel the timer to clean up */

  hrtimer_cancel_sync(timer);
}

/****************************************************************************
 * Name: hrtimer_test_maximum
 *
 * Description:
 *   Test HRTimer with maximum delay value. This function tests the behavior
 *   of HRTimer when started with the maximum possible delay (UINT64_MAX
 *   nanoseconds). The timer should not expire within a reasonable test
 *   period.
 *
 * Input Parameters:
 *   param - Test context structure containing timer and state information
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

static void hrtimer_test_maximum(FAR hrtimer_test_t *param)
{
  uint64_t       count;
  uint64_t       rest;
  FAR hrtimer_t *timer = &param->timer;

  printf("hrtimer_test_maximum: testing with UINT64_MAX delay\n");

  count = param->count;

  /* Start the hrtimer with maximum delay */

  ASSERT(hrtimer_start(timer, hrtimer_test_callback_oneshot, UINT64_MAX,
                       HRTIMER_MODE_REL) == OK);

  /* Sleep for at least 100ms to verify timer doesn't expire */

  hrtimer_test_ndelay(USEC_PER_SEC / 100);

  /* Ensure hrtimer has not expired (count should remain unchanged) */

  ASSERT(count == param->count);

  /* Get the remaining time until expiration */

  rest = hrtimer_gettime(timer);

  /* Verify the remaining time is still large (less than UINT64_MAX) */

  ASSERT(rest < UINT64_MAX);

  /* Cancel the timer */

  ASSERT(hrtimer_cancel_sync(timer) == OK);

  printf("hrtimer_test_maximum: remaining time = %" PRIu64 " ns\n", rest);
}

/****************************************************************************
 * Name: hrtimer_test_rand
 *
 * Description:
 *   Test HRTimer with random delays. This function tests HRTimer behavior
 *   with random delay values within a specified range. It performs multiple
 *   iterations, testing both timer expiration and cancellation scenarios.
 *
 * Input Parameters:
 *   param   - Test context structure containing timer and state information
 *   rand_ns - Maximum random delay value in nanoseconds
 *
 ****************************************************************************/

static void hrtimer_test_rand(FAR hrtimer_test_t *param, uint64_t rand_ns)
{
  uint64_t       count;
  uint64_t       now;
  unsigned int   idx;
  uint64_t       delay;
  irqstate_t     flags;
  FAR hrtimer_t *timer = &param->timer;

  printf("hrtimer_test_rand: max delay = %" PRIu64 " ns\n", rand_ns);

  /* Perform multiple iterations with random delays */

  for (idx = 0; idx < HRTIMER_TEST_RAND_ITER; idx++)
    {
      /* Generate a random delay within the specified range */

      delay = rand() % rand_ns;

      ASSERT(timer->func == NULL);

      /* Enter critical section if the callback count is odd */

      count = param->count;

      if (count % 2u)
        {
          flags = up_irq_save();
        }

      now = clock_systime_nsec();
      ASSERT(hrtimer_start(timer, hrtimer_test_callback_oneshot,
                           delay, HRTIMER_MODE_REL) == 0);
      if (count % 2u)
        {
          up_irq_restore(flags);
        }

      /* Decide to wait for the callback or cancel the hrtimer */

      if (delay % 2u)
        {
          /* Wait for the callback */

          while (count + 1u != param->count)
            {
              hrtimer_test_ndelay(delay);
            }

          /* Check the delay if the callback count is odd */

          if (count % 2u)
            {
              hrtimer_test_checkdelay(param->timestamp, now + delay);
            }
        }

      hrtimer_cancel_sync(timer);
      ASSERT(timer->func == NULL);
    }

  hrtimer_cancel_sync(timer);
}

/****************************************************************************
 * Name: hrtimer_test_cancel_callback
 *
 * Description:
 *   Callback function for cancel test. This callback is used in the cancel
 *   test to simulate a long-running callback that can be interrupted by
 *   timer cancellation.
 *
 * Input Parameters:
 *   timer      - Pointer to the HRTimer instance
 *   expired_ns - Time when the timer expired
 *
 * Returned Value:
 *   0 (oneshot hrtimer)
 *
 ****************************************************************************/

static uint64_t hrtimer_test_cancel_callback(FAR const hrtimer_t *timer,
                                             uint64_t expired_ns)
{
  FAR hrtimer_test_t *param = (FAR hrtimer_test_t *)timer;
  FAR spinlock_t      *lock = &param->lock;
  uint64_t            delay = 0;
  irqstate_t          flags = spin_lock_irqsave(lock);

  /* Calculate random sleep delay based on period */

  delay = expired_ns % param->period;

  /* Check if the expiration time matches (prevents duplicate processing) */

  if (expired_ns == timer->expired)
    {
      param->timestamp = clock_systime_nsec();

      /* Increment the callback count */

      param->count++;
    }

  spin_unlock_irqrestore(lock, flags);

  /* Simulate work with random delay */

  up_ndelay(delay);

  return 0;
}

/****************************************************************************
 * Name: hrtimer_test_rand_cancel
 *
 * Description:
 *   Test HRTimer cancellation with random delays. This function tests
 *   HRTimer cancellation behavior with random delay values. It verifies
 *   that timers can be properly cancelled and that callbacks behave
 *   correctly when cancellation occurs.
 *
 * Input Parameters:
 *   param   - Test context structure containing timer and state information
 *   rand_ns - Maximum random delay value in nanoseconds
 *
 ****************************************************************************/

static void hrtimer_test_rand_cancel(FAR hrtimer_test_t *param,
                                     uint64_t rand_ns)
{
  uint64_t       now;
  unsigned int   idx;
  uint64_t     count;
  uint64_t     delay;
  irqstate_t   flags;
  spinlock_t   *lock = &param->lock;

  printf("hrtimer_test_rand_cancel: max delay = %" PRIu64 " ns\n", rand_ns);

  param->period = rand_ns;

  /* Perform multiple iterations with random delays */

  for (idx = 0; idx < HRTIMER_TEST_RAND_ITER; idx++)
    {
      /* Generate a random delay within the specified range */

      delay = rand() % rand_ns;

      flags = spin_lock_irqsave(lock);

      now   = clock_systime_nsec();
      count = param->count;
      ASSERT(hrtimer_start(&param->timer, hrtimer_test_cancel_callback,
                           delay, HRTIMER_MODE_REL) == 0);

      spin_unlock_irqrestore(lock, flags);

      /* Decide to wait for the callback or cancel the hrtimer */

      if (delay % 2u)
        {
          /* Wait for the callback to finish */

          while (param->count != count + 1u)
            {
              hrtimer_test_ndelay(delay);
            }

          hrtimer_test_checkdelay(param->timestamp, now + delay);
        }

      hrtimer_cancel(&param->timer);
    }

  hrtimer_cancel_sync(&param->timer);
}

/****************************************************************************
 * Name: hrtimer_test_callback_period
 *
 * Description:
 *   Periodic timer callback function. This callback is invoked when a
 *   periodic HRTimer expires. It records the timestamp, increments the
 *   count, and returns the interval for the next expiration.
 *
 * Input Parameters:
 *   timer      - Pointer to the HRTimer instance
 *   expired_ns - Time when the timer expired (unused)
 *
 * Returned Value:
 *   The interval in nanoseconds for the next expiration
 *
 ****************************************************************************/

static uint64_t hrtimer_test_callback_period(FAR const hrtimer_t *timer,
                                             uint64_t expired_ns)
{
  FAR hrtimer_test_t *param = (FAR hrtimer_test_t *)timer;
  uint64_t         interval = param->period;

  param->count++;
  param->timestamp = clock_systime_nsec();

  return interval;
}

/****************************************************************************
 * Name: hrtimer_test_period
 *
 * Description:
 *   Test periodic HRTimer functionality. This function tests a periodic
 *   HRTimer by starting it with a specified interval and verifying that
 *   it triggers multiple times as expected.
 *
 * Input Parameters:
 *   param    - Test context structure containing timer and state information
 *   delay_ns - Period interval in nanoseconds
 *   iters    - Expected number of timer expirations
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

static void hrtimer_test_period(FAR hrtimer_test_t *param,
                                uint64_t delay_ns,
                                unsigned int iters)
{
  uint64_t   timestamp;
  uint64_t       count = param->count;
  FAR hrtimer_t *timer = &param->timer;

  printf("hrtimer_test_period: period = %" PRIu64 " ns, iters = %u\n",
         delay_ns, iters);

  param->period = delay_ns;
  ASSERT(param->period > 0);

  timestamp = clock_systime_nsec();

  ASSERT(hrtimer_start(timer, hrtimer_test_callback_period,
                       delay_ns, HRTIMER_MODE_REL) == OK);

  /* Wait for expected number of periods */

  hrtimer_test_ndelay(iters * delay_ns);

  hrtimer_cancel_sync(timer);
  ASSERT(timer->func == NULL);

  /* Report results */

  printf("hrtimer_test_period: triggered %" PRIu64 " times, "
         "elapsed nsec %" PRIu64 "\n", param->count - count,
         param->timestamp - timestamp);

  /* Warn if fewer expirations than expected */

  if (param->count - count < iters)
    {
      printf("hrtimer_test_period: [WARNING] periodical hrtimer "
             "triggered times < %u\n", iters);
    }
}

#ifdef CONFIG_SMP
/****************************************************************************
 * Name: hrtimer_test_callback_crita
 *
 * Description:
 *   Critical section test callback A. This callback is used to test
 *   critical section protection in SMP systems. It verifies that timer
 *   callbacks can safely access shared state.
 *
 * Input Parameters:
 *   timer      - Pointer to the HRTimer instance
 *   expired_ns - Time when the timer expired (unused)
 *
 * Returned Value:
 *   0 (oneshot hrtimer)
 *
 ****************************************************************************/

static uint64_t hrtimer_test_callback_crita(FAR const hrtimer_t *timer,
                                            uint64_t expired_ns)
{
  FAR hrtimer_test_t *param = (FAR hrtimer_test_t *)timer;

  /* Change status if in initial state */

  if (param->state == 0)
    {
      param->state = 1;
      param->count++;
    }

  /* Verify state wasn't changed by another critical section */

  ASSERT(param->state == 1);
  param->state = 0;

  return 0;
}

/****************************************************************************
 * Name: hrtimer_test_callback_critb
 *
 * Description:
 *   Critical section test callback B. This callback is used to test
 *   critical section protection in SMP systems. It verifies that timer
 *   callbacks can safely access shared state.
 *
 * Input Parameters:
 *   timer      - Pointer to the HRTimer instance
 *   expired_ns - Time when the timer expired (unused)
 *
 * Returned Value:
 *   0 (oneshot hrtimer)
 *
 ****************************************************************************/

static uint64_t hrtimer_test_callback_critb(FAR const hrtimer_t *timer,
                                            uint64_t expired_ns)
{
  FAR hrtimer_test_t *param = (FAR hrtimer_test_t *)timer;

  /* Change status if in expected state */

  if (param->state == 1)
    {
      param->state = 0;
      param->count++;
    }

  /* Verify state wasn't changed by another critical section */

  ASSERT(param->state == 0);
  param->state = 1;

  return 0;
}

/****************************************************************************
 * Name: hrtimer_test_callback_critdelay
 *
 * Description:
 *   Critical section delay test callback. This callback simulates a
 *   long-running timer callback with spinlock protection. It's used to
 *   test timer cancellation behavior with in-progress callbacks.
 *
 * Input Parameters:
 *   timer      - Pointer to the HRTimer instance
 *   expired_ns - Time when the timer expired (unused)
 *
 * Returned Value:
 *   300 microseconds (reschedules timer for periodic behavior)
 *
 ****************************************************************************/

static uint64_t hrtimer_test_callback_critdelay(FAR const hrtimer_t *timer,
                                                uint64_t expired_ns)
{
  FAR hrtimer_test_t *param = (FAR hrtimer_test_t *)timer;
  FAR spinlock_t *lock      = &param->lock;
  irqstate_t flags;

  /* Protect count increment with spinlock */

  flags = spin_lock_irqsave(lock);
  param->count++;
  spin_unlock_irqrestore(lock, flags);

  /* Simulate work with 100 microsecond delay */

  up_ndelay(100 * NSEC_PER_USEC);

  /* Return reschedule interval for periodic behavior */

  return 300 * NSEC_PER_USEC;
}

/****************************************************************************
 * Name: hrtimer_test_cancel_sync
 *
 * Description:
 *   Test HRTimer cancel synchronization. This function tests that
 *   hrtimer_cancel_sync properly waits for callback completion before
 *   returning. It verifies that timer callbacks complete execution even
 *   when cancellation is requested.
 *
 * Input Parameters:
 *   param - Test context structure containing timer and state information
 *
 ****************************************************************************/

static void hrtimer_test_cancel_sync(FAR hrtimer_test_t *param)
{
  unsigned int idx = 0;

  ASSERT(!param->timer.func);

  param->count = 0;

  for (idx = 0; idx < HRTIMER_TEST_CSECTION; )
    {
      param->state = 0;
      hrtimer_start(&param->timer, hrtimer_test_callback_crita,
                    0, HRTIMER_MODE_REL);

      hrtimer_cancel_sync(&param->timer);
      param->state = 1;
      hrtimer_start(&param->timer, hrtimer_test_callback_critb,
                    0, HRTIMER_MODE_REL);

      if (++idx % (HRTIMER_TEST_CSECTION / 4) == 0)
        {
          printf("hrtimer_test_cancel_sync: passed %u times\n", idx);
        }

      hrtimer_cancel_sync(&param->timer);
    }
}

/****************************************************************************
 * Name: hrtimer_test_cancel_periodic
 *
 * Description:
 *   Test periodic HRTimer cancellation. This function tests that periodic
 *   timers can be properly cancelled and that they don't restart after
 *   cancellation. It verifies that timer cancellation is effective and
 *   persistent.
 *
 * Input Parameters:
 *   param - Test context structure containing timer and state information
 *
 ****************************************************************************/

static void hrtimer_test_cancel_periodic(FAR hrtimer_test_t *param)
{
  uint64_t       count;
  unsigned int     idx = 0;
  FAR spinlock_t *lock = &param->lock;

  ASSERT(!param->timer.func);

  param->count = 0;

  for (idx = 0; idx < HRTIMER_TEST_CSECTION; idx++)
    {
      irqstate_t flags = spin_lock_irqsave(lock);

      hrtimer_start(&param->timer, hrtimer_test_callback_critdelay,
                    0, HRTIMER_MODE_REL);

      spin_unlock_irqrestore(lock, flags);

      up_ndelay(10000);

      flags = spin_lock_irqsave(lock);

      hrtimer_start(&param->timer, hrtimer_test_callback_critdelay,
                    0, HRTIMER_MODE_REL);

      spin_unlock_irqrestore(lock, flags);

      hrtimer_cancel(&param->timer);

      up_ndelay(10000);

      /* The hrtimer should not restart after cancellation */

      ASSERT(!param->timer.func);

      hrtimer_cancel_sync(&param->timer);
      count = param->count;

      hrtimer_test_ndelay(10000);

      ASSERT(count == param->count);

      if (++idx % (HRTIMER_TEST_CSECTION / 4) == 0)
        {
          printf("hrtimer_test_cancel_periodic: passed %u times, count = %"
                 PRIu64 "\n", idx, param->count);
        }
    }

  hrtimer_cancel_sync(&param->timer);
}
#endif

/****************************************************************************
 * Name: hrtimer_test_thread
 *
 * Description:
 *   HRTimer test thread function. This function runs a comprehensive test
 *   suite for HRTimer functionality. It tests various scenarios including
 *   one-shot timers, periodic timers, random delays, maximum delays, and
 *   cancellation behavior.
 *
 * Input Parameters:
 *   arg - Thread argument (unused)
 *
 * Returned Value:
 *   NULL.
 *
 ****************************************************************************/

static void *hrtimer_test_thread(void *arg)
{
  hrtimer_test_t param =
    {
      0
    };

  hrtimer_init(&param.timer);

  /* Test one-shot timers with various delays */

  /* Delay = 0 */

  hrtimer_test_oneshot(&param, 0u);

  /* 0 < Delay < 10000 */

  hrtimer_test_oneshot(&param, 1u);
  hrtimer_test_oneshot(&param, 10u);
  hrtimer_test_oneshot(&param, 100u);
  hrtimer_test_oneshot(&param, 1000u);
  hrtimer_test_oneshot(&param, 10000u);

  /* 10000 < Delay < 10000000 */

  hrtimer_test_oneshot(&param, 100000u);
  hrtimer_test_oneshot(&param, 1000000u);
  hrtimer_test_oneshot(&param, 10000000u);

#ifdef CONFIG_SMP
  /* Test SMP-specific cancellation behavior */

  hrtimer_test_cancel_sync(&param);
  hrtimer_test_cancel_periodic(&param);
#endif

  /* Test maximum delay handling */

  hrtimer_test_maximum(&param);

  /* Test periodic timer with 1ms period */

  hrtimer_test_period(&param, 1000000u, 128u);

  /* Test random delay and cancel scenarios */

  hrtimer_test_rand(&param, 12345u);
  hrtimer_test_rand_cancel(&param, 67890u);

  return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: hrtimer_test
 *
 * Description:
 *   Entry point for high-resolution timer functional test.
 *
 *   - Initializes a HRTimer
 *   - Starts it with a 500ms relative timeout
 *   - Verifies subsequent expirations occur at 500ms intervals
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void hrtimer_test(void)
{
  struct sched_param sparam;
  unsigned int    thread_id;
  pthread_attr_t       attr;
  pthread_t        pthreads[HRTIMER_TEST_THREAD_NR];

  printf("hrtimer_test start...\n");

  /* Initialize thread attributes */

  ASSERT(pthread_attr_init(&attr) == 0);

  sparam.sched_priority = PTHREAD_DEFAULT_PRIORITY;
  ASSERT(pthread_attr_setschedparam(&attr, &sparam) == 0);

  /* Create multiple test threads */

  for (thread_id = 0; thread_id < HRTIMER_TEST_THREAD_NR; thread_id++)
    {
      ASSERT(pthread_create(&pthreads[thread_id], &attr,
                            hrtimer_test_thread, NULL) == 0);
    }

  /* Wait for all threads to complete */

  for (thread_id = 0; thread_id < HRTIMER_TEST_THREAD_NR; thread_id++)
    {
      pthread_join(pthreads[thread_id], NULL);
    }

  /* Clean up thread attributes */

  ASSERT(pthread_attr_destroy(&attr) == 0);

  printf("hrtimer_test end...\n");
}
