/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 ******************************************************************/
/****************************************************************************
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

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>

#include <arch/irq.h>
#include <tinyara/gpio.h>
#include <tinyara/kmalloc.h>

#include "esp32_gpio.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct esp32_lowerhalf_s {
	FAR const struct gpio_ops_s *ops;
	struct gpio_upperhalf_s *parent;

	int pinnum;
	uint32_t pincfg;
	gpio_handler_t handler;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#ifndef CONFIG_DISABLE_POLL
static int esp32_gpio_interrupt(int irq, FAR void *context, FAR void *arg)
{
	struct esp32_lowerhalf_s *lower = (struct esp32_lowerhalf_s *)arg;

	esp32_gpio_clear_pending(lower->pincfg);

	if (lower->handler != NULL) {
		DEBUGASSERT(lower->handler != NULL);
		lower->handler(lower->parent);
	}

	return OK;
}
#endif

/****************************************************************************
 * Name: esp32_gpio_get
 *
 * Description:
 *   Get GPIO value
 *
 * Input Parameters:
 *   lower - lowerhalf GPIO driver
 *
 * Returned Value:
 *   >= 0: gpio value
 *   == -EINVAL: invalid gpio
 ****************************************************************************/
static int esp32_gpio_get(FAR struct gpio_lowerhalf_s *lower)
{
	struct esp32_lowerhalf_s *priv = (struct esp32_lowerhalf_s *)lower;

	return esp32_gpioread(priv->pinnum);
}

/****************************************************************************
 * Name: esp32_gpio_set
 *
 * Description:
 *   Set GPIO value
 *
 * Input Parameters:
 *   lower - lowerhalf GPIO driver
 *   value - value to set
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void esp32_gpio_set(FAR struct gpio_lowerhalf_s *lower, FAR unsigned int value)
{
	struct esp32_lowerhalf_s *priv = (struct esp32_lowerhalf_s *)lower;

	esp32_gpiowrite(priv->pinnum, value);
}

static int esp32_gpio_setdir(FAR struct gpio_lowerhalf_s *lower, unsigned long arg)
{
	struct esp32_lowerhalf_s *priv = (struct esp32_lowerhalf_s *)lower;

	priv->pincfg &= ~FUNCTION_MASK;

	if (arg == GPIO_DIRECTION_OUT) {
		priv->pincfg &= ~INPUT;
		priv->pincfg |= OUTPUT;
	} else {
		priv->pincfg &= ~OUTPUT;
		priv->pincfg |= INPUT;
	}

	return esp32_configgpio(priv->pinnum, priv->pincfg);
}

static int esp32_gpio_pull(FAR struct gpio_lowerhalf_s *lower, unsigned long arg)
{
	struct esp32_lowerhalf_s *priv = (struct esp32_lowerhalf_s *)lower;

	priv->pincfg &= ~PUPD_MASK;
	if (arg == GPIO_DRIVE_PULLUP) {
		priv->pincfg |= PULLUP;
	} else if (arg == GPIO_DRIVE_PULLDOWN) {
		priv->pincfg |= PULLDOWN;
	} else {
		return -EINVAL;
	}

	return esp32_configgpio(priv->pinnum, priv->pincfg);
}

static int esp32_gpio_enable(FAR struct gpio_lowerhalf_s *lower, int falling, int rising, gpio_handler_t handler)
{
#if 0
	int irqvector;
	struct esp32_lowerhalf_s *priv = (struct esp32_lowerhalf_s *)lower;

	irqvector = esp32_gpio_irqvector(priv->pincfg);
	if (!irqvector) {
		return -EINVAL;
	}

	/* clear function mask */
	priv->pincfg &= ~FUNC_MASK;

	if (falling && rising) {
		priv->pincfg |= GPIO_EINT | GPIO_EINT_BOTH_EDGE;
	} else if (falling) {
		priv->pincfg |= GPIO_EINT | GPIO_EINT_FALLING_EDGE;
	} else if (rising) {
		priv->pincfg |= GPIO_EINT | GPIO_EINT_RISING_EDGE;
	} else {
		priv->pincfg |= GPIO_INPUT;
		handler = NULL;
	}

	priv->handler = handler;
	if (handler) {
		irq_attach(irqvector, esp32_gpio_interrupt, priv);
		up_enable_irq(irqvector);
	} else {
		up_disable_irq(irqvector);
		irq_detach(irqvector);
	}

	return esp32_configgpio(priv->pincfg);
#endif
	return 0;
}

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct gpio_ops_s esp32_gpio_ops = {
	.set = esp32_gpio_set,
	.get = esp32_gpio_get,
	.pull = esp32_gpio_pull,
	.setdir = esp32_gpio_setdir,
	.enable = esp32_gpio_enable,

};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: esp32_gpio_lowerhalf
 *
 * Description:
 *   Instantiate the GPIO lower half driver for the S5J.
 *   General usage:
 *
 *     #include <tinyara/gpio.h>
 *     #include "esp32_gpio.h"
 *
 *     struct gpio_lowerhalf_s *lower;
 *     lower = 5j_gpio_lowerhalf();
 *     gpio_register(0, lower);
 *
 * Input Parameters:
 *   pincfg - bit encoded pinmux configuration
 *
 * Returned Value:
 *   On success, a non-NULL GPIO lower interface is returned. Otherwise, NULL.
 *
 ****************************************************************************/
FAR struct gpio_lowerhalf_s *esp32_gpio_lowerhalf(int pinnum, uint16_t pincfg)
{
	struct esp32_lowerhalf_s *lower;

	lower = (struct esp32_lowerhalf_s *)
			kmm_malloc(sizeof(struct esp32_lowerhalf_s));
	if (!lower) {
		return NULL;
	}

	lower->pincfg = pincfg;
	lower->pinnum = pinnum;
	lower->ops = &esp32_gpio_ops;

	return (struct gpio_lowerhalf_s *)lower;
}
