/**
 * @file      rtk_bt_mesh_sensor_model.h
 * @author    quinn_yang@realsil.com.cn
 * @brief     Bluetooth LE MESH sensor client and server model data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */


#ifndef __RTK_BT_MESH_SENSOR_MODEL_H__
#define __RTK_BT_MESH_SENSOR_MODEL_H__

#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_generic_onoff_model.h>

#define SENSOR_TRIGGER_DATA_MAX_LEN    0x10
#define SENSOR_CADENCE_DATA_MAX_LEN    0x10
#define SENSOR_SETTING_DATA_MAX_LEN    0x10
#define SENSOR_SETTINGS_DATA_MAX_LEN   0x10
#define SENSOR_COLUMN_DATA_MAX_LEN     0x10
#define SENSOR_GET_DATA_MAX_LEN        0x10
#define SENSOR_GET_COLUMN_MAX_LEN      0x10
#define SENSOR_GET_SERIES_MAX_LEN      0x10

typedef enum
{
    RTK_BT_MESH_SENSOR_SETTING_ACCESS_READ_ONLY = 0x01,
    RTK_BT_MESH_SENSOR_SETTING_ACCESS_READ_WRITE = 0x03,
} rtk_bt_mesh_sensor_setting_access_t;

typedef struct
{
    uint8_t raw_value_len;
    uint8_t fast_cadence_period_divisor: 7;
    uint8_t status_trigger_type: 1; /**< @ref sensor_trigger_type_t */
    void *status_trigger_delta_down;
    void *status_trigger_delta_up;
    uint8_t status_min_interval;
    void *fast_cadence_low;
    void *fast_cadence_high;
} _PACKED4_ rtk_bt_mesh_sensor_cadence_t;

typedef struct
{
    uint16_t setting_property_id;
    rtk_bt_mesh_sensor_setting_access_t setting_access;
    uint8_t setting_raw_len;
    void *setting_raw;
} _PACKED4_ rtk_bt_mesh_sensor_setting_t;

typedef enum
{
    RTK_BT_MESH_SENSOR_SAMPLING_UNSPECIFIED,
    RTK_BT_MESH_SENSOR_SAMPLING_INSTANTANEOUS,
    RTK_BT_MESH_SENSOR_SAMPLING_ARITHMETIC_MEAN,
    RTK_BT_MESH_SENSOR_SAMPLING_RMS,
    RTK_BT_MESH_SENSOR_SAMPLING_MAXIMUM,
    RTK_BT_MESH_SENSOR_SAMPLING_MINIMUM,
    RTK_BT_MESH_SENSOR_SAMPLING_ACCUMULATED,
    RTK_BT_MESH_SENSOR_SAMPLING_COUNT,
} rtk_bt_mesh_sensor_sampling_function_t;

typedef struct
{
    uint16_t property_id;
    uint32_t positive_tolerance: 12;
    uint32_t negative_tolerance: 12;
    uint32_t sampling_function: 8; /**< @ref sensor_sampling_function_t */
    uint8_t measurement_period;
    uint8_t update_interval;
} _PACKED4_ rtk_bt_mesh_sensor_descriptor_t;

/**
 * @typedef   rtk_bt_mesh_sensor_client_model_act_t
 * @brief     BLE MESH sensor client model act definition.
 */
