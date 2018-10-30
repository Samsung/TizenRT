/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file tc_mqueue.c

/// @brief Test Case Example for Message Queue API

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
#include "tc_internal.h"

/**************************************************************************
* Private Definitions
**************************************************************************/
#define TEST_MESSAGE        "This is a test and only a test"
#if defined(SDCC) || defined(__ZILOG__)
/* Cannot use strlen in array size */

#define TEST_MSGLEN         (31)
#else
/* Message lenght is the size of the message plus the null terminator */

#define TEST_MSGLEN         (strlen(TEST_MESSAGE) + 1)
#endif

#define TEST_SEND_NMSGS      (10)
#ifndef CONFIG_DISABLE_SIGNALS
#define TEST_RECEIVE_NMSGS   (11)
#else
#define TEST_RECEIVE_NMSGS   (10)
#endif

#define TEST_TIMEDSEND_NMSGS    3
#define TEST_TIMEDRECEIVE_NMSGS 3

#define HALF_SECOND_USEC_USEC   500000L

#ifdef CONFIG_EXAMPLES_OSTEST_STACKSIZE
#define STACKSIZE CONFIG_EXAMPLES_OSTEST_STACKSIZE
#else
#define STACKSIZE 1024
#endif
/**************************************************************************
* Private Types
**************************************************************************/

/**************************************************************************
* Private Function Prototypes
**************************************************************************/

/**************************************************************************
* Global Variables
**************************************************************************/

/**************************************************************************
* Private Variables
**************************************************************************/

static mqd_t g_send_mqfd;
static mqd_t g_recv_mqfd;
static mqd_t g_timedsend_mqfd;
static mqd_t g_timedrecv_mqfd;
static int enter_notify_handler = 0;
static int timedsend_check = 0;
static int timedreceive_check = 0;
/**************************************************************************
* Private Functions
**************************************************************************/

/**************************************************************************
* Public Functions
**************************************************************************/

/**
* @fn                   :mq_notify_handler
* @description          :Function for tc_libc_mqueue_mq_notify
* @return               :void
*/
static void mq_notify_handler(int param)
{
	enter_notify_handler = 1;
}

