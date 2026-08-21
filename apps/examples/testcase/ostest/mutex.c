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
 * apps/examples/testcase/ostest/mutex.c
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
#define NYIELDS 10

/****************************************************************************
 * Private Types
 ****************************************************************************/

typedef struct
{
  pthread_mutex_t *mutex_ptr;
  volatile int *mutex_value;
  int *nloops;
  int *nerrors;
  int thread_id;
} mutex_thread_args_t;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void *mutex_thread_func(FAR void *parameter)
{
  FAR mutex_thread_args_t *args = (FAR mutex_thread_args_t *)parameter;

  int id = args->thread_id;
  int ndx = id - 1;
  int i;
  int local_loops = 0;

  for (local_loops = 0; local_loops < NLOOPS; local_loops++)
    {
      int status = pthread_mutex_lock(args->mutex_ptr);
      if (status != 0)
        {
          printf("ERROR thread %d: pthread_mutex_lock failed, "
                 "status=%d\n", id, status);
          ASSERT(false);
        }

      if (*(args->mutex_value) == 1)
        {
          printf("ERROR thread=%d: "
                 "mutex value should be zero, "
                 "instead mutex value=%d\n",
                 id, *(args->mutex_value));
          ASSERT(false);
          args->nerrors[ndx]++;
        }

      *(args->mutex_value) = 1;
      for (i = 0; i < NYIELDS; i++)
        {
          pthread_yield();
        }

      *(args->mutex_value) = 0;

      status = pthread_mutex_unlock(args->mutex_ptr);
      if (status != 0)
        {
          printf("ERROR thread %d: pthread_mutex_unlock failed, "
                 "status=%d\n", id, status);
          ASSERT(false);
        }
    }

  args->nloops[ndx] = local_loops;
  pthread_exit(NULL);
  return NULL;
}

/****************************************************************************
 * Name: mutex_move_test
 *
 * Description:
 *   Test the mutex move functionality.  This test creates a mutex, moves
 *   it to a new location, and then starts two threads that use the moved
 *   mutex.
 *   POSIX specification does not define the behavior of a mutex that is
 *   moved.  This test is intended to verify that the mutex can be moved,
 *   which is useful for some cases, see the discussion in
 *   https://github.com/rust-lang/rust/pull/138400
 *
 ****************************************************************************/

static void mutex_move_test(void)
{
  pthread_t thread1;
  pthread_t thread2;
  int status;
  pthread_mutex_t initial_mutex;
  pthread_mutex_t moved_mutex;
  volatile int moved_mutex_value = 0;
  int moved_nloops[2] =
    {
      0,
      0
    };

  int moved_nerrors[2] =
    {
      0,
      0
    };

  printf("\nTesting moved mutex\n");

  /* Allocate and initialize first mutex */

  pthread_mutex_init(&initial_mutex, NULL);

  /* Copy the mutex to new location */

  memcpy(&moved_mutex, &initial_mutex, sizeof(pthread_mutex_t));

  /* Destroy the original mutex */

  pthread_mutex_destroy(&initial_mutex);

  mutex_thread_args_t thread1_args;
  mutex_thread_args_t thread2_args;

  thread1_args.mutex_ptr = &moved_mutex;
  thread1_args.mutex_value = &moved_mutex_value;
  thread1_args.nloops = moved_nloops;
  thread1_args.nerrors = moved_nerrors;
  thread1_args.thread_id = 1;

  thread2_args.mutex_ptr = &moved_mutex;
  thread2_args.mutex_value = &moved_mutex_value;
  thread2_args.nloops = moved_nloops;
  thread2_args.nerrors = moved_nerrors;
  thread2_args.thread_id = 2;

  /* Start two threads using the moved mutex */

  printf("Starting moved mutex thread 1\n");
  status = pthread_create(&thread1, NULL, mutex_thread_func,
                         &thread1_args);
  if (status != 0)
    {
      printf("ERROR in moved mutex thread#1 creation\n");
      ASSERT(false);
    }

  printf("Starting moved mutex thread 2\n");
  status = pthread_create(&thread2, NULL, mutex_thread_func,
                         &thread2_args);
  if (status != 0)
    {
      printf("ERROR in moved mutex thread#2 creation\n");
      ASSERT(false);
    }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  pthread_mutex_destroy(&moved_mutex);

  printf("\t\tThread1\tThread2\n");
  printf("\tMoved Loops\t%u\t%u\n", moved_nloops[0], moved_nloops[1]);
  printf("\tMoved Errors\t%u\t%u\n", moved_nerrors[0], moved_nerrors[1]);
}

void mutex_simple_test(void)
{
  pthread_t thread1;
  pthread_t thread2;
  int status;
  mutex_thread_args_t thread1_args;
  mutex_thread_args_t thread2_args;

  pthread_mutex_t mut;
  volatile int my_mutex = 0;
  int nloops[2] =
    {
      0,
      0
    };

  int nerrors[2] =
    {
      0,
      0
    };

  /* Initialize the mutex */

  printf("Initializing mutex\n");
  pthread_mutex_init(&mut, NULL);

  /* Set up thread arguments */

  thread1_args.mutex_ptr = &mut;
  thread1_args.mutex_value = &my_mutex;
  thread1_args.nloops = nloops;
  thread1_args.nerrors = nerrors;
  thread1_args.thread_id = 1;

  thread2_args.mutex_ptr = &mut;
  thread2_args.mutex_value = &my_mutex;
  thread2_args.nloops = nloops;
  thread2_args.nerrors = nerrors;
  thread2_args.thread_id = 2;

  /* Start two thread instances */

  printf("Starting thread 1\n");
  status = pthread_create(&thread1, NULL, mutex_thread_func, &thread1_args);
  if (status != 0)
    {
      printf("ERROR in thread#1 creation\n");
      ASSERT(false);
    }

  printf("Starting thread 2\n");
  status = pthread_create(&thread2, NULL, mutex_thread_func, &thread2_args);
  if (status != 0)
    {
      printf("ERROR in thread#2 creation\n");
      ASSERT(false);
    }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  pthread_mutex_destroy(&mut);

  printf("\t\tThread1\tThread2\n");
  printf("\tLoops\t%u\t%u\n", nloops[0], nloops[1]);
  printf("\tErrors\t%u\t%u\n", nerrors[0], nerrors[1]);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void mutex_test(void)
{
  mutex_simple_test();
  mutex_move_test();
}
