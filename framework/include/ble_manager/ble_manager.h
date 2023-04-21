/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/**
 * @defgroup BLE_Manager BLE_Manager
 * @ingroup BLE_Manager
 * @brief Provides APIs for BLE Manager
 * @{
 */

/**
 * @file ble_manager/ble_manager.h
 * @brief Provides APIs for BLE Manager
 */
#pragma once

#include <stdbool.h>

#include "ble_common.h"
#include "ble_client.h"
#include "ble_server.h"
#include "ble_scan.h"

typedef enum {
	BLEMGR_SERVER_MODE,
	BLEMGR_CLIENT_MODE,
	BLEMGR_COMBO_MODE,
	BLEMGR_UNKNOWN_MODE,
} blemgr_mode_e;

typedef struct _ble_bonded_device_list {
	ble_addr bd_addr;
} ble_bonded_device_list;

/****************************************************************************
 * Name: ble_manager_init
 *
 * Description:
 *   Initialize ble manager.
 * 
 * Input Parameters:
 *   server_config - Server config. It can be NULL if an user doesn't want server role.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_manager_init(ble_server_init_config *server_config);

/****************************************************************************
 * Name: ble_manager_deinit
 *
 * Description:
 *   Deinitialize ble manager.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_manager_deinit(void);

/****************************************************************************
 * Name: ble_manager_get_mac_addr
 *
 * Description:
 *   Get mac address of BLE device.
 * 
 * Output Parameters:
 *   mac       - Empty buffer for mac address.
 * 
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_manager_get_mac_addr(uint8_t mac[BLE_BD_ADDR_MAX_LEN]);

/****************************************************************************
 * Name: ble_manager_get_bonded_device
 *
 * Description:
 *   Get the bonded devices.
 *
 * Input Parameters:
 *   device_list   - The linked list of bonded devices(The mem of list should be allocated).
 *   device_count  - The number of bonded devices.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_manager_get_bonded_device(ble_bonded_device_list* device_list, uint16_t* device_count);

/****************************************************************************
 * Name: ble_manager_delete_bonded
 *
 * Description:
 *   Delete the bonded device.
 *
 * Input Parameters:
 *   addr      - The ble address of bonded device.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_manager_delete_bonded(ble_addr *addr);

/****************************************************************************
 * Name: ble_manager_delete_bonded_all
 *
 * Description:
 *   Delete all of the bonded devices.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_manager_delete_bonded_all(void);

// Get whether the con_handle is active or not
ble_result_e ble_manager_conn_is_active(ble_conn_handle con_handle, bool *is_active);
ble_result_e ble_manager_conn_is_any_active(bool *is_active);

/****************************************************************************
 * Name: ble_manager_get_version
 *
 * Description:
 *   Get 3 bytes data of current BLE Driver version.
 *
 * Input Parameters:
 *   version  - 3 bytes array to get BLE current version.
 * 
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_manager_get_version(uint8_t version[3]);


/****************************************************************************
 * Name: ble_manager_conn_param_update
 *
 * Description:
 *   Request to update connection parameters
 *
 * Input Parameters:
 *   con_handle : con handle for request
 *   ble_conn_param : structure including connection parameters.
 * 
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_manager_conn_param_update(ble_conn_handle *con_handle, ble_conn_param *conn_param);

