/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <debug.h>
#include <tinyara/bluetooth/bluetooth.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/bluetooth_internal.h>
//#include <eventloop/eventloop.h>
#include "bt_unit_test.h"

static bool adapter_enabled = false;

static const char *__bt_get_error_message(bt_error_e err)
{
	const char *err_str = NULL;

	switch (err) {
	case BT_ERROR_NONE:
		err_str = "BT_ERROR_NONE";
		break;
	case BT_ERROR_CANCELLED:
		err_str = "BT_ERROR_CANCELLED";
		break;
	case BT_ERROR_INVALID_PARAMETER:
		err_str = "BT_ERROR_INVALID_PARAMETER";
		break;
	case BT_ERROR_OUT_OF_MEMORY:
		err_str = "BT_ERROR_OUT_OF_MEMORY";
		break;
	case BT_ERROR_RESOURCE_BUSY:
		err_str = "BT_ERROR_RESOURCE_BUSY";
		break;
	case BT_ERROR_TIMED_OUT:
		err_str = "BT_ERROR_TIMED_OUT";
		break;
	case BT_ERROR_NOW_IN_PROGRESS:
		err_str = "BT_ERROR_NOW_IN_PROGRESS";
		break;
	case BT_ERROR_NOT_INITIALIZED:
		err_str = "BT_ERROR_NOT_INITIALIZED";
		break;
	case BT_ERROR_NOT_ENABLED:
		err_str = "BT_ERROR_NOT_ENABLED";
		break;
	case BT_ERROR_ALREADY_DONE:
		err_str = "BT_ERROR_ALREADY_DONE";
		break;
	case BT_ERROR_OPERATION_FAILED:
		err_str = "BT_ERROR_OPERATION_FAILED";
		break;
	case BT_ERROR_NOT_IN_PROGRESS:
		err_str = "BT_ERROR_NOT_IN_PROGRESS";
		break;
	case BT_ERROR_REMOTE_DEVICE_NOT_BONDED:
		err_str = "BT_ERROR_REMOTE_DEVICE_NOT_BONDED";
		break;
	case BT_ERROR_AUTH_REJECTED:
		err_str = "BT_ERROR_AUTH_REJECTED";
		break;
	case BT_ERROR_AUTH_FAILED:
		err_str = "BT_ERROR_AUTH_FAILED";
		break;
	case BT_ERROR_REMOTE_DEVICE_NOT_FOUND:
		err_str = "BT_ERROR_REMOTE_DEVICE_NOT_FOUND";
		break;
	case BT_ERROR_SERVICE_SEARCH_FAILED:
		err_str = "BT_ERROR_SERVICE_SEARCH_FAILED";
		break;
	case BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED:
		err_str = "BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED";
		break;
	case BT_ERROR_PERMISSION_DENIED:
		err_str = "BT_ERROR_PERMISSION_DENIED";
		break;
	case BT_ERROR_SERVICE_NOT_FOUND:
		err_str = "BT_ERROR_SERVICE_NOT_FOUND";
		break;
	case BT_ERROR_NO_DATA:
		err_str = "BT_ERROR_NO_DATA";
		break;
	case BT_ERROR_NOT_SUPPORTED:
		err_str = "BT_ERROR_NOT_SUPPORTED";
		break;
	case BT_ERROR_DEVICE_POLICY_RESTRICTION:
		err_str = "DEVICE_POLICY_RESTRICTION";
		break;
	default:
		err_str = "NOT Defined";
		break;
	}

	return err_str;
}

