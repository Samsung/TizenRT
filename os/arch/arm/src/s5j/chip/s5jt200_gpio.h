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
 * os/arch/arm/src/s5j/chip/s5j_gpio.h
 *
 *   Copyright (C) 2017 Gregory Nutt. All rights reserved.
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

#ifndef __ARCH_ARM_SRC_S5J_CHIP_S5JT200_GPIO_H
#define __ARCH_ARM_SRC_S5J_CHIP_S5JT200_GPIO_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define S5J_GPIO_NPORTS		13

/* GPIO block register offsets **********************************************/
#define S5J_GPIO_GPP0_OFFSET	0x0000
#define S5J_GPIO_GPP1_OFFSET	0x0020
#define S5J_GPIO_GPP2_OFFSET	0x0040
#define S5J_GPIO_GPP3_OFFSET	0x0060
#define S5J_GPIO_GPG0_OFFSET	0x0080
#define S5J_GPIO_GPG1_OFFSET	0x00a0
#define S5J_GPIO_GPG2_OFFSET	0x00c0
#define S5J_GPIO_GPG3_OFFSET	0x00e0
#define S5J_GPIO_GPA0_OFFSET	0x0100
#define S5J_GPIO_GPA1_OFFSET	0x0120
#define S5J_GPIO_GPA2_OFFSET	0x0140
#define S5J_GPIO_GPA3_OFFSET	0x0160
#define S5J_GPIO_GPP4_OFFSET	0x0180

#define S5J_GPIO_CON_OFFSET		0x00
#define S5J_GPIO_DAT_OFFSET		0x04
#define S5J_GPIO_PUD_OFFSET		0x08
#define S5J_GPIO_DRV_OFFSET		0x0c

/* Register addresses *******************************************************/
#define S5J_GPIO_PORT_BASE(n)	(S5J_GPIO_BASE + (0x20 * n))
#define S5J_GPIO_GPP0_BASE		S5J_GPIO_PORT_BASE(0)
#define S5J_GPIO_GPP1_BASE		S5J_GPIO_PORT_BASE(1)
#define S5J_GPIO_GPP2_BASE		S5J_GPIO_PORT_BASE(2)
#define S5J_GPIO_GPP3_BASE		S5J_GPIO_PORT_BASE(3)
#define S5J_GPIO_GPG0_BASE		S5J_GPIO_PORT_BASE(4)
#define S5J_GPIO_GPG1_BASE		S5J_GPIO_PORT_BASE(5)
#define S5J_GPIO_GPG2_BASE		S5J_GPIO_PORT_BASE(6)
#define S5J_GPIO_GPG3_BASE		S5J_GPIO_PORT_BASE(7)
#define S5J_GPIO_GPA0_BASE		S5J_GPIO_PORT_BASE(8)
#define S5J_GPIO_GPA1_BASE		S5J_GPIO_PORT_BASE(9)
#define S5J_GPIO_GPA2_BASE		S5J_GPIO_PORT_BASE(10)
#define S5J_GPIO_GPA3_BASE		S5J_GPIO_PORT_BASE(11)
#define S5J_GPIO_GPP4_BASE		S5J_GPIO_PORT_BASE(12)

#define S5J_GPIO_PORT_CON(n)	(S5J_GPIO_PORT_BASE(n) + S5J_GPIO_CON_OFFSET)
#define S5J_GPIO_PORT_DAT(n)	(S5J_GPIO_PORT_BASE(n) + S5J_GPIO_DAT_OFFSET)
#define S5J_GPIO_PORT_PUD(n)	(S5J_GPIO_PORT_BASE(n) + S5J_GPIO_PUD_OFFSET)
#define S5J_GPIO_PORT_DRV(n)	(S5J_GPIO_PORT_BASE(n) + S5J_GPIO_DRV_OFFSET)

