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
#include "trng_hw.h"
#include "trng_ll.h"
#include <driver/hal/hal_trng_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	trng_hw_t *hw;
	trng_unit_t id;
} trng_hal_t;

#define trng_hal_get_random_number(hal) trng_ll_get_random_data((hal)->hw)

bk_err_t trng_hal_init(trng_hal_t *hal);
bk_err_t trng_hal_start_common(trng_hal_t *hal);
bk_err_t trng_hal_stop_common(trng_hal_t *hal);

#if CFG_HAL_DEBUG_TRNG
void trng_struct_dump(void);
#else
#define trng_struct_dump()
#endif

#ifdef __cplusplus
}
#endif


