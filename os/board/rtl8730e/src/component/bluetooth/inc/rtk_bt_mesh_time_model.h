/**
 * @file      rtk_bt_mesh_time_model.h
 * @author    quinn_yang@realsil.com.cn
 * @brief     Bluetooth LE MESH time client and server model data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_MESH_TIME_MODEL_H__
#define __RTK_BT_MESH_TIME_MODEL_H__

#include <rtk_bt_mesh_def.h>

/****************common********************/
typedef struct
{
    uint8_t tai_seconds[5]; /* little endian */
    uint8_t subsecond;
    uint8_t uncertainty;
    uint16_t time_authority: 1;
    uint16_t tai_utc_delta: 15;
    uint8_t time_zone_offset;
} _PACKED4_ rtk_bt_mesh_tai_time_t;

typedef struct
{
    uint8_t time_zone_offset_current;
    uint8_t time_zone_offset_new;
    uint8_t tai_of_zone_change[5]; /* little endian */
} _PACKED4_ rtk_bt_mesh_time_zone_t;

typedef struct
{
    uint16_t tai_utc_delta_current: 15;
    uint16_t padding1: 1; /* always 0*/
    uint16_t tai_utc_delta_new: 15;
    uint16_t padding2: 1; /* always 0*/
    uint8_t tai_of_delta_change[5]; /* little endian */
} _PACKED4_ rtk_bt_mesh_tai_utc_delta_t;

typedef enum
{
    RTK_BT_MESH_TIME_ROLE_NONE,
    RTK_BT_MESH_TIME_ROLE_AUTHORITY,
    RTK_BT_MESH_TIME_ROLE_RELAY,
    RTK_BT_MESH_TIME_ROLE_CLIENT
} rtk_bt_mesh_time_role_t;

/************time client model*****************/
/**
 * @typedef   rtk_bt_mesh_time_client_model_act_t
 * @brief     BLE MESH time client model act definition.
 */
