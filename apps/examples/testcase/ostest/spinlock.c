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
 * apps/examples/testcase/ostest/spinlock.c
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
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <inttypes.h>
#include <tinyara/atomic.h>
#include <tinyara/spinlock.h>
#include <tinyara/seqlock.h>

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/

#define MAX_THREAD_NUM (CONFIG_SMP_NCPUS)
#define LOOP_TIMES     (CONFIG_TEST_LOOP_SCALE * 100000)

aligned_data(64) struct spinlock_pub_args_s
{
  FAR void         *lock;
  volatile uint32_t counter;
  atomic_t          barrier;
  uint32_t          thread_num;
};

struct spinlock_thread_args_s
{
  uint64_t delta;
  FAR struct spinlock_pub_args_s *pub;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* Helper functions for timespec calculating */

static inline uint64_t calc_diff(FAR struct timespec *start,
                                    FAR struct timespec *end)
{
  uint64_t diff_sec = end->tv_sec - start->tv_sec;
  long diff_nsec = end->tv_nsec - start->tv_nsec;
  if (diff_nsec < 0)
    {
      diff_sec -= 1;
      diff_nsec += 1000000000L;
    }

  return diff_sec * 1000000000ULL + diff_nsec;
}

#define LOCK_TEST_FUNC(lock_type, lock_func, unlock_func) \
FAR static void * lock_type##_test_thread(FAR void *arg) \
{ \
  struct spinlock_thread_args_s *param = \
                                (struct spinlock_thread_args_s *)arg; \
  irqstate_t flags; \
  struct timespec start; \
  struct timespec end; \
  uint32_t i; \
  FAR struct spinlock_pub_args_s *pub = param->pub; \
  FAR lock_type *l = (FAR lock_type *)pub->lock; \
  atomic_fetch_add(&pub->barrier, 1); \
  while (atomic_read(&pub->barrier) != pub->thread_num) \
    { \
      sched_yield(); \
    } \
  clock_gettime(CLOCK_REALTIME, &start); \
  for (i = 0; i < LOOP_TIMES; i++) \
    { \
      flags = lock_func(l); \
      pub->counter++; \
      unlock_func(l, flags); \
    } \
  clock_gettime(CLOCK_REALTIME, &end); \
  param->delta = calc_diff(&start, &end); \
  return NULL; \
}

LOCK_TEST_FUNC(spinlock_t, spin_lock_irqsave, spin_unlock_irqrestore)
LOCK_TEST_FUNC(rspinlock_t, rspin_lock_irqsave, rspin_unlock_irqrestore)
LOCK_TEST_FUNC(seqcount_t, write_seqlock_irqsave, write_sequnlock_irqrestore)

static inline
void run_test_thread(void *lock, FAR void *(*thread_func)(FAR void *arg),
                     uint32_t thread_num, const char *lock_type)
{
  pthread_t tid[MAX_THREAD_NUM];
  struct spinlock_pub_args_s pub;
  pthread_attr_t attr;
  struct sched_param sparam;
  struct spinlock_thread_args_s param[MAX_THREAD_NUM];
  struct timespec stime;
  struct timespec etime;
  cpu_set_t cpu_set = 1u;
  uint64_t total_ns = 0u;
  int status;
  int i;

  /* Initialize the public parameters. */

  printf("Test type: %s\n", lock_type);

  pub.lock       = lock;
  pub.counter    = 0u;
  pub.thread_num = thread_num;
  atomic_set_release(&pub.barrier, 0u);

  /* Set affinity to CPU0 */

#ifdef CONFIG_SMP
  if (OK != sched_setaffinity(getpid(), sizeof(cpu_set_t), &cpu_set))
    {
      printf("spinlock_test: ERROR: nxsched_set_affinity failed");
      ASSERT(false);
    }
#else
  UNUSED(cpu_set);
#endif

  /* Boost to maximum priority for test threads. */

  status = pthread_attr_init(&attr);
  if (status != 0)
    {
      printf("spinlock_test: ERROR: "
             "pthread_attr_init failed, status=%d\n",  status);
      ASSERT(false);
    }

  sparam.sched_priority = SCHED_PRIORITY_MAX;
  status = pthread_attr_setschedparam(&attr, &sparam);
  if (status != OK)
    {
      printf("spinlock_test: ERROR: "
             "pthread_attr_setschedparam failed, status=%d\n",  status);
      ASSERT(false);
    }

  clock_gettime(CLOCK_REALTIME, &stime);

  /* Create new test threads. */

  for (i = 0; i < thread_num; i++)
    {
      param[i].pub = &pub;
      param[i].delta = 0;

      /* Set affinity */

#ifdef CONFIG_SMP
      cpu_set = 1u << ((i + 1) % CONFIG_SMP_NCPUS);

      status = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t),
                                           &cpu_set);
      if (status != OK)
        {
          printf("spinlock_test: ERROR: "
                 "pthread_attr_setaffinity_np failed, status=%d\n",  status);
          ASSERT(false);
        }
#endif

      status = pthread_create(&tid[i], &attr, thread_func, &param[i]);
      if (status != 0)
        {
          printf("spinlock_test: ERROR: "
                 "pthread_create failed, status=%d\n",  status);
          ASSERT(false);
        }
    }

  for (i = 0; i < thread_num; i++)
    {
      status = pthread_join(tid[i], NULL);
      if (status != 0)
        {
          printf("spinlock_test: ERROR: "
                 "pthread_join failed, status=%d\n",  status);
          ASSERT(false);
        }
    }

  /* Calculate the average throughput. */

  clock_gettime(CLOCK_REALTIME, &etime);
  for (i = 0; i < thread_num; i++)
    {
      total_ns += param[i].delta;
    }

  if (total_ns != 0)
    {
      printf("%s: Test Results:\n", lock_type);
      printf("%s: Final counter: %" PRIu32 "\n", lock_type, pub.counter);
      assert(pub.counter == thread_num * LOOP_TIMES);
      printf("%s: Average throughput : %" PRIu64 " op/s\n", lock_type,
            (uint64_t)NSEC_PER_SEC * LOOP_TIMES * thread_num / total_ns);
      printf("%s: Total execution time: %" PRIu64 " ns\n \n",
            lock_type, calc_diff(&stime, &etime));
    }
  else
    {
      printf("spinlock_test: ERROR: total_ns is 0\n");
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: spinlock_test
 ****************************************************************************/

static void spinlock_test_thread_num(unsigned thread_num)
{
  aligned_data(64) union
    {
      spinlock_t  spinlock;
      rspinlock_t rspinlock;
      seqcount_t  seqcount;
    } lock;

  printf("Start Lock test:\n");
  printf("Thread num: %u, Loop times: %d\n\n", thread_num, LOOP_TIMES);

  spin_lock_init(&lock.spinlock);
  run_test_thread(&lock, spinlock_t_test_thread, thread_num, "spinlock");

  rspin_lock_init(&lock.rspinlock);
  run_test_thread(&lock, rspinlock_t_test_thread, thread_num, "rspinlock");

  seqlock_init(&lock.seqcount);
  run_test_thread(&lock, seqcount_t_test_thread, thread_num, "seqcount");
}

void spinlock_test(void)
{
  unsigned tnr;

  for (tnr = 1; tnr <= MAX_THREAD_NUM; tnr++)
    {
      spinlock_test_thread_num(tnr);
    }
}
