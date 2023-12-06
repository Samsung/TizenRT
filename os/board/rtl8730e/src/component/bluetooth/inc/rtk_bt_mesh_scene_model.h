/**
 * @file      rtk_bt_mesh_scene_model.h
 * @author    quinn_yang@realsil.com.cn
 * @brief     Bluetooth LE MESH scene client and server model data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_MESH_SCENE_MODEL_H__
#define __RTK_BT_MESH_SCENE_MODEL_H__


#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_generic_onoff_model.h>

#define SCENE_REGISTER_ARRAY_DATA_MAX_LEN    0x10
#define SCENE_DATA_MAX_LEN    0x10

typedef enum
{
    RTK_BT_MESH_SCENE_STATUS_SUCCESS,
    RTK_BT_MESH_SCENE_STATUS_REGISTER_FULL,
    RTK_BT_MESH_SCENE_STATUS_NOT_FOUND
// }uint8_t rtk_bt_mesh_scene_status_code_t;
}rtk_bt_mesh_scene_status_code_t;

/**
 * @typedef   rtk_bt_mesh_scene_client_model_act_t
 * @brief     BLE MESH scene client model act definition.
 */
typedef enum {
	RTK_BT_MESH_SCENE_CLIENT_MODEL_STORE = 1,
    RTK_BT_MESH_SCENE_CLIENT_MODEL_RECALL,
    RTK_BT_MESH_SCENE_CLIENT_MODEL_GET,
    RTK_BT_MESH_SCENE_CLIENT_MODEL_REGISTER_GET,
    RTK_BT_MESH_SCENE_CLIENT_MODEL_DELETE,
    RTK_BT_MESH_SCENE_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_scene_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_scene_client_model_act_t
 * @brief     BLE MESH scene client model act definition.
 */
typedef enum {
	RTK_BT_MESH_SCENE_CLIENT_MODEL_STATUS = 1,
    RTK_BT_MESH_SCENE_CLIENT_MODEL_REGISTER_STATUS,
    RTK_BT_MESH_SCENE_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_scene_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_scene_store_t
 * @brief     BLE MESH scene store act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t scene_number;
    bool ack;
} rtk_bt_mesh_scene_store_t;

/**
 * @typedef   rtk_bt_mesh_scene_recall_t
 * @brief     BLE MESH scene recall act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t scene_number;
    uint8_t tid;
    bool optional;
    rtk_bt_mesh_generic_transition_time_t trans_time;
    uint8_t delay;
    bool ack;
} rtk_bt_mesh_scene_recall_t;

/**
 * @typedef   rtk_bt_mesh_scene_get_t
 * @brief     BLE MESH scene get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_scene_get_t;

/**
 * @typedef   rtk_bt_mesh_scene_register_get_t
 * @brief     BLE MESH scene register get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_scene_register_get_t;

/**
 * @typedef   rtk_bt_mesh_scene_delete_t
 * @brief     BLE MESH scene delete act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t scene_number;
    bool ack;
} rtk_bt_mesh_scene_delete_t;

/**
 * @typedef   rtk_bt_mesh_time_client_status_t
 * @brief     BLE MESH time client model status message structure.
 */
typedef struct
{
    uint16_t src;
    rtk_bt_mesh_scene_status_code_t status;
    uint16_t current_scene;
    bool optional;
    uint16_t target_scene;
    rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_scene_client_status_t;

typedef struct
{
    uint16_t src;
    rtk_bt_mesh_scene_status_code_t status;
    uint16_t current_scene;
    uint8_t scene_array_len;
    uint16_t *scene_array;// Indication!!!
} rtk_bt_mesh_scene_client_register_status_t;


/*************scene server part*********************/
/**
 * @typedef   rtk_bt_mesh_scene_server_model_evt_t
 * @brief     BLE MESH scene server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_SCENE_SERVER_MODEL_GET = 1,
    RTK_BT_MESH_SCENE_SERVER_MODEL_RECALL,
    RTK_BT_MESH_SCENE_SERVER_MODEL_GET_REGISTER_STATUS,
    RTK_BT_MESH_SCENE_SERVER_MODEL_GET_STATUS_RECALL,
    RTK_BT_MESH_SCENE_SERVER_MODEL_GET_SCENES,
    RTK_BT_MESH_SCENE_SERVER_MODEL_GET_SCENES_NUM,
    RTK_BT_MESH_SCENE_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_scene_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_scene_server_model_act_t
 * @brief     BLE MESH scene server model act definition.
 */
typedef enum {
    RTK_BT_MESH_SCENE_SERVER_MODEL_ACT_DB_SET = 1,
	RTK_BT_MESH_SCENE_SERVER_MODEL_ACT_MAX,
} rtk_bt_mesh_scene_server_model_act_t;

/**
 * @typedef   rtk_bt_mesh_scene_storage_memory_t
 * @brief     BLE MESH scene server database structure.
 */
typedef struct
{
    uint16_t scene_number;
    void *pmemory;
} rtk_bt_mesh_scene_storage_memory_t;

/**
 * @typedef   rtk_bt_mesh_scene_server_db_set_t
 * @brief     BLE MESH scene server database set message structure.
 */
typedef struct {
	uint16_t value_len;
	uint8_t p_data[SCENE_DATA_MAX_LEN * 4];
} rtk_bt_mesh_scene_server_db_set_t;

/**
 * @typedef   rtk_bt_mesh_scene_server_get_t
 * @brief     BLE MESH scene get evt message structure.
 */
typedef struct
{
    uint16_t *current_scene;
} rtk_bt_mesh_scene_server_get_t;

/**
 * @typedef   rtk_bt_mesh_scene_server_get_register_status_t
 * @brief     BLE MESH scene get register status evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_scene_status_code_t *status;
} rtk_bt_mesh_scene_server_get_register_status_t;

/**
 * @typedef   rtk_bt_mesh_scene_server_get_default_transition_time_t
 * @brief     BLE MESH scene get default transition time evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_generic_transition_time_t trans_time;
} rtk_bt_mesh_scene_server_get_default_transition_time_t;

/**
 * @typedef   rtk_bt_mesh_scene_server_store_t
 * @brief     BLE MESH scene store evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_scene_status_code_t status;
    uint16_t scene_number;
    uint8_t *pmemory;
} rtk_bt_mesh_scene_server_store_t;

/**
 * @typedef   rtk_bt_mesh_scene_server_recall_t
 * @brief     BLE MESH scene recall evt message structure.
 */
typedef struct
{
    uint16_t scene_number;
    rtk_bt_mesh_generic_transition_time_t total_time;
    rtk_bt_mesh_generic_transition_time_t remaining_time;
    uint8_t *pmemory;
} rtk_bt_mesh_scene_server_recall_t;

/**
 * @typedef   rtk_bt_mesh_scene_server_delete_t
 * @brief     BLE MESH scene delete evt message structure.
 */
typedef struct
{
    uint16_t scene_number;
} rtk_bt_mesh_scene_server_delete_t;

typedef struct {
    uint16_t scene_number;
	rtk_bt_mesh_scene_status_code_t *status_recall;
} rtk_bt_mesh_scene_server_get_status_recall_t;

typedef struct {
    uint16_t num_scenes;
	uint16_t *scenes; //variable length
} rtk_bt_mesh_scene_server_get_scenes_t;

typedef struct {
	uint16_t *num_scenes;
} rtk_bt_mesh_scene_server_get_scenes_num_t;

/**
 * @typedef   rtk_bt_mesh_scene_setup_server_model_evt_t
 * @brief     BLE MESH scene setup server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_STORE = 1,
    RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_DELETE,
    RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET,
    RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET_STATUS_REGISTER,
    RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET_SCENES,
    RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET_SCENES_NUM,
    RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_scene_setup_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_scene_setup_server_model_act_t
 * @brief     BLE MESH scene setup server model act definition.
 */
typedef enum {
    RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_ACT_DB_SET = 1,
	RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_ACT_MAX,
} rtk_bt_mesh_scene_setup_server_model_act_t;

/**
 * @typedef   rtk_bt_mesh_scene_setup_server_get_register_status_t
 * @brief     BLE MESH scene get register status evt message structure.
 */
typedef struct
{
    uint16_t scene_number;
    rtk_bt_mesh_scene_status_code_t *status;
} rtk_bt_mesh_scene_setup_server_get_register_status_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  ble_mesh_scene_model BT LE Mesh Scene Model APIs
 * @brief     BT LE Mesh SCENE Model related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */
/**
 * @fn        uint16_t rtk_bt_mesh_scene_store(rtk_bt_mesh_scene_store_t *scene_client_store)
 * @brief     Send scene store message to scene setup server, will cause event @ref RTK_BT_MESH_SCENE_CLIENT_MODEL_STORE
 * @param[in] scene_client_store: scene store message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_scene_store(rtk_bt_mesh_scene_store_t *scene_client_store);

/**
 * @fn        uint16_t rtk_bt_mesh_scene_recall(rtk_bt_mesh_scene_recall_t *scene_client_recall)
 * @brief     Send scene recall message to scene server, will cause event @ref RTK_BT_MESH_SCENE_CLIENT_MODEL_RECALL
 * @param[in] scene_client_recall: scene recall message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_scene_recall(rtk_bt_mesh_scene_recall_t *scene_client_recall);

/**
 * @fn        uint16_t rtk_bt_mesh_scene_get(rtk_bt_mesh_scene_get_t *scene_client_get)
 * @brief     Send scene get message to scene server, will cause event @ref RTK_BT_MESH_SCENE_CLIENT_MODEL_GET
 * @param[in] scene_client_get: scene get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_scene_get(rtk_bt_mesh_scene_get_t *scene_client_get);

/**
 * @fn        uint16_t rtk_bt_mesh_scene_register_get(rtk_bt_mesh_scene_register_get_t *scene_register_get)
 * @brief     Send scene register get message to scene server, will cause event @ref RTK_BT_MESH_SCENE_CLIENT_MODEL_REGISTER_GET
 * @param[in] scene_register_get: scene register get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_scene_register_get(rtk_bt_mesh_scene_register_get_t *scene_register_get);

/**
 * @fn        uint16_t rtk_bt_mesh_scene_delete(rtk_bt_mesh_scene_delete_t *scene_delete);
 * @brief     Send scene delete message to scene setup server, will cause event @ref RTK_BT_MESH_SCENE_CLIENT_MODEL_DELETE
 * @param[in] scene_delete: scene delete message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_scene_delete(rtk_bt_mesh_scene_delete_t *scene_delete);

/**
 * @fn        uint16_t rtk_bt_mesh_scene_server_db_set(rtk_bt_mesh_scene_server_db_set_t *db_set)
 * @brief     Send scene database set message to set scene database, will cause event @ref RTK_BT_MESH_SCENE_SERVER_MODEL_ACT_DB_SET
 * @param[in] db_set: scene database set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_scene_server_db_set(rtk_bt_mesh_scene_server_db_set_t *db_set);

/**
 * @fn        uint16_t rtk_bt_mesh_scene_setup_server_db_set(rtk_bt_mesh_scene_server_db_set_t *db_set)
 * @brief     Send scene setup database set message to set scene setup database, will cause event @ref RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_ACT_DB_SET
 * @param[in] db_set: scene setup database set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_scene_setup_server_db_set(rtk_bt_mesh_scene_server_db_set_t *db_set);

/**
 * @}
 */

#endif