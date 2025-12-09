// Copyright 2023-2025 Beken
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

#include "iomx_hw.h"
#include <driver/gpio_types.h>
#include "iomx_hal.h"
#include "sys_ll.h"

#define IOMX_GPIO_FOREACH(i) \
	for (gpio_id_t i = 0; i < GPIO_NUM_MAX; i++)

bk_err_t iomx_hal_init(void)
{
	#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	iomx_ll_set_clkg_reset_soft_reset(1);
	#endif

	// workaround fix for gpio interrupt
	sys_ll_set_cpu_device_clk_enable_gpio_cken(1);

	return BK_OK;
}

bk_err_t iomx_hal_deinit(void)
{
	#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	iomx_ll_set_clkg_reset_soft_reset(0);
	#endif

	return BK_OK;
}

void iomx_hal_set_value(gpio_id_t id, uint32_t v)
{
	iomx_ll_set_gpio_cfg_value(v, id);
}

uint32_t iomx_hal_get_value(gpio_id_t id)
{
	return iomx_ll_get_gpio_cfg_value(id);
}

bk_err_t iomx_hal_set_func_code(gpio_id_t gpio_id, uint32_t code)
{
	iomx_ll_set_gpio_function_sel(code, gpio_id);

	return BK_OK;
}

uint32_t iomx_hal_get_func_code(gpio_id_t gpio_id)
{
	return iomx_ll_get_gpio_function_sel(gpio_id);
}

bk_err_t iomx_hal_pull_up_enable(gpio_id_t gpio_id, uint32 enable)
{
	iomx_ll_set_gpio_pull_mode(enable, gpio_id);

	return BK_OK;
}

bk_err_t iomx_hal_pull_enable(gpio_id_t gpio_id, uint32 enable)
{
	iomx_ll_set_gpio_pull_ena(enable, gpio_id);

	return BK_OK;
}

bk_err_t iomx_hal_monitor_input_enable(gpio_id_t gpio_id, uint32 enable)
{
	iomx_ll_set_input_monitor(enable, gpio_id);

	return BK_OK;
}

bk_err_t iomx_hal_set_capacity(gpio_id_t gpio_id, uint32 capacity)
{
	iomx_ll_set_gpio_capacity(capacity, gpio_id);

	return BK_OK;
}

bk_err_t iomx_hal_set_output_value(gpio_id_t gpio_id, uint32 output_value)
{
	iomx_ll_set_gpio_output(output_value, gpio_id);

	return BK_OK;
}

bk_err_t iomx_hal_get_input(gpio_id_t gpio_id)
{
	return iomx_ll_get_gpio_input(gpio_id);
}

bk_err_t iomx_hal_get_output(gpio_id_t gpio_id)
{
	return iomx_ll_get_gpio_output(gpio_id);
}


bk_err_t iomx_hal_set_int_type(gpio_id_t gpio_id, uint32_t type)
{
	iomx_ll_set_gpio_interrupt_type(type, gpio_id);

	return BK_OK;
}

//Beken internal use
bk_err_t iomx_hal_enable_interrupt(gpio_id_t gpio_id)
{
	iomx_ll_set_gpio_interrupt_ena(1, gpio_id);

	return BK_OK;
}

bk_err_t iomx_hal_enable_multi_interrupts(uint64_t gpio_idx)
{
	IOMX_GPIO_FOREACH(i)
	{
		if (gpio_idx & BIT64(i))
		{
			iomx_ll_set_gpio_interrupt_ena(1, i);
		}
	}

	return BK_OK;
}

bk_err_t iomx_hal_disable_all_interrupts(void)
{
	IOMX_GPIO_FOREACH(i)
	{
		iomx_ll_set_gpio_interrupt_ena(0, i);
		iomx_ll_set_gpio_interrupt_clear(1, i);
		iomx_ll_set_gpio_interrupt_clear(0, i);
	}

	return BK_OK;
}

bk_err_t iomx_hal_get_interrupt_overview(void)
{
	return iomx_ll_get_gpio_intsta_overview_value();
}

bk_err_t iomx_hal_bakup_configs(uint32_t *iomx_gpio_cfgs)
{
	if (!iomx_gpio_cfgs)
	{
		return BK_FAIL;
	}

	IOMX_GPIO_FOREACH(i)
	{
		iomx_gpio_cfgs[i] = iomx_ll_get_gpio_cfg_value(i);
	}

	return BK_OK;
}

bk_err_t iomx_hal_restore_configs(uint32_t *iomx_gpio_cfgs)
{
	if (!iomx_gpio_cfgs)
	{
		return BK_FAIL;
	}

	IOMX_GPIO_FOREACH(i)
	{
		iomx_ll_set_gpio_cfg_value(iomx_gpio_cfgs[i], i);
	}

	return BK_OK;
}
// eof