typedef enum {
	RTK_BT_MESH_SENSOR_DESCRIPTOR_CLIENT_MODEL_GET = 1,
    RTK_BT_MESH_SENSOR_CADENCE_CLIENT_MODEL_GET,
    RTK_BT_MESH_SENSOR_CADENCE_CLIENT_MODEL_SET,
    RTK_BT_MESH_SENSOR_SETTINGS_CLIENT_MODEL_GET,
    RTK_BT_MESH_SENSOR_SETTING_CLIENT_MODEL_GET,
    RTK_BT_MESH_SENSOR_SETTING_CLIENT_MODEL_SET,
    RTK_BT_MESH_SENSOR_CLIENT_MODEL_GET,
    RTK_BT_MESH_SENSOR_COLUMN_CLIENT_MODEL_GET,
    RTK_BT_MESH_SENSOR_SERIES_CLIENT_MODEL_GET,
    RTK_BT_MESH_SENSOR_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_sensor_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_sensor_client_model_evt_t
 * @brief     BLE MESH sensor client model event definition.
 */
typedef enum {
	RTK_BT_MESH_SENSOR_DESCRIPTOR_CLIENT_MODEL_STATUS = 1,
    RTK_BT_MESH_SENSOR_CADENCE_CLIENT_MODEL_STATUS,
    RTK_BT_MESH_SENSOR_SETTINGS_CLIENT_MODEL_STATUS,
    RTK_BT_MESH_SENSOR_SETTING_CLIENT_MODEL_STATUS,
    RTK_BT_MESH_SENSOR_CLIENT_MODEL_STATUS,
    RTK_BT_MESH_SENSOR_COLUMN_CLIENT_MODEL_STATUS,
    RTK_BT_MESH_SENSOR_SERIES_CLIENT_MODEL_STATUS,
    RTK_BT_MESH_SENSOR_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_sensor_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_sensor_descriptor_get_t
 * @brief     BLE MESH sensor descriptor get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
} rtk_bt_mesh_sensor_descriptor_get_t;

/**
 * @typedef   rtk_bt_mesh_sensor_cadence_get_t
 * @brief     BLE MESH sensor cadence get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
} rtk_bt_mesh_sensor_cadence_get_t;

/**
 * @typedef   rtk_bt_mesh_sensor_cadence_set_t
 * @brief     BLE MESH sensor cadence set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
    bool ack;
    uint8_t fast_cadence_period_divisor: 7;
    uint8_t status_trigger_type: 1;
    uint8_t raw_value_len;
    uint8_t trigger_down[SENSOR_TRIGGER_DATA_MAX_LEN];
    uint8_t trigger_up[SENSOR_TRIGGER_DATA_MAX_LEN];
    uint8_t status_min_interval;
    uint8_t cadence_low[SENSOR_CADENCE_DATA_MAX_LEN];
    uint8_t cadence_high[SENSOR_CADENCE_DATA_MAX_LEN];
} rtk_bt_mesh_sensor_cadence_set_t;

/**
 * @typedef   rtk_bt_mesh_sensor_settings_get_t
 * @brief     BLE MESH sensor settings get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
} rtk_bt_mesh_sensor_settings_get_t;

/**
 * @typedef   rtk_bt_mesh_sensor_setting_get_t
 * @brief     BLE MESH sensor setting get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
    uint16_t setting_property_id;
} rtk_bt_mesh_sensor_setting_get_t;

/**
 * @typedef   rtk_bt_mesh_sensor_setting_set_t
 * @brief     BLE MESH sensor setting get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
    uint16_t setting_property_id;
    bool ack;
    uint8_t setting_raw_len;
    uint8_t setting_raw[SENSOR_SETTING_DATA_MAX_LEN];
} rtk_bt_mesh_sensor_setting_set_t;

/**
 * @typedef   rtk_bt_mesh_sensor_get_t
 * @brief     BLE MESH sensor get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
} rtk_bt_mesh_sensor_get_t;

/**
 * @typedef   rtk_bt_mesh_sensor_column_get_t
 * @brief     BLE MESH sensor get column act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
    uint8_t raw_value_x_len;
    uint8_t raw_value_x[SENSOR_COLUMN_DATA_MAX_LEN];
} rtk_bt_mesh_sensor_column_get_t;

/**
 * @typedef   rtk_bt_mesh_sensor_series_get_t
 * @brief     BLE MESH sensor get series act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
    uint8_t raw_value_x_len;
    uint8_t raw_value_x1[SENSOR_COLUMN_DATA_MAX_LEN];
    uint8_t raw_value_x2[SENSOR_COLUMN_DATA_MAX_LEN];
} rtk_bt_mesh_sensor_series_get_t;

/**
 * @typedef   rtk_bt_mesh_sensor_client_status_descriptor_t
 * @brief     BLE MESH sensor client model descriptor status message structure.
 */
typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint16_t num_descriptors;
    rtk_bt_mesh_sensor_descriptor_t *descriptors;
} rtk_bt_mesh_sensor_client_status_descriptor_t;

