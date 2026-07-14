/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __RTK_SERVICE_CONFIG_H__
#define __RTK_SERVICE_CONFIG_H__

#ifdef __cplusplus
extern "C"  {
#endif

#ifdef CONFIG_PLATFORM_TIZENRT_OS
#define TIZENRT_SRV_ID                   0
#else
#define SIMPLE_BLE_SRV_ID                   0
#define DEVICE_INFO_SRV_ID                  1
#define HEART_RATE_SRV_ID                   2
#define BATTERY_SRV_ID                      3
#define IMMEDIATE_ALERT_SRV_ID              4
#define GLUCOSE_SRV_ID                      5
#define HID_SRV_ID                          6
#define LONG_UUID_SRV_ID                    7
#define CTE_SRV_ID                          8
#define PTS_VENDOR_SRV_ID                   9
#define THROUGHPUT_SRV_ID                   10
#define BLE_WIFIMATE_SRV_ID                 11
#define TRANSFER_MODULE_SRV_ID              12
#define BLE_OTA_SRV_ID                      13
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

#define TRANSFER_MODULE_SRV_ID_BASE         0x80

#ifdef __cplusplus
}
#endif

#endif  /* __RTK_SERVICE_CONFIG_H__ */