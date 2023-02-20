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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/clock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <tinyara/timer.h>
#include <ble_manager/ble_manager.h>

#define RMC_TAG "\x1b[33m[BLE]\x1b[0m "
#define RMC_CLIENT_TAG "\x1b[32m[CLIENT]\x1b[0m "
#define RMC_SERVER_TAG "\x1b[36m[SERVER]\x1b[0m "
#define RMC_LOG(tag, fmt, args...) printf(tag fmt, ##args)

#define BLE_APP_HANDLE_SERVICE_0 (0x006b)
#define BLE_STATE_MANAGER_RMC_HANDLE_KEY_COMMAND (0x006c)
#define BLE_STATE_MANAGER_RMC_HANDLE_KEY_CCCD (0x006e)

#define BLE_APP_HANDLE_SERVICE_1 (0x0073)
#define BLE_APP_HANDLE_CHAR_RMC_KEY (0x0074)
#define BLE_APP_HANDLE_DESC_RMC_KEY (0x0076)
#define BLE_APP_HANDLE_SERVICE_2 (0x0077)
#define BLE_APP_HANDLE_CHAR_RMC_SYNC (0x0078)
#define BLE_STATE_MANAGER_RMC_HANDLE_OTA_SERVICE (0xff00)
#define BLE_STATE_MANAGER_RMC_HANDLE_OTA_COMMAND (0xff01)
#define BLE_STATE_MANAGER_RMC_HANDLE_OTA_INDI_CCCD (0xff03)

#define COMBO_TEST_ADV_INTERVAL_PERFORMANCE (0x0020) // 20ms
#define COMBO_TEST_ADV_INTERVAL_BALANCED (0x0650) // 1010ms
#define COMBO_TEST_ADV_INTERVAL_POWERSAVE (0x4000) // 10240ms

#define SET_ADV_INTERVAL_PERFORMANCE 0
#define SET_ADV_INTERVAL_BALANCED 1
#define SET_ADV_INTERVAL_POWERSAVE 2

#define COMBO_DEFAULT_DURATION (1000)
#define COMBO_DEFAULT_ADV_INTERVAL (COMBO_TEST_ADV_INTERVAL_PERFORMANCE)

#define BLE_WRITE_RESP 0
#define BLE_WRITE_NORESP 1

#define BLE_MAX_MTU 247

#define BLE_TESTER_MQ_SIZE 2
#define BLE_TESTER_MQ_NAME "ble_tester"

typedef struct {
	uint16_t id;
	uint16_t total;
	uint16_t packet_size;
	unsigned long tiemstamp;
} ble_packet_s;

static sem_t g_conn_sem = { 0, };
static int g_is_conn = 0;
static int g_scan_done = -1;
static int g_is_scan = 0;
static int g_server_process_run = 0;
static ble_addr g_target = { 0, };
static ble_client_ctx *g_ctx = NULL;

static int g_scan_time = 20; // 20 seconds
static int g_packet_size = BLE_MAX_MTU - 3; // 244 bytes
static int g_packet_count = 100;
/* Total Packet Size : size * count */

static struct timer_status_s before;
static struct timer_status_s after;
static int frt_fd;
static unsigned long log_diff;

static mqd_t get_mqfd(int type) {
	struct mq_attr attr;
	attr.mq_maxmsg = 5;
	attr.mq_msgsize = BLE_TESTER_MQ_SIZE;
	attr.mq_flags = 0;
	mqd_t mqfd;
	
	if (type == 1) {
		mqfd = mq_open(BLE_TESTER_MQ_NAME, O_RDONLY | O_CREAT, 0666, &attr);
	} else {
		mqfd = mq_open(BLE_TESTER_MQ_NAME, O_WRONLY, 0666, &attr);
	}
	if (mqfd == (mqd_t)ERROR) {
		RMC_LOG(RMC_TAG, "mq_open FAIL\n");
	}

	return mqfd;
}

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

static void ble_get_state(ble_client_ctx *ctx) {
	RMC_LOG(RMC_CLIENT_TAG, "Client State [ %s ]\n", __client_state_str(ble_client_get_state(ctx)));
}

static void ble_scan_state_changed_cb(ble_scan_state_e scan_state)
{
	if (scan_state == BLE_SCAN_STOPPED) {
		g_is_scan = 0;
		RMC_LOG(RMC_TAG, "Scan Stopped\n");
	} else {
		g_is_scan = 1;
		RMC_LOG(RMC_TAG, "Scan Started\n");
	}
	return;
}

static uint8_t ble_filter[] = { 0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18 };

