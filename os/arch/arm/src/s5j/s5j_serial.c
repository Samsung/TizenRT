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
 * arch/arm/src/s5j/s5j_serial.c
 *
 *   Copyright (C) 2009-2010, 2012-2014 Gregory Nutt. All rights reserved.
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
#include <tinyara/pm/pm.h>

#include <arch/serial.h>
#include <arch/board/board.h>

#include "chip.h"
#include "up_arch.h"
#include "up_internal.h"

#ifdef CONFIG_SERIAL_TERMIOS
#include <termios.h>
#endif

#include <arch/chip/chip_types.h>
#include <chip.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* If we are not using the serial driver for the console, then we
 * still must provide some minimal implementation of up_putc.
 */

#ifndef CONFIG_NO_SERIAL_CONSOLE
#define HAVE_SERIAL_CONSOLE
#endif

/* Which UART with be tty0/console and which tty1-3?  The console will always
 * be ttyS0.  If there is no console then will use the lowest numbered UART.
 */

/* First pick the console and ttys0.  This could be any of UART0-3,dev */

#if defined(CONFIG_S5J_HAVE_UARTDBG)
#define TTYDBG_DEV          g_uart_port[UARTDBG]
#endif
#if defined(CONFIG_S5J_HAVE_UART0)
#define TTYS0_DEV           g_uart_port[UART0]
#endif
#if defined(CONFIG_S5J_HAVE_UART1)
#define TTYS1_DEV           g_uart_port[UART1]
#endif
#if defined(CONFIG_S5J_HAVE_UART2)
#define TTYS2_DEV           g_uart_port[UART2]
#endif
#if defined(CONFIG_S5J_HAVE_UART3)
#define TTYS3_DEV           g_uart_port[UART3]
#endif

#ifdef HAVE_SERIAL_CONSOLE
#if defined(CONFIG_UART0_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart_port[UART0]
#define CONSOLE_PORT UART0
#elif defined(CONFIG_UART1_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart_port[UART1]
#define CONSOLE_PORT UART1
#elif defined(CONFIG_UART2_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart_port[UART2]
#define CONSOLE_PORT UART2
#elif defined(CONFIG_UART3_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart_port[UART3]
#define CONSOLE_PORT UART3
#elif defined(CONFIG_OTHER_SERIAL_CONSOLE)
#define CONSOLE_DEV         g_uart_port[UARTDBG]
#define CONSOLE_PORT UARTDBG
#else
#define CONSOLE_DEV         g_uart_port[UARTDBG]
#define CONSOLE_PORT UARTDBG
#endif
#endif

