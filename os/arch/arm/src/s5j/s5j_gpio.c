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

/****************************************************************************
 * Private Functions prototypes
 ****************************************************************************/
static void *__gpio_to_eint_base(int gpio);
static unsigned __gpio_to_eint_bank(int gpio);
static void *__gpio_eint_filter_get_addr(int gpio);
static void *__gpio_eint_get_addr(int gpio, unsigned offset);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/
static struct gpio_bank s5jt200_gpio_bank[] = {
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
		.isr_num = {
			IRQ_WEINT_GPG10,
			IRQ_WEINT_GPG11,
			IRQ_WEINT_GPG12,
			IRQ_WEINT_GPG13,
			IRQ_WEINT_GPG14,
			IRQ_WEINT_GPG15,
			IRQ_WEINT_GPG16,
			IRQ_WEINT_GPG17,
		},
		.group_type = GPIO_GROUP_COMMON,
	},
	[GPG2] = {
		.name = "GPG2",
		.base = (void *)(GPIO_CON_BASE + 0xc0),
		.nr_port = 8,
		.isr_num = {
			IRQ_WEINT_GPG20,
			IRQ_WEINT_GPG21,
			IRQ_WEINT_GPG22,
			IRQ_WEINT_GPG23,
			IRQ_WEINT_GPG24,
			IRQ_WEINT_GPG25,
			IRQ_WEINT_GPG26,
			IRQ_WEINT_GPG27,
		},
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
		.isr_num = {
			IRQ_EINT0,
			IRQ_EINT1,
			IRQ_EINT2,
		},
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

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: __gpio_to_eint_base
 *
 * Description:
 *  Converts gpio port index into eint base address
 *
 * Input Parameters:
 *  gpio - port id
 *
 * Returned Value:
 *  eint base address
 ****************************************************************************/
static void *__gpio_to_eint_base(int gpio)
{
	struct gpio_bank *gb;
	gb = gpio_to_bank(gpio);

	if (!gb) {
		return NULL;
	}
	return gb->base;
}

/****************************************************************************
 * Name: __gpio_to_eint_bank
 *
 * Description:
 *  Converts gpio port index into eint bank
 *
 * Input Parameters:
 *  gpio - port id
 *
 * Returned Value:
 *  eint bank
 ****************************************************************************/
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

/****************************************************************************
 * Name: __gpio_eint_filter_get_addr
 *
 * Description:
 *  Returns EINT filter register address
 *
 * Input Parameters:
 *  gpio - port id
 *
 * Returned Value:
 *  Address
 ****************************************************************************/
static void *__gpio_eint_filter_get_addr(int gpio)
{
	void *gpio_filter_addr;
	struct gpio_bank *gb = gpio_to_bank(gpio);

	gpio_filter_addr = (void *)(gb->filter_offset_addr);

	return gpio_filter_addr;
}

/****************************************************************************
 * Name: __gpio_eint_get_addr
 *
 * Description:
 *  Returns EINT CON/FLTCON/MASK/PEND register address
 *
 * Input Parameters:
 *  gpio - port id
 *  offset - EINT CON/FLTCON/MASK/PEND offset addr
 *
 * Returned Value:
 *  Address
 ****************************************************************************/
static void *__gpio_eint_get_addr(int gpio, unsigned offset)
{
	void *gpio_base;
	void *eint_addr;
	unsigned bank_num;
	int port;

	gpio_base = __gpio_to_eint_base(gpio);
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

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: gpio_valid
 *
 * Description:
 *  Check if given gpio is in a valid range
 *
 * Input Parameters:
 *  gpio - port id
 *
 * Returned Value:
 *  == 1: valid gpio
 *  == 0: invalid gpio
 *
 ****************************************************************************/
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

/****************************************************************************
 * Name: gpio_cfg_pin
 *
 * Description:
 *  Set gpio pin configuration
 *
 * Input Parameters:
 *  gpio - port id
 *  cfg	a mode of gpio pin(input/output/function/irq)
 *
 * Returned Value:
 *  == 0: success
 *  == -EINVAL: invalid gpio
 *
 ****************************************************************************/
int gpio_cfg_pin(int gpio, int cfg)
{
	struct gpio_bank *gb;
	unsigned port, value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = getreg32(gb->base + GPIO_CON);
	value &= ~CON_MASK(port);
	value |= CON_SFR(port, cfg);

	putreg32(value, gb->base + GPIO_CON);

	return 0;
}

/****************************************************************************
 * Name: gpio_cfg_get_pin
 *
 * Description:
 *  Get gpio pin configuration
 *
 * Input Parameters:
 *  gpio - port id
 *
 * Returned Value:
 *  == 0: input
 *  == 1: output
 *  == f: eint
 *   >= 2: function
 *   < 0: error
 *
 ****************************************************************************/
int gpio_cfg_get_pin(int gpio)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = getreg32(gb->base + GPIO_CON);

	return ((value >> (port << 2)) & 0xF);
}

/****************************************************************************
 * Name: gpio_direction_output
 *
 * Description:
 *  Configure a direction of gpio pin as output and set an initial data
 *
 * Input Parameters:
 *  gpio - port id
 *  high - a value of gpio pin(low/high)
 *
 * Returned Value:
 *  == 0: success
 *  == -EINVAL: invalid gpio
 *
 ****************************************************************************/
int gpio_direction_output(int gpio, int high)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = getreg32(gb->base + GPIO_DAT);
	value &= ~(1 << port);
	if (high) {
		value |= (1 << port);
	}

	putreg32(value, gb->base + GPIO_DAT);

	return gpio_cfg_pin(gpio, GPIO_OUTPUT);
}

