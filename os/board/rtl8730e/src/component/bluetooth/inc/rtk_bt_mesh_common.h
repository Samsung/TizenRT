/**
 * @file      rtk_bt_mesh_common.h
 * @author    pico_zhai@realsil.com.cn
 * @brief     Bluetooth LE MESH stack api data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_MESH_COMMON_H__
#define __RTK_BT_MESH_COMMON_H__

#include <stdint.h>

#include <rtk_bt_le_gap.h>
#include <rtk_bt_mesh_def.h>

/**
 * @typedef   rtk_bt_mesh_stack_act_t
 * @brief     BLE MESH stack act definition.
 */
typedef enum {
	RTK_BT_MESH_STACK_ACT_NODE_RESET = 1,
	RTK_BT_MESH_STACK_ACT_PROVISIONER_INIT_SETTING,
	RTK_BT_MESH_STACK_ACT_DEV_INFO_SWITCH,
	RTK_BT_MESH_STACK_ACT_PB_ADV_CON,
	RTK_BT_MESH_STACK_ACT_PB_ADV_DISCON,
	RTK_BT_MESH_STACK_ACT_START_PROV,
	RTK_BT_MESH_STACK_ACT_METHOD_CHOOSE_FOR_PROV,
	RTK_BT_MESH_STACK_ACT_SET_RANDOM_VALUE,
	RTK_BT_MESH_STACK_ACT_GET_HB_SUB_RESULT,
	RTK_BT_MESH_STACK_ACT_SET_MODEL_SUB,
	RTK_BT_MESH_STACK_ACT_PROV_SERVICE_DISCOVERY,
	RTK_BT_MESH_STACK_ACT_PROV_SERVICE_SET_NOTIFY,
	RTK_BT_MESH_STACK_ACT_PROXY_SERVICE_DISCOVERY,
	RTK_BT_MESH_STACK_ACT_PROXY_SERVICE_SET_NOTIFY,
	RTK_BT_MESH_STACK_ACT_USER_LIST_INFO,
	RTK_BT_MESH_STACK_ACT_FN_INIT,
	RTK_BT_MESH_STACK_ACT_FN_DEINIT,
	RTK_BT_MESH_STACK_ACT_LPN_INIT,
	RTK_BT_MESH_STACK_ACT_LPN_REQ,
	RTK_BT_MESH_STACK_ACT_LPN_SUB,
	RTK_BT_MESH_STACK_ACT_LPN_CLEAR,
	RTK_BT_MESH_STACK_ACT_LPN_DEINIT,
	RTK_BT_MESH_STACK_ACT_SEND_ONE_SHOT_ADV,
	RTK_BT_MESH_STACK_ACT_MAX,
} rtk_bt_mesh_stack_act_t;

/**
 * @typedef   rtk_bt_mesh_stack_event_t
 * @brief     BLE MESH stack event definition.
 */
typedef enum {
	RTK_BT_MESH_STACK_EVT_DEVICE_INFO_UDB_DISPLAY = 1,
	RTK_BT_MESH_STACK_EVT_DEVICE_INFO_PROV_DISPLAY,
	RTK_BT_MESH_STACK_EVT_DEVICE_INFO_PROXY_DISPLAY,
	RTK_BT_MESH_STACK_EVT_PB_ADV_LINK_STATE,
	RTK_BT_MESH_STACK_EVT_SET_AUTH_VALUE_FOR_STATIC_OOB,
	RTK_BT_MESH_STACK_EVT_SET_AUTH_VALUE_FOR_OUTPUT_OOB,
	RTK_BT_MESH_STACK_EVT_SET_AUTH_VALUE_FOR_INPUT_OOB,
	RTK_BT_MESH_STACK_EVT_NOTIFY_FOR_OUTPUT_OOB_VALUE,
	RTK_BT_MESH_STACK_EVT_NOTIFY_FOR_INPUT_OOB_VALUE,
	RTK_BT_MESH_STACK_EVT_NOTIFY_CAPABILITY_OF_UDB,
	RTK_BT_MESH_STACK_EVT_PROV_COMPLETE,
	RTK_BT_MESH_STACK_EVT_PROV_FAIL,
	RTK_BT_MESH_STACK_EVT_DEVICE_START_PROVED,
	RTK_BT_MESH_STACK_EVT_PROV_SERVICE_DIS_RESULT,
	RTK_BT_MESH_STACK_EVT_PROXY_SERVICE_DIS_RESULT,
	RTK_BT_MESH_STACK_EVT_UNPROVISIONED_DEVICE,
	RTK_BT_MESH_STACK_EVT_PROVISIONED_DEVICE,
	RTK_BT_MESH_STACK_EVT_SCAN_SWITCH,
	RTK_BT_MESH_STACK_EVT_START_ADV,
	RTK_BT_MESH_STACK_EVT_STOP_ADV,
	RTK_BT_MESH_STACK_EVT_LIST_INFO,
	RTK_BT_MESH_STACK_EVT_FN_CB,
	RTK_BT_MESH_STACK_EVT_LPN_CB,
	RTK_BT_MESH_STACK_EVT_HB_PUB_TIMER_STATE,
	RTK_BT_MESH_STACK_EVT_HB_SUB_TIMER_STATE,
	RTK_BT_MESH_STACK_EVT_HB_PUB_COUNT_UPDATE,
	RTK_BT_MESH_STACK_EVT_HB_SUB_PERIOD_UPDATE,
	RTK_BT_MESH_STACK_EVT_HB_SUB_RECEIVE,
	RTK_BT_MESH_STACK_EVT_MAX,
} rtk_bt_mesh_stack_event_t;

