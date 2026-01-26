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
#include <pthread.h>
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

/* These values can be modified as a developer wants. */
static uint8_t ble_filter[] = { 0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18 };

static uint8_t g_adv_raw_1[] = {
	0x02, 0x01, 0x04, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x17
};
static uint8_t g_adv_raw_2[] = {
	0x02, 0x02, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18
};

static uint8_t g_adv_resp_1[] = {
	0x11, 0x08, '1', 'I', 'Z', 'E', 'N', 'R', 'T', ' ', 'T', 'E', 'S', 'T', '(', '0', '2', ')',
};
static uint8_t g_adv_resp_2[] = {
	0x11, 0x09, '2', 'I', 'Z', 'E', 'N', 'R', 'T', ' ', 'T', 'E', 'S', 'T', '(', '0', '2', ')',
};

const uint8_t def_ext_addr_val[BLE_BD_ADDR_MAX_LEN] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };

const uint8_t def_ext_adv_data[] = {
	0x02,			// Flags
	0x01,			// RTK_BT_LE_GAP_ADTYPE_FLAGS,
	0x01 | 0x04,	// RTK_BT_LE_GAP_ADTYPE_FLAGS_LIMITED | RTK_BT_LE_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

	0x12,			// Local name
	0x09,			// RTK_BT_LE_GAP_ADTYPE_LOCAL_NAME_COMPLETE,
	'0', 'E', 'K', '_', 'B', 'T', '_', 'P', 'E', 'R', 'I', 'P', 'H', 'E', 'R', 'A', 'L',
};

const uint8_t def_ext_resp_data[] = {
	0x02,			// Flags
	0x01,			// RTK_BT_LE_GAP_ADTYPE_FLAGS,
	0x01 | 0x04,	// RTK_BT_LE_GAP_ADTYPE_FLAGS_LIMITED | RTK_BT_LE_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

	0x12,			// Local name
	0x09,			// RTK_BT_LE_GAP_ADTYPE_LOCAL_NAME_COMPLETE,
	'R', 'E', 'S', 'P', '0', '_', '_', 'P', 'E', 'R', 'I', 'P', 'H', 'E', 'R', 'A', 'L',
};

static uint8_t coc_data[10] = { 0x67, 1, 2, 3, 4, 5, 6, 7, 0x88 };

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
	RMC_LOG(RMC_CLIENT_TAG, "attr : %x // len : %d\n", attr_handle, read_result->length);
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

static void ble_device_passkey_display_cb(ble_client_ctx *ctx, uint32_t passkey, ble_conn_handle con_handle)
{
	RMC_LOG(RMC_CLIENT_TAG, "'%s' is called[%p]\n", __FUNCTION__, ctx);
	RMC_LOG(RMC_CLIENT_TAG, "passkey %ld, con_handle %d\n", passkey, con_handle);
	return;
}

static void ble_device_pair_bond_cb(ble_client_ctx *ctx, uint32_t bond_pair, ble_conn_handle con_handle)
{
	RMC_LOG(RMC_CLIENT_TAG, "'%s' is called[%p]\n", __FUNCTION__, ctx);
	RMC_LOG(RMC_CLIENT_TAG, "bond_pair 0x%x, con_handle %d\n", bond_pair, con_handle);
	return;
}

