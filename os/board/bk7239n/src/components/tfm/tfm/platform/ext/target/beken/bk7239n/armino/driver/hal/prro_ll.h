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

#include "prro_hw.h"
#include "soc/soc.h"

#define PRRO_LL_REG_BASE(id) SOC_PRRO_REG_BASE

static inline void prro_ll_init(void)
{
	prro_hw_t *hw = (prro_hw_t*)PRRO_LL_REG_BASE(0);
	hw->global_ctrl.soft_reset = 1;
	hw->ahb_cfg_sresp.v = 0xff;
	hw->apb_cfg_sresp.v = 0x1f;
}

static inline void prro_ll_set_reg(uint32_t reg_offset, uint32_t value)
{
	uint32_t reg = PRRO_LL_REG_BASE(0) + (reg_offset << 2);
	*(volatile uint32_t*)reg = value;
}

static inline void prro_ll_set_reg_bit(uint32_t reg_offset, uint32_t bit, uint32_t value)
{
	uint32_t reg = PRRO_LL_REG_BASE(0) + (reg_offset << 2);

	if (value) {
		*(volatile uint32_t*)reg |= BIT(bit);
	} else {
		*(volatile uint32_t*)reg &= ~(BIT(bit));
	}
}

static inline uint32_t prro_ll_get_reg_bit(uint32_t reg_offset, uint32_t bit)
{
	uint32_t reg = PRRO_LL_REG_BASE(0) + (reg_offset << 2);
	uint32_t bit_val = ((*(volatile uint32_t*)reg) & BIT(bit)) >> bit;

	return bit_val;
}

static inline void prro_ll_set_bit_aon_gpio_nonsec0(uint32_t bit, uint32_t value)
{
	prro_hw_t *hw = (prro_hw_t*)PRRO_LL_REG_BASE(0);

	if (value) {
		hw->aon_gpio_nonsec0 |= BIT(bit);
	} else {
		hw->aon_gpio_nonsec0 &= ~(BIT(bit));
	}
}

static inline void prro_ll_set_bit_aon_gpio_nonsec1(uint32_t bit, uint32_t value)
{
	prro_hw_t *hw = (prro_hw_t*)PRRO_LL_REG_BASE(0);

	if (value) {
		hw->aon_gpio_nonsec1 |= BIT(bit);
	} else {
		hw->aon_gpio_nonsec1 &= ~(BIT(bit));
	}
}

static inline void prro_ll_set_aon_gpio_nonsec0(uint32_t value)
{
	prro_hw_t *hw = (prro_hw_t*)PRRO_LL_REG_BASE(0);
	hw->aon_gpio_nonsec0 = value;
}

static inline void prro_ll_set_aon_gpio_nonsec1(uint32_t value)
{
	prro_hw_t *hw = (prro_hw_t*)PRRO_LL_REG_BASE(0);
	hw->aon_gpio_nonsec1 = value;
}

static inline void prro_ll_set_ahb_cfg_ap(uint32_t value)
{
	prro_hw_t *hw = (prro_hw_t*)PRRO_LL_REG_BASE(0);
	hw->ahb_cfg_ap.v = value;
}

static inline void prro_ll_set_ahb_cfg_nsec(uint32_t value)
{
	prro_hw_t *hw = (prro_hw_t*)PRRO_LL_REG_BASE(0);
	hw->ahb_cfg_nsec.v = value;
}

static inline void prro_ll_set_ahb_cfg_sresp(uint32_t value)
{
	prro_hw_t *hw = (prro_hw_t*)PRRO_LL_REG_BASE(0);
	hw->ahb_cfg_sresp.v = value;
}

static inline void prro_ll_set_apb_cfg_ap(uint32_t value)
{
	prro_hw_t *hw = (prro_hw_t*)PRRO_LL_REG_BASE(0);
	hw->apb_cfg_ap.v = value;
}

static inline void prro_ll_set_apb_cfg_nsec(uint32_t value)
{
	prro_hw_t *hw = (prro_hw_t*)PRRO_LL_REG_BASE(0);
	hw->apb_cfg_nsec.v = value;
}

static inline void prro_ll_set_apb_cfg_sresp(uint32_t value)
{
	prro_hw_t *hw = (prro_hw_t*)PRRO_LL_REG_BASE(0);
	hw->apb_cfg_sresp.v = value;
}

