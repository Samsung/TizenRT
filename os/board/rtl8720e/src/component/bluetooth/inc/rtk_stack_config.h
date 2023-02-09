/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __RTK_STACK_CONFIG_H__
#define __RTK_STACK_CONFIG_H__

#include "platform_opts_bt.h"
#if UPPER_STACK_VERSION == VERSION_2019
#include <bt_flags.h>
#elif UPPER_STACK_VERSION == VERSION_2021
#include "upperstack_config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_AMEBAD2)      \
    || defined(CONFIG_PLATFORM_8735B) || defined(CONFIG_PLATFORM_AMEBALITE)
#define GAP_MAX_LINKS                       4
#define GAP_MAX_EXT_ADV_SETS                6
#define LE_AUDIO_MAX_EXT_ADV_SETS           1
#define GAP_MAX_PA_ADV_SETS                 3
#define GAP_MAX_PA_SYNC_HANDLES             3
#define GAP_MAX_ECFC_PROTOCAL_NUM           6
#define GAP_LE_MAX_ECFC_CHANN_NUM           20
#elif defined(CONFIG_PLATFORM_8710C)
#define GAP_MAX_LINKS  2
#else
#define GAP_MAX_LINKS  2
#endif

/**
 * @brief Default TX/RX preferred parameters 
 */
#if defined(CONFIG_PLATFORM_8710C)
#define GAP_PHYS_PREFER_TX (GAP_PHYS_PREFER_1M_BIT)
#define GAP_PHYS_PREFER_RX (GAP_PHYS_PREFER_1M_BIT)
#elif defined(CONFIG_PLATFORM_8721D)
#define GAP_PHYS_PREFER_TX (GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT)
#define GAP_PHYS_PREFER_RX (GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT)
#elif defined(CONFIG_PLATFORM_8735B) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_AMEBALITE)
#define GAP_PHYS_PREFER_TX (GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT | GAP_PHYS_PREFER_CODED_BIT)
#define GAP_PHYS_PREFER_RX (GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT | GAP_PHYS_PREFER_CODED_BIT)
#endif

#define API_TASK_STACK_SIZE         256*8
#define API_TASK_PRIORITY           4
#define API_TASK_IO_MSG_QUEUE_SIZE  0x20
#define API_TASK_GAP_MSG_QUEUE_SIZE 0x20
#define API_TASK_EVT_MSG_QUEUE      (API_TASK_IO_MSG_QUEUE_SIZE + API_TASK_GAP_MSG_QUEUE_SIZE) 

#define RTK_BT_GATT_DEFAULT_CREDITS 10

#define RTK_BT_GATTC_APP_NUM        16
#define RTK_BT_GATTC_SUBS_NUM       10

#define RTK_BT_GATTS_SERVICE_NUM    12
#define RTK_BT_GATTS_CCCD_NUM       10

#ifdef __cplusplus
}
#endif

#endif /* __RTK_STACK_CONFIG_H__ */