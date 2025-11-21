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
#include "spi_config.h"
#include "spi_hw.h"
#include "spi_ll.h"
#include <driver/hal/hal_spi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	spi_hw_t *hw;
	spi_unit_t id;
} spi_hal_t;

#define spi_hal_set_role(hal, role) spi_ll_set_role((hal)->hw, role)
#define spi_hal_set_bit_width(hal, bit_width) spi_ll_set_bit_width((hal)->hw, bit_width)
#define spi_hal_set_wire_mode(hal, wire_mode) spi_ll_set_wire_mode((hal)->hw, wire_mode)
#define spi_hal_set_first_bit(hal, first_bit) spi_ll_set_first_bit((hal)->hw, first_bit)
#define spi_hal_set_cpol(hal, cpol) spi_ll_set_cpol((hal)->hw, cpol)
#define spi_hal_set_cpha(hal, cpha) spi_ll_set_cpha((hal)->hw, cpha)
#define spi_hal_set_role_master(hal) spi_ll_set_role_master((hal)->hw)
#define spi_hal_set_role_slave(hal) spi_ll_set_role_slave((hal)->hw)
#define spi_hal_is_master(hal) spi_ll_is_role_master((hal)->hw)
#define spi_hal_is_slave(hal) spi_ll_is_role_slave((hal)->hw)
#define spi_hal_get_tx_fifo_int_level(hal) spi_ll_get_tx_fifo_int_level((hal)->hw)
#define spi_hal_get_rx_fifo_int_level(hal) spi_ll_get_rx_fifo_int_level((hal)->hw)
#define spi_hal_set_byte_interval(hal, val) spi_ll_set_byte_interval((hal)->hw, val)
#define spi_hal_get_byte_interval(hal) spi_ll_get_byte_interval((hal)->hw)
#define spi_hal_enable_tx(hal) spi_ll_enable_tx((hal)->hw)
#define spi_hal_disable_tx(hal) spi_ll_disable_tx((hal)->hw)
#define spi_hal_enable_rx(hal) spi_ll_enable_rx((hal)->hw)
#define spi_hal_disable_rx(hal) spi_ll_disable_rx((hal)->hw)
#define spi_hal_enable_tx_finish_int(hal) spi_ll_enable_tx_finish_int((hal)->hw)
#define spi_hal_disable_tx_finish_int(hal) spi_ll_disable_tx_finish_int((hal)->hw)
#define spi_hal_enable_rx_finish_int(hal) spi_ll_enable_rx_finish_int((hal)->hw)
#define spi_hal_disable_rx_finish_int(hal) spi_ll_disable_rx_finish_int((hal)->hw)
#define spi_hal_set_tx_trans_len(hal, len) spi_ll_set_tx_trans_len((hal)->hw, len)
#define spi_hal_set_rx_trans_len(hal, len) spi_ll_set_rx_trans_len((hal)->hw, len)

#define spi_hal_enable_tx_fifo_int(hal) spi_ll_enable_tx_fifo_int((hal)->hw)
#define spi_hal_disable_tx_fifo_int(hal) spi_ll_disable_tx_fifo_int((hal)->hw)
#define spi_hal_enable_rx_fifo_int(hal) spi_ll_enable_rx_fifo_int((hal)->hw)
#define spi_hal_disable_rx_fifo_int(hal) spi_ll_disable_rx_fifo_int((hal)->hw)
#define spi_hal_enable_tx_underflow_int(hal) spi_ll_enable_tx_underflow_int((hal)->hw)
#define spi_hal_disable_tx_underflow_int(hal) spi_ll_disable_tx_underflow_int((hal)->hw)
#define spi_hal_enable_rx_overflow_int(hal) spi_ll_enable_rx_overflow_int((hal)->hw)
#define spi_hal_disable_rx_overflow_int(hal) spi_ll_disable_rx_overflow_int((hal)->hw)
#define spi_hal_enable_slave_release_int(hal) spi_ll_enable_slave_release_int((hal)->hw)
#define spi_hal_disable_slave_release_int(hal) spi_ll_disable_slave_release_int((hal)->hw)

