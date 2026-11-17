/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * os/arch/arm/src/s5j/s5j_gpio_lowerhalf.c
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
#include <tinyara/irq.h>
#include <tinyara/ioexpander/gpio.h>
#include <tinyara/kmalloc.h>

#include "arm_internal.h"
#include "s5j_gpio.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct s5j_gpio_dev_s {
	struct gpio_dev_s gpio;		/* Must be the first member */
	uint32_t pincfg;
	pin_interrupt_t callback;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int s5j_gpio_read(FAR struct gpio_dev_s *dev, FAR bool *value);
static int s5j_gpio_write(FAR struct gpio_dev_s *dev, bool value);
static int s5j_gpio_attach(FAR struct gpio_dev_s *dev, pin_interrupt_t callback);
static int s5j_gpio_enable(FAR struct gpio_dev_s *dev, bool enable);
static int s5j_gpio_setpintype(FAR struct gpio_dev_s *dev, enum gpio_pintype_e pintype);
static int s5j_gpio_setdebounce(FAR struct gpio_dev_s *dev, unsigned long duration);
static int s5j_gpio_setmask(FAR struct gpio_dev_s *dev, bool mask);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct gpio_operations_s s5j_gpio_ops = {
	.go_read        = s5j_gpio_read,
	.go_write       = s5j_gpio_write,
	.go_attach      = s5j_gpio_attach,
	.go_enable      = s5j_gpio_enable,
	.go_setpintype  = s5j_gpio_setpintype,
	.go_setdebounce = s5j_gpio_setdebounce,
	.go_setmask     = s5j_gpio_setmask,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int s5j_gpio_interrupt(int irq, FAR void *context, FAR void *arg)
{
	FAR struct s5j_gpio_dev_s *priv = (FAR struct s5j_gpio_dev_s *)arg;

	s5j_gpio_clear_pending(priv->pincfg);

	if (priv->callback != NULL) {
		priv->callback(&priv->gpio, 0);
	}

	return OK;
}

/****************************************************************************
 * Name: s5j_gpio_pintype
 *
 * Description:
 *   Translate the bit-encoded S5J pin configuration into the GPIO pin type
 *   reported to the upper half driver.
 *
 ****************************************************************************/
static enum gpio_pintype_e s5j_gpio_pintype(uint32_t pincfg)
{
	switch (pincfg & GPIO_FUNC_MASK) {
	case GPIO_OUTPUT:
		return GPIO_OUTPUT_PIN;

	case GPIO_EINT:
		switch (pincfg & GPIO_EINT_MASK) {
		case GPIO_EINT_LOW:
			return GPIO_INTERRUPT_LOW_PIN;
		case GPIO_EINT_HIGH:
			return GPIO_INTERRUPT_HIGH_PIN;
		case GPIO_EINT_FALLING_EDGE:
			return GPIO_INTERRUPT_FALLING_PIN;
		case GPIO_EINT_RISING_EDGE:
			return GPIO_INTERRUPT_RISING_PIN;
		default:
			return GPIO_INTERRUPT_BOTH_PIN;
		}

	default:
		break;
	}

	switch (pincfg & GPIO_PUPD_MASK) {
	case GPIO_PULLUP:
		return GPIO_INPUT_PIN_PULLUP;
	case GPIO_PULLDOWN:
		return GPIO_INPUT_PIN_PULLDOWN;
	default:
		return GPIO_INPUT_PIN;
	}
}

/****************************************************************************
 * Name: s5j_gpio_read
 *
 * Description:
 *   Read the current value of the pin.
 *
 ****************************************************************************/
static int s5j_gpio_read(FAR struct gpio_dev_s *dev, FAR bool *value)
{
	FAR struct s5j_gpio_dev_s *priv = (FAR struct s5j_gpio_dev_s *)dev;

	DEBUGASSERT(priv != NULL && value != NULL);

	*value = s5j_gpioread(priv->pincfg);
	return OK;
}

/****************************************************************************
 * Name: s5j_gpio_write
 *
 * Description:
 *   Drive the output pin.
 *
 ****************************************************************************/
static int s5j_gpio_write(FAR struct gpio_dev_s *dev, bool value)
{
	FAR struct s5j_gpio_dev_s *priv = (FAR struct s5j_gpio_dev_s *)dev;

	DEBUGASSERT(priv != NULL);

	s5j_gpiowrite(priv->pincfg, value);
	return OK;
}

/****************************************************************************
 * Name: s5j_gpio_attach
 *
 * Description:
 *   Remember the upper half interrupt callback.  The interrupt itself is
 *   attached and enabled by s5j_gpio_enable().
 *
 ****************************************************************************/
static int s5j_gpio_attach(FAR struct gpio_dev_s *dev, pin_interrupt_t callback)
{
	FAR struct s5j_gpio_dev_s *priv = (FAR struct s5j_gpio_dev_s *)dev;

	DEBUGASSERT(priv != NULL);

	priv->callback = callback;
	return OK;
}

/****************************************************************************
 * Name: s5j_gpio_enable
 *
 * Description:
 *   Enable or disable the external interrupt of the pin.
 *
 ****************************************************************************/
static int s5j_gpio_enable(FAR struct gpio_dev_s *dev, bool enable)
{
	FAR struct s5j_gpio_dev_s *priv = (FAR struct s5j_gpio_dev_s *)dev;
	int irqvector;

	DEBUGASSERT(priv != NULL);

	irqvector = s5j_gpio_irqvector(priv->pincfg);
	if (!irqvector) {
		return -EINVAL;
	}

	if (enable && priv->callback != NULL) {
		s5j_gpio_clear_pending(priv->pincfg);
		irq_attach(irqvector, s5j_gpio_interrupt, priv);
		up_enable_irq(irqvector);
	} else {
		up_disable_irq(irqvector);
		irq_detach(irqvector);
	}

	return OK;
}

/****************************************************************************
 * Name: s5j_gpio_setpintype
 *
 * Description:
 *   Reconfigure the pin for the requested pin type.
 *
 ****************************************************************************/
static int s5j_gpio_setpintype(FAR struct gpio_dev_s *dev, enum gpio_pintype_e pintype)
{
	FAR struct s5j_gpio_dev_s *priv = (FAR struct s5j_gpio_dev_s *)dev;
	uint32_t pincfg;
	int ret;

	DEBUGASSERT(priv != NULL);

	pincfg = priv->pincfg & ~(GPIO_FUNC_MASK | GPIO_PUPD_MASK | GPIO_EINT_MASK);

	switch (pintype) {
	case GPIO_INPUT_PIN:
		pincfg |= GPIO_INPUT | GPIO_FLOAT;
		break;
	case GPIO_INPUT_PIN_PULLUP:
		pincfg |= GPIO_INPUT | GPIO_PULLUP;
		break;
	case GPIO_INPUT_PIN_PULLDOWN:
		pincfg |= GPIO_INPUT | GPIO_PULLDOWN;
		break;
	case GPIO_OUTPUT_PIN:
		pincfg |= GPIO_OUTPUT | GPIO_FLOAT;
		break;
	case GPIO_INTERRUPT_PIN:
	case GPIO_INTERRUPT_BOTH_PIN:
		pincfg |= GPIO_EINT | GPIO_EINT_BOTH_EDGE;
		break;
	case GPIO_INTERRUPT_HIGH_PIN:
		pincfg |= GPIO_EINT | GPIO_EINT_HIGH;
		break;
	case GPIO_INTERRUPT_LOW_PIN:
		pincfg |= GPIO_EINT | GPIO_EINT_LOW;
		break;
	case GPIO_INTERRUPT_RISING_PIN:
		pincfg |= GPIO_EINT | GPIO_EINT_RISING_EDGE;
		break;
	case GPIO_INTERRUPT_FALLING_PIN:
		pincfg |= GPIO_EINT | GPIO_EINT_FALLING_EDGE;
		break;
	default:
		/* Open-drain output and the wakeup interrupt types are not
		 * supported by the S5J GPIO block.
		 */
		return -EINVAL;
	}

	if ((pincfg & GPIO_FUNC_MASK) == GPIO_EINT && !s5j_gpio_irqvector(pincfg)) {
		/* Only the GPG1, GPG2 and GPA0 ports can generate interrupts */
		return -EINVAL;
	}

	ret = s5j_configgpio(pincfg);
	if (ret < 0) {
		return ret;
	}

	priv->pincfg = pincfg;
	priv->gpio.gp_pintype = pintype;
	return OK;
}

static int s5j_gpio_setdebounce(FAR struct gpio_dev_s *dev, unsigned long duration)
{
	return -ENOSYS;
}

static int s5j_gpio_setmask(FAR struct gpio_dev_s *dev, bool mask)
{
	return -ENOSYS;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_gpio_lowerhalf
 *
 * Description:
 *   Instantiate the GPIO lower half driver for the S5J.
 *   General usage:
 *
 *     #include <tinyara/ioexpander/gpio.h>
 *     #include "s5j_gpio.h"
 *
 *     struct gpio_dev_s *dev;
 *     dev = s5j_gpio_lowerhalf(pincfg);
 *     gpio_pin_register(dev, minor);
 *
 *   The pin is not reconfigured here; the initial pin type is derived from
 *   pincfg and the pin is configured when the pin type is changed.
 *
 * Input Parameters:
 *   pincfg - bit encoded pinmux configuration
 *
 * Returned Value:
 *   On success, a non-NULL GPIO device is returned. Otherwise, NULL.
 *
 ****************************************************************************/
FAR struct gpio_dev_s *s5j_gpio_lowerhalf(uint16_t pincfg)
{
	FAR struct s5j_gpio_dev_s *priv;

	priv = (FAR struct s5j_gpio_dev_s *)kmm_zalloc(sizeof(struct s5j_gpio_dev_s));
	if (!priv) {
		return NULL;
	}

	priv->pincfg = pincfg;
	priv->gpio.gp_pintype = s5j_gpio_pintype(pincfg);
	priv->gpio.gp_ops = &s5j_gpio_ops;

	return &priv->gpio;
}
