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

#define TC_SEND_PORT "send_port"
#define TC_SEND_MSG  "send_msg"

#define TC_SYNC_PORT "sync_port"
#define TC_ASYNC_PORT "async_port"

#define TC_REPLY_MSG "reply_msg"

#define TC_OK   0
#define TC_FAIL 1

#define SYNC_TEST  0
#define ASYNC_TEST 1

static sem_t send_sem;
static bool tc_send_chk = TC_OK;
static bool tc_sync_chk = TC_OK;
static bool sync_async_flag = SYNC_TEST;

static void async_callback(msg_reply_type_t msg_type, msg_recv_buf_t *recv_data, void *cb_data)
{
	if (strncmp(recv_data->buf, TC_REPLY_MSG, strlen(TC_REPLY_MSG) + 1) != 0) {
		tc_send_chk = TC_FAIL;
		(void)sem_post(&send_sem);
		return;
	}
	tc_send_chk = TC_OK;
}

static void noreply_send(int argc, FAR char *argv[])
{
	int ret;
	msg_send_data_t data;

	data.msglen = strlen(TC_SEND_MSG) + 1;
	data.msg = (char *)malloc(data.msglen);
	data.priority = 100;
	if (data.msg == NULL) {
		tc_send_chk = TC_FAIL;
		(void)sem_post(&send_sem);
		return;
	}

	strcpy(data.msg, TC_SEND_MSG);

	ret = messaging_send(TC_SEND_PORT, &data);
	if (ret != OK) {
		tc_send_chk = TC_FAIL;
		free(data.msg);
		(void)sem_post(&send_sem);
		return;
	}

	free(data.msg);
	(void)sem_post(&send_sem);
}

static void noreply_recv(int argc, FAR char *argv[])
{
	int ret;
	msg_recv_buf_t recv_buf;

	ret = sem_wait(&send_sem);
	if (ret != OK) {
		tc_send_chk = TC_FAIL;
		return;
	}

	recv_buf.buflen = strlen(TC_SEND_MSG) + 1;
	recv_buf.buf = (char *)malloc(recv_buf.buflen);
	if (recv_buf.buf == NULL) {
		tc_send_chk = TC_FAIL;
		sem_post(&send_sem);
		return;
	}

	ret = messaging_recv_block(TC_SEND_PORT, &recv_buf);
	if (ret == ERROR) {
		tc_send_chk = TC_FAIL;
		goto cleanup_return;
	}

	if (strncmp(TC_SEND_MSG, recv_buf.buf, recv_buf.buflen) != 0) {
		tc_send_chk = TC_FAIL;
		goto cleanup_return;
	}

	/* wait not to finish before receiving message. */
	sleep(2);

	tc_send_chk = TC_OK;
cleanup_return:
	free(recv_buf.buf);
	(void)sem_post(&send_sem);
	return;
}

static void send_sync(int argc, FAR char *argv[])
{
	int ret;
	msg_send_data_t data;
	msg_recv_buf_t reply;

	data.msglen = strlen(TC_SEND_MSG) + 1;
	data.msg = (char *)malloc(data.msglen);
	data.priority = 100;
	if (data.msg == NULL) {
		tc_send_chk = TC_FAIL;
		(void)sem_post(&send_sem);
		return;
	}

	strcpy(data.msg, TC_SEND_MSG);

	reply.buflen = strlen(TC_REPLY_MSG) + 1;
	reply.buf = (char *)malloc(reply.buflen);
	if (reply.buf == NULL) {
		tc_send_chk = TC_FAIL;
		free(data.msg);
		(void)sem_post(&send_sem);
		return;
	}

	ret = messaging_send_sync(TC_SYNC_PORT, &data, &reply);
	if (ret != OK) {
		tc_send_chk = TC_FAIL;
		free(reply.buf);
		free(data.msg);
		(void)sem_post(&send_sem);
		return;
	}

	free(data.msg);
	if (strncmp(reply.buf, TC_REPLY_MSG, strlen(TC_REPLY_MSG) + 1) != 0) {
		tc_send_chk = TC_FAIL;
		free(reply.buf);
		(void)sem_post(&send_sem);
		return;
	}
	tc_send_chk = TC_OK;
	free(reply.buf);
	(void)sem_post(&send_sem);
}

static void send_async(int argc, FAR char *argv[])
{
	int ret;
	msg_send_data_t data;
	msg_recv_buf_t reply;
	msg_callback_info_t cb_info;

	data.msglen = strlen(TC_SEND_MSG) + 1;
	data.msg = (char *)malloc(data.msglen);
	data.priority = 100;
	if (data.msg == NULL) {
		tc_send_chk = TC_FAIL;
		(void)sem_post(&send_sem);
		return;
	}

	strcpy(data.msg, TC_SEND_MSG);

	reply.buflen = strlen(TC_REPLY_MSG) + 1;
	reply.buf = (char *)malloc(reply.buflen);
	if (reply.buf == NULL) {
		tc_send_chk = TC_FAIL;
		free(data.msg);
		(void)sem_post(&send_sem);
		return;
	}

	cb_info.cb_func = async_callback;
	ret = messaging_send_async(TC_ASYNC_PORT, &data, &reply, &cb_info);
	if (ret != OK) {
		tc_send_chk = TC_FAIL;
		free(reply.buf);
		free(data.msg);
		(void)sem_post(&send_sem);
		return;
	}

	free(data.msg);

	sleep(2);
	free(reply.buf);
	ret = messaging_cleanup(TC_ASYNC_PORT);
	if (ret != OK) {
		tc_send_chk = TC_FAIL;
	}

	(void)sem_post(&send_sem);
}