/****************************************************************************
 * Name: gpio_direction_input
 *
 * Description:
 *  Configure a direction of gpio pin as input
 *
 * Input Parameters:
 *  gpio - port id
 *
 * Returned Value:
 *  == 0: success
 *  == -EINVAL: invalid gpio
 *
 ****************************************************************************/
int gpio_direction_input(int gpio)
{
	return gpio_cfg_pin(gpio, GPIO_INPUT);
}

/****************************************************************************
 * Name: gpio_set_value
 *
 * Description:
 *  Set a data value of gpio pin
 *
 * Input Parameters:
 *  gpio - port id
 *  high - value of gpio pin(low/high)
 *
 * Returned Value:
 *  == 0: success
 *  == -EINVAL: invalid gpio
 *
 ****************************************************************************/
int gpio_set_value(int gpio, int high)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = getreg32(gb->base + GPIO_DAT);
	value &= ~(1 << port);
	if (high) {
		value |= (1 << port);
	}

	putreg32(value, gb->base + GPIO_DAT);

	return 0;
}

/****************************************************************************
 * Name: gpio_get_value
 *
 * Description:
 *  Get a data value of gpio pin
 *
 * Input Parameters:
 *  gpio - port id
 *
 * Returned Value:
 *  == 1: gpio read as high== 0: success
 *  == 0: gpio read as low == -EINVAL: invalid gpio
 *   < 0: error
 ****************************************************************************/
int gpio_get_value(int gpio)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = getreg32(gb->base + GPIO_DAT);

	return (value >> port) & 1;
}

/****************************************************************************
 * Name: gpio_set_pull
 *
 * Description:
 *  Set a gpio pin as pull up or pull-down
 *
 * Input Parameters:
 *  gpio - port id
 *  mode - gpio pin mode(pull up/pull down)
 *
 * Returned Value:
 *  == 0: success
 *  == -EINVAL: invalid gpio
 ****************************************************************************/
int gpio_set_pull(int gpio, int mode)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = getreg32(gb->base + GPIO_PUD);
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

	putreg32(value, gb->base + GPIO_PUD);

	return 0;
}

/****************************************************************************
 * Name: gpio_get_pull
 *
 * Description:
 *  Get a gpio pin as pull up or pull-down
 *
 * Input Parameters:
 *  gpio - port id
 *
 *
 * Returned Value:
 *  >= 0: pull up down data
 *
 ****************************************************************************/
int gpio_get_pull(int gpio)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = getreg32(gb->base + GPIO_PUD);
	value &= PULL_MASK(port);

	return value;
}

/****************************************************************************
 * Name: gpio_set_drv
 *
 * Description:
 *  Set drive strength of gpio pin
 *
 * Input Parameters:
 *  gpio - port id
 *  mode - strength of gpio pin mode
 *
 * Returned Value:
 *   == 0: success
 *   == -EINVAL: invalid gpio
 ****************************************************************************/
int gpio_set_drv(int gpio, int mode)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = getreg32(gb->base + GPIO_DRVSR);
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

	putreg32(value, gb->base + GPIO_DRVSR);

	return 0;
}

/****************************************************************************
 * Name: gpio_get_drv
 *
 * Description:
 *  Get drive strength of gpio pin
 *
 * Input Parameters:
 *  gpio - port id
 *
 *
 * Returned Value:
 *   <= 0: strength of gpio pin mode
 *   == -EINVAL: invalid gpio
 ****************************************************************************/
int gpio_get_drv(int gpio)
{
	struct gpio_bank *gb;
	unsigned port, value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = getreg32(gb->base + GPIO_DRVSR);
	value &= ~DRV_MASK(port);

	return value;
}

