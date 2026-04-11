/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/spinlock.h>
#include <arch/spinlock.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/init.h>
#include <tinyara/serial/serial.h>

#include <arch/irq.h>

#include "chip.h"
#include "up_internal.h"

#ifdef CONFIG_QEMU_MPS2_AN505_CMSDK_UART

#define CMSDK_UART_STATE_TXBF          (1 << 0)
#define CMSDK_UART_STATE_RXBF          (1 << 1)
#define CMSDK_UART_STATE_TXOVERRUN     (1 << 2)
#define CMSDK_UART_STATE_RXOVERRUN     (1 << 3)

#define CMSDK_UART_CTRL_TXEN           (1 << 0)
#define CMSDK_UART_CTRL_RXEN           (1 << 1)
#define CMSDK_UART_CTRL_TXIRQEN        (1 << 2)
#define CMSDK_UART_CTRL_RXIRQEN        (1 << 3)
#define CMSDK_UART_CTRL_TXOVRIRQEN     (1 << 4)
#define CMSDK_UART_CTRL_RXOVRIRQEN     (1 << 5)

#define CMSDK_UART_INT_TX              (1 << 0)
#define CMSDK_UART_INT_RX              (1 << 1)
#define CMSDK_UART_INT_TXOVERRUN       (1 << 2)
#define CMSDK_UART_INT_RXOVERRUN       (1 << 3)
#define CMSDK_UART_INT_ALL             (CMSDK_UART_INT_TX | CMSDK_UART_INT_RX | \
					CMSDK_UART_INT_TXOVERRUN |      \
					CMSDK_UART_INT_RXOVERRUN)

#if defined(CONFIG_UART0_SERIAL_CONSOLE)
#define HAVE_CMSDK_CONSOLE             1
#define CONSOLE_DEV                    g_uart0port
#endif

#define TTYS0_DEV                      g_uart0port

struct cmsdk_uart_regs
{
	volatile uint32_t data;
	volatile uint32_t state;
	volatile uint32_t ctrl;
	union {
		volatile uint32_t intstatus;
		volatile uint32_t intclear;
	};
	volatile uint32_t bauddiv;
};

struct cmsdk_uart_port_s
{
	FAR volatile struct cmsdk_uart_regs *regs;
	uint32_t clock;
	unsigned int rxirq;
	unsigned int txirq;
	spinlock_t lock;
};

static int cmsdk_setup(FAR struct uart_dev_s *dev);
static void cmsdk_shutdown(FAR struct uart_dev_s *dev);
static int cmsdk_attach(FAR struct uart_dev_s *dev);
static void cmsdk_detach(FAR struct uart_dev_s *dev);
static int cmsdk_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg);
static int cmsdk_receive(FAR struct uart_dev_s *dev, FAR unsigned int *status);
static void cmsdk_rxint(FAR struct uart_dev_s *dev, bool enable);
static bool cmsdk_rxavailable(FAR struct uart_dev_s *dev);
static void cmsdk_send(FAR struct uart_dev_s *dev, int ch);
static void cmsdk_txint(FAR struct uart_dev_s *dev, bool enable);
static bool cmsdk_txready(FAR struct uart_dev_s *dev);
static bool cmsdk_txempty(FAR struct uart_dev_s *dev);

static const struct uart_ops_s g_uart_ops = {
	.setup       = cmsdk_setup,
	.shutdown    = cmsdk_shutdown,
	.attach      = cmsdk_attach,
	.detach      = cmsdk_detach,
	.ioctl       = cmsdk_ioctl,
	.receive     = cmsdk_receive,
	.rxint       = cmsdk_rxint,
	.rxavailable = cmsdk_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
	.rxflowcontrol = NULL,
#endif
	.send        = cmsdk_send,
	.txint       = cmsdk_txint,
	.txready     = cmsdk_txready,
	.txempty     = cmsdk_txempty,
};

static char g_uart0rxbuffer[CONFIG_UART0_RXBUFSIZE];
static char g_uart0txbuffer[CONFIG_UART0_TXBUFSIZE];

