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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

#define MSG_RECV_NOT_INIT (-1)
/****************************************************************************
 * private functions
 ****************************************************************************/
static int messaging_set_async_callback(const char *port_name, msg_recv_buf_t *recv_data, msg_callback_info_t *param)
{
	int ret;
	msg_recv_info_t *data;
	struct mq_attr internal_attr;
	int recv_size;
	char *reply_portname;
	mqd_t mqdes;

	ret = messaging_set_notify_signal(SIGMSG_MESSAGING, (_sa_sigaction_t)messaging_run_callback);
	if (ret != OK) {
		return ERROR;
	}

	recv_size = MSG_HEADER_SIZE + recv_data->buflen;

	internal_attr.mq_maxmsg = CONFIG_MESSAGING_MAXMSG;
	internal_attr.mq_msgsize = recv_size;
	internal_attr.mq_flags = 0;

	/* Sender waits the reply with "port_name + sender_pid + _r". */
	MSG_ASPRINTF(&reply_portname, "%s%d%s", port_name, getpid(), "_r");
	if (reply_portname == NULL) {
		msgdbg("[Messaging] send fail : out of memory for async port.\n");
		return ERROR;
	}

	mqdes = mq_open(reply_portname, O_RDONLY | O_CREAT, 0666, &internal_attr);
	if (mqdes == (mqd_t)ERROR) {
		msgdbg("[Messaging] send fail : open fail, errno %d.\n", errno);
		MSG_FREE(reply_portname);
		return ERROR;
	}

	data = (msg_recv_info_t *)MSG_ALLOC(sizeof(msg_recv_info_t));
	if (data == NULL) {
		msgdbg("[Messaging] send fail : out of memory for recv info.\n");
		MSG_FREE(reply_portname);
		return ERROR;
	}
	data->mqdes = mqdes;
	data->msg = recv_data;
	data->user_cb = param->cb_func;
	data->cb_data = param->cb_data;
	strncpy(data->port_name, reply_portname, strlen(reply_portname) + 1);
	MSG_FREE(reply_portname);

	ret = messaging_set_notification(SIGMSG_MESSAGING, data);
	if (ret != OK) {
		return ERROR;
	}
	return OK;
}
/****************************************************************************
 * Name : messaging_send_packet
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
int messaging_send_packet(const char *port_name, msg_send_type_t msg_type, msg_send_data_t *send_data, msg_callback_info_t *cb_info)
{
	int ret = OK;
	mqd_t mqdes;
	struct mq_attr internal_attr;
	char *send_packet;
	int send_size;
	uint32_t send_type;
	uint32_t msg_offset;
	uint32_t msg_version;

	send_size = MSG_HEADER_SIZE + send_data->msglen;

	internal_attr.mq_maxmsg = CONFIG_MESSAGING_MAXMSG;
	internal_attr.mq_msgsize = send_size;
	internal_attr.mq_flags = 0;

	mqdes = mq_open(port_name, O_WRONLY, 0666, &internal_attr);
	if (mqdes == (mqd_t)ERROR) {
		if (errno == ENOENT) {
			msgdbg("[Messagmemcpying] send fail : no receiver.\n");
		} else {
			msgdbg("[Messaging] send fail : open fail, errno %d.\n", errno);
		}
		return ERROR;
	}

	send_packet = (char *)MSG_ALLOC(send_size);
	if (send_packet == NULL) {
		msgdbg("[Messaging] send fail : out of memory for including header.\n");
		mq_close(mqdes);
		mq_unlink(port_name);
		return ERROR;
	}

	/* Send packet(version 1) is like below.
	 * +--------------------------------------------------------------------------------------------------------+
	 * | version(4bytes) | msg_offset(4bytes) | sender_pid(4bytes) | msg type(4bytes) | message(Max 65515bytes) |
	 * +--------------------------------------------------------------------------------------------------------+
	 */

	/* Add data header for message version and msg offset. */
	msg_version = messaging_get_version();
	((messaging_packet_t *)send_packet)->version = msg_version;
	msg_offset = MSG_HEADER_SIZE;
	((messaging_packet_t *)send_packet)->offset = msg_offset;

	/* Add data header for sender pid. */
	((messaging_packet_t *)send_packet)->sender_pid = getpid();

	/* Add data header for send type. */
	if (msg_type == MSG_SEND_NOREPLY || msg_type == MSG_SEND_MULTI) {
		send_type = MSG_REPLY_NO_REQUIRED;
	} else if (msg_type == MSG_SEND_REPLY) {
		send_type = MSG_SEND_REPLY;
	} else {
		send_type = MSG_REPLY_REQUIRED;
	}
	((messaging_packet_t *)send_packet)->msg_type = send_type;

	/* Copy the real send message. */
	memcpy(send_packet + msg_offset, send_data->msg, send_data->msglen);

	ret = mq_send(mqdes, (char *)send_packet, send_size, send_data->priority);
	if (ret != OK) {
		msgdbg("[Messaging] send fail : errno %d.\n", errno);
		MSG_FREE(send_packet);
		mq_close(mqdes);
		mq_unlink(port_name);
		return ERROR;
	}

	MSG_FREE(send_packet);
	mq_close(mqdes);
	return ret;
}

