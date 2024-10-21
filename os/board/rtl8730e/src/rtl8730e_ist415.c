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
#include <stdbool.h>
#include <stdio.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>
#include <tinyara/irq.h>
#include <tinyara/gpio.h>
#include <tinyara/input/touchscreen.h>
#include <tinyara/input/ist415.h>
#include "objects.h"
#include "gpio_irq_api.h"
#include "PinNames.h"
#include "gpio_api.h"

#define PIN_LOW		0
#define PIN_HIGH	1
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* i2c config */
#if CONFIG_RTL8730E_BOARD_REVISION >= 6
#define IST415_I2C_PORT		0
#else
#define IST415_I2C_PORT		1
#endif

#define IST415_I2C_FREQ		100000
#define IST415_I2C_ADDRLEN	7
#define IST415_I2C_ADDR		(0xA0 >> 1) 

/* pin config */
#define IST415_GPIO_RESET_PIN	PA_5
#if CONFIG_RTL8730E_BOARD_REVISION >= 6
#define IST415_GPIO_I2C_PIN		PA_4
#else
#define IST415_GPIO_I2C_PIN		PA_2
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct rtl8730e_ist415_s {
	gpio_irq_t data_ready;
};

struct rtl8730e_ist415_s g_rtl8730e_ist415_priv0;
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static void rtl8730e_ist415_enable_irq(void);
static void rtl8730e_ist415_disable_irq(void);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct ist415_dev_s g_ist415_dev0 = {
	.i2c = NULL,
	.i2c_config = {
		.frequency = IST415_I2C_FREQ,
		.address = IST415_I2C_ADDR,
		.addrlen = IST415_I2C_ADDRLEN,
	},
	.int_pending = false,
	.ops = &(struct ist415_ops_s){
		.irq_enable = rtl8730e_ist415_enable_irq,
		.irq_disable = rtl8730e_ist415_disable_irq,
	},
	.priv = &g_rtl8730e_ist415_priv0,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void rtl8730e_ist415_irq_handler(uint32_t id, gpio_irq_event event)
{
	/* we are using level based interrupt trigger here,
	 * so, we have to disable this particular gpio interrupt
	 * until we finish this particular interrupt related work
	 * in the HPWORK thread
	 */
	struct ist415_dev_s *dev;
	if (id == 0) {
		dev =  &g_ist415_dev0;
	}
	struct rtl8730e_ist415_s *priv = dev->priv;

	if (dev->handler != NULL) {
		dev->handler(dev);
	}
}

static void rtl8730e_ist415_enable_irq(void)
{
	gpio_irq_enable(&g_rtl8730e_ist415_priv0.data_ready);
}

static void rtl8730e_ist415_disable_irq(void)
{
	gpio_irq_disable(&g_rtl8730e_ist415_priv0.data_ready);
}

static void rtl8730e_ist415_gpio_reset(void)
{
	GPIO_WriteBit(IST415_GPIO_RESET_PIN, PIN_LOW);
	DelayMs(300);
	GPIO_WriteBit(IST415_GPIO_RESET_PIN, PIN_HIGH);
}

static void rtl8730e_ist415_gpio_init(void)
{
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

/****************************************************************************
 * Name: rtl8730e_ist415_initialize
 *
 * Description:
 *   This function is called by boot, setup logic to configure
 *   and register the IST415 Touch device.  This function will register the driver
 *   as /dev/touchN where N is determined by the minor device number.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void rtl8730e_ist415_initialize(void)
{
	FAR struct i2c_dev_s *i2c;

	rtl8730e_ist415_gpio_init();
	rtl8730e_ist415_gpio_reset();
	i2c = up_i2cinitialize(IST415_I2C_PORT);
	if (!i2c) {
		touchdbg("ERROR: Failed to initialize I2C\n");
		return;
	}
	g_ist415_dev0.i2c = i2c;
	gpio_irq_init(&g_rtl8730e_ist415_priv0.data_ready, IST415_GPIO_I2C_PIN, rtl8730e_ist415_irq_handler, (uint32_t)0);
	gpio_irq_set(&g_rtl8730e_ist415_priv0.data_ready, IRQ_FALL_RISE, 1);
	gpio_irq_enable(&g_rtl8730e_ist415_priv0.data_ready);

	int ret= ist415_initialize(TOUCH_DEV_PATH, &g_ist415_dev0);

	if (ret < 0) {
		touchdbg("ERROR: Touch driver register fail\n");
		return;
	}
	touchvdbg("Touch driver register success\n");
}