/****************************************************************************
 * Name: gpio_set_rate
 *
 * Description:
 *  Set a slew rate of gpio pin
 *
 * Input Parameters:
 *  gpio - port id
 *  mode - slew rate of a gpio pin
 *
 * Returned Value:
 *  == 0: success
 *  == -EINVAL: invalid gpio
 ****************************************************************************/
int gpio_set_rate(int gpio, int mode)
{
	struct gpio_bank *gb;
	unsigned port, value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = getreg32(gb->base + GPIO_DRVSR);
	value &= ~RATE_MASK(port);

	switch (mode) {
	case GPIO_DRV_FAST:
	case GPIO_DRV_SLOW:
		value |= RATE_SET(port);
		break;
	default:
		return -EINVAL;
	}

	putreg32(value, gb->base + GPIO_DRVSR);

	return 0;
}

/****************************************************************************
 * Name: gpio_cfg_pin_pdn
 *
 * Description:
 *  Configure a power down mode of gpio pin
 *
 * Input Parameters:
 *  gpio - port id
 *  cfg - configuration a power down mode of a gpio pin
 *
 * Returned Value:
 *  == 0: success
 *  == -EINVAL: invalid gpio
 ****************************************************************************/
int gpio_cfg_pin_pdn(int gpio, int cfg)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = getreg32(gb->base + GPIO_CONPDN);
	value &= ~CONPDN_MASK(port);
	value |= CONPDN_SFR(port, cfg);

	putreg32(value, gb->base + GPIO_CONPDN);

	return 0;
}

/****************************************************************************
 * Name: gpio_set_pull_pdn
 *
 * Description:
 *  Configure a gpio pin as pull-up or pull-down for power down mode
 *
 * Input Parameters:
 *  gpio - port id
 *  mode - gpio pin mode(pull up/pull down)
 *
 * Returned Value:
 *  == 0: success
 *  == -EINVAL: invalid gpio
 ****************************************************************************/
int gpio_set_pull_pdn(int gpio, int mode)
{
	struct gpio_bank *gb;
	unsigned int port;
	unsigned int value;

	if (!(gb = gpio_to_bank(gpio))) {
		return -EINVAL;
	}

	port = s5j_gpio_port(gpio);

	value = getreg32(gb->base + GPIO_PUDPDN);
	value &= ~PUDPDN_MASK(port);

	switch (mode) {
	case GPIO_PULL_DOWN:
	case GPIO_PULL_UP:
		value |= PUDPDN_MODE(port, mode);
		break;
	default:
		return -EINVAL;
	}

	putreg32(value, gb->base + GPIO_PUDPDN);

	return 0;
}

/****************************************************************************
 * Name: gpio_eint_mask
 *
 * Description:
 *  Mask a gpio external interrupt
 *
 * Input Parameters:
 *  gpio - port id
 *
 * Returned Value:
 *  == 0: success
 *  == -EINVAL: invalid gpio
 ****************************************************************************/
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

	mask = getreg32(eint_addr);
	mask |= 1 << port;
	putreg32(mask, eint_addr);

	return 0;
}

/****************************************************************************
 * Name: gpio_eint_unmask
 *
 * Description:
 *  Unmask a gpio external interrupt
 *
 * Input Parameters:
 *  gpio - port id
 *
 * Returned Value:
 *  == 0: success
 *  == -EINVAL: invalid gpio
 ****************************************************************************/
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

	mask = getreg32(eint_addr);
	mask &= ~(1 << port);
	putreg32(mask, eint_addr);

	return 0;
}

/****************************************************************************
 * Name: gpio_eint_ispending
 *
 * Description:
 *  Check if gpio external interrupt is pending
 *
 * Input Parameters:
 *  gpio - port id
 *
 * Returned Value:
 *  == true: gpio external interrupt is pending
 *  == false: gpio external interrupt is not pending
 ****************************************************************************/
bool gpio_eint_ispending(int gpio)
{
	u32 pend;
	void *eint_addr;
	int port;

	port = s5j_gpio_port(gpio);
	eint_addr = __gpio_eint_get_addr(gpio, GPIO_EINT_PEND);

	pend = getreg32(eint_addr);

	return (pend & (1 << port)) ? true : false;
}

/****************************************************************************
 * Name: gpio_eint_clear_pending
 *
 * Description:
 *   Clear  gpio pending external interrupt
 *
 * Input Parameters:
 *   gpio - port id
 *
 * Returned Value:
 *   >= 0: success
 *   == -EINVAL: invalid gpio
 ****************************************************************************/
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

	pend = getreg32(eint_addr);
	pend &= 1 << port;
	putreg32(pend, eint_addr);

	return 0;
}