/**
 * @typedef   rtk_bt_mesh_stack_api_result_t
 * @brief     BLE MESH stack api result definition.
 */
typedef enum {
	RTK_BT_MESH_STACK_API_SUCCESS = 0,
	RTK_BT_MESH_STACK_API_FAIL,
} rtk_bt_mesh_stack_api_result_t;

/**
 * @typedef   rtk_bt_mesh_stack_user_list_type_t
 * @brief     BLE MESH stack user list type definition.
 */
typedef enum {
	RTK_BT_MESH_STACK_USER_LIST_MESH_STATE = 1,
	RTK_BT_MESH_STACK_USER_LIST_DEV_UUID,
	RTK_BT_MESH_STACK_USER_LIST_DEV_ADDR,
	RTK_BT_MESH_STACK_USER_LIST_DEV_KEY,
	RTK_BT_MESH_STACK_USER_LIST_NET_KEY,
	RTK_BT_MESH_STACK_USER_LIST_APP_KEY,
	RTK_BT_MESH_STACK_USER_LIST_NORMAL_VALS,
	RTK_BT_MESH_STACK_USER_LIST_ELEMENT_INFO,
	RTK_BT_MESH_STACK_USER_LIST_MODEL_INFO,
	RTK_BT_MESH_STACK_USER_LIST_MODEL_APP_KEY,
	RTK_BT_MESH_STACK_USER_LIST_MODEL_PUB_INFO,
	RTK_BT_MESH_STACK_USER_LIST_MODEL_SUB_INFO,
} rtk_bt_mesh_stack_user_list_type_t;

/**
 * @typedef   rtk_bt_mesh_stack_role_t
 * @brief     BLE MESH device role type definition.
 */
typedef enum {
	RTK_BT_MESH_ROLE_DEVICE,
	RTK_BT_MESH_ROLE_PROVISIONER
} rtk_bt_mesh_stack_role_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_provisioner_init_setting_t
 * @brief     BLE MESH provisioner init setting param structure.
 */
typedef struct {
	uint16_t unicast_addr;
	uint8_t net_key[16];
	uint8_t app_key[16];
} rtk_bt_mesh_stack_act_provisioner_init_setting_t;

/**
 * @typedef   rkt_bt_mesh_stack_act_device_info_set_t
 * @brief     BLE MESH device info display switch param structure.
 */
typedef struct {
	uint8_t dev_info_dis;
} rkt_bt_mesh_stack_act_device_info_set_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_pb_adv_con_t
 * @brief     BLE MESH pbadv link open param structure.
 */
typedef struct {
	uint8_t uuid[16];
} rtk_bt_mesh_stack_act_pb_adv_con_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_start_prov_t
 * @brief     BLE MESH pbadv start provisioning param structure.
 */
typedef struct {
	uint8_t attn_dur;
	bool prov_manual;  // Use the provisioning method whitch device supported
} rtk_bt_mesh_stack_act_start_prov_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_prov_dis_t
 * @brief     BLE MESH prov service discovery param structure.
 */
typedef struct {
	uint16_t conn_handle;
} rtk_bt_mesh_stack_act_prov_dis_t;

/**
 * @typedef   rtk_bt_mesh_stack_prov_dis_result
 * @brief     BLE MESH prov service discovery result type.
 */
typedef enum {
	RTK_BT_MESH_PROV_DISC_DONE,
	RTK_BT_MESH_PROV_DISC_FAIL,
	RTK_BT_MESH_PROV_DISC_NOT_FOUND,
} rtk_bt_mesh_stack_prov_dis_result;

/**
 * @typedef   rtk_bt_mesh_stack_evt_prov_dis_t
 * @brief     BLE MESH prov service discovery event structure.
 */
typedef struct {
	rtk_bt_mesh_stack_prov_dis_result dis_result;
} rtk_bt_mesh_stack_evt_prov_dis_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_prov_notify_enable_t
 * @brief     BLE MESH prov service notify enable param structure.
 */
typedef struct {
	bool is_enable;
} rtk_bt_mesh_stack_act_prov_set_notify_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_proxy_dis_t
 * @brief     BLE MESH proxy service discovery param structure.
 */
typedef struct {
	uint16_t conn_handle;
} rtk_bt_mesh_stack_act_proxy_dis_t;

/**
 * @typedef   rtk_bt_mesh_stack_proxy_dis_result
 * @brief     BLE MESH proxy service discovery result type.
 */
typedef enum {
	RTK_BT_MESH_PROXY_DISC_DONE,
	RTK_BT_MESH_PROXY_DISC_FAIL,
	RTK_BT_MESH_PROXY_DISC_NOT_FOUND,
} rtk_bt_mesh_stack_proxy_dis_result;

/**
 * @typedef   rtk_bt_mesh_stack_evt_proxy_dis_t
 * @brief     BLE MESH proxy service discovery event structure.
 */
typedef struct {
	rtk_bt_mesh_stack_proxy_dis_result dis_result;
} rtk_bt_mesh_stack_evt_proxy_dis_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_proxy_set_notify_t
 * @brief     BLE MESH proxy service notify enable param structure.
 */
typedef struct {
	bool is_enable;
} rtk_bt_mesh_stack_act_proxy_set_notify_t;

/**
 * @typedef   rtk_bt_mesh_stack_evt_scan_switch_t
 * @brief     BLE MESH scan switch event structure.
 */
