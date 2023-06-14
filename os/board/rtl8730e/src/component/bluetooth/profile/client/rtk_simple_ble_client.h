/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */


#ifndef __RTK_SIMPLE_BLE_CLIENT_H__
#define __RTK_SIMPLE_BLE_CLIENT_H__

#ifdef __cplusplus
extern "C"  {
#endif

#include <rtk_bt_common.h>

#define SIMPLE_BLE_CLIENT_SHOW_DETAIL         1

typedef enum {
	SIMPLE_BLE_CHAR_READ_V0 = 0,
	SIMPLE_BLE_CHAR_WRITE_V1 = 1,
	SIMPLE_BLE_CHAR_NOTIFY_V2 = 2,
	SIMPLE_BLE_CHAR_INDICATE_V3 = 3,
	SIMPLE_BLE_CHAR_NUM,
} simple_ble_charac_index_t;

rtk_bt_evt_cb_ret_t simple_ble_client_app_callback(uint8_t event, void *data);

uint16_t simple_ble_client_attach_conn(uint16_t conn_handle);

uint16_t simple_ble_client_detach_conn(uint16_t conn_handle);

uint16_t simple_ble_client_add(void);

uint16_t simple_ble_client_delete(void);

#if !RTK_BLE_MGR_LIB
uint16_t simple_ble_client_srv_discover(uint16_t conn_handle);
#endif

uint16_t simple_ble_client_read_charac_v0(uint16_t conn_handle);

uint16_t simple_ble_client_write_charac_v1(uint16_t conn_handle, 
											uint8_t *data, uint16_t len);

uint16_t simple_ble_client_set_notify_charac_v2(uint16_t conn_handle, bool enable);

uint16_t simple_ble_client_set_indicate_charac_v3(uint16_t conn_handle, bool enable);


#ifdef __cplusplus
}
#endif

#endif  /* __RTK_SIMPLE_BLE_CLIENT_H__ */