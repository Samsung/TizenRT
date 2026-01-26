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
#include "flash_hw.h"
#include <driver/hal/hal_flash_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FLASH_LL_REG_BASE(_flash_unit_id)    (SOC_FLASH_REG_BASE)

static inline void flash_ll_soft_reset(flash_hw_t *hw)
{
	hw->global_ctrl.soft_reset = 1;
}

static inline void flash_ll_init(flash_hw_t *hw)
{
	flash_ll_soft_reset(hw);
}

static inline bool flash_ll_is_busy(flash_hw_t *hw)
{
	return hw->op_ctrl.busy_sw;
}

static inline void flash_ll_wait_op_done(flash_hw_t *hw)
{
	while (flash_ll_is_busy(hw));
}

static inline uint32_t flash_ll_read_flash_id(flash_hw_t *hw)
{
	return hw->rd_flash_id;
}

static inline void flash_ll_set_op_cmd(flash_hw_t *hw, flash_op_cmd_t cmd)
{
	hw->op_cmd.op_type_sw = cmd;
	hw->op_ctrl.op_sw = 1;
	hw->op_ctrl.wp_value = 1;
}

static inline uint32_t flash_ll_get_id(flash_hw_t *hw)
{
	flash_ll_wait_op_done(hw);
	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_RDID);
	flash_ll_wait_op_done(hw);
	return flash_ll_read_flash_id(hw);
}

static inline uint32_t flash_ll_get_mid(flash_hw_t *hw)
{
	flash_ll_wait_op_done(hw);
	hw->op_cmd.op_type_sw = FLASH_OP_CMD_RDID;
	hw->op_ctrl.op_sw = 1;
	flash_ll_wait_op_done(hw);
	return flash_ll_read_flash_id(hw);
}

static inline void flash_ll_init_wrsr_cmd(flash_hw_t *hw, uint8_t wrsr_cmd)
{
	flash_ll_wait_op_done(hw);
	hw->cmd_cfg.wrsr_cmd_reg = wrsr_cmd;
	hw->cmd_cfg.wrsr_cmd_sel = 1;
	flash_ll_wait_op_done(hw);
}

static inline void flash_ll_init_rdsr_cmd(flash_hw_t *hw, uint8_t rdsr_cmd)
{
	flash_ll_wait_op_done(hw);
	hw->cmd_cfg.rdsr_cmd_reg = rdsr_cmd;
	hw->cmd_cfg.rdsr_cmd_sel = 1;
	flash_ll_wait_op_done(hw);
}

static inline void flash_ll_deinit_wrsr_cmd(flash_hw_t *hw)
{
	flash_ll_wait_op_done(hw);
	hw->cmd_cfg.wrsr_cmd_reg = 0x1;
	hw->cmd_cfg.wrsr_cmd_sel = 0;
	flash_ll_wait_op_done(hw);
}

static inline void flash_ll_deinit_rdsr_cmd(flash_hw_t *hw)
{
	flash_ll_wait_op_done(hw);
	hw->cmd_cfg.rdsr_cmd_reg = 0x5;
	hw->cmd_cfg.rdsr_cmd_sel = 0;
	flash_ll_wait_op_done(hw);
}

static inline void flash_ll_set_volatile_status_write(flash_hw_t *hw)
{
	while (flash_ll_is_busy(hw));
	hw->flash_ctrl.susres_cmd_reg = 0x50;
	hw->flash_ctrl.susres_cmd_sel = 0x1;
	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WRSR);
	while (flash_ll_is_busy(hw));
	hw->flash_ctrl.susres_cmd_sel = 0x0;
}

static inline void flash_ll_write_status_reg(flash_hw_t *hw, uint8_t sr_width, uint32_t sr_data)
{
	#if CONFIG_FLASH_WRITE_STATUS_VOLATILE
		flash_ll_set_volatile_status_write(hw);
	#endif

	while (flash_ll_is_busy(hw));
	hw->cmd_cfg.v = 0;
	hw->config.wrsr_data = sr_data;
	hw->op_ctrl.wp_value = 1;
	if (sr_width == 1) {
		flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WRSR);
	} else if (sr_width == 2) {
		flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WRSR2);
	} else {
		if(FLASH_ID_GD25Q32C == flash_ll_get_id(hw) || FLASH_ID_TH25Q64 == flash_ll_get_id(hw)) {
			flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WRSR);

			while (flash_ll_is_busy(hw));
			hw->config.wrsr_data = (sr_data >> LEN_WRSR_S0_S7);
			flash_ll_init_wrsr_cmd(hw, CMD_WRSR_S8_S15);
			// hw->op_ctrl.wp_value = 1;    //  ???
			flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WRSR);

			#if 0
			while (flash_ll_is_busy(hw));
			hw->config.wrsr_data = (sr_data >> LEN_WRSR_S8_S15);
			flash_ll_init_wrsr_cmd(hw, CMD_WRSR_S16_S24);
			flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WRSR);
			#endif

			while (flash_ll_is_busy(hw));

			// flash_ll_deinit_wrsr_cmd(hw);
			hw->cmd_cfg.v = 0;
		} else {
			flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WRSR2);
		}
	}

	while (flash_ll_is_busy(hw));
	hw->op_ctrl.wp_value = 0;
}

static inline void flash_ll_set_qe(flash_hw_t *hw, uint8_t qe_bit, uint8_t qe_bit_post)
{
	flash_ll_wait_op_done(hw);
	hw->config.v |= qe_bit << qe_bit_post;
}

