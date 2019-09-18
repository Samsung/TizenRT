/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <tinyara/config.h>
#include <tinyara/bluetooth/bluetooth.h>
#include <tinyara/bluetooth/bt_hci.h>
#include <bluetooth/bluetooth.h>
#include "bt-adaptation-adapter.h"
#include "bt-adaptation-device.h"
#include "bluetooth_private.h"

/* Need to include BT stack's header in here */

int bt_adapt_get_state(bt_adapter_state_e *adapter_state)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

static void __bt_enable_cb(int err)
{
	bluetooth_event_param_t param = {0, };

	BT_PRT("Enter\n");

	param.event = BLUETOOTH_EVENT_ENABLED;
	param.result = err; // TODO: need to convert

	__bt_event_proxy(BLUETOOTH_EVENT_ENABLED, &param);
}

int bt_adapt_enable(void)
{
	int ret = BT_ERROR_NONE;

	BT_PRT("Enter\n");

	ret = _bt_get_error_code(bt_enable(__bt_enable_cb));
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)\n", _bt_convert_error_to_string(ret), ret);
	}

	return ret;
}

int bt_adapt_disable(void)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_get_local_address(char **address)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_get_local_name(char **name)
{
	BT_PRT("Enter\n");

	*name = bt_get_name();

	BT_PRT("name: %s\n", *name);

	return BT_ERROR_NONE;
}

int bt_adapt_set_local_name(const char *name)
{
	int ret = BT_ERROR_NONE;

	BT_PRT("Enter\n");

	ret = bt_set_name(name);
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)\n", _bt_convert_error_to_string(ret), ret);
	}

	return ret;
}

int bt_adapt_get_discoverable_mode(bt_adapter_visibility_mode_e *mode)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_set_discoverable_mode(bt_adapter_visibility_mode_e mode)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

static void __discover_start_cb(struct bt_br_discovery_result *results, size_t count)
{
	bluetooth_event_param_t start_param = {0, };
	bluetooth_event_param_t updated_param = {0, };
	bluetooth_device_info_t data_info = {0, };
	bluetooth_event_param_t finished_param = {0, };

	start_param.result = BT_ERROR_NONE;
	__bt_event_proxy(BLUETOOTH_EVENT_DISCOVERY_STARTED, &start_param);

	updated_param.result = BT_ERROR_NONE;
	updated_param.param_data = &data_info;
	__bt_event_proxy(BLUETOOTH_EVENT_REMOTE_DEVICE_NAME_UPDATED, &updated_param);

	finished_param.result = BT_ERROR_NONE;
	__bt_event_proxy(BLUETOOTH_EVENT_DISCOVERY_FINISHED, &finished_param);
}

int bt_adapt_start_discovery(void)
{
	int ret = BT_ERROR_NONE;
	struct bt_br_discovery_param param = {
		.length = 8,
		.limited = false,
	};
	struct bt_br_discovery_result results[5];
	size_t count;

	BT_PRT("Enter\n");

	ret = _bt_get_error_code(bt_br_discovery_start(&param, results, 5, __discover_start_cb));
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)\n", _bt_convert_error_to_string(ret), ret);
	}

	return ret;
}

int bt_adapt_stop_discovery(void)
{
	int ret = BT_ERROR_NONE;

	BT_PRT("Enter\n");

	ret = _bt_get_error_code(bt_br_discovery_stop());
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)\n", _bt_convert_error_to_string(ret), ret);
	}

	return BT_ERROR_NONE;
}

int bt_adapt_is_discovering(bool *is_discovering)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_is_connectable(bool *is_connectable)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_set_connectable(bool is_connectable)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

