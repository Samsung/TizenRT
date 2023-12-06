/**
 * @file      rtk_bt_mesh_scheduler_model.h
 * @author    quinn_yang@realsil.com.cn
 * @brief     Bluetooth LE MESH scheduler client and server model data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */


#ifndef __RTK_BT_MESH_SCHEDULER_MODEL_H__
#define __RTK_BT_MESH_SCHEDULER_MODEL_H__


#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_generic_onoff_model.h>

typedef struct
{
    uint64_t index: 4;
    uint64_t year: 7;
    uint64_t month: 12;
    uint64_t day: 5;
    uint64_t hour: 5;
    uint64_t minute: 6;
    uint64_t second: 6;
    uint64_t day_of_week: 7;
    uint64_t action: 4;
    uint64_t num_steps: 6;
    uint64_t step_resolution: 2;
    uint16_t scene_number;
} _PACKED4_ rtk_bt_mesh_scheduler_register_t;

/**
 * @typedef   rtk_bt_mesh_scheduler_client_model_act_t
 * @brief     BLE MESH scheduler client model act definition.
 */
typedef enum {
	RTK_BT_MESH_SCHEDULER_CLIENT_MODEL_GET = 1,
    RTK_BT_MESH_SCHEDULER_ACTION_CLIENT_MODEL_GET,
    RTK_BT_MESH_SCHEDULER_ACTION_CLIENT_MODEL_SET,
    RTK_BT_MESH_SCHEDULER_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_scheduler_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_scheduler_client_model_evt_t
 * @brief     BLE MESH scheduler client model event definition.
 */
typedef enum {
	RTK_BT_MESH_SCHEDULER_CLIENT_MODEL_STATUS = 1,
    RTK_BT_MESH_SCHEDULER_ACTION_CLIENT_MODEL_STATUS,
    RTK_BT_MESH_SCHEDULER_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_scheduler_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_scheduler_get_t
 * @brief     BLE MESH scheduler get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_scheduler_get_t;

/**
 * @typedef   rtk_bt_mesh_scheduler_action_get_t
 * @brief     BLE MESH scheduler action get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint8_t index;
} rtk_bt_mesh_scheduler_action_get_t;

/**
 * @typedef   rtk_bt_mesh_scheduler_action_set_t
 * @brief     BLE MESH scheduler action set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    rtk_bt_mesh_scheduler_register_t scheduler;
    bool ack;
} rtk_bt_mesh_scheduler_action_set_t;

/**
 * @typedef   rtk_bt_mesh_scheduler_client_status_t
 * @brief     BLE MESH scheduler client model status message structure.
 */
typedef struct
{
    uint16_t src;
    uint16_t schedulers;
} rtk_bt_mesh_scheduler_client_status_t;

/**
 * @typedef   rtk_bt_mesh_scheduler_client_status_action_t
 * @brief     BLE MESH scheduler client model action status message structure.
 */
typedef struct
{
    uint16_t src;
    rtk_bt_mesh_scheduler_register_t scheduler;
} rtk_bt_mesh_scheduler_client_status_action_t;


/**
 * @typedef   rtk_bt_mesh_scheduler_server_model_evt_t
 * @brief     BLE MESH scheduler server model event definition.
 */
typedef enum {
	RTK_BT_MESH_SCHEDULER_SERVER_MODEL_GET = 1,
    RTK_BT_MESH_SCHEDULER_ACTION_SERVER_MODEL_GET,
    RTK_BT_MESH_SCHEDULER_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_scheduler_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_scheduler_server_get_t
 * @brief     BLE MESH scheduler get evt message structure.
 */
typedef struct
{
    uint16_t *schedulers;
} rtk_bt_mesh_scheduler_server_get_t;

/**
 * @typedef   rtk_bt_mesh_scheduler_server_get_action_t
 * @brief     BLE MESH scheduler action set evt message structure.
 */
typedef struct
{
    uint8_t index;
    rtk_bt_mesh_scheduler_register_t *scheduler;
} rtk_bt_mesh_scheduler_server_get_action_t;

/**
 * @typedef   rtk_bt_mesh_scheduler_server_set_action_t
 * @brief     BLE MESH scheduler action set evt message structure.
 */
typedef rtk_bt_mesh_scheduler_register_t rtk_bt_mesh_scheduler_server_set_action_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_setup_server_model_evt_t
 * @brief     BLE MESH light lightness setup server model event definition.
 */
typedef enum {
	RTK_BT_MESH_SCHEDULER_ACTION_SERVER_MODEL_SET = 1,
    RTK_BT_MESH_SCHEDULER_SETUP_SERVER_MODEL_GET_ACTION,
    RTK_BT_MESH_SCHEDULER_SETUP_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_scheduler_setup_server_model_evt_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  ble_mesh_time_model BT LE Mesh Scheduler Model APIs
 * @brief     BT LE Mesh Scheduler Model related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_mesh_scheduler_get(rtk_bt_mesh_scheduler_get_t *scheduler_get)
 * @brief     Send scheduler get message to scheduler server, will cause event @ref RTK_BT_MESH_SCHEDULER_CLIENT_MODEL_GET
 * @param[in] scheduler_get: scheduler get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_scheduler_get(rtk_bt_mesh_scheduler_get_t *scheduler_get);

/**
 * @fn        uint16_t rtk_bt_mesh_scheduler_action_get(rtk_bt_mesh_scheduler_action_get_t *scheduler_action_get)
 * @brief     Send scheduler action get message to scheduler server, will cause event @ref RTK_BT_MESH_SCHEDULER_ACTION_CLIENT_MODEL_GET
 * @param[in] scheduler_action_get: scheduler action get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_scheduler_action_get(rtk_bt_mesh_scheduler_action_get_t *scheduler_action_get);

/**
 * @fn        uint16_t rtk_bt_mesh_scheduler_action_set(rtk_bt_mesh_scheduler_action_set_t *scheduler_action_set)
 * @brief     Send scheduler action set message to scheduler setup server, will cause event @ref RTK_BT_MESH_SCHEDULER_ACTION_CLIENT_MODEL_SET
 * @param[in] scheduler_action_set: scheduler action set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_scheduler_action_set(rtk_bt_mesh_scheduler_action_set_t *scheduler_action_set);

/**
 * @}
 */

#endif /* __RTK_BT_COMMON_H__ */
