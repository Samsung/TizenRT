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

#ifdef CONFIG_AMEBAD_BLE_SCATTERNET
extern trble_result_e rtw_ble_combo_init(trble_client_init_config* init_client, trble_server_init_config* init_server);
extern trble_result_e rtw_ble_combo_deinit(void);
#elif defined(CONFIG_AMEBAD_BLE_CENTRAL)
extern trble_result_e rtw_ble_client_init(trble_client_init_config* init_parm);
extern trble_result_e rtw_ble_client_deinit(void);
#elif defined(CONFIG_AMEBAD_BLE_PERIPHERAL)
extern trble_result_e rtw_ble_server_init(trble_server_init_config* init_parm);
extern trble_result_e rtw_ble_server_deinit(void);
#endif

extern trble_result_e rtw_ble_server_get_mac_address(uint8_t mac[TRBLE_BD_ADDR_MAX_LEN]);
extern uint16_t rtw_ble_server_get_profile_count(void);
extern trble_result_e rtw_ble_server_charact_notify(trble_attr_handle attr_handle, trble_conn_handle con_handle, uint8_t *data_ptr, uint16_t data_length);
extern trble_result_e rtw_ble_server_att_set_data_ptr(trble_attr_handle attr_handle, uint8_t *new_data_ptr);
extern trble_result_e rtw_ble_server_att_set_length(trble_attr_handle attr_handle, uint16_t new_length);
extern uint8_t* rtw_ble_server_att_get_data_ptr(trble_attr_handle attr_handle);
extern uint16_t rtw_ble_server_att_get_length(trble_attr_handle attr_handle);
extern trble_result_e rtw_ble_server_reject(trble_attr_handle attr_handle, uint8_t app_errorcode);
extern uint8_t* rtw_ble_server_get_mac_address_by_conn_handle(trble_conn_handle con_handle);
extern trble_conn_handle rtw_ble_server_get_conn_handle_by_address(uint8_t* mac);
extern trble_result_e rtw_ble_server_set_device_name(uint8_t* name);
extern trble_result_e rtw_ble_server_set_adv_data(uint8_t* data, uint16_t length);
extern trble_result_e rtw_ble_server_set_adv_name(uint8_t* data, uint16_t length);
extern trble_result_e rtw_ble_server_set_adv_type(trble_adv_type_e type, trble_addr *param);
extern bool rtw_ble_server_conn_is_active(trble_conn_handle con_handle);
extern bool rtw_ble_server_conn_is_any_active(void);
extern trble_result_e rtw_ble_server_disconnect(trble_conn_handle con_handle);
extern trble_result_e rtw_ble_server_start_adv(void);
extern trble_result_e rtw_ble_server_stop_adv(void);
extern trble_result_e rtw_ble_server_get_bonded_device(trble_bonded_device_list_s* bonded_device_list, uint16_t* device_count);
extern trble_result_e rtw_ble_server_delete_bonded_device(uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN]);
extern trble_result_e rtw_ble_server_delete_bonded_device_all(void);
extern trble_result_e rtw_ble_server_set_adv_interval(uint16_t interval); // interval range (0x0020 ~ 0x4000)

extern trble_result_e rtw_ble_client_scan_whiltelist_add(trble_addr *addr);
extern trble_result_e rtw_ble_client_scan_whiltelist_delete(trble_addr *addr);
extern trble_result_e rtw_ble_client_scan_whiltelist_clear_all(void);

extern trble_result_e rtw_ble_client_start_scan(void);
extern trble_result_e rtw_ble_client_start_scan_with_filter(trble_scan_filter* scan_parm, bool whitelist_enable);
extern trble_result_e rtw_ble_client_stop_scan(void);
extern trble_result_e rtw_ble_client_connect(trble_conn_info* conn_info, bool is_secured_connect);
extern trble_result_e rtw_ble_client_read_connected_device_list(trble_connected_list* out_connected_list);
extern trble_result_e rtw_ble_client_read_connected_info(trble_conn_handle conn_handle, trble_device_connected* out_connected_device);
extern trble_result_e rtw_ble_client_delete_bond(trble_addr* addr);
extern trble_result_e rtw_ble_client_delete_bond_all(void);
extern trble_result_e rtw_ble_client_disconnect(trble_conn_handle conn_handle);
extern trble_result_e rtw_ble_client_disconnect_all(void);
extern trble_result_e rtw_ble_client_operation_read(trble_operation_handle* handle, trble_data* out_data);
extern trble_result_e rtw_ble_client_operation_write(trble_operation_handle* handle, trble_data* in_data);
extern trble_result_e rtw_ble_client_operation_enable_notification(trble_operation_handle* handle);
extern trble_result_e rtw_ble_client_operation_write_no_response(trble_operation_handle* handle, trble_data* in_data);
