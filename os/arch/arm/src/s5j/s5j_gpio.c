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
 * arch/arm/src/s5j/s5j_gpio.c
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

#include <arch/irq.h>
#include <arch/chip/chip_types.h>
#include <tinyara/gpio.h>
#include <tinyara/wdog.h>
#include <tinyara/wqueue.h>
#include <tinyara/kmalloc.h>

#include "up_arch.h"
#include "s5j_gpio.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define EINT_INDEX		(57)
#define NUM_EINT		(3)
#define NUM_GPIO        (67)
/****************************************************************************
 * Private Types
 ****************************************************************************/
struct s5j_gpio_priv {
	int gpio;
	int idx;
	int isr_num;
};
CODE int s5j_gpio_open(FAR struct gpio_dev_s *dev);
CODE int s5j_gpio_close(FAR struct gpio_dev_s *dev);
CODE void s5j_gpio_set(FAR struct gpio_dev_s *dev, FAR unsigned int value);
CODE int s5j_gpio_get(FAR struct gpio_dev_s *dev);
CODE int s5j_gpio_ctrl(struct gpio_dev_s *dev, int cmd, unsigned long args);
static void s5j_gpio_disable_irq(struct gpio_dev_s *dev);
static void s5j_gpio_enable_irq(struct gpio_dev_s *dev);

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

static struct gpio_dev_s s5j_gpio_all[NUM_GPIO];

/****************************************************************************
 * Public Data
 ****************************************************************************/
#if defined(CONFIG_ARCH_CHIP_S5JT200)
static struct gpio_bank s5jt200_gpio_bank[] = {
	/* ALIVE */
	[GPP0] = {
		.name = "GPP0",
		.base = (void *)(GPIO_CON_BASE),
		.nr_port = 8,
		.isr_num = { IRQ_INVALID, },
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPP1] = {
		.name = "GPP1",
		.base = (void *)(GPIO_CON_BASE + 0x20),
		.nr_port = 8,
		.isr_num = { IRQ_INVALID, },
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPP2] = {
		.name = "GPP2",
		.base = (void *)(GPIO_CON_BASE + 0x40),
		.nr_port = 7,
		.isr_num = { IRQ_INVALID, },
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPP3] = {
		.name = "GPP3",
		.base = (void *)(GPIO_CON_BASE + 0x60),
		.nr_port = 6,
		.isr_num = { IRQ_INVALID, },
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPG0] = {
		.name = "GPG0",
		.base = (void *)(GPIO_CON_BASE + 0x80),
		.nr_port = 8,
		.isr_num = { IRQ_INVALID, },
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPG1] = {
		.name = "GPG1",
		.base = (void *)(GPIO_CON_BASE + 0xa0),
		.nr_port = 8,
		.isr_num = { IRQ_GPIO_GIC_0, },
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPG2] = {
		.name = "GPG2",
		.base = (void *)(GPIO_CON_BASE + 0xc0),
		.nr_port = 8,
		.isr_num = { IRQ_GPIO_GIC_1, },
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPG3] = {
		.name = "GPG3",
		.base = (void *)(GPIO_CON_BASE + 0xe0),
		.nr_port = 8,
		.isr_num = { IRQ_INVALID, },
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPA0] = {
		.name = "GPA0",
		.base = (void *)(GPIO_CON_BASE + 0x100),
		.nr_port = 3,
		.isr_num = { IRQ_EINT0, IRQ_EINT1, IRQ_EINT2 },
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPA1] = {
		.name = "GPA1",
		.base = (void *)(GPIO_CON_BASE + 0x120),
		.nr_port = 4,
		.isr_num = { IRQ_INVALID, },
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPA2] = {
		.name = "GPA2",
		.base = (void *)(GPIO_CON_BASE + 0x140),
		.nr_port = 3,
		.isr_num = { IRQ_INVALID, },
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPA3] = {
		.name = "GPA3",
		.base = (void *)(GPIO_CON_BASE + 0x160),
		.nr_port = 2,
		.isr_num = { IRQ_INVALID, },
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPP4] = {
		.name = "GPP4",
		.base = (void *)(GPIO_CON_BASE + 0x180),
		.nr_port = 4,
		.isr_num = { IRQ_INVALID, },
		.group_type = GPIO_GROUP_COMMON,
	},
	[ETC0] = {
		.name = "ETC0",
		.base = (void *)(GPIO_CON_BASE + 0x1A0),
		.nr_port = 3,
		.isr_num = { IRQ_INVALID, },
		.group_type = GPIO_GROUP_COMMON,
	},
};
#endif

