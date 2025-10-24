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

#include "hal_config.h"
#include "efuse_hw.h"
#include "efuse_ll.h"
#include <driver/hal/hal_efuse_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	efuse_hw_t *hw;
	efuse_unit_t id;
} efuse_hal_t;

#define efuse_hal_disable(hal) efuse_ll_disable((hal)->hw)
#define efuse_hal_reset_config_to_default(hal) efuse_ll_reset_config_to_default((hal)->hw)

bk_err_t efuse_hal_init(efuse_hal_t *hal);
bk_err_t efuse_hal_write(efuse_hal_t *hal, uint8_t addr, uint8_t wr_data);
bk_err_t efuse_hal_read(efuse_hal_t *hal, uint8_t addr, uint8_t *data);

#if CFG_HAL_DEBUG_EFUSE
void efuse_struct_dump(void);
#else
#define efuse_struct_dump()
#endif

#ifdef __cplusplus
}
#endif



