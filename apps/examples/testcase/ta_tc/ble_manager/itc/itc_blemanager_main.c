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
#include <sys/time.h>
#include <ble_manager/ble_manager.h>
#include <semaphore.h>
#include <stdbool.h>
#include <errno.h>
#include "tc_common.h"

#define LOOP_SIZE 2	//used to compute average time for different operations

#define ITC_FUNC_SIGNAL              \
	do {                                 \
		sem_post(&g_func_sem);        \
	} while (0)

#define ITC_FUNC_WAIT                \
	do {                                 \
		sem_wait(&g_func_sem);        \
	} while (0)

static sem_t g_func_sem;

static ble_addr g_target = { 0, };

static uint8_t ble_filter[] = { 0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18 };

static void addr_string_to_mac(const char *address)
{
        int i;
        char *ptr = NULL;

        for (i = 0; i < BLE_BD_ADDR_MAX_LEN; i++) {
                g_target.mac[i] = strtol(address, &ptr, 16);
                if (ptr != NULL) {
                        if (ptr[0] != ':') {
				return;
			}
                        address = ptr + 1;
                }
        }
}

static void ble_scan_state_changed_cb(ble_scan_state_e scan_state)
{
	printf("scan callback received\n");
	ITC_FUNC_SIGNAL;
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
}

static void ble_device_disconnected_cb(ble_client_ctx *ctx)
{
	printf("client disconnected callback received\n");
	ITC_FUNC_SIGNAL;
}

static void ble_device_connected_cb(ble_client_ctx *ctx, ble_device_connected *dev)
{
	printf("client connected callback received\n");
	ITC_FUNC_SIGNAL;
}

static void ble_operation_notification_cb(ble_client_ctx *ctx, ble_attr_handle attr_handle, ble_data *read_result)
{
	printf("notification callback received\n");
	ITC_FUNC_SIGNAL;
}

