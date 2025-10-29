/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>

#define NR_IRQS 80

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* IRQ numbers.  The IRQ number corresponds vector number and hence map directly to
 * bits in the NVIC.  This does, however, waste several words of memory in the IRQ
 * to handle mapping tables.
 */

/* Processor Exceptions (vectors 0-15) */

#define ARMINO_IRQ_RESERVED       (0) /* Reserved vector (only used with CONFIG_DEBUG) */
                                     /* Vector  0: Reset stack pointer value */
                                     /* Vector  1: Reset (not handler as an IRQ) */
#define ARMINO_IRQ_NMI            (2) /* Vector  2: Non-Maskable Interrupt (NMI) */
#define ARMINO_IRQ_HARDFAULT      (3) /* Vector  3: Hard fault */
#define ARMINO_IRQ_MEMFAULT       (4) /* Vector  4: Memory management (MPU) */
#define ARMINO_IRQ_BUSFAULT       (5) /* Vector  5: Bus fault */
#define ARMINO_IRQ_USAGEFAULT     (6) /* Vector  6: Usage fault */
#define ARMINO_IRQ_SECUREFAULT    (7) /* Vector  7: secure fault */
                                     /* Vectors 8-10: Reserved */
#define ARMINO_IRQ_SVCALL        (11) /* Vector 11: SVC call */
#define ARMINO_IRQ_DBGMONITOR    (12) /* Vector 12: Debug Monitor */
                                     /* Vector 13: Reserved */
#define ARMINO_IRQ_PENDSV        (14) /* Vector 14: Pendable system service request */
#define ARMINO_IRQ_SYSTICK       (15) /* Vector 15: System tick */

/* External interrupts (vectors >= 16).  These definitions are chip-specific */

#define ARMINO_IRQ_FIRST         (16) /* Vector number of the first external interrupt */

