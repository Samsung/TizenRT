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

#include <stdio.h>
#include <tinyara/net/if/ble.h>
#include <tinyara/ble/ble_manager.h>
#include "bledev_mgr_server.h"

#define BLE_DRV_TAG "[BLEDRV_SERVER]"

static void ble_server_connected_null_cb(trble_conn_handle con_handle, trble_server_connection_type_e conn_type, uint8_t mac[TRBLE_BD_ADDR_MAX_LEN])
{
	return;
}

static void ble_server_mtu_update_null_cb(trble_conn_handle con_handle, uint16_t mtu_size)
{
	return;
}

static void ble_server_null_cb(trble_attr_cb_type_e type, trble_conn_handle linkindex, trble_attr_handle handle, void *arg)
{
	return;
}

static trble_gatt_t gatt_null_profile[] = {
	{
		.type = TRBLE_GATT_SERVICE,
		.uuid = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01},
		.uuid_length = 16,
		.attr_handle = 0x00a1,
	},

	{
		.type = TRBLE_GATT_CHARACT, 
		.uuid = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02}, 
		.uuid_length = 16, 
		.property = TRBLE_ATTR_PROP_RWN | TRBLE_ATTR_PROP_WRITE_NO_RSP, 
		.permission = TRBLE_ATTR_PERM_R_PERMIT | TRBLE_ATTR_PERM_W_PERMIT, 
		.attr_handle = 0x00a2, 
		.cb = ble_server_null_cb, 
		.arg = NULL
	},

	{
		.type = TRBLE_GATT_DESC, 
		.uuid = {0x02, 0x29}, 
		.uuid_length = 2, 
		.permission = TRBLE_ATTR_PERM_R_PERMIT | TRBLE_ATTR_PERM_W_PERMIT, 
		.attr_handle = 0x00a3, 
		.cb = ble_server_null_cb, 
		.arg = NULL,
	},
};

// This config is for empty server.
static trble_server_init_config g_server_null_config = {
	ble_server_connected_null_cb,
	ble_server_mtu_update_null_cb,
	true,
	gatt_null_profile,
	sizeof(gatt_null_profile) / sizeof(trble_gatt_t)
};

trble_server_init_config *bledrv_server_get_null_config(void)
{
	return &g_server_null_config;
}
