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

#include <mqueue.h>
#include <ble_manager/ble_manager.h>

typedef struct {
	ble_conn_handle conn_handle;
	volatile ble_client_state_e state;
	ble_conn_info info;
	bool is_bonded;
	bool auto_connect;
	mqd_t mqfd;
	ble_client_callback_list callbacks;
} ble_client_ctx_internal;

typedef struct {
	volatile ble_scan_state_e state;
	ble_scan_filter filter;
	ble_scan_callback_list callback;
} ble_scan_ctx;

typedef struct {
	ble_conn_handle conn_handle;
	volatile ble_server_state_e state;
	bool is_secured_connect_allowed;
	ble_server_gatt_t *profile;
	uint16_t profile_count;
	ble_server_connected_t callback;
} ble_server_ctx;
