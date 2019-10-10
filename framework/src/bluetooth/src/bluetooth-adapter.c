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
#ifdef GLIB_SUPPORT
#include <glib.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/bluetooth_internal.h>

#include "bt-adaptation-adapter.h"
#include "bt-adaptation-device.h"
#include "bluetooth_private.h"

#define BT_NAME_LEN_MAX 248
#define BT_ADDR_STR_LEN 18 /**< This specifies BT device address length (AA:BB:CC:DD:EE:FF) */

#define BT_CHECK_LE_SUPPORT() \
{ \
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON); \
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_LE); \
}

#define BT_CHECK_LE_5_0_SUPPORT() \
{ \
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_LE); \
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_LE_5_0); \
}

#ifdef GLIB_SUPPORTED
static GSList *advertiser_list = NULL;
#else
sq_queue_t advertiser_list;
#endif

/* LCOV_EXCL_START */
int bt_adapter_enable(void)
{
	int error_code = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	error_code = bt_adapt_enable();
	if (error_code != BT_ERROR_NONE) { /* LCOV_EXCL_LINE */
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(error_code),
					error_code); /* LCOV_EXCL_LINE */
	} /* LCOV_EXCL_LINE */
	return error_code;
}

int bt_adapter_disable(void)
{
	int error_code = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	error_code = bt_adapt_disable();
	if (error_code != BT_ERROR_NONE) { /* LCOV_EXCL_LINE */
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(error_code),
					error_code); /* LCOV_EXCL_LINE */
	} /* LCOV_EXCL_LINE */
	return error_code;
}

int bt_adapter_recover(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_reset(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_adapter_get_state(bt_adapter_state_e *adapter_state)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(adapter_state);

	return bt_adapt_get_state(adapter_state);
}

/* LCOV_EXCL_START */
int bt_adapter_le_enable(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_disable(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_get_state(bt_adapter_le_state_e *adapter_le_state)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_adapter_get_address(char **address)
{
	int error_code = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();

	*address = calloc(1, BT_ADDR_STR_LEN * sizeof(char));

	error_code = bt_adapt_get_local_address(address);
	if (error_code != BT_ERROR_NONE) { /* LCOV_EXCL_LINE */
		free(*address);
		*address = NULL;
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(error_code), /* LCOV_EXCL_LINE */
					error_code); /* LCOV_EXCL_LINE */
		return error_code; /* LCOV_EXCL_LINE */
	}

	return BT_ERROR_NONE;
}

/* LCOV_EXCL_START */
int bt_adapter_get_version(char **version)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
int bt_adapter_get_local_info(char **chipset, char **firmware,
			char **stack_version, char **profiles)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_adapter_get_name(char **name)
{
	int ret = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();

	*name = calloc(1, BT_ADDR_STR_LEN * sizeof(char));

	ret = bt_adapt_get_local_name(name);
	if (ret != BT_ERROR_NONE) { /* LCOV_EXCL_LINE */
		free(*name);
		*name = NULL;

		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), /* LCOV_EXCL_LINE */
						ret); /* LCOV_EXCL_LINE */
		return ret; /* LCOV_EXCL_LINE */
	}

	return BT_ERROR_NONE;
}

int bt_adapter_set_name(const char *name)
{
	int ret = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(name);

	ret = bt_adapt_set_local_name(name);
	if (ret != BT_ERROR_NONE) { /* LCOV_EXCL_LINE */
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), /* LCOV_EXCL_LINE */
						ret); /* LCOV_EXCL_LINE */
	} /* LCOV_EXCL_LINE */

	return ret;
}

/* LCOV_EXCL_START */
int bt_adapter_get_visibility(bt_adapter_visibility_mode_e *mode, int *duration)
{
	int ret = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(mode);

	ret = bt_adapt_get_discoverable_mode(mode);
	if (ret != BT_ERROR_NONE) { /* LCOV_EXCL_LINE */
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret),
						ret); /* LCOV_EXCL_LINE */
		return ret; /* LCOV_EXCL_LINE */
	}

	/* Not support BLUETOOTH_DISCOVERABLE_MODE_TIME_LIMITED_DISCOVERABLE */
	*duration = 0;

	return ret;
}

int bt_adapter_set_visibility(bt_adapter_visibility_mode_e visibility_mode,
							int timeout_sec)
{
	int error_code = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();

	if (visibility_mode == BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE) {
		/* Not support BLUETOOTH_DISCOVERABLE_MODE_TIME_LIMITED_DISCOVERABLE */
		return BT_ERROR_NOT_SUPPORTED;
	}

	error_code = bt_adapt_set_discoverable_mode(visibility_mode);
	if (error_code != BT_ERROR_NONE) { /* LCOV_EXCL_LINE */
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(error_code),
					error_code); /* LCOV_EXCL_LINE */
	} /* LCOV_EXCL_LINE */

	return error_code;
}

int bt_adapter_set_connectable_changed_cb(bt_adapter_connectable_changed_cb callback,
							void *user_data)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(callback);
	_bt_set_cb(BT_EVENT_CONNECTABLE_CHANGED_EVENT, callback, user_data);
	return BT_ERROR_NONE;
}

int bt_adapter_unset_connectable_changed_cb(void)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	_bt_unset_cb(BT_EVENT_CONNECTABLE_CHANGED_EVENT);
	return BT_ERROR_NONE;
}

int bt_adapter_get_connectable(bool *connectable)
{
	int ret = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(connectable);

	ret = bt_adapt_is_connectable(connectable);
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
		return ret;
	}

	return BT_ERROR_NONE;
}

int bt_adapter_set_connectable(bool connectable)
{
	int error_code = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();

	error_code = bt_adapt_set_connectable(connectable);
	if (error_code != BT_ERROR_NONE) { /* LCOV_EXCL_LINE */
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(error_code),
					error_code); /* LCOV_EXCL_LINE */
	} /* LCOV_EXCL_LINE */

	return error_code;
}
/* LCOV_EXCL_STOP */

int bt_adapter_foreach_bonded_device(bt_adapter_bonded_device_cb foreach_cb,
							void *user_data)
{
#ifdef GLIB_SUPPORTED
	GPtrArray *dev_list = NULL;
	bt_device_info_s *dev_info = NULL;
	int ret = BT_ERROR_NONE;
	int i = 0;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(foreach_cb);

	dev_list = g_ptr_array_new();
	if (dev_list == NULL) {
		BT_ERR("OUT_OF_MEMORY(0x%08x)", /* LCOV_EXCL_LINE */
				BT_ERROR_OUT_OF_MEMORY); /* LCOV_EXCL_LINE */
		return BT_ERROR_OUT_OF_MEMORY;
	}

	ret = bt_adapt_get_bonded_device_list(&dev_list);
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x) : Failed to get bonded device list", /* LCOV_EXCL_LINE */
		_bt_convert_error_to_string(ret), ret); /* LCOV_EXCL_LINE */
		g_ptr_array_free(dev_list, TRUE);
		return ret;
	}

	for (i = 0; i < dev_list->len; i++) { /* LCOV_EXCL_LINE */
		dev_info = g_ptr_array_index(dev_list, i);
		if (dev_info != NULL) {
			if (!foreach_cb(dev_info, user_data))
				break;
		} else {
			BT_ERR("OPERATION_FAILED(0x%08x)", /* LCOV_EXCL_LINE */
			BT_ERROR_OPERATION_FAILED); /* LCOV_EXCL_LINE */
			ret = BT_ERROR_OPERATION_FAILED;
			break;
		}
	}

	g_ptr_array_foreach(dev_list, free, NULL);
	g_ptr_array_free(dev_list, TRUE);

	return ret;
#else
	sq_queue_t dev_list;
//	bt_device_info_s *dev_info = NULL;
	struct bt_device_info_t *dev_info_internal = NULL;
	int ret = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(foreach_cb);

	sq_init(&dev_list);

	ret = bt_adapt_get_bonded_device_list(&dev_list);
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x) : Failed to get bonded device list", _bt_convert_error_to_string(ret), ret); /* LCOV_EXCL_LINE */
		return ret;
	}

	dev_info_internal = (struct bt_device_info_t *)sq_peek(&dev_list);
	if (!dev_info_internal) {
		BT_ERR("OPERATION_FAILED(0x%08x)", BT_ERROR_OPERATION_FAILED); /* LCOV_EXCL_LINE */
		return BT_ERROR_OPERATION_FAILED;
	}

	while (dev_info_internal) {
		if (!foreach_cb(dev_info_internal->dev_info, user_data))
			break;
		dev_info_internal = (struct bt_device_info_t *)sq_next(dev_info_internal);
	}

	return ret;
