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
#include <bluetooth/bluetooth_internal.h>
#include "bluetooth_private.h"

int bt_hid_host_initialize(bt_hid_host_connection_state_changed_cb connection_cb,
								void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hid_host_deinitialize()
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hid_host_connect(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hid_host_disconnect(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_START */
int bt_hid_device_activate(bt_hid_device_connection_state_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hid_device_deactivate(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hid_device_connect(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}
int bt_hid_device_disconnect(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hid_device_send_mouse_event(const char *remote_address,
		const bt_hid_mouse_data_s *mouse_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hid_device_send_key_event(const char *remote_address,
		const bt_hid_key_data_s *key_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hid_device_send_custom_event(const char *remote_address,
		unsigned char report_id, const char *data, unsigned int data_len)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hid_device_reply_to_report(const char *remote_address,
		bt_hid_header_type_e header_type,
		bt_hid_param_type_e param_type,
		const char *data, unsigned int data_len)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hid_device_set_data_received_cb(bt_hid_device_data_received_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hid_device_unset_data_received_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */
