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
 * apps/examples/testcase/ostest/mqueue.c
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
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ostest.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

#define TEST_MESSAGE        "This is a test and only a test"
#if defined(SDCC) || defined(__ZILOG__)
  /* Cannot use strlen in array size */

#  define TEST_MSGLEN       (31)
#else
  /* Message length is the size of the message plus the null terminator */

#  define TEST_MSGLEN       sizeof(TEST_MESSAGE)
#endif

#define TEST_SEND_NMSGS     (10)
#define TEST_RECEIVE_NMSGS  (11)

#define HALF_SECOND_USEC_USEC 500000L

/****************************************************************************
 * Private Data
 ****************************************************************************/

static mqd_t g_send_mqfd;
static mqd_t g_recv_mqfd;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

static void *sender_thread(void *arg)
{
  char msg_buffer[TEST_MSGLEN];
  struct mq_attr attr;
  int status = 0;
  int nerrors = 0;
  int i;

  printf("sender_thread: Starting\n");

  /* Fill in attributes for message queue */

  attr.mq_maxmsg  = 20;
  attr.mq_msgsize = TEST_MSGLEN;
  attr.mq_flags   = 0;

  /* Set the flags for the open of the queue.
   * Make it a blocking open on the queue, meaning it will block if
   * this process tries to send to the queue and the queue is full.
   *
   *   O_CREAT - the queue will get created if it does not already exist.
   *   O_WRONLY - we are only planning to write to the queue.
   *
   * Open the queue, and create it if the receiving process hasn't
   * already created it.
   */

  g_send_mqfd = mq_open("mqueue", O_WRONLY | O_CREAT, 0666, &attr);
  if (g_send_mqfd == (mqd_t)-1)
    {
      printf("sender_thread: ERROR mq_open failed, errno=%d\n", errno);
      ASSERT(false);
      pthread_exit((pthread_addr_t)1);
    }

  /* Fill in a test message buffer to send */

  memcpy(msg_buffer, TEST_MESSAGE, TEST_MSGLEN);

  /* Perform the send TEST_SEND_NMSGS times */

  for (i = 0; i < TEST_SEND_NMSGS; i++)
    {
      status = mq_send(g_send_mqfd, msg_buffer, TEST_MSGLEN, 42);
      if (status < 0)
        {
          printf("sender_thread: ERROR mq_send failure=%d on msg %d, "
                 "errno=%d\n",
                 status, i, errno);
          ASSERT(false);
          nerrors++;
        }
      else
        {
          printf("sender_thread: mq_send succeeded on msg %d\n", i);
        }
    }

  /* Close the queue and return success */

  if (mq_close(g_send_mqfd) < 0)
    {
      printf("sender_thread: ERROR mq_close failed, errno=%d\n", errno);
      ASSERT(false);
    }
  else
    {
      g_send_mqfd = 0;
    }

  printf("sender_thread: returning nerrors=%d\n", nerrors);
  return (pthread_addr_t)((uintptr_t)nerrors);
}

