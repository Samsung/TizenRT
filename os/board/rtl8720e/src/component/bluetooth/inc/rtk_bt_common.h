/**
 * @file      rtk_bt_common.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth Common type and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_COMMON_H__
#define __RTK_BT_COMMON_H__

#include <dlist.h>
#include <basic_types.h>
#include <bt_app_config.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define RTK_EVT_POOL_SIZE 8

#define BT_TIMEOUT_NONE             0
#define BT_API_SYNC_TIMEOUT         3000
#define BT_TIMEOUT_FOREVER          0xffffffffUL

#define RTK_BT_API_LE_BASE          0x0
#define RTK_BT_API_BR_BASE          0x80
#define RTK_BT_API_COMMON_BASE      0xE0
#define RTK_BT_API_TASK_EXIT        0xFF
#define RTK_BT_EVENT_TASK_EXIT      0xFF


/* BT Protocol/Profile UUID definitions. */
#define RTK_BT_UUID_SDP                            0x0001
#define RTK_BT_UUID_UDP                            0x0002
#define RTK_BT_UUID_RFCOMM                         0x0003
#define RTK_BT_UUID_TCP                            0x0004
#define RTK_BT_UUID_TCSBIN                         0x0005
#define RTK_BT_UUID_TCSAT                          0x0006
#define RTK_BT_UUID_ATT                            0x0007
#define RTK_BT_UUID_OBEX                           0x0008
#define RTK_BT_UUID_IP                             0x0009
#define RTK_BT_UUID_FTP                            0x000A
#define RTK_BT_UUID_HTTP                           0x000C
#define RTK_BT_UUID_WSP                            0x000E
#define RTK_BT_UUID_BNEP                           0x000F
#define RTK_BT_UUID_UPNP                           0x0010
#define RTK_BT_UUID_HIDP                           0x0011
#define RTK_BT_UUID_HC_CONTROL                     0x0012
#define RTK_BT_UUID_HC_DATA                        0x0014
#define RTK_BT_UUID_HC_NOTIFICATION                0x0016
#define RTK_BT_UUID_AVCTP                          0x0017
#define RTK_BT_UUID_AVDTP                          0x0019
#define RTK_BT_UUID_CMTP                           0x001B
#define RTK_BT_UUID_HDP_CTRL_CHANNEL               0x001E
#define RTK_BT_UUID_HDP_DATA_CHANNEL               0x001F
#define RTK_BT_UUID_RDTP                           0x00D1

#define RTK_BT_UUID_L2CAP                          0x0100

#define RTK_BT_UUID_SERVICE_DISCOVERY_SERVER       0x1000
#define RTK_BT_UUID_BROWSE_GROUP_DESCRIPTOR        0x1001
#define RTK_BT_UUID_PUBLIC_BROWSE_GROUP            0x1002
#define RTK_BT_UUID_SERIAL_PORT                    0x1101
#define RTK_BT_UUID_LAN_ACCESS                     0x1102
#define RTK_BT_UUID_DIAL_UP_NETWORKING             0x1103
#define RTK_BT_UUID_IRMC_SYNC                      0x1104
#define RTK_BT_UUID_OBEX_OBJECT_PUSH               0x1105
#define RTK_BT_UUID_OBEX_FILE_TRANSFER             0x1106
#define RTK_BT_UUID_IRM_SYNC_COMMAND               0x1107
#define RTK_BT_UUID_HEADSET                        0x1108
#define RTK_BT_UUID_HEADSET_HS                     0x1131
#define RTK_BT_UUID_CORDLESS_TELEPHONY             0x1109
#define RTK_BT_UUID_AUDIO_SOURCE                   0x110A
#define RTK_BT_UUID_AUDIO_SINK                     0x110B
#define RTK_BT_UUID_AV_REMOTE_CONTROL_TARGET       0x110C
#define RTK_BT_UUID_ADVANCED_AUDIO_DISTRIBUTION    0x110D
#define RTK_BT_UUID_AV_REMOTE_CONTROL              0x110E
#define RTK_BT_UUID_AV_REMOTE_CONTROL_CONTROLLER   0x110F
#define RTK_BT_UUID_INTERCOM                       0x1110
#define RTK_BT_UUID_FAX                            0x1111
#define RTK_BT_UUID_HEADSET_AUDIO_GATEWAY          0x1112
#define RTK_BT_UUID_WAP                            0x1113
#define RTK_BT_UUID_WAP_CLIENT                     0x1114
#define RTK_BT_UUID_PANU                           0x1115
#define RTK_BT_UUID_NAP                            0x1116
#define RTK_BT_UUID_GN                             0x1117
#define RTK_BT_UUID_DIRECT_PRINTING                0x1118
#define RTK_BT_UUID_REFERENCE_PRINTING             0x1119
#define RTK_BT_UUID_IMAGING                        0x111A
#define RTK_BT_UUID_IMAGING_RESPONDER              0x111B
#define RTK_BT_UUID_IMAGING_AUTOMATIC_ARCHIVE      0x111C
#define RTK_BT_UUID_REFERENCED_OBJECTS             0x111D
#define RTK_BT_UUID_HANDSFREE                      0x111E
#define RTK_BT_UUID_HANDSFREE_AUDIO_GATEWAY        0x111F
#define RTK_BT_UUID_DIRECT_PRINTING_REF_OBJECTS    0x1120
#define RTK_BT_UUID_REFLECTED_UI                   0x1121
#define RTK_BT_UUID_BASIC_PRINTING                 0x1122
#define RTK_BT_UUID_PRINTING_STATUS                0x1123
#define RTK_BT_UUID_HUMAN_INTERFACE_DEVICE_SERVICE 0x1124
#define RTK_BT_UUID_HARDCOPY_CABLE_REPLACEMENT     0x1125
#define RTK_BT_UUID_HCR_PRINT                      0x1126
#define RTK_BT_UUID_HCR_SCAN                       0x1127
#define RTK_BT_UUID_COMMON_ISDN_ACCESS             0x1128
#define RTK_BT_UUID_VIDEO_CONFERENCING_GW          0x1129
#define RTK_BT_UUID_SIM                            0x112D
#define RTK_BT_UUID_PBAP_PCE                       0x112E
#define RTK_BT_UUID_PBAP_PSE                       0x112F
#define RTK_BT_UUID_PBAP                           0x1130
#define RTK_BT_UUID_MSG_ACCESS_SERVER              0x1132
#define RTK_BT_UUID_MSG_NOTIFICATION_SERVER        0x1133
#define RTK_BT_UUID_MSG_ACCESS_PROFILE             0x1134
#define RTK_BT_UUID_PNP_INFORMATION                0x1200
#define RTK_BT_UUID_GENERIC_NETWORKING             0x1201
#define RTK_BT_UUID_GENERIC_FILE_TRANSFER          0x1202
#define RTK_BT_UUID_GENERIC_AUDIO                  0x1203
#define RTK_BT_UUID_GENERIC_TELEPHONY              0x1204
#define RTK_BT_UUID_HDP_PROFILE                    0x1400
#define RTK_BT_UUID_HDP_PROFILE_SOURCE             0x1401
#define RTK_BT_UUID_HDP_PROFILE_SINK               0x1402
#define RTK_BT_UUID_GAP                            0x1800
#define RTK_BT_UUID_GATT                           0x1801

