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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/clock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ble_manager/ble_manager.h>
#include <semaphore.h>
#include <errno.h>

#define RMC_TAG "\x1b[33m[RMC]\x1b[0m"
#define RMC_CLIENT_TAG "\x1b[32m[RMC CLIENT]\x1b[0m"
#define RMC_SERVER_TAG "\x1b[36m[RMC SERVER]\x1b[0m"
#define RMC_LOG(tag, fmt, args...) printf(tag fmt, ##args)
#define RMC_MAX_CONNECTION 3

static int g_scan_done = 0;
static int g_scan_state = -1;
static ble_addr g_target = { 0, };
static ble_client_ctx *ctx_list[RMC_MAX_CONNECTION] = { 0, };
static int ctx_count = 0;

static char *client_state_str[] = {
	"\x1b[35mNONE\x1b[0m",
	"\x1b[35mIDLE\x1b[0m",
	"\x1b[35mCONNECTED\x1b[0m",
	"\x1b[35mCONNECTING\x1b[0m",
	"\x1b[35mDISCONNECTING\x1b[0m",
	"\x1b[35mAUTO-CONNECTING\x1b[0m",
};

static char *__client_state_str(ble_client_state_e state)
{
	return client_state_str[state];
}

static void ble_scan_state_changed_cb(ble_scan_state_e scan_state)
{
	RMC_LOG(RMC_CLIENT_TAG, "'%s' is called[%d]\n", __FUNCTION__, scan_state);
	if (scan_state == BLE_SCAN_STOPPED) {
		g_scan_state = 0;
	} else if (scan_state == BLE_SCAN_STARTED) {
		g_scan_state = 1;
	}
	return;
}

/* These values can be modified as a developer wants. */
static uint8_t ble_filter[] = { 0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18 };

static uint8_t g_adv_raw[] = { 
	0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18 
};
static uint8_t g_adv_resp[] = {
	0x11, 0x09, 'T', 'I', 'Z', 'E', 'N', 'R', 'T', ' ', 'T', 'E', 'S', 'T', '(', '0', '2', ')',
};

static void ble_device_scanned_cb_for_test(ble_scanned_device *scanned_device)
{
	RMC_LOG(RMC_CLIENT_TAG, "scanned mac : %02x:%02x:%02x:%02x:%02x:%02x\n", 
		scanned_device->addr.mac[0],
		scanned_device->addr.mac[1],
		scanned_device->addr.mac[2],
		scanned_device->addr.mac[3],
		scanned_device->addr.mac[4],
		scanned_device->addr.mac[5]
	);
}

static void ble_device_scanned_cb_for_connect(ble_scanned_device *scanned_device)
{
	if (g_scan_done == 1) {
		return;
	}

	RMC_LOG(RMC_CLIENT_TAG, "Found mac : %02x:%02x:%02x:%02x:%02x:%02x\n", 
		scanned_device->addr.mac[0],
		scanned_device->addr.mac[1],
		scanned_device->addr.mac[2],
		scanned_device->addr.mac[3],
		scanned_device->addr.mac[4],
		scanned_device->addr.mac[5]
	);

	if (g_scan_done == 0) {
		memcpy(g_target.mac, scanned_device->addr.mac, BLE_BD_ADDR_MAX_LEN);
		g_target.type = scanned_device->addr.type;
		g_scan_done = 1;
	}
}

static void ble_device_disconnected_cb(ble_client_ctx *ctx)
{
	RMC_LOG(RMC_CLIENT_TAG, "'%s' is called[ID : %d]\n", __FUNCTION__, ctx->conn_handle);
	return;
}

static void ble_device_connected_cb(ble_client_ctx *ctx, ble_device_connected *dev)
{
	RMC_LOG(RMC_CLIENT_TAG, "'%s' is called[%p]\n", __FUNCTION__, ctx);

	RMC_LOG(RMC_CLIENT_TAG, "Conn Handle : %d\n", dev->conn_handle);
	RMC_LOG(RMC_CLIENT_TAG, "Bonded : %d / CI : %d / SL : %d / MTU : %d\n", 
		dev->is_bonded,
		dev->conn_info.conn_interval,
		dev->conn_info.slave_latency,
		dev->conn_info.mtu
	);
	RMC_LOG(RMC_CLIENT_TAG, "Conn MAC : %02x:%02x:%02x:%02x:%02x:%02x\n", 
		dev->conn_info.addr.mac[0],
		dev->conn_info.addr.mac[1],
		dev->conn_info.addr.mac[2],
		dev->conn_info.addr.mac[3],
		dev->conn_info.addr.mac[4],
		dev->conn_info.addr.mac[5]
	);

	return;
}

