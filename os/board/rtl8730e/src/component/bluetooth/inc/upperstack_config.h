/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _UPPERSTACK_CONFIG_H_
#define _UPPERSTACK_CONFIG_H_

#define F_BT_BREDR_SUPPORT                  0       /* support BT Legacy */

#define F_BT_LE_SUPPORT                     1       /* support BT Low Energy */

/*============================================================================*
 *               BLE Feature Flags
 *============================================================================*/
#define F_BT_LE_GAP_CENTRAL_SUPPORT              (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_GAP_SCAN_SUPPORT                 (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_GAP_SCAN_FILTER_SUPPORT          (F_BT_LE_GAP_SCAN_SUPPORT && 1)
#define F_BT_LE_GAP_PERIPHERAL_SUPPORT           (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_GATT_CLIENT_SUPPORT              (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_GATT_SERVER_SUPPORT              (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_SMP_OOB_SUPPORT                  (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_BOND_KEY_REQ_SUPPORT             (F_BT_LE_SUPPORT && 0)

//BT 4.0
#define F_BT_LE_READ_REMOTE_FEATS                (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_ATT_SIGNED_WRITE_SUPPORT         (F_BT_LE_SUPPORT && 0)
#define F_BT_LE_4_0_DTM_SUPPORT                  (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_READ_ADV_TX_POWRE_SUPPORT        (F_BT_LE_GAP_PERIPHERAL_SUPPORT && 1)
#define F_BT_LE_READ_CHANN_MAP                   (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT (F_BT_LE_SUPPORT && 1)

//BT 4.1
#define F_BT_LE_4_1_COC_SUPPORT                  (F_BT_LE_SUPPORT && 1)

//BT 4.2
#define F_BT_LE_4_2_SUPPORT                      (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_4_2_SC_SUPPORT                   (F_BT_LE_4_2_SUPPORT && 1)
#define F_BT_LE_4_2_SC_OOB_SUPPORT               (F_BT_LE_SMP_OOB_SUPPORT && F_BT_LE_4_2_SC_SUPPORT && 1)
#define F_BT_LE_4_2_KEY_PRESS_SUPPORT            (F_BT_LE_4_2_SUPPORT && 1)
#define F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT         (F_BT_LE_4_2_SUPPORT && 1)
#define F_BT_LE_PRIVACY_SUPPORT                  (F_BT_LE_4_2_SUPPORT && 1)

//BT 5
#define F_BT_LE_5_0_SUPPORT                      (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_5_0_AE_ADV_SUPPORT               (F_BT_LE_5_0_SUPPORT && 1)
#define F_BT_LE_5_0_AE_SCAN_SUPPORT              (F_BT_LE_5_0_SUPPORT && 1)
#define F_BT_LE_5_0_PA_SUPPORT                   (F_BT_LE_5_0_SUPPORT && 1)
#define F_BT_LE_5_0_PA_ADV_SUPPORT               (F_BT_LE_5_0_AE_ADV_SUPPORT && F_BT_LE_5_0_PA_SUPPORT && 1)
#define F_BT_LE_5_0_PA_SYNC_SUPPORT              (F_BT_LE_5_0_PA_SUPPORT && 1)
#define F_BT_LE_5_0_PA_SYNC_SCAN_SUPPORT         (F_BT_LE_5_0_AE_SCAN_SUPPORT && F_BT_LE_5_0_PA_SYNC_SUPPORT && 1)
#define F_BT_LE_5_0_DTM_SUPPORT                  (F_BT_LE_5_0_SUPPORT && F_BT_LE_4_0_DTM_SUPPORT && 1)
#define F_BT_LE_5_0_SET_PHYS_SUPPORT             (F_BT_LE_5_0_SUPPORT && 1)
#define F_BT_LE_5_0_READ_POWER_SUPPORT           (F_BT_LE_5_0_SUPPORT && 1)
#define F_BT_LE_5_0_RF_PATH_SUPPORT              ((F_BT_LE_5_0_AE_ADV_SUPPORT || F_BT_LE_5_2_POWER_CONTROL_SUPPORT) && 1)

//BT 5.1
#define F_BT_LE_5_1_SUPPORT                               (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_5_1_PAST_SUPPORT                          ((F_BT_LE_5_0_PA_ADV_SUPPORT || F_BT_LE_5_0_PA_SYNC_SUPPORT) && F_BT_LE_5_1_SUPPORT && 1)
#define F_BT_LE_5_1_PAST_SENDER_SUPPORT                   (F_BT_LE_5_1_PAST_SUPPORT && 1)
#define F_BT_LE_5_1_PAST_SENDER_ADV_SUPPORT               (F_BT_LE_5_0_PA_ADV_SUPPORT && F_BT_LE_5_1_PAST_SENDER_SUPPORT && 1)
#define F_BT_LE_5_1_PAST_SENDER_SYNC_SUPPORT              (F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_1_PAST_SENDER_SUPPORT && 1)
#define F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT                (F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_1_PAST_SUPPORT && 1)
#define F_BT_LE_5_1_AOA_AOD_SUPPORT                       (F_BT_LE_5_1_SUPPORT && 1)
#define F_BT_LE_5_1_AOX_CONN_SUPPORT                      (F_BT_LE_5_1_AOA_AOD_SUPPORT && 1)
#define F_BT_LE_5_1_AOX_CONNLESS_SUPPORT                  (F_BT_LE_5_1_AOA_AOD_SUPPORT && 1)
#define F_BT_LE_5_1_AOX_CONNLESS_TRANSMITTER_SUPPORT      (F_BT_LE_5_0_PA_ADV_SUPPORT && F_BT_LE_5_1_AOX_CONNLESS_SUPPORT && 1)
#define F_BT_LE_5_1_AOX_CONNLESS_RECEIVER_SUPPORT         (F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_1_AOX_CONNLESS_SUPPORT && 1)
#define F_BT_LE_5_1_DTM_SUPPORT                           (F_BT_LE_5_1_SUPPORT && F_BT_LE_4_0_DTM_SUPPORT && 1)