#if defined(CONFIG_S5E_GPIO_LOG_DUMP)
static char *pull_str[] = { "none", "?", "down", "up" };
static char *drv_str[] = { "1x", "3x", "2x", "4x" };
#endif
bool isinit = 0;
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static u32 gpio_get_irq_id(int gpio);

int up_create_gpio(int32_t idx)
{
#ifdef CONFIG_GPIO
	char path[20];
	struct s5j_gpio_priv *pgpio;
	int bank = 0;
	int port;
	int idx_table[] = { 7, 15, 22, 28, 36, 44, 52, 56, 59, 63, 66 };

	pgpio = (struct s5j_gpio_priv *)kmm_malloc(sizeof(struct s5j_gpio_priv));
	s5j_gpio_all[idx].ops = &s5j_gpio_ops;
	s5j_gpio_all[idx].priv = pgpio;
	s5j_gpio_all[idx].wdog = NULL;
	s5j_gpio_all[idx].callback = NULL;

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
	gpio_register(path, &s5j_gpio_all[idx]);
	return 0;
#else
	return 0;
#endif
}

static struct gpio_bank *gpio_to_bank(int gpio)
{
	unsigned bank;

	bank = s5j_gpio_bank(gpio);

	if (!gpio_valid(gpio)) {
		return NULL;
	}

	return s5jt200_gpio_bank + bank;
}

static void *__gpio_to_eint_base(int gpio)
{
	struct gpio_bank *gb;
	gb = gpio_to_bank(gpio);

	if (!gb) {
		return NULL;
	}
	return gb->base;
}

static unsigned __gpio_to_eint_bank(int gpio)
{
	int bank;

	bank = s5j_gpio_bank(gpio);

	if (bank >= 1 && bank <= 8) {
		return (unsigned)bank;
	} else {
		return (unsigned)0;
	}
}

static void *__gpio_eint_filter_get_addr(int gpio)
{
	void *gpio_filter_addr;
	struct gpio_bank *gb = gpio_to_bank(gpio);

	gpio_filter_addr = (void *)(gb->filter_offset_addr);

	return gpio_filter_addr;
}

static void *__gpio_eint_get_addr(int gpio, unsigned offset)
{
	void *gpio_base;
	void *eint_addr;
	unsigned bank_num;
	int port;

	gpio_base = __gpio_to_eint_base(gpio);
	/* gpio_base = (void *)(((u64)gpio_base >> 16) << 16); TODO check this code correct or NOT */
	bank_num = __gpio_to_eint_bank(gpio);
	port = s5j_gpio_port(gpio);

	if (offset == GPIO_EINT_FLTCON) {
		eint_addr = gpio_base + offset + (unsigned)(__gpio_eint_filter_get_addr(gpio));
		if (port >= 4) {
			eint_addr += 0x4;
		}
	} else {
		eint_addr = gpio_base + offset;
	}

	return eint_addr;
}

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

	if (work_available(&dev->work)) {
		/* Schedule to perform the interrupt processing on the worker thread. */

		work_queue(HPWORK, &dev->work, s5j_gpio_callback_wqueue, dev, 0);
	} else {
		/* No.. Just re-start the watchdog poll timer, missing one polling
		 * cycle.
		 */

		(void)wd_start(dev->wdog, S5J_WDDELAY, s5j_gpio_poll_expiry, 1, (uint32_t)dev);
	}
}