static void ble_operation_notification_cb(ble_client_ctx *ctx, ble_attr_handle attr_handle, ble_data *read_result)
{
	RMC_LOG(RMC_CLIENT_TAG, "'%s' is called[%p]\n", __FUNCTION__, ctx);
	printf("attr : %x // len : %d\n", attr_handle, read_result->length);
	if (read_result->length > 0) {
		printf("read : ");
		int i;
		for (i = 0; i < read_result->length; i++) {
			printf("%02x ", read_result->data[i]);
		}
		printf("\n");
	}
	return;
}

void restart_server(void) {
	ble_result_e ret = BLE_MANAGER_FAIL;
	ble_data data[1] = { 0, };
	data->data = g_adv_raw;
	data->length = sizeof(g_adv_raw);

	ret = ble_server_set_adv_data(data);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_SERVER_TAG, "Fail to set adv raw data ret:[%d]\n");
		return;
	}
	RMC_LOG(RMC_SERVER_TAG, "Set adv raw data ... ok\n");

	data->data = g_adv_resp;
	data->length = sizeof(g_adv_resp);

	ret = ble_server_set_adv_resp(data);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_SERVER_TAG, "Fail to set adv resp data ret:[%d]\n");
		return;
	}
	RMC_LOG(RMC_SERVER_TAG, "Set adv resp data ... ok\n");

	ret = ble_server_start_adv();
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_SERVER_TAG, "Fail to start adv ret:[%d]\n");
		return;
	}
	RMC_LOG(RMC_SERVER_TAG, "Start adv ... ok\n");
}

static void ble_server_connected_cb(ble_conn_handle con_handle, ble_server_connection_type_e conn_type, uint8_t mac[BLE_BD_ADDR_MAX_LEN])
{
	RMC_LOG(RMC_SERVER_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_SERVER_TAG, "conn : %d / conn_type : %d\n", con_handle, conn_type);
	RMC_LOG(RMC_SERVER_TAG, "conn mac : %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	if (conn_type == BLE_SERVER_DISCONNECTED) {
		restart_server();
	}
	return;
}

static void ble_server_mtu_update_cb(ble_conn_handle con_handle, uint16_t mtu_size)
{
	RMC_LOG(RMC_SERVER_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_SERVER_TAG, "conn : %d\n", con_handle);
	RMC_LOG(RMC_SERVER_TAG, "mtu_size : %d\n", mtu_size);
	return;
}

static void utc_cb_charact_a_1(ble_server_attr_cb_type_e type, ble_conn_handle conn_handle, ble_attr_handle attr_handle, void *arg)
{
	char *arg_str = "None";
	if (arg != NULL) {
		arg_str = (char *)arg;
	}
	RMC_LOG(RMC_SERVER_TAG, "[CHAR_A_1][%s] type : %d / handle : %d / attr : %02x \n", arg_str, type, conn_handle, attr_handle);
}

static void utc_cb_desc_b_1(ble_server_attr_cb_type_e type, ble_conn_handle conn_handle, ble_attr_handle attr_handle, void *arg)
{
	char *arg_str = "None";
	if (arg != NULL) {
		arg_str = (char *)arg;
	}
	RMC_LOG(RMC_SERVER_TAG, "[DESC_A_1][%s] type : %d / handle : %d / attr : %02x \n", arg_str, type, conn_handle, attr_handle);
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
		.cb = utc_cb_charact_a_1, 
		.arg = "char_a_1"
	},

	{
		.type = BLE_SERVER_GATT_DESC, 
		.uuid = {0x02, 0x29}, 
		.uuid_length = 2, 
		.permission = BLE_ATTR_PERM_R_PERMIT | BLE_ATTR_PERM_W_PERMIT, 
		.attr_handle = 0x006c, 
		.cb = utc_cb_desc_b_1, 
		.arg = "desc_b_1",
	},
};

static ble_scan_callback_list scan_config = {
	ble_scan_state_changed_cb,
	NULL,
};