static void *receiver_thread(void *arg)
{
  char msg_buffer[TEST_MSGLEN];
  struct mq_attr attr;
  int nbytes;
  int nerrors = 0;
  int i;

  printf("receiver_thread: Starting\n");

  /* Fill in attributes for message queue */

  attr.mq_maxmsg  = 20;
  attr.mq_msgsize = TEST_MSGLEN;
  attr.mq_flags   = 0;

  /* Set the flags for the open of the queue.
   * Make it a blocking open on the queue, meaning it will block if
   * this task tries to read from the queue when the queue is empty
   *
   *   O_CREAT - the queue will get created if it does not already exist.
   *   O_RDONLY - we are only planning to read from the queue.
   *
   * Open the queue, and create it if the sending process hasn't
   * already created it.
   */

  g_recv_mqfd = mq_open("mqueue", O_RDONLY | O_CREAT, 0666, &attr);
  if (g_recv_mqfd == (mqd_t)-1)
    {
      printf("receiver_thread: ERROR mq_open failed, errno=%d\n", errno);
      ASSERT(false);
      pthread_exit((pthread_addr_t)1);
    }

  /* Perform the receive TEST_RECEIVE_NMSGS times */

  for (i = 0; i < TEST_RECEIVE_NMSGS; i++)
    {
      memset(msg_buffer, 0xaa, TEST_MSGLEN);
      nbytes = mq_receive(g_recv_mqfd, msg_buffer, TEST_MSGLEN, 0);
      if (nbytes < 0)
        {
          /* mq_receive failed.  If the error is because of EINTR then
           * it is not a failure.
           */

          if (errno != EINTR)
            {
              printf("receiver_thread: ERROR mq_receive failure on msg %d, "
                     "errno=%d\n", i, errno);
              ASSERT(false);
              nerrors++;
            }
          else
            {
              printf("receiver_thread: mq_receive interrupted!\n");
            }
        }
      else if (nbytes != TEST_MSGLEN)
        {
          printf("receiver_thread: "
                 "ERROR mq_receive return bad size %d on msg %d\n",
                 nbytes, i);
          ASSERT(false);
          nerrors++;
        }
      else if (memcmp(TEST_MESSAGE, msg_buffer, nbytes) != 0)
        {
          int j;

          printf("receiver_thread: "
                 "mq_receive returned corrupt message on msg %d\n", i);
          printf("receiver_thread:                  i  Expected Received\n");

          for (j = 0; j < TEST_MSGLEN - 1; j++)
            {
              if (isprint(msg_buffer[j]))
                {
                 printf("receiver_thread:                  "
                        "%2d %02x (%c) %02x (%c)\n",
                         j, TEST_MESSAGE[j], TEST_MESSAGE[j],
                         msg_buffer[j], msg_buffer[j]);
                }
              else
                {
                  printf("receiver_thread:                  "
                         "%2d %02x (%c) %02x\n",
                         j, TEST_MESSAGE[j], TEST_MESSAGE[j], msg_buffer[j]);
                }
            }

          printf("receiver_thread:                  %2d 00      %02x\n",
                  j, msg_buffer[j]);
        }
      else
        {
          printf("receiver_thread: mq_receive succeeded on msg %d\n", i);
        }
    }

  /* Close the queue and return success */

  if (mq_close(g_recv_mqfd) < 0)
    {
      printf("receiver_thread: ERROR mq_close failed\n");
      ASSERT(false);
      nerrors++;
    }
  else
    {
      g_recv_mqfd = 0;
    }

  printf("receiver_thread: returning nerrors=%d\n", nerrors);
  pthread_exit((pthread_addr_t)((uintptr_t)nerrors));
  return (pthread_addr_t)((uintptr_t)nerrors);
}

