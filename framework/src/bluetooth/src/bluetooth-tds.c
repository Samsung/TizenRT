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

int bt_tds_set_transport_activation_requested_cb(
		bt_tds_activation_requested_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_unset_transport_activation_requested_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_provider_register(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_provider_unregister(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_provider_create(bt_tds_provider_h *provider, bt_tds_transport_e transport)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_provider_destroy(bt_tds_provider_h provider)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_provider_set_transport_data(bt_tds_provider_h provider,
		bt_tds_transport_state_e transport_state, unsigned char *buf, int length)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_provider_send_activation_resp(char *address, int result, bt_tds_provider_h provider)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_provider_set_manufacturer_data(unsigned char *buf, int length)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_start_seeking_providers(bt_tds_provider_scan_result_cb cb, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_stop_seeking_providers(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_seeker_set_connection_state_changed_cb(bt_tds_seeker_h seeker,
			bt_tds_seeker_connection_state_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_seeker_unset_connection_state_changed_cb(bt_tds_seeker_h seeker)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_seeker_activate_control_point(bt_tds_seeker_h seeker,
				bt_tds_transport_e transport, unsigned char *buffer, int len,
				bt_tds_control_point_activation_indication_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_seeker_get_complete_transport_blocks(bt_tds_seeker_h seeker,
				bt_tds_seeker_complete_transport_data_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_seeker_create(const char *remote_address, bt_tds_seeker_h *seeker)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_seeker_destroy(bt_tds_seeker_h seeker)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_seeker_connect(bt_tds_seeker_h seeker)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_tds_seeker_disconnect(bt_tds_seeker_h seeker)
{
	return BT_ERROR_NOT_SUPPORTED;
}