#endif
}

int bt_adapter_get_bonded_device_info(const char *remote_address,
					bt_device_info_s **device_info)
{
	int ret;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(remote_address); /* LCOV_EXCL_START */

	*device_info = (bt_device_info_s *)calloc(1, sizeof(bt_device_info_s));
	ret = bt_adapt_get_bonded_device(remote_address, *device_info);
	if (ret != BT_ERROR_NONE) {
		free(*device_info);
		*device_info = NULL;

		BT_ERR("%s(0x%08x) : Failed to run function",
				_bt_convert_error_to_string(ret),
				ret);
	}

	return ret; /* LCOV_EXCL_STOP */
}

int bt_adapter_free_device_info(bt_device_info_s *device_info)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(device_info);

	_bt_free_bt_device_info_s(device_info); /* LCOV_EXCL_LINE */

	return BT_ERROR_NONE; /* LCOV_EXCL_LINE */
}

int bt_adapter_is_service_used(const char *service_uuid, bool *used)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_adapter_foreach_profile_connected_devices(const char *profile_uuid,
	bt_adapter_profile_connected_devices_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_adapter_set_state_changed_cb(bt_adapter_state_changed_cb callback,
							void *user_data)
{
	BT_DBG("");
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(callback);

	_bt_set_cb(BT_EVENT_STATE_CHANGED, callback, user_data);

	return BT_ERROR_NONE;
}

/* LCOV_EXCL_START */
int bt_adapter_le_set_state_changed_cb(bt_adapter_le_state_changed_cb callback,
							void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_adapter_set_name_changed_cb(bt_adapter_name_changed_cb callback,
							void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_set_visibility_mode_changed_cb(bt_adapter_visibility_mode_changed_cb callback,
							void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_set_device_discovery_state_changed_cb(bt_adapter_device_discovery_state_changed_cb callback,
							void *user_data)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(callback);

	_bt_set_cb(BT_EVENT_DEVICE_DISCOVERY_STATE_CHANGED, callback, user_data);

	return BT_ERROR_NONE;
}

int bt_adapter_unset_state_changed_cb(void)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	_bt_unset_cb(BT_EVENT_STATE_CHANGED);
	return BT_ERROR_NONE;
}

/* LCOV_EXCL_START */
int bt_adapter_le_unset_state_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_adapter_unset_name_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_unset_visibility_mode_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_set_visibility_duration_changed_cb(bt_adapter_visibility_duration_changed_cb callback,
							void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_unset_visibility_duration_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_unset_device_discovery_state_changed_cb(void)
{
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	_bt_unset_cb(BT_EVENT_DEVICE_DISCOVERY_STATE_CHANGED);
	return BT_ERROR_NONE;
}

int bt_adapter_start_device_discovery(void)
{
	int error_code = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	error_code = bt_adapt_start_discovery();
	if (error_code != BT_ERROR_NONE) { /* LCOV_EXCL_LINE */
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(error_code),
					error_code); /* LCOV_EXCL_LINE */
	} /* LCOV_EXCL_LINE */
	return error_code;
}

int bt_adapter_stop_device_discovery(void)
{
	int error_code = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	error_code = bt_adapt_stop_discovery();
	if (error_code != BT_ERROR_NONE) { /* LCOV_EXCL_LINE */
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(error_code),
					error_code); /* LCOV_EXCL_LINE */
	} /* LCOV_EXCL_LINE */
	return error_code;
}

int bt_adapter_is_discovering(bool *is_discovering)
{
	int ret = BT_ERROR_NONE;

	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_COMMON);
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(is_discovering);

	ret = bt_adapt_is_discovering(is_discovering);
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret),
						ret); /* LCOV_EXCL_LINE */
	}

	return ret;
}

int bt_adapter_le_is_discovering(bool *is_discovering)
{
	int ret = BT_ERROR_NONE;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(is_discovering);

	ret = bt_adapt_is_le_scanning(is_discovering);
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret),
						ret); /* LCOV_EXCL_LINE */
	}

	return ret;
}

