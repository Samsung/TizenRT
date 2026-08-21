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
 * apps/examples/testcase/ostest/cancel.c
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

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#include "ostest.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

static pthread_mutex_t mutex;
static pthread_cond_t  cond;
static sem_t sem_thread_started;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#if CONFIG_TLS_NCLEANUP > 0
static void sem_cleaner(FAR void *arg)
{
  printf("sem_cleaner #%u\n", (unsigned int)((uintptr_t)arg));
}
#endif

static FAR void *sem_waiter(FAR void *parameter)
{
  int status;

#if CONFIG_TLS_NCLEANUP > 0
  int i;

  /* Register some clean-up handlers */

  for (i = 0; i < CONFIG_TLS_NCLEANUP ; i++)
    {
      pthread_cleanup_push(sem_cleaner, (FAR void *)((uintptr_t)(i + 1)));
    }
#endif

  /* Take the mutex */

  printf("sem_waiter: Taking mutex\n");
  status = pthread_mutex_lock(&mutex);
  if (status != 0)
    {
       printf("sem_waiter: ERROR pthread_mutex_lock failed, status=%d\n",
               status);
       ASSERT(false);
    }

  sem_post(&sem_thread_started);
  printf("sem_waiter: Starting wait for condition\n");

  /* Are we a non-cancelable thread?   Yes, set the non-cancelable state */

  if (!parameter)
    {
      printf("sem_waiter: Setting non-cancelable\n");
      status = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
      if (status != 0)
        {
           printf("sem_waiter: "
                  "ERROR pthread_setcancelstate failed, status=%d\n",
                   status);
           ASSERT(false);
        }
    }

  /* Then wait -- we will never awaken from this normally. We will wake
   * either by signal or cancellation.
   */

  status = pthread_cond_wait(&cond, &mutex);
  if (status != 0)
    {
      printf("sem_waiter: ERROR pthread_cond_wait failed, status=%d\n",
             status);
      ASSERT(false);
    }

  if (!parameter)
    {
      /* Release the mutex */

      printf("sem_waiter: Releasing mutex\n");
      status = pthread_mutex_unlock(&mutex);
      if (status != 0)
        {
          printf("sem_waiter: "
                 "ERROR pthread_mutex_unlock failed, status=%d\n",
                  status);
          ASSERT(false);
        }

      /* Set the cancelable state */

      printf("sem_waiter: Setting cancelable\n");
      status = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
      if (status != 0)
        {
          printf("sem_waiter: "
                 "ERROR pthread_setcancelstate failed, status=%d\n", status);
          ASSERT(false);
        }
    }
  else
    {
      printf("sem_waiter: "
             "ERROR pthread_cond_wait returned after being cancelled!\n");
      ASSERT(false);
    }

  /* Why is this here?  Because pthread_setcancelstate() is not a
   * cancellation point and printf() may or may not be a cancellation point
   * (OpenGroup.org allows printf to optionally be a cancellation point).
   * If printf() is not a cancellation point, then the thread will never
   * be cancelled in the deferred cancellation mode and will instead exit
   * with no error.  The pthread_testcancel() is needed to correctly test
   * that cancellation has been re-enabled.
   */

  pthread_testcancel();

  printf("sem_waiter: Exit with status 0x12345678\n");
  pthread_exit((pthread_addr_t)0x12345678);
  return NULL;
}

#if !defined(CONFIG_DISABLE_MQUEUE) && defined(CONFIG_CANCELLATION_POINTS)
#if CONFIG_TLS_NCLEANUP > 0
static void mqueue_cleaner(FAR void *arg)
{
  FAR mqd_t *mqcancel = (FAR mqd_t *)arg;

  printf("mqueue_cleaner... closing message queue\n");
  if (mq_close(*mqcancel) < 0)
    {
      printf("mqueue_cleaner: ERROR mq_close failed\n");
      ASSERT(false);
    }
}
#endif

