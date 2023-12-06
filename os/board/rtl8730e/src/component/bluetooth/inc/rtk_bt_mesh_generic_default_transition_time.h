/**
 * @file      rtk_bt_mesh_generic_default_transition_time_model.h
 * @author    quinn_yang@realsil.com.cn
 * @brief     Bluetooth LE MESH generic client and server model data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_MODEL_H__
#define __RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_MODEL_H__

#include <stdint.h>
#include <rtk_bt_mesh_def.h>

#include <rtk_bt_mesh_generic_onoff_model.h>

/**
 * @typedef   rtk_bt_mesh_generic_default_transition_time_client_model_act_t
 * @brief     BLE MESH generic default transition time client model act definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_GET = 1,
	RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_SET,
	RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_generic_default_transition_time_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_generic_default_transition_time_client_model_evt_t
 * @brief     BLE MESH generic default transition time client model event definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_STATUS = 1,
	RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_default_transition_time_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_default_transition_time_get_t
 * @brief     BLE MESH generic default transition time get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_default_transition_time_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_default_transition_time_set_t
 * @brief     BLE MESH generic default transition time set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    rtk_bt_mesh_generic_transition_time_t trans_time;
    bool ack;
} rtk_bt_mesh_generic_default_transition_time_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_transition_time_client_status_t
 * @brief     BLE MESH generic default transition time client model status evt message structure.
 */
typedef struct {
	uint16_t src;
    rtk_bt_mesh_generic_transition_time_t trans_time;
} rtk_bt_mesh_generic_transition_time_client_status_t;

/**
 * @typedef   rtk_bt_mesh_generic_default_transition_time_server_model_evt_t
 * @brief     BLE MESH generic default transition time server model event definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL_GET = 1,
	RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL_SET,
	RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_default_transition_time_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_default_transition_time_server_set_t
 * @brief     BLE MESH generic default transition time set evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_generic_transition_time_t trans_time;
} rtk_bt_mesh_generic_default_transition_time_server_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_default_transition_time_server_get_t
 * @brief     BLE MESH generic default transition time set evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_generic_transition_time_t *trans_time;
} rtk_bt_mesh_generic_default_transition_time_server_get_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  ble_mesh_generic_default_transition_time_model BT LE Mesh Default Transition Time Model APIs
 * @brief     BT LE Mesh Default Transition Time Model related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */
/**
 * @fn        uint16_t rtk_bt_mesh_generic_default_transition_time_client_get(rtk_bt_mesh_generic_default_transition_time_get_t *time_get)
 * @brief     Send generic default transition time get message to generic default transition time server, will cause event @ref RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_GET
 * @param[in] time_get: generic default transition time get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_default_transition_time_client_get(rtk_bt_mesh_generic_default_transition_time_get_t *time_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_default_transition_time_client_set(rtk_bt_mesh_generic_default_transition_time_set_t *time_set)
 * @brief     Send generic default transition time set message to generic default transition time server, will cause event @ref RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_SET
 * @param[in] time_set: generic default transition time set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_default_transition_time_client_set(rtk_bt_mesh_generic_default_transition_time_set_t *time_set);
/**
 * @}
 */

#endif