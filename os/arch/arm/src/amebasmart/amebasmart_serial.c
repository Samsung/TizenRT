/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/serial/serial.h>
#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
#endif

#ifdef CONFIG_SERIAL_TERMIOS
#include <termios.h>
#endif

#include <arch/serial.h>
#include <arch/board/board.h>

#include "chip.h"
#include "up_arch.h"
#include "up_internal.h"

#include "serial_api.h"
#include "PinNames.h"
#include "objects.h"
#include "ameba_uart.h"
#include "tinyara/kmalloc.h"
#include "ameba_vector.h"

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/
/*
 * If we are not using the serial driver for the console, then we
 * still must provide some minimal implementation of up_putc.
 */

#undef TTYS0_DEV
#undef TTYS1_DEV
#undef TTYS2_DEV

#undef UART0_ASSIGNED
#undef UART1_ASSIGNED
#undef UART2_ASSIGNED

/* Which UART with be ttyS0/console and which ttyS1? ttyS2 */
/* First pick the console and ttys0. This could be any of UART0-2 */
#if defined(CONFIG_UART0_SERIAL_CONSOLE)
#define CONSOLE_DEV             g_uart0port             /* UART0 is console */
#define TTYS0_DEV               g_uart0port             /* UART0 is ttyS0 */
#define CONSOLE                 UART0_DEV
#define UART0_ASSIGNED  1
#define HAVE_SERIAL_CONSOLE
#elif defined(CONFIG_UART1_SERIAL_CONSOLE)
#define CONSOLE_DEV             g_uart1port             /* UART1 is console */
#define TTYS0_DEV               g_uart1port             /* UART1 is ttyS0 */
#define CONSOLE                 UART1_DEV
#define UART1_ASSIGNED  1
#define HAVE_SERIAL_CONSOLE
#elif defined(CONFIG_UART2_SERIAL_CONSOLE)
#define CONSOLE_DEV             g_uart2port             /* UART2 is console */
#define TTYS0_DEV               g_uart2port             /* UART2 is ttyS0 */
#define CONSOLE                 UART2_DEV
#define UART2_ASSIGNED  1
#define HAVE_SERIAL_CONSOLE
#elif defined(CONFIG_UART3_SERIAL_CONSOLE)
#define CONSOLE_DEV             g_uart3port             /* UART3 is console */
#define TTYS0_DEV               g_uart3port             /* UART3 is ttyS0 */
#define CONSOLE                 UART3_DEV
#define UART3_ASSIGNED  1
#define HAVE_SERIAL_CONSOLE
#elif defined(CONFIG_UART4_SERIAL_CONSOLE)
#define CONSOLE_DEV             g_uart4port             /* UART4 is console */
#define TTYS0_DEV               g_uart4port             /* UART4 is ttyS0 */
#define CONSOLE                 LOGUART_DEV
#define UART4_ASSIGNED  1
#define HAVE_SERIAL_CONSOLE
#else
#undef CONSOLE_DEV                                              /* No console */
#if defined(CONFIG_RTL8730E_UART0)
#define TTYS0_DEV               g_uart0port             /* UART0 is ttyS0 */
#define UART0_ASSIGNED  1
#elif defined(CONFIG_RTL8730E_UART1)
#define TTYS0_DEV               g_uart1port             /* UART1 is ttyS0 */
#define UART1_ASSIGNED  1
#elif defined(CONFIG_RTL8730E_UART2)
#define TTYS0_DEV               g_uart2port             /* UART2 is ttyS0 */
#define UART2_ASSIGNED  1
#elif defined(CONFIG_RTL8730E_UART3)
#define TTYS0_DEV               g_uart3port             /* UART3 is ttyS0 */
#define UART3_ASSIGNED  1
#elif defined(CONFIG_RTL8730E_UART4)
#define TTYS0_DEV               g_uart4port             /* UART4 is ttyS0 */
#define UART4_ASSIGNED  1
#endif
#endif

/* Pick ttyS1. This could be any of UART0-2 excluding the console UART. */
#if defined(CONFIG_RTL8730E_UART0) && !defined(UART0_ASSIGNED)
#define TTYS1_DEV               g_uart0port             /* UART0 is ttyS1 */
#define UART0_ASSIGNED  1
#elif defined(CONFIG_RTL8730E_UART1) && !defined(UART1_ASSIGNED)
#define TTYS1_DEV               g_uart1port             /* UART1 is ttyS1 */
#define UART1_ASSIGNED  1
#elif defined(CONFIG_RTL8730E_UART2) && !defined(UART2_ASSIGNED)
#define TTYS1_DEV               g_uart2port             /* UART2 is ttyS1 */
#define UART2_ASSIGNED  1
#elif defined(CONFIG_RTL8730E_UART3) && !defined(UART3_ASSIGNED)
#define TTYS1_DEV               g_uart3port             /* UART3 is ttyS1 */
#define UART3_ASSIGNED  1
#elif defined(CONFIG_RTL8730E_UART4) && !defined(UART4_ASSIGNED)
#define TTYS1_DEV               g_uart4port             /* UART4 is ttyS1 */
#define UART4_ASSIGNED  1
#endif

/*
 * Pick ttyS2. This could be one of UART1-2. It can't be UART0 because that
 * was either assigned as ttyS0 or ttyS1. One of these could also be the
 * console
 */
