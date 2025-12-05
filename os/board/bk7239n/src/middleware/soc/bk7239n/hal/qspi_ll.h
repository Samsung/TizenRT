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

#include "qspi_hw.h"
#include <driver/hal/hal_qspi_types.h>
#include <soc/soc.h>
#include "sys_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QSPI_LL_REG_BASE(_qspi_unit_id)            (SOC_QSPI0_REG_BASE)
#define QSPI_DATA_LL_REG_BASE(_qspi_unit_id)       (SOC_QSPI0_DATA_BASE)

#define QSPI0_LL_CLK_PIN     GPIO_2
#define QSPI0_LL_CSN_PIN     GPIO_3
#define QSPI0_LL_IO0_PIN     GPIO_4
#define QSPI0_LL_IO1_PIN     GPIO_5
#define QSPI0_LL_IO2_PIN     GPIO_6
#define QSPI0_LL_IO3_PIN     GPIO_7

#define READ_ID_CMD            0x9F
#define ENTER_QUAD_MODE_CMD    0x35
#define EXIT_QUAD_MODE_CMD     0xF5
#define QUAD_WRITE_CMD         0x38
#define WRITE_CMD              0x02
#define FAST_READ_QUAD         0xEB
#define FAST_READ              0x0B
#define READ_CMD               0x03

#define FLASH_READ_ID_CMD          0x9F
#define FLASH_WR_S0_S7_CMD         0x1
#define FLASH_WR_S8_S15_CMD        0x31
#define FLASH_RD_S0_S7_CMD         0x5
#define FLASH_RD_S8_S15_CMD        0x35
#define FLASH_WR_EN_CMD            0x6
#define FLASH_WR_CMD               0x2
#define FLASH_RD_CMD               0x3
#define FLASH_QUAD_WR_CMD          0x32
#define FLASH_QUAD_RD_CMD          0xeb
#define FLASH_ERASE_SECTOR_CMD     0x20

//TODO init more
static inline void qspi_ll_init(qspi_hw_t *hw)
{
	hw->glb_ctrl.soft_reset = 1;
}

static inline void qspi_ll_set_cmd_a_l(qspi_hw_t *hw, uint32_t cmd_value)
{
	hw->cmd_a_l.v = cmd_value;
}

static inline void qspi_ll_set_cmd_a_h(qspi_hw_t *hw, uint32_t cmd_value)
{
	hw->cmd_a_h.v = cmd_value;
}

static inline void qspi_ll_set_cmd_a_cfg1(qspi_hw_t *hw, uint32_t cmd_line)
{
	hw->cmd_a_cfg1.v = cmd_line;
}

static inline void qspi_ll_set_cmd_a_cfg2(qspi_hw_t *hw, uint32_t cmd_config)
{
	hw->cmd_a_cfg2.v = cmd_config;
}

static inline void qspi_ll_set_cmd_a_data_line(qspi_hw_t *hw, qspi_wire_mode_t data_line)
{
	hw->cmd_a_cfg2.data_line = data_line;
}

static inline void qspi_ll_set_cmd_a_dummy_clock(qspi_hw_t *hw, uint32_t dummy_clock)
{
	hw->cmd_a_cfg2.dummy_clock = dummy_clock & QSPI_F_DUMMY_CLK_M;
}

static inline void qspi_ll_set_cmd_b_l(qspi_hw_t *hw, uint32_t cmd_value)
{
	hw->cmd_b_l.v = cmd_value;
}

static inline void qspi_ll_set_cmd_b_h(qspi_hw_t *hw, uint32_t cmd_value)
{
	hw->cmd_b_h.v = cmd_value;
}

static inline void qspi_ll_set_cmd_b_cfg1(qspi_hw_t *hw, uint32_t cmd_line)
{
	hw->cmd_b_cfg1.v = cmd_line;
}

