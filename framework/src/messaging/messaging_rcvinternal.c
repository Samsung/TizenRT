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

static sq_queue_t g_port_info_list;
/****************************************************************************
 * private functions
 ****************************************************************************/
sq_queue_t *messaging_get_port_info_list(void)
{
	return &g_port_info_list;
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
	char *recv_packet;
	int msg_type;
	char *internal_portname;

	recv_size = recv_buf->buflen + MSG_HEADER_SIZE;
	recv_packet = (char *)MSG_ALLOC(recv_size);
	if (recv_packet == NULL) {
		msgdbg("[Messaging] recv fail : out of memory for packet.\n");
		goto errout_with_mq;
	}
	/* Check there were messages already before setting notification */
	while (1) {
		recv_size_chk = mq_receive(mqdes, (char *)recv_packet, recv_size, 0);
		if (recv_size_chk > 0 && recv_size_chk <= recv_size) {
			ret = messaging_parse_packet(recv_packet, recv_buf->buf, recv_buf->buflen, &recv_buf->sender_pid, &msg_type);
			if (ret != OK) {
				MSG_FREE(recv_packet);
				goto errout_with_mq;
			}
			recv_buf->buflen = recv_size_chk;
			(*cb_info->cb_func)(msg_type, recv_buf, cb_info->cb_data);
		} else if (recv_size_chk == ERROR && errno == EAGAIN) {
			msgdbg("[Messaging] recv : empty queue, but NONBLOCK mode.\n");
			break;
		} else {
			msgdbg("[Messaging] recv fail : errno %d, size %d.\n", errno, recv_size_chk);
			MSG_FREE(recv_packet);
			goto errout_with_mq;
		}
	}
	MSG_FREE(recv_packet);

	/* There was no msg, then set notification. */
	ret = messaging_set_notify_signal(SIGMSG_MESSAGING, (_sa_sigaction_t)messaging_run_callback);
	if (ret != OK) {
		goto errout_with_msg;
	}

	/* nonblock_data will be passed to callback through signal. */
	nonblock_data = (msg_recv_info_t *)MSG_ALLOC(sizeof(msg_recv_info_t));
	if (nonblock_data == NULL) {
		msgdbg("[Messaging] recv fail : out of memory.\n");
		goto errout_with_msg;
	}
	nonblock_data->mqdes = mqdes;
	nonblock_data->msg = recv_buf;
	nonblock_data->user_cb = cb_info->cb_func;
	nonblock_data->cb_data = cb_info->cb_data;
	strncpy(nonblock_data->port_name, port_name, strlen(port_name) + 1);

	/* Add a new message port into the list. */
	port_info = (msg_port_info_t *)MSG_ALLOC(sizeof(msg_port_info_t));
	if (port_info == NULL) {
		msgdbg("[Messaging] recv fail : out of memory for port_info.\n");
		MSG_FREE(nonblock_data);
		goto errout_with_mq;
	}

	port_info->mqdes = mqdes;
	port_info->data = nonblock_data;
	port_info->pid = getpid();
	strncpy(port_info->name, port_name, strlen(port_name) + 1);
	sq_addlast((FAR sq_entry_t *)port_info, &g_port_info_list);

	ret = messaging_set_notification(SIGMSG_MESSAGING, nonblock_data);
	if (ret != OK) {
		goto errout_with_msg;
	}

	return OK;

errout_with_msg:
	sq_rem((FAR sq_entry_t *)port_info, &g_port_info_list);
	MSG_FREE(port_info);
errout_with_mq:
	mq_close(mqdes);
	MSG_ASPRINTF(&internal_portname, "%s%d", port_name, getpid());
	mq_unlink(internal_portname);
	MSG_FREE(internal_portname);
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
	char *recv_packet;
	int msg_type = OK;
	char *internal_portname;

	recv_size = MSG_HEADER_SIZE + recv_buf->buflen;
	recv_packet = (char *)MSG_ALLOC(recv_size);
	if (recv_packet == NULL) {
		msgdbg("[Messaging] recv fail : out of memory for packet.\n");
		ret = ERROR;
		goto cleanup_return;
	}

	ret = mq_receive(mqdes, (char *)recv_packet, recv_size, 0);
	if (ret < 0) {
		msgdbg("[Messaging] recv fail : errno %d, %s.\n", errno, port_name);
		ret = ERROR;
		goto cleanup_return;
	}

	ret = messaging_parse_packet(recv_packet, recv_buf->buf, recv_buf->buflen, &recv_buf->sender_pid, &msg_type);
	if (ret != OK) {
		msg_type = ERROR;
		goto cleanup_return;
	}

cleanup_return:
	MSG_FREE(recv_packet);
	mq_close(mqdes);
	MSG_ASPRINTF(&internal_portname, "%s%d", port_name, getpid());
	mq_unlink(internal_portname);
	MSG_FREE(internal_portname);
	return msg_type;
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
 *	type    : MSG_RECV_TYPE_BLOCK or MSG_RECV_TYPE_NONBLOCK
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
		msgdbg("[Messaging] recv fail : open fail, errno %d.\n", errno);
		return ERROR;
	}

	/* Save the receivers information. It will be used by sender to check the receivers. */
	ret = SAVE_MSG_RECEIVER(port_name);
	if (ret != OK) {
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

