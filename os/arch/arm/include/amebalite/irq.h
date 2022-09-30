/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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

#define NR_IRQS 100

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* IRQ numbers.  The IRQ number corresponds vector number and hence map directly to
 * bits in the NVIC.  This does, however, waste several words of memory in the IRQ
 * to handle mapping tables.
 */

/* Processor Exceptions (vectors 0-15) */

#define AMEBALITE_IRQ_RESERVED       (0) /* Reserved vector (only used with CONFIG_DEBUG) */
                                     /* Vector  0: Reset stack pointer value */
                                     /* Vector  1: Reset (not handler as an IRQ) */
#define AMEBALITE_IRQ_NMI            (2) /* Vector  2: Non-Maskable Interrupt (NMI) */
#define AMEBALITE_IRQ_HARDFAULT      (3) /* Vector  3: Hard fault */
#define AMEBALITE_IRQ_MEMFAULT       (4) /* Vector  4: Memory management (MPU) */
#define AMEBALITE_IRQ_BUSFAULT       (5) /* Vector  5: Bus fault */
#define AMEBALITE_IRQ_USAGEFAULT     (6) /* Vector  6: Usage fault */
                                     /* Vectors 7-10: Reserved */
#define AMEBALITE_IRQ_SVCALL        (11) /* Vector 11: SVC call */
#define AMEBALITE_IRQ_DBGMONITOR    (12) /* Vector 12: Debug Monitor */
                                     /* Vector 13: Reserved */
#define AMEBALITE_IRQ_PENDSV        (14) /* Vector 14: Pendable system service request */
#define AMEBALITE_IRQ_SYSTICK       (15) /* Vector 15: System tick */

/* External interrupts (vectors >= 16).  These definitions are chip-specific */

#define AMEBALITE_IRQ_FIRST         (16) /* Vector number of the first external interrupt */