static inline void qspi_ll_set_cmd_b_data_line(qspi_hw_t *hw, qspi_wire_mode_t data_line)
{
	hw->cmd_b_cfg2.data_line = data_line;
}

static inline void qspi_ll_set_cmd_b_dummy_clock(qspi_hw_t *hw, uint32_t dummy_clock)
{
	hw->cmd_b_cfg2.dummy_clock = dummy_clock & QSPI_F_DUMMY_CLK_M;
}

static inline void qspi_ll_set_cmd_c_l(qspi_hw_t *hw, uint32_t cmd_value)
{
	hw->cmd_c_l.v = cmd_value;
}

static inline uint32_t qspi_ll_get_cmd_c_l(qspi_hw_t *hw)
{
	return hw->cmd_c_l.v;
}

static inline void qspi_ll_set_cmd_c_h(qspi_hw_t *hw, uint32_t cmd_value)
{
	hw->cmd_c_h.v = cmd_value;
}

static inline uint32_t qspi_ll_get_cmd_c_h(qspi_hw_t *hw)
{
	return hw->cmd_c_h.v;
}

static inline void qspi_ll_set_cmd_c_cfg1(qspi_hw_t *hw, uint32_t cmd_line)
{
	hw->cmd_c_cfg1.v = cmd_line;
}

static inline void qspi_ll_set_cmd_c_cfg2(qspi_hw_t *hw, uint32_t config_value)
{
	hw->cmd_c_cfg2.v = config_value;
}

static inline void qspi_ll_cmd_c_start(qspi_hw_t *hw)
{
	hw->cmd_c_cfg2.cmd_start = 1;
}

static inline void qspi_ll_set_cmd_c_data_line(qspi_hw_t *hw, qspi_wire_mode_t data_line)
{
	hw->cmd_c_cfg2.data_line = data_line;
}

static inline void qspi_ll_set_cmd_c_dummy_clock(qspi_hw_t *hw, uint32_t dummy_clock)
{
	hw->cmd_c_cfg2.dummy_clock = dummy_clock & QSPI_F_DUMMY_CLK_M;
}

static inline void qspi_ll_set_cmd_d_l(qspi_hw_t *hw, uint32_t cmd_value)
{
	hw->cmd_d_l.v = cmd_value;
}

static inline uint32_t qspi_ll_get_cmd_d_l(qspi_hw_t *hw)
{
	return hw->cmd_d_l.v;
}

static inline void qspi_ll_set_cmd_d_h(qspi_hw_t *hw, uint32_t cmd_value)
{
	hw->cmd_d_h.v = cmd_value;
}

static inline uint32_t qspi_ll_get_cmd_d_h(qspi_hw_t *hw)
{
	return hw->cmd_d_h.v;
}

static inline void qspi_ll_set_cmd_d_cfg1(qspi_hw_t *hw, uint32_t cmd_line)
{
	hw->cmd_d_cfg1.v = cmd_line;
}

static inline void qspi_ll_set_cmd_d_cfg2(qspi_hw_t *hw, uint32_t config_value)
{
	hw->cmd_d_cfg2.v = config_value;
}

static inline void qspi_ll_cmd_d_start(qspi_hw_t *hw)
{
	hw->cmd_d_cfg2.cmd_start = 1;
}

static inline void qspi_ll_set_cmd_d_data_line(qspi_hw_t *hw, qspi_wire_mode_t data_line)
{
	hw->cmd_d_cfg2.data_line = data_line;
}

static inline void qspi_ll_set_cmd_d_data_length(qspi_hw_t *hw, qspi_wire_mode_t data_len)
{
	hw->cmd_d_cfg2.data_len = data_len;
}

static inline void qspi_ll_set_cmd_d_dummy_clock(qspi_hw_t *hw, uint32_t dummy_clock)
{
	hw->cmd_d_cfg2.dummy_clock = dummy_clock & QSPI_F_DUMMY_CLK_M;
}

