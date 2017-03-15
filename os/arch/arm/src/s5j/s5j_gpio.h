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
 * arch/arm/src/s5j/s5j_gpio.h
 *
 *   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
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

#ifndef __ARCH_ARM_SRC_S5J_S5J_GPIO_H
#define __ARCH_ARM_SRC_S5J_S5J_GPIO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

#include <stdint.h>
#include <stdbool.h>
#include <debug.h>

#include <tinyara/irq.h>

#include <chip.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Pin configurations */
#define GPIO_INPUT    0x0
#define GPIO_OUTPUT   0x1
#define GPIO_IRQ      0xf
#define GPIO_FUNC(x)  (x)

/* Pin PDN configurations */
#define GPIO_PDN_OUTPUT0 0x0
#define GPIO_PDN_OUTPUT1 0x1
#define GPIO_PDN_INPUT   0x2
#define GPIO_PDN_PREV    0x3

/* Pull mode */
#define GPIO_PULL_NONE  0x0
#define GPIO_PULL_DOWN  0x1
#define GPIO_PULL_UP    0x3

/* Drive Strength level */
#define GPIO_DRV_1X    0x0
#define GPIO_DRV_3X    0x1
#define GPIO_DRV_2X    0x2
#define GPIO_DRV_4X    0x3
#define GPIO_DRV_FAST  0x0
#define GPIO_DRV_SLOW  0x1

/* EINT type */
#define EINT_TYPE_LEVEL_LOW     0x0
#define EINT_TYPE_LEVEL_HIGH    0x1
#define EINT_TYPE_EDGE_FALLING  0x2
#define EINT_TYPE_EDGE_RISING   0x3
#define EINT_TYPE_EDGE_BOTH     0x4

/* EINT filter type */
#define EINT_FILTER_DELAY      0x0
#define EINT_FILTER_DIGITAL    0x1

/* Register offsets */
#define GPIO_CON               0x0
#define GPIO_DAT               0x4
#define GPIO_PUD               0x8
#define GPIO_DRVSR             0xc
#define GPIO_CONPDN            0x10
#define GPIO_PUDPDN            0x14

#define GPIO_EINT_CON          0x608
#define GPIO_EINT_FLTCON       0x710
#define GPIO_EINT_MASK         0x808
#define GPIO_EINT_PEND         0x908
#define GPIO_SVC               0xB08

/* Bit field manipulation */
#define CON_MASK(x)         (0xf << ((x) << 2))
#define CON_SFR(x, v)       ((v) << ((x) << 2))
#define CON_GET(x, v)       ((v) << ((x) << 2))

#define PULL_MASK(x)        (0xf << ((x) << 2))
#define PULL_MODE(x, v)     ((v) << ((x) << 2))

#define DRV_MASK(x)         (0xf << ((x) << 2))
#define DRV_SET(x, m)       ((m) << ((x) << 2))

#define RATE_MASK(x)        (0x1 << (x + 16))
#define RATE_SET(x)         (0x1 << (x + 16))

#define CONPDN_MASK(x)      (0x3 << ((x) << 1))
#define CONPDN_SFR(x, v)    ((v) << ((x) << 1))

#define PUDPDN_MASK(x)      (0x3 << ((x) << 1))
#define PUDPDN_MODE(x, v)   ((v) << ((x) << 1))

#define GPIO_MAGIC          (0x4750 << 16)

#define s5j_gpio(bank, port)   (GPIO_MAGIC | (((bank) << 8) | port))
#define s5j_gpio_NC            (GPIO_MAGIC | 0xffff)

#define s5j_gpio_bank(pin)     ((pin >> 8) & 0xff)
#define s5j_gpio_port(pin)     ((pin) & 0xff)

struct gpio_bank {
	char *name;
	void *base;
	unsigned type;
	unsigned nr_port;
	unsigned isr_num[CONFIG_MAX_GPIO_PORT];
	int group_type;
	int filter_offset_addr;
};

#define irq_id_to_gpio(irq_id) (GPIO_MAGIC | (irq_id >> 16))
#define gpio_irq_id(gpio, isr_num) ((gpio & 0xFFFF) << 16 | isr_num)

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Inline Functions
 ****************************************************************************/
#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Data
 ****************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int gpio_valid(int gpio);
int gpio_cfg_pin(int gpio, int cfg);
int gpio_cfg_get_pin(int gpio);
int gpio_direction_output(int gpio, int high);
int gpio_direction_input(int gpio);
int gpio_set_value(int gpio, int high);
int gpio_get_value(int gpio);
int gpio_set_pull(int gpio, int mode);
int gpio_get_pull(int gpio);
int gpio_set_drv(int gpio, int mode);
int gpio_get_drv(int gpio);
int gpio_set_rate(int gpio, int mode);
int gpio_cfg_pin_pdn(int gpio, int cfg);
int gpio_set_pull_pdn(int gpio, int mode);
int gpio_eint_mask(int gpio);
int gpio_eint_unmask(int gpio);
bool gpio_eint_ispending(int gpio);
int gpio_eint_clear_pending(int gpio);
int gpio_eint_enable_filter(int gpio);
int gpio_eint_disable_filter(int gpio);
int gpio_eint_set_filter(int gpio, unsigned type, unsigned width);
int gpio_eint_set_type(int gpio, unsigned type);
int gpio_eint_get_type(int gpio);

#if defined(__cplusplus)
}
#endif
#endif							/* __ASSEMBLY__ */
#endif							/* __ARCH_ARM_SRC_S5J_S5J_GPIO_H */
