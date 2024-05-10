/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/lcd/st7789.h>
#include <debug.h>
#include "objects.h"
#include "PinNames.h"
#include "gpio_api.h"
#include "pwmout_api.h"

#ifndef CONFIG_LCD_ST7789_SPI_PORT
#define CONFIG_LCD_ST7789_SPI_PORT 1
#endif

#if CONFIG_RTL8730E_BOARD_REVISION >= 5
#error "ERROR, Not supported this revision"
#else
#define GPIO_PIN_RESET 		PB_11
#define GPIO_PIN_BACKLIGHT 	PA_9
#define GPIO_PIN_CMDDATA 	PA_10
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct rtl8730e_st7789_info_s {
        struct st7789_config_s lcd_config;
        pwmout_t pwm_led;
};


static int rtl8730_st7789_gpio_reset(void);
static int rtl8730_st7789_gpio_cmddata(uint8_t cmd);
static int rtl8730_st7789_control_backlight(uint8_t level);

struct rtl8730e_st7789_info_s g_rtl8730_config_dev_s = {
	.lcd_config = {
		.reset = rtl8730_st7789_gpio_reset,
		.cmddata = rtl8730_st7789_gpio_cmddata,
		.backlight = rtl8730_st7789_control_backlight,
	},
};

static int rtl8730_st7789_gpio_reset(void)
{
	GPIO_WriteBit(GPIO_PIN_RESET, 1);
	DelayMs(10);
	GPIO_WriteBit(GPIO_PIN_RESET, 0);
	DelayMs(10);
	GPIO_WriteBit(GPIO_PIN_RESET, 1);
	DelayMs(10);
	return OK;
}

static int rtl8730_st7789_gpio_cmddata(uint8_t cmd)	// cmd - 0, data - 1
{
	GPIO_WriteBit(GPIO_PIN_CMDDATA, cmd);
	return OK;
}

static int rtl8730_st7789_control_backlight(uint8_t level)	// range  0~100 percentage of brightness level
{
	float pwm_level = level/100.0;
	pwmout_write(&g_rtl8730_config_dev_s.pwm_led, pwm_level);
	return OK;
}

static void gpio_pins_init(void)
{
	GPIO_InitTypeDef ResetPin;
	GPIO_InitTypeDef CmdPin;

	Pinmux_Swdoff();
	Pinmux_Config(GPIO_PIN_RESET, PINMUX_FUNCTION_GPIO);

	ResetPin.GPIO_Pin = GPIO_PIN_RESET;
	ResetPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ResetPin.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(&ResetPin);

	Pinmux_Config(GPIO_PIN_CMDDATA, PINMUX_FUNCTION_GPIO);
	CmdPin.GPIO_Pin = GPIO_PIN_CMDDATA;
	CmdPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
	CmdPin.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(&CmdPin);

	pwmout_init(&g_rtl8730_config_dev_s.pwm_led, GPIO_PIN_BACKLIGHT);

}

void rtl8730_st7789_initialize(void)
{
	gpio_pins_init();

	FAR struct spi_dev_s *spi = up_spiinitialize(CONFIG_LCD_ST7789_SPI_PORT);
	struct lcd_dev_s *dev = st7789_lcdinitialize(spi, &g_rtl8730_config_dev_s.lcd_config);
	if (lcddev_register(dev) < 0) {
		lcddbg("ERROR: LCD driver register fail\n");
	} else {
		lcddbg("LCD driver register success\n");
	}
}

