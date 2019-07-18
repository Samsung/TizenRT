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

#include "bt-adaptation-device.h"
#include "bluetooth_private.h"

/* LCOV_EXCL_START */
int bt_device_disconnect(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_device_create_bond(const char *device_address)
{
	int error_code = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(device_address);

	error_code = bt_adapt_bond_device(device_address);
	if (error_code != BT_ERROR_NONE)
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(error_code), error_code);

	return error_code;
}

/* LCOV_EXCL_START */
int bt_device_create_bond_by_type(const char *device_address,
			bt_device_connection_link_type_e conn_type)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_device_cancel_bonding(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_destroy_bond(const char *device_address)
{
	int error_code = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(device_address); /* LCOV_EXCL_LINE */

	error_code = bt_adapt_unbond_device(device_address); /* LCOV_EXCL_LINE */
	if (error_code != BT_ERROR_NONE) { /* LCOV_EXCL_LINE */
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(error_code), error_code); /* LCOV_EXCL_LINE */
	}

	return error_code; /* LCOV_EXCL_LINE */
}

int bt_device_set_alias(const char *device_address, const char *alias)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_set_authorization(const char *device_address, bt_device_authorization_e authorization)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_start_service_search(const char *device_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_device_cancel_service_search(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_device_foreach_connected_profiles(const char *remote_address, bt_device_connected_profile callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_is_profile_connected(const char *remote_address, bt_profile_e bt_profile,
				bool *connected_status)
{
	int ret;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(remote_address);

	*connected_status = false;
	ret = bt_adapt_is_device_connected(remote_address, bt_profile, connected_status);

	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret); /* LCOV_EXCL_LINE */
		return ret;
	}

	return ret;

}

int bt_device_set_bond_created_cb(bt_device_bond_created_cb callback, void *user_data)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(callback);
	_bt_set_cb(BT_EVENT_BOND_CREATED, callback, user_data);

	BT_DBG("+");

	return BT_ERROR_NONE;
}

int bt_device_set_bond_destroyed_cb(bt_device_bond_destroyed_cb callback, void *user_data)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(callback);
	_bt_set_cb(BT_EVENT_BOND_DESTROYED, callback, user_data);

	BT_DBG("+");

	return BT_ERROR_NONE;
}

int bt_device_set_authorization_changed_cb(bt_device_authorization_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_set_service_searched_cb(bt_device_service_searched_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_device_get_connection_state(const char *remote_address,
		bt_device_connection_link_type_e link_type, bool *connected)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_STOP */
int bt_device_set_connection_state_changed_cb(bt_device_connection_state_changed_cb callback, void *user_data)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(callback);
	_bt_set_cb(BT_EVENT_DEVICE_CONNECTION_STATUS, callback, user_data);

	return BT_ERROR_NONE;
}

/* LCOV_EXCL_START */
int bt_device_set_att_mtu_changed_cb(bt_device_att_mtu_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_device_unset_bond_created_cb(void)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	_bt_unset_cb(BT_EVENT_BOND_CREATED);

	BT_DBG("+");

	return BT_ERROR_NONE;
}

int bt_device_unset_bond_destroyed_cb(void)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	_bt_unset_cb(BT_EVENT_BOND_DESTROYED);

	BT_DBG("+");

	return BT_ERROR_NONE;
}

int bt_device_unset_authorization_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_unset_service_searched_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_unset_connection_state_changed_cb(void)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	_bt_unset_cb(BT_EVENT_DEVICE_CONNECTION_STATUS);
	return BT_ERROR_NONE;
}

/* LCOV_EXCL_START */
int bt_device_unset_rssi_alert_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_unset_att_mtu_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_le_conn_update(const char *device_address,
	const bt_le_conn_update_s *parameters)
{
	int ret = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(device_address);
	BT_CHECK_INPUT_PARAMETER(parameters);

	ret = bt_adapt_le_conn_update(device_address, parameters);

	if (BT_ERROR_NONE != ret)
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);

	return ret;
}
/* LCOV_EXCL_STOP */

int bt_device_get_service_mask_from_uuid_list(char **uuids,
				      int no_of_service,
				      bt_service_class_t *service_mask_list)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_device_set_pin_code(const char *remote_address, const char *pin_code)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_unset_pin_code(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_update_le_connection_mode(const char *remote_address, bt_device_le_connection_mode_e mode)
{
	int ret;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(remote_address);

	if (mode < BT_DEVICE_LE_CONNECTION_MODE_BALANCED ||
		 mode > BT_DEVICE_LE_CONNECTION_MODE_LOW_ENERGY) {
		return BT_ERROR_INVALID_PARAMETER;
	}

	ret = bt_adapt_update_le_connection_mode(remote_address, mode);
	if (BT_ERROR_NONE != ret)
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);

	return ret;
}

int bt_device_request_att_mtu(const char *remote_address, unsigned int mtu)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_get_att_mtu(const char *remote_address, unsigned int *mtu)
{
	return BT_ERROR_NOT_SUPPORTED;

}

int bt_device_get_id_address(const char *remote_rpa, char **id_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_set_profile_trusted(const char *device_address,
		bt_trusted_profile_t profile, bool trust)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_get_profile_trusted(const char *device_address,
		bt_trusted_profile_t profile, int *trust)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_set_trusted_profile_cb(bt_device_trusted_profiles_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_unset_trusted_profile_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_set_profile_restricted(const char *device_address,
		bt_restricted_profile_t profile, bool restricted)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_device_get_profile_restricted(const char *device_address,
		bt_restricted_profile_t profile, int *restricted)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_get_bonded_device_is_alias_set(const char *remote_address, bool *alias_set)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_STOP */
