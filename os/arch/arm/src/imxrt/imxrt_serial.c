/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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
 * os/arch/arm/src/imxrt/imxrt_serial.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author: Ivan Ucherdzhiev <ivanucherdjiev@gmail.com>
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

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#ifdef CONFIG_SERIAL_TERMIOS
#include <termios.h>
#endif

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/init.h>
#include <tinyara/pm/pm.h>
#include <tinyara/serial/serial.h>

#include <arch/serial.h>
#include <arch/board/board.h>

#include "chip.h"
#include "up_arch.h"
#include "up_internal.h"

#include "chip/imxrt_lpuart.h"
#include "imxrt_gpio.h"
#include "chip/imxrt_pinmux.h"
#include "imxrt_config.h"
#include "imxrt_lowputc.h"
#include "imxrt_serial.h"

#ifdef USE_SERIALDRIVER

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Which LPUART with be tty0/console and which tty1-7?  The console will
 * always be ttyS0.  If there is no console then will use the lowest
 * numbered UART.
 */

/* First pick the console and ttys0.  This could be any of LPUART1-8 */

#if defined(CONFIG_LPUART1_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart1port	/* LPUART1 is console */
#define TTYS0_DEV           g_uart1port	/* LPUART1 is ttyS0 */
#define UART1_ASSIGNED      1
#define HAVE_SERIAL_CONSOLE 1
#elif defined(CONFIG_LPUART2_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart2port	/* LPUART2 is console */
#define TTYS0_DEV           g_uart2port	/* LPUART2 is ttyS0 */
#define UART2_ASSIGNED      1
#define HAVE_SERIAL_CONSOLE 1
#elif defined(CONFIG_LPUART3_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart3port	/* LPUART3 is console */
#define TTYS0_DEV           g_uart3port	/* LPUART3 is ttyS0 */
#define UART3_ASSIGNED      1
#define HAVE_SERIAL_CONSOLE 1
#elif defined(CONFIG_LPUART4_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart4port	/* LPUART4 is console */
#define TTYS0_DEV           g_uart4port	/* LPUART4 is ttyS0 */
#define UART4_ASSIGNED      1
#define HAVE_SERIAL_CONSOLE 1
#elif defined(CONFIG_LPUART5_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart5port	/* LPUART5 is console */
#define TTYS0_DEV           g_uart5port /* LPUART5 is ttyS0 */
#define UART5_ASSIGNED      1
#define HAVE_SERIAL_CONSOLE 1
#elif defined(CONFIG_LPUART6_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart6port	/* LPUART6 is console */
#define TTYS0_DEV           g_uart6port /* LPUART6 is ttyS0 */
#define UART6_ASSIGNED      1
#define HAVE_SERIAL_CONSOLE 1
#elif defined(CONFIG_LPUART7_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart7port	/* LPUART7 is console */
#define TTYS0_DEV           g_uart7port /* LPUART7 is ttyS0 */
#define UART7_ASSIGNED      1
#define HAVE_SERIAL_CONSOLE 1
#elif defined(CONFIG_LPUART8_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart8port	/* LPUART8 is console */
#define TTYS0_DEV           g_uart8port /* LPUART8 is ttyS0 */
#define UART8_ASSIGNED      1
#define HAVE_SERIAL_CONSOLE 1
#else
#undef CONSOLE_DEV				/* No console */
#if defined(CONFIG_IMXRT_LPUART1)
#define TTYS0_DEV         g_uart1port	/* LPUART1 is ttyS0 */
#define UART1_ASSIGNED    1
#elif defined(CONFIG_IMXRT_LPUART2)
#define TTYS0_DEV         g_uart2port	/* LPUART2 is ttyS0 */
#define UART2_ASSIGNED    1
#elif defined(CONFIG_IMXRT_LPUART3)
#define TTYS0_DEV         g_uart3port	/* LPUART3 is ttyS0 */
#define UART3_ASSIGNED    1
#elif defined(CONFIG_IMXRT_LPUART4)
#define TTYS0_DEV         g_uart4port	/* LPUART4 is ttyS0 */
#define UART4_ASSIGNED    1
#elif defined(CONFIG_IMXRT_LPUART5)
#define TTYS0_DEV         g_uart5port	/* LPUART5 is ttyS0 */
#define UART5_ASSIGNED    1
#elif defined(CONFIG_IMXRT_LPUART6)
#define TTYS0_DEV         g_uart6port	/* LPUART6 is ttyS0 */
#define UART6_ASSIGNED    1
#elif defined(CONFIG_IMXRT_LPUART7)
#define TTYS0_DEV         g_uart7port	/* LPUART7 is ttyS0 */
#define UART7_ASSIGNED    1
#elif defined(CONFIG_IMXRT_LPUART8)
#define TTYS0_DEV         g_uart8port	/* LPUART8 is ttyS0 */
#define UART8_ASSIGNED    1
#endif
#endif

/* Pick ttys1.  This could be any of UART1-8 excluding the console UART.
 * One of UART1-8 could be the console; one of UART1-8 has already been
 * assigned to ttys0.
 */

#if defined(CONFIG_IMXRT_LPUART1) && !defined(UART1_ASSIGNED)
#define TTYS1_DEV           g_uart1port	/* LPUART1 is ttyS1 */
#define UART1_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART2) && !defined(UART2_ASSIGNED)
#define TTYS1_DEV           g_uart2port	/* LPUART2 is ttyS1 */
#define UART2_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART3) && !defined(UART3_ASSIGNED)
#define TTYS1_DEV           g_uart3port	/* LPUART3 is ttyS1 */
#define UART3_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART4) && !defined(UART4_ASSIGNED)
#define TTYS1_DEV           g_uart4port	/* LPUART4 is ttyS1 */
#define UART4_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART5) && !defined(UART5_ASSIGNED)
#define TTYS1_DEV           g_uart5port	/* LPUART5 is ttyS1 */
#define UART5_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART6) && !defined(UART6_ASSIGNED)
#define TTYS1_DEV           g_uart6port	/* LPUART6 is ttyS1 */
#define UART6_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART7) && !defined(UART7_ASSIGNED)
#define TTYS1_DEV           g_uart7port	/* LPUART7 is ttyS1 */
#define UART7_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART8) && !defined(UART8_ASSIGNED)
#define TTYS1_DEV           g_uart8port	/* LPUART8 is ttyS1 */
#define UART8_ASSIGNED      1
#endif

