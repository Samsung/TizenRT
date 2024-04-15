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

#pragma once

#include <tinyara/config.h>
#include <tinyara/net/if/ble.h>
#include "ble_log.h"

#define TRBLE_TAG "[BLEMGR]"

#define BLE_IF_NAME 10

struct bledev {
	char ifname[BLE_IF_NAME];
	uint8_t hwaddr[TRBLE_BD_ADDR_MAX_LEN];
	struct trble_ops *ops;
	void *priv;
};

struct bledev *net_get_bledev(uint8_t *ifname);
struct bledev *bledev_register(struct trble_ops *ops);

/*** Common ***/
trble_result_e ble_drv_init(trble_server_init_config *server, trble_queue *scan_queue);
trble_result_e ble_drv_deinit(void);
trble_result_e ble_drv_get_mac_addr(uint8_t mac[TRBLE_BD_ADDR_MAX_LEN]);
trble_result_e ble_drv_get_bonded_device(trble_bonded_device_list_s *device_list, uint16_t *device_count);
trble_result_e ble_drv_delete_bonded(trble_addr *addr);
trble_result_e ble_drv_delete_bonded_all(void);
trble_result_e ble_drv_conn_is_active(trble_conn_handle con_handle, bool *is_active);
trble_result_e ble_drv_conn_is_any_active(bool *is_active);
trble_result_e ble_drv_conn_param_update(trble_conn_handle *con_handle, trble_conn_param *conn_param);
trble_result_e ble_drv_ioctl(trble_msg_s *msg);

/*** Scanner(Observer) ***/
trble_result_e ble_drv_start_scan(trble_scan_filter *filter);
trble_result_e ble_drv_stop_scan(void);
trble_result_e ble_drv_scan_whitelist_add(trble_addr *addr);
trble_result_e ble_drv_scan_whitelist_delete(trble_addr *addr);
trble_result_e ble_drv_scan_whitelist_clear_all(void);

/*** Central(Client) ***/
trble_result_e ble_drv_client_connect(trble_conn_info *conn_info);
trble_result_e ble_drv_client_disconnect(trble_conn_handle con_handle);
trble_result_e ble_drv_client_disconnect_all(void);
trble_result_e ble_drv_connected_device_list(trble_connected_list *out_connected_list);
trble_result_e ble_drv_connected_info(trble_conn_handle conn_handle, trble_device_connected *out_connected_device);
trble_result_e ble_drv_operation_enable_notification(trble_operation_handle *handle);
trble_result_e ble_drv_operation_read(trble_operation_handle *handle, trble_data *out_data);
trble_result_e ble_drv_operation_write(trble_operation_handle *handle, trble_data *in_data);
trble_result_e ble_drv_operation_write_no_response(trble_operation_handle *handle, trble_data *in_data);

/*** Peripheral(Server) ***/
trble_result_e ble_drv_get_profile_count(uint16_t *count);
trble_result_e ble_drv_charact_notify(trble_attr_handle attr_handle, trble_conn_handle con_handle, trble_data *data);
trble_result_e ble_drv_charact_indicate(trble_attr_handle attr_handle, trble_conn_handle con_handle, trble_data *data);
trble_result_e ble_drv_attr_set_data(trble_attr_handle attr_handle, trble_data *data);
trble_result_e ble_drv_attr_get_data(trble_attr_handle attr_handle, trble_data *data);
trble_result_e ble_drv_attr_reject(trble_attr_handle attr_handle, uint8_t app_errorcode);
trble_result_e ble_drv_server_disconnect(trble_conn_handle con_handle);
trble_result_e ble_drv_get_mac_addr_by_conn_handle(trble_conn_handle con_handle, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN]);
trble_result_e ble_drv_get_conn_handle_by_addr(uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN], trble_conn_handle *con_handle);
trble_result_e ble_drv_set_device_name(uint8_t* name);

/*** Advertiser(Broadcaster) ***/
trble_result_e ble_drv_set_adv_data(trble_data *data);
trble_result_e ble_drv_set_adv_resp(trble_data *data);
trble_result_e ble_drv_set_adv_type(trble_adv_type_e adv_type, trble_addr *addr);
trble_result_e ble_drv_set_adv_interval(uint16_t interval);
trble_result_e ble_drv_set_adv_txpower(uint8_t txpower);
trble_result_e ble_drv_start_adv(void);
trble_result_e ble_drv_stop_adv(void);
trble_result_e ble_drv_one_shot_adv_init(void);
trble_result_e ble_drv_one_shot_adv_deinit(void);
trble_result_e ble_drv_one_shot_adv(trble_data *data_adv, trble_data *data_scan_rsp, uint8_t *type);