void mqueue_test(void)
{
  pthread_t sender;
  pthread_t receiver;
  void *result;
  pthread_attr_t attr;
  struct sched_param sparam;
  FAR void *expected;
  int prio_min;
  int prio_max;
  int prio_mid;
  int status;

  /* Reset globals for the beginning of the test */

  g_send_mqfd = 0;
  g_recv_mqfd = 0;

  /* Start the sending thread at higher priority */

  printf("mqueue_test: Starting receiver\n");
  status = pthread_attr_init(&attr);
  if (status != 0)
    {
      printf("mqueue_test: ERROR pthread_attr_init failed, status=%d\n",
             status);
      ASSERT(false);
    }

  status = pthread_attr_setstacksize(&attr, STACKSIZE);
  if (status != 0)
    {
      printf("mqueue_test: "
             "ERROR pthread_attr_setstacksize failed, status=%d\n",
             status);
      ASSERT(false);
    }

  prio_min = sched_get_priority_min(SCHED_FIFO);
  prio_max = sched_get_priority_max(SCHED_FIFO);
  prio_mid = (prio_min + prio_max) / 2;

  sparam.sched_priority = prio_mid;
  status = pthread_attr_setschedparam(&attr, &sparam);
  if (status != OK)
    {
      printf("mqueue_test: "
             "ERROR pthread_attr_setschedparam failed, status=%d\n",
             status);
      ASSERT(false);
    }
  else
    {
      printf("mqueue_test: Set receiver priority to %d\n",
             sparam.sched_priority);
    }

  status = pthread_create(&receiver, &attr, receiver_thread, NULL);
  if (status != 0)
    {
      printf("mqueue_test: "
             "ERROR pthread_create failed, status=%d\n", status);
      ASSERT(false);
    }

  /* Start the sending thread at lower priority */

  printf("mqueue_test: Starting sender\n");
  status = pthread_attr_init(&attr);
  if (status != 0)
    {
      printf("mqueue_test: "
             "ERROR pthread_attr_init failed, status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_attr_setstacksize(&attr, STACKSIZE);
  if (status != 0)
    {
      printf("mqueue_test: "
             "ERROR pthread_attr_setstacksize failed, status=%d\n",
             status);
      ASSERT(false);
    }

  sparam.sched_priority = (prio_min + prio_mid) / 2;
  status = pthread_attr_setschedparam(&attr, &sparam);
  if (status != OK)
    {
      printf("mqueue_test: "
             "ERROR pthread_attr_setschedparam failed, status=%d\n",
             status);
      ASSERT(false);
    }
  else
    {
      printf("mqueue_test: Set sender thread priority to %d\n",
             sparam.sched_priority);
    }

  status = pthread_create(&sender, &attr, sender_thread, NULL);
  if (status != 0)
    {
      printf("mqueue_test: "
             "ERROR pthread_create failed, status=%d\n", status);
      ASSERT(false);
    }

  printf("mqueue_test: Waiting for sender to complete\n");
  pthread_join(sender, &result);
  if (result != (FAR void *)0)
    {
      printf("mqueue_test: ERROR sender thread exited with %d errors\n",
             (int)((intptr_t)result));
      ASSERT(false);
    }

  /* Wake up the receiver thread with a signal */

  printf("mqueue_test: Killing receiver\n");
  pthread_kill(receiver, SIGUSR1);

  /* Wait a bit to see if the thread exits on its own */

  usleep(HALF_SECOND_USEC_USEC);

  /* Then cancel the thread and see if it did */

  printf("mqueue_test: Canceling receiver\n");

  expected = PTHREAD_CANCELED;
  status = pthread_cancel(receiver);
  if (status == ESRCH)
    {
      printf("mqueue_test: receiver has already terminated\n");
      expected = (FAR void *)0;
    }

  /* Check the result.  If the pthread was canceled, PTHREAD_CANCELED is the
   * correct result.  Zero might be returned if the thread ran to completion
   * before it was canceled.
   */

  pthread_join(receiver, &result);
  if (result != expected)
    {
      printf("mqueue_test: "
             "ERROR receiver thread should have exited with %p\n",
             expected);
      printf("             ERROR Instead exited with nerrors=%d\n",
             (int)((intptr_t)result));
      ASSERT(false);
    }

  /* Message queues are global resources and persist for the life the
   * task group.  The message queue opened by the sender_thread must be
   * closed since the sender pthread may have been canceled and may have
   * left the message queue open.
   */

  if (result == PTHREAD_CANCELED && g_recv_mqfd)
    {
      if (mq_close(g_recv_mqfd) < 0)
        {
          printf("mqueue_test: ERROR mq_close failed\n");
          ASSERT(false);
        }
    }
  else if (result != PTHREAD_CANCELED && g_recv_mqfd)
    {
      printf("mqueue_test: ERROR send mqd_t left open\n");
      ASSERT(false);
      if (mq_close(g_recv_mqfd) < 0)
        {
          printf("mqueue_test: ERROR mq_close failed\n");
          ASSERT(false);
        }
    }

  /* Make sure that the receive queue is closed as well */

  if (g_send_mqfd)
    {
      printf("mqueue_test: ERROR receiver mqd_t left open\n");
      ASSERT(false);
      if (mq_close(g_send_mqfd) < 0)
        {
          printf("sender_thread: ERROR mq_close failed\n");
          ASSERT(false);
        }
    }

  /* Destroy the message queue */

  if (mq_unlink("mqueue") < 0)
    {
      printf("mqueue_test: ERROR mq_unlink failed\n");
      ASSERT(false);
    }
}