static ble_client_callback_list client_config = {
	ble_device_disconnected_cb,
	ble_device_connected_cb,
	ble_operation_notification_cb,
};

static ble_server_init_config server_config = {
	ble_server_connected_cb,
	ble_server_mtu_update_cb,
	true,
	gatt_profile, sizeof(gatt_profile) / sizeof(ble_server_gatt_t)};

static int ble_connect_common(ble_client_ctx *ctx, ble_addr *addr, bool is_auto)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	ble_attr_handle attr_handle;
	ble_client_state_e cli_state = BLE_CLIENT_NONE;
	ble_conn_info conn_info = { 0, };

	memcpy(conn_info.addr.mac, addr->mac, BLE_BD_ADDR_MAX_LEN);
	conn_info.addr.type = addr->type;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 128;
	conn_info.mtu = 240;
	conn_info.scan_timeout = 1000;
	conn_info.is_secured_connect = true;

	if (ctx == NULL) {
		RMC_LOG(RMC_CLIENT_TAG, "ctx fail\n");
		return -1;
	}

	ret = ble_client_autoconnect(ctx, is_auto);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_CLIENT_TAG, "fail to set autoconnect=%d [%d]\n", is_auto, ret);
	}

	cli_state = ble_client_get_state(ctx);
	RMC_LOG(RMC_CLIENT_TAG, "Client State [ %s ]\n", __client_state_str(cli_state));

	ret = ble_client_connect(ctx, &conn_info);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_CLIENT_TAG, "connect fail[%d]\n", ret);
		return -2;
	}

	cli_state = ble_client_get_state(ctx);
	RMC_LOG(RMC_CLIENT_TAG, "Client State [ %s ]\n", __client_state_str(cli_state));

	int wait_time = 10; // Wait 10 seconds
	int count = wait_time * 4;
	while (count--) {
		cli_state = ble_client_get_state(ctx);
		if (cli_state == BLE_CLIENT_CONNECTED) {
			RMC_LOG(RMC_CLIENT_TAG, "BLE is connected\n");
			break;
		} else if (cli_state == BLE_CLIENT_IDLE) {
			RMC_LOG(RMC_CLIENT_TAG, "BLE is not connected");
			return -3;
		}

		usleep(250 * 1000);
	}
	RMC_LOG(RMC_CLIENT_TAG, "Client State [ %s ]\n", __client_state_str(cli_state));

	attr_handle = 0xff03;
	ret = ble_client_operation_enable_notification(ctx, attr_handle);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_CLIENT_TAG, "Fail to enable noti handle1[%d]\n", ret);
	} else {
		RMC_LOG(RMC_CLIENT_TAG, "Success to enable noti handle1.\n");
	}

	attr_handle = 0x006e;
	ret = ble_client_operation_enable_notification(ctx, attr_handle);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_CLIENT_TAG, "Fail to enable noti handle2[%d]\n", ret);
	} else {
		RMC_LOG(RMC_CLIENT_TAG, "Success to enable noti handle2.\n");
	}

	return 0;
}

static void set_scan_timer(uint32_t *scan_time, char *data)
{
	int temp = atoi(data);
	if (temp < 0) {
		RMC_LOG(RMC_CLIENT_TAG, "Fail to set timer\n");
	} else {
		*scan_time = (uint32_t)temp;
	}
}

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

/****************************************************************************
 * ble_rmc_main
 ****************************************************************************/
