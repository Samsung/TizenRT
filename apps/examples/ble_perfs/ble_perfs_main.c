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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyara/timer.h>
#include <wifi_manager/wifi_manager.h>
#include <ble_manager/ble_manager.h>
#include <errno.h>

#define BLE_PERF_TAG "[BLE_PERFS] "
#define BLE_LOGD(fmt, args...) printf(BLE_PERF_TAG " %s:(%d) " fmt, __FUNCTION__, __LINE__, ##args)
#define BLE_LOGE(fmt, args...) printf(BLE_PERF_TAG " [ERR] %s:(%d) " fmt, __FUNCTION__, __LINE__, ##args)

#define BLE_APP_HANDLE_SERVICE_0 (0x006b)
#define BLE_STATE_MANAGER_RMC_HANDLE_KEY_COMMAND (0x006c)
#define BLE_STATE_MANAGER_RMC_HANDLE_KEY_CCCD (0x006e)

#define BLE_APP_HANDLE_SERVICE_1 (0x0073)
#define BLE_APP_HANDLE_CHAR_RMC_KEY (0x0074)
#define BLE_APP_HANDLE_DESC_RMC_KEY (0x0076)
#define BLE_APP_HANDLE_SERVICE_2 (0x0077)
#define BLE_APP_HANDLE_CHAR_RMC_SYNC (0x0078)
#define BLE_MAX_MTU 240
#define MAX_PACKET_COUNT 100
#define MAX_PACKET_SIZE BLE_MAX_MTU - 3; // 237 bytes
#define FIRST_PACKET 11
#define NEXT_PACKET 55
#define LAST_PACKET 99
#define USLEEP_USEC 100 * 1000

struct perfs_data_t
{
	int packet_count;
	int packet_size;
	struct timeval start_time;
	struct timeval end_time;
};

static struct perfs_data_t g_perf_data = {0, 0, {0, 0}, {0, 0} };

static wifi_manager_cb_s g_wifi_callbacks = { 0, };
static int g_is_ble_serv_init = 0;
static uint8_t g_adv_raw[] = {
        0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18
};
static uint8_t g_adv_resp[] = {
        0x0D, 0x09, 'T', 'I', 'Z', 'E', 'N', 'R', 'T', ' ', 'G', 'A', 'T', 'T',
};


static char * attr_cb_type_str[] = {
	"BLE_SERVER_ATTR_CB_WRITING",
	"BLE_SERVER_ATTR_CB_READING",
	"BLE_SERVER_ATTR_CB_WRITING_NO_RSP",
};

static char * connection_type_str[] = {
	"BLE_SERVER_LL_CONNECTED",  /* Link Layer connected */
	"BLE_SERVER_SM_CONNECTED",  /* Security Manager connected */
	"BLE_SERVER_DISCONNECTED",
};

static char * get_attr_cb_type_str(ble_server_attr_cb_type_e cb_type)
{
	return  attr_cb_type_str[cb_type];
};


static char * get_conn_type_str(ble_server_connection_type_e conn_type)
{
	return connection_type_str[conn_type];
}

static void addr_string_to_type(uint8_t *addr, const char *address)
{
	int i;
	char *ptr = NULL;

	for (i = 0; i < BLE_BD_ADDR_MAX_LEN; i++) {
		addr[i] = strtol(address, &ptr, 16);
		if (ptr != NULL) {
			if (ptr[0] != ':')
				return;
			address = ptr + 1;
		}
	}
}