static void ble_device_scanned_cb_with_filter(ble_scanned_device *scanned_device)
{
	RMC_LOG(RMC_CLIENT_TAG, "Find RMC with scan filter!!!\n");
	printf("scanned mac : %02x:%02x:%02x:%02x:%02x:%02x\n", 
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

static void ble_device_scanned_cb_without_filter(ble_scanned_device *scanned_device)
{
	/*
	In without filter callback, you should not do heavy works such as printing every result of scan result.
	*/
	if (memcmp(scanned_device->raw_data, ble_filter, sizeof(ble_filter)) == 0) {
		RMC_LOG(RMC_CLIENT_TAG,"Find RMC with raw data !!!\n");
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
	
	return;
}

static void ble_device_disconnected_cb(ble_client_ctx *ctx)
{
	RMC_LOG(RMC_CLIENT_TAG, "'%s' is called[%p]\n", __FUNCTION__, ctx);
	g_is_conn = 0;
	(void)sem_post(&g_conn_sem);
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

	g_is_conn = 1;
	(void)sem_post(&g_conn_sem);
	return;
}

static void ble_operation_notification_cb(ble_client_ctx *ctx, ble_attr_handle attr_handle, ble_data *read_result)
{
	RMC_LOG(RMC_CLIENT_TAG, "'%s' is called[%p]\n", __FUNCTION__, ctx);
	printf("attr : %x // len : %d\n", attr_handle, read_result->length);
	if (read_result->length > 0) {
		int i;
		printf("raw : ");
		for (i = 0; i < read_result->length; i++) {
			printf("%02x ", read_result->data[i]);
		}
		printf("\n");

		printf("text : ");
		for (i = 0; i < read_result->length; i++) {
			printf("%c", read_result->data[i]);
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

static void ble_server_mtu_update_cb(ble_conn_handle con_handle, uint16_t mtu_size)
{
	RMC_LOG(RMC_SERVER_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_SERVER_TAG, "conn : %d\n", con_handle);
	RMC_LOG(RMC_SERVER_TAG, "mtu_size : $d\n", mtu_size);
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

static void ble_peri_cb_charact_rmc_sync(ble_server_attr_cb_type_e type, ble_conn_handle conn_handle, ble_attr_handle attr_handle, void* arg) {
	uint8_t buf[256] = { 0, };
	ble_data data = { buf, sizeof(buf) };
	ble_result_e ret = ble_server_attr_get_data(attr_handle, &data);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_SERVER_TAG, "[RMC_SYNC] Fail to get attr data\n");
		return;
	}
	if (type == BLE_SERVER_ATTR_CB_WRITING_NO_RSP || type == BLE_SERVER_ATTR_CB_WRITING) {
		if (data.data[0] != data.data[data.length - 1]) {
			RMC_LOG(RMC_SERVER_TAG, "[WRITE] Fail[%d] : %d / %d\n" , data.length, data.data[0], data.data[1]);
		} else {
			RMC_LOG(RMC_SERVER_TAG, "[WRITE] Success[%d] : %d / %d\n" , data.length, data.data[0], data.data[1]);
		}
	} else {
		RMC_LOG(RMC_SERVER_TAG, "[READ] not yet\n");
	}

	if (data.data[0] != 0 && data.data[0] == data.data[1]) {
		RMC_LOG(RMC_SERVER_TAG, "[WRITE] ========================\n");
		RMC_LOG(RMC_SERVER_TAG, "[WRITE]        Write Done.\n");
		RMC_LOG(RMC_SERVER_TAG, "[WRITE] ========================\n");
		
		mqd_t mqfd = get_mqfd(0);
		char msg_buffer[BLE_TESTER_MQ_SIZE] = { 0, };
		memcpy(msg_buffer, &conn_handle, sizeof(conn_handle));
		int status = mq_send(mqfd, msg_buffer, BLE_TESTER_MQ_SIZE, 0);
		if (status < 0) {
			printf("mq_send FAIL : failure=%d (%d)\n", status, get_errno());
		}
		mq_close(mqfd);
	}
}

static void ble_peri_cb_charact_ota(ble_server_attr_cb_type_e type, ble_conn_handle conn_handle, ble_attr_handle attr_handle, void* arg) {
	char *arg_str = "None";
	if (arg != NULL) {
		arg_str = (char *)arg;
	}
	RMC_LOG(RMC_SERVER_TAG, "[CHAR_OTA][%s] type : %d / handle : %d / attr : %02x \n", arg_str, type, conn_handle, attr_handle);
}

static ble_server_gatt_t gatt_profile[] = {
	{.type = BLE_SERVER_GATT_SERVICE, .uuid = {0x12,0xB6,0x6E,0x45,0xA7,0x68,0x9D,0x8D,0x9A,0x40,0x17,0x2B,0xE9,0xCB,0xF2,0x13}, .uuid_length = 16,
	.attr_handle = BLE_APP_HANDLE_SERVICE_0,},

	{.type = BLE_SERVER_GATT_CHARACT, .uuid = {0x99,0xC7,0xAA,0xE7,0xF8,0x9A,0xCB,0x88,0x43,0x4C,0x44,0xCF,0x0D,0x5B,0xDA,0xF2}, .uuid_length = 16,
	.property =  BLE_ATTR_PROP_RWN|BLE_ATTR_PROP_WRITE_NO_RSP, .permission = BLE_ATTR_PERM_R_PERMIT|BLE_ATTR_PERM_W_PERMIT,
	.attr_handle = BLE_STATE_MANAGER_RMC_HANDLE_KEY_COMMAND, .cb = utc_cb_charact_a_1, .arg = "char_1"},

	{.type = BLE_SERVER_GATT_DESC, .uuid = {0x02,0x29}, .uuid_length = 2,
	.permission = BLE_ATTR_PERM_R_PERMIT|BLE_ATTR_PERM_W_PERMIT,
	.attr_handle = BLE_STATE_MANAGER_RMC_HANDLE_KEY_CCCD, .cb = utc_cb_desc_b_1, .arg = "desc_1"},

	{.type = BLE_SERVER_GATT_SERVICE, .uuid = {0xAD,0xB6,0x6E,0x45,0xA7,0x68,0x9D,0x8D,0x9A,0x40,0x17,0x2B,0xE9,0xCB,0xF2,0x13}, .uuid_length = 16,
	.attr_handle = BLE_APP_HANDLE_SERVICE_1,},

	{.type = BLE_SERVER_GATT_CHARACT, .uuid = {0x04,0xC7,0xAA,0xE7,0xF8,0x9A,0xCB,0x88,0x43,0x4C,0x44,0xCF,0x0D,0x5B,0xDA,0xF2}, .uuid_length = 16,
	.property =  BLE_ATTR_PROP_RWN|BLE_ATTR_PROP_WRITE_NO_RSP, .permission = BLE_ATTR_PERM_R_PERMIT|BLE_ATTR_PERM_W_PERMIT,
	.attr_handle = BLE_APP_HANDLE_CHAR_RMC_KEY, .cb = utc_cb_charact_a_1, .arg = "char_2"},

	{.type = BLE_SERVER_GATT_DESC, .uuid = {0x02,0x29}, .uuid_length = 2,
	.permission = BLE_ATTR_PERM_R_PERMIT|BLE_ATTR_PERM_W_PERMIT,
	.attr_handle = BLE_APP_HANDLE_DESC_RMC_KEY, .cb = utc_cb_desc_b_1, .arg = "desc_2"},

	{.type = BLE_SERVER_GATT_SERVICE, .uuid = {0xF4,0x7A,0x07,0x08,0xFD,0xC7,0x9D,0xB5,0xFF,0x4E,0x85,0xDE,0x48,0x80,0xFE,0xA2}, .uuid_length = 16,
	.attr_handle = BLE_APP_HANDLE_SERVICE_2,},

	{.type = BLE_SERVER_GATT_CHARACT, .uuid = {0x06,0xC7,0xAA,0xE7,0xF8,0x9A,0xCB,0x88,0x43,0x4C,0x44,0xCF,0x0D,0x5B,0xDA,0xBB}, .uuid_length = 16,
	.property =  BLE_ATTR_PROP_RWN|BLE_ATTR_PROP_WRITE_NO_RSP, .permission = BLE_ATTR_PERM_R_PERMIT|BLE_ATTR_PERM_W_PERMIT,
	.attr_handle = BLE_APP_HANDLE_CHAR_RMC_SYNC, .cb = ble_peri_cb_charact_rmc_sync, .arg = "char_3"},

	{.type = BLE_SERVER_GATT_SERVICE, .uuid = {0x11,0xB6,0x6E,0x45,0xA7,0x68,0x9D,0x8D,0x9A,0x40,0x17,0x2B,0xE9,0xCB,0xF2,0x13}, .uuid_length = 16,
	.attr_handle = BLE_STATE_MANAGER_RMC_HANDLE_OTA_SERVICE,},

	{.type = BLE_SERVER_GATT_CHARACT, .uuid = {0x22,0xC7,0xAA,0xE7,0xF8,0x9A,0xCB,0x88,0x43,0x4C,0x44,0xCF,0x0D,0x5B,0xDA,0xF2}, .uuid_length = 16,
	.property =  BLE_ATTR_PROP_READ|BLE_ATTR_PROP_WRITE|BLE_ATTR_PROP_INDICATE, .permission = BLE_ATTR_PERM_R_PERMIT|BLE_ATTR_PERM_W_PERMIT,
	.attr_handle = BLE_STATE_MANAGER_RMC_HANDLE_OTA_COMMAND, .cb = ble_peri_cb_charact_ota, .arg = "char_4"},

	{.type = BLE_SERVER_GATT_DESC, .uuid = {0x02,0x29}, .uuid_length = 2,
	.permission = BLE_ATTR_PERM_R_PERMIT|BLE_ATTR_PERM_W_PERMIT,
	.attr_handle = BLE_STATE_MANAGER_RMC_HANDLE_OTA_INDI_CCCD, .cb = utc_cb_desc_b_1, .arg = "desc_4"},
};

static uint8_t g_adv_raw[] = { 
	0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18 
};
static uint8_t g_adv_resp[] = {
	0x11, 0x09, 'T', 'I', 'Z', 'E', 'N', 'R', 'T', ' ', 'T', 'E', 'S', 'T', '(', '0', '2', ')',
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

static int ble_connect_common(bool is_auto)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	struct timespec abstime;
	ble_client_state_e cli_state = BLE_CLIENT_NONE;
	ble_conn_info conn_info = { 0, };
	int conn_timeout = 5; /* conn timeout : 5 seconds */

	memcpy(conn_info.addr.mac, g_target.mac, BLE_BD_ADDR_MAX_LEN);
	RMC_LOG(RMC_CLIENT_TAG, "BLE mac type : %d\n", g_target.type);
	conn_info.addr.type = g_target.type;
	conn_info.conn_interval = 8;
	conn_info.slave_latency = 12;
	conn_info.mtu = BLE_MAX_MTU;
	conn_info.scan_timeout = conn_timeout * 1000;
	conn_info.is_secured_connect = true;

	sem_init(&g_conn_sem, 0, 0);

	if (g_ctx == NULL) {
		g_ctx = ble_client_create_ctx(&client_config);
		if (g_ctx == NULL) {
			RMC_LOG(RMC_CLIENT_TAG, "create ctx fail\n");
			return 0;
		}
	}
	RMC_LOG(RMC_CLIENT_TAG, "create ctx ok[%p]\n", g_ctx);

	ret = ble_client_autoconnect(g_ctx, is_auto);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_CLIENT_TAG, "fail to set autoconnect=%d [%d]\n", is_auto, ret);
	}

	cli_state = ble_client_get_state(g_ctx);
	RMC_LOG(RMC_CLIENT_TAG, "Client State [ %s ]\n", __client_state_str(cli_state));

	ret = ble_client_connect(g_ctx, &conn_info);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_CLIENT_TAG, "connect fail[%d]\n", ret);
		sem_destroy(&g_conn_sem);
		return 0;
	}

	cli_state = ble_client_get_state(g_ctx);
	RMC_LOG(RMC_CLIENT_TAG, "Client State [ %s ]\n", __client_state_str(cli_state));

	ret = clock_gettime(CLOCK_REALTIME, &abstime);

	abstime.tv_sec = abstime.tv_sec + conn_timeout;
	abstime.tv_nsec = 0;

	int status = sem_timedwait(&g_conn_sem, &abstime);
	sem_destroy(&g_conn_sem);
	if (status < 0) {
		RMC_LOG(RMC_CLIENT_TAG, "ERROR: sem_timedwait failed with: %d\n", errno);
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
	attr_handle = BLE_STATE_MANAGER_RMC_HANDLE_KEY_CCCD;
	ret = ble_client_operation_enable_notification(g_ctx, attr_handle);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_CLIENT_TAG, "Fail to enable noti handle[%02x][%d]\n", attr_handle, ret);
	} else {
		RMC_LOG(RMC_CLIENT_TAG, "Success to enable noti handle[%02x]\n", attr_handle);
	}

	attr_handle = BLE_STATE_MANAGER_RMC_HANDLE_OTA_INDI_CCCD;
	ret = ble_client_operation_enable_indication(g_ctx, attr_handle);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_CLIENT_TAG, "Fail to enable indi handle[%02x][%d]\n", attr_handle, ret);
	} else {
		RMC_LOG(RMC_CLIENT_TAG, "Success to enable indi handle[%02x]\n", attr_handle);
	}

	return 0;
}

