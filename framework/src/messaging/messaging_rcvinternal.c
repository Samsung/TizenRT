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
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <queue.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

sq_queue_t g_port_info_list;
/****************************************************************************
 * private functions
 ****************************************************************************/
/****************************************************************************
 * Name : messaging_run_nonblock_callback
 * 
 * Description:
 *  This function is wrapper callback function for waiting until receiving the message.
 ****************************************************************************/
static void messaging_run_nonblock_callback(int signo, siginfo_t *data)
{
	int ret;
	ssize_t size;
	msg_recv_info_t *recv_info;
	msg_recv_buf_t recv_buf;
	struct mq_attr attr;
	char *recv_data;
	int msg_type;

	if (data == NULL) {
		msgdbg("[Messaging] recv fail : wrong param for wrapper callback(data).\n");
		return;
	}

	recv_info = (msg_recv_info_t *)data->si_value.sival_ptr;
	if (recv_info == NULL) {
		msgdbg("[Messaging] recv fail : wrong param for wrapper callback(info).\n");
		return;
	}

	recv_data = (char *)MSG_ALLOC(CONFIG_MESSAGING_MSGSIZE);
	if (recv_data == NULL) {
		MSG_FREE(recv_info);
		msgdbg("[Messaging] recv fail : out of memory for including header.\n");
		return;
	}

	recv_buf.buf = (char *)MSG_ALLOC(CONFIG_MESSAGING_MSGSIZE - (MSG_HEADER_SIZE));
	if (recv_buf.buf == NULL) {
		MSG_FREE(recv_info);
		msgdbg("[Messaging] recv fail : out of memory for recv buffer.\n");
		return;
	}

	while (1) {
		size = mq_receive(recv_info->mqdes, recv_data, CONFIG_MESSAGING_MSGSIZE, 0);
		if (size < 0) {
			msgdbg("[Messaging] recv fail : mq_receive fail. %d\n", errno);
			mq_close(recv_info->mqdes);
			MSG_FREE(recv_data);
			MSG_FREE(recv_info);
			MSG_FREE(recv_buf.buf);
			return;
		}
		memcpy(&recv_buf.sender_pid, recv_data, sizeof(pid_t));
		memcpy(&msg_type, recv_data + sizeof(pid_t), sizeof(int));
		memcpy(&recv_buf.buf, recv_data + MSG_HEADER_SIZE, CONFIG_MESSAGING_MSGSIZE - sizeof(pid_t));
		recv_buf.buflen = size;
		/* Call user callback */
		(*recv_info->user_cb)(msg_type, &recv_buf, recv_info->cb_data);
		MSG_FREE(recv_data);
		MSG_FREE(recv_buf.buf);

		ret = mq_getattr(recv_info->mqdes, &attr);
		if (ret < 0) {
			msgdbg("[Messaging] recv fail : mq_getattr fail.\n");
			mq_close(recv_info->mqdes);
			MSG_FREE(recv_info);
			MSG_FREE(data);
			return;
		} else if (attr.mq_curmsgs == 0) {
			/* All requests are handled. Register notification again. */
			messaging_set_notification(SIGMSG_MESSAGING, recv_info);
			break;
		}
	}
}

/****************************************************************************
 * Name : messaging_recv_nonblock
 * 
 * Description:
 *  This function is for non-blocking mq_receive.
 * If there is a message in the queue, this function calls the callback with msg.
 * If there is no message, then calls the notification function to register callback. 
 *
 * Input Parameters:
 *  mqdes     : Message queue descriptor
 *  port_name : The message port name to receive
 *  msg       : The message buffer to receive the message
 *  msglen    : The length of message to receive
 *  param     :
 *	type    : MSG_RECV_TYPE_BLOCK or MSG_RECV_TYPE_NONBLOCK\n
 *	cb_func : user callback function for nonblocking receive
 * 
 * Return Value:
 *  On success, 0 (OK) is returned.; On failure, -1 (ERROR) is returned.
 *  Sender pid is passed to callback function as 3rd argument.
 ****************************************************************************/