/* Pick ttys2.  This could be one of UART2-8. It can't be UART1 because that
 * was either assigned as ttyS0 or ttys1.  One of UART 1-8 could be the
 * console.  One of UART2-8 has already been assigned to ttys0 or ttyS1.
 */

#if defined(CONFIG_IMXRT_LPUART2) && !defined(UART2_ASSIGNED)
#define TTYS2_DEV           g_uart2port	/* LPUART2 is ttyS2 */
#define UART2_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART3) && !defined(UART3_ASSIGNED)
#define TTYS2_DEV           g_uart3port	/* LPUART3 is ttyS2 */
#define UART3_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART4) && !defined(UART4_ASSIGNED)
#define TTYS2_DEV           g_uart4port	/* LPUART4 is ttyS2 */
#define UART4_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART5) && !defined(UART5_ASSIGNED)
#define TTYS2_DEV           g_uart5port	/* LPUART5 is ttyS2 */
#define UART5_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART6) && !defined(UART6_ASSIGNED)
#define TTYS2_DEV           g_uart6port	/* LPUART6 is ttyS2 */
#define UART6_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART7) && !defined(UART7_ASSIGNED)
#define TTYS2_DEV           g_uart7port	/* LPUART7 is ttyS2 */
#define UART7_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART8) && !defined(UART8_ASSIGNED)
#define TTYS2_DEV           g_uart8port	/* LPUART8 is ttyS2 */
#define UART8_ASSIGNED      1
#endif

/* Pick ttys3. This could be one of UART3-8. It can't be UART1-2 because
 * those have already been assigned to ttsyS0, 1, or 2.  One of
 * UART3-8 could also be the console.  One of UART3-8 has already
 * been assigned to ttys0, 1, or 3.
 */

#if defined(CONFIG_IMXRT_LPUART3) && !defined(UART3_ASSIGNED)
#define TTYS3_DEV           g_uart3port	/* LPUART3 is ttyS3 */
#define UART3_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART4) && !defined(UART4_ASSIGNED)
#define TTYS3_DEV           g_uart4port	/* LPUART4 is ttyS3 */
#define UART4_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART5) && !defined(UART5_ASSIGNED)
#define TTYS3_DEV           g_uart5port	/* LPUART5 is ttyS3 */
#define UART5_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART6) && !defined(UART6_ASSIGNED)
#define TTYS3_DEV           g_uart6port	/* LPUART6 is ttyS3 */
#define UART6_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART7) && !defined(UART7_ASSIGNED)
#define TTYS3_DEV           g_uart7port	/* LPUART7 is ttyS3 */
#define UART7_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART8) && !defined(UART8_ASSIGNED)
#define TTYS3_DEV           g_uart8port	/* LPUART8 is ttyS3 */
#define UART8_ASSIGNED      1
#endif

/* Pick ttys4. This could be one of UART4-8. It can't be UART1-3 because
 * those have already been assigned to ttsyS0, 1, 2 or 3.  One of
 * UART 4-8 could be the console.  One of UART4-8 has already been
 * assigned to ttys0, 1, 3, or 4.
 */

#if defined(CONFIG_IMXRT_LPUART4) && !defined(UART4_ASSIGNED)
#define TTYS4_DEV           g_uart4port	/* LPUART4 is ttyS4 */
#define UART4_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART5) && !defined(UART5_ASSIGNED)
#define TTYS4_DEV           g_uart5port	/* LPUART5 is ttyS4 */
#define UART5_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART6) && !defined(UART6_ASSIGNED)
#define TTYS4_DEV           g_uart6port	/* LPUART6 is ttyS4 */
#define UART6_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART7) && !defined(UART7_ASSIGNED)
#define TTYS4_DEV           g_uart7port	/* LPUART7 is ttyS4 */
#define UART7_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART8) && !defined(UART8_ASSIGNED)
#define TTYS4_DEV           g_uart8port	/* LPUART8 is ttyS4 */
#define UART8_ASSIGNED      1
#endif

/* Pick ttys5. This could be one of UART5-8. It can't be UART1-4 because
 * those have already been assigned to ttsyS0, 1, 2, 3 or 4.  One of
 * UART 5-8 could be the console.  One of UART5-8 has already been
 * assigned to ttys0, 1, 2, 3, or 4.
 */
#if defined(CONFIG_IMXRT_LPUART5) && !defined(UART5_ASSIGNED)
#define TTYS5_DEV           g_uart5port	/* LPUART5 is ttyS5 */
#define UART5_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART6) && !defined(UART6_ASSIGNED)
#define TTYS5_DEV           g_uart6port	/* LPUART6 is ttyS5 */
#define UART6_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART7) && !defined(UART7_ASSIGNED)
#define TTYS5_DEV           g_uart7port	/* LPUART7 is ttyS5 */
#define UART7_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART8) && !defined(UART8_ASSIGNED)
#define TTYS5_DEV           g_uart8port	/* LPUART8 is ttyS5 */
#define UART8_ASSIGNED      1
#endif

/* Pick ttys6. This could be one of UART6-8. It can't be UART1-5 because
 * those have already been assigned to ttsyS0, 1, 2, 3, 4 or 5.  One of
 * UART 6-8 could be the console.  One of UART6-8 has already been
 * assigned to ttys0, 1, 2, 3, 4 or 5.
 */

#if defined(CONFIG_IMXRT_LPUART6) && !defined(UART6_ASSIGNED)
#define TTYS6_DEV           g_uart6port	/* LPUART6 is ttyS5 */
#define UART6_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART7) && !defined(UART7_ASSIGNED)
#define TTYS6_DEV           g_uart7port	/* LPUART7 is ttyS5 */
#define UART7_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART8) && !defined(UART8_ASSIGNED)
#define TTYS6_DEV           g_uart8port	/* LPUART8 is ttyS5 */
#define UART8_ASSIGNED      1
#endif

/* Pick ttys7. This could be one of UART7-8. It can't be UART1-6 because
 * those have already been assigned to ttsyS0, 1, 2, 3, 4, 5 or 6.  One of
 * UART 7-8 could be the console.  One of UART7-8 has already been
 * assigned to ttys0, 1, 2, 3, 4, 5 or 6.
 */

