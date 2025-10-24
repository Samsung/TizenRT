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
#include "efuse_hw.h"
#include "efuse_hal.h"
#include "efuse_ll.h"

#if CFG_HAL_DEBUG_EFUSE

void efuse_struct_dump(void)
{
	efuse_hw_t *hw = (efuse_hw_t *)EFUSE_LL_REG_BASE(0);
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

	SOC_LOGI("  ctrl=0x%x value=0x%x\r\n", &hw->ctrl, hw->ctrl.v);
	SOC_LOGI("    en:       0x%x\r\n", hw->ctrl.en);
	SOC_LOGI("    dir:      0x%x\r\n", hw->ctrl.dir);
	SOC_LOGI("    addr:     0x%x\r\n", hw->ctrl.addr);
	SOC_LOGI("    wr_data:  0x%x\r\n", hw->ctrl.wr_data);
	SOC_LOGI("    vdd25_en: 0x%x\r\n", hw->ctrl.vdd25_en);
	SOC_LOGI("\r\n");

	SOC_LOGI("  optr=0x%x value=0x%x\r\n", &hw->optr, hw->optr.v);
	SOC_LOGI("    rd_data:       0x%x\r\n", hw->optr.rd_data);
	SOC_LOGI("    rd_data_valid: 0x%x\r\n", hw->optr.rd_data_valid);
}

#endif