static FAR void *mqueue_waiter(FAR void *parameter)
{
  mqd_t mqcancel;
  struct mq_attr attr;
  char msgbuffer[CONFIG_MQ_MAXMSGSIZE];
  size_t nbytes;

#if CONFIG_TLS_NCLEANUP > 0
  /* Register clean-up handler */

  pthread_cleanup_push(mqueue_cleaner, (FAR void *)&mqcancel);
#endif

  /* Open the message queue for reading */

  attr.mq_maxmsg  = 20;
  attr.mq_msgsize = CONFIG_MQ_MAXMSGSIZE;
  attr.mq_flags   = 0;

  mqcancel = mq_open("mqcancel", O_RDONLY | O_CREAT, 0666, &attr);
  if (mqcancel < 0)
    {
      printf("mqueue_waiter: ERROR mq_open failed\n");
      ASSERT(false);
      pthread_exit((pthread_addr_t)0xdeadbeef);
    }

  /* Now wait for the message that will never come */

  printf("mqueue_waiter: Waiting to receive a message...\n");
  nbytes = mq_receive(mqcancel, msgbuffer, CONFIG_MQ_MAXMSGSIZE, 0);

  pthread_testcancel();
  printf("mqueue_waiter: Awakened with %d\n", (int)nbytes);

  if (mq_close(mqcancel) < 0)
    {
      printf("mqueue_waiter: ERROR mq_close failed\n");
      ASSERT(false);
    }

  printf("mqueue_waiter: Exit with status 0x12345678\n");
  pthread_exit((pthread_addr_t)0x12345678);
  return NULL;
}
#endif

#ifdef CONFIG_CANCELLATION_POINTS
static FAR void *sig_waiter(FAR void *parameter)
{
  struct siginfo info;
  sigset_t set;
  int ret;

  /* Wait for a signal that will never be delivered */

  printf("sig_waiter: Waiting to receive signal...\n");

  sigemptyset(&set);
  ret = sigwaitinfo(&set, &info);

  pthread_testcancel();
  printf("sig_waiter: Awakened with %d\n", ret);
  printf("sig_waiter: Exit with status 0x12345678\n");
  pthread_exit((pthread_addr_t)0x12345678);
  return NULL;
}
#endif

#ifdef CONFIG_CANCELLATION_POINTS
static FAR void *asynch_waiter(FAR void *parameter)
{
  int status;

#if CONFIG_TLS_NCLEANUP > 0
  int i;

  /* Register some clean-up handlers */

  for (i = 0; i < CONFIG_TLS_NCLEANUP ; i++)
    {
      pthread_cleanup_push(sem_cleaner,
                          (FAR void *)((uintptr_t)(i + 1)));
    }
#endif

  /* Set the non-cancelable state */

  printf("asynch_waiter: Setting non-cancelable\n");
  status = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
  if (status != 0)
    {
       printf("asynch_waiter: "
              "ERROR pthread_setcancelstate failed, status=%d\n",
               status);
       ASSERT(false);
    }

  /* Set the asynchronous cancellation type */

  printf("asynch_waiter: Setting synchronous cancellation type\n");
  status = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  if (status != 0)
    {
       printf("asynch_waiter: "
              "ERROR pthread_setcanceltype failed, status=%d\n",
               status);
       ASSERT(false);
    }

  /* Then wait a bit.  We should be canceled aynchronously while waiting, but
   * the cancellation should pend because we are non-cancellable.
   */

  usleep(250 * 1000);

  /* We should be canceled when restore the cancelable state. */

  printf("asynch_waiter: Restoring cancelable state\n");

  printf("asynch_waiter: Setting cancelable\n");
  status = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

  /* We should not get here!!! */

  if (status != 0)
    {
      printf("asynch_waiter: "
             "ERROR pthread_setcancelstate failed, status=%d\n",
             status);
      ASSERT(false);
    }

  /* Set the cancelable state */

  printf("asynch_waiter: Exit with status 0x12345678\n");
  pthread_exit((pthread_addr_t)0x12345678);
  return NULL;
}
#endif