#if defined(CONFIG_IMXRT_LPUART7) && !defined(UART7_ASSIGNED)
#define TTYS7_DEV           g_uart7port	/* LPUART7 is ttyS5 */
#define UART7_ASSIGNED      1
#elif defined(CONFIG_IMXRT_LPUART8) && !defined(UART8_ASSIGNED)
#define TTYS7_DEV           g_uart8port	/* LPUART8 is ttyS5 */
#define UART8_ASSIGNED      1
#endif

/* UART, if available, should have been assigned to ttyS0-7. */

/* Power management definitions */

#if defined(CONFIG_PM)
#define PM_IDLE_DOMAIN      0	/* Revisit */
#endif

#ifdef CONFIG_SERIAL_IFLOWCONTROL
#define IFLOW  1
#else
#define IFLOW  0
#endif

#ifdef CONFIG_SERIAL_OFLOWCONTROL
#define OFLOW  1
#else
#define OFLOW  0
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct imxrt_uart_s {
	uint32_t uartbase;			/* Base address of UART registers */
	uint32_t baud;				/* Configured baud */
	uint32_t ie;				/* Saved enabled interrupts */
	uint8_t irq;				/* IRQ associated with this UART */
	uint8_t parity;				/* 0=none, 1=odd, 2=even */
	uint8_t bits;				/* Number of bits (7 or 8) */
#ifdef CONFIG_SERIAL_IFLOWCONTROL
	const uint32_t rts_gpio;  /* U[S]ART RTS GPIO pin configuration */
#endif
#ifdef CONFIG_SERIAL_OFLOWCONTROL
	const uint32_t cts_gpio;  /* U[S]ART CTS GPIO pin configuration */
#endif

	uint8_t  stopbits2:1; /* 1: Configure with 2 stop bits vs 1 */
#ifdef CONFIG_SERIAL_IFLOWCONTROL
	uint8_t  iflow:1;     /* input flow control (RTS) enabled */
#endif
#ifdef CONFIG_SERIAL_OFLOWCONTROL
	uint8_t  oflow:1;     /* output flow control (CTS) enabled */
#endif
	uint8_t  reserved:(7 - IFLOW + OFLOW);
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static inline uint32_t imxrt_serialin(struct imxrt_uart_s *priv, uint32_t offset);
static inline void imxrt_serialout(struct imxrt_uart_s *priv, uint32_t offset, uint32_t value);
static inline void imxrt_disableuartint(struct imxrt_uart_s *priv, uint32_t *ie);
static inline void imxrt_restoreuartint(struct imxrt_uart_s *priv, uint32_t ie);

static int imxrt_setup(struct uart_dev_s *dev);
static void imxrt_shutdown(struct uart_dev_s *dev);
static int imxrt_attach(struct uart_dev_s *dev);
static void imxrt_detach(struct uart_dev_s *dev);
static int imxrt_interrupt(int irq, void *context, FAR void *arg);
static int imxrt_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg);
static int imxrt_receive(struct uart_dev_s *dev, uint32_t *status);
static void imxrt_rxint(struct uart_dev_s *dev, bool enable);
static bool imxrt_rxavailable(struct uart_dev_s *dev);
static void imxrt_send(struct uart_dev_s *dev, int ch);
static void imxrt_txint(struct uart_dev_s *dev, bool enable);
static bool imxrt_txready(struct uart_dev_s *dev);
static bool imxrt_txempty(struct uart_dev_s *dev);

#ifdef CONFIG_PM
static void up_pm_notify(struct pm_callback_s *cb, enum pm_state_e pmstate);
static int up_pm_prepare(struct pm_callback_s *cb, enum pm_state_e pmstate);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Serial driver UART operations */

static const struct uart_ops_s g_uart_ops = {
	.setup = imxrt_setup,
	.shutdown = imxrt_shutdown,
	.attach = imxrt_attach,
	.detach = imxrt_detach,
	.ioctl = imxrt_ioctl,
	.receive = imxrt_receive,
	.rxint = imxrt_rxint,
	.rxavailable = imxrt_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
	.rxflowcontrol = NULL,
#endif
	.send = imxrt_send,
	.txint = imxrt_txint,
	.txready = imxrt_txready,
	.txempty = imxrt_txempty,
};

/* I/O buffers */

#ifdef CONFIG_IMXRT_LPUART1
static char g_uart1rxbuffer[CONFIG_LPUART1_RXBUFSIZE];
static char g_uart1txbuffer[CONFIG_LPUART1_TXBUFSIZE];
#endif

#ifdef CONFIG_IMXRT_LPUART2
static char g_uart2rxbuffer[CONFIG_LPUART2_RXBUFSIZE];
static char g_uart2txbuffer[CONFIG_LPUART2_TXBUFSIZE];
#endif

#ifdef CONFIG_IMXRT_LPUART3
static char g_uart3rxbuffer[CONFIG_LPUART3_RXBUFSIZE];
static char g_uart3txbuffer[CONFIG_LPUART3_TXBUFSIZE];
#endif

#ifdef CONFIG_IMXRT_LPUART4
static char g_uart4rxbuffer[CONFIG_LPUART4_RXBUFSIZE];
static char g_uart4txbuffer[CONFIG_LPUART4_TXBUFSIZE];
#endif

#ifdef CONFIG_IMXRT_LPUART5
static char g_uart5rxbuffer[CONFIG_LPUART5_RXBUFSIZE];
static char g_uart5txbuffer[CONFIG_LPUART5_TXBUFSIZE];
#endif

#ifdef CONFIG_IMXRT_LPUART6
static char g_uart6rxbuffer[CONFIG_LPUART6_RXBUFSIZE];
static char g_uart6txbuffer[CONFIG_LPUART6_TXBUFSIZE];
#endif

#ifdef CONFIG_IMXRT_LPUART7
static char g_uart7rxbuffer[CONFIG_LPUART7_RXBUFSIZE];
static char g_uart7txbuffer[CONFIG_LPUART7_TXBUFSIZE];
#endif

#ifdef CONFIG_IMXRT_LPUART8
static char g_uart8rxbuffer[CONFIG_LPUART8_RXBUFSIZE];
static char g_uart8txbuffer[CONFIG_LPUART8_TXBUFSIZE];
#endif

/* This describes the state of the IMXRT lpuart1 port. */

