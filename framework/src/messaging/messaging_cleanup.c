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
#include <stdbool.h>
#include <queue.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

#define INVALID_PID (-1)

static int messaging_unlink_internalport(const char *port_name, int pid)
{
	int ret;
	char *internal_portname;

	/* User uses the port_name, but mqueue was opened with internal port name. */
	MSG_ASPRINTF(&internal_portname, "%s%d", port_name, pid);
	if (internal_portname == NULL) {
		msgdbg("[Messaging] cleanup fail : out of memory for internal_portname.\n");
		return ERROR;
	}

	ret = mq_unlink(internal_portname);
	MSG_FREE(internal_portname);
	if (ret != OK && errno != ENOENT) {
		msgdbg("[Messaging] unregister fail : unlink error, errno %d.\n", errno);
		return ERROR;
	}

	return OK;
}
/****************************************************************************
 * Name : messaging_cleanup
 * 
 * Description:
 *  Remove the information which related with the port.
 *  This API should be called if messaging_recv_nonblock or messaging_unicast_send_async was called.\n
 *  If this API is not called, memory leak can happen.
 *
 * Input Parameters:
 *	port_name : The message port name to clean(remove)
 * 
 * Return Value:
 *  On success, 0 (OK) is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
int messaging_cleanup(const char *port_name)
{
	int ret = ERROR;
	int cleanup_pid = INVALID_PID;
	msg_port_info_t *port_info;
	pid_t my_pid;
	sq_queue_t *port_info_list_ptr;

	if (port_name == NULL) {
		msgdbg("[Messaging] cleanup fail : invalid param.\n");
		return ERROR;
	}

	ret = FREE_MSG_RECEIVER(port_name);
	if (ret != OK) {
		return ERROR;
	}

	/* Remove the receiver information by port_name from the info list. */
	port_info_list_ptr = messaging_get_port_info_list();
	port_info = (msg_port_info_t *)sq_peek(port_info_list_ptr);
	if (port_info == NULL) {
		/* There is no registered port. So we don't need to clean anything. */
		return OK;
	}

	my_pid = getpid();
	do {
		if ((strncmp(port_info->name, port_name, strlen(port_name) + 1) == 0) && (my_pid == port_info->pid)) {
			cleanup_pid = port_info->pid;
			mq_close(port_info->mqdes);
			sq_rem((FAR sq_entry_t *)port_info, port_info_list_ptr);
			MSG_FREE(port_info->data);
			MSG_FREE(port_info);
			break;
		}
		port_info = (msg_port_info_t *)sq_next(port_info);
	} while (port_info != NULL);

	if (cleanup_pid != INVALID_PID) {
		ret = messaging_unlink_internalport(port_name, cleanup_pid);
	} else {
		ret = ERROR;
	}

	return ret;
}
