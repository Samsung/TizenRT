/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd All Rights Reserved
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
#include <bluetooth/bluetooth_internal.h>
#include "bluetooth_private.h"

int bt_otp_server_initialize(const char *directory)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_server_deinitialize()
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_set_server_state_changed_cb(bt_otp_server_state_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;

}
int bt_otp_unset_server_state_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_set_connection_state_changed_cb(bt_otp_client_h otp_client,
			bt_otp_client_state_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_unset_connection_state_changed_cb(bt_otp_client_h otp_client)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_create(const char *remote_address, bt_otp_client_h *otp_client)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_destroy(bt_otp_client_h otp_client)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_discover_all_objects(bt_otp_client_h otp_client,
										bt_otp_client_object_discovery_cb callback,
										void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_connect(bt_otp_client_h otp_client)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_disconnect(bt_otp_client_h otp_client)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_select_object(bt_otp_client_h otp_client, unsigned long long id,
				bt_otp_client_object_select_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_read_object_contents(bt_otp_client_h otp_client,
					bt_otp_client_read_object_complete_cb callback,
					void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_create_object(bt_otp_client_h otp_client,
					const char *file_path,
					bt_otp_client_object_create_cb callback,
					void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_write_object(bt_otp_client_h otp_client,
					const char *file_path,
					int offset, int length, int mode,
					bt_otp_client_object_write_cb callback,
					void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_execute_object(bt_otp_client_h otp_client,
							bt_otp_client_object_execute_cb callback,
							void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_otp_client_delete_object(bt_otp_client_h otp_client,
							bt_otp_client_object_delete_cb callback,
							void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}