int bt_adapter_get_local_oob_data(unsigned char **hash,
					unsigned char **randomizer,
					int *hash_len, int *randomizer_len)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_set_remote_oob_data(const char *remote_address,
				unsigned char *hash, unsigned char *randomizer,
				int hash_len, int randomizer_len)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_get_local_oob_ext_data(unsigned char **hash192, unsigned char **randomizer192,
		int *hash192_len, int *randomizer192_len,
		unsigned char **hash256, unsigned char **randomizer256,
		int *hash256_len, int *randomizer256_len)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_set_remote_oob_ext_data(const char *remote_address,
		const unsigned char *hash192, const unsigned char *randomizer192,
		int hash192_len, int randomizer192_len,
		const unsigned char *hash256, const unsigned char *randomizer256,
		int hash256_len, int randomizer256_len)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_adapter_le_set_remote_oob_ext_data(const char *remote_address,
		bt_device_address_type_e address_type,
		const unsigned char *hash256, const unsigned char *randomizer256,
		int hash256_len, int randomizer256_len)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_adapter_remove_remote_oob_data(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_adapter_set_manufacturer_data(char *data, int len)
{

	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_parse_manufacturer_data(bt_manufacturer_data *data, char *manufacturer_data, int manufacturer_data_len)
{

	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_free_manufacturer_data(bt_manufacturer_data *data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_set_manufacturer_data_changed_cb(
		bt_adapter_manufacturer_data_changed_cb callback,
		void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_unset_manufacturer_data_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_set_passkey_notification(
		bt_adapter_passkey_notification_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_unset_passkey_notification(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_add_white_list(const char *address,
				bt_device_address_type_e address_type)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_remove_white_list(const char *address, bt_device_address_type_e address_type)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_adapter_le_set_scan_mode(bt_adapter_le_scan_mode_e scan_mode)
{
	int ret = BT_ERROR_NONE;
	int type;
	unsigned int interval;
	unsigned int window;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();

	type = BT_ADAPTER_LE_ACTIVE_SCAN;

	if (scan_mode == BT_ADAPTER_LE_SCAN_MODE_BALANCED) {
		interval = 5000;
		window = 2000;
	} else if (scan_mode == BT_ADAPTER_LE_SCAN_MODE_LOW_LATENCY) {
		interval = 5000;
		window = 5000;
	} else if (scan_mode == BT_ADAPTER_LE_SCAN_MODE_LOW_ENERGY) {
		interval = 5000;
		window = 500;
	} else
		return BT_ERROR_INVALID_PARAMETER;

	ret = bt_adapt_set_scan_params(type, interval, window);
	if (ret != BT_ERROR_NONE) { /* LCOV_EXCL_LINE */
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret),
						ret); /* LCOV_EXCL_LINE */
	} /* LCOV_EXCL_LINE */
	return ret;
}

/* LCOV_EXCL_START */
int bt_adapter_le_set_customized_scan_mode(float scan_interval, float scan_window)
{
	int ret = BT_ERROR_NONE;
	int type;
	unsigned int interval;
	unsigned int window;

	BT_CHECK_INIT_STATUS();

	type = BT_ADAPTER_LE_ACTIVE_SCAN;
	interval = (unsigned int)scan_interval;
	window = (unsigned int)scan_window;

	ret = bt_adapt_set_scan_params(type, interval, window);
	if (ret != BT_ERROR_NONE)
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);

	return ret;
}
/* LCOV_EXCL_STOP */

int bt_adapter_le_create_advertiser(bt_advertiser_h *advertiser)
{
#ifdef GLIB_SUPPORTED
	bt_advertiser_s *__adv = NULL;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	__adv = (bt_advertiser_s *)g_malloc(sizeof(bt_advertiser_s));
	__adv->handle = GPOINTER_TO_INT(__adv);

	*advertiser = (bt_advertiser_h)__adv;

	advertiser_list = g_slist_append(advertiser_list, __adv);
#else
	bt_advertiser_s *__adv = NULL;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	__adv = (bt_advertiser_s *)calloc(1, sizeof(bt_advertiser_s));
	if (__adv == NULL)
		return BT_ERROR_OUT_OF_MEMORY;

	__adv->handle = (int)__adv;

	*advertiser = (bt_advertiser_h)__adv;

	sq_init(&advertiser_list);
	sq_addlast((sq_entry_t *)__adv, &advertiser_list);
#endif

	return BT_ERROR_NONE;
}

int bt_adapter_le_destroy_advertiser(bt_advertiser_h advertiser)
{
	int ret = BT_ERROR_NONE;
	bool is_advertising = FALSE;

	bt_advertiser_s *__adv = (bt_advertiser_s *)advertiser;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	_bt_unset_cb(BT_EVENT_ADVERTISING_STATE_CHANGED);

	/* This operation is not related to the result */
	ret = bt_adapt_is_adv(&is_advertising);

	if (is_advertising == TRUE) {
		ret = bt_adapt_stop_adv(__adv->handle);

		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret),
							ret); /* LCOV_EXCL_LINE */

			if (ret == BT_ERROR_NOT_IN_PROGRESS) {
				/* Other advertising handle is in there */
				ret = BT_ERROR_NONE;
			}
		}
	}

#ifdef GLIB_SUPPORTED
	advertiser_list = g_slist_remove(advertiser_list, __adv);
#else
	sq_rem((sq_entry_t *)__adv, &advertiser_list);
#endif

	/* Free advertising data */
	free(__adv->adv_data);
	free(__adv->scan_rsp_data);
	free(__adv);

	return ret;
}

/* LCOV_EXCL_START */
static int __bt_remove_ad_data_by_type(char *in_data, unsigned int in_len,
		char in_type, char **data, unsigned int *data_len)
{
	if (in_data == NULL || data == NULL || data_len == NULL)
		return BT_ERROR_OPERATION_FAILED;

	int i;
	int len = 0;
	int type = 0;
	char *p;

	for (i = 0; i < in_len; i++) {
		len = in_data[i];
		if (len <= 0 || i + 1 >= in_len) {
			BT_ERR("Invalid advertising data");
			return BT_ERROR_OPERATION_FAILED;
		}

		type = in_data[i + 1];
		if (type == in_type) {
			i = i + 2;
			len--;
			break;
		}

		i += len;
		len = 0;
	}

	if (i + len > in_len) {
		BT_ERR("Invalid advertising data");
		return BT_ERROR_OPERATION_FAILED;
	} else if (len == 0) {
		BT_INFO("AD Type 0x%02x data is not set", in_type);
		return BT_ERROR_OPERATION_FAILED;
	}

	p = (char *)malloc(sizeof(char) *(in_len - len));
	if (p == NULL)
		return BT_ERROR_OUT_OF_MEMORY;

	memcpy(p, in_data, sizeof(char) *i);
	memcpy(p + i, in_data + i + len, sizeof(char) *(in_len - len - i));

	*data = p;
	*data_len = in_len - len;

	return BT_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

static int bt_adapter_le_add_advertising_data(bt_advertiser_h advertiser,
		bt_adapter_le_packet_type_e pkt_type,
		bt_adapter_le_advertising_data_type_e data_type,
		void *data, unsigned int data_size)
{
	bt_advertiser_s *__adv = (bt_advertiser_s *)advertiser;
	char **p;
	unsigned int *len;
	unsigned int *system_data_len;
	char *new_p;
	int adv_flag_len = 0;
	int adv_ext_len = 0;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	if ((data == NULL || data_size == 0) &&
		(data_type != BT_ADAPTER_LE_ADVERTISING_DATA_LOCAL_NAME &&
		data_type != BT_ADAPTER_LE_ADVERTISING_DATA_TX_POWER_LEVEL)) {
		BT_ERR("empty data for packet type %d",
					pkt_type); /* LCOV_EXCL_LINE */
		return BT_ERROR_INVALID_PARAMETER; /* LCOV_EXCL_LINE */
	}

	BT_CHECK_INPUT_PARAMETER(advertiser);

	if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING) {
		p = &__adv->adv_data;
		len = &__adv->adv_data_len;
		system_data_len = &__adv->adv_system_data_len;
		adv_flag_len = 3;
	} else if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE) {
		p = &__adv->scan_rsp_data;
		len = &__adv->scan_rsp_data_len;
		system_data_len = &__adv->scan_rsp_system_data_len;
	} else
		return BT_ERROR_INVALID_PARAMETER;

	if (data_type == BT_ADAPTER_LE_ADVERTISING_DATA_LOCAL_NAME ||
		data_type == BT_ADAPTER_LE_ADVERTISING_DATA_TX_POWER_LEVEL)
		adv_ext_len = *system_data_len + 1;

	/* 2 bytes are required for Length and AD Type */
	if (adv_flag_len + *len + adv_ext_len + data_size + 2 > 31) {
		BT_ERR("Quota exceeded"); /* LCOV_EXCL_LINE */
		return BT_ERROR_QUOTA_EXCEEDED; /* LCOV_EXCL_LINE */
	}

	if (*len == 0)
		*p = NULL;

	new_p = realloc(*p, sizeof(char) *(*len + data_size + 2));
	if (new_p == NULL)
		return BT_ERROR_OUT_OF_MEMORY;

	new_p[*len] = data_size + 1;
	new_p[*len + 1] = data_type;
	if ((data != NULL) && (data_size != 0))
		memcpy(new_p + (*len + 2), data, data_size);

	*p = new_p;
	*len += data_size + 2;

	if (data_type == BT_ADAPTER_LE_ADVERTISING_DATA_LOCAL_NAME ||
		data_type == BT_ADAPTER_LE_ADVERTISING_DATA_TX_POWER_LEVEL)
		*system_data_len += 1;

	return BT_ERROR_NONE;
}