static int s5j_gpio_irq_handler(int irq, void *context)
{
	int i;
	struct gpio_dev_s *dev;

	for (i = 0; i < NUM_GPIO; i++) {
		if (&(s5j_gpio_all[i]) && ((((struct s5j_gpio_priv *)(s5j_gpio_all[i].priv))->isr_num) & 0x3ff) == irq) {
				break;
		}
	}

	if (i == NUM_GPIO) {
		return -1;
	}

	gpio_eint_clear_pending(((struct s5j_gpio_priv *)(s5j_gpio_all[i].priv))->gpio);

	dev = &(s5j_gpio_all[i]);

	if (dev->callback) {
		s5j_gpio_poll_expiry(1, (uint32_t)dev);
	}
#ifdef CONFIG_GPIO
#ifndef CONFIG_DISABLE_POLL
	gpio_notify(&s5j_gpio_all[i]);
#endif
#endif

	return OK;
}

static void s5j_gpio_enable_irq(struct gpio_dev_s *dev)
{
	int gpio;
	int irq;
	gpio = ((struct s5j_gpio_priv *)(dev->priv))->gpio;

	gpio_cfg_pin(gpio, GPIO_IRQ);
	gpio_eint_clear_pending(gpio);
	gpio_eint_unmask(gpio);

	irq = gpio_to_bank(gpio)->isr_num[s5j_gpio_port(gpio)];
	(void)irq_attach(irq, (xcpt_t)s5j_gpio_irq_handler);
	up_enable_irq(irq);
}

static void s5j_gpio_disable_irq(struct gpio_dev_s *dev)
{
	int gpio;
	int irq;
	gpio = ((struct s5j_gpio_priv *)(dev->priv))->gpio;
	gpio_eint_clear_pending(gpio);
	gpio_eint_unmask(gpio);
	irq = gpio_to_bank(gpio)->isr_num[s5j_gpio_port(gpio)];
	up_disable_irq(irq);
}

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
 * Public Functions
 ****************************************************************************/

/**
 * int gpio_valid(int gpio / Check if given gpio is in a valid range
 * @param[in] gpio	gpio id
 * @return    == 1: valid gpio
 *            == 0: invalid gpio
 *
 */
int gpio_valid(int gpio)
{
	unsigned bank, port;

	if (gpio < 0) {
		return 0;
	}

	if ((gpio & 0xffff0000) != GPIO_MAGIC) {
		return 0;
	}

	bank = s5j_gpio_bank(gpio);
	port = s5j_gpio_port(gpio);

	if (bank >= GPEND) {
		return 0;
	}

	if (port >= s5jt200_gpio_bank[bank].nr_port) {
		return 0;
	}

	return 1;
}

/**
 * const char *gpio_bank_name(int gpio) / Find and return a gpio bank name from a gpio id
 * @param int gpio id
 * @return gpio bank name
 *
 */
const char *gpio_bank_name(int gpio)
{
	struct gpio_bank *gb = gpio_to_bank(gpio);

	if (!gb) {
		return NULL;
	}

	return gb->name;
}

/**
 * int gpio_cfg_pin(int gpio, int cfg) /  Configure a gpio pin
 * @param[in] gpio	gpio id
 * @param[in] cfg	a mode of gpio pin(input/output/function/irq)
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 */
int gpio_cfg_pin(int gpio, int cfg)
{
	struct gpio_bank *gb;
	unsigned port, value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = __raw_readl(gb->base + GPIO_CON);
	value &= ~CON_MASK(port);
	value |= CON_SFR(port, cfg);

	__raw_writel(value, gb->base + GPIO_CON);

	return 0;
}

/**
 * @brief int gpio_cfg_get_pin(int gpio) / Configure a gpio get pin
 * @param[in] gpio	gpio id
 * @return    == 0: input
 *            == 1: output
 *            == f: eint
 *            >= 2: function
 *            < 0: error
 */