#ifndef CONSOLE_PORT
#define CONSOLE_PORT UARTDBG
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct up_dev_s {
	uint32_t uartbase;			/* Base address of UART registers */
	uint32_t baud;				/* Configured baud */
	UART_INTERRUPT im;			/* Saved IM value */
	uint16_t irq;				/* IRQ associated with this UART */
	uint8_t parity;				/* 0=none, 4=odd, 5=even */
	uint8_t bits;				/* Number of bits (5, 6, 7 or 8) */
	bool stopbits2;				/* true: Configure with 2 stop bits instead of 1 */
	UART_CHANNEL eCh;			/*  Number of Uart Channel  */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int up_setup(struct uart_dev_s *dev);
static void up_shutdown(struct uart_dev_s *dev);
static int up_attach(struct uart_dev_s *dev);
static void up_detach(struct uart_dev_s *dev);
static int up_interrupt(int irq, void *context, void *arg);
static int up_ioctl(struct file *filep, int cmd, unsigned long arg);
static int up_receive(struct uart_dev_s *dev, uint32_t *status);
static void up_rxint(struct uart_dev_s *dev, bool enable);
static bool up_rxavailable(struct uart_dev_s *dev);
static void up_send(struct uart_dev_s *dev, int ch);
static void up_txint(struct uart_dev_s *dev, bool enable);
static bool up_txready(struct uart_dev_s *dev);
static bool up_txempty(struct uart_dev_s *dev);
static void uart_init(UART_CHANNEL uart);

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/* UART operations structure */
static const struct uart_ops_s g_uart_ops = {
	.setup = up_setup,
	.shutdown = up_shutdown,
	.attach = up_attach,
	.detach = up_detach,
	.ioctl = up_ioctl,
	.receive = up_receive,
	.rxint = up_rxint,
	.rxavailable = up_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
	.rxflowcontrol = NULL,
#endif
	.send = up_send,
	.txint = up_txint,
	.txready = up_txready,
	.txempty = up_txempty,
};

/* I/O buffers */
static char g_uartdbgrxbuffer[CONFIG_UARTDBG_RXBUFSIZE];
static char g_uartdbgtxbuffer[CONFIG_UARTDBG_TXBUFSIZE];

#ifdef CONFIG_S5J_UART0
static char g_uart0rxbuffer[CONFIG_UART0_RXBUFSIZE];
static char g_uart0txbuffer[CONFIG_UART0_TXBUFSIZE];
#endif

#ifdef CONFIG_S5J_UART1
static char g_uart1rxbuffer[CONFIG_UART1_RXBUFSIZE];
static char g_uart1txbuffer[CONFIG_UART1_TXBUFSIZE];
#endif

#ifdef CONFIG_S5J_UART2
static char g_uart2rxbuffer[CONFIG_UART2_RXBUFSIZE];
static char g_uart2txbuffer[CONFIG_UART2_TXBUFSIZE];
#endif

#ifdef CONFIG_S5J_UART3
static char g_uart3rxbuffer[CONFIG_UART3_RXBUFSIZE];
static char g_uart3txbuffer[CONFIG_UART3_TXBUFSIZE];
#endif

/* This array contains private resource structures of each port */
static struct up_dev_s g_uart_priv[] = {
#ifdef CONFIG_S5J_UARTDBG
	[UARTDBG] = {
		.uartbase = UARTDBG_BASE,
		.baud = CONFIG_UARTDBG_BAUD,
		.irq = S5J_IRQ_UARTDBG,
		.parity = CONFIG_UARTDBG_PARITY,
		.bits = CONFIG_UARTDBG_BITS,
		.stopbits2 = CONFIG_UARTDBG_2STOP,
		.eCh = UARTDBG,
	},
#else
	[UARTDBG] = {
		.uartbase = 0,
		.baud = 0,
		.irq = 0,
		.parity = 0,
		.bits = 0,
		.stopbits2 = 0,
		.eCh = -1,
	},
#endif

#ifdef CONFIG_S5J_UART0
	[UART0] = {
		.uartbase = UART0_BASE,
		.baud = CONFIG_UART0_BAUD,
		.irq = S5J_IRQ_UART0,
		.parity = CONFIG_UART0_PARITY,
		.bits = CONFIG_UART0_BITS,
		.stopbits2 = CONFIG_UART0_2STOP,
		.eCh = UART0,
	},
#endif

#ifdef CONFIG_S5J_UART1
	[UART1] = {
		.uartbase = UART1_BASE,
		.baud = CONFIG_UART0_BAUD,
		.irq = S5J_IRQ_UART1,
		.parity = CONFIG_UART0_PARITY,
		.bits = CONFIG_UART0_BITS,
		.stopbits2 = CONFIG_UART0_2STOP,
		.eCh = UART1,
	},
#endif

#ifdef CONFIG_S5J_UART2
	[UART2] = {
		.uartbase = UART2_BASE,
		.baud = CONFIG_UART0_BAUD,
		.irq = S5J_IRQ_UART2,
		.parity = CONFIG_UART0_PARITY,
		.bits = CONFIG_UART0_BITS,
		.stopbits2 = CONFIG_UART0_2STOP,
		.eCh = UART2,
	},
#endif

#ifdef CONFIG_S5J_UART3
	[UART3] = {
		.uartbase = UART3_BASE,
		.baud = CONFIG_UART0_BAUD,
		.irq = S5J_IRQ_UART3,
		.parity = CONFIG_UART0_PARITY,
		.bits = CONFIG_UART0_BITS,
		.stopbits2 = CONFIG_UART0_2STOP,
		.eCh = UART3,
	},
#endif

};

/* This array contains UART ports dev resources description */
static uart_dev_t g_uart_port[] = {
#ifdef CONFIG_S5J_UARTDBG
	[UARTDBG] = {
		.recv = {
			.size = CONFIG_UARTDBG_RXBUFSIZE,
			.buffer = g_uartdbgrxbuffer,
		},
		.xmit = {
			.size = CONFIG_UARTDBG_TXBUFSIZE,
			.buffer = g_uartdbgtxbuffer,
		},
		.ops = &g_uart_ops,
		.priv = &g_uart_priv[UARTDBG],
	},
#else
	[UARTDBG] = {
		.recv = {
			.size = 0,
			.buffer = NULL,
		},
		.xmit = {
			.size = 0,
			.buffer = NULL,
		},
		.ops = NULL,
		.priv = NULL,
	},
#endif
#ifdef CONFIG_S5J_UART0
	[UART0] = {
		.recv = {
			.size = CONFIG_UART0_RXBUFSIZE,
			.buffer = g_uart0rxbuffer,
		},
		.xmit = {
			.size = CONFIG_UART0_TXBUFSIZE,
			.buffer = g_uart0txbuffer,
		},
		.ops = &g_uart_ops,
		.priv = &g_uart_priv[UART0],
	},
#endif
#ifdef CONFIG_S5J_UART1
	[UART1] = {
		.recv = {
			.size = CONFIG_UART0_RXBUFSIZE,
			.buffer = g_uart1rxbuffer,
		},
		.xmit = {
			.size = CONFIG_UART0_TXBUFSIZE,
			.buffer = g_uart1txbuffer,
		},
		.ops = &g_uart_ops,
		.priv = &g_uart_priv[UART1],
	},
#endif

#ifdef CONFIG_S5J_UART2
	[UART2] = {
		.recv = {
			.size = CONFIG_UART0_RXBUFSIZE,
			.buffer = g_uart2rxbuffer,
		},
		.xmit = {
			.size = CONFIG_UART0_TXBUFSIZE,
			.buffer = g_uart2txbuffer,
		},
		.ops = &g_uart_ops,
		.priv = &g_uart_priv[UART2],
	},
#endif

#ifdef CONFIG_S5J_UART3
	[UART3] = {
		.recv = {
			.size = CONFIG_UART0_RXBUFSIZE,
			.buffer = g_uart3rxbuffer,
		},
		.xmit = {
			.size = CONFIG_UART0_TXBUFSIZE,
			.buffer = g_uart3txbuffer,
		},
		.ops = &g_uart_ops,
		.priv = &g_uart_priv[UART3],
	},
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_uart_set_gpio
 *
 * Description:
 *   Configure GPIO ports to operate with UART
 *
 * input parameters:
 *   eCh - UART channel
 *
 * Returned value:
 *   None
 ****************************************************************************/
void up_uart_set_gpio(UART_CHANNEL eCh)
{
	s32 gpio_rxd, gpio_txd;

	if (eCh == UARTDBG) {
		gpio_rxd = s5j_gpio(GPA3, 0);
		gpio_txd = s5j_gpio(GPA3, 1);

		gpio_cfg_pin(gpio_rxd, GPIO_FUNC(2));
		gpio_set_pull(gpio_rxd, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_txd, GPIO_FUNC(2));
		gpio_set_pull(gpio_txd, GPIO_PULL_NONE);

	} else if (eCh == UART0) {
		gpio_rxd = s5j_gpio(GPA2, 0);
		gpio_txd = s5j_gpio(GPA2, 1);

		gpio_cfg_pin(gpio_rxd, GPIO_FUNC(2));
		gpio_set_pull(gpio_rxd, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_txd, GPIO_FUNC(2));
		gpio_set_pull(gpio_txd, GPIO_PULL_NONE);
	} else if (eCh == UART1) {
		gpio_rxd = s5j_gpio(GPP0, 4);
		gpio_txd = s5j_gpio(GPP0, 5);

		gpio_cfg_pin(gpio_rxd, GPIO_FUNC(2));
		gpio_set_pull(gpio_rxd, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_txd, GPIO_FUNC(2));
		gpio_set_pull(gpio_txd, GPIO_PULL_NONE);
	} else if (eCh == UART2) {
		gpio_rxd = s5j_gpio(GPP0, 6);
		gpio_txd = s5j_gpio(GPP0, 7);

		gpio_cfg_pin(gpio_rxd, GPIO_FUNC(2));
		gpio_set_pull(gpio_rxd, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_txd, GPIO_FUNC(2));
		gpio_set_pull(gpio_txd, GPIO_PULL_NONE);
	} else if (eCh == UART3) {
		gpio_rxd = s5j_gpio(GPP1, 6);
		gpio_txd = s5j_gpio(GPP1, 7);

		gpio_cfg_pin(gpio_rxd, GPIO_FUNC(2));
		gpio_set_pull(gpio_rxd, GPIO_PULL_NONE);
		gpio_cfg_pin(gpio_txd, GPIO_FUNC(2));
		gpio_set_pull(gpio_txd, GPIO_PULL_NONE);
	} else {
		return;
	}
}

/****************************************************************************
 * Name: up_uart_enable_interrupt
 *
 * Description:
 *   Enable interrupts defined by Interrupt mask.
 *
 * input parameters:
 *   uBase - base address of uart channel
 *   eInt - Interrupt mask
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_enable_interrupt(uint32_t uBase, UART_INTERRUPT eInt)
{
	if (eInt & ERROR_INT) {
		HW_REG32(uBase, UART_CON) |= (0x1 << 6);
	}

	if (eInt & MODEM_INT) {
		HW_REG32(uBase, UART_MCON) |= (0x1 << 3);
	}

	HW_REG32(uBase, UART_INTM) &= ~(eInt);
}

/****************************************************************************
 * Name: up_uart_disable_interrupt
 *
 * Description:
 *   Disable interrupts defined by Interrupt mask.
 *
 * input parameters:
 *   uBase - base address of uart channel
 *   eInt - Interrupt mask
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_disable_interrupt(uint32_t uBase, UART_INTERRUPT eInt)
{
	HW_REG32(uBase, UART_INTM) |= eInt;

	if (eInt & ERROR_INT) {
		HW_REG32(uBase, UART_CON) &= ~(0x1 << 6);
	}

	if (eInt & MODEM_INT) {
		HW_REG32(uBase, UART_MCON) &= ~(0x1 << 3);
	}
}

/****************************************************************************
 * Name: up_uart_set_tx_mode
 *
 * Description:
 *   Set UART TX operation mode POLL_MODE, INT_MODE, or DMA_MODE.
 *
 * input parameters:
 *   uBase - base address of uart channel
 *   eMode - Operation mode.
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_tx_mode(uint32_t uBase, UART_MODE eMode)
{
	HW_REG32(uBase, UART_CON) &= ~(0x3 << 2);
	HW_REG32(uBase, UART_CON) &= ~(0x1 << 9);
	up_uart_disable_interrupt(uBase, TX_INT);	/*  Disable TX Interrupt */

	switch (eMode) {
	case POLL_MODE:
		HW_REG32(uBase, UART_CON) |= (0x1 << 2);
		break;

	case INT_MODE:
		HW_REG32(uBase, UART_CON) |= (0x1 << 2);
		HW_REG32(uBase, UART_CON) |= (0x1 << 9);
		up_uart_enable_interrupt(uBase, TX_INT);	/*  Enable TX Interrupt */
		break;

	case DMA_MODE:
		HW_REG32(uBase, UART_CON) |= (0x2 << 2);
		break;

	case DISABLE_MODE:
	default:
		break;
	}
}

/****************************************************************************
 * Name: up_uart_set_rx_mode
 *
 * Description:
 *   Set UART RX operation mode POLL_MODE, INT_MODE, or DMA_MODE.
 *
 * input parameters:
 *   uBase - base address of uart channel
 *   eMode - Operation mode.
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_rx_mode(uint32_t uBase, UART_MODE eMode)
{
	HW_REG32(uBase, UART_CON) &= ~(0x3 << 0);
	HW_REG32(uBase, UART_CON) &= ~(0x1 << 8);
	up_uart_disable_interrupt(uBase, (UART_INTERRUPT)(RX_INT | ERROR_INT));

	switch (eMode) {
	case POLL_MODE:
		HW_REG32(uBase, UART_CON) |= (0x1 << 0);
		up_uart_enable_interrupt(uBase, ERROR_INT);
		break;

	case INT_MODE:
		HW_REG32(uBase, UART_CON) |= (0x1 << 0);
		HW_REG32(uBase, UART_CON) |= (0x1 << 8);
		up_uart_enable_interrupt(uBase, (UART_INTERRUPT)(RX_INT | ERROR_INT));
		break;

	case DMA_MODE:
		HW_REG32(uBase, UART_CON) |= (0x2 << 0);
		HW_REG32(uBase, UART_CON) |= (0x1 << 8);
		up_uart_enable_interrupt(uBase, (UART_INTERRUPT)(RX_INT | ERROR_INT));
		break;

	case DISABLE_MODE:
	default:
		break;
	}
}

/****************************************************************************
 * Name: up_uart_set_baudrate
 *
 * Description:
 *   Set Baud Rate
 *
 * input parameters:
 *   uBase - base addres of uart channel
 *   bBaudrate - BaudRate bps
 *   nClock - Frequency, Hz
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_baudrate(uint32_t uBase, UART_BAUDRATE nBaudrate, u32 nClock)
{
	float fDiv;
	float fFrac;

	fDiv = ((float)nClock / (float)(nBaudrate * 16)) - 1.0;
	fFrac = (u32)(((fDiv - (s32) fDiv) * 16));

	HW_REG32(uBase, UART_BRDIV) = (u32) fDiv;
	HW_REG32(uBase, UART_FRACVAL) = (u32) fFrac;
}

/****************************************************************************
 * Name: up_uart_set_infrared_mode
 *
 * Description:
 *   Set Infrared operation mode
 *
 * input parameters:
 *   uBase - base addres of uart channel
 *   bEnable - TRUE enable, FALSE disable
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_infrared_mode(uint32_t uBase, bool bEnable)
{
	if (bEnable) {
		HW_REG32(uBase, UART_LCON) |= (0x1 << 6);
	} else {
		HW_REG32(uBase, UART_LCON) &= ~(0x1 << 6);
	}
}

/****************************************************************************
 * Name: up_uart_set_parity_mode
 *
 * Description:
 *   Set parity mode
 *
 * Input parameters:
 *   uBase - base addres of uart channel
 *   eParityMode - Parity Mode
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_parity_mode(uint32_t uBase, UART_PARITY_MODE eParityMode)
{
	HW_REG32(uBase, UART_LCON) &= ~(0x7 << 3);
	HW_REG32(uBase, UART_LCON) |= (eParityMode << 3);
}

/****************************************************************************
 * Name: up_uart_set_stop_bit
 *
 * Description:
 *   Det number of stop bits
 *
 * Input parameters:
 *   uBase - base addres of uart channel
 *   eStopbit - number of stop bits
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_stop_bit(uint32_t uBase, UART_STOP_BIT eStopBit)
{
	HW_REG32(uBase, UART_LCON) &= ~(0x1 << 2);
	HW_REG32(uBase, UART_LCON) |= (eStopBit << 2);
}

/****************************************************************************
 * Name: up_uart_set_word_length
 *
 * Description:
 *   Set transferred word length
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   eWordLen - word length
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_word_length(uint32_t uBase, UART_WORD_LENGTH eWordLen)
{
	HW_REG32(uBase, UART_LCON) &= ~(0x3 << 0);
	HW_REG32(uBase, UART_LCON) |= (eWordLen << 0);
}

/****************************************************************************
 * Name: up_uart_set_loopback
 *
 * Description:
 *   Set loopback mode
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   bEnable - TRUE enable, FALSE disable
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_loopback(uint32_t uBase, bool bEnable)
{
	if (bEnable) {
		HW_REG32(uBase, UART_CON) |= (0x1 << 5);
	} else {
		HW_REG32(uBase, UART_CON) &= ~(0x1 << 5);
	}
}

/****************************************************************************
 * Name: up_uart_set_rx_timeout_interval
 *
 * Description:
 *   Set RX timeout value when FIFO is empty
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   nTime - set timeout, number of frames = 8 * (nTime + 1)
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_rx_timeout_interval(uint32_t uBase, u32 nTime)
{
	HW_REG32(uBase, UART_CON) &= ~(0xF << 12);
	HW_REG32(uBase, UART_CON) |= ((nTime & 0xF) << 12);
}

/****************************************************************************
 * Name: up_uart_set_rx_timeout
 *
 * Description:
 *   Enable RX timeout feature when FIFO is empty
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   bEnable - TRUE enable, FALSE disable
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_rx_timeout(uint32_t uBase, bool bEnable)
{
	if (bEnable) {
		HW_REG32(uBase, UART_CON) |= (0x1 << 7);
	} else {
		HW_REG32(uBase, UART_CON) &= ~(0x1 << 7);
	}
}

/****************************************************************************
 * Name: up_uart_set_rx_timeout_with_empty_rx_fifo
 *
 * Description:
 *   Enable RX timeout feature when FIFO is empty
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   bEnable - TRUE enable, FALSE disable
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_rx_timeout_with_empty_rx_fifo(uint32_t uBase, bool bEnable)
{
	if (bEnable) {
		HW_REG32(uBase, UART_CON) |= (0x1 << 11);
	} else {
		HW_REG32(uBase, UART_CON) &= ~(0x1 << 11);
	}
}

/****************************************************************************
 * Name: up_uart_set_rx_timeout_suspend_dma
 *
 * Description:
 *   Enable suspend DMA in case of RX timeout
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   bEnable - TRUE enable, FALSE disable
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_rx_timeout_suspend_dma(uint32_t uBase, bool bEnable)
{
	if (bEnable) {
		HW_REG32(uBase, UART_CON) |= (0x1 << 10);
	} else {
		HW_REG32(uBase, UART_CON) &= ~(0x1 << 10);
	}
}

/****************************************************************************
 * Name: up_uart_set_fifo_mode
 *
 * Description:
 *   Enable use of FIFO
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   bEnable - TRUE enable, FALSE disable
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_fifo_mode(uint32_t uBase, bool bEnable)
{
	HW_REG32(uBase, UART_FCON) |= (0x3 << 1);

	if (bEnable) {
		HW_REG32(uBase, UART_FCON) |= (0x1 << 0);
	} else {
		HW_REG32(uBase, UART_FCON) &= ~(0x1 << 0);
	}
}

/****************************************************************************
 * Name: up_uart_set_tx_trigger_level
 *
 * Description:
 *   Set TX FIFO interrupt trigger level
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   eTriggerLevel - trigger level
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_tx_trigger_level(uint32_t uBase, UART_TRIGGER_LEVEL eTriggerLevel)
{
	HW_REG32(uBase, UART_FCON) &= ~(0x7 << 8);
	HW_REG32(uBase, UART_FCON) |= (eTriggerLevel << 8);
}

/****************************************************************************
 * Name: up_uart_set_rx_trigger_level
 *
 * Description:
 *   Set RX FIFO interrupt trigger level
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   eTriggerLevel - trigger level
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_rx_trigger_level(uint32_t uBase, UART_TRIGGER_LEVEL eTriggerLevel)
{
	HW_REG32(uBase, UART_FCON) &= ~(0x7 << 4);
	HW_REG32(uBase, UART_FCON) |= (eTriggerLevel << 4);
}

/****************************************************************************
 * Name: up_uart_set_rts_trigger_level
 *
 * Description:
 *   Set RTS line trigger fifo level
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   eRTSTriggerLevel - trigger level
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_rts_trigger_level(uint32_t uBase, UART_RTS_TRIGGER_LEVEL eRTSTriggerLevel)
{
	HW_REG32(uBase, UART_MCON) &= ~(0x7 << 5);
	HW_REG32(uBase, UART_MCON) |= (eRTSTriggerLevel << 5);
}

/****************************************************************************
 * Name: up_uart_set_tx_dma_burst_size
 *
 * Description:
 *   Set DMA transfer burst size for TX channel
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   eBurstLength - Burst Length
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_tx_dma_burst_size(uint32_t uBase, UART_BURST_LENGTH eBurstLength)
{
	HW_REG32(uBase, UART_CON) &= ~(0x7 << 20);
	HW_REG32(uBase, UART_CON) |= (eBurstLength << 20);
}

/****************************************************************************
 * Name: up_uart_set_rx_dma_burst_size
 *
 * Description:
 *   Set DMA transfer burst size for RX channel
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   eBurstLength - Burst Length
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_set_rx_dma_burst_size(uint32_t uBase, UART_BURST_LENGTH eBurstLength)
{
	HW_REG32(uBase, UART_CON) &= ~(0x7 << 16);
	HW_REG32(uBase, UART_CON) |= (eBurstLength << 16);
}

/****************************************************************************
 * Name: up_uart_clear_interrupt_status
 *
 * Description:
 *   clear interrupt flags
 *
 * Input Parameters:
 *   uBase - base addres of UART channel
 *   eInt - Mask to clear interrupt flags
 *
 * Returned value:
 *   None
 ****************************************************************************/
static void up_uart_clear_interrupt_status(uint32_t uBase, UART_INTERRUPT eInt)
{
	HW_REG32(uBase, UART_INTP) |= eInt;
}

/****************************************************************************
 * Name: up_disableuartint
 *
 * Description:
 *   disable interrupt
 *
 * Input Parameters:
 *   priv - pointer to uart dev private sitructure
 *   im - pointer to store interrupt mask for restore later
 *
 * Returned value:
 *   None
 ****************************************************************************/

static inline void up_disableuartint(struct up_dev_s *priv, uint32_t *im)
{
	/* Return the current interrupt mask value */

	if (im) {
		*im = priv->im;
	}

	/* Disable all interrupts */

	priv->im = ALL_INT;

	HW_REG32(priv->uartbase, UART_INTM) = priv->im;

	if (priv->im & ERROR_INT) {
		HW_REG32(priv->uartbase, UART_CON) &= ~(0x1 << 6);
	}

	if (priv->im & MODEM_INT) {
		HW_REG32(priv->uartbase, UART_MCON) &= ~(0x1 << 3);
	}
}

/****************************************************************************
 * Name: up_restoreuartint
 *
 * Description:
 *   restore interrupt mask
 *
 * Input Parameters:
 *   priv - pointer to uart dev private sitructure
 *   im - interrupt mask to restore
 *
 * Returned value:
 *   None
 ****************************************************************************/

static inline void up_restoreuartint(struct up_dev_s *priv, uint32_t im)
{
	priv->im = im;

	if (priv->im & ERROR_INT) {
		HW_REG32(priv->uartbase, UART_CON) |= (0x1 << 6);
	}

	if (priv->im & MODEM_INT) {
		HW_REG32(priv->uartbase, UART_MCON) |= (0x1 << 3);
	}

	HW_REG32(priv->uartbase, UART_INTM) = priv->im;
}

/****************************************************************************
 * Name: up_uart_initialize
 *
 * Description:
 *   Initialize uart port.
 *
 * Input Parameters:
 *   priv - pointer to uart dev private sitructure
 *   nBaudrate - baudrate, bps
 *   nClock - clock source, HZ
 *
 * Returned Value:
 *   int value, lower byte contains read value
 ****************************************************************************/

void up_uart_initialize(struct up_dev_s *priv, UART_BAUDRATE nBaudrate, u32 nClock)
{
	UART_CHANNEL eCh;
	uint32_t uBase = priv->uartbase;

	eCh = priv->eCh;

	up_uart_set_gpio(eCh);

	up_uart_set_rx_mode(uBase, INT_MODE);
	up_uart_set_tx_mode(uBase, INT_MODE);

	up_uart_disable_interrupt(uBase, ALL_INT);
	up_uart_clear_interrupt_status(uBase, ALL_INT);

	up_uart_set_baudrate(uBase, nBaudrate, nClock);

	up_uart_set_loopback(uBase, FALSE);
	up_uart_set_infrared_mode(uBase, FALSE);

	up_uart_set_word_length(uBase, WL_8BIT);
	up_uart_set_stop_bit(uBase, priv->stopbits2);
	up_uart_set_parity_mode(uBase, priv->parity);

	up_uart_set_rx_timeout(uBase, TRUE);
	up_uart_set_rx_timeout_interval(uBase, 3);
	up_uart_set_rx_timeout_with_empty_rx_fifo(uBase, FALSE);
	up_uart_set_rx_timeout_suspend_dma(uBase, FALSE);

	up_uart_set_fifo_mode(uBase, TRUE);

	up_uart_set_rts_trigger_level(uBase, RTL_1);

	up_uart_set_tx_trigger_level(uBase, TL_1);
	up_uart_set_rx_trigger_level(uBase, TL_1);

	up_uart_set_tx_dma_burst_size(uBase, BL_1);
	up_uart_set_rx_dma_burst_size(uBase, BL_1);
}

/****************************************************************************
 * Name: up_setup
 *
 * Description:
 *   Configure the UART baud, bits, parity, fifos, etc. This
 *   method is called the first time that the serial port is
 *   opened.
 *
 ****************************************************************************/

static int up_setup(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;

	uart_init(priv->eCh);

	priv->im = HW_REG32(priv->uartbase, UART_INTM);
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

static void up_shutdown(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	up_disableuartint(priv, NULL);
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

static int up_attach(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	int ret;

	/* Attach and enable the IRQ */

	ret = irq_attach(priv->irq, up_interrupt, NULL);
	if (ret == OK) {
		/* Enable the interrupt (RX and TX interrupts are still disabled
		 * in the UART
		 */
		up_enable_irq(priv->irq);
	}

	return ret;
}

/****************************************************************************
 * Name: up_detach
 *
 * Description:
 *   Detach UART interrupts.  This method is called when the serial port is
 *   closed normally before the shutdown method is called.The exception is
 *   the serial console which is never shutdown.
 *
 ****************************************************************************/

static void up_detach(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	up_disable_irq(priv->irq);
	irq_detach(priv->irq);
}

/****************************************************************************
 * Name: up_interrupt
 *
 * Description:
 *   This is the UART interrupt handler.  It will be invoked
 *   when an interrupt received on the 'irq'  It should call
 *   uart_transmitchars or uart_receivechar to perform the
 *   appropriate data transfers.  The interrupt handling logic\
 *   must be able to map the 'irq' number into the approprite
 *   uart_dev_s structure in order to call these functions.
 *
 ****************************************************************************/

static int up_interrupt(int irq, void *context, void *arg)
{
	struct uart_dev_s *dev = NULL;
	struct up_dev_s *priv;
	UART_INTERRUPT mis;
	int passes;
	bool handled;
#ifdef CONFIG_S5J_UARTDBG
	if (g_uart_priv[UARTDBG].irq == irq) {
		dev = &g_uart_port[UARTDBG];
	} else
#endif

#ifdef CONFIG_S5J_UART0
		if (g_uart_priv[UART0].irq == irq) {
			dev = &g_uart_port[UART0];
		} else
#endif
#ifdef CONFIG_S5J_UART1
			if (g_uart_priv[UART1].irq == irq) {
				dev = &g_uart_port[UART1];
			} else
#endif
#ifdef CONFIG_S5J_UART2
				if (g_uart_priv[UART2].irq == irq) {
					dev = &g_uart_port[UART2];
				} else
#endif
#ifdef CONFIG_S5J_UART3
					if (g_uart_priv[UART3].irq == irq) {
						dev = &g_uart_port[UART3];
					} else
#endif
					{
						PANIC();
					}

	priv = (struct up_dev_s *)dev->priv;

	/* Loop until there are no characters to be transferred or,
	 * until we have been looping for a long time.
	 */

	handled = true;
	for (passes = 0; passes < 256 && handled; passes++) {
		handled = false;

		/* Get the masked UART status and clear the pending interrupts. */

		mis = (HW_REG32(priv->uartbase, UART_INTP) & UART_INTP_MASK);
		HW_REG32(priv->uartbase, UART_INTP) = mis;

		/* Handle incoming, receive bytes (with or without timeout) */

		if ((mis & RX_INT) != 0) {

			/* Rx buffer not empty ... process incoming bytes */

			uart_recvchars(dev);
			handled = true;
		}

		/* Handle outgoing, transmit bytes */

		if ((mis & TX_INT) != 0) {
			/* Tx FIFO not full ... process outgoing bytes */

			uart_xmitchars(dev);
			handled = true;
		}

		if ((mis & ERROR_INT) != 0) {
			PANIC();
		}

		if ((mis & MODEM_INT) != 0) {
			PANIC();
		}
	}

	return OK;
}

/****************************************************************************
 * Name: up_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method
 *
 * Input Parameters:
 *   filep - pointer to file structure
 *   cmd - command
 *   arg - argument
 *
 * Returned Value:
 *   error value
 ****************************************************************************/

static int up_ioctl(struct file *filep, int cmd, unsigned long arg)
{
#if defined(CONFIG_SERIAL_TERMIOS)
	struct inode *inode = filep->f_inode;
	struct uart_dev_s *dev = inode->i_private;
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
#endif
	int ret = OK;

	switch (cmd) {
#if defined(CONFIG_SERIAL_TERMIOS)
	case TCGETS: {
		struct termios *termiosp = (struct termios *)arg;
		if (!termiosp) {
			ret = -EINVAL;
			break;
		}
		cfsetispeed(termiosp, priv->baud);
		termiosp->c_cflag = ((priv->parity != 0) ? PARENB : 0) | ((priv->parity == PM_ODD) ? PARODD : 0) | ((priv->stopbits2) ? CSTOPB : 0) | CS8;
		/* We do not support flowcontrol. */
		break;
	}

	case TCSETS: {
		struct termios *termiosp = (struct termios *)arg;
		if (!termiosp) {
			ret = -EINVAL;
			break;
		}

		if ((termiosp->c_cflag & CSIZE) != CS8) {
			ret = -EINVAL;
			break;
		}

		priv->parity = 0;
		if (termiosp->c_cflag & PARENB) {
			priv->parity = (termiosp->c_cflag & PARODD) ? PM_ODD : PM_EVEN;
		}

		priv->stopbits2 = ((termiosp->c_cflag & CSTOPB) != 0) ? SB_2 : SB_1;
		priv->baud = cfgetispeed(termiosp);
		dev->ops->setup(dev);
		break;
	}
#endif
	default:
		ret = -ENOTTY;
		break;
	}

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
 * Input Parameters:
 *   dev - pointer to uart dev structure
 *   status - error bits
 *
 * Returned Value:
 *   int value, lower byte contains read value
 ****************************************************************************/

static int up_receive(struct uart_dev_s *dev, uint32_t *status)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	*status = 0;
	int empty;
	do {
		empty = !(__raw_readl(priv->uartbase + UART_FSTAT) & UART_FSTAT_RX_MASK);
	} while (empty);
	return (HW_REG32(priv->uartbase, UART_RXH) & UART_RX_MASK);
}

/****************************************************************************
 * Name: up_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts
 *
 * Input Parameters:
 *   dev - pointer to uart dev structure
 *   enable - true enable, falde disable
 *
 * Returned Value:
 *   None
 ****************************************************************************/
static void up_rxint(struct uart_dev_s *dev, bool enable)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	if (enable) {
#ifndef CONFIG_SUPPRESS_SERIAL_INTS
		priv->im &= ~(RX_INT);
#endif
	} else {
		priv->im |= (RX_INT);
	}

	up_restoreuartint(priv, priv->im);
}

/****************************************************************************
 * Name: up_rxavailable
 *
 * Description:
 *   Return true if the receive fifo is not empty
 *
 * Input Parameters:
 *   dev - pointer to uart dev structure
 *
 * Returned Value:
 *   true is fifo is not empty
 ****************************************************************************/
static bool up_rxavailable(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;

	return ((HW_REG32(priv->uartbase, UART_FSTAT) & UART_FSTAT_RX_MASK) != 0);
}

/****************************************************************************
 * Name: up_send
 *
 * Description:
 *   This method will send one byte on the UART
 *
 * Input Parameters:
 *   dev - pointer to uart dev structure
 *   ch - int value, lower byte will be sent through the uart
 *
 * Returned Value:
 *   None
 ****************************************************************************/
static void up_send(struct uart_dev_s *dev, int ch)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	HW_REG32(priv->uartbase, UART_TXH) = ch;

}

/****************************************************************************
 * Name: up_txint
 *
 * Description:
 *   Call to enable or disable TX interrupts
 *
 * Input Parameters:
 *   dev - pointer to uart dev structure
 *   enable - true enable, falde disable
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void up_txint(struct uart_dev_s *dev, bool enable)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	irqstate_t flags;

	flags = irqsave();
	if (enable) {
		/* Set to receive an interrupt when the TX fifo is half emptied */

#ifndef CONFIG_SUPPRESS_SERIAL_INTS
		priv->im &= ~(TX_INT);
		up_restoreuartint(priv, priv->im);
		/* The serial driver wants an interrupt here, but will not get get
		 * one unless we "prime the pump."  I believe that this is because
		 * behave like a level interrupt and the Stellaris interrupts behave
		 * (at least by default) like edge interrupts.
		 *
		 * In any event, faking a TX interrupt here solves the problem;
		 * Call uart_xmitchars() just as would have been done if we recieved
		 * the TX interrupt.
		 */

		uart_xmitchars(dev);
#endif
	} else {
		/* Disable the TX interrupt */
		priv->im |= (TX_INT);
		up_restoreuartint(priv, priv->im);
	}
	irqrestore(flags);
}

