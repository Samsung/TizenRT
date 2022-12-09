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

#ifdef __cplusplus
extern "C"
{
#endif

#define RTK_EVT_POOL_SIZE 8

#define BT_TIMEOUT_NONE             0
#define BT_API_SYNC_TIMEOUT         1000
#define BT_TIMEOUT_FOREVER          0xffffffffUL

#define RTK_BT_API_LE_BASE          0x0 
#define RTK_BT_API_BR_BASE          0x80
#define RTK_BT_API_TASK_EXIT        0xFF
#define RTK_BT_EVENT_TASK_EXIT      0xFF

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
    RTK_BT_LE_GAP_ACT_SET_PRIVACY,
    RTK_BT_LE_GAP_ACT_SET_PRIVACY_MODE,
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
    RTK_BT_LE_GAP_ACT_SET_EXT_ADV_DATA,
    RTK_BT_LE_GAP_ACT_SET_EXT_SCAN_RSP_DATA,
    RTK_BT_LE_GAP_ACT_START_EXT_ADV,
    RTK_BT_LE_GAP_ACT_STOP_EXT_ADV,
    RTK_BT_LE_GAP_ACT_REMOVE_EXT_ADV,
    RTK_BT_LE_GAP_ACT_EXT_SCAN_SET_PARAM,
    RTK_BT_LE_GAP_ACT_EXT_SCAN_START,
    RTK_BT_LE_GAP_ACT_EXT_SCAN_STOP,
    RTK_BT_LE_GAP_ACT_START_PA,
    RTK_BT_LE_GAP_ACT_MAX,
}rtk_bt_le_gap_act_t;

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
	RTK_BT_LE_GAP_EVT_WHITELIST_MODIFY_IND,         /*!< Indicate LE whitelist modified, with msg @ref rtk_bt_le_wl_modify_ind_t */
	RTK_BT_LE_GAP_EVT_AUTH_PAIRING_CONFIRM_IND,     /*!< Indicate LE just work pairing need to confirm, with msg @ref rtk_bt_le_auth_pair_cfm_ind_t */
	RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_DISPLAY_IND,     /*!< Indicate LE pairing passkey need to display, with msg @ref rtk_bt_le_auth_key_display_ind_t */
	RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_INPUT_IND,       /*!< Indicate LE pairing need to input passkey, with msg @ref rtk_bt_le_auth_key_input_ind_t */
	RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_CONFIRM_IND,     /*!< Indicate LE pairing passkey need to comfirm, with msg @ref rtk_bt_le_auth_key_cfm_ind_t */
	RTK_BT_LE_GAP_EVT_AUTH_OOB_KEY_INPUT_IND,       /*!< Indicate LE pairing need to input oob key, with msg @ref rtk_bt_le_auth_oob_input_ind_t */
	RTK_BT_LE_GAP_EVT_AUTH_COMPLETE_IND,            /*!< Indicate LE auth complete, with msg @ref rtk_bt_le_auth_complete_ind_t */
	RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND,              /*!< Indicate LE bond info modified, with msg @ref rtk_bt_le_bond_modify_ind_t */
	RTK_BT_LE_GAP_EVT_EXT_ADV_IND,                  /*!< Indicate LE ext adv state, with msg @ref rtk_bt_le_ext_adv_ind_t*/
	RTK_BT_LE_GAP_EVT_PA_IND,                 		/*!< Indicate LE periodic adv state, with msg @ref rtk_bt_le_pa_ind_t*/
	RTK_BT_LE_GAP_EVT_EXT_SCAN_RES_IND,             /*!< Indicate LE ext scan data report, with msg @ref rtk_bt_le_ext_scan_res_ind_t*/
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
typedef enum
{
	RTK_BT_GATTS_EVT_REGISTER_SERVICE = 1,      /*!< Indicate register service complete, with msg @ref rtk_bt_gatts_reg_ind_t */
	RTK_BT_GATTS_EVT_MTU_EXCHANGE,              /*!< Indicate mtu exchanged, with msg @ref rtk_bt_gatt_mtu_exchange_ind_t */
	RTK_BT_GATTS_EVT_READ_IND,                  /*!< Indicate remote client read local server, with msg @ref rtk_bt_gatts_read_ind_t */
	RTK_BT_GATTS_EVT_WRITE_IND,                 /*!< Indicate remote client write local server, with msg @ref rtk_bt_gatts_write_ind_t */
	RTK_BT_GATTS_EVT_CCCD_IND,                  /*!< Indicate remote client write the cccd attribute, with msg @ref rtk_bt_gatts_cccd_ind_t */
	RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND,     /*!< Indicate local server indicate remote server complete, with msg @ref rtk_bt_gatts_ntf_and_ind_ind_t */
	RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND,       /*!< Indicate local server notify remote server complete, with msg @ref rtk_bt_gatts_ntf_and_ind_ind_t */
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
	RTK_BT_GATTC_ACT_CCCD_ENABLE,
	RTK_BT_GATTC_ACT_CCCD_DISABLE,
	RTK_BT_GATTC_ACT_DISCOVER,
	RTK_BT_GATTC_ACT_READ,
	RTK_BT_GATTC_ACT_WRITE,
	RTK_BT_GATTC_ACT_CONFIRM,
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
	RTK_BT_GATTC_EVT_MAX,
} rtk_bt_gattc_evt_t;

/***************************************BR/EDR group************************************/

/**
 * @typedef   rtk_bt_le_iso_act_t
 * @brief     Action of BLE ISO Management 
 */
