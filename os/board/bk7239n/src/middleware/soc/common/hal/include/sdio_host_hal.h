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
#include "sdio_hw.h"
#include "sdio_ll.h"
#include <driver/hal/hal_sdio_host_types.h>
#include <driver/hal/hal_gpio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	sdio_hw_t *hw;
	sdio_host_unit_t id;
} sdio_host_hal_t;

typedef struct {
	gpio_id_t gpio_id;
	gpio_dev_t dev;

#if CONFIG_IO_MATRIX_VER2_0
	uint32_t func_code;
#endif
} sdio_host_gpio_map_t;

#define sdio_host_hal_reset_config_to_default(hal) sdio_host_ll_reset_config_to_default((hal)->hw)
#define sdio_host_hal_set_clk_freq(hal, clk_freq) sdio_host_ll_set_clk_freq((hal)->hw, clk_freq)
#define sdio_host_hal_set_bus_width(hal, bus_width) sdio_host_ll_set_bus_width((hal)->hw, bus_width)
#define sdio_host_hal_set_cmd_index(hal, cmd_index) sdio_host_ll_set_cmd_index((hal)->hw, cmd_index)
#define sdio_host_hal_set_send_cmd_argument(hal, cmd_argument) sdio_host_ll_set_send_cmd_argument((hal)->hw, cmd_argument)
#define sdio_host_hal_set_cmd_rsp_timeout(hal, rsp_timeout) sdio_host_ll_set_cmd_rsp_timeout((hal)->hw, rsp_timeout)
#define sdio_host_hal_set_cmd_rsp(hal, is_need_rsp) sdio_host_ll_set_cmd_rsp((hal)->hw, is_need_rsp)
#define sdio_host_hal_set_cmd_long_rsp(hal, is_long_rsp) sdio_host_ll_set_cmd_long_rsp((hal)->hw, is_long_rsp)
#define sdio_host_hal_set_cmd_crc_check(hal, is_need_crc_check) sdio_host_ll_set_cmd_crc_check((hal)->hw, is_need_crc_check)
#define sdio_host_hal_start_send_command(hal) sdio_host_ll_start_send_cmd((hal)->hw)
#define sdio_host_hal_get_cmd_rsp_argument(hal, argument_index) sdio_host_ll_get_cmd_rsp_argument((hal)->hw, argument_index)

#define sdio_host_hal_set_tx_fifo_threshold(hal, threshold) sdio_host_ll_set_tx_fifo_threshold((hal)->hw, threshold)
#define sdio_host_hal_set_rx_fifo_threshold(hal, threshold) sdio_host_ll_set_rx_fifo_threshold((hal)->hw, threshold)
#define sdio_host_hal_start_receive_data(hal) sdio_host_ll_start_receive_data((hal)->hw)
#define sdio_host_hal_set_write_data(hal, size) sdio_host_ll_set_write_data((hal)->hw, size)
#define sdio_host_hal_set_read_data(hal, block_size) sdio_host_ll_set_read_data((hal)->hw, block_size)
#define sdio_host_hal_start_send_data(hal) sdio_host_ll_start_send_data((hal)->hw)
#define sdio_host_hal_stop_send_data(hal) sdio_host_ll_stop_send_data((hal)->hw)
#define sdio_host_hal_set_data_timeout(hal, data_timeout) sdio_host_ll_set_data_timeout((hal)->hw, data_timeout)