static int ble_change_adv_interval(int type) {
	ble_result_e ret = BLE_MANAGER_FAIL;
	int interval = COMBO_DEFAULT_ADV_INTERVAL;
	switch(type) {
		case SET_ADV_INTERVAL_PERFORMANCE:
		RMC_LOG(RMC_SERVER_TAG, "Set Adv Performance mode\n");
		interval = COMBO_TEST_ADV_INTERVAL_PERFORMANCE;
		break;
		case SET_ADV_INTERVAL_BALANCED:
		RMC_LOG(RMC_SERVER_TAG, "Set Adv Balanced mode\n");
		interval = COMBO_TEST_ADV_INTERVAL_BALANCED;
		break;
		case SET_ADV_INTERVAL_POWERSAVE:
		RMC_LOG(RMC_SERVER_TAG, "Set Adv Powersave mode\n");
		interval = COMBO_TEST_ADV_INTERVAL_POWERSAVE;
		break;
	}
	ret = ble_server_set_adv_interval(interval);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_SERVER_TAG, "Fail to set adv interval[%d]\n", ret);
	} else {
		RMC_LOG(RMC_SERVER_TAG, "Success to set adv interval\n");
	}

	ret = ble_server_set_adv_type(BLE_ADV_TYPE_IND, NULL);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_SERVER_TAG, "Fail to set adv type[%d]\n", ret);
	} else {
		RMC_LOG(RMC_SERVER_TAG, "Success to set adv type\n");
	}

	usleep(100 * 1000);
	return 0;
}

