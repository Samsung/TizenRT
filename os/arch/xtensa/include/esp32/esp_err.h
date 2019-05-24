/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/
#ifndef _ESP_ERR_H_
#define _ESP_ERR_H_

#include <stdint.h>

typedef int32_t esp_err_t;

#define ESP_OK          0		/*!< esp_err_t value indicating success (no error) */
#define ESP_FAIL        -1		/*!< Generic esp_err_t code indicating failure */

#define ESP_ERR_NO_MEM          0x101	/*!< Out of memory */
#define ESP_ERR_INVALID_ARG     0x102	/*!< Invalid argument */
#define ESP_ERR_INVALID_STATE   0x103	/*!< Invalid state */
#define ESP_ERR_INVALID_SIZE    0x104	/*!< Invalid size */
#define ESP_ERR_NOT_FOUND       0x105	/*!< Requested resource not found */
#define ESP_ERR_NOT_SUPPORTED   0x106	/*!< Operation or feature not supported */
#define ESP_ERR_TIMEOUT         0x107	/*!< Operation timed out */
#define ESP_ERR_INVALID_RESPONSE    0x108	/*!< Received response was invalid */
#define ESP_ERR_INVALID_CRC     0x109	/*!< CRC or checksum was invalid */
#define ESP_ERR_INVALID_VERSION     0x10A	/*!< Version was invalid */
#define ESP_ERR_INVALID_MAC     0x10B	/*!< MAC address was invalid */

#define ESP_ERR_WIFI_BASE       0x3000	/*!< Starting number of WiFi error codes */
#define ESP_ERR_MESH_BASE       0x4000	/*!< Starting number of MESH error codes */

#endif //_ESP_ERR_H_
