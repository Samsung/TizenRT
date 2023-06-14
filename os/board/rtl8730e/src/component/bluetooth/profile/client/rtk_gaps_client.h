/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __RTK_GAPS_CLIENT_H__
#define __RTK_GAPS_CLIENT_H__

#ifdef __cplusplus
extern "C"  {
#endif

#include <rtk_bt_common.h>

#define GAPS_CLIENT_SHOW_DETAIL         1

typedef enum {
	GAPS_CHAR_DEVICE_NAME = 0,
	GAPS_CHAR_APPEARANCE = 1,
	GAPS_CHAR_PPCP = 2,
	GAPS_CHAR_CEN_ADDR_RES = 3,
	GAPS_CHAR_NUM,
} gaps_charac_index_t;

uint16_t gaps_client_attach_conn(uint16_t conn_handle);

uint16_t gaps_client_detach_conn(uint16_t conn_handle);

uint16_t gaps_client_add(void);

uint16_t gaps_client_delete(void);

rtk_bt_evt_cb_ret_t gaps_client_app_callback(uint8_t event, void *data);

#if !RTK_BLE_MGR_LIB
uint16_t gaps_client_srv_discover(uint16_t conn_handle);
#endif

uint16_t gaps_client_char_read(uint16_t conn_handle, gaps_charac_index_t char_index);

#ifdef __cplusplus
}
#endif

#endif  /* __RTK_GAPS_CLIENT_H__ */