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
#include <debug.h>
#include <stdlib.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

/****************************************************************************
 * Name : messaging_recv_block
 *
 * Description :
 *  Wait to receive unicast message from specified message port
 * 
 * Input Parameters:
 *  port_name : The message port name to receive
 *  data      :
 *	msg    : The message buffer to receive the message
 *	msglen : The length of message to receive
 *  param
 *
 * Return Value:
 *  On success, Received message Type is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
int messaging_recv_block(const char *port_name, msg_recv_buf_t *recv_buf)
{
	int ret = OK;

	if (port_name == NULL || recv_buf == NULL || recv_buf->buf == NULL || recv_buf->buflen <= 0) {
		msgdbg("[Messaging] recv fail : invalid param.\n");
		return ERROR;
	}

	ret = messaging_recv_internal(port_name, recv_buf, NULL);
	if (ret == MSG_SEND_NOREPLY) {
		ret = MSG_REPLY_NO_REQUIRED;
	} else {
		ret = MSG_REPLY_REQUIRED;
	}

	(void)messaging_cleanup(port_name);

	return ret;
}

/****************************************************************************
 * Name : messaging_recv_nonblock
 *
 * Description :
 *  Wait to receive unicast message from specified message port
 * 
 * Input Parameters:
 *  port_name : The message port name to receive
 *  data      :
 *	msg    : The message buffer to receive the message
 *	msglen : The length of message to receive
 *  param
 *
 * Return Value:
 *  On success, 0 (OK) is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
int messaging_recv_nonblock(const char *port_name, msg_recv_buf_t *recv_buf, msg_callback_info_t *cb_info)
{
	int ret = OK;

	if (port_name == NULL || recv_buf == NULL || recv_buf->buf == NULL || recv_buf->buflen <= 0) {
		msgdbg("[Messaging] recv fail : invalid param.\n");
		return ERROR;
	}

	if (cb_info == NULL || cb_info->cb_func == NULL) {
		msgdbg("[Messaging] recv fail : invaild callback function.\n");
		return ERROR;
	}

	ret = messaging_recv_internal(port_name, recv_buf, cb_info);
	return ret;
}
