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
 * apps/examples/testcase/ostest/timedmqueue.c
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

#  define TEST_MSGLEN         (31)
#else
/* Message length is the size of the message plus the null terminator */

#  define TEST_MSGLEN         sizeof(TEST_MESSAGE)
#endif

#define TEST_SEND_NMSGS     (10)
#define TEST_RECEIVE_NMSGS  (10)

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

static mqd_t g_send_mqfd;
static mqd_t g_recv_mqfd;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

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

  attr.mq_maxmsg  = TEST_SEND_NMSGS - 1;
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

  g_send_mqfd = mq_open("timedmq", O_WRONLY | O_CREAT, 0666, &attr);
  if (g_send_mqfd == (mqd_t)-1)
    {
        printf("sender_thread: ERROR mq_open failed\n");
        ASSERT(false);
        pthread_exit((pthread_addr_t)1);
    }

  /* Fill in a test message buffer to send */

  memcpy(msg_buffer, TEST_MESSAGE, TEST_MSGLEN);

  /* Perform the send TEST_SEND_NMSGS times */

  for (i = 0; i < TEST_SEND_NMSGS; i++)
    {
      struct timespec ts;
      status = clock_gettime(CLOCK_REALTIME, &ts);
      if (status != 0)
        {
          printf("sender_thread: ERROR clock_gettime failed\n");
          ASSERT(false);
        }

      ts.tv_sec += 5;

      /* The first TEST_SEND_NMSGS-1 send should succeed.  The last
       * one should fail with errno == ETIMEDOUT
       */

      status = mq_timedsend(g_send_mqfd, msg_buffer, TEST_MSGLEN, 42, &ts);
      if (status < 0)
        {
          if (i == TEST_SEND_NMSGS - 1 && errno == ETIMEDOUT)
            {
              printf("sender_thread: mq_timedsend %d "
                     "timed out as expected\n", i);
            }
          else
            {
              printf("sender_thread: ERROR mq_timedsend "
                     "failure=%d on msg %d\n", errno, i);
              ASSERT(false);
              nerrors++;
            }
        }
      else
        {
          if (i == TEST_SEND_NMSGS - 1)
            {
              printf("sender_thread: ERROR mq_timedsend of msg %d\n", i);
              ASSERT(false);
              nerrors++;
            }
          else
            {
              printf("sender_thread: mq_timedsend succeeded on msg %d\n", i);
            }
        }
    }

  /* Close the queue and return success */

  if (mq_close(g_send_mqfd) < 0)
    {
      printf("sender_thread: ERROR mq_close failed\n");
      ASSERT(false);
    }
  else
    {
      g_send_mqfd = 0;
    }

  printf("sender_thread: returning nerrors=%d\n", nerrors);
  FFLUSH();
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

  attr.mq_maxmsg  = TEST_SEND_NMSGS - 1;
  attr.mq_msgsize = TEST_MSGLEN;
  attr.mq_flags   = 0;

  /* Set the flags for the open of the queue.
   * Make it a blocking open on the queue, meaning it will block if
   * this process tries to* send to the queue and the queue is full.
   *
   *   O_CREAT - the queue will get created if it does not already exist.
   *   O_RDONLY - we are only planning to write to the queue.
   *
   * Open the queue, and create it if the sending process hasn't
   * already created it.
   */

  g_recv_mqfd = mq_open("timedmq", O_RDONLY | O_CREAT, 0666, &attr);
  if (g_recv_mqfd == (mqd_t)-1)
    {
      printf("receiver_thread: ERROR mq_open failed\n");
      ASSERT(false);
      pthread_exit((pthread_addr_t)1);
    }

  /* Perform the receive TEST_RECEIVE_NMSGS times */

  for (i = 0; i < TEST_RECEIVE_NMSGS; i++)
    {
      struct timespec ts;
      int status = clock_gettime(CLOCK_REALTIME, &ts);
      if (status != 0)
        {
          printf("sender_thread: ERROR clock_gettime failed\n");
          ASSERT(false);
        }

      ts.tv_sec += 5;

      /* The first TEST_SEND_NMSGS-1 send should succeed.  The last
       * one should fail with errno == ETIMEDOUT
       */

      memset(msg_buffer, 0xaa, TEST_MSGLEN);
      nbytes = mq_timedreceive(g_recv_mqfd, msg_buffer, TEST_MSGLEN, 0, &ts);
      if (nbytes < 0)
        {
          if (i == TEST_SEND_NMSGS - 1 && errno == ETIMEDOUT)
            {
              printf("receiver_thread: Receive %d "
                     "timed out as expected\n", i);
            }
          else
            {
              printf("receiver_thread: ERROR mq_timedreceive "
                     "failure=%d on msg %d\n", errno, i);
              ASSERT(false);
              nerrors++;
            }
        }
      else if (nbytes != TEST_MSGLEN)
        {
          printf("receiver_thread: mq_timedreceive return "
                 "ERROR bad size %d on msg %d\n", nbytes, i);
          ASSERT(false);
          nerrors++;
        }
      else if (memcmp(TEST_MESSAGE, msg_buffer, nbytes) != 0)
        {
          int j;

          printf("receiver_thread: mq_timedreceive returned "
                 "corrupt message on msg %d\n", i);
          printf("receiver_thread:  i  Expected Received\n");

          for (j = 0; j < TEST_MSGLEN - 1; j++)
            {
              if (isprint(msg_buffer[j]))
                {
                 printf("receiver_thread: %2d %02x (%c) %02x (%c)\n",
                         j, TEST_MESSAGE[j], TEST_MESSAGE[j],
                         msg_buffer[j], msg_buffer[j]);
                }
              else
                {
                  printf("receiver_thread: %2d %02x (%c) %02x\n",
                         j, TEST_MESSAGE[j], TEST_MESSAGE[j], msg_buffer[j]);
                }
            }

          printf("receiver_thread: %2d 00 %02x\n", j, msg_buffer[j]);
        }
      else if (i == TEST_SEND_NMSGS - 1)
        {
          printf("receiver_thread: ERROR mq_timedreceive of msg %d\n", i);
          ASSERT(false);
          nerrors++;
        }
      else
        {
          printf("receiver_thread: mq_timedreceive succeed on msg %d\n", i);
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
  FFLUSH();
  pthread_exit((pthread_addr_t)((uintptr_t)nerrors));
  return (pthread_addr_t)((uintptr_t)nerrors);
}

void timedmqueue_test(void)
{
  pthread_t sender;
  pthread_t receiver;
  void *result;
  pthread_attr_t attr;
  int status;

  /* Start the sending thread at the default priority */

  printf("timedmqueue_test: Starting sender\n");
  status = pthread_attr_init(&attr);
  if (status != 0)
    {
      printf("timedmqueue_test: ERROR pthread_attr_init failed, status=%d\n",
             status);
      ASSERT(false);
    }

  status = pthread_attr_setstacksize(&attr, STACKSIZE);
  if (status != 0)
    {
      printf("timedmqueue_test: ERROR pthread_attr_setstacksize failed, "
             "status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_create(&sender, &attr, sender_thread, NULL);
  if (status != 0)
    {
      printf("timedmqueue_test: ERROR pthread_create failed, status=%d\n",
             status);
      ASSERT(false);
    }

  /* Wait for the sending thread to complete */

  printf("timedmqueue_test: Waiting for sender to complete\n");
  pthread_join(sender, &result);
  if (result != NULL)
    {
      printf("timedmqueue_test: ERROR sender thread exited with %d errors\n",
             (int)((intptr_t)result));
      ASSERT(false);
    }

  /* Start the receiving thread at the default priority */

  printf("timedmqueue_test: Starting receiver\n");
  status = pthread_attr_init(&attr);
  if (status != 0)
    {
      printf("timedmqueue_test: ERROR pthread_attr_init failed, status=%d\n",
             status);
      ASSERT(false);
    }

  status = pthread_attr_setstacksize(&attr, STACKSIZE);
  if (status != 0)
    {
      printf("timedmqueue_test: ERROR pthread_attr_setstacksize failed, "
             "status=%d\n", status);
      ASSERT(false);
    }

  status = pthread_create(&receiver, &attr, receiver_thread, NULL);
  if (status != 0)
    {
      printf("timedmqueue_test: ERROR pthread_create failed, status=%d\n",
             status);
      ASSERT(false);
    }

  /* Wait for the receiving thread to complete */

  printf("timedmqueue_test: Waiting for receiver to complete\n");
  pthread_join(receiver, &result);
  if (result != NULL)
    {
      printf("timedmqueue_test: ERROR receiver thread exited "
             "with %d errors\n", (int)((intptr_t)result));
      ASSERT(false);
    }

  /* Make sure that the message queues were properly closed (otherwise, we
   * might have problems the next time in the test loop.
   */

  if (g_send_mqfd)
    {
      printf("timedmqueue_test: ERROR send mqd_t left open\n");
      ASSERT(false);
      if (mq_close(g_send_mqfd) < 0)
        {
          printf("timedmqueue_test: ERROR mq_close failed\n");
          ASSERT(false);
        }
    }

  if (g_recv_mqfd)
    {
      printf("timedmqueue_test: ERROR receive mqd_t left open\n");
      ASSERT(false);
      if (mq_close(g_recv_mqfd) < 0)
        {
          printf("timedmqueue_test: ERROR mq_close failed\n");
          ASSERT(false);
        }
    }

  /* Destroy the message queue */

  if (mq_unlink("timedmq") < 0)
    {
      printf("timedmqueue_test: ERROR mq_unlink failed\n");
      ASSERT(false);
    }

  printf("timedmqueue_test: Test complete\n");
}