#ifdef GLIB_SUPPORTED
int bt_adapt_get_bonded_device_list(GPtrArray **dev_list)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}
#else
int bt_adapt_get_bonded_device_list(sq_queue_t *dev_list)
{
	/* To be implemented */
	struct bt_device_info_t *_device_info = NULL;
	_device_info = (struct bt_device_info_t *)malloc(sizeof(struct bt_device_info_t));
	//_device_info->dev_info = dev_info;
	sq_addlast((sq_entry_t *)_device_info, dev_list);

	return BT_ERROR_NONE;
}
#endif

static void __scan_start_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type, struct net_buf_simple *buf)
{
	bluetooth_event_param_t param = {0, };
	bluetooth_le_device_info_t device_info = {0, };

	BT_PRT("Enter\n");

	memcpy(&device_info.device_address, &addr->val, BLUETOOTH_ADDRESS_LENGTH);
	device_info.addr_type = addr->type;
	device_info.rssi = rssi;
	if (adv_type == BT_LE_ADV_IND || adv_type == BT_LE_ADV_NONCONN_IND) {
		device_info.adv_ind_data.data_len = buf->len;
		memcpy(&device_info.adv_ind_data.data.data, buf->data, BLUETOOTH_ADVERTISING_DATA_LENGTH_MAX);
	} else if (adv_type == BT_LE_ADV_SCAN_RSP) {
		device_info.scan_resp_data.data_len = buf->len;
		memcpy(&device_info.scan_resp_data.data.data, buf->data, BLUETOOTH_ADVERTISING_DATA_LENGTH_MAX);
	} else {
		BT_PRT("Not supported adv_type(%d).\n", adv_type);
	}

	param.event = BT_EVENT_LE_SCAN_RESULT_UPDATED;
	param.result = 0;
	param.param_data = &device_info;

	__bt_le_event_proxy(BLUETOOTH_EVENT_REMOTE_LE_DEVICE_FOUND, &param);
}

int bt_adapt_start_le_scan(void)
{
	int ret = BT_ERROR_NONE;
	struct bt_le_scan_param param = {
		.type = BT_HCI_LE_SCAN_PASSIVE,
		.filter_dup = BT_HCI_LE_SCAN_FILTER_DUP_DISABLE,
		.interval = 0x010,
		.window = 0x010,
	};

	BT_PRT("Enter\n");

	ret = _bt_get_error_code(bt_le_scan_start(&param, __scan_start_cb));
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)\n", _bt_convert_error_to_string(ret), ret);
	}

	return ret;
}

int bt_adapt_stop_le_scan(void)
{
	int ret = BT_ERROR_NONE;

	BT_PRT("Enter\n");

	ret = _bt_get_error_code(bt_le_scan_stop());
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)\n", _bt_convert_error_to_string(ret), ret);
	}

	return ret;
}

int bt_adapt_is_le_scanning(bool *is_scanning)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_start_adv(int handle, int interval_min,
				int interval_max, unsigned int filter_policy,
				unsigned int type)
{
	int ret = BT_ERROR_NONE;
	struct bt_le_adv_param param = {0, };
	struct bt_data ad = {0, }; // TODO: use from global value
	size_t ad_len = 0;

	BT_PRT("Enter\n");

	param.id = 0;
	param.interval_min = interval_min;
	param.interval_max = interval_max;
	if (type == true)
		param.options = (BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_NAME);
	else
		param.options = BT_LE_ADV_OPT_NONE;

	ret = _bt_get_error_code(bt_le_adv_start(&param, &ad, ad_len, NULL, 0));
	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)\n", _bt_convert_error_to_string(ret), ret);
	}

	return ret;
}

int bt_adapt_stop_adv(int handle)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_is_adv(bool *is_advertising)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_set_adv_data(int handle, const char *data, int len)
{
	// TODO: update advertinsg data to global value...
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_set_scan_res_data(int handle, const char *data, int len)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

/*
	int type;  < passive 0, active 1 >
	float interval;  < LE scan interval >
	float window;  < LE scan window >
*/
int bt_adapt_set_scan_params(int type, unsigned int interval,
				unsigned int window)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

