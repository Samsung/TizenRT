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

int bt_proximity_reporter_create(bt_proximity_reporter_h *reporter)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_reporter_destroy(bt_proximity_reporter_h reporter)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_monitor_create(const char *remote_address, bt_proximity_monitor_h *monitor)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_monitor_destroy(bt_proximity_monitor_h monitor)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_monitor_connect(bt_proximity_monitor_h monitor)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_monitor_disconnect(bt_proximity_monitor_h monitor)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_monitor_set_connection_state_changed_cb(bt_proximity_monitor_h monitor,
				bt_proximity_monitor_connection_state_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_monitor_unset_connection_state_changed_cb(bt_proximity_monitor_h monitor)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_reporter_set_connection_state_changed_cb(bt_proximity_reporter_h reporter,
		bt_proximity_reporter_connection_state_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_reporter_unset_connection_state_changed_cb(bt_proximity_reporter_h reporter)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_reporter_set_property_changed_cb(bt_proximity_reporter_h reporter,
		bt_proximity_reporter_property_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_reporter_unset_property_changed_cb(bt_proximity_reporter_h reporter)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_monitor_set_linkloss_alert(bt_proximity_monitor_h monitor, int value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_monitor_set_immediate_alert(bt_proximity_monitor_h monitor, int value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_monitor_get_linkloss_alert(bt_proximity_monitor_h monitor, int *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_monitor_get_immediate_alert(bt_proximity_monitor_h monitor, int *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_monitor_get_signal_level(bt_proximity_monitor_h monitor, int *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_reporter_get_linkloss_alert(const char *remote_address, int *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_proximity_reporter_get_immediate_alert(const char *remote_address, int *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}
