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
#include "up_arch.h"
#include "up_internal.h"

#include "tinyara/kmalloc.h"

#include "uart_hal.h"
#include "hal_uart_types.h"
#include "uart.h"

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
#define CONSOLE                 UART3_DEV
#define UART1_ASSIGNED  1
#define HAVE_SERIAL_CONSOLE
#elif defined(CONFIG_UART2_SERIAL_CONSOLE)
#define CONSOLE_DEV             g_uart2port             /* UART2 is console */
#define TTYS0_DEV               g_uart2port             /* UART2 is ttyS0 */
#define CONSOLE                 UART2_DEV
#define UART2_ASSIGNED  1
#define HAVE_SERIAL_CONSOLE
#else
#undef CONSOLE_DEV                                              /* No console */
#if defined(CONFIG_ARMINO_UART0)
#define TTYS0_DEV               g_uart0port             /* UART0 is ttyS0 */
#define UART0_ASSIGNED  1
#elif defined(CONFIG_ARMINO_UART1)
#define TTYS0_DEV               g_uart1port             /* UART1 is ttyS0 */
#define UART1_ASSIGNED  1
#elif defined(CONFIG_ARMINO_UART2)
#define TTYS0_DEV               g_uart2port             /* UART2 is ttyS0 */
#define UART2_ASSIGNED  1
#endif
#endif

/* Pick ttyS1. This could be any of UART0-2 excluding the console UART. */
#if defined(CONFIG_ARMINO_UART0) && !defined(UART0_ASSIGNED)
#define TTYS1_DEV               g_uart0port             /* UART0 is ttyS1 */
#define UART0_ASSIGNED  1
#elif defined(CONFIG_ARMINO_UART1) && !defined(UART1_ASSIGNED)
#define TTYS1_DEV               g_uart1port             /* UART1 is ttyS1 */
#define UART1_ASSIGNED  1
#elif defined(CONFIG_ARMINO_UART2) && !defined(UART2_ASSIGNED)
#define TTYS1_DEV               g_uart2port             /* UART2 is ttyS1 */
#define UART2_ASSIGNED  1
#endif

/*
 * Pick ttyS2. This could be one of UART1-2. It can't be UART0 because that
 * was either assigned as ttyS0 or ttyS1. One of these could also be the
 * console
 */
#if defined(CONFIG_ARMINO_UART1) && !defined(UART1_ASSIGNED)
#define TTYS2_DEV               g_uart1port             /* UART1 is ttyS2 */
#define UART1_ASSIGNED  1
#elif defined(CONFIG_ARMINO_UART2) && !defined(UART2_ASSIGNED)
#define TTYS2_DEV               g_uart2port             /* UART2 is ttyS2 */
#define UART2_ASSIGNED  1
#endif
#define CHAR_TIMEOUT 6540
#define TX_FIFO_MAX 16

/****************************************************************************
 * Private Types
 ****************************************************************************/
/*
 * Available UARTs in ARMINO
 * UART0_DEV: KM4 uart0
 * UART1_DEV: KM4 uart1_bt
 * UART2_DEV: KM0 log uart
 * UART3_DEV: KM0 luart
 */

//static serial_t* sdrv[MAX_UART_INDEX + 1] = {NULL, NULL, NULL, NULL, NULL}; //uart 0~3, uart2 is configured as log uart

