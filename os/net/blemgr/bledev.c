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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/ble.h>
#include <tinyara/ble/ble_manager.h>
#include "bledev_mgr_client.h"
#include "bledev_mgr_server.h"

#define TRBLE_DRV_CALL(res, dev, method, param)		\
	do {										\
		if (dev->ops->method) {			\
			res = (dev->ops->method)param;	\
		}										\
	} while (0)

int bledev_handle(struct bledev *dev, lwnl_req cmd, void *data, uint32_t data_len)
{
	// To Do
	trble_result_e ret = TRBLE_FAIL;

	BLE_LOGV(TRBLE_TAG, "cmd(%d) data(%p) len(%d)\n", cmd.type, data, data_len);

	switch (cmd.type) {
	case LWNL_REQ_BLE_INIT:
	{
		trble_client_init_config *t_client = bledrv_client_get_fake_config();
		trble_server_init_config *t_server = (trble_server_init_config *)data;
		if (data == NULL) {
			t_server = bledrv_server_get_null_config();
		}

		TRBLE_DRV_CALL(ret, dev, init, (dev, t_client, t_server));
	}
	break;
	case LWNL_REQ_BLE_DEINIT:
	{
		TRBLE_DRV_CALL(ret, dev, deinit, (dev));
	}
	break;
	case LWNL_REQ_BLE_GET_MAC:
	{
		uint8_t *mac = (uint8_t *)data;
		if (mac != NULL) {
			TRBLE_DRV_CALL(ret, dev, get_mac, (dev, mac));
		} else {
			ret = TRBLE_INVALID_ARGS;
		}
	}
	break;
	case LWNL_REQ_BLE_GET_BONDED_DEV:
	{
		trble_bonded_device_list_s *device_list = NULL;
		uint16_t *device_count = NULL;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}
		device_list = (trble_bonded_device_list_s *)param.param[0];
		device_count = (uint16_t *)param.param[1];

		TRBLE_DRV_CALL(ret, dev, get_bonded_dev, (dev, device_list, device_count));
	}
	break;
	case LWNL_REQ_BLE_DEL_BOND:
	{
		uint8_t *addr = NULL;

		if (data != NULL) {
			addr = (uint8_t *)data;
		} else {
			return TRBLE_INVALID_ARGS;
		}
		
		TRBLE_DRV_CALL(ret, dev, del_bond, (dev, addr));
	}
	break;
	case LWNL_REQ_BLE_DEL_BOND_ALL:
	{
		TRBLE_DRV_CALL(ret, dev, del_bond_all, (dev));
	}
	break;
	case LWNL_REQ_BLE_CONN_IS_ACTIVE:
	{
		bool *is_active = NULL;
		trble_conn_handle con_handle = 0;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}
		con_handle = *(trble_conn_handle *)param.param[0];
		is_active = (bool *)param.param[1];

		TRBLE_DRV_CALL(ret, dev, conn_is_active, (dev, con_handle, is_active));
	}
	break;
	case LWNL_REQ_BLE_CONN_IS_ANY_ACTIVE:
	{
		bool *is_active = NULL;
		if (data != NULL) {
			is_active = (bool *)data;
		} else {
			return TRBLE_INVALID_ARGS;
		}
		TRBLE_DRV_CALL(ret, dev, conn_is_any_active, (dev, is_active));
	}
	break;
	
	// Client
	case LWNL_REQ_BLE_START_SCAN:
	{
		/* filter can be NULL */
		trble_scan_filter *filter = (trble_scan_filter *)data;
		TRBLE_DRV_CALL(ret, dev, start_scan, (dev, filter));
	}
	break;
	case LWNL_REQ_BLE_STOP_SCAN:
	{
		TRBLE_DRV_CALL(ret, dev, stop_scan, (dev));
	}
	break;
	case LWNL_REQ_BLE_CLIENT_CONNECT:
	{
		trble_conn_info *conn_info = NULL;
		if (data != NULL) {
			conn_info = (trble_conn_info *)data;
		} else {
			return TRBLE_INVALID_ARGS;
		}
		TRBLE_DRV_CALL(ret, dev, client_connect, (dev, conn_info));
	}
	break;
	case LWNL_REQ_BLE_CLIENT_DISCONNECT:
	{
		trble_conn_handle con_handle = 0;

		if (data != NULL) {
			con_handle = *(trble_conn_handle *)data;
		} else {
			return TRBLE_INVALID_ARGS;
		}

		TRBLE_DRV_CALL(ret, dev, client_disconnect, (dev, con_handle));
	}
	break;
	case LWNL_REQ_BLE_CLIENT_DISCONNECT_ALL:
	{
		TRBLE_DRV_CALL(ret, dev, client_disconnect_all, (dev));
	}
	break;
	case LWNL_REQ_BLE_CONNECTED_DEV_LIST:
	{
		trble_connected_list *list = NULL;
		if (data != NULL) {
			list = (trble_connected_list *)data;
		} else {
			return TRBLE_INVALID_ARGS;
		}
		TRBLE_DRV_CALL(ret, dev, conn_dev_list, (dev, list));
	}
	break;
	case LWNL_REQ_BLE_CONNECTED_INFO:
	{
		trble_device_connected *device = NULL;
		trble_conn_handle handle;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}
		handle = *(trble_conn_handle *)param.param[0];
		device = (trble_device_connected *)param.param[1];

		TRBLE_DRV_CALL(ret, dev, conn_info, (dev, handle, device));
	}
	break;
	case LWNL_REQ_BLE_OP_ENABLE_NOTI:
	{
		trble_operation_handle *handle = NULL;
		if (data != NULL) {
			handle = (trble_operation_handle *)data;
		} else {
			return TRBLE_INVALID_ARGS;
		}

		TRBLE_DRV_CALL(ret, dev, op_enable_noti, (dev, handle));
	}
	break;
	case LWNL_REQ_BLE_OP_READ:
	{
		trble_operation_handle *handle = NULL;
		trble_data *buf = NULL;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}
		handle = (trble_operation_handle *)param.param[0];
		buf = (trble_data *)param.param[1];

		TRBLE_DRV_CALL(ret, dev, op_read, (dev, handle, buf));
	}
	break;
	case LWNL_REQ_BLE_OP_WRITE:
	{
		trble_operation_handle *handle = NULL;
		trble_data *buf = NULL;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}
		handle = (trble_operation_handle *)param.param[0];
		buf = (trble_data *)param.param[1];

		TRBLE_DRV_CALL(ret, dev, op_write, (dev, handle, buf));
	}
	break;
	case LWNL_REQ_BLE_OP_WRITE_NO_RESP:
	{
		trble_operation_handle *handle = NULL;
		trble_data *buf = NULL;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}
		handle = (trble_operation_handle *)param.param[0];
		buf = (trble_data *)param.param[1];

		TRBLE_DRV_CALL(ret, dev, op_wrtie_no_resp, (dev, handle, buf));
	}
	break;
	
	//Server
	case LWNL_REQ_BLE_GET_PROFILE_COUNT:
	{
		uint16_t *count = NULL;
		if (data != NULL) {
			count = (uint16_t *)data;
		} else {
			return TRBLE_INVALID_ARGS;
		}
		TRBLE_DRV_CALL(ret, dev, get_profile_count, (dev, count));
	}
	break;
	case LWNL_REQ_BLE_CHARACT_NOTI:
	{
		trble_attr_handle attr_handle = 0;
		trble_conn_handle con_handle = 0;
		trble_data *buf = NULL;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}
		attr_handle = *(trble_attr_handle *)param.param[0];
		con_handle = *(trble_conn_handle *)param.param[1];
		buf = (trble_data *)param.param[2];

		TRBLE_DRV_CALL(ret, dev, charact_noti, (dev, attr_handle, con_handle, buf));
	}
	break;
	case LWNL_REQ_BLE_ATTR_SET_DATA:
	{
		trble_attr_handle attr_handle = 0;
		trble_data *buf = NULL;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}
		attr_handle = *(trble_attr_handle *)param.param[0];
		buf = (trble_data *)param.param[1];

		TRBLE_DRV_CALL(ret, dev, attr_set_data, (dev, attr_handle, buf));
	}
	break;
	case LWNL_REQ_BLE_ATTR_GET_DATA:
	{
		trble_attr_handle attr_handle = 0;
		trble_data *buf = NULL;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}
		attr_handle = *(trble_attr_handle *)param.param[0];
		buf = (trble_data *)param.param[1];

		TRBLE_DRV_CALL(ret, dev, attr_get_data, (dev, attr_handle, buf));
	}
	break;
	case LWNL_REQ_BLE_ATTR_REJECT:
	{
		trble_attr_handle attr_handle = 0;
		uint8_t err_code = 0;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}
		attr_handle = *(trble_attr_handle *)param.param[0];
		err_code = *(uint8_t *)param.param[1];

		TRBLE_DRV_CALL(ret, dev, attr_reject, (dev, attr_handle, err_code));
	}
	break;
	case LWNL_REQ_BLE_SERVER_DISCONNECT:
	{
		trble_conn_handle con_handle = 0;

		if (data != NULL) {
			con_handle = *(trble_conn_handle *)data;
		} else {
			return TRBLE_INVALID_ARGS;
		}

		TRBLE_DRV_CALL(ret, dev, server_disconnect, (dev, con_handle));
	}
	break;
	case LWNL_REQ_BLE_GET_MAC_BY_CONN:
	{
		trble_conn_handle con_handle = 0;
		uint8_t *bd_addr;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}
		con_handle = *(trble_conn_handle *)param.param[0];
		bd_addr = (uint8_t *)param.param[1];

		TRBLE_DRV_CALL(ret, dev, get_mac_by_conn, (dev, con_handle, bd_addr));
	}
	break;
	case LWNL_REQ_BLE_GET_CONN_BY_MAC:
	{
		uint8_t *bd_addr = NULL;
		trble_conn_handle *con_handle;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}
		bd_addr = (uint8_t *)param.param[0];
		con_handle = (trble_conn_handle *)param.param[1];

		TRBLE_DRV_CALL(ret, dev, get_conn_by_mac, (dev, bd_addr, con_handle));
	}
	break;
	case LWNL_REQ_BLE_SET_ADV_DATA:
	{
		trble_data *buf = NULL;
		if (data != NULL) {
			buf = (trble_data *)data;
		} else {
			return TRBLE_INVALID_ARGS;
		}
		TRBLE_DRV_CALL(ret, dev, set_adv_data, (dev, buf));
	}
	break;
	case LWNL_REQ_BLE_SET_ADV_RESP:
	{
		trble_data *buf = NULL;
		if (data != NULL) {
			buf = (trble_data *)data;
		} else {
			return TRBLE_INVALID_ARGS;
		}
		TRBLE_DRV_CALL(ret, dev, set_adv_resp, (dev, buf));
	}
	break;
	case LWNL_REQ_BLE_START_ADV:
	{
		TRBLE_DRV_CALL(ret, dev, start_adv, (dev));
	}
	break;
	case LWNL_REQ_BLE_SET_ADV_TYPE:
	{
		trble_adv_type_e adv_type;
		trble_addr *addr;

		lwnl_msg_params param = { 0, };
		if (data != NULL) {
			memcpy(&param, data, data_len);
		} else {
			return TRBLE_INVALID_ARGS;
		}

		adv_type = *(trble_adv_type_e *)param.param[0];
		addr = (trble_addr *)param.param[1];

		TRBLE_DRV_CALL(ret, dev, set_adv_type, (dev, adv_type, addr));
	}
	break;
	case LWNL_REQ_BLE_STOP_ADV:
	{
		TRBLE_DRV_CALL(ret, dev, stop_adv, (dev));
	}
	break;
	case LWNL_REQ_BLE_SET_ADV_INTERVAL:
	{
		uint16_t interval = 0;

		if (data != NULL) {
			interval = *(uint16_t *)data;
		} else {
			return TRBLE_INVALID_ARGS;
		}

		TRBLE_DRV_CALL(ret, dev, set_adv_interval, (dev, interval));
	}
	break;
	default:
		ret = TRBLE_UNKNOWN;
		break;
	}

	BLE_LOGV(TRBLE_TAG, "trble drv result : %d\n", ret);
	return ret;
}

int trble_post_event(lwnl_cb_ble evt, void *buffer, uint32_t buf_len)
{
	BLE_LOGV(TRBLE_TAG, "trble post event : %d\n", evt);
	return lwnl_postmsg(LWNL_DEV_BLE, (int)evt, buffer, buf_len);
}
