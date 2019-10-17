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
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <debug.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

#define MSG_REPLY_PRIO     10
/****************************************************************************
 * private functions
 ****************************************************************************/
static int messaging_send_param_validation(const char *port_name, msg_send_data_t *send_data)
{
	if (port_name == NULL) {
		msgdbg("[Messaging] unicast send sync fail : no port name.\n");
		return ERROR;
	}

	if (send_data == NULL || send_data->msg == NULL || send_data->msglen <= 0 || send_data->priority < 0) {
		msgdbg("[Messaging] unicast send sync fail : invalid param of send data.\n");
		return ERROR;
	}

	return OK;
}
static int messaging_sync_recv(const char *port_name, msg_recv_buf_t *reply_buf)
{
	int ret = OK;
	mqd_t sync_mqdes;
	char *sync_portname;
	struct mq_attr internal_attr;
	char *reply_data;
	int reply_size;
	int msg_type;

	reply_size = reply_buf->buflen + MSG_HEADER_SIZE;

	internal_attr.mq_maxmsg = CONFIG_MESSAGING_MAXMSG;
	internal_attr.mq_msgsize = reply_size;
	internal_attr.mq_flags = 0;

	/* sender waits the reply with "port_name + sender_pid + _r". */
	MSG_ASPRINTF(&sync_portname, "%s%d%s", port_name, getpid(), "_r");
	if (sync_portname == NULL) {
		msgdbg("message send fail : sync portname allocation fail.\n");
		return ERROR;
	}
	sync_mqdes = mq_open(sync_portname, O_RDONLY | O_CREAT, 0666, &internal_attr);
	if (sync_mqdes == (mqd_t)ERROR) {
		msgdbg("message send fail : sync open fail %d.\n", errno);
		MSG_FREE(sync_portname);
		return ERROR;
	}

	reply_data = (char *)MSG_ALLOC(reply_size);
	if (reply_data == NULL) {
		msgdbg("message send fail : out of memory for including header\n");
		mq_close(sync_mqdes);
		mq_unlink(sync_portname);
		return ERROR;
	}

	ret = mq_receive(sync_mqdes, reply_data, reply_size, 0);
	if (ret < 0) {
		msgdbg("message send fail : sync recv fail %d.\n", errno);
		ret = ERROR;
	} else {
		ret = messaging_parse_packet(reply_data, reply_buf->buf, reply_buf->buflen, &reply_buf->sender_pid, &msg_type);
		if (ret != OK) {
			ret = ERROR;
		}
	}

	mq_close(sync_mqdes);
	mq_unlink(sync_portname);
	MSG_FREE(reply_data);
	MSG_FREE(sync_portname);

	return ret;
}

/****************************************************************************
 * public functions
 ****************************************************************************/
/****************************************************************************
 * messaging_send_sync
 ****************************************************************************/
int messaging_send_sync(const char *port_name, msg_send_data_t *send_data, msg_recv_buf_t *reply_buf)
{
	int ret;

	ret = messaging_send_param_validation(port_name, send_data);
	if (ret == ERROR) {
		return ERROR;
	}

	if (reply_buf == NULL || reply_buf->buf == NULL || reply_buf->buflen <= 0) {
		msgdbg("[Messaging] unicast send sync fail : invalid param of reply buf\n");
		return ERROR;
	}

	ret = messaging_send_internal(port_name, MSG_SEND_SYNC, send_data, NULL, NULL);
	if (ret == ERROR) {
		return ERROR;
	}

	ret = messaging_sync_recv(port_name, reply_buf);
	if (ret != OK) {
		return ERROR;
	}

	return OK;	
}

/****************************************************************************
 * messaging_send_async
 ****************************************************************************/
int messaging_send_async(const char *port_name, msg_send_data_t *send_data, msg_recv_buf_t *reply_buf, msg_callback_info_t *cb_info)
{
	int ret;

	ret = messaging_send_param_validation(port_name, send_data);
	if (ret == ERROR) {
		return ERROR;
	}

	if (reply_buf == NULL || reply_buf->buf == NULL || reply_buf->buflen <= 0) {
		msgdbg("[Messaging] unicast send async fail : invalid param of reply buffer.\n");
		return ERROR;
	}

	if (cb_info == NULL || cb_info->cb_func == NULL) {
		msgdbg("[Messaging] unicast send asyc fail : invalid callback info.\n");
		return ERROR;
	}

	ret = messaging_send_internal(port_name, MSG_SEND_ASYNC, send_data, reply_buf, cb_info);
	if (ret == ERROR) {
		return ERROR;
	}

	return OK;
}

/****************************************************************************
 * messaging_send
 ****************************************************************************/
int messaging_send(const char *port_name, msg_send_data_t *send_data)
{
	int ret;
	if (port_name == NULL) {
		msgdbg("[Messaging] unicast send async fail : no port name.\n");
		return ERROR;
	}

	if (send_data == NULL || send_data->msg == NULL || send_data->msglen <= 0 || send_data->priority < 0) {
		msgdbg("[Messaging] unicast send async fail : invalid param of send data.\n");
		return ERROR;
	}

	ret = messaging_send_internal(port_name, MSG_SEND_NOREPLY, send_data, NULL, NULL);
	if (ret == ERROR) {
		return ERROR;
	}

	return OK;
}

/****************************************************************************
 * messaging_reply
 ****************************************************************************/
int messaging_reply(const char *port_name, pid_t sender_pid, msg_send_data_t *reply_data)
{
	int ret = OK;
	char *reply_portname;
	msg_send_data_t reply;

	if (port_name == NULL || sender_pid < 0 || reply_data == NULL || reply_data->msg == NULL || reply_data->msglen <= 0) {
		msgdbg("[Messaging] unicast reply fail : invalid param.\n");
		return ERROR;
	}

	/* Sender waits the reply with "port_name + sender_pid + _r". */
	MSG_ASPRINTF(&reply_portname, "%s%d%s", port_name, sender_pid, "_r");
	if (reply_portname == NULL) {
		msgdbg("[Messaging] unicast reply fail : out of memory.\n");
		return ERROR;
	}

	reply.msg = reply_data->msg;
	reply.msglen = reply_data->msglen;
	reply.priority = MSG_REPLY_PRIO;
	ret = messaging_send_packet(reply_portname, MSG_SEND_REPLY, &reply, NULL);
	MSG_FREE(reply_portname);
	return ret;
}