#ifdef CONFIG_IMXRT_LPUART1
static struct imxrt_uart_s g_uart1priv = {
	.uartbase = IMXRT_LPUART1_BASE,
	.baud = CONFIG_LPUART1_BAUD,
	.irq = IMXRT_IRQ_LPUART1,
	.parity = CONFIG_LPUART1_PARITY,
	.bits = CONFIG_LPUART1_BITS,
	.stopbits2 = CONFIG_LPUART1_2STOP,
#if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_LPUART1_OFLOWCONTROL)
	.oflow        = 1,
	.cts_gpio     = GPIO_LPUART1_CTS,
#endif
#if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_LPUART1_IFLOWCONTROL)
	.iflow        = 1,
	.rts_gpio     = GPIO_LPUART1_RTS,
#endif
};

static struct uart_dev_s g_uart1port = {
	.recv = {
		.size = CONFIG_LPUART1_RXBUFSIZE,
		.buffer = g_uart1rxbuffer,
	},
	.xmit = {
		.size = CONFIG_LPUART1_TXBUFSIZE,
		.buffer = g_uart1txbuffer,
	},
	.ops = &g_uart_ops,
	.priv = &g_uart1priv,
};
#endif

/* This describes the state of the IMXRT lpuart2 port. */

#ifdef CONFIG_IMXRT_LPUART2
static struct imxrt_uart_s g_uart2priv = {
	.uartbase = IMXRT_LPUART2_BASE,
	.baud = CONFIG_LPUART2_BAUD,
	.irq = IMXRT_IRQ_LPUART2,
	.parity = CONFIG_LPUART2_PARITY,
	.bits = CONFIG_LPUART2_BITS,
	.stopbits2 = CONFIG_LPUART2_2STOP,
#if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_LPUART2_OFLOWCONTROL)
	.oflow        = 1,
	.cts_gpio     = GPIO_LPUART2_CTS,
#endif
#if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_LPUART2_IFLOWCONTROL)
	.iflow        = 1,
	.rts_gpio     = GPIO_LPUART2_RTS,
#endif
};

static struct uart_dev_s g_uart2port = {
	.recv = {
		.size = CONFIG_LPUART2_RXBUFSIZE,
		.buffer = g_uart2rxbuffer,
	},
	.xmit = {
		.size = CONFIG_LPUART2_TXBUFSIZE,
		.buffer = g_uart2txbuffer,
	},
	.ops = &g_uart_ops,
	.priv = &g_uart2priv,
};
#endif

#ifdef CONFIG_IMXRT_LPUART3
static struct imxrt_uart_s g_uart3priv = {
	.uartbase = IMXRT_LPUART3_BASE,
	.baud = CONFIG_LPUART3_BAUD,
	.irq = IMXRT_IRQ_LPUART3,
	.parity = CONFIG_LPUART3_PARITY,
	.bits = CONFIG_LPUART3_BITS,
	.stopbits2 = CONFIG_LPUART3_2STOP,
#if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_LPUART3_OFLOWCONTROL)
	.oflow        = 1,
	.cts_gpio     = GPIO_LPUART3_CTS,
#endif
#if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_LPUART3_IFLOWCONTROL)
	.iflow        = 1,
	.rts_gpio     = GPIO_LPUART3_RTS,
#endif
};

static struct uart_dev_s g_uart3port = {
	.recv = {
		.size = CONFIG_LPUART3_RXBUFSIZE,
		.buffer = g_uart3rxbuffer,
	},
	.xmit = {
		.size = CONFIG_LPUART3_TXBUFSIZE,
		.buffer = g_uart3txbuffer,
	},
	.ops = &g_uart_ops,
	.priv = &g_uart3priv,
};
#endif

#ifdef CONFIG_IMXRT_LPUART4
static struct imxrt_uart_s g_uart4priv = {
	.uartbase = IMXRT_LPUART4_BASE,
	.baud = CONFIG_LPUART4_BAUD,
	.irq = IMXRT_IRQ_LPUART4,
	.parity = CONFIG_LPUART4_PARITY,
	.bits = CONFIG_LPUART4_BITS,
	.stopbits2 = CONFIG_LPUART4_2STOP,
#if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_LPUART4_OFLOWCONTROL)
	.oflow        = 1,
	.cts_gpio     = GPIO_LPUART4_CTS,
#endif
#if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_LPUART4_IFLOWCONTROL)
	.iflow        = 1,
	.rts_gpio     = GPIO_LPUART4_RTS,
#endif
};

static struct uart_dev_s g_uart4port = {
	.recv = {
		.size = CONFIG_LPUART4_RXBUFSIZE,
		.buffer = g_uart4rxbuffer,
	},
	.xmit = {
		.size = CONFIG_LPUART4_TXBUFSIZE,
		.buffer = g_uart4txbuffer,
	},
	.ops = &g_uart_ops,
	.priv = &g_uart4priv,
};
#endif

#ifdef CONFIG_IMXRT_LPUART5
static struct imxrt_uart_s g_uart5priv = {
	.uartbase = IMXRT_LPUART5_BASE,
	.baud = CONFIG_LPUART5_BAUD,
	.irq = IMXRT_IRQ_LPUART5,
	.parity = CONFIG_LPUART5_PARITY,
	.bits = CONFIG_LPUART5_BITS,
	.stopbits2 = CONFIG_LPUART5_2STOP,
#if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_LPUART5_OFLOWCONTROL)
	.oflow        = 1,
	.cts_gpio     = GPIO_LPUART5_CTS,
#endif
#if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_LPUART5_IFLOWCONTROL)
	.iflow        = 1,
	.rts_gpio     = GPIO_LPUART5_RTS,
#endif
};

static struct uart_dev_s g_uart5port = {
	.recv = {
		.size = CONFIG_LPUART5_RXBUFSIZE,
		.buffer = g_uart5rxbuffer,
	},
	.xmit = {
		.size = CONFIG_LPUART5_TXBUFSIZE,
		.buffer = g_uart5txbuffer,
	},
	.ops = &g_uart_ops,
	.priv = &g_uart5priv,
};
#endif

#ifdef CONFIG_IMXRT_LPUART6
static struct imxrt_uart_s g_uart6priv = {
	.uartbase = IMXRT_LPUART6_BASE,
	.baud = CONFIG_LPUART6_BAUD,
	.irq = IMXRT_IRQ_LPUART6,
	.parity = CONFIG_LPUART6_PARITY,
	.bits = CONFIG_LPUART6_BITS,
	.stopbits2 = CONFIG_LPUART6_2STOP,
#if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_LPUART6_OFLOWCONTROL)
	.oflow        = 1,
	.cts_gpio     = GPIO_LPUART6_CTS,
#endif
#if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_LPUART6_IFLOWCONTROL)
	.iflow        = 1,
	.rts_gpio     = GPIO_LPUART6_RTS,
#endif
};

