/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __DIS_SERVICE_H__
#define __DIS_SERVICE_H__

#ifdef __cplusplus
extern "C"  {
#endif

#include <rtk_bt_def.h>

typedef enum {
	DIS_PARAM_MANUFACTURER_NAME,
	DIS_PARAM_MODEL_NUMBER,
	DIS_PARAM_SERIAL_NUMBER,
	DIS_PARAM_HARDWARE_REVISION,
	DIS_PARAM_FIRMWARE_REVISION,
	DIS_PARAM_SOFTWARE_REVISION,
	DIS_PARAM_SYSTEM_ID,
	DIS_PARAM_IEEE_DATA_LIST,
	DIS_PARAM_PNP_ID
} dis_param_type_t;

uint16_t dis_set_params(dis_param_type_t param_type, uint8_t len, void *p_value);

void device_info_srv_callback(uint8_t event, void *data);

uint16_t device_info_srv_add(void);

#ifdef __cplusplus
}
#endif

#endif  /* __DIS_SERVICE_H__ */