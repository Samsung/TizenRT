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
#include "vault_hw.h"
#include "vault_ll.h"
//#include <driver/hal/hal_vault_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	vault_hw_t *hw;
	vault_unit_t id;
} vault_hal_t;

bk_err_t vault_hal_init(vault_hal_t *hal);
bk_err_t vault_hal_start_common(vault_hal_t *hal);
bk_err_t vault_hal_stop_common(vault_hal_t *hal);

#if CFG_HAL_DEBUG_VAULT
void vault_struct_dump(void);
#else
#define vault_struct_dump()
#endif

#ifdef __cplusplus
}
#endif