static void ble_server_connected_cb(ble_conn_handle con_handle, ble_server_connection_type_e conn_type, uint8_t mac[BLE_BD_ADDR_MAX_LEN], uint8_t adv_handle)
{
	printf("server connect callback received");
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
	.connected_cb = ble_server_connected_cb,
	.disconnected_cb = NULL,
	.mtu_update_cb = NULL,
	.passkey_display_cb = NULL,
	.pair_bond_cb = NULL,
	.is_secured_connect_allowed = true,
	.profile = gatt_profile,
	.profile_count = sizeof(gatt_profile) / sizeof(ble_server_gatt_t)
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

static double diff_time(struct timeval *x, struct timeval *y)
{
	double x_ms;
	double y_ms;
	double diff;
	x_ms = (double)x->tv_sec * 1000000 + (double)x->tv_usec;
	y_ms = (double)y->tv_sec * 1000000 + (double)y->tv_usec;
	diff = (double)y_ms - (double)x_ms;
	return diff;
}

/*********************************************************************
 * Test Cases
 *********************************************************************/

/**
 * @testcase         itc_blemanager_init_deinit_p
 * @brief            to check initalization and de-initalization of ble manager
 * @scenario         check initalization and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_init_deinit_p(void)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	//as client
	ret = ble_manager_init(NULL);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);
	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	//as server
	ret = ble_manager_init(&server_config);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);
	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_init_deinit_n
 * @brief            to check initalization and de-initalization of ble manager
 * @scenario         check initalization and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_init_deinit_n(void)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(NULL);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);
	ret = ble_manager_init(NULL);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_ALREADY_WORKING);
	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_scan_p
 * @brief            check initalization, scan,and de-initalization of ble manager
 * @scenario         check initalization, scan,and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_client_start_scan, ble_client_stop_scan, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_scan_p(void)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(NULL);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

	//no filter scan
	ret = ble_client_start_scan(NULL, &scan_config);
	TC_ASSERT_EQ_CLEANUP("ble_client_start_scan", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;
	ret = ble_client_stop_scan();
	TC_ASSERT_EQ_CLEANUP("ble_client_stop_scan", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;

	//filter scan
	ble_scan_filter filter = { 0, };
	set_scan_filter(&filter, ble_filter, sizeof(ble_filter), false, 1000);
	ret = ble_client_start_scan(&filter, &scan_config);
	TC_ASSERT_EQ_CLEANUP("ble_client_start_scan", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;
	ret = ble_client_stop_scan();
	TC_ASSERT_EQ_CLEANUP("ble_client_stop_scan", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;

	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_connect_disconnect_p
 * @brief            check initalization, connect, disconnect,and de-initalization of ble manager
 * @scenario         check initalization, connect, disconnect,and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_client_connect, ble_client_disconnect, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_connect_disconnect_p(void)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(NULL);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

	//connect
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, g_target.mac, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 3;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	printf("connecting to mac : %02x:%02x:%02x:%02x:%02x:%02x\n",
		conn_info.addr.mac[0],
		conn_info.addr.mac[1],
		conn_info.addr.mac[2],
		conn_info.addr.mac[3],
		conn_info.addr.mac[4],
		conn_info.addr.mac[5]
	);
	ret = ble_client_connect(ctx, &conn_info);
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;

	//disconnect
	ret = ble_client_disconnect(ctx);
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;

	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_connect_disconnect_n
 * @brief            check initalization, connect, disconnect,and de-initalization of ble manager
 * @scenario         check initalization, connect, disconnect,and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_client_connect, ble_client_disconnect, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_connect_disconnect_n(void)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(NULL);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

	//connect
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, g_target.mac, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 3;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	ret = ble_client_connect(ctx, &conn_info);
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());

	//disconnect
	ret = ble_client_disconnect(ctx);
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ret, BLE_MANAGER_INVALID_STATE, ble_manager_deinit());
	ITC_FUNC_WAIT;

	//disconnect
	ret = ble_client_disconnect(ctx);
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;

	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_bonded_device_p
 * @brief            check initalization, connect,get bonded, delete bonded, disconnect,and de-initalization of ble manager
 * @scenario         check initalization, connect,get bonded, delete bonded, disconnect,and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_client_connect, ble_manager_get_bonded_device, ble_manager_delete_bonded, ble_client_disconnect, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_bonded_device_p(void)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(NULL);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

	//connect
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, g_target.mac, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 3;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	printf("connecting to mac : %02x:%02x:%02x:%02x:%02x:%02x\n",
		conn_info.addr.mac[0],
		conn_info.addr.mac[1],
		conn_info.addr.mac[2],
		conn_info.addr.mac[3],
		conn_info.addr.mac[4],
		conn_info.addr.mac[5]
	);
	ret = ble_client_connect(ctx, &conn_info);
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;

	ble_bonded_device_list dev_list[BLE_MAX_BONDED_DEVICE] = { 0, };
	uint16_t dev_count = 0;
	ret = ble_manager_get_bonded_device(dev_list, &dev_count);
	TC_ASSERT_EQ_CLEANUP("ble_manager_get_bonded_device", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ret = ble_manager_delete_bonded(&conn_info.addr);
	TC_ASSERT_EQ_CLEANUP("ble_manager_delete_bonded", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());

	//disconnect
	ret = ble_client_disconnect(ctx);
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;

	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_client_con_list_p
 * @brief            check initalization, connect, connected_device_list, disconnect,and de-initalization of ble manager
 * @scenario         check initalization, connect, connected_device_list,and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_client_connect, ble_client_connected_device_list, ble_client_disconnect, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_client_con_list_p(void)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(NULL);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

	//connect
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, g_target.mac, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 3;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	printf("connecting to mac : %02x:%02x:%02x:%02x:%02x:%02x\n",
		conn_info.addr.mac[0],
		conn_info.addr.mac[1],
		conn_info.addr.mac[2],
		conn_info.addr.mac[3],
		conn_info.addr.mac[4],
		conn_info.addr.mac[5]
	);
	ret = ble_client_connect(ctx, &conn_info);
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;

	ble_device_connected_list connected_list;
	ret = ble_client_connected_device_list(&connected_list);
	TC_ASSERT_EQ_CLEANUP("ble_client_connected_device_list", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());

	//disconnect
	ret = ble_client_disconnect(ctx);
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;

	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_connected_dev_info_p
 * @brief            check initalization, connect, connected_device_info, disconnect,and de-initalization of ble manager
 * @scenario         check initalization, connect, connected_device_info,and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_client_connect, ble_client_connected_info, ble_client_disconnect, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_connected_dev_info_p(void)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(NULL);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

	//connect
	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, g_target.mac, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 3;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;
	printf("connecting to mac : %02x:%02x:%02x:%02x:%02x:%02x\n",
		conn_info.addr.mac[0],
		conn_info.addr.mac[1],
		conn_info.addr.mac[2],
		conn_info.addr.mac[3],
		conn_info.addr.mac[4],
		conn_info.addr.mac[5]
	);
	ret = ble_client_connect(ctx, &conn_info);
	TC_ASSERT_EQ_CLEANUP("ble_client_connect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;

	ble_device_connected connected_dev;
	ret = ble_client_connected_info(ctx, &connected_dev);
	TC_ASSERT_EQ_CLEANUP("ble_client_connected_info", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());

	//disconnect
	ret = ble_client_disconnect(ctx);
	TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ITC_FUNC_WAIT;

	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_advertisement_p
 * @brief            check initalization, set data, resp, type, interval, start adv, stop adv,and de-initalization of ble manager
 * @scenario         check initalization, set data, resp, type, interval, start adv, stop adv,and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_server_set_adv_data, ble_server_set_adv_type, ble_server_set_adv_interval, ble_server_start_adv, ble_server_stop_adv, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_advertisement_p(void)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(&server_config);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

	//set adv data
	ble_data data[1] = { 0, };
	data->data = g_adv_raw;
	data->length = sizeof(g_adv_raw);
	ret = ble_server_set_adv_data(data);
	TC_ASSERT_EQ_CLEANUP("ble_server_set_adv_data", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	//set adv resp
	data->data = g_adv_resp;
	data->length = sizeof(g_adv_resp);
	ret = ble_server_set_adv_data(data);
	TC_ASSERT_EQ_CLEANUP("ble_server_set_adv_data", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	//set adv type
	ble_adv_type_e type = BLE_ADV_TYPE_IND;
	uint8_t mac[BLE_BD_ADDR_MAX_LEN];
	ble_addr addr = {
		{
			*mac,
			type,
		}
	};
	ret = ble_server_set_adv_type(type, &addr);
	TC_ASSERT_EQ_CLEANUP("ble_server_set_adv_type", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	//set adv interval
	unsigned int interval = 10;
	ret = ble_server_set_adv_interval(interval);
	TC_ASSERT_EQ_CLEANUP("ble_server_set_adv_interval", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	//start adv
	ret = ble_server_start_adv();
	TC_ASSERT_EQ_CLEANUP("ble_server_start_adv", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	//stop adv
	ret = ble_server_stop_adv();
	TC_ASSERT_EQ_CLEANUP("ble_server_stop_adv", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());

	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_advertisement_n
 * @brief            check initalization, set data, resp, type, interval, start adv, stop adv,and de-initalization of ble manager
 * @scenario         check initalization, set data, resp, type, interval, start adv, stop adv,and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_server_set_adv_data, ble_server_set_adv_type, ble_server_set_adv_interval, ble_server_start_adv, ble_server_stop_adv, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_readvertisement_p(void)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(&server_config);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

	//set adv data
	ble_data data[1] = { 0, };
	data->data = g_adv_raw;
	data->length = sizeof(g_adv_raw);
	ret = ble_server_set_adv_data(data);
	TC_ASSERT_EQ_CLEANUP("ble_server_set_adv_data", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	//set adv resp
	data->data = g_adv_resp;
	data->length = sizeof(g_adv_resp);
	ret = ble_server_set_adv_data(data);
	TC_ASSERT_EQ_CLEANUP("ble_server_set_adv_data", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	//set adv type
	ble_adv_type_e type = BLE_ADV_TYPE_IND;
	uint8_t mac[BLE_BD_ADDR_MAX_LEN];
	ble_addr addr = {
		{
			*mac,
			type,
		}
	};
	ret = ble_server_set_adv_type(type, &addr);
	TC_ASSERT_EQ_CLEANUP("ble_server_set_adv_type", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	//set adv interval
	unsigned int interval = 10;
	ret = ble_server_set_adv_interval(interval);
	TC_ASSERT_EQ_CLEANUP("ble_server_set_adv_interval", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	//start adv
	ret = ble_server_start_adv();
	TC_ASSERT_EQ_CLEANUP("ble_server_start_adv", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	//start adv
	ret = ble_server_start_adv();
	TC_ASSERT_EQ_CLEANUP("ble_server_start_adv", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	//stop adv
	ret = ble_server_stop_adv();
	TC_ASSERT_EQ_CLEANUP("ble_server_stop_adv", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());

	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_whitelist_add_del_p
 * @brief            check initalization, whitelist add, whitelist delete,and de-initalization of ble manager
 * @scenario         check initalization, whitelist add, whitelist delete,and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_scan_whitelist_add, ble_scan_whitelist_delete, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_whitelist_add_del_p(void)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(NULL);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

	ble_addr addr;
	memcpy(addr.mac, g_target.mac, BLE_BD_ADDR_MAX_LEN);
	ret = ble_scan_whitelist_add(&addr);
	TC_ASSERT_EQ_CLEANUP("ble_scan_whitelist_add", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
	ret = ble_scan_whitelist_delete(&addr);
	TC_ASSERT_EQ_CLEANUP("ble_scan_whitelist_delete", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());

	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_get_profile_count_p
 * @brief            check initalization, profile count, and de-initalization of ble manager
 * @scenario         check initalization, profile count, and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_server_get_profile_count, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_get_profile_count_p(void) {
	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(&server_config);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

	uint16_t count;
	ret = ble_server_get_profile_count(&count);
	TC_ASSERT_EQ_CLEANUP("ble_server_get_profile_count", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());

	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_average_connect_time
 * @brief            check average time taken to connect to a server
 * @scenario         check initalization, connect, disconnect,and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_client_connect, ble_client_disconnect, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_average_connect_time(void)
{
	int i = 0;
	double average = 0.0f;
	struct timeval start_t;
	struct timeval end_t;

	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(NULL);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, g_target.mac, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 3;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;

	for (i = 1; i <= LOOP_SIZE; i++) {
		gettimeofday(&start_t, NULL);
		ret = ble_client_connect(ctx, &conn_info);
		TC_ASSERT_EQ_CLEANUP("ble_client_connect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
		ITC_FUNC_WAIT;
		gettimeofday(&end_t, NULL);
		average += diff_time(&start_t, &end_t);

		ret = ble_client_disconnect(ctx);
		TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
		ITC_FUNC_WAIT;
		sleep(1);
	}

	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	printf("\n[Average connect time: %.2f ms]\n", average / (LOOP_SIZE * 1000));
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_average_disconnect_time
 * @brief            check average time taken to disconnect from a server
 * @scenario         check initalization, connect, disconnect,and de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_client_connect, ble_client_disconnect, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_average_disconnect_time(void)
{
	int i = 0;
	double average = 0.0f;
	struct timeval start_t;
	struct timeval end_t;

	ble_result_e ret = BLE_MANAGER_FAIL;
	ret = ble_manager_init(NULL);
	TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

	ble_client_ctx *ctx;
	ctx = ble_client_create_ctx(&client_config);
	ble_conn_info conn_info = { 0, };
	memcpy(conn_info.addr.mac, g_target.mac, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = BLE_ADV_TYPE_IND;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 3;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;

	for (i = 1; i <= LOOP_SIZE; i++) {
		ret = ble_client_connect(ctx, &conn_info);
		TC_ASSERT_EQ_CLEANUP("ble_client_connect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
		ITC_FUNC_WAIT;

		gettimeofday(&start_t, NULL);
		ret = ble_client_disconnect(ctx);
		TC_ASSERT_EQ_CLEANUP("ble_client_disconnect", ret, BLE_MANAGER_SUCCESS, ble_manager_deinit());
		ITC_FUNC_WAIT;
		gettimeofday(&end_t, NULL);
		average += diff_time(&start_t, &end_t);
	}

	ret = ble_manager_deinit();
	TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
	printf("\n[Average disconnect time: %.2f ms]\n", average / (LOOP_SIZE * 1000));
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_blemanager_average_stoping_time
 * @brief            check average time taken to deinit
 * @scenario         check initalization, de-initalization of ble manager
 * @apicovered       ble_manager_init, ble_manager_deinit
 * @precondition     none
 * @postcondition    none
 */
static void itc_blemanager_average_stoping_time(void)
{
	int i = 0;
	double average = 0.0f;
	struct timeval start_t;
	struct timeval end_t;

	ble_result_e ret = BLE_MANAGER_FAIL;

	for (i = 1; i <= LOOP_SIZE; i++) {
		ret = ble_manager_init(NULL);
		TC_ASSERT_EQ("ble_manager_init", ret, BLE_MANAGER_SUCCESS);

		gettimeofday(&start_t, NULL);
		ret = ble_manager_deinit();
		TC_ASSERT_EQ("ble_manager deinit", ret, BLE_MANAGER_SUCCESS);
		gettimeofday(&end_t, NULL);
		average += diff_time(&start_t, &end_t);
	}

	printf("\n[Average deinit time: %.2f ms]\n", average / (LOOP_SIZE * 1000));
	TC_SUCCESS_RESULT();
}

int blemanager_itc(int argc, FAR char *argv[])
{
	if (testcase_state_handler(TC_START, "BLE Manager ITC") == ERROR) {
		return ERROR;
	}

	addr_string_to_mac(argv[1]);

	//init_deinit
	itc_blemanager_init_deinit_p();
	itc_blemanager_init_deinit_n();

	//scan
	itc_blemanager_scan_p();

	//whitelist
	itc_blemanager_whitelist_add_del_p();

	//connect_disconnect
	itc_blemanager_connect_disconnect_p();
	itc_blemanager_connect_disconnect_n();

	//connection info
	itc_blemanager_client_con_list_p();
	itc_blemanager_connected_dev_info_p();

	//bonded device
	itc_blemanager_bonded_device_p();

	//advertisement
	itc_blemanager_advertisement_p();
	itc_blemanager_readvertisement_p();

	//profile count
	itc_blemanager_get_profile_count_p();

	//average time
	itc_blemanager_average_connect_time();
	itc_blemanager_average_disconnect_time();
	itc_blemanager_average_stoping_time();

	(void)testcase_state_handler(TC_END, "BLEManager ITC");
	return 0;
}

int itc_blemanager_main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Please provide mac addr of BLE server to run ITC against\n");
		return ERROR;
	}

	int res = sem_init(&g_func_sem, 0, 0);
	if (res != 0) {
		printf("g_func_sem init failed\n");
		return ERROR;
	}

	blemanager_itc(argc, argv);

	sem_destroy(&g_func_sem);

	return 0;
}
