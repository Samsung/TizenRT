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

int bt_dpm_set_allow_bluetooth_mode(bt_dpm_allow_e value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_get_allow_bluetooth_mode(bt_dpm_allow_e *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_activate_device_restriction(bt_dpm_status_e value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_is_device_restriction_active(bt_dpm_status_e *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_activate_uuid_restriction(bt_dpm_status_e value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_is_uuid_restriction_active(bt_dpm_status_e *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_add_devices_to_blacklist(const char *device_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_add_devices_to_whitelist(const char *device_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_add_uuids_to_blacklist(const char *service_uuid)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_add_uuids_to_whitelist(const char *service_uuid)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_clear_devices_from_blacklist(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_clear_devices_from_whitelist(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_clear_uuids_from_blacklist(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_clear_uuids_from_whitelist(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_get_devices_from_blacklist(bt_dpm_device_list_s **device_list)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_get_devices_from_whitelist(bt_dpm_device_list_s **device_list)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_get_uuids_from_blacklist(bt_dpm_uuids_list_s **uuid_list)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_get_uuids_from_whitelist(bt_dpm_uuids_list_s **uuid_list)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_remove_device_from_whitelist(const char *device_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_remove_device_from_blacklist(const char *device_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_remove_uuid_from_whitelist(const char *service_uuid)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_remove_uuid_from_blacklist(const char *service_uuid)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_set_allow_outgoing_call(bt_dpm_status_e value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_get_allow_outgoing_call(bt_dpm_status_e *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_set_pairing_state(bt_dpm_status_e value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_get_pairing_state(bt_dpm_status_e *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_set_profile_state(bt_dpm_profile_e profile, bt_dpm_status_e value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_get_profile_state(bt_dpm_profile_e profile, bt_dpm_status_e *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_set_desktop_connectivity_state(bt_dpm_status_e value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_get_desktop_connectivity_state(bt_dpm_status_e *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_set_discoverable_state(bt_dpm_status_e value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_get_discoverable_state(bt_dpm_status_e *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_set_limited_discoverable_state(bt_dpm_status_e value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_get_limited_discoverable_state(bt_dpm_status_e *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_set_data_transfer_state(bt_dpm_status_e value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_dpm_get_data_transfer_state(bt_dpm_status_e *value)
{
	return BT_ERROR_NOT_SUPPORTED;
}
