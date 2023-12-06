/**
 * @file      rtk_bt_mesh_api_config_client_model.h
 * @author    pico_zhai@realsil.com.cn
 * @brief     Bluetooth LE MESH configuration client model data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_MESH_CONFIG_CLIENT_MODEL_H__
#define __RTK_BT_MESH_CONFIG_CLIENT_MODEL_H__

#include <stdint.h>
#include <rtk_bt_mesh_def.h>

/**
 * @typedef   rtk_bt_mesh_config_client_api_t
 * @brief     BLE MESH config client model act and event definition.
 */
typedef enum {
	RTK_BT_MESH_CONFIG_MODEL_ADD_APP_KEY = 1,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_APP_BIND,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_ADD,
	RTK_BT_MESH_CONFIG_MODEL_DEFAULT_TTL_GET,
	RTK_BT_MESH_CONFIG_MODEL_DEFAULT_TTL_SET,
	RTK_BT_MESH_CONFIG_MODEL_DEFAULT_TTL_STAT,
	RTK_BT_MESH_CONFIG_MODEL_RELAY_GET,
	RTK_BT_MESH_CONFIG_MODEL_RELAY_SET,
	RTK_BT_MESH_CONFIG_MODEL_RELAY_STAT,
	RTK_BT_MESH_CONFIG_MODEL_NET_TRANSMIT_GET,
	RTK_BT_MESH_CONFIG_MODEL_NET_TRANSMIT_SET,
	RTK_BT_MESH_CONFIG_MODEL_NET_TRANSMIT_STAT,
	RTK_BT_MESH_CONFIG_MODEL_NODE_RESET,
	RTK_BT_MESH_CONFIG_MODEL_NODE_RESET_STAT,
	RTK_BT_MESH_CONFIG_MODEL_COMPO_DATA_GET,
	RTK_BT_MESH_CONFIG_MODEL_BEACON_GET,
	RTK_BT_MESH_CONFIG_MODEL_BEACON_SET,
	RTK_BT_MESH_CONFIG_MODEL_PROXY_GET,
	RTK_BT_MESH_CONFIG_MODEL_PROXY_SET,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_PUB_GET,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_PUB_SET,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_DELETE,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_DELETE_ALL,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_OVERWRITE,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_GET,
	RTK_BT_MESH_CONFIG_MODEL_NET_KEY_ADD,
	RTK_BT_MESH_CONFIG_MODEL_NET_KEY_UPDATE,
	RTK_BT_MESH_CONFIG_MODEL_NET_KEY_DELETE,
	RTK_BT_MESH_CONFIG_MODEL_NET_KEY_GET,
	RTK_BT_MESH_CONFIG_MODEL_APP_KEY_UPDATE,
	RTK_BT_MESH_CONFIG_MODEL_APP_KEY_DELETE,
	RTK_BT_MESH_CONFIG_MODEL_APP_KEY_GET,
	RTK_BT_MESH_CONFIG_MODEL_NODE_IDENTITY_GET,
	RTK_BT_MESH_CONFIG_MODEL_NODE_IDENTITY_SET,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_APP_UNBIND,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_APP_GET,
	RTK_BT_MESH_CONFIG_MODEL_FRND_GET,
	RTK_BT_MESH_CONFIG_MODEL_FRND_SET,
	RTK_BT_MESH_CONFIG_MODEL_LPN_POLL_TIMEOUT_GET,
	RTK_BT_MESH_CONFIG_MODEL_KEY_REFRESH_PHASE_GET,
	RTK_BT_MESH_CONFIG_MODEL_KEY_REFRESH_PHASE_SET,
	RTK_BT_MESH_CONFIG_MODEL_HB_PUB_GET,
	RTK_BT_MESH_CONFIG_MODEL_HB_PUB_SET,
	RTK_BT_MESH_CONFIG_MODEL_HB_SUB_GET,
	RTK_BT_MESH_CONFIG_MODEL_HB_SUB_SET,
	RTK_BT_MESH_CONFIG_MODEL_BEACON_STAT,
	RTK_BT_MESH_CONFIG_MODEL_COMPO_DATA_STAT,
	RTK_BT_MESH_CONFIG_MODEL_PROXY_STAT,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_PUB_STAT,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_STAT,
	RTK_BT_MESH_CONFIG_MODEL_SIG_MODEL_SUB_LIST,
	RTK_BT_MESH_CONFIG_MODEL_VENDOR_MODEL_SUB_LIST,
	RTK_BT_MESH_CONFIG_MODEL_NET_KEY_STAT,
	RTK_BT_MESH_CONFIG_MODEL_NET_KEY_LIST,
	RTK_BT_MESH_CONFIG_MODEL_APP_KEY_STAT,
	RTK_BT_MESH_CONFIG_MODEL_APP_KEY_LIST,
	RTK_BT_MESH_CONFIG_MODEL_NODE_IDENTITY_STAT,
	RTK_BT_MESH_CONFIG_MODEL_MODEL_APP_STAT,
	RTK_BT_MESH_CONFIG_MODEL_SIG_MODEL_APP_LIST,
	RTK_BT_MESH_CONFIG_MODEL_VENDOR_MODEL_APP_LIST,
	RTK_BT_MESH_CONFIG_MODEL_FRND_STAT,
	RTK_BT_MESH_CONFIG_MODEL_LPN_POLL_TO_STAT,
	RTK_BT_MESH_CONFIG_MODEL_KEY_REFRESH_PHASE_STAT,
	RTK_BT_MESH_CONFIG_MODEL_HB_PUB_STAT,
	RTK_BT_MESH_CONFIG_MODEL_HB_SUB_STAT,
	RTK_BT_MESH_CONFIG_MODEL_MAX,
} rtk_bt_mesh_config_client_api_t;

