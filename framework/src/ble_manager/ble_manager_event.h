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

typedef enum {
	// Common
	BLE_CMD_INIT,	
	BLE_CMD_DEINIT,
	BLE_CMD_GET_MAC,
	BLE_CMD_GET_BONDED_DEV,
	BLE_CMD_DEL_BOND,
	BLE_CMD_DEL_BOND_ALL,
	BLE_CMD_CONN_IS_ACTIVE,
	BLE_CMD_CONN_IS_ANY_ACTIVE,
	BLE_CMD_GET_VERSION,

	// Scanner
	BLE_CMD_START_SCAN,
	BLE_CMD_STOP_SCAN,
	BLE_CMD_WHITELIST_ADD,
	BLE_CMD_WHITELIST_DELETE,
	BLE_CMD_WHITELIST_CLEAR_ALL,
	BLE_CMD_WHITELIST_LIST,
	
	// Client
	BLE_CMD_CREATE_CTX,
	BLE_CMD_DESTROY_CTX,
	BLE_CMD_GET_CLIENT_STATE,
	BLE_CMD_CLIENT_CONNECT,
	BLE_CMD_CLIENT_RECONNECT,
	BLE_CMD_CLIENT_ENABLE_AUTOCONNECT,
	BLE_CMD_CLIENT_DISABLE_AUTOCONNECT,
	BLE_CMD_CLIENT_DISCONNECT,
	BLE_CMD_CLIENT_DISCONNECT_ALL,
	BLE_CMD_CONNECTED_DEV_LIST,
	BLE_CMD_CONNECTED_INFO,
	BLE_CMD_OP_ENABLE_NOTI,
	BLE_CMD_OP_ENABLE_INDICATE,
	BLE_CMD_OP_ENABLE_NOTI_AND_INDICATE,
	BLE_CMD_OP_READ,
	BLE_CMD_OP_WRITE,
	BLE_CMD_OP_WRITE_NO_RESP,

	// Server
	BLE_CMD_GET_PROFILE_COUNT,
	BLE_CMD_CHARACT_NOTI,
	BLE_CMD_CHARACT_INDI,
	BLE_CMD_ATTR_SET_DATA,
	BLE_CMD_ATTR_GET_DATA,
	BLE_CMD_ATTR_REJECT,
	BLE_CMD_SERVER_DISCONNECT,
	BLE_CMD_GET_MAC_BY_CONN,
	BLE_CMD_GET_CONN_BY_MAC,

	// Advertiser
	BLE_CMD_SET_ADV_DATA,
	BLE_CMD_SET_ADV_RESP,
	BLE_CMD_SET_ADV_TYPE,
	BLE_CMD_SET_ADV_INTERVAL,
	BLE_CMD_SET_ADV_TXPOWER,
	BLE_CMD_START_ADV,
	BLE_CMD_STOP_ADV,
	
	// Callback Event
	BLE_EVT_CLIENT_CONNECT,
	BLE_EVT_CLIENT_DISCONNECT,
	BLE_EVT_CLIENT_NOTI,
	BLE_EVT_CLIENT_INDI,
	BLE_EVT_SCAN_STATE,
	BLE_EVT_NONE,
} blemgr_req_e;
