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
 * apps/examples/testcase/ostest/pthread_rwlock.c
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
#include <stdio.h>

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct race_cond_s
{
  sem_t * sem1;
  sem_t * sem2;
  pthread_rwlock_t *rw_lock;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static int g_race_cond_thread_pos;
static sem_t g_sem_thread_started;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static FAR void *race_cond_thread1(FAR void *data)
{
  FAR struct race_cond_s *rc = (FAR struct race_cond_s *)data;
  int status;

  /* Runs 1st */

  if (g_race_cond_thread_pos++ != 0)
    {
      printf("pthread_rwlock: Thread order unexpected. Expected 0, got %d",
              g_race_cond_thread_pos);
    }

  status = pthread_rwlock_wrlock(rc->rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: ERROR Failed to lock for writing\n");
      ASSERT(false);
    }

  sem_post(rc->sem2);
  sem_wait(rc->sem1);

  /* Context Switch -> Runs 3rd */

  if (g_race_cond_thread_pos++ != 2)
    {
      printf("pthread_rwlock: Thread order unexpected. Expected 2, got %d",
              g_race_cond_thread_pos);
    }

  status = pthread_rwlock_unlock(rc->rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR Failed to unlock lock held for writing\n");
      ASSERT(false);
    }

  sem_wait(rc->sem1);
  status = pthread_rwlock_rdlock(rc->rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR Failed to open rwlock for reading. Status: %d\n",
             status);
      ASSERT(false);
    }

  sem_wait(rc->sem1);

  /* Context Switch - Runs 5th */

  if (g_race_cond_thread_pos++ != 4)
    {
      printf("pthread_rwlock: Thread order unexpected. Expected 4, got %d",
              g_race_cond_thread_pos);
    }

  status = pthread_rwlock_unlock(rc->rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR Failed to unlock lock held for writing\n");
      ASSERT(false);
    }

  status = pthread_rwlock_rdlock(rc->rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR Failed to open rwlock for reading. Status: %d\n",
             status);
      ASSERT(false);
    }

  sem_post(rc->sem2);
  sem_wait(rc->sem1);

  /* Context switch - Runs 7th */

  if (g_race_cond_thread_pos++ != 6)
    {
      printf("pthread_rwlock: Thread order unexpected. Expected 6, got %d",
              g_race_cond_thread_pos);
    }

  status = pthread_rwlock_unlock(rc->rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR Failed to unlock lock held for reading. Status: %d\n",
             status);
      ASSERT(false);
    }

  return NULL;
}

static FAR void *race_cond_thread2(FAR void *data)
{
  FAR struct race_cond_s *rc = (FAR struct race_cond_s *)data;
  int status;

  status = sem_wait(rc->sem2);

  /* Runs 2nd */

  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR Failed to wait on semaphore. Status: %d\n", status);
      ASSERT(false);
    }

  if (g_race_cond_thread_pos++ != 1)
    {
      printf("pthread_rwlock: "
             "Thread order unexpected. Expected 1, got %d",
              g_race_cond_thread_pos);
    }

  status = pthread_rwlock_tryrdlock(rc->rw_lock);
  if (status != EBUSY)
    {
      printf("pthread_rwlock: "
             "Opened rw_lock for rd when locked for writing: %d\n", status);
    }

  status = pthread_rwlock_trywrlock(rc->rw_lock);
  if (status != EBUSY)
    {
      printf("pthread_rwlock: "
             "Opened rw_lock for wr when locked for writing: %d\n", status);
    }

  sem_post(rc->sem1);
  status = pthread_rwlock_rdlock(rc->rw_lock);

  /* Context - Switch Runs 4th */

  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR Failed to open rwlock for reading. Status: %d\n",
             status);
      ASSERT(false);
    }

  sem_post(rc->sem1);
  if (g_race_cond_thread_pos++ != 3)
    {
      printf("pthread_rwlock: Thread order unexpected. Expected 3, got %d",
             g_race_cond_thread_pos);
    }

  status = pthread_rwlock_unlock(rc->rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR Failed to unlock lock held for writing\n");
      ASSERT(false);
    }

  sem_post(rc->sem1);
  sem_wait(rc->sem2);

  /* Context switch Runs 6th */

  if (g_race_cond_thread_pos++ != 5)
    {
      printf("pthread_rwlock: Thread order unexpected. Expected 5, got %d",
              g_race_cond_thread_pos);
    }

  sem_post(rc->sem1);
  status = pthread_rwlock_wrlock(rc->rw_lock);

  /* Context switch runs 8th */

  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR Failed to open rwlock for reading. Status: %d\n",
             status);
      ASSERT(false);
    }

  if (g_race_cond_thread_pos++ != 7)
    {
      printf("pthread_rwlock: Thread order unexpected. Expected 7, got %d",
              g_race_cond_thread_pos);
    }

  status = pthread_rwlock_unlock(rc->rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR Failed to unlock lock held for writing. Status: %d\n",
             status);
      ASSERT(false);
    }

  return NULL;
}