static int messaging_rcv_nonblock(mqd_t mqdes, const char *port_name, msg_recv_buf_t *recv_buf, msg_callback_info_t *cb_info)
{
	int ret;
	ssize_t recv_size_chk;
	msg_recv_info_t *nonblock_data;
	msg_port_info_t *port_info = NULL;
	int recv_size;
	char *recv_data;
	int msg_type;

	recv_size = recv_buf->buflen + MSG_HEADER_SIZE;
	recv_data = MSG_ALLOC(recv_size);
	if (recv_data == NULL) {
		msgdbg("[Messaging] recv fail : out of memory for including header.\n");
		goto cleanup_mq;
	}
	/* Check there was msg already before setting notification */
	while (1) {
		recv_size_chk = mq_receive(mqdes, recv_data, recv_size, 0);
		if (recv_size_chk > 0 && recv_size_chk <= recv_size) {
			memcpy(&recv_buf->sender_pid, recv_data, sizeof(pid_t));
			memcpy(&msg_type, recv_data + sizeof(pid_t), sizeof(int));
			memcpy(recv_buf->buf, recv_data + MSG_HEADER_SIZE, recv_buf->buflen);
			recv_buf->buflen = recv_size_chk;
			(*cb_info->cb_func)(msg_type, recv_buf, cb_info->cb_data);
		} else if (recv_size_chk == ERROR && errno == EAGAIN) {
			msgdbg("[Messaging] recv : the queue was empty, and the NONBLOCK flag was set.\n");
			break;
		} else {
			msgdbg("[Messaging] recv fail : recv fail errno %d, recv_size %d.\n", errno, recv_size_chk);
			MSG_FREE(recv_data);
			goto cleanup_mq;
		}
	}
	MSG_FREE(recv_data);

	/* There was no msg, then set notification. */
	ret = messaging_set_notify_signal(SIGMSG_MESSAGING, (_sa_sigaction_t)messaging_run_nonblock_callback);
	if (ret != OK) {
		msgdbg("[Messaging] recv fail : set notification error.\n");
		goto cleanup_all;
	}

	nonblock_data = (msg_recv_info_t *)MSG_ALLOC(sizeof(msg_recv_info_t));
	if (nonblock_data == NULL) {
		msgdbg("[Messaging] recv fail : out of memory.\n");
		goto cleanup_all;
	}
	nonblock_data->mqdes = mqdes;
	nonblock_data->user_cb = cb_info->cb_func;
	nonblock_data->cb_data = cb_info->cb_data;

	/* Update the messag port info list. */
	port_info = (msg_port_info_t *)MSG_ALLOC(sizeof(msg_port_info_t));
	if (port_info == NULL) {
		msgdbg("[Messaging] recv fail : out of memory for msg_port_info.\n");
		MSG_FREE(nonblock_data);
		goto cleanup_mq;
	}

	port_info->mqdes = mqdes;
	port_info->data = nonblock_data;
	port_info->pid = getpid();
	strncpy(port_info->name, port_name, strlen(port_name) + 1);
	sq_addlast((FAR sq_entry_t *)port_info, &g_port_info_list);

	ret = messaging_set_notification(SIGMSG_MESSAGING, nonblock_data);
	if (ret != OK) {
		goto cleanup_all;
	}

	return OK;

cleanup_all:
	sq_rem((FAR sq_entry_t *)port_info, &g_port_info_list);
	MSG_FREE(port_info);
cleanup_mq:
	mq_close(mqdes);
	mq_unlink(port_name);
	return ERROR;
}

