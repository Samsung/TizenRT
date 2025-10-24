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
#include "uart_hw.h"
#include "uart_ll.h"
#include <driver/hal/hal_uart_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uart_hw_t *hw;
	uart_unit_t id;
} uart_hal_t;

#define UART_HAL_ATE_ID (UART_LL_ATE_ID)

#define UART_HAL_ATE_REUSE_PIN (UART_LL_ATE_REUSE_PIN)	//RE-USE for ATE

#define uart_hal_enable_tx_interrupt(hal, id) uart_ll_enable_tx_interrupt((hal)->hw, id)
#define uart_hal_disable_tx_interrupt(hal, id) uart_ll_disable_tx_interrupt((hal)->hw, id)
#define uart_hal_enable_rx_interrupt(hal, id) uart_ll_enable_rx_interrupt((hal)->hw, id)
#define uart_hal_disable_rx_interrupt(hal, id) uart_ll_disable_rx_interrupt((hal)->hw, id)
#define uart_hal_get_interrupt_status(hal, id) uart_ll_get_interrupt_status((hal)->hw, id)
#define uart_hal_clear_interrupt_status(hal, id, status) uart_ll_clear_interrupt_status((hal)->hw, id, status)
#define uart_hal_clear_id_interrupt_status(hal, id) uart_ll_clear_id_interrupt_status((hal)->hw, id)
#define uart_hal_clear_id_tx_interrupt_status(hal, id) uart_ll_clear_id_tx_interrupt_status((hal)->hw, id)
#define uart_hal_clear_id_rx_interrupt_status(hal, id) uart_ll_clear_id_rx_interrupt_status((hal)->hw, id)

#define uart_hal_set_int_enable_status(hal, id, value) uart_ll_set_int_enable_status((hal)->hw, id, value)
#define uart_hal_get_int_enable_status(hal, id) uart_ll_get_int_enable_status((hal)->hw, id)
#define uart_hal_is_rx_interrupt_triggered(hal, id, status) uart_ll_is_rx_interrupt_triggered((hal)->hw, id, status)
#define uart_hal_is_tx_interrupt_triggered(hal, id, status) uart_ll_is_tx_interrupt_triggered((hal)->hw, id, status)
#define uart_hal_is_rx_recv_fini_int_triggered(hal, id, status) uart_ll_is_rx_recv_fini_int_triggered((hal)->hw, id, status)
#if CONFIG_UART_ERR_INTERRUPT
	#define uart_hal_is_rx_over_flow_int_triggered(hal, id, status) uart_ll_is_rx_over_flow_int_triggered((hal)->hw, id, status)
	#define uart_hal_is_rx_parity_err_int_triggered(hal, id, status) uart_ll_is_rx_parity_err_int_triggered((hal)->hw, id, status)
	#define uart_hal_is_rx_stop_bits_err_int_triggered(hal, id, status) uart_ll_is_rx_stop_bits_err_int_triggered((hal)->hw, id, status)
#endif
#define uart_hal_disable_tx(hal, id) uart_ll_disable_tx((hal)->hw, id)
#define uart_hal_disable_rx(hal, id) uart_ll_disable_rx((hal)->hw, id)
#define uart_hal_set_tx_enable(hal, id, enable) uart_ll_set_tx_enable((hal)->hw, id, enable)
#define uart_hal_set_rx_enable(hal, id, enable) uart_ll_set_rx_enable((hal)->hw, id, enable)
#define uart_hal_get_tx_enable(hal) uart_ll_get_tx_enable((hal)->hw)
#define uart_hal_get_rx_enable(hal) uart_ll_get_rx_enable((hal)->hw)

