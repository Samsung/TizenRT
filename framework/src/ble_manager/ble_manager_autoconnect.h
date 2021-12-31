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
#pragma once

#define BLE_MQ_SIZE 1

typedef enum {
	BLE_AUTOCON_FAIL,
	BLE_AUTOCON_SUCCESS,
	BLE_AUTOCON_ALREADY_RUNNING,
	BLE_AUTOCON_INVALID_STATE,
} ble_autocon_result_e;

typedef enum {
	BLE_AUTOCON_STATE_DISCONNECT,
	BLE_AUTOCON_STATE_SCAN_STARTING,
	BLE_AUTOCON_STATE_SCAN_STOPPING,
	BLE_AUTOCON_STATE_CONNECTING,
	BLE_AUTOCON_STATE_CONNECTED,
	BLE_AUTOCON_STATE_DONE,
} ble_autocon_state;

typedef enum {
	BLE_AUTOCON_EVT_SCAN_START,
	BLE_AUTOCON_EVT_MAC_SCANNED,
	BLE_AUTOCON_EVT_SCAN_STOP,
	BLE_AUTOCON_EVT_CONNECTED,
	BLE_AUTOCON_EVT_DISCONNECTED,
	BLE_AUTOCON_EVT_CANCEL,
} ble_autocon_event;

ble_autocon_result_e ble_manager_autoconnect(ble_client_ctx_internal *ctx);
