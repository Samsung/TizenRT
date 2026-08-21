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
 * apps/examples/testcase/ostest/pthread_rwlock_cancel.c
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
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct sync_s
{
  pthread_rwlock_t *read_lock;
  pthread_rwlock_t *write_lock;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static FAR void *timeout_thread1(FAR void *data)
{
  FAR struct sync_s *sync = (FAR struct sync_s *)data;
  struct timespec time;
  int status;

  while (1)
    {
      clock_gettime(CLOCK_REALTIME, &time);
      time.tv_sec += 1;

      status = pthread_rwlock_timedrdlock(sync->write_lock, &time);
      if (status != ETIMEDOUT)
        {
          printf("pthread_rwlock_cancel: "
                 "ERROR Acquired held write_lock. Status: %d\n", status);
          ASSERT(false);
        }
    }

  return NULL;
}

static FAR void *timeout_thread2(FAR void *data)
{
  FAR struct sync_s *sync = (FAR struct sync_s *)data;
  struct timespec time;
  int status;

  while (1)
    {
      clock_gettime(CLOCK_REALTIME, &time);
      time.tv_sec += 1;

      status = pthread_rwlock_timedrdlock(sync->read_lock, &time);
      if (status != 0)
        {
          printf("pthread_rwlock_cancel: "
                 "ERROR Failed to acquire read_lock. Status: %d\n", status);
          ASSERT(false);
        }

      sched_yield(); /* Not a cancellation point. */

      if (status == 0)
        {
          status = pthread_rwlock_unlock(sync->read_lock);
          if (status != 0)
            {
              printf("pthread_rwlock_cancel: "
                     "ERROR Failed to release read_lock. Status: %d\n",
                     status);
              ASSERT(false);
            }
        }

      clock_gettime(CLOCK_REALTIME, &time);
      time.tv_sec += 1;

      status = pthread_rwlock_timedwrlock(sync->read_lock, &time);
      if (status != ETIMEDOUT)
        {
          printf("pthread_rwlock_cancel: "
                 "ERROR Acquired held read_lock for writing."
                 " Status: %d\n", status);
          ASSERT(false);
        }
    }

  return NULL;
}

static void test_timeout(void)
{
  pthread_rwlock_t read_lock;
  pthread_rwlock_t write_lock;
  struct sync_s sync;
  pthread_t thread1;
  pthread_t thread2;
  int status;
  int i;

  status = pthread_rwlock_init(&read_lock, NULL);
  if (status != 0)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR pthread_rwlock_init(read_lock), status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_rwlock_init(&write_lock, NULL);
  if (status != 0)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR pthread_rwlock_init(write_lock), status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_rwlock_rdlock(&read_lock);
  if (status != 0)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR pthread_rwlock_rdlock, status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_rwlock_wrlock(&write_lock);
  if (status != 0)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR pthread_rwlock_wrlock, status=%d\n", status);
      ASSERT(false);
    }

  sync.read_lock = &read_lock;
  sync.write_lock = &write_lock;

  status = pthread_create(&thread1, NULL, timeout_thread1, &sync);
  if (status != 0)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR pthread_create, status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_create(&thread2, NULL, timeout_thread2, &sync);
  if (status != 0)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR pthread_create, status=%d\n", status);
      ASSERT(false);
    }

  for (i = 0; i < 10; i++)
    {
      usleep(300 * 1000); /* Give threads few seconds to run */
    }

  status = pthread_cancel(thread1);
  if (status != 0)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR pthread_cancel, status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_cancel(thread2);
  if (status != 0)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR pthread_cancel, status=%d\n", status);
      ASSERT(false);
    }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  /* Do some operations on locks in order to check if they are still in
   * usable state after deferred cancellation.
   */

#if CONFIG_TLS_NCLEANUP > 0 && defined(CONFIG_CANCELLATION_POINTS)
  status = pthread_rwlock_trywrlock(&write_lock);
  if (status != EBUSY)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR able to acquire write lock when write lock already "
             "acquired, status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_rwlock_tryrdlock(&write_lock);
  if (status != EBUSY)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR able to acquire read lock when write lock already "
             "acquired, status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_rwlock_unlock(&read_lock);
  if (status != 0)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR pthread_rwlock_unlock, status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_rwlock_unlock(&write_lock);
  if (status != 0)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR pthread_rwlock_unlock, status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_rwlock_rdlock(&read_lock);
  if (status != 0)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR pthread_rwlock_rdlock, status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_rwlock_wrlock(&write_lock);
  if (status != 0)
    {
      printf("pthread_rwlock_cancel: "
             "ERROR pthread_rwlock_wrlock, status=%d\n", status);
      ASSERT(false);
    }
#endif /* CONFIG_TLS_NCLEANUP > 0 && CONFIG_CANCELLATION_POINTS */

    pthread_rwlock_destroy(&write_lock);
    pthread_rwlock_destroy(&read_lock);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void pthread_rwlock_cancel_test(void)
{
  printf("pthread_rwlock_cancel: Starting test\n");
  test_timeout();
}
