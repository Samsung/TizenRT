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
#include "mpc_hw.h"
#include "mpc_ll.h"
#include <driver/hal/hal_mpc_types.h>
#include "sdkconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	mpc_hw_t *hw;
	mpc_dev_t dev;
} mpc_hal_t;

#define MPC_BLOCK_LUT_MAX_BIT_NUM    (32)

#define mpc_hal_get_block_size(hal) mpc_ll_get_block_size((hal)->hw)
#define mpc_hal_enable_sec_lock(hal) mpc_ll_enable_sec_lock((hal)->hw)
#define mpc_hal_get_max_block_index(hal) mpc_ll_get_max_block_index((hal)->hw)
#define mpc_hal_set_block_index(hal, index) mpc_ll_set_block_index((hal)->hw, index)
#define mpc_hal_get_block_index(hal) mpc_ll_get_block_index((hal)->hw)
#define mpc_hal_enable_auto_increase(hal) mpc_ll_enable_auto_increase((hal)->hw)
#define mpc_hal_disable_auto_increase(hal) mpc_ll_disable_auto_increase((hal)->hw)
#define mpc_hal_get_block_lut(hal) mpc_ll_get_block_lut((hal)->hw)
#define mpc_hal_enable_sec_exception(hal) mpc_ll_enable_sec_response((hal)->hw)
#define mpc_hal_disable_sec_exception(hal) mpc_ll_disable_sec_response((hal)->hw)

bk_err_t mpc_hal_init(mpc_hal_t *hal);
bk_err_t mpc_hal_config_block_lut(mpc_hal_t *hal, uint32_t block_offset, uint32_t block_num, mpc_block_secure_type_t secure_type);

#if CONFIG_MPC_TEST
void mpc_struct_dump(mpc_dev_t mpc_dev);
#else
#define mpc_struct_dump(mpc_dev)
#endif

#ifdef __cplusplus
}
#endif


