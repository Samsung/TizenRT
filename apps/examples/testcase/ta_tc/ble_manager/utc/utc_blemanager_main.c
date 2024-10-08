/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include <stdlib.h>
#include <string.h>
#include <ble_manager/ble_manager.h>
#include <semaphore.h>
#include <stdbool.h>
#include <errno.h>
#include "ble_mock.h"
#include "tc_common.h"

#define UTC_FUNC_SIGNAL              \
	do {                                 \
		sem_post(&g_func_sem);        \
	} while (0)

#define UTC_FUNC_WAIT                \
	do {                                 \
		sem_wait(&g_func_sem);        \
	} while (0)

static sem_t g_func_sem;

#define BEVT_CONNECTED 		1
#define BEVT_DISCONNECTED 	0
#define BEVT_NO_NOTI 		0
#define BEVT_NOTI  		1

static uint8_t ble_filter[] = { 0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18 };

static ble_result_e g_client_connected = BEVT_DISCONNECTED;
static ble_scan_state_e g_scan_state = BLE_SCAN_STOPPED;
static ble_result_e g_client_noti = BEVT_NO_NOTI;

static void ble_scan_state_changed_cb(ble_scan_state_e scan_state)
{
	g_scan_state = scan_state;
	UTC_FUNC_SIGNAL;
	return;
}

static void ble_device_scanned_cb(ble_scanned_device *scanned_device)
{
	printf("scanned mac : %02x:%02x:%02x:%02x:%02x:%02x\n",
		scanned_device->addr.mac[0],
		scanned_device->addr.mac[1],
		scanned_device->addr.mac[2],
		scanned_device->addr.mac[3],
		scanned_device->addr.mac[4],
		scanned_device->addr.mac[5]
	);
	return;
}

static void ble_device_disconnected_cb(ble_client_ctx *ctx)
{
	printf("client disconnected callback received\n");
	g_client_connected = BEVT_DISCONNECTED;
	UTC_FUNC_SIGNAL;
	return;
}

static void ble_device_connected_cb(ble_client_ctx *ctx, ble_device_connected *dev)
{
	printf("client connected callback received\n");
	g_client_connected = BEVT_CONNECTED;
	UTC_FUNC_SIGNAL;
	return;
}

static void ble_operation_notification_cb(ble_client_ctx *ctx, ble_attr_handle attr_handle, ble_data *read_result)
{
	printf("notification callback received\n");
	g_client_noti = BEVT_NOTI;
	UTC_FUNC_SIGNAL;
	return;
}

static void ble_server_connected_cb(ble_conn_handle con_handle, ble_server_connection_type_e conn_type, uint8_t mac[BLE_BD_ADDR_MAX_LEN])
{
	printf("server connect callback received");
	return;
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
		.uuid_length = 16,
		.property = BLE_ATTR_PROP_RWN | BLE_ATTR_PROP_WRITE_NO_RSP,
		.permission = BLE_ATTR_PERM_R_PERMIT | BLE_ATTR_PERM_W_PERMIT,
		.attr_handle = 0x006b,
		.arg = "char_a_1"
	},

	{
		.type = BLE_SERVER_GATT_DESC,
		.uuid = {0x02, 0x29},
		.uuid_length = 2,
		.permission = BLE_ATTR_PERM_R_PERMIT | BLE_ATTR_PERM_W_PERMIT,
		.attr_handle = 0x006c,
		.arg = "desc_b_1",
	},
};

static uint8_t g_adv_raw[] = {
	0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18
};
static uint8_t g_adv_resp[] = {
	0x11, 0x09, 'T', 'I', 'Z', 'E', 'N', 'R', 'T', ' ', 'T', 'E', 'S', 'T', '(', '0', '2', ')',
};

static ble_scan_callback_list scan_config = {
	ble_scan_state_changed_cb,
	ble_device_scanned_cb,
};