/**
 * @typedef   rtk_bt_mesh_hb_pub_features_t
 * @brief     BLE MESH Config Pub Features definition.
 */
typedef union {
	uint16_t raw;
	struct {
		uint16_t relay: 1;
		uint16_t proxy: 1;
		uint16_t frnd: 1;
		uint16_t lpn: 1;
		uint16_t rfu: 12;
	};
} _PACKED4_ rtk_bt_mesh_hb_pub_features_t;

/**
 * @typedef   rtk_bt_mesh_pub_key_info_t
 * @brief     BLE MESH Config Pub Key Information definition.
 */
typedef struct {
	union {
		uint16_t value;
		struct {
			uint16_t app_key_index: 12; //!< global index
			uint16_t frnd_flag: 1;
			uint16_t rfu: 3;
		};
	};
} _PACKED4_ rtk_bt_mesh_pub_key_info_t;

/**
 * @typedef   rtk_bt_mesh_pub_period_t
 * @brief     BLE MESH Config Pub Period definition.
 */
typedef struct {
	uint8_t steps : 6; //!< 0x00 Publish Period is disabled
	uint8_t resol : 2; //!< The resolution of the Number of Steps field
} _PACKED4_ rtk_bt_mesh_pub_period_t;

/**
 * @typedef   rtk_bt_mesh_pub_period_t
 * @brief     BLE MESH Config Pub Retrans Information definition.
 */
typedef struct {
	uint8_t count : 3;
	uint8_t steps : 5;
} _PACKED4_ rtk_bt_mesh_pub_retrans_info_t;

/**
 * @typedef   rtk_bt_mesh_msg_stat_t
 * @brief     BLE MESH config client model received msg status definition.
 */
typedef enum {
	RTK_BT_MESH_MSG_STAT_SUCCESS,
	RTK_BT_MESH_MSG_STAT_INVALID_ADDR,
	RTK_BT_MESH_MSG_STAT_INVALID_MODEL,
	RTK_BT_MESH_MSG_STAT_INVALID_APP_KEY_INDEX,
	RTK_BT_MESH_MSG_STAT_INVALID_NET_KEY_INDEX,
	RTK_BT_MESH_MSG_STAT_INSUFFICIENT_RESOURCES,
	RTK_BT_MESH_MSG_STAT_KEY_INDEX_ALREADY_STORED,
	RTK_BT_MESH_MSG_STAT_INVALID_PUB_PARAMS,
	RTK_BT_MESH_MSG_STAT_NOT_SUB_MODEL, //8, TODO: What is not a sub model?
	RTK_BT_MESH_MSG_STAT_STORAGE_FAIL,
	RTK_BT_MESH_MSG_STAT_FEATURE_NOT_SUPPORTED,
	RTK_BT_MESH_MSG_STAT_CANNOT_UPDATE,
	RTK_BT_MESH_MSG_STAT_CANNOT_REMOVE,
	RTK_BT_MESH_MSG_STAT_CANNOT_BIND,
	RTK_BT_MESH_MSG_STAT_TEMP_UNABLE_CHANGE_STATE,
	RTK_BT_MESH_MSG_STAT_CANNOT_SET,
	RTK_BT_MESH_MSG_STAT_UNSPECIFIED_ERROR, //16
	RTK_BT_MESH_MSG_STAT_INVALID_BINDING,
} rtk_bt_mesh_msg_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_app_key_stat_t
 * @brief     BLE MESH Config AppKey Status definition.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint8_t key_index[3]; //!< NetKeyIndex, AppKeyIndex
} _PACKED4_ rtk_bt_mesh_cfg_app_key_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_model_app_stat_t
 * @brief     BLE MESH Config Model App Status definition.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint16_t element_addr;
	uint16_t app_key_index;
	uint32_t model_id; //!< 2 bytes or 4 bytes
} _PACKED4_ rtk_bt_mesh_cfg_model_app_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_model_sub_stat_t
 * @brief     BLE MESH Config Model Subscription Status definition.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint16_t element_addr;
	uint16_t addr;
	uint32_t model_id; //!< 2 bytes or 4 bytes
} _PACKED4_ rtk_bt_mesh_cfg_model_sub_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_default_ttl_stat_t
 * @brief     BLE MESH Config Default TTL Status definition.
 */