static void start_thread(FAR void *(*entry)(FAR void *), pthread_t *waiter,
                        int cancelable)
{
  pthread_attr_t attr;
  int status;

  /* Initialize the mutex */

  printf("start_thread: Initializing mutex\n");
  status = pthread_mutex_init(&mutex, NULL);
  if (status != 0)
    {
      printf("start_thread: "
             "ERROR pthread_mutex_init failed, status=%d\n", status);
      ASSERT(false);
    }

  /* Initialize the condition variable */

  printf("start_thread: Initializing cond\n");
  status = pthread_cond_init(&cond, NULL);
  if (status != 0)
    {
      printf("start_thread: ERROR pthread_cond_init failed, status=%d\n",
              status);
      ASSERT(false);
    }

  /* Set up attributes */

  status = pthread_attr_init(&attr);
  if (status != 0)
    {
      printf("start_thread: "
             "ERROR pthread_attr_init failed, status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_attr_setstacksize(&attr, STACKSIZE);
  if (status != 0)
    {
      printf("start_thread: "
             "ERROR pthread_attr_setstacksize failed, status=%d\n",
              status);
      ASSERT(false);
    }

  /* Start the waiter thread  */

  printf("start_thread: Starting thread\n");
  status = pthread_create(waiter, &attr, entry,
                          (pthread_addr_t)((uintptr_t)cancelable));
  if (status != 0)
    {
      printf("start_thread: ERROR pthread_create failed, status=%d\n",
              status);
      ASSERT(false);
    }

  /* Make sure that the waiter thread gets a chance to run */

  printf("start_thread: Yielding\n");
  pthread_yield();
}

static void restart_thread(FAR void *(*entry)(FAR void *), pthread_t *waiter,
                           int cancelable)
{
  int status;

  /* Destroy the condition variable */

  printf("restart_thread: Destroying cond\n");
  status = pthread_cond_destroy(&cond);
  if (status != 0)
    {
      printf("restart_thread: "
             "ERROR pthread_cond_destroy failed, status=%d\n", status);
      ASSERT(false);
    }

  /* Destroy the mutex.
   * Note that this relies on non-portable NuttX assumption, that it
   * is possible to destroy a locked mutex owned by a cancelled thread.
   */

  printf("restart_thread: Destroying mutex\n");
  status = pthread_mutex_destroy(&mutex);
  if (status != 0)
    {
      printf("restart_thread: "
             "ERROR pthread_mutex_destroy failed, status=%d\n", status);
      ASSERT(false);
    }

  /* Then restart the thread */

  printf("restart_thread: Re-starting thread\n");
  start_thread(entry, waiter, cancelable);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void cancel_test(void)
{
#if !defined(CONFIG_DISABLE_MQUEUE) && defined(CONFIG_CANCELLATION_POINTS)
  struct mq_attr attr;
  mqd_t mqcancel;
#endif
  pthread_t waiter;
  void *result;
  int status;

  sem_init(&sem_thread_started, 0, 0);

  /* Test 1: Normal Cancel **************************************************/

  /* Start the waiter thread  */

  printf("cancel_test: Test 1a: Normal Cancellation\n");
  printf("cancel_test: Starting thread\n");
  start_thread(sem_waiter, &waiter, 1);

  /* Then cancel it.  It should be in the usleep now -- wait bit to
   * make sure.
   */

  sem_wait(&sem_thread_started);

  /* Make sure sem_waiter run into pthread_cond_wait */

  pthread_mutex_lock(&mutex);

  pthread_mutex_unlock(&mutex);

  printf("cancel_test: Canceling thread\n");
  status = pthread_cancel(waiter);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_cancel failed, status=%d\n",
              status);
      ASSERT(false);
    }

  /* Then join to the thread to pick up the result (if we don't do
   * we will have a memory leak!)
   */

  printf("cancel_test: Joining\n");
  status = pthread_join(waiter, &result);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_join failed, status=%d\n",
              status);
      ASSERT(false);
    }
  else
    {
      printf("cancel_test: waiter exited with result=%p\n", result);
      if (result != PTHREAD_CANCELED)
        {
          printf("cancel_test: ERROR expected result=%p\n",
                  PTHREAD_CANCELED);
          ASSERT(false);
        }
      else
        {
          printf("cancel_test:"
                 " PASS thread terminated with PTHREAD_CANCELED\n");
        }
    }

  /* Test 2: Asynchronous Cancel ********************************************/

  printf("cancel_test: Test 2: Asynchronous Cancellation\n");

