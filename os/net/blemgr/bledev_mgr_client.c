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

#include <string.h>
#include <tinyara/net/if/ble.h>
#include <tinyara/ble/ble_manager.h>
#include <ble_manager/ble_manager.h>
#include "bledev_mgr_client.h"

#define BLE_DRV_SERVER_MAX_DATA_LEN 512
#define BLE_DRV_TAG "[BLEDRV_CLIENT]"

static ble_client_init_config g_client_config = { 0, };

static void bledrv_scan_state_changed_cb(trble_scan_state_e scan_state)
{
	if (g_client_config.ble_client_scan_state_changed_cb != NULL) {
		g_client_config.ble_client_scan_state_changed_cb((ble_client_scan_state_e)scan_state);
	}
	return;
}

static void bledrv_device_scanned_cb(trble_scanned_device *scanned_device)
{
	if (g_client_config.ble_client_device_scanned_cb != NULL) {
		g_client_config.ble_client_device_scanned_cb((ble_client_scanned_device *)scanned_device);
	}
	return;
}

static void bledrv_device_disconnected_cb(trble_conn_handle conn_id)
{
	if (g_client_config.ble_client_device_disconnected_cb != NULL) {
		g_client_config.ble_client_device_disconnected_cb((ble_conn_handle)conn_id);
	}
	return;
}

static void bledrv_device_connected_cb(trble_device_connected *dev)
{
	if (g_client_config.ble_client_device_connected_cb != NULL) {
		g_client_config.ble_client_device_connected_cb((ble_client_device_connected *)dev);
	}
	return;
}

static void bledrv_operation_notification_cb(trble_operation_handle *handle, trble_data *read_result)
{
	if (g_client_config.ble_client_operation_notification_cb != NULL) {
		g_client_config.ble_client_operation_notification_cb((ble_client_operation_handle *)handle, (ble_data *)read_result);
	}
	return;
}

static trble_client_init_config g_client_fake_config = {
	bledrv_scan_state_changed_cb,
	bledrv_device_scanned_cb,
	bledrv_device_disconnected_cb,
	bledrv_device_connected_cb,
	bledrv_operation_notification_cb,
	240
};

trble_client_init_config *bledrv_client_get_fake_config(void)
{
	return &g_client_fake_config;
}

void bledrv_client_set_config(trble_client_init_config *init_client)
{
	memcpy(&g_client_config, init_client, sizeof(trble_client_init_config));
	return;
}
