/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/s5j/s5j_ppd42ns.c
 *
 *   Copyright (C) 2009-2010, 2014-2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/irq.h>
#include <tinyara/sensors/ppd42ns.h>
#include <debug.h>
#include <errno.h>
#include "s5j_gpio.h"

#ifdef CONFIG_SENSOR_PPD42NS

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define DEVNAME_FORMAT	"/dev/dust%d"

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct s5j_gpio_info_s {
	uint8_t pinnum;
	uint16_t pincfg;
};

struct s5j_ppd42ns_priv_s {
	uint32_t irqvector;
	uint32_t pincfg;
	int event_on_rising;
	int event_on_falling;
};

struct s5j_ppd42ns_dev_s {
	/* Configuration structure as seen by the ppd42ns driver */
	struct ppd42ns_config_s config;

	/* Additional private definitions only known to this driver */
	ppd42ns_handler_t handler;
	FAR void *arg;
};

/****************************************************************************
 * Private Functions Prototype
 ****************************************************************************/
static int s5j_ppd42ns_gpio_read(struct ppd42ns_config_s *config);
static int s5j_ppd42ns_irq_attach(struct ppd42ns_config_s *config, ppd42ns_handler_t handler, FAR char *arg);
static int s5j_ppd42ns_irq_enable(struct ppd42ns_config_s *config, int enable);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct s5j_ppd42ns_dev_s g_s5j_dev = {
	.config = {
		.gpionum = CONFIG_SENSOR_PPD42NS_GPIO_NUM,
		.read_gpio = s5j_ppd42ns_gpio_read,
		.attach = s5j_ppd42ns_irq_attach,
		.enable = s5j_ppd42ns_irq_enable,
		.priv = NULL,
	},

	.handler = NULL
};

static struct s5j_gpio_info_s g_gpio_info[] = {
	{57, GPIO_INPUT | GPIO_PULLDOWN | GPIO_PORTA0 | GPIO_PIN0},	/* XEINT0 */
	{58, GPIO_INPUT | GPIO_PULLDOWN | GPIO_PORTA0 | GPIO_PIN1},	/* XEINT1 */
	{59, GPIO_INPUT | GPIO_PULLDOWN | GPIO_PORTA0 | GPIO_PIN2},	/* XEINT2 */
};

static struct s5j_ppd42ns_priv_s g_s5j_priv;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int s5j_ppd42ns_gpio_read(struct ppd42ns_config_s *config)
{
	struct s5j_ppd42ns_dev_s *dev = (struct s5j_ppd42ns_dev_s *)config;
	struct s5j_ppd42ns_priv_s *priv = dev ? (struct s5j_ppd42ns_priv_s *)dev->config.priv : NULL;

	if (priv == NULL) {
		lldbg("ERROR: s5j_ppd42ns_gpio_read() is failed.\n");
		return -1;
	}

	return s5j_gpioread(priv->pincfg) ? 1 : 0;
}

static int s5j_ppd42ns_irq_handler(int irq, FAR void *context, FAR void *arg)
{
	struct s5j_ppd42ns_dev_s *dev = (struct s5j_ppd42ns_dev_s *)arg;
	struct s5j_ppd42ns_priv_s *priv = dev ? (struct s5j_ppd42ns_priv_s *)dev->config.priv : NULL;

	if (priv == NULL) {
		return -1;
	}

	s5j_gpio_clear_pending(priv->pincfg);

	if (dev->handler) {
		dev->handler(dev->arg);
	}

	return 0;
}

static int s5j_ppd42ns_irq_attach(struct ppd42ns_config_s *config, ppd42ns_handler_t handler, FAR char *arg)
{
	struct s5j_ppd42ns_dev_s *dev = (struct s5j_ppd42ns_dev_s *)config;
	struct s5j_ppd42ns_priv_s *priv = dev ? (struct s5j_ppd42ns_priv_s *)dev->config.priv : NULL;

	if (priv == NULL) {
		lldbg("ERROR: s5j_ppd42ns_irq_attach() is failed.\n");
		return -1;
	}

	dev->handler = handler;
	dev->arg = arg;

	return 0;
}

static int s5j_ppd42ns_irq_enable(struct ppd42ns_config_s *config, int enable)
{
	int result = -1;
	struct s5j_ppd42ns_dev_s *dev = (struct s5j_ppd42ns_dev_s *)config;
	struct s5j_ppd42ns_priv_s *priv = dev ? (struct s5j_ppd42ns_priv_s *)dev->config.priv : NULL;
	uint32_t pincfg;
	irqstate_t flags;

	if (priv == NULL) {
		lldbg("ERROR: s5j_ppd42ns_irq_enable() is failed.\n");
		return -1;
	}

	/* disable interrupt */
	flags = irqsave();

	pincfg = priv->pincfg;
	pincfg &= ~GPIO_FUNC_MASK;
	if (priv->event_on_rising && priv->event_on_falling) {
		pincfg |= GPIO_EINT | GPIO_EINT_BOTH_EDGE;
	} else if (priv->event_on_rising) {
		pincfg |= GPIO_EINT | GPIO_EINT_RISING_EDGE;
	} else if (priv->event_on_falling) {
		pincfg |= GPIO_EINT | GPIO_EINT_FALLING_EDGE;
	}

	if (enable) {
		irq_attach(priv->irqvector, s5j_ppd42ns_irq_handler, dev);
		up_enable_irq(priv->irqvector);

	} else {
		up_disable_irq(priv->irqvector);
		irq_detach(priv->irqvector);
	}

	if (s5j_configgpio(pincfg) != 0) {
		goto done;
	}

	result = 0;

done:
	/* enable interrupt */
	irqrestore(flags);

	return result;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_ppd42ns_initialize
 *
 * Description:
 *   Initialize the ppd42ns dust sensor driver.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno on failure
 *
 ****************************************************************************/
int s5j_ppd42ns_initialize(void)
{
	int result = -1;
	int i;
	int total_pins = sizeof(g_gpio_info) / sizeof(g_gpio_info[0]);
	char devpath[32];

	for (i = 0; i < total_pins; i++) {
		if (g_gpio_info[i].pinnum == CONFIG_SENSOR_PPD42NS_GPIO_NUM) {
			g_s5j_priv.pincfg = g_gpio_info[i].pincfg;
			g_s5j_priv.irqvector = s5j_gpio_irqvector(g_s5j_priv.pincfg);
			break;
		}
	}
	if (i == total_pins) {
		result = -EINVAL;
		goto done;
	}
	/* set both edge */
	g_s5j_priv.event_on_rising = true;
	g_s5j_priv.event_on_falling = true;

	/* set ppd42ns config */
	g_s5j_dev.config.priv = (void *)&g_s5j_priv;

	/* register ppd42ns driver */
	snprintf(devpath, sizeof(devpath), DEVNAME_FORMAT, 0);
	if (ppd42ns_register(devpath, &g_s5j_dev.config) != 0) {
		goto done;
	}

	/* result is success */
	result = 0;

done:
	return result;
}

#endif							/* CONFIG_SENSOR_PPD42NS */
