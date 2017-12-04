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

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/serial/serial.h>
#include <tinyara/pm/pm.h>

#ifdef CONFIG_SERIAL_TERMIOS
#include <termios.h>
#endif

#include "s5j_gpio.h"
#include "s5j_clock.h"
#include "up_arch.h"
#include "up_internal.h"

#include "chip/s5jt200_uart.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/*
 * If we are not using the serial driver for the console, then we
 * still must provide some minimal implementation of up_putc.
 */

#undef TTYS0_DEV
#undef TTYS1_DEV
#undef TTYS2_DEV
#undef TTYS3_DEV
#undef TTYS4_DEV

#undef UART0_ASSIGNED
#undef UART1_ASSIGNED
#undef UART2_ASSIGNED
#undef UART3_ASSIGNED
#undef UART4_ASSIGNED

/* Which UART with be ttyS0/console and which ttyS1? ttyS2? ... ttyS4 */

/* First pick the console and ttys0. This could be any of UART0-4 */
#if defined(CONFIG_UART0_SERIAL_CONSOLE)
#define CONSOLE_DEV		g_uart0port		/* UART0 is console */
#define TTYS0_DEV		g_uart0port		/* UART0 is ttyS0 */
#define UART0_ASSIGNED	1
#define HAVE_SERIAL_CONSOLE
#elif defined(CONFIG_UART1_SERIAL_CONSOLE)
#define CONSOLE_DEV		g_uart1port		/* UART1 is console */
#define TTYS0_DEV		g_uart1port		/* UART1 is ttyS0 */
#define UART1_ASSIGNED	1
#define HAVE_SERIAL_CONSOLE
#elif defined(CONFIG_UART2_SERIAL_CONSOLE)
#define CONSOLE_DEV		g_uart2port		/* UART2 is console */
#define TTYS0_DEV		g_uart2port		/* UART2 is ttyS0 */
#define UART2_ASSIGNED	1
#define HAVE_SERIAL_CONSOLE
#elif defined(CONFIG_UART3_SERIAL_CONSOLE)
#define CONSOLE_DEV		g_uart3port		/* UART3 is console */
#define TTYS0_DEV		g_uart3port		/* UART3 is ttyS0 */
#define UART3_ASSIGNED	1
#define HAVE_SERIAL_CONSOLE
#elif defined(CONFIG_UART4_SERIAL_CONSOLE)
#define CONSOLE_DEV		g_uart4port		/* UART4 is console */
#define TTYS0_DEV		g_uart4port		/* UART4 is ttyS0 */
#define UART4_ASSIGNED	1
#define HAVE_SERIAL_CONSOLE
#else
#undef CONSOLE_DEV						/* No console */
#if defined(CONFIG_S5J_UART0)
#define TTYS0_DEV		g_uart0port		/* UART0 is ttyS0 */
#define UART0_ASSIGNED	1
#elif defined(CONFIG_S5J_UART1)
#define TTYS0_DEV		g_uart1port		/* UART1 is ttyS0 */
#define UART1_ASSIGNED	1
#elif defined(CONFIG_S5J_UART2)
#define TTYS0_DEV		g_uart2port		/* UART2 is ttyS0 */
#define UART2_ASSIGNED	1
#elif defined(CONFIG_S5J_UART3)
#define TTYS0_DEV		g_uart3port		/* UART3 is ttyS0 */
#define UART3_ASSIGNED	1
#elif defined(CONFIG_S5J_UART4)
#define TTYS0_DEV		g_uart4port		/* UART4 is ttyS0 */
#define UART4_ASSIGNED	1
#endif
#endif

/* Pick ttyS1. This could be any of UART0-4 excluding the console UART. */
#if defined(CONFIG_S5J_UART0) && !defined(UART0_ASSIGNED)
#define TTYS1_DEV		g_uart0port		/* UART0 is ttyS1 */
#define UART0_ASSIGNED	1
#elif defined(CONFIG_S5J_UART1) && !defined(UART1_ASSIGNED)
#define TTYS1_DEV		g_uart1port		/* UART1 is ttyS1 */
#define UART1_ASSIGNED	1
#elif defined(CONFIG_S5J_UART2) && !defined(UART2_ASSIGNED)
#define TTYS1_DEV		g_uart2port		/* UART2 is ttyS1 */
#define UART2_ASSIGNED	1
#elif defined(CONFIG_S5J_UART3) && !defined(UART3_ASSIGNED)
#define TTYS1_DEV		g_uart3port		/* UART3 is ttyS1 */
#define UART3_ASSIGNED	1
#elif defined(CONFIG_S5J_UART4) && !defined(UART4_ASSIGNED)
#define TTYS1_DEV		g_uart4port		/* UART4 is ttyS1 */
#define UART4_ASSIGNED	1
#endif

