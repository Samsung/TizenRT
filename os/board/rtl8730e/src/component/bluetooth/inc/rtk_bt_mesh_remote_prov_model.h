/**
 * @file      rtk_bt_mesh_api_config_client_model.h
 * @author    pico_zhai@realsil.com.cn
 * @brief     Bluetooth LE MESH configuration client model data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_MESH_REMOTE_PROVISIONING_H__
#define __RTK_BT_MESH_REMOTE_PROVISIONING_H__

#include <stdint.h>

/* =============================================== Remote provisioning client model relate data struct definition ======================================= */

/**
 * @typedef   rtk_bt_mesh_remote_prov_client_act_t
 * @brief     BLE MESH remote provisioning client model act definition.
 */
typedef enum {
	RTK_BT_MESH_REMOTE_PROV_CLIENT_ACT_SCAN_START = 1,
	RTK_BT_MESH_REMOTE_PROV_CLIENT_ACT_SCAN_CAPA_GET,
	RTK_BT_MESH_REMOTE_PROV_CLIENT_ACT_LINK_OPEN,
	RTK_BT_MESH_REMOTE_PROV_CLIENT_ACT_MAX,
} rtk_bt_mesh_remote_prov_client_act_t;

/**
 * @typedef   rtk_bt_mesh_remote_prov_client_evt_t
 * @brief     BLE MESH remote provisioning client model evt definition.
 */
typedef enum {
	RTK_BT_MESH_REMOTE_PROV_CLIENT_EVT_SCAN_CAPA_STATUS = 1,
	RTK_BT_MESH_REMOTE_PROV_CLIENT_EVT_SCAN_STATUS,
	RTK_BT_MESH_REMOTE_PROV_CLIENT_EVT_SCAN_REPORT,
	RTK_BT_MESH_REMOTE_PROV_CLIENT_EVT_LINK_STATUS,
	RTK_BT_MESH_REMOTE_PROV_CLIENT_EVT_LINK_REPORT,
} rtk_bt_mesh_remote_prov_client_evt_t;

/**
 * @typedef   rtk_bt_mesh_rmt_prov_status_t
 * @brief     BLE MESH remote provisioning status definition.
 */
typedef enum {
	RTK_BT_MESH_RMT_PROV_SUCCESS,
	RTK_BT_MESH_RMT_PROV_SCANNING_CANNOT_START,
	RTK_BT_MESH_RMT_PROV_INVALID_STATE,
	RTK_BT_MESH_RMT_PROV_LIMITED_RESOURCES,
	RTK_BT_MESH_RMT_PROV_LINK_CANNOT_OPEN,
	RTK_BT_MESH_RMT_PROV_LINK_OPEN_FAILED,
	RTK_BT_MESH_RMT_PROV_LINK_CLOSED_BY_DEVICE,  // 7
	RTK_BT_MESH_RMT_PROV_LINK_CLOSED_BY_SERVER,
	RTK_BT_MESH_RMT_PROV_LINK_CLOSED_BY_CLIENT,
	RTK_BT_MESH_RMT_PROV_LINK_CLOSED_AS_CANNOT_RECEIVE_PDU,
	RTK_BT_MESH_RMT_PROV_LINK_CLOSED_AS_CANNOT_SEND_PDU,
	RTK_BT_MESH_RMT_PROV_LINK_CLOSED_AS_CANNOT_DELIVER_PDU_REPORT,
} rtk_bt_mesh_rmt_prov_status_t;

/**
 * @typedef   rtk_bt_mesh_rmt_prov_scan_state_t
 * @brief     BLE MESH remote provisioning scan status definition.
 */
typedef enum {
	RTK_BT_MESH_RMT_PROV_SCAN_STATE_IDLE,
	RTK_BT_MESH_RMT_PROV_SCAN_STATE_MULTIPLE_DEVICES,
	RTK_BT_MESH_RMT_PROV_SCAN_STATE_SINGLE_DEVICE,
} rtk_bt_mesh_rmt_prov_scan_state_t;

/**
 * @typedef   rtk_bt_mesh_rmt_prov_link_state_t
 * @brief     BLE MESH remote provisioning link status definition.
 */
typedef enum {
	RTK_BT_MESH_RMT_PROV_LINK_STATE_IDLE,
	RTK_BT_MESH_RMT_PROV_LINK_STATE_LINK_OPENING,
	RTK_BT_MESH_RMT_PROV_LINK_STATE_LINK_ACTIVE,
	RTK_BT_MESH_RMT_PROV_LINK_STATE_OUTBOUND_PKT_TRANS,
	RTK_BT_MESH_RMT_PROV_LINK_STATE_LINK_CLOSING,
} rtk_bt_mesh_rmt_prov_link_state_t;

/**
 * @typedef   rtk_bt_mesh_rmt_prov_link_close_reason_t
 * @brief     BLE MESH remote provisioning link close reason definition.
 */
typedef enum {
	RTK_BT_MESH_RMT_PROV_LINK_CLOSE_SUCCESS,
	RTK_BT_MESH_RMT_PROV_LINK_CLOSE_PROHIBITED,
	RTK_BT_MESH_RMT_PROV_LINK_CLOSE_FAIL,
	RTK_BT_MESH_RMT_PROV_LINK_NOT_CLOSE,
} rtk_bt_mesh_rmt_prov_link_close_reason_t;