/****************************************************************************
 * Name: up_txready
 *
 * Description:
 *   Check if the tranmsit fifo is not full
 *
 * Input Parameters:
 *   dev - pointer to uart dev structure
 *
 * Returned Value:
 *   TRUE if fifo is not full
 ****************************************************************************/
static bool up_txready(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	return ((HW_REG32(priv->uartbase, UART_FSTAT) & UART_FSTAT_TX_MASK) == 0);

}

/****************************************************************************
 * Name: up_txempty
 *
 * Description:
 *   Check if the transmit fifo is empty
 *
 * Input Parameters:
 *   dev - pointer to uart dev structure
 *
 * Returned Value:
 *   TRUE if fifo is empty
 ****************************************************************************/
static bool up_txempty(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	return ((HW_REG32(priv->uartbase, UART_FSTAT) & UART_FSTAT_TX_MASK) == 0);
}

/****************************************************************************
 * Name: uart_send_data
 *
 * Description:
 *   Send sigle byte through selected channel.
 *
 * Input Parameters:
 *   eCh - channel number
 *   cData - byte to send
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void uart_send_data(UART_CHANNEL eCh, char cData)
{
	struct up_dev_s *priv;
	priv = g_uart_port[CONSOLE_PORT].priv;
	while (!up_txempty(&g_uart_port[CONSOLE_PORT])) ;
	HW_REG32(priv->uartbase, UART_TXH) = cData;
}

/****************************************************************************
 * Name: uart_init
 *
 * Description:
 *   Initializes seleced uart channel.
 *   Set CLK source, init baudrate, disable interrupt.
 *
 * Input Parameters:
 *   uart - channel number
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void uart_init(UART_CHANNEL uart)
{
	/* Initialize UART */
	int clk;
	struct up_dev_s *priv = g_uart_port[uart].priv;
	int baud_rate = priv->baud;

	cal_clk_enable(m1_clkcmu_uart);
	cal_clk_setrate(m1_clkcmu_uart, 0);	/* 26Mhz */
	clk = cal_clk_getrate(m1_clkcmu_uart);

	up_uart_initialize(priv, baud_rate, clk);
	up_uart_disable_interrupt(priv->uartbase, ALL_INT);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_earlyserialinit
 *
 * Description:
 *   Performs the low level UART initialization early in
 *   debug so that the serial console will be available
 *   during bootup.  This must be called before up_serialinit.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 ****************************************************************************/
