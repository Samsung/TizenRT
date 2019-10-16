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
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "tc_common.h"

#define TASK_PRIO 101
#define STACKSIZE 2048
#define MSG_PRIO 100

#define TC_MULTI_PORT "multi_port"
#define TC_MULTI_MSG  "multi_msg"

#define TC_OK   0
#define TC_FAIL 1

static bool tc_multi_chk = TC_OK;
static int tc_multi_count = 0;
static bool tc_cleanup_chk = false;

static sem_t multi_sem;

static void multi_recv_callback(msg_reply_type_t msg_type, msg_recv_buf_t *recv_data, void *cb_data)
{
	if (recv_data == NULL) {
		tc_multi_chk = TC_FAIL;
		return;
	}

	if (strncmp(recv_data->buf, TC_MULTI_MSG, strlen(TC_MULTI_MSG) + 1) != 0) {
		tc_multi_chk = TC_FAIL;
		return;
	}
	tc_multi_count++;
}

static int multi_recv_nonblock(int argc, FAR char *argv[])
{
	int ret;
	msg_callback_info_t cb_info;
	msg_recv_buf_t multi_data;

	cb_info.cb_func = multi_recv_callback;
	cb_info.cb_data = NULL;

	multi_data.buflen = strlen(TC_MULTI_MSG) + 1;
	multi_data.buf = (char *)malloc(multi_data.buflen);
	if (multi_data.buf == NULL) {
		tc_multi_chk = TC_FAIL;
		return ERROR;
	}

	ret = messaging_recv_nonblock(TC_MULTI_PORT, &multi_data, &cb_info);
	if (ret != OK) {
		tc_multi_chk = TC_FAIL;
		free(multi_data.buf);
		return ERROR;
	}

	/* Wait not to finish this task, because of receiving data through the callback. */
	sleep(5);

	free(multi_data.buf);

	ret = messaging_cleanup(TC_MULTI_PORT);
	if (ret == OK) {
		tc_cleanup_chk = true;
	}
	return OK;
}

static int multi_recv_block(int argc, FAR char *argv[])
{
	int ret;
	msg_recv_buf_t msg;

	msg.buflen = strlen(TC_MULTI_MSG) + 1;
	msg.buf = (char *)malloc(msg.buflen);
	if (msg.buf == NULL) {
		tc_multi_chk = TC_FAIL;
		return ERROR;
	}

	ret = messaging_recv_block(TC_MULTI_PORT, &msg);
	if (ret < 0) {
		goto errout;
	}

	if (strncmp(msg.buf, TC_MULTI_MSG, strlen(TC_MULTI_MSG) + 1) != 0) {
		goto errout;
	}

	tc_multi_count++;
	free(msg.buf);

	return OK;
errout:
	free(msg.buf);
	tc_multi_chk = TC_FAIL;
	return ERROR;
}

static int multi_sender(int argc, FAR char *argv[])
{
	int ret;
	msg_send_data_t multi_data;

	sem_wait(&multi_sem);

	multi_data.priority = MSG_PRIO;
	multi_data.msglen = strlen(TC_MULTI_MSG) + 1;
	multi_data.msg = (char *)malloc(multi_data.msglen);
	if (multi_data.msg == NULL) {
		goto errout;
	}

	strncpy(multi_data.msg, TC_MULTI_MSG, strlen(TC_MULTI_MSG) + 1);

	ret = messaging_multicast(TC_MULTI_PORT, &multi_data);
	if (ret != 3) {
		free(multi_data.msg);
		goto errout;
	}

	free(multi_data.msg);
	sem_post(&multi_sem);
	return 0;
errout:
	tc_multi_chk = TC_FAIL;
	sem_post(&multi_sem);
	return ERROR;
}


static void utc_messaging_multicast_n(void)
{
	int ret;
	msg_send_data_t multi_data;

	multi_data.priority = MSG_PRIO;
	multi_data.msglen = strlen(TC_MULTI_MSG) + 1;
	multi_data.msg = (char *)malloc(multi_data.msglen);
	TC_ASSERT_NEQ("messaging_multicast", multi_data.msg, NULL);

	strncpy(multi_data.msg, TC_MULTI_MSG, strlen(TC_MULTI_MSG) + 1);

	ret = messaging_multicast(NULL, &multi_data);
	TC_ASSERT_EQ_CLEANUP("messaging_multicast", ret, ERROR, free(multi_data.msg));

	ret = messaging_multicast(TC_MULTI_PORT, NULL);
	TC_ASSERT_EQ_CLEANUP("messaging_multicast", ret, ERROR, free(multi_data.msg));

	free(multi_data.msg);
	TC_SUCCESS_RESULT();
}

static void utc_messaging_multicast_p(void)
{
	int ret;
	int recv_block1;
	int recv_block2;
	int recv_nonblock;
	int sender_pid;

	sem_init(&multi_sem, 0, 1);
	tc_multi_count = 0;
	recv_nonblock = task_create("multi_recv1", TASK_PRIO, STACKSIZE, multi_recv_nonblock, NULL);
	TC_ASSERT_GEQ_CLEANUP("messaging_multicast", recv_nonblock, 0, sem_destroy(&multi_sem));

	recv_block1 = task_create("multi_recv2", TASK_PRIO, STACKSIZE, multi_recv_block, NULL);
	TC_ASSERT_GEQ_CLEANUP("messaging_multicast", recv_block1, 0, sem_destroy(&multi_sem));

	recv_block2 = task_create("multi_recv3", TASK_PRIO, STACKSIZE, multi_recv_block, NULL);
	TC_ASSERT_GEQ_CLEANUP("messaging_multicast", recv_block2, 0, sem_destroy(&multi_sem));

	sender_pid = task_create("multi_sender", TASK_PRIO, STACKSIZE, multi_sender, NULL);
	TC_ASSERT_GEQ_CLEANUP("messaging_multicast", sender_pid, 0, sem_destroy(&multi_sem));

	ret = sem_wait(&multi_sem);
	TC_ASSERT_EQ_CLEANUP("messaging_multicast", tc_multi_count, 3, sem_destroy(&multi_sem));
	TC_ASSERT_EQ_CLEANUP("messaging_multicast", tc_multi_chk, TC_OK, sem_destroy(&multi_sem));
	TC_ASSERT_EQ_CLEANUP("messaging_multicast", ret, OK, sem_destroy(&multi_sem));

	(void)sem_destroy(&multi_sem);

	TC_ASSERT_EQ("messging_multicast", tc_cleanup_chk, true);

	TC_SUCCESS_RESULT();
}

void utc_messaging_multicast_main(void)
{
	utc_messaging_multicast_n();
	utc_messaging_multicast_p();
}
