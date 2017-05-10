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

#include <tinyara/irq.h>
#include <tinyara/gpio.h>

#include "chip.h"
#include "chip/s5jt200_gpio.h"
#include "chip/s5jt200_pinconfig.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Bit-encoded input to s5j_configgpio() */

/*
 * 16-bits Encoding:
 *
 *                1111 1100 0000 0000
 *                5432 1098 7654 3210
 *                ---- ---- ---- ----
 * Output mode    FFFU USSS VGGG GPPP
 * Input mode     FFFU UEEE .GGG GPPP
 */

/*
 * Function mode:
 *
 * 1111 1100 0000 0000
 * 5432 1098 7654 3210
 * ---- ---- ---- ----
 * FFF. .... .... ....
 */
#define GPIO_FUNC_SHIFT		13
#define GPIO_FUNC_MASK		(0x7 << GPIO_FUNC_SHIFT)
#define GPIO_INPUT			(0x0 << GPIO_FUNC_SHIFT)
#define GPIO_OUTPUT			(0x1 << GPIO_FUNC_SHIFT)
#define GPIO_ALT1			(0x2 << GPIO_FUNC_SHIFT)
#define GPIO_ALT2			(0x3 << GPIO_FUNC_SHIFT)
#define GPIO_ALT3			(0x4 << GPIO_FUNC_SHIFT)
#define GPIO_ALT4			(0x5 << GPIO_FUNC_SHIFT)
#define GPIO_ALT5			(0x6 << GPIO_FUNC_SHIFT)
#define GPIO_EINT			(0x7 << GPIO_FUNC_SHIFT)

/*
 * pull-up/down:
 *
 * 1111 1100 0000 0000
 * 5432 1098 7654 3210
 * ---- ---- ---- ----
 * ...U U... .... ....
 */
#define GPIO_PUPD_SHIFT		11
#define GPIO_PUPD_MASK		(0x3 << GPIO_PUPD_SHIFT)
#define GPIO_FLOAT			(0x0 << GPIO_PUPD_SHIFT)
#define GPIO_PULLDOWN		(0x1 << GPIO_PUPD_SHIFT)
#define GPIO_RESERVED		(0x2 << GPIO_PUPD_SHIFT)
#define GPIO_PULLUP			(0x3 << GPIO_PUPD_SHIFT)

/*
 * Initial value:
 *
 * 1111 1100 0000 0000
 * 5432 1098 7654 3210
 * ---- ---- ---- ----
 * .... .... V... ....
 */
#define GPIO_VALUE_SHIFT	7
#define GPIO_VALUE_MASK		(0x1 << GPIO_VALUE_SHIFT)
#define GPIO_VALUE_ZERO		(0x0 << GPIO_VALUE_SHIFT)
#define GPIO_VALUE_ONE		(0x1 << GPIO_VALUE_SHIFT)

/*
 * Drive strength:
 *
 * 1111 1100 0000 0000
 * 5432 1098 7654 3210
 * ---- ---- ---- ----
 * .... .SSS .... ....
 */
#define GPIO_DRVSTR_SHIFT	8
#define GPIO_DRVSTR_MASK	(0x7 << GPIO_DRVSTR_SHIFT)
#define GPIO_FAST1X			(0x0 << GPIO_DRVSTR_SHIFT)
#define GPIO_FAST2X			(0x1 << GPIO_DRVSTR_SHIFT)
#define GPIO_FAST3X			(0x2 << GPIO_DRVSTR_SHIFT)
#define GPIO_FAST4X			(0x3 << GPIO_DRVSTR_SHIFT)
#define GPIO_SLOW1X			(0x4 << GPIO_DRVSTR_SHIFT)
#define GPIO_SLOW2X			(0x5 << GPIO_DRVSTR_SHIFT)
#define GPIO_SLOW3X			(0x6 << GPIO_DRVSTR_SHIFT)
#define GPIO_SLOW4X			(0x7 << GPIO_DRVSTR_SHIFT)

/*
 * External interrupt edge mode (when EINT mode) :
 *
 * 1111 1100 0000 0000
 * 5432 1098 7654 3210
 * ---- ---- ---- ----
 * .... .EEE .... ....
 */