/**
* @fn                   :timedsender_thread
* @description          :Function for tc_mqueue_mq_timedsend_timedreceive
* @return               :void*
*/
static int timedsend_test(void)
{
	char msg_buffer[TEST_MSGLEN];
	struct mq_attr attr;
	int status = 0;
	int msg_idx;
	struct timespec st_time;

	clock_gettime(CLOCK_REALTIME, &st_time);
	st_time.tv_sec += 1;

	/* Fill in attributes for message queue */

	attr.mq_maxmsg = TEST_TIMEDSEND_NMSGS;
	attr.mq_msgsize = TEST_MSGLEN;
	attr.mq_flags = 0;

	g_timedsend_mqfd = mq_open("t_mqueue", O_WRONLY | O_CREAT, 0666, &attr);
	if (g_timedsend_mqfd == (mqd_t)-1) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL: mq_open\n");
		return ERROR;
	}

	/* Fill in a test message buffer to send */

	memcpy(msg_buffer, TEST_MESSAGE, TEST_MSGLEN);

	/* Perform the send TEST_TIMEDSEND_NMSGS times */

	for (msg_idx = 0; msg_idx < TEST_TIMEDSEND_NMSGS + 1; msg_idx++) {
		status = mq_timedsend(g_timedsend_mqfd, msg_buffer, TEST_MSGLEN, 42, &st_time);
		if (status == OK) {
			timedsend_check++;
		} else if (status < 0) {
			if (timedsend_check != 3) {
				printf("tc_mqueue_mq_timedsend_timedreceive FAIL : failure=%d on msg %d\n", status, msg_idx);
				mq_close(g_timedsend_mqfd);
				return ERROR;
			} else {
				tckndbg("mq_timedsend is working well : to send is failed after some time when mqueue is full\n");
			}
		}
	}

	/* Close the queue and return success */

	if (mq_close(g_timedsend_mqfd) < 0) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL : mq_close\n");
		return ERROR;
	} else {
		g_timedsend_mqfd = NULL;
	}

	/* Perform the timedsend_test for file read permission cases */

	memcpy(msg_buffer, TEST_MESSAGE, TEST_MSGLEN);

	g_timedsend_mqfd = mq_open("t_mqueue", O_RDONLY | O_CREAT, 0666, &attr);
	if (g_timedsend_mqfd == (mqd_t)-1) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL: mq_open\n");
		return ERROR;
	}

	/* Perform the send for above failure check cases */

	status = mq_timedsend(g_timedsend_mqfd, msg_buffer, TEST_MSGLEN, 42, &st_time);
	if (status != ERROR) {
		mq_close(g_timedsend_mqfd);
		return ERROR;
	}

	if (mq_close(g_timedsend_mqfd) < 0) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL : mq_close\n");
		return ERROR;
	} else {
		g_timedsend_mqfd = NULL;
	}

	/* Perform the timedsend_test for NULL message queue parameter check */

	g_timedsend_mqfd = mq_open("t_mqueue", O_WRONLY | O_CREAT, 0666, &attr);
	if (g_timedsend_mqfd == (mqd_t)-1) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL: mq_open\n");
		return ERROR;
	}

	status = mq_timedsend(NULL, msg_buffer, TEST_MSGLEN, 42, &st_time);
	if (status != ERROR) {
		mq_close(g_timedsend_mqfd);
		return ERROR;
	}

	if (mq_close(g_timedsend_mqfd) < 0) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL : mq_close\n");
		return ERROR;
	} else {
		g_timedsend_mqfd = NULL;
	}

	/* Perform the timedsend_test for mqueue msg length check case */

	memcpy(msg_buffer, TEST_MESSAGE, TEST_MSGLEN);

	g_timedsend_mqfd = mq_open("t_mqueue", O_WRONLY | O_CREAT, 0666, &attr);
	if (g_timedsend_mqfd == (mqd_t)-1) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL: mq_open\n");
		return ERROR;
	}

	/* Perform the send for above failure check case */

	status = mq_timedsend(g_timedsend_mqfd, msg_buffer, 33, 42, &st_time);
	if (status != ERROR) {
		mq_close(g_timedsend_mqfd);
		return ERROR;
	}

	if (mq_close(g_timedsend_mqfd) < 0) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL : mq_close\n");
		return ERROR;
	} else {
		g_timedsend_mqfd = NULL;
	}

	/* Perform the timedsend_test for NULL time parameter check */

	memcpy(msg_buffer, TEST_MESSAGE, TEST_MSGLEN);

	g_timedsend_mqfd = mq_open("t_mqueue", O_WRONLY | O_CREAT, 0666, &attr);
	if (g_timedsend_mqfd == (mqd_t)-1) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL: mq_open\n");
		return ERROR;
	}

	/* Perform the send for above failure check case */

	status = mq_timedsend(g_timedsend_mqfd, msg_buffer, TEST_MSGLEN, 42, NULL);
	if (status != ERROR) {
		mq_close(g_timedsend_mqfd);
		return ERROR;
	}

	if (mq_close(g_timedsend_mqfd) < 0) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL : mq_close\n");
		return ERROR;
	} else {
		g_timedsend_mqfd = NULL;
	}

	return OK;
}

