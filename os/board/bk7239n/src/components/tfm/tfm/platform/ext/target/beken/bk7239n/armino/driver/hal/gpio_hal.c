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

#include <common/bk_include.h>
#include "gpio_hal.h"
#include <driver/gpio_types.h>
#include "icu_driver.h"

//move out from function, which uses too much STACK.
/*to optimize code size*/
#define GPIO_EN_SEC_FUNC(id) *(volatile uint32_t*) (SOC_AON_GPIO_REG_BASE + ((id) << 2)) = 0x48
static const gpio_map_t gpio_map_table[] = GPIO_DEV_MAP;
bk_err_t gpio_hal_init(gpio_hal_t *hal)
{
	hal->hw = (gpio_hw_t *)GPIO_LL_REG_BASE;
	gpio_ll_init(hal->hw);
	return BK_OK;
}

bk_err_t gpio_hal_disable_jtag_mode(gpio_hal_t *hal)
{
	return BK_OK;
}

bk_err_t gpio_hal_output_enable(gpio_hal_t *hal, gpio_id_t gpio_id, uint32 enable)
{
	gpio_ll_output_enable(hal->hw, gpio_id, enable);

	return BK_OK;
}

bk_err_t gpio_hal_input_enable(gpio_hal_t *hal, gpio_id_t gpio_id, uint32 enable)
{
	gpio_ll_input_enable(hal->hw, gpio_id, enable);

	return BK_OK;
}


bk_err_t gpio_hal_pull_up_enable(gpio_hal_t *hal, gpio_id_t gpio_id, uint32 enable)
{
	gpio_ll_pull_up_enable(hal->hw, gpio_id, enable);

	return BK_OK;
}


bk_err_t gpio_hal_pull_enable(gpio_hal_t *hal, gpio_id_t gpio_id, uint32 enable)
{
	gpio_ll_pull_enable(hal->hw, gpio_id, enable);

	return BK_OK;
}

bk_err_t gpio_hal_sencond_function_enable(gpio_hal_t *hal, gpio_id_t gpio_id, uint32 enable)
{
	gpio_ll_second_function_enable(hal->hw, gpio_id, enable);

	return BK_OK;
}


bk_err_t gpio_hal_monitor_input_enable(gpio_hal_t *hal, gpio_id_t gpio_id, uint32 enable)
{
	gpio_ll_monitor_input_value_enable(hal->hw, gpio_id, enable);

	return BK_OK;
}


bk_err_t gpio_hal_set_capacity(gpio_hal_t *hal, gpio_id_t gpio_id, uint32 capacity)
{
	gpio_ll_set_capacity(hal->hw, gpio_id, capacity);

	return BK_OK;
}


bk_err_t gpio_hal_set_output_value(gpio_hal_t *hal, gpio_id_t gpio_id, uint32 output_value)
{
	if(gpio_ll_check_output_enable(hal->hw, gpio_id))
		gpio_ll_set_gpio_output_value(hal->hw, gpio_id, output_value);
	else
		return BK_ERR_GPIO_NOT_OUTPUT_MODE;

	return BK_OK;
}

bk_err_t gpio_hal_get_input(gpio_hal_t *hal, gpio_id_t gpio_id)
{
	if(gpio_ll_check_input_enable(hal->hw, gpio_id))
		return (gpio_ll_get_gpio_input_value(hal->hw, gpio_id));
	else
		return BK_ERR_GPIO_NOT_INPUT_MODE;
}


bk_err_t gpio_hal_set_int_type(gpio_hal_t *hal, gpio_id_t gpio_id, gpio_int_type_t type)
{
	gpio_ll_set_interrupt_type(hal->hw, gpio_id, type);

	return BK_OK;
}

bk_err_t gpio_hal_enable_interrupt(gpio_hal_t *hal, gpio_id_t gpio_id)
{
	if(gpio_ll_check_input_enable(hal->hw, gpio_id)) {
		gpio_ll_enable_interrupt(hal->hw, gpio_id);
		return BK_OK;
	} else
		return BK_ERR_GPIO_NOT_INPUT_MODE;
}

