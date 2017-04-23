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

#ifdef CONFIG_SERIAL_TERMIOS
#include <termios.h>
#endif

#include <arch/serial.h>
#include <arch/board/board.h>

#include "chip.h"
#include "s5j_serial.h"
#include "s5j_gpio.h"
#include "s5j_vclk.h"
#include "up_arch.h"
#include "up_internal.h"

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

#ifdef HAVE_SERIAL_CONSOLE
#if defined(CONFIG_UART0_SERIAL_CONSOLE)
#define CONSOLE_PORT		UART0
#define CONSOLE_DEV			g_uart0priv.dev
#define CONSOLE_BASE		UART0_BASE
#elif defined(CONFIG_UART1_SERIAL_CONSOLE)
#define CONSOLE_PORT		UART1
#define CONSOLE_DEV			g_uart1priv.dev
#define CONSOLE_BASE		UART1_BASE
#elif defined(CONFIG_UART2_SERIAL_CONSOLE)
#define CONSOLE_PORT		UART2
#define CONSOLE_DEV			g_uart2priv.dev
#define CONSOLE_BASE		UART2_BASE
#elif defined(CONFIG_UART3_SERIAL_CONSOLE)
#define CONSOLE_PORT		UART3
#define CONSOLE_DEV			g_uart3priv.dev
#define CONSOLE_BASE		UART3_BASE
#elif defined(CONFIG_OTHER_SERIAL_CONSOLE)
#define CONSOLE_PORT		UARTDBG
#define CONSOLE_DEV			g_uartdbgpriv.dev
#define CONSOLE_BASE		UARTDBG_BASE
#else
#define CONSOLE_PORT		UARTDBG
#define CONSOLE_DEV			g_uartdbgpriv.dev
#define CONSOLE_BASE		UARTDBG_BASE
#endif
#endif

#ifndef CONSOLE_PORT
#define CONSOLE_PORT		UARTDBG
#endif