/**
* @fn                   :timedreceiver_thread
* @description          :Function for tc_mqueue_mq_timedsend_timedreceive
* @return               :void*
*/
static int timedreceive_test(void)
{
	char msg_buffer[TEST_MSGLEN];
	struct mq_attr attr;
	int nbytes;
	int msg_idx;
	struct timespec st_time;

	clock_gettime(CLOCK_REALTIME, &st_time);
	st_time.tv_sec += 1;

	/* Fill in attributes for message queue */

	attr.mq_maxmsg = TEST_TIMEDRECEIVE_NMSGS;
	attr.mq_msgsize = TEST_MSGLEN;
	attr.mq_flags = 0;

	g_timedrecv_mqfd = mq_open("t_mqueue", O_RDONLY | O_CREAT, 0666, &attr);
	if (g_timedrecv_mqfd == (mqd_t)ERROR) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL : mq_open\n");
		return ERROR;
	}

	/* Perform the receive TEST_TIMEDRECEIVE_NMSGS times */

	for (msg_idx = 0; msg_idx < TEST_TIMEDRECEIVE_NMSGS; msg_idx++) {
		memset(msg_buffer, 0xaa, TEST_MSGLEN);
		nbytes = mq_timedreceive(g_timedrecv_mqfd, msg_buffer, TEST_MSGLEN, 0, &st_time);
		if (nbytes == TEST_MSGLEN) {
			timedreceive_check++;
		} else if (nbytes < 0) {
			if (timedreceive_check != 3) {
				printf("tc_mqueue_mq_timedsend_timedreceive FAIL : mq_timedreceive\n");
				mq_close(g_timedrecv_mqfd);
				return ERROR;
			} else {
				tckndbg("mq_timedreceive is working well : to receive is failed after some time when mqueue is empty\n");
			}
		}
	}

	/* Close the queue and return success */

	if (mq_close(g_timedrecv_mqfd) < 0) {
		printf("tc_mqueue_mq_close FAIL\n");
		return ERROR;
	} else {
		g_timedrecv_mqfd = NULL;
	}

	/* Perform the timedreceive_test for NULL message queue parameter check & file write permission cases */

	memset(msg_buffer, 0xaa, TEST_MSGLEN);

	g_timedrecv_mqfd = mq_open("t_mqueue", O_WRONLY | O_CREAT, 0666, &attr);
	if (g_timedrecv_mqfd == (mqd_t)ERROR) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL : mq_open\n");
		return ERROR;
	}

	/* Perform the receive for above failure check cases */

	nbytes = mq_timedreceive(g_timedrecv_mqfd, msg_buffer, TEST_MSGLEN, 0, &st_time);
	if (nbytes != ERROR) {
		mq_close(g_timedrecv_mqfd);
		return ERROR;
	}

	if (mq_close(g_timedrecv_mqfd) < 0) {
		printf("tc_mqueue_mq_close FAIL\n");
		return ERROR;
	} else {
		g_timedrecv_mqfd = NULL;
	}

	/* Perform the timedreceive_test for NULL time parameter check */

	memset(msg_buffer, 0xaa, TEST_MSGLEN);

	g_timedrecv_mqfd = mq_open("t_mqueue", O_RDONLY | O_CREAT, 0666, &attr);
	if (g_timedrecv_mqfd == (mqd_t)ERROR) {
		printf("tc_mqueue_mq_timedsend_timedreceive FAIL : mq_open\n");
		return ERROR;
	}

	/* Perform the receive for above failure check cases */

	nbytes = mq_timedreceive(g_timedrecv_mqfd, msg_buffer, TEST_MSGLEN, 0, NULL);
	if (nbytes != ERROR || errno != EINVAL) {
		mq_close(g_timedrecv_mqfd);
		return ERROR;
	}

	if (mq_close(g_timedrecv_mqfd) < 0) {
		printf("tc_mqueue_mq_close FAIL\n");
		return ERROR;
	} else {
		g_timedrecv_mqfd = NULL;
	}

	return OK;
}

/**
* @fn                   :sender_thread
* @description          :Function for tc_mqueue_test
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
* @description          :Function for tc_mqueue_test
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
 * @fn                  :tc_mqueue_test
 * @brief               :Tests mqueue APIs open, close, send, receive.
 * @Scenario            :Tests mqueue APIs open, close, send, receive.
 * API's covered        :mq_open, mq_close, mq_send, mq_receive
 * Preconditions        :none
 * Postconditions       :none
 * @return              :void
 */
static void tc_mqueue_mq_open_close_send_receive(void)
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

	g_send_mqfd = NULL;
	g_recv_mqfd = NULL;

	/* Start the sending thread at higher priority */

	status = pthread_attr_init(&attr);
	TC_ASSERT_EQ("pthread_attr_init", status, OK);

	status = pthread_attr_setstacksize(&attr, STACKSIZE);
	TC_ASSERT_EQ("pthread_attr_setstacksize", status, OK);

	prio_min = sched_get_priority_min(SCHED_FIFO);
	prio_max = sched_get_priority_max(SCHED_FIFO);
	prio_mid = (prio_min + prio_max) / 2;

	sparam.sched_priority = prio_mid;
	status = pthread_attr_setschedparam(&attr, &sparam);
	TC_ASSERT_EQ("pthread_attr_setschedparam", status, OK);

	status = pthread_create(&receiver, &attr, receiver_thread, NULL);
	TC_ASSERT_EQ("pthread_create", status, OK);

	/* Start the sending thread at lower priority */

	status = pthread_attr_init(&attr);
	TC_ASSERT_EQ("pthread_attr_init", status, OK);

	status = pthread_attr_setstacksize(&attr, STACKSIZE);
	TC_ASSERT_EQ("pthread_attr_setstacksize", status, OK);

	sparam.sched_priority = (prio_min + prio_mid) / 2;
	status = pthread_attr_setschedparam(&attr, &sparam);
	TC_ASSERT_EQ("pthread_attr_setschedparam", status, OK);

	status = pthread_create(&sender, &attr, sender_thread, NULL);
	TC_ASSERT_EQ("pthread_create", status, OK);

	pthread_join(sender, &result);
	TC_ASSERT_EQ("pthread_join", result, (void *)0);
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

	/* Make sure that the receive queue is closed as well */
	TC_ASSERT_EQ("mq_close", g_send_mqfd, NULL);

	/* Destroy the message queue */
	TC_ASSERT_GEQ("mq_unlink", mq_unlink("mqueue"), 0);

	TC_SUCCESS_RESULT();
}

