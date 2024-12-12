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
	struct i2c_dev_s *i2c; /* Workaround: IC20 write hang issue */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static void rtl8730e_ist415_enable_irq(struct ist415_config_s *dev);
static void rtl8730e_ist415_disable_irq(struct ist415_config_s *dev);
static void rtl8730e_ist415_power_off(struct ist415_config_s *dev);
static void rtl8730e_ist415_power_on(struct ist415_config_s *dev);

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct rtl8730e_ist415_s g_rtl8730e_ist415_priv0;

struct ist415_ops_s g_rtl8730e_ist415_ops = {
	.irq_enable = rtl8730e_ist415_enable_irq,
	.irq_disable = rtl8730e_ist415_disable_irq,
	.power_off = rtl8730e_ist415_power_off,
	.power_on = rtl8730e_ist415_power_on,
};

struct ist415_config_s g_rtl8730e_ist415_0 = {
	.ops = &g_rtl8730e_ist415_ops,
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
	struct ist415_config_s *dev;
	if (id == 0) {
		dev =  &g_rtl8730e_ist415_0;
	}

	if (dev->handler != NULL) {
		dev->handler(dev);
	}
}

static void rtl8730e_ist415_enable_irq(struct ist415_config_s *dev)
{
	struct rtl8730e_ist415_s *priv = (struct rtl8730e_ist415_s *)dev->priv;
	gpio_irq_enable(&priv->data_ready);
}

static void rtl8730e_ist415_disable_irq(struct ist415_config_s *dev)
{
	struct rtl8730e_ist415_s *priv = (struct rtl8730e_ist415_s *)dev->priv;
	gpio_irq_disable(&priv->data_ready);
}

static void rtl8730e_ist415_power_off(struct ist415_config_s *dev)
{
	struct rtl8730e_ist415_s *priv = (struct rtl8730e_ist415_s *)dev->priv;
	up_i2cuninitialize(priv->i2c);	/* Workaround: IC20 write hang issue */
	GPIO_WriteBit(IST415_GPIO_RESET_PIN, PIN_LOW);
}

static void rtl8730e_ist415_power_on(struct ist415_config_s *dev)
{
	struct rtl8730e_ist415_s *priv = (struct rtl8730e_ist415_s *)dev->priv;
	GPIO_WriteBit(IST415_GPIO_RESET_PIN, PIN_HIGH);
	DelayMs(1);  /* Wait for stable voltage before i2c commands issued */
	priv->i2c = up_i2cinitialize(IST415_I2C_PORT);	/* Workaround: IC20 write hang issue */
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
	struct i2c_dev_s *i2c;
	struct ist415_config_s *dev = &g_rtl8730e_ist415_0;
	struct rtl8730e_ist415_s *priv = dev->priv;

	rtl8730e_ist415_gpio_init();
	
	i2c = up_i2cinitialize(IST415_I2C_PORT);
	if (!i2c) {
		touchdbg("ERROR: Failed to initialize I2C\n");
		return;
	}
	priv->i2c = i2c; /* Workaround: IC20 write hang issue */

	gpio_irq_init(&priv->data_ready, IST415_GPIO_I2C_PIN, rtl8730e_ist415_irq_handler, (uint32_t)0);
	gpio_irq_set(&priv->data_ready, IRQ_FALL, 1);

	if (ist415_initialize(TOUCH_DEV_PATH, i2c, dev) < 0) {
		touchdbg("ERROR: Touch driver register fail\n");
		up_i2cuninitialize(i2c);
	}
	touchdbg("Touch driver register success\n");
}