typedef struct {
	bool scan_enable;
} rtk_bt_mesh_stack_evt_scan_switch_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_send_adv_t
 * @brief     BLE MESH send adv param structure.
 */
typedef struct {
	uint8_t adv_raw_data[RTK_BT_LE_MAX_ADV_DATA_LEN];
	uint8_t adv_data_len;
	rtk_bt_le_adv_type_t adv_type;
	rtk_bt_le_addr_type_t addr_type;  // Addr type for mesh adv
} rtk_bt_mesh_stack_act_send_adv_t;

/**
 * @typedef   rtk_bt_mesh_stack_evt_start_adv_t
 * @brief     BLE MESH start adv event structure.
 */
typedef struct {
	uint16_t result;
} rtk_bt_mesh_stack_evt_start_adv_t;

/**
 * @typedef   rtk_bt_mesh_stack_evt_stop_adv_t
 * @brief     BLE MESH stop adv event structure.
 */
typedef struct {
	uint16_t result;
} rtk_bt_mesh_stack_evt_stop_adv_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_fn_init_t
 * @brief     BLE MESH friend node init param structure.
 */
typedef struct {
	uint8_t lpn_num;
	uint8_t queue_size;
	uint8_t rx_window;
} rtk_bt_mesh_stack_act_fn_init_t;

/**
 * @typedef   rtk_bt_mesh_stack_evt_fn_cb_type
 * @brief     BLE MESH friendship FN event cb type.
 */
typedef enum {
	RTK_BT_MESH_FN_CB_TYPE_ESTABLISHING,
	RTK_BT_MESH_FN_CB_TYPE_ESTABLISH_FAIL_NO_POLL,
	RTK_BT_MESH_FN_CB_TYPE_ESTABLISH_SUCCESS,
	RTK_BT_MESH_FN_CB_TYPE_FRND_LOST,
} rtk_bt_mesh_stack_evt_fn_cb_type;

/**
 * @typedef   rtk_bt_mesh_stack_evt_fn_t
 * @brief     BLE MESH friendship FN event structure.
 */
typedef struct {
	rtk_bt_mesh_stack_evt_fn_cb_type cb_type;
	uint16_t lpn_addr;
} rtk_bt_mesh_stack_evt_fn_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_lpn_init_t
 * @brief     BLE MESH low pwer node init param structure.
 */
typedef struct {
	uint8_t fn_num;
} rtk_bt_mesh_stack_act_lpn_init_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_lpn_req_t
 * @brief     BLE MESH low pwer node request a friendship param structure.
 */
typedef struct {
	uint8_t fn_index;
	uint16_t net_key_index;
	uint32_t poll_interval;
	uint32_t poll_timeout;
	uint8_t frnd_rx_delay;
	uint8_t frnd_rx_widen;
} rtk_bt_mesh_stack_act_lpn_req_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_lpn_sub_t
 * @brief     BLE MESH low pwer node subscribe add or remove param structure.
 */
typedef struct {
	uint8_t fn_index;
	uint16_t addr;
	bool add_rm;
} rtk_bt_mesh_stack_act_lpn_sub_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_lpn_clear_t
 * @brief     BLE MESH low pwer node clear friendship param structure.
 */
typedef struct {
	uint8_t fn_index;
} rtk_bt_mesh_stack_act_lpn_clear_t;

/**
 * @typedef   rtk_bt_mesh_stack_evt_fn_cb_type
 * @brief     BLE MESH friendship FN event cb type.
 */
typedef enum {
	RTK_BT_MESH_LPN_REQ_REASON_SUCCESS,
	RTK_BT_MESH_LPN_REQ_REASON_REQ_PARAMS_INVALID,
	RTK_BT_MESH_LPN_REQ_REASON_NET_KEY_INDEX_INVALID,
	RTK_BT_MESH_LPN_REQ_REASON_FRND_ON_SAME_NET_KEY_INDEX,
	RTK_BT_MESH_LPN_REQ_REASON_FRND_INDEX_INVALID,
	RTK_BT_MESH_LPN_REQ_REASON_FRND_ALREADY_EXIST_OR_ESTABLISING,
	RTK_BT_MESH_LPN_REQ_REASON_RESOURCE_INSUFFICIENT,
	RTK_BT_MESH_LPN_REQ_REASON_PROXY_SERVER_DO_NOT_SUPPORT_LPN,
} rtk_bt_mesh_stack_lpn_req_result_type;

/**
 * @typedef   rtk_bt_mesh_stack_evt_lpn_cb_type
 * @brief     BLE MESH friendship LPN event cb type.
 */
typedef enum {
	RTK_BT_MESH_LPN_CB_TYPE_ESTABLISH_SUCCESS,
	RTK_BT_MESH_LPN_CB_TYPE_ESTABLISH_FAIL_NO_OFFER,
	RTK_BT_MESH_LPN_CB_TYPE_ESTABLISH_FAIL_NO_UPDATE,
	RTK_BT_MESH_LPN_CB_TYPE_FRIENDSHIP_LOST,
	RTK_BT_MESH_LPN_CB_TYPE_UNKNOWN,
} rtk_bt_mesh_stack_evt_lpn_cb_type;

/**
 * @typedef   rtk_bt_mesh_stack_evt_lpn_t
 * @brief     BLE MESH friendship LPN event structure.
 */
typedef struct {
	rtk_bt_mesh_stack_evt_lpn_cb_type cb_type;
	uint16_t fn_addr;
} rtk_bt_mesh_stack_evt_lpn_t;

/**
 * @typedef   rtk_bt_mesh_stack_prov_cb_fail_type_t
 * @brief     BLE MESH prov fail reason type.
 */
