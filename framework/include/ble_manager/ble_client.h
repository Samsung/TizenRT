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

/**
 * @defgroup BLE_Client
 * @ingroup BLE_Client
 * @brief Provides APIs for BLE Client
 * @{
 */

/**
 * @file ble_manager/ble_client.h
 * @brief Provides APIs for BLE Client
 */

#ifndef BLE_CLIENT_H_
#define BLE_CLIENT_H_

#include <stdint.h>
#include <stdbool.h>
#include "ble_common.h"

typedef enum {
	BLE_CLIENT_SCAN_STARTED,
	BLE_CLIENT_SCAN_STOPPED,
} ble_client_scan_state;

typedef enum {
	BLE_CLIENT_ADV_TYPE_IND,
	BLE_CLIENT_ADV_TYPE_DIRECT,
	BLE_CLIENT_ADV_TYPE_SCAN_IND,
	BLE_CLIENT_ADV_TYPE_NONCONN_IND,
	BLE_CLIENT_ADV_TYPE_SCAN_RSP,
	BLE_CLIENT_ADV_TYPE_UNKNOWN,
} ble_client_adv_type;

typedef enum {
	BLE_CLIENT_GATT_TYPE_SERVICE,
	BLE_CLIENT_GATT_TYPE_CHARACT,
	BLE_CLIENT_GATT_TYPE_DESC,
	BLE_CLIENT_GATT_TYPE_UNKNOWN,
} ble_client_gatt_type;

typedef enum {
	BLE_CLIENT_ADDR_TYPE_PUBLIC,
	BLE_CLIENT_ADDR_TYPE_RANDOM_STATIC,
	BLE_CLIENT_ADDR_TYPE_RANDOM_RESOLVABLE,
	BLE_CLIENT_ADDR_TYPE_RANDOM_NON_RESOLVABLE,
	BLE_CLIENT_ADDR_TYPE_UNKNOWN,
} ble_client_addr_type;

typedef struct {
	uint8_t bd_addr[BLE_BD_ADDR_MAX_LEN];
	ble_client_addr_type addr_type;
	uint16_t conn_interval;
	uint16_t slave_latency;
	uint16_t mtu;
	bool is_secured_connect;
} ble_client_bd_addr;

typedef struct {
	ble_client_adv_type adv_type;
	int8_t rssi;
	ble_client_bd_addr addr;
	uint8_t raw_data[BLE_ADV_RAW_DATA_MAX_LEN];
	uint8_t raw_data_length;
	uint8_t resp_data[BLE_ADV_RESP_DATA_MAX_LEN];
	uint8_t resp_data_length;
} ble_client_scanned_device;

typedef struct {
	ble_client_bd_addr addr;
	bool is_bonded;
	ble_conn_handle conn_handle;
} ble_client_device_connected;

typedef struct {
	ble_conn_handle conn_handle[BLE_MAX_CONNECTION_COUNT];
	uint8_t connected_count;
} ble_client_connected_list;

typedef struct {
	ble_conn_handle conn_handle;
	ble_attr_handle attr_handle;
} ble_client_operation_handle;

typedef struct {
	uint8_t raw_data[BLE_ADV_RAW_DATA_MAX_LEN];
	uint8_t raw_data_length;
	uint16_t scan_duration;
} ble_client_scan_filter;

typedef struct {
	/* This is a set of callback function for BLE client */
	void(*ble_client_scan_state_changed_cb)(ble_client_scan_state scan_state);
	void(*ble_client_device_scanned_cb)(ble_client_scanned_device* scanned_device);
	void(*ble_client_device_disconnected_cb)(ble_conn_handle conn_id);
	void(*ble_client_device_connected_cb)(ble_client_device_connected* connected_device);
	void(*ble_client_operation_notification_cb)(ble_client_operation_handle* handle, ble_data* read_result);
	uint16_t mtu;
} ble_client_init_config;

/****************************************************************************
 * Name: ble_client_start_scan
 *
 * Description:
 *   Start BLE adv scanning
 *
 * Input Parameters:
 *   filter    - If this value is NULL, start to scan for BLE adv till 
 *               'ble_client_stop_scan' is called.
 *               To get specific BLE adv info, filter value should be filled
 *               with exact value.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_client_start_scan(ble_client_scan_filter* filter);

/****************************************************************************
 * Name: ble_client_stop_scan
 *
 * Description:
 *   Stop BLE adv scanning. This should be called to stop BLE scanning after 
 *   ble_client_start_scan is called without filter.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_client_stop_scan(void);

/****************************************************************************
 * Name: ble_client_connect
 *
 * Description:
 *   Connect to Server(Peripheral).
 *
 * Input Parameters:
 *   addr      - This includes Server(Peripheral) info.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_client_connect(ble_client_bd_addr* addr);

/****************************************************************************
 * Name: ble_client_connected_device_list
 *
 * Description:
 *   Get the connected device list.
 *
 * Input Parameters:
 *   out_connected_list - This should have pre-allocated and cleared memory.
 *                        The BLE driver will write the device list.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_client_connected_device_list(ble_client_connected_list* out_connected_list);

/****************************************************************************
 * Name: ble_client_connected_info
 *
 * Description:
 *   Get the specific device info with conn_handle.
 *
 * Input Parameters:
 *   conn_handle          - The handle ID of connected device.
 *   out_connected_device - This should have pre-allocated and cleared memory.
 *                          The BLE driver will write the device info.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_client_connected_info(ble_conn_handle conn_handle, ble_client_device_connected* out_connected_device);

/****************************************************************************
 * Name: ble_client_delete_bond
 *
 * Description:
 *   Delete the bonded device.
 *
 * Input Parameters:
 *   addr      - This value should include bd_addr and addr_type.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_client_delete_bond(ble_client_bd_addr* addr);

/****************************************************************************
 * Name: ble_client_delete_bond_all
 *
 * Description:
 *   Delete all of the bonded devicse.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_client_delete_bond_all(void);

/****************************************************************************
 * Name: ble_client_disconnect
 *
 * Description:
 *   Disconnect with connected handle ID.
 *
 * Input Parameters:
 *   conn_handle  - The handle ID of connected device.
 * 
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_client_disconnect(ble_conn_handle conn_handle);

/****************************************************************************
 * Name: ble_client_disconnect_all
 *
 * Description:
 *   Disconnect all of the connected devicse.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_client_disconnect_all(void);

/****************************************************************************
 * Name: ble_client_operation_enable_notification
 *
 * Description:
 *   Enable the notification comming from the Server(Peripheral).
 * 
 * Input Parameters:
 *   handle  - This includes the handle ID and the attribute ID.
 * 
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_client_operation_enable_notification(ble_client_operation_handle* handle);

/****************************************************************************
 * Name: ble_client_operation_read
 *       ble_client_operation_write
 *       ble_client_operation_write_no_response
 *
 * Description:
 *   I/O operation with Server(Peripheral).
 * 
 * Input Parameters:
 *   handle  - This includes the handle ID and the attribute ID.
 *   dat     - I/O buffer which trasfers data.
 * 
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_client_operation_read(ble_client_operation_handle* handle, ble_data* out_data);
ble_result ble_client_operation_write(ble_client_operation_handle* handle, ble_data* in_data);
ble_result ble_client_operation_write_no_response(ble_client_operation_handle* handle, ble_data* in_data);

#endif
