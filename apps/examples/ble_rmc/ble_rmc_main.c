/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ble_manager/ble_manager.h>

#define RMC_TAG "[RMC]"
#define RMC_LOG(tag, fmt, args...) printf(tag fmt, ##args)

static void ble_scan_state_changed_cb(ble_client_scan_state scan_state)
{
	RMC_LOG(RMC_TAG, "'%s' is called\n", __FUNCTION__);
	return;
}

static void ble_device_scanned_cb(ble_client_scanned_device *scanned_device)
{
	RMC_LOG(RMC_TAG, "'%s' is called\n", __FUNCTION__);
	return;
}

static void ble_device_disconnected_cb(ble_conn_handle conn_id)
{
	RMC_LOG(RMC_TAG, "'%s' is called\n", __FUNCTION__);
	return;
}

static void ble_device_connected_cb(ble_client_device_connected *connected_device)
{
	RMC_LOG(RMC_TAG, "'%s' is called\n", __FUNCTION__);
	return;
}

static void ble_operation_notification_cb(ble_client_operation_handle *handle, ble_data *read_result)
{
	RMC_LOG(RMC_TAG, "'%s' is called\n", __FUNCTION__);
	return;
}

static void ble_server_connected_cb(ble_conn_handle con_handle, ble_server_connection_type_e conn_type, uint8_t mac[BLE_BD_ADDR_MAX_LEN])
{
	RMC_LOG(RMC_TAG, "'%s' is called\n", __FUNCTION__);
	return;
}

static void utc_cb_charact_a_1(ble_server_attr_cb_type_e type, ble_conn_handle linkindex, ble_attr_handle handle, void *arg)
{
	char *arg_str = "None";
	if (arg != NULL) {
		arg_str = (char *)arg;
	}
	printf("-- [RMC CHAR] %s [ type : %d / handle : %d / attr : %d ] --\n", arg_str, type, linkindex, handle);
}

static void utc_cb_desc_b_1(ble_server_attr_cb_type_e type, ble_conn_handle linkindex, ble_attr_handle handle, void *arg)
{
	char *arg_str = "None";
	if (arg != NULL) {
		arg_str = (char *)arg;
	}
	printf("-- [RMC DESC] %s [ type : %d / handle : %d / attr : %d ] --\n", arg_str, type, linkindex, handle);
}

static ble_server_gatt_t gatt_profile[] = {
	{
		.type = BLE_SERVER_GATT_SERVICE,
		.uuid = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01},
		.uuid_length = 16,
		.attr_handle = 0x006a,
	},

	{
		.type = BLE_SERVER_GATT_CHARACT, 
		.uuid = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02}, 
		.uuid_length = 16, .property = BLE_ATTR_PROP_RWN | BLE_ATTR_PROP_WRITE_NO_RSP, 
		.permission = BLE_ATTR_PERM_R_PERMIT | BLE_ATTR_PERM_W_PERMIT, 
		.attr_handle = 0x006b, 
		.cb = utc_cb_charact_a_1, 
		.arg = NULL
	},

	{
		.type = BLE_SERVER_GATT_DESC, 
		.uuid = {0x02, 0x29}, 
		.uuid_length = 2, 
		.permission = BLE_ATTR_PERM_R_PERMIT | BLE_ATTR_PERM_W_PERMIT, 
		.attr_handle = 0x006c, 
		.cb = utc_cb_desc_b_1, 
		.arg = NULL,
	},
};

/****************************************************************************
 * ble_rmc_main
 ****************************************************************************/
int ble_rmc_main(int argc, char *argv[])
{
	RMC_LOG(RMC_TAG, "test!!\n");

	ble_result ret;

	ble_client_init_config client_config = {
		ble_scan_state_changed_cb,
		ble_device_scanned_cb,
		ble_device_disconnected_cb,
		ble_device_connected_cb,
		ble_operation_notification_cb,
		240};

	ble_server_init_config server_config = {
		ble_server_connected_cb,
		true,
		gatt_profile, sizeof(gatt_profile) / sizeof(ble_server_gatt_t)};

	if (argc < 2) {
		return 0;
	}

	if (strncmp(argv[1], "init", 4) == 0) {
		ret = ble_manager_init(&client_config, &server_config);
		RMC_LOG(RMC_TAG, "init done[%d]\n", ret);
	}

	if (strncmp(argv[1], "deinit", 6) == 0) {
		ret = ble_manager_deinit();
		RMC_LOG(RMC_TAG, "deinit done[%d]\n", ret);
	}

	if (strncmp(argv[1], "mac", 3) == 0) {
		uint8_t mac[BLE_BD_ADDR_MAX_LEN];
		int i;

		ret = ble_manager_get_mac_addr(mac);

		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_TAG, "get mac fail\n");
			return 0;
		}

		RMC_LOG(RMC_TAG, "BLE mac : %02x", mac[0]);
		for (i = 1; i < BLE_BD_ADDR_MAX_LEN; i++) {
			printf(":%02x", mac[i]);
		}
		printf("\n");
	}

	RMC_LOG(RMC_TAG, "done\n");

	return 0;
}
