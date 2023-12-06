/**
 * @file      rtk_bt_mesh_api_datatrans_model.h
 * @author    pico_zhai@realsil.com.cn
 * @brief     Bluetooth LE MESH datatrans model data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_MESH_API_DATATRANS_MODEL_H__
#define __RTK_BT_MESH_API_DATATRANS_MODEL_H__

#include <stdint.h>

// Max val is 258 on AmebaLite for single core case,if extend 258,will cause hardfalut
#define DATA_TRANS_DATA_MAX_LEN    0xff

/**
 * @typedef   rtk_bt_mesh_datatrans_model_act_t
 * @brief     BLE MESH datatrans model act definition.
 */
typedef enum {
	RTK_BT_MESH_DATATRANS_MODEL_ACT_READ = 1,
	RTK_BT_MESH_DATATRANS_MODEL_ACT_WRITE,
	RTK_BT_MESH_DATATRANS_MODEL_ACT_MAX,
} rtk_bt_mesh_datatrans_model_act_t;

/**
 * @typedef   rtk_bt_mesh_datatrans_model_evt_t
 * @brief     BLE MESH datatrans model evt definition.
 */
typedef enum {
	RTK_BT_MESH_DATATRANS_MODEL_EVT_SERVER_WRITE = 1,
	RTK_BT_MESH_DATATRANS_MODEL_EVT_SERVER_READ,
	RTK_BT_MESH_DATATRANS_MODEL_EVT_CLIENT_WRITE_STATUS,
	RTK_BT_MESH_DATATRANS_MODEL_EVT_CLIENT_READ_RESULT,
	RTK_BT_MESH_DATATRANS_MODEL_EVT_MAX,
} rtk_bt_mesh_datatrans_model_evt_t;

/**
 * @typedef rtk_bt_mesh_datatrans_stat_t
 * @brief BLE MESH datatrans server model write status event data structure
 */
enum {
	RTK_BT_MESH_DATATRANS_SUCCESS,
	RTK_BT_MESH_DATATRANS_FAIL,
};
typedef uint8_t rtk_bt_mesh_datatrans_stat_t;

/**
 * @typedef   rtk_bt_mesh_datatrans_client_write_api_t
 * @brief     BLE MESH datatrans client model write api data structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t data_len;
	uint8_t data[DATA_TRANS_DATA_MAX_LEN];
	bool ack;
} rtk_bt_mesh_datatrans_client_write_api_t;

/**
 * @typedef   rtk_bt_mesh_datatrans_client_read_api_t
 * @brief     BLE MESH datatrans client model read api data structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t read_len;
} rtk_bt_mesh_datatrans_client_read_api_t;

/**
 * @typedef   rtk_bt_mesh_datatrans_client_write_event_t
 * @brief     BLE MESH datatrans client model write result event data structure.
 */
typedef struct {
	rtk_bt_mesh_datatrans_stat_t status;
	uint16_t written_len;
} rtk_bt_mesh_datatrans_client_write_event_t;

/**
 * @typedef   rtk_bt_mesh_datatrans_client_read_event_t
 * @brief     BLE MESH datatrans client model read event data structure.
 */
typedef struct {
	uint8_t data_len;
	uint8_t data[DATA_TRANS_DATA_MAX_LEN];
} rtk_bt_mesh_datatrans_client_read_event_t;

/**
 * @typedef   rtk_bt_mesh_datatrans_server_read_event_t
 * @brief     BLE MESH datatrans server model read event data structure.
 */
typedef struct {
	uint16_t data_len;
	uint8_t *data;
} rtk_bt_mesh_datatrans_server_read_event_t;

/**
 * @typedef   rtk_bt_mesh_datatrans_server_write_event_t
 * @brief     BLE MESH datatrans server model write event data structure.
 */
typedef struct {
	rtk_bt_mesh_datatrans_stat_t status;
	uint8_t data_len;
	uint8_t data[DATA_TRANS_DATA_MAX_LEN];
} rtk_bt_mesh_datatrans_server_write_event_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  ble_mesh_datatrans_model BT LE Mesh Datatrans Model APIs
 * @brief     BT LE Mesh Datatrans Model related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_mesh_datatrans_model_write(rtk_bt_mesh_datatrans_client_write_api_t *client_write)
 * @brief     Send datatrans write message to device, will cause event @ref RTK_BT_MESH_DATATRANS_MODEL_ACT_WRITE
 * @param[in] client_write: write message param structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_datatrans_model_write(rtk_bt_mesh_datatrans_client_write_api_t *client_write);

/**
 * @fn        uint16_t rtk_bt_mesh_datatrans_model_read(rtk_bt_mesh_datatrans_client_read_api_t *client_read)
 * @brief     Send datatrans read message to device, will cause event @ref RTK_BT_MESH_DATATRANS_MODEL_ACT_READ
 * @param[in] client_read: read message param structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_datatrans_model_read(rtk_bt_mesh_datatrans_client_read_api_t *client_read);

/**
 * @}
 */

#endif