#if defined(CONFIG_RTL8730E_UART1) && !defined(UART1_ASSIGNED)
#define TTYS2_DEV               g_uart1port             /* UART1 is ttyS2 */
#define UART1_ASSIGNED  1
#elif defined(CONFIG_RTL8730E_UART2) && !defined(UART2_ASSIGNED)
#define TTYS2_DEV               g_uart2port             /* UART2 is ttyS2 */
#define UART2_ASSIGNED  1
#elif defined(CONFIG_RTL8730E_UART3) && !defined(UART3_ASSIGNED)
#define TTYS2_DEV               g_uart3port             /* UART3 is ttyS2 */
#define UART3_ASSIGNED  1
#elif defined(CONFIG_RTL8730E_UART4) && !defined(UART4_ASSIGNED)
#define TTYS2_DEV               g_uart4port             /* UART4 is ttyS2 */
#define UART4_ASSIGNED  1
#endif
#define CHAR_TIMEOUT 3127500 /*counter based on cpu clock, ~2.6ms*/
#define TX_FIFO_MAX 16

/****************************************************************************
 * Private Types
 ****************************************************************************/
/*
 * Available UARTs in AmebaSmart
 * UART0_DEV: hp uart0
 * UART1_DEV: hp uart1
 * UART2_DEV: hp uart2
 * UART3_DEV: hp uart3_bt
 * LOGUART_DEV: KM0 log uart
 */

static serial_t* sdrv[MAX_UART_INDEX + 1] = {NULL, NULL, NULL, NULL, NULL}; //uart 0~4, uart4 is configured as log uart
#ifdef CONFIG_PM
static volatile bool uart_active_state = 0;
static volatile bool log_uart_active_state = 0;
#endif

struct rtl8730e_up_dev_s {
	uint8_t parity;				/* 0=none, 1=odd, 2=even */
	uint8_t bits;				/* Number of bits (7 or 8) */
	uint8_t stopbit;			/* Number of StopBit (1 or 2) */
	uint32_t baud;				/* Configured baud rate */
	uint32_t irq;				/* IRQ associated with this UART */
	PinName tx;					/* TX UART pin number */
	PinName rx;					/* RX UART pin number */
	PinName rts;				/* UART RTS pin number */
	PinName cts;				/* UART CTS pin number */
	uint8_t FlowControl;
	bool txint_enable;
	bool rxint_enable;
#ifdef CONFIG_SERIAL_IFLOWCONTROL
	uint8_t iflow:1;			/* input flow control (RTS) enabled */
#endif
#ifdef CONFIG_SERIAL_OFLOWCONTROL
	uint8_t oflow:1;			/* output flow control (CTS) enabled */
#endif
	uint8_t tx_level;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int rtl8730e_log_uart_irq(void *Data);
static int rtl8730e_log_up_setup(struct uart_dev_s *dev);
static void rtl8730e_log_up_shutdown(struct uart_dev_s *dev);
static int rtl8730e_log_up_attach(struct uart_dev_s *dev);
static void rtl8730e_log_up_detach(struct uart_dev_s *dev);
static int rtl8730e_log_up_interrupt(int irq, void *context, FAR void *arg);
static int rtl8730e_log_up_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg);
static int rtl8730e_log_up_receive(struct uart_dev_s *dev, unsigned int *status);
static void rtl8730e_log_up_rxint(struct uart_dev_s *dev, bool enable);
static bool rtl8730e_log_up_rxavailable(struct uart_dev_s *dev);
static void rtl8730e_log_up_send(struct uart_dev_s *dev, int ch);
static void rtl8730e_log_up_txint(struct uart_dev_s *dev, bool enable);
static bool rtl8730e_log_up_txready(struct uart_dev_s *dev);
static bool rtl8730e_log_up_txempty(struct uart_dev_s *dev);

static int rtl8730e_up_setup(struct uart_dev_s *dev);
static void rtl8730e_up_shutdown(struct uart_dev_s *dev);
static int rtl8730e_up_attach(struct uart_dev_s *dev);
static void rtl8730e_up_detach(struct uart_dev_s *dev);
static int rtl8730e_up_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg);
static int rtl8730e_up_receive(struct uart_dev_s *dev, unsigned int*status);
static void rtl8730e_up_rxint(struct uart_dev_s *dev, bool enable);
static bool rtl8730e_up_rxavailable(struct uart_dev_s *dev);
static void rtl8730e_up_send(struct uart_dev_s *dev, int ch);
static void rtl8730e_up_txint(struct uart_dev_s *dev, bool enable);
static bool rtl8730e_up_txready(struct uart_dev_s *dev);
static bool rtl8730e_up_txempty(struct uart_dev_s *dev);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Serial driver UART operations */
static const struct uart_ops_s log_g_uart_ops = {
	.setup = rtl8730e_up_setup,
	.shutdown = rtl8730e_log_up_shutdown,
	.attach = rtl8730e_log_up_attach,
	.detach = rtl8730e_log_up_detach,
	.ioctl = rtl8730e_log_up_ioctl,
	.receive = rtl8730e_log_up_receive,
	.rxint = rtl8730e_log_up_rxint,
	.rxavailable = rtl8730e_log_up_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL //Flowcontrol available only for UART 0 & 1 & 2
	.rxflowcontrol = NULL,
#endif
	.send = rtl8730e_log_up_send,
	.txint = rtl8730e_log_up_txint,
	.txready = rtl8730e_log_up_txready,
	.txempty = rtl8730e_log_up_txempty,
};