/*
 * Pick ttyS2. This could be one of UART1-4. It can't be UART0 because that
 * was either assigned as ttyS0 or ttyS1. One of these could also be the
 * console
 */
#if defined(CONFIG_S5J_UART1) && !defined(UART1_ASSIGNED)
#define TTYS2_DEV		g_uart1port		/* UART1 is ttyS2 */
#define UART1_ASSIGNED	1
#elif defined(CONFIG_S5J_UART2) && !defined(UART2_ASSIGNED)
#define TTYS2_DEV		g_uart2port		/* UART2 is ttyS2 */
#define UART2_ASSIGNED	1
#elif defined(CONFIG_S5J_UART3) && !defined(UART3_ASSIGNED)
#define TTYS2_DEV		g_uart3port		/* UART3 is ttyS2 */
#define UART3_ASSIGNED	1
#elif defined(CONFIG_S5J_UART4) && !defined(UART4_ASSIGNED)
#define TTYS2_DEV		g_uart4port		/* UART4 is ttyS2 */
#define UART4_ASSIGNED	1
#endif

/*
 * Pick ttyS3. This could be one of UART2-4. It can't be UART0-1; those have
 * already been assigned to ttyS0, 1, or 2. One of UART2-4 could also be the
 * console.
 */
#if defined(CONFIG_S5J_UART2) && !defined(UART2_ASSIGNED)
#define TTYS3_DEV		g_uart2port		/* UART2 is ttyS3 */
#define UART2_ASSIGNED	1
#elif defined(CONFIG_S5J_UART3) && !defined(UART3_ASSIGNED)
#define TTYS3_DEV		g_uart3port		/* UART3 is ttyS3 */
#define UART3_ASSIGNED	1
#elif defined(CONFIG_S5J_UART4) && !defined(UART4_ASSIGNED)
#define TTYS3_DEV		g_uart4port		/* UART4 is ttyS3 */
#define UART4_ASSIGNED	1
#endif

/*
 * Pick ttyS4. This could be one of UART3-4. It can't be UART0-2; those have
 * already been assigned to ttyS0, 1, 2, or 3. One of UART3-4 could also be
 * the console.
 */
#if defined(CONFIG_S5J_UART3) && !defined(UART3_ASSIGNED)
#define TTYS4_DEV		g_uart3port		/* UART3 is ttyS4 */
#define UART3_ASSIGNED	1
#elif defined(CONFIG_S5J_UART4) && !defined(UART4_ASSIGNED)
#define TTYS4_DEV		g_uart4port		/* UART4 is ttyS4 */
#define UART4_ASSIGNED	1
#endif

