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

#include <common/bk_include.h>
#include <driver/efuse_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/* @brief Overview about this API header
 *
 */

/**
 * @brief EFUSE API
 * @defgroup bk_api_efuse EFUSE API group
 * @{
 */

/**
 * @brief     Init the EFUSE driver
 *
 * This API init the resoure common:
 *   - Init EFUSE driver control memory
 *
 * @attention 1. This API should be called before any other EFUSE APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_efuse_driver_init(void);

/**
 * @brief     Deinit the EFUSE driver
 *
 * This API free all resource related to EFUSE and disable EFUSE.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_efuse_driver_deinit(void);

/**
 * @brief     Efuse write operation
 * @Notice:   Each efuse address area has specific usage, please apply the area from PM
 *            before write data to the efuse address.
 *
 * @param addr efuse write address
 * @param data efuse write data
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_EFUSE_DRIVER_NOT_INIT: EFUSE driver not init
 *    - BK_ERR_EFUSE_ADDR_OUT_OF_RANGE: EFUSE address is out of range
 *    - BK_ERR_EFUSE_CANNOT_WRTIE: EFUSE cannot write, cannot change bit from 1 to 0
 *    - BK_ERR_EFUSE_WRTIE_NOT_EQUAL: EFUSE data read is not equal to the data written
 *    - others: other errors.
 */
bk_err_t bk_efuse_write_byte(uint8_t addr, uint8_t data);

/**
 * @brief     Efuse read operation
 *
 * @param addr read address
 * @param data pointer to accept value of efuse read data
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_EFUSE_DRIVER_NOT_INIT: EFUSE driver not init
 *    - BK_ERR_EFUSE_ADDR_OUT_OF_RANGE: EFUSE address is out of range
 *    - others: other errors.
 */
bk_err_t bk_efuse_read_byte(uint8_t addr, uint8_t *data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif



