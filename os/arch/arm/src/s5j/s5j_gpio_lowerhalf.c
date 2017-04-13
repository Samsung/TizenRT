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
#include <string.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>

#include <arch/irq.h>
#include <tinyara/gpio.h>
#include <tinyara/wdog.h>
#include <tinyara/wqueue.h>
#include <tinyara/kmalloc.h>

#include "up_arch.h"
#include "s5j_gpio.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define NUM_GPIO	67

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct s5j_gpio_priv {
	int gpio;
	int idx;
	int isr_num;
};

/****************************************************************************
 * Private Functions prototypes
 ****************************************************************************/
static int s5j_gpio_open(FAR struct gpio_dev_s *dev);
static int s5j_gpio_close(FAR struct gpio_dev_s *dev);
static int s5j_gpio_get(FAR struct gpio_dev_s *dev);
static void s5j_gpio_set(FAR struct gpio_dev_s *dev, FAR unsigned int value);
static int s5j_gpio_ctrl(struct gpio_dev_s *dev, int cmd, unsigned long args);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct gpio_ops_s s5j_gpio_ops = {
	.open  = s5j_gpio_open,
	.close = s5j_gpio_close,
	.set   = s5j_gpio_set,
	.get   = s5j_gpio_get,
	.ctrl  = s5j_gpio_ctrl
};

static struct gpio_dev_s *s5j_gpio_all[NUM_GPIO];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_gpio_callback_wqueue
 *
 * Description:
 *   Callback function assigned by s5j_gpio_poll_expiry, to execute by work
 *   queue
 *
 * Input Parameters:
 *  arg - arguments
 *
 * Returned Value:
 *  None
 *
 ****************************************************************************/
static void s5j_gpio_callback_wqueue(FAR void *arg)
{
	struct gpio_dev_s *dev;

	dev = (struct gpio_dev_s *)arg;

	if (dev->callback) {
		dev->callback();
	}
}

/****************************************************************************
 * Function: s5j_poll_expiry
 *
 * Description:
 *   Periodic timer handler.  Called from the timer interrupt handler.
 *
 * Parameters:
 *   argc - The number of available arguments
 *   arg  - The first argument
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   Global interrupts are disabled by the watchdog logic.
 *
 ****************************************************************************/
static void s5j_gpio_poll_expiry(int argc, uint32_t arg, ...)
{
	const uint32_t S5J_WDDELAY = 1 * CLK_TCK;
	FAR struct gpio_dev_s *dev = (FAR struct gpio_dev_s *)arg;

#ifdef CONFIG_SCHED_WORKQUEUE
	if (work_available(&dev->work)) {
		/* Schedule to perform the interrupt processing on the worker thread. */
		work_queue(HPWORK, &dev->work, s5j_gpio_callback_wqueue, dev, 0);
	} else {
#endif
		/*
		 * No.. Just re-start the watchdog poll timer, missing one polling
		 * cycle.
		 */
		(void)wd_start(dev->wdog, S5J_WDDELAY, s5j_gpio_poll_expiry, 1, (uint32_t)dev);
#ifdef CONFIG_SCHED_WORKQUEUE
	}
#endif
}

/****************************************************************************
 * Name: gpio_get_irq_id
 *
 * Description:
 *  Return IRQ ID
 *
 * Input Parameters:
 *  gpio - port id
 *
 * Returned Value:
 *  irq ID
 *  < 0, Error
 *
 ****************************************************************************/
static u32 gpio_get_irq_id(int gpio)
{
	unsigned port;
	unsigned isr;
	struct gpio_bank *gb = NULL;

	if (!gpio_valid(gpio)) {
		return -EINVAL;
	}

	gb = gpio_to_bank(gpio);
	port = s5j_gpio_port(gpio);

	isr = gb->isr_num[port];
	if (!isr) {
		isr = gb->isr_num[0];
	}

	return gpio_irq_id(gpio, isr);
}

/****************************************************************************
 * Name: s5j_gpio_irq_handler
 *
 * Description:
 *  IRQ handler to assign to handle IRQ and perform gpio notify, if configured
 *
 * Input Parameters:
 *  irq - IRQ number
 *  context - pointer to a dedicated context structure
 *
 * Returned Value:
 *  Ok
 *  -1, if Error
 *
 ****************************************************************************/
static int s5j_gpio_irq_handler(int irq, FAR void *context, FAR void *arg)
{
	struct gpio_dev_s *dev = (struct gpio_dev_s *)arg;
	struct s5j_gpio_priv *priv = (struct s5j_gpio_priv *)dev->priv;

	gpio_eint_clear_pending(priv->gpio);

	if (dev->callback) {
		s5j_gpio_poll_expiry(1, (uint32_t)dev);
	}

#ifdef CONFIG_GPIO
#ifndef CONFIG_DISABLE_POLL
	gpio_notify(dev);
#endif
#endif

	return OK;
}

