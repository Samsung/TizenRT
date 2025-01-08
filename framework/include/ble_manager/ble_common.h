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
 * @defgroup BLE_Common
 * @ingroup BLE_Common
 * @brief Provides APIs for BLE Common
 * @{
 */

/**
 * @file ble_manager/ble_client.h
 * @brief Provides APIs for BLE Common
 */
#pragma once

#include <stdint.h>

/* Length defines */
#define BLE_BD_ADDR_MAX_LEN 6
#define BLE_BD_ADDR_STR_LEN 17
#define BLE_GAP_DEVICE_NAME_LEN (39+1)/*!< Max length of device name, if device name length exceeds it, it will be truncated. */
#define BLE_ADV_RAW_DATA_MAX_LEN 31
#define BLE_ADV_RESP_DATA_MAX_LEN 31

/* These are board specific values */
#define BLE_MAX_CONNECTION_COUNT 3
#define BLE_MAX_BONDED_DEVICE 10
#define BLE_DEFAULT_CONN_TIMEOUT 10000 /* 10 seconds */

typedef struct _ble_data {
	uint8_t *data;
	uint16_t length;
} ble_data;

typedef enum {
	BLE_ADV_TYPE_IND,
	BLE_ADV_TYPE_DIRECT,
	BLE_ADV_TYPE_SCAN_IND,
	BLE_ADV_TYPE_NONCONN_IND,
	BLE_ADV_TYPE_SCAN_RSP,
	BLE_ADV_TYPE_UNKNOWN,
} ble_adv_type_e;

typedef enum {
	BLE_ADDR_TYPE_PUBLIC,
	BLE_ADDR_TYPE_RANDOM_STATIC,
	BLE_ADDR_TYPE_RANDOM_RESOLVABLE,
	BLE_ADDR_TYPE_RANDOM_NON_RESOLVABLE,
	BLE_ADDR_TYPE_UNKNOWN,
} ble_addr_type_e;

typedef struct {
	uint8_t mac[BLE_BD_ADDR_MAX_LEN];
	ble_addr_type_e type;
} ble_addr;

typedef enum {
	BLE_SLAVE_CONN_PARAM_UPDATE,
	BLE_MASTER_CONN_PARAM_UPDATE
} ble_conn_param_role;

typedef struct {
	uint16_t min_conn_interval;
	uint16_t max_conn_interval;
	uint16_t slave_latency;
	uint16_t supervision_timeout;
	ble_conn_param_role role;
} ble_conn_param;

typedef struct {
	uint8_t io_cap;                 /*!< IO capabilities */
	uint8_t oob_data_flag;          /*!< OOB data flag */
	uint8_t bond_flag;              /*!< Bonding flags */
	uint8_t mitm_flag;              /*!< MITM flag */
	uint8_t sec_pair_flag;          /*!< Secure connection pairing support flag */
	uint8_t use_fixed_key;          /*!< Pairing use fixed passkey */
	uint32_t fixed_key;             /*!< Fixed passkey value */
} ble_sec_param;
/**
 * @brief Result types of BLE Manager APIs such as FAIL, SUCCESS, or INVALID ARGS
 */
typedef enum {
	BLE_MANAGER_SUCCESS = 0,
	BLE_MANAGER_FAIL,
	BLE_MANAGER_NOT_FOUND,
	BLE_MANAGER_INVALID_STATE,
	BLE_MANAGER_INVALID_ARGS,
	BLE_MANAGER_TIMEOUT,
	BLE_MANAGER_BUSY,
	BLE_MANAGER_FILE_ERROR,
	BLE_MANAGER_UNSUPPORTED,
	BLE_MANAGER_CALLBACK_NOT_REGISTERED,
	BLE_MANAGER_ALREADY_WORKING,
	BLE_MANAGER_OUT_OF_MEMORY,
	BLE_MANAGER_UNKNOWN,
} ble_result_e;

typedef uint16_t ble_conn_handle;
typedef uint16_t ble_attr_handle;