static inline void qspi_ll_set_cmd_d_dummy_mode(qspi_hw_t *hw, uint32_t dummy_mode)
{
	hw->cmd_d_cfg2.dummy_mode = dummy_mode & 0x7;
}

static inline void qspi_ll_enable(qspi_hw_t *hw)
{
	hw->config.qspi_en = 1;
}

static inline void qspi_ll_disable(qspi_hw_t *hw)
{
	hw->config.qspi_en = 0;
}

static inline void qspi_ll_enable_io2_io3_mode(qspi_hw_t *hw)
{
	hw->config.io2_io3_mode = 1;
}

static inline void qspi_ll_disable_io2_io3_mode(qspi_hw_t *hw)
{
	hw->config.io2_io3_mode = 0;
}

static inline void qspi_ll_force_spi_cs_low_enable(qspi_hw_t *hw)
{
	hw->config.force_spi_cs_low = 1;
}

static inline void qspi_ll_force_spi_cs_low_disable(qspi_hw_t *hw)
{
	hw->config.force_spi_cs_low = 0;
}

static inline void qspi_ll_disable_cmd_sck_enable(qspi_hw_t *hw)
{
	hw->config.disable_cmd_sck = 1;
}

static inline void qspi_ll_disable_cmd_sck_disable(qspi_hw_t *hw)
{
	hw->config.disable_cmd_sck = 0;
}

static inline void qspi_ll_set_clk_div(qspi_hw_t *hw, uint32_t clk_div)
{
	hw->config.v &= (~(QSPI_F_CLK_DIV_M << QSPI_F_CLK_DIV_S));
	hw->config.v |= ((clk_div & QSPI_F_CLK_DIV_M) << QSPI_F_CLK_DIV_S);
}

static inline void qspi_ll_select_io_cpu_mem(qspi_hw_t *hw, uint32_t mem_sel)
{
	hw->config.io_cpu_mem_sel = mem_sel;
}

static inline bool qspi_ll_is_cmd_start_done(qspi_hw_t *hw)
{
	return !!(hw->status.cmd_start_done);
}

static inline void qspi_ll_wait_cmd_done(qspi_hw_t *hw)
{
	for(int i = 0; i <= 10000; i++) {
		if(0 != qspi_ll_is_cmd_start_done(hw)) {
			break;
		}
		if(i == 10000) {
			os_printf("ERROR: qspi_ll_wait_cmd_done timeout \n");
		}
		extern void delay_us(UINT32 us);
		delay_us(1);
	}
	hw->status_clr.clr_cmd_start_done = 1;
	hw->status_clr.clr_cmd_start_done = 0;
}

/* Start for qspi lcd with no ram and refresh by line */
static inline void qspi_ll_enable_soft_reset(qspi_hw_t *hw)
{
    hw->glb_ctrl.soft_reset = 1;
}

static inline void qspi_ll_disable_soft_reset(qspi_hw_t *hw)
{
    hw->glb_ctrl.soft_reset = 0;
}

static inline void qspi_ll_set_cmd_last_addr(qspi_hw_t *hw, uint32_t addr_value)
{
    hw->reg30.cmd_last_addr = addr_value;
}

static inline void qspi_ll_set_lcd_head_cmd0(qspi_hw_t *hw, uint32_t cmd_value)
{
    hw->lcd_head_cmd0 = cmd_value;
}

static inline void qspi_ll_set_lcd_head_cmd1(qspi_hw_t *hw, uint32_t cmd_value)
{
    hw->lcd_head_cmd1 = cmd_value;
}

static inline void qspi_ll_set_lcd_head_cmd2(qspi_hw_t *hw, uint32_t cmd_value)
{
    hw->lcd_head_cmd2 = cmd_value;
}

static inline void qspi_ll_set_lcd_head_cmd3(qspi_hw_t *hw, uint32_t cmd_value)
{
    hw->lcd_head_cmd3 = cmd_value;
}

