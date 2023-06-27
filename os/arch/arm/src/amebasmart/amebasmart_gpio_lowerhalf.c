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
#include "gpio_api.h"
#include "tinyara/gpio.h"

#include "objects.h"
#include "PinNames.h"
#include "gpio_irq_api.h"
#include "gpio_irq_ex_api.h"
#include <tinyara/kmalloc.h>

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct amebasmart_lowerhalf_s {
	FAR const struct gpio_ops_s *ops;
	struct gpio_upperhalf_s *parent;

	/* Including private value */
	gpio_t obj;
	u32 pinmode;
	u32 pinpull;
	gpio_handler_t handler;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#ifndef CONFIG_DISABLE_POLL
void amebasmart_gpio_interrupt(uint32_t arg, gpio_irq_event event)
{
	struct amebasmart_lowerhalf_s *lower = (struct amebasmart_lowerhalf_s *)arg;

	if (lower->handler != NULL) {
		DEBUGASSERT(lower->handler != NULL);
		lower->handler(lower->parent);
	}
	return;
}
#endif

/****************************************************************************
 * Name: amebasmart_gpio_set
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
static void amebasmart_gpio_set(FAR struct gpio_lowerhalf_s *lower,
						 FAR unsigned int value)
{
	struct amebasmart_lowerhalf_s *priv = (struct amebasmart_lowerhalf_s *)lower;
	gpio_write(&priv->obj, value);
}

/****************************************************************************
 * Name: amebasmart_gpio_get
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
static int amebasmart_gpio_get(FAR struct gpio_lowerhalf_s *lower)
{
	struct amebasmart_lowerhalf_s *priv = (struct amebasmart_lowerhalf_s *)lower;
	return gpio_read(&priv->obj);
}


static int amebasmart_gpio_setdir(FAR struct gpio_lowerhalf_s *lower,
						   unsigned long arg)
{
	struct amebasmart_lowerhalf_s *priv = (struct amebasmart_lowerhalf_s *)lower;
	
	if (arg == GPIO_DIRECTION_OUT) {
		priv->pinmode=PIN_OUTPUT;
		gpio_change_dir(&priv->obj,priv->pinmode);
	} else {
		priv->pinmode=PIN_INPUT;
		gpio_change_dir(&priv->obj,priv->pinmode);
	}

	return OK;
}

static int amebasmart_gpio_pull(FAR struct gpio_lowerhalf_s *lower, unsigned long arg)
{
	struct amebasmart_lowerhalf_s *priv = (struct amebasmart_lowerhalf_s *)lower;

	u32 GPIO_PuPd;

	if (arg == GPIO_DRIVE_FLOAT) {
		GPIO_PuPd = GPIO_PuPd_NOPULL;
	} else if (arg == GPIO_DRIVE_PULLUP) {
		GPIO_PuPd = GPIO_PuPd_UP;
	} else if (arg == GPIO_DRIVE_PULLDOWN) {
		GPIO_PuPd = GPIO_PuPd_DOWN;
	} else {
		return -EINVAL;
	}
	
	priv->pinpull=GPIO_PuPd;
	gpio_pull_ctrl(&priv->obj, GPIO_PuPd);
	
	return OK;
}

static int amebasmart_gpio_enable(FAR struct gpio_lowerhalf_s *lower, int falling,
						   int rising, gpio_handler_t handler)
{	
	struct amebasmart_lowerhalf_s *priv = (struct amebasmart_lowerhalf_s *)lower;
	gpio_irq_event event = IRQ_NONE;
	
	if (falling && rising) {
		event = IRQ_FALL_RISE;
	} else if (falling) {
		event = IRQ_FALL;
	} else if (rising) {
		event = IRQ_RISE;
	} else {
		handler = NULL;
	}
	priv->handler = handler;

	if (handler) {
		gpio_irq_init((gpio_irq_t *)&priv->obj, priv->obj.pin, amebasmart_gpio_interrupt,(uint32_t) priv);
		gpio_irq_set((gpio_irq_t *)&priv->obj, event, 1);
		gpio_irq_enable((gpio_irq_t *)&priv->obj);
	} else {
		gpio_irq_disable((gpio_irq_t *)&priv->obj);
		gpio_irq_set((gpio_irq_t *)&priv->obj, event, 0);
		gpio_irq_deinit((gpio_irq_t *)&priv->obj);
	}

	return OK;
}

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct gpio_ops_s amebasmart_gpio_ops = {
	.set    = amebasmart_gpio_set,
	.get    = amebasmart_gpio_get,
	.pull   = amebasmart_gpio_pull,
	.setdir = amebasmart_gpio_setdir,
	.enable = amebasmart_gpio_enable,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: amebasmart_gpio_lowerhalf
 *
 * Description:
 *   Instantiate the GPIO lower half driver for the amebasmart.
 *   General usage:
 *
 *     #include <tinyara/gpio.h>
 *     #include "amebasmart_gpio.h"
 *
 *     struct gpio_lowerhalf_s *lower;
 *     lower = amebasmart_gpio_lowerhalf();
 *     gpio_register(0, lower);
 *
 * Input Parameters:
 *   
 *
 * Returned Value:
 *   On success, a non-NULL GPIO lower interface is returned. Otherwise, NULL.
 *
 ****************************************************************************/
FAR struct gpio_lowerhalf_s *amebasmart_gpio_lowerhalf(u32 pinname, u32 pinmode, u32 pinpull)
{	
	struct amebasmart_lowerhalf_s *lower;
	
	lower = (struct amebasmart_lowerhalf_s *)kmm_malloc(sizeof(struct amebasmart_lowerhalf_s));
	if (!lower) {
		return NULL;
	}
	gpio_init(&lower->obj, pinname);
    	gpio_dir(&lower->obj, pinmode);
    	gpio_mode(&lower->obj, pinpull);
	lower->pinmode =pinmode;
	lower->pinpull = pinpull;
	lower->ops = &amebasmart_gpio_ops;
	return (struct gpio_lowerhalf_s *)lower;
}
