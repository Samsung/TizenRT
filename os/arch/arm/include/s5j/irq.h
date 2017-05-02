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
 * arch/arm/include/s5j/irq.h
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
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

/*
 * This file should never be included directed but, rather, only indirectly
 * through tinyara/irq.h
 */

#ifndef __ARCH_ARM_INCLUDE_S5J_IRQ_H
#define __ARCH_ARM_INCLUDE_S5J_IRQ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/*
 * IRQ numbers. The IRQ numbers corresponds vector number and hence map
 * directly to bits in the NVCI. This does, however, waste several words
 * of memory in the IRQ to handle mapping tables.
 */

#define IRQ_INVALID			0x3FF
#define IRQ_SPI(x)			(32 + (x))

#define IRQ_EINT0			IRQ_SPI(0)
#define IRQ_EINT1			IRQ_SPI(1)
#define IRQ_EINT2			IRQ_SPI(2)

#define IRQ_GPIO_GIC_0			IRQ_SPI(4)
#define IRQ_GPIO_GIC_1			IRQ_SPI(5)

#define IRQ_MCT_G0			IRQ_SPI(18)
#define IRQ_MCT_G1			IRQ_SPI(19)
#define IRQ_MCT_G2			IRQ_SPI(20)
#define IRQ_MCT_G3			IRQ_SPI(21)
#define IRQ_MCT_L0			IRQ_SPI(22)
#define IRQ_MCT_L1			IRQ_SPI(23)
#define IRQ_MCT_L2			IRQ_SPI(24)
#define IRQ_MCT_L3			IRQ_SPI(25)
#define IRQ_TOP_RTC_TIC			IRQ_SPI(26)
#define IRQ_TOP_RTC_ALARM		IRQ_SPI(27)
#define IRQ_TICK_COUNTER_TIC		IRQ_SPI(28)
#define IRQ_TICK_COUNTER_ALARM		IRQ_SPI(29)

#define IRQ_WDT				IRQ_SPI(32)
#define IRQ_UHD_EFUSE_WRITER		IRQ_SPI(33)

#define IRQ_MAILBOX_WIFI		IRQ_SPI(35)
#define IRQ_MAILBOX_M0			IRQ_SPI(36)

#define IRQ_PMU_CM0			IRQ_SPI(39)
#define IRQ_PMU_CR4			IRQ_SPI(40)
#define IRQ_PMU_ALIVE_WIFI_ACTIVE	IRQ_SPI(41)

#define IRQ_PDMA_U1			IRQ_SPI(44)

#define IRQ_SSS_KM			IRQ_SPI(47)
#define IRQ_SSS_MB			IRQ_SPI(48)
#define IRQ_SSS				IRQ_SPI(49)
#define IRQ_SSS_SWDT_HOST_1		IRQ_SPI(50)
#define IRQ_SSS_SWDT_HOST_2		IRQ_SPI(51)

#define S5J_IRQ_UART0			IRQ_SPI(54)
#define S5J_IRQ_UART1			IRQ_SPI(55)
#define S5J_IRQ_UART2			IRQ_SPI(56)
#define S5J_IRQ_UART3			IRQ_SPI(57)
#define S5J_IRQ_UART4			IRQ_SPI(58)

#define S5J_IRQ_SPI_0			IRQ_SPI(60)
#define S5J_IRQ_SPI_1			IRQ_SPI(61)
#define S5J_IRQ_SPI_2			IRQ_SPI(62)
#define S5J_IRQ_SPI_3			IRQ_SPI(63)

#define IRQ_HSI2C_0			IRQ_SPI(65)
#define IRQ_HSI2C_1			IRQ_SPI(66)
#define IRQ_HSI2C_2			IRQ_SPI(67)
#define IRQ_HSI2C_3			IRQ_SPI(68)

#define IRQ_I2S				IRQ_SPI(71)

#define IRQ_ADC				IRQ_SPI(74)

#define IRQ_PWM0_0			IRQ_SPI(77)
#define IRQ_PWM0_1			IRQ_SPI(78)
#define IRQ_PWM0_2			IRQ_SPI(79)
#define IRQ_PWM0_3			IRQ_SPI(80)
#define IRQ_PWM0_4			IRQ_SPI(81)
#define IRQ_PWM1_0			IRQ_SPI(82)
#define IRQ_PWM1_1			IRQ_SPI(83)
#define IRQ_SDIO_DEVICE_SDIO		IRQ_SPI(84)
#define IRQ_SDIO_DEVICE_RSTSOFT		IRQ_SPI(85)
#define IRQ_SDIO_DEVICE_RSTFUN		IRQ_SPI(86)
#define IRQ_WIFI_WB2AP_WDOG_RESET	IRQ_SPI(87)

#define IRQ_CR4_PMUIRQ			IRQ_SPI(90)
#define IRQ_CR4_VALIRQ			IRQ_SPI(91)
#define IRQ_CR4_VALFIQ			IRQ_SPI(92)

#define NR_VECTORS			(IRQ_CR4_VALFIQ + 1)

/* External Interrupts */
#define IRQ_WEINT_GPG10		(NR_VECTORS + 0)
#define IRQ_WEINT_GPG11		(NR_VECTORS + 1)
#define IRQ_WEINT_GPG12		(NR_VECTORS + 2)
#define IRQ_WEINT_GPG13		(NR_VECTORS + 3)
#define IRQ_WEINT_GPG14		(NR_VECTORS + 4)
#define IRQ_WEINT_GPG15		(NR_VECTORS + 5)
#define IRQ_WEINT_GPG16		(NR_VECTORS + 6)
#define IRQ_WEINT_GPG17		(NR_VECTORS + 7)
#define IRQ_WEINT_GPG20		(NR_VECTORS + 8)
#define IRQ_WEINT_GPG21		(NR_VECTORS + 9)
#define IRQ_WEINT_GPG22		(NR_VECTORS + 10)
#define IRQ_WEINT_GPG23		(NR_VECTORS + 11)
#define IRQ_WEINT_GPG24		(NR_VECTORS + 12)
#define IRQ_WEINT_GPG25		(NR_VECTORS + 13)
#define IRQ_WEINT_GPG26		(NR_VECTORS + 14)
#define IRQ_WEINT_GPG27		(NR_VECTORS + 15)

#define NR_IRQS				(NR_VECTORS + 16)

#endif /* __ARCH_ARM_INCLUDE_S5J_IRQ_H */