static void tc_mqueue_mq_notify(void)
{
	mqd_t mqdes;
	const char s_msg_ptr[15] = "test message\n";
	struct sigevent notification;
	struct sigaction sa;
	unsigned int prio = 1;

	mqdes = mq_open("noti_queue", O_CREAT | O_RDWR, 0666, 0);
	TC_ASSERT_NEQ("mq_open", mqdes, (mqd_t)-1);

	notification.sigev_notify = SIGEV_SIGNAL;
	notification.sigev_signo = SIGUSR1;
	sa.sa_handler = mq_notify_handler;
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);

	TC_ASSERT_EQ_ERROR_CLEANUP("mq_notify", mq_notify(mqdes, &notification), OK, get_errno(), goto cleanup);
	TC_ASSERT_EQ_CLEANUP("mq_send", mq_send(mqdes, s_msg_ptr, 15, prio), OK, goto cleanup);

	sleep(1);
	TC_ASSERT_EQ("mq_notify", enter_notify_handler, true);

	mq_close(mqdes);

	mqdes = mq_open("noti_queue", O_CREAT | O_RDWR, 0666, 0);
	TC_ASSERT_NEQ("mq_open", mqdes, (mqd_t)-1);

	notification.sigev_notify = SIGEV_SIGNAL;
	notification.sigev_signo = SIGUSR1;
	sa.sa_handler = mq_notify_handler;
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);

	/* Null Message queue descriptor check */
	TC_ASSERT_EQ_ERROR_CLEANUP("mq_notify", mq_notify(NULL, &notification), ERROR, get_errno(), goto cleanup);

	/* Invalid signal number check */
	notification.sigev_signo = 33;
	TC_ASSERT_EQ_ERROR_CLEANUP("mq_notify", mq_notify(mqdes, &notification), ERROR, get_errno(), goto cleanup);

	/* Valid signal number check */
	notification.sigev_signo = SIGUSR1;
	TC_ASSERT_EQ_ERROR_CLEANUP("mq_notify", mq_notify(mqdes, &notification), OK, get_errno(), goto cleanup);
	TC_ASSERT_EQ_ERROR_CLEANUP("mq_notify", mq_notify(mqdes, &notification), ERROR, get_errno(), goto cleanup);

	/* Detaching the notification */
	TC_ASSERT_EQ_ERROR_CLEANUP("mq_notify", mq_notify(mqdes, NULL), OK, get_errno(), goto cleanup);

	mq_close(mqdes);
	mq_unlink("noti_queue");

	TC_SUCCESS_RESULT();
	return;
cleanup:
	mq_close(mqdes);
	mq_unlink("noti_queue");
}

static void tc_mqueue_mq_timedsend_timedreceive(void)
{
	int ret_chk = OK;

	timedsend_check = timedreceive_check = 0;
	ret_chk = timedsend_test();
	TC_ASSERT_EQ("timedsend_test", ret_chk, OK);

	ret_chk = timedreceive_test();
	TC_ASSERT_EQ("timedreceive_test", ret_chk, OK);

	mq_unlink("t_mqueue");
	TC_SUCCESS_RESULT();
}