/* LCOV_EXCL_START */
static int bt_adapter_le_remove_advertising_data(bt_advertiser_h advertiser,
		bt_adapter_le_packet_type_e pkt_type,
		bt_adapter_le_advertising_data_type_e data_type)
{
	int ret = BT_ERROR_NONE;
	bt_advertiser_s *__adv = (bt_advertiser_s *)advertiser;
	char **p;
	unsigned int *len;
	unsigned int *system_data_len;
	char *new_p = NULL;
	unsigned int new_len = 0;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	switch (pkt_type) {
	case BT_ADAPTER_LE_PACKET_ADVERTISING:
		p = &__adv->adv_data;
		len = &__adv->adv_data_len;
		system_data_len = &__adv->adv_system_data_len;
		break;

	case BT_ADAPTER_LE_PACKET_SCAN_RESPONSE:
		p = &__adv->scan_rsp_data;
		len = &__adv->scan_rsp_data_len;
		system_data_len = &__adv->scan_rsp_system_data_len;
		break;

	default:
		BT_ERR("Unknown LE packet type : %d",
					pkt_type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	ret = __bt_remove_ad_data_by_type(*p, *len, data_type, &new_p, &new_len);
	if (ret != BT_ERROR_NONE)
		return ret;

	free(*p);
	*p = new_p;
	*len = new_len;

	if (data_type == BT_ADAPTER_LE_ADVERTISING_DATA_LOCAL_NAME ||
		data_type == BT_ADAPTER_LE_ADVERTISING_DATA_TX_POWER_LEVEL)
		*system_data_len -= 1;

	return ret;
}
/* LCOV_EXCL_STOP */

static int __bt_convert_string_to_uuid(const char *string,
					char **uuid, int *bit)
{
	char *data;

	if (strlen(string) == 4)  { /* 16 bit uuid */
		unsigned short val;
		data = calloc(1, sizeof(char) * 2);

		val = (unsigned short)strtoul(string, NULL, 16);
		val = htons(val);
		memcpy(data, &val, 2);
		*uuid = data;
		*bit = 16; /* LCOV_EXCL_START */
	} else if (strlen(string) == 36)  { /* 128 bit uuid */
		if (string[8] != '-' || string[13] != '-' ||
			string[18] != '-' || string[23] != '-') {
			BT_ERR("Invalid UUID");
			return BT_ERROR_INVALID_PARAMETER;
		}

		char str_ptr[37] = { 0 };
		char *ptr[7];
		char *next_ptr;
		int idx = 0;

		unsigned int val0, val4;
		unsigned short val1, val2, val3, val5;
		data = calloc(1, sizeof(char) * 16);
		if (data == NULL)
			return BT_ERROR_OUT_OF_MEMORY;

		/* UUID format : %08x-%04hx-%04hx-%04hx-%08x%04hx */
		strncpy(str_ptr, string, 36);

		ptr[idx++] = strtok_r(str_ptr, "-", &next_ptr);
		while ((ptr[idx++] = strtok_r(NULL, "-", &next_ptr)) != NULL) {
			BT_INFO("ptr : %s", ptr[idx - 1]);
			if (idx == 5)
				break;
		}

		/* ptr[4] contain "08x" and "04hx" */
		ptr[5] = calloc(1, sizeof(char) * 8);
		if (ptr[5] == NULL) {
			free(data);
			return BT_ERROR_OUT_OF_MEMORY;
		}

		ptr[6] = calloc(1, sizeof(char) * 4);
		if (ptr[6] == NULL) {
			free(data);
			free(ptr[5]);
			return BT_ERROR_OUT_OF_MEMORY;
		}

		strncpy(ptr[5], ptr[4], 8);
		strncpy(ptr[6], ptr[4] + 8, 4);

		val0 = (unsigned int)strtoul(ptr[0], NULL, 16);
		val1 = (unsigned short)strtoul(ptr[1], NULL, 16);
		val2 = (unsigned short)strtoul(ptr[2], NULL, 16);
		val3 = (unsigned short)strtoul(ptr[3], NULL, 16);
		val4 = (unsigned int)strtoul(ptr[5], NULL, 16);
		val5 = (unsigned short)strtoul(ptr[6], NULL, 16);

		val0 = htonl(val0);
		val1 = htons(val1);
		val2 = htons(val2);
		val3 = htons(val3);
		val4 = htonl(val4);
		val5 = htons(val5);

		BT_INFO("%x, %x, %x, %x, %x, %x", val0, val1, val2, val3, val4, val5);

		memcpy(data, &val0, 4);
		memcpy(data + 4, &val1, 2);
		memcpy(data + 6, &val2, 2);
		memcpy(data + 8, &val3, 2);
		memcpy(data + 10, &val4, 4);
		memcpy(data + 14, &val5, 2);

		*uuid = data;
		*bit = 128;

		if (ptr[5])
			free(ptr[5]);
		if (ptr[6])
			free(ptr[6]);
	} else {
		BT_ERR("Invalid UUID");
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

static int __bt_convert_byte_ordering(char *data, int data_len,
					char **converted_data)
{
	char *swp;
	int i, j;

	/* Convert to little endian */
	swp = calloc(1, data_len);
	for (i = 0, j = data_len - 1; i < data_len; i++, j--)
		swp[i] = data[j];

	*converted_data = swp;

	return BT_ERROR_NONE;
}

static int __bt_find_adv_data_type(bt_advertiser_h advertiser,
			bt_adapter_le_packet_type_e pkt_type,
			bt_adapter_le_advertising_data_type_e data_type,
			char **data_ptr, int *data_len)
{
	int type;
	int len;
	int i;
	char *adv_data = NULL;
	int adv_len = 0;
	bt_advertiser_s *__adv = (bt_advertiser_s *)advertiser;

	if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING) {
		adv_data = __adv->adv_data;
		adv_len = __adv->adv_data_len;
	} else if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE) {
		adv_data = __adv->scan_rsp_data;
		adv_len = __adv->scan_rsp_data_len;
	} else
		return BT_ERROR_INVALID_PARAMETER;

	if (!adv_data)
		return BT_ERROR_OPERATION_FAILED;

	for (i = 0; i < adv_len ; i++) { /* LCOV_EXCL_START */
		len = adv_data[i];
		type = adv_data[i + 1];

		if (type == data_type) {
			*data_ptr = &adv_data[i];
			*data_len = len;
			return BT_ERROR_NONE;
		} else {
			i = i + adv_data[i]; /* LCOV_EXCL_STOP */
		}
	}

	return BT_ERROR_NONE;
}

/* LCOV_EXCL_START */
static int __bt_append_adv_type_data(bt_advertiser_h advertiser,
			bt_adapter_le_packet_type_e pkt_type,
			char *new_data, int new_data_len,
			bt_adapter_le_advertising_data_type_e data_type,
			char *data_ptr, int data_len)
{
	int type;
	int len;
	int dest_type;
	int i;
	char *new_adv = NULL;
	char *adv_data = NULL;
	int adv_len = 0;
	int adv_flag_len = 0;
	int system_data_len = 0;
	bt_advertiser_s *__adv = (bt_advertiser_s *)advertiser;

	if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING) {
		adv_data = __adv->adv_data;
		adv_len = __adv->adv_data_len;
		system_data_len = __adv->adv_system_data_len;
		adv_flag_len = 3;
	} else if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE) {
		adv_data = __adv->scan_rsp_data;
		adv_len = __adv->scan_rsp_data_len;
		system_data_len = __adv->scan_rsp_system_data_len;
	} else
		return BT_ERROR_INVALID_PARAMETER;

	if (!adv_data)
		return BT_ERROR_OPERATION_FAILED;

	dest_type = data_ptr[1];

	if (data_type != dest_type) {
		BT_ERR("Invalid data type");
		return BT_ERROR_INVALID_PARAMETER;
	}

	if (adv_flag_len + adv_len + system_data_len + new_data_len > 31) {
		BT_ERR("Quota exceeded");
		return BT_ERROR_QUOTA_EXCEEDED;
	}

	new_adv = calloc(1, adv_len + new_data_len);
	if (new_adv == NULL)
		return BT_ERROR_OUT_OF_MEMORY;

	for (i = 0; i < adv_len; i++) {
		len = adv_data[i];
		type = adv_data[i + 1];

		if (type == dest_type) {
			int rest_data_len;

			memcpy(new_adv + i, &adv_data[i], len + 1);
			memcpy(new_adv + i + len + 1, new_data, new_data_len);
			new_adv[i] += new_data_len;

			rest_data_len = adv_len - (i  + len + 1);
			if (rest_data_len > 0)
				memcpy(new_adv + i + len + 1 + new_data_len,
					&adv_data[i + len + 1], rest_data_len);

			break;
		} else {
			memcpy(new_adv + i, &adv_data[i], len + 1);
			i = i + len;
		}
	}

	if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING) {
		if (__adv->adv_data)
			free(__adv->adv_data);
		__adv->adv_data = new_adv;
		__adv->adv_data_len += new_data_len;
	} else if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE) {
		if (__adv->scan_rsp_data)
			free(__adv->scan_rsp_data);
		__adv->scan_rsp_data = new_adv;
		__adv->scan_rsp_data_len += new_data_len;
	}

	return BT_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

int bt_adapter_le_add_advertising_service_uuid(bt_advertiser_h advertiser,
		bt_adapter_le_packet_type_e pkt_type, const char *uuid)
{
	int ret = BT_ERROR_NONE;
	bt_adapter_le_advertising_data_type_e data_type = 0;
	int byte_len = 0;
	int uuid_bit = 0;
	char *uuid_ptr = NULL;
	char *data_ptr = NULL;
	char *converted_uuid = NULL;
	int data_len = 0;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);
	BT_CHECK_INPUT_PARAMETER(uuid);

	ret = __bt_convert_string_to_uuid(uuid, &uuid_ptr, &uuid_bit);
	if (ret != BT_ERROR_NONE)
		return ret;

	if (uuid_bit == 16) {
		byte_len = 2;
		data_type = BT_ADAPTER_LE_ADVERTISING_DATA_INCOMP_LIST_16_BIT_SERVICE_CLASS_UUIDS;
	} else if (uuid_bit == 128) { /* LCOV_EXCL_LINE */
		byte_len = 16; /* LCOV_EXCL_LINE */
		data_type = BT_ADAPTER_LE_ADVERTISING_DATA_INCOMP_LIST_128_BIT_SERVICE_CLASS_UUIDS; /* LCOV_EXCL_LINE */
	}

	__bt_convert_byte_ordering(uuid_ptr, byte_len, &converted_uuid);
	free(uuid_ptr);

	__bt_find_adv_data_type(advertiser, pkt_type, data_type, &data_ptr, &data_len);
	if (data_ptr) {
		ret = __bt_append_adv_type_data(advertiser, pkt_type,
						converted_uuid, byte_len,
						data_type, data_ptr,
						data_len); /* LCOV_EXCL_LINE */
	} else {
		ret = bt_adapter_le_add_advertising_data(advertiser, pkt_type,
							data_type, converted_uuid,
							byte_len);
	}
	free(converted_uuid);

	return ret;
}

