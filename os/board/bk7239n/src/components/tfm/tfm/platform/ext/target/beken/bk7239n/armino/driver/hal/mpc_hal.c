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

#include "mpc_hal.h"
#include "mpc_ll.h"

#define TAG "mpc_hal"
#define MPC_HAL_LOGD BK_LOGD

static bk_err_t mpc_hal_update_block_lut_val(uint32_t *block_lut, uint32_t lut_start_bit, uint32_t lut_end_bit, mpc_block_secure_type_t secure_type)
{
	BK_RETURN_ON_NULL(block_lut);
	if ((lut_start_bit > lut_end_bit) ||
		(lut_start_bit > MPC_BLOCK_LUT_MAX_BIT_NUM) ||
		(lut_end_bit > MPC_BLOCK_LUT_MAX_BIT_NUM)) {
		return BK_ERR_MPC_INVALID_LUT_PARAM;
	}

	for (uint32_t i = lut_start_bit; i < lut_end_bit; i++) {
		if (secure_type == MPC_BLOCK_NON_SECURE) {
			*block_lut |= BIT(i);
		} else {
			*block_lut &= ~BIT(i);
		}
	}

	return BK_OK;
}

bk_err_t mpc_hal_init(mpc_hal_t *hal)
{
	hal->hw = (mpc_hw_t *)MPC_LL_REG_BASE(hal->dev);
	mpc_ll_init(hal->hw);
	return BK_OK;
}

bk_err_t mpc_hal_config_block_lut(mpc_hal_t *hal, uint32_t mem_block_index, uint32_t mem_block_num, mpc_block_secure_type_t secure_type)
{
	uint32_t lut_start_bit = mem_block_index % MPC_BLOCK_LUT_MAX_BIT_NUM;
	uint32_t lut_block_index = mem_block_index / MPC_BLOCK_LUT_MAX_BIT_NUM;
	uint32_t block_lut_val = 0;
	bk_err_t ret = BK_OK;

	mpc_hal_set_block_index(hal, lut_block_index); 
	block_lut_val = mpc_ll_get_block_lut(hal->hw);

	/* set lut from offset to tail */
	uint32_t lut_left_bit_num = (mem_block_num > (MPC_BLOCK_LUT_MAX_BIT_NUM - lut_start_bit)) ? (MPC_BLOCK_LUT_MAX_BIT_NUM - lut_start_bit) : mem_block_num;
	uint32_t lut_end_bit = lut_start_bit + lut_left_bit_num;

	MPC_HAL_LOGD(TAG, "lut_start_bit=%x lut_val=%x left_bit_num=%d end_bit=%d\r\n",
		lut_start_bit, block_lut_val, lut_left_bit_num, lut_end_bit);
	ret = mpc_hal_update_block_lut_val(&block_lut_val, lut_start_bit, lut_end_bit, secure_type);
	if (ret != BK_OK) {
		return ret;
	}
	mpc_ll_set_block_lut(hal->hw, block_lut_val);

	uint32_t lut_whole_num = (mem_block_num - lut_left_bit_num) / MPC_BLOCK_LUT_MAX_BIT_NUM;

	MPC_HAL_LOGD(TAG, "whole_num=%d\r\n", lut_whole_num);
	for (uint32_t i = 0; i < lut_whole_num; i++) {
		if (secure_type == MPC_BLOCK_NON_SECURE) {
			block_lut_val = (uint32_t)(~0x0);
		} else {
			block_lut_val = (uint32_t)(0x0);
		}
		mpc_hal_set_block_index(hal, ++lut_block_index); 
		mpc_ll_set_block_lut(hal->hw, block_lut_val);
	}

	lut_end_bit = (mem_block_num - lut_left_bit_num) % MPC_BLOCK_LUT_MAX_BIT_NUM;

	MPC_HAL_LOGD(TAG, "lut_end_bit=%d\r\n", lut_end_bit);
	if (lut_end_bit > 0) {
		mpc_hal_set_block_index(hal, ++lut_block_index); 
		block_lut_val = mpc_ll_get_block_lut(hal->hw);
		mpc_hal_update_block_lut_val(&block_lut_val, 0, lut_end_bit, secure_type);
		if (ret != BK_OK) {
			return ret;
		}
		mpc_ll_set_block_lut(hal->hw, block_lut_val);
	}

	return BK_OK;
}

