/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#define AMEBAD_IRQ_RESERVED       (0) /* Reserved vector (only used with CONFIG_DEBUG) */
                                     /* Vector  0: Reset stack pointer value */
                                     /* Vector  1: Reset (not handler as an IRQ) */
#define AMEBAD_IRQ_NMI            (2) /* Vector  2: Non-Maskable Interrupt (NMI) */
#define AMEBAD_IRQ_HARDFAULT      (3) /* Vector  3: Hard fault */
#define AMEBAD_IRQ_MEMFAULT       (4) /* Vector  4: Memory management (MPU) */
#define AMEBAD_IRQ_BUSFAULT       (5) /* Vector  5: Bus fault */
#define AMEBAD_IRQ_USAGEFAULT     (6) /* Vector  6: Usage fault */
                                     /* Vectors 7-10: Reserved */
#define AMEBAD_IRQ_SVCALL        (11) /* Vector 11: SVC call */
#define AMEBAD_IRQ_DBGMONITOR    (12) /* Vector 12: Debug Monitor */
                                     /* Vector 13: Reserved */
#define AMEBAD_IRQ_PENDSV        (14) /* Vector 14: Pendable system service request */
#define AMEBAD_IRQ_SYSTICK       (15) /* Vector 15: System tick */

/* External interrupts (vectors >= 16).  These definitions are chip-specific */

#define AMEBAD_IRQ_FIRST         (16) /* Vector number of the first external interrupt */

#define AMEBAD_IRQ_SYSTEM_ON				(AMEBAD_IRQ_FIRST + 0)  /*!< 0 SYS Interrupt for wakeup from power save */
#define AMEBAD_IRQ_WDG						(AMEBAD_IRQ_FIRST + 1)	/*!< 1 Watch dog global insterrupt              */
#define AMEBAD_IRQ_RXI300					(AMEBAD_IRQ_FIRST + 2)	/*!< 2 RXI300 interrupt               */
#define AMEBAD_IRQ_UART_LOG					(AMEBAD_IRQ_FIRST + 3)	/*!< 3 log uart intr              */
#define AMEBAD_IRQ_GPIOA					(AMEBAD_IRQ_FIRST + 4)	/*!< 4 GPIOA portA global interrupt              */
#define AMEBAD_IRQ_RTC						(AMEBAD_IRQ_FIRST + 5)	/*!< 5 rtc timer interrupt               */
#define AMEBAD_IRQ_I2C0						(AMEBAD_IRQ_FIRST + 6)	/*!< 6 I2C0 global interrupt               */
#define AMEBAD_IRQ_SPI_FLASH				(AMEBAD_IRQ_FIRST + 7)	/*!< 7 SPI Flash global interrupt               */
#define AMEBAD_IRQ_GPIOB					(AMEBAD_IRQ_FIRST + 8)	/*!< 8 GPIOB portA global interrupt              */
#define AMEBAD_IRQ_UARTLP					(AMEBAD_IRQ_FIRST + 9)	/*!< 9 UART0 global interrupt               */
#define AMEBAD_IRQ_KEYSCAN					(AMEBAD_IRQ_FIRST + 10)	/*!< 10 KEYSCAN interrupt              */
#define AMEBAD_IRQ_CTOUCH					(AMEBAD_IRQ_FIRST + 11)	/*!< 11 Cap-Touch interrupt              */
#define AMEBAD_IRQ_BOR2						(AMEBAD_IRQ_FIRST + 12)	/*!< 12 BOR2 interrupt              */
#define AMEBAD_IRQ_SGPIO					(AMEBAD_IRQ_FIRST + 13)	/*!< 13 SGPIO interrupt              */
#define AMEBAD_IRQ_IPC						(AMEBAD_IRQ_FIRST + 14)	/*!< 14 IPC_KM0 interrupt              */
#define AMEBAD_IRQ_ADC						(AMEBAD_IRQ_FIRST + 15)	/*!< 15 adc interrupt               */
#define AMEBAD_IRQ_QDECODER					(AMEBAD_IRQ_FIRST + 16)	/*!< 16 Q-DECODER interrupt               */
#define AMEBAD_IRQ_TIMER0					(AMEBAD_IRQ_FIRST + 17)	/*!< 17 Timer0 global interrupt               */
#define AMEBAD_IRQ_TIMER1					(AMEBAD_IRQ_FIRST + 18)	/*!< 18 Timer1 global interrupt              */
#define AMEBAD_IRQ_TIMER2					(AMEBAD_IRQ_FIRST + 19)	/*!< 19 Timer2 global interrupt               */
#define AMEBAD_IRQ_TIMER3					(AMEBAD_IRQ_FIRST + 20)	/*!< 20 Timer3 global interrupt               */
#define AMEBAD_IRQ_TIMER4					(AMEBAD_IRQ_FIRST + 21)	/*!< 21 Timer4 global interrupt               */
#define AMEBAD_IRQ_TIMER5					(AMEBAD_IRQ_FIRST + 22)	/*!< 22 Timer5 global interrupt               */
#define AMEBAD_IRQ_LCDC						(AMEBAD_IRQ_FIRST + 23)	/*!< 23 LCDC interrupt               */
#define AMEBAD_IRQ_USB_OTG					(AMEBAD_IRQ_FIRST + 24)	/*!< 24 USOC interrupt              */
#define AMEBAD_IRQ_SDIO_DEVICE				(AMEBAD_IRQ_FIRST + 25)	/*!< 25 SDIO device global interrupt               */
#define AMEBAD_IRQ_SDIO_HOST				(AMEBAD_IRQ_FIRST + 26)	/*!< 26 SDIO host global interrupt               */
#define AMEBAD_IRQ_CRYPTO					(AMEBAD_IRQ_FIRST + 27)	/*!< 27 IPsec global interrupt               */
#define AMEBAD_IRQ_I2S0_PCM0				(AMEBAD_IRQ_FIRST + 28)	/*!< 28 I2S0 global interrupt               */
#define AMEBAD_IRQ_PWR_DOWN					(AMEBAD_IRQ_FIRST + 29)	/*!< 29 power down enable interrupt               */
#define AMEBAD_IRQ_ADC_COMP					(AMEBAD_IRQ_FIRST + 30)	/*!< 30 ADC compare interrupt               */
#define AMEBAD_IRQ_WL_DMA					(AMEBAD_IRQ_FIRST + 31)	/*!< 31 Wlan Host global interrupt              */
#define AMEBAD_IRQ_WL_PROTOCOL				(AMEBAD_IRQ_FIRST + 32)	/*!< 32 Wlan Firmware Wlan global interrupt              */
#define AMEBAD_IRQ_PSRAMC					(AMEBAD_IRQ_FIRST + 33)	/*!< 33 PSRAM controller interrupt              */	
#define AMEBAD_IRQ_UART0					(AMEBAD_IRQ_FIRST + 34)	/*!< 34 UART0 global interrupt               */
#define AMEBAD_IRQ_UART1					(AMEBAD_IRQ_FIRST + 35)	/*!< 35 UART1 BT UART global interrupt               */
#define AMEBAD_IRQ_SPI0						(AMEBAD_IRQ_FIRST + 36)	/*!< 36 SPI0 global interrupt for communication spi              */
#define AMEBAD_IRQ_SPI1						(AMEBAD_IRQ_FIRST + 37)	/*!< 37 SPI1 global interrupt for communication spi               */
#define AMEBAD_IRQ_USI						(AMEBAD_IRQ_FIRST + 38)	/*!< 38 USI global interrupt      */
#define AMEBAD_IRQ_IR						(AMEBAD_IRQ_FIRST + 39)	/*!< 39 IR global interrupt      */
#define AMEBAD_IRQ_BT2WL_STS				(AMEBAD_IRQ_FIRST + 40)	/*!< 40 BT to WL Status Interrupt      */