static void *_ble_scan_process(void *data)
{
	int scan_time = *(int *)(data);
	ble_result_e ret = BLE_MANAGER_FAIL;
	scan_config.device_scanned_cb = ble_device_scanned_cb_without_filter;
	RMC_LOG(RMC_CLIENT_TAG, "Start Scan for [ %d ] seconds \n", scan_time);
	ret = ble_client_start_scan(NULL, &scan_config);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_CLIENT_TAG, "scan start fail[%d]\n", ret);
		return NULL;
	}

	sleep(scan_time);

	ret = ble_client_stop_scan();
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_CLIENT_TAG, "scan stop fail[%d]\n", ret);
		return NULL;
	}

	return NULL;
}

static int ble_scan_with_timer(int scan_time) {
	pthread_t pid;
	int ret = pthread_create(&pid, NULL, (pthread_startroutine_t)_ble_scan_process, (void *)&scan_time);
	if (ret < 0) {
		return -1;
	}
	pthread_detach(pid);
	return 0;
}

static int ble_prepare_test(void) 
{
	ble_result_e ret;
	
	ret = ble_server_stop_adv();
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_TAG, "Fail to start adv [%d]\n", ret);
	}

	ret = ble_client_stop_scan();
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_TAG, "scan stop fail[%d]\n", ret);
	}

	while (g_is_scan) {
		usleep(200 * 1000);
	}
	return 0;
}