#ifdef CONFIG_CANCELLATION_POINTS
  /* If cancellation points were enabled, then the first test was done
   * in deferred mode.  Do it again it asynchronous mode.
   *
   * This test does not really test asynchronous cancellation (which is
   * inherently dangerous), but does exercides pthread_setcanceltype().
   */

  /* Start the waiter thread  */

  printf("cancel_test: Starting thread\n");
  restart_thread(asynch_waiter, &waiter, 1);

  /* Then cancel it.  It should be in the pthread_cond_wait now -- wait
   * bit to make sure.
   */

  usleep(100 * 1000);

  printf("cancel_test: Canceling thread\n");
  status = pthread_cancel(waiter);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_cancel failed, status=%d\n",
              status);
      ASSERT(false);
    }

  /* Then join to the thread to pick up the result (if we don't do
   * we will have a memory leak!)
   */

  printf("cancel_test: Joining\n");
  status = pthread_join(waiter, &result);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_join failed, status=%d\n",
              status);
      ASSERT(false);
    }
  else
    {
      printf("cancel_test: waiter exited with result=%p\n", result);
      if (result != PTHREAD_CANCELED)
        {
          printf("cancel_test: ERROR expected result=%p\n",
                  PTHREAD_CANCELED);
          ASSERT(false);
        }
      else
        {
          printf("cancel_test: "
                 "PASS thread terminated with PTHREAD_CANCELED\n");
        }
    }
#else
  printf("... Skipped\n");
#endif

  /* Test 3: Cancel Detached Thread *****************************************/

  printf("cancel_test: Test 3: Cancellation of detached thread\n");
  printf("cancel_test: Re-starting thread\n");
  restart_thread(sem_waiter, &waiter, 1);

  /* Detach the thread */

  status = pthread_detach(waiter);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_detach, status=%d\n", status);
      ASSERT(false);
    }

  /* Then cancel it.  It should be in the pthread_cond_wait now -- wait a
   * bit to be certain.
   */

  sem_wait(&sem_thread_started);

  /* Make sure sem_waiter run into pthread_cond_wait */

  pthread_mutex_lock(&mutex);

  pthread_mutex_unlock(&mutex);

  printf("cancel_test: Canceling thread\n");
  status = pthread_cancel(waiter);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_cancel failed, status=%d\n",
              status);
      ASSERT(false);
    }

#ifdef CONFIG_CANCELLATION_POINTS
  /* If we are doing deferred cancellation, then we will have to wait a bit
   * of the following pthread_join() may succeed because it happens before
   * before the cancellation.
   */

  usleep(100 * 1000);
#endif

  /* Join should now fail */

  printf("cancel_test: Joining\n");
  status = pthread_join(waiter, &result);
  if (status == 0)
    {
      printf("cancel_test: ERROR pthread_join succeeded\n");
      ASSERT(false);
    }
  else if (status != ESRCH)
    {
      printf("cancel_test:"
             " ERROR pthread_join failed but with wrong status=%d\n",
             status);
      ASSERT(false);
    }
  else
    {
      printf("cancel_test: PASS pthread_join failed with status=ESRCH\n");
    }

  /* Test 4: Non-cancelable threads *****************************************/

  /* This test currently depends on signals.  It doesn't have to and
   * could be re-designed so that it does not depend on signals.
   */

  printf("cancel_test: Test 5: Non-cancelable threads\n");
  printf("cancel_test: Re-starting thread (non-cancelable)\n");
  restart_thread(sem_waiter, &waiter, 0);

  /* Give the thread a chance to run an to set the non-cancelable state.
   * This is the dependency on signals:
   */

  usleep(200 * 1000);

  /* Then cancel it.  It should be in the pthread_cond_wait now.  The
   * behavior here is non-standard:  when the thread is at a cancellation
   * point, it should be cancelable, even when cancellation is disable.
   *
   * The cancellation should succeed, because the cancellation is pending.
   */

  sem_wait(&sem_thread_started);

  /* Make sure sem_waiter run into pthread_cond_wait */

  pthread_mutex_lock(&mutex);

  pthread_mutex_unlock(&mutex);

  printf("cancel_test: Canceling thread\n");
  status = pthread_cancel(waiter);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_cancel failed, status=%d\n",
             status);
      ASSERT(false);
    }

  /* Signal the thread.  It should wake up and restore the cancelable state.
   * When the cancelable state is re-enabled, the thread should be canceled.
   */

  status = pthread_mutex_lock(&mutex);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_mutex_lock failed, status=%d\n",
              status);
      ASSERT(false);
    }

  status = pthread_cond_signal(&cond);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_cond_signal failed, status=%d\n",
             status);
      ASSERT(false);
    }

  status = pthread_mutex_unlock(&mutex);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_mutex_unlock failed, status=%d\n",
             status);
      ASSERT(false);
    }

  /* Then join to the thread to pick up the result (if we don't do
   * we will have a memory leak!)
   */

  printf("cancel_test: Joining\n");
  status = pthread_join(waiter, &result);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_join failed, status=%d\n", status);
      ASSERT(false);
    }
  else
    {
      printf("cancel_test: waiter exited with result=%p\n", result);
      if (result != PTHREAD_CANCELED)
        {
          printf("cancel_test: ERROR expected result=%p\n",
                  PTHREAD_CANCELED);
          ASSERT(false);
        }
      else
        {
          printf("cancel_test: "
                 "PASS thread terminated with PTHREAD_CANCELED\n");
        }
    }

  printf("cancel_test: Test 6: Cancel message queue wait\n");
  printf("cancel_test: Starting thread (cancelable)\n");
  sem_destroy(&sem_thread_started);