#define sdio_host_hal_get_interrupt_status(hal) sdio_host_ll_get_interrupt_status((hal)->hw)
#define sdio_host_hal_clear_interrupt_status(hal) sdio_host_ll_clear_interrupt_status((hal)->hw)
#define sdio_host_hal_is_cmd_rsp_interrupt_triggered(hal, int_status) sdio_host_ll_is_cmd_rsp_interrupt_triggered((hal)->hw, int_status)
#define sdio_host_hal_is_cmd_rsp_timeout_interrupt_triggered(hal, int_status) sdio_host_ll_is_cmd_rsp_timeout_interrupt_triggered((hal)->hw, int_status)
#define sdio_host_hal_clear_cmd_rsp_interrupt_status(hal, int_status) sdio_host_ll_clear_cmd_rsp_interrupt_status((hal)->hw, int_status)
#define sdio_host_hal_is_cmd_rsp_crc_fail_interrupt_triggered(hal, int_status) sdio_host_ll_is_cmd_rsp_crc_fail_interrupt_triggered((hal)->hw, int_status)
#define sdio_host_hal_is_recv_data_interrupt_triggered(hal, int_status) sdio_host_ll_is_recv_data_interrupt_triggered((hal)->hw, int_status)
#define sdio_host_hal_clear_data_interrupt_status(hal, int_status) sdio_host_ll_clear_data_interrupt_status((hal)->hw, int_status)
#define sdio_host_hal_is_data_timeout_int_triggered(hal, int_status) sdio_host_ll_is_data_timeout_int_triggered((hal)->hw, int_status)
#define sdio_host_hal_is_data_crc_fail_int_triggered(hal, int_status) sdio_host_ll_is_data_crc_fail_int_triggered((hal)->hw, int_status)
#define sdio_host_hal_is_data_recv_end_int_triggered(hal, int_status) sdio_host_ll_is_data_recv_end_int_triggered((hal)->hw, int_status)
#define sdio_host_hal_is_data_write_end_int_triggered(hal, int_status) sdio_host_ll_is_data_write_end_int_triggered((hal)->hw, int_status)
#define sdio_host_hal_is_fifo_empty_int_triggered(hal, int_status) sdio_host_ll_is_fifo_empty_int_triggered((hal)->hw, int_status)
#define sdio_host_hal_is_tx_fifo_need_write(hal, int_status) sdio_host_ll_is_tx_fifo_need_write((hal)->hw, int_status)

#define sdio_host_hal_enable_tx_fifo_empty_mask(hal) sdio_host_ll_enable_tx_fifo_empty_mask((hal)->hw)
#define sdio_host_hal_disable_tx_fifo_empty_mask(hal) sdio_host_ll_disable_tx_fifo_empty_mask((hal)->hw)

#define sdio_host_hal_is_tx_fifo_write_ready(hal) sdio_host_ll_is_tx_fifo_write_ready((hal)->hw)
#define sdio_host_hal_is_rx_fifo_read_ready(hal) sdio_host_ll_is_rx_fifo_read_ready((hal)->hw)
#define sdio_host_hal_write_fifo(hal, data) sdio_host_ll_write_fifo((hal)->hw, data)
#define sdio_host_hal_read_fifo(hal) sdio_host_ll_read_fifo((hal)->hw)

/*
 * BK7256 add some function for the SDCARD
 * 1.clock gate: SDIO CHIP uses clock gate to control whether support clock for SDCARD when write/read data
 * 2.write status check: BK7256 write data finish should check the status of SDCARD responsed is whether right
 * 3.ISR: Use ISR to avoid sync wait SDCARD operate finish, as some special SDCARD busy time is too long(> 5ms)
 *   F.E:CMD response, programming data to memory...
 */
