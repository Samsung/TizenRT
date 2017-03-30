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
 * arch/arm/src/sidk_s5jt200/src/s5j_buttons.c
 *
 *   Copyright (C) 2013, 2015 Gregory Nutt. All rights reserved.
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

#include <stdio.h>
#include <syslog.h>
#include <errno.h>

#include <tinyara/board.h>
#include <arch/chip/chip.h>
#include <tinyara/irq.h>

#include <arch/chip/chip_types.h>
#include <chip.h>

#ifdef CONFIG_ARCH_BUTTONS

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_button_initialize
 *
 * Description:
 *   board_button_initialize() must be called to initialize button resources.  After
 *   that, board_buttons() may be called to collect the current state of all
 *   buttons or board_button_irq() may be called to register button interrupt
 *   handlers.
 *
 ****************************************************************************/

void board_button_initialize(void)
{
	s32 gpio;

	/* Configure the GPIO pins as EINT. */

	gpio = s5j_gpio(GPA0, 0);
	gpio_eint_set_filter(gpio, EINT_FILTER_DIGITAL, 0);
	gpio_set_pull(gpio, GPIO_PULL_UP);
	gpio_eint_set_type(gpio, EINT_TYPE_EDGE_FALLING);
	gpio_cfg_pin(gpio, GPIO_IRQ);
	gpio_eint_clear_pending(gpio);
	gpio_eint_unmask(gpio);
}

/****************************************************************************
 * Name: board_buttons
 ****************************************************************************/

uint8_t board_buttons(void)
{
	uint8_t ret = 0;
	bool released;
	s32 gpio;

	gpio = s5j_gpio(GPA0, 0);

	/* Check that state of each key */

	released = gpio_get_value(gpio);

	/* Accumulate the set of depressed (not released) keys */

	if (!released) {
		ret |= (1 << 0);
	}

	return ret;
}

/************************************************************************************
 * Button support.
 *
 * Description:
 *   board_button_initialize() must be called to initialize button resources.  After
 *   that, board_buttons() may be called to collect the current state of all
 *   buttons or board_button_irq() may be called to register button interrupt
 *   handlers.
 *
 *   After board_button_initialize() has been called, board_buttons() may be called to
 *   collect the state of all buttons.  board_buttons() returns an 8-bit bit set
 *   with each bit associated with a button.  See the BUTTON_*_BIT
 *   definitions in board.h for the meaning of each bit.
 *
 *   board_button_irq() may be called to register an interrupt handler that will
 *   be called when a button is depressed or released.  The ID value is a
 *   button enumeration value that uniquely identifies a button resource. See the
 *   BUTTON_* definitions in board.h for the meaning of enumeration
 *   value.  The previous interrupt handler address is returned (so that it may
 *   restored, if so desired).
 *
 ************************************************************************************/

#ifdef CONFIG_ARCH_IRQBUTTONS
xcpt_t board_button_irq(int id, xcpt_t irqhandler)
{
	xcpt_t oldhandler = NULL;

	/* The following should be atomic */

	if (irqhandler == NULL) {
		up_disable_irq(IRQ_EINT0);
		irq_attach(IRQ_EINT0, NULL, NULL);
		return oldhandler;
	}

	if (irq_attach(IRQ_EINT0, irqhandler, NULL) == OK) {
		up_enable_irq(IRQ_EINT0);
	} else {
		/* TO DO: How can it contolled ? */

		//oldhandler = irqhandler;
	}

	return oldhandler;
}
#endif

#ifdef CONFIG_ARCH_IRQBUTTONS
void board_button_handler(int id, int irq)
{
	s32 gpio;

	gpio = s5j_gpio(GPA0, 0);
	gpio_eint_clear_pending(gpio);
}
#endif
#endif							/* CONFIG_ARCH_BUTTONS */