/* BT Protocol/Service Multiplexer definitions. */
#define RTK_BT_PSM_SDP                             0x0001
#define RTK_BT_PSM_RFCOMM                          0x0003
#define RTK_BT_PSM_HID_CONTROL                     0x0011
#define RTK_BT_PSM_HID_INTERRUPT                   0x0013
#define RTK_BT_PSM_AVCTP                           0x0017
#define RTK_BT_PSM_AVDTP                           0x0019
#define RTK_BT_PSM_AVCTP_BROWSING                  0x001B
#define RTK_BT_PSM_ATT                             0x001F
#define RTK_BT_PSM_RDTP                            0x00D1
#define RTK_BT_PSM_EATT                            0x0027

/**
 * @typedef   rtk_bt_profile_t
 * @brief     Bluetooth LE profile type bit mask
 */
typedef enum {
	RTK_BT_PROFILE_GATTS    =  BIT0,
	RTK_BT_PROFILE_GATTC    =  BIT1,
	RTK_BT_PROFILE_A2DP     =  BIT2,
	RTK_BT_PROFILE_AVRCP    =  BIT3,
	RTK_BT_PROFILE_SDP      =  BIT4,
	RTK_BT_PROFILE_LEAUDIO  =  BIT5,
	RTK_BT_PROFILE_SPP      =  BIT6,
	RTK_BT_PROFILE_HFP      =  BIT7,
	RTK_BT_PROFILE_MESH     =  BIT8,
} rtk_bt_profile_t;

/***************************************BLE group************************************/

/**
 * @typedef   rtk_bt_le_group_t
 * @brief     Bluetooth LE action/event group
 */
typedef enum {
	RTK_BT_LE_GP_GAP = RTK_BT_API_LE_BASE,      /*!< LE gap group */
	RTK_BT_LE_GP_GATTC,                         /*!< LE gatt client group */
	RTK_BT_LE_GP_GATTS,                         /*!< LE gatt server group */
	RTK_BT_LE_GP_ISO,                           /*!< LE iso group */
	RTK_BT_LE_GP_AUDIO,                         /*!< LE audio group */
	RTK_BT_LE_GP_MESH_STACK,                    /*!< LE mesh stack group */
	RTK_BT_LE_GP_MESH_CONFIG_MODEL,             /*!< LE mesh config model group */
	RTK_BT_LE_GP_MESH_DATATRANS_MODEL,          /*!< LE mesh datatrans model group */
	RTK_BT_LE_GP_MAX,                           /*!< Reserved */
} rtk_bt_le_group_t;

/**
 * @typedef   rtk_bt_le_gap_act_t
 * @brief     Bluetooth LE GAP action
 */
typedef enum {
	RTK_BT_LE_GAP_ACT_GET_VERSION = 1,
	RTK_BT_LE_GAP_ACT_GET_DEVICE_ADDR,
	RTK_BT_LE_GAP_ACT_SET_DEVICE_NAME,
	RTK_BT_LE_GAP_ACT_SET_APPEARANCE,
	RTK_BT_LE_GAP_ACT_SET_PREFERRED_CONN_PARAM,
	RTK_BT_LE_GAP_ACT_SET_RAND_ADDR,
	RTK_BT_LE_GAP_ACT_SET_ADV_DATA,
	RTK_BT_LE_GAP_ACT_SET_SCAN_RSP_DATA,
	RTK_BT_LE_GAP_ACT_START_ADV,
	RTK_BT_LE_GAP_ACT_STOP_ADV,
	RTK_BT_LE_GAP_ACT_SCAN_INFO_FILTER,
	RTK_BT_LE_GAP_ACT_SET_SCAN_PARAM,
	RTK_BT_LE_GAP_ACT_START_SCAN,
	RTK_BT_LE_GAP_ACT_STOP_SCAN,
	RTK_BT_LE_GAP_ACT_CONN,
	RTK_BT_LE_GAP_ACT_DISCONN,
	RTK_BT_LE_GAP_ACT_UPDATE_CONN_PARAM,
	RTK_BT_LE_GAP_ACT_READ_RSSI,
	RTK_BT_LE_GAP_ACT_MODIFY_WHITELIST,
	RTK_BT_LE_GAP_ACT_GET_CONN_INFO,
	RTK_BT_LE_GAP_ACT_GET_DEV_STATE,
	RTK_BT_LE_GAP_ACT_GET_ACTIVE_CONN,
	RTK_BT_LE_GAP_ACT_GET_CONN_HANDLE_BY_ADDR,
	RTK_BT_LE_GAP_ACT_GET_MTU_SIZE,
	RTK_BT_LE_GAP_ACT_SET_CHANNELS,
	RTK_BT_LE_GAP_ACT_SET_DATA_LEN,
	RTK_BT_LE_GAP_ACT_SET_PHY,
	RTK_BT_LE_GAP_ACT_PRIVACY_INIT,
	RTK_BT_LE_GAP_ACT_PRIVACY_MODIFY_LIST,
	RTK_BT_LE_GAP_ACT_SET_SEC_PARAM,
	RTK_BT_LE_GAP_ACT_START_SECURITY,
	RTK_BT_LE_GAP_ACT_PAIRING_CONFIRM,
	RTK_BT_LE_GAP_ACT_PASSKEY_ENTRY,
	RTK_BT_LE_GAP_ACT_PASSKEY_CONFIRM,
	RTK_BT_LE_GAP_ACT_OOB_KEY_INPUT,
	RTK_BT_LE_GAP_ACT_GET_BOND_NUM,
	RTK_BT_LE_GAP_ACT_GET_BOND_INFO,
	RTK_BT_LE_GAP_ACT_DELETE_BOND_DEVICE,
	RTK_BT_LE_GAP_ACT_CLEAR_BOND_LIST,
	RTK_BT_LE_GAP_ACT_VENDOR_CMD_REQ,
	RTK_BT_LE_GAP_ACT_SET_EXT_ADV_DATA,
	RTK_BT_LE_GAP_ACT_SET_EXT_SCAN_RSP_DATA,
	RTK_BT_LE_GAP_ACT_CREATE_EXT_ADV,
	RTK_BT_LE_GAP_ACT_START_EXT_ADV,
	RTK_BT_LE_GAP_ACT_STOP_EXT_ADV,
	RTK_BT_LE_GAP_ACT_REMOVE_EXT_ADV,
	RTK_BT_LE_GAP_ACT_EXT_SCAN_SET_PARAM,
	RTK_BT_LE_GAP_ACT_EXT_SCAN_START,
	RTK_BT_LE_GAP_ACT_EXT_SCAN_STOP,
	RTK_BT_LE_GAP_ACT_EXT_CONN,
	RTK_BT_LE_GAP_ACT_START_PA,
	RTK_BT_LE_GAP_ACT_STOP_PA,
	RTK_BT_LE_GAP_ACT_UPDATE_PA,
	RTK_BT_LE_GAP_ACT_PA_SYNC_GET_PARAM,
	RTK_BT_LE_GAP_ACT_PA_SYNC_MODIFY_ADV_LIST,
	RTK_BT_LE_GAP_ACT_PA_SYNC_CREATE,
	RTK_BT_LE_GAP_ACT_PA_SYNC_TERMINATE,
	RTK_BT_LE_GAP_ACT_PAST_TRANSFER,
	RTK_BT_LE_GAP_ACT_PAST_RECV_SET,
	RTK_BT_LE_GAP_ACT_DEFAULT_PAST_RECV_SET,
	RTK_BT_LE_GAP_ACT_GET_TX_PENDING_NUM,
	RTK_BT_LE_GAP_READ_LOCAL_TX_POWER,
	RTK_BT_LE_GAP_READ_REMOTE_TX_POWER,
	RTK_BT_LE_GAP_TX_POWER_REPORT_SET,
	RTK_BT_LE_GAP_ACT_MAX,
} rtk_bt_le_gap_act_t;