typedef struct {
	uint8_t ttl;
} _PACKED4_ rtk_bt_mesh_cfg_default_ttl_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_relay_stat_t
 * @brief     BLE MESH Config Relay Status message structure.
 */
typedef struct {
	uint8_t state;
	uint8_t count : 3;
	uint8_t steps : 5;
} _PACKED4_ rtk_bt_mesh_cfg_relay_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_net_transmit_stat_t
 * @brief     BLE MESH Config Network Transmit Status message structure.
 */
typedef struct {
	uint8_t count : 3;
	uint8_t steps : 5;
} _PACKED4_ rtk_bt_mesh_cfg_net_transmit_stat_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_node_reset_stat_t
 * @brief     BLE MESH Config Node Reset Status message structure.
 */
typedef struct {
	uint16_t src;
} _PACKED4_ rtk_bt_mesh_config_client_model_node_reset_stat_t;

/**
 * @typedef   rtk_bt_mesh_config_client_add_app_key_t
 * @brief     BLE MESH Config AppKey Add message structure.
 */
typedef struct {
	uint16_t unicast_addr;
	uint8_t net_key_index;
	uint8_t app_key_index;
} rtk_bt_mesh_config_client_add_app_key_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_app_bind_t
 * @brief     BLE MESH Config Model App Bind message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t element_addr;
	uint16_t app_key_index;
	uint32_t model_id;
} rtk_bt_mesh_config_client_model_app_bind_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_sub_add_t
 * @brief     BLE MESH Config Model Subscription Add message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t element_addr;
	bool va_flag;
	uint8_t group_addr[16];//for virtual address
	uint32_t model_id;
} rtk_bt_mesh_config_client_model_sub_add_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_default_ttl_get_t
 * @brief     BLE MESH Config Default TTL Get message structure.
 */
typedef struct {
	uint16_t dst;
} rtk_bt_mesh_config_client_model_default_ttl_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_default_ttl_set_t
 * @brief     BLE MESH Config Default TTL Set message structure.
 */
typedef struct {
	uint16_t dst;
	uint8_t ttl;
} rtk_bt_mesh_config_client_model_default_ttl_set_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_relay_get_t
 * @brief     BLE MESH Config Relay Get message structure.
 */
typedef struct {
	uint16_t dst;
} rtk_bt_mesh_config_client_model_relay_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_relay_set_t
 * @brief     BLE MESH Config Relay Set message structure.
 */
typedef struct {
	uint16_t dst;
	uint8_t state;
	uint8_t count;
	uint8_t steps;
} rtk_bt_mesh_config_client_model_relay_set_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_net_transmit_get_t
 * @brief     BLE MESH Config Network Transmit Get message structure.
 */
typedef struct {
	uint16_t dst;
} rtk_bt_mesh_config_client_model_net_transmit_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_net_transmit_set_t
 * @brief     BLE MESH Config Network Transmit Set message structure.
 */
typedef struct {
	uint16_t dst;
	uint8_t count;
	uint8_t steps;
} rtk_bt_mesh_config_client_model_net_transmit_set_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_node_reset_t
 * @brief     BLE MESH Config Node Reset message structure.
 */
typedef struct {
	uint16_t dst;
} rtk_bt_mesh_config_client_model_node_reset_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_compo_data_get_t
 * @brief     BLE MESH Config Composition Date Get message structure.
 */
typedef struct {
	uint16_t dst;
	uint8_t page;
} rtk_bt_mesh_config_client_model_compo_data_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_beacon_get_t
 * @brief     BLE MESH Config Beacon Get message structure.
 */
typedef struct {
	uint16_t dst;
} rtk_bt_mesh_config_client_model_beacon_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_beacon_set_t
 * @brief     BLE MESH Config Beacon Set message structure.
 */
typedef struct {
	uint16_t dst;
	uint8_t state;
} rtk_bt_mesh_config_client_model_beacon_set_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_beacon_set_t
 * @brief     BLE MESH Config GATT Proxy Get message structure.
 */
typedef struct {
	uint16_t dst;
} rtk_bt_mesh_config_client_model_proxy_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_proxy_set_t
 * @brief     BLE MESH Config GATT Proxy Set message structure.
 */