static void reply_recv(int argc, FAR char *argv[])
{
	int ret;
	msg_recv_buf_t recv_buf;
	msg_send_data_t reply_data;

	ret = sem_wait(&send_sem);
	if (ret != OK) {
		tc_send_chk = TC_FAIL;
		return;
	}

	recv_buf.buflen = strlen(TC_SEND_MSG) + 1;
	recv_buf.buf = (char *)malloc(recv_buf.buflen);
	if (recv_buf.buf == NULL) {
		tc_send_chk = TC_FAIL;
		sem_post(&send_sem);
		return;
	}

	if (sync_async_flag == SYNC_TEST) {
		ret = messaging_recv_block(TC_SYNC_PORT, &recv_buf);
	} else {
		ret = messaging_recv_block(TC_ASYNC_PORT, &recv_buf);
	}
	if (ret == ERROR) {
		tc_send_chk = TC_FAIL;
		goto cleanup_return;
	}

	if (strncmp(TC_SEND_MSG, recv_buf.buf, recv_buf.buflen) != 0) {
		tc_send_chk = TC_FAIL;
		goto cleanup_return;
	}

	reply_data.msglen = strlen(TC_REPLY_MSG) + 1;
	reply_data.msg = (char *)malloc(reply_data.msglen);
	if (reply_data.msg == NULL) {
		tc_send_chk = TC_FAIL;
		goto cleanup_return;
	}

	strncpy(reply_data.msg, TC_REPLY_MSG, strlen(TC_REPLY_MSG) + 1);
	if (sync_async_flag == SYNC_TEST) {
		ret = messaging_reply(TC_SYNC_PORT, recv_buf.sender_pid, &reply_data);
	} else {
		ret = messaging_reply(TC_ASYNC_PORT, recv_buf.sender_pid, &reply_data);
	}
	if (ret != OK) {
		tc_send_chk = TC_FAIL;
		goto cleanup_return;
	}

	/* wait not to finish before receiving message. */
	sleep(2);

	tc_send_chk = TC_OK;
cleanup_return:
	free(recv_buf.buf);
	free(reply_data.msg);
	(void)sem_post(&send_sem);
	return;
}

static void utc_messaging_send_sync_n(void)
{
	int ret;
	msg_send_data_t send_data;
	msg_recv_buf_t reply;

	send_data.msglen = strlen(TC_SEND_MSG) + 1;
	send_data.msg = (char *)malloc(send_data.msglen);
	TC_ASSERT_NEQ("messaging_send_sync", send_data.msg, NULL);

	strncpy(send_data.msg, TC_SEND_MSG, send_data.msglen);

	reply.buflen = strlen(TC_REPLY_MSG) + 1;
	reply.buf = (char *)malloc(reply.buflen);
	TC_ASSERT_NEQ_CLEANUP("messaging_send_sync", reply.buf, NULL, free(send_data.msg));

	ret = messaging_send_sync(NULL, &send_data, &reply);
	TC_ASSERT_EQ_CLEANUP("messaging_send_sync", ret, ERROR, free(send_data.msg); free(reply.buf));

	free(reply.buf);
	reply.buf = NULL;
	ret = messaging_send_sync(TC_SYNC_PORT, &send_data, &reply);
	TC_ASSERT_EQ_CLEANUP("messaging_send_sync", ret, ERROR, free(send_data.msg));

	free(send_data.msg);
	send_data.msg = NULL;
	ret = messaging_send_sync(TC_SYNC_PORT, &send_data, &reply);
	TC_ASSERT_EQ("messaging_send_sync", ret, ERROR);

	TC_SUCCESS_RESULT();
}

static void utc_messaging_send_sync_p(void)
{
	int ret;

	sync_async_flag = SYNC_TEST;
	sem_init(&send_sem, 0, 1);

	ret = task_create("reply_recv", TASK_PRIO, STACKSIZE, (main_t)reply_recv, (FAR char * const *)NULL);
	TC_ASSERT_GEQ("messaging_send_sync", ret, 0);

	ret = task_create("send_sync", TASK_PRIO, STACKSIZE, (main_t)send_sync, (FAR char * const *)NULL);
	TC_ASSERT_GEQ("messaging_send_sync", ret, 0);

	ret = sem_wait(&send_sem);
	TC_ASSERT_EQ_CLEANUP("messaging_send_sync", tc_sync_chk, TC_OK, sem_destroy(&send_sem));
	TC_ASSERT_EQ_CLEANUP("messaging_send_sync", ret, OK, sem_destroy(&send_sem));

	sem_destroy(&send_sem);
	TC_SUCCESS_RESULT();
}