/**
 * @typedef   rtk_bt_le_gap_evt_t
 * @brief     Bluetooth LE GAP event indication
 */
typedef enum {
	RTK_BT_LE_GAP_EVT_ADV_START_IND = 1,            /*!< Indicate LE adv started, with msg @ref rtk_bt_le_adv_start_ind_t */
	RTK_BT_LE_GAP_EVT_ADV_STOP_IND,                 /*!< Indicate LE adv stopped, with msg @ref rtk_bt_le_adv_stop_ind_t*/
	RTK_BT_LE_GAP_EVT_CONNECT_IND,                  /*!< Indicate LE connection complete, with msg @ref rtk_bt_le_conn_ind_t*/
	RTK_BT_LE_GAP_EVT_DISCONN_IND,                  /*!< Indicate LE disconnection complete, with msg @ref rtk_bt_le_disconn_ind_t*/
	RTK_BT_LE_GAP_EVT_SCAN_START_IND,               /*!< Indicate LE scan started, with msg @ref rtk_bt_le_scan_start_ind_t */
	RTK_BT_LE_GAP_EVT_SCAN_RES_IND,                 /*!< Indicate LE scan data report, with msg @ref rtk_bt_le_scan_res_ind_t*/
	RTK_BT_LE_GAP_EVT_SCAN_STOP_IND,                /*!< Indicate LE scan stopped, with msg @ref rtk_bt_le_scan_stop_ind_t*/
	RTK_BT_LE_GAP_EVT_CONN_UPDATE_IND,              /*!< Indicate LE connection parameter updated, with msg @ref rtk_bt_le_conn_update_ind_t*/
	RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND,   /*!< Indicate LE remote device connection parameter change request, with msg @ref rtk_bt_le_remote_conn_update_req_ind_t*/
	RTK_BT_LE_GAP_EVT_DATA_LEN_CHANGE_IND,          /*!< Indicate LE data length changed, with msg @ref rtk_bt_le_data_len_change_ind_t */
	RTK_BT_LE_GAP_EVT_PHY_UPDATE_IND,               /*!< Indicate LE PHY updated, with msg @ref rtk_bt_le_phy_update_ind_t */
	RTK_BT_LE_GAP_EVT_AUTH_PAIRING_CONFIRM_IND,     /*!< Indicate LE just work pairing need to confirm, with msg @ref rtk_bt_le_auth_pair_cfm_ind_t */
	RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_DISPLAY_IND,     /*!< Indicate LE pairing passkey need to display, with msg @ref rtk_bt_le_auth_key_display_ind_t */
	RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_INPUT_IND,       /*!< Indicate LE pairing need to input passkey, with msg @ref rtk_bt_le_auth_key_input_ind_t */
	RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_CONFIRM_IND,     /*!< Indicate LE pairing passkey need to comfirm, with msg @ref rtk_bt_le_auth_key_cfm_ind_t */
	RTK_BT_LE_GAP_EVT_AUTH_OOB_KEY_INPUT_IND,       /*!< Indicate LE pairing need to input oob key, with msg @ref rtk_bt_le_auth_oob_input_ind_t */
	RTK_BT_LE_GAP_EVT_AUTH_COMPLETE_IND,            /*!< Indicate LE auth complete, with msg @ref rtk_bt_le_auth_complete_ind_t */
	RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND,              /*!< Indicate LE bond info modified, with msg @ref rtk_bt_le_bond_modify_ind_t */
	RTK_BT_LE_GAP_EVT_EXT_ADV_IND,                  /*!< Indicate LE ext adv state, with msg @ref rtk_bt_le_ext_adv_ind_t*/
	RTK_BT_LE_GAP_EVT_EXT_SCAN_RES_IND,             /*!< Indicate LE ext scan data report, with msg @ref rtk_bt_le_ext_scan_res_ind_t*/
	RTK_BT_LE_GAP_EVT_PA_IND,                 		/*!< Indicate LE periodic adv state, with msg @ref rtk_bt_le_pa_ind_t*/
	RTK_BT_LE_GAP_EVT_PA_SYNC_STATE_IND,           	/*!< Indicate LE periodic adv synchronization state, with msg @ref rtk_bt_le_pa_sync_ind_t*/
	RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND,      		/*!< Indicate LE periodic adv synchronization adv report, with msg @ref rtk_bt_le_pa_adv_report_ind_t*/
	RTK_BT_LE_GAP_PAST_RECEIVED_INFO_IND,      		/*!< Indicate LE periodic adv sync transfer received info, with msg @ref rtk_bt_le_past_recv_ind_t*/
	RTK_BT_LE_GAP_EVT_RESOLV_LIST_MODIFY_IND,       /*!< Indicate LE resolving list modified, with msg @ref rtk_bt_le_modify_resolv_list_ind_t */
	RTK_BT_LE_GAP_EVT_TXPOWER_REPORT_IND,	        /*!< Indicate LE Tx power report, with msg @ref rtk_bt_le_txpower_ind_t */
	RTK_BT_LE_GAP_EVT_MAX,
} rtk_bt_le_gap_evt_t;

/**
 * @typedef   rtk_bt_gatts_act_t
 * @brief     Bluetooth GATTS action
 */
typedef enum {
	RTK_BT_GATTS_ACT_REGISTER_SERVICE = 1,
	RTK_BT_GATTS_ACT_NOTIFY,
	RTK_BT_GATTS_ACT_INDICATE,
	RTK_BT_GATTS_ACT_READ_RSP,
	RTK_BT_GATTS_ACT_WRITE_RSP,
	RTK_BT_GATTS_ACT_MAX,
} rtk_bt_gatts_act_t;

/**
 * @typedef   rtk_bt_gatts_evt_t
 * @brief     Bluetooth GATTS event indication
 */
