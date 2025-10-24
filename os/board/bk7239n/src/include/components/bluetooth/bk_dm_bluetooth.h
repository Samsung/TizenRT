// Copyright 2020-2021 Beken
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

#include "bk_dm_bluetooth_types.h"

#ifdef __cplusplus
extern"C" {
#endif

/**
 * @defgroup dm_bluetooth BLUETOOTH API
 * @{
 */

/**
 * @brief     Get bluetooth status
 *
 * @return    Bluetooth status
 *
 */
bk_bluetooth_status_t bk_bluetooth_get_status(void);

/**
 *
 * @brief           init bluetooth.
 *
 *
 * @return
 *                  - BT_OK: success
 *                  -  others: fail
 *
 */
bt_err_t bk_bluetooth_init(void);

/**
 *
 * @brief           deinit bluetooth.
 *
 *
 * @return
 *                  - BT_OK: success
 *                  -  others: fail
 *
 */
bt_err_t bk_bluetooth_deinit(void);


/**
 *
 * @brief      Get bluetooth device address.  Must use after "bk_bluetooth_init".
 *
 * @param[out]      addr - bluetooth device address
 *
 * @return
 *                  - BT_OK: success
 *                  -  others: fail
 */
bt_err_t bk_bluetooth_get_address(uint8_t *addr);

///@}

#ifdef __cplusplus
}
#endif

