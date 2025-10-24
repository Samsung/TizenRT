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
#include "timer_hw.h"
#include "timer_hal.h"
#include "timer_ll.h"

#if CFG_HAL_DEBUG_TIMER

void timer_struct_dump(void)
{
	timer_hw_t *hw = (timer_hw_t *)TIMER_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	for (int group = 0; group < SOC_TIMER_GROUP_NUM; group++) {
		SOC_LOGI("group(%x)\r\n", group);

		SOC_LOGI("  device_id=0x%x value=0x%x\n", &hw->group[group].dev_id, hw->group[group].dev_id);
		SOC_LOGI("  dev_version=0x%x value=0x%x\n", &hw->group[group].dev_version, hw->group[group].dev_version);


		SOC_LOGI("  global_ctrl=0x%x value=0x%x\n", &hw->group[group].global_ctrl, hw->group[group].global_ctrl.v);
		SOC_LOGI("    soft_reset:      %x\n", hw->group[group].global_ctrl.soft_reset);
		SOC_LOGI("    clk_gate_bypass: %x\n", hw->group[group].global_ctrl.clk_gate_bypass);
		SOC_LOGI("    reserved:        %x\n", hw->group[group].global_ctrl.reserved);
		SOC_LOGI("\r\n");

		SOC_LOGI("  dev_status=0x%x value=0x%x\n", &hw->group[group].dev_status, hw->group[group].dev_status);

		for (int chan = 0; chan < SOC_TIMER_CHAN_NUM_PER_GROUP; chan++) {
			SOC_LOGI("  timer%x_val addr=%x value=%x\r\n", chan, &hw->group[group].timer_cnt[chan], hw->group[group].timer_cnt[chan]);
		}

		SOC_LOGI("\n");
		SOC_LOGI("  ctrl addr=%x value=%x\n", &hw->group[group].ctrl, hw->group[group].ctrl.v);
		SOC_LOGI("    timer0_en:  %x\n", hw->group[group].ctrl.timer0_en);
		SOC_LOGI("    timer1_en:  %x\n", hw->group[group].ctrl.timer1_en);
		SOC_LOGI("    timer2_en:  %x\n", hw->group[group].ctrl.timer2_en);
		SOC_LOGI("    clk_div:    %x\n", hw->group[group].ctrl.clk_div);
		SOC_LOGI("    timer0_int: %x\n", hw->group[group].ctrl.timer0_int_en);
		SOC_LOGI("    timer1_int: %x\n", hw->group[group].ctrl.timer1_int_en);
		SOC_LOGI("    timer2_int: %x\n", hw->group[group].ctrl.timer2_int_en);

		SOC_LOGI("\n");
		SOC_LOGI("  read_ctrl addr=%x value=%x\n", &hw->group[group].read_ctrl, hw->group[group].read_ctrl.v);
		SOC_LOGI("    timer_cnt_read: %x\n", hw->group[group].read_ctrl.timer_cnt_read);
		SOC_LOGI("    timer_index:    %x\n", hw->group[group].read_ctrl.timer_index);

		SOC_LOGI("\n");
		SOC_LOGI("  timer_cnt addr=%x value=%x\n", &hw->group[group].timer_read_value, hw->group[group].timer_read_value);
		SOC_LOGI("\n");
	}
}

#endif

