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

static sem_t g_conn_sem = { 0, };
static ble_conn_handle g_conn = 0;
static int g_scan_done = -1;
static uint8_t g_target[BLE_BD_ADDR_MAX_LEN] = { 0, };
static ble_client_ctx *g_ctx = NULL;

static char *client_state_str[] = {
	"\x1b[35mNONE\x1b[0m",
	"\x1b[35mIDLE\x1b[0m",
	"\x1b[35mCONNECTED\x1b[0m",
	"\x1b[35mCONNECTING\x1b[0m",
	"\x1b[35mDISCONNECTING\x1b[0m",
};

static char *__client_state_str(ble_client_state_e state)
{
	return client_state_str[state];
}

static void ble_scan_state_changed_cb(ble_scan_state_e scan_state)
{
	RMC_LOG(RMC_CLIENT_TAG, "'%s' is called[%d]\n", __FUNCTION__, scan_state);
	return;
}

static void ble_device_scanned_cb(ble_scanned_device *scanned_device)
{
	RMC_LOG(RMC_CLIENT_TAG, "'%s' is called\n", __FUNCTION__);
	printf("scan mac : %02x:%02x:%02x:%02x:%02x:%02x\n", 
		scanned_device->conn_info.addr.mac[0],
		scanned_device->conn_info.addr.mac[1],
		scanned_device->conn_info.addr.mac[2],
		scanned_device->conn_info.addr.mac[3],
		scanned_device->conn_info.addr.mac[4],
		scanned_device->conn_info.addr.mac[5]
	);
	if (g_scan_done == 0) {
		g_target[0] = scanned_device->conn_info.addr.mac[0];
		g_target[1] = scanned_device->conn_info.addr.mac[1];
		g_target[2] = scanned_device->conn_info.addr.mac[2];
		g_target[3] = scanned_device->conn_info.addr.mac[3];
		g_target[4] = scanned_device->conn_info.addr.mac[4];
		g_target[5] = scanned_device->conn_info.addr.mac[5];
		g_scan_done = 1;
	}
	
	return;
}

static void ble_device_disconnected_cb(ble_client_ctx *ctx)
{
	RMC_LOG(RMC_CLIENT_TAG, "'%s' is called[%p]\n", __FUNCTION__, ctx);
	(void)sem_post(&g_conn_sem);
	return;
}