static void test_two_threads(void)
{
  pthread_rwlock_t rw_lock;
  struct race_cond_s rc;
  pthread_t thread1;
  pthread_t thread2;
  sem_t sem1;
  sem_t sem2;
  int status;

  status = pthread_rwlock_init(&rw_lock, NULL);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR pthread_rwlock_init failed, status=%d\n", status);
      ASSERT(false);
    }

  status = sem_init(&sem1, 0, 0);
  if (status != 0)
    {
      printf("pthread_rwlock: ERROR sem_init failed, status=%d\n", status);
      ASSERT(false);
    }

  status = sem_init(&sem2, 0, 0);
  if (status != 0)
    {
      printf("pthread_rwlock: ERROR sem_init failed, status=%d\n", status);
      ASSERT(false);
    }

  rc.sem1 = &sem1;
  rc.sem2 = &sem2;
  rc.rw_lock = &rw_lock;

  g_race_cond_thread_pos = 0;
  status = pthread_create(&thread1, NULL, race_cond_thread1, &rc);
  status = pthread_create(&thread2, NULL, race_cond_thread2, &rc);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
}

static FAR void *timeout_thread1(FAR void *data)
{
  FAR struct race_cond_s *rc = (FAR struct race_cond_s *)data;
  int status;

  status = pthread_rwlock_wrlock(rc->rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: ERROR Failed to acquire rw_lock. Status: %d\n",
              status);
      ASSERT(false);
    }

  sem_post(&g_sem_thread_started);
  sem_wait(rc->sem1);

  status = pthread_rwlock_unlock(rc->rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: ERROR Failed to unlock rw_lock. Status: %d\n",
              status);
      ASSERT(false);
    }

  return NULL;
}

static FAR void *timeout_thread2(FAR void *data)
{
  FAR struct race_cond_s *rc = (FAR struct race_cond_s *)data;
  struct timespec time;
  int status;

  pthread_yield();

  status = clock_gettime(CLOCK_REALTIME, &time);
  time.tv_sec += 2;

  status = pthread_rwlock_timedwrlock(rc->rw_lock, &time);
  if (status != ETIMEDOUT)
    {
      printf("pthread_rwlock: "
             "ERROR Failed to properly timeout write lock\n");
      ASSERT(false);
    }

  status = clock_gettime(CLOCK_REALTIME, &time);
  time.tv_sec += 2;

  status = pthread_rwlock_timedrdlock(rc->rw_lock, &time);
  if (status != ETIMEDOUT)
    {
      printf("pthread_rwlock: ERROR Failed to properly timeout rd lock\n");
      ASSERT(false);
    }

  status = clock_gettime(CLOCK_REALTIME, &time);
  time.tv_sec += 2;

  sem_post(rc->sem1);
  status = pthread_rwlock_timedrdlock(rc->rw_lock, &time);
  if (status != 0)
    {
      printf("pthread_rwlock: ERROR Failed to properly acquire rdlock\n");
      ASSERT(false);
    }

  status = pthread_rwlock_unlock(rc->rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: ERROR Failed to release rdlock\n");
      ASSERT(false);
    }

  return NULL;
}

static void test_timeout(void)
{
  pthread_rwlock_t rw_lock;
  struct race_cond_s rc;
  pthread_t thread1;
  pthread_t thread2;
  int status;
  sem_t sem1;
  sem_t sem2;

  status = pthread_rwlock_init(&rw_lock, NULL);
  if (status != 0)
    {
      printf("pthread_rwlock: ERROR pthread_rwlock_init failed, status=%d\n",
              status);
      ASSERT(false);
    }

  status = sem_init(&sem1, 0, 0);
  if (status != 0)
    {
      printf("pthread_rwlock: ERROR sem_init failed, status=%d\n", status);
      ASSERT(false);
    }

  status = sem_init(&sem2, 0, 0);
  if (status != 0)
    {
      printf("pthread_rwlock: ERROR sem_init failed, status=%d\n", status);
      ASSERT(false);
    }

  rc.sem1 = &sem1;
  rc.sem2 = &sem2;
  rc.rw_lock = &rw_lock;

  status = pthread_create(&thread1, NULL, timeout_thread1, &rc);

  status = sem_wait(&g_sem_thread_started);

  status = pthread_create(&thread2, NULL, timeout_thread2, &rc);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void pthread_rwlock_test(void)
{
  pthread_rwlock_t rw_lock;
  int status;

  printf("pthread_rwlock: Initializing rwlock\n");

  sem_init(&g_sem_thread_started, 0, 0);

  status = pthread_rwlock_init(&rw_lock, NULL);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR pthread_rwlock_init failed, status=%d\n",
             status);
      ASSERT(false);
    }

  status = pthread_rwlock_trywrlock(&rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR pthread_rwlock_trywrlock failed, status=%d\n",
             status);
      ASSERT(false);
    }

  status = pthread_rwlock_unlock(&rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR pthread_rwlock_unlock failed, status=%d\n",
             status);
      ASSERT(false);
    }

  status = pthread_rwlock_trywrlock(&rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR pthread_rwlock_trywrlock failed, status=%d\n",
             status);
      ASSERT(false);
    }

  status = pthread_rwlock_trywrlock(&rw_lock);
  if (status != EBUSY)
    {
      printf("pthread_rwlock: ERROR "
            "able to acquire write lock when write lock already acquired\n");
      ASSERT(false);
    }

  status = pthread_rwlock_tryrdlock(&rw_lock);
  if (status != EBUSY)
    {
      printf("pthread_rwlock: ERROR "
             "able to acquire read lock when write lock already acquired\n");
      ASSERT(false);
    }

  status = pthread_rwlock_unlock(&rw_lock);
  if (status != 0)
    {
      printf("pthread_rwlock: "
             "ERROR pthread_rwlock_unlock failed, status=%d\n",
             status);
      ASSERT(false);
    }

  test_two_threads();

  test_timeout();

  sem_destroy(&g_sem_thread_started);
}