static struct uart_dev_s g_uart6port = {
	.recv = {
		.size = CONFIG_LPUART6_RXBUFSIZE,
		.buffer = g_uart6rxbuffer,
	},
	.xmit = {
		.size = CONFIG_LPUART6_TXBUFSIZE,
		.buffer = g_uart6txbuffer,
	},
	.ops = &g_uart_ops,
	.priv = &g_uart6priv,
};
#endif

#ifdef CONFIG_IMXRT_LPUART7
static struct imxrt_uart_s g_uart7priv = {
	.uartbase = IMXRT_LPUART7_BASE,
	.baud = CONFIG_LPUART7_BAUD,
	.irq = IMXRT_IRQ_LPUART7,
	.parity = CONFIG_LPUART7_PARITY,
	.bits = CONFIG_LPUART7_BITS,
	.stopbits2 = CONFIG_LPUART7_2STOP,
#if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_LPUART7_OFLOWCONTROL)
	.oflow        = 1,
	.cts_gpio     = GPIO_LPUART7_CTS,
#endif
#if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_LPUART7_IFLOWCONTROL)
	.iflow        = 1,
	.rts_gpio     = GPIO_LPUART7_RTS,
#endif
};

static struct uart_dev_s g_uart7port = {
	.recv = {
		.size = CONFIG_LPUART7_RXBUFSIZE,
		.buffer = g_uart7rxbuffer,
	},
	.xmit = {
		.size = CONFIG_LPUART7_TXBUFSIZE,
		.buffer = g_uart7txbuffer,
	},
	.ops = &g_uart_ops,
	.priv = &g_uart7priv,
};
#endif

#ifdef CONFIG_IMXRT_LPUART8
static struct imxrt_uart_s g_uart8priv = {
	.uartbase = IMXRT_LPUART8_BASE,
	.baud = CONFIG_LPUART8_BAUD,
	.irq = IMXRT_IRQ_LPUART8,
	.parity = CONFIG_LPUART8_PARITY,
	.bits = CONFIG_LPUART8_BITS,
	.stopbits2 = CONFIG_LPUART8_2STOP,
#if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_LPUART8_OFLOWCONTROL)
	.oflow        = 1,
	.cts_gpio     = GPIO_LPUART8_CTS,
#endif
#if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_LPUART8_IFLOWCONTROL)
	.iflow        = 1,
	.rts_gpio     = GPIO_LPUART8_RTS,
#endif
};

static struct uart_dev_s g_uart8port = {
	.recv = {
		.size = CONFIG_LPUART8_RXBUFSIZE,
		.buffer = g_uart8rxbuffer,
	},
	.xmit = {
		.size = CONFIG_LPUART8_TXBUFSIZE,
		.buffer = g_uart8txbuffer,
	},
	.ops = &g_uart_ops,
	.priv = &g_uart8priv,
};
#endif