#if defined(USE_EARLYSERIALINIT)
void up_earlyserialinit(void)
{
	/* NOTE:  All GPIO configuration for the UARTs was performed in
	 * up_lowsetup
	 */
	/* Disable all UARTS */
#ifdef TTYS0_DEV
	up_disableuartint(TTYS0_DEV.priv, NULL);
#endif

#ifdef TTYS1_DEV
	up_disableuartint(TTYS1_DEV.priv, NULL);
#endif

#ifdef TTYS2_DEV
	up_disableuartint(TTYS2_DEV.priv, NULL);
#endif

#ifdef TTYS3_DEV
	up_disableuartint(TTYS3_DEV.priv, NULL);
#endif

#ifdef TTYDBG_DEV
	up_disableuartint(TTYDBG_DEV.priv, NULL);
#endif

	/* Configure whichever one is the console */

#ifdef HAVE_SERIAL_CONSOLE
	CONSOLE_DEV.isconsole = true;
	up_setup(&CONSOLE_DEV);
#endif
}
#endif

/****************************************************************************
 * Name: up_serialinit
 *
 * Description:
 *   Register serial console and serial ports.  This assumes
 *   that up_earlyserialinit was called previously.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 ****************************************************************************/
#if defined(USE_SERIALDRIVER)
void up_serialinit()
{
	/* Register the console */

#ifdef CONFIG_S5J_UART0
	(void)uart_register("/dev/ttyS0", &TTYS0_DEV);
#endif
#ifdef CONFIG_S5J_UART1
	(void)uart_register("/dev/ttyS1", &TTYS1_DEV);
#endif
#ifdef CONFIG_S5J_UART2
	(void)uart_register("/dev/ttyS2", &TTYS2_DEV);
#endif
#ifdef CONFIG_S5J_UART3
	(void)uart_register("/dev/ttyS3", &TTYS3_DEV);
#endif

#ifdef CONFIG_S5J_UARTDBG
	(void)uart_register("/dev/ttyDBG", &TTYDBG_DEV);
#endif

#ifdef HAVE_SERIAL_CONSOLE

#if !defined(USE_EARLYSERIALINIT)
	CONSOLE_DEV.isconsole = true;
	up_setup(&CONSOLE_DEV);
#endif

	(void)uart_register("/dev/console", &CONSOLE_DEV);
#endif

}
#endif

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
#ifdef HAVE_SERIAL_CONSOLE
	/* Check for LF */
	if (ch == '\n') {
		/* Add CR */
		up_lowputc('\r');
	}
	up_lowputc(ch);
#endif
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
int up_getc()
{
#ifdef HAVE_SERIAL_CONSOLE
	struct uart_dev_s *dev = &g_uart_port[CONSOLE_PORT];
	uint32_t state;

	if (up_rxavailable(dev)) {
		return up_receive(dev, &state);
	} else {
		return -1;
	}
#else
	return -1;
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
	uart_send_data(CONSOLE_PORT, ch);
}
