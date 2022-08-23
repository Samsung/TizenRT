/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#ifndef __BLE_TIZENRT_IPC_H__
#define __BLE_TIZENRT_IPC_H__

/* -------------------------------- Includes -------------------------------- */
#include <ble_tizenrt_defs.h>
#include <tinyara/net/if/ble.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	trble_attr_cb_type_e type;
  trble_conn_handle conn_id;
  trble_attr_handle att_handle;
  void *arg;
  trble_server_cb_t cb;
} T_TIZENRT_PROFILE_CALLBACK_DATA;

typedef struct
{
	trble_conn_handle conn_id;
  trble_server_connection_type_e conn_type;
  uint8_t remote_bd[TRBLE_BD_ADDR_MAX_LEN];
} T_TIZENRT_CONNECTED_CALLBACK_DATA;

typedef struct
{
  trble_operation_handle handle;
  trble_data noti_data;
} T_TIZENRT_CLIENT_NOTIFICATION;

trble_result_e rtw_ble_server_init(trble_server_init_config* init_parm);

trble_result_e rtw_ble_server_deinit(void);

trble_result_e rtw_ble_server_get_mac_address(uint8_t mac[TRBLE_BD_ADDR_MAX_LEN]);

/* set data pointer of attribute value */
trble_result_e rtw_ble_server_att_set_data_ptr(trble_attr_handle attr_handle, uint8_t *new_data_ptr);

/* set current length of attribute value */
trble_result_e rtw_ble_server_att_set_length(trble_attr_handle attr_handle, uint16_t new_length);

/* get data pointer of attribute value */
uint8_t* rtw_ble_server_att_get_data_ptr(trble_attr_handle attr_handle);

/* get current length of attribute value */
uint16_t rtw_ble_server_att_get_length(trble_attr_handle attr_handle);

trble_result_e rtw_ble_server_charact_notify(trble_attr_handle attr_handle, trble_conn_handle con_handle, uint8_t *data_ptr, uint16_t data_length);

trble_result_e rtw_ble_server_reject(trble_attr_handle attr_handle, uint8_t app_errorcode);

uint8_t* rtw_ble_server_get_mac_address_by_conn_handle(trble_conn_handle con_handle);

trble_conn_handle rtw_ble_server_get_conn_handle_by_address(uint8_t* mac);

void rtw_ble_server_adv_into_idle(void);

/* Set Advertisement Data API */
trble_result_e rtw_ble_server_set_adv_data(uint8_t* data, uint16_t length);

trble_result_e rtw_ble_server_set_adv_name(uint8_t* data, uint16_t length);

bool rtw_ble_server_conn_is_active(trble_conn_handle con_handle);

bool rtw_ble_server_conn_is_any_active(void);

trble_result_e rtw_ble_server_disconnect(trble_conn_handle con_handle);

trble_result_e rtw_ble_server_start_adv(void);

trble_result_e rtw_ble_server_stop_adv(void);

trble_result_e rtw_ble_server_get_bonded_device(trble_bonded_device_list_s* bonded_device_list, uint16_t* device_count);

trble_result_e rtw_ble_server_delete_bonded_device(uint8_t bond_addr[TRBLE_BD_ADDR_MAX_LEN]);

trble_result_e rtw_ble_server_delete_bonded_device_all(void);

int rtw_ble_server_set_adv_interval(unsigned int interval);

trble_result_e rtw_ble_server_set_adv_type(trble_adv_type_e type, trble_addr *addr);

trble_result_e rtw_ble_client_init(trble_client_init_config* init_parm);

trble_result_e rtw_ble_client_scan_whitelist_add(trble_addr *addr);

trble_result_e rtw_ble_client_scan_whitelist_delete(trble_addr *addr);

trble_result_e rtw_ble_client_scan_whitelist_clear_all(void);

trble_result_e rtw_ble_client_start_scan(void);

trble_result_e rtw_ble_client_start_scan_with_filter(trble_scan_filter* scan_parm, bool whitelist_enable);

trble_result_e rtw_ble_client_stop_scan(void);

trble_result_e rtw_ble_client_connect(trble_conn_info* conn_info, bool is_secured_connect);

trble_result_e rtw_ble_client_read_connected_device_list(trble_connected_list* out_connected_list);

trble_result_e rtw_ble_client_read_connected_info(trble_conn_handle conn_handle, trble_device_connected* out_connected_device);

trble_result_e rtw_ble_client_delete_bond(trble_addr* addr);

trble_result_e rtw_ble_client_delete_bond_all(void);

trble_result_e rtw_ble_client_disconnect(trble_conn_handle conn_handle);

trble_result_e rtw_ble_client_disconnect_all(void);

trble_result_e rtw_ble_client_operation_read(trble_operation_handle* handle, trble_data* out_data);

trble_result_e rtw_ble_client_operation_write(trble_operation_handle* handle, trble_data* in_data);

trble_result_e rtw_ble_client_operation_write_no_response(trble_operation_handle* handle, trble_data* in_data);

trble_result_e rtw_ble_client_operation_enable_notification(trble_operation_handle* handle);

trble_result_e rtw_ble_client_deinit(void);

trble_result_e rtw_ble_combo_init(trble_client_init_config* init_client, trble_server_init_config* init_server);

trble_result_e rtw_ble_combo_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /*!< __RTK_BT_A2DP_H__ */