#if 0
static bk_err_t gpio_hal_map_check(gpio_hal_t *hal, gpio_id_t gpio_id)
{
	const gpio_map_t *gpio_map = NULL;

	//special for BK7235:the GPIO ID isn't from 0~47, some of the GPIO are not exist.
	for(int i = 0; i < sizeof(gpio_map_table)/sizeof(gpio_map_t); i++)
	{
		if(gpio_map_table[i].id == gpio_id)
		{
			gpio_map = &gpio_map_table[i];
			break;
		}
	}
	if(gpio_map == NULL)
	{
		HAL_LOGE("gpio id=%d is not exist\r\n", gpio_id);
		return BK_ERR_GPIO_INVALID_ID;
	}

	//TODO check gpio is busy
	if(gpio_ll_check_func_mode_enable(hal->hw, gpio_id)) {
		uint32 func_mode = gpio_ll_get_gpio_perial_mode((hal)->hw, gpio_id);
		uint32 dev_use = gpio_map->dev[func_mode];

		HAL_LOGW("[gpio_log]:gpio:%d was busy: device num:0x%x!\r\n", gpio_id, dev_use);

		return BK_ERR_GPIO_INTERNAL_USED;
	}

	return BK_OK;
}
#endif

bk_err_t gpio_hal_func_map(gpio_hal_t *hal, gpio_id_t gpio_id, gpio_dev_t dev)
{
	const gpio_map_t *gpio_map = NULL;

	//special for BK7235:the GPIO ID isn't from 0~47, some of the GPIO are not exist.
	for(int i = 0; i < sizeof(gpio_map_table)/sizeof(gpio_map_t); i++)
	{
		if(gpio_map_table[i].id == gpio_id)
		{
			gpio_map = &gpio_map_table[i];
			break;
		}
	}
	if(gpio_map == NULL)
	{
		HAL_LOGE("gpio id=%d is not exist\r\n", gpio_id);
		return BK_ERR_GPIO_INVALID_ID;
	}
#if 0 /*to optimize code size*/
	if(gpio_hal_map_check(hal, gpio_id)) {
		return BK_ERR_GPIO_INTERNAL_USED;
	}
#endif
	//get peri mode of id
	if (dev == GPIO_DEV_NONE) {
		HAL_LOGE("gpio device is none, id=%d dev=%d\r\n", gpio_id, dev);
		return BK_ERR_GPIOS_MAP_NONE;
	} else {
		for (int peri_func = 0; peri_func < GPIO_PERI_FUNC_NUM; peri_func ++) {
			if (dev == gpio_map->dev[peri_func])
				gpio_ll_set_gpio_perial_mode((hal)->hw, gpio_id, peri_func);
				GPIO_EN_SEC_FUNC(gpio_id);
		}
	}

	// set gpio as second function mode
	/*to optimize code size*/
	// gpio_hal_output_enable(hal, gpio_id, 0);
	// gpio_hal_input_enable(hal, gpio_id, 0);
	// gpio_hal_pull_enable(hal, gpio_id, 0);
	// gpio_hal_sencond_function_enable(hal, gpio_id, 1);

	return BK_OK;
}

bk_err_t gpio_hal_func_unmap(gpio_hal_t *hal, gpio_id_t gpio_id)
{
	gpio_hal_sencond_function_enable(hal, gpio_id, 0);
	return BK_OK;
}

bk_err_t gpio_hal_devs_map(gpio_hal_t *hal, uint64 gpios, gpio_dev_t *devs, uint8 dev_num)
{
	int gpio_index = 0;
	int dev_id = 0;

	//check dev is null
	if(!devs) {
		HAL_LOGE("gpio devs is null \r\n");
		return BK_ERR_NULL_PARAM;
	}

	for(gpio_index = 0; gpio_index < SOC_GPIO_NUM; gpio_index++)
	{
		if (gpios & BIT64(gpio_index)) {
			HAL_LOGD("gpio_index = %d\r\n", gpio_index);
			gpio_hal_func_map(hal, gpio_index, devs[dev_id++]);
		}
	}

	if(dev_id != dev_num) {
		HAL_LOGE("dev_num expected %d actual is %d \r\n", dev_num, dev_id);

		return BK_ERR_GPIO_BITS_NUM;
	}

	return BK_OK;
}