static void __discovery_state_changed_cb(int result, bt_adapter_device_discovery_state_e discovery_state, bt_adapter_device_discovery_info_s *discovery_info, void *user_data)
{
	int i;

	TC_PRT("discovery_state: %d", discovery_state);

	if (discovery_info == NULL) {
		TC_PRT("No discovery_info!");
		return;
	}

	TC_PRT("remote_address: %s", discovery_info->remote_address);
	TC_PRT("remote_name: %s", discovery_info->remote_name);
	TC_PRT("rssi: %d", discovery_info->rssi);
	TC_PRT("is_bonded: %d", discovery_info->is_bonded);
	TC_PRT("service_count: %d", discovery_info->service_count);

	if (discovery_info->service_uuid == NULL) {
		TC_PRT("No uuids");
	} else {
		for (i = 0; i < discovery_info->service_count; i++)
			TC_PRT("uuid: %s", discovery_info->service_uuid[i]);
	}

	if (discovery_info->manufacturer_data_len > 0) {
		TC_PRT("manufacturer specific data(len:%d)",
			discovery_info->manufacturer_data_len);
		printf("data [");
		for (i = 0; i < discovery_info->manufacturer_data_len; i++)
			printf("%02x ", discovery_info->manufacturer_data[i]);
		printf("]\n");
	}
}

static void __le_scan_result_cb(int result, bt_adapter_le_device_scan_result_info_s *info, void *user_data)
{
	int i;
	bt_adapter_le_packet_type_e pkt_type = BT_ADAPTER_LE_PACKET_ADVERTISING;

	if (info == NULL) {
		TC_PRT("No discovery_info!");
		return;
	}

	TC_PRT(" ");
	TC_PRT("%s Adv %d Scan resp %d RSSI %d Addr_type %d",
			info->remote_address, info->adv_data_len,
			info->scan_data_len, info->rssi,
			info->address_type);

	if (info->adv_data_len > 31 || info->scan_data_len > 31) {
		TC_PRT("###################");
		bt_adapter_le_stop_scan();
		TC_PRT("###################");
		return;
	}

	for (i = 0; i < 2; i++) {
		char **uuids;
		char *device_name;
		int tx_power_level;
		bt_adapter_le_service_data_s *data_list;
		int appearance;
		int manufacturer_id;
		char *manufacturer_data;
		int manufacturer_data_len;
		int count;
		bt_adapter_le_ibeacon_scan_result_info_s *ibeacon_info = NULL;

		pkt_type += i;
		if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING
			&& info->adv_data == NULL) continue;
		if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE
			&& info->scan_data == NULL) break;

		if (bt_adapter_le_get_scan_result_service_uuids(
			info, pkt_type, &uuids, &count) == BT_ERROR_NONE) {
			int i;
			for (i = 0; i < count; i++) {
				TC_PRT("UUID[%d] = %s", i + 1, uuids[i]);
				free(uuids[i]);
			}
			free(uuids);
		}
		if (bt_adapter_le_get_scan_result_device_name(
			info, pkt_type, &device_name) == BT_ERROR_NONE) {
			TC_PRT("Device name = %s", device_name);
			free(device_name);
		}
		if (bt_adapter_le_get_scan_result_tx_power_level(
			info, pkt_type, &tx_power_level) == BT_ERROR_NONE) {
			TC_PRT("TX Power level = %d", tx_power_level);
		}
		if (bt_adapter_le_get_scan_result_service_solicitation_uuids(
			info, pkt_type, &uuids, &count) == BT_ERROR_NONE) {
			int i;
			for (i = 0; i < count; i++) {
				TC_PRT("Solicitation UUID[%d] = %s",
					i + 1, uuids[i]);
				free(uuids[i]);
			}
			free(uuids);
		}
		if (bt_adapter_le_get_scan_result_service_data_list(
			info, pkt_type, &data_list, &count) == BT_ERROR_NONE) {
			int i;
			for (i = 0; i < count; i++)
				TC_PRT("Service Data[%d] = [0x%2.2X%2.2X:0x%.2X...]",
					i + 1, data_list[i].service_uuid[0],
					data_list[i].service_uuid[1],
					data_list[i].service_data[0]);
			bt_adapter_le_free_service_data_list(data_list, count);
		}
		if (bt_adapter_le_get_scan_result_appearance(
			info, pkt_type, &appearance) == BT_ERROR_NONE) {
			TC_PRT("Appearance = %d", appearance);
		}
		if (bt_adapter_le_get_scan_result_manufacturer_data(
			info, pkt_type, &manufacturer_id, &manufacturer_data,
			&manufacturer_data_len) == BT_ERROR_NONE) {

			if (manufacturer_data_len > 1) {
				TC_PRT("Manufacturer data[ID:%.4X, 0x%.2X%.2X...(len:%d)]",
					manufacturer_id, manufacturer_data[0],
					manufacturer_data[1], manufacturer_data_len);
			} else {
				TC_PRT("Manufacturer data[ID:%.4X, len:%d]", manufacturer_id, manufacturer_data_len);
			}

			free(manufacturer_data);
		}
		if (bt_adapter_le_get_scan_result_ibeacon_report(info, pkt_type,
								&ibeacon_info) == BT_ERROR_NONE) {
				TC_PRT("APPLE IBEACON");
				TC_PRT("Company_id: %d", ibeacon_info->company_id);
				TC_PRT("ibeacon_type: %d", ibeacon_info->ibeacon_type);
				TC_PRT("uuid: %s", ibeacon_info->uuid);
				TC_PRT("major_id: %d", ibeacon_info->major_id);
				TC_PRT("minor_id: %d", ibeacon_info->minor_id);
				TC_PRT("measured_power: %d", ibeacon_info->measured_power);
				bt_adapter_le_free_ibeacon_report(ibeacon_info);
		}
	}
}

