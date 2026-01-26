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

#include <components/log.h>
#include <driver/flash_types.h>

#define FLASH_TAG "flash"
#define FLASH_LOGI(...) BK_LOGI(FLASH_TAG, ##__VA_ARGS__)
#define FLASH_LOGW(...) BK_LOGW(FLASH_TAG, ##__VA_ARGS__)
#define FLASH_LOGE(...) BK_LOGE(FLASH_TAG, ##__VA_ARGS__)
#define FLASH_LOGD(...) BK_LOGD(FLASH_TAG, ##__VA_ARGS__)

#define FLASH_SIZE_1M                    0x100000
#define FLASH_SIZE_2M                    0x200000
#define FLASH_SIZE_4M                    0x400000
#define FLASH_SIZE_8M                    0x800000
#define FLASH_SIZE_16M                   0x1000000
#define FLASH_STATUS_REG_PROTECT_MASK    0xff
#define FLASH_STATUS_REG_PROTECT_OFFSET  8
#define FLASH_CMP_MASK                   0x1
#define FLASH_BUFFER_LEN                 8
#define FLASH_BYTES_CNT                  32
#define FLASH_ADDRESS_MASK               0x1f
#define FLASH_ERASE_SECTOR_MASK          0xFFF000
#define FLASH_ERASE_BLOCK_64K_MASK       0xFF0000
#define FLASH_ERASE_BLOCK_32K_MASK       0xFF8000
#define FLASH_ERASE_BLOCK_MASK       	 FLASH_ERASE_BLOCK_64K_MASK
#define FLASH_SECTOR_SIZE_MASK           0x000FFF
#define FLASH_SECTOR_SIZE                0x1000 /* each sector has 4K bytes */
#define FLASH_SECTOR_SIZE_OFFSET         12
#define FLASH_PAGE_SIZE                  256 /* each page has 256 bytes */
#define FLASH_DPLL_DIV_VALUE_TEN         3
#define FLASH_DPLL_DIV_VALUE_SIX         1
#define FLASH_ManuFacID_POSI             (16)
#define FLASH_ManuFacID_GD               (0xC8)
#define FLASH_ManuFacID_TH               (0xCD)

typedef struct {
	uint32_t flash_id;
	uint8_t status_reg_size; /**< the byte count of status register */
	uint32_t flash_size;
	flash_line_mode_t line_mode;
	uint8_t cmp_post; /**< CMP bit position in status register */
	uint8_t protect_post; /**< block protect bits position in status register */
	uint8_t protect_mask; /**< block protect bits mask value in status register */
	uint16_t protect_all;
	uint16_t protect_none;
	uint16_t protect_half;
	uint16_t unprotect_last_block;
	uint8_t quad_en_post; /**< quad enable bit position in status register */
	uint8_t quad_en_val; /**< When the QE pin is set to quad_en_val(1 or 0), the Quad IO2 and IO3 pins are enabled */
	uint8_t coutinuous_read_mode_bits_val;
	uint8_t mode_sel;
} flash_config_t;
