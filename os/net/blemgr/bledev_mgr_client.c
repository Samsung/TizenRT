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

#include <tinyara/kmalloc.h>
#include <string.h>
#include <tinyara/net/if/ble.h>
#include <tinyara/ble/ble_manager.h>
#include <ble_manager/ble_manager.h>
#include "bledev_mgr_client.h"

#define BLE_DRV_TAG "[BLEDRV_CLIENT]"

static inline void _reverse_mac(uint8_t *mac)
{
	int i;
	int j;
	uint8_t temp;

	for (i = 0, j = TRBLE_BD_ADDR_MAX_LEN - 1; i < j; i++, j--) {
		temp = mac[i];
		mac[i] = mac[j];
		mac[j] = temp;
	}
}

static void bledrv_scan_state_changed_cb(trble_scan_state_e scan_state)
{
	trble_post_event(LWNL_EVT_BLE_SCAN_STATE, (void *)&scan_state, sizeof(trble_scan_state_e));
	return;
}

static void bledrv_device_scanned_cb(trble_scanned_device *scanned_device)
{
	_reverse_mac(scanned_device->addr.mac);
	trble_scan_data_enque(scanned_device);
	return;
}

static void bledrv_device_disconnected_cb(trble_conn_handle conn_id)
{
	trble_post_event(LWNL_EVT_BLE_CLIENT_DISCONNECT, (void *)&conn_id, sizeof(trble_conn_handle));
	return;
}

static void bledrv_device_connected_cb(trble_device_connected *dev)
{
	_reverse_mac(dev->conn_info.addr.mac);
	trble_post_event(LWNL_EVT_BLE_CLIENT_CONNECT, (void *)dev, sizeof(trble_device_connected));
	return;
}

static void bledrv_operation_notification_cb(trble_operation_handle *handle, trble_data *read_result)
{
	int32_t size = sizeof(trble_conn_handle) + sizeof(trble_attr_handle) + sizeof(read_result->length) + read_result->length;
	uint8_t *data = (uint8_t *)kmm_malloc(size);
	if (data == NULL) {
		BLE_LOGE(BLE_DRV_TAG, "out of memroy\n");
		return;
	}
	uint8_t *ptr = data;
	
	// Copy conn handle
	memcpy(ptr, &(handle->conn_handle), sizeof(trble_conn_handle));
	ptr += sizeof(trble_conn_handle);

	// Copy attr handle
	memcpy(ptr, &(handle->attr_handle), sizeof(trble_attr_handle));
	ptr += sizeof(trble_attr_handle);

	// Copy read_result len
	memcpy(ptr, &(read_result->length), sizeof(read_result->length));
	ptr += sizeof(read_result->length);

	// Copy read_result data
	memcpy(ptr, read_result->data, read_result->length);

	trble_post_event(LWNL_EVT_BLE_CLIENT_NOTI, data, -(size));

	return;
}

static void bledrv_operation_indication_cb(trble_operation_handle *handle, trble_data *read_result)
{
	int32_t size = sizeof(trble_conn_handle) + sizeof(trble_attr_handle) + sizeof(read_result->length) + read_result->length;
	uint8_t *data = (uint8_t *)kmm_malloc(size);
	if (data == NULL) {
		BLE_LOGE(BLE_DRV_TAG, "out of memroy\n");
		return;
	}
	uint8_t *ptr = data;

	// Copy conn handle
	memcpy(ptr, &(handle->conn_handle), sizeof(trble_conn_handle));
	ptr += sizeof(trble_conn_handle);

	// Copy attr handle
	memcpy(ptr, &(handle->attr_handle), sizeof(trble_attr_handle));
	ptr += sizeof(trble_attr_handle);

	// Copy read_result len
	memcpy(ptr, &(read_result->length), sizeof(read_result->length));
	ptr += sizeof(read_result->length);

	// Copy read_result data
	memcpy(ptr, read_result->data, read_result->length);

	trble_post_event(LWNL_EVT_BLE_CLIENT_INDI, data, -(size));

	return;
}

static void bledrv_operation_display_passkey_cb(uint32_t passkey, trble_conn_handle handle)
{
	int32_t size = sizeof(uint32_t) + sizeof(trble_conn_handle);
	uint8_t *data = (uint8_t *)kmm_malloc(size);
	if (data == NULL) {
		BLE_LOGE(BLE_DRV_TAG, "out of memroy\n");
		return;
	}
	uint8_t *ptr = data;
	// Copy connection handle
	memcpy(ptr, &handle, sizeof(trble_conn_handle));
	ptr += sizeof(trble_conn_handle);
	// Copy passkey
	memcpy(ptr, &passkey, sizeof(uint32_t));
	
	trble_post_event(LWNL_EVT_BLE_CLIENT_DISPLAY_PASSKEY, data, size);
	return;
}

static trble_client_init_config g_client_fake_config = {
	bledrv_scan_state_changed_cb,
	bledrv_device_scanned_cb,
	bledrv_device_disconnected_cb,
	bledrv_device_connected_cb,
	bledrv_operation_notification_cb,
	bledrv_operation_indication_cb,
	bledrv_operation_display_passkey_cb,
	247
};

trble_client_init_config *bledrv_client_get_fake_config(void)
{
	return &g_client_fake_config;
}