static inline void qspi_ll_set_lcd_head_hcnt(qspi_hw_t *hw, uint32_t hcnt_value)
{
    hw->reg35.lcd_head_hcnt = hcnt_value;
}

static inline void qspi_ll_set_lcd_head_vcnt(qspi_hw_t *hw, uint32_t vcnt_value)
{
    hw->reg35.lcd_head_vcnt = vcnt_value;
}

static inline void qspi_ll_clear_lcd_head(qspi_hw_t *hw, uint32_t value)
{
    hw->reg36.lcd_head_clr = value;
}

static inline void qspi_ll_lcd_head_enable(qspi_hw_t *hw)
{
    hw->reg36.lcd_head_sel = 1;
}

static inline void qspi_ll_lcd_head_disable(qspi_hw_t *hw)
{
    hw->reg36.lcd_head_sel = 0;
}

static inline void qspi_ll_set_lcd_head_length(qspi_hw_t *hw, uint32_t length)
{
    hw->reg36.lcd_head_len = length;
}

static inline void qspi_ll_set_lcd_head_interval(qspi_hw_t *hw, uint32_t intv_cnt)
{
    hw->reg36.lcd_head_dly = intv_cnt;
}

/* End for qspi lcd with no ram and refresh by line */


static inline void qspi_ll_direct_write(qspi_id_t id, uint32_t base_addr, const void *data, uint32_t size)
{
	for (int i = 0; i < size; i++) {
		*((uint8_t *)(QSPI_DATA_LL_REG_BASE(id) + base_addr + i)) = *((uint8_t *)data + i);
	}
}

static inline void qspi_ll_direct_read(qspi_id_t id, uint32_t base_addr, void *data, uint32_t size)
{
	for (int i = 0; i < size; i++) {
		*((uint8_t *)data + i) = *((uint8_t *)(QSPI_DATA_LL_REG_BASE(id) + base_addr + i));
	}
}

static inline void qspi_ll_io_write(qspi_hw_t *hw, const void *data, uint32_t data_len)
{
	uint32_t fifo_num = 0;

	if(0 == (data_len % 4)) {
		fifo_num = data_len / 4;
	} else 	{
		fifo_num = (data_len / 4) + 1;
	}

	hw->rst_cfg.fifo_io_wr = 1;
	hw->rst_cfg.clk_man_en = 1;
	hw->rst_cfg.clk_man_sel = 0;
	extern void delay_ms(UINT32 ms);
	delay_ms(1);

	for(uint32_t i = 0; i < fifo_num; i++) {
		*((uint32_t *)(hw->fifo_data) + i) = *((uint32_t *)data + i);
	}

	hw->rst_cfg.fifo_io_wr = 0;
	hw->rst_cfg.clk_man_en = 1;
	hw->rst_cfg.clk_man_sel = 1;
	extern void delay_ms(UINT32 ms);
	delay_ms(1);
}

static inline void qspi_ll_io_read(qspi_hw_t *hw, void *data, uint32_t data_len)
{
	uint32_t size  = 0;

	if(0 == (data_len % 4))	{
		size = data_len / 4;
	} else {
		size = (data_len / 4) + 1;
	}

	hw->rst_cfg.fifo_io_wr = 0;
	hw->rst_cfg.clk_man_en = 1;
	hw->rst_cfg.clk_man_sel = 0;
	extern void delay_ms(UINT32 ms);
	delay_ms(1);

	for(uint32_t i = 0; i < size; i++) {
		*((uint32_t *)data + i) = *((uint32_t *)(hw->fifo_data) + i);
	}

	hw->rst_cfg.clk_man_en = 0;
	extern void delay_ms(UINT32 ms);
	delay_ms(1);
}

