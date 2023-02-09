/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_callback_le.h
  * @brief   This file contains function prototype for all GAP roles.
  * @details
  * @author  ranhui
  * @date    2016-02-18
  * @version v0.1
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_CALLBACK_LE_H
#define GAP_CALLBACK_LE_H

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2019

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <bt_flags.h>
#include <gap_storage_le.h>
#include <gap_le_types.h>

/** @defgroup GAP_CB_MSG_MODULE GAP Callback Message
  * @brief GAP Callback Message
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup Gap_CB_Msg_Exported_Macros GAP Callback Msg Exported Macros
  * @{
  */

/** @defgroup GAP_LE_MSG_Types GAP LE Msg Types
  * @{
  */

//gap_le.h
#define GAP_MSG_LE_MODIFY_WHITE_LIST                0x01 //!<Response msg type for le_modify_white_list
#define GAP_MSG_LE_SET_RAND_ADDR                    0x02 //!<Response msg type for le_set_rand_addr
#if F_BT_LE_GAP_CENTRAL_SUPPORT
#define GAP_MSG_LE_SET_HOST_CHANN_CLASSIF           0x03 //!<Response msg type for le_set_host_chann_classif
#endif
#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
#define GAP_MSG_LE_WRITE_DEFAULT_DATA_LEN           0x04 //!<Response msg type for le_write_default_data_len
#endif

//gap_conn_le.h
#define GAP_MSG_LE_READ_RSSI                        0x10 //!<Response msg type for le_read_rssi
#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
#define GAP_MSG_LE_SET_DATA_LEN                     0x13 //!<Response msg type for le_set_data_len
#define GAP_MSG_LE_DATA_LEN_CHANGE_INFO             0x14 //!<Notification msg type for data length changed
#endif
#if F_BT_LE_GAP_CENTRAL_SUPPORT
#define GAP_MSG_LE_CONN_UPDATE_IND                  0x15 //!<Indication for le connection parameter update
#endif
#define GAP_MSG_LE_CREATE_CONN_IND                  0x16 //!<Indication for create le connection
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
#define GAP_MSG_LE_PHY_UPDATE_INFO                  0x17 //!<Indication for le phyical update information
#endif
#if F_BT_LE_READ_REMOTE_FEATS
#define GAP_MSG_LE_REMOTE_FEATS_INFO                0x19 //!<Information for remote device supported features
#endif

//gap_bond_le.h
#define GAP_MSG_LE_BOND_MODIFY_INFO                 0x20 //!<Notification msg type for bond modify
#if F_BT_LE_ATT_SIGNED_WRITE_SUPPORT
#define GAP_MSG_LE_GATT_SIGNED_STATUS_INFO          0x23 //!<Notification msg type for le signed status information
#endif

//gap_scan.h
#if F_BT_LE_GAP_SCAN_SUPPORT
#define GAP_MSG_LE_SCAN_INFO                        0x30 //!<Notification msg type for le scan
#if F_BT_LE_PRIVACY_SUPPORT
#define GAP_MSG_LE_DIRECT_ADV_INFO                  0x31 //!<Notification msg type for le direct adv info
#endif
#endif

//gap_adv.h
#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
#define GAP_MSG_LE_ADV_UPDATE_PARAM                 0x40 //!<Response msg type for le_adv_update_param
#endif

//gap_dtm.h
#if F_BT_LE_4_0_DTM_SUPPORT
#define GAP_MSG_LE_DTM_RECEIVER_TEST                0x70 //!<Response msg type for le_dtm_receiver_test
#define GAP_MSG_LE_DTM_TRANSMITTER_TEST             0x71 //!<Response msg type for le_dtm_transmitter_test
#define GAP_MSG_LE_DTM_TEST_END                     0x72 //!<Response msg type for le_dtm_test_end
#endif

#define GAP_MSG_LE_GAP_STATE_MSG                    0xB0

/** End of GAP_LE_MSG_Types
  * @}
  */