int ble_rmc_main(int argc, char *argv[])
{
	RMC_LOG(RMC_TAG, "- BLE Remote Test -\n");

	ble_result_e ret = BLE_MANAGER_FAIL;

	if (argc < 2) {
		return 0;
	}

	RMC_LOG(RMC_TAG, "cmd : %s\n", argv[1]);

	if (strncmp(argv[1], "init", 5) == 0) {
		if (argc == 3 && strncmp(argv[2], "null", 5) == 0) {
			ret = ble_manager_init(NULL);
			if (ret != BLE_MANAGER_SUCCESS) {
				if (ret != BLE_MANAGER_ALREADY_WORKING) {
					RMC_LOG(RMC_CLIENT_TAG, "init with null fail[%d]\n", ret);
					goto ble_rmc_done;
				}
				RMC_LOG(RMC_CLIENT_TAG, "init is already done\n");
			} else {
				RMC_LOG(RMC_CLIENT_TAG, "init with NULL done[%d]\n", ret);
			}
		} else {
			ret = ble_manager_init(&server_config);
			if (ret != BLE_MANAGER_SUCCESS) {
				if (ret != BLE_MANAGER_ALREADY_WORKING) {
					RMC_LOG(RMC_CLIENT_TAG, "init fail[%d]\n", ret);
					goto ble_rmc_done;
				}
				RMC_LOG(RMC_CLIENT_TAG, "init is already done\n");
			} else {
				RMC_LOG(RMC_CLIENT_TAG, "init with config done[%d]\n", ret);
			}
		}
	}

	if (strncmp(argv[1], "version", 8) == 0) {
		uint8_t version[3] = { 0, };
		ret = ble_manager_get_version(version);
		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_TAG, "Fail to get BLE version[%d]\n", ret);
		} else {
			RMC_LOG(RMC_TAG, "BLE Version : %02x %02x %02x\n", version[0], version[1], version[2]);
		}
	}

	if (strncmp(argv[1], "state", 6) == 0) {
		if (argc < 3) {
			goto ble_rmc_done;
		}
		int id = atoi(argv[2]);
		RMC_LOG(RMC_CLIENT_TAG, "Client State [ %s ]\n", __client_state_str(ble_client_get_state(ctx_list[id])));
	}

	if (strncmp(argv[1], "deinit", 7) == 0) {
		ret = ble_manager_deinit();
		RMC_LOG(RMC_CLIENT_TAG, "deinit done[%d]\n", ret);
	}

	if (strncmp(argv[1], "reconn", 7) == 0) {
		RMC_LOG(RMC_CLIENT_TAG, "== Try Auto Connect ==\n");

		ble_bonded_device_list dev_list[BLE_MAX_BONDED_DEVICE] = { 0, };
		uint16_t dev_count = 0;
		ble_addr *addr;
		ble_client_ctx *ctx;

		ret = ble_manager_get_bonded_device(dev_list, &dev_count);
		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_CLIENT_TAG, "Fail to get bond data[%d]\n", ret);
			goto ble_rmc_done;
		}
		
		RMC_LOG(RMC_CLIENT_TAG, "Bonded Dev Num : %d\n", dev_count);
		if (dev_count > 0) {
			addr = &(dev_list[0].bd_addr);
			RMC_LOG(RMC_CLIENT_TAG, "Bond[%d] %02x:%02x:%02x:%02x:%02x:%02x\n", addr->type, 
				addr->mac[0], addr->mac[1], addr->mac[2], addr->mac[3], addr->mac[4], addr->mac[5]);
		} else {
			RMC_LOG(RMC_CLIENT_TAG, "There is no bonded data.");
		}

		ctx = ble_client_create_ctx(&client_config);
		if (ctx == NULL) {
			RMC_LOG(RMC_CLIENT_TAG, "create ctx fail\n");
			goto ble_rmc_done;
		}

		int val;
		if (argc == 3 && strncmp(argv[2], "auto", 5) == 0) {
			val = ble_connect_common(ctx, addr, true);
		} else {
			val = ble_connect_common(ctx, addr, false);
		}
		RMC_LOG(RMC_CLIENT_TAG, "Re-Connect Result : %d\n", val);
		if (val == 0) {
			RMC_LOG(RMC_CLIENT_TAG, "Re-Connect Success [ID : %d]\n", ctx_count);
			ctx_list[ctx_count++] = ctx;
		}
	}

	if (strncmp(argv[1], "bond", 5) == 0) {
		if (argc == 3) {
			if (strncmp(argv[2], "list", 5) == 0) {
				RMC_LOG(RMC_CLIENT_TAG, "== BLE Bonded List ==\n");

				ble_bonded_device_list dev_list[BLE_MAX_BONDED_DEVICE] = { 0, };
				uint16_t dev_count = 0;
				uint8_t *mac;

				ret = ble_manager_get_bonded_device(dev_list, &dev_count);

				RMC_LOG(RMC_CLIENT_TAG, "Bonded Dev : %d\n", dev_count);
				
				for (int i = 0; i < dev_count; i++) {
					mac = dev_list[i].bd_addr.mac;
					RMC_LOG(RMC_CLIENT_TAG, "DEV#%d[%d] : %02x:%02x:%02x:%02x:%02x:%02x\n", i + 1, dev_list[i].bd_addr.type, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
				}

			} else if (strncmp(argv[2], "clear", 6) == 0) {
				ret = ble_manager_delete_bonded_all();
				if (ret != BLE_MANAGER_SUCCESS) {
					RMC_LOG(RMC_CLIENT_TAG, "fail to delete all of bond dev[%d]\n", ret);
				} else {
					RMC_LOG(RMC_CLIENT_TAG, "success to delete all of bond dev\n");
				}
			}
		}

		if (argc == 4 && strncmp(argv[2], "del", 4) == 0) {
			int cnt = 0;
			ble_addr addr[1] = { 0, };
			uint8_t *mac = addr->mac;

			char *ptr = strtok(argv[3], ":");
			while (ptr != NULL) {
				mac[cnt++] = strtol(ptr, NULL, 16);
				ptr = strtok(NULL, ":");
			}
			RMC_LOG(RMC_CLIENT_TAG, "TARGET : %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
			ret = ble_manager_delete_bonded(addr);
			if (ret == BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "success to delete bond dev\n");
			} else if (ret == BLE_MANAGER_NOT_FOUND) {
				RMC_LOG(RMC_CLIENT_TAG, "[%02x:%02x:%02x:%02x:%02x:%02x] is not found\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
			} else {
				RMC_LOG(RMC_CLIENT_TAG, "fail to delete bond dev[%d]\n", ret);
			}
		}
		RMC_LOG(RMC_CLIENT_TAG, "bond command done.\n");
	}

	if (strncmp(argv[1], "mac", 4) == 0) {
		uint8_t mac[BLE_BD_ADDR_MAX_LEN];
		int i;

		ret = ble_manager_get_mac_addr(mac);

		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_CLIENT_TAG, "get mac fail[%d]\n", ret);
			goto ble_rmc_done;
		}

		RMC_LOG(RMC_CLIENT_TAG, "BLE mac : %02x", mac[0]);
		for (i = 1; i < BLE_BD_ADDR_MAX_LEN; i++) {
			printf(":%02x", mac[i]);
		}
		printf("\n");
	}

	if (strncmp(argv[1], "whitelist", 10) == 0) {
		if (argc == 4 && strncmp(argv[2], "add", 4) == 0) {
			ble_addr addr[1] = { 0, };
			int count = 0;

			count = sscanf(argv[3], "%02x:%02x:%02x:%02x:%02x:%02x", &addr->mac[0], &addr->mac[1], &addr->mac[2],
				&addr->mac[3], &addr->mac[4], &addr->mac[5]);
			if (count != BLE_BD_ADDR_MAX_LEN) {
				RMC_LOG(RMC_CLIENT_TAG, "Fail to read MAC[%d]\n", count);
				goto ble_rmc_done;
			}

			RMC_LOG(RMC_CLIENT_TAG, "Input Mac[%d] : %02x:%02x:%02x:%02x:%02x:%02x\n", addr->type, addr->mac[0], 
				addr->mac[1], addr->mac[2], addr->mac[3], addr->mac[4], addr->mac[5]);

			ret = ble_scan_whitelist_add(addr);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "Add whitelist fail[%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_CLIENT_TAG, "Add whitelist Success\n");
		} else if (argc == 4 && strncmp(argv[2], "del", 4) == 0) {
			ble_addr addr[1] = { 0, };
			int count = 0;

			count = sscanf(argv[3], "%02x:%02x:%02x:%02x:%02x:%02x", &addr->mac[0], &addr->mac[1], &addr->mac[2],
				&addr->mac[3], &addr->mac[4], &addr->mac[5]);
			if (count != BLE_BD_ADDR_MAX_LEN) {
				RMC_LOG(RMC_CLIENT_TAG, "Fail to read MAC[%d]\n", count);
				goto ble_rmc_done;
			}

			RMC_LOG(RMC_CLIENT_TAG, "Input Mac[%d] : %02x:%02x:%02x:%02x:%02x:%02x\n", addr->type, addr->mac[0], 
				addr->mac[1], addr->mac[2], addr->mac[3], addr->mac[4], addr->mac[5]);

			ret = ble_scan_whitelist_delete(addr);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "Del whitelist fail[%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_CLIENT_TAG, "Del whitelist Success\n");
		} else if (argc == 3 && strncmp(argv[2], "clear", 6) == 0) {
			ret = ble_scan_whitelist_clear_all();
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "Clear whitelist fail[%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_CLIENT_TAG, "Clear whitelist Success\n");
		} else if (argc == 3 && strncmp(argv[2], "list", 5) == 0) {
			ble_addr addr_list[10] = { 0, };
			ble_addr *addr;
			uint16_t count = 0;
			int i;
			count = ble_scan_whitelist_list(addr_list, 10);

			RMC_LOG(RMC_CLIENT_TAG, "Total List : %u\n", count);
			for (i = 0; i < count; i++) {
				addr = &addr_list[i];
				RMC_LOG(RMC_CLIENT_TAG, "#%d Mac[%d] : %02x:%02x:%02x:%02x:%02x:%02x\n", i+1, addr->type, addr->mac[0], 
				addr->mac[1], addr->mac[2], addr->mac[3], addr->mac[4], addr->mac[5]);
			}

		} else {
			RMC_LOG(RMC_CLIENT_TAG, "No whitelist command\n");
			goto ble_rmc_done;
		}
	}

	/* 
	* [ Scan ] Usage :
	* 1. Normal Scan with MAX Scan Timeout
	* TASH>> ble_rmc scan 1
	* 2. Whitelist Scan
	* TASH>> ble_rmc scan 2 [timer_value]
	* ( timer_value : optional. this should be in seconds, default : 5s )
	* 3. Filter Scan
	* TASH>> ble_rmc scan 3 [timer_value]
	* ( timer_value : optional. this should be in seconds, default : 5s )
	* 4. Stop Scan
	* TASH>> ble_rmc scan
	*/
	if (strncmp(argv[1], "scan", 5) == 0) {
		if (argc >= 3 && strncmp(argv[2], "1", 2) == 0) {
			RMC_LOG(RMC_CLIENT_TAG, "Scan Start without filter !\n");
			scan_config.device_scanned_cb = ble_device_scanned_cb_for_test;
			ret = ble_client_start_scan(NULL, &scan_config);

			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "scan start fail[%d]\n", ret);
				goto ble_rmc_done;
			}
		} else if (argc >= 3 && strncmp(argv[2], "2", 2) == 0) {
			RMC_LOG(RMC_CLIENT_TAG, "Scan Start with WhiteList!\n");

			uint32_t scan_time = 5; // Seconds
			if (argc == 4) {
				set_scan_timer(&scan_time, argv[3]);
			}
			RMC_LOG(RMC_CLIENT_TAG, "Timer : %us\n", scan_time);

			ble_scan_filter filter = { 0, };
			set_scan_filter(&filter, NULL, 0, true, scan_time * 1000);
			scan_config.device_scanned_cb = ble_device_scanned_cb_for_test;
			ret = ble_client_start_scan(&filter, &scan_config);

			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "scan start fail[%d]\n", ret);
				goto ble_rmc_done;
			}
		} else if (argc >= 3 && strncmp(argv[2], "3", 2) == 0) {
			RMC_LOG(RMC_CLIENT_TAG, "Scan Start with Packet Filter!\n");

			uint32_t scan_time = 5; // Seconds
			if (argc == 4) {
				set_scan_timer(&scan_time, argv[3]);
			}
			RMC_LOG(RMC_CLIENT_TAG, "Timer : %us\n", scan_time);

			ble_scan_filter filter = { 0, };
			set_scan_filter(&filter, ble_filter, sizeof(ble_filter), false, scan_time * 1000);
			scan_config.device_scanned_cb = ble_device_scanned_cb_for_test;
			ret = ble_client_start_scan(&filter, &scan_config);

			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "scan start fail[%d]\n", ret);
				goto ble_rmc_done;
			}
		} else {
			printf("stop !\n");
			ret = ble_client_stop_scan();

			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "scan stop fail[%d]\n", ret);
				goto ble_rmc_done;
			}
		}
	}

	if (strncmp(argv[1], "disconn", 8) == 0) {
		if (argc < 3) {
			goto ble_rmc_done;
		}
		int id = atoi(argv[2]);

		ret = ble_client_disconnect(ctx_list[id]);
		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_CLIENT_TAG, "disconnect fail[%d]\n", ret);
			goto ble_rmc_done;
		}
		RMC_LOG(RMC_CLIENT_TAG, "disconnect ok\n");
	}

	if (strncmp(argv[1], "stop", 5) == 0) {
		if (argc == 4 && strncmp(argv[2], "auto", 5) == 0) {
			int id = atoi(argv[3]);
			ret = ble_client_autoconnect(ctx_list[id], false);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "Stop auto connection fail[%d]\n", ret);
				goto ble_rmc_done;
			}
		}
	}

	if (strncmp(argv[1], "connect", 8) == 0) {
		ble_client_ctx *ctx = NULL;
		
		/*
		1. scan
		2. delete bond
		3. create ctx
		4. connect
		*/

		// 1. scan & delete bond
		if (g_scan_state == 1) {
			RMC_LOG(RMC_CLIENT_TAG, "Scan is running\n");
			goto ble_rmc_done;
		}
		g_scan_state = -1;

		if (argc == 3 && strncmp(argv[2], "fail", 5) == 0) {
			memset(g_target.mac, 1, BLE_BD_ADDR_MAX_LEN);
			g_target.type = BLE_ADDR_TYPE_PUBLIC;
		} else {
			ble_scan_filter filter = { 0, };
			set_scan_filter(&filter, ble_filter, sizeof(ble_filter), false, 1500);
			scan_config.device_scanned_cb = ble_device_scanned_cb_for_connect;
			g_scan_done = 0;
			ret = ble_client_start_scan(&filter, &scan_config);

			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "scan start fail[%d]\n", ret);
				goto ble_rmc_done;
			}

			while (1) {
				if (g_scan_state == 0) {
					break;
				}
				usleep(100 * 1000);
			}
			
			if (g_scan_done == 0) {
				RMC_LOG(RMC_CLIENT_TAG, "No target device\n");
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_CLIENT_TAG, "Found device!\n");

			ret = ble_manager_delete_bonded_all();
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "fail to delete bond dev[%d]\n", ret);
			} else {
				RMC_LOG(RMC_CLIENT_TAG, "success to delete bond dev\n");
			}
		}

		// 3. create ctx
		ctx = ble_client_create_ctx(&client_config);
		if (ctx == NULL) {
			RMC_LOG(RMC_CLIENT_TAG, "create ctx fail\n");
			goto ble_rmc_done;
		}

		RMC_LOG(RMC_CLIENT_TAG, "Try to connect! [%02x:%02x:%02x:%02x:%02x:%02x]\n", 
			g_target.mac[0],
			g_target.mac[1],
			g_target.mac[2],
			g_target.mac[3],
			g_target.mac[4],
			g_target.mac[5]
		);

		int val;
		if (argc == 3 && strncmp(argv[2], "auto", 5) == 0) {
			/* For initial connection, remove bonded data all */
			val = ble_connect_common(ctx, &g_target, true);
		} else {
			val = ble_connect_common(ctx, &g_target, false);
		}
		RMC_LOG(RMC_CLIENT_TAG, "Connect Result : %d\n", val);
		if (val == 0) {
			RMC_LOG(RMC_CLIENT_TAG, "Connect Success [ID : %d]\n", ctx_count);
			ctx_list[ctx_count++] = ctx;
		}
	}


	/* Server Test */
	if (strncmp(argv[1], "server", 7) == 0) {
		RMC_LOG(RMC_SERVER_TAG, " [ Server Control ]\n");

		if (argc == 3 && strncmp(argv[2], "set", 4) == 0) {
			ble_data data[1] = { 0, };

			data->data = g_adv_raw;
			data->length = sizeof(g_adv_raw);

			ret = ble_server_set_adv_data(data);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to set adv raw data[%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "Set adv raw data ... ok\n");

			data->data = g_adv_resp;
			data->length = sizeof(g_adv_resp);

			ret = ble_server_set_adv_resp(data);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to set adv resp data[%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "Set adv resp data ... ok\n");
		}

		if (argc == 3 && strncmp(argv[2], "start", 6) == 0) {
			ret = ble_server_start_adv();
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to start adv [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "Start adv ... ok\n");
		}

		if (argc == 3 && strncmp(argv[2], "stop", 5) == 0) {
			ret = ble_server_stop_adv();
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to stop adv [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "Stop adv ... ok\n");
		}
	}

ble_rmc_done:
	RMC_LOG(RMC_CLIENT_TAG, "done\n");
	return 0;
}