#define ARMINO_IRQ_DMA_NSEC_INT				(ARMINO_IRQ_FIRST + 0)  /*!< 0 DMA non-secure interrupt */
#define ARMINO_IRQ_ENCP_SEC_INT			    (ARMINO_IRQ_FIRST + 1)	/*!< 1 ENCRYPT secure interrupt  */
#define ARMINO_IRQ_ENCP_NSEC_INT			(ARMINO_IRQ_FIRST + 2)	/*!< 2 ENCRYPT non-secure interrupt */
#define ARMINO_IRQ_TIMER					(ARMINO_IRQ_FIRST + 3)	/*!< 3 Timer interrupt */
#define ARMINO_IRQ_UART_LP					(ARMINO_IRQ_FIRST + 4)	/*!< 4 UART_LP interrupt */
#define ARMINO_IRQ_PWM0						(ARMINO_IRQ_FIRST + 5)	/*!< 5 PWM0 interrupt */
#define ARMINO_IRQ_I2C0						(ARMINO_IRQ_FIRST + 6)	/*!< 6 I2C0 global interrupt */
#define ARMINO_IRQ_SPI0						(ARMINO_IRQ_FIRST + 7)	/*!< 7 SPI0 global interrupt */
#define ARMINO_IRQ_SADC					    (ARMINO_IRQ_FIRST + 8)	/*!< 8 SADC global interrupt */
#define ARMINO_IRQ_UART3				    (ARMINO_IRQ_FIRST + 9)	/*!< 9 UART3 interrupt */
#define ARMINO_IRQ_SDIO					    (ARMINO_IRQ_FIRST + 10)	/*!< 10 SDIO interrupt */
#define ARMINO_IRQ_GDMA					    (ARMINO_IRQ_FIRST + 11)	/*!< 11 GDMA interrupt */
#define ARMINO_IRQ_LA						(ARMINO_IRQ_FIRST + 12)	/*!< 12 LA interrupt */
#define ARMINO_IRQ_TIMER1					(ARMINO_IRQ_FIRST + 13)	/*!< 13 Timer1 interrupt */
#define ARMINO_IRQ_I2C1						(ARMINO_IRQ_FIRST + 14)	/*!< 14 I2C1 global interrupt */
#define ARMINO_IRQ_UART1					(ARMINO_IRQ_FIRST + 15)	/*!< 15 UART1 interrupt */
#define ARMINO_IRQ_UART2					(ARMINO_IRQ_FIRST + 16)	/*!< 16 UART2 interrupt */
#define ARMINO_IRQ_SPI1						(ARMINO_IRQ_FIRST + 17)	/*!< 17 SPI1 global interrupt */
#define ARMINO_IRQ_LED						(ARMINO_IRQ_FIRST + 18)	/*!< 18 LED interrupt */
#define ARMINO_IRQ_CKMN					    (ARMINO_IRQ_FIRST + 21)	/*!< 21 CKMN interrupt */
#define ARMINO_IRQ_I2S0					    (ARMINO_IRQ_FIRST + 24)	/*!< 24 I2S0 interrupt */
#define ARMINO_IRQ_PHY_MBP_INT				(ARMINO_IRQ_FIRST + 29)	/*!< 29 PHY MBP interrupt */
#define ARMINO_IRQ_PHY_RIU_INT				(ARMINO_IRQ_FIRST + 30)	/*!< 30 PHY RIU interrupt */
#define ARMINO_IRQ_MAC_INT_TX_RX_TIMER_N	(ARMINO_IRQ_FIRST + 31)	/*!< 31 MAC INT TX RX TIMER non-secure interrupt */
#define ARMINO_IRQ_MAC_INT_TX_RX_MISC_N		(ARMINO_IRQ_FIRST + 32)	/*!< 32 MAC INT TX RX MISC non-secure interrupt */
#define ARMINO_IRQ_MAC_INT_RX_TRIGGER_N		(ARMINO_IRQ_FIRST + 33)	/*!< 33 MAC INT RX TRIGGER non-secure interrupt */	
#define ARMINO_IRQ_MAC_INT_TX_TRIGGER_N		(ARMINO_IRQ_FIRST + 34)	/*!< 34 MAC INT TX TRIGGER non-secure interrupt */
#define ARMINO_IRQ_MAC_INT_PROT_TRIGGER_N	(ARMINO_IRQ_FIRST + 35)	/*!< 35 MAC INT PROT TRIGGER non-secure interrupt */
#define ARMINO_IRQ_MAC_INT_GEN_N			(ARMINO_IRQ_FIRST + 36)	/*!< 36 MAC INT GEN non-secure interrupt */
#define ARMINO_IRQ_HSU						(ARMINO_IRQ_FIRST + 37)	/*!< 37 HSU interrupt */
#define ARMINO_IRQ_INT_MAC_WAKEUP			(ARMINO_IRQ_FIRST + 38)	/*!< 38 MAC INT WAKEUP interrupt */
#define ARMINO_IRQ_DM						(ARMINO_IRQ_FIRST + 39)	/*!< 39 DM IRQ interrupt */
#define ARMINO_IRQ_BLE						(ARMINO_IRQ_FIRST + 40)	/*!< 40 BLE interrupt */
#define ARMINO_IRQ_BT						(ARMINO_IRQ_FIRST + 41)	/*!< 41 BT interrupt */
#define ARMINO_IRQ_QSPI1					(ARMINO_IRQ_FIRST + 42)	/*!< 42 QSPI1 global interrupt */
#define ARMINO_IRQ_THREAD					(ARMINO_IRQ_FIRST + 48)	/*!< 48 THREAD interrupt */
#define ARMINO_IRQ_SCAL0		            (ARMINO_IRQ_FIRST + 49)	/*!< 49 SCAL0 interrupt */
#define ARMINO_IRQ_OTP						(ARMINO_IRQ_FIRST + 50)	/*!< 50 OTP interrupt */
#define ARMINO_IRQ_DPLL_UNLOCK				(ARMINO_IRQ_FIRST + 51)	/*!< 51 DPLL UNLOCK interrupt */
#define ARMINO_IRQ_GPIO_S					(ARMINO_IRQ_FIRST + 55)	/*!< 55 GPIO secure interrupt */
#define ARMINO_IRQ_GPIO_NS					(ARMINO_IRQ_FIRST + 56)	/*!< 56 GPIO non-secure interrupt */
#define ARMINO_IRQ_ABNORMAL_WAKEUP			(ARMINO_IRQ_FIRST + 57)	/*!< 57 ABNORMAL WAKEUP interrupt */
#define ARMINO_IRQ_ANA_GPIO					(ARMINO_IRQ_FIRST + 58)	/*!< 58 ANA GPIO interrupt */
#define ARMINO_IRQ_ANA_RTC					(ARMINO_IRQ_FIRST + 59)	/*!< 59 ANA RTC interrupt */