int gpio_cfg_get_pin(int gpio)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = __raw_readl(gb->base + GPIO_CON);

	return ((value >> (port << 2)) & 0xF);
}

/**
 * @brief int gpio_direction_output(int gpio, int high) / Configure a direction of gpio pin as output and set an initial data
 * @param[in] gpio	gpio id
 * @param[in] high	a value of gpio pin(low/high)
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_direction_output(int gpio, int high)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = __raw_readl(gb->base + GPIO_DAT);
	value &= ~(1 << port);
	if (high) {
		value |= (1 << port);
	}

	__raw_writel(value, gb->base + GPIO_DAT);

	return gpio_cfg_pin(gpio, GPIO_OUTPUT);
}

/**
 * @brief  int gpio_direction_input(int gpio) / Configure a direction of gpio pin as input
 * @fn        int gpio_direction_input(int gpio)
 * @param[in] gpio	gpio id
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_direction_input(int gpio)
{
	return gpio_cfg_pin(gpio, GPIO_INPUT);
}

/**
 * @brief int gpio_set_value(int gpio, int high) / Set a data value of gpio pin
 * @param[in] gpio	gpio id
 * @param[in] high	a value of gpio pin(low/high)
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_set_value(int gpio, int high)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = __raw_readl(gb->base + GPIO_DAT);
	value &= ~(1 << port);
	if (high) {
		value |= (1 << port);
	}

	__raw_writel(value, gb->base + GPIO_DAT);

	return 0;
}

/**
 * @brief  int gpio_get_value(int gpio) / Get a data value of gpio pin
 * @fn        int gpio_get_value(int gpio)
 * @param[in] gpio	gpio id
 * @return    == 1: gpio read as high
 *            == 0: gpio read as low
 *             < 0: error
 *
 */
int gpio_get_value(int gpio)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = __raw_readl(gb->base + GPIO_DAT);

	return (value >> port) & 1;
}

/**
 * @brief  int gpio_set_pull(int gpio, int mode) / Set a gpio pin as pull up or pull-down
 * @param[in] gpio	gpio id
 * @param[in] mode	gpio pin mode(pull up/pull down)
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_set_pull(int gpio, int mode)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = __raw_readl(gb->base + GPIO_PUD);
	value &= ~PULL_MASK(port);

	switch (mode) {
	case GPIO_PULL_DOWN:
	case GPIO_PULL_UP:
		value |= PULL_MODE(port, mode);
		break;
	case GPIO_PULL_NONE:
		/* do nothing: bit is cleared */
		break;
	default:
		return -EINVAL;
	}

	__raw_writel(value, gb->base + GPIO_PUD);

	return 0;
}

/**
 * @brief  int gpio_get_pull(int gpio) / Set a gpio pin as pull up or pull-down
 * @param[in] gpio	gpio id
 * @return    >= 0: pull up down data
 *
 */
int gpio_get_pull(int gpio)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = __raw_readl(gb->base + GPIO_PUD);
	value &= PULL_MASK(port);

	return value;
}

