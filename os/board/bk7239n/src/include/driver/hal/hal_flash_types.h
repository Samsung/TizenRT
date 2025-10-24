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

#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t flash_unit_t; /**< flash uint id */

typedef enum {
	FLASH_OP_CMD_WREN  = 1,  /**< flash command write enable */
	FLASH_OP_CMD_WRDI  = 2,  /**< flash command write disable */
	FLASH_OP_CMD_RDSR  = 3,  /**< flash command read status register */
	FLASH_OP_CMD_WRSR  = 4,  /**< flash command write status register */
	FLASH_OP_CMD_READ  = 5,  /**< flash command read data */
	FLASH_OP_CMD_RDSR2 = 6,  /**< flash command read status register-2 */
	FLASH_OP_CMD_WRSR2 = 7,  /**< flash command write status register-2 */
	FLASH_OP_CMD_PP    = 12, /**< flash command page program */
	FLASH_OP_CMD_SE    = 13, /**< flash command sector erase */
	FLASH_OP_CMD_BE1   = 14, /**< flash command block erase-1 */
	FLASH_OP_CMD_BE2   = 15, /**< flash command block erase-2 */
	FLASH_OP_CMD_CE    = 16, /**< flash command chip erase */
	FLASH_OP_CMD_DP    = 17,
	FLASH_OP_CMD_RFDP  = 18,
	FLASH_OP_CMD_RDID  = 20, /**< flash command read device id */
	FLASH_OP_CMD_HPM   = 21, /**< flash command high performance mode */
	FLASH_OP_CMD_CRMR  = 22,
	FLASH_OP_CMD_CRMR2 = 23,
} flash_op_cmd_t;

typedef enum
{
	FLASH_MODE_STANDARD = 0, /**< flash standard mode */
	FLASH_MODE_DUAL,         /**< flash dual mode */
	FLASH_MODE_QUAD,         /**< flash quad mode */
} flash_mode_t;

#ifdef __cplusplus
}
#endif