/****************************************************************************
 * Name: gpio_eint_enable_filter
 *
 * Description:
 *   Enable gpio external interrupt filter
 *
 * Input Parameters:
 *   gpio - port id
 *
 * Returned Value:
 *   >= 0: success
 *   == -EINVAL: invalid gpio
 ****************************************************************************/
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

	filter_con = getreg32(eint_addr);
	filter_con |= (1 << 7) << shift_port;
	putreg32(filter_con, eint_addr);

	return 0;
}

/****************************************************************************
 * Name: gpio_eint_disable_filter
 *
 * Description:
 *   Disable a gpio external interrupt filter
 *
 * Input Parameters:
 *   gpio - port id
 *
 * Returned Value:
 *   >= 0: success
 *   == -EINVAL: invalid gpio
 ****************************************************************************/
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

	filter_con = getreg32(eint_addr);
	filter_con &= ~((1 << 7) << shift_port);
	putreg32(filter_con, eint_addr);

	return 0;
}

/****************************************************************************
 * Name: gpio_eint_set_filter
 *
 * Description:
 *   Configure type and width of a gpio external interrupt filter
 *
 * Input Parameters:
 *   gpio - port id
 *   type - filter type(delay/digital)
 *   width - filter width(it will be ignored when filter type is delay)
 *
 * Returned Value:
 *   >= 0: success
 *   == -EINVAL: invalid gpio
 ****************************************************************************/
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

	filter_con = getreg32(eint_addr);

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
	putreg32(filter_con, eint_addr);

	return 0;
}

/****************************************************************************
 * Name: gpio_eint_set_type
 *
 * Description:
 *   Set edge/level type of a gpio external interrurt
 *
 * Input Parameters:
 *   gpio - port id
 *   type - eint interrupt type (edge/level)
 *
 * Returned Value:
 *   >= 0: success
 *   == -EINVAL: invalid gpio
 ****************************************************************************/
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
	ctrl = getreg32(eint_addr);
	ctrl &= ~mask;
	ctrl |= type << (port * 4);
	putreg32(ctrl, eint_addr);

	if (gpio_eint_ispending(gpio)) {
		gpio_eint_clear_pending(gpio);
	}

	return 0;
}

/****************************************************************************
 * Name: gpio_eint_get_type
 *
 * Description:
 *   Get edge/level type of a gpio external interrurt
 *
 * Input Parameters:
 *   gpio - port id
 *
 * Returned Value:
 *   >= 0: eint interrupt type (edge/level)
 *   == -EINVAL: invalid gpio
 ****************************************************************************/
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
	ctrl = getreg32(eint_addr);
	ctrl &= mask;
	ctrl = ctrl >> (port * 4);

	return ctrl;
}

/****************************************************************************
 * Name: gpio_to_bank
 *
 * Description:
 *  Converts gpio port index into gpio bank structure address
 *
 * Input Parameters:
 *  gpio - port id
 *
 * Returned Value:
 *  gpio bank structure address
 ****************************************************************************/
struct gpio_bank *gpio_to_bank(int gpio)
{
	unsigned bank;

	bank = s5j_gpio_bank(gpio);

	if (!gpio_valid(gpio)) {
		return NULL;
	}

	return s5jt200_gpio_bank + bank;
}

static int gic_interrupt(int irq, FAR void *context, FAR void *arg)
{
	int gpio_irq;
	uint32_t intpnd;

	if (irq == IRQ_GPIO_GIC_0) {
		irq = IRQ_WEINT_GPG10;
		intpnd = getreg32(0x80040900);
	} else {
		irq = IRQ_WEINT_GPG20;
		intpnd = getreg32(0x80040904);
	}

	/* Dispatch each GPIO interrupt */
	for (gpio_irq = 0; intpnd; gpio_irq++) {
		if (intpnd & (1 << gpio_irq)) {
			irq_dispatch(irq + gpio_irq, context);
			intpnd &= ~(1 << gpio_irq);
		}
	}

	return OK;
}

void s5j_gpio_irqinitialize(void)
{
	/* Attach the GPIO interrupt handler for second-level irq decoding */
	DEBUGVERIFY(irq_attach(IRQ_GPIO_GIC_0, gic_interrupt, NULL));
	DEBUGVERIFY(irq_attach(IRQ_GPIO_GIC_1, gic_interrupt, NULL));

	up_enable_irq(IRQ_GPIO_GIC_0);
	up_enable_irq(IRQ_GPIO_GIC_1);
}