static inline void qspi_ll_init_flash_command(qspi_hw_t *hw, const qspi_cmd_t *cmd)
{
	if (INDIRECT_MODE != cmd->work_mode) {
		return;
	}

	uint8_t addr_h;
	uint8_t addr_m;
	uint8_t addr_l;

	if (QSPI_WRITE == cmd->op) {
		hw->cmd_c_l.v = 0;
		hw->cmd_c_h.v &= (~(QSPI_F_CMD1_M << QSPI_F_CMD1_S));
		hw->cmd_c_h.v |= (cmd->cmd & (QSPI_F_CMD1_M << QSPI_F_CMD1_S));

		if (FLASH_WR_S0_S7_CMD == ((cmd->cmd) & QSPI_F_CMD1_M) || (FLASH_WR_S8_S15_CMD == ((cmd->cmd) & QSPI_F_CMD1_M))) {
			hw->cmd_c_h.v &= (~(QSPI_F_CMD2_M << QSPI_F_CMD2_S));
			hw->cmd_c_h.v |= (cmd->cmd & (QSPI_F_CMD2_M << QSPI_F_CMD2_S));
			if (cmd->cmd & (QSPI_F_CMD3_M << QSPI_F_CMD3_S)) {
				hw->cmd_c_h.v &= (~(QSPI_F_CMD3_M << QSPI_F_CMD3_S));
				hw->cmd_c_h.v |= (cmd->cmd & (QSPI_F_CMD3_M << QSPI_F_CMD3_S));
				hw->cmd_c_cfg1.v = 0xc0;
			} else {
				hw->cmd_c_cfg1.v = 0x30;
			}
		} else if (FLASH_WR_EN_CMD == ((cmd->cmd) & QSPI_F_CMD1_M)) {
			hw->cmd_c_cfg1.v = 0xc;
		} else {
			addr_h = (cmd->addr & 0xff0000) >> 16;
			addr_m = (cmd->addr & 0x00ff00) >> 8;
			addr_l = cmd->addr & 0x0000ff;
			hw->cmd_c_h.v &= (~(QSPI_F_CMD2_M << QSPI_F_CMD2_S));
			hw->cmd_c_h.v |= ((addr_h & QSPI_F_CMD2_M) << QSPI_F_CMD2_S);
			hw->cmd_c_h.v &= (~(QSPI_F_CMD3_M << QSPI_F_CMD3_S));
			hw->cmd_c_h.v |= ((addr_m & QSPI_F_CMD3_M) << QSPI_F_CMD3_S);
			hw->cmd_c_h.v &= (~(QSPI_F_CMD4_M << QSPI_F_CMD4_S));
			hw->cmd_c_h.v |= ((addr_l & QSPI_F_CMD4_M) << QSPI_F_CMD4_S);

			hw->cmd_c_cfg1.v = 0x300;
		}
		hw->cmd_c_cfg2.data_len = cmd->data_len;
		hw->cmd_c_cfg2.data_line = cmd->wire_mode;
		hw->cmd_c_cfg2.dummy_clock = cmd->dummy_cycle;
		if (cmd->dummy_cycle) {
			hw->cmd_c_cfg2.dummy_mode = 4;
		} else {
			hw->cmd_c_cfg2.dummy_mode = 0;
		}

		hw->cmd_c_cfg2.cmd_start = 1;
		qspi_ll_wait_cmd_done(hw);
	} else {
		hw->rst_cfg.fifo_io_wr = 0;
		hw->rst_cfg.clk_man_en = 1;
		hw->rst_cfg.clk_man_sel = 1;

		hw->cmd_d_l.v = 0;
		hw->cmd_d_h.v &= (~(QSPI_F_CMD1_M << QSPI_F_CMD1_S));
		hw->cmd_d_h.v |= (cmd->cmd & (QSPI_F_CMD1_M << QSPI_F_CMD1_S));
		if ((FLASH_RD_S0_S7_CMD == cmd->cmd) || (FLASH_RD_S8_S15_CMD == cmd->cmd) || (FLASH_READ_ID_CMD == cmd->cmd)) {
			hw->cmd_d_cfg1.v = 0xc;
		} else {
			addr_h = (cmd->addr & 0xff0000) >> 16;
			addr_m = (cmd->addr & 0x00ff00) >> 8;
			addr_l = cmd->addr & 0x0000ff;
			hw->cmd_d_h.v &= (~(QSPI_F_CMD2_M << QSPI_F_CMD2_S));
			hw->cmd_d_h.v |= ((addr_h & QSPI_F_CMD2_M) << QSPI_F_CMD2_S);
			hw->cmd_d_h.v &= (~(QSPI_F_CMD3_M << QSPI_F_CMD3_S));
			hw->cmd_d_h.v |= ((addr_m & QSPI_F_CMD3_M) << QSPI_F_CMD3_S);
			hw->cmd_d_h.v &= (~(QSPI_F_CMD4_M << QSPI_F_CMD4_S));
			hw->cmd_d_h.v |= ((addr_l & QSPI_F_CMD4_M) << QSPI_F_CMD4_S);
			if (QSPI_4WIRE == cmd->wire_mode) {
				hw->cmd_d_cfg1.v = 0xea8;
			} else {
				hw->cmd_d_cfg1.v = 0x300;
			}
		}
		hw->cmd_d_cfg2.data_len = cmd->data_len;
		hw->cmd_d_cfg2.data_line = cmd->wire_mode;
		hw->cmd_d_cfg2.dummy_clock = cmd->dummy_cycle;
		if (cmd->dummy_cycle) {
			hw->cmd_d_cfg2.dummy_mode = 5;
		} else {
			hw->cmd_d_cfg2.dummy_mode = 0;
		}

		hw->cmd_d_cfg2.cmd_start = 1;
		qspi_ll_wait_cmd_done(hw);
	}
}

