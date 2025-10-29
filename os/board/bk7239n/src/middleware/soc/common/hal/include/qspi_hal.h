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

#include "hal_config.h"
#include "qspi_hw.h"
#include "qspi_ll.h"
#include <driver/hal/hal_qspi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	qspi_hw_t *hw;
	qspi_unit_t id;
} qspi_hal_t;

#define qspi_hal_enable_ge1_rx(hal) qspi_ll_enable_ge1_rx((hal)->hw)
#define qspi_hal_disable_ge1_rx(hal) qspi_ll_disable_ge1_rx((hal)->hw)
#define qspi_hal_enable_ge1_tx(hal) qspi_ll_enable_ge1_tx((hal)->hw)
#define qspi_hal_disable_ge1_tx(hal) qspi_ll_disable_ge1_tx((hal)->hw)
#define qspi_hal_enable_ge0_rx(hal) qspi_ll_enable_ge0_rx((hal)->hw)
#define qspi_hal_disable_ge0_rx(hal) qspi_ll_disable_ge0_rx((hal)->hw)
#define qspi_hal_enable_ge0_tx(hal) qspi_ll_enable_ge0_tx((hal)->hw)
#define qspi_hal_disable_ge0_tx(hal) qspi_ll_disable_ge0_tx((hal)->hw)

#define qspi_hal_start_sw_op(hal) qspi_ll_start_sw_op((hal)->hw)
#define qspi_hal_stop_sw_op(hal) qspi_ll_stop_sw_op((hal)->hw)
#define qspi_hal_clear_sw_op_int(hal) qspi_ll_clear_sw_op_int((hal)->hw)

#define qspi_hal_set_clk_div(hal, clk_div) qspi_ll_set_clk_div((hal)->hw, clk_div)

#define qspi_hal_get_interrupt_status_before_mask(hal) qspi_ll_get_interrupt_status_before_mask((hal)->hw)
#define qspi_hal_get_interrupt_status_after_mask(hal) qspi_ll_get_interrupt_status_after_mask((hal)->hw)
#define qspi_hal_clear_interrupt_status(hal, status) qspi_ll_clear_interrupt_status((hal)->hw, status)
#define qspi_hal_is_sw_op_int_triggered(hal, status) qspi_ll_is_sw_op_int_triggered((hal)->hw, status)

#define qspi_hal_init_common(hal) qspi_ll_init_common((hal)->hw)
#define qspi_hal_deinit_common(hal) qspi_ll_deinit_common((hal)->hw)

bk_err_t qspi_hal_init(qspi_hal_t *hal);
bool qspi_hal_is_cur_sw_op_write_data(void);
bool qspi_hal_is_cur_sw_op_read_data(void);
bk_err_t qspi_hal_command(qspi_hal_t *hal, const qspi_cmd_t *cmd);
bk_err_t qspi_hal_direct_write(qspi_id_t id, uint32_t base_addr, const void *data, uint32_t size);
bk_err_t qspi_hal_direct_read(qspi_id_t id, uint32_t base_addr, void *data, uint32_t size);
bk_err_t qspi_hal_io_write(qspi_hal_t *hal, const void *data, uint32_t size);
bk_err_t qspi_hal_io_read(qspi_hal_t *hal, void *data, uint32_t size);

bk_err_t qspi_hal_set_cmd_a_l(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_cmd_a_h(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_cmd_a_cfg1(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_cmd_a_cfg2(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_cmd_c_l(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_get_cmd_c_l(qspi_hal_t *hal);
bk_err_t qspi_hal_set_cmd_c_h(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_get_cmd_c_h(qspi_hal_t *hal);
bk_err_t qspi_hal_set_cmd_c_cfg1(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_cmd_c_cfg2(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_cmd_c_start(qspi_hal_t *hal);
bk_err_t qspi_hal_set_cmd_d_l(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_get_cmd_d_l(qspi_hal_t *hal);
bk_err_t qspi_hal_set_cmd_d_h(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_get_cmd_d_h(qspi_hal_t *hal);
bk_err_t qspi_hal_set_cmd_d_cfg1(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_cmd_d_cfg2(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_cmd_d_start(qspi_hal_t *hal);
bk_err_t qspi_hal_set_cmd_d_data_line(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_cmd_d_data_length(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_cmd_d_dummy_clock(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_cmd_d_dummy_mode(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_wait_cmd_done(qspi_hal_t *hal);
bk_err_t qspi_hal_force_spi_cs_low_enable(qspi_hal_t *hal);
bk_err_t qspi_hal_force_spi_cs_low_disable(qspi_hal_t *hal);
bk_err_t qspi_hal_disable_cmd_sck_enable(qspi_hal_t *hal);
bk_err_t qspi_hal_disable_cmd_sck_disable(qspi_hal_t *hal);

#if (defined(CONFIG_SOC_BK7236XX))
bk_err_t qspi_hal_enable_soft_reset(qspi_hal_t *hal);
bk_err_t qspi_hal_disable_soft_reset(qspi_hal_t *hal);
bk_err_t qspi_hal_io_cpu_mem_select(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_cmd_last_addr(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_lcd_head_cmd0(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_lcd_head_cmd1(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_lcd_head_cmd2(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_lcd_head_cmd3(qspi_hal_t *hal, uint32_t cmd_value);
bk_err_t qspi_hal_set_lcd_head_resolution(qspi_hal_t *hal, uint32_t hcnt_value, uint32_t vcnt_value);
bk_err_t qspi_hal_clear_lcd_head(qspi_hal_t *hal, uint32_t enable);
bk_err_t qspi_hal_enable_lcd_head_selection_without_ram(qspi_hal_t *hal);
bk_err_t qspi_hal_disable_lcd_head_selection_without_ram(qspi_hal_t *hal);
bk_err_t qspi_hal_set_lcd_head_len(qspi_hal_t *hal, uint32_t length);
bk_err_t qspi_hal_set_lcd_head_dly(qspi_hal_t *hal, uint32_t delay_cnt);
#endif

#if CFG_HAL_DEBUG_QSPI
void qspi_struct_dump(uint32_t id);
#else
#define qspi_struct_dump()
#endif

#ifdef __cplusplus
}
#endif