#if defined(CONFIG_SDIO_V2P0)
#define sdio_host_hal_is_tx_fifo_need_write_int_triggered(hal, int_status) sdio_host_ll_is_tx_fifo_need_write((hal)->hw, int_status)
#define sdio_host_hal_clear_read_data_timeout_interrupt_status(hal, int_status) sdio_host_ll_clear_read_data_timeout_interrupt_status((hal)->hw, int_status)
#define sdio_host_hal_is_data_recv_need_read_int_triggered(hal, int_status) sdio_host_ll_is_data_recv_need_read_int_triggered((hal)->hw, int_status)
#define sdio_host_hal_is_data_recv_overflow_int_triggered(hal, int_status) sdio_host_ll_is_data_recv_overflow_int_triggered((hal)->hw, int_status)
#define sdio_host_hal_enable_tx_fifo_need_write_mask(hal) sdio_host_ll_enable_tx_fifo_need_write_mask((hal)->hw)
#define sdio_host_hal_disable_tx_fifo_need_write_mask(hal) sdio_host_ll_disable_tx_fifo_need_write_mask((hal)->hw)
#define sdio_host_hal_reset_sd_state(hal) sdio_host_ll_reset_sd_state((hal)->hw)
#define sdio_host_hal_set_sd_data_stop_en(hal, en) sdio_host_ll_set_sd_data_stop_en((hal)->hw, en)
#define sdio_host_hal_set_read_multi_block_data(hal, block_size) sdio_host_ll_set_read_multi_block_data((hal)->hw, block_size)
#define sdio_host_hal_set_write_multi_block_data(hal, size) sdio_host_ll_set_write_multi_block_data((hal)->hw, size)
#define sdio_host_hal_clear_read_data_interrupt_status(hal, int_status) sdio_host_ll_clear_read_data_interrupt_status((hal)->hw, int_status)
#define sdio_host_hal_is_data_crc_ok_int_triggered(hal, int_status) sdio_host_ll_is_data_crc_ok_int_triggered((hal)->hw, int_status)
#define sdio_host_hal_is_cmd_end_interrupt_triggered(hal, int_status) sdio_host_ll_is_cmd_end_interrupt_triggered((hal)->hw, int_status)
#define sdio_host_hal_enable_rx_end_mask(hal) sdio_host_ll_enable_rx_end_mask((hal)->hw)
#define sdio_host_hal_get_cmd_index_interrupt_status(hal, int_status) sdio_host_ll_get_cmd_index_interrupt_status((hal)->hw, int_status)
#define sdio_host_hal_get_wr_status(hal) sdio_host_ll_get_wr_status((hal)->hw)
#define sdio_host_hal_is_cmd_rsp_crc_ok_interrupt_triggered(hal, int_status) sdio_host_ll_is_cmd_rsp_crc_ok_interrupt_triggered((hal)->hw, int_status)
#define sdio_host_hal_clear_write_data_interrupt_status(hal, int_status) sdio_host_ll_clear_write_data_interrupt_status((hal)->hw, int_status)
#define sdio_host_hal_disable_rx_end_empty_mask(hal) sdio_host_ll_disable_rx_end_mask((hal)->hw)
#define sdio_host_hal_enable_rx_need_read_mask(hal) sdio_host_ll_enable_rx_need_read_mask((hal)->hw)
#define sdio_host_hal_disable_rx_need_read_mask(hal) sdio_host_ll_disable_rx_need_read_mask((hal)->hw)
#define sdio_host_hal_enable_all_mask(hal) sdio_host_ll_enable_all_mask((hal)->hw)
#define sdio_host_hal_set_clock_gate(hal, clk_gate) sdio_host_ll_set_clock_gate((hal)->hw, clk_gate)
#define sdio_hal_host_set_tx_fifo_need_write_mask_cg(hal, clk_gate) sdio_host_ll_set_tx_fifo_clock_gate((hal)->hw, clk_gate);

#if (defined(CONFIG_SOC_BK7236XX) || defined(CONFIG_SOC_BK7239XX) || defined(CONFIG_SOC_BK7286XX))
#define sdio_host_hal_write_blk_en(hal, en) sdio_host_ll_write_blk_en((hal)->hw, en)
#define sdio_host_hal_set_fifo_send_cnt(hal, cnt) sdio_host_ll_set_fifo_send_cnt((hal)->hw, cnt)
#endif

#endif

bk_err_t sdio_host_hal_init(sdio_host_hal_t *hal);
bk_err_t sdio_host_hal_init_commad(sdio_host_hal_t *hal, const sdio_host_cmd_cfg_t *command);

#if CFG_HAL_DEBUG_SDIO_HOST
void sdio_host_struct_dump(void);
#else
#define sdio_host_struct_dump()
#endif
#if (defined(CONFIG_SDIO_PM_CB_SUPPORT))
void sdio_hal_backup(sdio_host_hal_t *hal, uint32_t *pm_backup);
void sdio_hal_restore(sdio_host_hal_t *hal, uint32_t *pm_backup);
#endif

#ifdef __cplusplus
}
#endif