#define S5J_GPIO_GPP0_CON		S5J_GPIO_PORT_CON(0)
#define S5J_GPIO_GPP0_DAT		S5J_GPIO_PORT_DAT(0)
#define S5J_GPIO_GPP0_PUD		S5J_GPIO_PORT_PUD(0)
#define S5J_GPIO_GPP0_DRV		S5J_GPIO_PORT_DRV(0)
#define S5J_GPIO_GPP1_CON		S5J_GPIO_PORT_CON(1)
#define S5J_GPIO_GPP1_DAT		S5J_GPIO_PORT_DAT(1)
#define S5J_GPIO_GPP1_PUD		S5J_GPIO_PORT_PUD(1)
#define S5J_GPIO_GPP1_DRV		S5J_GPIO_PORT_DRV(1)
#define S5J_GPIO_GPP2_CON		S5J_GPIO_PORT_CON(2)
#define S5J_GPIO_GPP2_DAT		S5J_GPIO_PORT_DAT(2)
#define S5J_GPIO_GPP2_PUD		S5J_GPIO_PORT_PUD(2)
#define S5J_GPIO_GPP2_DRV		S5J_GPIO_PORT_DRV(2)
#define S5J_GPIO_GPP3_CON		S5J_GPIO_PORT_CON(3)
#define S5J_GPIO_GPP3_DAT		S5J_GPIO_PORT_DAT(3)
#define S5J_GPIO_GPP3_PUD		S5J_GPIO_PORT_PUD(3)
#define S5J_GPIO_GPP3_DRV		S5J_GPIO_PORT_DRV(3)
#define S5J_GPIO_GPG0_CON		S5J_GPIO_PORT_CON(4)
#define S5J_GPIO_GPG0_DAT		S5J_GPIO_PORT_DAT(4)
#define S5J_GPIO_GPG0_PUD		S5J_GPIO_PORT_PUD(4)
#define S5J_GPIO_GPG0_DRV		S5J_GPIO_PORT_DRV(4)
#define S5J_GPIO_GPG1_CON		S5J_GPIO_PORT_CON(5)
#define S5J_GPIO_GPG1_DAT		S5J_GPIO_PORT_DAT(5)
#define S5J_GPIO_GPG1_PUD		S5J_GPIO_PORT_PUD(5)
#define S5J_GPIO_GPG1_DRV		S5J_GPIO_PORT_DRV(5)
#define S5J_GPIO_GPG2_CON		S5J_GPIO_PORT_CON(6)
#define S5J_GPIO_GPG2_DAT		S5J_GPIO_PORT_DAT(6)
#define S5J_GPIO_GPG2_PUD		S5J_GPIO_PORT_PUD(6)
#define S5J_GPIO_GPG2_DRV		S5J_GPIO_PORT_DRV(6)
#define S5J_GPIO_GPG3_CON		S5J_GPIO_PORT_CON(7)
#define S5J_GPIO_GPG3_DAT		S5J_GPIO_PORT_DAT(7)
#define S5J_GPIO_GPG3_PUD		S5J_GPIO_PORT_PUD(7)
#define S5J_GPIO_GPG3_DRV		S5J_GPIO_PORT_DRV(7)
#define S5J_GPIO_GPA0_CON		S5J_GPIO_PORT_CON(8)
#define S5J_GPIO_GPA0_DAT		S5J_GPIO_PORT_DAT(8)
#define S5J_GPIO_GPA0_PUD		S5J_GPIO_PORT_PUD(8)
#define S5J_GPIO_GPA0_DRV		S5J_GPIO_PORT_DRV(8)
#define S5J_GPIO_GPA1_CON		S5J_GPIO_PORT_CON(9)
#define S5J_GPIO_GPA1_DAT		S5J_GPIO_PORT_DAT(9)
#define S5J_GPIO_GPA1_PUD		S5J_GPIO_PORT_PUD(9)
#define S5J_GPIO_GPA1_DRV		S5J_GPIO_PORT_DRV(9)
#define S5J_GPIO_GPA2_CON		S5J_GPIO_PORT_CON(10)
#define S5J_GPIO_GPA2_DAT		S5J_GPIO_PORT_DAT(10)
#define S5J_GPIO_GPA2_PUD		S5J_GPIO_PORT_PUD(10)
#define S5J_GPIO_GPA2_DRV		S5J_GPIO_PORT_DRV(10)
#define S5J_GPIO_GPA3_CON		S5J_GPIO_PORT_CON(11)
#define S5J_GPIO_GPA3_DAT		S5J_GPIO_PORT_DAT(11)
#define S5J_GPIO_GPA3_PUD		S5J_GPIO_PORT_PUD(11)
#define S5J_GPIO_GPA3_DRV		S5J_GPIO_PORT_DRV(11)
#define S5J_GPIO_GPP4_CON		S5J_GPIO_PORT_CON(12)
#define S5J_GPIO_GPP4_DAT		S5J_GPIO_PORT_DAT(12)
#define S5J_GPIO_GPP4_PUD		S5J_GPIO_PORT_PUD(12)
#define S5J_GPIO_GPP4_DRV		S5J_GPIO_PORT_DRV(12)
#define S5J_GPIO_ETC0_PUD		S5J_GPIO_PORT_PUD(13)
#define S5J_GPIO_ETC0_DRV		S5J_GPIO_PORT_DRV(13)

