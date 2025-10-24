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

#include <soc/soc.h>
#include "hal_port.h"
#include "trng_hw.h"
#include <driver/hal/hal_trng_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TRNG_LL_REG_BASE(_trng_unit_id)    (SOC_TRNG_REG_BASE)

static inline void trng_ll_soft_reset(trng_hw_t *hw)
{
	hw->global_ctrl.soft_reset = 1;
}

static inline uint32_t trng_ll_get_device_id(trng_hw_t *hw)
{
	return hw->dev_id;
}

static inline uint32_t trng_ll_get_version_id(trng_hw_t *hw)
{
	return hw->dev_version;
}

static inline uint32_t trng_ll_get_dev_status(trng_hw_t *hw)
{
	return hw->dev_status;
}

static inline void trng_ll_init(trng_hw_t *hw)
{
	trng_ll_soft_reset(hw);
	hw->ctrl.en = 0;
}

static inline void trng_ll_enable(trng_hw_t *hw)
{
	hw->global_ctrl.soft_reset = 1;
	hw->global_ctrl.clk_gate_bypass = 1;    //clock gate enable will make trng get same value every first time get
	hw->ctrl.en = 1;
}

static inline void trng_ll_disable(trng_hw_t *hw)
{
	hw->ctrl.en = 0;
	hw->global_ctrl.clk_gate_bypass = 0;
}

static inline uint32_t trng_ll_get_random_data(trng_hw_t *hw)
{
	return hw->data;
}

#ifdef __cplusplus
}
#endif

