/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include "ble_tizenrt_coc.h"
#include "ble_tizenrt_advertiser.h"

///common
trble_result_e bktr_ble_init(trble_client_init_config* init_client, trble_server_init_config* init_server);
trble_result_e bktr_ble_deinit(void);
trble_result_e bktr_ble_common_sm_set_security_param(trble_sec_param *sec_param);
trble_result_e bktr_ble_common_pairing_passkey_confirm(uint8_t *conn_handle, uint8_t *confirm);
trble_result_e bktr_ble_common_get_bonded_device(trble_bonded_device_list_s* bonded_device_list, uint16_t* device_count);
trble_result_e bktr_ble_common_delete_bond(trble_addr* addr);
trble_result_e bktr_ble_common_delete_bond_all(void);
bool bktr_ble_common_conn_is_active(trble_conn_handle con_handle);
bool bktr_ble_common_conn_is_any_active(void);
trble_result_e bktr_ble_common_conn_param_update(trble_conn_handle *conn_handle, trble_conn_param *conn_param);
trble_result_e bktr_ble_common_get_version(uint8_t *version);
trble_result_e bktr_ble_common_get_mac_address(uint8_t mac[TRBLE_BD_ADDR_MAX_LEN]);
trble_result_e bktr_ble_common_pairing_passkey_confirm(uint8_t *conn_handle, uint8_t *confirm);

///Scanner(Observer)
trble_result_e bktr_ble_client_set_scan(uint16_t scan_interval, uint16_t scan_window, trble_scan_type scan_type);
trble_result_e bktr_ble_client_start_scan_with_filter(trble_scan_filter* scan_parm, bool whitelist_enable);
trble_result_e bktr_ble_client_stop_scan(void);
trble_result_e bktr_ble_client_scan_whitelist_add(trble_addr *addr);
trble_result_e bktr_ble_client_scan_whitelist_delete(trble_addr *addr);
trble_result_e bktr_ble_client_scan_whitelist_clear_all(void);

///Central(Client)
trble_result_e bktr_ble_client_connect(trble_conn_info* conn_info, bool is_secured_connect);
trble_result_e bktr_ble_client_bond(trble_conn_handle conn_handle);
trble_result_e bktr_ble_client_conn_param_update(trble_conn_handle *conn_handle, trble_conn_param *conn_param);
trble_result_e bktr_ble_client_read_connected_device_list(trble_connected_list* out_connected_list);
trble_result_e bktr_ble_client_read_connected_info(trble_conn_handle conn_handle, trble_device_connected* out_connected_device);
trble_result_e bktr_ble_client_disconnect(trble_conn_handle conn_handle);
trble_result_e bktr_ble_client_disconnect_all(void);
trble_result_e bktr_ble_client_operation_read(trble_operation_handle* handle, trble_data* out_data);
trble_result_e bktr_ble_client_operation_write(trble_operation_handle* handle, trble_data* in_data);
trble_result_e bktr_ble_client_operation_write_no_response(trble_operation_handle* handle, trble_data* in_data);
trble_result_e bktr_ble_client_write_read_queue_cnt(trble_conn_handle* handle, uint8_t* write_read_count);
trble_result_e bktr_ble_client_operation_enable_notification(trble_operation_handle* handle);
trble_result_e bktr_ble_client_operation_enable_indication(trble_operation_handle* handle);
trble_result_e bktr_ble_client_operation_enable_notification_and_indication(trble_operation_handle* handle);

///server
int32_t bk_tr_ble_server_init(trble_server_init_config *config);
uint16_t bktr_ble_server_get_profile_count(void);
trble_server_init_config *bktr_ble_server_get_param(void);
int32_t bk_tr_ble_server_attr_set_data_ptr(trble_attr_handle attr_handle, uint8_t *buffer, uint16_t buffer_len);
int32_t bk_tr_ble_server_attr_get_data_ptr(trble_attr_handle attr_handle, uint8_t **buffer, uint16_t *buffer_len, uint16_t *buffer_max_len);
int32_t bk_tr_ble_server_charact_notify(trble_attr_handle attr_handle, trble_conn_handle con_handle, uint8_t *buffer, uint16_t buffer_len, uint8_t is_notify);
int32_t bk_tr_ble_server_disconnect(trble_conn_handle conn_handle);
int32_t bk_tr_ble_server_get_mac_addr_by_conn_handle(trble_conn_handle conn_handle, uint8_t *bd_addr);
int32_t bk_tr_ble_server_get_conn_handle_by_addr(uint8_t *bd_addr, trble_conn_handle *con_handle);
int32_t bk_tr_ble_server_set_gap_device_name(uint8_t len, uint8_t *device_name);
int32_t bk_tr_ble_server_indicate_queue_count(trble_conn_handle *con_handle, uint8_t *count);
int32_t bk_tr_ble_server_attr_reject(trble_attr_handle attr_handle, uint8_t app_errorcode);

//adv
int32_t bk_tr_ble_advertiser_set_adv_data(uint8_t *data, uint16_t length);
int32_t bk_tr_ble_advertiser_set_adv_resp(uint8_t *data, uint16_t length);
int32_t bk_tr_ble_advertiser_set_adv_type(trble_adv_type_e adv_type, trble_addr *addr);
int32_t bk_tr_ble_advertiser_set_adv_interval(uint16_t interval);
int32_t bk_tr_ble_advertiser_adv_enable(uint8_t enable);
int32_t bk_tr_ble_advertiser_create_multi_adv(uint8_t adv_event_prop,
        uint32_t primary_adv_interval[2], //note: see rtk impl
        uint8_t own_addr_type,
        uint8_t *own_addr_val,
        uint8_t *adv_handle);
int32_t bk_tr_ble_advertiser_delete_multi_adv(uint8_t adv_handle);
int32_t bk_tr_ble_advertiser_set_multi_adv_data(uint8_t adv_handle, uint8_t *pdata, uint8_t len);
int32_t bk_tr_ble_advertiser_set_multi_resp_data(uint8_t adv_handle, uint8_t *pdata, uint8_t len);
int32_t bk_tr_ble_advertiser_multi_adv_enable(uint8_t adv_handle, uint8_t enable);
int32_t bk_tr_ble_advertiser_one_shot_adv_init(void);
int32_t bk_tr_ble_advertiser_one_shot_adv_deinit(void);
int32_t bk_tr_ble_advertiser_one_shot_adv_set(uint8_t *adv_id, trble_data *data_adv, trble_data *data_scan_rsp, uint8_t *type);
int32_t bk_tr_ble_advertiser_one_shot_adv_enable(uint8_t adv_id);
int32_t bk_tr_ble_advertiser_set_multi_adv_type(uint8_t adv_handle, uint8_t adv_event_prop, trble_addr *addr);
int32_t bk_tr_ble_advertiser_set_multi_adv_interval(uint8_t adv_handle, unsigned int interval);
int32_t bk_tr_ble_advertiser_set_multi_adv_tx_power(uint8_t adv_handle, uint8_t txpower);