/** End of Gap_CB_Msg_Exported_Macros
* @}
*/


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup Gap_CB_Msg_Exported_Types GAP Callback Msg Exported Types
  * @{
  */
typedef struct {
	uint16_t            cause;
} T_LE_CAUSE;

/** @brief  Response of le modify white list request.*/
typedef struct {
	T_GAP_WHITE_LIST_OP operation;
	uint16_t            cause;
} T_LE_MODIFY_WHITE_LIST_RSP;

/** @brief  Response of le set random address request. */
typedef struct {
	uint16_t        cause;
} T_LE_SET_RAND_ADDR_RSP;

#if F_BT_LE_GAP_CENTRAL_SUPPORT
/** @brief  Response of le set channel classification request. */
typedef struct {
	uint16_t        cause;
} T_LE_SET_HOST_CHANN_CLASSIF_RSP;
#endif

/** @brief  Response for read rssi.*/
typedef struct {
	uint8_t         conn_id;
	int8_t          rssi;
	uint16_t        cause;
} T_LE_READ_RSSI_RSP;

#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
/** @brief  Response for set data length, which is used for BT4.2 data length extension.*/
typedef struct {
	uint8_t         conn_id;
	uint16_t        cause;
} T_LE_SET_DATA_LEN_RSP;

/** @brief  Notification for data length change info, which is used for BT4.2 data length extension.*/
typedef struct {
	uint8_t         conn_id;
	uint16_t        max_tx_octets;
	uint16_t        max_tx_time;
	uint16_t        max_rx_octets;
	uint16_t        max_rx_time;
} T_LE_DATA_LEN_CHANGE_INFO;
#endif

#if F_BT_LE_GAP_CENTRAL_SUPPORT
/** @brief  Indication for connection paramete update.*/
typedef struct {
	uint8_t         conn_id;
	uint16_t        conn_interval_min;
	uint16_t        conn_interval_max;
	uint16_t        conn_latency;
	uint16_t        supervision_timeout;
} T_LE_CONN_UPDATE_IND;
#endif

/** @brief  Indication of le connectiona. */
typedef struct {
	uint8_t                bd_addr[6];/**< Bluetooth address of remote device */
	T_GAP_REMOTE_ADDR_TYPE remote_addr_type; /**< Address type of remote device */
}  T_LE_CREATE_CONN_IND;

#if F_BT_LE_5_0_SET_PHYS_SUPPORT
/** @brief  Notification information when phy changed.*/
typedef struct {
	uint8_t         conn_id;
	uint16_t        cause;
	T_GAP_PHYS_TYPE tx_phy;
	T_GAP_PHYS_TYPE rx_phy;
} T_LE_PHY_UPDATE_INFO;
#endif

#if F_BT_LE_READ_REMOTE_FEATS
/** @brief  Information for remote device features.*/
typedef struct {
	uint8_t         conn_id;
	uint16_t        cause;
	uint8_t         remote_feats[8];
} T_LE_REMOTE_FEATS_INFO;
#endif

//gap_bond_le.h
/** @brief  Bond information modify type*/
typedef enum {
	LE_BOND_DELETE,
	LE_BOND_ADD,
	LE_BOND_CLEAR,
	LE_BOND_FULL,
	LE_BOND_KEY_MISSING,
} T_LE_BOND_MODIFY_TYPE;

/** @brief  Structure for modify bonding information.*/
typedef struct {
	T_LE_BOND_MODIFY_TYPE type;
	T_LE_KEY_ENTRY        *p_entry;
} T_LE_BOND_MODIFY_INFO;

#if F_BT_LE_ATT_SIGNED_WRITE_SUPPORT
/** @brief  Structure for LE signed information.*/
typedef struct {
	uint8_t         conn_id;
	uint16_t        cause;
	bool            update_local;
	uint32_t        local_sign_count;
	uint32_t        remote_sign_count;
} T_LE_GATT_SIGNED_STATUS_INFO;
#endif

#if F_BT_LE_4_0_DTM_SUPPORT
/** @brief  Response of le receiver test request. */
typedef struct {
	uint16_t        cause;
	uint16_t        num_pkts;
} T_LE_DTM_TEST_END_RSP;
#endif

//gap_scan.h
#if F_BT_LE_GAP_SCAN_SUPPORT
/** @brief  Information of le scan information.  */
typedef struct {
	uint8_t                bd_addr[6];/**< Bluetooth address of remote device. */
	T_GAP_REMOTE_ADDR_TYPE remote_addr_type;/**< Address type of remote device. */
	T_GAP_ADV_EVT_TYPE     adv_type;/**< Advertising event type. */
	int8_t                 rssi;   /**< RSSI. */
	uint8_t                data_len;
	uint8_t                data[31];
} T_LE_SCAN_INFO;

#if F_BT_LE_PRIVACY_SUPPORT
/** @brief  Information of le direct advertising.   */
typedef struct {
	uint8_t                bd_addr[6];
	T_GAP_REMOTE_ADDR_TYPE remote_addr_type;
	uint8_t                direct_bd_addr[6];
	T_GAP_DIRECT_ADDR_TYPE direct_addr_type;
	T_GAP_ADV_EVT_TYPE     direct_adv_type;
	int8_t                 rssi;
} T_LE_DIRECT_ADV_INFO;
#endif
#endif

//gap_adv.h
#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
/** @brief  LE advertising paramter update result.*/
typedef struct {
	uint16_t        cause;
} T_LE_ADV_UPDATE_PARAM_RSP;
#endif

/** @brief  GAP LE Callback Data*/
typedef union {
	T_LE_CAUSE                                le_cause;

//gap_le.h
	T_LE_MODIFY_WHITE_LIST_RSP               *p_le_modify_white_list_rsp;
	T_LE_SET_RAND_ADDR_RSP                   *p_le_set_rand_addr_rsp;
#if F_BT_LE_GAP_CENTRAL_SUPPORT
	T_LE_SET_HOST_CHANN_CLASSIF_RSP          *p_le_set_host_chann_classif_rsp;
#endif
//gap_conn_le.h
	T_LE_READ_RSSI_RSP                       *p_le_read_rssi_rsp;
#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
	T_LE_SET_DATA_LEN_RSP                    *p_le_set_data_len_rsp;
	T_LE_DATA_LEN_CHANGE_INFO                *p_le_data_len_change_info;
#endif
#if F_BT_LE_GAP_CENTRAL_SUPPORT
	T_LE_CONN_UPDATE_IND                     *p_le_conn_update_ind;
#endif
	T_LE_CREATE_CONN_IND                     *p_le_create_conn_ind;
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
	T_LE_PHY_UPDATE_INFO                     *p_le_phy_update_info;
#endif
#if F_BT_LE_READ_REMOTE_FEATS
	T_LE_REMOTE_FEATS_INFO                   *p_le_remote_feats_info;
#endif
//gap_bond_le.h
	T_LE_BOND_MODIFY_INFO                    *p_le_bond_modify_info;
#if F_BT_LE_ATT_SIGNED_WRITE_SUPPORT
	T_LE_GATT_SIGNED_STATUS_INFO             *p_le_gatt_signed_status_info;
#endif
//gap_scan.h
#if F_BT_LE_GAP_SCAN_SUPPORT
	T_LE_SCAN_INFO                           *p_le_scan_info;
#if F_BT_LE_PRIVACY_SUPPORT
	T_LE_DIRECT_ADV_INFO                     *p_le_direct_adv_info;
#endif
#endif
//gap_adv.h
#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
	T_LE_ADV_UPDATE_PARAM_RSP                *p_le_adv_update_param_rsp;
#endif
//gap_dtm.h
#if F_BT_LE_4_0_DTM_SUPPORT
	T_LE_DTM_TEST_END_RSP                    *p_le_dtm_test_end_rsp;
#endif
	void                                     *p_gap_state_msg;
} T_LE_CB_DATA;

/** End of Gap_CB_Msg_Exported_Types
* @}
*/

/** End of GAP_CB_MSG_MODULE
* @}
*/

#ifdef __cplusplus
}
#endif

#elif UPPER_STACK_VERSION == VERSION_2021

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "upperstack_config.h"
#include "gap_storage_le.h"
#include "gap_le_types.h"

/** @defgroup GAP_CB_MSG_MODULE GAP Callback Message
  * @brief GAP Callback Message
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup Gap_CB_Msg_Exported_Macros GAP Callback Msg Exported Macros
  * @{
  */

/** @defgroup GAP_LE_MSG_Types GAP LE Msg Types
  * @{
  */

//gap_le.h
#define GAP_MSG_LE_MODIFY_WHITE_LIST                0x01 //!<Response msg type for le_modify_white_list
#define GAP_MSG_LE_SET_RAND_ADDR                    0x02 //!<Response msg type for le_set_rand_addr
#define GAP_MSG_LE_SET_HOST_CHANN_CLASSIF           0x03 //!<Response msg type for le_set_host_chann_classif
#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
#define GAP_MSG_LE_WRITE_DEFAULT_DATA_LEN           0x04 //!<Response msg type for le_write_default_data_len
#endif