static void addr_type_to_string(char *address, const uint8_t *addr)
{
	if (addr == NULL) {
		strncpy(address, "NULL", BLE_BD_ADDR_STR_LEN);
		return;
	}

	snprintf(address, BLE_BD_ADDR_STR_LEN + 1, "%02x:%02x:%02x:%02x:%02x:%02x",
			addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

}


static void utc_cb_charact_a_1(ble_server_attr_cb_type_e type, ble_conn_handle conn_handle, ble_attr_handle attr_handle, void *arg, uint16_t result, uint16_t pending)
{
	char *arg_str = "None";

	if (arg != NULL) {
		arg_str = (char *)arg;
	}

	BLE_LOGD("[CHAR_A_1][%s] type : %d / handle : %d / attr : %02x / result : %d / pending : %d\n", arg_str, type, conn_handle, attr_handle, result, pending);
}

static void utc_cb_desc_b_1(ble_server_attr_cb_type_e type, ble_conn_handle conn_handle, ble_attr_handle attr_handle, void *arg, uint16_t result, uint16_t pending)
{
	char *arg_str = "None";
	if (arg != NULL) {
		arg_str = (char *)arg;
	}
	BLE_LOGD( "[DESC_A_1][%s] type : %d / handle : %d / attr : %02x / result : %d / pending : %d\n", arg_str, type, conn_handle, attr_handle, result, pending);
}


static void perfs_ble_reset_data(void)
{
	g_perf_data.packet_size = 0;
	g_perf_data.packet_count = 0;

	g_perf_data.start_time.tv_sec = 0;
	g_perf_data.start_time.tv_usec = 0;
	g_perf_data.end_time.tv_sec = 0;
	g_perf_data.end_time.tv_usec = 0;
}

static int perfs_ble_get_result(void)
{
	struct timeval diff_time = {0, 0};
	double total_time = 0;
	double speed = 0;

	BLE_LOGD( "======== Bandwidth Analysis =================\n");
        BLE_LOGD( "start time: %ld sec %ld usec \n", g_perf_data.start_time.tv_sec, g_perf_data.start_time.tv_usec);
        BLE_LOGD( "end time: %ld sec %ld usec \n", g_perf_data.end_time.tv_sec, g_perf_data.end_time.tv_usec);

	diff_time.tv_sec = g_perf_data.end_time.tv_sec - g_perf_data.start_time.tv_sec;
	diff_time.tv_usec = g_perf_data.end_time.tv_usec - g_perf_data.start_time.tv_usec;
	if (diff_time.tv_usec < 0) {
		diff_time.tv_sec = diff_time.tv_sec - 1;
		diff_time.tv_usec = diff_time.tv_usec + 1000000;
	}

        BLE_LOGD( "diff time: %ld sec %ld usec \n", diff_time.tv_sec, diff_time.tv_usec);
	
	total_time = diff_time.tv_sec + (double)diff_time.tv_usec / 1000000;
	speed = g_perf_data.packet_size * g_perf_data.packet_count * 8 / total_time;

	BLE_LOGD( "=================================================\n");
	BLE_LOGD( "Packet Count  : % d \n", g_perf_data.packet_count);
	BLE_LOGD( "Packet Size   : % d byte\n", g_perf_data.packet_size);
	BLE_LOGD( "Total time    : %.2lf ms \n", total_time * 1000);
	BLE_LOGD( "data received : %ld byte \n", g_perf_data.packet_size * g_perf_data.packet_count);
	BLE_LOGD( "Bandwidth     : %.2lf Kbps \n", speed / 1024);
	BLE_LOGD( "=================================================\n");
	
	return 0;
}

static void ble_peri_cb_charact_rmc_sync(ble_server_attr_cb_type_e type, ble_conn_handle conn_handle, ble_attr_handle attr_handle, void* arg, uint16_t result, uint16_t pending)
{
	uint8_t buf[256] = { 0, };
	ble_data blue_data = { buf, sizeof(buf) };
	struct perfs_data_t *p_data = (struct perfs_data_t *)arg;

	ble_result_e ret = ble_server_attr_get_data(attr_handle, &blue_data);
	if (ret != BLE_MANAGER_SUCCESS) {
		BLE_LOGD( "[RMC_SYNC] Fail to get attr data\n");
		return;
	}

	if (blue_data.data[0] == FIRST_PACKET) {
		perfs_ble_reset_data();	
		p_data->packet_size = blue_data.length;
        	gettimeofday(&p_data->start_time, NULL);
        	BLE_LOGD( "start time: %ld sec %ld usec \n", p_data->start_time.tv_sec, p_data->start_time.tv_usec);
	}

	p_data->packet_count ++;

	if (type == BLE_SERVER_ATTR_CB_WRITING_NO_RSP || type == BLE_SERVER_ATTR_CB_WRITING) {
		if (blue_data.data[0] == blue_data.data[p_data->packet_size - 1]) {
			BLE_LOGD( "[WRITE] Success [%d] : %d \n" , blue_data.length, p_data->packet_count);
		} else {
			BLE_LOGD( "[WRITE] Fail [%d] : %d \n" , blue_data.length, p_data->packet_count);
		}
	} else {
		BLE_LOGD( "[READ] not yet\n");
	}

	if (blue_data.data[0] == LAST_PACKET) {
		BLE_LOGD( "[WRITE] ========================\n");
		BLE_LOGD( "[WRITE]        Write Done.\n");
		BLE_LOGD( "[WRITE] ========================\n");

		/* end time on receive of packet */
	       	gettimeofday(&p_data->end_time, NULL);

		perfs_ble_get_result();
	}
}

static ble_server_gatt_t gatt_profile[] = {
	{.type = BLE_SERVER_GATT_SERVICE, .uuid = {0x12, 0xB6, 0x6E, 0x45, 0xA7, 0x68, 0x9D, 0x8D, 0x9A, 0x40, 0x17, 0x2B, 0xE9, 0xCB, 0xF2, 0x13}, .uuid_length = 16,
		.attr_handle = BLE_APP_HANDLE_SERVICE_0,},

	{.type = BLE_SERVER_GATT_CHARACT, .uuid = {0x99, 0xC7, 0xAA, 0xE7, 0xF8, 0x9A, 0xCB, 0x88, 0x43, 0x4C, 0x44, 0xCF, 0x0D, 0x5B, 0xDA, 0xF2}, .uuid_length = 16,
		.property =  BLE_ATTR_PROP_RWN|BLE_ATTR_PROP_WRITE_NO_RSP, .permission = BLE_ATTR_PERM_R_PERMIT|BLE_ATTR_PERM_W_PERMIT,
		.attr_handle = BLE_STATE_MANAGER_RMC_HANDLE_KEY_COMMAND, .cb = utc_cb_charact_a_1, .arg = "char_1"},

	{.type = BLE_SERVER_GATT_DESC, .uuid = {0x02, 0x29}, .uuid_length = 2,
		.permission = BLE_ATTR_PERM_R_PERMIT|BLE_ATTR_PERM_W_PERMIT,
		.attr_handle = BLE_STATE_MANAGER_RMC_HANDLE_KEY_CCCD, .cb = utc_cb_desc_b_1, .arg = "desc_1"},

	{.type = BLE_SERVER_GATT_SERVICE, .uuid = {0xAD, 0xB6, 0x6E, 0x45, 0xA7, 0x68, 0x9D, 0x8D, 0x9A, 0x40, 0x17, 0x2B, 0xE9, 0xCB, 0xF2, 0x13}, .uuid_length = 16,
		.attr_handle = BLE_APP_HANDLE_SERVICE_1,},

	{.type = BLE_SERVER_GATT_CHARACT, .uuid = {0x04, 0xC7, 0xAA, 0xE7, 0xF8, 0x9A, 0xCB, 0x88, 0x43, 0x4C, 0x44, 0xCF, 0x0D, 0x5B, 0xDA, 0xF2}, .uuid_length = 16,
		.property =  BLE_ATTR_PROP_RWN|BLE_ATTR_PROP_WRITE_NO_RSP, .permission = BLE_ATTR_PERM_R_PERMIT|BLE_ATTR_PERM_W_PERMIT,
		.attr_handle = BLE_APP_HANDLE_CHAR_RMC_KEY, .cb = utc_cb_charact_a_1, .arg = "char_2"},

	{.type = BLE_SERVER_GATT_DESC, .uuid = {0x02, 0x29}, .uuid_length = 2,
		.permission = BLE_ATTR_PERM_R_PERMIT|BLE_ATTR_PERM_W_PERMIT,
		.attr_handle = BLE_APP_HANDLE_DESC_RMC_KEY, .cb = utc_cb_desc_b_1, .arg = "desc_2"},


	{.type = BLE_SERVER_GATT_SERVICE, .uuid = {0xF4, 0x7A, 0x07, 0x08, 0xFD, 0xC7, 0x9D, 0xB5, 0xFF, 0x4E, 0x85, 0xDE, 0x48, 0x80, 0xFE, 0xA2}, .uuid_length = 16,
		.attr_handle = BLE_APP_HANDLE_SERVICE_2,},

	{.type = BLE_SERVER_GATT_CHARACT, .uuid = {0x06, 0xC7, 0xAA, 0xE7, 0xF8, 0x9A, 0xCB, 0x88, 0x43, 0x4C, 0x44, 0xCF, 0x0D, 0x5B, 0xDA, 0xBB}, .uuid_length = 16,
		.property =  BLE_ATTR_PROP_RWN|BLE_ATTR_PROP_WRITE_NO_RSP, .permission = BLE_ATTR_PERM_R_PERMIT|BLE_ATTR_PERM_W_PERMIT,
		.attr_handle = BLE_APP_HANDLE_CHAR_RMC_SYNC, .cb = ble_peri_cb_charact_rmc_sync, .arg = &g_perf_data},
};

static void ble_server_connected_cb(ble_conn_handle con_handle, ble_server_connection_type_e conn_type, uint8_t mac[BLE_BD_ADDR_MAX_LEN], uint8_t adv_handle)
{
	char address_str[BLE_BD_ADDR_STR_LEN + 1] = {0,};

	BLE_LOGD( "conn : %d / conn_type : %s(%d) / adv_handle : %d\n", con_handle, get_conn_type_str(conn_type), conn_type, adv_handle);

	addr_type_to_string(address_str, mac);
	BLE_LOGD( "device address [%s] \n", address_str);

	if (conn_type == BLE_SERVER_DISCONNECTED) {
		perfs_ble_reset_data();
	}

	return;
}

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


static void perfs_ble_usage(void)
{
	BLE_LOGD( "BLE Perfs Usage: \n\n" \
		 " ble_perfs start \n" \
		 " ble_perfs adv \n" \
		 " ble_perfs stop \n" \
		 " ble_perfs result \n");
}

static int perfs_ble_server_init(void)
{
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	ble_result_e ble_ret = BLE_MANAGER_FAIL;

	/*1. Enable WiFi
	2. Enable BLE Manager
	*/

	res = wifi_manager_init(&g_wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		BLE_LOGD( "Fail to init wifi manager [%d] \n", res);
		return -1;
	}

	usleep(USLEEP_USEC);

	ble_ret = ble_manager_init(&server_config);
	if (ble_ret != BLE_MANAGER_SUCCESS) {
		if (ble_ret == BLE_MANAGER_ALREADY_WORKING) {
			BLE_LOGD( "ble init is already done \n");
		} 
		BLE_LOGD( "Fail to init ble manager \n");
		return -1;
	}

	g_is_ble_serv_init = 1;
	usleep(USLEEP_USEC);

	return 0;
}

static int perfs_ble_server_start(void)
{
	ble_data blue_data[1] = { 0, };
	ble_result_e ret = BLE_MANAGER_FAIL;

	if (g_is_ble_serv_init != 1) {
		BLE_LOGD( "ble not initialised \n");
		return -1;
	}

	/*3. Server Start = start advertising*/
	blue_data->data = g_adv_raw;
	blue_data->length = sizeof(g_adv_raw);

	ret = ble_server_set_adv_data(blue_data);
	if (ret != BLE_MANAGER_SUCCESS) {
		BLE_LOGD( "Fail to set adv raw data ret:[%d]\n", ret);
		return -1;
	}
	BLE_LOGD( "Set adv raw data ... ok\n");

	blue_data->data = g_adv_resp;
	blue_data->length = sizeof(g_adv_resp);

	ret = ble_server_set_adv_resp(blue_data);
	if (ret != BLE_MANAGER_SUCCESS) {
		BLE_LOGD( "Fail to set adv resp data ret:[%d]\n", ret);
		return -1;
	}
	BLE_LOGD( "Set adv resp data ... ok\n");

	ret = ble_server_start_adv();
	if (ret != BLE_MANAGER_SUCCESS) {
		BLE_LOGD( "Fail to start adv ret:[%d]\n", ret);
		return -1;
	}
	BLE_LOGD( "Start adv ... ok\n");

	return 0;
}

static int perfs_ble_deinit(void)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	res = wifi_manager_deinit();
	if (res != WIFI_MANAGER_SUCCESS) {
		BLE_LOGD( "Fail to deinit wifi manager [%d] \n", res);
		return -1;
	}

	ret = ble_manager_deinit();
	if (ret != BLE_MANAGER_SUCCESS) {
		BLE_LOGD( "Fail to deinit ble ret:[%d]\n", ret);
		return -1;
	}

	perfs_ble_reset_data();

	return 0;
}