#define uart_hal_set_data_bits(hal, id, data_bits) uart_ll_set_data_bits((hal)->hw, id, data_bits)
#define uart_hal_set_stop_bits(hal, id, stop_bits) uart_ll_set_stop_bits((hal)->hw, id, stop_bits)
#define uart_hal_reset_config_to_default(hal, id) uart_ll_reset_config_to_default((hal)->hw, id)
#define uart_hal_set_frame_mode(hal, id, mode) uart_ll_set_frame_mode((hal)->hw, id, mode)
#define uart_hal_set_tx_fifo_threshold(hal, id, value) uart_ll_set_tx_fifo_threshold((hal)->hw, id, value)
#define uart_hal_set_rx_fifo_threshold(hal, id, value) uart_ll_set_rx_fifo_threshold((hal)->hw, id, value)
#define uart_hal_set_rx_stop_detect_time(hal, id, value) uart_ll_set_rx_stop_detect_time((hal)->hw, id, value)

#define uart_hal_get_tx_fifo_cnt(hal, id) uart_ll_get_tx_fifo_cnt((hal)->hw, id)
#define uart_hal_get_rx_fifo_cnt(hal, id) uart_ll_get_rx_fifo_cnt((hal)->hw, id)
#define uart_hal_is_tx_fifo_full(hal, id) uart_ll_is_tx_fifo_full((hal)->hw, id)
#define uart_hal_is_tx_fifo_empty(hal, id) uart_ll_is_tx_fifo_empty((hal)->hw, id)
#define uart_hal_is_rx_fifo_full(hal, id) uart_ll_is_rx_fifo_full((hal)->hw, id)
#define uart_hal_is_rx_fifo_empty(hal, id) uart_ll_is_rx_fifo_empty((hal)->hw, id)
#define uart_hal_is_fifo_write_ready(hal, id) uart_ll_is_fifo_write_ready((hal)->hw, id)
#define uart_hal_is_fifo_read_ready(hal, id) uart_ll_is_fifo_read_ready((hal)->hw, id)

#define uart_hal_enable_flow_control(hal, id) uart_ll_enable_flow_control((hal)->hw, id)
#define uart_hal_disable_flow_control(hal, id) uart_ll_disable_flow_control((hal)->hw, id)
#define uart_hal_is_flow_control_enabled(hal, id) uart_ll_is_flow_control_enabled((hal)->hw, id)

#define uart_hal_write_byte(hal, id, data) uart_ll_write_byte((hal)->hw, id, data)
#define uart_hal_read_byte(hal, id) uart_ll_read_byte((hal)->hw, id)
#define uart_hal_get_tx_pin(id) uart_ll_get_tx_pin(id)
#define uart_hal_get_rx_pin(id) uart_ll_get_rx_pin(id)
#define uart_hal_get_cts_pin(id) uart_ll_get_cts_pin(id)
#define uart_hal_get_rts_pin(id) uart_ll_get_rts_pin(id)

#define uart_hal_wait_tx_over() uart_ll_wait_tx_over()
//#define uart_hal_get_ate_id() uart_ll_ate_uart_id()

bk_err_t uart_hal_init(uart_hal_t *hal);
bk_err_t uart_hal_init_uart(uart_hal_t *hal, uart_id_t id, const uart_config_t *config);
bk_err_t uart_hal_start_common(uart_hal_t *hal, uart_id_t id);
bk_err_t uart_hal_stop_common(uart_hal_t *hal, uart_id_t id);
bk_err_t uart_hal_flush_fifo(uart_hal_t *hal, uart_id_t id);
bk_err_t uart_hal_set_baud_rate(uart_hal_t *hal, uart_id_t id, uint32_t sclk, uint32_t baud_rate);
bk_err_t uart_hal_set_parity(uart_hal_t *hal, uart_id_t id, uart_parity_t parity);
bk_err_t uart_hal_set_hw_flow_ctrl(uart_hal_t *hal, uart_id_t id, uint8_t rx_threshold);
bk_err_t uart_hal_disable_hw_flow_ctrl(uart_hal_t *hal, uart_id_t id);
uint32_t uart_hal_get_system_interrput_en_status(uart_id_t id);


#if CFG_HAL_DEBUG_UART
void uart_struct_dump(uart_id_t id);
#else
#define uart_struct_dump(id)
#endif

#ifdef __cplusplus
}
#endif

