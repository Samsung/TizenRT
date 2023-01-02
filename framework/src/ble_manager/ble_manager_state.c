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
#include <stdlib.h>
#include <mqueue.h>
#include <errno.h>
#include <unistd.h>
#include <tinyara/ble/ble_manager.h>
#include <ble_manager/ble_manager.h>
#include "ble_queue.h"
#include "ble_manager_common.h"
#include "ble_manager_autoconnect.h"
#include "ble_manager_event.h"
#include "ble_manager_msghandler.h"
#include "ble_manager_state.h"
#include "ble_manager_log.h"

#define BLE_EVT_HIGH_BUFFER_SIZE 20
#define BLE_EVT_LOW_BUFFER_SIZE 100

static ble_client_ctx_internal g_client_table[BLE_MAX_CONNECTION_COUNT] = { 0, };
static ble_scan_ctx g_scan_ctx = { 0, };
static blemgr_state_e g_manager_state = BLEMGR_UNINITIALIZED;
static ble_scan_whitelist g_scan_whitelist[SCAN_WHITELIST_SIZE] = { 0, };

#define BLE_STATE_CHECK                                   \
	do {                                                  \
		if (g_manager_state != BLEMGR_INITIALIZED) { \
			BLE_LOG_INFO("[BLEMGR] is not running\n");    \
			ret = TRBLE_INVALID_STATE;                    \
			goto handle_req_done;                         \
		}                                                 \
	} while (0)

static bool _whitelist_delete(ble_addr *addr)
{
	int i;
	for (i = 0; i < SCAN_WHITELIST_SIZE; i++) {
		if (g_scan_whitelist[i].use != SCAN_WHITELIST_IN_USE) {
			continue;
		}
		if (memcmp(g_scan_whitelist[i].addr.mac, addr->mac, TRBLE_BD_ADDR_MAX_LEN) == 0 && 
			g_scan_whitelist[i].addr.type == addr->type) {
			g_scan_whitelist[i].use = SCAN_WHITELIST_EMPTY;
			memset(&g_scan_whitelist[i], 0, sizeof(ble_scan_whitelist));
			return true;
		}
	}
	return false;
}

static bool _whitelist_add(ble_addr *addr)
{
	int i;
	for (i = 0; i < SCAN_WHITELIST_SIZE; i++) {
		if (g_scan_whitelist[i].use != SCAN_WHITELIST_EMPTY) {
			continue;
		}
		g_scan_whitelist[i].use = SCAN_WHITELIST_IN_USE;
		g_scan_whitelist[i].addr.type = addr->type;
		memcpy(g_scan_whitelist[i].addr.mac, addr->mac, TRBLE_BD_ADDR_MAX_LEN);
		return true;
	}
	return false;
}

static bool _whitelist_is_full(void)
{
	int i;
	for (i = 0; i < SCAN_WHITELIST_SIZE; i++) {
		if (g_scan_whitelist[i].use == SCAN_WHITELIST_EMPTY) {
			return false;
		}
	}
	return true;
}

static bool _whitelist_is_exist(ble_addr *addr)
{
	int i;
	for (i = 0; i < SCAN_WHITELIST_SIZE; i++) {
		if (g_scan_whitelist[i].use != SCAN_WHITELIST_IN_USE) {
			continue;
		}
		if (memcmp(g_scan_whitelist[i].addr.mac, addr->mac, TRBLE_BD_ADDR_MAX_LEN) == 0 && 
			g_scan_whitelist[i].addr.type == addr->type) {
			return true;
		}
	}
	return false;
}

static ble_result_e _convert_ret(trble_result_e val)
{
	return (ble_result_e)val;
}