/**
 * @typedef   rtk_bt_mesh_sensor_client_status_cadence_t
 * @brief     BLE MESH sensor client model cadence status message structure.
 */
typedef struct
{
    uint16_t src;
    uint16_t property_id;
    bool flag;
    rtk_bt_mesh_sensor_cadence_t *cadence;
} rtk_bt_mesh_sensor_client_status_cadence_t;

/**
 * @typedef   rtk_bt_mesh_sensor_client_status_settings_t
 * @brief     BLE MESH sensor client model settings status message structure.
 */
typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint16_t num_ids;
    uint16_t *setting_ids;
} rtk_bt_mesh_sensor_client_status_settings_t;

/**
 * @typedef   rtk_bt_mesh_sensor_client_status_setting_t
 * @brief     BLE MESH sensor client model setting status message structure.
 */
typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint16_t setting_property_id;
    bool flag;
    rtk_bt_mesh_sensor_setting_t *setting;
} rtk_bt_mesh_sensor_client_status_setting_t;

/**
 * @typedef   rtk_bt_mesh_sensor_client_status_t
 * @brief     BLE MESH sensor client model status message structure.
 */
typedef struct
{
    uint16_t src;
    uint16_t marshalled_sensor_data_len;
    void *marshalled_sensor_data;
} rtk_bt_mesh_sensor_client_status_t;

/**
 * @typedef   rtk_bt_mesh_sensor_client_status_t
 * @brief     BLE MESH sensor client model status message structure.
 */
typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint16_t column_raw_value_len;
    void *column_raw_value;
} rtk_bt_mesh_sensor_client_status_column_t;

/**
 * @typedef   rtk_bt_mesh_sensor_client_status_series_t
 * @brief     BLE MESH sensor client model series status message structure.
 */
typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint16_t series_raw_value_len;
    void *series_raw_value;
} rtk_bt_mesh_sensor_client_status_series_t;

/*****************server*********************/
/**
 * @typedef   rtk_bt_mesh_sensor_server_model_evt_t
 * @brief     BLE MESH sensor server model event definition.
 */
