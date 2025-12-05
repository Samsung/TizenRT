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
#include <driver/gpio.h>
#include <driver/gpio_types.h>
#include "gpio_driver.h"
#include "gpio_clk_pin.h"
#include "gpio_map.h"
#include "sys_hal.h"

static const gpio_clk_pin_map_t gpio_clk_pin_map_table[] = GPIO_CLK_PIN_MAP;

#define GPIO_RETURN_ON_CLK_PIN_INVALID(src, port, op) do {\
		if ((src) >= GPIO_CLK_PIN_SRC_INVALID || (port) >= GPIO_CLK_PIN_PORT_INVALID || (op) >= GPIO_CLK_PIN_OP_INVALID) {\
			return BK_ERR_GPIO_CHAN_ID;\
		}\
	} while(0)

#if (defined(CONFIG_SUPPORT_IO_MATRIX) && defined(CONFIG_IO_MATRIX_VER2_0))
#define GPIO_CFG_VALUE_HIZ 0x8
#else
#define GPIO_CFG_VALUE_HIZ 0x0
#endif
#define GPIO_INVALID_VALUE 0xFFFFFFFF


static bk_err_t gpio_clk_pin_open(gpio_clk_pin_src_t src, gpio_clk_pin_port_t port, clk_pin_div_t div, gpio_id_t io_port, bool en);
#if GPIO_CLK_PIN_GPIO_NEED_RECOVER
static uint32_t gpio_reg_value[GPIO_NUM_MAX] = {
	[0 ... (GPIO_NUM_MAX - 1)] = GPIO_INVALID_VALUE
};
#endif

bk_err_t bk_gpio_clk_pin_ctrl(gpio_clk_pin_src_t src, gpio_clk_pin_port_t port, gpio_clk_pin_op_t opt)
{
	GPIO_RETURN_ON_CLK_PIN_INVALID(src, port, opt.op);

	bk_err_t ret = BK_OK;
	bool open_en = false;
	clk_pin_div_t clk_div = 0;
	gpio_id_t io_port = SOC_GPIO_NUM;

	switch (opt.op)
	{
	case GPIO_CLK_PIN_OPEN:
		open_en = true;
	case GPIO_CLK_PIN_CLOSE:
		if (opt.args)
		{
			uint32_t *params = opt.args;
			clk_div = params[0];
			io_port = params[1];
		}
		gpio_clk_pin_open(src, port, clk_div, io_port, open_en);
		break;

	default:
		GPIO_LOGW("clk pin op not recongnized %d\r\n", opt.op);
		ret = BK_FAIL;
		break;
	}

	return ret;
}

static void gpio_clk_pin_gpio_backup(gpio_id_t id)
{
#if GPIO_CLK_PIN_GPIO_NEED_RECOVER
	if (gpio_reg_value[id] != GPIO_INVALID_VALUE)
	{
		GPIO_LOGW("gpio %d has value backuped already: 0x%x\r\n", id, gpio_reg_value[id]);
	}
	else
	{
		gpio_reg_value[id] = bk_gpio_get_value(id);
		GPIO_LOGD("gpio %d back up success: 0x%x\r\n", id, gpio_reg_value[id]);
	}
#endif
}

static void gpio_clk_pin_gpio_restore(gpio_id_t id)
{
#if GPIO_CLK_PIN_GPIO_NEED_RECOVER
	if (gpio_reg_value[id] == GPIO_INVALID_VALUE)
	{
		GPIO_LOGW("gpio %d do not have backup value\r\n", id);
	}
	else
	{
		bk_gpio_set_value(id, gpio_reg_value[id]);
		GPIO_LOGD("gpio %d restore success: 0x%x\r\n", id, gpio_reg_value[id]);
		gpio_reg_value[id] = GPIO_INVALID_VALUE;
	}
#endif
}

static bk_err_t gpio_clk_pin_open(gpio_clk_pin_src_t src, gpio_clk_pin_port_t port, clk_pin_div_t div, gpio_id_t io_port, bool en)
{
	bk_err_t ret = BK_OK;
	gpio_id_t id;
	gpio_dev_t dev;
	clk_pin_ana_src_t ana_src;
	clk_pin_dig_src_t dig_src;

	switch (port)
	{
	case GPIO_CLK_PIN_PORT_ANA:
		// get from ana_map
		id = gpio_clk_pin_map_table[src].ana_map.gpio_id;
		ana_src = gpio_clk_pin_map_table[src].ana_map.ana_src;
		// config gpio as hz mode
		if (en)
		{
			gpio_clk_pin_gpio_backup(id);
			bk_gpio_set_value(id, GPIO_CFG_VALUE_HIZ);
			GPIO_LOGI("set gpio %d as ana clk pin %d\r\n", id, ana_src);
		}
		else
		{
			gpio_clk_pin_gpio_restore(id);
		}
		// call sys_hal to output the clock
		sys_hal_clk_pin_ana_open(ana_src, div, en);
		break;
	case GPIO_CLK_PIN_PORT_DIG:
		// get from dig_map
		id = gpio_clk_pin_map_table[src].dig_map.gpio_id;
		dev = gpio_clk_pin_map_table[src].dig_map.gpio_dev;
		dig_src = gpio_clk_pin_map_table[src].dig_map.dig_src;

		// check if io port is specified
		if (io_port < SOC_GPIO_NUM)
		{
			id = io_port;
		}

		// config gpio
		if (en)
		{
			gpio_clk_pin_gpio_backup(id);
			bk_gpio_set_value(id, GPIO_CFG_VALUE_HIZ);
			gpio_dev_map(id, dev);
			GPIO_LOGI("set gpio %d as dig clk pin %d\r\n", id, dig_src);
		}
		else
		{
			gpio_clk_pin_gpio_restore(id);
		}
		// call sys_hal to output the clock
		sys_hal_clk_pin_dig_open(dig_src, div, en);
		break;

	default:
		GPIO_LOGW("clk pin port not recongnized %d\r\n", port);
		ret = BK_FAIL;
		break;
	}

	return ret;
}