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
#include "efuse_hw.h"
#include <driver/hal/hal_efuse_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EFUSE_LL_REG_BASE(_efuse_unit_id)    (SOC_EFUSE_REG_BASE)

static inline void efuse_ll_soft_reset(efuse_hw_t *hw)
{
	hw->global_ctrl.soft_reset = 1;
}

static inline uint32_t efuse_ll_get_device_id(efuse_hw_t *hw)
{
	return hw->dev_id;
}

static inline uint32_t efuse_ll_get_version_id(efuse_hw_t *hw)
{
	return hw->dev_version;
}

static inline uint32_t efuse_ll_get_dev_status(efuse_hw_t *hw)
{
	return hw->dev_status;
}


static inline void efuse_ll_reset_config_to_default(efuse_hw_t *hw)
{
	efuse_ll_soft_reset(hw);

	hw->ctrl.addr = 0;
	hw->ctrl.wr_data = 0;

	hw->optr.rd_data = 0;
	hw->optr.rd_data_valid = 0;
}

static inline void efuse_ll_init(efuse_hw_t *hw)
{
	efuse_ll_soft_reset(hw);
	efuse_ll_reset_config_to_default(hw);
}

static inline void efuse_ll_enable(efuse_hw_t *hw)
{
	hw->ctrl.en = 1;
}

static inline void efuse_ll_disable(efuse_hw_t *hw)
{
	hw->ctrl.en = 0;
}

static inline bool efuse_ll_is_operate_finished(efuse_hw_t *hw)
{
	return (hw->ctrl.en == 0);
}

static inline void efuse_ll_set_direction_write(efuse_hw_t *hw)
{
	hw->ctrl.dir = 0x1;
}

static inline void efuse_ll_set_direction_read(efuse_hw_t *hw)
{
	hw->ctrl.dir = 0x0;
}

static inline void efuse_ll_set_addr(efuse_hw_t *hw, uint8_t addr)
{
	hw->ctrl.addr = addr & EFUSE_F_ADDR_M;
}

static inline void efuse_ll_set_wr_data(efuse_hw_t *hw, uint8_t data)
{
	hw->ctrl.v &= (~(EFUSE_F_WR_DATA_M << EFUSE_F_WR_DATA_S));
	hw->ctrl.v |= ((data & EFUSE_F_WR_DATA_M) << EFUSE_F_WR_DATA_S);
}

static inline bool efuse_ll_is_rd_data_valid(efuse_hw_t *hw)
{
	return !!(hw->optr.rd_data_valid);
}

static inline uint8_t efuse_ll_get_rd_data(efuse_hw_t *hw)
{
	return hw->optr.rd_data & EFUSE_F_RD_DATA_M;
}

static inline void efuse_ll_enable_vdd25(efuse_hw_t *hw)
{
	hw->ctrl.vdd25_en = 1;
}

static inline void efuse_ll_disable_vdd25(efuse_hw_t *hw)
{
	hw->ctrl.vdd25_en = 0;
}

#ifdef __cplusplus
}
#endif