/**
 *
 * @brief  int gpio_set_drv(int gpio, int mode) / Set drive strength of gpio pin
 * @param[in] gpio	gpio id
 * @param[in] mode	strength of gpio pin mode
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_set_drv(int gpio, int mode)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = __raw_readl(gb->base + GPIO_DRVSR);
	value &= ~DRV_MASK(port);

	switch (mode) {
	case GPIO_DRV_1X:
	case GPIO_DRV_2X:
	case GPIO_DRV_3X:
	case GPIO_DRV_4X:
		value |= DRV_SET(port, mode);
		break;
	default:
		return -EINVAL;
	}

	__raw_writel(value, gb->base + GPIO_DRVSR);

	return 0;
}

/**
 *
 * @brief  int gpio_get_drv(int gpio) / Get drive strength of gpio pin
 * @param[in] gpio	gpio id
 * @return    <= 0: strength of gpio pin mode
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_get_drv(int gpio)
{
	struct gpio_bank *gb;
	unsigned port, value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = __raw_readl(gb->base + GPIO_DRVSR);
	value &= ~DRV_MASK(port);

	return value;
}

/**
 *
 * @brief int gpio_set_rate(int gpio, int mode) / Set a slew rate of gpio pin
 * @param[in] gpio	gpio id
 * @param[in] mode	slew rate of a gpio pin
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_set_rate(int gpio, int mode)
{
	struct gpio_bank *gb;
	unsigned port, value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = __raw_readl(gb->base + GPIO_DRVSR);
	value &= ~RATE_MASK(port);

	switch (mode) {
	case GPIO_DRV_FAST:
	case GPIO_DRV_SLOW:
		value |= RATE_SET(port);
		break;
	default:
		return -EINVAL;
	}

	__raw_writel(value, gb->base + GPIO_DRVSR);

	return 0;
}

/**
 *
 * @brief int gpio_cfg_pin_pdn(int gpio, int cfg) / Configure a power down mode of gpio pin
 * @param[in] gpio	gpio id
 * @param[in] cfg	configuration a power down mode of a gpio pin
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_cfg_pin_pdn(int gpio, int cfg)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = __raw_readl(gb->base + GPIO_CONPDN);
	value &= ~CONPDN_MASK(port);
	value |= CONPDN_SFR(port, cfg);

	__raw_writel(value, gb->base + GPIO_CONPDN);

	return 0;
}

/**
 *
 * @brief  int gpio_set_pull_pdn(int gpio, int mode) / Configure a gpio pin as pull-up or pull-down for power down mode
 * @param[in] gpio	gpio id
 * @param[in] mode	gpio pin mode(pull up/pull down)
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_set_pull_pdn(int gpio, int mode)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = __raw_readl(gb->base + GPIO_PUDPDN);
	value &= ~PUDPDN_MASK(port);

	switch (mode) {
	case GPIO_PULL_DOWN:
	case GPIO_PULL_UP:
		value |= PUDPDN_MODE(port, mode);
		break;
	default:
		return -EINVAL;
	}

	__raw_writel(value, gb->base + GPIO_PUDPDN);

	return 0;
}

/**
 *
 * @brief  void gpio_dump(int gpio) / Print a information of a gpio pin
 * @param[in] gpio	gpio id
 * @return    No return
 *
 */
void gpio_dump(int gpio)
{
	struct gpio_bank *gb = gpio_to_bank(gpio);
	int port = s5j_gpio_port(gpio);
	unsigned int con;
	unsigned int dat;
	unsigned int pull;
	unsigned int drv;
	unsigned int conpdn;
	unsigned int pudpdn;

	con = __raw_readl(gb->base + GPIO_CON) >> (port * 4);
	con &= 0xf;

	dat = __raw_readl(gb->base + GPIO_DAT) >> port;
	dat &= 1;

	pull = __raw_readl(gb->base + GPIO_PUD) >> (port * 2);
	pull &= 0x3;

	drv = __raw_readl(gb->base + GPIO_DRVSR) >> (port * 2);
	drv &= 0x3;

	conpdn = __raw_readl(gb->base + GPIO_CONPDN) >> (port * 2);
	conpdn &= 0x3;

	pudpdn = __raw_readl(gb->base + GPIO_PUDPDN) >> (port * 2);
	pudpdn &= 0x3;

#if defined(CONFIG_S5J_GPIO_LOG_DUMP)
	cprintf("%s.%d: con=%x (%s), dat=%s, pull=%s, drv=%s, conpdn=%x (%s), pudpdn=%s\n", gb->name, port, con, con == 0 ? "input" : (con == 1 ? "output" : "function"), dat ? "high" : "low", pull_str[pull], drv_str[drv], conpdn, conpdn == 0 ? "output0" : (conpdn == 1 ? "output1" : (conpdn == 2 ? "input" : "previous state")), pull_str[pudpdn]);
#endif
}

