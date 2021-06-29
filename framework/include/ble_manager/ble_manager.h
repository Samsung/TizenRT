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

#ifndef BLE_MANAGER_H_
#define BLE_MANAGER_H_

#include "ble_common.h"
#include "ble_error.h"
#include "ble_client.h"
#include "ble_server.h"

#define BLEMGR_SERVER_MODE	0x01
#define BLEMGR_CLIENT_MODE	0x02
#define BLEMGR_COMBO_MODE	BLEMGR_CLIENT_MODE | BLEMGR_SERVER_MODE
#define BLEMGR_UNKNOWN_MODE 0

typedef struct _ble_mgr_config {
	ble_client_init_config *client;
	ble_server_init_config *server;
} blemgr_config;

/****************************************************************************
 * Name: ble_manager_init
 *
 * Description:
 *   Initialize ble manager.
 * 
 * Input Parameters:
 *   client_config - Client config. If this is NULL, diable client mode.
 *   server_config - Server config. If this is NULL, diable server mode.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_manager_init(ble_client_init_config *client_config, ble_server_init_config *server_config);

/****************************************************************************
 * Name: ble_manager_deinit
 *
 * Description:
 *   Deinitialize ble manager.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_manager_deinit(void);

/****************************************************************************
 * Name: ble_manager_get_mac_addr
 *
 * Description:
 *   Get mac address of BLE device.
 * 
 * Input Parameters:
 *   mac       - Empty buffer for mac address.
 * 
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a negated value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result ble_manager_get_mac_addr(uint8_t mac[BLE_BD_ADDR_MAX_LEN]);

#endif