static void ble_device_connected_cb(ble_client_ctx *ctx, ble_device_connected *dev)
{
	RMC_LOG(RMC_CLIENT_TAG, "'%s' is called[%p]\n", __FUNCTION__, ctx);

	g_conn = dev->conn_handle;
	RMC_LOG(RMC_CLIENT_TAG, "Conn Handle : %d\n", dev->conn_handle);
	RMC_LOG(RMC_CLIENT_TAG, "Conn Bonded : %d\n", dev->is_bonded);
	RMC_LOG(RMC_CLIENT_TAG, "Conn CI : %d\n", dev->conn_info.conn_interval);
	RMC_LOG(RMC_CLIENT_TAG, "Conn SL : %d\n", dev->conn_info.slave_latency);
	RMC_LOG(RMC_CLIENT_TAG, "Conn MTU : %d\n", dev->conn_info.mtu);
	RMC_LOG(RMC_CLIENT_TAG, "Conn MAC : %02x:%02x:%02x:%02x:%02x:%02x\n", 
		dev->conn_info.addr.mac[0],
		dev->conn_info.addr.mac[1],
		dev->conn_info.addr.mac[2],
		dev->conn_info.addr.mac[3],
		dev->conn_info.addr.mac[4],
		dev->conn_info.addr.mac[5]
	);

	(void)sem_post(&g_conn_sem);
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

static void ble_server_connected_cb(ble_conn_handle con_handle, ble_server_connection_type_e conn_type, uint8_t mac[BLE_BD_ADDR_MAX_LEN])
{
	RMC_LOG(RMC_SERVER_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_SERVER_TAG, "conn : %d / conn_type : %d\n", con_handle, conn_type);
	RMC_LOG(RMC_SERVER_TAG, "conn mac : %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
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

static uint8_t ble_filter[] = { 0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18 };
static uint8_t g_adv_raw[] = { 
	0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18 
};
static uint8_t g_adv_resp[] = {
	0x11, 0x09, 'T', 'I', 'Z', 'E', 'N', 'R', 'T', ' ', 'T', 'E', 'S', 'T', '(', '0', '2', ')',
};

/****************************************************************************
 * ble_rmc_main
 ****************************************************************************/
int ble_rmc_main(int argc, char *argv[])
{
	RMC_LOG(RMC_TAG, "- BLE Remote Test -\n");

	ble_result_e ret = BLE_MANAGER_FAIL;

	ble_scan_callback_list scan_config = {
		ble_scan_state_changed_cb,
		ble_device_scanned_cb,
	};

	ble_client_callback_list client_config = {
		ble_device_disconnected_cb,
		ble_device_connected_cb,
		ble_operation_notification_cb,
	};

	ble_server_init_config server_config = {
		ble_server_connected_cb,
		true,
		gatt_profile, sizeof(gatt_profile) / sizeof(ble_server_gatt_t)};

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
					return 0;
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
					return 0;
				}
				RMC_LOG(RMC_CLIENT_TAG, "init is already done\n");
			} else {
				RMC_LOG(RMC_CLIENT_TAG, "init with config done[%d]\n", ret);
			}
		}
	}

	if (strncmp(argv[1], "state", 6) == 0) {
		RMC_LOG(RMC_CLIENT_TAG, "Client State [ %s ]\n", __client_state_str(ble_client_get_state(g_ctx)));
	}

	if (strncmp(argv[1], "deinit", 7) == 0) {
		ret = ble_manager_deinit();
		RMC_LOG(RMC_CLIENT_TAG, "deinit done[%d]\n", ret);
	}

	if (strncmp(argv[1], "bond", 5) == 0) {
		if (argc == 3) {
			if (strncmp(argv[2], "list", 5) == 0) {
				RMC_LOG(RMC_CLIENT_TAG, "== BLE Bonded List ==\n", ret);

				ble_bonded_device_list dev_list[BLE_MAX_BONDED_DEVICE] = { 0, };
				uint16_t dev_count = 0;
				uint8_t *mac;

				ret = ble_manager_get_bonded_device(dev_list, &dev_count);

				RMC_LOG(RMC_CLIENT_TAG, "Bonded Dev : %d\n", dev_count);
				
				for (int i = 0; i < dev_count; i++) {
					mac = dev_list[i].bd_addr;
					RMC_LOG(RMC_CLIENT_TAG, "DEV#%d : %02x:%02x:%02x:%02x:%02x:%02x\n", i + 1, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
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
			uint8_t mac[BLE_BD_ADDR_MAX_LEN] = { 0, };

			char *ptr = strtok(argv[3], ":");
			while (ptr != NULL) {
				mac[cnt++] = strtol(ptr, NULL, 16);
				ptr = strtok(NULL, ":");
			}
			RMC_LOG(RMC_CLIENT_TAG, "TARGET : %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
			ret = ble_manager_delete_bonded(mac);
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
			return 0;
		}

		RMC_LOG(RMC_CLIENT_TAG, "BLE mac : %02x", mac[0]);
		for (i = 1; i < BLE_BD_ADDR_MAX_LEN; i++) {
			printf(":%02x", mac[i]);
		}
		printf("\n");
	}

	if (strncmp(argv[1], "scan", 5) == 0) {
		if (argc == 3 && argv[2][0] == '1') {
			printf("Start !\n");
			ret = ble_client_start_scan(NULL, &scan_config);

			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "scan start fail[%d]\n", ret);
				return 0;
			}
		} else if (argc == 3 && argv[2][0] == '2') {
			printf("Start with filter!\n");

			ble_scan_filter filter = { 0, };
			memcpy(&(filter.raw_data), ble_filter, sizeof(ble_filter));
			filter.raw_data_length = sizeof(ble_filter);
			filter.scan_duration = 1000;
			ret = ble_client_start_scan(&filter, &scan_config);

			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "scan start fail[%d]\n", ret);
				return 0;
			}
		} else {
			printf("stop !\n");
			ret = ble_client_stop_scan();

			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "scan stop fail[%d]\n", ret);
				return 0;
			}
		}
	}

	if (strncmp(argv[1], "check", 6) == 0) {
		bool chk = false;
		ret = ble_manager_conn_is_active(g_conn, &chk);
		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_CLIENT_TAG, "Fail to get status[%d]\n", ret);
		} else {
			if (chk == true) {
				RMC_LOG(RMC_CLIENT_TAG, "Connected!\n");
			} else {
				RMC_LOG(RMC_CLIENT_TAG, "Disonnected!\n");
			}
		}
	}

	if (strncmp(argv[1], "disconn", 8) == 0) {
		ret = ble_client_disconnect(g_ctx);
		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_CLIENT_TAG, "disconnect fail[%d]\n", ret);
			return 0;
		}
		RMC_LOG(RMC_CLIENT_TAG, "disconnect ok\n");
	}

	if (strncmp(argv[1], "connect", 8) == 0) {
		struct timespec abstime;
		ble_client_state_e cli_state = BLE_CLIENT_NONE;

		if (argc == 3 && strncmp(argv[2], "fail", 5) == 0) {
			memset(g_target, 1, BLE_BD_ADDR_MAX_LEN);
		} else {
			ble_scan_filter filter = { 0, };
			memcpy(&(filter.raw_data), ble_filter, sizeof(ble_filter));
			filter.raw_data_length = sizeof(ble_filter);
			filter.scan_duration = 1000;
			g_scan_done = 0;
			ret = ble_client_start_scan(&filter, &scan_config);

			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "scan start fail[%d]\n", ret);
				return 0;
			}

			sleep(2);

			if (g_scan_done != 1) {
				RMC_LOG(RMC_CLIENT_TAG, "scan fail to get adv packet\n");
				return 0;
			}
		}

		RMC_LOG(RMC_CLIENT_TAG, "Try to connect! [%02x:%02x:%02x:%02x:%02x:%02x]\n", 
			g_target[0],
			g_target[1],
			g_target[2],
			g_target[3],
			g_target[4],
			g_target[5]
		);

		ble_conn_info conn_info = { 0, };
		memcpy(conn_info.addr.mac, g_target, BLE_BD_ADDR_MAX_LEN);
		conn_info.addr.type = BLE_ADDR_TYPE_PUBLIC;
		conn_info.conn_interval = 8;
		conn_info.slave_latency = 128;
		conn_info.mtu = 240;
		conn_info.is_secured_connect = true;

		sem_init(&g_conn_sem, 0, 0);

		if (g_ctx == NULL) {
			g_ctx = ble_client_create_ctx(&client_config);
		}
		if (g_ctx == NULL) {
			RMC_LOG(RMC_CLIENT_TAG, "create ctx fail\n");
			return 0;
		} else {
			RMC_LOG(RMC_CLIENT_TAG, "create ctx ok[%p]\n", g_ctx);
		}

		cli_state = ble_client_get_state(g_ctx);
		RMC_LOG(RMC_CLIENT_TAG, "Client State [ %s ]\n", __client_state_str(cli_state));

		ret = ble_client_connect(g_ctx, &conn_info);
		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_CLIENT_TAG, "connect fail[%d]\n", ret);
		}

		cli_state = ble_client_get_state(g_ctx);
		RMC_LOG(RMC_CLIENT_TAG, "Client State [ %s ]\n", __client_state_str(cli_state));

		ret = clock_gettime(CLOCK_REALTIME, &abstime);

		abstime.tv_sec = abstime.tv_sec + 10;
		abstime.tv_nsec = 0;

		int status = sem_timedwait(&g_conn_sem, &abstime);
		sem_destroy(&g_conn_sem);
		int errcode = errno;
		if (status < 0) {
			RMC_LOG(RMC_CLIENT_TAG, "ERROR: sem_timedwait failed with: %d\n", errcode);
			return 0;
		} else {
			RMC_LOG(RMC_CLIENT_TAG, "PASS: sem_timedwait succeeded\n");
		}

		cli_state = ble_client_get_state(g_ctx);
		RMC_LOG(RMC_CLIENT_TAG, "Client State [ %s ]\n", __client_state_str(cli_state));

		if (cli_state != BLE_CLIENT_CONNECTED) {
			RMC_LOG(RMC_CLIENT_TAG, "BLE is not connected");
			return 0;
		} else {
			RMC_LOG(RMC_CLIENT_TAG, "BLE is connected\n");
		}
		
		ble_attr_handle attr_handle;

		attr_handle = 0xff03;
		ret = ble_client_operation_enable_notification(g_ctx, attr_handle);
		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_CLIENT_TAG, "Fail to enable noti handle1[%d]\n", ret);
		} else {
			RMC_LOG(RMC_CLIENT_TAG, "Success to enable noti handle1.\n");
		}

		attr_handle = 0x006e;
		ret = ble_client_operation_enable_notification(g_ctx, attr_handle);
		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_CLIENT_TAG, "Fail to enable noti handle2[%d]\n", ret);
		} else {
			RMC_LOG(RMC_CLIENT_TAG, "Success to enable noti handle2.\n");
		}
	}

	if (strncmp(argv[1], "server", 7) == 0) {
		RMC_LOG(RMC_SERVER_TAG, " [ Server Control ]\n");

		if (argc == 3 && strncmp(argv[2], "set", 4) == 0) {
			ble_data data[1] = { 0, };

			data->data = g_adv_raw;
			data->length = sizeof(g_adv_raw);

			ret = ble_server_set_adv_data(data);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to set adv raw data[%d]\n", ret);
				return 0;
			}
			RMC_LOG(RMC_SERVER_TAG, "Set adv raw data ... ok\n");

			data->data = g_adv_resp;
			data->length = sizeof(g_adv_resp);

			ret = ble_server_set_adv_resp(data);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to set adv resp data[%d]\n", ret);
				return 0;
			}
			RMC_LOG(RMC_SERVER_TAG, "Set adv resp data ... ok\n");
		}

		if (argc == 3 && strncmp(argv[2], "start", 6) == 0) {
			ret = ble_server_start_adv();
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to start adv [%d]\n", ret);
				return 0;
			}
			RMC_LOG(RMC_SERVER_TAG, "Start adv ... ok\n");
		}

		if (argc == 3 && strncmp(argv[2], "stop", 5) == 0) {
			ret = ble_server_stop_adv();
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to stop adv [%d]\n", ret);
				return 0;
			}
			RMC_LOG(RMC_SERVER_TAG, "Stop adv ... ok\n");
		}
	}

	RMC_LOG(RMC_CLIENT_TAG, "done\n");
	return 0;
}
