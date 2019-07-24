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

int bt_hdp_register_sink_app(unsigned short data_type, char **app_id)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hdp_unregister_sink_app(const char *app_id)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hdp_send_data(unsigned int channel, const char *data, unsigned int size)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hdp_connect_to_source(const char *remote_address, const char *app_id)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hdp_disconnect(const char *remote_address, unsigned int channel)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hdp_set_connection_state_changed_cb(bt_hdp_connected_cb connected_cb,
		bt_hdp_disconnected_cb disconnected_cb, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hdp_unset_connection_state_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hdp_set_data_received_cb(bt_hdp_data_received_cb callback,
				void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hdp_unset_data_received_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}