#define GPIO_CON_PIN_SHIFT(p)	((p) * 4)
#define GPIO_CON_PIN_MASK(p)	(0xf << GPIO_CON_PIN_SHIFT(p))
#define GPIO_CON_INPUT			0x0
#define GPIO_CON_OUTPUT			0x1
#define GPIO_CON_ALT1			0x2
#define GPIO_CON_ALT2			0x3
#define GPIO_CON_ALT3			0x4
#define GPIO_CON_ALT4			0x5
#define GPIO_CON_ALT5			0x6
#define GPIO_CON_ALT6			0x7
#define GPIO_CON_ALT7			0x8
#define GPIO_CON_ALT8			0x9
#define GPIO_CON_ALT9			0xa
#define GPIO_CON_ALT10			0xb
#define GPIO_CON_ALT11			0xc
#define GPIO_CON_ALT12			0xd
#define GPIO_CON_ALT13			0xe
#define GPIO_CON_EINT			0xf

#define GPIO_PUD_PIN_SHIFT(p)	((p) * 4)
#define GPIO_PUD_PIN_MASK(p)	(0xf << GPIO_PUD_PIN_SHIFT(p))
#define GPIO_PUD_DISABLE		0x0
#define GPIO_PUD_PULLDOWN		0x1
#define GPIO_PUD_RESERVED		0x2
#define GPIO_PUD_PULLUP			0x3

#define GPIO_DRV_PIN_SHIFT(p)	((p) * 4)
#define GPIO_DRV_PIN_MASK(p)	(0xf << GPIO_DRV_PIN_SHIFT(p))
#define GPIO_DRV_FAST1X			0x0
#define GPIO_DRV_FAST2X			0x1
#define GPIO_DRV_FAST3X			0x2
#define GPIO_DRV_FAST4X			0x3
#define GPIO_DRV_SLOW1X			0x4
#define GPIO_DRV_SLOW2X			0x5
#define GPIO_DRV_SLOW3X			0x6
#define GPIO_DRV_SLOW4X			0x7
#define GPIO_DRV_RESERVED		0xf

/* GPIO interrupt block register addresses **********************************/
#define S5J_GPIOINT_BASE			(S5J_GPIO_BASE + 0x0700)

#define S5J_GPIOINT_CON_BASE		(S5J_GPIOINT_BASE + 0x0000)
#define S5J_GPIOINT_FLTCON_BASE		(S5J_GPIOINT_BASE + 0x0100)
#define S5J_GPIOINT_MASK_BASE		(S5J_GPIOINT_BASE + 0x0200)
#define S5J_GPIOINT_PEND_MASK		(S5J_GPIOINT_BASE + 0x0300)