int bt_adapter_le_add_advertising_service_solicitation_uuid(bt_advertiser_h advertiser,
		bt_adapter_le_packet_type_e pkt_type, const char *uuid)
{
	int ret = BT_ERROR_NONE;
	bt_adapter_le_advertising_data_type_e data_type = 0;
	int byte_len = 0;
	int uuid_bit = 0;
	char *uuid_ptr = NULL;
	char *data_ptr = NULL;
	char *converted_uuid = NULL;
	int data_len = 0;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);
	BT_CHECK_INPUT_PARAMETER(uuid);

	ret = __bt_convert_string_to_uuid(uuid, &uuid_ptr, &uuid_bit);
	if (ret != BT_ERROR_NONE)
		return ret;

	if (uuid_bit == 16) {
		byte_len = 2;
		data_type = BT_ADAPTER_LE_ADVERTISING_DATA_16_BIT_SERVICE_SOLICITATION_UUIDS;
	} else if (uuid_bit == 128) { /* LCOV_EXCL_LINE */
		byte_len = 16; /* LCOV_EXCL_LINE */
		data_type = BT_ADAPTER_LE_ADVERTISING_DATA_128_BIT_SERVICE_SOLICITATION_UUIDS; /* LCOV_EXCL_LINE */
	}

	__bt_convert_byte_ordering(uuid_ptr, byte_len, &converted_uuid);
	free(uuid_ptr);

	__bt_find_adv_data_type(advertiser, pkt_type, data_type, &data_ptr, &data_len);
	if (data_ptr) {
		ret = __bt_append_adv_type_data(advertiser, pkt_type,
						converted_uuid, byte_len,
						data_type, data_ptr,
						data_len); /* LCOV_EXCL_LINE */
	} else {
		ret = bt_adapter_le_add_advertising_data(advertiser, pkt_type,
							data_type, converted_uuid,
							byte_len);
	}
	free(converted_uuid);

	return ret;
}

int bt_adapter_le_add_advertising_service_data(bt_advertiser_h advertiser,
		bt_adapter_le_packet_type_e pkt_type, const char *uuid,
		const char *service_data, int service_data_len)
{
	int ret = BT_ERROR_NONE;
	char *data_ptr = NULL;
	int data_len;
	char *adv_data = NULL;
	int uuid_bit;
	char *uuid_ptr;
	int byte_len = 0;
	char *converted_uuid = NULL;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);
	BT_CHECK_INPUT_PARAMETER(service_data);

	__bt_find_adv_data_type(advertiser, pkt_type,
				BT_ADAPTER_LE_ADVERTISING_DATA_SERVICE_DATA,
				&data_ptr, &data_len);
	if (data_ptr) {
		BT_ERR("Aleady existed"); /* LCOV_EXCL_LINE */
		return BT_ERROR_ALREADY_DONE; /* LCOV_EXCL_LINE */
	}

	ret = __bt_convert_string_to_uuid(uuid, &uuid_ptr, &uuid_bit);
	if (ret != BT_ERROR_NONE)
		return ret;

	if (uuid_bit == 16) {
		byte_len = 2;
	} else if (uuid_bit == 128) { /* LCOV_EXCL_LINE */
		BT_ERR("128 bit UUID is not supported"); /* LCOV_EXCL_LINE */
		free(uuid_ptr); /* LCOV_EXCL_LINE */
		return BT_ERROR_INVALID_PARAMETER; /* LCOV_EXCL_LINE */
	}

	__bt_convert_byte_ordering(uuid_ptr, byte_len, &converted_uuid);
	free(uuid_ptr);

	adv_data = calloc(1, sizeof(char) *(service_data_len + 2));

	memcpy(adv_data, converted_uuid, 2);
	memcpy(adv_data + 2, service_data, service_data_len);

	ret = bt_adapter_le_add_advertising_data(advertiser,
		pkt_type, BT_ADAPTER_LE_ADVERTISING_DATA_SERVICE_DATA,
		adv_data, service_data_len + 2);

	free(adv_data);
	free(converted_uuid);

	return ret;
}

int bt_adapter_le_set_advertising_appearance(bt_advertiser_h advertiser,
		bt_adapter_le_packet_type_e pkt_type, int appearance)
{
	int ret = BT_ERROR_NONE;
	char *data_ptr = NULL;
	int data_len;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	__bt_find_adv_data_type(advertiser, pkt_type, BT_ADAPTER_LE_ADVERTISING_DATA_APPEARANCE,
				&data_ptr, &data_len);
	if (data_ptr) {
		BT_ERR("Aleady existed"); /* LCOV_EXCL_LINE */
		return BT_ERROR_ALREADY_DONE; /* LCOV_EXCL_LINE */
	}

	ret = bt_adapter_le_add_advertising_data(advertiser,
		pkt_type, BT_ADAPTER_LE_ADVERTISING_DATA_APPEARANCE, &appearance, 2);

	return ret;
}

int bt_adapter_le_add_advertising_manufacturer_data(bt_advertiser_h advertiser,
		bt_adapter_le_packet_type_e pkt_type, int manufacturer_id,
		const char *manufacturer_data, int manufacturer_data_len)
{
	int ret = BT_ERROR_NONE;
	char *data_ptr = NULL;
	int data_len;
	char *adv_data = NULL;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);
	BT_CHECK_INPUT_PARAMETER(manufacturer_data);

	__bt_find_adv_data_type(advertiser, pkt_type,
				BT_ADAPTER_LE_ADVERTISING_DATA_MANUFACTURER_SPECIFIC_DATA,
				&data_ptr, &data_len);
	if (data_ptr) {
		BT_ERR("Aleady existed"); /* LCOV_EXCL_LINE */
		return BT_ERROR_ALREADY_DONE; /* LCOV_EXCL_LINE */
	}

	adv_data = calloc(1, sizeof(char) *(manufacturer_data_len + 2));
	if (adv_data == NULL)
		return BT_ERROR_OUT_OF_MEMORY;

	adv_data[0] = manufacturer_id & 0xffffffff;
	adv_data[1] = (manufacturer_id & 0xff00) >> 8;
	memcpy(adv_data + 2, manufacturer_data, manufacturer_data_len);

	ret = bt_adapter_le_add_advertising_data(advertiser,
		pkt_type, BT_ADAPTER_LE_ADVERTISING_DATA_MANUFACTURER_SPECIFIC_DATA,
		adv_data, manufacturer_data_len + 2);

	free(adv_data);

	return ret;
}

int bt_adapter_le_set_advertising_device_name(bt_advertiser_h advertiser,
		bt_adapter_le_packet_type_e pkt_type, bool include_name)
{
	int ret = BT_ERROR_NONE;
	char *data_ptr = NULL;
	int data_len;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	if (!include_name) {
		ret = bt_adapter_le_remove_advertising_data(advertiser,
				pkt_type, /* LCOV_EXCL_LINE */
				BT_ADAPTER_LE_ADVERTISING_DATA_LOCAL_NAME);
		return ret;
	}

	__bt_find_adv_data_type(advertiser, pkt_type,
				BT_ADAPTER_LE_ADVERTISING_DATA_LOCAL_NAME,
				&data_ptr, &data_len);
	if (data_ptr) {
		BT_ERR("Aleady existed"); /* LCOV_EXCL_LINE */
		return BT_ERROR_ALREADY_DONE; /* LCOV_EXCL_LINE */
	}

	ret = bt_adapter_le_add_advertising_data(advertiser,
				pkt_type,
				BT_ADAPTER_LE_ADVERTISING_DATA_LOCAL_NAME,
		NULL, 0);

	return ret;
}

int bt_adapter_le_set_advertising_tx_power_level(bt_advertiser_h advertiser,
		bt_adapter_le_packet_type_e pkt_type, bool include_tx_power)
{
	int ret = BT_ERROR_NONE;
	char *data_ptr = NULL;
	int data_len;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	if (!include_tx_power) {
		ret = bt_adapter_le_remove_advertising_data(advertiser,
				pkt_type, /* LCOV_EXCL_LINE */
				BT_ADAPTER_LE_ADVERTISING_DATA_TX_POWER_LEVEL);
		return ret;
	}

	__bt_find_adv_data_type(advertiser, pkt_type,
				BT_ADAPTER_LE_ADVERTISING_DATA_TX_POWER_LEVEL,
				&data_ptr, &data_len);
	if (data_ptr) {
		BT_ERR("Aleady existed"); /* LCOV_EXCL_LINE */
		return BT_ERROR_ALREADY_DONE; /* LCOV_EXCL_LINE */
	}

	ret = bt_adapter_le_add_advertising_data(advertiser,
				pkt_type,
				BT_ADAPTER_LE_ADVERTISING_DATA_TX_POWER_LEVEL,
		NULL, 0);

	return ret;
}

int bt_adapter_le_clear_advertising_data(bt_advertiser_h advertiser,
		bt_adapter_le_packet_type_e pkt_type)
{
	int ret = BT_ERROR_NONE;
	bt_advertiser_s *__adv = (bt_advertiser_s *)advertiser;
	char **p;
	unsigned int *len;
	unsigned int *system_data_len;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	switch (pkt_type) {
	case BT_ADAPTER_LE_PACKET_ADVERTISING:
		p = &__adv->adv_data;
		len = &__adv->adv_data_len;
		system_data_len = &__adv->adv_system_data_len;
		break;

	case BT_ADAPTER_LE_PACKET_SCAN_RESPONSE:
		p = &__adv->scan_rsp_data;
		len = &__adv->scan_rsp_data_len;
		system_data_len = &__adv->scan_rsp_system_data_len;
		break;

	default:
		BT_ERR("Unknown LE packet type : %d",
					pkt_type); /* LCOV_EXCL_LINE */
		return BT_ERROR_INVALID_PARAMETER; /* LCOV_EXCL_LINE */
	}

	if (*p) {
		free(*p);
		*p = NULL;
	}
	*len = 0;
	*system_data_len = 0;

	return ret;
}