/****************************************************************************
 * Name: s5j_gpio_enable_irq
 *
 * Description:
 *   Enable IRQ
 *
 * Input Parameters:
 *   dev - GPIO device
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void s5j_gpio_enable_irq(struct gpio_dev_s *dev)
{
	int gpio;
	int irq;
	int pin;

	gpio = ((struct s5j_gpio_priv *)(dev->priv))->gpio;

	gpio_cfg_pin(gpio, GPIO_IRQ);
	gpio_eint_clear_pending(gpio);
	gpio_eint_unmask(gpio);

	pin = s5j_gpio_port(gpio);
	irq = gpio_to_bank(gpio)->isr_num[pin];

	irq_attach(irq, s5j_gpio_irq_handler, dev);
}

/****************************************************************************
 * Name: s5j_gpio_disable_irq
 *
 * Description:
 *  Disable IRQ
 *
 * Input Parameters:
 *  dev - GPIO device
 *
 * Returned Value:
 *  None
 *
 ****************************************************************************/
static void s5j_gpio_disable_irq(struct gpio_dev_s *dev)
{
	int gpio;
	int irq;

	gpio = ((struct s5j_gpio_priv *)(dev->priv))->gpio;
	gpio_eint_clear_pending(gpio);
	gpio_eint_unmask(gpio);
	irq = gpio_to_bank(gpio)->isr_num[s5j_gpio_port(gpio)];

	irq_detach(irq);
}

/****************************************************************************
 * Name: s5j_gpio_open
 *
 * Description:
 *   Open GPIO port
 *
 * Input Parameters:
 *   dev - device
 *
 * Returned Value:
 *   == OK: Open success
 *   == -EINVAL: invalid gpio
 ****************************************************************************/
static int s5j_gpio_open(FAR struct gpio_dev_s *dev)
{
	return OK;
}

/****************************************************************************
 * Name: s5j_gpio_close
 *
 * Description:
 *   Close GPIO port
 *
 * Input Parameters:
 *   dev - device
 *
 * Returned Value:
 *   == 0: close success
 *   == -EINVAL: invalid gpio
 ****************************************************************************/
static int s5j_gpio_close(FAR struct gpio_dev_s *dev)
{
	struct s5j_gpio_priv *priv = (struct s5j_gpio_priv *)(dev->priv);
	int idx = priv->idx;

	s5j_gpio_disable_irq(s5j_gpio_all[idx]);

	if (NULL != dev->wdog) {
		wd_cancel(dev->wdog);
		dev->wdog = NULL;
	}

	return OK;
}

/****************************************************************************
 * Name: s5j_gpio_get
 *
 * Description:
 *   Get GPIO value
 *
 * Input Parameters:
 *   dev - device
 *
 * Returned Value:
 *   >= 0: gpio value
 *   == -EINVAL: invalid gpio
 ****************************************************************************/
static int s5j_gpio_get(FAR struct gpio_dev_s *dev)
{
	int gpio = ((struct s5j_gpio_priv *)(dev->priv))->gpio;

	if (!gpio_valid(gpio)) {
		return -EINVAL;
	}

	return gpio_get_value(gpio);
}

/****************************************************************************
 * Name: s5j_gpio_set
 *
 * Description:
 *   Set GPIO value
 *
 * Input Parameters:
 *   dev - device
 *   value - value to set
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void s5j_gpio_set(FAR struct gpio_dev_s *dev, FAR unsigned int value)
{
	int gpio = ((struct s5j_gpio_priv *)(dev->priv))->gpio;
	gpio_set_value(gpio, value);
}

/****************************************************************************
 * Name: s5j_gpio_ctrl
 *
 * Description:
 *   GPIO IOCTRL function
 *
 * Input Parameters:
 *   dev - device
 *   cmd - ioctl command
 *   arg - ioctl argument
 *
 * Returned Value:
 *   >= 0: gpio ret value or OK
 *   == -EINVAL: invalid gpio
 *
 ****************************************************************************/