static const struct uart_ops_s g_uart_ops = {
	.setup = rtl8730e_up_setup,
	.shutdown = rtl8730e_up_shutdown,
	.attach = rtl8730e_up_attach,
	.detach = rtl8730e_up_detach,
	.ioctl = rtl8730e_up_ioctl,
	.receive = rtl8730e_up_receive,
	.rxint = rtl8730e_up_rxint,
	.rxavailable = rtl8730e_up_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
	.rxflowcontrol = NULL,
#endif
	.send = rtl8730e_up_send,
	.txint = rtl8730e_up_txint,
	.txready = rtl8730e_up_txready,
	.txempty = rtl8730e_up_txempty,
};

#ifdef CONFIG_RTL8730E_UART0
static char g_uart0rxbuffer[CONFIG_UART0_RXBUFSIZE];
static char g_uart0txbuffer[CONFIG_UART0_TXBUFSIZE];
#endif
#ifdef CONFIG_RTL8730E_UART1
static char g_uart1rxbuffer[CONFIG_UART1_RXBUFSIZE];
static char g_uart1txbuffer[CONFIG_UART1_TXBUFSIZE];
#endif
#ifdef CONFIG_RTL8730E_UART2
static char g_uart2rxbuffer[CONFIG_UART2_RXBUFSIZE];
static char g_uart2txbuffer[CONFIG_UART2_TXBUFSIZE];
#endif
#ifdef CONFIG_RTL8730E_UART3
static char g_uart3rxbuffer[CONFIG_UART3_RXBUFSIZE];
static char g_uart3txbuffer[CONFIG_UART3_TXBUFSIZE];
#endif
#ifdef CONFIG_RTL8730E_UART4
static char g_uart4rxbuffer[CONFIG_UART4_RXBUFSIZE];
static char g_uart4txbuffer[CONFIG_UART4_TXBUFSIZE];
#endif

#define RTL8730E_UART0_IRQ	(82)
#define RTL8730E_UART1_IRQ	(83)
#define RTL8730E_UART2_IRQ	(84)
#define RTL8730E_UART3_BT_IRQ	(85)
#define RTL8730E_UART_LOG_IRQ	(40)

#ifdef CONFIG_RTL8730E_UART0
static struct rtl8730e_up_dev_s g_uart0priv = {

	.parity = CONFIG_UART0_PARITY,
	.bits = CONFIG_UART0_BITS,
#if (CONFIG_UART0_2STOP)
	.stopbit = 2,
#else
	.stopbit = 1,
#endif
	.baud = CONFIG_UART0_BAUD,
	.irq = RTL8730E_UART0_IRQ,
	.tx = PA_3,
	.rx = PA_2,
	.FlowControl = FlowControlNone,
	.txint_enable = false,
	.rxint_enable = false,
};

static uart_dev_t g_uart0port = {
	.isconsole = false,
	.recv = {
		.size = CONFIG_UART0_RXBUFSIZE,
		.buffer = g_uart0rxbuffer,
	},
	.xmit = {
		.size = CONFIG_UART0_TXBUFSIZE,
		.buffer = g_uart0txbuffer,
	},
	.ops = &g_uart_ops,
	.priv = &g_uart0priv,
};
#endif

#ifdef CONFIG_RTL8730E_UART1
static struct rtl8730e_up_dev_s g_uart1priv = {

	.parity = CONFIG_UART1_PARITY,
	.bits = CONFIG_UART1_BITS,
#if (CONFIG_UART1_2STOP)
	.stopbit = 2,
#else
	.stopbit = 1,
#endif
	.baud = CONFIG_UART1_BAUD,
	.irq = RTL8730E_UART1_IRQ,

#if CONFIG_RTL8730E_BOARD_REVISION >= 5
	.tx = PA_10,
	.rx = PA_9,
#else
	.tx = PA_5,
	.rx = PA_4,
#endif
	.FlowControl = FlowControlNone,
	.txint_enable = false,
	.rxint_enable = false,
};

static uart_dev_t g_uart1port = {
	.isconsole = false,
	.recv = {
		.size = CONFIG_UART1_RXBUFSIZE,
		.buffer = g_uart1rxbuffer,
	},
	.xmit = {
		.size = CONFIG_UART1_TXBUFSIZE,
		.buffer = g_uart1txbuffer,
	},
	.ops = &g_uart_ops,
	.priv = &g_uart1priv,
};
#endif

#ifdef CONFIG_RTL8730E_UART2
static struct rtl8730e_up_dev_s g_uart2priv = {

	.parity = CONFIG_UART2_PARITY,
	.bits = CONFIG_UART2_BITS,
#if (CONFIG_UART2_2STOP)
	.stopbit = 2,
#else
	.stopbit = 1,
#endif
	.baud = CONFIG_UART2_BAUD,
	.irq = RTL8730E_UART2_IRQ,
	.tx = PB_22,
	.rx = PB_21,
	.rts = PB_20,
	.cts = PB_19,
	.FlowControl = FlowControlNone,
	.txint_enable = false,
	.rxint_enable = false,
};

static uart_dev_t g_uart2port = {
	.isconsole = false,
	.recv = {
		.size = CONFIG_UART2_RXBUFSIZE,
		.buffer = g_uart2rxbuffer,
	},
	.xmit = {
		.size = CONFIG_UART2_TXBUFSIZE,
		.buffer = g_uart2txbuffer,
	},
	.ops = &g_uart_ops,
	.priv = &g_uart2priv,
};
#endif