#ifdef CONFIG_PM
static struct pm_callback_s g_serial_pmcb = {
	.notify = up_pm_notify,
	.prepare = up_pm_prepare,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: imxrt_serialin
 ****************************************************************************/

static inline uint32_t imxrt_serialin(struct imxrt_uart_s *priv, uint32_t offset)
{
	return getreg32(priv->uartbase + offset);
}

/****************************************************************************
 * Name: imxrt_serialout
 ****************************************************************************/

static inline void imxrt_serialout(struct imxrt_uart_s *priv, uint32_t offset, uint32_t value)
{
	putreg32(value, priv->uartbase + offset);
}

/****************************************************************************
 * Name: imxrt_disableuartint
 ****************************************************************************/

static inline void imxrt_disableuartint(struct imxrt_uart_s *priv, uint32_t *ie)
{
	irqstate_t flags;
	uint32_t regval;

	flags = irqsave();
	regval = imxrt_serialin(priv, IMXRT_LPUART_CTRL_OFFSET);

	/* Return the current Rx and Tx interrupt state */

	if (ie != NULL) {
		*ie = regval & LPUART_ALL_INTS;
	}

	regval &= ~LPUART_ALL_INTS;
	imxrt_serialout(priv, IMXRT_LPUART_CTRL_OFFSET, regval);
	irqrestore(flags);
}

/****************************************************************************
 * Name: imxrt_restoreuartint
 ****************************************************************************/

static inline void imxrt_restoreuartint(struct imxrt_uart_s *priv, uint32_t ie)
{
	irqstate_t flags;
	uint32_t regval;

	/* Enable/disable any interrupts that are currently disabled but should be
	 * enabled/disabled.
	 */

	flags = irqsave();
	regval = imxrt_serialin(priv, IMXRT_LPUART_CTRL_OFFSET);
	regval &= ~LPUART_ALL_INTS;
	regval |= ie;
	imxrt_serialout(priv, IMXRT_LPUART_CTRL_OFFSET, regval);
	irqrestore(flags);
}

/****************************************************************************
 * Name: imxrt_setup
 *
 * Description:
 *   Configure the UART baud, bits, parity, fifos, etc. This
 *   method is called the first time that the serial port is
 *   opened.
 *
 ****************************************************************************/

static int imxrt_setup(struct uart_dev_s *dev)
{
	struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;
#ifndef CONFIG_SUPPRESS_LPUART_CONFIG
	struct uart_config_s config;
	int ret;

	/* Configure the UART */

	config.baud = priv->baud;	/* Configured baud */
	config.parity = priv->parity;	/* 0=none, 1=odd, 2=even */
	config.bits = priv->bits;	/* Number of bits (5-9) */
	config.stopbits2 = priv->stopbits2;	/* true: Configure with 2 stop bits instead of 1 */

	ret = imxrt_lpuart_configure(priv->uartbase, &config);

	priv->ie = imxrt_serialin(priv, IMXRT_LPUART_CTRL_OFFSET) & LPUART_ALL_INTS;
	return ret;

#else
	priv->ie = imxrt_serialin(priv, IMXRT_LPUART_CTRL_OFFSET) & LPUART_ALL_INTS;
	return OK;
#endif
}

/****************************************************************************
 * Name: imxrt_shutdown
 *
 * Description:
 *   Disable the UART.  This method is called when the serial
 *   port is closed
 *
 ****************************************************************************/

static void imxrt_shutdown(struct uart_dev_s *dev)
{
	struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;

	/* Disable the UART */

	imxrt_serialout(priv, IMXRT_LPUART_GLOBAL_OFFSET, LPUART_GLOBAL_RST(1U));
}

/****************************************************************************
 * Name: imxrt_attach
 *
 * Description:
 *   Configure the UART to operation in interrupt driven mode.  This method
 *   is called when the serial port is opened.  Normally, this is just after
 *   the setup() method is called, however, the serial console may operate
 *   in a non-interrupt driven mode during the boot phase.
 *
 *   RX and TX interrupts are not enabled when by the attach method (unless
 *   the hardware supports multiple levels of interrupt enabling).  The RX
 *   and TX interrupts are not enabled until the txint() and rxint() methods
 *   are called.
 *
 ****************************************************************************/

static int imxrt_attach(struct uart_dev_s *dev)
{
	struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;
	int ret;

	/* Attach and enable the IRQ */

	ret = irq_attach(priv->irq, imxrt_interrupt, dev);
	if (ret == OK) {
		/* Enable the interrupt (RX and TX interrupts are still disabled
		 * in the UART
		 */

		up_enable_irq(priv->irq);
	}

	return ret;
}

/****************************************************************************
 * Name: imxrt_detach
 *
 * Description:
 *   Detach UART interrupts.  This method is called when the serial port is
 *   closed normally just before the shutdown method is called.  The
 *   exception is the serial console which is never shutdown.
 *
 ****************************************************************************/

static void imxrt_detach(struct uart_dev_s *dev)
{
	struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;

	up_disable_irq(priv->irq);
	irq_detach(priv->irq);
}

/****************************************************************************
 * Name: imxrt_interrupt (and front-ends)
 *
 * Description:
 *   This is the common UART interrupt handler.  It should cal
 *   uart_transmitchars or uart_receivechar to perform the appropriate data
 *   transfers.
 *
 ****************************************************************************/

static int imxrt_interrupt(int irq, void *context, FAR void *arg)
{
	struct uart_dev_s *dev = (struct uart_dev_s *)arg;
	struct imxrt_uart_s *priv;
	uint32_t usr;
	int passes = 0;
	bool handled;

	DEBUGASSERT(dev != NULL && dev->priv != NULL);
	priv = (struct imxrt_uart_s *)dev->priv;

	/* Loop until there are no characters to be transferred or,
	 * until we have been looping for a long time.
	 */

	handled = true;
	for (passes = 0; passes < 256 && handled; passes++) {
		handled = false;

		/* Get the current UART status and check for loop
		 * termination conditions
		 */

		usr = imxrt_serialin(priv, IMXRT_LPUART_STAT_OFFSET);
		usr &= (LPUART_STAT_RDRF(1U) | LPUART_STAT_TC(1U) | LPUART_STAT_OR(1U) | LPUART_STAT_FE(1U));

		/* Clear serial overrun and framing errors */

		if ((usr & LPUART_STAT_OR(1U)) != 0) {
			imxrt_serialout(priv, IMXRT_LPUART_STAT_OFFSET, LPUART_STAT_OR(1U));
		}

		if ((usr & LPUART_STAT_FE(1U)) != 0) {
			imxrt_serialout(priv, IMXRT_LPUART_STAT_OFFSET, LPUART_STAT_FE(1U));
		}

		/* Handle incoming, receive bytes */

		if ((usr & LPUART_STAT_RDRF(1U)) != 0 && (priv->ie & LPUART_CTRL_RIE(1U)) != 0) {
			uart_recvchars(dev);
			handled = true;
		}

		/* Handle outgoing, transmit bytes */

		if ((usr & LPUART_STAT_TC(1U)) != 0 && (priv->ie & LPUART_CTRL_TCIE(1U)) != 0) {
			uart_xmitchars(dev);
			handled = true;
		}
	}

	return OK;
}

/****************************************************************************
 * Name: imxrt_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method
 *
 ****************************************************************************/

static int imxrt_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg)
{
	int ret = OK;

	switch (cmd) {
#ifdef CONFIG_SERIAL_TIOCSERGSTRUCT
	case TIOCSERGSTRUCT: {
		struct imxrt_uart_s *user = (struct imxrt_uart_s *)arg;
		if (!user) {
			ret = -EINVAL;
		} else {
			memcpy(user, dev, sizeof(struct imxrt_uart_s));
		}
	}
	break;
#endif

#ifdef CONFIG_SERIAL_TERMIOS
	case TCGETS: {
		struct termios *termiosp = (struct termios *)arg;
		struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;

		if (!termiosp) {
			ret = -EINVAL;
			break;
		}

		/* Return baud */

		cfsetispeed(termiosp, priv->baud);

		/* Return parity */

		termiosp->c_cflag = ((priv->parity != 0) ? PARENB : 0) | ((priv->parity == 1) ? PARODD : 0);

		/* Return stop bits */

		termiosp->c_cflag |= (priv->stopbits2) ? CSTOPB : 0;

		/* Return flow control */

#ifdef CONFIG_SERIAL_OFLOWCONTROL
		termiosp->c_cflag |= ((priv->oflow) ? CCTS_OFLOW : 0);
#endif
#ifdef CONFIG_SERIAL_IFLOWCONTROL
		termiosp->c_cflag |= ((priv->iflow) ? CRTS_IFLOW : 0);
#endif
		/* Return number of bits */

		switch (priv->bits) {
		case 5:
			termiosp->c_cflag |= CS5;
			break;

		case 6:
			termiosp->c_cflag |= CS6;
			break;

		case 7:
			termiosp->c_cflag |= CS7;
			break;

		default:
		case 8:
			termiosp->c_cflag |= CS8;
			break;

		case 9:
			termiosp->c_cflag |= CS8 /* CS9 */ ;
			break;
		}
	}
	break;

	case TCSETS: {
		struct termios *termiosp = (struct termios *)arg;
		struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;
		uint32_t baud;
		uint32_t ie;
		uint8_t parity;
		uint8_t nbits;
		bool stop2;

		if ((!termiosp)
#ifdef CONFIG_SERIAL_OFLOWCONTROL
			|| ((termiosp->c_cflag & CCTS_OFLOW) && (priv->cts_gpio == 0))
#endif
#ifdef CONFIG_SERIAL_IFLOWCONTROL
			|| ((termiosp->c_cflag & CRTS_IFLOW) && (priv->rts_gpio == 0))
#endif
		) {
			ret = -EINVAL;
			break;
		}

		/* Decode baud. */

		ret = OK;
		baud = cfgetispeed(termiosp);

		/* Decode number of bits */

		switch (termiosp->c_cflag & CSIZE) {
		case CS5:
			nbits = 5;
			break;

		case CS6:
			nbits = 6;
			break;

		case CS7:
			nbits = 7;
			break;

		case CS8:
			nbits = 8;
			break;
#if 0
		case CS9:
			nbits = 9;
			break;
#endif
		default:
			ret = -EINVAL;
			break;
		}

		/* Decode parity */

		if ((termiosp->c_cflag & PARENB) != 0) {
			parity = (termiosp->c_cflag & PARODD) ? 1 : 2;
		} else {
			parity = 0;
		}

		/* Decode stop bits */

		stop2 = (termiosp->c_cflag & CSTOPB) != 0;

		/* Verify that all settings are valid before committing */

		if (ret == OK) {
			/* Commit */

			priv->baud = baud;
			priv->parity = parity;
			priv->bits = nbits;
			priv->stopbits2 = stop2;
#ifdef CONFIG_SERIAL_OFLOWCONTROL
			priv->oflow     = (termiosp->c_cflag & CCTS_OFLOW) != 0;
#endif
#ifdef CONFIG_SERIAL_IFLOWCONTROL
			priv->iflow     = (termiosp->c_cflag & CRTS_IFLOW) != 0;
#endif
			/* effect the changes immediately - note that we do not
			 * implement TCSADRAIN / TCSAFLUSH
			 */

			imxrt_disableuartint(priv, &ie);
			ret = imxrt_setup(dev);

			/* Restore the interrupt state */

			imxrt_restoreuartint(priv, ie);
			priv->ie = ie;
		}
	}
	break;
#endif							/* CONFIG_SERIAL_TERMIOS */

	case TIOCSBRK:				/* BSD compatibility: Turn break on, unconditionally */
	case TIOCCBRK:				/* BSD compatibility: Turn break off, unconditionally */
	default:
		ret = -ENOTTY;
		break;
	}

	return ret;
}