static void tc_mqueue_mq_unlink(void)
{
	int ret;
	mqd_t mqdes;

	mqdes = mq_open("mqunlink", O_CREAT | O_RDWR, 0666, 0);
	TC_ASSERT_NEQ("mq_open", mqdes, (mqd_t)-1);

	mq_close(mqdes);

	ret = mq_unlink("mqunlink");
	TC_ASSERT_EQ("mq_unlink", ret, OK);

	ret = mq_unlink("mqunlink");
	TC_ASSERT_EQ("mq_unlink", ret, ERROR);
	TC_ASSERT_EQ("mq_unlink", errno, ENOENT);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_mqueue_mq_timedsend_timedreceive_failurechecks
* @description          :Function for tc_mqueue_mq_timedsend_timedreceive corner case checks
* @return               :void*
*/
static void tc_mqueue_mq_timedsend_timedreceive_failurechecks(void)
{
	char msg_buffer[TEST_MSGLEN];
	struct mq_attr attr;
	struct timespec st_time;
	int ret_chk = OK;

	/* TIMEDSEND_TEST */

	clock_gettime(CLOCK_REALTIME, &st_time);
	st_time.tv_sec += 1;

	/* Fill in attributes for message queue */

	attr.mq_maxmsg = TEST_TIMEDRECEIVE_NMSGS;
	attr.mq_msgsize = TEST_MSGLEN;
	attr.mq_flags = 0;

	/* Fill in a test message buffer to send */

	memcpy(msg_buffer, TEST_MESSAGE, TEST_MSGLEN);

	/* Perform the timedsend_test for NULL time parameter check */

	g_timedsend_mqfd = mq_open("t_mqueues", O_WRONLY | O_CREAT, 0666, &attr);
	TC_ASSERT_NEQ("timedsend_test", g_timedsend_mqfd, (mqd_t)ERROR);

	/* Perform the send for above failure check case */

	ret_chk = mq_timedsend(NULL, msg_buffer, TEST_MSGLEN, 42, NULL);
	TC_ASSERT_EQ_CLEANUP("timedsend_test", ret_chk, ERROR, goto cleanup1);
	TC_ASSERT_EQ_CLEANUP("timedsend_test", errno, EINVAL, goto cleanup1);

	/* TIMEDRECEIVE_TEST */

	clock_gettime(CLOCK_REALTIME, &st_time);
	st_time.tv_sec += 1;

	/* Fill in attributes for message queue */

	attr.mq_maxmsg = TEST_TIMEDRECEIVE_NMSGS;
	attr.mq_msgsize = TEST_MSGLEN;
	attr.mq_flags = 0;

	/* Perform the timedreceive_test for time-related errors */

	memset(msg_buffer, 0xaa, TEST_MSGLEN);

	g_timedrecv_mqfd = mq_open("t_mqueuer", O_RDONLY | O_CREAT, 0666, &attr);
	TC_ASSERT_NEQ_CLEANUP("timedreceive_test", g_timedrecv_mqfd, (mqd_t)ERROR, goto cleanup1);

	/* Perform the receive for above failure check cases */

	ret_chk = mq_timedreceive(g_timedrecv_mqfd, msg_buffer, TEST_MSGLEN, 0, &st_time);
	TC_ASSERT_EQ_CLEANUP("timedreceive_test", ret_chk, ERROR, goto cleanup2);

	/* Close the queue and return success */

	TC_ASSERT_GEQ_CLEANUP("timedreceive_test", mq_close(g_timedrecv_mqfd), 0, goto cleanup1);

	clock_gettime(CLOCK_REALTIME, &st_time);
	st_time.tv_sec += 1;

	sleep(10);

	/* Perform the timedreceive_test for expired time & empty message queue cases */

	memset(msg_buffer, 0xaa, TEST_MSGLEN);

	g_timedrecv_mqfd = mq_open("t_mqueuer", O_RDONLY | O_CREAT, 0666, &attr);
	TC_ASSERT_NEQ_CLEANUP("timedreceive_test", g_timedrecv_mqfd, (mqd_t)ERROR, goto cleanup1);

	/* Perform the receive for above failure check cases */

	ret_chk = mq_timedreceive(g_timedrecv_mqfd, msg_buffer, TEST_MSGLEN, 0, &st_time);
	TC_ASSERT_EQ_CLEANUP("timedreceive_test", ret_chk, ERROR, goto cleanup2);

	/* Close the queue and return success */

	TC_ASSERT_GEQ_CLEANUP("timedreceive_test", mq_close(g_timedrecv_mqfd), 0, goto cleanup1);
	TC_ASSERT_GEQ("timedsend_test", mq_close(g_timedsend_mqfd), 0);

	TC_SUCCESS_RESULT();

cleanup2:
	mq_close(g_timedrecv_mqfd);
	mq_unlink("t_mqueuer");
cleanup1:
	mq_close(g_timedsend_mqfd);
	mq_unlink("t_mqueues");
}

/****************************************************************************
 * Name: mqueue
 ****************************************************************************/
int mqueue_main(void)
{
	tc_mqueue_mq_open_close_send_receive();
	tc_mqueue_mq_notify();
	tc_mqueue_mq_timedsend_timedreceive();
	tc_mqueue_mq_timedsend_timedreceive_failurechecks();
	tc_mqueue_mq_unlink();

	return 0;
}