/**
 * @typedef   rtk_bt_mesh_remote_prov_client_scan_capa_get_t
 * @brief     BLE MESH remote provisioning scan capability get data struct definition.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
} rtk_bt_mesh_remote_prov_client_scan_capa_get_t;

/**
 * @typedef   rtk_bt_mesh_rmt_prov_client_scan_capa_status_t
 * @brief     BLE MESH remote provisioning scan capability status data struct definition.
 */
typedef struct {
	uint16_t src;
	uint8_t max_scan_items;  // Max scanned items capability
	bool support_active_scan;
} rtk_bt_mesh_rmt_prov_client_scan_capa_status_t;

/**
 * @typedef   rtk_bt_mesh_remote_prov_client_scan_start_t
 * @brief     BLE MESH remote provisioning scan start data struct definition.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
	uint8_t scanned_items_limit;
	uint8_t scan_timeout;  // Unit in second
	uint8_t uuid[16];
} rtk_bt_mesh_remote_prov_client_scan_start_t;

/**
 * @typedef   rtk_bt_mesh_rmt_prov_client_scan_status_t
 * @brief     BLE MESH remote provisioning scan status data struct definition.
 */
typedef struct {
	uint16_t src;
	rtk_bt_mesh_rmt_prov_status_t prov_status;
	rtk_bt_mesh_rmt_prov_scan_state_t scan_status;
	uint8_t scanned_items_limit;  // Max scanned items limit
	uint8_t timeout;  // Unit in second
} rtk_bt_mesh_rmt_prov_client_scan_status_t;

/**
 * @typedef   rtk_bt_mesh_rmt_prov_client_scan_report_t
 * @brief     BLE MESH remote provisioning scan report data struct definition.
 */
typedef struct {
	uint16_t src;
	int8_t rssi;
	uint16_t oob;
	uint32_t uri_hash;
	uint8_t uuid[16];
} rtk_bt_mesh_rmt_prov_client_scan_report_t;

/**
 * @typedef   rtk_bt_mesh_remote_prov_client_link_open_t
 * @brief     BLE MESH remote provisioning link open data struct definition.
 */
typedef struct {
	uint16_t dst;
	uint16_t net_key_index;
	uint8_t uuid[16];
	uint8_t link_open_timeout;  // Unit in second
} rtk_bt_mesh_remote_prov_client_link_open_t;

/**
 * @typedef   rtk_bt_mesh_rmt_prov_client_link_status_t
 * @brief     BLE MESH remote provisioning link status data struct definition.
 */
typedef struct {
	uint16_t src;
	rtk_bt_mesh_rmt_prov_status_t prov_status;
	rtk_bt_mesh_rmt_prov_link_state_t link_status;
} rtk_bt_mesh_rmt_prov_client_link_status_t;

/**
 * @typedef   rtk_bt_mesh_rmt_prov_client_link_report_t
 * @brief     BLE MESH remote provisioning link report data struct definition.
 */
typedef struct {
	uint16_t src;
	rtk_bt_mesh_rmt_prov_status_t prov_status;
	rtk_bt_mesh_rmt_prov_link_state_t link_status;
	rtk_bt_mesh_rmt_prov_link_close_reason_t close_reason;  // Only exit when the prov link is close
} rtk_bt_mesh_rmt_prov_client_link_report_t;

/* =============================================== Remote provisioning client model relate function definition ======================================= */
/**
 * @defgroup  ble_mesh_remote_provisioning_client_model BT LE Mesh Rempte Provisioning Client Model APIs
 * @brief     BT LE Mesh remote provisioning client model related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_mesh_remote_prov_scan_start(rtk_bt_mesh_remote_prov_client_scan_start_t *rmt_scan)
 * @brief     Send remote scan start message to device, will cause event @ref RTK_BT_MESH_REMOTE_PROV_CLIENT_ACT_SCAN_START
 * @param[in] rmt_scan: remote scan start message param structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_remote_prov_scan_start(rtk_bt_mesh_remote_prov_client_scan_start_t *rmt_scan);

/**
 * @fn        uint16_t rtk_bt_mesh_remote_prov_scan_capa_get(rtk_bt_mesh_remote_prov_client_scan_capa_get_t *scan_capa_get)
 * @brief     Send remote scan capa get message to device, will cause event @ref RTK_BT_MESH_REMOTE_PROV_CLIENT_ACT_SCAN_CAPA_GET
 * @param[in] scan_capa_get: remote scan capability get message param structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_remote_prov_scan_capa_get(rtk_bt_mesh_remote_prov_client_scan_capa_get_t *scan_capa_get);

/**
 * @fn        uint16_t rtk_bt_mesh_remote_prov_link_open(rtk_bt_mesh_remote_prov_client_link_open_t *link_open)
 * @brief     Send remote link open message to device, will cause event @ref RTK_BT_MESH_REMOTE_PROV_CLIENT_ACT_LINK_OPEN
 * @param[in] link_open: remote link open message param structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_remote_prov_link_open(rtk_bt_mesh_remote_prov_client_link_open_t *link_open);

/**
 * @}
 */

#endif  // end of __RTK_BT_MESH_REMOTE_PROVISIONING_H__
