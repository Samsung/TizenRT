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
#include "flash_hw.h"
#include "flash_ll.h"
#include <driver/hal/hal_flash_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	flash_hw_t *hw;
	flash_unit_t id;
} flash_hal_t;

#define flash_hal_is_busy(hal) flash_ll_is_busy((hal)->hw)
#define flash_hal_wait_op_done(hal) flash_ll_wait_op_done((hal)->hw)
#define flash_hal_get_id(hal) flash_ll_get_id((hal)->hw)
#define flash_hal_get_mid(hal) flash_ll_get_mid((hal)->hw)
#define flash_hal_read_status_reg(hal, sr_width) flash_ll_read_status_reg((hal)->hw, sr_width)
#define flash_hal_write_status_reg(hal, sr_width, sr_data) flash_ll_write_status_reg((hal)->hw, sr_width, sr_data)
#define flash_hal_get_crc_err_num(hal) flash_ll_get_crc_err_num((hal)->hw)
#define flash_hal_enable_cpu_data_wr(hal) flash_ll_enable_cpu_data_wr((hal)->hw)
#define flash_hal_disable_cpu_data_wr(hal) flash_ll_disable_cpu_data_wr((hal)->hw)
#define flash_hal_clear_qwfr(hal) flash_ll_clear_qwfr((hal)->hw)
#define flash_hal_set_mode(hal, mode_sel) flash_ll_set_mode((hal)->hw, mode_sel)
#define flash_hal_set_dual_mode(hal) flash_ll_set_dual_mode((hal)->hw)
#define flash_hal_set_quad_m_value(hal, m_value) flash_ll_set_quad_m_value((hal)->hw, m_value)
#define flash_hal_set_qe(hal, qe_bit, qe_bit_post) flash_ll_set_qe((hal)->hw, qe_bit, qe_bit_post)
#define flash_hal_set_clk(hal, clk_cfg) flash_ll_set_clk((hal)->hw, clk_cfg)
#define flash_hal_set_default_clk(hal) flash_ll_set_default_clk((hal)->hw)
#define flash_hal_set_clk_dpll(hal) flash_ll_set_clk_dpll((hal)->hw)
#define flash_hal_set_clk_dco(hal, ate_enabled) flash_ll_set_clk_dco((hal)->hw, ate_enabled)
#define flash_hal_erase_block(hal, erase_addr, type) flash_ll_erase_block((hal)->hw, erase_addr, type)
#define flash_hal_set_op_cmd_read(hal, read_addr) flash_ll_set_op_cmd_read((hal)->hw, read_addr)
#define flash_hal_read_data(hal) flash_ll_read_data((hal)->hw)
#define flash_hal_set_op_cmd_write(hal, write_addr) flash_ll_set_op_cmd_write((hal)->hw, write_addr)
#define flash_hal_write_data(hal, data) flash_ll_write_data((hal)->hw, data)
#define flash_hal_write_enable(hal) flash_ll_write_enable((hal)->hw)
#define flash_hal_write_disable(hal) flash_ll_write_disable((hal)->hw)
#define flash_hal_read_offset_enable(hal) flash_ll_read_offset_enable((hal)->hw)
#define flash_hal_set_volatile_status_write(hal) flash_ll_set_volatile_status_write((hal)->hw)

bk_err_t flash_hal_init(flash_hal_t *hal);
uint16_t flash_hal_get_protect_value(flash_hal_t *hal, uint8_t status_reg_size,
									uint8_t protect_post, uint8_t protect_mask, uint8_t cmp_post);

#if CFG_HAL_DEBUG_FLASH
void flash_struct_dump(void);
#else
#define flash_struct_dump()
#endif

#ifdef __cplusplus
}
#endif