#define GPIO_EINT_SHIFT			8
#define GPIO_EINT_MASK			(0x7 << GPIO_EINT_SHIFT)
#define GPIO_EINT_LOW			(0x0 << GPIO_EINT_SHIFT)
#define GPIO_EINT_HIGH			(0x1 << GPIO_EINT_SHIFT)
#define GPIO_EINT_FALLING_EDGE	(0x2 << GPIO_EINT_SHIFT)
#define GPIO_EINT_RISING_EDGE	(0x3 << GPIO_EINT_SHIFT)
#define GPIO_EINT_BOTH_EDGE		(0x4 << GPIO_EINT_SHIFT)

/*
 * This identifies the port group:
 *
 * 1111 1100 0000 0000
 * 5432 1098 7654 3210
 * ---- ---- ---- ----
 * .... .... .GGG G...
 */
#define GPIO_PORT_SHIFT		3
#define GPIO_PORT_MASK		(0xf << GPIO_PORT_SHIFT)
#define GPIO_PORTP0			(0x0 << GPIO_PORT_SHIFT)
#define GPIO_PORTP1			(0x1 << GPIO_PORT_SHIFT)
#define GPIO_PORTP2			(0x2 << GPIO_PORT_SHIFT)
#define GPIO_PORTP3			(0x3 << GPIO_PORT_SHIFT)
#define GPIO_PORTG0			(0x4 << GPIO_PORT_SHIFT)
#define GPIO_PORTG1			(0x5 << GPIO_PORT_SHIFT)
#define GPIO_PORTG2			(0x6 << GPIO_PORT_SHIFT)
#define GPIO_PORTG3			(0x7 << GPIO_PORT_SHIFT)
#define GPIO_PORTA0			(0x8 << GPIO_PORT_SHIFT)
#define GPIO_PORTA1			(0x9 << GPIO_PORT_SHIFT)
#define GPIO_PORTA2			(0xa << GPIO_PORT_SHIFT)
#define GPIO_PORTA3			(0xb << GPIO_PORT_SHIFT)
#define GPIO_PORTP4			(0xc << GPIO_PORT_SHIFT)

/*
 * This identifies the bit in the port:
 *
 * 1111 1100 0000 0000
 * 5432 1098 7654 3210
 * ---- ---- ---- ----
 * .... .... .... .PPP
 */
#define GPIO_PIN_SHIFT		0
#define GPIO_PIN_MASK		(0x7 << GPIO_PIN_SHIFT)
#define GPIO_PIN0			(0x0 << GPIO_PIN_SHIFT)
#define GPIO_PIN1			(0x1 << GPIO_PIN_SHIFT)
#define GPIO_PIN2			(0x2 << GPIO_PIN_SHIFT)
#define GPIO_PIN3			(0x3 << GPIO_PIN_SHIFT)
#define GPIO_PIN4			(0x4 << GPIO_PIN_SHIFT)
#define GPIO_PIN5			(0x5 << GPIO_PIN_SHIFT)
#define GPIO_PIN6			(0x6 << GPIO_PIN_SHIFT)
#define GPIO_PIN7			(0x7 << GPIO_PIN_SHIFT)

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__
#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_gpio_lowerhalf
 *
 * Description:
 *
 ****************************************************************************/
struct gpio_lowerhalf_s *s5j_gpio_lowerhalf(uint16_t pincfg);

/****************************************************************************
 * Name: s5j_configgpio
 *
 * Description:
 *   Configure a GPIO pin based on bit-encoded description of the pin.
 *
 ****************************************************************************/
int s5j_configgpio(uint32_t cfgset);

/****************************************************************************
 * Name: s5j_configgpio
 *
 * Description:
 *   Configure a GPIO pin based on bit-encoded description of the pin.
 *
 ****************************************************************************/
int s5j_unconfiggpio(uint32_t cfgset);

/****************************************************************************
 * Name: s5j_gpiowrite
 *
 * Description:
 *   Write one or zero to the selected GPIO pin
 *
 ****************************************************************************/
void s5j_gpiowrite(uint32_t pinset, bool value);

/****************************************************************************
 * Name: s5j_gpioread
 *
 * Description:
 *   Read one or zero to the selected GPIO pin
 *
 ****************************************************************************/
bool s5j_gpioread(uint32_t pinset);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __ARCH_ARM_SRC_S5J_S5J_GPIO_H */