/**
 *
 * @brief int gpio_eint_mask(int gpio) / Mask a gpio external interrupt
 * @param[in] gpio	gpio id
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_eint_mask(int gpio)
{
	u32 mask;
	void *eint_addr;
	int port;

	if (!gpio_valid(gpio)) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);
	eint_addr = __gpio_eint_get_addr(gpio, GPIO_EINT_MASK);

	mask = __raw_readl(eint_addr);
	mask |= 1 << port;
	__raw_writel(mask, eint_addr);

	return 0;
}

/**
 *
 * @brief  int gpio_eint_unmask(int gpio) / Unmask a gpio external interrupt
 * @param[in] gpio	gpio id
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_eint_unmask(int gpio)
{
	u32 mask;
	void *eint_addr;
	int port;

	if (!gpio_valid(gpio)) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);
	eint_addr = __gpio_eint_get_addr(gpio, GPIO_EINT_MASK);

	mask = __raw_readl(eint_addr);
	mask &= ~(1 << port);
	__raw_writel(mask, eint_addr);

	return 0;
}

/**
 *
 * @brief bool gpio_eint_ispending(int gpio) / Check if a gpio external interrupt is pending
 * @param[in] gpio	gpio id
 * @return    == true: gpio external interrupt is pending
 *            == false: gpio external interrupt is not pending
 *
 */
bool gpio_eint_ispending(int gpio)
{
	u32 pend;
	void *eint_addr;
	int port;

	port = s5j_gpio_port(gpio);
	eint_addr = __gpio_eint_get_addr(gpio, GPIO_EINT_PEND);

	pend = __raw_readl(eint_addr);

	return (pend & (1 << port)) ? true : false;
}

/**
 *
 * @brief  int gpio_eint_clear_pending(int gpio) / Clear a gpio external interrupt pending
 * @param[in] gpio	gpio id
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_eint_clear_pending(int gpio)
{
	u32 pend;
	void *eint_addr;
	int port;

	if (!gpio_valid(gpio)) {
		return -EINVAL;
	}

	if (!gpio_eint_ispending(gpio)) {
		/* cprintf("%s bank %d port is not pending\n",gpio_bank_name(gpio),s5j_gpio_port(gpio)); */
		/* err("eint%d: port '%d' is not pending.\n"); */
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);
	eint_addr = __gpio_eint_get_addr(gpio, GPIO_EINT_PEND);

	pend = __raw_readl(eint_addr);
	pend &= 1 << port;
	__raw_writel(pend, eint_addr);

	return 0;
}

/**
 *
 * @brief  int gpio_eint_enable_filter(int gpio) / Enable a gpio external interrupt filter
 * @param[in] gpio	gpio id
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_eint_enable_filter(int gpio)
{
	u32 filter_con;
	unsigned shift_port;
	void *eint_addr;
	int port;

	if (!gpio_valid(gpio)) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);
	eint_addr = __gpio_eint_get_addr(gpio, GPIO_EINT_FLTCON);
	if (port < 4) {
		shift_port = 8 * port;
	} else {
		shift_port = 8 * (port - 4);
	}

	filter_con = __raw_readl(eint_addr);
	filter_con |= (1 << 7) << shift_port;
	__raw_writel(filter_con, eint_addr);

	return 0;
}

/**
 *
 * @brief int gpio_eint_disable_filter(int gpio) / Disable a gpio external interrupt filter
 * @param[in] gpio	gpio id
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_eint_disable_filter(int gpio)
{
	u32 filter_con;
	unsigned shift_port;
	void *eint_addr;
	int port;

	if (!gpio_valid(gpio)) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);
	eint_addr = __gpio_eint_get_addr(gpio, GPIO_EINT_FLTCON);
	if (port < 4) {
		shift_port = 8 * port;
	} else {
		shift_port = 8 * (port - 4);
	}

	filter_con = __raw_readl(eint_addr);
	filter_con &= ~((1 << 7) << shift_port);
	__raw_writel(filter_con, eint_addr);

	return 0;
}

/**
 *
 * @brief  int gpio_eint_set_filter(int gpio, unsigned type, unsigned width) / Configure type and width of a gpio external interrupt filter
 * @param[in] gpio	gpio id
 * @param[in] type	filter type(delay/digital)
 * @param[in] width	filter width(it will be ignored when filter type is delay)
 * @return    == 0: success
 *            == -EINVAL: invalid gpio
 *
 */