#ifdef CONFIG_RTL8730E_UART3
static struct rtl8730e_up_dev_s g_uart3priv = {

	.parity = CONFIG_UART3_PARITY,
	.bits = CONFIG_UART3_BITS,
#if (CONFIG_UART3_2STOP)
	.stopbit = 2,
#else
	.stopbit = 1,
#endif
	.baud = CONFIG_UART3_BAUD,
	.irq = RTL8730E_UART3_BT_IRQ,
	.FlowControl = FlowControlNone,
	.txint_enable = false,
	.rxint_enable = false,
};

static uart_dev_t g_uart3port = {
	.isconsole = false,
	.recv = {
		.size = CONFIG_UART3_RXBUFSIZE,
		.buffer = g_uart3rxbuffer,
	},
	.xmit = {
		.size = CONFIG_UART3_TXBUFSIZE,
		.buffer = g_uart3txbuffer,
	},
	.ops = &g_uart_ops,
	.priv = &g_uart3priv,
};
#endif

#ifdef CONFIG_RTL8730E_UART4
static struct rtl8730e_up_dev_s g_uart4priv = {

	.parity = CONFIG_UART4_PARITY,
	.bits = CONFIG_UART4_BITS,
#if (CONFIG_UART4_2STOP)
	.stopbit = 2,
#else
	.stopbit = 1,
#endif
	.baud = CONFIG_UART4_BAUD,
	.irq = RTL8730E_UART_LOG_IRQ,
	.tx = PB_24,
	.rx = PB_23,
	.FlowControl = FlowControlNone,
	.txint_enable = false,
	.rxint_enable = false,
};

static uart_dev_t g_uart4port = {
	.isconsole = false,
	.recv = {
		.size = CONFIG_UART4_RXBUFSIZE,
		.buffer = g_uart4rxbuffer,
	},
	.xmit = {
		.size = CONFIG_UART4_TXBUFSIZE,
		.buffer = g_uart4txbuffer,
	},
	.ops = &log_g_uart_ops,
	.priv = &g_uart4priv,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static u32 uart_index_get(PinName tx)
{
	if ((tx == PA_3) || (tx == PA_14) || (tx == PA_29) || (tx == PB_6)) {
		return 0;
	} else if ((tx == PA_5) || (tx == PA_10) || (tx == PA_25) || (tx == PB_11) || (tx == PB_20) || (tx == PB_30)) {
		return 1;
	} else if ((tx == PA_8) || (tx == PA_12) || (tx == PA_23) || (tx == PB_22)) {
		return 2;
	} else if (tx == PB_24) {
		return 4;
	} else {
		assert_param(0);
	}
	return -1;
}

/****************************************************************************
 * Name: up_shutdown
 *
 * Description:
 *   Disable the UART.  This method is called when the serial
 *   port is closed
 *
 ****************************************************************************/

static void rtl8730e_log_up_shutdown(struct uart_dev_s *dev)
{
	/*Do not shut down log uart*/
}

/****************************************************************************
 * Name: up_attach
 *
 * Description:
 *   Configure the UART to operation in interrupt driven mode.  This method is
 *   called when the serial port is opened.  Normally, this is just after the
 *   the setup() method is called, however, the serial console may operate in
 *   a non-interrupt driven mode during the boot phase.
 *
 *   RX and TX interrupts are not enabled when by the attach method (unless the
 *   hardware supports multiple levels of interrupt enabling).  The RX and TX
 *   interrupts are not enabled until the txint() and rxint() methods are called.
 *
 ****************************************************************************/

static int rtl8730e_log_uart_irq(void *Data)
{

	u32 IrqEn = LOGUART_GetIMR();
	u32 reg_lsr = LOGUART_GetStatus(CONSOLE);

	if (reg_lsr & LOGUART_BIT_RXFIFO_INT) {
		uart_recvchars(&CONSOLE_DEV);
	}

	u32 txempty_en = LOGUART_GET_ETPFEI(IrqEn);
	if ((txempty_en == 0x4 && (reg_lsr & LOGUART_BIT_TP4F_EMPTY)) || (reg_lsr & LOGUART_BIT_TP4F_NOT_FULL)) {
		uart_xmitchars(&CONSOLE_DEV);
	}
	return 0;
}

static int rtl8730e_log_up_attach(struct uart_dev_s *dev)
{
	InterruptRegister((IRQ_FUN)rtl8730e_log_uart_irq, RTL8730E_UART_LOG_IRQ-32, (int)NULL, INT_PRI_LOWEST);
	InterruptEn(RTL8730E_UART_LOG_IRQ-32, INT_PRI_LOWEST);
	return 0;
}

/****************************************************************************
 * Name: up_detach
 *
 * Description:
 *   Detach UART interrupts.  This method is called when the serial port is
 *   closed normally just before the shutdown method is called.  The exception is
 *   the serial console which is never shutdown.
 *
 ****************************************************************************/

static void rtl8730e_log_up_detach(struct uart_dev_s *dev)
{
	InterruptUnRegister(RTL8730E_UART_LOG_IRQ-32);
	InterruptDis(RTL8730E_UART_LOG_IRQ-32);
}

/****************************************************************************
 * Name: up_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method
 *
 ****************************************************************************/

static int rtl8730e_log_up_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg)
{
#if defined(CONFIG_SERIAL_TERMIOS)

	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	int ret = OK;
	struct termios *termiosp = (struct termios *)arg;

	DEBUGASSERT(priv);
	switch (cmd) {
	case TCGETS:
		if (!termiosp) {
			return -EINVAL;
		}

		cfsetispeed(termiosp, priv->baud);

		termiosp->c_cflag = 0;

		if (priv->parity) {
			termiosp->c_cflag |= PARENB;
			if (priv->parity == RUART_ODD_PARITY) {
				termiosp->c_cflag |= PARODD;
			}
		}

		if (priv->stopbit == 2) {
			termiosp->c_cflag |= CSTOPB;
	}

		termiosp->c_cflag |= CS5 + ((priv->bits) - 5);
			break;

	case TCSETS:
		if (!termiosp) {
			return -EINVAL;
		}

		priv->bits = 5 + (termiosp->c_cflag & CSIZE);

		priv->parity = 0;		//ParityNoon
		if (termiosp->c_cflag & PARENB) {
			if (termiosp->c_cflag & PARODD) {
				priv->parity = ParityOdd;
			} else {
				priv->parity = ParityEven;
			}
		}
		if (!!(termiosp->c_cflag & CSTOPB)) {
			priv->stopbit = 2;
		} else {
			priv->stopbit = 1;
		}

		priv->baud = cfgetispeed(termiosp);
		if (sdrv[uart_index_get(priv->tx)]) {
			rtl8730e_log_up_shutdown(dev);
		}
		rtl8730e_up_setup(dev);
		rtl8730e_log_up_attach(dev);
		rtl8730e_log_up_txint(dev, priv->txint_enable);
		rtl8730e_log_up_rxint(dev, priv->rxint_enable);
		break;

	default:
		ret = -ENOTTY;
			break;
		}
#else
	int ret = -ENOTTY;
#endif
	return ret;
}