typedef enum {
	RTK_BT_MESH_PROV_CB_FAIL_TYPE_BEARER_LOSS, //!< pb-adv link close & le link disconnect
	RTK_BT_MESH_PROV_CB_FAIL_TYPE_BEARER_TIMEOUT, //!< pb-adv transaction timeout @ref MESH_PB_GENERIC_TIMEOUT_PERIOD
	RTK_BT_MESH_PROV_CB_FAIL_TYPE_BEARER_IDLE, //!< pb-adv link idle timeout @ref MESH_PB_ADV_LINK_IDLE_TIMEOUT
	RTK_BT_MESH_PROV_CB_FAIL_TYPE_PROCEDURE_TIMEOUT, //!< prov procedure timeout @ref MESH_PROV_TIMEOUT_PERIOD
	RTK_BT_MESH_PROV_CB_FAIL_TYPE_PROCEDURE_ABNORMAL //!< prov procedure fail @ref prov_result_t
} rtk_bt_mesh_stack_prov_cb_fail_type_t;

/**
 * @typedef   rtk_bt_mesh_stack_evt_prov_fail_t
 * @brief     BLE MESH provisioning fail reason structure.
 */
typedef struct {
	rtk_bt_mesh_stack_prov_cb_fail_type_t fail_reason;
} rtk_bt_mesh_stack_evt_prov_fail_t;

/**
 * @typedef   rtk_bt_mesh_stack_evt_provisioned_device_t
 * @brief     BLE MESH provisioned device info structure.
 */
typedef struct {
	uint16_t unicast_addr;
} rtk_bt_mesh_stack_evt_provisioned_device_t;

/**
 * @typedef   rtk_bt_mesh_stack_device_info_common_data
 * @brief     BLE MESH device beacon common info structure.
 */
typedef struct {
	uint8_t bt_addr[6];
	uint8_t bt_addr_type;
	int8_t rssi;
} rtk_bt_mesh_stack_device_info_common_data;

/**
 * @typedef   rtk_bt_mesh_stack_evt_dev_info_udb_t
 * @brief     BLE MESH device unprovisioned device beacon info structure.
 */
typedef struct {
	rtk_bt_mesh_stack_device_info_common_data dev_info;
	uint8_t dev_uuid[16];
	uint8_t oob_info[2];
	uint8_t uri_hash[4];
} rtk_bt_mesh_stack_evt_dev_info_udb_t;

/**
 * @typedef   rtk_bt_mesh_stack_evt_prov_complete_t
 * @brief     BLE MESH device provisioning complete info structure.
 */
typedef struct {
	uint16_t unicast_addr;
} rtk_bt_mesh_stack_evt_prov_complete_t;

/**
 * @typedef   rtk_bt_mesh_stack_prov_oob_info_t
 * @brief     BLE MESH device oob info structure.
 */
typedef struct {
	uint16_t other: 1;
	uint16_t electronic_uri: 1;
	uint16_t qr_code: 1; //!< 2D machine-readable code
	uint16_t bar_code: 1;
	uint16_t nfc: 1; //!< Near Field Communication (NFC)
	uint16_t number: 1;
	uint16_t string: 1;
	uint16_t rfu: 4;
	uint16_t on_box: 1;
	uint16_t inside_box: 1;
	uint16_t on_paper: 1;
	uint16_t inside_manual: 1;
	uint16_t on_device: 1;
} rtk_bt_mesh_stack_prov_oob_info_t;

/**
 * @typedef   rtk_bt_mesh_stack_evt_dev_info_provision_adv_t
 * @brief     BLE MESH device provisioin adv info structure.
 */
typedef struct {
	rtk_bt_mesh_stack_device_info_common_data dev_info;
	uint8_t dev_uuid[16];
	rtk_bt_mesh_stack_prov_oob_info_t oob_info;
} rtk_bt_mesh_stack_evt_dev_info_provision_adv_t;

/**
 * @typedef   proxy_adv_type_t
 * @brief     BLE MESH device proxy adv info structure.
 */
typedef uint8_t proxy_adv_type_t;

/**
 * @typedef   rtk_bt_mesh_stack_mesh_service_data_proxy_t
 * @brief     BLE MESH device service data info structure.
 */
typedef struct {
	proxy_adv_type_t type;
	union {
		uint8_t net_id[8];
		struct {
			uint8_t hash[8];
			uint8_t random[8];
		};
	};
} rtk_bt_mesh_stack_mesh_service_data_proxy_t;

/**
 * @typedef   rtk_bt_mesh_stack_evt_dev_info_proxy_adv_t
 * @brief     BLE MESH device proxy data info structure.
 */
typedef struct {
	rtk_bt_mesh_stack_device_info_common_data dev_info;
	uint16_t len;
	rtk_bt_mesh_stack_mesh_service_data_proxy_t proxy;
} rtk_bt_mesh_stack_evt_dev_info_proxy_adv_t;

/**
 * @typedef   rtk_bt_mesh_stack_hb_timer_state_t
 * @brief     BLE MESH heartbeat timer state date structure.
 */
typedef enum
{
    RTK_BT_MESH_STACK_HB_TIMER_STATE_START,
    RTK_BT_MESH_STACK_HB_TIMER_STATE_STOP
} rtk_bt_mesh_stack_hb_timer_state_t;

/**
 * @typedef   rtk_bt_mesh_stack_hb_data_timer_state_t
 * @brief     BLE MESH heartbeat data timer state date structure.
 */
