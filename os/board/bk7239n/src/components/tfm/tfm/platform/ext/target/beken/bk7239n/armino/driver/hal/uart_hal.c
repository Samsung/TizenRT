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

#include "uart_hal.h"
#include "uart_ll.h"

bk_err_t uart_hal_init(uart_hal_t *hal)
{
	hal->hw = (uart_hw_t *)UART_LL_REG_BASE(hal->id);
	uart_ll_init(hal->hw);
	return BK_OK;
}

/* 1. disable int
 * 2. set config: tx,rx en; idra; data_bits; parity; stop_bits; clk_div
 * 3. set fifo_config: fifo_threshold; stop_detect_time
 * 4. disable flow_control
 * 5. disable wake_en
 * 6. enable int
 */
bk_err_t uart_hal_init_uart(uart_hal_t *hal, uart_id_t id, const uart_config_t *config)
{
	uart_ll_reset_int_en_to_default(hal->hw, id);
	uart_ll_reset_fifo_port_to_default(hal->hw, id);

	uart_ll_set_mode_uart(hal->hw, id);
	uart_ll_set_data_bits(hal->hw, id, config->data_bits);
	uart_ll_set_stop_bits(hal->hw, id, config->stop_bits);
	uart_hal_set_parity(hal, id, config->parity);
	uart_hal_set_baud_rate(hal, id, config->src_clk, config->baud_rate);

	uart_ll_set_tx_fifo_threshold(hal->hw, id, UART_TX_FIFO_THRESHOLD);
	uart_ll_set_rx_fifo_threshold(hal->hw, id, UART_RX_FIFO_THRESHOLD);
	uart_ll_set_rx_stop_detect_time(hal->hw, id, UART_RX_STOP_DETECT_TIME_32_BITS);

	uart_ll_reset_flow_control_to_default(hal->hw, id);
	if (config->flow_ctrl != UART_FLOWCTRL_DISABLE) {
		uart_hal_set_hw_flow_ctrl(hal, id, CONFIG_KFIFO_SIZE & 0xff);
		uart_ll_enable_flow_control(hal->hw, id);
	}

	#if CONFIG_UART_ERR_INTERRUPT
		uart_ll_enable_fifo_over_flow_interrupt(hal->hw, id);
		uart_ll_enable_parity_err_interrupt(hal->hw, id);
		uart_ll_enable_stop_bit_err_interrupt(hal->hw, id);
	#endif

	uart_ll_reset_wake_config_to_default(hal->hw, id);

	return BK_OK;
}

bk_err_t uart_hal_start_common(uart_hal_t *hal, uart_id_t id)
{
	uart_ll_enable_tx(hal->hw, id);
	uart_ll_enable_rx(hal->hw, id);
	return BK_OK;
}

bk_err_t uart_hal_stop_common(uart_hal_t *hal, uart_id_t id)
{
	uart_ll_reset_int_en_to_default(hal->hw, id);
	uart_ll_clear_id_interrupt_status(hal->hw, id);
	uart_ll_disable_tx(hal->hw, id);
	uart_ll_disable_rx(hal->hw, id);
	return BK_OK;
}

bk_err_t uart_hal_flush_fifo(uart_hal_t *hal, uart_id_t id)
{
	uart_ll_disable_tx(hal->hw, id);
	uart_ll_disable_rx(hal->hw, id);

	uart_ll_enable_tx(hal->hw, id);
	uart_ll_enable_rx(hal->hw, id);

	return BK_OK;
}

bk_err_t uart_hal_set_baud_rate(uart_hal_t *hal, uart_id_t id, uint32_t sclk, uint32_t baud_rate)
{
	uint32_t clk_div = 0;
#if (CONFIG_SYSTEM_CTRL)
        if (sclk == UART_SCLK_APLL) {
                clk_div = UART_CLOCK_FREQ_120M / baud_rate - 1;
        } else {
                clk_div = UART_CLOCK / baud_rate - 1;
	}
#else
	if (sclk == UART_SCLK_DCO) {
		clk_div = UART_CLOCK_FREQ_120M / baud_rate - 1;
	} else {
		clk_div = UART_CLOCK / baud_rate - 1;
	}
#endif
	
	uart_ll_set_clk_div(hal->hw, id, clk_div);

	return BK_OK;
}

bk_err_t uart_hal_set_parity(uart_hal_t *hal, uart_id_t id, uart_parity_t parity)
{
	if (parity == UART_PARITY_NONE) {
		uart_ll_disable_parity(hal->hw, id);
	} else {
		uart_ll_enable_parity(hal->hw, id);
	}
	uart_ll_set_parity(hal->hw, id, parity);
	return BK_OK;
}

bk_err_t uart_hal_set_hw_flow_ctrl(uart_hal_t *hal, uart_id_t id, uint8_t rx_threshold)
{
	uart_ll_set_flow_control_low_cnt(hal->hw, id, 0);
	uart_ll_set_flow_control_high_cnt(hal->hw, id, rx_threshold);
	uart_ll_set_rts_polarity(hal->hw, id, 1);
	uart_ll_set_cts_polarity(hal->hw, id, 1);
	return BK_OK;
}

bk_err_t uart_hal_disable_hw_flow_ctrl(uart_hal_t *hal, uart_id_t id)
{
	uart_ll_disable_flow_control(hal->hw, id);
	uart_ll_reset_flow_control_to_default(hal->hw, id);
	return BK_OK;
}

uint32_t uart_hal_get_system_interrput_en_status(uart_id_t id)
{
	uint32_t sys_int_en_status = 0;
#if (CONFIG_SYSTEM_CTRL)
	switch(id)
	{
		case UART_ID_0:
			sys_int_en_status = sys_ll_get_cpu0_int_0_31_en_cpu0_uart_int_en();
			break;
		case UART_ID_1:
			sys_int_en_status = sys_ll_get_cpu0_int_0_31_en_cpu0_uart1_int_en();
			break;
		case UART_ID_2:
			sys_int_en_status = sys_ll_get_cpu0_int_0_31_en_cpu0_uart2_int_en();
			break;
		default:
			sys_int_en_status = 0;
			break;
	}
#endif
	return sys_int_en_status;
}