//gap_conn_le.h
#define GAP_MSG_LE_READ_RSSI                        0x10 //!<Response msg type for le_read_rssi
#if F_BT_LE_READ_CHANN_MAP
#define GAP_MSG_LE_READ_CHANN_MAP                   0x11 //!<Response msg type for le_read_chann_map
#endif
#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
#define GAP_MSG_LE_SET_DATA_LEN                     0x13 //!<Response msg type for le_set_data_len
#define GAP_MSG_LE_DATA_LEN_CHANGE_INFO             0x14 //!<Notification msg type for data length changed
#endif
#if F_BT_LE_GAP_CENTRAL_SUPPORT
#define GAP_MSG_LE_CONN_UPDATE_IND                  0x15 //!<Indication for le connection parameter update
#endif
#define GAP_MSG_LE_CREATE_CONN_IND                  0x16 //!<Indication for create le connection
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
#define GAP_MSG_LE_PHY_UPDATE_INFO                  0x17 //!<Indication for le phyical update information
#endif
#if F_BT_LE_READ_REMOTE_FEATS
#define GAP_MSG_LE_REMOTE_FEATS_INFO                0x19 //!<Information for remote device supported features
#endif
#if F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT
#define GAP_MSG_LE_READ_REMOTE_VERSION              0x1B //!<Response msg type for le_read_remote_version
#endif
#if F_BT_LE_5_0_AE_ADV_SUPPORT
#define GAP_MSG_LE_ADV_SET_CONN_OWN_ADDR_TYPE_INFO  0x1C //!<Information of own address type for advertiser using adv set
#endif

//gap_bond_le.h
#define GAP_MSG_LE_BOND_MODIFY_INFO                 0x20 //!<Notification msg type for bond modify
#if F_BT_LE_4_2_KEY_PRESS_SUPPORT
#define GAP_MSG_LE_KEYPRESS_NOTIFY                  0x21 //!<Response msg type for le_bond_keypress_notify
#define GAP_MSG_LE_KEYPRESS_NOTIFY_INFO             0x22 //!<Notification msg type for le_bond_keypress_notify
#endif
#if F_BT_LE_ATT_SIGNED_WRITE_SUPPORT
#define GAP_MSG_LE_GATT_SIGNED_STATUS_INFO          0x23 //!<Notification msg type for le signed status information
#endif
#if F_BT_LE_BOND_KEY_REQ_SUPPORT
/* The type of callback will only be used when gap_param_key_manager equals 2 and no matching key entry is found*/
#define GAP_MSG_LE_BOND_KEY_REQ                     0x24 //!<Notification msg type for le bond key request information
#endif

//gap_scan.h
#if F_BT_LE_GAP_SCAN_SUPPORT
#define GAP_MSG_LE_SCAN_INFO                        0x30 //!<Notification msg type for le scan
#if F_BT_LE_PRIVACY_SUPPORT
#define GAP_MSG_LE_DIRECT_ADV_INFO                  0x31 //!<Notification msg type for le direct adv info
#endif
#endif

//gap_adv.h
#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
#define GAP_MSG_LE_ADV_UPDATE_PARAM                 0x40 //!<Response msg type for le_adv_update_param
#if F_BT_LE_READ_ADV_TX_POWRE_SUPPORT
#define GAP_MSG_LE_ADV_READ_TX_POWER                0x41 //!<Response msg type for le_adv_read_tx_power
#endif
#endif
//gap_dtm.h
#if F_BT_LE_4_0_DTM_SUPPORT
#define GAP_MSG_LE_DTM_RECEIVER_TEST                0x70 //!<Response msg type for le_dtm_receiver_test
#define GAP_MSG_LE_DTM_TRANSMITTER_TEST             0x71 //!<Response msg type for le_dtm_transmitter_test
#define GAP_MSG_LE_DTM_TEST_END                     0x72 //!<Response msg type for le_dtm_test_end
#if F_BT_LE_5_0_DTM_SUPPORT
#define GAP_MSG_LE_DTM_ENHANCED_RECEIVER_TEST       0x73 //!<Response msg type for le_dtm_enhanced_receiver_test
#define GAP_MSG_LE_DTM_ENHANCED_TRANSMITTER_TEST    0x74 //!<Response msg type for le_dtm_enhanced_transmitter_test
#endif
#if F_BT_LE_5_1_DTM_SUPPORT
#define GAP_MSG_LE_DTM_RECEIVER_TEST_V3             0x75 //!<Response msg type for le_dtm_receiver_test_v3
#define GAP_MSG_LE_DTM_TRANSMITTER_TEST_V3          0x76 //!<Response msg type for le_dtm_transmitter_test_v3
#define GAP_MSG_LE_DTM_TRANSMITTER_TEST_V4          0x77 //!<Response msg type for le_dtm_transmitter_test_v4
#endif
#endif
#if F_BT_LE_5_0_AE_SCAN_SUPPORT
//gap_ext_scan.h
#define GAP_MSG_LE_EXT_ADV_REPORT_INFO                      0x50 //!<Notification msg type for le extended adv report
#if F_BT_LE_GAP_MSG_INFO_WAY
/* The type of callback will only be used after APP calls @ref le_ext_scan_gap_msg_info_way(false). */
#define GAP_MSG_LE_EXT_SCAN_STATE_CHANGE_INFO               0x51 //!<Notification msg type for extended scanning state
#endif
#endif
#if F_BT_LE_5_0_PA_SYNC_SUPPORT
//gap_pa_sync.h
#define GAP_MSG_LE_PA_SYNC_MODIFY_PERIODIC_ADV_LIST         0x54 //!<Response msg type for le_pa_sync_modify_periodic_adv_list
#define GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO                 0x55 //!<Notification msg type for LE periodic advertising report
#define GAP_MSG_LE_PA_SYNC_DEV_STATE_CHANGE_INFO            0x56 //!<Notification msg type for synchronization device state of periodic advertising trains
#define GAP_MSG_LE_PA_SYNC_STATE_CHANGE_INFO                0x57 //!<Notification msg type for synchronization state of periodic advertising trains
#endif
#if ((F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_1_SUPPORT) || F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
//gap_pa_sync.h
#define GAP_MSG_LE_PA_SYNC_SET_PERIODIC_ADV_RECEIVE_ENABLE  0x58 //!<Response msg type for le_pa_sync_set_periodic_adv_receive_enable
#endif
#if F_BT_LE_5_1_PAST_SENDER_SYNC_SUPPORT
//gap_past_sender.h
#define GAP_MSG_LE_PAST_SENDER_PERIODIC_ADV_SYNC_TRANSFER   0x59 //!<Response msg type for le_past_sender_periodic_adv_sync_transfer
#endif
#if F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT
//gap_past_recipient.h
#define GAP_MSG_LE_PAST_RECIPIENT_SET_DEFAULT_PERIODIC_ADV_SYNC_TRANSFER_PARAMS   0x5A //!<Response msg type for le_past_recipient_set_default_periodic_adv_sync_transfer_params
#define GAP_MSG_LE_PAST_RECIPIENT_SET_PERIODIC_ADV_SYNC_TRANSFER_PARAMS           0x5B //!<Response msg type for le_past_recipient_set_periodic_adv_sync_transfer_params
#define GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO        0x5C //!<Notification msg type for le_past_recipient_set_default_periodic_adv_sync_transfer_params or le_past_recipient_set_periodic_adv_sync_transfer_params
#endif
#if F_BT_LE_5_2_SUPPORT
//gap_pa_sync.h
#define GAP_MSG_LE_BIGINFO_ADV_REPORT_INFO          0x5D //!<Notification msg type for LE BIGInfo advertising report
#endif

