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
 * @defgroup BLE_Scan
 * @ingroup BLE_Scan
 * @brief Provides APIs for BLE Scan
 * @{
 */

/**
 * @file ble_manager/ble_scan.h
 * @brief Provides APIs for BLE Scan
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "ble_common.h"

typedef enum {
	BLE_SCAN_STOPPED = 0,
	BLE_SCAN_STARTED,
	BLE_SCAN_CHANGING,
} ble_scan_state_e;

typedef struct {
	uint8_t raw_data[BLE_ADV_RAW_DATA_MAX_LEN];
	uint8_t raw_data_length;
	bool whitelist_enable;
	uint16_t scan_duration;
} ble_scan_filter;

typedef struct {
	ble_adv_type_e adv_type;
	int8_t rssi;
	ble_addr addr;
	uint8_t raw_data[BLE_ADV_RAW_DATA_MAX_LEN];
	uint8_t raw_data_length;
	uint8_t resp_data[BLE_ADV_RESP_DATA_MAX_LEN];
	uint8_t resp_data_length;
} __attribute__((aligned(4), packed)) ble_scanned_device;

typedef struct {
	/* This is a set of callback function for BLE Scan */
	void(*ble_client_scan_state_changed_cb)(ble_scan_state_e scan_state);
	void(*ble_client_device_scanned_cb)(ble_scanned_device* scanned_device);
} ble_scan_callback_list;

/****************************************************************************
 * Name: ble_client_start_scan
 *
 * Description:
 *   Start BLE adv scanning
 *
 * Input Parameters:
 *   filter    - If this value is NULL, start to scan for BLE adv till 
 *               'ble_client_stop_scan' is called.
 *               To get specific BLE adv info, filter value should be filled
 *               with exact value.
 *   callbacks - The list of scan result callback.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_client_start_scan(ble_scan_filter* filter, ble_scan_callback_list *callbacks);

/****************************************************************************
 * Name: ble_client_stop_scan
 *
 * Description:
 *   Stop BLE adv scanning. This should be called to stop BLE scanning after 
 *   ble_client_start_scan is called without filter.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_client_stop_scan(void);

/****************************************************************************
 * Name: ble_scan_whitelist_add
 *       ble_scan_whitelist_delete
 *
 * Input Parameters:
 *   addr  - BLE Address value including type.
 * 
 * Description:
 *   Add/Delete a BLE address to whitelist. 
 *   If a scanning is started with whitelist enabling, scan results are filtered by the whitelist.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_scan_whitelist_add(ble_addr *addr);
ble_result_e ble_scan_whitelist_delete(ble_addr *addr);

/****************************************************************************
 * Name: ble_scan_whitelist_clear_all
 *
 * Description:
 *   Clear data of the scan whitelist.
 *
 * Returned Value
 *   Zero (BLE_RESULT_SUCCESS) is returned on success; a positive value is returned on
 *   failure.
 *
 ****************************************************************************/
ble_result_e ble_scan_whitelist_clear_all(void);