typedef enum {
	RTK_BT_GATTS_EVT_REGISTER_SERVICE = 1,      /*!< Indicate register service complete, with msg @ref rtk_bt_gatts_reg_ind_t */
	RTK_BT_GATTS_EVT_MTU_EXCHANGE,              /*!< Indicate mtu exchanged, with msg @ref rtk_bt_gatt_mtu_exchange_ind_t */
	RTK_BT_GATTS_EVT_READ_IND,                  /*!< Indicate remote client read local server, with msg @ref rtk_bt_gatts_read_ind_t */
	RTK_BT_GATTS_EVT_WRITE_IND,                 /*!< Indicate remote client write local server, with msg @ref rtk_bt_gatts_write_ind_t */
	RTK_BT_GATTS_EVT_CCCD_IND,                  /*!< Indicate remote client write the cccd attribute, with msg @ref rtk_bt_gatts_cccd_ind_t */
	RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND,     /*!< Indicate local server indicate remote server complete, with msg @ref rtk_bt_gatts_ntf_and_ind_ind_t */
	RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND,       /*!< Indicate local server notify remote server complete, with msg @ref rtk_bt_gatts_ntf_and_ind_ind_t */
	RTK_BT_GATTS_EVT_CLIENT_SUPPORTED_FEATURES, /*!< Indicate remote client write Client Supported Features Characteristic, with msg @ref rtk_bt_gatts_client_supported_features_ind_t */
	RTK_BT_GATTS_EVT_MAX,
} rtk_bt_gatts_evt_t;

/**
 * @typedef   rtk_bt_gattc_act_t
 * @brief     Bluetooth GATTC action
 */
typedef enum {
	RTK_BT_GATTC_ACT_REGISTER_PROFILE = 1,
	RTK_BT_GATTC_ACT_UNREGISTER_PROFILE,
	RTK_BT_GATTC_ACT_ATTACH_CONN,
	RTK_BT_GATTC_ACT_DETACH_CONN,
	RTK_BT_GATTC_ACT_EXCHANGE_MTU,
	RTK_BT_GATTC_ACT_DISCOVER,
	RTK_BT_GATTC_ACT_READ,
	RTK_BT_GATTC_ACT_WRITE,
	RTK_BT_GATTC_ACT_CCCD_ENABLE,
	RTK_BT_GATTC_ACT_CCCD_DISABLE,
	RTK_BT_GATTC_ACT_CONFIRM,
	RTK_BT_GATTC_ACT_FIND,
	RTK_BT_GATTC_ACT_MAX,
} rtk_bt_gattc_act_t;

/**
 * @typedef   rtk_bt_gattc_evt_t
 * @brief     Bluetooth GATTC event indication
 */
typedef enum {
	RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND = 1,    /*!< Indicate local client discover remote server result, with msg @ref rtk_bt_gattc_discover_ind_t */
	RTK_BT_GATTC_EVT_READ_RESULT_IND,            /*!< Indicate local client read remote server result, with msg @ref rtk_bt_gattc_read_ind_t */
	RTK_BT_GATTC_EVT_WRITE_RESULT_IND,           /*!< Indicate local client write remote server result, with msg @ref rtk_bt_gattc_write_ind_t */
	RTK_BT_GATTC_EVT_NOTIFY_IND,                 /*!< Indicate local client receive notify from remote server, with msg @ref rtk_bt_gattc_cccd_value_ind_t */
	RTK_BT_GATTC_EVT_INDICATE_IND,               /*!< Indicate local client receive indicate from remote server, with msg @ref rtk_bt_gattc_cccd_value_ind_t */
	RTK_BT_GATTC_EVT_CCCD_ENABLE_IND,            /*!< Indicate local client enable remote server's cccd notify bit complete, with msg @ref rtk_bt_gattc_cccd_update_ind_t */
	RTK_BT_GATTC_EVT_CCCD_DISABLE_IND,           /*!< Indicate local client disable remote server's cccd notify bit complete, with msg @ref rtk_bt_gattc_cccd_update_ind_t */
	RTK_BT_GATTC_EVT_MTU_EXCHANGE,               /*!< Indicate local client mtu exchange complete, with msg @ref rtk_bt_gatt_mtu_exchange_ind_t */
	RTK_BT_GATTC_EVT_DISCOVER_ALL_STATE_IND,     /*!< Indicate local client discover all attributes of remote server state, with msg @ref rtk_bt_gattc_discover_all_state_ind_t */
	RTK_BT_GATTC_EVT_GATT_SERVICE_INFO_IND,      /*!< Indicate local client discover GATT service info, with msg @ref rtk_bt_gattc_gatt_service_info_ind_t */
	RTK_BT_GATTC_EVT_MAX,
} rtk_bt_gattc_evt_t;

/**
 * @typedef   rtk_bt_le_iso_act_t
 * @brief     Action of BLE ISO Management
 */
typedef enum {
	RTK_BT_LE_ISO_ACT_CIG_SETUP_PATH  = 1,
	RTK_BT_LE_ISO_ACT_CIG_REMOVE_PATH,
	RTK_BT_LE_ISO_ACT_CIG_DISCONNECT,
	RTK_BT_LE_ISO_ACT_CIG_READ_ISO_TX_SYNC,
	RTK_BT_LE_ISO_ACT_CIG_READ_LINK_QUALITY,
	RTK_BT_LE_ISO_ACT_CIG_INITIATOR_ADD_CIS,
	RTK_BT_LE_ISO_ACT_CIG_INITIATOR_SET_CIG_PARAM,
	RTK_BT_LE_ISO_ACT_CIG_INITIATOR_SET_CIS_PARAM,
	RTK_BT_LE_ISO_ACT_CIG_INITIATOR_START_SETTING,
	RTK_BT_LE_ISO_ACT_CIG_INITIATOR_SET_CIS_ACL_LINK,
	RTK_BT_LE_ISO_ACT_CIG_INITIATOR_CREATE_CIS_BY_CIG_ID,
	RTK_BT_LE_ISO_ACT_CIG_INITIATOR_CREATE_CIS_BY_CIG_CONN_HANDLE,
	RTK_BT_LE_ISO_ACT_CIG_INITIATOR_REMOVE_CIG,
	RTK_BT_LE_ISO_ACT_CIG_ACCEPTOR_ACCEPT_CIS,
	RTK_BT_LE_ISO_ACT_CIG_ACCEPTOR_REJECT_CIS,
	RTK_BT_LE_ISO_ACT_CIG_ACCEPTOR_CONFIG_SDU_PARAM,
	RTK_BT_LE_ISO_ACT_CIG_ACCEPTOR_CONFIG_CIS_REQ_ACTION,
	RTK_BT_LE_ISO_ACT_CIG_ACCEPTOR_REGISTER_CALLBACK,
	RTK_BT_LE_ISO_ACT_CIG_INITIATOR_REGISTER_CALLBACK,
	RTK_BT_LE_ISO_ACT_CIG_GET_CONN_ID,
	RTK_BT_LE_ISO_ACT_CIG_GET_CIS_INFO,
	RTK_BT_LE_ISO_ACT_CIG_GET_ISOCH_INFO,
	RTK_BT_LE_ISO_ACT_CIG_INITIATOR_GET_CIS_CONN_HANDLE,
	RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_INIT,
	RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_CREATE,
	RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_TERMINATE,
	RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_READ_TX_SYNC,
	RTK_BT_LE_ISO_ACT_BIG_RECEIVER_INIT,
	RTK_BT_LE_ISO_ACT_BIG_RECEIVER_CREATE_SYNC,
	RTK_BT_LE_ISO_ACT_BIG_RECEIVER_TERMINATE_SYNC,
	RTK_BT_LE_ISO_ACT_BIG_RECEIVER_READ_LINK_QUALITY,
	RTK_BT_LE_ISO_ACT_BIG_SETUP_PATH,
	RTK_BT_LE_ISO_ACT_BIG_REMOVE_PATH,
	RTK_BT_LE_ISO_ACT_ISO_DATA_SEND,
	RTK_BT_LE_ISO_ACT_MAX
} rtk_bt_le_iso_act_t;