static int ble_write_test(int mode) 
{
	ble_result_e ret;
	ble_client_state_e cli_state;
	
	if ((cli_state = ble_client_get_state(g_ctx)) != BLE_CLIENT_CONNECTED) {
		RMC_LOG(RMC_CLIENT_TAG, "BLE is not connected [ %s ]\n", __client_state_str(cli_state));
		return 0;
	}
	RMC_LOG(RMC_CLIENT_TAG, "BLE is connected [ %s ]\n", __client_state_str(cli_state));
	RMC_LOG(RMC_CLIENT_TAG, "Send %d Bytes packet %d times.\n", g_packet_size, g_packet_count);

	ble_attr_handle attr_handle = BLE_APP_HANDLE_CHAR_RMC_SYNC + 1;
	ble_data packet[1] = { 0, };
	uint8_t packet_data[256] = { 0, };
	
	int i;
	int send_ok = 0;
	int send_fail = 0;
	packet->data = packet_data;
	packet->length = g_packet_size;

	ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&before);
	for (i = 0; i < g_packet_count; i++) { 
		packet_data[0] = i + 1;
		packet_data[1] = g_packet_count;
		packet_data[g_packet_size - 1] = i + 1;
		if (mode == BLE_WRITE_NORESP) {
			ret = ble_client_operation_write_no_response(g_ctx, attr_handle, packet);
		} else {
			ret = ble_client_operation_write(g_ctx, attr_handle, packet);
		}
		if (ret != BLE_MANAGER_SUCCESS) {
			send_fail++;
			RMC_LOG(RMC_CLIENT_TAG, "Fail[X] (%d / %d) [%d]\n", i + 1, g_packet_count, ret);
		} else {
			send_ok++;
			RMC_LOG(RMC_CLIENT_TAG, "Success (%d / %d)\n", i + 1, g_packet_count);
		}
	}
	ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&after);
	log_diff = (unsigned long)after.timeleft - (unsigned long)before.timeleft;
	double total_time = (double)log_diff / 1000000;
	double speed = (g_packet_size * g_packet_count * 8) / total_time;
	RMC_LOG(RMC_TAG, "[[ Total Success : %d / %d\t]]\n", send_ok, g_packet_count);
	RMC_LOG(RMC_TAG, "[[ Total Time    : %.2lf sec\t]]\n", total_time);
	RMC_LOG(RMC_TAG, "[[ BLE Bandwidth : %.2lf Kbps\t]]\n", speed / 1024);

	return 0;
}

