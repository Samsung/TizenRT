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

#include <common/bk_include.h>
#include <driver/hal/hal_io_matrix_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief IO_MUX defines
 * @defgroup bk_api_iomx_defs macos
 * @ingroup bk_api_iomx
 * @{
 */
#define BK_ERR_IO_MATRIX_DRIVER_NOT_INIT    (BK_ERR_IO_MATRIX_BASE - 1) /**< IO_MUX driver not init */
#define BK_ERR_IO_MATRIX_NOT_INIT           (BK_ERR_IO_MATRIX_BASE - 2) /**< IO_MUX not init */
#define BK_ERR_IO_MATRIX_INVALID_GPIO       (BK_ERR_IO_MATRIX_BASE - 3) /**< IO_MUX invalid gpio id*/
#define BK_ERR_IO_MATRIX_DEBUG_DISABLED     (BK_ERR_IO_MATRIX_BASE - 4) /**< IO_MUX disabled via cmdline flag */
#define BK_ERR_IO_MATRIX_INVALID_FUNC_CODE  (BK_ERR_IO_MATRIX_BASE - 5) /**< IO_MUX invalid function code*/

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


