// Copyright 2022-2025 Beken
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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#include <soc/soc.h>
#include "hal_port.h"
#include "system_hw.h"
#include <driver/flash_types.h>

#ifdef __cplusplus
extern "C" {
#endif

flash_protect_type_t psa_flash_get_protect_type(void);
int psa_flash_set_protect_type(uint32_t type);
int psa_flash_erase_sector(uint32_t address);
int psa_flash_read_bytes(uint32_t address, uint8_t *user_buf, uint32_t size);
int psa_flash_write_bytes(uint32_t address, const uint8_t *user_buf, uint32_t size);
int psa_flash_read_instruction(uint32_t address, uint8_t *user_buf, uint32_t size, uint32_t offset_flag);
#ifdef __cplusplus
}
#endif
