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
 * arch/arm/src/s5j/s5j_gpio.c
 *
 *   Copyright (C) 2009, 2011-2012 Gregory Nutt. All rights reserved.
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

#include "up_arch.h"
#include "s5j_gpio.h"

/****************************************************************************
 * Pre-processor definitions
 ****************************************************************************/
#define DEFAULT_INPUT (GPIO_INPUT | GPIO_PULLDOWN)

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* Base addresses for each GPIO block */
static const uint32_t g_gpiobase[S5J_GPIO_NPORTS] = {
	S5J_GPIO_GPP0_BASE,
	S5J_GPIO_GPP1_BASE,
	S5J_GPIO_GPP2_BASE,
	S5J_GPIO_GPP3_BASE,
	S5J_GPIO_GPG0_BASE,
	S5J_GPIO_GPG1_BASE,
	S5J_GPIO_GPG2_BASE,
	S5J_GPIO_GPG3_BASE,
	S5J_GPIO_GPA0_BASE,
	S5J_GPIO_GPA1_BASE,
	S5J_GPIO_GPA2_BASE,
	S5J_GPIO_GPA3_BASE,
	S5J_GPIO_GPP4_BASE,
};

/* GPG1, GPG2 and GPA0 provides interrupt. */
static const uint32_t g_intbase[S5J_GPIO_NPORTS] = {
	0,
	0,
	0,
	0,
	0,
	S5J_GPIOINT_CON_GPG1,
	S5J_GPIOINT_CON_GPG2,
	0,
	S5J_GPIOINT_CON_GPA0,
	0,
	0,
	0,
	0,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: s5j_setdrv
 *
 * Description:
 *   Set drive strength of a GPIO pin.
 *
 ****************************************************************************/
static void s5j_setdrv(uint32_t cfgset, unsigned int port, unsigned int pin)
{
	uint32_t cfg;
	uint32_t base = g_gpiobase[port];

	/* CAVEAT: GPIO_FAST|SLOWXXX is compatible with GPIO_DRV_XXX */
	cfg = (cfgset & GPIO_DRVSTR_MASK) >> GPIO_DRVSTR_SHIFT;

	modifyreg32(base + S5J_GPIO_DRV_OFFSET, GPIO_DRV_PIN_MASK(pin),
				cfg << GPIO_DRV_PIN_SHIFT(pin));
}

/****************************************************************************
 * Name: s5j_pullup
 *
 * Description:
 *   Set pull-up/down on a GPIO pin.
 *
 ****************************************************************************/
static void s5j_pullup(uint32_t cfgset, unsigned port, unsigned int pin)
{
	uint32_t cfg;
	uint32_t base = g_gpiobase[port];

	/* CAVEAT: GPIO_PUPD_XXX is compatible with GPIO_PUD_XXX */
	cfg = (cfgset & GPIO_PUPD_MASK) >> GPIO_PUPD_SHIFT;

	modifyreg32(base + S5J_GPIO_PUD_OFFSET, GPIO_PUD_PIN_MASK(pin),
				cfg << GPIO_PUD_PIN_SHIFT(pin));
}

/****************************************************************************
 * Name: s5j_setintedge
 *
 * Description:
 *
 ****************************************************************************/
static inline void s5j_setintedge(unsigned int port, unsigned int pin, int edge)
{
	DEBUGASSERT(g_intbase[port] != 0);

	modifyreg32(g_intbase[port], GPIOINT_CON_MASK(pin),
				edge << GPIOINT_CON_SHIFT(pin));
}

/****************************************************************************
 * Name: s5j_setintmask
 *
 * Description:
 *   Mask or unmask interrupts from GPIO pins.
 *
 ****************************************************************************/
static inline void s5j_setintmask(unsigned int port, unsigned int pin,
								  int mask)
{
	uint32_t regbase;

	/* Only GPG1, GPG2 and GPA0 can be configured as EINT */
	if (g_gpiobase[port] == S5J_GPIO_GPG1_BASE) {
		regbase = S5J_GPIOINT_MASK_GPG1;
	} else if (g_gpiobase[port] == S5J_GPIO_GPG2_BASE) {
		regbase = S5J_GPIOINT_MASK_GPG2;
	} else if (g_gpiobase[port] == S5J_GPIO_GPA0_BASE) {
		regbase = S5J_GPIOINT_MASK_GPA0;
	} else {
		return;
	}

	modifyreg32(regbase, GPIOINT_MASK_MASK(pin),
				mask << GPIOINT_CON_SHIFT(pin));
}

/****************************************************************************
 * Name: s5j_configinput
 *
 * Description:
 *   COnfigure a GPIO input pin based on bit-encoded description of the pin.
 *
 ****************************************************************************/
static inline int s5j_configinput(uint32_t cfgset, unsigned int port,
								  unsigned int pin)
{
	uint32_t base;
	uint32_t intbase;

	base = g_gpiobase[port];

	/* Set as input */
	modifyreg32(base + S5J_GPIO_CON_OFFSET, GPIO_CON_PIN_MASK(pin),
				GPIO_CON_INPUT << GPIO_CON_PIN_SHIFT(pin));

	/* Disable all interrupt configurations that this pin might have. */
	intbase = g_intbase[port];
	if (intbase != 0) {
		s5j_setintmask(port, pin, 1);
		s5j_setintedge(port, pin, 0);
	}

	/* Set pull-up/down */
	s5j_pullup(cfgset, port, pin);

	return OK;
}

/****************************************************************************
 * Name: s5j_configalt
 *
 * Description:
 *   Configure a GPIO alternate function pin based on bit-encoded
 *   description of the pin.
 *
 ****************************************************************************/
static int s5j_configalt(uint32_t cfgset, unsigned int port,
						 unsigned int pin, uint32_t alt)
{
	uint32_t base = g_gpiobase[port];

	/*
	 * First, configure the port as a generic input so that we have
	 * a known starting point and consistent behavior during the
	 * re-configuration.
	 */
	s5j_configinput(DEFAULT_INPUT, port, pin);

	modifyreg32(base + S5J_GPIO_CON_OFFSET, GPIO_CON_PIN_MASK(pin),
				alt << GPIO_CON_PIN_SHIFT(pin));

	/* Set pull-up mode */
	s5j_pullup(cfgset, port, pin);

	/* Set drive strength */
	s5j_setdrv(cfgset, port, pin);

	return OK;
}

/****************************************************************************
 * Name: s5j_configoutput
 *
 * Description:
 *   Configure a GPIO output pin based on bit-encoded description of the pin.
 *
 ****************************************************************************/
static inline int s5j_configoutput(uint32_t cfgset, unsigned int port,
								   unsigned int pin)
{
	/*
	 * First, configure the port as a generic input so that we have
	 * a known starting point and conssitent behavior during the
	 * re-configuration.
	 */
	s5j_configinput(DEFAULT_INPUT, port, pin);

	/* Set the initial value of the output */
	s5j_gpiowrite(cfgset, (cfgset & GPIO_VALUE_MASK) ^ GPIO_VALUE_ZERO);

	/* Now, reconfigure the pin as an output */
	modifyreg32(g_gpiobase[port] + S5J_GPIO_CON_OFFSET,
				GPIO_CON_PIN_MASK(pin),
				GPIO_CON_OUTPUT << GPIO_CON_PIN_SHIFT(pin));

	/* Set drive strength */
	s5j_setdrv(cfgset, port, pin);

	return OK;
}

/****************************************************************************
 * Name: s5j_configinterrupt
 *
 * Description:
 *   Configure a GPIO interrupt pin based on bit-encoded description
 *   of the pin
 *
 ****************************************************************************/
static inline int s5j_configinterrupt(uint32_t cfgset, unsigned int port,
									  unsigned int pin)
{
	uint32_t base = g_gpiobase[port];

	DEBUGASSERT(g_intbase[port] != 0);

	/*
	 * First, configure the port as a generic input so that we have
	 * a known starting point and consistent behavior during the
	 * re-configuration.
	 */
	s5j_configinput(cfgset, port, pin);

	/* Set as interrupt */
	modifyreg32(base + S5J_GPIO_CON_OFFSET, GPIO_CON_PIN_MASK(pin),
				GPIO_CON_EINT << GPIO_CON_PIN_SHIFT(pin));

	/* Then, just remember the rising/falling edge interrupt enabled */
	s5j_setintedge(port, pin, (cfgset & GPIO_EINT_MASK) >> GPIO_EINT_SHIFT);
	s5j_setintmask(port, pin, 0);

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
uint8_t s5j_gpio_irqvector(uint32_t pincfg)
{
	uint8_t pin  = (pincfg & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT;
	uint8_t port = (pincfg & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT;

	if (port == GPIO_PORTG1 >> GPIO_PORT_SHIFT) {
		return IRQ_WEINT_GPG10 + pin;
	} else if (port == GPIO_PORTG2 >> GPIO_PORT_SHIFT) {
		return IRQ_WEINT_GPG20 + pin;
	} else if (port == GPIO_PORTA0 >> GPIO_PORT_SHIFT) {
		return IRQ_EINT0 + pin;
	}

	return 0;
}

void s5j_gpio_clear_pending(uint32_t pincfg)
{
	uint32_t regbase;
	uint8_t pin  = (pincfg & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT;
	uint8_t port = (pincfg & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT;

	if (port == GPIO_PORTG1 >> GPIO_PORT_SHIFT) {
		regbase = S5J_GPIOINT_PEND_GPG1;
	} else if (port == GPIO_PORTG2 >> GPIO_PORT_SHIFT) {
		regbase = S5J_GPIOINT_PEND_GPG2;
	} else if (port  == GPIO_PORTA0 >> GPIO_PORT_SHIFT) {
		regbase = S5J_GPIOINT_PEND_GPA0;
	} else {
		return;
	}

	putreg32(1 << pin, regbase);
}

/****************************************************************************
 * Name: s5j_configgpio
 *
 * Description:
 *   Configure a GPIO pin based on bit-encoded description of the pin.
 *   Once it is configured as alternative (GPIO_ALT1-5) function, it must be
 *   unconfigured with s5j_unconfiggpio() with the same cfgset first before
 *   it can be set to non-alternative function.
 *
 * Returns:
 *   OK on success
 *   A negated errno value on invalid port, or when pin is locked as ALT
 *   function.
 *
 ****************************************************************************/
int s5j_configgpio(uint32_t cfgset)
{
	int ret = -EINVAL;
	unsigned int pin;
	unsigned int port;

	/* Verify that this hardware supports the selected GPIO port */
	port = (cfgset & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT;
	if (port >= S5J_GPIO_NPORTS) {
		return ret;
	}

	/* Get the pin number */
	pin = (cfgset & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT;

	switch (cfgset & GPIO_FUNC_MASK) {
	case GPIO_INPUT:
		ret = s5j_configinput(cfgset, port, pin);
		break;

	case GPIO_EINT:
		ret = s5j_configinterrupt(cfgset, port, pin);
		break;

	case GPIO_OUTPUT:
		ret = s5j_configoutput(cfgset, port, pin);
		break;

	case GPIO_ALT1 ... GPIO_ALT5:
		ret = s5j_configalt(cfgset, port, pin,
							(cfgset & GPIO_FUNC_MASK) >> GPIO_FUNC_SHIFT);
		break;
	}

	return ret;
}

/****************************************************************************
 * Name: s5j_unconfiggpio
 *
 * Description:
 *   Unconfigure a GPIO pin based on bit-encoded description of the pin, set
 *   it into default state.
 *
 * Returns:
 *   OK on success
 *   A negated errno value on invalid port
 *
 ****************************************************************************/
int s5j_unconfiggpio(uint32_t cfgset)
{
	cfgset &= (GPIO_PORT_MASK | GPIO_PIN_MASK);

	return s5j_configgpio(cfgset);
}

/****************************************************************************
 * Name: s5j_gpiowrite
 *
 * Description:
 *   Write one or zero to the selected GPIO pin
 *
 ****************************************************************************/
void s5j_gpiowrite(uint32_t pinset, bool value)
{
	unsigned int pin;
	unsigned int port;

	port = (pinset & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT;

	if (port < S5J_GPIO_NPORTS) {
		pin = (pinset & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT;
		modifyreg32(g_gpiobase[port] + S5J_GPIO_DAT_OFFSET,
					1 << pin, value << pin);
	}
}

/****************************************************************************
 * Name: s5j_gpioread
 *
 * Description:
 *   Write one or zero to the selected GPIO pin
 *
 ****************************************************************************/
bool s5j_gpioread(uint32_t pinset)
{
	uint32_t base;
	unsigned int pin;
	unsigned int port;

	port = (pinset & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT;

	if (port < S5J_GPIO_NPORTS) {
		base = g_gpiobase[port];
		pin  = (pinset & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT;

		return !!(getreg32(base + S5J_GPIO_DAT_OFFSET) & (1 << pin));
	}

	return false;
}

static int gic_interrupt(int irq, FAR void *context, FAR void *arg)
{
	int gpio_irq;
	uint32_t intpnd;

	if (irq == IRQ_GPIO_GIC_0) {
		irq = IRQ_WEINT_GPG10;
		intpnd = getreg32(S5J_GPIOINT_PEND_GPG1);
	} else {
		irq = IRQ_WEINT_GPG20;
		intpnd = getreg32(S5J_GPIOINT_PEND_GPG2);
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
