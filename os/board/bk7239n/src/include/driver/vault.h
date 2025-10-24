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

#include <driver/vault_types.h>


#ifdef __cplusplus
extern "C" {
#endif


/* @brief Overview about this API header
 *
 */

/**
 * @brief vault API
 * @defgroup bk_api_vault vault API group
 * @{
 */
 
/**
 * @brief vault init operation
 *
 * @param module_name Specifies the module that control the vault
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_vault_driver_init(security_module_name_t module_name);

/**
 * @brief vault deinit operation
 *
 * @param module_name Specifies the module that control the vault
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_vault_driver_deinit(security_module_name_t module_name);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


