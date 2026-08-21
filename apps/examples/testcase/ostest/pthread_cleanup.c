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
 * apps/examples/testcase/ostest/pthread_cleanup.c
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
#include <unistd.h>

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct sync_s
{
  pthread_cond_t cond;
  pthread_mutex_t lock;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void cleanup(FAR void *data)
{
  FAR struct sync_s *sync = (FAR struct sync_s *)data;
  int status;

  /* Note:  The behavior of canceling pthread_cond_wait() with asynchronous
   * cancellation is not defined. On NuttX we get EPERM here, but application
   * code must not rely on this.
   */

  status = pthread_mutex_unlock(&sync->lock);
#ifndef CONFIG_CANCELLATION_POINTS
  if (status == EPERM)
    {
      printf("pthread_cleanup: thread did not have mutex locked: %d\n",
              status);
      return;
    }
#endif

  if (status != 0)
    {
      printf("pthread_cleanup:"
             "ERROR pthread_mutex_unlock in cleanup handler. "
             "Status: %d\n", status);
      ASSERT(false);
    }
}

static void *cleanup_thread(FAR void *data)
{
  FAR struct sync_s *sync = (FAR struct sync_s *)data;
  int status;

  status = pthread_mutex_lock(&sync->lock);
  if (status != 0)
    {
      printf("pthread_cleanup: ERROR pthread_mutex_lock, status=%d\n",
              status);
      ASSERT(false);
      return NULL;
    }

  pthread_cleanup_push(&cleanup, sync);

  while (1)
    {
      status = pthread_cond_wait(&sync->cond, &sync->lock);
      if (status != 0)
        {
          printf("pthread_cleanup: ERROR wait returned. Status: %d\n",
                 status);
          ASSERT(false);
        }
    }

  pthread_cleanup_pop(1);
  return NULL;
}

static void test_cleanup(void)
{
  pthread_t thread1;
  int status;
  void *result;
  struct sync_s sync = {
    PTHREAD_COND_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
  };

  status = pthread_create(&thread1, NULL, cleanup_thread, &sync);
  if (status != 0)
    {
      printf("pthread_cleanup: ERROR pthread_create, status=%d\n", status);
      ASSERT(false);
      return;
    }

  usleep(500 * 1000);

  status = pthread_cancel(thread1);
  if (status != 0)
    {
      printf("pthread_cleanup: ERROR pthread_cancel, status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_join(thread1, &result);
  if (status != 0)
    {
      printf("pthread_cleanup: ERROR pthread_join, status=%d\n", status);
      ASSERT(false);
    }
  else if (result != PTHREAD_CANCELED)
    {
      printf("pthread_cleanup: "
             "ERROR pthread_join returned wrong result: %p\n", result);
      ASSERT(false);
    }

#ifdef CONFIG_CANCELLATION_POINTS
  /* Do some operations on lock in order to check if it is in usable state. */

  status = pthread_mutex_trylock(&sync.lock);
  if (status != 0)
    {
      printf("pthread_cleanup: ERROR pthread_mutex_trylock, status=%d\n",
              status);
      ASSERT(false);
    }

  status = pthread_mutex_unlock(&sync.lock);
  if (status != 0)
    {
      printf("pthread_cleanup: ERROR pthread_mutex_unlock, status=%d\n",
              status);
      ASSERT(false);
    }
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void pthread_cleanup_test(void)
{
  printf("pthread_cleanup: Starting test\n");
  test_cleanup();
}
