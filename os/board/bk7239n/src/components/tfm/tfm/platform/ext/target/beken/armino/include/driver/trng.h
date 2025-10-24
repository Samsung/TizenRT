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
#include <driver/trng_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/* @brief Overview about this API header
 *
 */

/**
 * @brief TRNG API
 * @defgroup bk_api_trng TRNG API group
 * @{
 */

/**
 * @brief     Init the TRNG driver
 *
 * This API init the resoure common:
 *   - Init TRNG driver control memory
 *
 * @attention 1. This API should be called before any other TRNG APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_trng_driver_init(void);

/**
 * @brief     Deinit the TRNG driver
 *
 * This API free all resource related to TRNG and disable TRNG.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_trng_driver_deinit(void);

/**
 * @brief     Start the TRNG
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_TRNG_DRIVER_NOT_INIT: TRNG driver not init
 *    - others: other errors.
 */
bk_err_t bk_trng_start(void);

/**
 * @brief     Stop the TRNG
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_TRNG_DRIVER_NOT_INIT: TRNG driver not init
 *    - others: other errors.
 */
bk_err_t bk_trng_stop(void);

/**
 * @brief     Get the random number
 *
 * @return random number between 0 and RAND_MAX
 */
int bk_rand(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


