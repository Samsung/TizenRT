/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __RTK_STACK_CONFIG_H__
#define __RTK_STACK_CONFIG_H__

#include "platform_autoconf.h"
#include "upperstack_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GAP_MAX_EXT_ADV_SETS                6
#define GAP_MAX_PA_ADV_SETS                 3
#define GAP_MAX_PA_SYNC_HANDLES             3
#define GAP_MAX_ECFC_PROTOCAL_NUM           6
#define GAP_LE_MAX_ECFC_CHANN_NUM           10
#define GAP_MAX_CCC_BITS_CNT                32
#define GAP_MAX_LE_COC_CHANN_NUM            10
#define GAP_MAX_LE_L2CAP_CHANN_NUM          20  /* L2CAP dynamic channel total number, both ecfc connection and le coc connection will occupy
                                                   channel number, so when user need to use both ecfc and le coc, shall set
                                                   GAP_MAX_LE_L2CAP_CHANN_NUM >= (GAP_LE_MAX_ECFC_CHANN_NUM + GAP_MAX_LE_COC_CHANN_NUM) */
#define GAP_MAX_LE_SEC_ENTRY_NUM            12  /* LE security entry number, it will limit the number of ecfc protocal and le coc security */

/**
 * @brief Default TX/RX preferred parameters
 */
#define GAP_PHYS_PREFER_TX (GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT | GAP_PHYS_PREFER_CODED_BIT)
#define GAP_PHYS_PREFER_RX (GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT | GAP_PHYS_PREFER_CODED_BIT)

#define API_TASK_STACK_SIZE         256*12
#define API_TASK_PRIORITY           4
#define API_TASK_IO_MSG_QUEUE_SIZE  0x20
#define API_TASK_GAP_MSG_QUEUE_SIZE 0x20
#define API_TASK_EVT_MSG_QUEUE      (API_TASK_IO_MSG_QUEUE_SIZE + API_TASK_GAP_MSG_QUEUE_SIZE)

#define RTK_BT_GATT_DEFAULT_CREDITS 10

#define RTK_BT_GATTC_APP_NUM        16
#define RTK_BT_GATTC_SUBS_NUM       10

#define RTK_BT_GATTS_SERVICE_NUM    24
#define RTK_BT_GATTS_CCCD_NUM       10

#define RTK_BT_STACK_LOG_CONFIG     0

#ifdef __cplusplus
}
#endif

#endif /* __RTK_STACK_CONFIG_H__ */