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
#include "gpio_hw.h"
#include "gpio_hal.h"
#include "gpio_ll.h"

#if CFG_HAL_DEBUG_GPIO

void gpio_struct_dump(gpio_id_t gpio_index)
{
	gpio_hw_t *hw = (gpio_hw_t *)GPIO_LL_REG_BASE;
	system_gpio_func_mode_hw_t *gpio_func_hw = (system_gpio_func_mode_hw_t *)GPIO_LL_SYSTEM_REG_BASE;

	SOC_LOGI("base=%x\n", (uint32_t)hw);

	SOC_LOGI("gpio[%d] cfg: %x\n", gpio_index, hw->gpio_num[gpio_index].cfg.v);

	SOC_LOGI("  gpio_input:         %x\n", hw->gpio_num[gpio_index].cfg.gpio_input);
	SOC_LOGI("  gpio_output:        %x\n", hw->gpio_num[gpio_index].cfg.gpio_output);
	SOC_LOGI("  gpio_input_en:      %x\n", hw->gpio_num[gpio_index].cfg.gpio_input_en);
	SOC_LOGI("  gpio_output_en:     %x\n", hw->gpio_num[gpio_index].cfg.gpio_output_en);
	SOC_LOGI("  gpio_pull_mode:     %x\n", hw->gpio_num[gpio_index].cfg.gpio_pull_mode);
	SOC_LOGI("  gpio_pull_mode_en:  %x\n", hw->gpio_num[gpio_index].cfg.gpio_pull_mode_en);
	SOC_LOGI("  gpio_2_func_en:     %x\n", hw->gpio_num[gpio_index].cfg.gpio_2_func_en);
	SOC_LOGI("  gpio_input_monitor: %x\n", hw->gpio_num[gpio_index].cfg.gpio_input_monitor);
	SOC_LOGI("  gpio_capacity:      %x\n", hw->gpio_num[gpio_index].cfg.gpio_capacity);
	SOC_LOGI("  gpio_int_type:      %x\n", hw->gpio_num[gpio_index].cfg.gpio_int_type);
	SOC_LOGI("  gpio_int_en:        %x\n", hw->gpio_num[gpio_index].cfg.gpio_int_en);
	SOC_LOGI("  gpio_int_clear:     %x\n", hw->gpio_num[gpio_index].cfg.gpio_int_clear);
	SOC_LOGI("  reserved:           %x\n", hw->gpio_num[gpio_index].cfg.reserved);
	SOC_LOGI("\r\n");

	SOC_LOGI("  gpio_0_7_func_mode:   %x\n", gpio_func_hw->gpio_sys_num[0].v);
	SOC_LOGI("  gpio_8_15_func_mode:  %x\n", gpio_func_hw->gpio_sys_num[1].v);
	SOC_LOGI("  gpio_16_23_func_mode: %x\n", gpio_func_hw->gpio_sys_num[2].v);
	SOC_LOGI("  gpio_24_31_func_mode: %x\n", gpio_func_hw->gpio_sys_num[3].v);
	SOC_LOGI("  gpio_32_39_func_mode: %x\n", gpio_func_hw->gpio_sys_num[4].v);
	SOC_LOGI("  gpio_40_47_func_mode: %x\n", gpio_func_hw->gpio_sys_num[5].v);
	SOC_LOGI("\r\n");

	SOC_LOGI("  gpio_0_31_int_st:  %x\n", hw->gpio_0_31_int_st.v);
	SOC_LOGI("  gpio_32_47_int_st: %x\n", hw->gpio_32_47_int_st.v);
}

#endif

