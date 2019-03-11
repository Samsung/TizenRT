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
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

#define MSG_REPLY_PRIO 10
/****************************************************************************
 * private functions
 ****************************************************************************/
/****************************************************************************
 * Name : messaging_sync_recv
 * 
 * Description:
 *  This function is for waiting reply from receiver.
 *
 * Input Parameters:
 *  port_name : The message port name to send
 *  param     :
 *	priority    : A non-negative integer that specifies the priority of this message\n
 *	type        : MSG_SEND_SYNC or MSG_SEND_ASYNC\n
 *	sync_msg    : A message buffer to receive the sync reply message\n
 *	sync_msglen : A message size for sync reply message
 * 
 * Return Value:
 *  On success, 0 (OK) is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
static int messaging_sync_recv(const char *port_name, msg_data_t *reply_buf)
{
	int ret = OK;
	mqd_t sync_mqdes;
	char *sync_portname;
	struct mq_attr internal_attr;
	char *reply_data;
	int reply_size;

	reply_size = reply_buf->msglen + MSG_HEADER_SIZE;

	internal_attr.mq_maxmsg = CONFIG_MESSAGING_UNICAST_MAXMSG;
	internal_attr.mq_msgsize = reply_size;
	internal_attr.mq_flags = 0;

	/* sender waits the reply with "port_name + sender_pid". */
	MSG_ASPRINTF(&sync_portname, "%s%d", port_name, getpid());
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
		msgdbg("message send fail : out of memory for including header~\n");
		return ERROR;
	}

	ret = mq_receive(sync_mqdes, reply_data, reply_size, 0);
	if (ret < 0) {
		msgdbg("message send fail : sync recv fail %d.\n", errno);
		ret = ERROR;
	} else {
		memcpy(&reply_buf->sender_pid, reply_data, sizeof(pid_t));
		memcpy(reply_buf->msg, reply_data + MSG_HEADER_SIZE, reply_buf->msglen);
		ret = OK;
	}

	MSG_FREE(reply_data);
	MSG_FREE(sync_portname);
	mq_close(sync_mqdes);
	mq_unlink(sync_portname);

	return ret;
}
/****************************************************************************
 * Name : messaging_send_internal
 * 
 * Description:
 *  This function calls receive function based on receive type.
 *
 * Input Parameters:
 *  port_name : The message port name to send
 *  msg       : The message to be sent
 *  msglen    : The length of message to be sent
 *  priority  : A non-negative integer that specifies the priority of this message
 * 
 * Return Value:
 *  On success, 0 (OK) is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
static int messaging_send_internal(const char *port_name, msg_send_data_t *send_data)
{
	int ret = OK;
	mqd_t mqdes;
	struct mq_attr internal_attr;
	char *send_packet;
	int send_size;
	pid_t sender_pid;
	int msg_type;

	send_size = MSG_HEADER_SIZE + send_data->msglen;

	internal_attr.mq_maxmsg = CONFIG_MESSAGING_UNICAST_MAXMSG;
	internal_attr.mq_msgsize = send_size;
	internal_attr.mq_flags = 0;

	mqdes = mq_open(port_name, O_WRONLY | O_CREAT, 0666, &internal_attr);
	if (mqdes == (mqd_t)ERROR) {
		msgdbg("message send fail : open fail %d.\n", errno);
		return ERROR;
	}

	send_packet = (char *)MSG_ALLOC(send_size);
	if (send_packet == NULL) {
		msgdbg("message send fail : out of memory for including header.\n");
		return ERROR;
	}

	/* Send packet is like below.
	 * +-----------------------------------------+
	 * | sender_pid | send type | message        |
	 * +-----------------------------------------+
	 */

	/* Add data header for sender pid. */
	sender_pid = getpid();
	memcpy(send_packet, &sender_pid, sizeof(pid_t));
	/* Add data header for send type. */
	msg_type = send_data->type;
	memcpy(send_packet + sizeof(pid_t), &msg_type, sizeof(int));
	/* Copy the real send message. */
	memcpy(send_packet + MSG_HEADER_SIZE, send_data->msg, send_data->msglen);

	ret = mq_send(mqdes, (char *)send_packet, send_size, send_data->priority);
	if (ret != OK) {
		msgdbg("message send fail : send fail %d.\n", errno);
		MSG_FREE(send_packet);
		mq_close(mqdes);
		mq_unlink(port_name);
		return ERROR;
	}

	MSG_FREE(send_packet);
	mq_close(mqdes);
	return ret;
}

/****************************************************************************
 * messaging_send
 ****************************************************************************/
int messaging_send(const char *port_name, msg_send_data_t *send_data, msg_data_t *reply_data)
{
	int ret;

	if ((send_data->type < MSG_SEND_TYPE_SYNC) || (send_data->type > MSG_SEND_TYPE_ASYNC) || (send_data->type == MSG_SEND_TYPE_SYNC && reply_data->msg == NULL)) {
		msgdbg("message send fail : invalid param2.\n");
		return ERROR;
	}

	ret = messaging_send_internal(port_name, send_data);
	if (ret != OK) {
		return ERROR;
	}

	if (send_data->type == MSG_SEND_TYPE_SYNC) {
		ret = messaging_sync_recv(port_name, reply_data);
		if (ret != OK) {
			return ERROR;
		}
	}
	return OK;
}

/****************************************************************************
 * messaging_send_reply
 ****************************************************************************/
int messaging_send_reply(const char *port_name, msg_data_t *reply_data)
{
	int ret = OK;
	char *reply_portname;
	msg_send_data_t reply;

	if (reply_data->sender_pid < 0) {
		msgdbg("message reply fail : invalid sender pid.\n");
		return ERROR;
	}

	/* sender waits the reply with "port_name + sender_pid". */
	MSG_ASPRINTF(&reply_portname, "%s%d", port_name, reply_data->sender_pid);
	if (reply_portname == NULL) {
		msgdbg("message reply fail : out of memory.\n");
		return ERROR;
	}

	reply.msg = reply_data->msg;
	reply.msglen = reply_data->msglen;
	reply.priority = MSG_REPLY_PRIO;
	reply.type = MSG_SEND_TYPE_ASYNC;
	ret = messaging_send_internal(reply_portname, &reply);
	if (ret != OK) {
		msgdbg("message reply fail.\n");
	}
	MSG_FREE(reply_portname);
	return ret;
}
