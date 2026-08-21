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
 * apps/examples/testcase/ostest/specific.c
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
#include <pthread.h>
#include <stdio.h>

#include "ostest.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define NLOOPS 32

/****************************************************************************
 * Private Data
 ****************************************************************************/

static pthread_key_t g_pthread_key;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void *thread_func(FAR void *parameter)
{
  FAR void *data;
  int ret;

  /* Set the pthread specific data for the main thread */

  ret = pthread_setspecific(g_pthread_key, (FAR const void *)0xcafebabe);
  if (ret != 0)
    {
      printf("ERROR: pthread_setspecific() failed: %d\n", ret);
      ASSERT(false);
    }

  /* Check the thread-specific data */

  data = pthread_getspecific(g_pthread_key);
  if (data != (FAR void *)0xcafebabe)
    {
      printf("ERROR: pthread_getspecific() failed: Returned %p vs %p\n",
             data, (FAR void *)0xcafebabe);
      ASSERT(false);
    }

  return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void specific_test(void)
{
  FAR void *data;
  pthread_t thread;
#ifdef SDCC
  pthread_addr_t result;
  pthread_attr_t attr;
#endif
  int ret;

  /* Create the pthread key */

  ret = pthread_key_create(&g_pthread_key, NULL);
  if (ret != 0)
    {
      printf("ERROR: pthread_key_create() failed: %d\n", ret);
      ASSERT(false);
    }

  /* Set the pthread specific data for the main thread */

  ret = pthread_setspecific(g_pthread_key, (FAR const void *)0xf00dface);
  if (ret != 0)
    {
      printf("ERROR: pthread_setspecific() failed: %d\n", ret);
      ASSERT(false);
    }

  /* Check the thread-specific data */

  data = pthread_getspecific(g_pthread_key);
  if (data != (FAR void *)0xf00dface)
    {
      printf("ERROR: pthread_getspecific() failed: Returned %p vs %p\n",
             data, (FAR void *)0xf00dface);
      ASSERT(false);
    }

  /* Start a thread */

  printf("Starting thread\n");
#ifdef SDCC
  pthread_attr_init(&attr);
  ret = pthread_create(&thread, &attr, thread_func, (pthread_addr_t)0);
#else
  ret = pthread_create(&thread, NULL, thread_func, (pthread_addr_t)0);
#endif
  if (ret != 0)
    {
      printf("ERROR: pthread_create() failed: %d\n", ret);
      ASSERT(false);
    }

  /* Wait for the thread to terminate */

#ifdef SDCC
  pthread_join(thread, result);
#else
  pthread_join(thread, NULL);
#endif

  /* Re-check the thread-specific data */

  data = pthread_getspecific(g_pthread_key);
  if (data != (FAR void *)0xf00dface)
    {
      printf("ERROR: pthread_getspecific() failed: Returned %p vs %p\n",
             data, (FAR void *)0xf00dface);
      ASSERT(false);
    }

  /* Destroy the pthread key */

  ret = pthread_key_delete(g_pthread_key);
  if (ret != 0)
    {
      printf("ERROR: pthread_key_delete() failed: %d\n", ret);
      ASSERT(false);
    }
}
