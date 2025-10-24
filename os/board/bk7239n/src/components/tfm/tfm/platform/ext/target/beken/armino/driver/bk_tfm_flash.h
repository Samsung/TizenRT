// Copyright 2022-2023 Beken
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

#include "driver/flash.h"

#define BK_ERR_FLASH_ADDR_NOT_CRC_ALIGNED (BK_ERR_FLASH_BASE - 10)
#define BK_ERR_FLASH_LEN_TOO_BIG          (BK_ERR_FLASH_BASE - 11)

bk_err_t bk_flash_erase_range(uint32_t start_addr, uint32_t end_addr);
bk_err_t bk_flash_erase_range_with_crc(uint32_t start_addr, uint32_t end_addr);
bk_err_t bk_flash_copy(uint32_t src_addr, uint32_t dst_addr, uint32_t len);
bk_err_t bk_flash_copy_with_crc(uint32_t src_addr, uint32_t dst_addr, uint32_t len);