#if F_BT_LE_5_0_AE_ADV_SUPPORT
//gap_ext_adv.h
#define GAP_MSG_LE_EXT_ADV_START_SETTING            0x60 //!<Response msg type for le_ext_adv_start_setting
#define GAP_MSG_LE_EXT_ADV_REMOVE_SET               0x61 //!<Response msg type for le_ext_adv_remove_set
#define GAP_MSG_LE_EXT_ADV_CLEAR_SET                0x62 //!<Response msg type for le_ext_adv_clear_set
#define GAP_MSG_LE_EXT_ADV_ENABLE                   0x63 //!<Response msg type for le_ext_adv_enable
#define GAP_MSG_LE_EXT_ADV_DISABLE                  0x64 //!<Response msg type for le_ext_adv_disable
#define GAP_MSG_LE_SCAN_REQ_RECEIVED_INFO           0x65 //!<Notification msg type for le scan received info
#if F_BT_LE_GAP_MSG_INFO_WAY
/* The type of callback will only be used after APP calls @ref le_ext_adv_gap_msg_info_way(false). */
#define GAP_MSG_LE_EXT_ADV_STATE_CHANGE_INFO        0x66 //!<Notification msg type for extended advertising state
#endif
#endif
#if F_BT_LE_5_0_PA_ADV_SUPPORT
//gap_pa_adv.h
#define GAP_MSG_LE_PA_ADV_START_SETTING             0x6A //!<Response msg type for le_pa_adv_start_setting
#define GAP_MSG_LE_PA_ADV_SET_PERIODIC_ADV_ENABLE   0x6B //!<Response msg type for le_pa_adv_set_periodic_adv_enable
#define GAP_MSG_LE_PA_ADV_STATE_CHANGE_INFO         0x6C //!<Notification msg type for PA periodic advertising state
#endif
#if F_BT_LE_5_1_PAST_SENDER_ADV_SUPPORT
//gap_past_sender.h
#define GAP_MSG_LE_PAST_SENDER_PERIODIC_ADV_SET_INFO_TRANSFER  0x6E //!<Response msg type for le_past_sender_periodic_adv_set_info_transfer
#endif


#define GAP_MSG_LE_GAP_STATE_MSG                    0xB0

#if F_BT_LE_FIX_CHANN_SUPPORT
//gap_fix_chann_conn.h
#define GAP_MSG_LE_FIXED_CHANN_DATA_SEND            0xC0 //!<Response msg type for le_fixed_chann_data_send
#define GAP_MSG_LE_FIXED_CHANN_DATA_IND             0xC1 //!<Indication for le fixed channel data received
#define GAP_MSG_LE_FIXED_CHANN_REG                  0xC2 //!<Response msg type for le_fixed_chann_reg
#endif

/** End of GAP_LE_MSG_Types
  * @}
  */

/** End of Gap_CB_Msg_Exported_Macros
* @}
*/


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup Gap_CB_Msg_Exported_Types GAP Callback Msg Exported Types
  * @{
  */
typedef struct
{
    uint16_t            cause;
} T_LE_CAUSE;

/** @brief  Response of le modify white list request.*/
typedef struct
{
    T_GAP_WHITE_LIST_OP operation;
    uint16_t            cause;
} T_LE_MODIFY_WHITE_LIST_RSP;

/** @brief  Response of le set random address request. */
typedef struct
{
    uint16_t        cause;
} T_LE_SET_RAND_ADDR_RSP;

/** @brief  Response of le set channel classification request. */
typedef struct
{
    uint16_t        cause;
} T_LE_SET_HOST_CHANN_CLASSIF_RSP;

/** @brief  Response for read rssi.*/
typedef struct
{
    uint8_t         conn_id;
    int8_t          rssi;
    uint16_t        cause;
} T_LE_READ_RSSI_RSP;

#if F_BT_LE_READ_CHANN_MAP
/** @brief  Response for read chanel map.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
    uint8_t         channel_map[5];
} T_LE_READ_CHANN_MAP_RSP;
#endif
#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
/** @brief  Response for set data length, which is used for BT4.2 data length extension.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
} T_LE_SET_DATA_LEN_RSP;

/** @brief  Notification for data length change info, which is used for BT4.2 data length extension.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        max_tx_octets;
    uint16_t        max_tx_time;
    uint16_t        max_rx_octets;
    uint16_t        max_rx_time;
} T_LE_DATA_LEN_CHANGE_INFO;
#endif

#if F_BT_LE_GAP_CENTRAL_SUPPORT
/** @brief  Indication for connection paramete update.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        conn_interval_min;
    uint16_t        conn_interval_max;
    uint16_t        conn_latency;
    uint16_t        supervision_timeout;
} T_LE_CONN_UPDATE_IND;
#endif

/** @brief  Indication of le connectiona. */
typedef struct
{
    uint8_t                bd_addr[6];/**< Bluetooth address of remote device */
    T_GAP_REMOTE_ADDR_TYPE remote_addr_type; /**< Address type of remote device */
}  T_LE_CREATE_CONN_IND;

#if F_BT_LE_5_0_SET_PHYS_SUPPORT
/** @brief  Notification information when phy changed.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
    T_GAP_PHYS_TYPE tx_phy;
    T_GAP_PHYS_TYPE rx_phy;
    bool            read_phy_rsp; /* True:  Callback data is result of read PHY.
                                     False: Callback data is result of update PHY. */
} T_LE_PHY_UPDATE_INFO;
#endif