typedef struct {
	rtk_bt_mesh_stack_hb_timer_state_t timer_state;
	uint16_t period;
} rtk_bt_mesh_stack_hb_data_timer_state_t;

/**
 * @typedef   rtk_bt_mesh_stack_hb_pub_count_update_t
 * @brief     BLE MESH heartbeat publish count update date structure.
 */
typedef struct {
	uint16_t count;
} rtk_bt_mesh_stack_hb_pub_count_update_t;

/**
 * @typedef   rtk_bt_mesh_stack_hb_sub_period_update_t
 * @brief     BLE MESH heartbeat subscription period update date structure.
 */
typedef struct {
	uint16_t period;
} rtk_bt_mesh_stack_hb_sub_period_update_t;

/**
 * @typedef   rtk_bt_mesh_stack_hb_pub_features_t
 * @brief     BLE MESH heartbeat publish device feature date structure.
 */
typedef union
{
    uint16_t raw;
    struct
    {
        uint16_t relay: 1;
        uint16_t proxy: 1;
        uint16_t frnd: 1;
        uint16_t lpn: 1;
        uint16_t rfu: 12;
    };
} _PACKED4_ rtk_bt_mesh_stack_hb_pub_features_t;

/**
 * @typedef   rtk_bt_mesh_stack_hb_data_sub_receive_t
 * @brief     BLE MESH heartbeat message date structure.
 */
typedef struct
{
    uint16_t src;
    uint8_t init_ttl;
    rtk_bt_mesh_stack_hb_pub_features_t features;
    uint8_t ttl;
} rtk_bt_mesh_stack_hb_data_sub_receive_t;

/**
 * @typedef   rtk_bt_mesh_hb_sub_t
 * @brief     BLE MESH heartbeat subscription result date structure.
 */
typedef struct
{
    /**
     * @brief source address for heartbeat messages a node shall process
     */
    uint16_t src;
    /**
     * @brief determines the destination address for heartbeat messages
     */
    uint16_t dst;
    /**
     * @brief preiod for processing periodical heartbeat transport control messages
     * 0: heartbeat messages are not being processed
     * 1-0xffff: remaining period in seconds for processing periodical heartbeat message
     */
    uint16_t period;
    /**
     * @brief number of heartbeat messages that received
     * 0-0xfffe: number of heartbeat messages received
     * 0xffff: more than 0xfffe messages have been received
     */
    uint16_t count;
    /**
     * @brief minimum hops in received heartbeat messages
     */
    uint8_t min_hops;
    /**
     * @brief maximum hops in received heartbeat messages
     */
    uint8_t max_hops;
} rtk_bt_mesh_hb_sub_t;

/**
 * @typedef   rtk_bt_mesh_set_model_subscribe_t
 * @brief     BLE MESH set model subscribe date structure.
 */
typedef struct
{
    uint8_t element_index;
	uint32_t model_id;
	uint16_t sub_addr;
} rtk_bt_mesh_set_model_subscribe_t;

/**
 * @typedef   rtk_bt_mesh_prov_generic_cb_type_t
 * @brief     BLE MESH PB ADV link state @ref RTK_BT_MESH_STACK_EVT_PB_ADV_LINK_STATE.
 */
typedef enum {
	RTK_BT_MESH_PB_GENERIC_CB_LINK_OPENED,
	RTK_BT_MESH_PB_GENERIC_CB_LINK_OPEN_FAILED,
	RTK_BT_MESH_PB_GENERIC_CB_LINK_CLOSED,
	RTK_BT_MESH_PB_GENERIC_CB_MSG_ACKED,
	RTK_BT_MESH_PB_GENERIC_CB_MSG,
	//todo: RTK_BT_MESH_PB_GENERIC_CB_MSG_TRANSMITED maybe usefull calc ecdh secret after send public key to save time
} rtk_bt_mesh_prov_generic_cb_type_t;

/**
 * @typedef   rtk_bt_mesh_stack_act_set_random_value_t
 * @brief     BLE MESH set random value data structure for oob in Authentication process of provisioning.
 */
typedef struct {
	uint32_t random;
} rtk_bt_mesh_stack_act_set_random_value_t;

/**
 * @typedef   rtk_bt_mesh_stack_set_auth_value_for_static_oob
 * @brief     BLE MESH set auth value event for static oob in Authentication process when provisioning.
 */
typedef struct {
	bool status;
	uint8_t data[16];
} rtk_bt_mesh_stack_set_auth_value_for_static_oob;

/**
 * @typedef   rtk_bt_mesh_stack_set_auth_value_for_oob_data
 * @brief     BLE MESH set auth value event for output or input oob in Authentication process when provisioning.
 */
typedef struct {
	bool status;
	uint32_t random;
} rtk_bt_mesh_stack_set_auth_value_for_oob_data;

/**
 * @typedef   rtk_bt_mesh_stack_prov_cap_algorithm_t
 * @brief     BLE MESH Algorithm of provisioning method unprovisioned device supported when provisioning.
 */
typedef enum
{
    RTK_BT_MESH_PROV_CAP_ALGO_FIPS_P256_ELLIPTIC_CURVE = BIT0
} rtk_bt_mesh_stack_prov_cap_algorithm_t;

/**
 * @typedef   rtk_bt_mesh_stack_prov_cap_public_key_t
 * @brief     BLE MESH Public Key Type unprovisioned device supported when provisioning.
 */
typedef enum
{
    RTK_BT_MESH_PROV_CAP_PUBLIC_KEY_OOB = BIT0
} rtk_bt_mesh_stack_prov_cap_public_key_t;

