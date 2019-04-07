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
#include <errno.h>
#include <mqueue.h>
#include <string.h>
#include <queue.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

extern sq_queue_t g_port_info_list;
/****************************************************************************
 * Name : messaging_cleanup
 * 
 * Description:
 *  Remove the information which related with the port.
 *  This API should be called if messaging_recv_nonblock or messaging_unicast_send_async was called.\n
 *  If this API is not called, memory leak can happen.
 *
 * Input Parameters:
 *	port_name : The message port name to clean(remove)\n
 * 
 * Return Value:
 *  On success, 0 (OK) is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
int messaging_cleanup(const char *port_name)
{
	int ret;
	char *internal_portname;
	msg_port_info_t *port_info;

	if (port_name == NULL) {
		msgdbg("[Messaging] unregister fail : invalid param.\n");
		return ERROR;
	}

	ret = messaging_handle_data(MSG_HANDLE_REMOVE, port_name, NULL, NULL);
	if (ret != OK) {
		msgdbg("[Messaging] unregister fail : remove recv info error.\n");
		return ERROR;
	}

	port_info = (msg_port_info_t *)sq_peek(&g_port_info_list);
	while (port_info != NULL) {
		if (strncmp(port_info->name, port_name, strlen(port_name) + 1) == 0) {
			MSG_ASPRINTF(&internal_portname, "%s%d", port_name, port_info->pid);
			mq_close(port_info->mqdes);
			sq_rem((FAR sq_entry_t *)port_info, &g_port_info_list);
			MSG_FREE(port_info->data);
			MSG_FREE(port_info);
			break;
		}
		port_info = (msg_port_info_t *)sq_next(port_info);
	}


	ret = mq_unlink(internal_portname);
	MSG_FREE(internal_portname);
	if (ret != OK && errno != ENOENT) {
		msgdbg("[Messaging] unregister fail : unlink error %d.\n", errno);
		return ERROR;
	}
	return OK;
}