#if F_BT_LE_READ_REMOTE_FEATS
/** @brief  Information for remote device features.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
    uint8_t         remote_feats[8];
} T_LE_REMOTE_FEATS_INFO;
#endif

#if F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT
/** @brief  Response for reading remote version information.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
    uint8_t         version;
    uint16_t        manufacturer_name;
    uint16_t        subversion;
} T_LE_READ_REMOTE_VERSION_RSP;
#endif

#if F_BT_LE_5_0_AE_ADV_SUPPORT
/** @brief  Information of own address type for advertiser using adv set.*/
typedef struct
{
    uint8_t         conn_id;
    uint8_t         adv_handle;
    uint8_t         own_address_type;
    uint16_t        cause;
} T_LE_ADV_SET_CONN_OWN_ADDR_TYPE_INFO;
#endif

//gap_bond_le.h
/** @brief  Bond information modify type*/
typedef enum
{
    LE_BOND_DELETE,
    LE_BOND_ADD,
    LE_BOND_CLEAR,
    LE_BOND_FULL,
    LE_BOND_KEY_MISSING,
} T_LE_BOND_MODIFY_TYPE;

/** @brief  Structure for modify bonding information.*/
typedef struct
{
    T_LE_BOND_MODIFY_TYPE type;
    T_LE_KEY_ENTRY        *p_entry;
} T_LE_BOND_MODIFY_INFO;

#if F_BT_LE_4_2_KEY_PRESS_SUPPORT
/** @brief  Structure for notify keypress result.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
} T_LE_KEYPRESS_NOTIFY_RSP;

/** @brief  Structure for notify keypress information.*/
typedef struct
{
    uint8_t                   conn_id;
    T_GAP_KEYPRESS_NOTIF_TYPE event_type;
} T_LE_KEYPRESS_NOTIFY_INFO;
#endif

#if F_BT_LE_ATT_SIGNED_WRITE_SUPPORT
/** @brief  Structure for LE signed information.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
    bool            update_local;
    uint32_t        local_sign_count;
    uint32_t        remote_sign_count;
} T_LE_GATT_SIGNED_STATUS_INFO;
#endif

#if F_BT_LE_BOND_KEY_REQ_SUPPORT
typedef struct
{
    uint8_t                     bd_addr[6];
    T_GAP_REMOTE_ADDR_TYPE      remote_addr_type;
    T_GAP_KEY_TYPE              key_type;           /**< Key type. */
    uint8_t                     key_len;            /**< Provided by APP. */
    uint8_t                     link_key[28];       /**< Provided by APP. */
} T_LE_BOND_KEY_REQ;
#endif

#if F_BT_LE_4_0_DTM_SUPPORT
/** @brief  Response of le receiver test request. */
typedef struct
{
    uint16_t        cause;
    uint16_t        num_pkts;
} T_LE_DTM_TEST_END_RSP;
#endif

//gap_scan.h
#if F_BT_LE_GAP_SCAN_SUPPORT
/** @brief  Information of le scan information.  */
typedef struct
{
    uint8_t                bd_addr[6];/**< Bluetooth address of remote device. */
    T_GAP_REMOTE_ADDR_TYPE remote_addr_type;/**< Address type of remote device. */
    T_GAP_ADV_EVT_TYPE     adv_type;/**< Advertising event type. */
    int8_t                 rssi;   /**< RSSI. */
    uint8_t                data_len;
    uint8_t                data[31];
} T_LE_SCAN_INFO;

#if F_BT_LE_PRIVACY_SUPPORT
/** @brief  Information of le direct advertising.   */
typedef struct
{
    uint8_t                bd_addr[6];
    T_GAP_REMOTE_ADDR_TYPE remote_addr_type;
    uint8_t                direct_bd_addr[6];
    T_GAP_DIRECT_ADDR_TYPE direct_addr_type;
    T_GAP_ADV_EVT_TYPE     direct_adv_type;
    int8_t                 rssi;
} T_LE_DIRECT_ADV_INFO;
#endif
#endif

//gap_adv.h
#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
/** @brief  LE advertising paramter update result.*/
typedef struct
{
    uint16_t        cause;
} T_LE_ADV_UPDATE_PARAM_RSP;

#if F_BT_LE_READ_ADV_TX_POWRE_SUPPORT
/** @brief  Response of le read advertising transmitter power request. */
typedef struct
{
    uint16_t        cause;
    uint8_t         tx_power_level;
} T_LE_ADV_READ_TX_POWER_RSP;
#endif
#endif

#if F_BT_LE_5_0_AE_SCAN_SUPPORT
/** @brief  Information of le extended advertising report. */
typedef struct
{
    uint16_t                      event_type;
    T_GAP_EXT_ADV_EVT_DATA_STATUS data_status;
    T_GAP_REMOTE_ADDR_TYPE        addr_type;
    uint8_t                       bd_addr[6];
    T_GAP_PHYS_PRIM_ADV_TYPE      primary_phy;
    uint8_t                       secondary_phy;
    uint8_t                       adv_sid;
    int8_t                        tx_power;
    int8_t                        rssi;
    uint16_t                      peri_adv_interval;
    T_GAP_DIRECT_ADDR_TYPE        direct_addr_type;
    uint8_t                       direct_addr[6];
    uint8_t                       data_len;
    uint8_t                      *p_data; //max 229 bytes
} T_LE_EXT_ADV_REPORT_INFO;

#if F_BT_LE_GAP_MSG_INFO_WAY
/** @brief  Notification of extended scanning state.
 *          The struct will only be used after APP calls le_ext_scan_gap_msg_info_way(false).
 */
typedef struct
{
    uint16_t cause;
    uint8_t  state;         /**< @ref GAP_SCAN_STATE. */
} T_LE_EXT_SCAN_STATE_CHANGE_INFO;
#endif
#endif

#if F_BT_LE_5_0_AE_ADV_SUPPORT
//gap_ext_adv.h
typedef struct
{
    uint16_t        cause;
    uint8_t         adv_handle;
    uint8_t         flag;
} T_LE_EXT_ADV_START_SETTING_RSP;

typedef struct
{
    uint16_t        cause;
    uint8_t         adv_handle;
} T_LE_EXT_ADV_REMOVE_SET_RSP;