typedef struct {
	uint16_t dst;
	uint8_t state;
} rtk_bt_mesh_config_client_model_proxy_set_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_model_pub_get_t
 * @brief     BLE MESH Config Model Publication Get message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t element_addr;
	uint32_t model_id;
} rtk_bt_mesh_config_client_model_model_pub_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_model_pub_set_t
 * @brief     BLE MESH Config Model Publication Set message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t element_addr;
	bool va_flag;
	uint8_t pub_addr[16];//pico_zhai's style
	rtk_bt_mesh_pub_key_info_t pub_key_info;
	uint8_t pub_ttl;
	rtk_bt_mesh_pub_period_t pub_period;
	rtk_bt_mesh_pub_retrans_info_t pub_retrans_info;
	uint32_t model_id;
} rtk_bt_mesh_config_client_model_model_pub_set_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_model_sub_delete_t
 * @brief     BLE MESH Config Model Subscription Delete message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t element_addr;
	bool va_flag;
	uint8_t group_addr[16];//pico_zhai's style
	uint32_t model_id;
} rtk_bt_mesh_config_client_model_model_sub_delete_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_model_sub_delete_all_t
 * @brief     BLE MESH Config Model Subscription Delete All message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t element_addr;
	uint32_t model_id;
} rtk_bt_mesh_config_client_model_model_sub_delete_all_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_model_sub_overwrite_t
 * @brief     BLE MESH Config Model Subscription Overwrite message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t element_addr;
	bool va_flag;
	uint8_t group_addr[16];//pico_zhai's style
	uint32_t model_id;
} rtk_bt_mesh_config_client_model_model_sub_overwrite_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_model_sub_get_t
 * @brief     BLE MESH Config Model Subscription Get message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t element_addr;
	uint32_t model_id;
} rtk_bt_mesh_config_client_model_model_sub_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_net_key_add_t
 * @brief     BLE MESH Config NetKey Add message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
	uint8_t net_key[16];
} rtk_bt_mesh_config_client_model_net_key_add_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_net_key_update_t
 * @brief     BLE MESH Config NetKey Update message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
	uint8_t net_key[16];
} rtk_bt_mesh_config_client_model_net_key_update_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_net_key_delete_t
 * @brief     BLE MESH Config NetKey Delete message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
} rtk_bt_mesh_config_client_model_net_key_delete_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_net_key_get_t
 * @brief     BLE MESH Config NetKey Get message structure.
 */
typedef struct {
	uint16_t dst;
} rtk_bt_mesh_config_client_model_net_key_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_app_key_update_t
 * @brief     BLE MESH Config AppKey Update message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
	uint16_t app_key_index;
	uint8_t app_key[16];
} rtk_bt_mesh_config_client_model_app_key_update_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_app_key_delete_t
 * @brief     BLE MESH Config AppKey Delete message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
	uint16_t app_key_index;
} rtk_bt_mesh_config_client_model_app_key_delete_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_app_key_get_t
 * @brief     BLE MESH Config AppKey Get message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
} rtk_bt_mesh_config_client_model_app_key_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_node_identity_get_t
 * @brief     BLE MESH Config Node Identity Get message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
} rtk_bt_mesh_config_client_model_node_identity_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_node_identity_get_t
 * @brief     BLE MESH Config Node Identity Set message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
	uint8_t identity;
} rtk_bt_mesh_config_client_model_node_identity_set_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_model_app_unbind_t
 * @brief     BLE MESH Config Model App Unbind message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t element_addr;
	uint16_t app_key_index;
	uint32_t model_id;
} rtk_bt_mesh_config_client_model_model_app_unbind_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_model_app_get_t
 * @brief     BLE MESH Config Model App Get message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t element_addr;
	uint32_t model_id;
} rtk_bt_mesh_config_client_model_model_app_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_frnd_get_t
 * @brief     BLE MESH Config Friend Get message structure.
 */
typedef struct {
	uint16_t dst;
} rtk_bt_mesh_config_client_model_frnd_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_frnd_set_t
 * @brief     BLE MESH Config Friend Set message structure.
 */
typedef struct {
	uint16_t dst;
	uint8_t state;
} rtk_bt_mesh_config_client_model_frnd_set_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_lpn_poll_timeout_get_t
 * @brief     BLE MESH Config Low Power Node PollTimeout Get message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t lpn_addr;
} rtk_bt_mesh_config_client_model_lpn_poll_timeout_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_key_refresh_phase_get_t
 * @brief     BLE MESH Config Key Refresh Phase Get message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
} rtk_bt_mesh_config_client_model_key_refresh_phase_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_key_refresh_phase_set_t
 * @brief     BLE MESH Config Key Refresh Phase Set message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
	uint8_t state;
} rtk_bt_mesh_config_client_model_key_refresh_phase_set_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_hb_pub_get_t
 * @brief     BLE MESH Config Heartbeat Publication Get message structure.
 */
typedef struct {
	uint16_t dst;
} rtk_bt_mesh_config_client_model_hb_pub_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_hb_pub_set_t
 * @brief     BLE MESH Config Heartbeat Publication Set message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t dst_pub;
	uint8_t count_log;
	uint8_t period_log;
	uint8_t ttl;
	rtk_bt_mesh_hb_pub_features_t features;
	uint16_t net_key_index;
} rtk_bt_mesh_config_client_model_hb_pub_set_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_hb_sub_get_t
 * @brief     BLE MESH Config Heartbeat Subscription Get message structure.
 */