static struct cmsdk_uart_port_s g_uart0priv = {
	.regs  = (FAR volatile struct cmsdk_uart_regs *)QEMU_MPS2_AN505_UART0_BASE,
	.clock = QEMU_MPS2_AN505_UART_CLK,
	.rxirq = QEMU_MPS2_AN505_IRQ_UART0_RX,
	.txirq = QEMU_MPS2_AN505_IRQ_UART0_TX,
	.lock  = SP_UNLOCKED,
};

static struct uart_dev_s g_uart0port = {
	.recv = {
		.size   = CONFIG_UART0_RXBUFSIZE,
		.buffer = g_uart0rxbuffer,
	},
	.xmit = {
		.size   = CONFIG_UART0_TXBUFSIZE,
		.buffer = g_uart0txbuffer,
	},
	.ops  = &g_uart_ops,
	.priv = &g_uart0priv,
};

static inline void cmsdk_clear_interrupts(FAR struct cmsdk_uart_port_s *priv,
					  uint32_t mask)
{
	priv->regs->intclear = mask;
}

static inline uint32_t cmsdk_calc_bauddiv(FAR struct cmsdk_uart_port_s *priv,
					  uint32_t baud)
{
	uint32_t divider;

	if (baud == 0) {
		return 16;
	}

	divider = priv->clock / baud;
	return divider < 16 ? 16 : divider;
}

static inline uint32_t cmsdk_irq_mask(bool enable, uint32_t bit)
{
	return enable ? bit : 0;
}

static int cmsdk_irq_handler(int irq, FAR void *context, FAR void *arg)
{
	FAR struct uart_dev_s *dev = arg;

	UNUSED(irq);
	UNUSED(context);

	if (cmsdk_rxavailable(dev)) {
		uart_recvchars(dev);
	}

	if (cmsdk_txready(dev)) {
		uart_xmitchars(dev);
	}

	return OK;
}

static int cmsdk_setup(FAR struct uart_dev_s *dev)
{
	FAR struct cmsdk_uart_port_s *priv = dev->priv;

	priv->regs->ctrl = 0;
	cmsdk_clear_interrupts(priv, CMSDK_UART_INT_ALL);
	priv->regs->state = CMSDK_UART_STATE_TXOVERRUN | CMSDK_UART_STATE_RXOVERRUN;
	priv->regs->bauddiv = cmsdk_calc_bauddiv(priv, CONFIG_UART0_BAUD);
	priv->regs->ctrl = CMSDK_UART_CTRL_TXEN | CMSDK_UART_CTRL_RXEN;

	return OK;
}

static void cmsdk_shutdown(FAR struct uart_dev_s *dev)
{
	FAR struct cmsdk_uart_port_s *priv = dev->priv;

	priv->regs->ctrl = 0;
	cmsdk_clear_interrupts(priv, CMSDK_UART_INT_ALL);
}

static int cmsdk_attach_irqs(FAR struct uart_dev_s *dev)
{
	FAR struct cmsdk_uart_port_s *priv = dev->priv;
	int ret;

	ret = irq_attach(priv->rxirq, cmsdk_irq_handler, dev);
	if (ret != OK) {
		return ret;
	}

	ret = irq_attach(priv->txirq, cmsdk_irq_handler, dev);
	if (ret != OK) {
		irq_detach(priv->rxirq);
		return ret;
	}

#ifdef CONFIG_ARCH_IRQPRIO
	up_prioritize_irq(priv->rxirq, NVIC_SYSH_PRIORITY_DEFAULT);
	up_prioritize_irq(priv->txirq, NVIC_SYSH_PRIORITY_DEFAULT);
#endif
	up_enable_irq(priv->rxirq);
	up_enable_irq(priv->txirq);
	return OK;
}

static int cmsdk_attach(FAR struct uart_dev_s *dev)
{
	return cmsdk_attach_irqs(dev);
}

static void cmsdk_detach(FAR struct uart_dev_s *dev)
{
	FAR struct cmsdk_uart_port_s *priv = dev->priv;

	up_disable_irq(priv->rxirq);
	up_disable_irq(priv->txirq);
	irq_detach(priv->rxirq);
	irq_detach(priv->txirq);
}

static int cmsdk_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg)
{
	UNUSED(dev);
	UNUSED(arg);

	switch (cmd) {
	case TIOCSBRK:
	case TIOCCBRK:
	default:
		return -ENOTTY;
	}
}

