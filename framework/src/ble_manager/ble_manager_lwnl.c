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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <debug.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/ble/ble_manager.h>
#include <tinyara/net/if/ble.h>
#include "ble_manager_log.h"

#define BLE_INTF_NAME "ble0"

static inline int _send_msg(lwnl_msg *msg)
{
	int fd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (fd < 0) {
		BLE_LOG_ERROR("send lwnl msg open error(%d)\n", get_errno());
		return -1;
	}

	int res = write(fd, msg, sizeof(*msg));
	close(fd);
	if (res < 0) {
		BLE_LOG_ERROR("send lwnl msg write error(%d)\n", get_errno());
		return -2;
	}
	return 0;
}

/* Common */
trble_result_e ble_drv_init(trble_server_init_config *server, trble_queue *scan_queue)
{
	trble_result_e res = TRBLE_SUCCESS;

	lwnl_msg_params msg_data = { 2, {(void *)server, (void *)scan_queue} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_INIT}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_deinit(void)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_DEINIT}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_get_mac_addr(uint8_t mac[TRBLE_BD_ADDR_MAX_LEN])
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_GET_MAC}, TRBLE_BD_ADDR_MAX_LEN, (void *)mac, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_get_bonded_device(trble_bonded_device_list_s *device_list, uint16_t *device_count)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)device_list, (void *)device_count} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_GET_BONDED_DEV}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_delete_bonded(trble_addr *addr)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_DEL_BOND}, TRBLE_BD_ADDR_MAX_LEN, (void *)addr, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_delete_bonded_all(void)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_DEL_BOND_ALL}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_conn_is_active(trble_conn_handle con_handle, bool *is_active)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)&con_handle, (void *)is_active} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_CONN_IS_ACTIVE}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_conn_is_any_active(bool *is_active)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_CONN_IS_ANY_ACTIVE}, sizeof(bool), (void *)is_active, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_ioctl(trble_msg_s *dmsg)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_IOCTL}, sizeof(trble_msg_s), (void *)dmsg, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

/*** Scanner(Observer) ***/
trble_result_e ble_drv_start_scan(trble_scan_filter *filter)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_START_SCAN}, sizeof(trble_scan_filter), (void *)filter, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_stop_scan()
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_STOP_SCAN}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_scan_whitelist_add(trble_addr *addr)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_WHITELIST_ADD}, sizeof(trble_addr), addr, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_scan_whitelist_delete(trble_addr *addr)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_WHITELIST_DELETE}, sizeof(trble_addr), addr, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_scan_whitelist_clear_all(void)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_WHITELIST_CLEAR_ALL}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

/*** Central(Client) ***/
trble_result_e ble_drv_client_connect(trble_conn_info *conn_info)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_CLIENT_CONNECT}, sizeof(trble_conn_info), (void *)conn_info, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_client_disconnect(trble_conn_handle con_handle)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_CLIENT_DISCONNECT}, sizeof(trble_conn_handle), (void *)(&con_handle), (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_client_disconnect_all()
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_CLIENT_DISCONNECT_ALL}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_connected_device_list(trble_connected_list *out_connected_list)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_CONNECTED_DEV_LIST}, sizeof(trble_connected_list *), (void *)out_connected_list, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_connected_info(trble_conn_handle conn_handle, trble_device_connected *out_connected_device)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)&conn_handle, (void *)out_connected_device} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_CONNECTED_INFO}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_operation_enable_notification(trble_operation_handle *handle)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_OP_ENABLE_NOTI}, sizeof(trble_operation_handle *), (void *)handle, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_operation_enable_indication(trble_operation_handle *handle)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_OP_ENABLE_INDICATE}, sizeof(trble_operation_handle *), (void *)handle, (void *)&res};
	if (_send_msg(&msg) < 0) {
		dbg("res %d\n", res);
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_operation_enable_notification_and_indication(trble_operation_handle *handle)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_OP_ENABLE_NOTI_AND_INDICATE}, sizeof(trble_operation_handle *), (void *)handle, (void *)&res};
	if (_send_msg(&msg) < 0) {
	dbg("res %d\n", res);
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_operation_read(trble_operation_handle *handle, trble_data *out_data)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)handle, (void *)out_data} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_OP_READ}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_operation_write(trble_operation_handle *handle, trble_data *in_data)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)handle, (void *)in_data} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_OP_WRITE}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_operation_write_no_response(trble_operation_handle *handle, trble_data *in_data)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)handle, (void *)in_data} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_OP_WRITE_NO_RESP}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

