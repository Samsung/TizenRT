// Copyright 2021-2025 Beken
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


#include "gpio_map.h"
#include "gpio_driver.h"
#include <driver/gpio.h>
#include <driver/uart.h>
#include "bk_sys_ctrl.h"
#include "bk_rf_internal.h"

#if defined(CONFIG_ATE)

//if modify to 1, should change the value of gpio_cfg to {GPIO_INPUT_ENABLE, GPIO_PULL_DOWN_EN} and confirm HW change the GPIO default output level.
#define ATE_ENABLE_GPIO_LEVEL (0)

static bool s_ate_enabled = false;

//NOTICE:Now we re-use the GPIO which is used by UART.
static gpio_id_t ate_get_gpio_id(void)
{
	return bk_uart_get_ate_detect_gpio();
}

static bk_err_t ate_gpio_init(void)
{
	bk_err_t ret = BK_ERR_BUSY;
	bool gpio_value = 0;
	gpio_id_t gpio_id;
	const gpio_config_t gpio_cfg = {GPIO_INPUT_ENABLE, GPIO_PULL_UP_EN};
	uint32_t gpio_cfg_v;

	gpio_id = ate_get_gpio_id();

	gpio_cfg_v = bk_gpio_get_value(gpio_id);
	//bootrom or bootloader may have inited this GPIO, so release it firstly.
	gpio_dev_unmap(gpio_id);

	//set to input and check the GPIO input level
	ret = bk_gpio_set_config(gpio_id, &gpio_cfg);
	if(ret == BK_OK)
	{
		gpio_value = bk_gpio_get_input(gpio_id);
		if(gpio_value == ATE_ENABLE_GPIO_LEVEL) {
			#if defined(CONFIG_SHELL_ASYNCLOG)
			shell_set_uart_port(CONFIG_UART_ATE_PRINT_PORT);
			#else
			bk_set_printf_port(CONFIG_UART_ATE_PRINT_PORT);
			#endif
			s_ate_enabled = true;
		}
	}
#if defined(CONFIG_ATE_TEST)
	s_ate_enabled = true;
#endif
	if(s_ate_enabled == true)
	{
	    //rf_ps_enable_clear();
	    //remove code below for bringup
	    //rf_module_vote_ctrl(RF_OPEN,RF_BY_ATE_WIFI_BIT);
	}

	bk_gpio_set_value(gpio_id, gpio_cfg_v);

	return ret;
}

__IRAM_SEC bool ate_is_enabled(void)
{
#if defined(CONFIG_RF_TEST_ENABLE)
    return true;
#elif defined(CONFIG_RF_TEST_SWITCH)
    return s_ate_enabled;
#else
    return false;
#endif
}

bool cp_is_enabled(void)
{
    #if defined(CONFIG_ATE_TEST)
    return true;
    #else
    return false;
    #endif
}

int bk_ate_ctrl(bool enable)
{
	s_ate_enabled = enable;
	return BK_OK;
}
//This function should be called before UART init, or caused UART can't work.
int bk_ate_init(void)
{
	ate_gpio_init();
	return BK_OK;
}

int bk_ate_deinit(void)
{
	return BK_OK;
}
#else
bool ate_is_enabled(void)
{
	return false;
}
#endif