int bt_adapter_le_stop_advertising(bt_advertiser_h advertiser)
{
	int ret = BT_ERROR_NONE;
	bt_advertiser_s *__adv = (bt_advertiser_s *)advertiser;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	ret = bt_adapt_stop_adv(__adv->handle);
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret),
						ret); /* LCOV_EXCL_LINE */
	}

	return ret;
}

int bt_adapter_le_start_advertising_new(bt_advertiser_h advertiser,
		bt_adapter_le_advertising_state_changed_cb cb, void *user_data)
{
	int ret = BT_ERROR_NONE;
	bt_advertiser_s *__adv = (bt_advertiser_s *)advertiser;
	unsigned int interval = 500;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	if (__adv->adv_data_len > 0 &&
		__adv->adv_data) { /* LCOV_EXCL_START */
		ret = bt_adapt_set_adv_data(__adv->handle,
					(const char *)__adv->adv_data, __adv->adv_data_len);

		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret),
						ret); /* LCOV_EXCL_STOP */
			return ret;
		}
	}

	if (__adv->scan_rsp_data_len > 0 &&
		__adv->scan_rsp_data) { /* LCOV_EXCL_START */
		ret = bt_adapt_set_scan_res_data(__adv->handle,
					(const char *)__adv->scan_rsp_data, __adv->scan_rsp_data_len);

		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret),
						ret); /* LCOV_EXCL_STOP */
			return ret;
		}
	}

	if (__adv->adv_params.mode == BT_ADAPTER_LE_ADVERTISING_MODE_BALANCED)
		interval = 500;
	else if (__adv->adv_params.mode == BT_ADAPTER_LE_ADVERTISING_MODE_LOW_LATENCY)
		interval = 150;
	else if (__adv->adv_params.mode == BT_ADAPTER_LE_ADVERTISING_MODE_LOW_ENERGY)
		interval = 1000;

	ret = bt_adapt_start_adv(__adv->handle, interval, interval,
							__adv->adv_params.filter_policy,
							__adv->adv_params.type);

	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret),
						ret); /* LCOV_EXCL_LINE */
		return ret;
	}

	__adv->cb = cb;
	__adv->user_data = user_data;

	return ret;
}

void _bt_adapter_le_invoke_advertising_state_cb(int handle, int result,
				bt_adapter_le_advertising_state_e adv_state)
{
#ifdef GLIB_SUPPORTED
	const GSList *l = NULL;

	for (l = advertiser_list; l; l = g_slist_next(l)) {
		bt_advertiser_s *__adv = (bt_advertiser_s *)l->data;
		if (__adv->handle == handle) {
			if (__adv->cb == NULL) {
				BT_ERR("advertiser cb is NULL"); /* LCOV_EXCL_LINE */
				return; /* LCOV_EXCL_LINE */
			}

			__adv->cb(result, (bt_advertiser_h)__adv,
					adv_state, __adv->user_data);
			return;
		}
	}
#else
	bt_advertiser_s *__adv = (bt_advertiser_s *)sq_peek(&advertiser_list);
	while (__adv) {
		if (__adv->handle == handle) {
			if (__adv->cb == NULL) {
				BT_ERR("advertiser cb is NULL"); /* LCOV_EXCL_LINE */
				return; /* LCOV_EXCL_LINE */
			}

			__adv->cb(result, (bt_advertiser_h)__adv,
					adv_state, __adv->user_data);
			return;
		}
		__adv = (bt_advertiser_s *)sq_next(__adv);
	}
#endif

	BT_ERR("No available advertiser"); /* LCOV_EXCL_LINE */
}

int bt_adapter_le_set_advertising_mode(bt_advertiser_h advertiser,
		bt_adapter_le_advertising_mode_e mode)
{
	int error_code = BT_ERROR_NONE;
	bt_advertiser_s *__adv = (bt_advertiser_s *)advertiser;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	if (mode < BT_ADAPTER_LE_ADVERTISING_MODE_BALANCED ||
		mode > BT_ADAPTER_LE_ADVERTISING_MODE_LOW_ENERGY)
		return BT_ERROR_INVALID_PARAMETER;

	/* TODO : Before set the mode, check the inprogress status */
	__adv->adv_params.mode = mode;

	return error_code;
}

/* LCOV_EXCL_START */
int bt_adapter_le_set_advertising_filter_policy(bt_advertiser_h advertiser,
		bt_adapter_le_advertising_filter_policy_e filter_policy)
{
	int error_code = BT_ERROR_NONE;
	bt_advertiser_s *__adv = (bt_advertiser_s *)advertiser;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	if (filter_policy < BT_ADAPTER_LE_ADVERTISING_FILTER_DEFAULT ||
		filter_policy > BT_ADAPTER_LE_ADVERTISING_FILTER_ALLOW_SCAN_CONN_WL)
		return BT_ERROR_INVALID_PARAMETER;

	/* TODO : Before set the filter policy, check the inprogress status */
	__adv->adv_params.filter_policy = filter_policy;

	return error_code;
}
/* LCOV_EXCL_STOP */

int bt_adapter_le_set_advertising_connectable(bt_advertiser_h advertiser,
							bool connectable)
{
	int error_code = BT_ERROR_NONE;
	bt_advertiser_s *__adv = (bt_advertiser_s *)advertiser;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(advertiser);

	if (connectable)
		__adv->adv_params.type = BT_ADAPTER_LE_ADVERTISING_CONNECTABLE; /* LCOV_EXCL_LINE */
	else
		__adv->adv_params.type = BT_ADAPTER_LE_ADVERTISING_SCANNABLE;

	/* TODO : Before set the type, check the inprogress status */

	return error_code;
}

int bt_adapter_le_enable_privacy(bool enable_privacy)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_set_static_random_address(bool enable)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_start_scan(bt_adapter_le_scan_result_cb cb, void *user_data)
{
	int error_code = BT_ERROR_NONE;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(cb);

	_bt_set_cb(BT_EVENT_LE_SCAN_RESULT_UPDATED, cb, user_data);

	error_code = bt_adapt_start_le_scan();
	if (error_code != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(error_code),
					error_code); /* LCOV_EXCL_LINE */
	}
	return error_code;
}

int bt_adapter_le_stop_scan(void)
{
	int error_code = BT_ERROR_NONE;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();

	error_code = bt_adapt_stop_le_scan();
	if (error_code != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(error_code),
					error_code); /* LCOV_EXCL_LINE */
	}

	_bt_unset_cb(BT_EVENT_LE_SCAN_RESULT_UPDATED);

	return error_code;
}

