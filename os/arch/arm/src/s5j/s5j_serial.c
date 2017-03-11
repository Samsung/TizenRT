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

//#ifdef USE_SERIALDRIVER

#ifndef CONFIG_NO_SERIAL_CONSOLE
#define HAVE_SERIAL_CONSOLE
#endif

/* Which UART with be tty0/console and which tty1-7?  The console will always
 * be ttyS0.  If there is no console then will use the lowest numbered UART.
 */

/* First pick the console and ttys0.  This could be any of UART0-5 */

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
#define CONSOLE_DEV         g_uart_port[UART0]	/* default setting - might be wrong TODO */
#define CONSOLE_PORT UART0
#endif
#endif

#ifndef CONSOLE_PORT
#define CONSOLE_PORT UARTDBG
#endif

#if defined(CONFIG_PM) && !defined(CONFIG_PM_SERIAL_ACTIVITY)
#define CONFIG_PM_SERIAL_ACTIVITY 10
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

#ifdef CONFIG_PM
static void up_pm_notify(struct pm_callback_s *cb, enum pm_state_e pmstate);
static int up_pm_prepare(struct pm_callback_s *cb, enum pm_state_e pmstate);
#endif
static int up_setup(struct uart_dev_s *dev);
static void up_shutdown(struct uart_dev_s *dev);
static int up_attach(struct uart_dev_s *dev);
static void up_detach(struct uart_dev_s *dev);
static int up_interrupt(int irq, void *context);
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
static char g_uart1rxbuffer[CONFIG_UART0_RXBUFSIZE];
static char g_uart1txbuffer[CONFIG_UART0_TXBUFSIZE];
#endif

#ifdef CONFIG_S5J_UART2
static char g_uart2rxbuffer[CONFIG_UART0_RXBUFSIZE];
static char g_uart2txbuffer[CONFIG_UART0_TXBUFSIZE];
#endif

#ifdef CONFIG_S5J_UART3
static char g_uart3rxbuffer[CONFIG_UART0_RXBUFSIZE];
static char g_uart3txbuffer[CONFIG_UART0_TXBUFSIZE];
#endif

/* This describes the state of the Stellaris uart0 port. */

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