static void messaging_init_recv_arr(int *arr)
{
	int arr_idx;
	for (arr_idx = 0; arr_idx < CONFIG_MESSAGING_RECV_LIST_SIZE; arr_idx++) {
		arr[arr_idx] = MSG_RECV_NOT_INIT;
	}
}
/****************************************************************************
 * Name : messaging_send_internal
 * 
 * Description:
 *  This function checks how many receivers are waiting and send message to them.
 *
 * Input Parameters:
 *  port_name : The message port name to send
 *  msg       : The message to be sent
 *  msglen    : The length of message to be sent
 *  priority  : A non-negative integer that specifies the priority of this message
 * 
 * Return Value:
 *  On success, the number of waiting receivers is returned.
 *  On failure, -1 (ERROR) is returned.
 ****************************************************************************/
int messaging_send_internal(const char *port_name, msg_send_type_t msg_type, msg_send_data_t *send_data, msg_recv_buf_t *recv_data, msg_callback_info_t *cb_info)
{
	int recv_idx;
	int ret = ERROR;
	int read_status = MSG_READ_YET;
	int recv_arr[CONFIG_MESSAGING_RECV_LIST_SIZE];
	char *private_portname;
	int recv_cnt;

	/* Check that how many receivers are waiting. */
	while (read_status != MSG_READ_ALL) {
		(void)messaging_init_recv_arr(recv_arr);
		read_status = READ_MSG_RECEIVER(port_name, recv_arr, recv_cnt);
		if (read_status == ERROR) {
			return ERROR;
		}

		if (msg_type != MSG_SEND_MULTI && recv_cnt > 1) {
			msgdbg("[Messaging] send fail : too many receivers(%d)are waiting.\n", recv_cnt);
			return ERROR;
		}

		/* Send message to each receivers. */
		for (recv_idx = 0; recv_idx < recv_cnt; recv_idx++) {
			if (recv_arr[recv_idx] == MSG_RECV_NOT_INIT) {
				continue;
			}
			MSG_ASPRINTF(&private_portname, "%s%d", port_name, recv_arr[recv_idx]);
			if (private_portname == NULL) {
				msgdbg("[Messaging] send fail : out of memory for private portname.\n");
				return ERROR;
			}
			if (msg_type == MSG_SEND_ASYNC) {
				if (recv_cnt == 1) {
					ret = messaging_set_async_callback(port_name, recv_data, cb_info);
					if (ret != OK) {
						MSG_FREE(private_portname);
						return ERROR;
					}
				}
				ret = messaging_send_packet(private_portname, msg_type, send_data, cb_info);
			} else {
				ret = messaging_send_packet(private_portname, msg_type, send_data, NULL);
			}
			MSG_FREE(private_portname);
		}
	}
	if (ret == OK) {
		return recv_cnt;
	}
	return ret;
}