struct armino_up_dev_s {
	uint8_t uart_id;            /* uart id */
	uart_config_t config;       /* uart config */
	uint8_t parity;				/* 0=none, 1=odd, 2=even */
	uint8_t bits;				/* Number of bits (7 or 8) */
	uint8_t stopbit;			/* Number of StopBit (1 or 2) */
	uint32_t baud;				/* Configured baud rate */
	uint32_t irq;				/* IRQ associated with this UART */
	uint8_t tx;					/* TX UART pin number */
	uint8_t rx;					/* RX UART pin number */
	uint8_t rts;				/* UART RTS pin number */
	uint8_t cts;				/* UART CTS pin number */
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

static int armino_up_setup(struct uart_dev_s *dev);
static void armino_up_shutdown(struct uart_dev_s *dev);
static int armino_up_attach(struct uart_dev_s *dev);
static void armino_up_detach(struct uart_dev_s *dev);
static int armino_up_interrupt(int irq, void *context, void *arg);
static int armino_up_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg);
static int armino_up_receive(struct uart_dev_s *dev, unsigned int *status);
static void armino_up_rxint(struct uart_dev_s *dev, bool enable);
static bool armino_up_rxavailable(struct uart_dev_s *dev);
static void armino_up_send(struct uart_dev_s *dev, int ch);
static void armino_up_txint(struct uart_dev_s *dev, bool enable);
static bool armino_up_txready(struct uart_dev_s *dev);
static bool armino_up_txempty(struct uart_dev_s *dev);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Serial driver UART operations */

static const struct uart_ops_s g_uart_ops = {
	.setup = armino_up_setup,
	.shutdown = armino_up_shutdown,
	.attach = armino_up_attach,
	.detach = armino_up_detach,
	.ioctl = armino_up_ioctl,
	.receive = armino_up_receive,
	.rxint = armino_up_rxint,
	.rxavailable = armino_up_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
	.rxflowcontrol = NULL,
#endif
	.send = armino_up_send,
	.txint = armino_up_txint,
	.txready = armino_up_txready,
	.txempty = armino_up_txempty,
};

#ifdef CONFIG_ARMINO_UART0
static char g_uart0rxbuffer[CONFIG_UART0_RXBUFSIZE];
static char g_uart0txbuffer[CONFIG_UART0_TXBUFSIZE];
#endif
#ifdef CONFIG_ARMINO_UART1
static char g_uart1rxbuffer[CONFIG_UART1_RXBUFSIZE];
static char g_uart1txbuffer[CONFIG_UART1_TXBUFSIZE];
#endif
#ifdef CONFIG_ARMINO_UART2
static char g_uart2rxbuffer[CONFIG_UART2_RXBUFSIZE];
static char g_uart2txbuffer[CONFIG_UART2_TXBUFSIZE];
#endif

#define ARMINO_UART0_IRQ	(50)
#define ARMINO_UART1_IRQ	(51)
#define ARMINO_UART_LOG_IRQ	(19)
#define ARMINO_UARTLP_IRQ	(25)

#ifdef CONFIG_ARMINO_UART0
static struct armino_up_dev_s g_uart0priv = {
	.uart_id = 0,
	.parity = CONFIG_UART0_PARITY,
	.bits = CONFIG_UART0_BITS,
#if (CONFIG_UART0_2STOP)
	.stopbit = 2,
#else
	.stopbit = 1,
#endif
	.baud = CONFIG_UART0_BAUD,
	.irq = ARMINO_UART0_IRQ,
	.tx = 0,
	.rx = 0,
	.rts = 0,
	.cts = 0,
	.FlowControl = 0,
	.txint_enable = false,
	.rxint_enable = false,
};