/****************************************************************************
 * Name: imxrt_receive
 *
 * Description:
 *   Called (usually) from the interrupt level to receive one
 *   character from the UART.  Error bits associated with the
 *   receipt are provided in the return 'status'.
 *
 ****************************************************************************/

static int imxrt_receive(struct uart_dev_s *dev, uint32_t *status)
{
	struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;
	uint32_t rxd;

	rxd = imxrt_serialin(priv, IMXRT_LPUART_DATA_OFFSET);
	*status = rxd >> LPUART_DATA_STATUS_SHIFT;
	return (rxd & LPUART_DATA_MASK) >> LPUART_DATA_SHIFT;
}

/****************************************************************************
 * Name: imxrt_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts
 *
 ****************************************************************************/

static void imxrt_rxint(struct uart_dev_s *dev, bool enable)
{
	struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;
	irqstate_t flags;
	uint32_t regval;

	/* Enable interrupts for data available at Rx */

	flags = irqsave();
	if (enable) {
#ifndef CONFIG_SUPPRESS_SERIAL_INTS
		priv->ie |= LPUART_CTRL_RIE(1U) | LPUART_CTRL_FEIE(1U) | LPUART_CTRL_ORIE(1U);
#endif
	} else {
		priv->ie &= ~(LPUART_CTRL_RIE(1U) | LPUART_CTRL_FEIE(1U) | LPUART_CTRL_ORIE(1U));
	}

	regval = imxrt_serialin(priv, IMXRT_LPUART_CTRL_OFFSET);
	regval &= ~LPUART_ALL_INTS;
	regval |= priv->ie;
	imxrt_serialout(priv, IMXRT_LPUART_CTRL_OFFSET, regval);
	irqrestore(flags);
}

/****************************************************************************
 * Name: imxrt_rxavailable
 *
 * Description:
 *   Return true if the receive fifo is not empty
 *
 ****************************************************************************/

static bool imxrt_rxavailable(struct uart_dev_s *dev)
{
	struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;
	uint32_t regval;

	/* Return true is data is ready in the Rx FIFO */

	regval = imxrt_serialin(priv, IMXRT_LPUART_STAT_OFFSET);
	return ((regval & LPUART_STAT_RDRF(1U)) != 0);
}

/****************************************************************************
 * Name: imxrt_send
 *
 * Description:
 *   This method will send one byte on the UART
 *
 ****************************************************************************/

static void imxrt_send(struct uart_dev_s *dev, int ch)
{
	struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;
	imxrt_serialout(priv, IMXRT_LPUART_DATA_OFFSET, (uint32_t)ch);
}

/****************************************************************************
 * Name: imxrt_txint
 *
 * Description:
 *   Call to enable or disable TX interrupts
 *
 ****************************************************************************/

static void imxrt_txint(struct uart_dev_s *dev, bool enable)
{
	struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;
	irqstate_t flags;
	uint32_t regval;

	/* Enable interrupt for TX complete */

	flags = irqsave();
	if (enable) {
#ifndef CONFIG_SUPPRESS_SERIAL_INTS
		priv->ie |= LPUART_CTRL_TCIE(1U);
#endif
	} else {
		priv->ie &= ~LPUART_CTRL_TCIE(1U);
	}

	regval = imxrt_serialin(priv, IMXRT_LPUART_CTRL_OFFSET);
	regval &= ~LPUART_ALL_INTS;
	regval |= priv->ie;
	imxrt_serialout(priv, IMXRT_LPUART_CTRL_OFFSET, regval);
	irqrestore(flags);
}

/****************************************************************************
 * Name: imxrt_txready
 *
 * Description:
 *   Return true if the transmit is completed
 *
 ****************************************************************************/

static bool imxrt_txready(struct uart_dev_s *dev)
{
	struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;
	uint32_t regval;

	regval = imxrt_serialin(priv, IMXRT_LPUART_STAT_OFFSET);
	return ((regval & LPUART_STAT_TC(1U)) != 0);
}

/****************************************************************************
 * Name: imxrt_txempty
 *
 * Description:
 *   Return true if the transmit reg is empty
 *
 ****************************************************************************/