typedef struct
{
    uint16_t        cause;
} T_LE_EXT_ADV_CLEAR_SET_RSP;

/** @brief  Information of le scan request received. */
typedef struct
{
    uint8_t         adv_handle;
    uint8_t         scanner_addr_type;
    uint8_t         scanner_addr[6];
} T_LE_SCAN_REQ_RECEIVED_INFO;

#if F_BT_LE_GAP_MSG_INFO_WAY
/** @brief  Notification of extended advertising state.
 *          The struct will only be used after APP calls le_ext_adv_gap_msg_info_way(false).
 */
typedef struct
{
    uint16_t cause;
    uint8_t  adv_handle;       /**< Identify an advertising set. */
    uint8_t  state;            /**< GAP extended advertising state. @ref T_GAP_EXT_ADV_STATE. */
} T_LE_EXT_ADV_STATE_CHANGE_INFO;
#endif
#endif

#if F_BT_LE_5_0_PA_ADV_SUPPORT
typedef struct
{
    uint16_t        cause;
    uint8_t         adv_handle;
    uint8_t         flag;
} T_LE_PA_ADV_START_SETTING_RSP;

typedef struct
{
    uint16_t        cause;
    uint8_t         adv_handle;
    uint8_t         enable;
} T_LE_PA_ADV_SET_PERIODIC_ADV_ENABLE_RSP;

/** @brief  Notification of PA periodic advertising state. */
typedef struct
{
    uint16_t cause;
    uint8_t  adv_handle;          /**< Identify an advertising set. */
    uint8_t  state;               /**< PA periodic advertising state. @ref T_GAP_PA_ADV_STATE. */
} T_LE_PA_ADV_STATE_CHANGE_INFO;
#endif

#if F_BT_LE_5_0_PA_SYNC_SUPPORT
/** @brief  Response of le modify periodic advertiser list request.*/
typedef struct
{
    T_GAP_PA_SYNC_PERIODIC_ADV_LIST_OP operation;
    uint16_t            cause;
} T_LE_PA_SYNC_MODIFY_PERIODIC_ADV_LIST_RSP;

/** @brief  Information of le periodic advertising report. */
typedef struct
{
    uint8_t     sync_id;          /**< Identify the periodic advertising train. */
    uint16_t    sync_handle;      /**< Sync_Handle identifying the periodic advertising train. */
    int8_t      tx_power;         /**< Range: -127 to +20, Units: dBm.
                                       0x7F: Tx Power information not available. */
    int8_t      rssi;             /**< Range: -127 to +20, Units: dBm.
                                       0x7F: RSSI is not available. */
    T_GAP_PERIODIC_ADV_REPORT_CTE_TYPE     cte_type;
    T_GAP_PERIODIC_ADV_REPORT_DATA_STATUS  data_status;
    uint8_t     data_len;/**< Length of the Data field. Range: 0 to 247. */
    uint8_t    *p_data; /**< Data received from a Periodic Advertising packet. */
} T_LE_PERIODIC_ADV_REPORT_INFO;

/** @brief  Definition of synchronization device state of periodic advertising trains. */
typedef struct
{
    uint8_t gap_terminate_sync_state: 1;               /**< @ref GAP_PA_TERMINATE_SYNC_DEV_STATE. */
    uint8_t gap_create_sync_state: 1;                  /**< @ref GAP_PA_CREATE_SYNC_DEV_STATE. */
    uint8_t gap_periodic_adv_receive_enable_state: 1;  /**< @ref GAP_PA_RECEIVE_ENABLE_DEV_STATE. */
    uint8_t gap_pa_sync_rfu_state: 5;                  /**< Reserved for future use. */
} T_GAP_PA_SYNC_DEV_STATE;

/** @brief  Notification of synchronization device state of periodic advertising trains. */
typedef struct
{
    T_GAP_PA_SYNC_DEV_STATE  state;
    uint16_t cause;
} T_LE_PA_SYNC_DEV_STATE_CHANGE_INFO;

/** @brief  Notification of synchronization state of periodic advertising trains. */
typedef struct
{
    uint8_t sync_id;                  /**< Identify the periodic advertising train. */
    uint16_t  sync_handle;            /**< Sync_Handle identifying the periodic advertising train. */
    uint8_t state;                    /**< @ref T_GAP_PA_SYNC_STATE. */
    bool sync_transfer_received_flag; /* sync_transfer_received_flag will be only used when state is GAP_PA_SYNC_STATE_SYNCHRONIZED.
                                         True:  Periodic advertising synchroinzation is received through le_past_recipient_set_default_periodic_adv_sync_transfer_params
                                                or le_past_recipient_set_periodic_adv_sync_transfer_params.
                                         False: Periodic advertising packet is received through le_pa_sync_create_sync.*/
    uint16_t cause;
} T_LE_PA_SYNC_STATE_CHANGE_INFO;
#endif

#if ((F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_1_SUPPORT) || F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
typedef struct
{
    uint8_t             sync_id;
    uint16_t            sync_handle;
    uint16_t            cause;
    uint8_t             enable;
} T_LE_PA_SET_PERIODIC_ADV_RECEIVE_ENABLE_RSP;
#endif

#if F_BT_LE_5_1_PAST_SUPPORT
#if F_BT_LE_5_1_PAST_SENDER_SUPPORT
#if F_BT_LE_5_1_PAST_SENDER_ADV_SUPPORT
typedef struct
{
    uint16_t        cause;
    uint8_t         conn_id;
    uint8_t         adv_handle;    /* Not used */
} T_LE_PAST_SENDER_PERIODIC_ADV_SET_INFO_TRANSFER_RSP;
#endif
#if F_BT_LE_5_1_PAST_SENDER_SYNC_SUPPORT
typedef struct
{
    uint16_t        cause;
    uint8_t         conn_id;
    uint8_t         sync_id;       /* Not used */
    uint16_t        sync_handle;   /* Not used */
} T_LE_PAST_SENDER_PERIODIC_ADV_SYNC_TRANSFER_RSP;
#endif
#endif
#if F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT
typedef struct
{
    uint16_t            cause;
} T_LE_PAST_RECIPIENT_SET_DEFAULT_PERIODIC_ADV_SYNC_TRANSFER_PARAMS_RSP;

typedef struct
{
    uint16_t            cause;
    uint8_t             conn_id;
} T_LE_PAST_RECIPIENT_SET_PERIODIC_ADV_SYNC_TRANSFER_PARAMS_RSP;