typedef enum {
    RTK_BT_LE_ISO_ACT_CIG_SETUP_PATH,
    RTK_BT_LE_ISO_ACT_CIG_REMOVE_PATH,
    RTK_BT_LE_ISO_ACT_CIG_DISCONNECT,
    RTK_BT_LE_ISO_ACT_CIG_READ_TX_SYNC,
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
    RTK_BT_LE_ISO_ACT_CIG_REGISTER_DIRECT_CALLBACK,
    RTK_BT_LE_ISO_ACT_CIG_ACCEPTOR_REGISTER_CALLBACK,
    RTK_BT_LE_ISO_ACT_CIG_INITIATOR_REGISTER_CALLBACK,
    RTK_BT_LE_ISO_ACT_ISO_DATA_SEND,
    RTK_BT_LE_ISO_ACT_ISO_DATA_CONFIRM,
    RTK_BT_LE_ISO_ACT_CIG_GET_CONN_ID,
    RTK_BT_LE_ISO_ACT_CIG_GET_CIS_INFO,
    RTK_BT_LE_ISO_ACT_CIG_GET_ISOCH_INFO,
    RTK_BT_LE_ISO_ACT_CIG_INITIATOR_GET_CIS_CONN_HANDLE,
} rtk_bt_le_iso_act_t; 


/**
 * @typedef   rtk_bt_br_group_t
 * @brief     Bluetooth BR action group
 */
typedef enum {
	RTK_BT_BR_GP_GAP = RTK_BT_API_BR_BASE,
	RTK_BT_BR_GP_AVRCP,                           /*!< BR/EDR avrcp group */
	RTK_BT_BR_GP_A2DP,                            /*!< BR/EDR a2dp group */
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
    RTK_BT_BR_GAP_ACT_MAX,
}rtk_bt_br_gap_act_t;

/**
 * @typedef   rtk_bt_br_gap_evt_t
 * @brief     Bluetooth BR/EDR GAP event 
 */
typedef enum {
    RTK_BT_BR_GAP_EVT_MAX = 1,
}rtk_bt_br_gap_evt_t;

/**
 * @typedef   rtk_bt_a2dp_act_t
 * @brief     Bluetooth A2DP action
 */
typedef enum {
    RTK_BT_A2DP_ACT_CODEC_CFG = 1,
    RTK_BT_A2DP_ACT_MAX,
} rtk_bt_a2dp_act_t;

/**
 * @typedef   rtk_bt_a2dp_evt_t
 * @brief     Bluetooth A2DP event indication 
 */
typedef enum
{
	RTK_BT_A2DP_EVT_CONN_IND = 1,                  /*!< A2DP connection indication */
	RTK_BT_A2DP_EVT_CONFIG_CMPL,                   /*!< A2DP configure complete */
	RTK_BT_A2DP_EVT_STREAM_OPEN,                   /*!< A2DP stream open */
	RTK_BT_A2DP_EVT_STREAM_START_IND,              /*!< A2DP stream start indication */
	RTK_BT_A2DP_EVT_STREAM_START_RSP,              /*!< A2DP stream start response */
	RTK_BT_A2DP_EVT_STREAM_STOP,                   /*!< A2DP stream stop */
	RTK_BT_A2DP_EVT_STREAM_CLOSE,                  /*!< A2DP stream close */
	RTK_BT_A2DP_EVT_STREAM_DATA_IND,               /*!< A2DP stream data indication */
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
typedef enum
{
    RTK_BT_AVRCP_EVT_CONN_IND = 1,                  /*!< AVRCP connection indication */
	RTK_BT_AVRCP_EVT_VOLUME_CHANGED,                /*!< AVRCP volume changed */
	RTK_BT_AVRCP_EVT_VOLUME_UP,                     /*!< AVRCP volume up */
	RTK_BT_AVRCP_EVT_VOLUME_DOWN,                   /*!< AVRCP volume down */
	RTK_BT_AVRCP_EVT_REG_VOLUME_CHANGED,            /*!< AVRCP volume reg changed */
	RTK_BT_AVRCP_EVT_CONN_CMPL,                     /*!< AVRCP conn completion */
	RTK_BT_AVRCP_EVT_PLAY_STATUS_CHANGED,           /*!< AVRCP plat status changed */
	RTK_BT_AVRCP_EVT_MAX,
} rtk_bt_avrcp_evt_t;

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
typedef enum
{
    RTK_BT_SDP_EVT_MAX = 1,
} rtk_bt_sdp_evt_t;

/**
 * @typedef rtk_bt_app_conf_t
 * @brief   structure of default GAP configuration
 */
typedef struct {
	uint8_t   app_profile_support;      /*!< app profile support @ref rtk_bt_profile_t */
	uint16_t  mtu_size;                 /*!< Deafult MTU config */
	uint8_t   prefer_all_phy;           /*!< Deafult PHY config: preferred all phy */
	uint8_t   prefer_tx_phy;            /*!< Deafult PHY config: preferred tx phy */
	uint8_t   prefer_rx_phy;            /*!< Deafult PHY config: preferred rx phy */
	uint16_t  max_tx_octets;            /*!< Default data len config: Max tx octects */
	uint16_t  max_tx_time;              /*!< Default data len config: Max tx time */
} rtk_bt_app_conf_t;

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
typedef rtk_bt_evt_cb_ret_t (*rtk_bt_evt_cb_t)(uint8_t evt_code, void* data);

/********************* Data structures for API internal use *******************/
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

void rtk_bt_event_free(rtk_bt_evt_t* pevt);

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
	BT_API_ERROR,BT_API_WARNING,BT_API_INFO,BT_API_DEBUG,BT_API_DUMP
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
void BT_API_DUMPBUF(uint8_t level, const char* func, uint8_t * buf, uint16_t len);
#else
#define BT_API_PRINT(level,...)  do {} while(0)
#define BT_API_DUMPBUF(level, func, buf, len)  NULL

#endif /* CONFIG_BT_API_DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_COMMON_H__ */