typedef enum {
	RTK_BT_MESH_SENSOR_SERVER_MODEL_GET = 1,
    RTK_BT_MESH_SENSOR_SERVER_MODEL_COLUMN_GET,
    RTK_BT_MESH_SENSOR_SERVER_MODEL_SERIES_GET,
    RTK_BT_MESH_SENSOR_SERVER_MODEL_COMPARE_CADENCE,
    RTK_BT_MESH_SENSOR_SERVER_MODEL_DESCRIPTOR_NUM_GET,
    RTK_BT_MESH_SENSOR_SERVER_MODEL_DESCRIPTOR_GET,
    RTK_BT_MESH_SENSOR_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_sensor_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_model_act_t
 * @brief     BLE MESH sensor server model act definition.
 */
typedef enum {
    RTK_BT_MESH_SENSOR_SERVER_MODEL_ACT_DB_SET = 1,
	RTK_BT_MESH_SENSOR_SERVER_MODEL_ACT_MAX,
} rtk_bt_mesh_sensor_server_model_act_t;

/**
 * @typedef   rtk_bt_mesh_sensor_db_t
 * @brief     BLE MESH sensor server database structure.
 */
typedef struct
{
    rtk_bt_mesh_sensor_descriptor_t descriptor;
    uint8_t sensor_raw_data_len;
    uint16_t num_settings;
    rtk_bt_mesh_sensor_setting_t *settings;
    rtk_bt_mesh_sensor_cadence_t *cadence;
    int32_t pub_count;
} rtk_bt_mesh_sensor_db_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_db_set_t
 * @brief     BLE MESH sensor server database set message structure.
 */
typedef struct {
	uint16_t value_len;
	uint8_t p_data[SENSOR_GET_DATA_MAX_LEN * 4];
} rtk_bt_mesh_sensor_server_db_set_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_get_t
 * @brief     BLE MESH sensor server model get message structure.
 */
typedef struct
{
    uint16_t property_id;
    /* app need to modify pointer blew, default is NULL */
    void *raw_data;
} rtk_bt_mesh_sensor_server_get_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_get_column_t
 * @brief     BLE MESH sensor server model get column message structure.
 */
typedef struct
{
    uint16_t property_id;
    uint8_t raw_value_x_len;
    const void *raw_value_x;
    /* app need to modify pointer blew, default is NULL */
    uint16_t column_len;
    /* contains raw value x, column width and raw value y */
    void *column;
} rtk_bt_mesh_sensor_server_get_column_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_direct_get_column_t
 * @brief     BLE MESH sensor server model get column for direct calling message structure.
 */
typedef struct
{
    uint16_t property_id;
    uint8_t raw_value_x_len;
    uint8_t raw_value_x[SENSOR_COLUMN_DATA_MAX_LEN];
    void *value;
} rtk_bt_mesh_sensor_server_direct_get_column_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_get_series_t
 * @brief     BLE MESH sensor server model get series message structure.
 */
typedef struct
{
    uint16_t property_id;
    uint8_t raw_value_x_len;
    const void *raw_value_x1;
    const void *raw_value_x2;
    /* app need to modify pointer blew, default is NULL */
    uint16_t series_len;
    /* contains n raw value x, column width and raw value y */
    void *series;
} rtk_bt_mesh_sensor_server_get_series_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_direct_get_series_t
 * @brief     BLE MESH sensor server model get series for direct calling message structure.
 */
typedef struct
{
    uint16_t property_id;
    uint8_t raw_value_x_len;
    uint8_t raw_value_x1[SENSOR_GET_SERIES_MAX_LEN];
    uint8_t raw_value_x2[SENSOR_GET_SERIES_MAX_LEN];
    void *value;
} rtk_bt_mesh_sensor_server_direct_get_series_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_get_cadence_t
 * @brief     BLE MESH sensor server model get cadence message structure.
 */
typedef struct
{
    uint16_t property_id;
    /*
     * just notify new cadence value in database,
     * do not need to modify it, just use it if needed
     */
    uint8_t *cadence;
} rtk_bt_mesh_sensor_server_get_cadence_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_set_cadence_t
 * @brief     BLE MESH sensor server model set cadence message structure.
 */
typedef struct
{
    uint16_t property_id;
    /*
     * just notify new cadence value in database,
     * do not need to modify it, just use it if needed
     */
    rtk_bt_mesh_sensor_cadence_t *cadence;
} rtk_bt_mesh_sensor_server_set_cadence_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_set_setting_t
 * @brief     BLE MESH sensor server model set setting message structure.
 */
typedef struct
{
    uint16_t property_id;
    /*
     * just notify new setting value in database,
     * do not need to modify it, just use it if needed
     */
    rtk_bt_mesh_sensor_setting_t *setting;
} rtk_bt_mesh_sensor_server_set_setting_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_get_settings_t
 * @brief     BLE MESH sensor server model get settings message structure.
 */
typedef struct {
	uint16_t property_id;
	uint16_t *settings_data;// reserve 2 bytes to store setting num
} rtk_bt_mesh_sensor_server_get_settings_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_get_setting_t
 * @brief     BLE MESH sensor server model get setting message structure.
 */
typedef struct {
	uint16_t property_id;
	uint16_t setting_property_id;
	uint8_t *setting_data;
} rtk_bt_mesh_sensor_server_get_setting_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_compare_cadence_t
 * @brief     BLE MESH sensor server model compare cadence message structure.
 */
typedef struct
{
    uint16_t property_id;
    bool *need_fast_divisor;
} rtk_bt_mesh_sensor_server_compare_cadence_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_get_descriptor_num_t
 * @brief     BLE MESH sensor server model get descriptor number message structure.
 */
typedef struct {
	uint16_t property_id; //0 means get all
	uint16_t *descriptor_num; //0 means NULL
} rtk_bt_mesh_sensor_server_get_descriptor_num_t;

/**
 * @typedef   rtk_bt_mesh_sensor_server_get_descriptor_t
 * @brief     BLE MESH sensor server model get descriptor message structure.
 */
typedef struct {
	uint16_t property_id; //0 means get all
	uint16_t descriptor_num; //0 means NULL
    rtk_bt_mesh_sensor_descriptor_t *descriptor;
} rtk_bt_mesh_sensor_server_get_descriptor_t;

/**
 * @typedef   rtk_bt_mesh_sensor_setup_server_model_evt_t
 * @brief     BLE MESH sensor setup server model event definition.
 */
typedef enum {
    RTK_BT_MESH_SENSOR_SERVER_MODEL_CADENCE_SET = 1,
    RTK_BT_MESH_SENSOR_SERVER_MODEL_SETTING_SET,
    RTK_BT_MESH_SENSOR_SERVER_MODEL_CADENCE_GET,
    RTK_BT_MESH_SENSOR_SERVER_MODEL_SETTINGS_GET,
    RTK_BT_MESH_SENSOR_SERVER_MODEL_SETTING_GET,
    RTK_BT_MESH_SENSOR_SETUP_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_sensor_setup_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_sensor_setup_server_model_act_t
 * @brief     BLE MESH sensor setup server model act definition.
 */
typedef enum {
    RTK_BT_MESH_SENSOR_SETUP_SERVER_MODEL_ACT_DB_SET = 1,
	RTK_BT_MESH_SENSOR_SETUP_SERVER_MODEL_ACT_MAX,
} rtk_bt_mesh_sensor_setup_server_model_act_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  ble_mesh_sensor_model BT LE Mesh Sensor Model APIs
 * @brief     BT LE Mesh Sensor Model related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_mesh_sensor_descriptor_get(rtk_bt_mesh_sensor_descriptor_get_t *sensor_descriptor_get)
 * @brief     Send sensor descriptor get message to sensor server, will cause event @ref RTK_BT_MESH_SENSOR_DESCRIPTOR_CLIENT_MODEL_GET
 * @param[in] sensor_descriptor_get: sensor descriptor get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_sensor_descriptor_get(rtk_bt_mesh_sensor_descriptor_get_t *sensor_descriptor_get);

/**
 * @fn        uint16_t rtk_bt_mesh_sensor_cadence_get(rtk_bt_mesh_sensor_cadence_get_t *sensor_cadence_get)
 * @brief     Send sensor cadence get message to sensor setup server, will cause event @ref RTK_BT_MESH_SENSOR_CADENCE_CLIENT_MODEL_GET
 * @param[in] sensor_cadence_get: sensor cadence get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_sensor_cadence_get(rtk_bt_mesh_sensor_cadence_get_t *sensor_cadence_get);

/**
 * @fn        uint16_t rtk_bt_mesh_sensor_cadence_set(rtk_bt_mesh_sensor_cadence_set_t *sensor_cadence_set)
 * @brief     Send sensor cadence set message to sensor setup server, will cause event @ref RTK_BT_MESH_SENSOR_CADENCE_CLIENT_MODEL_SET
 * @param[in] sensor_cadence_set: sensor cadence set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_sensor_cadence_set(rtk_bt_mesh_sensor_cadence_set_t *sensor_cadence_set);

/**
 * @fn        uint16_t rtk_bt_mesh_sensor_settings_get(rtk_bt_mesh_sensor_settings_get_t *sensor_settings_get)
 * @brief     Send sensor settings get message to sensor setup server, will cause event @ref RTK_BT_MESH_SENSOR_SETTINGS_CLIENT_MODEL_GET
 * @param[in] sensor_settings_get: sensor settings get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_sensor_settings_get(rtk_bt_mesh_sensor_settings_get_t *sensor_settings_get);

/**
 * @fn        uint16_t rtk_bt_mesh_sensor_setting_get(rtk_bt_mesh_sensor_setting_get_t *sensor_setting_get)
 * @brief     Send sensor setting get message to sensor setup server, will cause event @ref RTK_BT_MESH_SENSOR_SETTING_CLIENT_MODEL_GET
 * @param[in] sensor_setting_get: sensor setting get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_sensor_setting_get(rtk_bt_mesh_sensor_setting_get_t *sensor_setting_get);

/**
 * @fn        uint16_t rtk_bt_mesh_sensor_setting_set(rtk_bt_mesh_sensor_setting_set_t *sensor_setting_set)
 * @brief     Send sensor setting set message to sensor setup server, will cause event @ref RTK_BT_MESH_SENSOR_SETTING_CLIENT_MODEL_SET
 * @param[in] sensor_setting_set: sensor setting set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_sensor_setting_set(rtk_bt_mesh_sensor_setting_set_t *sensor_setting_set);

/**
 * @fn        uint16_t rtk_bt_mesh_sensor_get(rtk_bt_mesh_sensor_get_t *sensor_get)
 * @brief     Send sensor get message to sensor server, will cause event @ref RTK_BT_MESH_SENSOR_CLIENT_MODEL_GET
 * @param[in] sensor_get: sensor get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_sensor_get(rtk_bt_mesh_sensor_get_t *sensor_get);

/**
 * @fn        uint16_t rtk_bt_mesh_sensor_column_get(rtk_bt_mesh_sensor_column_get_t *sensor_column_get)
 * @brief     Send sensor column get message to sensor server, will cause event @ref RTK_BT_MESH_SENSOR_COLUMN_CLIENT_MODEL_GET
 * @param[in] sensor_column_get: sensor column get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_sensor_column_get(rtk_bt_mesh_sensor_column_get_t *sensor_column_get);

/**
 * @fn        uint16_t rtk_bt_mesh_sensor_series_get(rtk_bt_mesh_sensor_series_get_t *sensor_series_get)
 * @brief     Send sensor series get message to sensor server, will cause event @ref RTK_BT_MESH_SENSOR_SERIES_CLIENT_MODEL_GET
 * @param[in] sensor_series_get: sensor series get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_sensor_series_get(rtk_bt_mesh_sensor_series_get_t *sensor_series_get);

/**
 * @fn        uint16_t rtk_bt_mesh_sensor_server_db_set(rtk_bt_mesh_sensor_server_db_set_t *db_set)
 * @brief     Send sensor database set message to set sensor database, will cause event @ref RTK_BT_MESH_SENSOR_SERVER_MODEL_ACT_DB_SET
 * @param[in] db_set: sensor database set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_sensor_server_db_set(rtk_bt_mesh_sensor_server_db_set_t *db_set);

/**
 * @fn        uint16_t rtk_bt_mesh_sensor_setup_server_db_set(rtk_bt_mesh_sensor_server_db_set_t *db_set)
 * @brief     Send sensor setup database set message to set sensor setup database, will cause event @ref RTK_BT_MESH_SENSOR_SETUP_SERVER_MODEL_ACT_DB_SET
 * @param[in] db_set: sensor setup database set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_sensor_setup_server_db_set(rtk_bt_mesh_sensor_server_db_set_t *db_set);

/**
 * @}
 */

#endif