static void _event_caller(int evt_pri, void *data) {
	if (data == NULL) {
		return;
	}

	switch (evt_pri) {
	case BLE_QUEUE_EVT_PRI_HIGH: {
		blemgr_msg_params *msg = (blemgr_msg_params *)data;
		switch (msg->evt) {
		case BLE_EVT_CLIENT_CONNECT: {
			ble_client_device_connected_cb callback = msg->param[0];
			callback(msg->param[1], msg->param[2]);
		} break;
		case BLE_EVT_CLIENT_DISCONNECT: {
			ble_client_device_disconnected_cb callback = msg->param[0];
			callback(msg->param[1]);
		} break;
		case BLE_EVT_CLIENT_NOTI: {
			ble_client_operation_notification_cb callback = msg->param[0];
			ble_attr_handle attr_handle = *(ble_attr_handle *)(msg->param[2] + sizeof(ble_conn_handle));

			ble_data read_result[1];
			read_result->length = *(uint16_t *)(msg->param[2] + sizeof(ble_conn_handle) + sizeof(ble_attr_handle));
			read_result->data = (uint8_t *)(msg->param[2] + sizeof(ble_conn_handle) + sizeof(ble_attr_handle) + sizeof(read_result->length));
			
			callback(msg->param[1], attr_handle, read_result);
		} break;
		case BLE_EVT_CLIENT_INDI: {
			ble_client_operation_indication_cb callback = msg->param[0];
			ble_attr_handle attr_handle = *(ble_attr_handle *)(msg->param[2] + sizeof(ble_conn_handle));

			ble_data read_result[1];
			read_result->length = *(uint16_t *)(msg->param[2] + sizeof(ble_conn_handle) + sizeof(ble_attr_handle));
			read_result->data = (uint8_t *)(msg->param[2] + sizeof(ble_conn_handle) + sizeof(ble_attr_handle) + sizeof(read_result->length));
			
			callback(msg->param[1], attr_handle, read_result);
		} break;
		case BLE_EVT_SCAN_STATE: {
			ble_scan_state_e state = *(ble_scan_state_e *)msg->param[2];
			ble_client_scan_state_changed_cb callback = msg->param[0];
			callback(state);
			if (state == BLE_SCAN_STOPPED) {
				memset(&(g_scan_ctx.filter), 0, sizeof(ble_scan_filter));
				memset(&(g_scan_ctx.callback), 0, sizeof(ble_scan_callback_list));
			}
		} break;
		default:
			break;
		}

		if (msg->param[2] != NULL) {
			free(msg->param[2]);
		}
	} break;
	case BLE_QUEUE_EVT_PRI_LOW: {
		ble_scanned_device *scanned_device = (ble_scanned_device *)data;
		if (g_scan_ctx.state == BLE_SCAN_STARTED && g_scan_ctx.callback.device_scanned_cb) {
			g_scan_ctx.callback.device_scanned_cb(scanned_device);
		}
	} break;
	default:
		break;
	}
}

/*
 * public
 */