/****************************************************************************
 * Name: up_receive
 *
 * Description:
 *   Called (usually) from the interrupt level to receive one
 *   character from the UART.  Error bits associated with the
 *   receipt are provided in the return 'status'.
 *
 ****************************************************************************/

static int rtl8730e_log_up_receive(struct uart_dev_s *dev, unsigned int *status)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	uint32_t rxd;

	DEBUGASSERT(priv);
	rxd = up_lowgetc();
	*status = rxd;

	return rxd & 0xff;
}

/****************************************************************************
 * Name: up_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts
 *
 ****************************************************************************/
static void rtl8730e_log_up_rxint(struct uart_dev_s *dev, bool enable)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	priv->rxint_enable = enable;
	if (enable) {
		LOGUART_INTConfig(LOGUART_DEV, LOGUART_BIT_ERBI, ENABLE);
	} else {
		LOGUART_INTConfig(LOGUART_DEV, LOGUART_BIT_ERBI, DISABLE);
	}
}

/****************************************************************************
 * Name: up_rxavailable
 *
 * Description:
 *   Return true if the receive fifo is not empty
 *
 ****************************************************************************/

static bool rtl8730e_log_up_rxavailable(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	return (LOGUART_Readable());
}

/****************************************************************************
 * Name: up_send
 *
 * Description:
 *   This method will send one byte on the UART
 *
 ****************************************************************************/

static void rtl8730e_log_up_send(struct uart_dev_s *dev, int ch)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	/*write one byte to tx fifo*/
	up_lowputc(ch);
	priv->tx_level--;
}

/****************************************************************************
 * Name: up_txint
 *
 * Description:
 *   Call to enable or disable TX interrupts
 *
 ****************************************************************************/

static void rtl8730e_log_up_txint(struct uart_dev_s *dev, bool enable)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	priv->txint_enable = enable;
#ifdef CONFIG_PM
	irqstate_t flags = enter_critical_section();
	if (log_uart_active_state != enable) {	/* State has changed */
		bsp_pm_domain_control(BSP_UART_DRV, enable);
		log_uart_active_state = enable;
	}
	leave_critical_section(flags);
#endif
	if (enable) {
		LOGUART_INTConfig(LOGUART_DEV, LOGUART_TX_EMPTY_PATH_4_INTR, ENABLE);
		//LOGUART_RxCmd(LOGUART_DEV, ENABLE);
	} else {
		LOGUART_INTConfig(LOGUART_DEV, LOGUART_TX_EMPTY_PATH_4_INTR, DISABLE);
		//LOGUART_RxCmd(LOGUART_DEV, DISABLE);
	}
}

/****************************************************************************
 * Name: up_txready
 *
 * Description:
 *   Return true if the tranmsit fifo is not full
 *
 ****************************************************************************/

static bool rtl8730e_log_up_txready(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);

	 return (LOGUART_Ready());

}


/****************************************************************************
 * Name: up_txempty
 *
 * Description:
 *   Return true if the transmit fifo is empty
 *
 ****************************************************************************/

static bool rtl8730e_log_up_txempty(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);

	u32 reg_lsr = LOGUART_GetStatus(CONSOLE);
	return (reg_lsr & LOGUART_BIT_TP4F_EMPTY);
}


/****************************************************************************
 * Name: rtl8730e_up_setup
 *
 * Description:
 *   Configure the UART baud, bits, parity, fifos, etc. This
 *   method is called the first time that the serial port is
 *   opened.
 *
 ****************************************************************************/
