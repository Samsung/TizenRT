/*
 * Bluetooth-frwk
 *
 * Contact: Abhishek Chandra (abhishek.ch@samsung.com)
 *
 * Copyright (c) 2018-2019 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <tinyara/config.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/bluetooth_internal.h>
#include "bluetooth_private.h"

int bt_hrp_sensor_set_heartrate_value(int heartrate)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_sensor_set_location_value(bt_body_sensor_location_e location)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_sensor_set_energy_value(int energy_value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_sensor_set_contact_value(bool enable)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_sensor_set_rr_interval(int interval)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_sensor_set_device_manufacturer_name(char *manuf_name)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_sensor_create(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_sensor_destroy(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_collector_create(const char *remote_address, bt_hrp_collector_h *collector)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_collector_destory(bt_hrp_collector_h collector)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_collector_set_connection_state_changed_cb(bt_hrp_collector_h collector,
		bt_hrp_collector_connection_state_changed_cb callback,
		void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_collector_unset_connection_state_changed_cb(bt_hrp_collector_h collector)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_collector_start_scan(bt_hrp_collector_scan_result_cb cb, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_collector_stop_scan(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_collector_connect(bt_hrp_collector_h collector)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_collector_disconnect(bt_hrp_collector_h collector)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_collector_set_notification(bt_hrp_collector_h collector,
		bt_hrp_collector_heart_rate_value_changed_cb callback,
		bool isNotify)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hrp_collector_get_body_sensor_location(bt_hrp_collector_h collector,
			bt_hrp_collector_bsl_read_completed_cb bsl_read_cb)
{
	return BT_ERROR_NOT_SUPPORTED;
}