#define S5J_NUART	5

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct up_dev_s {
	struct uart_dev_s dev;		/* Generic UART device */
	uint32_t uartbase;			/* Base address of UART registers */
	uint32_t baud;				/* Configured baud */
	UART_INTERRUPT im;			/* Saved IM value */
	uint16_t irq;				/* IRQ associated with this UART */
	uint8_t parity;				/* 0=none, 4=odd, 5=even */
	uint8_t bits;				/* Number of bits (5, 6, 7 or 8) */
	bool stopbits2;				/* true: Configure with 2 stop bits instead of 1 */
	UART_CHANNEL eCh;			/*  Number of Uart Channel  */
	s32 rx_gpio;
	s32 tx_gpio;
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

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/* UART operations structure */

static const struct uart_ops_s g_uart_ops = {
	.setup		= up_setup,
	.shutdown	= up_shutdown,
	.attach		= up_attach,
	.detach		= up_detach,
	.ioctl		= up_ioctl,
	.receive	= up_receive,
	.rxint		= up_rxint,
	.rxavailable = up_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
	.rxflowcontrol = NULL,
#endif
	.send		= up_send,
	.txint		= up_txint,
	.txready	= up_txready,
	.txempty	= up_txempty,
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

#ifdef CONFIG_S5J_UARTDBG
static struct up_dev_s g_uartdbgpriv = {
	.dev		= {
		.recv = {
			.size	= CONFIG_UARTDBG_RXBUFSIZE,
			.buffer	= g_uartdbgrxbuffer,
		},
		.xmit = {
			.size	= CONFIG_UARTDBG_TXBUFSIZE,
			.buffer	= g_uartdbgtxbuffer,
		},
		.ops		= &g_uart_ops,
		.priv		= &g_uartdbgpriv,
	},
	.uartbase	= UARTDBG_BASE,
	.baud		= CONFIG_UARTDBG_BAUD,
	.irq		= S5J_IRQ_UARTDBG,
	.parity		= CONFIG_UARTDBG_PARITY,
	.bits		= CONFIG_UARTDBG_BITS,
	.stopbits2	= CONFIG_UARTDBG_2STOP,
	.eCh		= UARTDBG,
	.rx_gpio	= GPIO_UARTDBG_RXD,
	.tx_gpio	= GPIO_UARTDBG_TXD,
};
#else
static struct up_dev_s g_uartdbgpriv = {
	.dev		= {
		.recv = {
			.size	= 0,
			.buffer	= NULL,
		},
		.xmit = {
			.size	= 0,
			.buffer	= NULL,
		},
		.ops		= NULL,
		.priv		= NULL,
	},
	.uartbase	= 0,
	.baud		= 0,
	.irq		= 0,
	.parity		= 0,
	.bits		= 0,
	.stopbits2	= 0,
	.eCh		= -1,
	.rx_gpio	= 0,
	.tx_gpio	= 0,
};
#endif

#ifdef CONFIG_S5J_UART0
static struct up_dev_s g_uart0priv = {
	.dev		= {
		.recv = {
			.size	= CONFIG_UART0_RXBUFSIZE,
			.buffer	= g_uart0rxbuffer,
		},
		.xmit = {
			.size	= CONFIG_UART0_TXBUFSIZE,
			.buffer	= g_uart0txbuffer,
		},
		.ops		= &g_uart_ops,
		.priv		= &g_uart0priv,
	},
	.uartbase	= UART0_BASE,
	.baud		= CONFIG_UART0_BAUD,
	.irq		= S5J_IRQ_UART0,
	.parity		= CONFIG_UART0_PARITY,
	.bits		= CONFIG_UART0_BITS,
	.stopbits2	= CONFIG_UART0_2STOP,
	.eCh		= UART0,
	.rx_gpio	= GPIO_UART0_RXD,
	.tx_gpio	= GPIO_UART0_TXD,
};
#endif

#ifdef CONFIG_S5J_UART1
static struct up_dev_s g_uart1priv = {
	.dev		= {
		.recv = {
			.size	= CONFIG_UART1_RXBUFSIZE,
			.buffer	= g_uart1rxbuffer,
		},
		.xmit = {
			.size	= CONFIG_UART1_TXBUFSIZE,
			.buffer	= g_uart1txbuffer,
		},
		.ops		= &g_uart_ops,
		.priv		= &g_uart1priv,
	},
	.uartbase	= UART1_BASE,
	.baud		= CONFIG_UART1_BAUD,
	.irq		= S5J_IRQ_UART1,
	.parity		= CONFIG_UART1_PARITY,
	.bits		= CONFIG_UART1_BITS,
	.stopbits2	= CONFIG_UART1_2STOP,
	.eCh		= UART1,
	.rx_gpio	= GPIO_UART1_RXD,
	.tx_gpio	= GPIO_UART1_TXD,
};
#endif

#ifdef CONFIG_S5J_UART2
static struct up_dev_s g_uart2priv = {
	.dev		= {
		.recv = {
			.size	= CONFIG_UART2_RXBUFSIZE,
			.buffer	= g_uart2rxbuffer,
		},
		.xmit = {
			.size	= CONFIG_UART2_TXBUFSIZE,
			.buffer	= g_uart2txbuffer,
		},
		.ops		= &g_uart_ops,
		.priv		= &g_uart2priv,
	},
	.uartbase	= UART2_BASE,
	.baud		= CONFIG_UART2_BAUD,
	.irq		= S5J_IRQ_UART2,
	.parity		= CONFIG_UART2_PARITY,
	.bits		= CONFIG_UART2_BITS,
	.stopbits2	= CONFIG_UART2_2STOP,
	.eCh		= UART2,
	.rx_gpio	= GPIO_UART2_RXD,
	.tx_gpio	= GPIO_UART2_TXD,
};
#endif

#ifdef CONFIG_S5J_UART3
static struct up_dev_s g_uart3priv = {
	.dev		= {
		.recv = {
			.size	= CONFIG_UART3_RXBUFSIZE,
			.buffer	= g_uart3rxbuffer,
		},
		.xmit = {
			.size	= CONFIG_UART3_TXBUFSIZE,
			.buffer	= g_uart3txbuffer,
		},
		.ops		= &g_uart_ops,
		.priv		= &g_uart3priv,
	},
	.uartbase	= UART3_BASE,
	.baud		= CONFIG_UART3_BAUD,
	.irq		= S5J_IRQ_UART3,
	.parity		= CONFIG_UART3_PARITY,
	.bits		= CONFIG_UART3_BITS,
	.stopbits2 	= CONFIG_UART3_2STOP,
	.eCh		= UART3,
	.rx_gpio	= GPIO_UART3_RXD,
	.tx_gpio	= GPIO_UART3_TXD,
};
#endif

static struct up_dev_s * const uart_devs[S5J_NUART] =
{
#ifdef CONFIG_S5J_UART0
	[0] = &g_uart0priv,
#endif
#ifdef CONFIG_S5J_UART1
	[1] = &g_uart1priv,
#endif
#ifdef CONFIG_S5J_UART2
	[2] = &g_uart2priv,
#endif
#ifdef CONFIG_S5J_UART3
	[3] = &g_uart3priv,
#endif
#ifdef CONFIG_S5J_UARTDBG
	[S5J_NUART - 1] = &g_uartdbgpriv,
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

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
		modifyreg32(uBase + UART_CON, 0, (0x1 << 6));
	}

	if (eInt & MODEM_INT) {
		modifyreg32(uBase + UART_MCON, 0, (0x1 << 3));
	}

	modifyreg32(uBase + UART_INTM, eInt, 0);
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
	modifyreg32(uBase + UART_INTM, 0, eInt);

	if (eInt & ERROR_INT) {
		modifyreg32(uBase + UART_CON, 0x1 << 6, 0);
	}

	if (eInt & MODEM_INT) {
		modifyreg32(uBase + UART_MCON, 0x1 << 3, 0);
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
	modifyreg32(uBase + UART_CON, 0x3 << 2, 0);
	modifyreg32(uBase + UART_CON, 0x1 << 9, 0);
	up_uart_disable_interrupt(uBase, TX_INT);	/*  Disable TX Interrupt */

	switch (eMode) {
	case POLL_MODE:
		modifyreg32(uBase + UART_CON, 0, 0x1 << 2);
		break;

	case INT_MODE:
		modifyreg32(uBase + UART_CON, 0, 0x1 << 2);
		modifyreg32(uBase + UART_CON, 0, 0x1 << 9);
		up_uart_enable_interrupt(uBase, TX_INT);	/*  Enable TX Interrupt */
		break;

	case DMA_MODE:
		modifyreg32(uBase + UART_CON, 0, 0x2 << 2);
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
	modifyreg32(uBase + UART_CON, 0x3 << 0, 0);
	modifyreg32(uBase + UART_CON, 0x1 << 8, 0);
	up_uart_disable_interrupt(uBase, (UART_INTERRUPT)(RX_INT | ERROR_INT));

	switch (eMode) {
	case POLL_MODE:
		modifyreg32(uBase + UART_CON, 0, 0x1 << 0);
		up_uart_enable_interrupt(uBase, ERROR_INT);
		break;

	case INT_MODE:
		modifyreg32(uBase + UART_CON, 0, 0x1 << 0);
		modifyreg32(uBase + UART_CON, 0, 0x1 << 8);
		up_uart_enable_interrupt(uBase, (UART_INTERRUPT)(RX_INT | ERROR_INT));
		break;

	case DMA_MODE:
		modifyreg32(uBase + UART_CON, 0, 0x2 << 0);
		modifyreg32(uBase + UART_CON, 0, 0x1 << 8);
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

	putreg32(fDiv, uBase + UART_BRDIV);
	putreg32(fFrac, uBase + UART_FRACVAL);
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
		modifyreg32(uBase + UART_LCON, 0, 0x1 << 6);
	} else {
		modifyreg32(uBase + UART_LCON, 0x1 << 6, 0);
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
	modifyreg32(uBase + UART_LCON, 0x7 << 3, eParityMode << 3);
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
	modifyreg32(uBase + UART_LCON, 0x1 << 2, eStopBit << 2);
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
	modifyreg32(uBase + UART_LCON, 0x3 << 0, eWordLen << 0);
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
		modifyreg32(uBase + UART_CON, 0, 0x1 << 5);
	} else {
		modifyreg32(uBase + UART_CON, 0x1 << 5, 0);
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
	modifyreg32(uBase + UART_CON, 0xf << 12, (nTime & 0xf) << 12);
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
		modifyreg32(uBase + UART_CON, 0, 0x1 << 7);
	} else {
		modifyreg32(uBase + UART_CON, 0x1 << 7, 0);
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
		modifyreg32(uBase + UART_CON, 0, 0x1 << 11);
	} else {
		modifyreg32(uBase + UART_CON, 0x1 << 11, 0);
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
		modifyreg32(uBase + UART_CON, 0, 0x1 << 10);
	} else {
		modifyreg32(uBase + UART_CON, 0x1 << 10, 0);
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
	modifyreg32(uBase + UART_FCON, 0, 0x3 << 1);

	if (bEnable) {
		modifyreg32(uBase + UART_FCON, 0, 0x1 << 0);
	} else {
		modifyreg32(uBase + UART_FCON, 0x1 << 0, 0);
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
	modifyreg32(uBase + UART_FCON, 0x7 << 8, eTriggerLevel << 8);
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
	modifyreg32(uBase + UART_FCON, 0x7 << 4, eTriggerLevel << 4);
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
	modifyreg32(uBase + UART_MCON, 0x7 << 5, eRTSTriggerLevel << 5);
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
	modifyreg32(uBase + UART_CON, 0x7 << 20, eBurstLength << 20);
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
	modifyreg32(uBase + UART_CON, 0x7 << 16, eBurstLength << 16);
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
	putreg32(eInt, uBase + UART_INTP);
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

	putreg32(priv->im, priv->uartbase + UART_INTM);

	if (priv->im & ERROR_INT) {
		modifyreg32(priv->uartbase + UART_CON, 0x1 << 6, 0);
	}

	if (priv->im & MODEM_INT) {
		modifyreg32(priv->uartbase + UART_MCON, 0x1 << 3, 0);
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
		modifyreg32(priv->uartbase + UART_CON, 0, 0x1 << 6);
	}

	if (priv->im & MODEM_INT) {
		modifyreg32(priv->uartbase + UART_MCON, 0, 0x1 << 3);
	}

	putreg32(priv->im, priv->uartbase + UART_INTM);
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

void up_uart_initialize(struct up_dev_s *priv, u32 nClock)
{
	uint32_t uBase = priv->uartbase;

	/* Configure pins for USART use */
	s5j_configgpio(priv->rx_gpio);
	s5j_configgpio(priv->tx_gpio);

	up_uart_set_rx_mode(uBase, INT_MODE);
	up_uart_set_tx_mode(uBase, INT_MODE);

	up_uart_disable_interrupt(uBase, ALL_INT);
	up_uart_clear_interrupt_status(uBase, ALL_INT);

	up_uart_set_baudrate(uBase, priv->baud, nClock);

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

	/* Initialize UART */

	switch (priv->eCh) {
		case UART0:
			priv = &g_uart0priv;
			break;
		case UART1:
			priv = &g_uart1priv;
			break;
		case UART2:
			priv = &g_uart2priv;
			break;
		case UART3:
			priv = &g_uart3priv;
			break;
		case UARTDBG:
		default:
			priv = &g_uartdbgpriv;
			break;
	}

	int clk = cal_clk_getrate(m1_clkcmu_uart);	/* 26Mhz */
	up_uart_initialize(priv, clk);
	up_uart_disable_interrupt(priv->uartbase, ALL_INT);

	priv->im = getreg32(priv->uartbase + UART_INTM);

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

	ret = irq_attach(priv->irq, up_interrupt, dev);
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
	struct uart_dev_s *dev = (struct uart_dev_s *)arg;
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	int passes;
	bool handled;

	/* Loop until there are no characters to be transferred or,
	 * until we have been looping for a long time.
	 */

	handled = true;
	for (passes = 0; passes < 256 && handled; passes++) {
		handled = false;

		/* Get the masked UART status and clear the pending interrupts. */

		UART_INTERRUPT mis = getreg32(priv->uartbase + UART_INTP) & UART_INTP_MASK;
		putreg32(mis, priv->uartbase + UART_INTP);

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
		empty = !(getreg32(priv->uartbase + UART_FSTAT) & UART_FSTAT_RX_MASK);
	} while (empty);

	return getreg32(priv->uartbase + UART_RXH) & UART_RX_MASK;
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

	return ((getreg32(priv->uartbase + UART_FSTAT) & UART_FSTAT_RX_MASK) != 0);
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
	putreg32(ch, priv->uartbase + UART_TXH);
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
	return ((getreg32(priv->uartbase + UART_FSTAT) & UART_FSTAT_TX_MASK) == 0);
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
	return ((getreg32(priv->uartbase + UART_FSTAT) & UART_FSTAT_TX_MASK) == 0);
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

	unsigned int i;
	for (i = 0; i < S5J_NUART; i++) {
		if (uart_devs[i]) {
			up_disableuartint(uart_devs[i], NULL);
		}
	}

	/* Configure whichever one is the console */

#ifdef CONSOLE_DEV
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
void up_serialinit(void)
{
	/* Register the console */

	unsigned int i;
	char devname[16];
	for (i = 0; i < (S5J_NUART - 1); i++) {
		if (uart_devs[i]) {
			strcpy(devname, "/dev/ttySx");
			devname[9] = '0' + i;
			(void)uart_register(devname, &uart_devs[i]->dev);
		}
	}

#ifdef CONFIG_S5J_UARTDBG
	(void)uart_register("/dev/ttyDBG", &uart_devs[S5J_NUART - 1]->dev);
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
int up_getc(void)
{
#ifdef HAVE_SERIAL_CONSOLE
	uint32_t state;
	if (up_rxavailable(&CONSOLE_DEV)) {
		return up_receive(&CONSOLE_DEV, &state);
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
	while (!up_txempty(&CONSOLE_DEV));
	putreg32(ch, CONSOLE_BASE + UART_TXH);
}