/*** Peripheral(Server) ***/
trble_result_e ble_drv_get_profile_count(uint16_t *count)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_GET_PROFILE_COUNT}, sizeof(uint16_t *), (void *)count, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_charact_notify(trble_attr_handle attr_handle, trble_conn_handle con_handle, trble_data *data)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 3, {(void *)&attr_handle, (void *)&con_handle, (void *)data} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_CHARACT_NOTI}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_charact_indicate(trble_attr_handle attr_handle, trble_conn_handle con_handle, trble_data *data)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 3, {(void *)&attr_handle, (void *)&con_handle, (void *)data} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_CHARACT_INDI}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_attr_set_data(trble_attr_handle attr_handle, trble_data *data)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)&attr_handle, (void *)data} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_ATTR_SET_DATA}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_attr_get_data(trble_attr_handle attr_handle, trble_data *data)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)&attr_handle, (void *)data} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_ATTR_GET_DATA}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_attr_reject(trble_attr_handle attr_handle, uint8_t app_errorcode)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)&attr_handle, (void *)&app_errorcode} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_ATTR_REJECT}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_conn_param_update(trble_conn_handle *con_handle, trble_conn_param *conn_param)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)con_handle, (void *)conn_param} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_CONN_PARAM_UPDATE}, sizeof(msg_data), (void *)(&msg_data), (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}
trble_result_e ble_drv_server_disconnect(trble_conn_handle con_handle)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_SERVER_DISCONNECT}, sizeof(trble_conn_handle), (void *)(&con_handle), (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_get_mac_addr_by_conn_handle(trble_conn_handle con_handle, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN])
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)&con_handle, (void *)bd_addr} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_GET_MAC_BY_CONN}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_get_conn_handle_by_addr(uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN], trble_conn_handle *con_handle)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)bd_addr, (void *)con_handle} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_GET_CONN_BY_MAC}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_set_adv_data(trble_data *data)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_SET_ADV_DATA}, sizeof(trble_data), (void *)data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_set_adv_resp(trble_data *data)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_SET_ADV_RESP}, sizeof(trble_data), (void *)data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_set_adv_type(trble_adv_type_e adv_type, trble_addr *addr)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 2, {(void *)&adv_type, (void *)addr} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_SET_ADV_TYPE}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_set_adv_interval(uint16_t interval)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_SET_ADV_INTERVAL}, sizeof(interval), (void *)&interval, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_set_adv_txpower(uint8_t txpower)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_SET_ADV_TXPOWER}, sizeof(txpower), (void *)&txpower, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_start_adv()
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_START_ADV}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_stop_adv()
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_STOP_ADV}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_one_shot_adv_init()
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_ONE_SHOT_ADV_INIT}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_one_shot_adv_deinit()
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_ONE_SHOT_ADV_DEINIT}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_one_shot_adv(trble_data *data_adv, trble_data *data_scan_rsp, uint8_t *type)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg_params msg_data = { 3, {(void *)data_adv, (void *)data_scan_rsp, (void *)type} };
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_ONE_SHOT_ADV}, sizeof(msg_data), (void *)&msg_data, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}

trble_result_e ble_drv_set_device_name(uint8_t* name)
{
	trble_result_e res = TRBLE_SUCCESS;
	lwnl_msg msg = {BLE_INTF_NAME, {LWNL_REQ_BLE_SET_DEVICE_NAME}, sizeof(name), (void *)name, (void *)&res};
	if (_send_msg(&msg) < 0) {
		res = TRBLE_FILE_ERROR;
	}
	return res;
}