static int perfs_ble_parse_arguments(int argc, char **argv)
{
	BLE_LOGD( "parse arguments \n");

	if (argc < 2) {
		BLE_LOGD( "No of arguments are less than 2 \n");
		return -1;
	}

	int ret = 0;

	if (strncmp(argv[1], "start", 5) == 0) {
		BLE_LOGD( "GATT server init \n");
		ret = perfs_ble_server_init();
		if (ret < 0) {
			BLE_LOGD( "Fail to init \n");
			return -1;
		}
	} else if (strncmp(argv[1], "adv", 4) == 0) {
		ret = perfs_ble_server_start();
		if (ret < 0) {
			BLE_LOGD( "Fail to start \n");
			return -1;
		}

	} else if (strncmp(argv[1], "result", 6) == 0) {
		BLE_LOGD( "GATT server  performance result \n");

		ret = perfs_ble_get_result();
		if (ret < 0) {
			BLE_LOGD( "Fail to get performance result \n");
			return -1;
		}
	} else if (strncmp(argv[1], "stop", 4) == 0 ) {
		BLE_LOGD( "GATT server deinit \n");	
		ret = perfs_ble_deinit();
		if (ret < 0) {
			BLE_LOGD( "Failed to stop GATT Server \n");
			return -1;
		}
	} else {
		BLE_LOGD( "Unknown option  \n");
		return -1;
	}

	return 0;
}

/****************************************************************************
 * ble_perfs_main
 ****************************************************************************/

int ble_perfs_main(int argc, char *argv[])
{
	int rc = 0;

	BLE_LOGD( "- BLE Server: Performance Test -\n");

	rc = perfs_ble_parse_arguments(argc, argv);
	if (rc < 0) {
		perfs_ble_usage();
	}

	return 0;
}
