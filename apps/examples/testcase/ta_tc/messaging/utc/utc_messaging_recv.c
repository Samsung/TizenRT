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

#define TC_BLOCK_PORT "block_port"
#define TC_BLOCK_MSG  "block_msg"

#define TC_NONBLOCK_PORT "nonblock_port"
#define TC_NONBLOCK_MSG  "nonblock_msg"

#define TC_REPLY_MSG "reply_msg"

#define TC_OK   0
#define TC_FAIL 1

static sem_t recv_sem;
static bool tc_block_chk = TC_OK;
static bool tc_nonblock_chk = TC_OK;
static bool tc_reply_chk = TC_OK;
static bool tc_cleanup_chk = false;

static void utc_messaging_recv_block_n(void)
{
	int ret;
	msg_recv_buf_t recv_buf;

	ret = messaging_recv_block(NULL, &recv_buf);
	TC_ASSERT_EQ("messaging_recv_block", ret, ERROR);

	recv_buf.buflen = strlen(TC_BLOCK_MSG) + 1;
	recv_buf.buf = NULL;

	ret = messaging_recv_block(TC_BLOCK_PORT, &recv_buf);
	TC_ASSERT_EQ("messaging_recv_block", ret, ERROR);

	recv_buf.buf = (char *)malloc(strlen(TC_BLOCK_MSG) + 1);
	TC_ASSERT_NEQ("messaging_recv_block", recv_buf.buf, NULL);
	recv_buf.buflen = -1;

	ret = messaging_recv_block(TC_BLOCK_PORT, &recv_buf);
	TC_ASSERT_EQ_CLEANUP("messaging_recv_block", ret, ERROR, free(recv_buf.buf));

	free(recv_buf.buf);
	TC_SUCCESS_RESULT();
}

static void block_sender(int argc, FAR char *argv[])
{
	int ret;
	msg_send_data_t data;

	data.msglen = strlen(TC_BLOCK_MSG) + 1;
	data.priority = 100;
	data.msg = (char *)malloc(data.msglen);
	if (data.msg == NULL) {
		tc_block_chk = TC_FAIL;
		(void)sem_post(&recv_sem);
		return;
	}

	strcpy(data.msg, TC_BLOCK_MSG);

	ret = messaging_send(TC_BLOCK_PORT, &data);
	if (ret != OK) {
		tc_block_chk = TC_FAIL;
		free((void *)data.msg);
		(void)sem_post(&recv_sem);
		return;
	}

	free((void *)data.msg);
}

static void block_receiver(int argc, FAR char *argv[])
{
	int ret;
	msg_recv_buf_t recv_buf;

	ret = sem_wait(&recv_sem);
	if (ret != OK) {
		tc_block_chk = TC_FAIL;
		return;
	}

	recv_buf.buflen = strlen(TC_BLOCK_MSG) + 1;
	recv_buf.buf = (char *)malloc(recv_buf.buflen);
	if (recv_buf.buf == NULL) {
		tc_block_chk = TC_FAIL;
		(void)sem_post(&recv_sem);
		return;
	}

	ret = messaging_recv_block(TC_BLOCK_PORT, &recv_buf);
	if (ret == ERROR || (strncmp(recv_buf.buf, TC_BLOCK_MSG, recv_buf.buflen) != 0)) {
		tc_block_chk = TC_FAIL;
		free(recv_buf.buf);
		(void)sem_post(&recv_sem);
		return;
	}

	free(recv_buf.buf);

	(void)sem_post(&recv_sem);
}

static void utc_messaging_recv_block_p(void)
{
	int ret;

	sem_init(&recv_sem, 0, 1);

	ret = task_create("block_receiver", TASK_PRIO, STACKSIZE, (main_t)block_receiver, (FAR char * const *)NULL);
	TC_ASSERT_GEQ("messaging_recv_block", ret, 0);

	ret = task_create("block_sender", TASK_PRIO, STACKSIZE, (main_t)block_sender, (FAR char * const *)NULL);
	TC_ASSERT_GEQ("messaging_recv_block", ret, 0);

	ret = sem_wait(&recv_sem);
	TC_ASSERT_EQ_CLEANUP("messaging_recv_block", tc_block_chk, TC_OK, sem_destroy(&recv_sem));
	TC_ASSERT_EQ_CLEANUP("messaging_recv_block", ret, OK, sem_destroy(&recv_sem));

	sem_destroy(&recv_sem);
	TC_SUCCESS_RESULT();
}