static ble_client_callback_list client_config = {
	ble_device_disconnected_cb,
	ble_device_connected_cb,
	ble_operation_notification_cb,
};

static ble_server_init_config server_config = {
	ble_server_connected_cb,
	true,
	gatt_profile,
	sizeof(gatt_profile) / sizeof(ble_server_gatt_t)
};

static void set_scan_filter(ble_scan_filter *filter, uint8_t *raw_data, uint8_t len, bool whitelist_enable, uint32_t scan_duration)
{
	memset(filter, 0, sizeof(ble_scan_filter));
	if (raw_data != NULL && len > 0) {
		memcpy(filter->raw_data, raw_data, len);
		filter->raw_data_length = len;
	}

	filter->scan_duration = scan_duration;
	filter->whitelist_enable = whitelist_enable;
}

static void utc_blemanager_init_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);//not server role
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);//server role
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_init_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_ALREADY_WORKING);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_deinit_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);//not server role
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);//server role
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_deinit_n(void)
{
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_INVALID_STATE);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_mac_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	uint8_t mac[BLE_BD_ADDR_MAX_LEN];
	TC_ASSERT_EQ_CLEANUP("ble_manager_get_mac_addr", ble_manager_get_mac_addr(mac), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_mac_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	TC_ASSERT_EQ("ble_manager_get_mac_addr", ble_manager_get_mac_addr(NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_get_bonded_device_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_bonded_device_list dev_list[BLE_MAX_BONDED_DEVICE] = { 0, };
	uint16_t dev_count = 0;
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_get_bonded_device", ble_manager_get_bonded_device(dev_list, &dev_count), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_get_bonded_device_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_bonded_device_list dev_list[BLE_MAX_BONDED_DEVICE] = { 0, };
	uint16_t dev_count = 0;
	TC_ASSERT_EQ("ble_manager_get_bonded_device", ble_manager_get_bonded_device(NULL, &dev_count), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager_get_bonded_device", ble_manager_get_bonded_device(dev_list, NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_delete_bonded_device_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_bonded_device_list dev_list[BLE_MAX_BONDED_DEVICE] = { 0, };
	uint16_t dev_count = 0;
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_get_bonded_device", ble_manager_get_bonded_device(dev_list, &dev_count), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_delete_bonded", ble_manager_delete_bonded(&conn_info.addr), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_get_bonded_device", ble_manager_get_bonded_device(dev_list, &dev_count), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_delete_bonded_device_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_bonded_device_list dev_list[BLE_MAX_BONDED_DEVICE] = { 0, };
	uint16_t dev_count = 0;
	TC_ASSERT_EQ("ble_manager_get_bonded_device", ble_manager_get_bonded_device(NULL, &dev_count), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager_get_bonded_device", ble_manager_get_bonded_device(dev_list, NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_delete_bonded_all_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_bonded_device_list dev_list[BLE_MAX_BONDED_DEVICE] = { 0, };
	uint16_t dev_count = 0;
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_get_bonded_device", ble_manager_get_bonded_device(dev_list, &dev_count), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_delete_bonded_all", ble_manager_delete_bonded_all(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_get_bonded_device", ble_manager_get_bonded_device(dev_list, &dev_count), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_conn_active_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_conn_handle conn_handle = 0;
	bool is_active;
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_conn_is_active", ble_manager_conn_is_active(conn_handle, &is_active), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_conn_is_active", ble_manager_conn_is_active(conn_handle, &is_active), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_conn_is_active", ble_manager_conn_is_active(conn_handle, &is_active), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_conn_active_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_conn_handle conn_handle = 0;
	TC_ASSERT_EQ("ble_manager_conn_is_active", ble_manager_conn_is_active(conn_handle, NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_conn_any_active_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	bool is_active;
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_conn_is_active", ble_manager_conn_is_any_active(&is_active), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_conn_is_active", ble_manager_conn_is_any_active(&is_active), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager_conn_is_active", ble_manager_conn_is_any_active(&is_active), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_set_adv_data_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	ble_data data[1] = { 0, };
	data->data = g_adv_raw;
	data->length = sizeof(g_adv_raw);
	TC_ASSERT_EQ_CLEANUP("ble_server_set_adv_data", ble_server_set_adv_data(data), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_set_adv_data_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	TC_ASSERT_EQ("ble_server_set_adv_data", ble_server_set_adv_data(NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_set_adv_resp_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	ble_data data[1] = { 0, };
	data->data = g_adv_resp;
	data->length = sizeof(g_adv_resp);
	TC_ASSERT_EQ_CLEANUP("ble_server_set_adv_resp", ble_server_set_adv_resp(data), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_set_adv_resp_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	TC_ASSERT_EQ("ble_server_set_adv_resp", ble_server_set_adv_resp(NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_set_adv_type_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	ble_adv_type_e type = BLE_ADV_TYPE_IND;
	uint8_t mac[BLE_BD_ADDR_MAX_LEN];
	ble_addr addr = {
		{
			*mac,
			type,
		}
	};
	TC_ASSERT_EQ_CLEANUP("ble_server_set_adv_type", ble_server_set_adv_type(type, &addr), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_set_adv_type_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	ble_adv_type_e type = BLE_ADV_TYPE_IND;
	TC_ASSERT_EQ("ble_server_set_adv_type", ble_server_set_adv_type(type, NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_set_adv_interval_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	unsigned int interval = 10;
	TC_ASSERT_EQ_CLEANUP("ble_server_set_adv_interval", ble_server_set_adv_interval(interval), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_start_adv_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	TC_ASSERT_EQ_CLEANUP("ble_server_start_adv", ble_server_start_adv(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_server_stop_adv", ble_server_stop_adv(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_stop_adv_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	TC_ASSERT_EQ_CLEANUP("ble_server_start_adv", ble_server_start_adv(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_server_stop_adv", ble_server_stop_adv(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_get_profile_count_p(void) {
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	uint16_t count;
	TC_ASSERT_EQ_CLEANUP("ble_server_get_profile_count", ble_server_get_profile_count(&count), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_get_profile_count_n(void) {
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	TC_ASSERT_EQ("ble_server_get_profile_count", ble_server_get_profile_count(NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_attr_set_data_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	ble_attr_handle attr_handle = 0x006b + 1;
	uint8_t buf[256] = { 0, };
	ble_data in_data = { buf, sizeof(buf) };
	TC_ASSERT_EQ_CLEANUP("ble_server_attr_set_data", ble_server_attr_set_data(attr_handle, &in_data), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_attr_set_data_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	ble_attr_handle attr_handle = 0x006b + 1;
	TC_ASSERT_EQ("ble_server_attr_set_data", ble_server_attr_set_data(attr_handle, NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_attr_get_data_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	ble_attr_handle attr_handle = 0x006b + 1;
	uint8_t buf[256] = { 0, };
	ble_data in_data = { buf, sizeof(buf) };
	TC_ASSERT_EQ_CLEANUP("ble_server_attr_set_data", ble_server_attr_set_data(attr_handle, &in_data), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ble_data out_data;
	TC_ASSERT_EQ_CLEANUP("ble_server_attr_get_data", ble_server_attr_get_data(attr_handle, &out_data), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_attr_get_data_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	ble_attr_handle attr_handle = 0x006b + 1;
	TC_ASSERT_EQ("ble_server_attr_get_data", ble_server_attr_get_data(attr_handle, NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_reject_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(&server_config), BLE_MANAGER_SUCCESS);
	ble_attr_handle attr_handle = 0x006b + 1;
	uint8_t err_code = 0x02;
	TC_ASSERT_EQ_CLEANUP("ble_server_reject", ble_server_reject(attr_handle, err_code), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_client_start_scan_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	//no filter scan
	TC_ASSERT_EQ_CLEANUP("ble_client_start_scan", ble_client_start_scan(NULL, &scan_config), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_SCAN_STATE, BLE_SCAN_STARTED, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_stop_scan cb", g_scan_state, BLE_SCAN_STARTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_stop_scan", ble_client_stop_scan(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_SCAN_STATE, BLE_SCAN_STOPPED, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_stop_scan cb", g_scan_state, BLE_SCAN_STOPPED, ble_manager_deinit());
	//filter scan without whitelist
	ble_scan_filter filter = { 0, };
	set_scan_filter(&filter, ble_filter, sizeof(ble_filter), false, 1000);
	TC_ASSERT_EQ_CLEANUP("ble_client_start_scan", ble_client_start_scan(&filter, &scan_config), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_SCAN_STATE, BLE_SCAN_STARTED, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_stop_scan cb", g_scan_state, BLE_SCAN_STARTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_stop_scan", ble_client_stop_scan(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_SCAN_STATE, BLE_SCAN_STOPPED, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_stop_scan cb", g_scan_state, BLE_SCAN_STOPPED, ble_manager_deinit());
	//scan with whitelist
	set_scan_filter(&filter, NULL, 0, true, 1000);
	ble_addr addr;
	addr.type = BLE_ADV_TYPE_IND;
	uint8_t dummy[] = {0x01, 0x1E, 0xC0, 0x04, 0x9F, 0xF6};
	memcpy(addr.mac, dummy, BLE_BD_ADDR_MAX_LEN);
	TC_ASSERT_EQ_CLEANUP("ble_scan_whitelist_add", ble_scan_whitelist_add(&addr), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_start_scan", ble_client_start_scan(&filter, &scan_config), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_SCAN_STATE, BLE_SCAN_STARTED, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_stop_scan cb", g_scan_state, BLE_SCAN_STARTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_stop_scan", ble_client_stop_scan(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_SCAN_STATE, BLE_SCAN_STOPPED, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_stop_scan cb", g_scan_state, BLE_SCAN_STOPPED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_client_stop_scan_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	TC_ASSERT_EQ_CLEANUP("ble_client_start_scan", ble_client_start_scan(NULL, &scan_config), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_SCAN_STATE, BLE_SCAN_STARTED, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_stop_scan", ble_client_stop_scan(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_SCAN_STATE, BLE_SCAN_STOPPED, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_start_scan cb", g_scan_state, BLE_SCAN_STOPPED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_whitelist_add_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_scan_filter filter2 = { 0, };
	set_scan_filter(&filter2, NULL, 0, true, 100);
	ble_addr addr;
	addr.type = BLE_ADV_TYPE_IND;
	uint8_t dummy[] = {0x01, 0x1E, 0xC0, 0x04, 0x9F, 0xF6};
	memcpy(addr.mac, dummy, BLE_BD_ADDR_MAX_LEN);
	TC_ASSERT_EQ_CLEANUP("ble_scan_whitelist_add", ble_scan_whitelist_add(&addr), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_scan_whitelist_delete", ble_scan_whitelist_delete(&addr), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_whitelist_add_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	TC_ASSERT_EQ("ble_scan_whitelist_add", ble_scan_whitelist_add(NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_whitelist_del_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_scan_filter filter2 = { 0, };
	set_scan_filter(&filter2, NULL, 0, true, 100);
	ble_addr addr;
	addr.type = BLE_ADV_TYPE_IND;
	uint8_t dummy[] = {0x01, 0x1E, 0xC0, 0x04, 0x9F, 0xF6};
	memcpy(addr.mac, dummy, BLE_BD_ADDR_MAX_LEN);
	TC_ASSERT_EQ_CLEANUP("ble_scan_whitelist_add", ble_scan_whitelist_add(&addr), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_scan_whitelist_delete", ble_scan_whitelist_delete(&addr), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_whitelist_del_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	TC_ASSERT_EQ("ble_scan_whitelist_add", ble_scan_whitelist_delete(NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_whitelist_clear_all_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_scan_filter filter2 = { 0, };
	set_scan_filter(&filter2, NULL, 0, true, 100);
	ble_addr addr;
	addr.type = BLE_ADV_TYPE_IND;
	uint8_t dummy[] = {0x01, 0x1E, 0xC0, 0x04, 0x9F, 0xF6};
	memcpy(addr.mac, dummy, BLE_BD_ADDR_MAX_LEN);
	TC_ASSERT_EQ_CLEANUP("ble_scan_whitelist_add", ble_scan_whitelist_add(&addr), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_scan_whitelist_clear_all", ble_scan_whitelist_clear_all(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_client_connect_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);//not server role
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_client_disconnect_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);//not server role
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_client_disconnect_all_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect_all", ble_client_disconnect_all(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_client_con_list_p(void)
{
	ble_device_connected_list connected_list;
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_client_ctx *ctx1, *ctx2;
	ctx1 = ble_client_create_ctx(&client_config);
	ctx2 = ble_client_create_ctx(&client_config);
	uint8_t dummy1[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	uint8_t dummy2[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF2};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy1, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx1, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	memcpy(conn_info.addr.mac, dummy2, BLE_BD_ADDR_MAX_LEN);
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx2, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_connected_device_list", ble_client_connected_device_list(&connected_list), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect_all", ble_client_disconnect_all(), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_connected_dev_info_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy1[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy1, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	ble_device_connected connected_dev;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected_info", ble_client_connected_info(ctx, &connected_dev), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_operation_enable_notification_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy1[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy1, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	ble_attr_handle attr_handle = 0x006e + 1;
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_operation_enable_notification", ble_client_operation_enable_notification(ctx, attr_handle), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_NOTI, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_notification cb", g_client_noti, BEVT_NOTI, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_read_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy1[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy1, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	ble_attr_handle attr_handle = 0x006e + 1;
	uint8_t buf[256] = { 0, };
	ble_data data = { buf, sizeof(buf) };
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_operation_read", ble_client_operation_read(ctx, attr_handle, &data), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_read_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy1[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy1, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	ble_attr_handle attr_handle = 0x006e + 1;
	TC_ASSERT_EQ("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS);
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ("ble_client_connected cb", g_client_connected, BEVT_CONNECTED);
	TC_ASSERT_EQ("ble_client_operation_read", ble_client_operation_read(ctx, attr_handle, NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS);
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_write_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy1[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy1, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	ble_attr_handle attr_handle = 0x006e + 1;
	uint8_t buf[256] = { 0, };
	ble_data data = { buf, sizeof(buf) };
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_operation_write", ble_client_operation_write(ctx, attr_handle, &data), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_write_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy1[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy1, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	ble_attr_handle attr_handle = 0x006e + 1;
	TC_ASSERT_EQ("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS);
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ("ble_client_connected cb", g_client_connected, BEVT_CONNECTED);
	TC_ASSERT_EQ("ble_client_operation_write", ble_client_operation_write(ctx, attr_handle, NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS);
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_write_no_resp_p(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy1[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy1, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	ble_attr_handle attr_handle = 0x006e + 1;
	uint8_t buf[256] = { 0, };
	ble_data data = { buf, sizeof(buf) };
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_connected cb", g_client_connected, BEVT_CONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_operation_write_no_response", ble_client_operation_write_no_response(ctx, attr_handle, &data), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS, ble_manager_deinit());
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED, ble_manager_deinit());
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_blemanager_write_no_resp_n(void)
{
	TC_ASSERT_EQ("ble_manager_init", ble_manager_init(NULL), BLE_MANAGER_SUCCESS);
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	uint8_t dummy1[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, dummy1, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	ble_attr_handle attr_handle = 0x006e + 1;
	TC_ASSERT_EQ("ble_client_connect", ble_client_connect(ctx, &conn_info), BLE_MANAGER_SUCCESS);
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_CONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ("ble_client_connected cb", g_client_connected, BEVT_CONNECTED);
	TC_ASSERT_EQ("ble_client_operation_write_no_response", ble_client_operation_write_no_response(ctx, attr_handle, NULL), BLE_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("ble_client_disconnect", ble_client_disconnect(ctx), BLE_MANAGER_SUCCESS);
	CONTROL_VVDRIVER(VBLE_CMD_GEN_EVT, LWNL_EVT_BLE_CLIENT_DISCONNECT, 0, 3);
	UTC_FUNC_WAIT;
	TC_ASSERT_EQ("ble_client_disconnected cb", g_client_connected, BEVT_DISCONNECTED);
	TC_ASSERT_EQ("ble_manager deinit", ble_manager_deinit(), BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

int blemanager_utc(int argc, FAR char *argv[])
{
	if (testcase_state_handler(TC_START, "BLE Manager UTC") == ERROR) {
		return ERROR;
	}
	int res = sem_init(&g_func_sem, 0, 0);
	if (res != 0) {
		printf("g_func_sem init failed\n");
		return ERROR;
	}

	//init
	utc_blemanager_init_p();
	utc_blemanager_init_n();

	//deinit
	utc_blemanager_deinit_p();
	utc_blemanager_deinit_n();

	//mac address of ble device
	utc_blemanager_mac_p();
	utc_blemanager_mac_n();

	//bonded devices
	utc_blemanager_get_bonded_device_p();
	utc_blemanager_get_bonded_device_n();
	utc_blemanager_delete_bonded_device_p();
	utc_blemanager_delete_bonded_device_n();
	utc_blemanager_delete_bonded_all_p();

	//connection is_active
	utc_blemanager_conn_active_p();
	utc_blemanager_conn_active_n();
	utc_blemanager_conn_any_active_p();

	//server adv set data
	utc_blemanager_set_adv_data_p();
	utc_blemanager_set_adv_data_n();

	//server adv set resp
	utc_blemanager_set_adv_resp_p();
	utc_blemanager_set_adv_resp_n();

	//server adv type
	utc_blemanager_set_adv_type_p();
	utc_blemanager_set_adv_type_n();

	//server adv interval
	utc_blemanager_set_adv_interval_p();

	// start advertising
	utc_blemanager_start_adv_p();

	// stop advertising
	utc_blemanager_stop_adv_p();

	//server profile count
	utc_blemanager_get_profile_count_p();
	utc_blemanager_get_profile_count_n();

	//attribute set data
	utc_blemanager_attr_set_data_p();
	utc_blemanager_attr_set_data_n();

	//attribute get data
	utc_blemanager_attr_get_data_p();
	utc_blemanager_attr_get_data_n();

	//reject attribute request
	utc_blemanager_reject_p();

	//scan
	utc_blemanager_client_start_scan_p();
	utc_blemanager_client_stop_scan_p();

	//whitelist
	utc_blemanager_whitelist_add_p();
	utc_blemanager_whitelist_add_n();
	utc_blemanager_whitelist_del_p();
	utc_blemanager_whitelist_del_n();
	utc_blemanager_whitelist_clear_all_p();

	//client connect disconnect
	utc_blemanager_client_connect_p();
	utc_blemanager_client_disconnect_p();
	utc_blemanager_client_disconnect_all_p();

	//list connected devices to client
	utc_blemanager_client_con_list_p();
	utc_blemanager_connected_dev_info_p();
	utc_blemanager_operation_enable_notification_p();

	//io
	utc_blemanager_read_p();
	utc_blemanager_read_n();

	utc_blemanager_write_p();
	utc_blemanager_write_n();

	utc_blemanager_write_no_resp_p();
	utc_blemanager_write_no_resp_n();

	sem_destroy(&g_func_sem);

	(void)testcase_state_handler(TC_END, "BLEManager UTC");
	return 0;
}

int utc_blemanager_main(int argc, char *argv[])
{

	blemanager_utc(argc, argv);

	return 0;
}