#define S5J_GPIOINT_PORT_CON(p)		(S5J_GPIOINT_CON_BASE + ((p) << 2))
#define S5J_GPIOINT_PORT_FLTCON(p)	(S5J_GPIOINT_FLTCON_BASE + ((p) << 4))
#define S5J_GPIOINT_PORT_MASK(p)	(S5J_GPIOINT_MASK_BASE + ((p) << 2))
#define S5J_GPIOINT_PORT_PEND(p)	(S5J_GPIOINT_PEND_MASK + ((p) << 2))

#define S5J_GPIOINT_CON_GPG1		S5J_GPIOINT_PORT_CON(0)
#define S5J_GPIOINT_CON_GPG2		S5J_GPIOINT_PORT_CON(1)
#define S5J_GPIOINT_CON_GPA0		S5J_GPIOINT_PORT_CON(2)
#define S5J_GPIOINT_FLTCON_GPG1		S5J_GPIOINT_PORT_FLTCON(0)
#define S5J_GPIOINT_FLTCON_GPG2		S5J_GPIOINT_PORT_FLTCON(1)
#define S5J_GPIOINT_FLTCON_GPA0		S5J_GPIOINT_PORT_FLTCON(2)
#define S5J_GPIOINT_MASK_GPG1		S5J_GPIOINT_PORT_MASK(0)
#define S5J_GPIOINT_MASK_GPG2		S5J_GPIOINT_PORT_MASK(1)
#define S5J_GPIOINT_MASK_GPA0		S5J_GPIOINT_PORT_MASK(2)
#define S5J_GPIOINT_PEND_GPG1		S5J_GPIOINT_PORT_PEND(0)
#define S5J_GPIOINT_PEND_GPG2		S5J_GPIOINT_PORT_PEND(1)
#define S5J_GPIOINT_PEND_GPA0		S5J_GPIOINT_PORT_PEND(2)

#define GPIOINT_CON_SHIFT(p)		((p) * 4)
#define GPIOINT_CON_MASK(p)			(0x7 << GPIOINT_CON_SHIFT(p))
#define GPIOINT_CON_LOW				0x0
#define GPIOINT_CON_HIGH			0x1
#define GPIOINT_CON_FALLING			0x2
#define GPIOINT_CON_RISING			0x3
#define GPIOINT_CON_BOTH			0x4

#define GPIOINT_FLTCON_EN_SHIFT(p)	((p) * 4 + 7)
#define GPIOINT_FLTCON_EN_MASK(p)	(0x1 << GPIOINT_FLTCON_EN_SHIFT(p))
#define GPIOINT_FLTCON_EN_DISABLE	0x0
#define GPIOINT_FLTCON_EN_ENABLE	0x1

#define GPIOINT_FLTCON_SEL_SHIFT(p)	((p) * 4 + 6)
#define GPIOINT_FLTCON_SEL_MASK(p)	(0x1 << GPIOINT_FLTCON_SEL_SHIFT(p))
#define GPIOINT_FLTCON_SEL_DELAY	0x0
#define GPIOINT_FLTCON_SEL_DIGITAL	0x1

#define GPIOINT_FLTCON_WIDTH_SHIFT(p)	((p) * 4)
#define GPIOINT_FLTCON_WIDTH_MASK(p)	(0x3f << GPIOINT_FLTCON_WIDTH_SHIFT(p))

#define GPIOINT_MASK_SHIFT(p)		(p)
#define GPIOINT_MASK_MASK(p)		(1 << GPIOINT_MASK_SHIFT(p))
#define GPIOINT_MASK_ENABLE			0x0
#define GPIOINT_MASK_DISABLE		0x1

#endif /* __ARCH_ARM_SRC_S5J_CHIP_S5JT200_GPIO_H */