#define spi_hal_get_interrupt_status(hal) spi_ll_get_interrupt_status((hal)->hw)
#define spi_hal_clear_interrupt_status(hal, status) spi_ll_clear_interrupt_status((hal)->hw, status)
#define spi_hal_clear_tx_fifo_int_status(hal) spi_ll_clear_tx_fifo_int_status((hal)->hw)
#define spi_hal_clear_rx_fifo_int_status(hal) spi_ll_clear_rx_fifo_int_status((hal)->hw)
#define spi_hal_clear_tx_finish_int_status(hal) spi_ll_clear_tx_finish_int_status((hal)->hw)
#define spi_hal_clear_rx_finish_int_status(hal) spi_ll_clear_rx_finish_int_status((hal)->hw)
#define spi_hal_is_tx_fifo_wr_ready(hal) spi_ll_is_tx_fifo_wr_ready((hal)->hw)
#define spi_hal_is_rx_fifo_rd_ready(hal) spi_ll_is_rx_fifo_rd_ready((hal)->hw)
#define spi_hal_is_tx_fifo_int_triggered(hal) spi_ll_is_tx_fifo_int_triggered((hal)->hw)
#define spi_hal_is_rx_fifo_int_triggered(hal) spi_ll_is_rx_fifo_int_triggered((hal)->hw)
#define spi_hal_is_tx_fifo_int_triggered_with_status(hal, status) spi_ll_is_tx_fifo_int_triggered_with_status((hal)->hw, status)
#define spi_hal_is_rx_fifo_int_triggered_with_status(hal, status) spi_ll_is_rx_fifo_int_triggered_with_status((hal)->hw, status)
#define spi_hal_is_tx_finish_int_triggered(hal, status) spi_ll_is_tx_finish_int_triggered((hal)->hw, status)
#define spi_hal_is_rx_finish_int_triggered(hal, status) spi_ll_is_rx_finish_int_triggered((hal)->hw, status)
#define spi_hal_is_slave_release_int_triggered(hal, status) spi_ll_is_slave_release_int_triggered((hal)->hw, status)
#define spi_hal_is_rx_overflow_int_triggered(hal, status) spi_ll_is_rx_overflow_int_triggered((hal)->hw, status)
#define spi_hal_is_tx_underflow_int_triggered(hal, status) spi_ll_is_tx_underflow_int_triggered((hal)->hw, status)
#define spi_hal_clear_tx_fifo(hal) spi_ll_clear_tx_fifo((hal)->hw)
#define spi_hal_clear_rx_fifo(hal) spi_ll_clear_rx_fifo((hal)->hw)
#define spi_hal_is_rx_fifo_empty(hal) spi_ll_is_rx_fifo_empty((hal)->hw)
#define spi_hal_write_byte(hal, data) spi_ll_write_byte((hal)->hw, data)

#define SPI_EN_APLL_MANU_VAL   0x14
#define SPI_EN_APLL_VSEL_VAL   0x7
#define SPI_DIS_APLL_MANU_VAL  0x10
#define SPI_DIS_APLL_VSEL_VAL  0x5
#define SPI_APLL_98M           98000000
#define SPI_APLL_120M          120000000
#define SPI_MAX_LENGTH         0x10000

bk_err_t spi_hal_init(spi_hal_t *hal);
bk_err_t spi_hal_reset_config_to_default(spi_hal_t *hal);
bk_err_t spi_hal_configure(spi_hal_t *hal, const spi_config_t *config);
bk_err_t spi_hal_start_common(spi_hal_t *hal);
bk_err_t spi_hal_stop_common(spi_hal_t *hal);
bk_err_t spi_hal_set_baud_rate(spi_hal_t *hal, uint32_t baud_rate);
bk_err_t spi_hal_read_byte(spi_hal_t *hal, uint16_t *data);
bk_err_t spi_hal_duplex_config(spi_hal_t *hal);
bk_err_t spi_hal_duplex_release(spi_hal_t *hal);
bk_err_t spi_hal_enable_tx_rx(spi_hal_t *hal);
#if (defined(CONFIG_SPI_PM_CB_SUPPORT))
void spi_hal_backup(spi_hal_t *hal, uint32_t *pm_backup);
void spi_hal_restore(spi_hal_t *hal, uint32_t *pm_backup);
#endif

#if CFG_HAL_DEBUG_SPI
void spi_struct_dump(spi_id_t id);
#else
#define spi_struct_dump(id)
#endif

#ifdef __cplusplus
}
#endif