int bt_adapter_le_get_scan_result_service_uuids(const bt_adapter_le_device_scan_result_info_s *info,
			bt_adapter_le_packet_type_e pkt_type,
			char ***uuids, int *count)
{
	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(info);
	BT_CHECK_INPUT_PARAMETER(uuids); /* LCOV_EXCL_START */
	BT_CHECK_INPUT_PARAMETER(count);

	int adv_length = 0;
	char *adv_data = NULL;
	char *remain_data = NULL;
	int remain_len = 0;
	int field_len = 0;
	int uuid_size = 0;
	int uuid_count = 0;
	int uuid_index = 0;
	int i;

	if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING) {
		adv_data = info->adv_data;
		adv_length = info->adv_data_len;
	} else if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE) {
		adv_data = info->scan_data;
		adv_length = info->scan_data_len;
	} else
		return BT_ERROR_INVALID_PARAMETER;

	if (!adv_data || adv_length < 3)
		return BT_ERROR_NO_DATA;

	remain_data = adv_data;
	remain_len = adv_length;
	field_len = 0;
	while (remain_len > 0) {
		field_len = remain_data[0];
		if (remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_INCOMP_LIST_16_BIT_SERVICE_CLASS_UUIDS ||
			remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_COMP_LIST_16_BIT_SERVICE_CLASS_UUIDS)
			uuid_count += (field_len - 1) / 2;
		else if (remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_INCOMP_LIST_128_BIT_SERVICE_CLASS_UUIDS ||
			remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_COMP_LIST_128_BIT_SERVICE_CLASS_UUIDS)
			uuid_count += (field_len - 1) / 16;

		remain_len = remain_len - field_len - 1;
		remain_data += field_len + 1;
	}

	if (uuid_count == 0)
		return BT_ERROR_NO_DATA;

	*uuids = calloc(1, sizeof(char *) *uuid_count);

	*count = uuid_count;

	remain_data = adv_data;
	remain_len = adv_length;
	field_len = 0;
	while (remain_len) {
		field_len = remain_data[0];
		if (remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_INCOMP_LIST_16_BIT_SERVICE_CLASS_UUIDS ||
			remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_COMP_LIST_16_BIT_SERVICE_CLASS_UUIDS)
			uuid_size = 2;
		else if (remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_INCOMP_LIST_128_BIT_SERVICE_CLASS_UUIDS ||
			remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_COMP_LIST_128_BIT_SERVICE_CLASS_UUIDS)
			uuid_size = 16;
		else
			uuid_size = 0;

		if (uuid_size != 0) {
			for (i = 0; i < (field_len - 1); i += uuid_size) {
				if (uuid_size == 2) {
					(*uuids)[uuid_index] = calloc(1, sizeof(char) * 4 + 1);
					snprintf((*uuids)[uuid_index], 5,
							"%2.2X%2.2X",
							remain_data[i+3],
							remain_data[i+2]);
				} else {
					(*uuids)[uuid_index] = calloc(1, sizeof(char) * 36 + 1);
					snprintf((*uuids)[uuid_index], 37, "%2.2X%2.2X%2.2X%2.2X-%2.2X%2.2X-%2.2X%2.2X-%2.2X%2.2X-%2.2X%2.2X%2.2X%2.2X%2.2X%2.2X",
						remain_data[i+17], remain_data[i+16], remain_data[i+15], remain_data[i+14],
						remain_data[i+13], remain_data[i+12], remain_data[i+11], remain_data[i+10], remain_data[i+9], remain_data[i+8],
						remain_data[i+7], remain_data[i+6], remain_data[i+5], remain_data[i+4], remain_data[i+3], remain_data[i+2]);
				}
				uuid_index++;
			}
		}

		remain_len = remain_len - field_len - 1;
		remain_data += field_len + 1;
	}

	return BT_ERROR_NONE;
} /* LCOV_EXCL_STOP */

int bt_adapter_le_get_scan_result_device_name(const bt_adapter_le_device_scan_result_info_s *info,
			bt_adapter_le_packet_type_e pkt_type, char **name)
{
	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(info); /* LCOV_EXCL_START */
	BT_CHECK_INPUT_PARAMETER(name);

	int adv_length = 0;
	char *adv_data = NULL;
	int field_len = 0;

	if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING) {
		adv_data = info->adv_data;
		adv_length = info->adv_data_len;
	} else if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE) {
		adv_data = info->scan_data;
		adv_length = info->scan_data_len;
	} else
		return BT_ERROR_INVALID_PARAMETER;

	if (!adv_data || adv_length < 3)
		return BT_ERROR_NO_DATA;

	while (adv_length > 0) {
		field_len = adv_data[0];
		if (adv_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_LOCAL_NAME ||
			adv_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_SHORT_LOCAL_NAME) {
			*name = calloc(1, sizeof(char) *field_len);
			memcpy(*name, &adv_data[2], field_len - 1);

			return BT_ERROR_NONE;
		}

		adv_length = adv_length - field_len - 1;
		adv_data += field_len + 1;
	}

	return BT_ERROR_NO_DATA; /* LCOV_EXCL_STOP */
}

int bt_adapter_le_get_scan_result_tx_power_level(const bt_adapter_le_device_scan_result_info_s *info,
			bt_adapter_le_packet_type_e pkt_type, int *power_level)
{
	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(info); /* LCOV_EXCL_START */
	BT_CHECK_INPUT_PARAMETER(power_level);

	int adv_length = 0;
	char *adv_data = NULL;
	int field_len = 0;

	if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING) {
		adv_data = info->adv_data;
		adv_length = info->adv_data_len;
	} else if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE) {
		adv_data = info->scan_data;
		adv_length = info->scan_data_len;
	} else
		return BT_ERROR_INVALID_PARAMETER;

	if (!adv_data || adv_length < 3)
		return BT_ERROR_NO_DATA;

	while (adv_length > 0) {
		field_len = adv_data[0];
		if (adv_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_TX_POWER_LEVEL) {
			if (adv_data[2] & 0x80)
				*power_level = 0xffffff00 | adv_data[2];
			else
				*power_level = (int)adv_data[2];

			return BT_ERROR_NONE;
		}

		adv_length = adv_length - field_len - 1;
		adv_data += field_len + 1;
	}

	return BT_ERROR_NO_DATA; /* LCOV_EXCL_STOP */
}

int bt_adapter_le_get_scan_result_service_solicitation_uuids(const bt_adapter_le_device_scan_result_info_s *info,
			bt_adapter_le_packet_type_e pkt_type,
			char ***uuids, int *count)
{
	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(info); /* LCOV_EXCL_START */
	BT_CHECK_INPUT_PARAMETER(uuids);
	BT_CHECK_INPUT_PARAMETER(count);

	int adv_length = 0;
	char *adv_data = NULL;
	char *remain_data = NULL;
	int remain_len = 0;
	int field_len = 0;
	int uuid_size = 0;
	int uuid_count = 0;
	int uuid_index = 0;
	int i;

	if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING) {
		adv_data = info->adv_data;
		adv_length = info->adv_data_len;
	} else if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE) {
		adv_data = info->scan_data;
		adv_length = info->scan_data_len;
	} else
		return BT_ERROR_INVALID_PARAMETER;

	if (!adv_data || adv_length < 3)
		return BT_ERROR_NO_DATA;

	remain_data = adv_data;
	remain_len = adv_length;
	field_len = 0;
	while (remain_len > 0) {
		field_len = remain_data[0];
		if (remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_16_BIT_SERVICE_SOLICITATION_UUIDS)
			uuid_count += (field_len - 1) / 2;
		else if (remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_128_BIT_SERVICE_SOLICITATION_UUIDS)
			uuid_count += (field_len - 1) / 16;

		remain_len = remain_len - field_len - 1;
		remain_data += field_len + 1;
	}

	if (uuid_count == 0)
		return BT_ERROR_NO_DATA;

	*uuids = calloc(1, sizeof(char *) * uuid_count);
	*count = uuid_count;

	remain_data = adv_data;
	remain_len = adv_length;
	field_len = 0;
	while (remain_len > 0) {
		field_len = remain_data[0];
		if (remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_16_BIT_SERVICE_SOLICITATION_UUIDS)
			uuid_size = 2;
		else if (remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_128_BIT_SERVICE_SOLICITATION_UUIDS)
			uuid_size = 16;
		else
			uuid_size = 0;

		if (uuid_size != 0) {
			for (i = 0; i < (field_len - 1); i += uuid_size) {
				if (uuid_size == 2) {
					(*uuids)[uuid_index] = calloc(1, sizeof(char) * 4 + 1);
					snprintf((*uuids)[uuid_index], 5,
						"%2.2X%2.2X", remain_data[i+3],
						remain_data[i+2]);
				} else {
					(*uuids)[uuid_index] = calloc(1, sizeof(char) * 36 + 1);
					snprintf((*uuids)[uuid_index], 37, "%2.2X%2.2X%2.2X%2.2X-%2.2X%2.2X-%2.2X%2.2X-%2.2X%2.2X-%2.2X%2.2X%2.2X%2.2X%2.2X%2.2X",
						remain_data[i+17], remain_data[i+16], remain_data[i+15], remain_data[i+14],
						remain_data[i+13], remain_data[i+12], remain_data[i+11], remain_data[i+10], remain_data[i+9], remain_data[i+8],
						remain_data[i+7], remain_data[i+6], remain_data[i+5], remain_data[i+4], remain_data[i+3], remain_data[i+2]);
				}
				uuid_index++;
			}
		}

		remain_len = remain_len - field_len - 1;
		remain_data += field_len + 1;
	}

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