static inline uint32_t flash_ll_read_status_reg(flash_hw_t *hw, uint8_t sr_width)
{
	uint32_t state_reg_data = 0;

	flash_ll_wait_op_done(hw);
	hw->cmd_cfg.v = 0;
	flash_ll_wait_op_done(hw);
	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_RDSR);
	flash_ll_wait_op_done(hw);
	state_reg_data = hw->state.status_reg;

	if (sr_width ==1) return state_reg_data;

	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_RDSR2);
	flash_ll_wait_op_done(hw);
	state_reg_data |= hw->state.status_reg << 8;

	if (sr_width ==2) return state_reg_data;

	hw->cmd_cfg.rdsr_cmd_sel = 1;
	hw->cmd_cfg.rdsr_cmd_reg = 0x15;
	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_RDSR);
	flash_ll_wait_op_done(hw);
	state_reg_data |= hw->state.status_reg << 16;
	hw->cmd_cfg.v = 0;

	return state_reg_data;
}

static inline void flash_ll_enable_cpu_data_wr(flash_hw_t *hw)
{
	flash_ll_wait_op_done(hw);
	hw->config.cpu_data_wr_en = 1;
}

static inline void flash_ll_disable_cpu_data_wr(flash_hw_t *hw)
{
	flash_ll_wait_op_done(hw);
	hw->config.cpu_data_wr_en = 0;
}

static inline void flash_ll_clear_qwfr(flash_hw_t *hw)
{
	flash_ll_wait_op_done(hw);
	hw->config.mode_sel = 0;
	hw->op_cmd.addr_sw_reg = 0;
	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_CRMR);
	flash_ll_wait_op_done(hw);
}

static inline void flash_ll_set_mode(flash_hw_t *hw, uint8_t mode_sel)
{
	flash_ll_wait_op_done(hw);
	hw->config.mode_sel = mode_sel;
}

static inline void flash_ll_set_dual_mode(flash_hw_t *hw)
{
	flash_ll_wait_op_done(hw);
	hw->config.mode_sel = FLASH_MODE_DUAL;
}

static inline void flash_ll_set_quad_m_value(flash_hw_t *hw, uint32_t m_value)
{
	flash_ll_wait_op_done(hw);
	hw->state.m_value = m_value;
}

static inline void flash_ll_erase_block(flash_hw_t *hw, uint32_t erase_addr, int type)
{
	flash_ll_wait_op_done(hw);
	hw->op_cmd.addr_sw_reg = erase_addr;
	hw->op_cmd.op_type_sw = type;
	hw->op_ctrl.op_sw = 1;
	flash_ll_wait_op_done(hw);
}

static inline void flash_ll_set_op_cmd_read(flash_hw_t *hw, uint32_t read_addr)
{
	flash_ll_wait_op_done(hw);
	hw->op_cmd.addr_sw_reg = read_addr;
	hw->op_cmd.op_type_sw = FLASH_OP_CMD_READ;
	hw->op_ctrl.op_sw = 1;
	flash_ll_wait_op_done(hw);
}

static inline uint32_t flash_ll_read_data(flash_hw_t *hw)
{
	return hw->data_flash_sw;
}

static inline void flash_ll_set_op_cmd_write(flash_hw_t *hw, uint32_t write_addr)
{
	flash_ll_wait_op_done(hw);
	hw->op_cmd.addr_sw_reg = write_addr;
	hw->op_cmd.op_type_sw = FLASH_OP_CMD_PP;
	hw->op_ctrl.op_sw = 1;
	flash_ll_wait_op_done(hw);
}

static inline void flash_ll_write_data(flash_hw_t *hw, uint32_t data)
{
	flash_ll_wait_op_done(hw);
	hw->data_sw_flash = data;
}

static inline void flash_ll_set_clk(flash_hw_t *hw, uint8_t clk_cfg)
{
	flash_ll_wait_op_done(hw);
	hw->config.clk_cfg = clk_cfg;

#if defined(CONFIG_JTAG)
	hw->config.crc_en = 0;
#endif
}

static inline void flash_ll_set_default_clk(flash_hw_t *hw)
{
	flash_ll_set_clk(hw, 0x5);
}

static inline void flash_ll_set_clk_dpll(flash_hw_t *hw)
{
	flash_ll_wait_op_done(hw);
	hw->config.clk_cfg = 5;
}

static inline void flash_ll_set_clk_dco(flash_hw_t *hw, bool ate_enabled)
{
	flash_ll_wait_op_done(hw);
	if (ate_enabled) {
		hw->config.clk_cfg = 0xB;
	} else {
		hw->config.clk_cfg = 0x9;
	}
}

static inline void flash_ll_write_enable(flash_hw_t *hw)
{
	flash_ll_wait_op_done(hw);
	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WREN);
	flash_ll_wait_op_done(hw);
}

static inline void flash_ll_write_disable(flash_hw_t *hw)
{
	flash_ll_wait_op_done(hw);
	flash_ll_set_op_cmd(hw, FLASH_OP_CMD_WRDI);
	flash_ll_wait_op_done(hw);
}

static inline uint32_t flash_ll_read_offset_enable(flash_hw_t *hw)
{
	flash_ll_wait_op_done(hw);
	return hw->flash_ctrl.flash_offset_enable & 0x1;
}

#ifdef __cplusplus
}
#endif


