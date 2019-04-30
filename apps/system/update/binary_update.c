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

/***************************************************************************
 * Included Files
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <debug.h>
#include <sched.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <mqueue.h>
#include <tinyara/binary_manager.h>

#include <apps/system/binary_update.h>

static int send_request(binmgr_request_t *request_msg)
{
	mqd_t binmgr_mq;
	binmgr_mq = mq_open(BINMGR_REQUEST_MQ, O_WRONLY, 0666, 0);
	if (binmgr_mq == (mqd_t)ERROR) {
		bmdbg("mq_open failed!\n");
		return ERROR;
	}

	if (mq_send(binmgr_mq, (const char *)request_msg, sizeof(binmgr_request_t), BINMGR_NORMAL_PRIO) == ERROR) {
		mq_close(binmgr_mq);
		bmdbg("mq_send failed!\n");
		return ERROR;
	}

	mq_close(binmgr_mq);
	return OK;
}

static int receive_response(mqd_t private_mq, binmgr_response_t *response_msg)
{
	int nbytes;

	nbytes = mq_receive(private_mq, (char *)response_msg, sizeof(binmgr_response_t), 0);
	if (nbytes <= 0) {
		bmdbg("receive ERROR %d, errno %d, retry!\n", nbytes, errno);
		return ERROR;
	}

	return OK;
}

static void copy_binary_info(binary_info_t *binary_info, binmgr_response_t *response_msg)
{
	binary_info->part_size = response_msg->part_size;
	strncpy(binary_info->name, response_msg->name, BIN_NAME_MAX);
	strncpy(binary_info->version, response_msg->version, BIN_VERSION_MAX);
	strncpy(binary_info->dev_path, response_msg->dev_path, BINMGR_DEVNAME_LEN);

	return;
}

int load_new_binary(char *binary_name)
{
	binmgr_request_t request_msg;

	if (binary_name == NULL || strlen(binary_name) > BIN_NAME_MAX - 1) {
		bmdbg("load_new_binary failed : invalid binary name\n");
		return ERROR;
	}

	request_msg.cmd = BINMGR_RELOAD;
	request_msg.pid = getpid();
	snprintf(request_msg.b_name, BIN_NAME_MAX, "%s", binary_name);

	if (send_request(&request_msg) == ERROR) {
		bmdbg("mq_send failed!\n");
		return ERROR;
	}
	return OK;
}

int get_binary_info(char *binary_name, binary_info_t *binary_info)
{
	binmgr_request_t request_msg;
	binmgr_response_t response_msg;
	mqd_t private_mq;
	struct mq_attr attr;

	if (binary_name == NULL || binary_info == NULL) {
		bmdbg("get_binary_info failed : invalid param.\n");
		return ERROR;
	}

	request_msg.cmd = BINMGR_GET_INFO;
	request_msg.pid = getpid();
	snprintf(request_msg.q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, request_msg.pid);
	snprintf(request_msg.b_name, BIN_NAME_MAX, "%s", binary_name);

	if (send_request(&request_msg) == ERROR) {
		bmdbg("mq_send failed!\n");
		return ERROR;
	}

	attr.mq_maxmsg = BINMGR_MAX_MSG;
	attr.mq_msgsize = sizeof(binmgr_response_t);
	attr.mq_flags = 0;

	private_mq = mq_open(request_msg.q_name, O_RDONLY | O_CREAT, 0666, &attr);
	if (private_mq == (mqd_t)ERROR) {
		bmdbg("mq_open failed!\n");
		return ERROR;
	}

	if (receive_response(private_mq, &response_msg) == ERROR) {
		mq_close(private_mq);
		mq_unlink(request_msg.q_name);
		return ERROR;
	}

	mq_close(private_mq);
	mq_unlink(request_msg.q_name);

	if (response_msg.cmd == BINMGR_RESPONSE_INVALID) {
		bmdbg("There is no binary information.!\n");
		return ERROR;
	}

	copy_binary_info(binary_info, &response_msg);

	return OK;
}

int get_binary_info_all(binary_info_list_t *binary_info_list)
{
	mqd_t private_mq;

	binmgr_request_t request_msg;
	binmgr_response_t response_msg;
	binary_info_list_t *next_list;

	struct mq_attr attr;

	if (binary_info_list == NULL) {
		bmdbg("get_binary_info_all fail : invalid param.\n");
		return ERROR;
	}

	request_msg.cmd = BINMGR_GET_INFO_ALL;
	request_msg.pid = getpid();

	snprintf(request_msg.q_name, BIN_NAME_MAX, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, request_msg.pid);

	if (send_request(&request_msg) == ERROR) {
		bmdbg("Fail to get all information of binaries.\n");
		return ERROR;
	}

	attr.mq_maxmsg = BINMGR_MAX_MSG;
	attr.mq_msgsize = sizeof(binmgr_response_t);
	attr.mq_flags = 0;

	private_mq = mq_open(request_msg.q_name, O_RDONLY | O_CREAT, 0666, &attr);
	if (private_mq == (mqd_t)ERROR) {
		bmdbg("mq_open failed!\n");
		return ERROR;
	}

	if (receive_response(private_mq, &response_msg) == ERROR) {
		mq_close(private_mq);
		mq_unlink(request_msg.q_name);
		return ERROR;
	}

	copy_binary_info(&(binary_info_list->binary_info), &response_msg);

	next_list = binary_info_list;
	if (response_msg.cmd == BINMGR_RESPONSE_INVALID) {
		bmdbg("There is no binary information.!\n");
		mq_close(private_mq);
		mq_unlink(request_msg.q_name);
		return ERROR;
	}

	while (response_msg.cmd == BINMGR_RESPONSE_CONTINUE) {
		if (receive_response(private_mq, &response_msg) == ERROR) {
			clean_binary_info_list(binary_info_list);
			mq_close(private_mq);
			mq_unlink(request_msg.q_name);
			return ERROR;
		}
		next_list->next = (binary_info_list_t *)malloc(sizeof(binary_info_list_t));
		if (next_list->next == NULL) {
			bmdbg("malloc for binaries list failed!\n");
			clean_binary_info_list(binary_info_list);
			mq_close(private_mq);
			mq_unlink(request_msg.q_name);
			return ERROR;
		}
		next_list = next_list->next;
		next_list->next = NULL;
		copy_binary_info(&(next_list->binary_info), &response_msg);
	}

	mq_close(private_mq);
	mq_unlink(request_msg.q_name);
	return OK;
}

void clean_binary_info_list(binary_info_list_t *binary_info_list)
{
	if (binary_info_list != NULL && binary_info_list->next != NULL) {
		clean_binary_info_list(binary_info_list->next);
		free(binary_info_list->next);
		binary_info_list->next = NULL;
	}

	return;
}