static void *_ble_server_process(void *val)
{
	g_server_process_run = 0;

	char msg_buffer[BLE_TESTER_MQ_SIZE];
	int nbytes;
	struct timespec ts;
	int status;
	mqd_t mqfd = get_mqfd(1);
	if (mqfd == (mqd_t)ERROR) {
		RMC_LOG(RMC_SERVER_TAG, "mq_open FAIL\n");
		return NULL;
	}

	g_server_process_run = 1;

	RMC_LOG(RMC_SERVER_TAG, "BLE Server Running\n");
	while(g_server_process_run) {
		status = clock_gettime(CLOCK_REALTIME, &ts);
		if (status != 0) {
			RMC_LOG(RMC_SERVER_TAG, "ERROR clock_gettime failed\n");
			g_server_process_run = 0;
			break;
		}
		ts.tv_sec += 1;

		memset(msg_buffer, 0, sizeof(msg_buffer));
		nbytes = mq_timedreceive(mqfd, msg_buffer, sizeof(msg_buffer), 0, &ts);
		if (nbytes > 0) {
			ble_conn_handle conn_handle;
			memcpy(&conn_handle, msg_buffer, sizeof(conn_handle));
			RMC_LOG(RMC_SERVER_TAG, "[NOTI] CONN : %d \n", conn_handle);

			ble_result_e ret;
			ble_data data;
			char send_str[] = "Recevied BLE Packet!";
			
			data.data = (uint8_t *)send_str;
			data.length = strlen(send_str) + 1;
			ret = ble_server_charact_notify(BLE_STATE_MANAGER_RMC_HANDLE_KEY_COMMAND + 1, conn_handle, &data);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Notify Value fail[%d]\n", ret);
			}
			RMC_LOG(RMC_TAG, "[NOTI] Send Noti OK\n");
		}
	}

	RMC_LOG(RMC_SERVER_TAG, "BLE Server Done\n");

	mq_close(mqfd);
	mq_unlink(BLE_TESTER_MQ_NAME);

	return NULL;
}

/****************************************************************************
 * ble_tester_main
 ****************************************************************************/
