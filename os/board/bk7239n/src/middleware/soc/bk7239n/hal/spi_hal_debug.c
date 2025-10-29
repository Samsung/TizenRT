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

#include "hal_config.h"
#include "spi_hw.h"
#include "spi_hal.h"
#include "spi_ll.h"

#if CFG_HAL_DEBUG_SPI

void spi_struct_dump(spi_id_t id)
{
	spi_hw_t *hw = (spi_hw_t *)SPI_LL_REG_BASE(id);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  device_id=0x%x value=0x%x\n", &hw->dev_id, hw->dev_id);
	SOC_LOGI("  dev_version=0x%x value=0x%x\n", &hw->dev_version, hw->dev_version);

	SOC_LOGI("  global_ctrl=0x%x value=0x%x\n", &hw->global_ctrl, hw->global_ctrl.v);
	SOC_LOGI("    soft_reset:      %x\n", hw->global_ctrl.soft_reset);
	SOC_LOGI("    clk_gate_bypass: %x\n", hw->global_ctrl.clk_gate_bypass);
	SOC_LOGI("    reserved:        %x\n", hw->global_ctrl.reserved);
	SOC_LOGI("\r\n");

	SOC_LOGI("  dev_status=0x%x value=0x%x\n", &hw->dev_status, hw->dev_status);
	SOC_LOGI("\r\n");

	SOC_LOGI("  ctrl=0x%x value=0x%x\n", &hw->ctrl, hw->ctrl.v);
	SOC_LOGI("    tx_fifo_int_level:    %x\n", hw->ctrl.tx_fifo_int_level);
	SOC_LOGI("    rx_fifo_int_level:    %x\n", hw->ctrl.rx_fifo_int_level);
	SOC_LOGI("    tx_udf_int_en:        %x\n", hw->ctrl.tx_udf_int_en);
	SOC_LOGI("    rx_ovf_int_en:        %x\n", hw->ctrl.rx_ovf_int_en);
	SOC_LOGI("    tx_fifo_int_en:       %x\n", hw->ctrl.tx_fifo_int_en);
	SOC_LOGI("    rx_fifo_int_en:       %x\n", hw->ctrl.rx_fifo_int_en);
	SOC_LOGI("    clk_rate:             %x\n", hw->ctrl.clk_rate);
	SOC_LOGI("    slave_release_int_en: %x\n", hw->ctrl.slave_release_int_en);
	SOC_LOGI("    wire3_en:             %x\n", hw->ctrl.wire3_en);
	SOC_LOGI("    bit_width:            %x\n", hw->ctrl.bit_width);
	SOC_LOGI("    lsb_first_en:         %x\n", hw->ctrl.lsb_first_en);
	SOC_LOGI("    cpol:                 %x\n", hw->ctrl.cpol);
	SOC_LOGI("    cpha:                 %x\n", hw->ctrl.cpha);
	SOC_LOGI("    master_en:            %x\n", hw->ctrl.master_en);
	SOC_LOGI("    enable:               %x\n", hw->ctrl.enable);
	SOC_LOGI("    byte_interval:        %x\n", hw->ctrl.byte_interval);
	SOC_LOGI("    rx_sample_edge:       %x\n", hw->ctrl.reserved);

	SOC_LOGI("\n");
	SOC_LOGI("  config=0x%x value=0x%x\n", &hw->cfg, hw->cfg.v);
	SOC_LOGI("    tx_en:            %x\n", hw->cfg.tx_en);
	SOC_LOGI("    rx_en:            %x\n", hw->cfg.rx_en);
	SOC_LOGI("    tx_finish_int_en: %x\n", hw->cfg.tx_finish_int_en);
	SOC_LOGI("    rx_finish_int_en: %x\n", hw->cfg.rx_finish_int_en);
	SOC_LOGI("    tx_trans_len:     %x\n", hw->cfg.tx_trans_len);
	SOC_LOGI("    rx_trans_len:     %x\n", hw->cfg.rx_trans_len);

	SOC_LOGI("\n");
	SOC_LOGI("  status=0x%x value=0x%x\n", &hw->int_status, hw->int_status.v);
	SOC_LOGI("    tx_fifo_wr_ready:  %x\n", hw->int_status.tx_fifo_wr_ready);
	SOC_LOGI("    rx_fifo_rd_ready:  %x\n", hw->int_status.rx_fifo_rd_ready);
	SOC_LOGI("    tx_fifo_int:       %x\n", hw->int_status.tx_fifo_int);
	SOC_LOGI("    rx_fifo_int:       %x\n", hw->int_status.rx_fifo_int);
	SOC_LOGI("    slave_release_int: %x\n", hw->int_status.slave_release_int);
	SOC_LOGI("    tx_underflow:      %x\n", hw->int_status.tx_underflow_int);
	SOC_LOGI("    rx_overflow:       %x\n", hw->int_status.rx_overflow_int);
	SOC_LOGI("    tx_finish_int:     %x\n", hw->int_status.tx_finish_int);
	SOC_LOGI("    rx_finish_int:     %x\n", hw->int_status.rx_finish_int);
	SOC_LOGI("    tx_fifo_clr:       %x\n", hw->int_status.tx_fifo_clr);
	SOC_LOGI("    rx_fifo_clr:       %x\n", hw->int_status.rx_fifo_clr);

	SOC_LOGI("\n");
	SOC_LOGI("  data=0x%x value=0x%x\n", &hw->data, hw->data.v);
	SOC_LOGI("    fifo_data:  %x\n", hw->data.fifo_data);
}

#endif