static uart_dev_t g_uart0port = {
	.isconsole = true,
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

#ifdef CONFIG_ARMINO_UART1
static struct armino_up_dev_s g_uart1priv = {
	.uart_id = 1,
	.parity = CONFIG_UART1_PARITY,
	.bits = CONFIG_UART1_BITS,
#if (CONFIG_UART1_2STOP)
	.stopbit = 2,
#else
	.stopbit = 1,
#endif
	.baud = CONFIG_UART1_BAUD,
	.irq = ARMINO_UARTLP_IRQ,
	.tx = 0,
	.rx = 0,
	.FlowControl = 0,
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

#ifdef CONFIG_ARMINO_UART2
static struct armino_up_dev_s g_uart2priv = {
	.uart_id = 2,
	.parity = CONFIG_UART2_PARITY,
	.bits = CONFIG_UART2_BITS,
#if (CONFIG_UART2_2STOP)
	.stopbit = 2,
#else
	.stopbit = 1,
#endif
	.baud = CONFIG_UART2_BAUD,
	.irq = ARMINO_UART_LOG_IRQ,
	.tx = 0,
	.rx = 0,
	.FlowControl = 0,
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

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: armino_up_setup
 *
 * Description:
 *   Configure the UART baud, bits, parity, fifos, etc. This
 *   method is called the first time that the serial port is
 *   opened.
 *
 ****************************************************************************/
static int armino_up_setup(struct uart_dev_s *dev)
{
	dbg("[+]%s[+]\r\n",__func__);
	struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	uart_config_t *config = &(priv->config);
	DEBUGASSERT(config);
	int ret;
    config->baud_rate = priv->baud;//UART_BAUD_RATE;
    config->data_bits = 3;//UART_DATA_8_BITS;
    config->parity = 0;//UART_PARITY_NONE;
    config->stop_bits = 0;//UART_STOP_BITS_1;
    config->flow_ctrl = UART_FLOWCTRL_DISABLE;
    config->src_clk = UART_SCLK_XTAL_26M;
	dbg("[=]%s[=]\r\n",__func__);
    ret = bk_uart_init(priv->uart_id, config);
    if (BK_OK != ret)
    {
		return ret;
    }
	dbg("[-]%s[-]\r\n",__func__);
	while(uart_read_byte(priv->uart_id) != -1);

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

static void armino_up_shutdown(struct uart_dev_s *dev)
{
	struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
	bk_uart_deinit(priv->uart_id);
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
static int armino_up_interrupt(int irq, void *context, void *arg)
{
	struct uart_dev_s *dev = (struct uart_dev_s *)arg;
	struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
	// uint32_t get_enable_status = uart_get_int_enable_status(priv->uart_id); // Unused variable
	uint32_t get_int_status = uart_get_interrupt_status(priv->uart_id);
	uart_clear_interrupt_status(priv->uart_id, get_int_status);
	bk_uart_enable_rx_interrupt(priv->uart_id);
	if(get_int_status & (0x1 << 6)){
		uart_recvchars(dev);
	}
	if(get_int_status & (0x1 << 0)){
		uart_xmitchars(dev);
	}

	return 0;
}

static inline int armino_irq_vector_from_id(uint8_t uart_id) {
    switch (uart_id) {
        case 0: return ARMINO_IRQ_UART_LP;
        case 1: return ARMINO_IRQ_UART1;
        case 2: return ARMINO_IRQ_UART2;
        default: return ARMINO_IRQ_UART_LP;
    }
}

static int armino_up_attach(struct uart_dev_s *dev)
{
	struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
    DEBUGASSERT(priv);

    int vector = armino_irq_vector_from_id(priv->uart_id);
	/* Attach and enable the IRQ */
	int ret = irq_attach(vector, armino_up_interrupt, dev);
	if (ret == OK) {
        up_enable_irq(vector);
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

static void armino_up_detach(struct uart_dev_s *dev)
{
	struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	bk_uart_register_rx_isr(priv->uart_id, NULL, 0);
	bk_uart_register_tx_isr(priv->uart_id, NULL, 0);
	int vector = armino_irq_vector_from_id(priv->uart_id);
    up_disable_irq(vector);
}

/****************************************************************************
 * Name: up_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method
 *
 ****************************************************************************/

static int armino_up_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg)
{
#if defined(CONFIG_SERIAL_TERMIOS)
	struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
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
			if (priv->parity == UART_PARITY_ODD) {
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
				priv->parity = UART_PARITY_ODD;
			} else {
				priv->parity = UART_PARITY_EVEN;
			}
		}
		if (!!(termiosp->c_cflag & CSTOPB)) {
			priv->stopbit = 2;
		} else {
			priv->stopbit = 1;
		}
		priv->baud = cfgetispeed(termiosp);
		armino_up_shutdown(dev);

		armino_up_setup(dev);
		armino_up_attach(dev);
		armino_up_txint(dev, priv->txint_enable);
		armino_up_rxint(dev, priv->rxint_enable);

		break;

	case TIOCLOOPBACK:
		if (!arg) {
			return -EINVAL;
		}
		//serial_control_loopback(sdrv[uart_index_get(priv->tx)], *(bool *)arg);
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

static int armino_up_receive(struct uart_dev_s *dev, unsigned int *status)
{
	struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
	uint32_t rxd = 0;

	DEBUGASSERT(priv);
	rxd = uart_read_byte(priv->uart_id);

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
static void armino_up_rxint(struct uart_dev_s *dev, bool enable)
{
	struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	priv->rxint_enable = enable;
	if(enable) {
		bk_uart_enable_rx_interrupt(priv->uart_id);
	} else {
		bk_uart_disable_rx_interrupt(priv->uart_id);
	}
	//serial_irq_set(sdrv[uart_index_get(priv->tx)], RxIrq, enable);	// 1= ENABLE
}

/****************************************************************************
 * Name: up_rxavailable
 *
 * Description:
 *   Return true if the receive fifo is not empty
 *
 ****************************************************************************/

static bool armino_up_rxavailable(struct uart_dev_s *dev)
{
	struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
  	bk_err_t ret = BK_OK;
	ret = uart_read_ready(priv->uart_id);
	if(ret == BK_OK){
		return true;
	} else {
		return false;
	}
    return false;
}

/****************************************************************************
 * Name: up_send
 *
 * Description:
 *   This method will send one byte on the UART
 *
 ****************************************************************************/

static void armino_up_send(struct uart_dev_s *dev, int ch)
{
    struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	/*write one byte to tx fifo*/
	uart_write_byte(priv->uart_id, ch);
}

/****************************************************************************
 * Name: up_txint
 *
 * Description:
 *   Call to enable or disable TX interrupts
 *
 ****************************************************************************/

static void armino_up_txint(struct uart_dev_s *dev, bool enable)
{
	struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	priv->txint_enable = enable;
	if(enable) {
		bk_uart_enable_tx_interrupt(priv->uart_id);
	} else {
		bk_uart_disable_tx_interrupt(priv->uart_id);
	}
}

/****************************************************************************
 * Name: up_txready
 *
 * Description:
 *   Return true if the tranmsit fifo is not full
 *
 ****************************************************************************/

static bool armino_up_txready(struct uart_dev_s *dev)
{
	struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
  	bk_err_t ret = BK_OK;
	ret = uart_write_ready(priv->uart_id);
	if(ret == BK_OK){
		return true;
	} else {
		return false;
	}
    return false;
}

/****************************************************************************
 * Name: up_txempty
 *
 * Description:
 *   Return true if the transmit fifo is empty
 *
 ****************************************************************************/

static bool armino_up_txempty(struct uart_dev_s *dev)
{
	struct armino_up_dev_s *priv = (struct armino_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	return bk_uart_is_tx_over(priv->uart_id);
}

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
volatile uint32_t g_system_log_ready = 0;
void up_serialinit(void)
{
#ifdef CONSOLE_DEV
	CONSOLE_DEV.isconsole = true;
	armino_up_setup(&CONSOLE_DEV);

	/* Register the console */
	uart_register("/dev/console", &CONSOLE_DEV);
#endif

	/* Register all UARTs */
#ifdef TTYS0_DEV
	uart_register("/dev/ttyS0", &TTYS0_DEV);
#endif
#ifdef TTYS1_DEV
	armino_up_setup(&TTYS1_DEV);
	uart_register("/dev/ttyS1", &TTYS1_DEV);
#endif
#ifdef TTYS2_DEV
	armino_up_setup(&TTYS2_DEV);
	uart_register("/dev/ttyS2", &TTYS2_DEV);
#endif

    g_system_log_ready = 1;

}

/****************************************************************************
 * Name: up_get_console_dev
 *
 * Description:
 *   Read CONSOLE_DEV device serial structure
 *
 * Input Parameters:
 *   none
 *
 * Returned Value:
 *   CONSOLE_DEV structure address
 *
 ****************************************************************************/
void *up_get_console_dev(void)
{
#ifdef CONSOLE_DEV
	return &CONSOLE_DEV;
#else
	return NULL;
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
	uart_write_byte(UART_ID_0, ch);
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
	return uart_read_byte(UART_ID_0);
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
