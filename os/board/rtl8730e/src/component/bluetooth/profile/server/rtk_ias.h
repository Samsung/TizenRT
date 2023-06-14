/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __IAS_SERVICE_H__
#define __IAS_SERVICE_H__

#ifdef __cplusplus
extern "C"  {
#endif

#include <rtk_bt_def.h>

void immediate_alert_srv_callback(uint8_t event, void *data);

uint16_t immediate_alert_srv_add(void);

#ifdef __cplusplus
}
#endif

#endif  /* __IAS_SERVICE_H__ */