/**
 * @typedef   rtk_bt_mesh_stack_prov_cap_static_oob_t
 * @brief     BLE MESH Static OOB Type unprovisioned device supported when provisioning.
 */
typedef enum
{
    RTK_BT_MESH_PROV_CAP_NOT_SUPPORT_STATIC_OOB = 0,
    RTK_BT_MESH_PROV_CAP_SUPPORT_STATIC_OOB = BIT0
} rtk_bt_mesh_stack_prov_cap_static_oob_t;

/**
 * @typedef   rtk_bt_mesh_stack_prov_cap_output_oob_action_t
 * @brief     BLE MESH Output OOB Action unprovisioned device supported when provisioning.
 */
typedef enum
{
    RTK_BT_MESH_PROV_CAP_OUTPUT_OOB_ACTION_NOT_ENABLE = 0,
    RTK_BT_MESH_PROV_CAP_OUTPUT_OOB_ACTION_BLINK = BIT0,
    RTK_BT_MESH_PROV_CAP_OUTPUT_OOB_ACTION_BEEP = BIT1,
    RTK_BT_MESH_PROV_CAP_OUTPUT_OOB_ACTION_VIBRATE = BIT2,
    RTK_BT_MESH_PROV_CAP_OUTPUT_OOB_ACTION_OUTPUT_NUMERIC = BIT3,
    RTK_BT_MESH_PROV_CAP_OUTPUT_OOB_ACTION_OUTPUT_ALPHANUMERIC = BIT4
} rtk_bt_mesh_stack_prov_cap_output_oob_action_t;

/**
 * @typedef   rtk_bt_mesh_stack_prov_cap_input_oob_action_t
 * @brief     BLE MESH Input OOB Actions unprovisioned device supported when provisioning.
 */
typedef enum
{
    RTK_BT_MESH_PROV_CAP_INPUT_OOB_ACTION_NOT_ENABLE = 0,
    RTK_BT_MESH_PROV_CAP_INPUT_OOB_ACTION_BIT_PUSH = BIT0,
    RTK_BT_MESH_PROV_CAP_INPUT_OOB_ACTION_BIT_TWIST = BIT1,
    RTK_BT_MESH_PROV_CAP_INPUT_OOB_ACTION_BIT_INPUT_NUMERIC = BIT2,
    RTK_BT_MESH_PROV_CAP_INPUT_OOB_ACTION_BIT_INPUT_ALPHANUMERIC = BIT3
} rtk_bt_mesh_stack_prov_cap_input_oob_action_t;

/**
 * @typedef   rtk_bt_mesh_stack_prov_capabilities_t
 * @brief     BLE MESH prov capability of unprovisioned device when provisioning.
 */
typedef struct
{
    uint8_t element_num; //  element number of unprovisioned device
    uint16_t algorithm; //!< @ref rtk_bt_mesh_stack_prov_cap_algorithm_t
    uint8_t public_key; //!< @ref rtk_bt_mesh_stack_prov_cap_public_key_t
    uint8_t static_oob; //!< @ref rtk_bt_mesh_stack_prov_cap_static_oob_t
    uint8_t output_oob_size;
    uint16_t output_oob_action; //!< @ref rtk_bt_mesh_stack_prov_cap_output_oob_action_t
    uint8_t input_oob_size;
    uint16_t input_oob_action; //!< @ref rtk_bt_mesh_stack_prov_cap_input_oob_action_t
} _PACKED4_ rtk_bt_mesh_stack_prov_capabilities_t;

/**
 * @typedef   rtk_bt_mesh_prov_start_algorithm_t
 * @brief     BLE MESH Algorithm values used for provisioner when provisioning.
 */
typedef enum
{
    RTK_BT_MESH_PROV_START_FIPS_P256_ELLIPTIC_CURVE
} rtk_bt_mesh_prov_start_algorithm_t;

/**
 * @typedef   rtk_bt_mesh_prov_start_public_key_t
 * @brief     BLE MESH Public Key values used for provisioner when provisioning.
 */
typedef enum
{
    RTK_BT_MESH_PROV_START_NO_OOB_PUBLIC_KEY,
    RTK_BT_MESH_PROV_START_OOB_PUBLIC_KEY
} rtk_bt_mesh_prov_start_public_key_t;

/**
 * @typedef   rtk_bt_mesh_prov_auth_method_t
 * @brief     BLE MESH prov auth method for Authentication used for provisioner when provisioning.
 */
typedef enum
{
    RTK_BT_MESH_PROV_AUTH_METHOD_NO_OOB,
    RTK_BT_MESH_PROV_AUTH_METHOD_STATIC_OOB,
    RTK_BT_MESH_PROV_AUTH_METHOD_OUTPUT_OOB,
    RTK_BT_MESH_PROV_AUTH_METHOD_INPUT_OOB
} rtk_bt_mesh_prov_auth_method_t;

/**
 * @typedef   rtk_bt_mesh_prov_start_output_oob_action_t
 * @brief     BLE MESH prov Output OOB Action values used for provisioner when provisioning.
 */
typedef enum
{
    RTK_BT_MESH_PROV_START_OUTPUT_OOB_ACTION_BLINK,
    RTK_BT_MESH_PROV_START_OUTPUT_OOB_ACTION_BEEP,
    RTK_BT_MESH_PROV_START_OUTPUT_OOB_ACTION_VIBRATE,
    RTK_BT_MESH_PROV_START_OUTPUT_OOB_ACTION_OUTPUT_NUMERIC,
    RTK_BT_MESH_PROV_START_OUTPUT_OOB_ACTION_OUTPUT_ALPHANUMERIC
} rtk_bt_mesh_prov_start_output_oob_action_t;