#define AMEBAD_IRQ_GDMA0_CHANNEL0			(AMEBAD_IRQ_FIRST + 41)	/*!< 41 GDMA0 channel 0 global interrupt               */
#define AMEBAD_IRQ_GDMA0_CHANNEL1			(AMEBAD_IRQ_FIRST + 42)	/*!< 42 GDMA0 channel 1 global interrupt               */
#define AMEBAD_IRQ_GDMA0_CHANNEL2			(AMEBAD_IRQ_FIRST + 43)	/*!< 43 GDMA0 channel 2 global interrupt               */
#define AMEBAD_IRQ_GDMA0_CHANNEL3			(AMEBAD_IRQ_FIRST + 44)	/*!< 44 GDMA0 channel 3 global interrupt               */
#define AMEBAD_IRQ_GDMA0_CHANNEL4			(AMEBAD_IRQ_FIRST + 45)	/*!< 45 GDMA0 channel 4 global interrupt               */
#define AMEBAD_IRQ_GDMA0_CHANNEL5			(AMEBAD_IRQ_FIRST + 46)	/*!< 46 GDMA0 channel 5 global interrupt               */

#define AMEBAD_IRQ_S_CRYPTO					(AMEBAD_IRQ_FIRST + 50)	/*!< 50 IPsec global interrupt               */
#define AMEBAD_IRQ_S_RXI300					(AMEBAD_IRQ_FIRST + 51)	/*!< 51 RXI300 interrupt               */
#define AMEBAD_IRQ_S_GDMA0_CHANNEL0			(AMEBAD_IRQ_FIRST + 52)	/*!< 52 GDMA0 channel 0 global interrupt               */
#define AMEBAD_IRQ_S_GDMA0_CHANNEL1			(AMEBAD_IRQ_FIRST + 53)	/*!< 53 GDMA0 channel 1 global interrupt               */
#define AMEBAD_IRQ_S_GDMA0_CHANNEL2			(AMEBAD_IRQ_FIRST + 54)	/*!< 54 GDMA0 channel 2 global interrupt               */
#define AMEBAD_IRQ_S_GDMA0_CHANNEL3			(AMEBAD_IRQ_FIRST + 55)	/*!< 55 GDMA0 channel 3 global interrupt               */
#define AMEBAD_IRQ_S_GDMA0_CHANNEL4			(AMEBAD_IRQ_FIRST + 56)	/*!< 56 GDMA0 channel 4 global interrupt               */
#define AMEBAD_IRQ_S_GDMA0_CHANNEL5			(AMEBAD_IRQ_FIRST + 57)	/*!< 57 GDMA0 channel 5 global interrupt               */