/**
 * @typedef   rtk_bt_le_iso_evt_code_t
 * @brief     Bluetooth LE ISO management event code definition
*/
typedef enum {
	RTK_BT_LE_ISO_EVT_CIG_DISCONNECT_INFO  = 1,                   /*!< comes when receive disconnection notification */
	RTK_BT_LE_ISO_EVT_CIG_CIS_ESTABLISHED_INFO,                   /*!< comes when receive CIS established indication */
	RTK_BT_LE_ISO_EVT_CIG_ACCEPTOR_REQUEST_CIS_IND,               /*!< comes when receive a request to establish a CIS for acceptor */
	RTK_BT_LE_ISO_EVT_BIG_BROADCASTER_CREATE_CMPL_INFO,           /*!< comes when RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_CREATE is done */
	RTK_BT_LE_ISO_EVT_BIG_BROADCASTER_SYNC_STATE_IND,         	  /*!< comes when broadcaster synchronization state change */
	RTK_BT_LE_ISO_EVT_BIG_RECEIVER_SYNC_ESTABLISHED_INFO,         /*!< comes when RTK_BT_LE_ISO_ACT_BIG_RECEIVER_CREATE_SYNC is done */
	RTK_BT_LE_ISO_EVT_BIG_RECEIVER_SYNC_STATE_IND,         		  /*!< comes when receiver synchronization state change */
	RTK_BT_LE_ISO_EVT_DATA_SEND_DONE,                             /*!< comes when RTK_BT_LE_ISO_ACT_ISO_DATA_SEND is done */
	RTK_BT_LE_ISO_EVT_DATA_RECEIVE_IND,						  	  /*!< comes when receive iso data */
	RTK_BT_LE_ISO_EVT_MAX
} rtk_bt_le_iso_evt_code_t;


/**
 * @typedef   rtk_bt_le_audio_act_t
 * @brief     Bluetooth LE audio action
 */
typedef enum {
	RTK_BT_LE_AUDIO_ACT_ISO_DATA_SEND = 1,
	RTK_BT_LE_AUDIO_ACT_CSIS_GEN_RSI,
	RTK_BT_LE_AUDIO_ACT_CSIS_CHANGE_SIRK,
	RTK_BT_LE_AUDIO_ACT_MCS_CLIENT_SEND_MCP_OP,
	RTK_BT_LE_AUDIO_ACT_MCS_CLIENT_READ_CHAR_VALUE,
	RTK_BT_LE_AUDIO_ACT_MCS_CLIENT_CFG_CCCD,
	RTK_BT_LE_AUDIO_ACT_MCS_MEDIA_PLAYER_INFO_SET,
	RTK_BT_LE_AUDIO_ACT_MCS_MEDIA_TRACK_INFO_SET,
	RTK_BT_LE_AUDIO_ACT_MCS_MEDIA_STATE_UPDATE,
	RTK_BT_LE_AUDIO_ACT_MCS_SET_MCP_SUP_OPCODE,
	RTK_BT_LE_AUDIO_ACT_SYNC_CREATE,
	RTK_BT_LE_AUDIO_ACT_SYNC_RELEASE,
	RTK_BT_LE_AUDIO_ACT_PA_SYNC_ESTABLISH,
	RTK_BT_LE_AUDIO_ACT_PA_SYNC_TERMINATE,
	RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIG_INFO,
	RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_CODEC_CFG,
	RTK_BT_LE_AUDIO_ACT_BIG_SYNC_ESTABLISH,
	RTK_BT_LE_AUDIO_ACT_BIG_SYNC_TERMINATE,
	RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_INFO,
	RTK_BT_LE_AUDIO_ACT_SYNC_SETUP_DATA_PATH,
	RTK_BT_LE_AUDIO_ACT_SYNC_REMOVE_DATA_PATH,
	RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CREATE,
	RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_GET_INFO,
	RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_GET_CONN_HANDLE,
	RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CONFIG,
	RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_ENABLE,
	RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_DISABLE,
	RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RELEASE,
	RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_SETUP_DATA_PATH,
	RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_REMOVE_DATA_PATH,
	RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RECONFIG,
	RTK_BT_LE_AUDIO_ACT_VCS_PARAM_SET,
	RTK_BT_LE_AUDIO_ACT_VCS_PARAM_GET,
	RTK_BT_LE_AUDIO_ACT_VCS_SEND_VOLUME_CP,
	RTK_BT_LE_AUDIO_ACT_VCS_GET_VOLUME_STATE,
	RTK_BT_LE_AUDIO_ACT_MAX,
} rtk_bt_le_audio_act_t;

/**
 * @typedef   rtk_bt_le_audio_evt_t
 * @brief     Bluetooth LE audio event indication
 */
typedef enum {
	RTK_BT_LE_AUDIO_EVT_CODEC_CFG_IND = 1,
	RTK_BT_LE_AUDIO_EVT_QOS_CFG_IND,
	RTK_BT_LE_AUDIO_EVT_ENABLE_IND,
	RTK_BT_LE_AUDIO_EVT_DISABLE_IND,
	RTK_BT_LE_AUDIO_EVT_RELEASE_IND,
	RTK_BT_LE_AUDIO_EVT_ISO_DATA_RECEIVE_IND,
	RTK_BT_LE_AUDIO_EVT_MCS_CLIENT_DISCOVERY_DONE_IND,
	RTK_BT_LE_AUDIO_EVT_MCS_CLIENT_READ_RESULT_IND,
	RTK_BT_LE_AUDIO_EVT_MCS_CLIENT_NOTIFY_RESULT_IND,
	RTK_BT_LE_AUDIO_EVT_PA_SYNC_STATE_IND,
	RTK_BT_LE_AUDIO_EVT_BIG_SYNC_STATE_IND,
	RTK_BT_LE_AUDIO_EVT_PA_BIG_INFO_IND,
	RTK_BT_LE_AUDIO_EVT_PA_ADV_REPORT_IND,
	RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND,
	RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND,
	RTK_BT_LE_AUDIO_EVT_BASS_GET_BROADCAST_CODE_IND,
	RTK_BT_LE_AUDIO_EVT_BASS_GET_PREFER_BIS_SYNC_IND,
	RTK_BT_LE_AUDIO_EVT_BROADCAST_SOURCE_STATE_IND,
	RTK_BT_LE_AUDIO_EVT_MAX,
} rtk_bt_le_audio_evt_t;

/***************************************BR/EDR group************************************/

/**
 * @typedef   gap_uuid_type_t
 * @brief     gap uuid type
 */
typedef enum {
	BR_GAP_UUID16,    /*!< UUID type 16 bits */
	BR_GAP_UUID32,    /*!< UUID type 32 bits */
	BR_GAP_UUID128    /*!< UUID type 128 bits */
} br_gap_uuid_type_t;

