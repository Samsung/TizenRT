/**
 * @file      rtk_bt_mesh_api_generic_onoff_model.h
 * @author    pico_zhai@realsil.com.cn
 * @brief     Bluetooth LE MESH generic on off client and server model data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_MESH_GENERIC_ONOFF_MODEL_H__
#define __RTK_BT_MESH_GENERIC_ONOFF_MODEL_H__

#include <stdint.h>
#include <rtk_bt_mesh_def.h>

/**
 * @typedef   rtk_bt_mesh_generic_onoff_client_model_act_t
 * @brief     BLE MESH generic on off client model act definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_ONOFF_CLIENT_MODEL_ACT_SET = 1,
	RTK_BT_MESH_GENERIC_ONOFF_CLIENT_MODEL_ACT_GET,
	RTK_BT_MESH_GENERIC_ONOFF_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_generic_onoff_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_generic_onoff_client_model_evt_t
 * @brief     BLE MESH generic on off client model event definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_ONOFF_CLIENT_MODEL_EVT_STATUS = 1,
	RTK_BT_MESH_GENERIC_ONOFF_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_onoff_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_onoff_server_model_evt_t
 * @brief     BLE MESH generic on off server model event definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_ONOFF_SERVER_MODEL_EVT_SET = 1,
	RTK_BT_MESH_GENERIC_ONOFF_SERVER_MODEL_EVT_GET,
	RTK_BT_MESH_GENERIC_ONOFF_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_onoff_server_model_evt_t;

/**
 * @typedef rtk_bt_mesh_generic_on_off_t
 * @brief BLE MESH generic on off set data structure
 */
typedef enum {
	RTK_BT_MESH_GENERIC_OFF,
	RTK_BT_MESH_GENERIC_ON
} rtk_bt_mesh_generic_on_off_t;

/**
 * @typedef   rtk_bt_mesh_generic_transition_time_t
 * @brief     BLE MESH generic on off set transition time structure.
 */
typedef struct {
	uint8_t num_steps: 6;
	uint8_t step_resolution: 2;
} _PACKED4_ rtk_bt_mesh_generic_transition_time_t;

/**
 * @typedef   rtk_bt_mesh_generic_onoff_client_act_set_t
 * @brief     BLE MESH generic on off set message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	rtk_bt_mesh_generic_on_off_t on_off;
	uint8_t tid;    //not need be set for customer
	bool optional;    //whether the trans time is existence
	rtk_bt_mesh_generic_transition_time_t trans_time;
	uint8_t delay;
	bool ack;
} rtk_bt_mesh_generic_onoff_client_act_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_onoff_client_act_get_t
 * @brief     BLE MESH generic on off get message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_onoff_client_act_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_onoff_server_evt_set_t
 * @brief     BLE MESH generic on off set evt message structure.
 */
typedef struct {
	rtk_bt_mesh_generic_on_off_t on_off;
	rtk_bt_mesh_generic_transition_time_t total_time;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_generic_onoff_server_evt_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_onoff_server_evt_get_t
 * @brief     BLE MESH generic on off get evt message structure.
 */
typedef struct {
	rtk_bt_mesh_generic_on_off_t *on_off;
} rtk_bt_mesh_generic_onoff_server_evt_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_onoff_client_evt_status_t
 * @brief     BLE MESH generic on off client model set evt message structure.
 */
typedef struct {
	uint16_t src;
	rtk_bt_mesh_generic_on_off_t present_on_off;
	bool optional;
	rtk_bt_mesh_generic_on_off_t target_on_off;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_generic_onoff_client_evt_status_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  ble_mesh_generic_onoff_model BT LE Mesh Generic On Off Model APIs
 * @brief     BT LE Mesh Generic On Off Model related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_mesh_generic_onoff_client_set(rtk_bt_mesh_generic_onoff_client_act_set_t *onoff_set)
 * @brief     Send generic on off set message to generic on off server, will cause event @ref RTK_BT_MESH_GENERIC_ONOFF_CLIENT_MODEL_ACT_SET
 * @param[in] onoff_set: generic on off set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_onoff_client_set(rtk_bt_mesh_generic_onoff_client_act_set_t *onoff_set);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_onoff_client_get(rtk_bt_mesh_generic_onoff_client_act_get_t *onoff_get)
 * @brief     Send generic on off get message to generic on off server, will cause event @ref RTK_BT_MESH_GENERIC_ONOFF_CLIENT_MODEL_ACT_GET
 * @param[in] onoff_get: generic on off get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_onoff_client_get(rtk_bt_mesh_generic_onoff_client_act_get_t *onoff_get);

/**
 * @}
 */

#endif