int gpio_eint_set_filter(int gpio, unsigned type, unsigned width)
{
	u32 filter_con;
	unsigned shift_port;
	void *eint_addr;
	int port;
	int bank;

	if (!gpio_valid(gpio)) {
		return -EINVAL;
	}

	switch (type) {
	case EINT_FILTER_DELAY:
	case EINT_FILTER_DIGITAL:
		break;
	default:
		/* err("No such eint filter type %d", type); */
		return -EINVAL;
	}

	bank = s5j_gpio_bank(gpio);
	port = s5j_gpio_port(gpio);
	eint_addr = __gpio_eint_get_addr(gpio, GPIO_EINT_FLTCON);
	if (port < 4) {
		shift_port = 8 * port;
	} else {
		shift_port = 8 * (port - 4);
	}

	filter_con = __raw_readl(eint_addr);

	if (bank < 4) {				/* Alive Filter Setting */
		if (type == EINT_FILTER_DELAY) {
			filter_con &= ~((1 << 6) << shift_port);
		} else if (type == EINT_FILTER_DIGITAL) {
			filter_con |= (1 << 6) << shift_port;
		}

		filter_con |= ((1 << 7) << shift_port);	/* Filter Enable */
		filter_con &= ~(0x3f << shift_port);
		filter_con |= width << shift_port;
	}

	else {
		filter_con |= ((1 << 7) << shift_port);	/* Filter Enable */
		filter_con &= ~(0x7f << shift_port);
		filter_con |= width << shift_port;
	}
	__raw_writel(filter_con, eint_addr);

	return 0;
}

/**
 *
 * @brief  int gpio_eint_set_type(int gpio, unsigned type) / Configure type of a gpio external interrurt
 * @param[in] gpio	gpio id
 * @param[in] type	eint interrupt type (edge/level)
 * @return == 0: success
 *         == -EINVAL: invalid gpio
 *
 */
int gpio_eint_set_type(int gpio, unsigned type)
{
	u32 ctrl, mask;
	void *eint_addr;
	int port;

	if (!gpio_valid(gpio)) {
		return -EINVAL;
	}

	switch (type) {
	case EINT_TYPE_LEVEL_LOW:
	case EINT_TYPE_LEVEL_HIGH:
	case EINT_TYPE_EDGE_FALLING:
	case EINT_TYPE_EDGE_RISING:
	case EINT_TYPE_EDGE_BOTH:
		break;

	default:
		/* err("No such irq type %d", type); */
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);
	eint_addr = __gpio_eint_get_addr(gpio, GPIO_EINT_CON);
	mask = 0x7 << (port * 4);
	ctrl = __raw_readl(eint_addr);
	ctrl &= ~mask;
	ctrl |= type << (port * 4);
	__raw_writel(ctrl, eint_addr);

	if (gpio_eint_ispending(gpio)) {
		gpio_eint_clear_pending(gpio);
	}

	return 0;
}

/**
 *
 * @brief  int gpio_eint_get_type(int gpio) / Get Eage type of a gpio external interrurt
 * @param[in] gpio	gpio id
 * @return >= 0: eint interrupt type (edge/level)
 *         == -EINVAL: invalid gpio
 *
 */