static void nonblock_callback(msg_reply_type_t msg_type, msg_recv_buf_t *recv_data, void *cb_data)
{
	int ret;
	msg_send_data_t reply;

	if (strncmp(recv_data->buf, TC_NONBLOCK_MSG, strlen(TC_NONBLOCK_MSG) + 1) == 0) {
		tc_nonblock_chk = TC_OK;

		reply.msglen = strlen(TC_REPLY_MSG) + 1;
		reply.msg = (char *)malloc(reply.msglen);
		if (reply.msg == NULL) {
			tc_reply_chk = TC_FAIL;
			return;
		}

		strncpy(reply.msg, TC_REPLY_MSG, strlen(TC_REPLY_MSG) + 1);
		ret = messaging_reply(TC_NONBLOCK_PORT, recv_data->sender_pid, &reply);
		if (ret != OK) {
			tc_reply_chk = TC_FAIL;
		} else {
			tc_reply_chk = TC_OK;
		}
		free(reply.msg);
	} else {
		tc_nonblock_chk = TC_FAIL;
	}
}

static void utc_messaging_recv_nonblock_n(void)
{
	int ret;
	msg_recv_buf_t recv_buf;
	msg_callback_info_t cb_info;
	cb_info.cb_func = nonblock_callback;

	ret = messaging_recv_nonblock(NULL, &recv_buf, &cb_info);
	TC_ASSERT_EQ("messaging_recv_nonblock", ret, ERROR);

	recv_buf.buflen = strlen(TC_NONBLOCK_MSG) + 1;
	recv_buf.buf = NULL;
	ret = messaging_recv_nonblock(TC_NONBLOCK_PORT, &recv_buf, &cb_info);
	TC_ASSERT_EQ("messaging_recv_nonblock", ret, ERROR);

	recv_buf.buf = (char *)malloc(strlen(TC_NONBLOCK_MSG) + 1);
	TC_ASSERT_NEQ("messaging_recv_nonblock", recv_buf.buf, NULL);
	recv_buf.buflen = -1;
	ret = messaging_recv_nonblock(TC_NONBLOCK_PORT, &recv_buf, &cb_info);
	TC_ASSERT_EQ_CLEANUP("messaging_recv_nonblock", ret, ERROR, free(recv_buf.buf));

	recv_buf.buflen = strlen(TC_NONBLOCK_MSG) + 1;
	cb_info.cb_func = NULL;
	ret = messaging_recv_nonblock(TC_NONBLOCK_PORT, &recv_buf, &cb_info);
	TC_ASSERT_EQ_CLEANUP("messaging_recv_nonblock", ret, ERROR, free(recv_buf.buf));

	free(recv_buf.buf);
	TC_SUCCESS_RESULT();
}

static void nonblock_sender(int argc, FAR char *argv[])
{
	int ret;
	msg_send_data_t data;
	msg_recv_buf_t reply;

	data.msglen = strlen(TC_NONBLOCK_MSG) + 1;
	data.msg = (char *)malloc(data.msglen);
	data.priority = 100;
	if (data.msg == NULL) {
		tc_nonblock_chk = TC_FAIL;
		(void)sem_post(&recv_sem);
		return;
	}

	strcpy(data.msg, TC_NONBLOCK_MSG);

	reply.buflen = strlen(TC_REPLY_MSG) + 1;
	reply.buf = (char *)malloc(reply.buflen);
	if (reply.buf == NULL) {
		tc_reply_chk = TC_FAIL;
	}

	ret = messaging_send_sync(TC_NONBLOCK_PORT, &data, &reply);
	if (ret != OK) {
		tc_nonblock_chk = TC_FAIL;
		free(reply.buf);
		free(data.msg);
		(void)sem_post(&recv_sem);
		return;
	}

	if (strncmp(reply.buf, TC_REPLY_MSG, strlen(TC_REPLY_MSG) + 1) != 0) {
		tc_reply_chk = TC_FAIL;
	}

	free(reply.buf);
	free(data.msg);

	(void)sem_post(&recv_sem);
}