static void __state_changed_cb(int result, bt_adapter_state_e adapter_state, void *user_data)
{
	int ret;
	char *name = NULL;

	TC_PRT("__state_changed_cb\n");
	TC_PRT("result: %s\n", __bt_get_error_message(result));
	TC_PRT("state: %s\n", (adapter_state == BT_ADAPTER_ENABLED) ? "ENABLED" : "DISABLED");

//	adapter_enabled = ((adapter_state == BT_ADAPTER_ENABLED) ? true : false);

	if (!(adapter_state == true && result == BT_ERROR_NONE)) {
		TC_PRT("Adapter is NOT enabled\n");
		return;
	}

	ret = bt_adapter_get_name(&name);
	TC_PRT("bt_adapter_get_name() returns %s\n", __bt_get_error_message(ret));
	TC_PRT("name: [%s]\n", name);

	ret = bt_adapter_set_name("Test Name");
	TC_PRT("bt_adapter_set_name() returns %s\n", __bt_get_error_message(ret));

	ret = bt_adapter_get_name(&name);
	TC_PRT("bt_adapter_get_name() returns %s\n", __bt_get_error_message(ret));
	TC_PRT("name: [%s]\n", name);

	ret = bt_adapter_le_start_scan(__le_scan_result_cb, NULL);
	TC_PRT("bt_adapter_le_start_scan() returns %s\n", __bt_get_error_message(ret));
}

#if 0
static bool __check_adapter_enabled(void *data)
{
	TC_PRT("Waiting adapter enabled...\n");
	if (adapter_enabled == true)
		return EVENTLOOP_CALLBACK_STOP;
	return EVENTLOOP_CALLBACK_CONTINUE;
}
#endif

int bt_unit_test_main(int argc, char **argv)
{
	int ret = -1;

	TC_PRT("Enter bt_unit_test_main()\n");

	ret = bt_initialize();
	TC_PRT("bt_initialize() returns %s\n", __bt_get_error_message(ret));

	ret = bt_adapter_set_state_changed_cb(__state_changed_cb, NULL);
	TC_PRT("bt_adapter_set_state_changed_cb() returns %s\n", __bt_get_error_message(ret));

	ret = bt_adapter_enable();
	TC_PRT("bt_adapter_enable() returns %s\n", __bt_get_error_message(ret));

// TODO: temporarily block the code because if EVENTLOOP feature is enabled, can NOT enter TASH shell.
//	eventloop_add_timer_async(1000, true, __check_adapter_enabled, NULL);
//	eventloop_loop_run();
/*
	ret = bt_adapter_set_device_discovery_state_changed_cb(__discovery_state_changed_cb, NULL);
	TC_PRT("returns %s\n", __bt_get_error_message(ret));

	ret = bt_adapter_start_device_discovery();
	TC_PRT("returns %s\n", __bt_get_error_message(ret));

	ret = bt_adapter_le_start_scan(__le_scan_result_cb, NULL);
	TC_PRT("returns %s\n", __bt_get_error_message(ret));
*/
	return 0;
}
