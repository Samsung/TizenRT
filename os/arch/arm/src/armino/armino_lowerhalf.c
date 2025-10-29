/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
 *
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
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

#include "up_arch.h"
#include "tinyara/gpio.h"

#include <tinyara/kmalloc.h>
#include "hal_gpio_types.h"
#include "gpio.h"
/****************************************************************************
 * Private Types
 ****************************************************************************/
struct armino_lowerhalf_s {
	FAR const struct gpio_ops_s *ops;
	struct gpio_upperhalf_s *parent;

	/* Including private value */
	u32 pin;
	u32 pinmode;
	u32 pinpull;
	gpio_handler_t handler;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#ifndef CONFIG_DISABLE_POLL

void armino_gpio_interrupt(gpio_id_t gpio_id, void *priv)
{
	struct armino_lowerhalf_s *lower = (struct armino_lowerhalf_s *)priv;

	if (lower->handler != NULL) {
		DEBUGASSERT(lower->handler != NULL);
		lower->handler(lower->parent);
	}
	return;
}

#endif

/****************************************************************************
 * Name: armino_gpio_set
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
static void armino_gpio_set(FAR struct gpio_lowerhalf_s *lower,
						 FAR unsigned int value)
{
	struct armino_lowerhalf_s *priv = (struct armino_lowerhalf_s *)lower;
	if (value == 0)
	{
		bk_gpio_set_output_low(priv->pin);
	} else {
		bk_gpio_set_output_high(priv->pin);
	}
	
}

/****************************************************************************
 * Name: armino_gpio_get
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
static int armino_gpio_get(FAR struct gpio_lowerhalf_s *lower)
{
	struct armino_lowerhalf_s *priv = (struct armino_lowerhalf_s *)lower;
	return bk_gpio_get_input(priv->pin);
}


static int armino_gpio_setdir(FAR struct gpio_lowerhalf_s *lower,
						   unsigned long arg)
{
	struct armino_lowerhalf_s *priv = (struct armino_lowerhalf_s *)lower;
	
	if (arg == GPIO_DIRECTION_OUT) {
		bk_gpio_enable_output(priv->pin);
	} else if (arg == GPIO_DIRECTION_IN) {
		bk_gpio_enable_input(priv->pin);
	} else if(arg == GPIO_DIRECTION_NONE) {
		bk_gpio_set_high_impedance(priv->pin);
	} else {
		return -EINVAL;
	}
	

	return OK;
}

static int armino_gpio_pull(FAR struct gpio_lowerhalf_s *lower, unsigned long arg) 
{
    struct armino_lowerhalf_s *priv = (struct armino_lowerhalf_s *)lower;

    if (arg > GPIO_DRIVE_FLOAT) {
        // gpioerr("ERROR: Invalid drive mode %lu\n", arg);
        return -EINVAL;
    }

    switch (arg) {
		case GPIO_DRIVE_FLOAT:
			bk_gpio_disable_pull(priv->pin);
			break;
		case GPIO_DRIVE_PULLUP:
			bk_gpio_pull_up(priv->pin);
			break;
		case GPIO_DRIVE_PULLDOWN:
			bk_gpio_pull_down(priv->pin);
			break;
		default:
			break;
    }

    // gpioinfo("Set pin%d pull mode: 0x%lx\n", priv->pin, arg);
    return OK;
}

static int armino_gpio_enable(FAR struct gpio_lowerhalf_s *lower, int falling,
						   int rising, gpio_handler_t handler)
{	
	struct armino_lowerhalf_s *priv = (struct armino_lowerhalf_s *)lower;
	gpio_int_type_t event = GPIO_INT_TYPE_MAX;
	
	if (falling && rising) {
		handler = NULL;
	} else if (falling) {
		event = GPIO_INT_TYPE_FALLING_EDGE;
	} else if (rising) {
		event = GPIO_INT_TYPE_RISING_EDGE;
	} else {
		handler = NULL;
	}
	priv->handler = handler;

	if (handler) {
		// gpio_irq_init((gpio_irq_t *)&priv->obj, priv->obj.pin, armino_gpio_interrupt,(uint32_t) priv);
		bk_gpio_register_isr_ex(priv->pin, armino_gpio_interrupt, (void *)priv);
		bk_gpio_set_interrupt_type(priv->pin, event);
		bk_gpio_enable_interrupt(priv->pin);
	} else {
		bk_gpio_disable_interrupt(priv->pin);
	}

	return OK;
}

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct gpio_ops_s armino_gpio_ops = {
	.set    = armino_gpio_set,
	.get    = armino_gpio_get,
	.pull   = armino_gpio_pull,
	.setdir = armino_gpio_setdir,
	.enable = armino_gpio_enable,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: armino_gpio_lowerhalf
 *
 * Description:
 *   Instantiate the GPIO lower half driver for the armino.
 *   General usage:
 *
 *     #include <tinyara/gpio.h>
 *     #include "armino_gpio.h"
 *
 *     struct gpio_lowerhalf_s *lower;
 *     lower = armino_gpio_lowerhalf();
 *     gpio_register(0, lower);
 *
 * Input Parameters:
 *   
 *
 * Returned Value:
 *   On success, a non-NULL GPIO lower interface is returned. Otherwise, NULL.
 *
 ****************************************************************************/
FAR struct gpio_lowerhalf_s *armino_gpio_lowerhalf(u32 pin, u32 pinmode, u32 pinpull)
{	
	struct armino_lowerhalf_s *lower;
	
	lower = (struct armino_lowerhalf_s *)kmm_malloc(sizeof(struct armino_lowerhalf_s));
	if (!lower) {
		return NULL;
	}
	gpio_config_t mode = {0};

	mode.io_mode = pinmode;
	mode.pull_mode = pinpull;

	int ret = bk_gpio_set_config(pin, &mode);
	if (ret != 0) {
		kmm_free(lower);
		return NULL;
	}

	lower->pin = pin;
	lower->pinmode =pinmode;
	lower->pinpull = pinpull;
	lower->ops = &armino_gpio_ops;
	return (struct gpio_lowerhalf_s *)lower;
}
