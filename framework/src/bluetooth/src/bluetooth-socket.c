/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <tinyara/config.h>
#include <bluetooth/bluetooth.h>
#include "bluetooth_private.h"

int bt_socket_create_rfcomm(const char *uuid, int *socket_fd)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_destroy_rfcomm(int socket_fd)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_socket_is_service_used(const char *service_uuid, bool *used)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_socket_listen_and_accept_rfcomm(int socket_fd, int max_pending_connections)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_socket_listen(int socket_fd, int max_pending_connections)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_accept(int socket_fd)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_reject(int socket_fd)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_create_rfcomm_ex(const char *uuid, const char *bus_name, const char *object_path)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_destroy_rfcomm_ex(const char *uuid)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_listen_and_accept_rfcomm_ex(const char *uuid, int max_pending_connections, const char* bus_name, const char *object_path)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_STOP */
int bt_socket_connect_rfcomm(const char *remote_address, const char *remote_port_uuid)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_disconnect_rfcomm(int socket_fd)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_send_data(int socket_fd, const char *data, int length)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_set_data_received_cb(bt_socket_data_received_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_unset_data_received_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_set_connection_requested_cb(bt_socket_connection_requested_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_unset_connection_requested_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_set_connection_state_changed_cb(bt_socket_connection_state_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_socket_unset_connection_state_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

