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
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <unistd.h>
#include <debug.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <mqueue.h>
#include <time.h>
#include <sys/types.h>
#include <tinyara/clock.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

/****************************************************************************
 * task_manager_interface
 ****************************************************************************/
int taskmgr_send_request(tm_request_t *request_msg)
{
	int status;
	struct mq_attr attr;
	mqd_t g_tm_send_mqfd;

	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(tm_request_t);
	attr.mq_flags = 0;

	g_tm_send_mqfd = mq_open(TM_PUBLIC_MQ, O_WRONLY | O_CREAT, 0666, &attr);
	if (g_tm_send_mqfd == (mqd_t)ERROR) {
		tmdbg("mq_open failed!\n");
		return TM_COMMUCATION_FAIL;
	}

	status = mq_send(g_tm_send_mqfd, (const char *)request_msg, sizeof(tm_request_t), TM_MQ_PRIO);
	if (status < 0) {
		tmdbg("mq_send failed! %d\n", errno);
		mq_close(g_tm_send_mqfd);
		return TM_COMMUCATION_FAIL;
	}

	mq_close(g_tm_send_mqfd);

	return OK;
}

int taskmgr_send_response(char *q_name, tm_response_t *response_msg)
{
	int status;
	mqd_t private_mqfd;
	struct mq_attr attr;

	if (q_name == NULL) {
		return TM_INVALID_PARAM;
	}

	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(tm_response_t);
	attr.mq_flags = 0;

	private_mqfd = mq_open(q_name, O_WRONLY | O_CREAT, 0666, &attr);
	if (private_mqfd == (mqd_t)ERROR) {
		tmdbg("mq_open failed!\n");
		return TM_COMMUCATION_FAIL;
	}

	status = mq_send(private_mqfd, (char *)response_msg, sizeof(tm_response_t), TM_MQ_PRIO);
	if (status != OK) {
		tmdbg("mq_send failed! %d\n", errno);
		mq_close(private_mqfd);
		return TM_COMMUCATION_FAIL;
	}

	mq_close(private_mqfd);

	return OK;
}

int taskmgr_receive_response(char *q_name, tm_response_t *response_msg, int timeout)
{
	int status;
	mqd_t private_mqfd;
	FAR struct timespec time;
	struct mq_attr attr;

	if (q_name == NULL) {
		return TM_INVALID_PARAM;
	}

	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(tm_response_t);
	attr.mq_flags = 0;

	private_mqfd = mq_open(q_name, O_RDONLY | O_CREAT, 0666, &attr);
	if (private_mqfd == (mqd_t)ERROR) {
		tmdbg("mq_open failed!\n");
		return TM_COMMUCATION_FAIL;
	}

	if (timeout == TM_RESPONSE_WAIT_INF) {
		status = mq_receive(private_mqfd, (char *)response_msg, sizeof(tm_response_t), 0);
	} else {
		status = taskmgr_calc_time(&time, timeout);
		if (status != OK) {
			mq_close(private_mqfd);
			mq_unlink(q_name);
			return TM_COMMUCATION_FAIL;
		}
		status = mq_timedreceive(private_mqfd, (char *)response_msg, sizeof(tm_response_t), NULL, &time);
	}

	mq_close(private_mqfd);
	mq_unlink(q_name);

	if (status <= 0) {
		tmdbg("mq_receive failed! %d\n", errno);
		return TM_COMMUCATION_FAIL;
	}

	return response_msg->status;
}