static int cmsdk_receive(FAR struct uart_dev_s *dev, FAR unsigned int *status)
{
	FAR struct cmsdk_uart_port_s *priv = dev->priv;

	*status = priv->regs->state;
	cmsdk_clear_interrupts(priv, CMSDK_UART_INT_RX);
	return priv->regs->data & 0xff;
}

static void cmsdk_rxint(FAR struct uart_dev_s *dev, bool enable)
{
	FAR struct cmsdk_uart_port_s *priv = dev->priv;
	uint32_t regval;

	regval = priv->regs->ctrl;
	regval &= ~(CMSDK_UART_CTRL_RXIRQEN | CMSDK_UART_CTRL_RXOVRIRQEN);
	regval |= cmsdk_irq_mask(enable, CMSDK_UART_CTRL_RXIRQEN);
	priv->regs->ctrl = regval;

	if (!enable) {
		cmsdk_clear_interrupts(priv, CMSDK_UART_INT_RX);
	}
}

static bool cmsdk_rxavailable(FAR struct uart_dev_s *dev)
{
	FAR struct cmsdk_uart_port_s *priv = dev->priv;

	return (priv->regs->state & CMSDK_UART_STATE_RXBF) != 0;
}

static void cmsdk_send(FAR struct uart_dev_s *dev, int ch)
{
	FAR struct cmsdk_uart_port_s *priv = dev->priv;

	while ((priv->regs->state & CMSDK_UART_STATE_TXBF) != 0) {
	}

	cmsdk_clear_interrupts(priv, CMSDK_UART_INT_TX);
	priv->regs->data = (uint32_t)ch & 0xff;
}

static void cmsdk_txint(FAR struct uart_dev_s *dev, bool enable)
{
	FAR struct cmsdk_uart_port_s *priv = dev->priv;
	irqstate_t flags;
	uint32_t regval;

	flags = spin_lock_irqsave(&priv->lock);

	regval = priv->regs->ctrl;
	regval &= ~(CMSDK_UART_CTRL_TXIRQEN | CMSDK_UART_CTRL_TXOVRIRQEN);
	regval |= cmsdk_irq_mask(enable, CMSDK_UART_CTRL_TXIRQEN);
	priv->regs->ctrl = regval;

	spin_unlock_irqrestore(&priv->lock, flags);

	if (enable) {
		uart_xmitchars(dev);
	} else {
		cmsdk_clear_interrupts(priv, CMSDK_UART_INT_TX);
	}
}

static bool cmsdk_txready(FAR struct uart_dev_s *dev)
{
	FAR struct cmsdk_uart_port_s *priv = dev->priv;

	return (priv->regs->state & CMSDK_UART_STATE_TXBF) == 0;
}

static bool cmsdk_txempty(FAR struct uart_dev_s *dev)
{
	return cmsdk_txready(dev);
}

static void cmsdk_putc(FAR struct uart_dev_s *dev, int ch)
{
	FAR struct cmsdk_uart_port_s *priv = dev->priv;
	irqstate_t flags;

	flags = spin_lock_irqsave(&priv->lock);
	while (!cmsdk_txready(dev)) {
	}

	cmsdk_send(dev, ch);
	spin_unlock_irqrestore(&priv->lock, flags);
}

void up_earlyserialinit(void)
{
#ifdef CONSOLE_DEV
	CONSOLE_DEV.isconsole = true;
	cmsdk_setup(&CONSOLE_DEV);
#endif
}

void up_serialinit(void)
{
#ifdef CONSOLE_DEV
	uart_register("/dev/console", &CONSOLE_DEV);
#endif
	uart_register("/dev/ttyS0", &TTYS0_DEV);
}

void *up_get_console_dev(void)
{
#ifdef CONSOLE_DEV
	return &CONSOLE_DEV;
#else
	return NULL;
#endif
}

void up_lowputc(char ch)
{
#ifdef CONSOLE_DEV
	cmsdk_putc(&CONSOLE_DEV, ch);
#else
	UNUSED(ch);
#endif
}

#ifdef HAVE_CMSDK_CONSOLE
int up_putc(int ch)
{
	cmsdk_putc(&CONSOLE_DEV, ch);
	return ch;
}
#endif

#endif /* CONFIG_QEMU_MPS2_AN505_CMSDK_UART */