static void utc_messaging_send_async_n(void)
{
	int ret;
	msg_send_data_t send_data;
	msg_recv_buf_t reply;
	msg_callback_info_t cb_info;

	send_data.msglen = strlen(TC_SEND_MSG) + 1;
	send_data.msg = (char *)malloc(send_data.msglen);
	TC_ASSERT_NEQ("messaging_send_async", send_data.msg, NULL);

	strncpy(send_data.msg, TC_SEND_MSG, send_data.msglen);

	cb_info.cb_func = async_callback;

	reply.buflen = strlen(TC_REPLY_MSG) + 1;
	reply.buf = (char *)malloc(reply.buflen);
	TC_ASSERT_NEQ_CLEANUP("messaging_send_async", reply.buf, NULL, free(send_data.msg));

	ret = messaging_send_async(NULL, &send_data, &reply, &cb_info);
	TC_ASSERT_EQ_CLEANUP("messaging_send_async", ret, ERROR, free(send_data.msg); free(reply.buf));

	free(reply.buf);
	reply.buf = NULL;
	ret = messaging_send_async(TC_ASYNC_PORT, &send_data, &reply, &cb_info);
	TC_ASSERT_EQ_CLEANUP("messaging_send_async", ret, ERROR, free(send_data.msg));

	free(send_data.msg);
	send_data.msg = NULL;
	ret = messaging_send_async(TC_ASYNC_PORT, &send_data, &reply, &cb_info);
	TC_ASSERT_EQ("messaging_send_async", ret, ERROR);

	cb_info.cb_func = NULL;
	ret = messaging_send_async(TC_ASYNC_PORT, &send_data, &reply, &cb_info);
	TC_ASSERT_EQ("messaging_send_async", ret, ERROR);

	TC_SUCCESS_RESULT();
}

static void utc_messaging_send_async_p(void)
{
	int ret;

	sync_async_flag = ASYNC_TEST;
	sem_init(&send_sem, 0, 1);

	ret = task_create("recv_async", TASK_PRIO, STACKSIZE, (main_t)reply_recv, (FAR char * const *)NULL);
	TC_ASSERT_GEQ("messaging_send_async", ret, 0);

	ret = task_create("send_async", TASK_PRIO, STACKSIZE, (main_t)send_async, (FAR char * const *)NULL);
	TC_ASSERT_GEQ("messaging_send_async", ret, 0);

	ret = sem_wait(&send_sem);
	TC_ASSERT_EQ_CLEANUP("messaging_send_async", tc_send_chk, TC_OK, sem_destroy(&send_sem); messaging_cleanup(TC_ASYNC_PORT));
	TC_ASSERT_EQ_CLEANUP("messaging_send_async", ret, OK, sem_destroy(&send_sem); messaging_cleanup(TC_ASYNC_PORT));

	sem_destroy(&send_sem);
	messaging_cleanup(TC_ASYNC_PORT);
	TC_SUCCESS_RESULT();
}

static void utc_messaging_send_n(void)
{
	int ret;
	msg_send_data_t send_data;

	send_data.msglen = strlen(TC_SEND_MSG) + 1;
	send_data.msg = (char *)malloc(send_data.msglen);
	TC_ASSERT_NEQ("messaging_send", send_data.msg, NULL);

	strncpy(send_data.msg, TC_SEND_MSG, send_data.msglen);

	ret = messaging_send(NULL, &send_data);
	TC_ASSERT_EQ_CLEANUP("messaging_send", ret, ERROR, free(send_data.msg));

	ret = messaging_send("temp_port", NULL);
	TC_ASSERT_EQ_CLEANUP("messaging_send", ret, ERROR, free(send_data.msg));

	free(send_data.msg);
	TC_SUCCESS_RESULT();
}

static void utc_messaging_send_p(void)
{
	int ret;

	sem_init(&send_sem, 0, 1);

	ret = task_create("noreply_recv", TASK_PRIO, STACKSIZE, (main_t)noreply_recv, (FAR char * const *)NULL);
	TC_ASSERT_GEQ("messaging_send", ret, 0);

	ret = task_create("noreply_send", TASK_PRIO, STACKSIZE, (main_t)noreply_send, (FAR char * const *)NULL);
	TC_ASSERT_GEQ("messaging_send", ret, 0);

	ret = sem_wait(&send_sem);
	TC_ASSERT_EQ_CLEANUP("messaging_send", tc_send_chk, TC_OK, sem_destroy(&send_sem));
	TC_ASSERT_EQ_CLEANUP("messaging_send", ret, OK, sem_destroy(&send_sem));

	sem_destroy(&send_sem);
	TC_SUCCESS_RESULT();
}

void utc_messaging_send_main(void)
{
	utc_messaging_send_n();
	utc_messaging_send_p();

	utc_messaging_send_sync_n();
	utc_messaging_send_sync_p();

	utc_messaging_send_async_n();
	utc_messaging_send_async_p();
}