#if !defined(CONFIG_DISABLE_MQUEUE) && defined(CONFIG_CANCELLATION_POINTS)
  /* Create the message queue */

  /* Fill in attributes for message queue */

  attr.mq_maxmsg  = 20;
  attr.mq_msgsize = CONFIG_MQ_MAXMSGSIZE;
  attr.mq_flags   = 0;

  mqcancel = mq_open("mqcancel", O_WRONLY | O_CREAT, 0666, &attr);
  if (mqcancel == (mqd_t)-1)
    {
      printf("sender_thread: ERROR mq_open failed\n");
      ASSERT(false);
      pthread_exit((pthread_addr_t)1);
    }

  /* Start the mqueue_waiter thread */

  restart_thread(mqueue_waiter, &waiter, 0);

  /* Then cancel it.  It should be in the mq_read now -- wait bit to
   * make sure.
   */

  usleep(75 * 1000);

  printf("cancel_test: Canceling thread\n");
  status = pthread_cancel(waiter);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_cancel failed, status=%d\n",
             status);
      ASSERT(false);
    }

  /* Then join to the thread to pick up the result (if we don't do
   * we will have a memory leak!)
   */

  printf("cancel_test: Joining\n");
  status = pthread_join(waiter, &result);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_join failed, status=%d\n",
             status);
      ASSERT(false);
    }
  else
    {
      printf("cancel_test: waiter exited with result=%p\n", result);
      if (result != PTHREAD_CANCELED)
        {
          printf("cancel_test: ERROR expected result=%p\n",
                 PTHREAD_CANCELED);
          ASSERT(false);
        }
      else
        {
          printf("cancel_test: "
                 "PASS thread terminated with PTHREAD_CANCELED\n");
        }
    }

  /* Close the message queue */

  if (mq_close(mqcancel) < 0)
    {
      printf("sender_thread: ERROR mq_close failed\n");
      ASSERT(false);
    }
#else
  printf("Skipped\n");
#endif

  printf("cancel_test: Test 7: Cancel signal wait\n");
  printf("cancel_test: Starting thread (cancelable)\n");

#ifdef CONFIG_CANCELLATION_POINTS
  /* Start the sig_waiter thread */

  restart_thread(sig_waiter, &waiter, 0);

  /* Then cancel it.  It should be waiting for a signal now -- wait bit to
   * make sure.
   */

  usleep(75 * 1000);

  printf("cancel_test: Canceling thread\n");
  status = pthread_cancel(waiter);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_cancel failed, status=%d\n",
             status);
      ASSERT(false);
    }

  /* Then join to the thread to pick up the result (if we don't do
   * we will have a memory leak!)
   */

  printf("cancel_test: Joining\n");
  status = pthread_join(waiter, &result);
  if (status != 0)
    {
      printf("cancel_test: ERROR pthread_join failed, status=%d\n",
             status);
      ASSERT(false);
    }
  else
    {
      printf("cancel_test: waiter exited with result=%p\n", result);
      if (result != PTHREAD_CANCELED)
        {
          printf("cancel_test: ERROR expected result=%p\n",
                 PTHREAD_CANCELED);
          ASSERT(false);
        }
      else
        {
          printf("cancel_test: "
                 "PASS thread terminated with PTHREAD_CANCELED\n");
        }
    }
#else
  printf("Skipped\n");
#endif
}
