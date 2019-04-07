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
#include <sys/prctl.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

#define MSG_RECV_NOT_INIT (-1)
/****************************************************************************
 * private functions
 ****************************************************************************/
static int messaging_set_async_callback(mqd_t mqdes, msg_callback_info_t *param)
{
	int ret;
	msg_recv_info_t *data;

	ret = messaging_set_notify_signal(SIGMSG_MESSAGING, (_sa_sigaction_t)param->cb_func);
	if (ret != OK) {
		return ERROR;
	}

	data = (msg_recv_info_t *)MSG_ALLOC(sizeof(msg_recv_info_t));
	data->mqdes = mqdes;
	data->user_cb = param->cb_func;
	data->cb_data = param->cb_data;

	ret = messaging_set_notification(SIGMSG_MESSAGING, data);
	if (ret != OK) {
		MSG_FREE(data);
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
static int messaging_send_packet(const char *port_name, int msg_type, msg_send_data_t *send_data, msg_callback_info_t *cb_info, int recv_cnt)
{
	int ret = OK;
	mqd_t mqdes;
	struct mq_attr internal_attr;
	char *send_packet;
	int send_size;
	pid_t sender_pid;
	int send_type;

	send_size = MSG_HEADER_SIZE + send_data->msglen;

	internal_attr.mq_maxmsg = CONFIG_MESSAGING_MAXMSG;
	internal_attr.mq_msgsize = send_size;
	internal_attr.mq_flags = 0;

	mqdes = mq_open(port_name, O_WRONLY, 0666, &internal_attr);
	if (mqdes == (mqd_t)ERROR) {
		if (errno == ENOENT) {
			msgdbg("[Messaging] send fail : There is no receiver.\n");
		} else {
			msgdbg("[Messaging] send fail : open fail %d.\n", errno);
		}
		return ERROR;
	}

	/* Sender can receive the reply when only one receiver waits. */
	if (recv_cnt == 1 && msg_type == MSG_SEND_ASYNC) {
		ret = messaging_set_async_callback(mqdes, cb_info);
		if (ret != OK) {
			msgdbg("[Messaging] send fail : set async callback error.\n");
			return ERROR;
		}
	}

	send_packet = (char *)MSG_ALLOC(send_size);
	if (send_packet == NULL) {
		msgdbg("[Messaging] send fail : out of memory for including header.\n");
		return ERROR;
	}

	/* Send packet is like below.
	 * +-----------------------------------------+
	 * | sender_pid | msg type | message        |
	 * +-----------------------------------------+
	 */

	/* Add data header for sender pid. */
	sender_pid = getpid();
	memcpy(send_packet, &sender_pid, sizeof(pid_t));
	/* Add data header for send type. */
	if (msg_type == MSG_SEND_NOREPLY) {
		send_type = MSG_REPLY_NO_REQUIRED;
	} else {
		send_type = MSG_REPLY_REQUIRED;
	}
	memcpy(send_packet + sizeof(pid_t), &send_type, sizeof(int));
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
int messaging_send_internal(const char *port_name, int msg_type, msg_send_data_t *send_data, msg_callback_info_t *cb_info)
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
		read_status = messaging_handle_data(MSG_HANDLE_READ, port_name, recv_arr, &recv_cnt);
		if (read_status == ERROR) {
			msgdbg("[Messaging] send fail : getting receivers info error.\n");
			return ERROR;
		}

		if (msg_type != MSG_SEND_MULTI && recv_cnt > 1) {
			msgdbg("[Messaging] send fail : too many receivers are waiting unicast.\n");
			return ERROR;
		}

		/* Send message to each receivers. */
		for (recv_idx = 0; recv_idx < recv_cnt; recv_idx++) {
			if (recv_arr[recv_idx] != MSG_RECV_NOT_INIT) {
				MSG_ASPRINTF(&private_portname, "%s%d", port_name, recv_arr[recv_idx]);
				if (private_portname == NULL) {
					msgdbg("[Messaging] send fail : out of memory for private portname.\n");
					return ERROR;
				}
				if (msg_type == MSG_SEND_ASYNC) {
					ret = messaging_send_packet(private_portname, msg_type, send_data, cb_info, recv_cnt);
				} else {
					ret = messaging_send_packet(private_portname, msg_type, send_data, NULL, recv_cnt);
				}
				MSG_FREE(private_portname);
			}
		}
	}
	if (ret == OK) {
		return recv_cnt;
	} else {
		return ret;
	}
}