int ble_tester_main(int argc, char *argv[])
{
	RMC_LOG(RMC_TAG, "- BLE Spec Test -\n");

	ble_result_e ret = BLE_MANAGER_FAIL;
	static int is_ble_init = 0;
	static int is_ble_ready = 0;
	
	frt_fd = open("/dev/timer0", O_RDONLY);
	ioctl(frt_fd, TCIOC_SETMODE, MODE_FREERUN);
	ioctl(frt_fd, TCIOC_START, 0);

	RMC_LOG(RMC_TAG, "cmd : %s\n", argv[1]);

	if (argc >= 2) {
		if (strncmp(argv[1], "init", 5) == 0) {
			/*
			1. BLE Manager init
			*/
			if (is_ble_init == 1) {
				RMC_LOG(RMC_TAG, "init is already done\n");
			} else {
				ret = ble_manager_init(&server_config);
				if (ret != BLE_MANAGER_SUCCESS) {
					if (ret != BLE_MANAGER_ALREADY_WORKING) {
						RMC_LOG(RMC_TAG, "init fail[%d]\n", ret);
						goto tester_done;
					}
					RMC_LOG(RMC_TAG, "init is already done\n");
					ret = ble_manager_deinit();
					if (ret != BLE_MANAGER_SUCCESS) {
						RMC_LOG(RMC_TAG, "deinit fail[%d]\n", ret);
						goto tester_done;
					}
					ret = ble_manager_init(&server_config);
					if (ret != BLE_MANAGER_SUCCESS) {
						RMC_LOG(RMC_TAG, "init fail[%d]\n", ret);
						goto tester_done;
					}
				} else {
					RMC_LOG(RMC_TAG, "init with config done[%d]\n", ret);
				}

				is_ble_init = 1;
				usleep(100 * 1000);
			}

			/*
			2. Start server (= advertising)
			*/
			ble_data data[1] = { 0, };
			data->data = g_adv_raw;
			data->length = sizeof(g_adv_raw);

			ret = ble_server_set_adv_data(data);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to set adv raw data[%d]\n", ret);
				goto tester_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "Set adv raw data ... ok\n");

			data->data = g_adv_resp;
			data->length = sizeof(g_adv_resp);

			ret = ble_server_set_adv_resp(data);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to set adv resp data[%d]\n", ret);
				goto tester_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "Set adv resp data ... ok\n");

			ret = ble_server_set_adv_type(BLE_ADV_TYPE_IND, NULL);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to set adv type[%d]\n", ret);
				goto tester_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "Set adv type ... ok\n");

			ret = ble_server_start_adv();
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to start adv [%d]\n", ret);
				goto tester_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "Start adv ... ok\n");

			pthread_t pid;
			if (pthread_create(&pid, NULL, (pthread_startroutine_t)_ble_server_process, NULL) < 0) {
				return -1;
			}
			pthread_detach(pid);
			
		} else if (strncmp(argv[1], "set", 4) == 0) {
			/*
			set packet size and count
			ble_tester 20 240 100
			*/
			if (argc == 5) {
				g_scan_time = atoi(argv[2]);
				g_packet_size = atoi(argv[3]);
				if (g_packet_size > BLE_MAX_MTU - 3) {
					g_packet_size = BLE_MAX_MTU - 3;
				}
				g_packet_count = atoi(argv[4]);
				if (g_packet_count <= 1) {
					g_packet_count = 1;
				}
			} else {
				RMC_LOG(RMC_TAG, "Param is not enough.\n");
				RMC_LOG(RMC_TAG, "Usage:   ble_tester set [SCAN_TIME] [PACKET_SIZE] [PACKET_COUNT]\n");
			}

			double total = (double)(g_packet_count * g_packet_size) / 1024;
			RMC_LOG(RMC_TAG, "< Param is set >\n");
			RMC_LOG(RMC_TAG, "Scan Time    : %d\n", g_scan_time);
			RMC_LOG(RMC_TAG, "Packet Size  : %d\n", g_packet_size);
			RMC_LOG(RMC_TAG, "Packet Count : %d\n", g_packet_count);
			RMC_LOG(RMC_TAG, "Total Packet : %.1lf Kbytes\n", total);

		} else if (strncmp(argv[1], "disconn", 8) == 0) {
			if (g_ctx != NULL) {
				ret = ble_client_disconnect(g_ctx);
				if (ret != BLE_MANAGER_SUCCESS) {
					RMC_LOG(RMC_CLIENT_TAG, "Fail to disconnect[%d]\n", ret);
				}

				int i;				
				for (i = 0; i < 10; i++) {
					if (ble_client_get_state(g_ctx) == BLE_CLIENT_IDLE) {
						RMC_LOG(RMC_CLIENT_TAG, "Success to disconnect\n");
						is_ble_ready = 0;
						break;
					}
					sleep(1);
				}
				if (i == 5) {
					RMC_LOG(RMC_CLIENT_TAG, "Disconnect is not working.\n");
				}
			}

		} else if (strncmp(argv[1], "ready", 6) == 0) {
			/*
			0. delete bond devices
			1. scan
			2. try connect 3 times
			*/

			if (is_ble_ready != 0) {
				RMC_LOG(RMC_CLIENT_TAG, "BLE is already connected\n");
				goto tester_done;
			}

			ret = ble_manager_delete_bonded_all();
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "fail to delete all of bond dev[%d]\n", ret);
			} else {
				RMC_LOG(RMC_CLIENT_TAG, "success to delete all of bond dev\n");
			}

			ble_scan_filter filter = { 0, };
			int scan_time = 10; // wait 10 seconds
			memcpy(&(filter.raw_data), ble_filter, sizeof(ble_filter));
			filter.raw_data_length = sizeof(ble_filter);
			filter.scan_duration = 1000 * scan_time;
			scan_config.device_scanned_cb = ble_device_scanned_cb_with_filter;
			g_scan_done = 0;
			ret = ble_client_start_scan(&filter, &scan_config);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_CLIENT_TAG, "scan start fail[%d]\n", ret);
				goto tester_done;
			}

			int t_count = scan_time * 5;
			while (g_scan_done == 0 && t_count-- > 0) {
				usleep(200 * 1000);
			}

			if (g_scan_done == 0 || scan_time <= 0) {
				RMC_LOG(RMC_CLIENT_TAG, "Device is not found\n", ret);
				goto tester_done;
			} else {
				ret = ble_client_stop_scan();
				if (ret != BLE_MANAGER_SUCCESS) {
					RMC_LOG(RMC_CLIENT_TAG, "scan stop fail[%d]\n", ret);
				}
			}

			/* Let's try to connect 3 times */
			int conn_count = 3;

			while (conn_count--) {
				g_is_conn = 0;
				
				ble_connect_common(false);

				sleep(1);

				if (ble_client_get_state(g_ctx) == BLE_CLIENT_CONNECTED) {
					is_ble_ready = 1;
					break;
				}
			}

			if (is_ble_ready != 1) {
				RMC_LOG(RMC_CLIENT_TAG, "BLE connect fail\n");
			}

		} else if (strncmp(argv[1], "test1", 6) == 0) {
			ble_prepare_test();
			
			ble_change_adv_interval(SET_ADV_INTERVAL_PERFORMANCE);

			ret = ble_server_start_adv();
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to start adv [%d]\n", ret);
			}

			ble_scan_with_timer(g_scan_time);

		} else if (strncmp(argv[1], "test2", 6) == 0) {
			ble_prepare_test();

			ble_change_adv_interval(SET_ADV_INTERVAL_BALANCED);

			ret = ble_server_start_adv();
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to start adv [%d]\n", ret);
			}

			ble_scan_with_timer(g_scan_time);

		} else if (strncmp(argv[1], "test3", 6) == 0) {
			ble_prepare_test();

			ble_change_adv_interval(SET_ADV_INTERVAL_POWERSAVE);

			ret = ble_server_start_adv();
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to start adv [%d]\n", ret);
			}

			ble_scan_with_timer(g_scan_time);

		} else if (strncmp(argv[1], "test4", 6) == 0) {
			ble_prepare_test();

			ble_scan_with_timer(g_scan_time);
			
			ble_write_test(BLE_WRITE_NORESP);
		} else if (strncmp(argv[1], "test5", 6) == 0) {
			ble_prepare_test();

			ble_scan_with_timer(g_scan_time);
			
			ble_write_test(BLE_WRITE_RESP);
		} else if (strncmp(argv[1], "test6", 6) == 0) {
			ble_prepare_test();

			ble_change_adv_interval(SET_ADV_INTERVAL_BALANCED);
			
			ble_write_test(BLE_WRITE_NORESP);
		} else if (strncmp(argv[1], "test7", 6) == 0) {
			ble_prepare_test();

			ble_change_adv_interval(SET_ADV_INTERVAL_BALANCED);
			
			ble_write_test(BLE_WRITE_RESP);
		} else if (strncmp(argv[1], "noti", 5) == 0) {
			ble_data packet;
			ble_conn_handle conn_handle = 0;

			uint8_t data[4] = { 0, };

			if (argc > 2) {
				data[0] = atoi(argv[2]);
			} else {
				data[0] = 0x01;
				data[1] = 0x02;
				data[2] = 0x03;
				data[3] = 0x04;
			}

			RMC_LOG(RMC_SERVER_TAG, "Send Noti Value : %02x %02x %02x %02x\n", data[0], data[1], data[2], data[3]);
			
			packet.data = (uint8_t *)data;
			packet.length = sizeof(data);
			ret = ble_server_charact_notify(BLE_STATE_MANAGER_RMC_HANDLE_KEY_COMMAND + 1, conn_handle, &packet);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Notify Value fail[%d]\n", ret);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "[NOTI] Send Noti OK\n");
			}
		} else if (strncmp(argv[1], "noti2", 6) == 0) {
			ble_data packet;
			ble_conn_handle conn_handle = 0;

			uint8_t data[4] = { 0, };

			if (argc > 2) {
				data[0] = atoi(argv[2]);
			} else {
				data[0] = 0x01;
				data[1] = 0x02;
				data[2] = 0x03;
				data[3] = 0x04;
			}

			RMC_LOG(RMC_SERVER_TAG, "Send Noti Value : %02x %02x %02x %02x\n", data[0], data[1], data[2], data[3]);
			
			packet.data = (uint8_t *)data;
			packet.length = sizeof(data);
			ret = ble_server_charact_notify(BLE_STATE_MANAGER_RMC_HANDLE_KEY_COMMAND, conn_handle, &packet);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Notify Value fail[%d]\n", ret);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "[NOTI] Send Noti OK\n");
			}
		} else if (strncmp(argv[1], "indi", 5) == 0) {
			ble_data packet;
			ble_conn_handle conn_handle = 0;

			uint8_t data[4] = { 0, };

			if (argc > 2) {
				data[0] = atoi(argv[2]);
			} else {
				data[0] = 0x04;
				data[1] = 0x03;
				data[2] = 0x02;
				data[3] = 0x01;
			}

			RMC_LOG(RMC_SERVER_TAG, "Send Indi Value : %02x %02x %02x %02x\n", data[0], data[1], data[2], data[3]);
			
			packet.data = (uint8_t *)data;
			packet.length = sizeof(data);
			ret = ble_server_charact_indicate(BLE_STATE_MANAGER_RMC_HANDLE_OTA_COMMAND + 1, conn_handle, &packet);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Indicate Value fail[%d]\n", ret);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "[INDI] Send Indi OK\n");
			}
		} else {
			RMC_LOG(RMC_TAG, "Fail to run command [ %s ]\n", argv[1]);
		}
	}

tester_done:
	ioctl(frt_fd, TCIOC_STOP, 0);
	close(frt_fd);

	RMC_LOG(RMC_TAG, "done\n");
	return 0;
}