ble_result_e blemgr_handle_request(blemgr_msg_s *msg)
{
	trble_result_e ret = TRBLE_FAIL;
	blemgr_msg_params queue_msg = { 
		.evt = msg->event,
		.count = 3
	};

	BLE_LOG_DEBUG("[BLEMGR] T%d --> _handle_request[%d]\n", getpid(), msg->event);

	switch (msg->event) {
	case BLE_CMD_INIT: {
		int i;
		if (g_manager_state != BLEMGR_UNINITIALIZED) {
			BLE_LOG_INFO("[BLEMGR] already running\n");
			ret = TRBLE_ALREADY_WORKING;
			break;
		}

		/* BLE Priority Queue Setup */
		if (ble_queue_init(_event_caller) != BLE_QUEUE_SUCCESS) {
			break;
		}
		if (ble_queue_pri_set(BLE_QUEUE_EVT_PRI_HIGH, BLE_EVT_HIGH_BUFFER_SIZE, sizeof(blemgr_msg_params)) != BLE_QUEUE_SUCCESS) {
			ble_queue_deinit();
			break;
		}
		if (ble_queue_pri_set(BLE_QUEUE_EVT_PRI_LOW, BLE_EVT_LOW_BUFFER_SIZE, sizeof(trble_scanned_device)) != BLE_QUEUE_SUCCESS) {
			ble_queue_deinit();
			break;
		}
		
		trble_server_init_config *server = (trble_server_init_config *)msg->param;
		ret = ble_drv_init(server, (trble_queue *)ble_queue_get_pri_queue(BLE_QUEUE_EVT_PRI_LOW));
		if (ret != TRBLE_SUCCESS) {
			/* Clear Queue */
			ble_queue_deinit();
			break;
		}

		for (i = 0; i < BLE_MAX_CONNECTION_COUNT; i++) {
			memset(&g_client_table[i], 0, sizeof(ble_client_ctx_internal));
		}

		for (i = 0; i < SCAN_WHITELIST_SIZE; i++) {
			memset(&g_scan_whitelist[i], 0, sizeof(ble_scan_whitelist));
		}
		g_manager_state = BLEMGR_INITIALIZED;
		g_scan_ctx.state = BLE_SCAN_STOPPED;
	} break;

	case BLE_CMD_DEINIT: {
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
		ble_queue_deinit();
		g_manager_state = BLEMGR_UNINITIALIZED;
		g_scan_ctx.state = BLE_SCAN_STOPPED;
	} break;

	case BLE_CMD_GET_MAC: {
		BLE_STATE_CHECK;

		uint8_t *mac = (uint8_t *)msg->param;
		if (mac == NULL) {
			ret = TRBLE_INVALID_ARGS;
		}
		ret = ble_drv_get_mac_addr(mac);
	} break;
	
	case BLE_CMD_GET_BONDED_DEV: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_bonded_device_list_s *list = (trble_bonded_device_list_s *)param->param[0];
		uint16_t *device_count = (uint16_t *)param->param[1];

		ret = ble_drv_get_bonded_device(list, device_count);
	} break;

	case BLE_CMD_DEL_BOND: {
		BLE_STATE_CHECK;

		trble_addr *addr = (trble_addr *)msg->param;
		ret = ble_drv_delete_bonded(addr);
	} break;

	case BLE_CMD_DEL_BOND_ALL: {
		BLE_STATE_CHECK;

		ret = ble_drv_delete_bonded_all();
	} break;

	case BLE_CMD_CONN_IS_ACTIVE: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_conn_handle con_handle = *(trble_conn_handle *)param->param[0];
		bool *is_active = (bool *)param->param[1];
		ret = ble_drv_conn_is_active(con_handle, is_active);
	} break;

	case BLE_CMD_CONN_IS_ANY_ACTIVE: {
		BLE_STATE_CHECK;

		bool *is_active = (bool *)msg->param;
		ret = ble_drv_conn_is_any_active(is_active);
	} break;

	case BLE_CMD_GET_VERSION: {
		BLE_STATE_CHECK;

		trble_msg_s tmsg = { TRBLE_MSG_GET_VERSION, msg->param };
		ret = ble_drv_ioctl(&tmsg);
	} break;

	// Scanner
	case BLE_CMD_START_SCAN: {
		BLE_STATE_CHECK;

		ret = TRBLE_SUCCESS;
		if (g_scan_ctx.state != BLE_SCAN_STOPPED) {
			ret = TRBLE_BUSY;
			break;
		}

		ble_scan_state_e priv_state = g_scan_ctx.state;
		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		ble_scan_filter *filter = (ble_scan_filter *)param->param[0];
		ble_scan_callback_list *callbacks = (ble_scan_callback_list *)param->param[1];

		if (callbacks == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}
		memcpy(&(g_scan_ctx.callback), callbacks, sizeof(ble_scan_callback_list));

		if (filter != NULL) {
			memcpy(&(g_scan_ctx.filter), filter, sizeof(ble_scan_filter));
		} else {
			memset(&(g_scan_ctx.filter), 0, sizeof(ble_scan_filter));
			filter = &(g_scan_ctx.filter);
		}

		if (filter->scan_duration > SCAN_MAX_TIMEOUT || filter->scan_duration == 0) {
			filter->scan_duration = SCAN_MAX_TIMEOUT;
		}

		g_scan_ctx.state = BLE_SCAN_CHANGING;
		ret = ble_drv_start_scan((trble_scan_filter *)filter);
		if (ret != TRBLE_SUCCESS) {
			memset(&(g_scan_ctx.callback), 0, sizeof(ble_scan_callback_list));
			memset(&(g_scan_ctx.filter), 0, sizeof(ble_scan_filter));
			g_scan_ctx.state = priv_state;
		}
	} break;

	case BLE_CMD_STOP_SCAN: {
		BLE_STATE_CHECK;

		ret = TRBLE_SUCCESS;
		if (g_scan_ctx.state == BLE_SCAN_STOPPED) {
			break;
		} else if (g_scan_ctx.state == BLE_SCAN_CHANGING) {
			ret = TRBLE_BUSY;
			break;
		}

		ble_scan_state_e priv_state = g_scan_ctx.state;

		g_scan_ctx.state = BLE_SCAN_CHANGING;
		ret = ble_drv_stop_scan();
		if (ret != TRBLE_SUCCESS) {
			g_scan_ctx.state = priv_state;
		}
	} break;

	case BLE_CMD_WHITELIST_ADD: {
		BLE_STATE_CHECK;

		ret = TRBLE_SUCCESS;
		ble_addr *addr = (ble_addr *)msg->param;
		if (addr == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		if (_whitelist_is_exist(addr) == true) {
			/* Already Exist */
			break;
		}

		if (_whitelist_is_full() == true) {
			ret = TRBLE_OUT_OF_MEMORY;
			break;
		}

		if (g_scan_ctx.state != BLE_SCAN_STOPPED) {
			ret = TRBLE_INVALID_STATE;
			break;
		}

		ret = ble_drv_scan_whitelist_add((trble_addr *)addr);
		if (ret == TRBLE_SUCCESS) {
			if (_whitelist_add(addr) == false) {
				ret = TRBLE_FAIL;
			}
		}
	} break;

	case BLE_CMD_WHITELIST_DELETE: {
		BLE_STATE_CHECK;

		ret = TRBLE_SUCCESS;
		ble_addr *addr = (ble_addr *)msg->param;
		if (addr == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		if (_whitelist_is_exist(addr) != true) {
			/* Already Deleted */
			break;
		}

		if (g_scan_ctx.state != BLE_SCAN_STOPPED) {
			ret = TRBLE_INVALID_STATE;
			break;
		}
		
		ret = ble_drv_scan_whitelist_delete((trble_addr *)addr);
		if (ret == TRBLE_SUCCESS) {
			if (_whitelist_delete(addr) == false) {
				ret = TRBLE_FAIL;
			}
		}

	} break;

	case BLE_CMD_WHITELIST_CLEAR_ALL: {
		BLE_STATE_CHECK;

		int i;
		ret = TRBLE_SUCCESS;
		if (g_scan_ctx.state != BLE_SCAN_STOPPED) {
			ret = TRBLE_INVALID_STATE;
			break;
		}
		
		ret = ble_drv_scan_whitelist_clear_all();
		if (ret == TRBLE_SUCCESS) {
			for (i = 0; i < SCAN_WHITELIST_SIZE; i++) {
				memset(&g_scan_whitelist[i], 0, sizeof(ble_scan_whitelist));
			}
		}

	} break;

	case BLE_CMD_WHITELIST_LIST: {
		BLE_STATE_CHECK;

		int i;
		uint16_t count = 0;
		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		ble_addr *addr = (ble_addr *)param->param[0];
		uint16_t size = *(uint16_t *)param->param[1];
		for (i = 0; i < SCAN_WHITELIST_SIZE; i++) {
			if (g_scan_whitelist[i].use == SCAN_WHITELIST_IN_USE) {
				if (count < size) {
					memcpy(&addr[count], &g_scan_whitelist[i].addr, sizeof(ble_addr));
				}
				count++;
			}
		}
		msg->ret.val = count;
		ret = TRBLE_SUCCESS;
	} break;

	// Client
	case BLE_CMD_CREATE_CTX: {
		BLE_STATE_CHECK;
		
		int i;
		ble_client_ctx_internal *ctx = NULL;
		ret = TRBLE_SUCCESS;
		ble_client_callback_list *callbacks = (ble_client_callback_list *)msg->param;
		
		for (i = 0; i < BLE_MAX_CONNECTION_COUNT; i++) {
			if (g_client_table[i].state == BLE_CLIENT_NONE) {
				ctx = &g_client_table[i];
				break;
			}
		}

		msg->ret.ptr = (void *)ctx;
		if (ctx == NULL) {
			ret = TRBLE_NOT_FOUND;
			break;
		}
		ctx->state = BLE_CLIENT_IDLE;
		if (callbacks) {
			memcpy(&(ctx->callbacks), callbacks, sizeof(ble_client_callback_list));
		}
	} break;

	case BLE_CMD_DESTROY_CTX: {
		BLE_STATE_CHECK;

		/* 
		[To Do] Need to Clear Values 
		1. disconnect
		2. auto connection
		*/
		ret = TRBLE_SUCCESS;
		ble_client_ctx_internal *ctx = (ble_client_ctx_internal *)msg->param;

		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}
		if (ctx->state != BLE_CLIENT_IDLE) {
			ret = TRBLE_INVALID_STATE;
			break;
		}
		memset(ctx, 0, sizeof(ble_client_ctx_internal));
	} break;

	case BLE_CMD_GET_CLIENT_STATE: {
		BLE_STATE_CHECK;

		ble_client_ctx *ctx = (ble_client_ctx *)msg->param;		
		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		msg->ret.val = ctx->state;
		ret = TRBLE_SUCCESS;
	} break;

	case BLE_CMD_CLIENT_CONNECT: {
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
		if (ctx->info.scan_timeout == 0) {
			ctx->info.scan_timeout = BLE_DEFAULT_CONN_TIMEOUT;
		}
		ctx->state = BLE_CLIENT_CONNECTING;
		ret = ble_drv_client_connect((trble_conn_info *)&(ctx->info));
		if (ret != TRBLE_SUCCESS) {
			ctx->state = BLE_CLIENT_IDLE;
			memset(&(ctx->info), 0, sizeof(ble_conn_info));
		}
	} break;

	case BLE_CMD_CLIENT_ENABLE_AUTOCONNECT: {
		ble_client_ctx *ctx = (ble_client_ctx *)msg->param;

		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		if (ctx->state != BLE_CLIENT_IDLE && ctx->state != BLE_CLIENT_CONNECTED) {
			ret = TRBLE_INVALID_STATE;
			break;
		}

		ctx->auto_connect = true;
		ret = TRBLE_SUCCESS;
	} break;

	case BLE_CMD_CLIENT_DISABLE_AUTOCONNECT: {
		ble_client_ctx_internal *ctx = (ble_client_ctx_internal *)msg->param;
		ret = TRBLE_SUCCESS;		

		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		if (ctx->mqfd == (mqd_t)ERROR) {
			ret = TRBLE_INVALID_STATE;
			break;
		}

		if (ctx->mqfd == 0) {
			/* Auto-connecting is already disabled */
			ret = TRBLE_SUCCESS;
			break;
		}
		
		if (ctx->state != BLE_CLIENT_AUTOCONNECTING) {
			/* This is good state */
			ctx->auto_connect = false;
			break;
		}

		char buf[BLE_MQ_SIZE] = { 0, };
		buf[0] = BLE_AUTOCON_EVT_CANCEL;
		int status = mq_send(ctx->mqfd, buf, BLE_MQ_SIZE, 0);
		if (status < 0) {
			BLE_LOG_ERROR("[BLEMGR] fail to send mqueue(evt : %d)[%d]\n", msg->event, errno);
			ret = TRBLE_FILE_ERROR;
			break;
		}
	} break;

	case BLE_CMD_CLIENT_RECONNECT: {
		BLE_STATE_CHECK;

		ble_client_ctx *ctx = (ble_client_ctx *)msg->param;

		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		if (ctx->state != BLE_CLIENT_IDLE && ctx->state != BLE_CLIENT_AUTOCONNECTING) {
			ret = TRBLE_INVALID_STATE;
			break;
		}

		if (ctx->state == BLE_CLIENT_IDLE) {
			ctx->state = BLE_CLIENT_CONNECTING;
			break;
		}
		
		ret = ble_drv_client_connect((trble_conn_info *)&(ctx->info));
		if (ret != TRBLE_SUCCESS) {
			ctx->state = BLE_CLIENT_IDLE;
			memset(&(ctx->info), 0, sizeof(ble_conn_info));
		}
	} break;

	case BLE_CMD_CLIENT_DISCONNECT: {
		BLE_STATE_CHECK;

		ble_client_ctx_internal *ctx = (ble_client_ctx_internal *)msg->param;
		if (ctx == NULL) {
			ret = TRBLE_INVALID_ARGS;
			break;
		}

		ret = TRBLE_SUCCESS;
		if (ctx->state == BLE_CLIENT_CONNECTED) {
			ctx->state = BLE_CLIENT_DISCONNECTING;
			ret = ble_drv_client_disconnect(ctx->conn_handle);
			if (ret != TRBLE_SUCCESS) {
				ctx->state = BLE_CLIENT_CONNECTED;
			}
		} else if (ctx->state == BLE_CLIENT_CONNECTING) {
			ret = TRBLE_INVALID_STATE;		
		} else if (ctx->state == BLE_CLIENT_AUTOCONNECTING) {
			char buf[BLE_MQ_SIZE] = { 0, };
			buf[0] = BLE_AUTOCON_EVT_CANCEL;
			int status = mq_send(ctx->mqfd, buf, BLE_MQ_SIZE, 0);
			if (status < 0) {
				BLE_LOG_ERROR("[BLEMGR] fail to send mqueue(evt : %d)[%d]\n", msg->event, errno);
				ret = TRBLE_FILE_ERROR;
				break;
			}
		}
	} break;

	case BLE_CMD_CLIENT_DISCONNECT_ALL: {
		BLE_STATE_CHECK;

		ret = ble_drv_client_disconnect_all();
	} break;

	case BLE_CMD_CONNECTED_DEV_LIST: {
		BLE_STATE_CHECK;

		trble_connected_list *list = (trble_connected_list *)msg->param;
		ret = ble_drv_connected_device_list(list);
	} break;

	case BLE_CMD_CONNECTED_INFO: {
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

	case BLE_CMD_OP_ENABLE_NOTI: {
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

	case BLE_CMD_OP_ENABLE_INDICATE: {
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

		ret = ble_drv_operation_enable_indication(handle);
	} break;

	case BLE_CMD_OP_ENABLE_NOTI_AND_INDICATE: {
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

		ret = ble_drv_operation_enable_notification_and_indication(handle);
	} break;

	case BLE_CMD_OP_READ: {
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

	case BLE_CMD_OP_WRITE: {
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

	case BLE_CMD_OP_WRITE_NO_RESP: {
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
	case BLE_CMD_GET_PROFILE_COUNT: {
		BLE_STATE_CHECK;

		uint16_t *count = (uint16_t *)msg->param;
		ret = ble_drv_get_profile_count(count);
	} break;

	case BLE_CMD_CHARACT_NOTI: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_attr_handle attr_handle = *(trble_attr_handle *)param->param[0];
		trble_conn_handle con_handle = *(trble_conn_handle *)param->param[1];
		trble_data *data = (trble_data *)param->param[2];

		ret = ble_drv_charact_notify(attr_handle, con_handle, data);
	} break;

	case BLE_CMD_CHARACT_INDI: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_attr_handle attr_handle = *(trble_attr_handle *)param->param[0];
		trble_conn_handle con_handle = *(trble_conn_handle *)param->param[1];
		trble_data *data = (trble_data *)param->param[2];

		ret = ble_drv_charact_indicate(attr_handle, con_handle, data);
	} break;

	case BLE_CMD_ATTR_SET_DATA: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_attr_handle attr_handle = *(trble_attr_handle *)param->param[0];
		trble_data *data = (trble_data *)param->param[1];

		ret = ble_drv_attr_set_data(attr_handle, data);
	} break;

	case BLE_CMD_ATTR_GET_DATA: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_attr_handle attr_handle = *(trble_attr_handle *)param->param[0];
		trble_data *data = (trble_data *)param->param[1];

		ret = ble_drv_attr_get_data(attr_handle, data);
	} break;

	case BLE_CMD_ATTR_REJECT: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_attr_handle attr_handle = *(trble_attr_handle *)param->param[0];
		uint8_t app_errorcode = *(uint8_t *)param->param[1];

		ret = ble_drv_attr_reject(attr_handle, app_errorcode);
	} break;

	case BLE_CMD_SERVER_DISCONNECT: {
		BLE_STATE_CHECK;

		trble_conn_handle conn_handle = *(trble_conn_handle *)msg->param;
		ret = ble_drv_server_disconnect(conn_handle);
	} break;

	case BLE_CMD_GET_MAC_BY_CONN: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_conn_handle con_handle = *(trble_conn_handle *)param->param[0];
		uint8_t *bd_addr = (uint8_t *)param->param[1];

		ret = ble_drv_get_mac_addr_by_conn_handle(con_handle, bd_addr);
	} break;

	case BLE_CMD_GET_CONN_BY_MAC: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		uint8_t *bd_addr = (uint8_t *)param->param[0];
		trble_conn_handle *con_handle = (trble_conn_handle *)param->param[1];

		ret = ble_drv_get_conn_handle_by_addr(bd_addr, con_handle);
	} break;

	case BLE_CMD_SET_ADV_DATA: {
		BLE_STATE_CHECK;

		trble_data *data = (trble_data *)msg->param;
		ret = ble_drv_set_adv_data(data);
	} break;

	case BLE_CMD_SET_ADV_RESP: {
		BLE_STATE_CHECK;

		trble_data *data = (trble_data *)msg->param;
		ret = ble_drv_set_adv_resp(data);
	} break;

	case BLE_CMD_SET_ADV_TYPE: {
		BLE_STATE_CHECK;

		blemgr_msg_params *param = (blemgr_msg_params *)msg->param;
		trble_adv_type_e adv_type = *(trble_adv_type_e *)param->param[0];
		trble_addr *addr = (trble_addr *)param->param[1];
		ret = ble_drv_set_adv_type(adv_type, addr);
	} break;

	case BLE_CMD_SET_ADV_INTERVAL: {
		BLE_STATE_CHECK;

		uint16_t interval = *(uint16_t *)msg->param;
		ret = ble_drv_set_adv_interval(interval);
	} break;

	case BLE_CMD_SET_ADV_TXPOWER: {
		BLE_STATE_CHECK;

		uint8_t txpower = *(uint8_t *)msg->param;
		ret = ble_drv_set_adv_txpower(txpower);
	} break;

	case BLE_CMD_START_ADV: {
		BLE_STATE_CHECK;

		ret = ble_drv_start_adv();
	} break;

	case BLE_CMD_STOP_ADV: {
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
		ble_client_ctx_internal *ctx = NULL;
		ble_client_state_e priv_state = BLE_CLIENT_NONE;
		for (i = 0; i < BLE_MAX_CONNECTION_COUNT; i++) {
			if (memcmp(g_client_table[i].info.addr.mac, data->conn_info.addr.mac, BLE_BD_ADDR_MAX_LEN) == 0) {
				if (g_client_table[i].state == BLE_CLIENT_CONNECTING || g_client_table[i].state == BLE_CLIENT_AUTOCONNECTING) {
					priv_state = g_client_table[i].state;
					g_client_table[i].state = BLE_CLIENT_CONNECTED;
					g_client_table[i].conn_handle = data->conn_handle;
					g_client_table[i].is_bonded = data->is_bonded;

					memcpy(&(g_client_table[i].info.addr), &(data->conn_info.addr), sizeof(trble_addr));
					g_client_table[i].info.conn_interval = data->conn_info.conn_interval;
					g_client_table[i].info.slave_latency = data->conn_info.slave_latency;
					g_client_table[i].info.mtu = data->conn_info.mtu;
					ctx = &g_client_table[i];
					break;
				}
			}
		}

		if (ctx == NULL) {
			BLE_LOG_ERROR("[BLEMGR] fail to find BLE context table\n");
			ret = TRBLE_NOT_FOUND;
			free(msg->param);
			break;
		}

		if (priv_state == BLE_CLIENT_AUTOCONNECTING) {
			char buf[BLE_MQ_SIZE] = { 0, };
			buf[0] = BLE_AUTOCON_EVT_CONNECTED;
			int status = mq_send(ctx->mqfd, buf, BLE_MQ_SIZE, 0);
			if (status < 0) {
				BLE_LOG_ERROR("[BLEMGR] fail to send mqueue(evt : %d)[%d]\n", msg->event, errno);
			}
		}

		if (ctx && ctx->callbacks.connected_cb) {
			memcpy(queue_msg.param, (void*[]){ctx->callbacks.connected_cb, ctx, msg->param}, sizeof(void*) * queue_msg.count);
			ble_queue_enque(BLE_QUEUE_EVT_PRI_HIGH, &queue_msg);
		}
	} break;

	case BLE_EVT_CLIENT_DISCONNECT: {
		if (msg->param == NULL) {
			break;
		}
		int i;
		ble_conn_handle data = *(ble_conn_handle *)msg->param;
		ble_client_ctx_internal *ctx = NULL;
		ble_client_state_e priv_state = BLE_CLIENT_NONE;

		for (i = 0; i < BLE_MAX_CONNECTION_COUNT; i++) {
			if (g_client_table[i].state != BLE_CLIENT_NONE && g_client_table[i].conn_handle == data) {
				ctx = &g_client_table[i];
				break;
			}
		}
		free(msg->param);

		if (ctx == NULL) {
			break;
		}

		priv_state = ctx->state;
		if (ctx->auto_connect == true && 
				(priv_state == BLE_CLIENT_CONNECTED || priv_state == BLE_CLIENT_AUTOCONNECTING)) {
			if (ctx->state == BLE_CLIENT_AUTOCONNECTING) {
				char buf[BLE_MQ_SIZE] = { 0, };
				buf[0] = BLE_AUTOCON_EVT_DISCONNECTED;
				int status = mq_send(ctx->mqfd, buf, BLE_MQ_SIZE, 0);
				if (status < 0) {
					BLE_LOG_ERROR("[BLEMGR] fail to send mqueue(evt : %d)[%d]\n", msg->event, errno);
				}
			} else {
				ctx->state = BLE_CLIENT_AUTOCONNECTING;
				ble_autocon_result_e result = ble_manager_autoconnect(ctx);
				if (result != BLE_AUTOCON_SUCCESS) {
					BLE_LOG_ERROR("[BLEMGR] auto connnect fail[%d]\n", result);
				}
			}
		} else {
			ctx->state = BLE_CLIENT_IDLE;
		}

		if (priv_state != BLE_CLIENT_AUTOCONNECTING && ctx->callbacks.disconnected_cb) {
			memcpy(queue_msg.param, (void*[]){ctx->callbacks.disconnected_cb, ctx, NULL}, sizeof(void*) * queue_msg.count);
			ble_queue_enque(BLE_QUEUE_EVT_PRI_HIGH, &queue_msg);
		}
	} break;

	case BLE_EVT_CLIENT_NOTI: {
		if (msg->param == NULL) {
			break;
		}
		int i;
		ble_client_ctx_internal *ctx = NULL;
		ble_conn_handle conn_handle = *(ble_conn_handle *)msg->param;

		for (i = 0; i < BLE_MAX_CONNECTION_COUNT; i++) {
			if (g_client_table[i].conn_handle == conn_handle) {
				ctx = &g_client_table[i];
				break;
			}
		}

		if (ctx && ctx->callbacks.notification_cb) {
			memcpy(queue_msg.param, (void*[]){ctx->callbacks.notification_cb, ctx, msg->param}, sizeof(void*) * queue_msg.count);
			ble_queue_enque(BLE_QUEUE_EVT_PRI_HIGH, &queue_msg);
		}
	} break;

    case BLE_EVT_CLIENT_INDI: {
		if (msg->param == NULL) {
			break;
		}
		int i;
		ble_client_ctx_internal *ctx = NULL;
		ble_conn_handle conn_handle = *(ble_conn_handle *)msg->param;

		for (i = 0; i < BLE_MAX_CONNECTION_COUNT; i++) {
			if (g_client_table[i].conn_handle == conn_handle) {
				ctx = &g_client_table[i];
				break;
			}
		}

		if (ctx && ctx->callbacks.indication_cb) {
			memcpy(queue_msg.param, (void*[]){ctx->callbacks.indication_cb, ctx, msg->param}, sizeof(void*) * queue_msg.count);
			ble_queue_enque(BLE_QUEUE_EVT_PRI_HIGH, &queue_msg);
		}
	} break;

	case BLE_EVT_SCAN_STATE: {
		if (msg->param == NULL) {
			break;
		}
		ble_scan_state_e data = *(ble_scan_state_e *)msg->param;
		g_scan_ctx.state = data;
		if (g_scan_ctx.callback.state_changed_cb) {
			memcpy(queue_msg.param, (void*[]){g_scan_ctx.callback.state_changed_cb, NULL, msg->param}, sizeof(void*) * queue_msg.count);
			ble_queue_enque(BLE_QUEUE_EVT_PRI_HIGH, &queue_msg);
		}
	} break;

	default:
		ret = TRBLE_UNKNOWN;
		break;
	}

handle_req_done:
	BLE_LOG_DEBUG("[BLEMGR] T%d <-- _handle_request\n", getpid());
	return _convert_ret(ret);
}