static inline void qspi_ll_init_psram_command(qspi_hw_t *hw, const qspi_cmd_t *cmd)
{
	if (MEMORY_MAPPED_MODE == cmd->work_mode) {
		if (QSPI_WRITE == cmd->op) {
			hw->cmd_a_l.v = 0;
			hw->cmd_a_h.cmd1 = cmd->cmd;

			if (QUAD_WRITE_CMD == cmd->cmd) {
				hw->cmd_a_cfg1.v = 0x3aa;
			} else if (WRITE_CMD == cmd->cmd) {
				hw->cmd_a_cfg1.v = 0x300;
			}
			hw->cmd_a_cfg2.data_line = cmd->wire_mode;
			hw->cmd_a_cfg2.dummy_clock = cmd->dummy_cycle;
			if (cmd->dummy_cycle) {
				hw->cmd_a_cfg2.dummy_mode = 4;
			} else {
				hw->cmd_a_cfg2.dummy_mode = 0;
			}
			//hw->cmd_a_cfg2.cmd_mode = cmd->cmd_mode;
		} else {
			hw->cmd_b_l.v = 0;
			hw->cmd_b_h.cmd1 = cmd->cmd;
			if (FAST_READ_QUAD == cmd->cmd) {
				if (QSPI_1WIRE == cmd->wire_mode) {
					hw->cmd_b_cfg1.v = 0x3a8;
				} else if (QSPI_4WIRE == cmd->wire_mode) {
					hw->cmd_b_cfg1.v = 0x3aa;
				}
			} else if (READ_CMD == cmd->cmd) {
				hw->cmd_b_cfg1.v = 0x300;
			}

			hw->cmd_b_cfg2.data_line = cmd->wire_mode;
			hw->cmd_b_cfg2.dummy_clock = cmd->dummy_cycle;
			if (cmd->dummy_cycle) {
				hw->cmd_b_cfg2.dummy_mode = 4;
			} else {
				hw->cmd_b_cfg2.dummy_mode = 0;
			}
			//hw->cmd_b_cfg2.cmd_mode = cmd->cmd_mode;
		}
	} else {
		if (QSPI_WRITE == cmd->op) {
			hw->cmd_c_l.v = 0;
			hw->cmd_c_h.v &= (~(QSPI_F_CMD1_M << QSPI_F_CMD1_S));
			hw->cmd_c_h.v |= (cmd->cmd & (QSPI_F_CMD1_M << QSPI_F_CMD1_S));

			if (ENTER_QUAD_MODE_CMD == cmd->cmd) {
				hw->cmd_c_cfg1.v = 0xc;
			} else if (EXIT_QUAD_MODE_CMD == cmd->cmd) {
				hw->cmd_c_cfg1.v = 0xe;
			}

			hw->cmd_c_cfg2.data_line = cmd->wire_mode;
			hw->cmd_c_cfg2.dummy_clock = cmd->dummy_cycle;
			if (cmd->dummy_cycle) {
				hw->cmd_c_cfg2.dummy_mode = 4;
			} else {
				hw->cmd_c_cfg2.dummy_mode = 0;
			}

			hw->cmd_c_cfg2.cmd_start = 1;
			qspi_ll_wait_cmd_done(hw);
		} else {
			hw->cmd_d_l.v = 0;
			hw->cmd_d_h.cmd1 = cmd->cmd;
			hw->cmd_d_cfg1.v = 0xc;
			hw->cmd_d_cfg2.data_line = cmd->wire_mode;
			hw->cmd_d_cfg2.dummy_clock = cmd->dummy_cycle;
			if (cmd->dummy_cycle) {
				hw->cmd_d_cfg2.dummy_mode = 4;
			} else {
				hw->cmd_d_cfg2.dummy_mode = 0;
			}

			hw->cmd_d_cfg2.cmd_start = 1;
			qspi_ll_wait_cmd_done(hw);
		}
	}
}