static void nonblock_recv(int argc, FAR char *argv[])
{
	int ret;
	msg_recv_buf_t recv_buf;
	msg_callback_info_t cb_info;
	cb_info.cb_func = nonblock_callback;

	tc_cleanup_chk = false;

	ret = sem_wait(&recv_sem);
	if (ret != OK) {
		tc_nonblock_chk = TC_FAIL;
		return;
	}

	recv_buf.buflen = strlen(TC_NONBLOCK_MSG) + 1;
	recv_buf.buf = (char *)malloc(recv_buf.buflen);
	if (recv_buf.buf == NULL) {
		tc_block_chk = TC_FAIL;
		(void)sem_post(&recv_sem);
		return;
	}

	ret = messaging_recv_nonblock(TC_NONBLOCK_PORT, &recv_buf, &cb_info);
	if (ret != OK) {
		tc_nonblock_chk = TC_FAIL;
		free(recv_buf.buf);
		(void)sem_post(&recv_sem);
		return;
	}

	free(recv_buf.buf);

	/* wait not to finish before receiving message. */
	sleep(2);

	ret = messaging_cleanup(TC_NONBLOCK_PORT);
	if (ret == OK) {
		tc_cleanup_chk = true;
	}
}

static void utc_messaging_recv_nonblock_p(void)
{
	int ret;

	sem_init(&recv_sem, 0, 1);

	ret = task_create("nonblock_recv", TASK_PRIO, STACKSIZE, (main_t)nonblock_recv, (FAR char * const *)NULL);
	TC_ASSERT_GEQ("messaging_recv_nonblock", ret, 0);

	ret = task_create("nonblock_sender", TASK_PRIO, STACKSIZE, (main_t)nonblock_sender, (FAR char * const *)NULL);
	TC_ASSERT_GEQ("messaging_recv_nonblock", ret, 0);

	ret = sem_wait(&recv_sem);
	TC_ASSERT_EQ_CLEANUP("messaging_recv_nonblock", tc_nonblock_chk, TC_OK, sem_destroy(&recv_sem); messaging_cleanup(TC_NONBLOCK_PORT));
	TC_ASSERT_EQ_CLEANUP("messaging_recv_nonblock", ret, OK, sem_destroy(&recv_sem); messaging_cleanup(TC_NONBLOCK_PORT));

	sem_destroy(&recv_sem);
	TC_SUCCESS_RESULT();
}

static void utc_messaging_reply_n(void)
{
	int ret;
	pid_t invalid_pid = -1;

	ret = messaging_reply(NULL, invalid_pid, NULL);
	TC_ASSERT_EQ("messaging_reply", ret, ERROR);

	TC_SUCCESS_RESULT();
}

static void utc_messaging_reply_p(void)
{
	/* messaging_reply test is belongs to utc_messaging_recv_nonblock_p(). */
	TC_ASSERT_EQ("messaging_reply", tc_reply_chk, TC_OK);
	TC_SUCCESS_RESULT();
}

static void utc_messaging_cleanup_n(void)
{
	int ret;
	ret = messaging_cleanup(NULL);
	TC_ASSERT_EQ("messaging_cleanup", ret, ERROR);

	TC_SUCCESS_RESULT();
}

static void utc_messaging_cleanup_p(void)
{
	TC_ASSERT_EQ("messaging_cleanup", tc_cleanup_chk, true);

	TC_SUCCESS_RESULT();
}

void utc_messaging_recv_reply_and_cleanup_main(void)
{
	utc_messaging_recv_block_n();
	utc_messaging_recv_block_p();

	utc_messaging_recv_nonblock_n();
	utc_messaging_recv_nonblock_p();

	utc_messaging_reply_n();
	utc_messaging_reply_p();

	utc_messaging_cleanup_n();
	utc_messaging_cleanup_p();
}