/**
 * @typedef   gap_uuid_data_t
 * @brief     gap uuid data
 */
typedef union {
	uint16_t    uuid_16;        /*!< UUID data in 16 bits */
	uint32_t    uuid_32;        /*!< UUID data in 32 bits */
	uint8_t     uuid_128[16];   /*!< UUID data in 128 bits */
} br_gap_uuid_data_t;

/**
 * @typedef   rtk_bt_br_group_t
 * @brief     Bluetooth BR action group
 */
typedef enum {
	RTK_BT_BR_GP_GAP = RTK_BT_API_BR_BASE,
	RTK_BT_BR_GP_AVRCP,                           /*!< BR/EDR avrcp group */
	RTK_BT_BR_GP_A2DP,                            /*!< BR/EDR a2dp group */
	RTK_BT_BR_GP_SPP,                             /*!< BR/EDR spp group */
	RTK_BT_BR_GP_HFP,                             /*!< BR/EDR hfp group */
	RTK_BT_BR_GP_SDP,                             /*!< BR/EDR sdp group */
	RTK_BT_BR_GP_MAX,
} rtk_bt_br_group_t;

/**
 * @typedef   rtk_bt_br_gap_act_t
 * @brief     Bluetooth BR/EDR GAP action
 */
typedef enum {
	RTK_BT_BR_GAP_ACT_SET_DEFAULT_PARAM = 1,
	RTK_BT_BR_GAP_ACT_GET_DEVICE_ADDR,
	RTK_BT_BR_GAP_ACT_SET_DEVICE_NAME,
	RTK_BT_BR_GAP_ACT_SET_PAGE_PARAM,
	RTK_BT_BR_GAP_ACT_SET_INQUIRY_PARAM,
	RTK_BT_BR_GAP_ACT_START_INQUIRY,
	RTK_BT_BR_GAP_ACT_SET_PINCODE,
	RTK_BT_BR_GAP_ACT_SET_COD,
	RTK_BT_BR_GAP_ACT_SET_SUPV_TIMEOUT,
	RTK_BT_BR_GAP_ACT_MAX,
} rtk_bt_br_gap_act_t;

/**
 * @typedef   rtk_bt_br_gap_evt_t
 * @brief     Bluetooth BR/EDR GAP event
 */
typedef enum {
	RTK_BT_BR_GAP_EVT_MAX = 1,
	RTK_BT_BR_GAP_INQUIRY_RESULT,
	RTK_BT_BR_GAP_ACL_CONN_IND,
	RKT_BT_BR_GAP_ACL_CONN_SUCCESS,
	RTK_BT_BR_GAP_ACL_DISCONN,
} rtk_bt_br_gap_evt_t;

/**
 * @typedef   rtk_bt_a2dp_act_t
 * @brief     Bluetooth A2DP action
 */
typedef enum {
	RTK_BT_A2DP_ACT_CODEC_CFG = 1,
	RTK_BT_A2DP_ACT_CONNECT,
	RTK_BT_A2DP_ACT_DISCONNECT,
	RTK_BT_A2DP_ACT_START,
	RTK_BT_A2DP_ACT_SUSPEND,
	RTK_BT_A2DP_ACT_SEND_DATA,
	RTK_BT_A2DP_ACT_MAX,
} rtk_bt_a2dp_act_t;

/**
 * @typedef   rtk_bt_a2dp_evt_t
 * @brief     Bluetooth A2DP event indication
 */
typedef enum {
	RTK_BT_A2DP_EVT_CONN_IND = 1,                  /*!< A2DP connection indication */
	RTK_BT_A2DP_EVT_CONN_CMPL,                     /*!< A2DP connection complete */
	RTK_BT_A2DP_EVT_DISCONN_CMPL,                  /*!< A2DP disconnection complete */
	RTK_BT_A2DP_EVT_CONFIG_CMPL,                   /*!< A2DP configure complete */
	RTK_BT_A2DP_EVT_STREAM_OPEN,                   /*!< A2DP stream open */
	RTK_BT_A2DP_EVT_STREAM_START_IND,              /*!< A2DP stream start indication */
	RTK_BT_A2DP_EVT_STREAM_START_RSP,              /*!< A2DP stream start response */
	RTK_BT_A2DP_EVT_STREAM_STOP,                   /*!< A2DP stream stop */
	RTK_BT_A2DP_EVT_STREAM_CLOSE,                  /*!< A2DP stream close */
	RTK_BT_A2DP_EVT_STREAM_DATA_IND,               /*!< A2DP stream data indication */
	RTK_BT_A2DP_EVT_STREAM_DATA_RSP,               /*!< A2DP stream data recev rsp */
	RTK_BT_A2DP_EVT_SDP_ATTR_INFO,                 /*!< A2DP SRC get sink sdp data info */
	RTK_BT_A2DP_EVT_MAX,
} rtk_bt_a2dp_evt_t;

/**
 * @typedef   rtk_bt_avrcp_act_t
 * @brief     Bluetooth AVRCP action
 */
typedef enum {
	RTK_BT_AVRCP_ACT_MAX = 1,
} rtk_bt_avrcp_act_t;

/**
 * @typedef   rtk_bt_avrcp_evt_t
 * @brief     Bluetooth AVRCP event indication
 */
typedef enum {
	RTK_BT_AVRCP_EVT_CONN_IND = 1,                  /*!< AVRCP connection indication */
	RTK_BT_AVRCP_EVT_ABSOLUTE_VOLUME_SET,           /*!< AVRCP absolute volume set */
	RTK_BT_AVRCP_EVT_VOLUME_CHANGED,                /*!< AVRCP volume changed */
	RTK_BT_AVRCP_EVT_VOLUME_UP,                     /*!< AVRCP volume up */
	RTK_BT_AVRCP_EVT_VOLUME_DOWN,                   /*!< AVRCP volume down */
	RTK_BT_AVRCP_EVT_REG_VOLUME_CHANGED,            /*!< AVRCP volume reg changed */
	RTK_BT_AVRCP_EVT_CONN_CMPL,                     /*!< AVRCP conn completion */
	RTK_BT_AVRCP_EVT_PLAY_STATUS_CHANGED,           /*!< AVRCP plat status changed */
	RTK_BT_AVRCP_EVT_MAX,
} rtk_bt_avrcp_evt_t;

/**
 * @typedef   rtk_bt_spp_act_t
 * @brief     Bluetooth SPP action
 */

typedef enum {
	RTK_BT_SPP_ACT_SRV_CFG = 1,
	RTK_BT_SPP_ACT_CONNECT,
	RTK_BT_SPP_ACT_DISCONNECT,
	RTK_BT_SPP_ACT_DISCONNECT_ALL,
	RTK_BT_SPP_ACT_SEND_DATA,
	RTK_BT_SPP_ACT_GIVE_CREDITS,
	RTK_BT_SPP_ACT_MAX,
} rtk_bt_spp_act_t;

/**
 * @typedef   rtk_bt_spp_evt_t
 * @brief     Bluetooth SPP event indication
 */