/**
 * @typedef   rtk_bt_mesh_prov_start_input_oob_action_t
 * @brief     BLE MESH prov Input OOB Actions used for provisioner when provisioning.
 */
typedef enum
{
    RTK_BT_MESH_PROV_START_INPUT_OOB_ACTION_PUSH,
    RTK_BT_MESH_PROV_START_INPUT_OOB_ACTION_TWIST,
    RTK_BT_MESH_PROV_START_INPUT_OOB_ACTION_INPUT_NUMERIC,
    RTK_BT_MESH_PROV_START_INPUT_OOB_ACTION_INPUT_ALPHANUMERIC
} rtk_bt_mesh_prov_start_input_oob_action_t;

/**
 * @typedef   rtk_bt_mesh_prov_auth_action_t
 * @brief     BLE MESH prov auth action values used for provisioner when provisioning.
 */
typedef union
{
    uint8_t oob_action;
    rtk_bt_mesh_prov_start_output_oob_action_t output_oob_action;
    rtk_bt_mesh_prov_start_input_oob_action_t input_oob_action;
} _PACKED4_ rtk_bt_mesh_prov_auth_action_t;

/**
 * @typedef   rtk_bt_mesh_prov_auth_size_t
 * @brief     BLE MESH prov auth action size used for provisioner when provisioning.
 */
typedef union
{
    uint8_t oob_size;
    uint8_t output_oob_size;
    uint8_t input_oob_size;
} _PACKED4_ rtk_bt_mesh_prov_auth_size_t;

/**
 * @typedef   rtk_bt_mesh_stack_prov_start_t
 * @brief     BLE MESH provisioning method used for provisioner when provisioning.
 */
typedef struct
{
    rtk_bt_mesh_prov_start_algorithm_t algorithm;
    rtk_bt_mesh_prov_start_public_key_t public_key;
    rtk_bt_mesh_prov_auth_method_t auth_method;
    rtk_bt_mesh_prov_auth_action_t auth_action;
    rtk_bt_mesh_prov_auth_size_t auth_size;
} _PACKED4_ rtk_bt_mesh_stack_prov_start_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  ble_mesh_common BT LE Mesh Common APIs
 * @brief     BT LE Mesh Common related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */

#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT

/**
 * @fn        uint16_t rtk_bt_mesh_stack_provisioner_setting_init(rtk_bt_mesh_stack_act_provisioner_init_setting_t *init_setting)
 * @brief     Setting provisioner init param, will cause event @ref RTK_BT_MESH_STACK_ACT_PROVISIONER_INIT_SETTING
 * @param[in] init_setting: init setting param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_provisioner_setting_init(rtk_bt_mesh_stack_act_provisioner_init_setting_t *init_setting);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_prov_pb_adv_con(rtk_bt_mesh_stack_act_pb_adv_con_t *pbadvcon)
 * @brief     Establish PBADV link, will cause event @ref RTK_BT_MESH_STACK_ACT_PB_ADV_CON
 * @param[in] pbadvcon: PB-ADV link param struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_prov_pb_adv_con(rtk_bt_mesh_stack_act_pb_adv_con_t *pbadvcon);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_prov_pb_adv_discon(void)
 * @brief     Disconnect PBADV link, will cause event @ref RTK_BT_MESH_STACK_ACT_PB_ADV_DISCON
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_prov_pb_adv_discon(void);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_start_provisioning(rtk_bt_mesh_stack_act_start_prov_t *start_prov)
 * @brief     Start provisioning, will cause event @ref RTK_BT_MESH_STACK_ACT_START_PROV
 * @param[in] start_prov: start provisioning param struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_start_provisioning(rtk_bt_mesh_stack_act_start_prov_t *start_prov);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_method_choose_for_provisioning(rtk_bt_mesh_stack_prov_start_t *method)
 * @brief     Select a method according unprovisioned device support for provisioning, will cause event @ref RTK_BT_MESH_STACK_ACT_METHOD_CHOOSE_FOR_PROV
 * @param[in] method: provisioning method data param struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_method_choose_for_provisioning(rtk_bt_mesh_stack_prov_start_t *method);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_prov_service_discovery(rtk_bt_mesh_stack_act_prov_dis_t *prov_dis)
 * @brief     Prov service discovery, will cause event @ref RTK_BT_MESH_STACK_ACT_PROV_SERVICE_DISCOVERY
 * @param[in] prov_dis : prov service discovery param struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_prov_service_discovery(rtk_bt_mesh_stack_act_prov_dis_t *prov_dis);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_prov_service_set_notify(rtk_bt_mesh_stack_act_prov_set_notify_t *prov_notify)
 * @brief     Prov service set notify, will cause event @ref RTK_BT_MESH_STACK_ACT_PROV_SERVICE_SET_NOTIFY
 * @param[in] prov_notify : prov service notify set param struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_prov_service_set_notify(rtk_bt_mesh_stack_act_prov_set_notify_t *prov_notify);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_proxy_service_discovery(rtk_bt_mesh_stack_act_proxy_dis_t *proxy_dis)
 * @brief     Proxy service discovery, will cause event @ref RTK_BT_MESH_STACK_ACT_PROXY_SERVICE_DISCOVERY
 * @param[in] proxy_dis : proxy service discovery param struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_proxy_service_discovery(rtk_bt_mesh_stack_act_proxy_dis_t *proxy_dis);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_proxy_service_set_notify(rtk_bt_mesh_stack_act_proxy_set_notify_t *proxy_notify)
 * @brief     Proxy service set notify, will cause event @ref RTK_BT_MESH_STACK_ACT_PROXY_SERVICE_SET_NOTIFY
 * @param[in] proxy_notify : proxy service notify set param struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_proxy_service_set_notify(rtk_bt_mesh_stack_act_proxy_set_notify_t *proxy_notify);

#endif // end of RTK_BLE_MESH_PROVISIONER_SUPPORT

#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT

/**
 * @fn        uint16_t rtk_bt_mesh_stack_lpn_init(rtk_bt_mesh_stack_act_lpn_init_t *lpn_init)
 * @brief     Init low power node, will cause event @ref RTK_BT_MESH_STACK_ACT_LPN_INIT
 * @param[in] lpn_init : low power node init param struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_lpn_init(rtk_bt_mesh_stack_act_lpn_init_t *lpn_init);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_lpn_req(rtk_bt_mesh_stack_act_lpn_req_t *lpn_req)
 * @brief     Low power node request a friendship to friend node, will cause event @ref RTK_BT_MESH_STACK_ACT_LPN_REQ
 * @param[in] lpn_req : low power node request friendship param struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_lpn_req(rtk_bt_mesh_stack_act_lpn_req_t *lpn_req);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_lpn_sub(rtk_bt_mesh_stack_act_lpn_sub_t *lpn_sub)
 * @brief     Low power node add or remove subscribe address, will cause event @ref RTK_BT_MESH_STACK_ACT_LPN_SUB
 * @param[in] lpn_sub : low power node subscribe add or remove param struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_lpn_sub(rtk_bt_mesh_stack_act_lpn_sub_t *lpn_sub);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_lpn_clear(rtk_bt_mesh_stack_act_lpn_clear_t *lpn_clear)
 * @brief     Low power node clear friendship, will cause event @ref RTK_BT_MESH_STACK_ACT_LPN_CLEAR
 * @param[in] lpn_clear : low power node clear friendship data param struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_lpn_clear(rtk_bt_mesh_stack_act_lpn_clear_t *lpn_clear);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_lpn_deinit(void)
 * @brief     Deinit low power node, will cause event @ref RTK_BT_MESH_STACK_ACT_LPN_DEINIT
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_lpn_deinit(void);

#endif // end of RTK_BLE_MESH_DEVICE_SUPPORT

/**
 * @fn        uint16_t rtk_bt_mesh_stack_node_reset(void)
 * @brief     Node reset, clear the mesh param, will cause event @ref RTK_BT_MESH_STACK_ACT_NODE_RESET
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_node_reset(void);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_set_device_info_display(rkt_bt_mesh_stack_act_device_info_set_t *p_data)
 * @brief     Device info display switch, will cause event @ref RTK_BT_MESH_STACK_ACT_DEV_INFO_SWITCH
 * @param[in] p_data: device info display data struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_set_device_info_display(rkt_bt_mesh_stack_act_device_info_set_t *p_data);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_user_list_info(void)
 * @brief     Print user list info, will cause event @ref RTK_BT_MESH_STACK_ACT_USER_LIST_INFO
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_user_list_info(void);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_send_one_shot_adv(rtk_bt_mesh_stack_act_send_adv_t *adv_param)
 * @brief     Only send one mesh adv, will cause event @ref RTK_BT_MESH_STACK_ACT_SEND_ONE_SHOT_ADV
 * @param[in] adv_param: adv param data structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_send_one_shot_adv(rtk_bt_mesh_stack_act_send_adv_t *adv_param);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_set_random_value_for_authentication(rtk_bt_mesh_stack_act_set_random_value_t *value)
 * @brief     Set random value when use output or input oob method for Authentication for provisioning, will cause event @ref RTK_BT_MESH_STACK_ACT_SET_RANDOM_VALUE
 * @param[in] value: the random value data structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_set_random_value_for_authentication(rtk_bt_mesh_stack_act_set_random_value_t *value);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_get_heartbeat_subscribe_result(rtk_bt_mesh_hb_sub_t *hb_sub)
 * @brief     Get heartbeat subscribe result, will cause event @ref RTK_BT_MESH_STACK_ACT_GET_HB_SUB_RESULT
 * @param[in] value: the subscribe result data structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_get_heartbeat_subscribe_result(rtk_bt_mesh_hb_sub_t *hb_sub);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_set_model_subscribe(rtk_bt_mesh_set_model_subscribe_t *model_sub)
 * @brief     Set model subscribe, will cause event @ref RTK_BT_MESH_STACK_ACT_SET_MODEL_SUB
 * @param[in] model_sub: the set model subscribe data structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_set_model_subscribe(rtk_bt_mesh_set_model_subscribe_t *model_sub);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_fn_init(rtk_bt_mesh_stack_act_fn_init_t *friend_init)
 * @brief     Init friendship FN node, will cause event @ref RTK_BT_MESH_STACK_ACT_FN_INIT
 * @param[in] friend_init: friend node init data struct
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_fn_init(rtk_bt_mesh_stack_act_fn_init_t *friend_init);

/**
 * @fn        uint16_t rtk_bt_mesh_stack_fn_deinit(void)
 * @brief     Deinit friendship FN node, will cause event @ref RTK_BT_MESH_STACK_ACT_FN_DEINIT
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_stack_fn_deinit(void);

/**
 * @}
 */

#endif