static inline void qspi_ll_init_command(qspi_hw_t *hw, const qspi_cmd_t *cmd)
{
	hw->rst_cfg.fifo_io_wr = 0;
	hw->rst_cfg.clk_man_en = 1;
	hw->rst_cfg.clk_man_sel = 1;

	if (QSPI_FLASH == cmd->device) {
		qspi_ll_init_flash_command(hw, cmd);
	} else {
		qspi_ll_init_psram_command(hw, cmd);
	}
}

static inline void qspi_ll_init_common(qspi_hw_t *hw)
{
	qspi_ll_enable(hw);
	qspi_ll_enable_io2_io3_mode(hw);
}

static inline void qspi_ll_deinit_common(qspi_hw_t *hw)
{
	qspi_ll_disable(hw);
	qspi_ll_disable_io2_io3_mode(hw);
}

static inline uint32_t qspi_ll_get_interrupt_status_before_mask(qspi_hw_t *hw)
{
	return hw->status.v;
}

static inline void qspi_ll_clear_interrupt_status(qspi_hw_t *hw, uint32_t status)
{
	hw->status_clr.v = status;
	hw->status_clr.v = 0;
}

static inline uint32_t qspi_ll_get_interrupt_status_after_mask(qspi_hw_t *hw)
{
	return 0;
}

static inline bool qspi_ll_is_sw_op_int_triggered(qspi_hw_t *hw, uint32_t status)
{
	return 0;
}

static inline void qspi_ll_clear_sw_op_int(qspi_hw_t *hw)
{
}

static inline void qspi_ll_enable_sw_op_int_mask(qspi_hw_t *hw)
{
}

static inline void qspi_ll_disable_sw_op_int_mask(qspi_hw_t *hw)
{
}

static inline void qspi_ll_stop_sw_op(qspi_hw_t *hw)
{
}

static inline void qspi_ll_disable_ge0_tx(qspi_hw_t *hw)
{
}

static inline void qspi_ll_disable_ge1_rx(qspi_hw_t *hw)
{
}

#ifdef __cplusplus
}
#endif