/* Power management definitions */
#if defined(CONFIG_PM) && !defined(CONFIG_PM_SERIAL_ACTIVITY)
#define CONFIG_PM_SERIAL_ACTIVITY	10
#define PM_IDLE_DOMAIN				0	/* Revisit */
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct up_dev_s {
	uint32_t	uartbase;	/* Base address of UART registers */
	uint32_t	baud;		/* Configured baud */
	uint8_t		irq;		/* IRQ associated with this UART */
	uint8_t		parity;		/* 0=none, 4=odd, 5=even */
	uint8_t		bits;		/* Number of bits (5, 6, 7 or 8) */
	uint8_t		stopbits2;	/* true: Configure with 2 stop bits instead of 1 */
	uint16_t	rxd;
	uint16_t	txd;
	uint16_t	rts;
	uint16_t	cts;
	uint32_t	pclk;
	uint32_t	extclk;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static inline uint32_t uart_getreg8(struct up_dev_s *priv, int offset)
{
	return getreg8(priv->uartbase + offset);
}

static inline void uart_putreg8(struct up_dev_s *priv, int offset,
				uint32_t value)
{
	putreg8(value, priv->uartbase + offset);
}

static inline uint32_t uart_getreg32(struct up_dev_s *priv, int offset)
{
	return getreg32(priv->uartbase + offset);
}

static inline void uart_putreg32(struct up_dev_s *priv, int offset,
				uint32_t value)
{
	putreg32(value, priv->uartbase + offset);
}

static inline void uart_modifyreg32(struct up_dev_s *priv, int offset,
				uint32_t clearbits, uint32_t setbits)
{
	modifyreg32(priv->uartbase + offset, clearbits, setbits);
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
	uart_dev_t *dev = (uart_dev_t *)arg;
	uint32_t uintp = uart_getreg32(dev->priv, S5J_UART_UINTP_OFFSET);

#if defined(CONFIG_PM) && CONFIG_PM_SERIAL_ACTIVITY > 0
	/* Report serial activity to the power management logic */
	pm_activity(PM_IDLE_DOMAIN, CONFIG_PM_SERIAL_ACTIVITY);
#endif

	if (uintp & UART_UINTP_TXD) {
		uart_xmitchars(dev);
	}

	if (uintp & UART_UINTP_RXD) {
		uart_recvchars(dev);
	}

	/* clear interrupt pending bits */
	uart_putreg32(dev->priv, S5J_UART_UINTP_OFFSET, uintp);

	return OK;
}

/****************************************************************************
 * Name: up_configure
 *
 * Description:
 *   Configure the UART baud, bits, parity, etc.
 *
 ****************************************************************************/
static void up_configure(struct up_dev_s *priv)
{
#if !defined(CONFIG_SUPPRESS_UART_CONFIG)
	float div;
	uint32_t regval;

	/* wait until Tx FIFO gets empty not to disturb on-going transmission */
	do {
		regval = uart_getreg32(priv, S5J_UART_UTRSTAT_OFFSET);
	} while ((regval & UART_UTRSTAT_TX_BUF_EMPTY) == 0);

	/* Ensure that UART clock is supplied... */

	/* UMCON */
	if (priv->rts || priv->cts) {
		/* nRTS and nCTS are controlled by hardware */
		uart_putreg32(priv, S5J_UART_UMCON_OFFSET, UART_UMCON_AFC_ENABLE);
	}

	/* ULCON */
	regval = UART_ULCON_SAMPLING_16 | UART_ULCON_INFRARED_NORMAL;

	switch (priv->bits) {
	case 5:
		regval |= UART_ULCON_DATABITS_5BITS;
		break;
	case 6:
		regval |= UART_ULCON_DATABITS_6BITS;
		break;
	case 7:
		regval |= UART_ULCON_DATABITS_7BITS;
		break;
	default:
	case 8:
		regval |= UART_ULCON_DATABITS_8BITS;
		break;
	}

	if (priv->parity == 1) {
		regval |= UART_ULCON_PARITY_ODD;
	} else if (priv->parity == 2) {
		regval |= UART_ULCON_PARITY_EVEN;
	}

	if (priv->stopbits2) {
		regval |= UART_ULCON_STOPBITS_2BITS;
	}

	uart_putreg32(priv, S5J_UART_ULCON_OFFSET, regval);

	/* UCON */
	regval = UART_UCON_RX_TOUT_32FRAMES |
			UART_UCON_TX_INTTYPE_LEVEL | UART_UCON_RX_INTTYPE_LEVEL |
			UART_UCON_RX_TOUTINT_ENABLE | UART_UCON_RX_ERRINT_DISABLE |
			UART_UCON_TX_MODE_IRQPOLL | UART_UCON_RX_MODE_IRQPOLL;

	uart_putreg32(priv, S5J_UART_UCON_OFFSET, regval);

	/* UBRDIV and UFRACVAL */
	div = ((float)s5j_clk_get_rate(CLK_SPL_UART) / (priv->baud * 16)) - 1;

	uart_putreg32(priv, S5J_UART_UBRDIV_OFFSET, (uint32_t)div);
	uart_putreg32(priv, S5J_UART_UFRACVAL_OFFSET, ((div - (uint32_t)div) * 16));
#endif /* CONFIG_SUPPRESS_UART_CONFIG */
}

/****************************************************************************
 * Name: up_setup
 *
 * Description:
 *   Configure the UART baud, bits, parity, etc.
 *   This also disables all interrupts and initializes FIFO mode.
 *   This method is called the first time that the serial port is
 *   opened.
 *
 ****************************************************************************/
static int up_setup(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;

	/* Configure pinmux to set function for RXD/TXD/RTS/CTS pins */
	s5j_configgpio(priv->rxd);
	s5j_configgpio(priv->txd);
#if defined(CONFIG_S5J_UART_FLOWCONTROL)
	if (priv->rts) {
		s5j_configgpio(priv->rts);
	}

	if (priv->cts) {
		s5j_configgpio(priv->cts);
	}
#endif

	s5j_clk_enable(priv->pclk);
	s5j_clk_enable(priv->extclk);

	/* Configure the UART baud, bits, parity, etc. */
	up_configure(priv);

	/* Mask all interrupts; will be enabled by upper-half */
	uart_putreg32(priv, S5J_UART_UINTM_OFFSET,
					UART_UINTM_MODEM_MASK | UART_UINTM_TXD_MASK |
					UART_UINTM_ERROR_MASK | UART_UINTM_RXD_MASK);

	/* Reset TX and RX FIFO and enable FIFO mode */
	uart_putreg32(priv, S5J_UART_UFCON_OFFSET,
					UART_UFCON_FIFO_ENABLE |
					UART_UFCON_TX_FIFO_RESET |
					UART_UFCON_RX_FIFO_RESET |
					UART_UFCON_TX_FIFO_TRIG_4BYTES |
					UART_UFCON_RX_FIFO_TRIG_4BYTES);

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

	/* Disable all interrupts */
	uart_modifyreg32(priv, S5J_UART_UINTM_OFFSET, 0,
					UART_UINTM_TXD_MASK | UART_UINTM_RXD_MASK);

	s5j_clk_disable(priv->pclk);
	s5j_clk_disable(priv->extclk);

	s5j_unconfiggpio(priv->rxd);
	s5j_unconfiggpio(priv->txd);

#if defined(CONFIG_S5J_UART_FLOWCONTROL)
	if (priv->rts) {
		s5j_unconfiggpio(priv->rts);
	}

	if (priv->cts) {
		s5j_unconfiggpio(priv->cts);
	}
#endif
}

/****************************************************************************
 * Name: up_attach
 *
 * Description:
 *   Configure the UART to operation in interrupt driven mode. This method is
 *   called when the serial port is opened. Normally, this is just after the
 *   the setup() method is called, however, the serial console may operate in
 *   a non-interrupt driven mode during the boot phase.
 *
 *   RX and TX interrupts are not enabled when by the attach method (unless
 *   the hardware supports multiple levels of interrupt enabling). The RX and
 *   TX interrupts are not enabled until the txint() and rxint() methods are
 *   called.
 *
 ****************************************************************************/
static int up_attach(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	int ret;

	/* Register interrupt vector for the UART */
	ret = irq_attach(priv->irq, up_interrupt, dev);
	if (ret == OK) {
		/*
		 * Enable the interrupt (RX and TX interrupts are still disabled
		 * until txint() and rxint() are called.
		 */
		up_enable_irq(priv->irq);
	}

	return ret;
}

/****************************************************************************
 * Name: up_detach
 *
 * Description:
 *   Detach UART interrupts. This method is called when the serial port is
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
 * Name: up_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method
 *
 ****************************************************************************/
static int up_ioctl(struct file *filep, int cmd, unsigned long arg)
{
#if defined(CONFIG_SERIAL_TERMIOS)
	struct inode *inode = filep->f_inode;
	struct uart_dev_s *dev = inode->i_private;
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	int ret = OK;
	struct termios *termiosp = (struct termios *)arg;

	switch (cmd) {
	case TCGETS:
		if (!termiosp) {
			return -EINVAL;
		}

		cfsetispeed(termiosp, priv->baud);

		termiosp->c_cflag = 0;

		if (priv->parity) {
			termiosp->c_cflag |= PARENB;
			if (priv->parity == 1) {
				termiosp->c_cflag |= PARODD;
			}
		}

		if (priv->stopbits2) {
			termiosp->c_cflag |= CSTOPB;
		}

		termiosp->c_cflag |= CS5 + (priv->bits - 5);
		break;

	case TCSETS:
		if (!termiosp) {
			return -EINVAL;
		}

		priv->bits = 5 + (termiosp->c_cflag & CSIZE);

		priv->parity = 0;
		if (termiosp->c_cflag & PARENB) {
			if (termiosp->c_cflag & PARODD) {
				priv->parity = 1;
			} else {
				priv->parity = 2;
			}
		}

		priv->stopbits2 = !!(termiosp->c_cflag & CSTOPB);

		priv->baud = cfgetispeed(termiosp);

		up_configure(priv);
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
 *   Called (usually) from the interrupt level to receive one character from
 *   the UART. Error bits associated with the receipt are provided in the
 *   return 'status'.
 *
 ****************************************************************************/
static int up_receive(struct uart_dev_s *dev, uint32_t *status)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;

	/* Return the error information in the saved status */
	*status = 0;

	return uart_getreg8(priv, S5J_UART_URXH_OFFSET);
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
	uint32_t uintm = uart_getreg32(priv, S5J_UART_UINTM_OFFSET);

	if (enable) {
		uintm &= ~UART_UINTM_RXD_MASK;
	} else {
		uintm |= UART_UINTM_RXD_MASK;
	}

	uart_putreg32(priv, S5J_UART_UINTM_OFFSET, uintm);
}

/****************************************************************************
 * Name: up_rxavailable
 *
 * Description:
 *   Return true if the receive holding register is not empty
 *
 ****************************************************************************/
static bool up_rxavailable(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	uint32_t ufstat = uart_getreg32(priv, S5J_UART_UFSTAT_OFFSET);

	return !!(ufstat & (UART_UFSTAT_RX_FIFO_COUNT_MASK |
						UART_UFSTAT_RX_FIFO_FULL_MASK));
}

/****************************************************************************
 * Name: up_send
 *
 * Description:
 *   This method will send one byte on the UART.
 *
 ****************************************************************************/
static void up_send(struct uart_dev_s *dev, int ch)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;

	uart_putreg8(priv, S5J_UART_UTXH_OFFSET, ch);
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
	uint32_t uintm = uart_getreg32(priv, S5J_UART_UINTM_OFFSET);

	if (enable) {
		uintm &= ~UART_UINTM_TXD_MASK;
	} else {
		uintm |= UART_UINTM_TXD_MASK;
	}

	uart_putreg32(priv, S5J_UART_UINTM_OFFSET, uintm);
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
 *   Return true if the tx fifo is not full
 *
 ****************************************************************************/
static bool up_txready(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	uint32_t ufstat = uart_getreg32(priv, S5J_UART_UFSTAT_OFFSET);

	return !(ufstat & UART_UFSTAT_TX_FIFO_FULL);
}

/****************************************************************************
 * Name: up_txempty
 *
 * Description:
 *   Return true if the transmit holding and shift registers are empty.
 *
 ****************************************************************************/
static bool up_txempty(struct uart_dev_s *dev)
{
	struct up_dev_s *priv = (struct up_dev_s *)dev->priv;
	uint32_t utrstat = uart_getreg32(priv, S5J_UART_UTRSTAT_OFFSET);

	return !!(utrstat & UART_UTRSTAT_TX_BUF_EMPTY);
}

#ifdef CONFIG_PM
/****************************************************************************
 * Name: up_pm_notify
 *
 * Description:
 *   Notify the driver of new power state. This callback is called after
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
 *   consumption state when it returned OK to the prepare() call.
 *
 ****************************************************************************/
static void up_pm_notify(struct pm_callback_s *cb, int domain,
				enum pm_state_e pmstate)
{
	switch (pmstate) {
	case PM_NORMAL:
		/* Logic for PM_NORMAL goes here */
		break;

	case PM_IDLE:
		/* Logic for PM_IDLE goes here */
		break;

	case PM_STANDBY:
		/* Logic for PM_STANDBY goes here */
		break;

	case PM_SLEEP:
		/* Logic for PM_SLEEP goes here */
		break;

	default:
		/* Should not get here */
		break;
	}
}

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
static int up_pm_prepare(struct pm_callback_s *cb, int domain,
		enum pm_state_e pmstate)
{
	/* Logic to prepare for a reduced power state goes here. */
	return OK;
}
#endif /* CONFIG_PM */

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* UART operations structure */
static const struct uart_ops_s g_uart_ops = {
	.setup			= up_setup,
	.shutdown		= up_shutdown,
	.attach			= up_attach,
	.detach			= up_detach,
	.ioctl			= up_ioctl,
	.receive		= up_receive,
	.rxint			= up_rxint,
	.rxavailable	= up_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
	.rxflowcontrol	= NULL,
#endif
	.send			= up_send,
	.txint			= up_txint,
	.txready		= up_txready,
	.txempty		= up_txempty,
};

/* I/O buffers */
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

#ifdef CONFIG_S5J_UART4
static char g_uart4rxbuffer[CONFIG_UART4_RXBUFSIZE];
static char g_uart4txbuffer[CONFIG_UART4_TXBUFSIZE];
#endif

#ifdef CONFIG_S5J_UART0
static struct up_dev_s g_uart0priv = {
	.uartbase	= S5J_UART0_BASE,
	.baud		= CONFIG_UART0_BAUD,
	.irq		= S5J_IRQ_UART0,
	.parity		= CONFIG_UART0_PARITY,
	.bits		= CONFIG_UART0_BITS,
	.stopbits2	= CONFIG_UART0_2STOP,
	.rxd		= GPIO_UART0_RXD,
	.txd		= GPIO_UART0_TXD,
	.pclk		= CLK_GATE_UART0_PCLK,
	.extclk		= CLK_GATE_UART0_EXTCLK,
};

static uart_dev_t g_uart0port = {
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
};
#endif

#ifdef CONFIG_S5J_UART1
static struct up_dev_s g_uart1priv = {
	.uartbase	= S5J_UART1_BASE,
	.baud		= CONFIG_UART1_BAUD,
	.irq		= S5J_IRQ_UART1,
	.parity		= CONFIG_UART1_PARITY,
	.bits		= CONFIG_UART1_BITS,
	.stopbits2	= CONFIG_UART1_2STOP,
	.rxd		= GPIO_UART1_RXD,
	.txd		= GPIO_UART1_TXD,
	.pclk		= CLK_GATE_UART1_PCLK,
	.extclk		= CLK_GATE_UART1_EXTCLK,
};

static uart_dev_t g_uart1port = {
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
};
#endif

#ifdef CONFIG_S5J_UART2
static struct up_dev_s g_uart2priv = {
	.uartbase	= S5J_UART2_BASE,
	.baud		= CONFIG_UART2_BAUD,
	.irq		= S5J_IRQ_UART2,
	.parity		= CONFIG_UART2_PARITY,
	.bits		= CONFIG_UART2_BITS,
	.stopbits2	= CONFIG_UART2_2STOP,
	.rxd		= GPIO_UART2_RXD,
	.txd		= GPIO_UART2_TXD,
#ifdef CONFIG_S5J_UART2_FLOWCONTROL
	.rts		= GPIO_UART2_RTS,
	.cts		= GPIO_UART2_CTS,
#endif
	.pclk		= CLK_GATE_UART2_PCLK,
	.extclk		= CLK_GATE_UART2_EXTCLK,
};

static uart_dev_t g_uart2port = {
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
};
#endif

#ifdef CONFIG_S5J_UART3
static struct up_dev_s g_uart3priv = {
	.uartbase	= S5J_UART3_BASE,
	.baud		= CONFIG_UART3_BAUD,
	.irq		= S5J_IRQ_UART3,
	.parity		= CONFIG_UART3_PARITY,
	.bits		= CONFIG_UART3_BITS,
	.stopbits2	= CONFIG_UART3_2STOP,
	.rxd		= GPIO_UART3_RXD,
	.txd		= GPIO_UART3_TXD,
#ifdef CONFIG_S5J_UART3_FLOWCONTROL
	.rts		= GPIO_UART3_RTS,
	.cts		= GPIO_UART3_CTS,
#endif
	.pclk		= CLK_GATE_UART3_PCLK,
	.extclk		= CLK_GATE_UART3_EXTCLK,
};

static uart_dev_t g_uart3port = {
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
};
#endif

#ifdef CONFIG_S5J_UART4
static struct up_dev_s g_uart4priv = {
	.uartbase	= S5J_UART4_BASE,
	.baud		= CONFIG_UART4_BAUD,
	.irq		= S5J_IRQ_UART4,
	.parity		= CONFIG_UART4_PARITY,
	.bits		= CONFIG_UART4_BITS,
	.stopbits2	= CONFIG_UART4_2STOP,
	.rxd		= GPIO_UART4_RXD,
	.txd		= GPIO_UART4_TXD,
	.pclk		= CLK_GATE_UARTDBG_PCLK,
	.extclk		= CLK_GATE_UARTDBG_EXTCLK,
};

static uart_dev_t g_uart4port = {
	.recv = {
		.size	= CONFIG_UART4_RXBUFSIZE,
		.buffer	= g_uart4rxbuffer,
	},
	.xmit = {
		.size	= CONFIG_UART4_TXBUFSIZE,
		.buffer	= g_uart4txbuffer,
	},
	.ops		= &g_uart_ops,
	.priv		= &g_uart4priv,
};
#endif

#ifdef CONFIG_PM
static struct pm_callback_s g_serialcb = {
	.notify  = up_pm_notify,
	.prepare = up_pm_prepare,
};
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_earlyserialinit
 *
 * Description:
 *   Performs the low level UART initialization early in debug so that the
 *   serial console will be available during bootup.  This must be called
 *   before up_serialinit.
 *
 ****************************************************************************/
#if defined(USE_EARLYSERIALINIT)
void up_earlyserialinit(void)
{
	/* Disable clocks on UARTs by default except UARTDBG */
	s5j_clk_disable(CLK_GATE_UART0_EXTCLK);
	s5j_clk_disable(CLK_GATE_UART0_PCLK);
	s5j_clk_disable(CLK_GATE_UART1_EXTCLK);
	s5j_clk_disable(CLK_GATE_UART1_PCLK);
	s5j_clk_disable(CLK_GATE_UART2_EXTCLK);
	s5j_clk_disable(CLK_GATE_UART2_PCLK);
	s5j_clk_disable(CLK_GATE_UART3_EXTCLK);
	s5j_clk_disable(CLK_GATE_UART3_PCLK);

#if defined(HAVE_SERIAL_CONSOLE)
	CONSOLE_DEV.isconsole = true;
	CONSOLE_DEV.ops->setup(&CONSOLE_DEV);
#endif
}
#endif /* USE_EARLYSERIALINIT */

/****************************************************************************
 * Name: up_serialinit
 *
 * Description:
 *   Register serial console and serial ports.  This assumes
 *   that up_earlyserialinit was called previously.
 *
 ****************************************************************************/
#if defined(USE_SERIALDRIVER)
void up_serialinit(void)
{
#if CONFIG_NFILE_DESCRIPTORS > 0

	/* Register to receive power management callbacks */
#ifdef CONFIG_PM
	DEBUGVERIFY(pm_register(0, &g_serialcb) == OK);
#endif

	/* Register the console */
#if defined(HAVE_SERIAL_CONSOLE)
	uart_register("/dev/console", &CONSOLE_DEV);
#endif

	/* Register all UARTs */
#ifdef TTYS0_DEV
	uart_register("/dev/ttyS0", &TTYS0_DEV);
#endif
#ifdef TTYS1_DEV
	uart_register("/dev/ttyS1", &TTYS1_DEV);
#endif
#ifdef TTYS2_DEV
	uart_register("/dev/ttyS2", &TTYS2_DEV);
#endif
#ifdef TTYS3_DEV
	uart_register("/dev/ttyS3", &TTYS3_DEV);
#endif
#ifdef TTYS4_DEV
	uart_register("/dev/ttyS4", &TTYS4_DEV);
#endif
#endif /* CONFIG_NFILE_DESCRIPTORS > 0 */
}
#endif /* USE_SERIALDRIVER */

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
#if defined(HAVE_SERIAL_CONSOLE)
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
	uint32_t ufstat = uart_getreg32(CONSOLE_DEV.priv, S5J_UART_UFSTAT_OFFSET);

	if ((ufstat & (UART_UFSTAT_RX_FIFO_COUNT_MASK |
				UART_UFSTAT_RX_FIFO_FULL_MASK)) != 0) {
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
#if defined(HAVE_SERIAL_CONSOLE)
	irqstate_t flags;

	while (1) {
		/* Wait for the transmitter to be available */
		while ((uart_getreg32(CONSOLE_DEV.priv, S5J_UART_UTRSTAT_OFFSET) &
					UART_UTRSTAT_TX_BUF_MASK) != UART_UTRSTAT_TX_BUF_EMPTY) {
			/* Polling */
		}

		/*
		 * Disable interrupts so that the test and the transmission are
		 * atomic
		 */
		flags = irqsave();

		if ((uart_getreg32(CONSOLE_DEV.priv, S5J_UART_UTRSTAT_OFFSET) &
					UART_UTRSTAT_TX_BUF_MASK) == UART_UTRSTAT_TX_BUF_EMPTY) {
			/* Send the character */
			uart_putreg32(CONSOLE_DEV.priv, S5J_UART_UTXH_OFFSET, ch);
			irqrestore(flags);
			return;
		}

		irqrestore(flags);
	}
#endif
}
