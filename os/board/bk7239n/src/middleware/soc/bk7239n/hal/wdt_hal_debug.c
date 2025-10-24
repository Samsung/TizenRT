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
#include "wdt_hw.h"
#include "wdt_hal.h"
#include "wdt_ll.h"

#if CFG_HAL_DEBUG_WDT

void wdt_struct_dump(void)
{
	SOC_LOGI("system_0xa:%x\r\n", REG_READ(SOC_SYSTEM_REG_BASE + 0xa * 4));
	SOC_LOGI("system_0xc:%x\r\n", REG_READ(SOC_SYSTEM_REG_BASE + 0xc * 4));
	SOC_LOGI("system_0x20=0x%x\r\n", REG_READ(SOC_SYSTEM_REG_BASE + 0x20 * 4));
	SOC_LOGI("pmu_0x2=0x%x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + 0x2 * 4));

	wdt_hw_t *hw = (wdt_hw_t *)WDT_LL_REG_BASE(0);
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
	SOC_LOGI("    period: 0x%x\n", hw->ctrl.period);
	SOC_LOGI("    key: 0x%x\n", hw->ctrl.key);
}

#endif