typedef struct
{
    uint16_t            cause;
    uint8_t             conn_id;      /**< Used to identify a connection. */
    uint8_t             sync_id;      /**< Identify the periodic advertising train. */
    uint16_t            sync_handle;  /**< Sync_Handle identifying the periodic advertising train. */
    uint16_t            service_data; /**< A value provided by peer device. */
    uint8_t             adv_sid;      /**< Value of the Advertising SID used to advertise the
                                           periodic advertising. */
    T_GAP_PA_SYNC_ADV_ADDR_TYPE            adv_addr_type; /**< Advertiser address type. @ref
                                                               T_GAP_PA_SYNC_ADV_ADDR_TYPE for reception of info. */
    uint8_t             adv_addr[GAP_BD_ADDR_LEN];  /**< Public Device Address, Random Device Address,
                                                         Public Identity Address, or Random (static) Identity
                                                         Address of the advertiser. */
    T_GAP_PHYS_TYPE     adv_phy;                    /**< Advertiser_PHY specifies the PHY used for the
                                                         periodic advertising. */
    uint16_t            periodic_adv_interval;      /**< Periodic advertising interval.
                                                         Range: 0x0006 to 0xFFFF
                                                         Time = N * 1.25 ms
                                                         Time Range: 7.5 ms to 81.91875 s */
    uint8_t             adv_clock_accuracy;         /**< Advertiser_Clock_Accuracy specifies the
                                                         accuracy of the periodic advertiser's clock.
                                                         0x00: 500 ppm
                                                         0x01: 250 ppm
                                                         0x02: 150 ppm
                                                         0x03: 100 ppm
                                                         0x04: 75  ppm
                                                         0x05: 50  ppm
                                                         0x06: 30  ppm
                                                         0x07: 20  ppm */
} T_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO;
#endif
#endif

#if F_BT_LE_5_2_SUPPORT
typedef struct
{
    uint8_t sync_id;         /**< Identify the periodic advertising train. */
    uint16_t  sync_handle;   /**< Sync_Handle identifying the periodic advertising train. */
    uint8_t num_bis;         /**< The number of BISes in the BIG. */
    uint8_t nse;             /**< The number of subevents per BIS in each BIG event. */
    uint16_t iso_interval;   /**< Value of the ISO_Interval subfield of the BIGInfo field.
                                  Time between two adjacent BIG anchor points, in units of 1.25 ms.
                                  The value shall be between 4 and 3200 (i.e. 5 ms to 4 s).*/
    uint8_t bn;              /**< Value of the BN subfield of the BIGInfo field.
                                  Burst Number contains the number of new payloads for each BIS in a BIS event. */
    uint8_t pto;             /**< Value of the PTO subfield of the BIGInfo field.
                                  Pre_Transmission_Offset contains the offset in number of ISO_Intervals for
                                  pre transmissions of data packets. */
    uint8_t irc;             /**< Value of the IRC subfield of the BIGInfo field.
                                  Immediate Repetition Count contains the number of times the scheduled
                                  data packet is transmitted. */
    uint16_t max_pdu;        /**< The maximum number of data octets (excluding the MIC, if any) that can be
                                  carried in each BIS Data PDU in the BIG. */
    uint32_t sdu_interval;   /**< Value of the SDU_Interval subfield of the BIGInfo field.
                                  The time interval, in microseconds, of the periodic SDUs. */
    uint16_t max_sdu;        /**< The maximum size, in octets, of an SDU. */
    T_GAP_PHYS_TYPE phy;     /**< Indicate the PHY used by the BIG. @ref T_GAP_PHYS_TYPE. */
    uint8_t framing;         /**< Indicate whether the BIG carries framed or unframed data.
                                  0x00: Unframed
                                  0x01: Framed */
    uint8_t encryption;      /**< Indicate whether the BIG is encrypted or not.
                                  0x00: BIG carries unencrypted data
                                  0x01: BIG carries encrypted data */
} T_LE_BIGINFO_ADV_REPORT_INFO;
#endif

#if F_BT_LE_FIX_CHANN_SUPPORT
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cid;
    uint16_t        cause;
} T_LE_FIXED_CHANN_DATA_SEND_RSP;

typedef struct
{
    uint8_t         conn_id;        /**<  local link ID.  */
    uint16_t        cid;            /**<  channel ID  */
    uint16_t        value_len;      /**<  value length  */
    uint8_t         *p_data;        /**<  value.  */
} T_LE_FIXED_CHANN_DATA_IND;

typedef struct
{
    uint16_t        cid;
    uint16_t        cause;
} T_LE_FIXED_CHANN_REG_RSP;
#endif

