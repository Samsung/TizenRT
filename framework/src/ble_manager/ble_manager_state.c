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
#include <tinyara/config.h>
#include <string.h>
#include <tinyara/ble/ble_manager.h>
#include <ble_manager/ble_manager.h>
#include "ble_manager_event.h"
#include "ble_manager_msghandler.h"
#include "ble_manager_state.h"
#include "ble_manager_log.h"

static ble_client_ctx g_client_table[BLE_MAX_CONNECTION_COUNT] = { 0, };
// static ble_server_ctx g_server_ctx = { 0, };
static ble_scan_ctx g_scan_ctx = { 0, };
static blemgr_state_e g_manager_state = BLEMGR_UNINITIALIZED;

#define BLE_STATE_CHECK                                   \
	do {                                                  \
		if (g_manager_state != BLEMGR_INITIALIZED) { \
			BLE_LOG_INFO("[BLEMGR] is not running\n");    \
			ret = TRBLE_INVALID_STATE;                    \
			goto handle_req_done;                         \
		}                                                 \
	} while (0)

static ble_result_e _convert_ret(trble_result_e val)
{
	return (ble_result_e)val;
}

/*
 * public
 */
ble_result_e blemgr_handle_request(blemgr_msg_s *msg)
{
	trble_result_e ret = TRBLE_FAIL;

	BLE_LOG_INFO("[BLEMGR] T%d --> _handle_request[%d]\n", getpid(), msg->event);

	switch (msg->event) {
	case BLE_EVT_CMD_INIT: {
		int i;
		if (g_manager_state != BLEMGR_UNINITIALIZED) {
			BLE_LOG_INFO("[BLEMGR] already running\n");
			ret = TRBLE_ALREADY_WORKING;
			break;
		}
		trble_server_init_config *server = (trble_server_init_config *)msg->param;
		ret = ble_drv_init(server);
		if (ret != TRBLE_SUCCESS) {
			BLE_LOG_ERROR("[BLEMGR] init fail[%d]\n", ret);
			break;
		}

		for (i = 0; i < BLE_MAX_CONNECTION_COUNT; i++) {
			memset(&g_client_table[i], 0, sizeof(ble_client_ctx));
		}
		g_manager_state = BLEMGR_INITIALIZED;
		g_scan_ctx.state = BLE_SCAN_STOPPED;
	} break;

	case BLE_EVT_CMD_DEINIT: {
		BLE_STATE_CHECK;

		int i;
		ret = ble_drv_deinit();
		if (ret != TRBLE_SUCCESS) {
			BLE_LOG_ERROR("[BLEMGR] deinit fail[%d]\n", ret);
			break;
		}

		for (i = 0; i < BLE_MAX_CONNECTION_COUNT; i++) {
			g_client_table[i].state = BLE_CLIENT_NONE;
		}
		g_manager_state = BLEMGR_UNINITIALIZED;
		g_scan_ctx.state = BLE_SCAN_STOPPED;
	} break;

	case BLE_EVT_CMD_GET_MAC: {
		BLE_STATE_CHECK;

		uint8_t *mac = (uint8_t *)msg->param;
		ret = ble_drv_get_mac_addr(mac);
	} break;
	
	case BLE_EVT_CMD_GET_BONDED_DEV: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_bonded_device_list_s *list = (trble_bonded_device_list_s *)param->param[0];
		uint16_t *device_count = (uint16_t *)param->param[1];

		ret = ble_drv_get_bonded_device(list, device_count);
	} break;

	case BLE_EVT_CMD_DEL_BOND: {
		BLE_STATE_CHECK;

		uint8_t *addr = (uint8_t *)msg->param;
		ret = ble_drv_delete_bonded(addr);
	} break;

	case BLE_EVT_CMD_DEL_BOND_ALL: {
		BLE_STATE_CHECK;

		ret = ble_drv_delete_bonded_all();
	} break;

	case BLE_EVT_CMD_CONN_IS_ACTIVE: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_conn_handle con_handle = *(trble_conn_handle *)param->param[0];
		bool *is_active = (bool *)param->param[1];
		ret = ble_drv_conn_is_active(con_handle, is_active);
	} break;

	case BLE_EVT_CMD_CONN_IS_ANY_ACTIVE: {
		BLE_STATE_CHECK;

		bool *is_active = (bool *)msg->param;
		ret = ble_drv_conn_is_any_active(is_active);
	} break;

	// Client
	case BLE_EVT_CMD_CREATE_CTX: {
		BLE_STATE_CHECK;
		
		int i;
		ble_client_ctx *ctx = NULL;
		ret = TRBLE_SUCCESS;
		ble_client_callback_list *callbacks = (ble_client_callback_list *)msg->param;
		
		for (i = 0; i < BLE_MAX_CONNECTION_COUNT; i++) {
			if (g_client_table[i].state == BLE_CLIENT_NONE) {
				ctx = &g_client_table[i];
				break;
			}
		}

		msg->param = (void *)ctx;
		if (ctx == NULL) {
			ret = TRBLE_NOT_FOUND;
			break;
		}
		ctx->state = BLE_CLIENT_IDLE;
		if (callbacks) {
			memcpy(&(ctx->callbacks), callbacks, sizeof(ble_client_callback_list));
		}
	} break;

	case BLE_EVT_CMD_DESTROY_CTX: {
		BLE_STATE_CHECK;

		/* 
		[To Do] Need to Clear Values 
		1. disconnect
		2. auto connection
		*/
		ret = TRBLE_SUCCESS;
		ble_client_ctx *ctx = (ble_client_ctx *)msg->param;

		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}
		memset(ctx, 0, sizeof(ble_client_ctx));
	} break;

	case BLE_EVT_CMD_GET_CLIENT_STATE: {
		BLE_STATE_CHECK;

		ble_client_ctx *ctx = (ble_client_ctx *)msg->param;		
		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		msg->param = (void *)(ctx->state);
		ret = TRBLE_SUCCESS;
	} break;

	case BLE_EVT_CMD_START_SCAN: {
		BLE_STATE_CHECK;

		if (g_scan_ctx.state == BLE_SCAN_STARTED) {
			ret = TRBLE_BUSY;
			break;
		}

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		ble_scan_filter *filter = (ble_scan_filter *)param->param[0];
		ble_scan_callback_list *callbacks = (ble_scan_callback_list *)param->param[1];

		if (filter != NULL) {
			memcpy(&(g_scan_ctx.filter), filter, sizeof(ble_scan_filter));
		}

		if (callbacks == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}
		memcpy(&(g_scan_ctx.callback), callbacks, sizeof(ble_scan_callback_list));

		ret = ble_drv_start_scan((trble_scan_filter *)filter);
		if (ret != TRBLE_SUCCESS) {
			memset(&(g_scan_ctx.callback), 0, sizeof(ble_scan_callback_list));
			memset(&(g_scan_ctx.filter), 0, sizeof(ble_scan_filter));
		}
	} break;

	case BLE_EVT_CMD_STOP_SCAN: {
		BLE_STATE_CHECK;

		ret = TRBLE_SUCCESS;
		if (g_scan_ctx.state == BLE_SCAN_STOPPED) {
			break;
		}
		
		ret = ble_drv_stop_scan();
	} break;

	case BLE_EVT_CMD_CLIENT_CONNECT: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		ble_client_ctx *ctx = (ble_client_ctx *)param->param[0];
		ble_conn_info *conn_info = (ble_conn_info *)param->param[1];

		if (ctx == NULL || conn_info == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		if (ctx->state != BLE_CLIENT_IDLE) {
			ret = TRBLE_BUSY;
			break;
		}

		memcpy(&(ctx->info), conn_info, sizeof(ble_conn_info));
		ctx->state = BLE_CLIENT_CONNECTING;
		ret = ble_drv_client_connect((trble_conn_info *)&(ctx->info));
		if (ret != TRBLE_SUCCESS) {
			ctx->state = BLE_CLIENT_IDLE;
			memset(&(ctx->info), 0, sizeof(ble_conn_info));
		}
	} break;

	case BLE_EVT_CMD_CLIENT_DISCONNECT: {
		BLE_STATE_CHECK;

		ble_client_ctx *ctx = (ble_client_ctx *)msg->param;
		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		if (ctx->state != BLE_CLIENT_CONNECTED) {
			ret = TRBLE_INVALID_STATE;
			break;
		}

		ctx->state = BLE_CLIENT_DISCONNECTING;
		ret = ble_drv_client_disconnect(ctx->conn_handle);
		if (ret != TRBLE_SUCCESS) {
			ctx->state = BLE_CLIENT_CONNECTED;
		}
	} break;

	case BLE_EVT_CMD_CLIENT_DISCONNECT_ALL: {
		BLE_STATE_CHECK;

		ret = ble_drv_client_disconnect_all();
	} break;

	case BLE_EVT_CMD_CONNECTED_DEV_LIST: {
		BLE_STATE_CHECK;

		trble_connected_list *list = (trble_connected_list *)msg->param;
		ret = ble_drv_connected_device_list(list);
	} break;

	case BLE_EVT_CMD_CONNECTED_INFO: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		ble_client_ctx *ctx = (ble_client_ctx *)param->param[0];
		trble_device_connected *device = (trble_device_connected *)param->param[1];

		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		if (ctx->state != BLE_CLIENT_CONNECTED) {
			ret = TRBLE_INVALID_STATE;
			break;
		}

		ret = ble_drv_connected_info(ctx->conn_handle, device);
	} break;

	case BLE_EVT_CMD_OP_ENABLE_NOTI: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		ble_client_ctx *ctx = (ble_client_ctx *)param->param[0];
		ble_attr_handle attr_handle = *(ble_attr_handle *)param->param[1];

		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		if (ctx->state != BLE_CLIENT_CONNECTED) {
			ret = TRBLE_INVALID_STATE;
			break;
		}

		trble_operation_handle handle[1] = { 0, };
		handle->conn_handle = ctx->conn_handle;
		handle->attr_handle = attr_handle;

		ret = ble_drv_operation_enable_notification(handle);
	} break;

	case BLE_EVT_CMD_OP_READ: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		ble_client_ctx *ctx = (ble_client_ctx *)param->param[0];
		ble_attr_handle attr_handle = *(ble_attr_handle *)param->param[1];
		trble_data *data = (trble_data *)param->param[2];

		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		if (ctx->state != BLE_CLIENT_CONNECTED) {
			ret = TRBLE_INVALID_STATE;
			break;
		}

		trble_operation_handle handle[1] = { 0, };
		handle->conn_handle = ctx->conn_handle;
		handle->attr_handle = attr_handle;

		ret = ble_drv_operation_read(handle, data);
	} break;

	case BLE_EVT_CMD_OP_WRITE: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		ble_client_ctx *ctx = (ble_client_ctx *)param->param[0];
		ble_attr_handle attr_handle = *(ble_attr_handle *)param->param[1];
		trble_data *data = (trble_data *)param->param[2];

		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		if (ctx->state != BLE_CLIENT_CONNECTED) {
			ret = TRBLE_INVALID_STATE;
			break;
		}

		trble_operation_handle handle[1] = { 0, };
		handle->conn_handle = ctx->conn_handle;
		handle->attr_handle = attr_handle;

		ret = ble_drv_operation_write(handle, data);
	} break;

	case BLE_EVT_CMD_OP_WRITE_NO_RESP: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		ble_client_ctx *ctx = (ble_client_ctx *)param->param[0];
		ble_attr_handle attr_handle = *(ble_attr_handle *)param->param[1];
		trble_data *data = (trble_data *)param->param[2];

		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		if (ctx->state != BLE_CLIENT_CONNECTED) {
			ret = TRBLE_INVALID_STATE;
			break;
		}

		trble_operation_handle handle[1] = { 0, };
		handle->conn_handle = ctx->conn_handle;
		handle->attr_handle = attr_handle;

		ret = ble_drv_operation_write_no_response(handle, data);
	} break;

	// Server
	case BLE_EVT_CMD_GET_PROFILE_COUNT: {
		BLE_STATE_CHECK;

		uint16_t *count = (uint16_t *)msg->param;
		ret = ble_drv_get_profile_count(count);
	} break;

	case BLE_EVT_CMD_CHARACT_NOTI: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_attr_handle attr_handle = *(trble_attr_handle *)param->param[0];
		trble_conn_handle con_handle = *(trble_conn_handle *)param->param[1];
		trble_data *data = (trble_data *)param->param[2];

		ret = ble_drv_charact_notify(attr_handle, con_handle, data);
	} break;

	case BLE_EVT_CMD_ATTR_SET_DATA: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_attr_handle attr_handle = *(trble_attr_handle *)param->param[0];
		trble_data *data = (trble_data *)param->param[1];

		ret = ble_drv_attr_set_data(attr_handle, data);
	} break;

	case BLE_EVT_CMD_ATTR_GET_DATA: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_attr_handle attr_handle = *(trble_attr_handle *)param->param[0];
		trble_data *data = (trble_data *)param->param[1];

		ret = ble_drv_attr_get_data(attr_handle, data);
	} break;

	case BLE_EVT_CMD_ATTR_REJECT: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_attr_handle attr_handle = *(trble_attr_handle *)param->param[0];
		uint8_t app_errorcode = *(uint8_t *)param->param[1];

		ret = ble_drv_attr_reject(attr_handle, app_errorcode);
	} break;

	case BLE_EVT_CMD_SERVER_DISCONNECT: {
		BLE_STATE_CHECK;

		trble_conn_handle conn_handle = *(trble_conn_handle *)msg->param;
		ret = ble_drv_server_disconnect(conn_handle);
	} break;

	case BLE_EVT_CMD_GET_MAC_BY_CONN: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_conn_handle con_handle = *(trble_conn_handle *)param->param[0];
		uint8_t *bd_addr = (uint8_t *)param->param[1];

		ret = ble_drv_get_mac_addr_by_conn_handle(con_handle, bd_addr);
	} break;

	case BLE_EVT_CMD_GET_CONN_BY_MAC: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		uint8_t *bd_addr = (uint8_t *)param->param[0];
		trble_conn_handle *con_handle = (trble_conn_handle *)param->param[1];

		ret = ble_drv_get_conn_handle_by_addr(bd_addr, con_handle);
	} break;

	case BLE_EVT_CMD_SET_ADV_DATA: {
		BLE_STATE_CHECK;

		trble_data *data = (trble_data *)msg->param;
		ret = ble_drv_set_adv_data(data);
	} break;

	case BLE_EVT_CMD_SET_ADV_RESP: {
		BLE_STATE_CHECK;

		trble_data *data = (trble_data *)msg->param;
		ret = ble_drv_set_adv_resp(data);
	} break;

	case BLE_EVT_CMD_SET_ADV_TYPE: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_adv_type_e adv_type = *(trble_adv_type_e *)param->param[0];
		trble_addr *addr = (trble_addr *)param->param[1];
		ret = ble_drv_set_adv_type(adv_type, addr);
	} break;

	case BLE_EVT_CMD_SET_ADV_INTERVAL: {
		BLE_STATE_CHECK;

		uint16_t interval = *(uint16_t *)msg->param;
		ret = ble_drv_set_adv_interval(interval);
	} break;

	case BLE_EVT_CMD_START_ADV: {
		BLE_STATE_CHECK;

		ret = ble_drv_start_adv();
	} break;

	case BLE_EVT_CMD_STOP_ADV: {
		BLE_STATE_CHECK;

		ret = ble_drv_stop_adv();
	} break;

	
	// Event Handling
	case BLE_EVT_CLIENT_CONNECT: {
		if (msg->param == NULL) {
			break;
		}
		ble_device_connected *data = (ble_device_connected *)msg->param;

		int i;
		ble_client_ctx *ctx = NULL;
		for (i = 0; i < BLE_MAX_CONNECTION_COUNT; i++) {
			if (g_client_table[i].state == BLE_CLIENT_CONNECTING && memcmp(g_client_table[i].info.addr.mac, data->conn_info.addr.mac, BLE_BD_ADDR_MAX_LEN) == 0) {
				g_client_table[i].state = BLE_CLIENT_CONNECTED;
				g_client_table[i].conn_handle = data->conn_handle;
				g_client_table[i].is_bonded = data->is_bonded;
				memcpy(&(g_client_table[i].info), &(data->conn_info), sizeof(ble_conn_info));
				ctx = &g_client_table[i];
				break;
			}
		}

		if (ctx && ctx->callbacks.ble_client_device_connected_cb) {
			ctx->callbacks.ble_client_device_connected_cb(ctx, data);
		}
		free(msg->param);
	} break;

	case BLE_EVT_CLIENT_DISCONNECT: {
		if (msg->param == NULL) {
			break;
		}
		int i;
		ble_conn_handle data = *(ble_conn_handle *)msg->param;
		ble_client_ctx *ctx = NULL;

		for (i = 0; i < BLE_MAX_CONNECTION_COUNT; i++) {
			if (g_client_table[i].state != BLE_CLIENT_NONE && g_client_table[i].conn_handle == data) {
				g_client_table[i].state = BLE_CLIENT_IDLE;
				ctx = &g_client_table[i];
				break;
			}
		}

		if (ctx && ctx->callbacks.ble_client_device_disconnected_cb) {
			ctx->callbacks.ble_client_device_disconnected_cb(ctx);
		}
		free(msg->param);
	} break;

	case BLE_EVT_CLIENT_NOTI: {
		if (msg->param == NULL) {
			break;
		}
		int i;
		ble_client_ctx *ctx = NULL;
		uint8_t *data = (uint8_t *)msg->param;
		ble_conn_handle conn_handle = *(ble_conn_handle *)data;
		ble_attr_handle attr_handle = *(ble_attr_handle *)(data + sizeof(ble_conn_handle));

		ble_data read_result[1];
		read_result->length = *(uint16_t *)(data + sizeof(ble_conn_handle) + sizeof(ble_attr_handle));
		read_result->data = (uint8_t *)(data + sizeof(ble_conn_handle) + sizeof(ble_attr_handle) + sizeof(read_result->length));

		for (i = 0; i < BLE_MAX_CONNECTION_COUNT; i++) {
			if (g_client_table[i].conn_handle == conn_handle) {
				ctx = &g_client_table[i];
				break;
			}
		}

		if (ctx && ctx->callbacks.ble_client_operation_notification_cb) {
			ctx->callbacks.ble_client_operation_notification_cb(ctx, attr_handle, read_result);
		}
		free(msg->param);
	} break;

	case BLE_EVT_SCAN_STATE: {
		if (msg->param == NULL) {
			break;
		}
		ble_scan_state_e data = *(ble_scan_state_e *)msg->param;
		g_scan_ctx.state = data;
		if (g_scan_ctx.callback.ble_client_scan_state_changed_cb) {
			g_scan_ctx.callback.ble_client_scan_state_changed_cb(data);
		}
		if (data == BLE_SCAN_STOPPED) {
			memset(&(g_scan_ctx.filter), 0, sizeof(ble_scan_filter));
			memset(&(g_scan_ctx.callback), 0, sizeof(ble_scan_callback_list));
		}
		free(msg->param);
	} break;

	case BLE_EVT_SCAN_DATA: {
		if (msg->param == NULL) {
			break;
		}
		ble_scanned_device *data = (ble_scanned_device *)msg->param;
		if (g_scan_ctx.state == BLE_SCAN_STARTED && g_scan_ctx.callback.ble_client_device_scanned_cb) {
			g_scan_ctx.callback.ble_client_device_scanned_cb(data);
		}
		free(msg->param);
	} break;

	default:
		ret = TRBLE_UNKNOWN;
		break;
	}

handle_req_done:
	BLE_LOG_INFO("[BLEMGR] T%d <-- _handle_request\n", getpid());
	return _convert_ret(ret);
}
