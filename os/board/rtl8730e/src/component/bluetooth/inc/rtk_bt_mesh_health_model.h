/**
 * @file      rtk_bt_mesh_health_model.h
 * @author    quinn_yang@realsil.com.cn
 * @brief     Bluetooth LE MESH light client and server model data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */


#ifndef __RTK_BT_MESH_HEALTH_MODEL_H__
#define __RTK_BT_MESH_HEALTH_MODEL_H__

#include <rtk_bt_mesh_def.h>

/* ------------------------------- Data Types ------------------------------- */

#define HEALTH_FAULT_ARRAY_LEN 0x10  //The max len in spec is 0xFF

/**
 * @typedef   rtk_bt_mesh_health_client_model_act_t
 * @brief     BLE MESH health client model act definition.
 */
typedef enum {
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_FAULT_GET = 1,
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_FAULT_CLEAR,
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_FAULT_TEST,
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_PERIOD_GET,
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_PERIOD_SET,
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_ATTN_GET,
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_ATTN_SET,
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_health_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_health_client_model_evt_t
 * @brief     BLE MESH health client model event definition.
 */
typedef enum {
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_ATTN_STATUS = 1,
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_CURT_STATUS,
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_FAULT_STATUS,
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_PERIOD_STATUS,
	RTK_BT_MESH_HEALTH_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_health_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_health_fault_get_t
 * @brief     BLE MESH health fault get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t company_id;
} rtk_bt_mesh_health_fault_get_t;

/**
 * @typedef   rtk_bt_mesh_health_fault_clear_t
 * @brief     BLE MESH health fault clear act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t company_id;
    bool ack;
} rtk_bt_mesh_health_fault_clear_t;

/**
 * @typedef   rtk_bt_mesh_health_fault_test_t
 * @brief     BLE MESH health fault test act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint8_t test_id;
    uint16_t company_id;
    bool ack;
} rtk_bt_mesh_health_fault_test_t;

/**
 * @typedef   rtk_bt_mesh_health_period_get_t
 * @brief     BLE MESH health period get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_health_period_get_t;

/**
 * @typedef   rtk_bt_mesh_health_period_set_t
 * @brief     BLE MESH health period set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint8_t fast_period_divisor;
    bool ack;
} rtk_bt_mesh_health_period_set_t;

/**
 * @typedef   rtk_bt_mesh_health_attn_get_t
 * @brief     BLE MESH health attn get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_health_attn_get_t;

/**
 * @typedef   rtk_bt_mesh_health_attn_set_t
 * @brief     BLE MESH health attn set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint8_t attn;
    bool ack;
} rtk_bt_mesh_health_attn_set_t;

/**
 * @typedef   rtk_bt_mesh_health_client_status_t
 * @brief     BLE MESH health client model status message structure.
 */
typedef struct
{
    uint16_t src;
    uint8_t test_id;
    uint16_t company_id;
    uint8_t fault_array_len;
    uint8_t *fault_array;
} rtk_bt_mesh_health_client_status_t;

/**
 * @typedef   rtk_bt_mesh_health_client_status_period_t
 * @brief     BLE MESH health client model period status message structure.
 */
typedef struct
{
    uint8_t fast_period_divisor;
} rtk_bt_mesh_health_client_status_period_t;

/**
 * @typedef   rtk_bt_mesh_health_client_status_attention_t
 * @brief     BLE MESH health client model attention status message structure.
 */
typedef struct
{
    uint8_t attention;
} rtk_bt_mesh_health_client_status_attention_t;

/**
 * @typedef   rtk_bt_mesh_health_server_model_evt_t
 * @brief     BLE MESH health server model event definition.
 */
typedef enum {
	RTK_BT_MESH_HEALTH_SERVER_MODEL_FAULT_GET = 1,
	RTK_BT_MESH_HEALTH_SERVER_MODEL_FAULT_CLEAR,
	RTK_BT_MESH_HEALTH_SERVER_MODEL_FAULT_TEST,
	RTK_BT_MESH_HEALTH_SERVER_MODEL_PERIOD_GET,
	RTK_BT_MESH_HEALTH_SERVER_MODEL_PERIOD_SET,
	RTK_BT_MESH_HEALTH_SERVER_MODEL_ATTN_GET,
	RTK_BT_MESH_HEALTH_SERVER_MODEL_ATTN_SET,
	RTK_BT_MESH_HEALTH_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_health_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_health_server_model_act_t
 * @brief     BLE MESH health server model act definition.
 */
typedef enum {
	RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_FAULT_REGISTER = 1,
	RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_CURRENT_FAULT_CLEAR,
	RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_CURRENT_FAULT_CLEAR_ALL,
	RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_FAULT_COUNT,
	RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_CHECK_FAULT_IS_SET,
	RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_MAX,
} rtk_bt_mesh_health_server_model_act_t;

/**
 * @typedef   rtk_bt_mesh_health_server_fault_register_act_t
 * @brief     BLE MESH health server fault register act message structure.
 */
typedef struct {
    uint8_t fault;
} rtk_bt_mesh_health_server_fault_register_act_t;

/**
 * @typedef   rtk_bt_mesh_health_server_fault_clear_act_t
 * @brief     BLE MESH health server fault clear act message structure.
 */
typedef struct {
    uint8_t fault;
} rtk_bt_mesh_health_server_fault_clear_act_t;

/**
 * @typedef   rtk_bt_mesh_health_server_check_fault_act_t
 * @brief     BLE MESH health server fault check act message structure.
 */
typedef struct {
    uint8_t fault;
	bool set;
} rtk_bt_mesh_health_server_check_fault_act_t;

/**
 * @typedef   rtk_bt_mesh_health_server_count_fault_act_t
 * @brief     BLE MESH health server fault count act message structure.
 */
typedef struct {
    uint8_t fault_count;
} rtk_bt_mesh_health_server_count_fault_act_t;

/**
 * @typedef   rtk_bt_mesh_health_server_fault_get_t
 * @brief     BLE MESH health get fault evt message structure.
 */
typedef struct {
    uint16_t company_id;
    uint8_t *fault_array;
} rtk_bt_mesh_health_server_fault_get_t;

/**
 * @typedef   rtk_bt_mesh_health_server_fault_clear_t
 * @brief     BLE MESH health clear fault evt message structure.
 */
typedef struct {
    uint16_t company_id;
    uint8_t *fault_array;
} rtk_bt_mesh_health_server_fault_clear_t;

/**
 * @typedef   rtk_bt_mesh_health_server_fault_test_t
 * @brief     BLE MESH health test fault evt message structure.
 */
typedef struct {
    uint8_t test_id;
    uint16_t company_id;
    uint8_t *fault_array;
} rtk_bt_mesh_health_server_fault_test_t;

/**
 * @typedef   rtk_bt_mesh_health_server_period_get_t
 * @brief     BLE MESH health period evt get message structure.
 */
typedef struct {
	uint8_t *fast_period_divisor;
} rtk_bt_mesh_health_server_period_get_t;

/**
 * @typedef   rtk_bt_mesh_health_server_period_set_t
 * @brief     BLE MESH health period evt set message structure.
 */
typedef struct {
	uint8_t fast_period_divisor;
} rtk_bt_mesh_health_server_period_set_t;

/**
 * @typedef   rtk_bt_mesh_health_server_attn_get_t
 * @brief     BLE MESH health attention timer get evt message structure.
 */
typedef struct {
	uint8_t *attn;
} rtk_bt_mesh_health_server_attn_get_t;

/**
 * @typedef   rtk_bt_mesh_health_server_attn_set_t
 * @brief     BLE MESH health attention timer set evt message structure.
 */
typedef struct {
	uint8_t attn;
} rtk_bt_mesh_health_server_attn_set_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  ble_mesh_health_model BT LE Mesh Health Model APIs
 * @brief     BT LE Mesh Health Model related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_mesh_health_fault_get(rtk_bt_mesh_health_fault_get_t *health_fault_get)
 * @brief     Send health fault get message to health server, will cause event @ref RTK_BT_MESH_HEALTH_CLIENT_MODEL_FAULT_GET
 * @param[in] health_fault_get: health fault get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_health_fault_get(rtk_bt_mesh_health_fault_get_t *health_fault_get);

/**
 * @fn        uint16_t rtk_bt_mesh_health_fault_clear(rtk_bt_mesh_health_fault_clear_t *health_fault_clear)
 * @brief     Send health fault clear message to health server, will cause event @ref RTK_BT_MESH_HEALTH_CLIENT_MODEL_FAULT_CLEAR
 * @param[in] health_fault_clear: health fault clear message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_health_fault_clear(rtk_bt_mesh_health_fault_clear_t *health_fault_clear);

/**
 * @fn        uint16_t rtk_bt_mesh_health_fault_test(rtk_bt_mesh_health_fault_test_t *health_fault_test)
 * @brief     Send health fault test message to health server, will cause event @ref RTK_BT_MESH_HEALTH_CLIENT_MODEL_FAULT_TEST
 * @param[in] health_fault_test: health fault test message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_health_fault_test(rtk_bt_mesh_health_fault_test_t *health_fault_test);

/**
 * @fn        uint16_t rtk_bt_mesh_health_period_get(rtk_bt_mesh_health_period_get_t *health_period_get)
 * @brief     Send health period get message to health server, will cause event @ref RTK_BT_MESH_HEALTH_CLIENT_MODEL_PERIOD_GET
 * @param[in] health_period_get: health period get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_health_period_get(rtk_bt_mesh_health_period_get_t *health_period_get);

/**
 * @fn        uint16_t rtk_bt_mesh_health_period_set(rtk_bt_mesh_health_period_set_t *health_period_set)
 * @brief     Send health period set message to health server, will cause event @ref RTK_BT_MESH_HEALTH_CLIENT_MODEL_PERIOD_SET
 * @param[in] health_period_set: health period set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_health_period_set(rtk_bt_mesh_health_period_set_t *health_period_set);

/**
 * @fn        uint16_t rtk_bt_mesh_health_attn_get(rtk_bt_mesh_health_attn_get_t *health_attn_get)
 * @brief     Send health attn get message to health server, will cause event @ref RTK_BT_MESH_HEALTH_CLIENT_MODEL_ATTN_GET
 * @param[in] health_attn_get: health attn get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_health_attn_get(rtk_bt_mesh_health_attn_get_t *health_attn_get);

/**
 * @fn        uint16_t rtk_bt_mesh_health_attn_set(rtk_bt_mesh_health_attn_set_t *health_attn_set)
 * @brief     Send health attn set message to health server, will cause event @ref RTK_BT_MESH_HEALTH_CLIENT_MODEL_ATTN_SET
 * @param[in] health_attn_set: health attn set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_health_attn_set(rtk_bt_mesh_health_attn_set_t *health_attn_set);

/**
 * @fn        uint16_t rtk_bt_mesh_health_server_fault_register(rtk_bt_mesh_health_server_fault_register_act_t *health_fault_register)
 * @brief     Send health server fault register message to health server, will cause event @ref RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_FAULT_REGISTER
 * @param[in] health_fault_register: health fault register message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_health_server_fault_register(rtk_bt_mesh_health_server_fault_register_act_t *health_fault_register);

/**
 * @fn        uint16_t rtk_bt_mesh_health_server_fault_clear(rtk_bt_mesh_health_server_fault_clear_act_t *health_fault_clear)
 * @brief     Send health server fault clear message to health server, will cause event @ref RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_CURRENT_FAULT_CLEAR
 * @param[in] health_fault_clear: health fault clear message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_health_server_fault_clear(rtk_bt_mesh_health_server_fault_clear_act_t *health_fault_clear);

/**
 * @fn        uint16_t rtk_bt_mesh_health_server_fault_clear_all(void)
 * @brief     Send health server fault clear all message to health server, will cause event @ref RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_CURRENT_FAULT_CLEAR_ALL
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_health_server_fault_clear_all(void);

/**
 * @fn        uint16_t rtk_bt_mesh_health_server_fault_count(void)
 * @brief     Send health server fault count message to health server, will cause event @ref RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_FAULT_COUNT
 * @param[in] fault_count: health fault count message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_health_server_fault_count(rtk_bt_mesh_health_server_count_fault_act_t *fault_count);

/**
 * @fn        uint16_t rtk_bt_mesh_health_server_fault_check(rtk_bt_mesh_health_server_check_fault_act_t *health_fault_check)
 * @brief     Send health server fault check message to check if the fault is set, will cause event @ref RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_CHECK_FAULT_IS_SET
 * @param[in] health_fault_check: health fault check message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_health_server_fault_check(rtk_bt_mesh_health_server_check_fault_act_t *health_fault_check);

/**
 * @}
 */

#endif /* __RTK_BT_COMMON_H__ */