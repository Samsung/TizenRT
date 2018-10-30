/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
/**************************************************************************
*
*   Copyright (C) 2007-2009, 2011 Gregory Nutt. All rights reserved.
*   Author: Gregory Nutt <gnutt@nuttx.org>
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name NuttX nor the names of its contributors may be
*    used to endorse or promote products derived from this software
*    without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************/

/// @file tc_memory_safety.c

/// @brief Test Case for checking heap memory safety 
///				when sending/receiving bulk message (i.e., 8K) through message queue

/**************************************************************************
* Included Files
**************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <ctype.h>
#ifndef CONFIG_DISABLE_SIGNALS
#include <signal.h>
#endif
#include <fcntl.h>
#include <mqueue.h>
#include <sched.h>
#include <pthread.h>
#include "tc_internal.h"

/**************************************************************************
* Private Definitions
**************************************************************************/
#define TEST_MESSAGE        "This is a test and only a test"
#define TEST_MSGLEN         CONFIG_MQ_MAXMSGSIZE

#define TEST_SEND_NMSGS      (int) 8000000 / TEST_MSGLEN
#ifndef CONFIG_DISABLE_SIGNALS
#define TEST_RECEIVE_NMSGS   TEST_SEND_NMSGS + 1
#else
#define TEST_RECEIVE_NMSGS   TEST_SEND_NMSGS
#endif

#define HALF_SECOND_USEC_USEC   500000L
#define STACKSIZE TEST_MSGLEN + 1024

static mqd_t g_send_mqfd;
static mqd_t g_recv_mqfd;
static int g_exit;

/**
* @fn                   :sender_thread
* @description          :Function for tc_memory_safety
* @return               :void*
*/
static void *sender_thread(void *arg)
{
	char msg_buffer[TEST_MSGLEN];
	struct mq_attr attr;
	int status = 0;
	int nerrors = 0;
	int msg_idx;

	/* Fill in attributes for message queue */

	attr.mq_maxmsg = 20;
	attr.mq_msgsize = TEST_MSGLEN;
	attr.mq_flags = 0;

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
	if (g_send_mqfd == (mqd_t)-1) {
		printf("tc_mqueue_mq_open FAIL\n");
		pthread_exit((pthread_addr_t)1);
	}

	/* Fill in a test message buffer to send */

	memset(msg_buffer, 0x00, TEST_MSGLEN);
	memcpy(msg_buffer, TEST_MESSAGE, TEST_MSGLEN);

	/* Perform the send TEST_SEND_NMSGS times */

	for (msg_idx = 0; msg_idx < TEST_SEND_NMSGS; msg_idx++) {
		status = mq_send(g_send_mqfd, msg_buffer, TEST_MSGLEN, 42);
		if (status < 0) {
			printf("tc_mqueue_mq_send FAIL : failure=%d on msg %d\n", status, msg_idx);
			nerrors++;
		}
	}

	/* Close the queue and return success */

	if (mq_close(g_send_mqfd) < 0) {
		printf("tc_mqueue_mq_close FAIL\n");
	} else {
		g_send_mqfd = NULL;
	}

	pthread_exit((pthread_addr_t)nerrors);
	return (pthread_addr_t)nerrors;
}

