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
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "ble_common.h"

typedef enum {
	BLE_CLIENT_NONE = 0,
	BLE_CLIENT_IDLE,
	BLE_CLIENT_CONNECTED,
	BLE_CLIENT_CONNECTING,
	BLE_CLIENT_DISCONNECTING,
	BLE_CLIENT_AUTOCONNECTING,
} ble_client_state_e;

typedef struct {
	ble_addr addr;
	uint16_t conn_interval;
	uint16_t slave_latency;
	uint16_t mtu;
	uint16_t scan_timeout; /* ms */
	bool is_secured_connect;
} ble_conn_info;

typedef struct {
	ble_conn_info conn_info;
	bool is_bonded;
	ble_conn_handle conn_handle;
} ble_device_connected;

typedef struct {
	ble_conn_handle conn_handle[BLE_MAX_CONNECTION_COUNT];
	uint8_t connected_count;
} ble_device_connected_list;

typedef struct {
	ble_conn_handle conn_handle;
	volatile ble_client_state_e state;
	ble_conn_info info;
	bool is_bonded;
	bool auto_connect;
} ble_client_ctx;

typedef void(*ble_client_device_disconnected_cb)(ble_client_ctx *ctx);
typedef void(*ble_client_device_connected_cb)(ble_client_ctx *ctx, ble_device_connected* connected_device);
typedef void(*ble_client_operation_notification_cb)(ble_client_ctx *ctx, ble_attr_handle attr_handle, ble_data* read_result);
typedef void (*ble_client_operation_indication_cb)(ble_client_ctx *ctx, ble_attr_handle attr_handle, ble_data* read_result);
typedef void (*ble_client_passkey_display_cb)(ble_client_ctx *ctx, uint32_t passkey, ble_conn_handle conn_handle);
typedef void (*ble_client_coc_reg_psm_cb)(ble_client_ctx *ctx, uint16_t le_psm, uint16_t err);
typedef void (*ble_client_coc_set_sec_cb)(ble_client_ctx *ctx, uint16_t err);
typedef void (*ble_client_coc_con_cb)(ble_client_ctx *ctx, uint16_t conn_handle, uint16_t cid, uint16_t err);
typedef void (*ble_client_coc_discon_cb)(ble_client_ctx *ctx, uint16_t conn_handle, uint16_t cid, uint16_t err);
typedef void (*ble_client_coc_send_cb)(ble_client_ctx *ctx, uint16_t conn_handle, uint16_t cid, uint16_t err, uint8_t credit);
typedef void (*ble_client_coc_recv_cb)(ble_client_ctx *ctx, uint16_t conn_handle, uint16_t cid, ble_data *read_result);

typedef struct {
	/* This is a set of callback function for BLE client */
	ble_client_device_disconnected_cb disconnected_cb;
	ble_client_device_connected_cb connected_cb;
	ble_client_operation_notification_cb notification_cb;
	ble_client_operation_indication_cb indication_cb;
	ble_client_passkey_display_cb passkey_display_cb;
	ble_client_coc_reg_psm_cb coc_reg_psm_cb;
	ble_client_coc_set_sec_cb coc_set_sec_cb;
	ble_client_coc_con_cb coc_con_cb;
	ble_client_coc_discon_cb coc_discon_cb;
	ble_client_coc_send_cb coc_send_cb;
	ble_client_coc_recv_cb coc_recv_cb;
} ble_client_callback_list;

ble_client_ctx *ble_client_create_ctx(ble_client_callback_list *callbacks);
ble_result_e ble_client_destroy_ctx(ble_client_ctx *ctx);
ble_client_state_e ble_client_get_state(ble_client_ctx *ctx);

/****************************************************************************
 * Name: ble_client_connect
 *
 * Description:
 *   Connect to Server(Peripheral).
 *
 * Input Parameters:
 *   ctx        - The context of client.
 *   conn_info  - This includes Server(Peripheral) info.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_client_connect(ble_client_ctx *ctx, ble_conn_info* conn_info);
ble_result_e ble_client_reconnect(ble_client_ctx *ctx);
ble_result_e ble_client_autoconnect(ble_client_ctx *ctx, bool is_auto);

/****************************************************************************
 * Name: ble_client_disconnect
 *
 * Description:
 *   Disconnect with connected handle ID.
 *
 * Input Parameters:
 *   ctx        - The context of client.
 * 
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_client_disconnect(ble_client_ctx *ctx);

/****************************************************************************
 * Name: ble_client_disconnect_all
 *
 * Description:
 *   Disconnect all of the connected devices.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_client_disconnect_all(void);

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
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_client_connected_device_list(ble_device_connected_list* out_connected_list);

/****************************************************************************
 * Name: ble_client_connected_info
 *
 * Description:
 *   Get the specific device info with conn_handle.
 *
 * Input Parameters:
 *   ctx                  - The context of client.
 *   out_connected_device - This should have pre-allocated and cleared memory.
 *                          The BLE driver will write the device info.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_client_connected_info(ble_client_ctx *ctx, ble_device_connected* out_connected_device);

/****************************************************************************
 * Name: ble_client_operation_enable_notification
 *       ble_client_operation_enable_indication
 *       ble_client_operation_enable_notification_and_indication
 *
 * Description:
 *   Enable the notification coming from the Server(Peripheral).
 * 
 * Input Parameters:
 *   ctx     - The context of client.
 *   attr_handle  - attribute handle.
 * 
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_client_operation_enable_notification(ble_client_ctx *ctx, ble_attr_handle attr_handle);
ble_result_e ble_client_operation_enable_indication(ble_client_ctx *ctx, ble_attr_handle attr_handle);
ble_result_e ble_client_operation_enable_notification_and_indication(ble_client_ctx *ctx, ble_attr_handle attr_handle);

/****************************************************************************
 * Name: ble_client_operation_read
 *       ble_client_operation_write
 *       ble_client_operation_write_no_response
 *
 * Description:
 *   I/O operation with Server(Peripheral).
 * 
 * Input Parameters:
 *   ctx          - The context of client.
 *   attr_handle  - attribute handle.
 *   data         - I/O buffer which transfers data.
 *                  (On read, this value should be allocated and set the length as the size of buffer.)
 *
 * 
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_client_operation_read(ble_client_ctx *ctx, ble_attr_handle attr_handle, ble_data* data);
ble_result_e ble_client_operation_write(ble_client_ctx *ctx, ble_attr_handle attr_handle, ble_data* data);
ble_result_e ble_client_operation_write_no_response(ble_client_ctx *ctx, ble_attr_handle attr_handle, ble_data* data);
