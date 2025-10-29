// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.



#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "common/bk_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BK_BT_ADDR_LEN       (6)


/**
 * @brief Bluetooth status type, to indicate whether the bluetooth is ready.
 */
typedef enum {
    BK_BLUETOOTH_STATUS_UNINITIALIZED   = 0,        /*!< Bluetooth not initialized */
    BK_BLUETOOTH_STATUS_ENABLED                     /*!< Bluetooth initialized and enabled */
} bk_bluetooth_status_t;

//typedef bk_err_t ble_err_t;
typedef int ble_err_t;

#ifndef bt_err_t
typedef int bt_err_t;
#endif

#ifdef __cplusplus
}
#endif

