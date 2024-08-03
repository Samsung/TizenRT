/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#include <stdbool.h>
#include <stdio.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/irq.h>
#include <tinyara/gpio.h>
#include <tinyara/input/touchscreen.h>
#include "objects.h"
#include "gpio_irq_api.h"
#include "PinNames.h"
#include "gpio_api.h"

#define PIN_LOW 0
#define PIN_HIGH 1

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* i2c config */
#if CONFIG_RTL8730E_BOARD_REVISION >= 6
#define IST415_I2C_PORT 0
#else
#define IST415_I2C_PORT 1
#endif

#define IST415_I2C_FREQ		100000
#define IST415_I2C_ADDRLEN		7

#define IST415_I2C_ADDR		(0xA0 >> 1) 

/*other pin config */
#define IST415_GPIO_RESET_PIN		PA_5
#if CONFIG_RTL8730E_BOARD_REVISION >= 6
#define IST415_GPIO_I2C_PIN PA_4
#else
#define IST415_GPIO_I2C_PIN PA_2
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct rtl8730e_ist415_touchinfo_s {
	struct lcd_touch_config touch_config;
	touch_handler_t handler;
	gpio_irq_t data_ready;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static void rtl8730e_ist415_enable_irq(bool enable);
static void rtl8730e_ist415_irq_attach(touch_handler_t handler, FAR char *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct rtl8730e_ist415_touchinfo_s g_ist415info = {
	.touch_config = {
		.i2c_config = {
			.frequency = IST415_I2C_FREQ,
			.address = IST415_I2C_ADDR,
			.addrlen = IST415_I2C_ADDRLEN,
		},
		.attach = rtl8730e_ist415_irq_attach,
		.irq_enable = rtl8730e_ist415_enable_irq,
	},
	.handler = NULL,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int flagg = 0;
static clock_t prev;

static void rtl8730e_ist415_irq_handler(uint32_t id, gpio_irq_event event)
{
	/* we are using level based interrupt trigger here,
	 * so, we have to disable this particular gpio interrupt
	 * until we finish this particular interrupt related work
	 * in the HPWORK thread
	 */
	gpio_irq_disable(&g_ist415info.data_ready);

	if (g_ist415info.handler != NULL) {
		g_ist415info.handler(id);
	} else {
		gpio_irq_enable(&g_ist415info.data_ready);
	}
}

static void rtl8730e_ist415_enable_irq(bool enable)
{
	if (enable) {
		gpio_irq_enable(&g_ist415info.data_ready);
	} else {
		gpio_irq_disable(&g_ist415info.data_ready);
	}
}

static void rtl8730e_ist415_irq_attach(touch_handler_t handler, FAR char *arg)
{
	g_ist415info.handler = handler;
	gpio_irq_init(&g_ist415info.data_ready, IST415_GPIO_I2C_PIN, rtl8730e_ist415_irq_handler, arg);
	gpio_irq_set(&g_ist415info.data_ready, IRQ_FALL_RISE, 1);
	gpio_irq_enable(&g_ist415info.data_ready);
}

static void ist415_gpio_reset()
{
	GPIO_WriteBit(IST415_GPIO_RESET_PIN, PIN_HIGH);
        DelayMs(500);
        GPIO_WriteBit(IST415_GPIO_RESET_PIN, PIN_LOW);
        DelayMs(500);
        GPIO_WriteBit(IST415_GPIO_RESET_PIN, PIN_HIGH);
        DelayMs(500);

	return;
}

static void ist415_gpio_init()
{
	//GPIO_INTConfig(IST415_GPIO_I2C_PIN, DISABLE);
        /*Pinmux_Config(IST415_GPIO_I2C_PIN, PINMUX_FUNCTION_GPIO);
        GPIO_InitTypeDef TouchPin;
        TouchPin.GPIO_Pin = IST415_GPIO_I2C_PIN;
        TouchPin.GPIO_PuPd = GPIO_PuPd_UP;
        TouchPin.GPIO_Mode = GPIO_Mode_INT;
        TouchPin.GPIO_ITTrigger= GPIO_INT_Trigger_LEVEL;
        TouchPin.GPIO_ITPolarity = GPIO_PIN_LOW;

	InterruptRegister(GPIO_INTHandler, GPIOA_IRQ, (u32)GPIOA_BASE, 5);
        InterruptEn(GPIOA_IRQ, 5);
	//GPIO_UserRegIrq(IST415_GPIO_I2C_PIN, (VOID *)app_check_i2c_handler, NULL);
        GPIO_INTMode(IST415_GPIO_I2C_PIN, ENABLE, GPIO_INT_Trigger_LEVEL, GPIO_PIN_LOW, GPIO_INT_DEBOUNCE_DISABLE);
        //GPIO_INTConfig(IST415_GPIO_I2C_PIN, ENABLE);
	*/
        Pinmux_Config(IST415_GPIO_RESET_PIN, PINMUX_FUNCTION_GPIO);
        GPIO_InitTypeDef TouchResetPin;
        TouchResetPin.GPIO_Pin = IST415_GPIO_RESET_PIN;
        TouchResetPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
        TouchResetPin.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_Init(&TouchResetPin);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void rtl8730e_ist415_initialize()
{
	//FAR struct touch_dev_s *dev;
	FAR struct i2c_dev_s *i2c;
	
	ist415_gpio_init();
	ist415_gpio_reset();
	i2c = up_i2cinitialize(IST415_I2C_PORT);
	if (!i2c) {
		lldbg("ERROR: Failed to initialize I2C\n");
	}
	uint8_t reg[3];
	reg[0] = 0x60;
	int ret = i2c_write(i2c, &(g_ist415info.touch_config.i2c_config), (uint8_t *)reg, 1);

	ret = ist415_register("/dev/input0", i2c, &(g_ist415info.touch_config));
	//int ret = touch_register(dev);
	if (ret < 0) {
		lldbg("ERROR: Touch driver register fail\n");
		return;
	}
	
	lldbg("Touch driver regsiter success\n");
}