bk_err_t gpio_hal_set_config(gpio_hal_t *hal, gpio_id_t gpio_id, const gpio_config_t *config)
{
#if 0 /*to optimize code size*/
	if(gpio_hal_map_check(hal, gpio_id)) {
		HAL_LOGE("gpio has map\r\n");
		return BK_ERR_GPIO_INTERNAL_USED;
	}
#endif
	gpio_ll_set_mode(hal->hw, gpio_id, config);

	return BK_OK;
}

#if CONFIG_GPIO_WAKEUP_SUPPORT
bk_err_t gpio_hal_bak_configs(uint16_t *gpio_cfg, uint32_t count)
{
	gpio_ll_bak_configs(gpio_cfg, count);
	return BK_OK;
}

bk_err_t gpio_hal_restore_configs(uint16_t *gpio_cfg, uint32_t count)
{
	gpio_ll_restore_configs(gpio_cfg, count);
	return BK_OK;
}

bk_err_t gpio_hal_bak_int_type_configs(uint32_t *gpio_int_type_cfg, uint32_t count)
{
	gpio_ll_bak_int_type_configs(gpio_int_type_cfg, count);

	return BK_OK;
}

bk_err_t gpio_hal_restore_int_type_configs(uint32_t *gpio_int_type_cfg, uint32_t count)
{
	gpio_ll_restore_int_type_configs(gpio_int_type_cfg, count);
	return BK_OK;
}

bk_err_t gpio_hal_bak_int_enable_configs(uint32_t *gpio_int_enable_cfg, uint32_t count)
{
	gpio_ll_bak_int_enable_configs(gpio_int_enable_cfg, count);

	return BK_OK;
}

bk_err_t gpio_hal_restore_int_enable_configs(uint32_t *gpio_int_enable_cfg, uint32_t count)
{
	gpio_ll_restore_int_enable_configs(gpio_int_enable_cfg, count);
	return BK_OK;
}

/* gpio switch to low power status:set all gpios to input mode to avoid power leakage */
bk_err_t gpio_hal_switch_to_low_power_status(uint64_t skip_io)
{
	gpio_ll_switch_to_low_power_status(skip_io);
	return BK_OK;
}
#else
bk_err_t gpio_hal_reg_save(uint32_t*  gpio_cfg)
{
	gpio_ll_reg_save(gpio_cfg);
	return BK_OK;
}
bk_err_t gpio_hal_reg_restore(uint32_t*  gpio_cfg)
{
	gpio_ll_reg_restore(gpio_cfg);
	return BK_OK;
}
bk_err_t gpio_hal_wakeup_enable(int64_t index, uint64_t type_l, uint64_t type_h)
{
	gpio_ll_wakeup_enable(index, type_l, type_h);
	return BK_OK;
}
bk_err_t gpio_hal_wakeup_interrupt_clear()
{
	gpio_ll_wakeup_interrupt_clear();
	return BK_OK;
}
#endif