#define AMEBALITE_IRQ_WIFI_FISR_FESR				(AMEBALITE_IRQ_FIRST + 0)
#define AMEBALITE_IRQ_WIFI_FTSR_MAILBOX				(AMEBALITE_IRQ_FIRST + 1)
#define AMEBALITE_IRQ_WL_DMA				(AMEBALITE_IRQ_FIRST + 2)
#define AMEBALITE_IRQ_WL_PROTOCOL				(AMEBALITE_IRQ_FIRST + 3)
#define AMEBALITE_IRQ_BT_SCB				(AMEBALITE_IRQ_FIRST + 4)
#define AMEBALITE_IRQ_NP_WAKE				(AMEBALITE_IRQ_FIRST + 5)
#define AMEBALITE_IRQ_DSP_WAKE				(AMEBALITE_IRQ_FIRST + 6)
#define AMEBALITE_IRQ_BT_WAKE_HOST				(AMEBALITE_IRQ_FIRST + 7)
#define AMEBALITE_IRQ_RXI300				(AMEBALITE_IRQ_FIRST + 8)
#define AMEBALITE_IRQ_SYS_ILLEGAL_WRITE				(AMEBALITE_IRQ_FIRST + 9)
#define AMEBALITE_IRQ_IPC_KM4				(AMEBALITE_IRQ_FIRST + 10)
#define AMEBALITE_IRQ_IWDG0				(AMEBALITE_IRQ_FIRST + 11)
#define AMEBALITE_IRQ_KM4_NS_WDG_INT				(AMEBALITE_IRQ_FIRST + 12)
#define AMEBALITE_IRQ_KM4_S_WDG_INT				(AMEBALITE_IRQ_FIRST + 13)
#define AMEBALITE_IRQ_KR4_WDG_RST				(AMEBALITE_IRQ_FIRST + 14)
#define AMEBALITE_IRQ_DSP_WDG_RST				(AMEBALITE_IRQ_FIRST + 15)
#define AMEBALITE_IRQ_TIMER0				(AMEBALITE_IRQ_FIRST + 16)
#define AMEBALITE_IRQ_TIMER1				(AMEBALITE_IRQ_FIRST + 17)
#define AMEBALITE_IRQ_TIMER2				(AMEBALITE_IRQ_FIRST + 18)
#define AMEBALITE_IRQ_TIMER3				(AMEBALITE_IRQ_FIRST + 19)
#define AMEBALITE_IRQ_TIMER4				(AMEBALITE_IRQ_FIRST + 20)
#define AMEBALITE_IRQ_TIMER5				(AMEBALITE_IRQ_FIRST + 21)
#define AMEBALITE_IRQ_TIMER6				(AMEBALITE_IRQ_FIRST + 22)
#define AMEBALITE_IRQ_TIMER7				(AMEBALITE_IRQ_FIRST + 23)
#define AMEBALITE_IRQ_TIMER8				(AMEBALITE_IRQ_FIRST + 24)
#define AMEBALITE_IRQ_TIMER9				(AMEBALITE_IRQ_FIRST + 25)
#define AMEBALITE_IRQ_TIMER10				(AMEBALITE_IRQ_FIRST + 26)
#define AMEBALITE_IRQ_TIMER11				(AMEBALITE_IRQ_FIRST + 27)
#define AMEBALITE_IRQ_TIMER12				(AMEBALITE_IRQ_FIRST + 28)
#define AMEBALITE_IRQ_TIMER13				(AMEBALITE_IRQ_FIRST + 29)
#define AMEBALITE_IRQ_TIMER14				(AMEBALITE_IRQ_FIRST + 30)
#define AMEBALITE_IRQ_UART0				(AMEBALITE_IRQ_FIRST + 31)
#define AMEBALITE_IRQ_UART1				(AMEBALITE_IRQ_FIRST + 32)
#define AMEBALITE_IRQ_UART2				(AMEBALITE_IRQ_FIRST + 33)
#define AMEBALITE_IRQ_UART3_BT				(AMEBALITE_IRQ_FIRST + 34)
#define AMEBALITE_IRQ_UART_LOG				(AMEBALITE_IRQ_FIRST + 35)
#define AMEBALITE_IRQ_GPIOA				(AMEBALITE_IRQ_FIRST + 36)
#define AMEBALITE_IRQ_GPIOB				(AMEBALITE_IRQ_FIRST + 37)
#define AMEBALITE_IRQ_I2C0				(AMEBALITE_IRQ_FIRST + 38)
#define AMEBALITE_IRQ_I2C1				(AMEBALITE_IRQ_FIRST + 39)
#define AMEBALITE_IRQ_CTOUCH				(AMEBALITE_IRQ_FIRST + 40)
#define AMEBALITE_IRQ_GDMA0_CHANNEL0				(AMEBALITE_IRQ_FIRST + 41)
#define AMEBALITE_IRQ_GDMA0_CHANNEL1				(AMEBALITE_IRQ_FIRST + 42)
#define AMEBALITE_IRQ_GDMA0_CHANNEL2				(AMEBALITE_IRQ_FIRST + 43)
#define AMEBALITE_IRQ_GDMA0_CHANNEL3				(AMEBALITE_IRQ_FIRST + 44)
#define AMEBALITE_IRQ_GDMA0_CHANNEL4				(AMEBALITE_IRQ_FIRST + 45)
#define AMEBALITE_IRQ_GDMA0_CHANNEL5				(AMEBALITE_IRQ_FIRST + 46)
#define AMEBALITE_IRQ_GDMA0_CHANNEL6				(AMEBALITE_IRQ_FIRST + 47)
#define AMEBALITE_IRQ_GDMA0_CHANNEL7				(AMEBALITE_IRQ_FIRST + 48)
#define AMEBALITE_IRQ_SPI0				(AMEBALITE_IRQ_FIRST + 49)
#define AMEBALITE_IRQ_SPI1				(AMEBALITE_IRQ_FIRST + 50)
#define AMEBALITE_IRQ_SPORT0				(AMEBALITE_IRQ_FIRST + 51)
#define AMEBALITE_IRQ_SPORT1				(AMEBALITE_IRQ_FIRST + 52)
#define AMEBALITE_IRQ_RTC				(AMEBALITE_IRQ_FIRST + 53)
#define AMEBALITE_IRQ_ADC				(AMEBALITE_IRQ_FIRST + 54)
#define AMEBALITE_IRQ_ADC_COMP				(AMEBALITE_IRQ_FIRST + 55)
#define AMEBALITE_IRQ_BOR				(AMEBALITE_IRQ_FIRST + 56)
#define AMEBALITE_IRQ_PWR_DOWN				(AMEBALITE_IRQ_FIRST + 57)
#define AMEBALITE_IRQ_SPI_FLASH				(AMEBALITE_IRQ_FIRST + 58)
#define AMEBALITE_IRQ_THERMAL				(AMEBALITE_IRQ_FIRST + 59)
#define AMEBALITE_IRQ_RSIP				(AMEBALITE_IRQ_FIRST + 60)
#define AMEBALITE_IRQ_AES				(AMEBALITE_IRQ_FIRST + 61)
#define AMEBALITE_IRQ_SHA				(AMEBALITE_IRQ_FIRST + 62)
#define AMEBALITE_IRQ_ECDSA				(AMEBALITE_IRQ_FIRST + 63)
#define AMEBALITE_IRQ_PSRAMC				(AMEBALITE_IRQ_FIRST + 64)
#define AMEBALITE_IRQ_TRNG				(AMEBALITE_IRQ_FIRST + 65)
#define AMEBALITE_IRQ_LEDC				(AMEBALITE_IRQ_FIRST + 66)
#define AMEBALITE_IRQ_AES_S				(AMEBALITE_IRQ_FIRST + 67)
#define AMEBALITE_IRQ_SHA_S				(AMEBALITE_IRQ_FIRST + 68)
#define AMEBALITE_IRQ_AON_TIM				(AMEBALITE_IRQ_FIRST + 69)
#define AMEBALITE_IRQ_AON_WAKEPIN				(AMEBALITE_IRQ_FIRST + 70)
#define AMEBALITE_IRQ_DSP_ERROR				(AMEBALITE_IRQ_FIRST + 71)
#define AMEBALITE_IRQ_DSP_FATAL_ERROR				(AMEBALITE_IRQ_FIRST + 72)
#define AMEBALITE_IRQ_VAD				(AMEBALITE_IRQ_FIRST + 73)
#define AMEBALITE_IRQ_ZIGBEE				(AMEBALITE_IRQ_FIRST + 74)