void restart_server(void) {
	ble_result_e ret = BLE_MANAGER_FAIL;
	ble_data data[1] = { 0, };
	data->data = g_adv_raw_1;
	data->length = sizeof(g_adv_raw_1);

	ret = ble_server_set_adv_data(data);
	if (ret != BLE_MANAGER_SUCCESS) {
		RMC_LOG(RMC_SERVER_TAG, "Fail to set adv raw data ret:[%d]\n");
		return;
	}
	RMC_LOG(RMC_SERVER_TAG, "Set adv raw data ... ok\n");

	data->data = g_adv_resp_1;
	data->length = sizeof(g_adv_resp_1);

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

int multi_adv_stop = 0;
static void *multi_adv_thread(void *data)
{
	ble_result_e ret = BLE_MANAGER_FAIL;
	uint8_t adv_id_0 = 0;
	uint8_t adv_id_1 = 1;
	int max_count = *(int *)(data);
	int adv_counter = 0;

	RMC_LOG(RMC_SERVER_TAG, "Multi Adv Start, ID: %d, %d, for %d times\n", adv_id_0, adv_id_1, max_count);
	while (1) {
		adv_counter++;
		if (adv_counter == max_count || multi_adv_stop == 0xFFFF) {
			multi_adv_stop = 0;
			break;
		}

		usleep(80000);
		ret = ble_server_one_shot_adv(adv_id_0);

		usleep(20000);
		ret = ble_server_one_shot_adv(adv_id_1);
	}
	RMC_LOG(RMC_SERVER_TAG, "Multi Adv Stop, ID: %d, %d\n", adv_id_0, adv_id_1);
}


static void ble_server_connected_cb(ble_conn_handle con_handle, ble_server_connection_type_e conn_type, uint8_t mac[BLE_BD_ADDR_MAX_LEN], uint8_t adv_handle)
{
	RMC_LOG(RMC_SERVER_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_SERVER_TAG, "conn : %d / conn_type : %d\n", con_handle, conn_type);
	RMC_LOG(RMC_SERVER_TAG, "conn mac : %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	RMC_LOG(RMC_SERVER_TAG, "adv_handle : %d\n", adv_handle);
	if (conn_type == BLE_SERVER_DISCONNECTED) {
		restart_server();
	}
	adv_handle = 0xff;
	return;
}

static void ble_server_disconnected_cb(ble_conn_handle con_handle, uint16_t cause)
{
	RMC_LOG(RMC_SERVER_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_SERVER_TAG, "conn : %d \n", con_handle);
	RMC_LOG(RMC_SERVER_TAG, "cause : %d \n", cause);
	return;
}

static void ble_server_mtu_update_cb(ble_conn_handle con_handle, uint16_t mtu_size)
{
	RMC_LOG(RMC_SERVER_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_SERVER_TAG, "conn : %d\n", con_handle);
	RMC_LOG(RMC_SERVER_TAG, "mtu_size : %d\n", mtu_size);
	return;
}

static void ble_server_passkey_display_cb(uint32_t passkey, ble_conn_handle con_handle)
{
	RMC_LOG(RMC_SERVER_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_SERVER_TAG, "passkey %ld, con_handle : %d\n", passkey, con_handle);
	return;
}

static void ble_server_pair_bond_cb(uint32_t bond_pair, ble_conn_handle con_handle)
{
	RMC_LOG(RMC_SERVER_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_SERVER_TAG, "bond_pair 0x%x, con_handle : %d\n", bond_pair, con_handle);
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

static void ble_coc_reg_psm_cb(uint16_t le_psm, uint16_t err)
{
	RMC_LOG(RMC_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_TAG, "le_psm %d, err %d\n", le_psm, err);
	return;
}

static void ble_coc_set_sec_cb(uint16_t err)
{
	RMC_LOG(RMC_TAG, "'%s' is called[%d]\n", __FUNCTION__, err);
	return;
}

static void ble_coc_con_cb(uint16_t con_handle, uint16_t cid, uint16_t err)
{
	RMC_LOG(RMC_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_TAG, "con_handle %d, cid %d, err %d\n", con_handle, cid, err);
	return;
}

static void ble_coc_discon_cb(uint16_t con_handle, uint16_t cid, uint16_t err)
{
	RMC_LOG(RMC_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_TAG, "con_handle %d, cid %d, err %d\n", con_handle, cid, err);
	return;
}

static void ble_coc_send_cb(uint16_t con_handle, uint16_t cid, uint16_t err, uint8_t credit)
{
	RMC_LOG(RMC_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_TAG, "con_handle %d, cid %d, err %d, credit %d\n", con_handle, cid, err, credit);
	return;
}

static void ble_coc_recv_cb(uint16_t con_handle, uint16_t cid, ble_data *read_result)
{
	RMC_LOG(RMC_TAG, "'%s' is called\n", __FUNCTION__);
	RMC_LOG(RMC_TAG, "con_handle %d, cid %d, len %d\n", con_handle, cid, read_result->length);
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

static ble_le_coc_callback_list le_coc_config = {
	ble_coc_reg_psm_cb,
	ble_coc_set_sec_cb,
	ble_coc_con_cb,
	ble_coc_discon_cb,
	ble_coc_send_cb,
	ble_coc_recv_cb,
};

static ble_client_callback_list client_config = {
	ble_device_disconnected_cb,
	ble_device_connected_cb,
	ble_operation_notification_cb,
	NULL,
	ble_device_passkey_display_cb,
	ble_device_pair_bond_cb
};

static ble_server_init_config server_config = {
	ble_server_connected_cb,
	ble_server_disconnected_cb,
	ble_server_mtu_update_cb,
	ble_server_passkey_display_cb,
	ble_server_pair_bond_cb,
	true,
	gatt_profile, sizeof(gatt_profile) / sizeof(ble_server_gatt_t)
};

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

static uint8_t ctoi(char c)
{
	if ((c >= 'A') && (c <= 'F')) {
		return (c - 'A' + 0x0A);
	}

	if ((c >= 'a') && (c <= 'f')) {
		return (c - 'a' + 0x0A);
	}

	if ((c >= '0') && (c <= '9')) {
		return (c - '0' + 0x00);
	}

	printf("[%s]Error: Hex char is invalid !!!\r\n", __func__);
	return 0xFF;
}

bool hexdata_str_to_bd_addr(char *str, uint8_t *addr_buf, uint8_t buf_len)
{
	uint32_t str_len = strlen(str);
	uint32_t n = 0;
	uint8_t num = 0;
	uint8_t idx = 0;

	if (str_len != 2 * BLE_BD_ADDR_MAX_LEN || buf_len < BLE_BD_ADDR_MAX_LEN) {
		printf("[%s]Error: Invalid bd addr string\r\n",__func__);
		return false;
	}

	while (n < str_len) {
		if ((num = ctoi(str[n++])) == 0xFF) {		// High nibble
			return false;
		}
		addr_buf[idx] = num << 4;

		if ((num = ctoi(str[n++])) == 0xFF) {		// Low nibble
			return false;
		}
		addr_buf[idx] |= num;
		idx++;
	}
	return true;
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
		ctx_count = 0;
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

		if (argc == 3 && strncmp(argv[2], "auto", 5) == 0) {
			ret = ble_connect_common(ctx, addr, true);
		} else {
			ret = ble_connect_common(ctx, addr, false);
		}
		RMC_LOG(RMC_CLIENT_TAG, "Re-Connect Result : %d\n", ret);
		if (ret == 0) {
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
		if (argc == 4) {
			if (strncmp(argv[2], "start", 6) == 0) {
				int conn_handle = atoi(argv[3]);
				ret = ble_manager_start_bond(conn_handle);
			}
		}
		RMC_LOG(RMC_CLIENT_TAG, "bond command done.\n");
	}

	if (strncmp(argv[1], "mac", 4) == 0) {
		uint8_t mac[BLE_BD_ADDR_MAX_LEN];

		ret = ble_manager_get_mac_addr(mac);
		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_CLIENT_TAG, "get mac fail[%d]\n", ret);
			goto ble_rmc_done;
		}

		RMC_LOG(RMC_CLIENT_TAG, "BLE mac : %02x:%02x:%02x:%02x:%02x:%02x\n",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
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
			RMC_LOG(RMC_CLIENT_TAG, "Scan stop\n");
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

		for (int i = 0; i < RMC_MAX_CONNECTION; i++) {
			if (ctx_list[i] != NULL && ctx_list[i]->conn_handle == id) {
				ret = ble_client_disconnect(ctx_list[i]);
				break;
			}
		}

		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_CLIENT_TAG, "disconnect fail[%d]\n", ret);
			goto ble_rmc_done;
		}
		RMC_LOG(RMC_CLIENT_TAG, "disconnect ok\n");
	}

	if (strncmp(argv[1], "disconns", 9) == 0) {
		if (argc < 3) {
			goto ble_rmc_done;
		}
		int id = atoi(argv[2]);
		
		ret = ble_server_disconnect(id);
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
		ble_addr addr_info = { 0, };
		hexdata_str_to_bd_addr(argv[2], addr_info.mac, 6); 

		// 3. create ctx
		ctx = ble_client_create_ctx(&client_config);
		if (ctx == NULL) {
			RMC_LOG(RMC_CLIENT_TAG, "create ctx fail\n");
			goto ble_rmc_done;
		}

		if (argc == 3 && strncmp(argv[2], "r", 2) == 0) {
			addr_info.type = BLE_ADDR_TYPE_RANDOM_STATIC;
		} else {
			addr_info.type = BLE_ADDR_TYPE_PUBLIC;			/* Default connect to public */
		}
		RMC_LOG(RMC_CLIENT_TAG, "Try to connect! [%02x:%02x:%02x:%02x:%02x:%02x]\n",
			addr_info.mac[0], addr_info.mac[1], addr_info.mac[2], addr_info.mac[3], addr_info.mac[4], addr_info.mac[5]);
		RMC_LOG(RMC_CLIENT_TAG, "Address type : %d\n", addr_info.type);

		if (argc == 3 && strncmp(argv[2], "auto", 5) == 0) {
			/* For initial connection, remove bonded data all */
			ret = ble_connect_common(ctx, &addr_info, true);
		} else {
			ret = ble_connect_common(ctx, &addr_info, false);
		}
		RMC_LOG(RMC_CLIENT_TAG, "Connect Result : %d\n", ret);
		if (ret == 0) {
			RMC_LOG(RMC_CLIENT_TAG, "Connect Success [ID : %d]\n", ctx_count);
			ctx_list[ctx_count++] = ctx;
		}
	}

	//connection parameter update, use this when AI-Lite is slave
	if (strncmp(argv[1], "updates", 8) == 0) {
		ble_conn_handle conn_handle = 24;
		ble_conn_param conn_param;
		conn_param.min_conn_interval = 0x0010;
		conn_param.max_conn_interval = 0x0010;
		conn_param.slave_latency = 2;
		conn_param.supervision_timeout = 0x00aa;
		conn_param.role = BLE_SLAVE_CONN_PARAM_UPDATE;

		ble_manager_conn_param_update(&conn_handle, &conn_param);
	}

	//connection parameter update, use this when TPdual is master
	if (strncmp(argv[1], "updatem", 8) == 0) {
		ble_conn_handle conn_handle = 16;
		ble_conn_param conn_param;
		conn_param.min_conn_interval = 0x0010;
		conn_param.max_conn_interval = 0x0010;
		conn_param.slave_latency = 2;
		conn_param.supervision_timeout = 0x00aa;
		conn_param.role = BLE_SLAVE_CONN_PARAM_UPDATE;

		ble_manager_conn_param_update(&conn_handle, &conn_param);
	}

	if (strncmp(argv[1], "advon", 6) == 0) {
		ret = ble_server_one_shot_adv_init();
		uint8_t type1 = 0;
		uint8_t adv_id_0 = 0;
		ble_data adv_data_1[1] = { 0x99, };
		ble_data scan_rsp_data_1[1] = { 0x98, };
		ble_data adv_data_2[1] = { 0x99, };
		ble_data scan_rsp_data_2[1] = { 0x98, };

		adv_data_1->data = g_adv_raw_1;
		adv_data_1->length = sizeof(g_adv_raw_1);
		scan_rsp_data_1->data = g_adv_resp_1;
		scan_rsp_data_1->length = sizeof(g_adv_resp_1);
		ret = ble_server_one_shot_adv_set(adv_id_0, adv_data_1, scan_rsp_data_1, type1);
		RMC_LOG(RMC_SERVER_TAG, "Adv id: %d set ret: [%d]\n", adv_id_0, ret);

		uint8_t type3 = 3;
		uint8_t adv_id_1 = 1;
		adv_data_2->data = g_adv_raw_2;
		adv_data_2->length = sizeof(g_adv_raw_2);
		scan_rsp_data_2->data = g_adv_resp_2;
		scan_rsp_data_2->length = sizeof(g_adv_resp_2);
		ret = ble_server_one_shot_adv_set(adv_id_1, adv_data_2, scan_rsp_data_2, type3);
		RMC_LOG(RMC_SERVER_TAG, "Adv id: %d set ret: [%d]\n", adv_id_1, ret);
	}

	if (strncmp(argv[1], "advoff", 7) == 0) {
		multi_adv_stop = 0xFFFF;					/* Stop multi adv loop */
		while(multi_adv_stop) {						/* Wait for Adv to stop */
			usleep(50000);
		}
		ret = ble_server_one_shot_adv_deinit();
		RMC_LOG(RMC_SERVER_TAG, "Advoff ret: [%d]\n", ret);
	}

	if (strncmp(argv[1], "adv", 4) == 0) {
		pthread_t pid;
		int max_count = 100;
		if (argc == 3) {
			max_count = atoi(argv[2]);
		}
		/* Create multi Adv pthread */
		if ((ret = pthread_create(&pid, NULL, (pthread_startroutine_t)multi_adv_thread, (void*)&max_count)) != 0) {
			printf("%s: pthread_create failed, status=%d\n", __func__, ret);
		} else {
			pthread_detach(pid);
		}
	}

	if (strncmp(argv[1], "passkeycfm", 11) == 0) {
		uint8_t conn_handle = 0;
		uint8_t confirm = 0;
		if (argc >= 4) {
			conn_handle = atoi(argv[2]);
			confirm = atoi(argv[3]);
		}
		ret = ble_manager_passkey_confirm(conn_handle, confirm);
		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_SERVER_TAG, "Passkey confirm fail: [%d]\n", ret);
		} else {
			RMC_LOG(RMC_SERVER_TAG, "Passkey confirm OK\n");
		}
	}

	if (strncmp(argv[1], "secureparam", 12) == 0) {
		ble_sec_param sec_param;
		// RTK_IO_CAP_DISPALY_ONLY     = 0x00,		/*!< 0x00 DisplayOnly */
		// RTK_IO_CAP_DISPLAY_YES_NO   = 0x01,		/*!< 0x01 DisplayYesNo */
		// RTK_IO_CAP_KEYBOARD_ONLY    = 0x02,		/*!< 0x02 KeyboardOnly */
		// RTK_IO_CAP_NO_IN_NO_OUT     = 0x03,		/*!< 0x03 NoInputNoOutput */
		// RTK_IO_CAP_KEYBOARD_DISPALY = 0x04,		/*!< 0x04 KeyboardDisplay */
		sec_param.io_cap = atoi(argv[2]);
		sec_param.oob_data_flag = atoi(argv[3]);
		sec_param.bond_flag = atoi(argv[4]);
		sec_param.mitm_flag = atoi(argv[5]);
		sec_param.sec_pair_flag = atoi(argv[6]);
		sec_param.sec_pair_only_flag = atoi(argv[7]);
		sec_param.use_fixed_key = atoi(argv[8]);
		sec_param.fixed_key = atoi(argv[9]);
		ret = ble_manager_set_secure_param(&sec_param);
		if (ret != BLE_MANAGER_SUCCESS) {
			RMC_LOG(RMC_SERVER_TAG, "set secure param fail: [%d]\n", ret);
		} else {
			RMC_LOG(RMC_SERVER_TAG, "set secure param OK\n");
		}
	}

	if (strncmp(argv[1], "powerset", 9) == 0) {
		uint8_t power = atoi(argv[2]);
		ble_server_set_adv_tx_power(power);
	}

	/* Server Test */
	if (strncmp(argv[1], "server", 7) == 0) {
		RMC_LOG(RMC_SERVER_TAG, " [ Server Control ]\n");

		if (argc == 3 && strncmp(argv[2], "set", 4) == 0) {
			ble_data data[1] = { 0, };

			data->data = g_adv_raw_1;
			data->length = sizeof(g_adv_raw_1);

			ret = ble_server_set_adv_data(data);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to set adv raw data[%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "Set adv raw data ... ok\n");

			data->data = g_adv_resp_1;
			data->length = sizeof(g_adv_resp_1);

			ret = ble_server_set_adv_resp(data);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to set adv resp data[%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "Set adv resp data ... ok\n");
		}

		if (argc > 3 && strncmp(argv[2], "type", 5) == 0) {
			ble_adv_type_e adv_type;
			ble_addr_type_e addr_type = BLE_ADDR_TYPE_RANDOM_STATIC;
			uint8_t addrr [BLE_BD_ADDR_MAX_LEN] = { 0 };
			ble_addr addr_info;
			if (argc == 6 && strncmp(argv[3], "dir", 4) == 0) {
				adv_type = BLE_ADV_TYPE_DIRECT;
				RMC_LOG(RMC_SERVER_TAG, "Adv type = BLE_ADV_TYPE_DIRECT\n");
				if (strncmp(argv[4], "p", 5) == 0) {
					addr_type = BLE_ADDR_TYPE_PUBLIC;
					RMC_LOG(RMC_SERVER_TAG, "Addr type = BLE_ADDR_TYPE_PUBLIC\n");
				} else {
					RMC_LOG(RMC_SERVER_TAG, "Addr type = BLE_ADDR_TYPE_RANDOM_STATIC\n");
				}
				hexdata_str_to_bd_addr(argv[5], addrr, BLE_BD_ADDR_MAX_LEN);
				RMC_LOG(RMC_CLIENT_TAG, "Addr = [%02x:%02x:%02x:%02x:%02x:%02x]\n",
					addrr[0], addrr[1], addrr[2], addrr[3], addrr[4], addrr[5] );

			} else if (strncmp(argv[3], "ind", 4) == 0) {
				adv_type = BLE_ADV_TYPE_IND;
				RMC_LOG(RMC_SERVER_TAG, "Adv type = BLE_ADV_TYPE_IND\n");
			} else if (strncmp(argv[3], "scn", 4) == 0) {
				adv_type = BLE_ADV_TYPE_SCAN_IND;
				RMC_LOG(RMC_SERVER_TAG, "Adv type = BLE_ADV_TYPE_SCAN_IND\n");
			} else if (strncmp(argv[3], "ncon", 5) == 0) {
				adv_type = BLE_ADV_TYPE_NONCONN_IND;
				RMC_LOG(RMC_SERVER_TAG, "Adv type = BLE_ADV_TYPE_NONCONN_IND\n");
			} else if (strncmp(argv[3], "scnr", 5) == 0) {
				adv_type = BLE_ADV_TYPE_SCAN_RSP;
				RMC_LOG(RMC_SERVER_TAG, "Adv type = BLE_ADV_TYPE_SCAN_RSP\n");
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done;
			}
			memcpy(addr_info.mac, addrr, BLE_BD_ADDR_MAX_LEN);
			addr_info.type = addr_type;
			ble_server_set_adv_type(adv_type, &addr_info);
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

		if (argc == 4 && strncmp(argv[2], "cremuladv", 10) == 0) {
			uint8_t adv_handle = 0;
			if (argc > 2) {
				adv_handle = atoi(argv[3]);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done;
			}

			uint32_t adv_interval_int[2] = { 32, 32 };
			//uint8_t adv_event_prop = 0x01;		//LE_EXT_ADV_EXTENDED_ADV_CONN_UNDIRECTED
			//uint8_t adv_event_prop = 0x10;		//LE_EXT_ADV_LEGACY_ADV_NON_SCAN_NON_CONN_UNDIRECTED
			//uint8_t adv_event_prop = 0x12;		//LE_EXT_ADV_LEGACY_ADV_SCAN_UNDIRECTED
			uint8_t adv_event_prop = 0x13;			//LE_EXT_ADV_LEGACY_ADV_CONN_SCAN_UNDIRECTED
			//uint8_t adv_addr_type = 0;			//RTK_BT_LE_ADDR_TYPE_PUBLIC
			uint8_t adv_addr_type = 1;				//RTK_BT_LE_ADDR_TYPE_RANDOM
			uint8_t adv_addr[BLE_BD_ADDR_MAX_LEN] = { 0 };
			memcpy(adv_addr, def_ext_addr_val, BLE_BD_ADDR_MAX_LEN);

			RMC_LOG(RMC_SERVER_TAG, "adv_handle = [%d]\n", adv_handle);
			RMC_LOG(RMC_SERVER_TAG, "adv_interval_int = [%d]\n", adv_interval_int[0]);

			adv_addr[0] = adv_handle * 0x11;
			RMC_LOG(RMC_SERVER_TAG, "adv_addr = [%02x:%02x:%02x:%02x:%02x:%02x]\n", adv_addr[0], adv_addr[1], adv_addr[2], adv_addr[3], adv_addr[4], adv_addr[5]);
			RMC_LOG(RMC_SERVER_TAG, "adv_event_prop = [%d]\n", adv_event_prop);
			ret = ble_server_create_multi_adv(adv_event_prop,
											adv_interval_int, adv_addr_type,
											adv_addr, &adv_handle);

			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to create adv [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "create adv %d... ok\n", adv_handle);
		}

		if (argc == 4 && strncmp(argv[2], "delmuladv", 10) == 0) {
			uint8_t adv_handle = 0;
			if (argc > 2) {
				adv_handle = atoi(argv[3]);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done;
			}
			ret = ble_server_delete_multi_adv(adv_handle);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to delete adv [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "Stop delete ... ok\n");
		}

		if (argc == 4 && strncmp(argv[2], "setmuladv", 10) == 0) {
			uint8_t adv_data_len = 20;
			uint8_t adv_handle = 0;
			uint8_t ext_adv_data[BLE_ADV_RAW_DATA_MAX_LEN] = { 0 };
			uint8_t ext_resp_data[BLE_ADV_RAW_DATA_MAX_LEN] = { 0 };
			memcpy(ext_adv_data, def_ext_adv_data, sizeof(def_ext_adv_data));
			memcpy(ext_resp_data, def_ext_resp_data, sizeof(def_ext_resp_data));

			if (argc > 2) {
				adv_handle = atoi(argv[3]);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done;
			}

			ext_adv_data[5] = '0' + adv_handle;
			ext_resp_data[9] = '0' + adv_handle;
			RMC_LOG(RMC_SERVER_TAG, "def_ext_adv_data[5] = [%x]\n", def_ext_adv_data[5]);
			RMC_LOG(RMC_SERVER_TAG, "def_ext_resp_data[9] = [%x]\n", def_ext_resp_data[9]);

			adv_data_len = sizeof(def_ext_adv_data);
			ret = ble_server_set_multi_adv_data(adv_handle, adv_data_len, ext_adv_data);
			adv_data_len = sizeof(def_ext_resp_data);
			ret = ble_server_set_multi_resp_data(adv_handle, adv_data_len, ext_resp_data);

			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to set adv data [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, " set adv data ... ok\n");
		}

		if (argc == 4 && strncmp(argv[2], "stamuladv", 10) == 0) {
			uint8_t adv_handle = 0;
			if (argc > 2) {
				adv_handle = atoi(argv[3]);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done;
			}
			ret = ble_server_start_multi_adv(adv_handle);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to start adv [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "start adv ... ok\n");
		}

		if (argc == 4 && strncmp(argv[2], "stpmuladv", 10) == 0) {
			uint8_t adv_handle = 0;
			if (argc > 2) {
				adv_handle = atoi(argv[3]);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done; 
			}
			ret = ble_server_stop_multi_adv(adv_handle);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to stop adv [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "Stop adv ... ok\n");
		}
	}

	if (strncmp(argv[1], "coc", 4) == 0) {
		if (strncmp(argv[2], "init", 5) == 0) {
			RMC_LOG(RMC_SERVER_TAG, "coc init\n");
			ret = ble_manager_le_coc_init(&le_coc_config);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to init coc [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "coc init... ok\n");
		} else if (strncmp(argv[2], "reg_psm", 8) == 0) {
			uint8_t is_reg = 0;
			uint16_t psm = 0;
			if (argc > 4) {
				is_reg = atoi(argv[3]);
				psm = atoi(argv[4]);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done; 
			}
			RMC_LOG(RMC_SERVER_TAG, "is_reg %d psm %d\n", is_reg, psm);
			ret = ble_coc_register_psm(is_reg, psm);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to register psm [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "register psm... ok\n");
		} else if (strncmp(argv[2], "set_sec", 8) == 0) {
			uint8_t active = 0;
			uint16_t le_psm = 0;
			uint8_t sec_mode = 0;
			uint8_t key_size = 0;
			if (argc > 6) {
				active = atoi(argv[3]);
				le_psm = atoi(argv[4]);
				sec_mode = atoi(argv[5]);
				key_size = atoi(argv[6]);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "le_psm %d active %d sec_mode %d key_size %d\n", le_psm, active, sec_mode, key_size);
			ret = ble_coc_set_psm_security(le_psm, active, sec_mode, key_size);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to set coc psm security [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "set coc psm security... ok\n");
		} else if (strncmp(argv[2], "set_param", 8) == 0) {
			uint16_t value = 0;
			if (argc > 3) {
				value = atoi(argv[3]);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "value %d\n", value);
			ret = ble_coc_set_param(value);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to set coc param [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "set coc param... ok\n");
		} else if (strncmp(argv[2], "get_param", 8) == 0) {
			uint8_t param_type = 0;
			uint16_t cid = 0;
			uint16_t value = 0;
			if (argc > 4) {
				param_type = atoi(argv[3]);
				cid = atoi(argv[4]);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done;
			}
			ret = ble_coc_get_param(param_type, cid, &value);
			RMC_LOG(RMC_SERVER_TAG, "param_type %d cid %d value %d\n", param_type, cid, value);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to get coc param [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "get coc param... ok\n");
		} else if (strncmp(argv[2], "con", 8) == 0) {
			uint16_t conn_handle = 0;
			uint16_t le_psm = 0;
			if (argc > 4) {
				conn_handle = atoi(argv[3]);
				le_psm = atoi(argv[4]);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "conn_handle %d le_psm %d\n", conn_handle, le_psm);
			ret = ble_coc_connect(conn_handle, le_psm);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to coc connect [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "coc connect... ok\n");
		} else if (strncmp(argv[2], "discon", 11) == 0) {
			uint16_t cid = 0;
			if (argc > 3) {
				cid = atoi(argv[3]);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "cid %d\n", cid);
			ret = ble_coc_disconnect(cid);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to coc disconnect [%d]\n", ret);
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "coc disconnect... ok\n");
		} else if (strncmp(argv[2], "send", 5) == 0) {
			uint16_t cid = 0;
			uint16_t len = sizeof(coc_data);
			if (argc > 3) {
				cid = atoi(argv[3]);
			} else {
				RMC_LOG(RMC_SERVER_TAG, "Wrong argument\n");
				goto ble_rmc_done;
			}
			RMC_LOG(RMC_SERVER_TAG, "cid %d\n", cid);
			ret = ble_coc_send_data(cid, len, coc_data);
			if (ret != BLE_MANAGER_SUCCESS) {
				RMC_LOG(RMC_SERVER_TAG, "Fail to coc send [%d]\n", ret);
				goto ble_rmc_done;
			}
			coc_data[0]++;
			RMC_LOG(RMC_SERVER_TAG, "coc send... ok\n");
		}
	}

ble_rmc_done:
	RMC_LOG(RMC_CLIENT_TAG, "done\n");
	return 0;
}
