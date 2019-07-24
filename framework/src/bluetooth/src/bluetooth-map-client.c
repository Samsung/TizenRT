/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

int bt_map_client_initialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_map_client_deinitialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_map_client_create_session(const char* remote_address,
			bt_map_client_session_info_h* handle)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_map_client_destroy_session(bt_map_client_session_info_h handle)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_map_client_set_folder(bt_map_client_session_info_h handle, const char *name)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_map_client_list_folders(bt_map_client_session_info_h handle,
			bt_map_client_list_folders_filter_h filter,
			bt_map_client_list_folders_cb callback,
			void* user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_map_client_list_filter_fields(bt_map_client_session_info_h handle,
			bt_map_client_list_filter_fields_cb callback,
			void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_map_client_list_messages(bt_map_client_session_info_h handle,
			const char* folder,
			bt_map_client_list_messages_filter_h filter,
			bt_map_client_list_messages_cb callback,
			void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_map_client_update_inbox(bt_map_client_session_info_h handle)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_map_client_push_message(bt_map_client_session_info_h handle,
			const char* source_file,
			const char* folder,
			bt_map_client_push_message_args_h args,
			bt_map_client_push_message_cb callback,
			void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_map_client_get_message(bt_map_client_session_info_h handle,
			const bt_map_client_message_object_h message_object,
			const char* target_file,
			bool attachment,
			bt_map_client_get_message_cb callback,
			void* user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}