/**
* @fn                   :receiver_thread
* @description          :Function for tc_memory_safety
* @return               :void*
*/
static void *receiver_thread(void *arg)
{
	char msg_buffer[TEST_MSGLEN];
	struct mq_attr attr;
	int nbytes;
	int nerrors = 0;
	int msg_idx;

	/* Fill in attributes for message queue */

	attr.mq_maxmsg = 20;
	attr.mq_msgsize = TEST_MSGLEN;
	attr.mq_flags = 0;

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
	if (g_recv_mqfd == (mqd_t)ERROR) {
		printf("tc_mqueue_mq_open FAIL\n");
		pthread_exit((pthread_addr_t)1);
	}

	/* Perform the receive TEST_RECEIVE_NMSGS times */

	for (msg_idx = 0; msg_idx < TEST_RECEIVE_NMSGS; msg_idx++) {
		memset(msg_buffer, 0xaa, TEST_MSGLEN);
		nbytes = mq_receive(g_recv_mqfd, msg_buffer, TEST_MSGLEN, 0);
		if (nbytes < 0) {
			/* mq_receive failed.  If the error is because of EINTR then
			 * it is not a failure.
			 */

			if (errno != EINTR) {
				printf("tc_mqueue_mq_receive FAIL : failure on msg %d, errno=%d\n", msg_idx, errno);
				nerrors++;
			} else {
				tckndbg("mq_receive interrupted!\n");
			}
		} else if (nbytes != TEST_MSGLEN) {
			printf("tc_mqueue_mq_receive FAIL : return bad size %d on msg %d\n", nbytes, msg_idx);
			nerrors++;
		} else if (memcmp(TEST_MESSAGE, msg_buffer, nbytes) != OK) {
			int msg_chk;

			printf("tc_mqueue_mq_receive FAIL : returned corrupt message on msg %d\n", msg_idx);
			tckndbg("receiver_thread:                  i  Expected Received\n");

			for (msg_chk = 0; msg_chk < TEST_MSGLEN - 1; msg_chk++) {
				if (isprint(msg_buffer[msg_chk])) {
					tckndbg("receiver_thread: %2d %02x (%c) %02x (%c)\n", msg_chk, TEST_MESSAGE[msg_chk], TEST_MESSAGE[msg_chk], msg_buffer[msg_chk], msg_buffer[msg_chk]);
				} else {
					tckndbg("receiver_thread: %2d %02x (%c) %02x\n", msg_chk, TEST_MESSAGE[msg_chk], TEST_MESSAGE[msg_chk], msg_buffer[msg_chk]);
				}
			}

			tckndbg("receiver_thread: %2d 00 %02x\n", msg_chk, msg_buffer[msg_chk]);
		}
	}

	/* Close the queue and return success */

	if (mq_close(g_recv_mqfd) < 0) {
		printf("tc_mqueue_mq_close FAIL\n");
		nerrors++;
	} else {
		g_recv_mqfd = NULL;
	}

	pthread_exit((pthread_addr_t)nerrors);
	return (pthread_addr_t)nerrors;
}

/**
* @fn                   :infiniteloop
* @description          :Function for tc_memory_safety
* @return               :void*
*/
static void *infiniteloop(void *arg)
{
	int i = 0;
	while (1) {
		i++;
		i = i % 1000;
		if (g_exit == 1) {
			break;
		}
	}
	return NULL;
}

/**
 * @fn                  :tc_memory_safety_with_mqueue
 * @brief               :Tests mqueue APIs open, close, send, receive.
 * @Scenario            :Tests mqueue APIs open, close, send, receive.
 * API's covered        :mq_open, mq_close, mq_send, mq_receive
 * Preconditions        :none
 * Postconditions       :none
 * @return              :void
 */