/** @brief  GAP LE Callback Data*/
typedef union
{
    T_LE_CAUSE                                le_cause;

//gap_le.h
    T_LE_MODIFY_WHITE_LIST_RSP               *p_le_modify_white_list_rsp;
    T_LE_SET_RAND_ADDR_RSP                   *p_le_set_rand_addr_rsp;
    T_LE_SET_HOST_CHANN_CLASSIF_RSP          *p_le_set_host_chann_classif_rsp;
//gap_conn_le.h
    T_LE_READ_RSSI_RSP                       *p_le_read_rssi_rsp;
#if F_BT_LE_READ_CHANN_MAP
    T_LE_READ_CHANN_MAP_RSP                  *p_le_read_chann_map_rsp;
#endif
#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
    T_LE_SET_DATA_LEN_RSP                    *p_le_set_data_len_rsp;
    T_LE_DATA_LEN_CHANGE_INFO                *p_le_data_len_change_info;
#endif
#if F_BT_LE_GAP_CENTRAL_SUPPORT
    T_LE_CONN_UPDATE_IND                     *p_le_conn_update_ind;
#endif
    T_LE_CREATE_CONN_IND                     *p_le_create_conn_ind;
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
    T_LE_PHY_UPDATE_INFO                     *p_le_phy_update_info;
#endif
#if F_BT_LE_READ_REMOTE_FEATS
    T_LE_REMOTE_FEATS_INFO                   *p_le_remote_feats_info;
#endif
#if F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT
    T_LE_READ_REMOTE_VERSION_RSP             *p_le_read_remote_version_rsp;
#endif
#if F_BT_LE_5_0_AE_ADV_SUPPORT
    T_LE_ADV_SET_CONN_OWN_ADDR_TYPE_INFO     *p_le_adv_set_conn_own_addr_type_info;
#endif
//gap_bond_le.h
    T_LE_BOND_MODIFY_INFO                    *p_le_bond_modify_info;
#if F_BT_LE_4_2_KEY_PRESS_SUPPORT
    T_LE_KEYPRESS_NOTIFY_RSP                 *p_le_keypress_notify_rsp;
    T_LE_KEYPRESS_NOTIFY_INFO                *p_le_keypress_notify_info;
#endif
#if F_BT_LE_ATT_SIGNED_WRITE_SUPPORT
    T_LE_GATT_SIGNED_STATUS_INFO             *p_le_gatt_signed_status_info;
#endif
#if F_BT_LE_BOND_KEY_REQ_SUPPORT
    T_LE_BOND_KEY_REQ                        *p_le_bond_key_req;
#endif
//gap_scan.h
#if F_BT_LE_GAP_SCAN_SUPPORT
    T_LE_SCAN_INFO                           *p_le_scan_info;
#if F_BT_LE_PRIVACY_SUPPORT
    T_LE_DIRECT_ADV_INFO                     *p_le_direct_adv_info;
#endif
#endif
//gap_adv.h
#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
    T_LE_ADV_UPDATE_PARAM_RSP                *p_le_adv_update_param_rsp;
#if F_BT_LE_READ_ADV_TX_POWRE_SUPPORT
    T_LE_ADV_READ_TX_POWER_RSP               *p_le_adv_read_tx_power_rsp;
#endif
#endif
//gap_dtm.h
#if F_BT_LE_4_0_DTM_SUPPORT
    T_LE_DTM_TEST_END_RSP                    *p_le_dtm_test_end_rsp;
#endif
//gap_ext_scan.h
#if F_BT_LE_5_0_AE_SCAN_SUPPORT
    T_LE_EXT_ADV_REPORT_INFO                 *p_le_ext_adv_report_info;
#if F_BT_LE_GAP_MSG_INFO_WAY
    /* The callback data will only be used after APP calls @ref le_ext_scan_gap_msg_info_way(false). */
    T_LE_EXT_SCAN_STATE_CHANGE_INFO          *p_le_ext_scan_state_change_info;
#endif
#endif
//gap_ext_adv.h
#if F_BT_LE_5_0_AE_ADV_SUPPORT
    T_LE_EXT_ADV_START_SETTING_RSP           *p_le_ext_adv_start_setting_rsp;
    T_LE_EXT_ADV_REMOVE_SET_RSP              *p_le_ext_adv_remove_set_rsp;
    T_LE_EXT_ADV_CLEAR_SET_RSP               *p_le_ext_adv_clear_set_rsp;
    T_LE_SCAN_REQ_RECEIVED_INFO              *p_le_scan_req_received_info;
#if F_BT_LE_GAP_MSG_INFO_WAY
    /* The callback data will only be used after APP calls @ref le_ext_adv_gap_msg_info_way(false). */
    T_LE_EXT_ADV_STATE_CHANGE_INFO           *p_le_ext_adv_state_change_info;
#endif
#endif

#if F_BT_LE_5_0_PA_ADV_SUPPORT
    T_LE_PA_ADV_START_SETTING_RSP            *p_le_pa_adv_start_setting_rsp;
    T_LE_PA_ADV_SET_PERIODIC_ADV_ENABLE_RSP  *p_le_pa_adv_set_periodic_adv_enable_rsp;
    T_LE_PA_ADV_STATE_CHANGE_INFO            *p_le_pa_adv_state_change_info;
#endif
#if F_BT_LE_5_0_PA_SYNC_SUPPORT
    T_LE_PA_SYNC_MODIFY_PERIODIC_ADV_LIST_RSP *p_le_pa_sync_modify_periodic_adv_list_rsp;
    T_LE_PERIODIC_ADV_REPORT_INFO             *p_le_periodic_adv_report_info;
    T_LE_PA_SYNC_DEV_STATE_CHANGE_INFO        *p_le_pa_sync_dev_state_change_info;
    T_LE_PA_SYNC_STATE_CHANGE_INFO            *p_le_pa_sync_state_change_info;
#endif
#if ((F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_1_SUPPORT) || F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
    T_LE_PA_SET_PERIODIC_ADV_RECEIVE_ENABLE_RSP *p_le_pa_set_periodic_adv_receive_enable_rsp;
#endif

#if F_BT_LE_5_1_PAST_SUPPORT
#if F_BT_LE_5_1_PAST_SENDER_SUPPORT
#if F_BT_LE_5_1_PAST_SENDER_ADV_SUPPORT
    T_LE_PAST_SENDER_PERIODIC_ADV_SET_INFO_TRANSFER_RSP
    *p_le_past_sender_periodic_adv_set_info_transfer_rsp;
#endif
#if F_BT_LE_5_1_PAST_SENDER_SYNC_SUPPORT
    T_LE_PAST_SENDER_PERIODIC_ADV_SYNC_TRANSFER_RSP
    *p_le_past_sender_periodic_adv_sync_transfer_rsp;
#endif
#endif
#if F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT
    T_LE_PAST_RECIPIENT_SET_DEFAULT_PERIODIC_ADV_SYNC_TRANSFER_PARAMS_RSP
    *p_le_past_recipient_set_default_periodic_adv_sync_transfer_params_rsp;
    T_LE_PAST_RECIPIENT_SET_PERIODIC_ADV_SYNC_TRANSFER_PARAMS_RSP
    *p_le_past_recipient_set_periodic_adv_sync_transfer_params_rsp;
    T_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO
    *p_le_past_recipient_periodic_adv_sync_transfer_received_info;
#endif
#endif

#if F_BT_LE_5_2_SUPPORT
    T_LE_BIGINFO_ADV_REPORT_INFO             *p_le_biginfo_adv_report_info;
#endif

    void                                     *p_gap_state_msg;

    void                                     *p_le_cb_data;
//gap_fixed_chann_conn.h
#if F_BT_LE_FIX_CHANN_SUPPORT
    T_LE_FIXED_CHANN_DATA_SEND_RSP           *p_le_fixed_chann_data_send_rsp;
    T_LE_FIXED_CHANN_DATA_IND                *p_le_fixed_chann_data_ind;
    T_LE_FIXED_CHANN_REG_RSP                 *p_le_fixed_chann_reg_rsp;
#endif
} T_LE_CB_DATA;

/** End of Gap_CB_Msg_Exported_Types
* @}
*/

/** End of GAP_CB_MSG_MODULE
* @}
*/

#ifdef __cplusplus
}
#endif

#endif

#endif /* GAP_MSG_H */
