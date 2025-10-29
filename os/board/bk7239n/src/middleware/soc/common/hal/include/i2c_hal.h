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
#include "i2c_hw.h"
#include "i2c_ll.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	i2c_hw_t hw;
	i2c_unit_t id;
} i2c_hal_t;

#define i2c_hal_set_pin(hal) i2c_ll_set_pin(&(hal)->hw, (hal)->id)
#define i2c_hal_enable(hal) i2c_ll_enable(&(hal)->hw, (hal)->id)
#define i2c_hal_disable(hal) i2c_ll_disable(&(hal)->hw, (hal)->id)
#define i2c_hal_enable_start(hal) i2c_ll_enable_start(&(hal)->hw, (hal)->id)
#define i2c_hal_disable_start(hal) i2c_ll_disable_start(&(hal)->hw, (hal)->id)
#define i2c_hal_is_start(hal) i2c_ll_is_start(&(hal)->hw, (hal)->id)
#define i2c_hal_is_start_triggered(hal, int_status) i2c_ll_is_start_triggered(&(hal)->hw, (hal)->id, int_status)
#define i2c_hal_enable_stop(hal) i2c_ll_enable_stop(&(hal)->hw, (hal)->id)
#define i2c_hal_disable_stop(hal) i2c_ll_disable_stop(&(hal)->hw, (hal)->id)
#define i2c_hal_is_stop_triggered(hal, int_status) i2c_ll_is_stop_triggered(&(hal)->hw, (hal)->id, int_status)
#define i2c_hal_reset_config_to_default(hal) i2c_ll_reset_config_to_default(&(hal)->hw, (hal)->id)
#define i2c_hal_set_clk_src(hal, clk_src) i2c_ll_set_clk_src(&(hal)->hw, (hal)->id, clk_src)
#define i2c_hal_set_slave_addr(hal, slave_addr)i2c_ll_set_slave_addr(&(hal)->hw, (hal)->id, slave_addr)
#define i2c_hal_enable_scl_timeout(hal) i2c_ll_enable_scl_timeout(&(hal)->hw, (hal)->id)
#define i2c_hal_disable_scl_timeout(hal) i2c_ll_disable_scl_timeout(&(hal)->hw, (hal)->id)

#define i2c_hal_get_write_empty_fifo_num(hal) i2c_ll_get_write_empty_fifo_num(&(hal)->hw, (hal)->id)
#define i2c_hal_get_read_fifo_num(hal) i2c_ll_get_read_fifo_num(&(hal)->hw, (hal)->id)
#define i2c_hal_is_busy(hal) i2c_ll_is_busy(&(hal)->hw, (hal)->id)

#define i2c_hal_get_interrupt_status(hal) i2c_ll_get_interrupt_status(&(hal)->hw, (hal)->id)
#define i2c_hal_clear_interrupt_status(hal, status) i2c_ll_clear_interrupt_status(&(hal)->hw, (hal)->id, status)
#define i2c_hal_is_sm_int_triggered(hal, int_status) i2c_ll_is_sm_int_triggered(&(hal)->hw, (hal)->id, int_status)
#define i2c_hal_is_scl_timeout_triggered(hal, int_status) i2c_ll_is_scl_timeout_triggered(&(hal)->hw, (hal)->id, int_status)
#define i2c_hal_is_arb_lost_triggered(hal, int_status) i2c_ll_is_arb_lost_triggered(&(hal)->hw, (hal)->id, int_status)
#define i2c_hal_is_tx_ack_triggered(hal, int_status) i2c_ll_is_tx_ack_triggered(&(hal)->hw, (hal)->id, int_status)
#define i2c_hal_is_rx_ack_triggered(hal, int_status) i2c_ll_is_rx_ack_triggered(&(hal)->hw, (hal)->id, int_status)
#define i2c_hal_is_receiver_ack_req(hal, int_status) i2c_ll_is_receiver_ack_req(&(hal)->hw, (hal)->id, int_status)
#define i2c_hal_is_transmitter_mode(hal, int_status) i2c_ll_is_transmitter_mode(&(hal)->hw, (hal)->id, int_status)
#define i2c_hal_is_master_mode(hal, int_status) i2c_ll_is_master_mode(&(hal)->hw, (hal)->id, int_status)

#define i2c_hal_tx_ack(hal) i2c_ll_tx_ack(&(hal)->hw, (hal)->id)
#define i2c_hal_tx_non_ack(hal) i2c_ll_tx_non_ack(&(hal)->hw, (hal)->id)
#define i2c_hal_is_addr_matched(hal) i2c_ll_is_addr_matched(&(hal)->hw, (hal)->id)
#define i2c_hal_is_tx_mode(hal) i2c_ll_is_tx_mode(&(hal)->hw, (hal)->id)
#define i2c_hal_is_rx_mode(hal) i2c_ll_is_rx_mode(&(hal)->hw, (hal)->id)
#define i2c_hal_set_tx_mode(hal) i2c_ll_set_tx_mode(&(hal)->hw, (hal)->id)
#define i2c_hal_set_rx_mode(hal) i2c_ll_set_rx_mode(&(hal)->hw, (hal)->id)

#define i2c_hal_write_byte(hal, data) i2c_ll_write_byte(&(hal)->hw, (hal)->id, data)
#define i2c_hal_read_byte(hal) i2c_ll_read_byte(&(hal)->hw, (hal)->id)
#define i2c_hal_enable_addr_output(hal) i2c_ll_enable_addr_output(&(hal)->hw, (hal)->id)
#define i2c_hal_disable_addr_output(hal) i2c_ll_disable_addr_output(&(hal)->hw, (hal)->id)
#define i2c_hal_enable_data_output(hal) i2c_ll_enable_data_output(&(hal)->hw, (hal)->id)
#define i2c_hal_disable_data_output(hal) i2c_ll_disable_data_output(&(hal)->hw, (hal)->id)
#define i2c_hal_set_byte_interval(hal, interval) i2c_ll_set_byte_interval(&(hal)->hw, (hal)->id, interval)

bk_err_t i2c_hal_init(i2c_hal_t *hal);
bk_err_t i2c_hal_set_baud_rate(i2c_hal_t *hal, uint32_t baud_rate);
bk_err_t i2c_hal_configure(i2c_hal_t *hal, const i2c_config_t *cfg);
bk_err_t i2c_hal_start_common(i2c_hal_t *hal);
bk_err_t i2c_hal_stop_common(i2c_hal_t *hal);
bk_err_t i2c_hal_set_write_int_mode(i2c_hal_t *hal, uint32_t data_size);
bk_err_t i2c_hal_set_read_int_mode(i2c_hal_t *hal, uint32_t data_size);
bk_err_t i2c_hal_soft_reset(i2c_hal_t *hal);

#if (defined(CONFIG_I2C_PM_CB_SUPPORT))
void i2c_hal_backup(i2c_hal_t *hal, uint32_t *pm_backup);
void i2c_hal_restore(i2c_hal_t *hal, uint32_t *pm_backup);
#endif

#if CFG_HAL_DEBUG_I2C
void i2c_struct_dump(i2c_id_t id);
#else
#define i2c_struct_dump(id)
#endif

#ifdef __cplusplus
}
#endif