static void tc_memory_safety_with_mqueue(void)
{
	pthread_t sender;
	pthread_t receiver;
	pthread_t looper;
	void *result;
	pthread_attr_t attr;
	struct sched_param sparam;
	FAR void *expected;
	int priority1 = CONFIG_SCHED_LPWORKPRIORITY + 1;
	int priority2 = CONFIG_SCHED_LPWORKPRIORITY + 2;
	int priority3 = CONFIG_SCHED_LPWORKPRIORITY + 3;
	int status;
	struct sched_param st_getparam;

	/* Reset globals for the beginning of the test */

	g_send_mqfd = NULL;
	g_recv_mqfd = NULL;
	g_exit = 0;

	/* Check whether the priority of this task is the largest among all threads */

	sched_getparam(getpid(), &st_getparam);
	TC_ASSERT_GT("sched_getparam", st_getparam.sched_priority, priority3);

	/* Start the infinite loop thread with priority1 */

	status = pthread_attr_init(&attr);
	TC_ASSERT_EQ("pthread_attr_init", status, OK);

	status = pthread_attr_setstacksize(&attr, STACKSIZE);
	TC_ASSERT_EQ("pthread_attr_setstacksize", status, OK);

	sparam.sched_priority = priority1;
	status = pthread_attr_setschedparam(&attr, &sparam);
	TC_ASSERT_EQ("pthread_attr_setschedparam", status, OK);

	status = pthread_create(&looper, &attr, infiniteloop, NULL);
	TC_ASSERT_EQ("pthread_create", status, OK);
	pthread_setname_np(looper, "infinite_loop");

	/* Start the receiving thread with priority2 */

	status = pthread_attr_init(&attr);
	TC_ASSERT_EQ("pthread_attr_init", status, OK);

	status = pthread_attr_setstacksize(&attr, STACKSIZE);
	TC_ASSERT_EQ("pthread_attr_setstacksize", status, OK);

	sparam.sched_priority = priority2;
	status = pthread_attr_setschedparam(&attr, &sparam);
	TC_ASSERT_EQ("pthread_attr_setschedparam", status, OK);

	status = pthread_create(&receiver, &attr, receiver_thread, NULL);
	TC_ASSERT_EQ("pthread_create", status, OK);

	/* Start the sending thread with priority3 */

	status = pthread_attr_init(&attr);
	TC_ASSERT_EQ("pthread_attr_init", status, OK);

	status = pthread_attr_setstacksize(&attr, STACKSIZE);
	TC_ASSERT_EQ("pthread_attr_setstacksize", status, OK);

	sparam.sched_priority = priority3;
	status = pthread_attr_setschedparam(&attr, &sparam);
	TC_ASSERT_EQ("pthread_attr_setschedparam", status, OK);

	status = pthread_create(&sender, &attr, sender_thread, NULL);
	TC_ASSERT_EQ("pthread_create", status, OK);

	/* The test is done. Let's wrap up the test */

	pthread_join(sender, &result);
	TC_ASSERT_EQ("pthread_join", result, (void *)0);

	g_exit = 1;
	
#ifndef CONFIG_DISABLE_SIGNALS
	/* Wake up the receiver thread with a signal */

	pthread_kill(receiver, 9);

	/* Wait a bit to see if the thread exits on its own */

	usleep(HALF_SECOND_USEC_USEC);
#endif

	/* Then cancel the thread and see if it did */

	expected = PTHREAD_CANCELED;
	status = pthread_cancel(receiver);
	if (status == ESRCH) {
		tckndbg("receiver has already terminated\n");
		expected = (FAR void *)0;
	}

	/* Check the result.  If the pthread was canceled, PTHREAD_CANCELED is the
	 * correct result.  Zero might be returned if the thread ran to completion
	 * before it was canceled.
	 */

	pthread_join(receiver, &result);
	TC_ASSERT_EQ("pthread_join", result, expected);

	/* Message queues are global resources and persist for the life the the
	 * task group.  The message queue opened by the sender_thread must be closed
	 * since the sender pthread may have been canceled and may have left the
	 * message queue open.
	 */

	if (result == PTHREAD_CANCELED && g_recv_mqfd) {
		TC_ASSERT_GEQ("mq_close", mq_close(g_recv_mqfd), 0);
	} else if (result != PTHREAD_CANCELED && g_recv_mqfd) {
		TC_ASSERT_EQ("mq_close", mq_close(g_recv_mqfd), 0);
	}

	pthread_join(looper, &result);
	TC_ASSERT_EQ("pthread_join", result, 0);

	/* Make sure that the receive queue is closed as well */
	TC_ASSERT_EQ("mq_close", g_send_mqfd, NULL);

	/* Destroy the message queue */
	TC_ASSERT_GEQ("mq_unlink", mq_unlink("mqueue"), 0);

	TC_SUCCESS_RESULT();
}


/****************************************************************************
 * Name: memory_safety
 ****************************************************************************/
int memory_safety_main(void)
{
	tc_memory_safety_with_mqueue();
	return 0;
}