static int rtl8730e_up_setup(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	DEBUGASSERT(!sdrv[uart_index_get(priv->tx)]);
	sdrv[uart_index_get(priv->tx)] = (serial_t *)kmm_malloc(sizeof(serial_t));
	DEBUGASSERT(sdrv[uart_index_get(priv->tx)]);
	if (uart_index_get(priv->tx) == 4)	{//Loguart cannot be stopped
		irq_disable(RTL8730E_UART_LOG_IRQ-32);
		irq_unregister(RTL8730E_UART_LOG_IRQ-32);
		InterruptRegister((IRQ_FUN)rtl8730e_log_uart_irq, RTL8730E_UART_LOG_IRQ-32, (int)NULL, INT_PRI_LOWEST);
		InterruptEn(RTL8730E_UART_LOG_IRQ-32, INT_PRI_LOWEST);
	} else {
		sdrv[uart_index_get(priv->tx)]->uart_idx = uart_index_get(priv->tx);
		serial_init((serial_t *) sdrv[uart_index_get(priv->tx)], priv->tx, priv->rx);
		serial_baud(sdrv[uart_index_get(priv->tx)], priv->baud);
		serial_format(sdrv[uart_index_get(priv->tx)], priv->bits, priv->parity, priv->stopbit);

#if defined(CONFIG_SERIAL_IFLOWCONTROL) || defined(CONFIG_SERIAL_OFLOWCONTROL)
		if ((uart_index_get(priv->tx) == 0) || (uart_index_get(priv->tx) == 1) || (uart_index_get(priv->tx) == 2)) {
	serial_set_flow_control(sdrv[uart_index_get(priv->tx)], priv->FlowControl, priv->rts, priv->cts);
		} else {
			printf("\nSetting UART%d flow control failed.\n", uart_index_get(priv->tx));
			printf("\nNOTE: Only UART0,1,2 are able to use flowcontrol!\n");
		}
#endif
	}
	return OK;
}

/****************************************************************************
 * Name: rtl8730e_up_setup_pin
 *
 * Description:
 *   Operation is only setting serial pin.
 *
 ****************************************************************************/
static int rtl8730e_up_setup_pin(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);

	serial_pin_init(priv->tx, priv->rx);
	return OK;
}

/****************************************************************************
 * Name: up_shutdown
 *
 * Description:
 *   Disable the UART.  This method is called when the serial
 *   port is closed
 *
 ****************************************************************************/

static void rtl8730e_up_shutdown(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	DEBUGASSERT(sdrv[uart_index_get(priv->tx)]);
	serial_free(sdrv[uart_index_get(priv->tx)]);
	rtos_mem_free(sdrv[uart_index_get(priv->tx)]);
	sdrv[uart_index_get(priv->tx)] = NULL;
}

/****************************************************************************
 * Name: up_attach
 *
 * Description:
 *   Configure the UART to operation in interrupt driven mode.  This method is
 *   called when the serial port is opened.  Normally, this is just after the
 *   the setup() method is called, however, the serial console may operate in
 *   a non-interrupt driven mode during the boot phase.
 *
 *   RX and TX interrupts are not enabled when by the attach method (unless the
 *   hardware supports multiple levels of interrupt enabling).  The RX and TX
 *   interrupts are not enabled until the txint() and rxint() methods are called.
 *
 ****************************************************************************/

void rtl8730e_uart_irq(uint32_t id, SerialIrq event)
{
	struct uart_dev_s *dev = (struct uart_dev_s *)id;
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;

	if (event == RxIrq) {
		uart_recvchars(dev);
	}
	if (event == TxIrq) {
		priv->tx_level = TX_FIFO_MAX;
		uart_xmitchars(dev);
		priv->tx_level = 0;
	}
}
static int rtl8730e_up_attach(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	int ret = 0;
	DEBUGASSERT(priv);
	serial_irq_handler(sdrv[uart_index_get(priv->tx)], rtl8730e_uart_irq, (uint32_t) dev);
	return ret;
}

/****************************************************************************
 * Name: up_detach
 *
 * Description:
 *   Detach UART interrupts.  This method is called when the serial port is
 *   closed normally just before the shutdown method is called.  The exception is
 *   the serial console which is never shutdown.
 *
 ****************************************************************************/

static void rtl8730e_up_detach(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	serial_irq_handler(sdrv[uart_index_get(priv->tx)], NULL, 0);
}

/****************************************************************************
 * Name: up_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method
 *
 ****************************************************************************/

