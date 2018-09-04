/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <stdlib.h>
#include <string.h>
#include <task_manager/task_manager.h>
#include <task_manager/task_manager_broadcast_list.h>
#include "../../../framework/src/task_manager/task_manager_internal.h"
#include "message_port.h"
#include "app_manager.h"
#include "bundle.h"

#define FIND_MSG_ID_WITH_PORT -1

msg_port_info_t msg_port_list[] = {
#include "msg_port_list.h"
	{NULL, 0, 0}
};

void tm_temp_cb(void *data, void *cb_data)
{
	(*((msg_port_data_t *)cb_data)->cb)(((msg_port_data_t *)cb_data)->port_id, ((msg_port_data_t *)cb_data)->app_id, ((msg_port_data_t *)cb_data)->port, false, (bundle *)data, ((tm_broadcast_info_t *)cb_data)->cb_data);
}

static int generate_local_port_id(void)
{
	static int id = 1;
	return id++;
}

static int find_msg_id(const char *port, int port_id)
{
	int port_idx;

	if (port == NULL && port_id < 0) {
		return MESSAGE_PORT_ERROR_PORT_NOT_FOUND;
	}

	for (port_idx = 0; msg_port_list[port_idx].msg_port != NULL; port_idx++) {
		if (port != NULL) {
			if (strncmp(msg_port_list[port_idx].msg_port, port, strlen(port) + 1) == 0) {
				return msg_port_list[port_idx].msg_id;
			}
		} else {
			if (msg_port_list[port_idx].port_id == port_id) {
				return msg_port_list[port_idx].msg_id;
			}
		}
	}
	return MESSAGE_PORT_ERROR_PORT_NOT_FOUND;
}

static void set_port_id(int msg_id, int port_id)
{
	int port_idx;
	for (port_idx = 0; msg_port_list[port_idx].msg_port != NULL; port_idx++) {
		if (msg_port_list[port_idx].msg_id == msg_id) {
			msg_port_list[port_idx].port_id = port_id;
			return;
		}
	}
}

int message_port_register_local_port(const char *local_port, message_port_message_cb callback, void *user_data)
{
	int ret;
	int msg_id = 0;
	int port_id = 0;
	msg_port_data_t *data;

	if (local_port == NULL || callback == NULL) {
		return MESSAGE_PORT_ERROR_INVALID_PARAMETER;
	}

	msg_id = find_msg_id(local_port, FIND_MSG_ID_WITH_PORT);
	if (msg_id == MESSAGE_PORT_ERROR_PORT_NOT_FOUND) {
		printf("MESSAGE_PORT_ERROR_PORT_NOT_FOUND\n");
		return MESSAGE_PORT_ERROR_PORT_NOT_FOUND;
	}

	port_id = generate_local_port_id();
	set_port_id(msg_id, port_id);

	data = (msg_port_data_t *)malloc(sizeof(msg_port_data_t));
	if (data == NULL) {
		printf("MESSAGE_PORT_ERROR_OUT_OF_MEMORY\n");
		return MESSAGE_PORT_ERROR_OUT_OF_MEMORY;
	}

	data->cb = callback;
	data->user_data = user_data;
	data->port_id = port_id;
	data->app_id = NULL;
	app_manager_get_app_id(getpid(), &data->app_id);
	strcpy(data->port, local_port);

	ret = task_manager_set_broadcast_cb(msg_id, tm_temp_cb, data);
	if (ret != OK) {
		free(data->app_id);
		free(data);
		printf("MESSAGE_PORT_ERROR_PORT_NOT_FOUND\n");
		return MESSAGE_PORT_ERROR_PORT_NOT_FOUND;
	}

	return MESSAGE_PORT_ERROR_NONE;
}

int message_port_unregister_local_port(int local_port_id)
{
	int ret;
	int msg_id = 0;

	if (local_port_id <= 0) {
		return MESSAGE_PORT_ERROR_INVALID_PARAMETER;
	}

	msg_id = find_msg_id(NULL, local_port_id);
	if (msg_id == MESSAGE_PORT_ERROR_PORT_NOT_FOUND) {
		return MESSAGE_PORT_ERROR_PORT_NOT_FOUND;
	}

	ret = task_manager_unset_broadcast_cb(msg_id, TM_RESPONSE_WAIT_INF);
	if (ret == TM_OUT_OF_MEMORY) {
		return MESSAGE_PORT_ERROR_OUT_OF_MEMORY;
	} else if (ret < 0) {
		return MESSAGE_PORT_ERROR_PORT_NOT_FOUND;
	}

	return MESSAGE_PORT_ERROR_NONE;
}

int message_port_check_remote_port(const char *remote_app_id, const char *remote_port, bool *exist)
{
	if (find_msg_id(remote_port, FIND_MSG_ID_WITH_PORT) == MESSAGE_PORT_ERROR_PORT_NOT_FOUND) {
		*exist = false;
	} else {
		*exist = true;
	}

	return MESSAGE_PORT_ERROR_NONE;
}

int message_port_send_message(const char *remote_app_id, const char *remote_port, bundle *message)
{
	int ret;
	int msg_id = 0;
	tm_msg_t data;
	bundle *dup_b;

	if (remote_app_id == NULL || remote_port == NULL || message == NULL) {
		return MESSAGE_PORT_ERROR_INVALID_PARAMETER;
	}

	msg_id = find_msg_id(remote_port, FIND_MSG_ID_WITH_PORT);
	if (msg_id == MESSAGE_PORT_ERROR_PORT_NOT_FOUND) {
		return MESSAGE_PORT_ERROR_PORT_NOT_FOUND;
	}

	dup_b = bundle_dup(message);

	data.msg_size = sizeof(bundle *);
	data.msg = (void *)dup_b;

	ret = task_manager_broadcast(msg_id, &data);
	if (ret == TM_OUT_OF_MEMORY) {
		return MESSAGE_PORT_ERROR_OUT_OF_MEMORY;
	} else if (ret < 0) {
		return MESSAGE_PORT_ERROR_PORT_NOT_FOUND;
	}

	free(data.msg);
	return MESSAGE_PORT_ERROR_NONE;
}

int message_port_register_trusted_local_port(const char *trusted_local_port, message_port_trusted_message_cb callback, void *user_data)
{
	return message_port_register_local_port(trusted_local_port, callback, user_data);
}

int message_port_unregister_trusted_local_port(int trusted_local_port_id)
{
	return message_port_unregister_local_port(trusted_local_port_id);
}

int message_port_send_trusted_message(const char *remote_app_id, const char *remote_port, bundle *message)
{
	return message_port_send_message(remote_app_id, remote_port, message);
}
