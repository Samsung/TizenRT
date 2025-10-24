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
#include "i2c_hal.h"
#include "i2c_hw.h"
#include "i2c_ll.h"

#if CFG_HAL_DEBUG_I2C

void i2c_struct_dump(i2c_id_t id)
{
	i2c_typedef_t *hw = (i2c_typedef_t *)I2C_LL_REG_BASE(id);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  sm_bus_cfg=0x%x value=0x%x\n", &hw->sm_bus_cfg, hw->sm_bus_cfg.v);
	SOC_LOGI("    idle_cr:     %x\n", hw->sm_bus_cfg.idle_cr);
	SOC_LOGI("    scl_cr:      %x\n", hw->sm_bus_cfg.scl_cr);
	SOC_LOGI("    freq_div:    %x\n", hw->sm_bus_cfg.freq_div);
	SOC_LOGI("    slave_addr:  %x\n", hw->sm_bus_cfg.slave_addr);
	SOC_LOGI("    clk_src:     %x\n", hw->sm_bus_cfg.clk_src);
	SOC_LOGI("    timeout_en:  %x\n", hw->sm_bus_cfg.timeout_en);
	SOC_LOGI("    idle_det_en: %x\n", hw->sm_bus_cfg.idle_det_en);
	SOC_LOGI("    inh:         %x\n", hw->sm_bus_cfg.inh);
	SOC_LOGI("    en:          %x\n", hw->sm_bus_cfg.en);

	SOC_LOGI("\n");
	SOC_LOGI("  sm_bus_status=0x%x value=0x%x\n", &hw->sm_bus_status, hw->sm_bus_status.v);
	SOC_LOGI("    sm_int:        %x\n", hw->sm_bus_status.sm_int);
	SOC_LOGI("    scl_timeout:   %x\n", hw->sm_bus_status.scl_timeout);
	SOC_LOGI("    reserved0:     %x\n", hw->sm_bus_status.reserved0);
	SOC_LOGI("    arb_lost:      %x\n", hw->sm_bus_status.arb_lost);
	SOC_LOGI("    rx_fifo_empty: %x\n", hw->sm_bus_status.rx_fifo_empty);
	SOC_LOGI("    tx_fifo_full:  %x\n", hw->sm_bus_status.tx_fifo_full);
	SOC_LOGI("    int_mode:      %x\n", hw->sm_bus_status.int_mode);
	SOC_LOGI("    ack:           %x\n", hw->sm_bus_status.ack);
	SOC_LOGI("    stop:          %x\n", hw->sm_bus_status.stop);
	SOC_LOGI("    start:         %x\n", hw->sm_bus_status.start);
	SOC_LOGI("    addr_match:    %x\n", hw->sm_bus_status.addr_match);
	SOC_LOGI("    ack_req:       %x\n", hw->sm_bus_status.ack_req);
	SOC_LOGI("    tx_mode:       %x\n", hw->sm_bus_status.tx_mode);
	SOC_LOGI("    master:        %x\n", hw->sm_bus_status.master);
	SOC_LOGI("    busy:          %x\n", hw->sm_bus_status.busy);
	SOC_LOGI("    reserved1:     %x\n", hw->sm_bus_status.reserved1);

	SOC_LOGI("\n");
	SOC_LOGI("  sm_bus_data=0x%x value=0x%x\n", &hw->sm_bus_data, hw->sm_bus_data.v);
	SOC_LOGI("    data:     %x\n", hw->sm_bus_data.data);
	SOC_LOGI("    reserved: %x\n", hw->sm_bus_data.reserved);
}

#endif

