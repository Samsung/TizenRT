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
#if CONFIG_TFM_BK7236_V5
#include "prro_hw_v5.h"
#include "prro_ll_v5.h"
#else
#include "prro_hw.h"
#include "prro_ll.h"
#endif
#include "driver/prro_types.h"
#include <driver/hal/hal_prro_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	prro_hw_t *hw;
} prro_hal_t;

bk_err_t prro_hal_init(void);
bk_err_t prro_hal_set_privilege(prro_dev_t dev, prro_privilege_type_t privilege_type);
bk_err_t prro_hal_set_ahb_dev_privilege(uint32_t ahb_priv_bits);
bk_err_t prro_hal_set_apb_dev_privilege(uint32_t ahb_priv_bits);

bk_err_t prro_hal_set_secure(prro_dev_t dev, prro_secure_type_t secure_type);
bk_err_t prro_hal_set_gpios_secure(uint32_t gpio_0_31_bits, uint32_t gpio_32_63_bits);
bk_err_t prro_hal_set_ahb_dev_secure(uint32_t ahb_dev_sec_bits);
bk_err_t prro_hal_set_apb_dev_secure(uint32_t apb_dev_sec_bits);
bk_err_t prro_hal_set_hnonsec_dev_secure(uint32_t hnonsec_dev_sec_bits);

prro_secure_type_t prro_hal_get_secure(prro_dev_t dev);

#if CONFIG_PRRO_TEST
void prro_struct_dump(void);
#else
#define prro_struct_dump()
#endif

#ifdef __cplusplus
}
#endif