//BT 5.2
#define F_BT_LE_5_2_SUPPORT                               (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_5_2_ISOC_SUPPORT                          (F_BT_LE_5_2_SUPPORT && 1)
#define F_BT_LE_5_2_ISOC_CIS_SUPPORT                      (F_BT_LE_5_2_ISOC_SUPPORT && 1)
#define F_BT_LE_5_2_ISOC_CIS_CENTRAL_SUPPORT              (F_BT_LE_GAP_CENTRAL_SUPPORT && F_BT_LE_5_2_ISOC_CIS_SUPPORT && 1)
#define F_BT_LE_5_2_ISOC_CIS_PERIPHERAL_SUPPORT           (F_BT_LE_GAP_PERIPHERAL_SUPPORT && F_BT_LE_5_2_ISOC_CIS_SUPPORT && 1)
#define F_BT_LE_5_2_ISOC_BIS_SUPPORT                      (F_BT_LE_5_2_ISOC_SUPPORT && 1)
#define F_BT_LE_5_2_ISOC_BIS_BROADCASTER_SUPPORT          (F_BT_LE_5_0_PA_ADV_SUPPORT && F_BT_LE_5_2_ISOC_BIS_SUPPORT && 1)
#define F_BT_LE_5_2_ISOC_BIS_RECEIVER_SUPPORT             (F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_2_ISOC_BIS_SUPPORT && 1)
#define F_BT_CONNECTION_ACCEPT_TIMEOUT_SUPPORT            ((F_BT_BREDR_SUPPORT || F_BT_LE_5_2_ISOC_CIS_PERIPHERAL_SUPPORT) && 1)
#define F_BT_5_2_READ_LOCAL_SUPPORTED_CODECS_V2_SUPPORT   ((F_BT_LE_5_2_SUPPORT) && 1)

#define F_BT_LE_5_2_POWER_CONTROL_SUPPORT                 (F_BT_LE_5_2_SUPPORT && 1)
#define F_BT_LE_5_2_PATH_LOSS_MONITORING_SUPPORT          (F_BT_LE_5_2_SUPPORT && 1)
#define F_BT_LE_5_2_DTM_SUPPORT                           (F_BT_LE_5_2_SUPPORT && 1)

#define F_BT_5_2_EATT_SUPPORT                          (1)
#define F_BT_5_2_L2C_ECFC_SUPPORT                      (F_BT_5_2_EATT_SUPPORT && 1)
#define F_BT_GATT_SERVER_EXT_API                       (F_BT_LE_GATT_SERVER_SUPPORT && 1)
#define F_BT_GATT_CLIENT_EXT_API                       (F_BT_LE_GATT_CLIENT_SUPPORT && 1)
#define F_BT_ATT_MULTIPLE_VALUE_NTF                    (F_BT_5_2_EATT_SUPPORT && 1)
#define F_BT_ATT_READ_MULTIPLE_VARIABLE                (F_BT_5_2_EATT_SUPPORT && 1)
#define F_BT_EATT_OVER_BREDR                           (F_BT_BREDR_SUPPORT && F_BT_5_2_L2C_ECFC_SUPPORT && 0)
#define F_BT_ATT_OVER_BREDR                            (F_BT_BREDR_SUPPORT && 0)

/*============================================================================*
 *               Function Configuration Flags
 *============================================================================*/
#define F_BT_APP_BOND_MANAGER                    (F_BT_LE_SUPPORT && 0)
#define F_BT_LE_FIX_CHANN_SUPPORT                (F_BT_LE_SUPPORT && 0)
#define F_BT_LE_GATT_CCCD_DATA_PENDING           (F_BT_LE_GATT_SERVER_SUPPORT && 1)
#define F_BT_LE_LOCAL_IRK_SETTING_SUPPORT        (F_BT_LE_SUPPORT && 1)

#define F_BT_LE_GAP_MSG_INFO_WAY            1

/*============================================================================*
 *               Dualmode Configuration Flags
 *============================================================================*/
#define F_BT_KEY_PRESS_SUPPORT              (F_BT_LE_4_2_KEY_PRESS_SUPPORT || F_BT_BREDR_SUPPORT)
#define F_BT_OOB_SUPPORT                         (F_BT_LE_SMP_OOB_SUPPORT || F_BT_LE_4_2_SC_OOB_SUPPORT || F_BT_BREDR_SUPPORT)
#define F_BT_SC_LINK_CONVERT_SUPPORT             (F_BT_BREDR_SUPPORT&&F_BT_LE_4_2_SC_SUPPORT&&1)

/*============================================================================*
 *               Function Configuration Flags For BT LIb
 *============================================================================*/
#define F_BT_DEINIT                              1

#endif /* _UPPERSTACK_CONFIG_H_ */