/****************************************************************************
 * Name : messaging_rcv_block
 * 
 * Description:
 *  This function calls mq_receive with block mode.
 *
 * Input Parameters:
 *  mqdes     : Message queue descriptor
 *  port_name : The message port name to receive
 *  msg       : The message buffer to receive the message
 *  msglen    : The length of message to receive
 * 
 * Return Value:
 *  On success, sender pid (>=0) is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
static int messaging_rcv_block(mqd_t mqdes, const char *port_name, msg_recv_buf_t *recv_buf)
{
	int ret = OK;
	int recv_size;
	char *recv_data;
	int msg_type;
	char *internal_portname;

	recv_size = MSG_HEADER_SIZE + recv_buf->buflen;
	recv_data = MSG_ALLOC(recv_size);
	if (recv_data == NULL) {
		msgdbg("[Messaging] recv fail : out of memory for including header.\n");
		ret = ERROR;
		goto cleanup_return;
	}

	ret = mq_receive(mqdes, recv_data, CONFIG_MESSAGING_MSGSIZE, 0);
	if (ret < 0) {
		msgdbg("[Messaging] recv fail : recv fail %d.\n", errno);
		ret = ERROR;
		goto cleanup_return;
	}

	/* Received packet is like below.
	 * +-----------------------------------------+
	 * | sender_pid | msg type | message        |
	 * +-----------------------------------------+
	 */
	memcpy(&recv_buf->sender_pid, recv_data, sizeof(pid_t));
	memcpy(&msg_type, recv_data + sizeof(pid_t), sizeof(int));
	ret = msg_type;
	memcpy(recv_buf->buf, recv_data + MSG_HEADER_SIZE, recv_buf->buflen);

cleanup_return:
	MSG_FREE(recv_data);
	mq_close(mqdes);
	MSG_ASPRINTF(&internal_portname, "%s%d", port_name, getpid());
	mq_unlink(internal_portname);
	MSG_FREE(internal_portname);
	return ret;
}
/****************************************************************************
 * Name : messaging_recv_internal
 * 
 * Description:
 *  This function calls receive function based on receive type.
 *
 * Input Parameters:
 *  port_name : The message port name to receive
 *  param :
 *	msg     : The message buffer to receive the message
 *	msglen  : The length of message to receive
 *	type    : MSG_RECV_TYPE_BLOCK or MSG_RECV_TYPE_NONBLOCK\n
 *	cb_func : user callback function for nonblocking receive
 * 
 * Return Value:
 *  On success, 0 (OK) is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
int messaging_recv_internal(const char *port_name, msg_recv_buf_t *recv_buf, msg_callback_info_t *cb_info)
{
	int ret = OK;
	mqd_t mqdes;
	struct mq_attr internal_attr;
	char *internal_portname;

	MSG_ASPRINTF(&internal_portname, "%s%d", port_name, getpid());

	internal_attr.mq_maxmsg = CONFIG_MESSAGING_MAXMSG;
	internal_attr.mq_msgsize = recv_buf->buflen + MSG_HEADER_SIZE;
	internal_attr.mq_flags = 0;

	if (cb_info == NULL) {
		/* This is block receive case. */
		mqdes = mq_open(internal_portname, O_RDONLY | O_CREAT, 0666, &internal_attr);
	} else {
		/* This is non-block receive case. */
		mqdes = mq_open(internal_portname, O_RDONLY | O_CREAT | O_NONBLOCK, 0666, &internal_attr);
	}

	if (mqdes == (mqd_t)ERROR) {
		MSG_FREE(internal_portname);
		msgdbg("[Messaging] recv fail : open fail %d.\n", errno);
		return ERROR;
	}

	ret = messaging_handle_data(MSG_HANDLE_SAVE, port_name, NULL, NULL);
	if (ret != OK) {
		msgdbg("[Messaging] recv fail : saving data error.\n");
		mq_close(mqdes);
		mq_unlink(internal_portname);
		MSG_FREE(internal_portname);
		return ERROR;
	}

	if (cb_info == NULL) {
		ret = messaging_rcv_block(mqdes, port_name, recv_buf);
	} else {
		ret = messaging_rcv_nonblock(mqdes, port_name, recv_buf, cb_info);
	}

	MSG_FREE(internal_portname);
	return ret;
}

