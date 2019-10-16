/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * apps/examples/heavy_signal_message_test/heavy_signal_message_test_snder.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
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
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <pthread.h>

#include <sys/types.h>

#include "heavy_signal_message_test.h"

static int sig_mq_num;
static int mq_num;
static bool running;
static int total_cnt;
static int succese_cnt;

static void *heavy_signal_message_test_sig_send(void)
{
	int ret;
	int recv_pid;
	int pid;
	mqd_t fd;
	struct mq_attr attr;
	char q_name[HEAVY_SIGNAL_MESSAGE_TEST_MQ_NAME_MAX];
	union sigval value;

	total_cnt++;
	pid = getpid();
	printf("[%d] heavy_signal_message_test_sig_send pthread \n", pid);

	attr.mq_maxmsg = HEAVY_SIGNAL_MESSAGE_TEST_MAX_MSG;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags = 0;

	snprintf(q_name, HEAVY_SIGNAL_MESSAGE_TEST_MQ_NAME_MAX, "%s%d", HEAVY_SIGNAL_MESSAGE_TEST_SIG_MQ, sig_mq_num++);

	fd = mq_open(q_name, O_RDWR | O_CREAT, 0666, &attr);
	if (fd < 0) {
		printf("Failed to open message queue\n");
		return 0;
	}

	/* sender receive a receiver's pid. */
	ret = mq_receive(fd, (char *)&recv_pid, sizeof(int), NULL);
	if (ret < 0) {
		printf("Failed to send message\n");
		mq_close(fd);
		mq_unlink(q_name);
		return 0;
	}
	mq_close(fd);
	mq_unlink(q_name);

	while (running == true) {
		(void)sigqueue(recv_pid, HEAVY_SIGNAL_MESSAGE_TEST_SIG_WAIT_NO, value);
		printf("[%d] send signal to %d\n", pid, recv_pid);
		usleep(HEAVY_SIGNAL_MESSAGE_TEST_USLEEP);
	}

	succese_cnt++;
	printf("[%d] heavy_signal_message_test_sig_send pthread stop\n", pid);
	return 0;
}

static void *heavy_signal_message_test_mq_send(void)
{
	int ret;
	int pid;
	mqd_t fd;
	struct mq_attr attr;
	char q_name[HEAVY_SIGNAL_MESSAGE_TEST_MQ_NAME_MAX];
	char buffer[HEAVY_SIGNAL_MESSAGE_TEST_BUFFER_SIZE + 1];

	total_cnt++;
	pid = getpid();
	printf("[%d] heavy_signal_message_test_mq_send pthread \n", pid);

	attr.mq_maxmsg = HEAVY_SIGNAL_MESSAGE_TEST_MAX_MSG;
	attr.mq_msgsize = HEAVY_SIGNAL_MESSAGE_TEST_BUFFER_SIZE;
	attr.mq_flags = 0;

	/* sender and receiver open the same mq. safe_mq1, safe_mq2 ....*/
	snprintf(q_name, HEAVY_SIGNAL_MESSAGE_TEST_MQ_NAME_MAX, "%s%d", HEAVY_SIGNAL_MESSAGE_TEST_MQ, mq_num++);

	fd = mq_open(q_name, O_RDWR | O_CREAT, 0666, &attr);
	if (fd < 0) {
		printf("Failed to open message queue\n");
		return 0;
	}

	for (int i = 0; i < HEAVY_SIGNAL_MESSAGE_TEST_BUFFER_SIZE; ++i) {
		buffer[i] = 'a' + (char)mq_num;
	}
	buffer[HEAVY_SIGNAL_MESSAGE_TEST_BUFFER_SIZE] = 0;

	while (running == true) {
		ret = mq_send(fd, (char *)&buffer, HEAVY_SIGNAL_MESSAGE_TEST_BUFFER_SIZE, 100);
		if (ret < 0) {
			printf("Failed to send\n");
			mq_close(fd);
			mq_unlink(q_name);
			return 0;
		}
		printf("[%d] send message\n", pid);
		usleep(HEAVY_SIGNAL_MESSAGE_TEST_USLEEP);
	}

	succese_cnt++;
	printf("[%d] heavy_signal_message_test_mq_send pthread stop\n", pid);

	mq_close(fd);
	mq_unlink(q_name);
	return 0;
}

