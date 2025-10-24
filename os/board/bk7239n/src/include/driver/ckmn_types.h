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
#include <driver/hal/hal_ckmn_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief CKMN defines
 * @defgroup bk_api_ckmn_defs macos
 * @ingroup bk_api_ckmn
 * @{
 */

#define BK_ERR_CKMN_DRIVER_NOT_INIT    (BK_ERR_CKMN_BASE - 1) /**< CKMN driver not init */
#define BK_ERR_CKMN_INT_TYPE           (BK_ERR_CKMN_BASE - 2) /**< CKMN unknow int type */
#define BK_ERR_CKMN_CKEST_BUSY         (BK_ERR_CKMN_BASE - 3) /**< CKMN skest is busy*/

typedef enum{
	CKMN_INT_CKEST,
	CKMN_INT_26M,
	CKMN_INT_32K,
	CKMN_INT_MAX,
} ckmn_int_type_t;
/**
 * @brief DMA interrupt service routine
 */
typedef void (*ckmn_isr_t)(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