static int rtl8730e_up_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg)
{
#if defined(CONFIG_SERIAL_TERMIOS)
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	int ret = OK;
	struct termios *termiosp = (struct termios *)arg;

	DEBUGASSERT(priv);
	switch (cmd) {
	case TCGETS:
		if (!termiosp) {
			return -EINVAL;
		}

		cfsetispeed(termiosp, priv->baud);

		termiosp->c_cflag = 0;

		if (priv->parity) {
			termiosp->c_cflag |= PARENB;
			if (priv->parity == RUART_ODD_PARITY) {
				termiosp->c_cflag |= PARODD;
			}
		}

		if (priv->stopbit == 2) {
			termiosp->c_cflag |= CSTOPB;
		}

		termiosp->c_cflag |= CS5 + ((priv->bits) - 5);
		break;

	case TCSETS:
		if (!termiosp) {
			return -EINVAL;
		}

		priv->bits = 5 + (termiosp->c_cflag & CSIZE);

		priv->parity = 0;		//ParityNoon
		if (termiosp->c_cflag & PARENB) {
			if (termiosp->c_cflag & PARODD) {
				priv->parity = ParityOdd;
			} else {
				priv->parity = ParityEven;
			}
		}
		if (!!(termiosp->c_cflag & CSTOPB)) {
			priv->stopbit = 2;
		} else {
			priv->stopbit = 1;
		}

		priv->baud = cfgetispeed(termiosp);
		if (sdrv[uart_index_get(priv->tx)]) {
			rtl8730e_up_shutdown(dev);
		}
		rtl8730e_up_setup(dev);
		rtl8730e_up_attach(dev);
		rtl8730e_up_txint(dev, priv->txint_enable);
		rtl8730e_up_rxint(dev, priv->rxint_enable);
		break;

	case TIOCLOOPBACK:
		if (!arg) {
			return -EINVAL;
		}
		serial_control_loopback(sdrv[uart_index_get(priv->tx)], *(bool *)arg);
		break;
	default:
		ret = -ENOTTY;
		break;
	}
#else
	int ret = -ENOTTY;
#endif
	return ret;
}

/****************************************************************************
 * Name: up_receive
 *
 * Description:
 *   Called (usually) from the interrupt level to receive one
 *   character from the UART.  Error bits associated with the
 *   receipt are provided in the return 'status'.
 *
 ****************************************************************************/

static int rtl8730e_up_receive(struct uart_dev_s *dev, unsigned int*status)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	uint32_t rxd;

	DEBUGASSERT(priv);
	rxd = serial_getc(sdrv[uart_index_get(priv->tx)]);
	*status = rxd;

	return rxd & 0xff;
}

/****************************************************************************
 * Name: up_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts
 *
 ****************************************************************************/
static void rtl8730e_up_rxint(struct uart_dev_s *dev, bool enable)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	priv->rxint_enable = enable;
	serial_irq_set(sdrv[uart_index_get(priv->tx)], RxIrq, enable);	// 1= ENABLE
}

/****************************************************************************
 * Name: up_rxavailable
 *
 * Description:
 *   Return true if the receive fifo is not empty
 *
 ****************************************************************************/

static bool rtl8730e_up_rxavailable(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	return (serial_readable(sdrv[uart_index_get(priv->tx)]));
}

/****************************************************************************
 * Name: up_send
 *
 * Description:
 *   This method will send one byte on the UART
 *
 ****************************************************************************/

static void rtl8730e_up_send(struct uart_dev_s *dev, int ch)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	/*write one byte to tx fifo*/
	serial_putc(sdrv[uart_index_get(priv->tx)], ch);
	priv->tx_level--;
}

/****************************************************************************
 * Name: up_txint
 *
 * Description:
 *   Call to enable or disable TX interrupts
 *
 ****************************************************************************/

static void rtl8730e_up_txint(struct uart_dev_s *dev, bool enable)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	priv->txint_enable = enable;
#ifdef CONFIG_PM
	irqstate_t flags = enter_critical_section();
	if (uart_active_state != enable) {	/* State has changed */
		bsp_pm_domain_control(BSP_UART_DRV, enable);
		uart_active_state = enable;
	}
	leave_critical_section(flags);
#endif
	serial_irq_set(sdrv[uart_index_get(priv->tx)], TxIrq, enable);
	if (enable)
		UART_INTConfig(UART_DEV_TABLE[uart_index_get(priv->tx)].UARTx, RUART_BIT_ETBEI, ENABLE);
}

/****************************************************************************
 * Name: up_txready
 *
 * Description:
 *   Return true if the tranmsit fifo is not full
 *
 ****************************************************************************/

static bool rtl8730e_up_txready(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);

	return priv->tx_level;
}

/****************************************************************************
 * Name: up_txempty
 *
 * Description:
 *   Return true if the transmit fifo is empty
 *
 ****************************************************************************/

static bool rtl8730e_up_txempty(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);

	return (serial_tx_empty(sdrv[uart_index_get(priv->tx)]));
}

/****************************************************************************
 * Name: rtk_loguart/uart_suspend/resume
 *
 * Description:
 *   Suspend or resume serial peripherals for/from sleep modes.
 *
 ****************************************************************************/

#ifdef CONFIG_PM
static uint32_t rtk_loguart_suspend(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;

	LOGUART_Suspend();
	/* Pre process is done in LP core */
	/* Reference code:
	* 	RCC_PeriphClockSource_LOGUART(UARTLOG_CLK_OSC_LP);
	*	LOGUART_LPBaudSet(LOGUART_DEV, 115200, 2000000);
	*/
	return 1;
}

static uint32_t rtk_loguart_resume(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;

	/* Post process is done in LP core */
	/* Reference code:
	*	RCC_PeriphClockSource_LOGUART(UARTLOG_CLK_XTAL_40M);
	*	LOGUART_SetBaud(LOGUART_DEV, 115200);
	*/
	return 1;
}

static uint32_t rtk_uart_suspend(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;
#ifdef CONFIG_RTL8730E_UART1
	if (sdrv[uart_index_get(g_uart1priv.tx)] != NULL) {
		serial_change_clcksrc(sdrv[uart_index_get(g_uart1priv.tx)], g_uart1priv.baud, 0);
	}
#endif
	return 1;
}

