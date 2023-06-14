/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __BAS_H__
#define __BAS_H__


#ifdef __cplusplus
extern "C"  {
#endif

#include <rtk_bt_def.h>

typedef enum {
	BAS_PARAM_BATTERY_LEVEL = 0x01,
} T_BAS_PARAM_TYPE;

void battery_service_callback(uint8_t event, void *data);

void battery_cccd_notify(uint16_t conn_handle);

uint16_t battery_srv_add(void);

void bas_disconnect(uint16_t conn_handle);

void bas_status_deinit(void);

#ifdef __cplusplus
}
#endif

#endif  /* __BAS_H__ */