typedef enum {
	RTK_BT_SPP_EVT_CONN_COMPL = 1,			/*!< SPP connection establishment completed */
	RTK_BT_SPP_EVT_CREDIT_RCVD,				/*!< SPP link credits received */
	RTK_BT_SPP_EVT_DATA_IND,				/*!< SPP data indication */
	RTK_BT_SPP_EVT_DATA_RSP,				/*!< SPP data response */
	RTK_BT_SPP_EVT_CONN_IND,				/*!< SPP connection indication */
	RTK_BT_SPP_EVT_DISCONN_CMPL,			/*!< SPP disconnection completed */
	RTK_BT_SPP_EVT_SDP_ATTR_INFO,			/*!< SPP get sdp attribute info */
	RTK_BT_SPP_EVT_SDP_DISCOV_CMPL,			/*!< SPP sdp discovery completed */
	RTK_BT_SPP_EVT_MAX,
} rtk_bt_spp_evt_t;

/**
 * @typedef   rtk_bt_sdp_act_t
 * @brief     Bluetooth SDP action
 */
typedef enum {
	RTK_BT_SDP_ACT_RECORD_ADD = 1,
	RTK_BT_SDP_ACT_MAX,
} rtk_bt_sdp_act_t;

/**
 * @typedef   rtk_bt_sdp_evt_t
 * @brief     Bluetooth SDP event indication
 */
typedef enum {
	RTK_BT_SDP_EVT_MAX = 1,
} rtk_bt_sdp_evt_t;

/**
 * @typedef   rtk_bt_hfp_act_t
 * @brief     Bluetooth HFP action
 */
typedef enum {
	RTK_BT_HFP_ACT_CONFIG_HF = 1,
	RTK_BT_HFP_ACT_CONFIG_AG,
	RTK_BT_HFP_ACT_CONNECT,
	RTK_BT_HFP_ACT_DISCONNECT,
	RTK_BT_HFP_ACT_SCO_CONNECT,
	RTK_BT_HFP_ACT_SCO_DISCONNECT,
	RTK_BT_HFP_ACT_CALL_INCOMING,
	RTK_BT_HFP_ACT_CALL_ANSWER,
	RTK_BT_HFP_ACT_CALL_TERMINATE,
	RTK_BT_HFP_ACT_SEND_SCO_DATA,
	RTK_BT_HFP_ACT_MAX,
} rtk_bt_hfp_act_t;

/**
 * @typedef   rtk_bt_hfp_evt_t
 * @brief     Bluetooth HFP event indication
 */
typedef enum {
	RTK_BT_HFP_EVT_SDP_ATTR_INFO = 1,                  /*!< HFP AG get hf sdp data info */
	RTK_BT_HFP_EVT_CONN_IND,                           /*!< HFP connection indication */
	RTK_BT_HFP_EVT_CONN_CMPL,                          /*!< HFP connection completed */
	RTK_BT_HFP_EVT_CALL_STATUS,                        /*!< HFP CALL status indication */
	RTK_BT_HFP_EVT_CALLER_ID_IND,                      /*!< HFP CALLER number indication */
	RTK_BT_HFP_EVT_DISCONN_CMPL,                       /*!< HFP disconnection completed */
	RTK_BT_HFP_EVT_SCO_CONN_IND,                       /*!< HFP sco connection indication */
	RTK_BT_HFP_EVT_SCO_CONN_CMPL,                      /*!< HFP sco connection completed */
	RTK_BT_HFP_EVT_SCO_DATA_IND,                       /*!< HFP sco data indication */
	RTK_BT_HFP_EVT_SCO_DISCONNCTED_IND,                /*!< HFP sco disconnected indication */
	RTK_BT_HFP_EVT_AG_CONN_IND,                        /*!< HFP ag conn indication */
	RTK_BT_HFP_EVT_AG_CONN_CMPL,                       /*!< HFP ag conn complete */
	RTK_BT_HFP_EVT_AG_DISCONN_CMPL,                    /*!< HFP ag disconnection complete */
	RTK_BT_HFP_EVT_AG_INDICATORS_STATUS_REQ,           /*!< HFP ag disconnection complete */
	RTK_BT_HFP_EVT_AG_CURR_CALLS_LIST_QUERY,           /*!< HFP ag current call list query */
	RTK_BT_HFP_EVT_AG_DIAL_LAST_NUMBER,                /*!< HFP ag dial last number */
	RTK_BT_HFP_EVT_AG_CALL_ANSWER_REQ,                 /*!< HFP ag call answer request */
	RTK_BT_HFP_EVT_AG_CALL_TERMINATE_REQ,              /*!< HFP ag call terminate request */
	RTK_BT_HFP_EVT_AG_CALL_STATUS,                     /*!< HFP ag call status incoming */
	RTK_BT_HFP_EVT_MAX,
} rtk_bt_hfp_evt_t;

/***************************************BT gap common group************************************/
/**
 * @typedef   rtk_bt_common_group_t
 * @brief     Bluetooth common action group
 */
typedef enum {
	RTK_BT_COMMON_GP_GAP = RTK_BT_API_COMMON_BASE,
	RTK_BT_COMMON_GP_MAX,
} rtk_bt_group_t;

/**
 * @typedef   rtk_bt_gap_act_t
 * @brief     Bluetooth common GAP action
 */
typedef enum {
	RTK_BT_GAP_ACT_ECFC_SET_PARAM = 1,
	RTK_BT_GAP_ACT_ECFC_SEC_REG_REQ,
	RTK_BT_GAP_ACT_ECFC_REGISTER,
	RTK_BT_GAP_ACT_ECFC_CONN_REQ,
	RTK_BT_GAP_ACT_ECFC_CONN_CFM,
	RTK_BT_GAP_ACT_ECFC_DISCONN_REQ,
	RTK_BT_GAP_ACT_ECFC_SEND_DATA,
	RTK_BT_GAP_ACT_ECFC_RECONF_REQ,
	RTK_BT_GAP_ACT_MAX,
} rtk_bt_gap_act_t;

/**
 * @typedef   rtk_bt_gap_evt_t
 * @brief     Bluetooth common GAP event indication
 */
typedef enum {
	RTK_BT_GAP_EVT_ECFC_CONN_CMPL_IND = 1,            /*!< Indicate ECFC connection establish result, with msg @ref rtk_bt_ecfc_conn_cmpl_ind_t */
	RTK_BT_GAP_EVT_ECFC_DATA_IND,		              /*!< Indicate ECFC data reception, with msg @ref rtk_bt_ecfc_data_ind_t */
	RTK_BT_GAP_EVT_ECFC_CONN_REQ_IND,		          /*!< Indicate ECFC connection request, with msg @ref rtk_bt_ecfc_conn_req_ind_t */
	RTK_BT_GAP_EVT_ECFC_DISCONN_IND,		          /*!< Indicate ECFC disconnect, with msg @ref rtk_bt_ecfc_disconn_ind_t */
	RTK_BT_GAP_EVT_ECFC_RECONF_REQ_IND,			      /*!< Indicate ECFC reconfiguration request, with msg @ref rtk_bt_ecfc_reconf_req_ind_t */
	RTK_BT_GAP_EVT_ECFC_RECONF_RSP_IND,			      /*!< Indicate ECFC reconfiguration response, with msg @ref rtk_bt_ecfc_reconf_rsp_ind_t */
	RTK_BT_GAP_EVT_MAX,
} rtk_bt_gap_evt_t;