static bool imxrt_txempty(struct uart_dev_s *dev)
{
	struct imxrt_uart_s *priv = (struct imxrt_uart_s *)dev->priv;
	uint32_t regval;

	regval = imxrt_serialin(priv, IMXRT_LPUART_STAT_OFFSET);
	return ((regval & LPUART_STAT_TDRE(1U)) != 0);
}

/****************************************************************************
 * Name: up_pm_notify
 *
 * Description:
 *   Notify the driver of new power state. This callback is  called after
 *   all drivers have had the opportunity to prepare for the new power state.
 *
 * Input Parameters:
 *
 *    cb - Returned to the driver. The driver version of the callback
 *         structure may include additional, driver-specific state data at
 *         the end of the structure.
 *
 *    pmstate - Identifies the new PM state
 *
 * Returned Value:
 *   None - The driver already agreed to transition to the low power
 *   consumption state when when it returned OK to the prepare() call.
 *
 ****************************************************************************/

#ifdef CONFIG_PM
static void up_pm_notify(struct pm_callback_s *cb, enum pm_state_e pmstate)
{
	switch (pmstate) {
	case (PM_NORMAL): {
			/* Logic for PM_NORMAL goes here */

		}
		break;

	case (PM_IDLE): {
		/* Logic for PM_IDLE goes here */

	}
	break;

	case (PM_STANDBY): {
		/* Logic for PM_STANDBY goes here */

	}
	break;

	case (PM_SLEEP): {
		/* Logic for PM_SLEEP goes here */

	}
	break;

	default:
		/* Should not get here */
		break;
	}
}
#endif

/****************************************************************************
 * Name: up_pm_prepare
 *
 * Description:
 *   Request the driver to prepare for a new power state. This is a warning
 *   that the system is about to enter into a new power state. The driver
 *   should begin whatever operations that may be required to enter power
 *   state. The driver may abort the state change mode by returning a
 *   non-zero value from the callback function.
 *
 * Input Parameters:
 *
 *    cb - Returned to the driver. The driver version of the callback
 *         structure may include additional, driver-specific state data at
 *         the end of the structure.
 *
 *    pmstate - Identifies the new PM state
 *
 * Returned Value:
 *   Zero - (OK) means the event was successfully processed and that the
 *          driver is prepared for the PM state change.
 *
 *   Non-zero - means that the driver is not prepared to perform the tasks
 *              needed achieve this power setting and will cause the state
 *              change to be aborted. NOTE: The prepare() method will also
 *              be called when reverting from lower back to higher power
 *              consumption modes (say because another driver refused a
 *              lower power state change). Drivers are not permitted to
 *              return non-zero values when reverting back to higher power
 *              consumption modes!
 *
 *
 ****************************************************************************/

#ifdef CONFIG_PM
static int up_pm_prepare(struct pm_callback_s *cb, enum pm_state_e pmstate)
{
	/* Logic to prepare for a reduced power state goes here. */

	return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: imxrt_earlyserialinit
 *
 * Description:
 *   Performs the low level UART initialization early in debug so that the
 *   serial console will be available during bootup.  This must be called
 *   before up_serialinit.
 *
 ****************************************************************************/

void up_earlyserialinit(void)
{
	/* NOTE: This function assumes that low level hardware configuration
	 * -- including all clocking and pin configuration -- was performed by the
	 * function imxrt_lowsetup() earlier in the boot sequence.
	 */

	/* Enable the console UART.  The other UARTs will be initialized if and
	 * when they are first opened.
	 */

#ifdef CONSOLE_DEV
	CONSOLE_DEV.isconsole = true;
	CONSOLE_DEV.ops->setup(&CONSOLE_DEV);
#endif
}

/****************************************************************************
 * Name: up_serialinit
 *
 * Description:
 *   Register serial console and serial ports.  This assumes
 *   that imxrt_earlyserialinit was called previously.
 *
 ****************************************************************************/

void up_serialinit(void)
{
#ifdef CONFIG_PM
	int ret;

	/* Register to receive power management callbacks */

	ret = pm_register(&g_serial_pmcb);
	DEBUGASSERT(ret == OK);
	UNUSED(ret);
#endif

#ifdef CONSOLE_DEV
	(void)uart_register("/dev/console", &CONSOLE_DEV);
#endif

	/* Register all UARTs */

	(void)uart_register("/dev/ttyS0", &TTYS0_DEV);
#ifdef TTYS1_DEV
	(void)uart_register("/dev/ttyS1", &TTYS1_DEV);
#endif
#ifdef TTYS2_DEV
	(void)uart_register("/dev/ttyS2", &TTYS2_DEV);
#endif
#ifdef TTYS3_DEV
	(void)uart_register("/dev/ttyS3", &TTYS3_DEV);
#endif
#ifdef TTYS4_DEV
	(void)uart_register("/dev/ttyS4", &TTYS4_DEV);
#endif
#ifdef TTYS5_DEV
	(void)uart_register("/dev/ttyS5", &TTYS5_DEV);
#endif
#ifdef TTYS6_DEV
	(void)uart_register("/dev/ttyS6", &TTYS6_DEV);
#endif
#ifdef TTYS7_DEV
	(void)uart_register("/dev/ttyS7", &TTYS7_DEV);
#endif
}

/****************************************************************************
 * Name: up_putc
 *
 * Description:
 *   Provide priority, low-level access to support OS debug  writes
 *
 ****************************************************************************/

int up_putc(int ch)
{
#ifdef HAVE_SERIAL_CONSOLE
	struct imxrt_uart_s *priv = CONSOLE_DEV.priv;
	uint32_t ie;

	imxrt_disableuartint(priv, &ie);

	/* Check for LF */

	if (ch == '\n') {
		/* Add CR */
		imxrt_lowputc('\r');
	}

	imxrt_lowputc(ch);
	imxrt_restoreuartint(priv, ie);
#endif

	return ch;
}

#else							/* USE_SERIALDRIVER */

/****************************************************************************
 * Name: up_putc
 *
 * Description:
 *   Provide priority, low-level access to support OS debug writes
 *
 ****************************************************************************/

int up_putc(int ch)
{
#if CONSOLE_LPUART > 0
	/* Check for LF */

	if (ch == '\n') {
		/* Add CR */

		up_lowputc('\r');
	}

	up_lowputc(ch);
#endif

	return ch;
}

#endif							/* USE_SERIALDRIVER */
