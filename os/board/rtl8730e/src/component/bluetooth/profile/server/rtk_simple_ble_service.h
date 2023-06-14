/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __SIMPLE_BLE_SERVICE_H__
#define __SIMPLE_BLE_SERVICE_H__

#ifdef __cplusplus
extern "C"  {
#endif

#include <rtk_bt_def.h>

void simple_ble_srv_cccd_indicate(uint16_t conn_handle);

void simple_ble_srv_cccd_notify(uint16_t conn_handle);

uint16_t simple_ble_srv_add(void);

void simple_ble_service_callback(uint8_t event, void *data);

void simple_ble_srv_disconnect(uint16_t conn_handle);

void simple_ble_srv_status_deinit(void);

#ifdef __cplusplus
}
#endif

#endif  /* __SIMPLE_BLE_SERVICE_H__ */