/* This describes the state of the Stellaris uart1 port. */
static const int baudrate[] = {
	[BAUD_9600] = 9600,
	[BAUD_14400] = 14400,
	[BAUD_38400] = 38400,
	[BAUD_57600] = 57600,
	[BAUD_115200] = 115200,
	[BAUD_230400] = 230400,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

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
 *
 ****************************************************************************/

#ifdef CONFIG_PM
static void up_pm_notify(struct pm_callback_s *cb, enum pm_state_e pmstate)
{
	/* printf("up_pm_notify %d\n", pmstate); */
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
 ****************************************************************************/

#ifdef CONFIG_PM
static int up_pm_prepare(struct pm_callback_s *cb, enum pm_state_e pmstate)
{
	/* Logic to prepare for a reduced power state goes here. */

	/* printf("up_pm_prepare %d\n", pmstate); */

	return OK;
}
#endif

static void up_uart_set_initial_configure(uint32_t uBase)
{
	HW_REG32(uBase, UART_LCON) = 0x3;
	HW_REG32(uBase, UART_CON) = 0x3045;
	HW_REG32(uBase, UART_FCON) = 0x1;

	HW_REG32(uBase, UART_BRDIV) = 0x2;
	HW_REG32(uBase, UART_FRACVAL) = 0x8;
}

/*  UART0/1 supported  */
void up_uart_set_gpio(UART_CHANNEL eCh, bool bEnable)
{
	s32 gpio_rxd, gpio_txd;

	if (eCh == UARTDBG) {
		HW_REG32(0x80040000, 0x160) = 0x22;	//GPA3_CON Xdebug_TXD, Xdebug_RXD
		HW_REG32(0x80040000, 0x168) = 0x00;	// PULLUP/DOWN disabled
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

static void up_uart_set_baudrate(uint32_t uBase, u32 nBaudrate, u32 nClock)
{
	float fDiv;
	float fFrac;

	fDiv = ((float)nClock / (float)(nBaudrate * 16)) - 1.0;
	fFrac = (u32)(((fDiv - (s32)fDiv) * 16));

	HW_REG32(uBase, UART_BRDIV) = (u32)fDiv;
	HW_REG32(uBase, UART_FRACVAL) = (u32)fFrac;
}

static void up_uart_set_infrared_mode(uint32_t uBase, bool bEnable)
{
	if (bEnable) {
		HW_REG32(uBase, UART_LCON) |= (0x1 << 6);
	} else {
		HW_REG32(uBase, UART_LCON) &= ~(0x1 << 6);
	}
}

static void up_uart_set_parity_mode(uint32_t uBase, UART_PARITY_MODE eParityMode)
{
	HW_REG32(uBase, UART_LCON) &= ~(0x7 << 3);
	HW_REG32(uBase, UART_LCON) |= (eParityMode << 3);
}

static void up_uart_set_stop_bit(uint32_t uBase, UART_STOP_BIT eStopBit)
{
	HW_REG32(uBase, UART_LCON) &= ~(0x1 << 2);
	HW_REG32(uBase, UART_LCON) |= (eStopBit << 2);
}

static void up_uart_set_word_length(uint32_t uBase, UART_WORD_LENGTH eWordLen)
{
	HW_REG32(uBase, UART_LCON) &= ~(0x3 << 0);
	HW_REG32(uBase, UART_LCON) |= (eWordLen << 0);
}

static void up_uart_set_loopback(uint32_t uBase, bool bEnable)
{
	if (bEnable) {
		HW_REG32(uBase, UART_CON) |= (0x1 << 5);
	} else {
		HW_REG32(uBase, UART_CON) &= ~(0x1 << 5);
	}
}

static void up_uart_set_rx_timeout_interval(uint32_t uBase, u32 nTime)
{
	HW_REG32(uBase, UART_CON) &= ~(0xF << 12);
	HW_REG32(uBase, UART_CON) |= ((nTime & 0xF) << 12);
}

static void up_uart_set_rx_timeout(uint32_t uBase, bool bEnable)
{
	if (bEnable) {
		HW_REG32(uBase, UART_CON) |= (0x1 << 7);
	} else {
		HW_REG32(uBase, UART_CON) &= ~(0x1 << 7);
	}
}

static void up_uart_set_rx_timeout_with_empty_rx_fifo(uint32_t uBase, bool bEnable)
{
	if (bEnable) {
		HW_REG32(uBase, UART_CON) |= (0x1 << 11);
	} else {
		HW_REG32(uBase, UART_CON) &= ~(0x1 << 11);
	}
}

static void up_uart_set_rx_timeout_suspend_dma(uint32_t uBase, bool bEnable)
{
	if (bEnable) {
		HW_REG32(uBase, UART_CON) |= (0x1 << 10);
	} else {
		HW_REG32(uBase, UART_CON) &= ~(0x1 << 10);
	}
}

static void up_uart_set_fifo_mode(uint32_t uBase, bool bEnable)
{
	HW_REG32(uBase, UART_FCON) |= (0x3 << 1);

	if (bEnable) {
		HW_REG32(uBase, UART_FCON) |= (0x1 << 0);
	} else {
		HW_REG32(uBase, UART_FCON) &= ~(0x1 << 0);
	}
}

static void up_uart_set_tx_trigger_level(uint32_t uBase, UART_TRIGGER_LEVEL eTriggerLevel)
{
	HW_REG32(uBase, UART_FCON) &= ~(0x7 << 8);
	HW_REG32(uBase, UART_FCON) |= (eTriggerLevel << 8);
}

static void up_uart_set_rx_trigger_level(uint32_t uBase, UART_TRIGGER_LEVEL eTriggerLevel)
{
	HW_REG32(uBase, UART_FCON) &= ~(0x7 << 4);
	HW_REG32(uBase, UART_FCON) |= (eTriggerLevel << 4);
}

static void up_uart_set_rts_trigger_level(uint32_t uBase, UART_RTS_TRIGGER_LEVEL eRTSTriggerLevel)
{
	HW_REG32(uBase, UART_MCON) &= ~(0x7 << 5);
	HW_REG32(uBase, UART_MCON) |= (eRTSTriggerLevel << 5);
}

static void up_uart_set_tx_dma_burst_size(uint32_t uBase, UART_BURST_LENGTH eBurstLength)
{
	HW_REG32(uBase, UART_CON) &= ~(0x7 << 20);
	HW_REG32(uBase, UART_CON) |= (eBurstLength << 20);
}

static void up_uart_set_rx_dma_burst_size(uint32_t uBase, UART_BURST_LENGTH eBurstLength)
{
	HW_REG32(uBase, UART_CON) &= ~(0x7 << 16);
	HW_REG32(uBase, UART_CON) |= (eBurstLength << 16);
}

static void up_uart_clear_interrupt_status(uint32_t uBase, UART_INTERRUPT eInt)
{
	HW_REG32(uBase, UART_INTP) |= eInt;
}

/****************************************************************************
 * Name: up_serialin
 ****************************************************************************/

static inline uint32_t up_serialin(struct up_dev_s *priv, int offset)
{
	return HW_REG32(priv->uartbase, offset);
}

/****************************************************************************
 * Name: up_serialout
 ****************************************************************************/

static inline void up_serialout(struct up_dev_s *priv, int offset, uint32_t value)
{
	HW_REG32(priv->uartbase, offset) = value;
	/*  WHY is it here???  up_lowputc((char)value); */
}

/****************************************************************************
 * Name: up_disableuartint
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
 * Name: up_waittxnotfull
 ****************************************************************************/

#ifdef HAVE_SERIAL_CONSOLE
/*  STRANGE FUNCTION ... Ivan  */
static inline void up_waittxnotfull(struct up_dev_s *priv)
{
	volatile int tmp;

	/* Limit how long we will wait for the TX available condition */

	for (tmp = 1000; tmp > 0; tmp--) {
		/* Check Tx FIFO is full */

		if (!(HW_REG32(priv->uartbase, UART_FSTAT) & (0x1 << 24))) {
			break;    /* The Tx FIFO is not full... return */
		}

	}

	/* If we get here, then the wait has timed out and the Tx FIFO remains
	 * full.
	 */
}
#endif

static inline void up_uart_set_filt(struct up_dev_s *priv, int enable, int cycle)
{
	//UART_CHANNEL ech = priv->eCh;
}

/****************************************************************************
 * Name: up_uart_initialize
 *
 * Description:
 *   Initialize uart port.
 *
 ****************************************************************************/

void up_uart_initialize(struct up_dev_s *priv, u32 nBaudrate, u32 nClock)
{
	UART_CHANNEL eCh;
	uint32_t uBase = priv->uartbase;

	eCh = priv->eCh;

	up_uart_set_gpio(eCh, TRUE);
	up_uart_set_initial_configure(uBase);

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

	uart_init((uint8_t)priv->eCh, 1);

#ifdef CONFIG_SUPPRESS_UART_CONFIG
	/*  Do it here - Ivan  */
#endif
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

	ret = irq_attach(priv->irq, up_interrupt);
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
 *   closed normally just before the shutdown method is called.  The exception is
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

static int up_interrupt(int irq, void *context)
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

		mis = (HW_REG32(priv->uartbase, UART_INTP) & 0xF);
		HW_REG32(priv->uartbase, UART_INTP) = mis;

		/* Handle incoming, receive bytes (with or without timeout) */

		if ((mis & RX_INT) != 0) {

#if defined(CONFIG_PM) && CONFIG_PM_SERIAL_ACTIVITY > 0
			/*          pm_activity(CONFIG_PM_SERIAL_ACTIVITY); */
#endif

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
 *   Do we haev IOCTLs ??? Ivan
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
		/* T20 does not support flowcontrol. */
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
 ****************************************************************************/

static int up_receive(struct uart_dev_s *dev, uint32_t *status)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	*status = 0;				/*  WHAT KIND OF STATUS DO YOU NEED???  */
	int empty;
	do {
		empty = !(__raw_readl(priv->uartbase + UART_FSTAT) & 0x1FF);
	} while (empty);
	return (HW_REG32(priv->uartbase, UART_RXH) & 0xFF);
}

/****************************************************************************
 * Name: up_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts
 *
 ****************************************************************************/

static void up_rxint(struct uart_dev_s *dev, bool enable)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	if (enable) {
		/* Receive an interrupt when their is anything in the Rx FIFO (or an Rx
		 * timeout occurs.
		 */

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
 ****************************************************************************/

static bool up_rxavailable(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;

	return ((HW_REG32(priv->uartbase, UART_FSTAT) & 0x1FF) != 0);
}

/****************************************************************************
 * Name: up_send
 *
 * Description:
 *   This method will send one byte on the UART
 *
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
 *   Return true if the tranmsit fifo is not full
 *
 ****************************************************************************/

static bool up_txready(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	/*   return ((HW_REG32(priv->uartbase, UART_FSTAT) & (0x1 << 24)) == 0); */
	return ((HW_REG32(priv->uartbase, UART_FSTAT) & (0x1FF << 16)) == 0);

}

/****************************************************************************
 * Name: up_txempty
 *
 * Description:
 *   Return true if the transmit fifo is empty
 *
 ****************************************************************************/

static bool up_txempty(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	return ((HW_REG32(priv->uartbase, UART_FSTAT) & (0x1FF << 16)) == 0);
}

/**
 *
 * @brief send data to uart
 * @param UART_CHANNEL eCh : Uart Channel
 * @param char cData : one charactor ASCII code
 *
 * @note  put the data to UART TXFIFO.
 *      data in TXFIFO will be send shortly.
 *
 */
static void uart_send_data(UART_CHANNEL eCh, char cData)
{
	struct up_dev_s *priv;
	priv = g_uart_port[CONSOLE_PORT].priv;
	while (!up_txempty(&g_uart_port[CONSOLE_PORT])) ;
	HW_REG32(priv->uartbase, UART_TXH) = cData;
#if 0
	if (eCh != UART0) {
		return;
	}
	priv = g_uart_port[UART0].priv;
	while (!up_txempty(&g_uart_port[UART0])) ;
	HW_REG32(priv->uartbase, UART_TXH) = cData;
#endif
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
 ****************************************************************************/
/**
 *
 * @brief Low level UART initialization
 * @note  Performs the low level UART initialization early in
 *      debug so that the serial console will be available
 *      during bootup.  This must be called before up_serialinit.
 */
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
 ****************************************************************************/

/**
 *
 * @brief Register serial UART ports
 * @note  Register serial console and serial ports.
 *      This assumes that up_earlyserialinit was called previously.'
 *
 */

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

/**
 *
 * @brief Register serial UART1 ports
 * @note  Register serial UART1 ports.
 *      This assumes that up_earlyserialinit was called previously.
 *      This function for only UART1 port register
 */
/* NEVER USED
void uartdrv_register(void)
{
#ifdef CONFIG_S5J_UART0
  lldbg("uart0 driver register\n");
  (void)uart_register("/dev/ttyS0", &g_uart_port[UART0]);
#endif
#ifdef CONFIG_S5J_UART1
  lldbg("uart1 driver register\n");
  (void)uart_register("/dev/ttyS1", &g_uart_port[UART1]);
#endif

#ifdef CONFIG_S5J_UART2
  lldbg("uart2 driver register\n");
  (void)uart_register("/dev/ttyS2", &g_uart_port[UART2]);
#endif

#ifdef CONFIG_S5J_UART3
  lldbg("uart3 driver register\n");
  (void)uart_register("/dev/ttyS3", &g_uart_port[UART3]);
#endif

#ifdef CONFIG_S5J_UARTDBG
  lldbg("uart_dbg driver register\n");
  (void)uart_register("/dev/ttyDBG", &g_uart_port[UARTDBG]);
#endif
}
*/

/****************************************************************************
 * Name: up_putc
 *
 * Description:
 *   Provide priority, low-level access to support OS debug  writes
 *
 ****************************************************************************/
/**
 *
 * @brief   OS debug write charactor
 * @param    int ch: one charactor ASCII code
 * @return     inputed charactor ASCII code
 * @note     Provide priority, low-level access to support OS debug writes
 */

int up_putc(int ch)
{
	//__asm__ __volatile__ ("b .");
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

int up_getc()
{
#ifdef HAVE_SERIAL_CONSOLE
	uint8_t ch;
	if (uart_getchar_nb(CONSOLE_PORT, &ch)) {
		return ch;
	} else {
		return -1;
	}
#else
	return -1;
#endif
}

/**
 *
 * @brief   low level uart send one charactor function
 * @param   char ch: one charactor ASCII code
 * @note    send data to UART0.
 */

void up_lowputc(char ch)
{
	/* Report serial activity to the power management logic */

	uart_send_data(CONSOLE_PORT, ch);
}

/**
 *
 * @brief   initialize UART channel
 * @param   uint8_t uart: uart channel
 * @param   uint8_t interrupts: enable interrupts bit (1 : enable, 0 : disable)
 * @note    UART initialization function. at now, only UART0 and UART1 is able to be initialized
 */

void uart_init(uint8_t uart, uint8_t interrupts)
{
	/* Initialize UART */
	int clk;
	struct up_dev_s *priv = g_uart_port[uart].priv;
	int baud_rate = priv->baud;

#ifdef CONFIG_S5J_UART_CLOCK_FROM_WPLL
	cal_clk_enable(m1_clkcmu_uart);
	cal_clk_setrate(m1_clkcmu_uart, 1);	/* 80Mhz */
#else
	cal_clk_enable(m1_clkcmu_uart);
	cal_clk_setrate(m1_clkcmu_uart, 0);	/* 26Mhz */
#endif
	clk = cal_clk_getrate(m1_clkcmu_uart);

	up_uart_initialize(priv, baud_rate, clk);
	if (!interrupts) {
		up_uart_disable_interrupt(priv->uartbase, ALL_INT);
	}
}

void up_lowconsole_init(void)
{
	uart_init(CONSOLE_PORT, 1);
}

/**
 *
 * @brief   Put charactor to UART (busy waiting)
 * @param   uint8_t uart: uart channel
 * @param   int c: One ASCII charactor
 * @note    put charactor to UART.
 *        if tx fifo is full state, wait a moment until fifo is empty.
 *        And then, write one charactor
 */

void uart_putchar_wait(uint8_t uart, int c)
{
	/* if tx fifo is full state, wait, and write one char */
	while (!up_txempty(&g_uart_port[uart])) ;

	up_send(&g_uart_port[uart], c);
}

/**
 *
 * @brief   Put charactor to UART
 * @param   uint8_t uart: uart channel
 * @param   int c: One ASCII charactor
 * @return    == 1: put one charactor to FIFO
 *        == 0: FIFO is not empty.
 * @note    put charactor to UART.
 *        if tx fifo is empty, write one charactor.
 *        this function does not wait for fifo.
 */

int uart_putchar_nb(uint8_t uart, int c)
{
	struct uart_dev_s *dev = &g_uart_port[uart];
	if (up_txempty(dev)) {
		up_send(dev, c);
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
 *
 * @brief   get charactor from UART
 * @param   uint8_t uart: uart channel
 * @param   uint8_t *ch: one ASCII charactor container
 * @return    == 1: Get one charactor from RXFIFO
 *        == 0: FIFO is empty.
 * @note    get charactor from UART.
 *        if rx fifo is not empty, get one charactor into *ch.
 *        this function does not wait for fifo.
 */

int uart_getchar_nb(uint8_t uart, uint8_t *ch)
{
	struct uart_dev_s *dev = &g_uart_port[uart];
	uint32_t state;
	if (up_rxavailable(dev)) {
		*ch = up_receive(dev, &state);
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
 *
 * @brief   check the UART is transmitting
 * @param   uint8_t uart: uart channel
 * @return    == 1: UART is transmitting now. TX FIFO has some data.
 *        == 0: UART is idle. FIFO is empty
 * @note    check the UART is transmitting.
 */

int uart_tx_busy(uint8_t uart)
{
	if (!up_txready(&g_uart_port[uart])) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
 *
 * @brief   Set UART's Baudrate
 * @param   uint8_t uart: uart channel
 * @param   enum uart_boadrate bdrt: selected baudrate
 * @return    == -1: wrong baudrate.
 *        == 0: set baudrate to UART
 * @note    Set UART baudrate.
 */

int uart_baudrate(uint8_t uart, enum uart_baudrate bdrt)
{
	u32 nBaudrate;
	u32 uBase = ((struct up_dev_s *)g_uart_port[uart].priv)->uartbase;
	if (bdrt < 0 || bdrt >= ARRAY_SIZE(baudrate)) {
		return -1;
	}

	nBaudrate = baudrate[bdrt];
	up_uart_set_baudrate(uBase, nBaudrate, 133000000);
	return 0;

}

/**
 *
 * @brief   Set UART's interrupt
 * @param   uint8_t uart: uart channel
 * @param   UART_INTERRUPT irq: kinds of irq type
 * @param   int on: enable bit (1 : enable)
 * @note    Set UART's interrupt
 */

void uart_irq_enable(uint8_t uart, UART_INTERRUPT irq, int on)
{
	/* UART_INTERRUPT is same as enum uart_irq */
	struct up_dev_s *priv = (struct up_dev_s *)g_uart_port[uart].priv;
	if (on) {
		up_uart_enable_interrupt(priv->uartbase, irq);
	} else {
		up_uart_disable_interrupt(priv->uartbase, irq);
	}
}

/**
 *
 * @brief   polling UART rxfifo (busy waiting)
 * @param   uint8_t uart: uart channel
 * @note    polling UART rxfifo.
 *        if rx fifo is empty, wait a moment until rxfifo is available.
 */

void uart_poll(uint8_t uart)
{
	/* What does that mean "POLL"? rx poll? */
	struct uart_dev_s *dev = &g_uart_port[uart];
	while (!up_rxavailable(dev)) ;

	return;

}
