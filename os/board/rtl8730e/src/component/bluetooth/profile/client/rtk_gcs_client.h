/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __RTK_GCS_CLIENT_H__
#define __RTK_GCS_CLIENT_H__

#ifdef __cplusplus
extern "C"  {
#endif

#include <rtk_bt_common.h>

void gattc_dump(uint16_t len, uint8_t* data, uint8_t* tag);

rtk_bt_evt_cb_ret_t general_client_app_callback(uint8_t event, void *data);

uint16_t general_client_attach_conn(uint16_t conn_handle);

uint16_t general_client_detach_conn(uint16_t conn_handle);

uint16_t general_client_add(void);

uint16_t general_client_delete(void);

void general_client_discover_res_hdl(void *data);

void general_client_read_res_hdl(void *data);

void general_client_write_res_hdl(void *data);

void general_client_notify_hdl(void *data);

void general_client_indicate_hdl(void *data);

void general_client_cccd_enable_hdl(void *data);

void general_client_cccd_disable_hdl(void *data);

#ifdef __cplusplus
}
#endif

#endif  /* __RTK_GCS_CLIENT_H__ */