static uint32_t rtk_uart_resume(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;
#ifdef CONFIG_RTL8730E_UART1
	if (sdrv[uart_index_get(g_uart1priv.tx)] != NULL) {
		serial_change_clcksrc(sdrv[uart_index_get(g_uart1priv.tx)], g_uart1priv.baud, 1);
	}
#endif
	return 1;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef USE_SERIALDRIVER
/****************************************************************************
 * Name: up_serialinit
 *
 * Description:
 *   Register serial console and serial ports.  This assumes
 *   that up_earlyserialinit was called previously.
 *
 ****************************************************************************/

void up_serialinit(void)
{
#ifdef CONSOLE_DEV
	CONSOLE_DEV.isconsole = true;
	rtl8730e_up_setup(&CONSOLE_DEV);

	/* Register the console */
	uart_register("/dev/console", &CONSOLE_DEV);
#endif

	/* Register all UARTs */
#ifdef TTYS0_DEV
/* Default LOGUART(UART4) is already running and could not be reinit or stopped */
#if !defined(CONFIG_UART4_SERIAL_CONSOLE)
	rtl8730e_up_setup_pin(&TTYS0_DEV);
#endif
	uart_register("/dev/ttyS0", &TTYS0_DEV);
#endif
#ifdef TTYS1_DEV
	rtl8730e_up_setup_pin(&TTYS1_DEV);
	uart_register("/dev/ttyS1", &TTYS1_DEV);
#endif
#ifdef TTYS2_DEV
	rtl8730e_up_setup_pin(&TTYS2_DEV);
	uart_register("/dev/ttyS2", &TTYS2_DEV);
#endif

#ifdef CONFIG_PM
	/* Domain registration for operation control */
	bsp_pm_domain_register("UART", BSP_UART_DRV);
	pmu_register_sleep_callback(PMU_LOGUART_DEVICE, (PSM_HOOK_FUN)rtk_loguart_suspend, NULL, (PSM_HOOK_FUN)rtk_loguart_resume, NULL);
	pmu_register_sleep_callback(PMU_UART1_DEVICE, (PSM_HOOK_FUN)rtk_uart_suspend, NULL, (PSM_HOOK_FUN)rtk_uart_resume, NULL);
#endif
}

/****************************************************************************
 * Name: up_lowputc
 *
 * Description:
 *   Output one byte on the serial console
 *
 * Input Parameters:
 *   ch - chatacter to output
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void up_lowputc(char ch)
{
#ifdef CONFIG_UART4_SERIAL_CONSOLE
	u32 CounterIndex = 0;
	/*check if there is space in fifo*/
	while(!LOGUART_Ready()) {
		CounterIndex++;
		if (CounterIndex >= CHAR_TIMEOUT) {
			break;
		}
	}
	LOGUART_PutChar(ch);
#else
	if (CONSOLE_DEV.isconsole == false)
		return;
	rtl8730e_up_send(&CONSOLE_DEV, ch);
#endif
}

/****************************************************************************
 * Name: up_lowgetc
 *
 * Description:
 *   Read one byte from the serial console
 *
 * Input Parameters:
 *   none
 *
 * Returned Value:
 *   int value, -1 if error, 0~255 if byte successfully read
 *
 ****************************************************************************/
int up_lowgetc(void)
{
	uint8_t rxd;
#ifdef CONFIG_UART4_SERIAL_CONSOLE
	u32 IrqEn = LOGUART_GetIMR();
	LOGUART_SetIMR(0);
	rxd = LOGUART_GetChar(_FALSE);
	LOGUART_SetIMR(IrqEn);
#else
	if (CONSOLE_DEV.isconsole == false)
		return;
	rxd = rtl8730e_up_receive(&CONSOLE_DEV, &rxd);
#endif
	return rxd & 0xff;
}

/****************************************************************************
 * Name: up_putc
 *
 * Description:
 *   Output one byte on the serial console
 *
 * Input Parameters:
 *   ch - chatacter to output
 *
 * Returned Value:
 *  sent character
 *
 ****************************************************************************/
int up_putc(int ch)
{
	/* Check for LF */

	if (ch == '\n') {
		/* Add CR */

		up_lowputc('\r');
	}

	up_lowputc(ch);
	return ch;
}

/****************************************************************************
 * Name: up_getc
 *
 * Description:
 *   Read one byte from the serial console
 *
 * Input Parameters:
 *   none
 *
 * Returned Value:
 *   int value, -1 if error, 0~255 if byte successfully read
 *
 ****************************************************************************/
int up_getc(void)
{
	int ch;
	ch = up_lowgetc();
	return ch;
}

#else							/* USE_SERIALDRIVER */
/****************************************************************************
 * Name: up_putc
 *
 * Description:
 *   Output one byte on the serial console
 *
 * Input Parameters:
 *   ch - chatacter to output
 *
 * Returned Value:
 *  sent character
 *
 ****************************************************************************/
int up_putc(int ch)
{
	/* Check for LF */

	if (ch == '\n') {
		/* Add CR */

		up_lowputc('\r');
	}
	up_lowputc(ch);

	return ch;
}

/****************************************************************************
 * Name: up_getc
 *
 * Description:
 *   Read one byte from the serial console
 *
 * Input Parameters:
 *   none
 *
 * Returned Value:
 *   int value, -1 if error, 0~255 if byte successfully read
 *
 ****************************************************************************/
int up_getc(void)
{
	int ch;
	ch = up_lowgetc();
	return ch;
}
#endif							/* USE_SERIALDRIVER */