typedef struct {
	uint16_t dst;
} rtk_bt_mesh_config_client_model_hb_sub_get_t;

/**
 * @typedef   rtk_bt_mesh_config_client_model_hb_sub_set_t
 * @brief     BLE MESH Config Heartbeat Subscription Set message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t src;
	uint16_t dst_set;
	uint8_t period_log;
} rtk_bt_mesh_config_client_model_hb_sub_set_t;

/**************callback data struct style**********************/
/**
 * @typedef   rtk_bt_mesh_cfg_beacon_stat_t
 * @brief     BLE MESH Config Beacon Status definition.
 */
typedef struct {
	uint8_t state;
} rtk_bt_mesh_cfg_beacon_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_compo_data_stat_t
 * @brief     BLE MESH Config Composition Data Status definition.
 */
typedef struct {
	uint8_t page;
	uint8_t data[2]; //!< variable length
} _PACKED4_ rtk_bt_mesh_cfg_compo_data_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_compo_data_stat_call_t
 * @brief     BLE MESH Config Composition Data Status definition for callback to app.
 */
typedef struct {
	uint8_t page;
	uint16_t msg_len;
	uint16_t src;
	uint8_t flag;
	uint32_t length; //!< for ipc test
	uint8_t *data; //!< variable length
} _PACKED4_ rtk_bt_mesh_cfg_compo_data_stat_call_t;

/**
 * @typedef   rtk_bt_mesh_cfg_proxy_stat_t
 * @brief     BLE MESH Config GATT Proxy Status definition.
 */
typedef struct {
	uint8_t state;
} _PACKED4_ rtk_bt_mesh_cfg_proxy_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_model_pub_stat_t
 * @brief     BLE MESH Config Model Publication Status definition.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint16_t element_addr;
	uint16_t pub_addr;
	rtk_bt_mesh_pub_key_info_t pub_key_info;
	uint8_t pub_ttl;
	rtk_bt_mesh_pub_period_t pub_period;
	rtk_bt_mesh_pub_retrans_info_t pub_retrans_info;
	uint32_t model_id; //!< 2 bytes or 4 bytes
} _PACKED4_ rtk_bt_mesh_cfg_model_pub_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_model_sub_list_t
 * @brief     BLE MESH Config Model Subscription List definition.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint16_t element_addr;
	uint32_t model_id; //!< 2 bytes or 4 bytes
	uint8_t data[2]; //!< variable length
} _PACKED4_ rtk_bt_mesh_cfg_model_sub_list_t;

/**
 * @typedef   rtk_bt_mesh_cfg_net_key_stat_t
 * @brief     BLE MESH Config NetKey Status definition.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint16_t net_key_index;
} _PACKED4_ rtk_bt_mesh_cfg_net_key_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_net_key_list_t
 * @brief     BLE MESH Config NetKey List definition.
 */
typedef struct {
	uint8_t net_key_indexes[2]; //!< variable length
} rtk_bt_mesh_cfg_net_key_list_t;

/**
 * @typedef   rtk_bt_mesh_cfg_app_key_list_t
 * @brief     BLE MESH Config AppKey List definition.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint16_t net_key_index;
	uint8_t app_key_indexes[2]; //!< variable length
} _PACKED4_ rtk_bt_mesh_cfg_app_key_list_t;

/**
 * @typedef   rtk_bt_mesh_cfg_node_identity_stat_t
 * @brief     BLE MESH Config Node Identity Status definition.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint16_t net_key_index;
	uint8_t identity;
} _PACKED4_ rtk_bt_mesh_cfg_node_identity_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_model_app_list_t
 * @brief     BLE MESH Config Model App List definition.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint16_t element_addr;
	uint32_t model_id; //!< 2 bytes or 4 bytes
	uint8_t app_key_indexes[3]; //!< variable length
} rtk_bt_mesh_cfg_model_app_list_t;

/**
 * @typedef   rtk_bt_mesh_cfg_frnd_stat_t
 * @brief     BLE MESH Config Friend Status definition.
 */
typedef struct {
	uint8_t state;
} _PACKED4_ rtk_bt_mesh_cfg_frnd_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_lpn_poll_timeout_stat_t
 * @brief     BLE MESH Config Low Power Node PollTimeout Status definition.
 */
typedef struct {
	uint16_t lpn_addr;
	uint8_t poll_to[3];
} _PACKED4_ rtk_bt_mesh_cfg_lpn_poll_timeout_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_key_refresh_phase_stat_t
 * @brief     BLE MESH Config Key Refresh Phase Status definition.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint16_t net_key_index;
	uint8_t state;
} _PACKED4_ rtk_bt_mesh_cfg_key_refresh_phase_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_key_refresh_phase_stat_call_t
 * @brief     BLE MESH Config Key Refresh Phase Status definition for callback to app.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint16_t net_key_index;
	uint8_t state;
	uint16_t src;
} _PACKED4_ rtk_bt_mesh_cfg_key_refresh_phase_stat_call_t;

