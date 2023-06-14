/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __RTK_BAS_CLIENT_H__
#define __RTK_BAS_CLIENT_H__

#ifdef __cplusplus
extern "C"  {
#endif

#include <rtk_bt_common.h>

#define BAS_CLIENT_SHOW_DETAIL    1

rtk_bt_evt_cb_ret_t bas_client_app_callback(uint8_t event, void *data);

uint16_t bas_client_attach_conn(uint16_t conn_handle);

uint16_t bas_client_detach_conn(uint16_t conn_handle);

uint16_t bas_client_add(void);

uint16_t bas_client_delete(void);

#if !RTK_BLE_MGR_LIB
uint16_t bas_client_srv_discover(uint16_t conn_handle);
#endif

uint16_t bas_client_read_battery_level(uint16_t conn_handle);

uint16_t bas_client_set_notify(uint16_t conn_handle, bool enable);

uint16_t bas_client_read_notify(uint16_t conn_handle);

#ifdef __cplusplus
}
#endif

#endif  /* __RTK_BAS_CLIENT_H__ */