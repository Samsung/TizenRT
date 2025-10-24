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
#include "ckmn_hw.h"
#include "ckmn_ll.h"
#include <driver/hal/hal_ckmn_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	ckmn_hw_t *hw;
	uint32_t id;
} ckmn_hal_t;

bk_err_t ckmn_hal_init(ckmn_hal_t *hal);
bk_err_t ckmn_hal_set_soft_reset(ckmn_hal_t *hal);
bk_err_t ckmn_hal_clk_bypass(ckmn_hal_t *hal, uint32_t v);
bk_err_t ckmn_hal_set_rc32k_count(ckmn_hal_t *hal, uint32_t v);
bk_err_t ckmn_hal_get_rc32k_count(ckmn_hal_t *hal);
bk_err_t ckmn_hal_rc32k_enable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_rc32k_disable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_rc32k_intr_enable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_rc32k_intr_disable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_get_rc26m_count(ckmn_hal_t *hal);
bk_err_t ckmn_hal_set_26m_target(ckmn_hal_t *hal, ckmn_acc_t target);
bk_err_t ckmn_hal_get_26m_target(ckmn_hal_t *hal);
bk_err_t ckmn_hal_set_32k_target(ckmn_hal_t *hal, ckmn_acc_t target);
bk_err_t ckmn_hal_get_32k_target(ckmn_hal_t *hal);
bk_err_t ckmn_hal_autosw_26m_enable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_autosw_26m_disable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_corr_26m_enable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_corr_26m_disable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_autosw_32k_enable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_autosw_32k_disable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_corr_32k_enable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_corr_32k_disable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_cor26m_intr_enable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_cor26m_intr_disable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_cor32k_intr_enable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_cor32k_intr_disable(ckmn_hal_t *hal);
bk_err_t ckmn_hal_get_intr_status(ckmn_hal_t *hal);
bk_err_t ckmn_hal_clear_ckest_intr_status(ckmn_hal_t *hal);
bk_err_t ckmn_hal_clear_cor26m_intr_status(ckmn_hal_t *hal);
bk_err_t ckmn_hal_clear_cor32k_intr_status(ckmn_hal_t *hal);
bk_err_t ckmn_hal_get_ckest_intr_status(ckmn_hal_t *hal);
bk_err_t ckmn_hal_get_cor26m_intr_status(ckmn_hal_t *hal);
bk_err_t ckmn_hal_get_cor32k_intr_status(ckmn_hal_t *hal);

#if CFG_HAL_DEBUG_TRNG
void ckmn_struct_dump(void);
#else
#define ckmn_struct_dump()
#endif

#ifdef __cplusplus
}
#endif