int bt_adapter_le_get_scan_result_service_data_list(const bt_adapter_le_device_scan_result_info_s *info,
			bt_adapter_le_packet_type_e pkt_type,
			bt_adapter_le_service_data_s **data_list, int *count)
{
	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(info); /* LCOV_EXCL_START */
	BT_CHECK_INPUT_PARAMETER(count);

	int adv_length = 0;
	char *adv_data = NULL;
	char *remain_data = NULL;
	int remain_len = 0;
	int field_len = 0;
	int data_count = 0;
	int data_index = 0;

	if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING) {
		adv_data = info->adv_data;
		adv_length = info->adv_data_len;
	} else if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE) {
		adv_data = info->scan_data;
		adv_length = info->scan_data_len;
	} else
		return BT_ERROR_INVALID_PARAMETER;

	if (!adv_data || adv_length < 3)
		return BT_ERROR_NO_DATA;

	remain_data = adv_data;
	remain_len = adv_length;
	field_len = 0;
	while (remain_len > 0) {
		field_len = remain_data[0];
		if (remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_SERVICE_DATA)
			data_count++;

		remain_len = remain_len - field_len - 1;
		remain_data += field_len + 1;
	}

	if (data_count == 0)
		return BT_ERROR_NO_DATA;

	*data_list = calloc(1, sizeof(bt_adapter_le_service_data_s) * data_count);
	if (*data_list == NULL)
		return BT_ERROR_OUT_OF_MEMORY;

	*count = data_count;

	remain_data = adv_data;
	remain_len = adv_length;
	field_len = 0;
	while (remain_len > 0) {
		field_len = remain_data[0];
		if (remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_SERVICE_DATA) {
			(*data_list)[data_index].service_uuid = calloc(1, sizeof(char) *4 + 1);
			if ((*data_list)[data_index].service_uuid == NULL)
				return BT_ERROR_OUT_OF_MEMORY;

			snprintf((*data_list)[data_index].service_uuid, 5,
				"%2.2X%2.2X", remain_data[3], remain_data[2]);

#ifdef GLIB_SUPPORTED
			(*data_list)[data_index].service_data = g_memdup(
							&remain_data[4], field_len - 3);
#else
			(*data_list)[data_index].service_data = calloc(1, field_len - 3);
			if ((*data_list)[data_index].service_data == NULL)
				return BT_ERROR_OUT_OF_MEMORY;

			memcpy((*data_list)[data_index].service_data, &remain_data[4], field_len - 3);
#endif
			(*data_list)[data_index].service_data_len = field_len - 3;

			data_index++;
		}

		remain_len = remain_len - field_len - 1;
		remain_data += field_len + 1;
	}

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

int bt_adapter_le_free_service_data_list(bt_adapter_le_service_data_s *data_list,
					int count)
{
	int i;

	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(data_list); /* LCOV_EXCL_START */

	for (i = 0; i < count; i++) {
		free(data_list[i].service_uuid);
		free(data_list[i].service_data);
	}
	free(data_list);

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

int bt_adapter_le_get_scan_result_appearance(const bt_adapter_le_device_scan_result_info_s *info,
			bt_adapter_le_packet_type_e pkt_type, int *appearance)
{
	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(info); /* LCOV_EXCL_START */
	BT_CHECK_INPUT_PARAMETER(appearance);

	int adv_length = 0;
	char *adv_data = NULL;
	int field_len = 0;

	if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING) {
		adv_data = info->adv_data;
		adv_length = info->adv_data_len;
	} else if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE) {
		adv_data = info->scan_data;
		adv_length = info->scan_data_len;
	} else
		return BT_ERROR_INVALID_PARAMETER;

	if (!adv_data || adv_length < 3)
		return BT_ERROR_NO_DATA;

	while (adv_length > 0) {
		field_len = adv_data[0];
		if (adv_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_APPEARANCE) {
			*appearance = adv_data[3] << 8;
			*appearance += adv_data[2];


			return BT_ERROR_NONE;
		}

		adv_length = adv_length - field_len - 1;
		adv_data += field_len + 1;
	}

	return BT_ERROR_NO_DATA; /* LCOV_EXCL_STOP */
}


int bt_adapter_le_get_scan_result_manufacturer_data(const bt_adapter_le_device_scan_result_info_s *info,
			bt_adapter_le_packet_type_e pkt_type,
			int *manufacturer_id, char **manufacturer_data,
			int *manufacturer_data_len)
{
	BT_CHECK_LE_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(info);
	BT_CHECK_INPUT_PARAMETER(manufacturer_id); /* LCOV_EXCL_START */
	BT_CHECK_INPUT_PARAMETER(manufacturer_data);
	BT_CHECK_INPUT_PARAMETER(manufacturer_data_len);

	int adv_length = 0;
	char *adv_data = NULL;
	char *remain_data = NULL;
	int remain_len = 0;
	int field_len = 0;

	if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING) {
		adv_data = info->adv_data;
		adv_length = info->adv_data_len;
	} else if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE) {
		adv_data = info->scan_data;
		adv_length = info->scan_data_len;
	} else
		return BT_ERROR_INVALID_PARAMETER;

	if (!adv_data || adv_length < 3)
		return BT_ERROR_NO_DATA;

	remain_data = adv_data;
	remain_len = adv_length;
	field_len = 0;
	while (remain_len > 0) {
		field_len = remain_data[0];
		if (remain_data[1] == BT_ADAPTER_LE_ADVERTISING_DATA_MANUFACTURER_SPECIFIC_DATA) {
			*manufacturer_id = remain_data[3] << 8;
			*manufacturer_id += remain_data[2];

#ifdef GLIB_SUPPORTED
			*manufacturer_data = g_memdup(&remain_data[4], field_len - 3);
#else
			*manufacturer_data = calloc(1, field_len - 3);
			memcpy(*manufacturer_data, &remain_data[4], field_len - 3);
#endif
			*manufacturer_data_len = field_len - 3;

			return BT_ERROR_NONE;
		}

		remain_len = remain_len - field_len - 1;
		remain_data += field_len + 1;
	}

	return BT_ERROR_NO_DATA; /* LCOV_EXCL_STOP */
}

int bt_adapter_le_free_ibeacon_report(bt_adapter_le_ibeacon_scan_result_info_s *ibeacon_info)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_get_scan_result_ibeacon_report(const bt_adapter_le_device_scan_result_info_s *info,
			bt_adapter_le_packet_type_e pkt_type, bt_adapter_le_ibeacon_scan_result_info_s **ibeacon_info)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_create(bt_scan_filter_h *scan_filter)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_destroy(bt_scan_filter_h scan_filter)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_set_device_address(bt_scan_filter_h scan_filter,
							const char *address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_set_device_name(bt_scan_filter_h scan_filter, const char *name)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_set_service_uuid(bt_scan_filter_h scan_filter,
							const char *uuid)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_set_service_uuid_with_mask(bt_scan_filter_h scan_filter,
			const char *uuid, const char *mask)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_set_service_solicitation_uuid(bt_scan_filter_h scan_filter,
							const char *uuid)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_set_service_solicitation_uuid_with_mask(bt_scan_filter_h scan_filter,
			const char *uuid, const char *mask)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_set_service_data(bt_scan_filter_h scan_filter,
			const char *uuid, const char *data,
			unsigned int data_len)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_set_service_data_with_mask(bt_scan_filter_h scan_filter,
			const char *uuid, const char *data, unsigned int data_len,
			const char *mask, unsigned int mask_len)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_set_manufacturer_data(bt_scan_filter_h scan_filter,
			int manufacturer_id, const char *data,
			unsigned int data_len)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_set_manufacturer_data_with_mask(bt_scan_filter_h scan_filter,
			int manufacturer_id, const char *data,
			unsigned int data_len, const char *mask,
			unsigned int mask_len)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_register(bt_scan_filter_h scan_filter)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_unregister(bt_scan_filter_h scan_filter)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_unregister_all(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_adapter_le_read_maximum_data_length(
		int *max_tx_octets, int *max_tx_time,
		int *max_rx_octets, int *max_rx_time)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_write_host_suggested_default_data_length(
			const unsigned int def_tx_Octets,
			const unsigned int def_tx_Time)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_read_suggested_default_data_length(
	unsigned int *def_tx_Octets,  unsigned int *def_tx_Time)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_force_hcidump(int timeout)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_set_authentication_req_cb(bt_adapter_authentication_req_cb callback,
							void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_unset_authentication_req_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_set_ibeacon(bt_scan_filter_h scan_filter)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_set_proximity_uuid(bt_scan_filter_h scan_filter)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_scan_filter_unset_proximity_uuid(bt_scan_filter_h scan_filter)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_adapter_le_scan_filter_set_type(bt_scan_filter_h scan_filter,
			bt_adapter_le_scan_filter_type_e mode)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_adapter_passkey_reply(char *passkey, bool authentication_reply)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_passkey_confirmation_reply(bool confirmation_reply)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_adapter_le_is_2m_phy_supported(bool *is_supported)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_adapter_le_is_coded_phy_supported(bool *is_supported)
{
	return BT_ERROR_NOT_SUPPORTED;
}