int gpio_eint_get_type(int gpio)
{
	u32 ctrl, mask;
	void *eint_addr;
	int port;

	if (!gpio_valid(gpio)) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);
	eint_addr = __gpio_eint_get_addr(gpio, GPIO_EINT_CON);
	mask = 0x7 << (port * 4);
	ctrl = __raw_readl(eint_addr);
	ctrl &= mask;
	ctrl = ctrl >> (port * 4);

	return ctrl;
}

/**
 *
 * @brief  int  s5j_gpio_open(FAR struct gpio_dev_s *dev) / GPIO Driver Open
 * @param[in] dev	Device
 * @return == 0: Open success
 *         == -EINVAL: invalid gpio
 *
 */
int s5j_gpio_open(FAR struct gpio_dev_s *dev)
{
	int idx = ((struct s5j_gpio_priv *)(dev->priv))->idx;
	if (!isinit) {
		up_create_gpio(idx);
	}
	return OK;
}

/**
 *
 * @brief  int  s5j_gpio_close(FAR struct gpio_dev_s *dev) / GPIO Driver close
 * @param[in] dev	Device
 * @return == 0: close success
 *         == -EINVAL: invalid gpio
 *
 */
int s5j_gpio_close(FAR struct gpio_dev_s *dev)
{
	struct s5j_gpio_priv *priv = (struct s5j_gpio_priv *)(dev->priv);
	int idx = priv->idx;
	s5j_gpio_disable_irq(&s5j_gpio_all[idx]);

	if (NULL != dev->wdog) {
		wd_cancel(dev->wdog);
		dev->wdog = NULL;
	}

	return OK;
}

/**
 *
 * @brief  int  s5j_gpio_get(FAR struct gpio_dev_s *dev) / Get GPIO Value
 * @param[in] dev	Device
 * @return >= 0: gpio value
 *         == -EINVAL: invalid gpio
 *
 */
int s5j_gpio_get(FAR struct gpio_dev_s *dev)
{
	int gpio = ((struct s5j_gpio_priv *)(dev->priv))->gpio;
	if (!gpio_valid(gpio)) {
		return -EINVAL;
	}
	return gpio_get_value(gpio);
}

/**
 *
 * @brief  void  s5j_gpio_set(FAR struct gpio_dev_s *dev) / Set GPIO Value
 * @param[in] dev	Device
 * @return    No return
 *
 */
void s5j_gpio_set(FAR struct gpio_dev_s *dev, FAR unsigned int value)
{
	int gpio = ((struct s5j_gpio_priv *)(dev->priv))->gpio;
	gpio_set_value(gpio, value);
}

/**
 *
 * @brief  int  s5j_gpio_ctrl(struct gpio_dev_s *dev, int cmd, unsigned long args) / GPIO IOCTL Control
 * @param[in] dev	Device
 * @param[in] cmd	ioctl command
 * @param[in] arg	ioctl arg
 * @return >= 0: gpio eage value or OK
 *         == -EINVAL: invalid gpio
 *
 */

int s5j_gpio_ctrl(struct gpio_dev_s *dev, int cmd, unsigned long args)
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

/**
 *
 * @brief  void up_gpioinitialize(void) / GPIO initailize for Device Driver
 * @return no return
 *
 */
void up_gpioinitialize(void)
{
	int i;

	for (i = 0; i < NUM_GPIO; i++) {
		up_create_gpio(i);
	}
	isinit = OK;
}

/**
 *
 * @brief  int up_destroy_gpio(int32_t idx) / GPIO Destroy for Device Driver
 * @return == 0 return OK
 *
 */
int up_destroy_gpio(int32_t idx)
{
	struct s5j_gpio_priv *priv = (struct s5j_gpio_priv *)(s5j_gpio_all[idx].priv);
	if (priv) {
		kmm_free(priv);
	}
	s5j_gpio_all[idx].ops = NULL;
	s5j_gpio_all[idx].priv = NULL;

	if (s5j_gpio_all[idx].wdog) {
		wd_cancel(s5j_gpio_all[idx].wdog);
	}

	return OK;
}