#if CONFIG_GPIO_DEFAULT_SET_SUPPORT
bk_err_t gpio_hal_default_map_init(gpio_hal_t *hal)
{
	gpio_interrupt_status_t gpio_status;
	const gpio_default_map_t default_map[] = GPIO_DEFAULT_DEV_CONFIG;

	for(int i = 0; i < sizeof(default_map)/sizeof(gpio_default_map_t); i++)
	{
		gpio_hal_output_enable(hal, default_map[i].gpio_id, 0);
		gpio_hal_input_enable(hal, default_map[i].gpio_id, 0);
		gpio_hal_pull_enable(hal, default_map[i].gpio_id, 0);
		gpio_hal_disable_interrupt(hal, default_map[i].gpio_id);

		HAL_LOGD("gpio_id: %d, second_func_en:%d, second_func_dev %d, low_power_io_ctrl:%d",
					default_map[i].gpio_id,
					default_map[i].second_func_en, default_map[i].second_func_dev,
					default_map[i].low_power_io_ctrl);

		HAL_LOGD("int_en: %d, int_type:%d \r\n",
				default_map[i].int_en, default_map[i].int_type);

		//function mode
		if(default_map[i].second_func_en) {
			gpio_hal_func_unmap(hal, default_map[i].gpio_id);
			gpio_hal_func_map(hal, default_map[i].gpio_id, default_map[i].second_func_dev);
		}

		//low power
		if (default_map[i].low_power_io_ctrl == GPIO_LOW_POWER_KEEP_OUTPUT_STATUS)
			gpio_hal_output_enable(hal, default_map[i].gpio_id, 1);
		else if (default_map[i].low_power_io_ctrl == GPIO_LOW_POWER_KEEP_INPUT_STATUS)
			gpio_hal_input_enable(hal, default_map[i].gpio_id, 1);

		//io mode
		switch(default_map[i].io_mode)
		{
			case GPIO_IO_DISABLE:
				gpio_hal_output_enable(hal, default_map[i].gpio_id, 0);
				gpio_hal_input_enable(hal, default_map[i].gpio_id, 0);
				break;
			case GPIO_OUTPUT_ENABLE:
				gpio_hal_output_enable(hal, default_map[i].gpio_id, 1);
				gpio_hal_input_enable(hal, default_map[i].gpio_id, 0);
				//NOTES:special combine use it with pull mode
				if(default_map[i].pull_mode == GPIO_PULL_DOWN_EN)
					gpio_hal_set_output_value(hal, default_map[i].gpio_id, 0);
				if(default_map[i].pull_mode == GPIO_PULL_UP_EN)
					gpio_hal_set_output_value(hal, default_map[i].gpio_id, 1);
				break;
			case GPIO_INPUT_ENABLE:
				gpio_hal_output_enable(hal, default_map[i].gpio_id, 0);
				gpio_hal_input_enable(hal, default_map[i].gpio_id, 1);
				break;
			default:
				break;
		}

		//pull mode
		switch(default_map[i].pull_mode)
		{
			case GPIO_PULL_DISABLE:
				gpio_hal_pull_enable(hal, default_map[i].gpio_id, 0);
				break;
			case GPIO_PULL_DOWN_EN:
				gpio_hal_pull_enable(hal, default_map[i].gpio_id, 1);
				gpio_hal_pull_up_enable(hal, default_map[i].gpio_id, 0);
				break;
			case GPIO_PULL_UP_EN:
				gpio_hal_pull_enable(hal, default_map[i].gpio_id, 1);
				gpio_hal_pull_up_enable(hal, default_map[i].gpio_id, 1);
				break;
			default:
				break;
		}

		//interrupt
		if(default_map[i].int_en) {
			gpio_hal_disable_interrupt(hal, default_map[i].gpio_id);	//disable it first to avoid enable IRQ and comes an interrupt at once.
			gpio_hal_set_int_type(hal, default_map[i].gpio_id, default_map[i].int_type);
			gpio_hal_enable_interrupt(hal, default_map[i].gpio_id);
		} else
			gpio_hal_disable_interrupt(hal, default_map[i].gpio_id);

		//driver_capacity
		gpio_hal_set_capacity(hal, default_map[i].gpio_id, default_map[i].driver_capacity);
	}

	/* After disable interrupt,and then clear int status, to avoid level-interrupt comes again
	 * if clear interrupt status before disable interrupt.
	 */
	gpio_hal_get_interrupt_status(hal, &gpio_status);
	gpio_hal_clear_interrupt_status(hal, &gpio_status);

	return BK_OK;
}
#endif