static void *heavy_signal_message_test_usleep(void)
{
	int pid;

	total_cnt++;
	pid = getpid();
	printf("[%d] heavy_signal_message_test_usleep_pthread \n", pid);

	while (running == true) {
		printf("[%d] usleep\n", pid);
		usleep(HEAVY_SIGNAL_MESSAGE_TEST_USLEEP);
	}

	succese_cnt++;
	printf("[%d] heavy_signal_message_test_usleep_pthread stop\n", pid);
	return 0;
}

void heavy_signal_message_test_sender(void)
{
	int i;
	int ret;
	pthread_t pid;
	mqd_t fd;
	int command;
	struct timespec start_time;
	struct timespec end_time;
	struct mq_attr attr;
	bool is_stop = false;
	char q_name[HEAVY_SIGNAL_MESSAGE_TEST_MQ_NAME_MAX];

	total_cnt = 0;
	succese_cnt = 0;
	running = true;

	ret = clock_gettime(CLOCK_REALTIME, &start_time);
	if (ret != OK) {
		printf("Fail to clock_gettime");
		return;
	}

	for (i = 0; i < HEAVY_SIGNAL_MESSAGE_TEST_TASK_NUM; ++i) {
		ret = pthread_create(&pid, NULL, (pthread_startroutine_t)heavy_signal_message_test_sig_send, NULL);
		if (ret < 0) {
			printf("heavy_signal_message_test_sig_send pthread create FAIL\n");
			return;
		}
		pthread_setname_np(pid, "sig_send");
		pthread_detach(pid);

		ret = pthread_create(&pid, NULL, (pthread_startroutine_t)heavy_signal_message_test_mq_send, NULL);
		if (ret < 0) {
			printf("heavy_signal_message_test_mq_send pthread create FAIL\n");
			return;
		}
		pthread_setname_np(pid, "mq_send");
		pthread_detach(pid);

		ret = pthread_create(&pid, NULL, (pthread_startroutine_t)heavy_signal_message_test_usleep, NULL);
		if (ret < 0) {
			printf("heavy_signal_message_test_usleep pthread create FAIL\n");
			return;
		}
		pthread_setname_np(pid, "usleep");
		pthread_detach(pid);
	}

	attr.mq_maxmsg = HEAVY_SIGNAL_MESSAGE_TEST_MAX_MSG;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags = 0;

	strncpy(q_name, HEAVY_SIGNAL_MESSAGE_TEST_SEND_STOP_MQ, HEAVY_SIGNAL_MESSAGE_TEST_MQ_NAME_MAX);

	fd = mq_open(q_name, O_RDWR | O_CREAT, 0666, &attr);
	if (fd < 0) {
		bmdbg("Failed to open message queue\n");
		return;
	}

	while (is_stop != true) {
		ret = mq_receive(fd, (char *)&command, sizeof(int), NULL);
		if (ret < 0) {
			printf("Failed to send message\n");
			mq_close(fd);
			mq_unlink(q_name);
			return;
		}

		switch (command) {
		case HEAVY_SIGNAL_MESSAGE_TEST_STOP:
			running = false;
			is_stop = true;
			break;
		default:
			break;
		}
	}

	ret = clock_gettime(CLOCK_REALTIME, &end_time);
	if (ret != OK) {
		printf("Fail to clock_gettime");
		return;
	}

	/* Wait for pthread to exit. */
	sleep(1);

	printf("heavy_signal_message_test_sender task stop\n");
	printf("sender test total task count = [%d], succese taks count = [%d]\n", total_cnt, succese_cnt);
	printf("sender running time = [%ds]\n", end_time.tv_sec - start_time.tv_sec);

	mq_close(fd);
	mq_unlink(q_name);
}