static int s5j_gpio_ctrl(struct gpio_dev_s *dev, int cmd, unsigned long args)
{
	int gpio = ((struct s5j_gpio_priv *)(dev->priv))->gpio;
	bool isenable = true;

	GPIO_CB_FUNC func = (GPIO_CB_FUNC)args;

	if (!gpio_valid(gpio)) {
		return -EINVAL;
	}

	switch (cmd) {
	case GPIO_CMD_SET_DIRECTION:
		switch (args) {
		case GPIO_DIRECTION_OUT:
			gpio_cfg_pin(gpio, GPIO_OUTPUT);
			break;
		case GPIO_DIRECTION_IN:
			gpio_cfg_pin(gpio, GPIO_INPUT);
			break;
		default:
			return -EINVAL;
		}
		break;

	case GPIO_CMD_GET_DIRECTION:
		return gpio_cfg_get_pin(gpio);
		break;

	case GPIO_CMD_SET_EDGE:
		gpio_eint_set_filter(gpio, EINT_FILTER_DELAY, 0);
		switch (args) {
		case GPIO_EDGE_NONE:
			s5j_gpio_disable_irq(dev);
			isenable = false;
			break;
		case GPIO_EDGE_BOTH:
			gpio_eint_set_type(gpio, EINT_TYPE_EDGE_BOTH);
			break;
		case GPIO_EDGE_RISING:
			gpio_eint_set_type(gpio, EINT_TYPE_EDGE_RISING);
			break;
		case GPIO_EDGE_FALLING:
			gpio_eint_set_type(gpio, EINT_TYPE_EDGE_FALLING);
			break;
		case GPIO_LEVEL_LOW:
			gpio_eint_set_type(gpio, EINT_TYPE_LEVEL_LOW);
			break;
		case GPIO_LEVEL_HIGH:
			gpio_eint_set_type(gpio, EINT_TYPE_LEVEL_HIGH);
			break;
		default:
			return -EINVAL;
		}
		if (isenable) {
			s5j_gpio_enable_irq(dev);
		}
		break;

	case GPIO_CMD_GET_EDGE:
		switch (gpio_eint_get_type(gpio)) {
		case EINT_TYPE_EDGE_BOTH:
			return GPIO_EDGE_BOTH;
			break;
		case EINT_TYPE_EDGE_RISING:
			return GPIO_EDGE_RISING;
			break;
		case EINT_TYPE_EDGE_FALLING:
			return GPIO_EDGE_FALLING;
			break;
		case EINT_TYPE_LEVEL_LOW:
			return GPIO_LEVEL_LOW;
			break;
		case EINT_TYPE_LEVEL_HIGH:
			return GPIO_LEVEL_HIGH;
			break;
		default:
			return -EINVAL;
		}
		break;

	case GPIO_CMD_SET_DRIVE:
		switch (args) {
		case GPIO_DRIVE_NONE:
			gpio_set_pull(gpio, GPIO_PULL_NONE);
			break;
		case GPIO_DRIVE_PULLUP:
			gpio_set_pull(gpio, GPIO_PULL_UP);
			break;
		case GPIO_DRIVE_PULLDOWN:
			gpio_set_pull(gpio, GPIO_PULL_DOWN);
			break;
		default:
			return -EINVAL;
		}
		break;
	case GPIO_CMD_GET_DRIVE:
		switch (gpio_get_pull(gpio)) {
		case GPIO_PULL_NONE:
			return GPIO_DRIVE_NONE;
		case GPIO_PULL_UP:
			return GPIO_DRIVE_PULLUP;
		case GPIO_PULL_DOWN:
			return GPIO_DRIVE_PULLDOWN;
		default:
			return -EINVAL;
		}
		break;

	case GPIO_CMD_SET_CALLBACK:
		/* if someone creates wdog, we don't need to create again */
		if (dev->wdog == NULL) {
			dev->wdog = wd_create();
		}

		dev->callback = func;
		break;

	default:
		return -EINVAL;
		break;
	}

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_create_gpio
 *
 * Description:
 *   Creates GPIO port and expose it to file system: dev/gpioXX
 *
 * Input Parameters:
 *   gpio - port id
 *
 * Returned Value:
 *   0
 ****************************************************************************/
int up_create_gpio(int32_t idx)
{
#ifdef CONFIG_GPIO
	char path[20];
	struct s5j_gpio_priv *pgpio;
	int bank = 0;
	int port;
	int idx_table[] = { 7, 15, 22, 28, 36, 44, 52, 56, 59, 63, 66 };

	pgpio = (struct s5j_gpio_priv *)kmm_malloc(sizeof(struct s5j_gpio_priv));
	s5j_gpio_all[idx] = (struct gpio_dev_s *)kmm_malloc(sizeof(struct gpio_dev_s));
	s5j_gpio_all[idx]->ops = &s5j_gpio_ops;
	s5j_gpio_all[idx]->priv = pgpio;
	s5j_gpio_all[idx]->wdog = NULL;
	s5j_gpio_all[idx]->callback = NULL;

	for (bank = 0; bank < sizeof(idx_table) / sizeof(int); bank++) {
		if (idx_table[bank] >= idx) {
			break;
		}
	}

	if (bank) {
		port = idx - (idx_table[bank - 1] + 1);
	} else {
		port = idx;
	}

	pgpio->gpio = s5j_gpio(bank, port);
	pgpio->idx = idx;
	pgpio->isr_num = gpio_get_irq_id(pgpio->gpio);
	snprintf(path, sizeof(path), "/dev/gpio%d", idx);
	gpio_register(path, s5j_gpio_all[idx]);
#endif

	return 0;
}

/****************************************************************************
 * Name: up_destroy_gpio
 *
 * Description:
 *   Destroy GPIO idx entry, release allocated priv memory.
 *
 * Input Parameters:
 *   idx - GPIO index
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
int up_destroy_gpio(int32_t idx)
{
	struct s5j_gpio_priv *priv = (struct s5j_gpio_priv *)(s5j_gpio_all[idx]->priv);

	if (priv) {
		kmm_free(priv);
	}

	s5j_gpio_all[idx]->ops = NULL;
	s5j_gpio_all[idx]->priv = NULL;

	if (s5j_gpio_all[idx]->wdog) {
		wd_cancel(s5j_gpio_all[idx]->wdog);
	}

	kmm_free(s5j_gpio_all[idx]);

	return OK;
}