/**
 * @typedef   rtk_bt_mesh_cfg_hb_pub_stat_t
 * @brief     BLE MESH Config Heartbeat Publication Status definition.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint16_t dst;
	uint8_t count_log;
	uint8_t period_log;
	uint8_t ttl;
	rtk_bt_mesh_hb_pub_features_t features;
	uint16_t net_key_index;
} _PACKED4_ rtk_bt_mesh_cfg_hb_pub_stat_t;

/**
 * @typedef   rtk_bt_mesh_cfg_hb_sub_stat_t
 * @brief     BLE MESH Config Heartbeat Subscription Status definition.
 */
typedef struct {
	rtk_bt_mesh_msg_stat_t stat;
	uint16_t src;
	uint16_t dst;
	uint8_t period_log;
	uint8_t count_log;
	uint8_t min_hops;
	uint8_t max_hops;
} _PACKED4_ rtk_bt_mesh_cfg_hb_sub_stat_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  ble_mesh_config_client_model BT LE Mesh Config Client Model APIs
 * @brief     BT LE Mesh Config Client Model related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_add_app_key(rtk_bt_mesh_config_client_add_app_key_t *add_app_key)
 * @brief     Send Config AppKey Add message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_ADD_APP_KEY
 * @param[in] add_app_key: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_add_app_key(rtk_bt_mesh_config_client_add_app_key_t *add_app_key);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_app_bind(rtk_bt_mesh_config_client_model_app_bind_t *model_app_bind)
 * @brief     Send Config Model App Bind message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_MODEL_APP_BIND
 * @param[in] model_app_bind: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_app_bind(rtk_bt_mesh_config_client_model_app_bind_t *model_app_bind);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_sub_add(rtk_bt_mesh_config_client_model_sub_add_t *model_sub_add)
 * @brief     Send Config Model Subscription Add message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_ADD
 * @param[in] model_sub_add: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_sub_add(rtk_bt_mesh_config_client_model_sub_add_t *model_sub_add);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_default_ttl_get(rtk_bt_mesh_config_client_model_default_ttl_get_t *ttl_get)
 * @brief     Send Config Default TTL Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_DEFAULT_TTL_GET
 * @param[in] ttl_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_default_ttl_get(rtk_bt_mesh_config_client_model_default_ttl_get_t *ttl_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_default_ttl_set(rtk_bt_mesh_config_client_model_default_ttl_set_t *ttl_set)
 * @brief     Send Config Default TTL Set message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_DEFAULT_TTL_SET
 * @param[in] ttl_set: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_default_ttl_set(rtk_bt_mesh_config_client_model_default_ttl_set_t *ttl_set);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_relay_get(rtk_bt_mesh_config_client_model_relay_get_t *relay_get)
 * @brief     Send Config Relay Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_RELAY_GET
 * @param[in] relay_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_relay_get(rtk_bt_mesh_config_client_model_relay_get_t *relay_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_relay_set(rtk_bt_mesh_config_client_model_relay_set_t *relay_set)
 * @brief     Send Config Relay Set message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_RELAY_SET
 * @param[in] relay_set: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_relay_set(rtk_bt_mesh_config_client_model_relay_set_t *relay_set);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_net_transmit_get(rtk_bt_mesh_config_client_model_net_transmit_get_t *net_transmit_get)
 * @brief     Send Config Network Transmit Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_NET_TRANSMIT_GET
 * @param[in] net_transmit_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_net_transmit_get(rtk_bt_mesh_config_client_model_net_transmit_get_t *net_transmit_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_net_transmit_set(rtk_bt_mesh_config_client_model_net_transmit_set_t *net_transmit_set)
 * @brief     Send Config Network Transmit Set message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_NET_TRANSMIT_SET
 * @param[in] net_transmit_set: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_net_transmit_set(rtk_bt_mesh_config_client_model_net_transmit_set_t *net_transmit_set);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_node_reset(rtk_bt_mesh_config_client_model_node_reset_t *node_reset)
 * @brief     Send Config Node Reset message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_NODE_RESET
 * @param[in] node_reset: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_node_reset(rtk_bt_mesh_config_client_model_node_reset_t *node_reset);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_compo_data_get(rtk_bt_mesh_config_client_model_compo_data_get_t *compo_data_get)
 * @brief     Send Config Composition Data Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_COMPO_DATA_GET
 * @param[in] compo_data_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_compo_data_get(rtk_bt_mesh_config_client_model_compo_data_get_t *compo_data_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_beacon_get(rtk_bt_mesh_config_client_model_beacon_get_t *beacon_get)
 * @brief     Send Config Beacon Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_BEACON_GET
 * @param[in] beacon_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_beacon_get(rtk_bt_mesh_config_client_model_beacon_get_t *beacon_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_beacon_set(rtk_bt_mesh_config_client_model_beacon_set_t *beacon_set)
 * @brief     Send Config Beacon Set message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_BEACON_SET
 * @param[in] beacon_set: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_beacon_set(rtk_bt_mesh_config_client_model_beacon_set_t *beacon_set);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_proxy_get(rtk_bt_mesh_config_client_model_proxy_get_t *proxy_get)
 * @brief     Send Config GATT Proxy Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_PROXY_GET
 * @param[in] proxy_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_proxy_get(rtk_bt_mesh_config_client_model_proxy_get_t *proxy_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_proxy_set(rtk_bt_mesh_config_client_model_proxy_set_t *proxy_set)
 * @brief     Send Config GATT Proxy Set message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_PROXY_SET
 * @param[in] proxy_set: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_proxy_set(rtk_bt_mesh_config_client_model_proxy_set_t *proxy_set);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_pub_get(rtk_bt_mesh_config_client_model_model_pub_get_t *pub_get)
 * @brief     Send Config Model Publication Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_MODEL_PUB_GET
 * @param[in] pub_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_pub_get(rtk_bt_mesh_config_client_model_model_pub_get_t *pub_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_pub_set(rtk_bt_mesh_config_client_model_model_pub_set_t *pub_set)
 * @brief     Send Config Model Publication Set message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_MODEL_PUB_SET
 * @param[in] pub_set: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_pub_set(rtk_bt_mesh_config_client_model_model_pub_set_t *pub_set);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_sub_delete(rtk_bt_mesh_config_client_model_model_sub_delete_t *sub_delete)
 * @brief     Send Config Model Subscription Delete message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_DELETE
 * @param[in] sub_delete: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_sub_delete(rtk_bt_mesh_config_client_model_model_sub_delete_t *sub_delete);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_sub_delete_all(rtk_bt_mesh_config_client_model_model_sub_delete_all_t *sub_delete_all)
 * @brief     Send Config Model Subscription Delete All message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_DELETE_ALL
 * @param[in] sub_delete_all: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_sub_delete_all(rtk_bt_mesh_config_client_model_model_sub_delete_all_t *sub_delete_all);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_sub_overwrite(rtk_bt_mesh_config_client_model_model_sub_overwrite_t *sub_overwrite)
 * @brief     Send Config Model Subscription Overwrite message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_OVERWRITE
 * @param[in] sub_overwrite: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_sub_overwrite(rtk_bt_mesh_config_client_model_model_sub_overwrite_t *sub_overwrite);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_sub_get(rtk_bt_mesh_config_client_model_model_sub_get_t *sub_get)
 * @brief     Send Config Model Subscription Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_MODEL_SUB_GET
 * @param[in] sub_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_sub_get(rtk_bt_mesh_config_client_model_model_sub_get_t *sub_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_net_key_add(rtk_bt_mesh_config_client_model_net_key_add_t *net_key_add)
 * @brief     Send Config NetKey Add message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_NET_KEY_ADD
 * @param[in] net_key_add: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_net_key_add(rtk_bt_mesh_config_client_model_net_key_add_t *net_key_add);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_net_key_update(rtk_bt_mesh_config_client_model_net_key_update_t *net_key_update)
 * @brief     Send Config NetKey Update message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_NET_KEY_UPDATE
 * @param[in] net_key_update: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_net_key_update(rtk_bt_mesh_config_client_model_net_key_update_t *net_key_update);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_net_key_delete(rtk_bt_mesh_config_client_model_net_key_delete_t *net_key_delete)
 * @brief     Send Config NetKey Delete message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_NET_KEY_DELETE
 * @param[in] net_key_delete: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_net_key_delete(rtk_bt_mesh_config_client_model_net_key_delete_t *net_key_delete);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_net_key_get(rtk_bt_mesh_config_client_model_net_key_get_t *net_key_get)
 * @brief     Send Config NetKey Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_NET_KEY_GET
 * @param[in] net_key_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_net_key_get(rtk_bt_mesh_config_client_model_net_key_get_t *net_key_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_app_key_update(rtk_bt_mesh_config_client_model_app_key_update_t *app_key_update)
 * @brief     Send Config AppKey Update message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_APP_KEY_UPDATE
 * @param[in] app_key_update: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_app_key_update(rtk_bt_mesh_config_client_model_app_key_update_t *app_key_update);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_app_key_delete(rtk_bt_mesh_config_client_model_app_key_delete_t *app_key_delete)
 * @brief     Send Config AppKey Delete message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_APP_KEY_DELETE
 * @param[in] app_key_delete: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_app_key_delete(rtk_bt_mesh_config_client_model_app_key_delete_t *app_key_delete);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_app_key_get(rtk_bt_mesh_config_client_model_app_key_get_t *app_key_get)
 * @brief     Send Config AppKey Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_APP_KEY_GET
 * @param[in] app_key_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_app_key_get(rtk_bt_mesh_config_client_model_app_key_get_t *app_key_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_node_identity_get(rtk_bt_mesh_config_client_model_node_identity_get_t *node_identity_get)
 * @brief     Send Config Node Identity Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_NODE_IDENTITY_GET
 * @param[in] node_identity_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_node_identity_get(rtk_bt_mesh_config_client_model_node_identity_get_t *node_identity_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_node_identity_set(rtk_bt_mesh_config_client_model_node_identity_set_t *node_identity_set)
 * @brief     Send Config Node Identity Set message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_NODE_IDENTITY_SET
 * @param[in] node_identity_set: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_node_identity_set(rtk_bt_mesh_config_client_model_node_identity_set_t *node_identity_set);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_app_unbind(rtk_bt_mesh_config_client_model_model_app_unbind_t *model_app_unbind)
 * @brief     Send Config Model App Unbind message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_MODEL_APP_UNBIND
 * @param[in] model_app_unbind: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_app_unbind(rtk_bt_mesh_config_client_model_model_app_unbind_t *model_app_unbind);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_app_get(rtk_bt_mesh_config_client_model_model_app_get_t *model_app_get)
 * @brief     Send Config Model App Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_MODEL_APP_GET
 * @param[in] model_app_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_app_get(rtk_bt_mesh_config_client_model_model_app_get_t *model_app_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_frnd_get(rtk_bt_mesh_config_client_model_frnd_get_t *frnd_get)
 * @brief     Send Config Friend Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_FRND_GET
 * @param[in] frnd_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_frnd_get(rtk_bt_mesh_config_client_model_frnd_get_t *frnd_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_frnd_set(rtk_bt_mesh_config_client_model_frnd_set_t *frnd_set)
 * @brief     Send Config Friend Set message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_FRND_SET
 * @param[in] frnd_set: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_frnd_set(rtk_bt_mesh_config_client_model_frnd_set_t *frnd_set);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_lpn_poll_timeout_get(rtk_bt_mesh_config_client_model_lpn_poll_timeout_get_t *lpn_poll_timeout_get)
 * @brief     Send Config Low Power Node PollTimeout Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_LPN_POLL_TIMEOUT_GET
 * @param[in] lpn_poll_timeout_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_lpn_poll_timeout_get(rtk_bt_mesh_config_client_model_lpn_poll_timeout_get_t *lpn_poll_timeout_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_key_refresh_phase_get(rtk_bt_mesh_config_client_model_key_refresh_phase_get_t *key_refresh_phase_get)
 * @brief     Send Config Key Refresh Phase Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_KEY_REFRESH_PHASE_GET
 * @param[in] key_refresh_phase_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_key_refresh_phase_get(rtk_bt_mesh_config_client_model_key_refresh_phase_get_t *key_refresh_phase_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_key_refresh_phase_set(rtk_bt_mesh_config_client_model_key_refresh_phase_set_t *key_refresh_phase_set)
 * @brief     Send Config Key Refresh Phase Set message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_KEY_REFRESH_PHASE_SET
 * @param[in] key_refresh_phase_set: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_key_refresh_phase_set(rtk_bt_mesh_config_client_model_key_refresh_phase_set_t *key_refresh_phase_set);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_hb_pub_get(rtk_bt_mesh_config_client_model_hb_pub_get_t *hb_pub_get)
 * @brief     Send Config Heartbeat Publication Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_HB_PUB_GET
 * @param[in] hb_pub_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_hb_pub_get(rtk_bt_mesh_config_client_model_hb_pub_get_t *hb_pub_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_hb_pub_set(rtk_bt_mesh_config_client_model_hb_pub_set_t *hb_pub_set)
 * @brief     Send Config Heartbeat Publication Set message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_HB_PUB_SET
 * @param[in] hb_pub_set: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_hb_pub_set(rtk_bt_mesh_config_client_model_hb_pub_set_t *hb_pub_set);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_hb_sub_get(rtk_bt_mesh_config_client_model_hb_sub_get_t *hb_sub_get)
 * @brief     Send Config Heartbeat Subscription Get message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_HB_SUB_GET
 * @param[in] hb_sub_get: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_hb_sub_get(rtk_bt_mesh_config_client_model_hb_sub_get_t *hb_sub_get);

/**
 * @fn        uint16_t rtk_bt_mesh_config_client_model_hb_sub_set(rtk_bt_mesh_config_client_model_hb_sub_set_t *hb_sub_set)
 * @brief     Send Config Heartbeat Subscription Set message to device, will cause event @ref RTK_BT_MESH_CONFIG_MODEL_HB_SUB_SET
 * @param[in] hb_sub_set: config message param
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_config_client_model_hb_sub_set(rtk_bt_mesh_config_client_model_hb_sub_set_t *hb_sub_set);

/**
 * @}
 */

#endif