/**
 * @enum	rtk_bt_audio_err_common
 * @brief	Bluetooth audio API common err code.
 */
enum rtk_bt_audio_err_common {
	RTK_BT_AUDIO_OK						=	0x00,	/*!< 0, means success */
	RTK_BT_AUDIO_FAIL					=	0x01,	/*!< 1, means common failure */
	RTK_BT_AUDIO_GET_VALUE_FAIL			=	0xFF,	/*!< 0xFF, means get value fail */
};

/**
 * @typedef	rtk_bt_audio_codec_t
 * @brief	Bluetooth audio codec type bit mask
 */
typedef enum {
	RTK_BT_AUDIO_CODEC_SBC			=	BIT0,
	RTK_BT_AUDIO_CODEC_mSBC			=	BIT1,
	RTK_BT_AUDIO_CODEC_AAC			=	BIT2,
	RTK_BT_AUDIO_CODEC_LC3			=	BIT3,
	RTK_BT_AUDIO_CODEC_CVSD			=	BIT4,
} rtk_bt_audio_codec_t;

/**
 * @typedef rtk_bt_evt_cb_ret_t
 * @brief   Bluetooth event callback exucute result
 */
typedef enum {
	RTK_BT_EVT_CB_OK = 0,
	RTK_BT_EVT_CB_ACCEPT = 1,
	RTK_BT_EVT_CB_REJECT = 2,
} rtk_bt_evt_cb_ret_t;

/**
 * @typedef   rtk_bt_evt_cb_t
 * @brief     Bluetooth event callback function type definition
 * @param[in] evt_code: event code
 * @param[in] param: event data
 *
 */
typedef rtk_bt_evt_cb_ret_t (*rtk_bt_evt_cb_t)(uint8_t evt_code, void *data);


/************************** Data structures for API internal use ***********************/
typedef struct {
	uint8_t group;
	uint32_t evt_bit_mask;
} rtk_bt_direct_call_t;

typedef struct {
	struct list_head list;
	uint8_t group;
	uint8_t act;
	uint32_t user_data;
	void *param;
	uint32_t param_len;
	uint16_t ret;
	void *psem;
} rtk_bt_cmd_t;

typedef struct {
	uint8_t group;
	uint8_t evt;
	void *data;
	uint32_t data_len;
	uint8_t data_pool[RTK_EVT_POOL_SIZE];
	void *user_data;
} rtk_bt_evt_t;

struct act_mem_option {
	uint8_t act;
	uint32_t size;
};

struct evt_mem_option {
	uint8_t evt;
	uint32_t size;
};


/********************************* Functions Declaration *******************************/
/**
 * @defgroup  bt_common BT Common APIs
 * @brief     BT Common function APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @fn        void rtk_bt_le_addr_to_str(void *paddr, char *str, uint32_t len)
 * @brief     Convert bt address hexnum to normal format string.
 * @param[in] paddr: Device address
 * @param[out] str: String buf to get the normal format address
 * @param[in] len: Length of string buf
 */
void rtk_bt_le_addr_to_str(void *paddr, char *str, uint32_t len);

/**
 * @fn        void rtk_bt_br_addr_to_str(uint8_t *paddr, char *str, uint32_t len)
 * @brief     Convert bt address hexnum to normal format string.
 * @param[in] paddr: Device address
 * @param[out] str: String buf to get the normal format address
 * @param[in] len: Length of string buf
 */
void rtk_bt_br_addr_to_str(uint8_t *paddr, char *str, uint32_t len);

/**
 * @fn        void rtk_bt_addr_to_str(uint8_t *paddr, char *str, uint32_t len)
 * @brief     Convert bt address hexnum to normal format string.
 * @param[in] type: Device address type
 * @param[in] paddr: Device address
 * @param[out] str: String buf to get the normal format address
 * @param[in] len: Length of string buf
 */
void rtk_bt_addr_to_str(uint8_t type, uint8_t *paddr, char *str, uint32_t len);

/**
 * @fn        uint16_t rtk_bt_evt_register_callback(uint8_t group, rtk_bt_evt_cb_t cb)
 * @brief     Register event callback
 * @note      User's event callback will be excuted in bt_evt_task @ref rtk_bt_evt_taskentry,
 *            if user's callback is complex and consume too much stack size, please enlarge
 * 	          @ref EVENT_TASK_STACK_SIZE
 * @param[in] group: API cmd/evt group
 * @param[in] cb: Callback to be registered to the group
 * @return
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_bt_evt_register_callback(uint8_t group, rtk_bt_evt_cb_t cb);

/**
 * @fn        uint16_t rtk_bt_evt_unregister_callback(uint8_t group)
 * @brief     Unregister event callback
 * @param[in] group: API cmd/evt group
 * @return
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_bt_evt_unregister_callback(uint8_t group);

/**
 * @fn        uint16_t rtk_bt_set_evt_cb_direct_calling(uint8_t group, uint32_t evt_bit_mask)
 * @brief     Set event callback direct calling in stack. Default event callback is excuted in event task.
 *            If it is set direct calling, it will be excuted in stack. This is designed for those event
 *            that will return a result and affect the stack's behaviour.
 * @note      BT api cannot be excuted in callback when this callback event is set direct calling.
 * @param[in] group: API cmd/evt group
 * @param[in] evt_bit_mask: Event bit mask. e.g. 1<<RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND | 1<< RTK_BT_LE_GAP_EVT_XXX
 * @return
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_bt_set_evt_cb_direct_calling(uint8_t group, uint32_t evt_bit_mask);

/**
 * @}
 */

/*********************** Functions for API internal use **********************/
uint16_t rtk_bt_send_cmd(uint8_t group, uint8_t act, void *param, uint32_t param_len);

uint16_t rtk_bt_evt_init(void);

uint16_t rtk_bt_evt_deinit(void);

rtk_bt_evt_t *rtk_bt_event_create(uint8_t group, uint8_t evt, uint32_t param_len);

uint16_t rtk_bt_evt_indicate(void *evt, uint8_t *cb_ret);

void rtk_bt_event_free(rtk_bt_evt_t *pevt);

/* for api log print */
#if 1
#define API_PRINT(...)
#else
#define API_PRINT   printf
#endif

//for debug
#define CONFIG_BT_API_DEBUG

#ifdef CONFIG_BT_API_DEBUG
enum {
	BT_API_ERROR, BT_API_WARNING, BT_API_INFO, BT_API_DEBUG, BT_API_DUMP
};
#define BT_API_DEBUG_LEVEL  BT_API_WARNING
#define BT_API_PRINT(level,...)     \
	do {\
		if (level <= BT_API_DEBUG_LEVEL) {\
			{\
				printf("[BT_API] ");\
				printf(__VA_ARGS__);\
			} \
		}\
	}while(0)
void BT_API_DUMPBUF(uint8_t level, const char *func, uint8_t *buf, uint16_t len);
#else
#define BT_API_PRINT(level,...)  do {} while(0)
#define BT_API_DUMPBUF(level, func, buf, len)  NULL

#endif /* CONFIG_BT_API_DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_COMMON_H__ */