typedef enum {
	RTK_BT_MESH_TIME_CLIENT_MODEL_GET = 1,
    RTK_BT_MESH_TIME_CLIENT_MODEL_ZONE_GET,
    RTK_BT_MESH_TIME_CLIENT_MODEL_TAI_UTC_DELTA_GET,
    RTK_BT_MESH_TIME_CLIENT_MODEL_ROLE_GET,
    RTK_BT_MESH_TIME_CLIENT_MODEL_SET,
    RTK_BT_MESH_TIME_CLIENT_MODEL_ZONE_SET,
    RTK_BT_MESH_TIME_CLIENT_MODEL_TAI_UTC_DELTA_SET,
    RTK_BT_MESH_TIME_CLIENT_MODEL_ROLE_SET,
    RTK_BT_MESH_TIME_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_time_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_time_client_model_evt_t
 * @brief     BLE MESH time client model evt definition.
 */
typedef enum {
	RTK_BT_MESH_TIME_CLIENT_MODEL_STATUS = 1,
    RTK_BT_MESH_TIME_CLIENT_MODEL_ROLE_STATUS,
    RTK_BT_MESH_TIME_CLIENT_MODEL_ZONE_STATUS,
    RTK_BT_MESH_TIME_CLIENT_MODEL_TAI_UTC_DELTA_STATUS,
    RTK_BT_MESH_TIME_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_time_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_time_get_t
 * @brief     BLE MESH time get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_time_get_t;

/**
 * @typedef   rtk_bt_mesh_time_zone_get_t
 * @brief     BLE MESH time zone get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_time_zone_get_t;

/**
 * @typedef   rtk_bt_mesh_time_tai_utc_delta_get_t
 * @brief     BLE MESH time tai utc delta get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_time_tai_utc_delta_get_t;

/**
 * @typedef   rtk_bt_mesh_time_role_get_t
 * @brief     BLE MESH time role get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_time_role_get_t;

/**
 * @typedef   rtk_bt_mesh_time_set_t
 * @brief     BLE MESH time set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    rtk_bt_mesh_tai_time_t time;
}_PACKED4_ rtk_bt_mesh_time_set_t;

/**
 * @typedef   rtk_bt_mesh_time_zone_set_t;
 * @brief     BLE MESH time zone set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint8_t time_zone_offset_new;
    uint8_t tai_of_zone_change[5];
} rtk_bt_mesh_time_zone_set_t;

/**
 * @typedef   rtk_bt_mesh_time_tai_utc_delta_set_t;
 * @brief     BLE MESH time tai utc delta set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t tai_utc_delta_new;
    uint8_t tai_of_delta_change[5];
} rtk_bt_mesh_time_tai_utc_delta_set_t;

/**
 * @typedef   rtk_bt_mesh_time_role_set_t
 * @brief     BLE MESH time role set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    rtk_bt_mesh_time_role_t role;
}_PACKED4_ rtk_bt_mesh_time_role_set_t;

/**
 * @typedef   rtk_bt_mesh_time_client_status_t
 * @brief     BLE MESH time client model status message structure.
 */
typedef struct
{
    uint16_t src;
    rtk_bt_mesh_tai_time_t tai_time;
}_PACKED4_ rtk_bt_mesh_time_client_status_t;

/**
 * @typedef   rtk_bt_mesh_time_client_status_zone_t
 * @brief     BLE MESH time client model zone status message structure.
 */
typedef struct
{
    uint16_t src;
    uint8_t time_zone_offset_current;
    uint8_t time_zone_offset_new;
    uint8_t tai_of_zone_change[5];
} rtk_bt_mesh_time_client_status_zone_t;

/**
 * @typedef   rtk_bt_mesh_time_client_status_tai_utc_delta_t
 * @brief     BLE MESH time client model tai utc delta status message structure.
 */
typedef struct
{
    uint16_t src;
    uint16_t tai_utc_delta_current: 15;
    uint16_t padding1: 1; /* always 0*/
    uint16_t tai_utc_delta_new: 15;
    uint16_t padding2: 1; /* always 0*/
    uint8_t tai_of_delta_change[5]; /* little endian */
} rtk_bt_mesh_time_client_status_tai_utc_delta_t;

/**
 * @typedef   rtk_bt_mesh_time_client_status_role_t
 * @brief     BLE MESH time client model role status message structure.
 */
typedef struct
{
    rtk_bt_mesh_time_role_t role;
} rtk_bt_mesh_time_client_status_role_t;


/************time server callback data struct*******************/

/**
 * @typedef   rtk_bt_mesh_time_server_model_evt_t
 * @brief     BLE MESH time server model event definition.
 */
typedef enum {
	RTK_BT_MESH_TIME_SERVER_MODEL_GET = 1,
    RTK_BT_MESH_TIME_SERVER_MODEL_ZONE_GET,
    RTK_BT_MESH_TIME_SERVER_MODEL_TAI_UTC_DELTA_GET,
    RTK_BT_MESH_TIME_SERVER_MODEL_STATUS_SET,
    RTK_BT_MESH_TIME_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_time_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_time_server_get_t
 * @brief     BLE MESH time get evt message structure.
 */
typedef rtk_bt_mesh_tai_time_t               rtk_bt_mesh_time_server_get_t;

/**
 * @typedef   rtk_bt_mesh_time_server_direct_get_t
 * @brief     BLE MESH time direct get evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_time_server_get_t *value;
} rtk_bt_mesh_time_server_direct_get_t;

/**
 * @typedef   rtk_bt_mesh_time_server_get_zone_t
 * @brief     BLE MESH time zone get evt message structure.
 */
typedef rtk_bt_mesh_time_zone_t              rtk_bt_mesh_time_server_get_zone_t;

/**
 * @typedef   rtk_bt_mesh_time_server_direct_get_zone_t
 * @brief     BLE MESH time zone direct get evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_time_server_get_zone_t *value;
} rtk_bt_mesh_time_server_direct_get_zone_t;

/**
 * @typedef   rtk_bt_mesh_time_server_get_tai_utc_delta_t
 * @brief     BLE MESH time tai utc delta get evt message structure.
 */
typedef rtk_bt_mesh_tai_utc_delta_t          rtk_bt_mesh_time_server_get_tai_utc_delta_t;

/**
 * @typedef   rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t
 * @brief     BLE MESH time tai utc delta direct get evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_time_server_get_tai_utc_delta_t *value;
} rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t;

/** 
 * @typedef   rtk_bt_mesh_time_server_status_set_t
 * @brief     BLE MESH time server status set evt message structure.
 */
typedef struct {
	uint8_t tai_seconds[5]; /* little endian */
	uint8_t subsecond;
	uint8_t uncertainty;
	uint16_t tai_utc_delta;
	uint8_t time_zone_offset;
} _PACKED4_ rtk_bt_mesh_time_server_status_set_t;

/**
 * @typedef   rtk_bt_mesh_time_setup_server_model_evt_t
 * @brief     BLE MESH time setup server model event definition.
 */
typedef enum {
	RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_SET = 1,
    RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_ZONE_SET,
    RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_ROLE_GET,
    RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_ROLE_SET,
    RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_TAI_UTC_DELTA_SET,
    RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_ZONE_GET,
    RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_TAI_UTC_DELTA_GET,
    RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_time_setup_server_model_evt_t;

/** 
 * @typedef   rtk_bt_mesh_time_server_set_t
 * @brief     BLE MESH time set evt message structure.
 */
typedef rtk_bt_mesh_tai_time_t               rtk_bt_mesh_time_server_set_t;

/** 
 * @typedef   rtk_bt_mesh_time_server_get_role_t
 * @brief     BLE MESH time get role evt message structure.
 */
typedef struct {
	rtk_bt_mesh_time_role_t *role;
} rtk_bt_mesh_time_server_get_role_t;

/** 
 * @typedef   rtk_bt_mesh_time_server_set_role_t
 * @brief     BLE MESH time set role evt message structure.
 */
typedef struct {
	rtk_bt_mesh_time_role_t role;
} rtk_bt_mesh_time_server_set_role_t;

/** 
 * @typedef   rtk_bt_mesh_time_server_set_zone_t
 * @brief     BLE MESH time set zone evt message structure.
 */
typedef struct {
	uint8_t time_zone_offset_new;
	uint8_t tai_of_zone_change[5]; /* little endian */
} rtk_bt_mesh_time_server_set_zone_t;

/** 
 * @typedef   rtk_bt_mesh_time_server_set_tai_utc_delta_t
 * @brief     BLE MESH time set tai utc delta evt message structure.
 */
typedef struct {
	uint16_t tai_utc_delta_new: 15;
	uint16_t padding: 1; /* always 0*/
	uint8_t tai_of_delta_change[5]; /* little endian */
} rtk_bt_mesh_time_server_set_tai_utc_delta_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  ble_mesh_time_model BT LE Mesh Time Model APIs
 * @brief     BT LE Mesh Time Model related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */
/**
 * @fn        uint16_t rtk_bt_mesh_time_get(rtk_bt_mesh_time_get_t *time_client_get)
 * @brief     Send time get message to time server, will cause event @ref RTK_BT_MESH_TIME_CLIENT_MODEL_GET
 * @param[in] time_client_get: time get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_time_get(rtk_bt_mesh_time_get_t *time_client_get);

/**
 * @fn        uint16_t rtk_bt_mesh_time_zone_get(rtk_bt_mesh_time_zone_get_t *time_client_zone_get)
 * @brief     Send time zone get message to time server, will cause event @ref RTK_BT_MESH_TIME_CLIENT_MODEL_ZONE_GET
 * @param[in] time_client_zone_get: time get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_time_zone_get(rtk_bt_mesh_time_zone_get_t *time_client_zone_get);

/**
 * @fn        uint16_t rtk_bt_mesh_time_tai_utc_delta_get(rtk_bt_mesh_time_tai_utc_delta_get_t *time_client_delta_get)
 * @brief     Send time tai tuc delta get message to time server, will cause event @ref RTK_BT_MESH_TIME_CLIENT_MODEL_TAI_UTC_DELTA_GET
 * @param[in] time_client_delta_get: time tai tuc delta get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_time_tai_utc_delta_get(rtk_bt_mesh_time_tai_utc_delta_get_t *time_client_delta_get);

/**
 * @fn        uint16_t rtk_bt_mesh_time_role_get(rtk_bt_mesh_time_role_get_t *time_client_role_get)
 * @brief     Send time role get message to time setup server, will cause event @ref RTK_BT_MESH_TIME_CLIENT_MODEL_ROLE_GET
 * @param[in] time_client_role_get: time role get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_time_role_get(rtk_bt_mesh_time_role_get_t *time_client_role_get);

/**
 * @fn        uint16_t rtk_bt_mesh_time_set(rtk_bt_mesh_time_set_t *time_client_set)
 * @brief     Send time set message to time setup server, will cause event @ref RTK_BT_MESH_TIME_CLIENT_MODEL_SET
 * @param[in] time_client_set: time set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_time_set(rtk_bt_mesh_time_set_t *time_client_set);

/**
 * @fn        uint16_t rtk_bt_mesh_time_zone_set(rtk_bt_mesh_time_zone_set_t *time_client_zone_set)
 * @brief     Send time zone set message to time setup server, will cause event @ref RTK_BT_MESH_TIME_CLIENT_MODEL_ZONE_SET
 * @param[in] time_client_zone_set: time zone set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_time_zone_set(rtk_bt_mesh_time_zone_set_t *time_client_zone_set);

/**
 * @fn        uint16_t rtk_bt_mesh_time_tai_utc_delta_set(rtk_bt_mesh_time_tai_utc_delta_set_t *time_client_delta_set)
 * @brief     Send time tai utc delta set message to time setup server, will cause event @ref RTK_BT_MESH_TIME_CLIENT_MODEL_TAI_UTC_DELTA_SET
 * @param[in] time_client_delta_set: time tai utc delta set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_time_tai_utc_delta_set(rtk_bt_mesh_time_tai_utc_delta_set_t *time_client_delta_set);

/**
 * @fn        uint16_t rtk_bt_mesh_time_role_set(rtk_bt_mesh_time_role_set_t *time_client_role_set)
 * @brief     Send time role set message to time setup server, will cause event @ref RTK_BT_MESH_TIME_CLIENT_MODEL_ROLE_SET
 * @param[in] time_client_role_set: time role set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_time_role_set(rtk_bt_mesh_time_role_set_t *time_client_role_set);

/**
 * @}
 */

#endif /* __RTK_BT_COMMON_H__ */