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

#include "hal_config.h"
#include "uart_hw.h"
#include "uart_hal.h"
#include "uart_ll.h"

#if CFG_HAL_DEBUG_UART

void uart_struct_dump(uart_id_t id)
{
	uart_hw_t *hw = (uart_hw_t *)UART_LL_REG_BASE(id);
	SOC_LOGI("uart(%x) base=%x\r\n", id, (uint32_t)hw);

	SOC_LOGI("  device_id=0x%x value=0x%x\n", &hw->dev_id, hw->dev_id);
	SOC_LOGI("  dev_version=0x%x value=0x%x\n", &hw->dev_version, hw->dev_version);


	SOC_LOGI("  global_ctrl=0x%x value=0x%x\n", &hw->global_ctrl, hw->global_ctrl.v);
	SOC_LOGI("    soft_reset:      %x\n", hw->global_ctrl.soft_reset);
	SOC_LOGI("    clk_gate_bypass: %x\n", hw->global_ctrl.clk_gate_bypass);
	SOC_LOGI("    reserved:        %x\n", hw->global_ctrl.reserved);
	SOC_LOGI("\r\n");

	SOC_LOGI("  dev_status=0x%x value=0x%x\n", &hw->dev_status, hw->dev_status);
	SOC_LOGI("\r\n");

	SOC_LOGI("  config=0x%x value=0x%x\n", &hw->config, hw->config.v);
	SOC_LOGI("    tx_enable: %x\n", hw->config.tx_enable);
	SOC_LOGI("    rx_enable: %x\n", hw->config.rx_enable);
	SOC_LOGI("    data_bits: %x\n", hw->config.data_bits);
	SOC_LOGI("    parity_en: %x\n", hw->config.parity_en);
	SOC_LOGI("    parity:    %x\n", hw->config.parity);
	SOC_LOGI("    stop_bits: %x\n", hw->config.stop_bits);
	SOC_LOGI("    clk_div:   %x\n", hw->config.clk_div);
	SOC_LOGI("\r\n");

	SOC_LOGI("  fifo_config=0x%x value=0x%x\n", &hw->fifo_config, hw->fifo_config.v);
	SOC_LOGI("    tx_fifo_threshold:   %x\n", hw->fifo_config.tx_fifo_threshold);
	SOC_LOGI("    rx_fifo_threshold:   %x\n", hw->fifo_config.rx_fifo_threshold);
	SOC_LOGI("    rx_stop_detect_time: %x\n", hw->fifo_config.rx_stop_detect_time);

	SOC_LOGI("\n");
	SOC_LOGI("  fifo_status=0x%x value=0x%x\n", &hw->fifo_status, hw->fifo_status.v);
	SOC_LOGI("    tx_fifo_count: %x\n", hw->fifo_status.tx_fifo_count);
	SOC_LOGI("    rx_fifo_count: %x\n", hw->fifo_status.rx_fifo_count);
	SOC_LOGI("    tx_fifo_full:  %x\n", hw->fifo_status.tx_fifo_full);
	SOC_LOGI("    tx_fifo_empty: %x\n", hw->fifo_status.tx_fifo_empty);
	SOC_LOGI("    rx_fifo_full:  %x\n", hw->fifo_status.rx_fifo_full);
	SOC_LOGI("    rx_fifo_empty: %x\n", hw->fifo_status.rx_fifo_empty);
	SOC_LOGI("    fifo_wr_ready: %x\n", hw->fifo_status.fifo_wr_ready);
	SOC_LOGI("    fifo_rd_ready: %x\n", hw->fifo_status.fifo_rd_ready);

	SOC_LOGI("\n");
	SOC_LOGI("  fifo_port=0x%x value=0x%x\n", &hw->fifo_port, hw->fifo_port.v);
	SOC_LOGI("    tx_fifo_data_in:  %x\n", hw->fifo_port.tx_fifo_data_in);
	SOC_LOGI("    rx_fifo_data_out: %x\n", hw->fifo_port.rx_fifo_data_out);

	SOC_LOGI("\n");
	SOC_LOGI("  int_enable=0x%x value=0x%x\n", &hw->int_enable, hw->int_enable.v);
	SOC_LOGI("    tx_fifo_need_write: %x\n", hw->int_enable.tx_fifo_need_write);
	SOC_LOGI("    rx_fifo_need_read:  %x\n", hw->int_enable.rx_fifo_need_read);
	SOC_LOGI("    rx_fifo_overflow:   %x\n", hw->int_enable.rx_fifo_overflow);
	SOC_LOGI("    rx_parity_err:      %x\n", hw->int_enable.rx_parity_err);
	SOC_LOGI("    rx_stop_bits_err:   %x\n", hw->int_enable.rx_stop_bits_err);
	SOC_LOGI("    tx_finish:          %x\n", hw->int_enable.tx_finish);
	SOC_LOGI("    rx_finish:          %x\n", hw->int_enable.rx_finish);
	SOC_LOGI("    rxd_wakeup:         %x\n", hw->int_enable.rxd_wakeup);

	SOC_LOGI("\n");
	SOC_LOGI("  int_status=0x%x value=0x%x\n", &hw->int_status, hw->int_status.v);
	SOC_LOGI("    tx_fifo_need_write: %x\n", hw->int_status.tx_fifo_need_write);
	SOC_LOGI("    rx_fifo_need_read:  %x\n", hw->int_status.rx_fifo_need_read);
	SOC_LOGI("    rx_fifo_overflow:   %x\n", hw->int_status.rx_fifo_overflow);
	SOC_LOGI("    rx_parity_err:      %x\n", hw->int_status.rx_parity_err);
	SOC_LOGI("    rx_stop_bits_err:   %x\n", hw->int_status.rx_stop_bits_err);
	SOC_LOGI("    tx_finish:          %x\n", hw->int_status.tx_finish);
	SOC_LOGI("    rx_finish:          %x\n", hw->int_status.rx_finish);
	SOC_LOGI("    rxd_wakeup:         %x\n", hw->int_status.rxd_wakeup);

	SOC_LOGI("\n");
	SOC_LOGI("  flow_config=0x%x value=0x%x\n", &hw->flow_ctrl_config, hw->flow_ctrl_config.v);
	SOC_LOGI("    flow_ctrl_low_cnt:  %x\n", hw->flow_ctrl_config.flow_ctrl_low_cnt);
	SOC_LOGI("    flow_ctrl_high_cnt: %x\n", hw->flow_ctrl_config.flow_ctrl_high_cnt);
	SOC_LOGI("    flow_ctrl_en:       %x\n", hw->flow_ctrl_config.flow_ctrl_en);
	SOC_LOGI("    rts_polarity_sel:   %x\n", hw->flow_ctrl_config.rts_polarity_sel);
	SOC_LOGI("    cts_polarity_sel:   %x\n", hw->flow_ctrl_config.cts_polarity_sel);

	SOC_LOGI("\n");
	SOC_LOGI("  wake_config=0x%x value=0x%x\n", &hw->wake_config, hw->wake_config.v);
	SOC_LOGI("    wake_cnt:             %x\n", hw->wake_config.wake_cnt);
	SOC_LOGI("    txd_wait_cnt:         %x\n", hw->wake_config.txd_wait_cnt);
	SOC_LOGI("    rxd_wake_en:          %x\n", hw->wake_config.rxd_wake_en);
	SOC_LOGI("    txd_wake_en:          %x\n", hw->wake_config.txd_wake_en);
	SOC_LOGI("    rxd_neg_edge_wake_en: %x\n", hw->wake_config.rxd_neg_edge_wake_en);
}

#endif

