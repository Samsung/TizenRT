// Copyright 2020-2025 Beken
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
#include "mpc_hw.h"
#include <driver/hal/hal_mpc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MPC_LL_REG_BASE(mpc_dev)    (mpc_ll_get_reg_base(mpc_dev))

static inline uint32_t mpc_ll_get_reg_base(mpc_dev_t dev)
{
	switch (dev) {
	case MPC_DEV_PSRAM:
		return SOC_MPC_PSRAM_REG_BASE;
	case MPC_DEV_QSPI0:
		return SOC_MPC_QSPI0_REG_BASE;
	case MPC_DEV_QSPI1:
		return SOC_MPC_QSPI1_REG_BASE;
	case MPC_DEV_OTP2:
		return SOC_MPC_OTP_REG_BASE;
	case MPC_DEV_FLASH:
		return SOC_MPC_FLASH_REG_BASE;
	case MPC_DEV_SMEM0:
		return SOC_MPC_SMEM0_REG_BASE;
	case MPC_DEV_SMEM1:
		return SOC_MPC_SMEM1_REG_BASE;
	case MPC_DEV_SMEM2:
		return SOC_MPC_SMEM2_REG_BASE;
	case MPC_DEV_SMEM3:
		return SOC_MPC_SMEM3_REG_BASE;
	case MPC_DEV_SMEM4:
		return SOC_MPC_SMEM4_REG_BASE;
	// case MPC_DEV_SMEM5:        //bk7239  no  smem5
	// 	return SOC_MPC_SMEM5_REG_BASE;
	default:
		return BK_ERR_MPC_BASE;
	}
}

static inline void mpc_ll_init(mpc_hw_t *hw)
{
	hw->ctrl.sec_lock = 0;
	hw->ctrl.cfg_sec_rsp = 1;
}

/* unit is byte */
static inline uint32_t mpc_ll_get_block_size(mpc_hw_t *hw)
{
	return (32 << hw->block_size.block_size);
}

static inline uint32_t mpc_ll_get_max_block_index(mpc_hw_t *hw)
{
	return hw->block_index_max;
}

static inline void mpc_ll_set_block_index(mpc_hw_t *hw, uint32_t index)
{
	hw->block_index = index;
}

static inline uint32_t mpc_ll_get_block_index(mpc_hw_t *hw)
{
	return hw->block_index;
}

static inline void mpc_ll_enable_sec_lock(mpc_hw_t *hw)
{
	hw->ctrl.sec_lock = 1;
}

static inline void mpc_ll_disable_sec_lock(mpc_hw_t *hw)
{
	hw->ctrl.sec_lock = 0;
}

static inline void mpc_ll_enable_auto_increase(mpc_hw_t *hw)
{
	hw->ctrl.auto_inc = 1;
}

static inline void mpc_ll_disable_auto_increase(mpc_hw_t *hw)
{
	hw->ctrl.auto_inc = 0;
}

static inline void mpc_ll_set_block_lut(mpc_hw_t *hw, uint32_t block_lut)
{
	hw->block_lut = block_lut;
}

static inline uint32_t mpc_ll_get_block_lut(mpc_hw_t *hw)
{
	return hw->block_lut;
}

static inline void mpc_ll_enable_sec_response(mpc_hw_t *hw)
{
	hw->ctrl.cfg_sec_rsp = 1;
}

static inline void mpc_ll_disable_sec_response(mpc_hw_t *hw)
{
	hw->ctrl.cfg_sec_rsp = 0;
}

#ifdef __cplusplus
}
#endif

