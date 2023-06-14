/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __HRS_SERVICE_H__
#define __HRS_SERVICE_H__

#ifdef __cplusplus
extern "C"  {
#endif

#include <rtk_bt_def.h>

#define HRS_HEART_RATE_MEASUREMENT_VALUE_MAX_LEN    30
#define HRS_HEART_RATE_MEASUREMENT_RR_INTERVAL_NUM  7

typedef struct {
    uint8_t heart_rate_value_format_bit: 1;
    uint8_t sensor_contact_status_bits: 2;
    uint8_t energy_expended_status_bit: 1;
    uint8_t rr_interval_bit: 1;
    uint8_t rfu: 3;
} heart_rate_measurement_val_flag_t;  /* flag bit_field, 1 Byte */

typedef struct {
    heart_rate_measurement_val_flag_t flags;
    uint16_t measurement_val;
    uint16_t energy_expended;
    uint16_t rr_interval[HRS_HEART_RATE_MEASUREMENT_RR_INTERVAL_NUM];
} heart_rate_measurement_val_t;

typedef enum {
    HRS_HEART_RATE_MEASUREMENT_PARAM_FLAG = 0x01,
    HRS_HEART_RATE_MEASUREMENT_PARAM_MEASUREMENT_VALUE,
    HRS_HEART_RATE_MEASUREMENT_PARAM_ENERGY_EXPENDED,
    HRS_HEART_RATE_MEASUREMENT_PARAM_RR_INTERVAL,
    HRS_BODY_SENSOR_LOCATION_PARAM_VALUE,
} hrs_param_type_t;

void heart_rate_srv_callback(uint8_t event, void *data);

void heart_rate_measurement_cccd_notify(uint16_t conn_handle);

uint16_t heart_rate_srv_add(void);

void hrs_disconnect(uint16_t conn_handle);

void hrs_status_deinit(void);

#ifdef __cplusplus
}
#endif

#endif  /* __HRS_SERVICE_H__ */