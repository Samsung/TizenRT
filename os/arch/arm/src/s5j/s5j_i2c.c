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
 * arch/arm/src/s5j/s5j_i2c.c
 *
 *   Copyright (C) 2014-2016 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * The Samsung sample code has a BSD compatible license that requires this
 * copyright notice:
 *
 *   Copyright (c) 2016 Samsung Electronics, Inc.
 *   All rights reserved.
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
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <tinyara/i2c.h>
#include <tinyara/clock.h>

#include <arch/serial.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "s5j_gpio.h"
#include "s5j_i2c.h"
#include "s5j_vclk.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define S5J_DEFAULT_I2CXFER_CLOCK	(100 * 1000)	/* 100Khz */
#define S5J_DEFAULT_I2CSLAVE_ADDR	0x22
#define S5J_DEFAULT_I2C_TIMEOUT		10000

#define HSI2C_INT_XFER_DONE (HSI2C_INT_XFER_DONE_NOACK_MANUAL | HSI2C_INT_XFER_DONE_MANUAL)

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct s5j_i2c_priv_s *g_s5j_i2c_priv[4] = { NULL };

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* I2C Interface */
static const struct i2c_ops_s s5j_i2c_ops = {
	.setfrequency = s5j_i2c_setclock,
	.setaddress = s5j_i2c_setownaddress,
	.write = s5j_i2c_write,
	.read = s5j_i2c_read,
#ifdef CONFIG_I2C_TRANSFER
	.transfer = s5j_i2c_transfer,
#endif
#ifdef CONFIG_I2C_SLAVE
	.setownaddress = NULL,
	.registercallback = NULL,
#endif
};

static const struct s5j_i2c_config_s s5j_i2c0_config = {
	.base = S5J_HSI2C0_BASE,
	.scl_pin = GPIO_I2C0_SCL,
	.sda_pin = GPIO_I2C0_SDA,
	.isr = s5j_i2c0_interrupt,
	.irq = IRQ_HSI2C_0,
	.devno = 0,
};

static struct s5j_i2c_priv_s s5j_i2c0_priv = {
	.xfer_speed = S5J_DEFAULT_I2CXFER_CLOCK,
	.master = I2C_MASTER,
	.mode = I2C_POLLING,
	.slave_addr = S5J_DEFAULT_I2CSLAVE_ADDR,
	.addrlen = 7,
	.timeout = S5J_DEFAULT_I2C_TIMEOUT,
	.name = "s5j_i2c0",
	.initialized = 0,
	.retries = 3,
};

static const struct s5j_i2c_config_s s5j_i2c1_config = {
	.base = S5J_HSI2C1_BASE,
	.scl_pin = GPIO_I2C1_SCL,
	.sda_pin = GPIO_I2C1_SDA,
	.isr = s5j_i2c1_interrupt,
	.irq = IRQ_HSI2C_1,
	.devno = 1,
};

static struct s5j_i2c_priv_s s5j_i2c1_priv = {
	.xfer_speed = S5J_DEFAULT_I2CXFER_CLOCK,
	.master = I2C_MASTER,
	.mode = I2C_POLLING,
	.slave_addr = S5J_DEFAULT_I2CSLAVE_ADDR,
	.addrlen = 7,
	.timeout = S5J_DEFAULT_I2C_TIMEOUT,
	.name = "s5j_i2c1",
	.initialized = 0,
	.retries = 3,
};

static const struct s5j_i2c_config_s s5j_i2c2_config = {
	.base = S5J_HSI2C2_BASE,
	.scl_pin = GPIO_I2C2_SCL,
	.sda_pin = GPIO_I2C2_SDA,
	.isr = s5j_i2c2_interrupt,
	.irq = IRQ_HSI2C_2,
	.devno = 2,
};

static struct s5j_i2c_priv_s s5j_i2c2_priv = {
	.xfer_speed = S5J_DEFAULT_I2CXFER_CLOCK,
	.master = I2C_MASTER,
	.mode = I2C_POLLING,
	.slave_addr = S5J_DEFAULT_I2CSLAVE_ADDR,
	.addrlen = 7,
	.timeout = S5J_DEFAULT_I2C_TIMEOUT,
	.name = "s5j_i2c2",
	.initialized = 0,
	.retries = 3,
};

static const struct s5j_i2c_config_s s5j_i2c3_config = {
	.base = S5J_HSI2C3_BASE,
	.scl_pin = GPIO_I2C3_SCL,
	.sda_pin = GPIO_I2C3_SDA,
	.isr = s5j_i2c3_interrupt,
	.irq = IRQ_HSI2C_3,
	.devno = 3,
};

static struct s5j_i2c_priv_s s5j_i2c3_priv = {
	.xfer_speed = S5J_DEFAULT_I2CXFER_CLOCK,
	.master = I2C_MASTER,
	.mode = I2C_POLLING,
	.slave_addr = S5J_DEFAULT_I2CSLAVE_ADDR,
	.addrlen = 7,
	.timeout = S5J_DEFAULT_I2C_TIMEOUT,
	.name = "s5j_i2c3",
	.initialized = 0,
	.retries = 3,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void hsi2c_set_hs_timing(unsigned int base, unsigned int nClkDiv,
				unsigned int tSTART_SU, unsigned int tSTART_HD,
				unsigned int tSTOP_SU, unsigned int tSDA_SU,
				unsigned int tDATA_SU, unsigned int tDATA_HD,
				unsigned int tSCL_L, unsigned int tSCL_H,
				unsigned int tSR_RELEASE)
{
	tSTART_SU &= 0xFF;
	tSTART_HD &= 0xFF;
	tSTOP_SU &= 0xFF;
	putreg32(((tSTART_SU << 24) | (tSTART_HD << 16) | (tSTOP_SU << 8)), base + I2C_TIMING_HS1);

	tDATA_SU &= 0xFF;
	tSCL_L &= 0xFF;
	tSCL_H &= 0xFF;
	putreg32(((tDATA_SU << 24) | (tSCL_L << 8) | (tSCL_H << 0)), base + I2C_TIMING_HS2);

	nClkDiv &= 0xFF;
	tSR_RELEASE &= 0xFF;
	putreg32(((nClkDiv << 16) | (tSR_RELEASE << 0)), base + I2C_TIMING_HS3);

	tDATA_HD &= 0xFFFF;
	putreg32(tDATA_HD, base + I2C_TIMING_SLA);

}

static void hsi2c_set_fs_timing(unsigned int base, unsigned int nClkDiv,
				unsigned int tSTART_SU, unsigned int tSTART_HD,
				unsigned int tSTOP_SU, unsigned int tDATA_SU,
				unsigned int tDATA_HD, unsigned int tSCL_L,
				unsigned int tSCL_H, unsigned int tSR_RELEASE)
{
	tSTART_SU &= 0xFF;
	tSTART_HD &= 0xFF;
	tSTOP_SU &= 0xFF;
	putreg32(((tSTART_SU << 24) | (tSTART_HD << 16) | (tSTOP_SU << 8)), base + I2C_TIMING_FS1);

	tDATA_SU &= 0xFF;
	tSCL_L &= 0xFF;
	tSCL_H &= 0xFF;
	putreg32(((tDATA_SU << 24) | (tSCL_L << 8) | (tSCL_H << 0)), base + I2C_TIMING_FS2);

	nClkDiv &= 0xFF;
	tSR_RELEASE &= 0xFF;
	putreg32(((nClkDiv << 16) | (tSR_RELEASE << 0)), base + I2C_TIMING_FS3);

	tDATA_HD &= 0xFFFF;
	putreg32(tDATA_HD, base + I2C_TIMING_SLA);
}

static void hsi2c_calculate_timing(unsigned int base, unsigned int nPclk,
				unsigned int nOpClk)
{
	unsigned int reg;
	unsigned int nClkDiv;
	unsigned int tFTL_CYCLE_SCL;
	s32 i;
	s32 uTemp0;
	s32 uTemp1;
	s32 uTemp2 = 0;

	reg = getreg32(base + I2C_CONF);
	reg &= ~(0x7 << 13);
	putreg32(reg, base + I2C_CONF);

	reg = getreg32(base + I2C_CONF);
	reg &= ~(0x7 << 16);
	putreg32(reg, base + I2C_CONF);

	tFTL_CYCLE_SCL = getreg32((base + I2C_CONF) >> 16) & 0x7;

	uTemp0 = (float)(nPclk / nOpClk) - (tFTL_CYCLE_SCL + 3) * 2;

	for (i = 0; i < 256; i++) {
		uTemp1 = (uTemp0 + ((tFTL_CYCLE_SCL + 3) % (i + 1)) * 2) / (i + 1);
		if (uTemp1 < 512) {		/*  TSCL_L/H max is 512 / 2 */
			uTemp2 = uTemp1 - 2;
			break;
		}
	}

	unsigned int tSCL_H;
	nClkDiv = i;
	if (nOpClk > I2C_SPEED_400KHZ) {
		tSCL_H = ((uTemp2 + 10) / 3) - 5;
	} else {
		tSCL_H = uTemp2 / 2;
	}

	unsigned int tSCL_L = uTemp2 - tSCL_H;
	unsigned int tSTART_SU = tSCL_L;
	unsigned int tSTART_HD = tSCL_L;
	unsigned int tSTOP_SU = tSCL_L;
	unsigned int tSDA_SU = tSCL_L;
	unsigned int tDATA_SU = tSCL_L;
	unsigned int tDATA_HD = tSCL_L / 2;
	unsigned int tSR_RELEASE = uTemp2;

	if (nOpClk > I2C_SPEED_400KHZ) {
		/* 400Khz setting for Extended ID */
		hsi2c_set_fs_timing(base, 1, 38, 38, 38, 19, 19, 38, 38, 76);
		hsi2c_set_hs_timing(base, nClkDiv, tSTART_SU, tSTART_HD, tSTOP_SU,
					tSDA_SU, tDATA_SU, tDATA_HD, tSCL_L, tSCL_H, tSR_RELEASE);
	} else {
		hsi2c_set_fs_timing(base, nClkDiv, tSTART_SU, tSTART_HD, tSTOP_SU,
					tDATA_SU, tDATA_HD, tSCL_L, tSCL_H, tSR_RELEASE);
	}
}

static void hsi2c_conf(unsigned int base, unsigned int nOpClk)
{
	unsigned int val;

	val = getreg32(base + I2C_CONF);
	val &= ~(3 << 30);
	if (nOpClk > I2C_SPEED_400KHZ) {
		val |= (1 << 29);
	} else {
		val &= ~(1 << 29);
	}
	putreg32(val, base + I2C_CONF);
}

static void hsi2c_enable_int(unsigned int base, unsigned int bit)
{
	unsigned int val;
	val = getreg32(base + INT_EN);
	val |= bit;
	putreg32(val, base + INT_EN);
}

static void hsi2c_disable_int(unsigned int base, unsigned int bit)
{
	unsigned int val;

	val = getreg32(base + INT_EN);
	val &= ~bit;
	putreg32(val, base + INT_EN);
}

static void hsi2c_clear_int(unsigned int base, unsigned int bit)
{
	putreg32(bit, base + INT_STAT);
}

static unsigned int hsi2c_read_int_status(unsigned int base)
{
	return getreg32(base + INT_STAT) & getreg32(base + INT_EN);
}

static void hsi2c_set_slave_addr(unsigned int base, u16 addr,
				unsigned int is_master)
{
	unsigned int val;

	addr &= 0x3FF;

	val = getreg32(base + I2C_ADDR);

	if (is_master == 0) {
		val &= ~0x3ff;
		val |= addr;
	} else {
		val &= ~(0x3FF << 10);
		val |= (addr << 10);
	}
	putreg32(val, base + I2C_ADDR);
}

static int hsi2c_manual_fast_init(struct s5j_i2c_priv_s *priv)
{
	unsigned int val;
	unsigned int base = priv->config->base;

	hsi2c_conf(base, priv->xfer_speed);
	hsi2c_calculate_timing(base, priv->clock, priv->xfer_speed);
	hsi2c_enable_int(base, HSI2C_INT_XFER_DONE_MANUAL | HSI2C_INT_XFER_DONE_NOACK_MANUAL);

	priv->initialized = 1;

	val = getreg32(base + I2C_CONF);
	val &= ~((1 << 31) | (1 << 29));
	putreg32(val, base + I2C_CONF);

	hsi2c_enable_int(base, HSI2C_INT_XFER_DONE);

	return 0;
}

static int hsi2c_wait_xfer_done(struct s5j_i2c_priv_s *priv)
{
	int val;
	int timeout = priv->timeout;
	unsigned int base = priv->config->base;

	while (timeout-- > 0) {
		val = getreg32(base + INT_STAT) & HSI2C_INT_XFER_DONE_MANUAL;
		if (val) {
			putreg32(val, base + INT_STAT);
			return (val == HSI2C_INT_XFER_DONE_MANUAL);
		}
	}

	return -ETIMEDOUT;
}

static int hsi2c_wait_xfer_noack(struct s5j_i2c_priv_s *priv)
{
	int val;
	int timeout = priv->timeout;
	unsigned int base = priv->config->base;

	while (timeout-- > 0) {
		val = getreg32(base + INT_STAT) & HSI2C_INT_XFER_DONE_NOACK_MANUAL;
		if (val) {
			putreg32(val, base + INT_STAT);
			return (val == HSI2C_INT_XFER_DONE_NOACK_MANUAL);
		}
	}

	return -ETIMEDOUT;
}

static void hsi2c_start(struct s5j_i2c_priv_s *priv)
{
	int debug_msg;

	putreg32(0x88, priv->config->base + CTL);

	putreg32(I2C_START, priv->config->base + I2C_MANUAL_CMD);

	debug_msg = hsi2c_wait_xfer_done(priv);
}

static void hsi2c_stop(struct s5j_i2c_priv_s *priv)
{
	putreg32(I2C_STOP, priv->config->base + I2C_MANUAL_CMD);

	hsi2c_wait_xfer_done(priv);
}

static void hsi2c_repstart(struct s5j_i2c_priv_s *priv)
{
	putreg32(I2C_RESTART, priv->config->base + I2C_MANUAL_CMD);
}

static int hsi2c_outb(struct s5j_i2c_priv_s *priv, u8 data)
{
	unsigned int val;
	int ret;

	val = ((unsigned int)data) << 24 | I2C_SEND_DATA;
	putreg32(val, priv->config->base + I2C_MANUAL_CMD);

	ret = hsi2c_wait_xfer_done(priv);

	return ret;
}

static int hsi2c_inb(struct s5j_i2c_priv_s *priv, bool is_ack)
{
	unsigned int val = I2C_READ_DATA;
	u8 data;
	int ret;
	unsigned int base = priv->config->base;

	/* Looks awkward, but if I2C_RX_ACK is set, ACK is NOT generated */
	if (!is_ack) {
		val |= I2C_RX_ACK;
	}
	putreg32(val, base + I2C_MANUAL_CMD);

	ret = hsi2c_wait_xfer_done(priv);
	if (ret < 0) {
		return ret; /* timeout */
	}

	data = (getreg32(base + I2C_MANUAL_CMD) >> 16) & 0xff;

	return data;
}

static int sendbytes(struct s5j_i2c_priv_s *priv, struct i2c_msg_s *msg)
{
	const u8 *p = msg->buffer;
	int count = msg->length;
	int nak_ok = msg->flags & I2C_M_IGNORE_NAK;
	int wrcount = 0, ret;

	while (count > 0) {
		ret = hsi2c_outb(priv, *p);
		if ((ret == 1) || ((ret == 0) && nak_ok)) {
			count--;
			p++;
			wrcount++;
		} else if (ret == 0) {
			/* NAK from the slave */
			lldbg("%s: sendbytes: NAK bailout at #%d byte\n", priv->name, count);
			return -EIO;
		} else {
			/* Timeout */
			lldbg("%s: sendbytes: error %d\n", priv->name, ret);
			return ret;
		}
	}

	return wrcount;
}

static int readbytes(struct s5j_i2c_priv_s *priv, struct i2c_msg_s *msg)
{
	int val;
	int rdcount = 0;
	u8 *p = msg->buffer;
	int count = msg->length;

	while (count > 0) {
		val = hsi2c_inb(priv, (count > 1));
		if (val < 0) {
			break;
		}

		*p++ = val;
		rdcount++;
		count--;
	}

	lldbg("\nreadbytes: %d\n", rdcount);
	return rdcount;
}

static int try_address(struct s5j_i2c_priv_s *priv, u8 addr, int retries)
{
	int i, ret = 0;

	for (i = 0; i <= retries; i++) {
		ret = hsi2c_outb(priv, addr);
		if (ret == 1 || i == retries) {
			break;
		}
		hsi2c_stop(priv);
		up_udelay(priv->timeout / 2);
		hsi2c_start(priv);
	}

	if (i && ret) {
		lldbg("%s: used %d tries to %s client at 0x%02x: %s\n", priv->name,
						i + 1, addr & 1 ? "read from" : "write to",
						addr >> 1, ret == 1 ? "success" : "failed, timeout?");
	}

	return ret;
}

static int do_address(struct s5j_i2c_priv_s *priv, struct i2c_msg_s *msg)
{
	unsigned short flags = msg->flags;
	unsigned short nak_ok = msg->flags & I2C_M_IGNORE_NAK;
	u8 addr;
	int ret;
	int retries;

	retries = nak_ok ? 0 : priv->retries;

	if (flags & I2C_M_TEN) {
		/* a 10-bit address in manual mode */
		addr = 0xf0 | ((msg->addr >> 7) & 0x06);
		lldbg("%s: addr0: %d\n", priv->name, addr);

		ret = try_address(priv, addr, retries);
		if ((ret != 1) && !nak_ok) {
			lldbg("%s: died at extended address\n", priv->name);
			return -ENXIO;
		}

		/* the remaining 8 bit address */
		ret = hsi2c_outb(priv, msg->addr & 0xff);
		if ((ret != 1) && !nak_ok) {
			lldbg("%s: died at 2nd address\n", priv->name);
			return -ENXIO;
		}

		if (flags & I2C_M_READ) {
			hsi2c_repstart(priv);
			hsi2c_wait_xfer_done(priv);
			addr |= 0x1;
			ret = try_address(priv, addr, retries);
			if ((ret != 1) && !nak_ok) {
				lldbg("%s: died at repeated address\n", priv->name);
				return -EIO;
			}
		}
	} else {
		/* 7-bit address */
		addr = msg->addr << 1;
		if (flags & I2C_M_READ) {
			addr |= 0x1;
		}

		ret = try_address(priv, addr, retries);
		if ((ret != 1) && !nak_ok) {
			return -ENXIO;
		}
	}

	return 0;
}

static void hsi2c_run_auto_mode(unsigned int base, int on)
{
	unsigned int val = getreg32(base + I2C_AUTO_CONF);

	if (on) {
		val |= (1 << 31);
	} else {
		val &= ~(1 << 31);
	}

	putreg32(val, base + I2C_AUTO_CONF);
}

static void hsi2c_tx_fifo_reset(unsigned int base, int resetb)
{
	unsigned int val = getreg32(base + FIFO_CTL);

	if (resetb) {
		val |= (1 << 3);
	} else {
		val &= ~(1 << 3);
	}

	putreg32(val, base + FIFO_CTL);
}

static void hsi2c_set_auto_config(unsigned int base, unsigned int stop,
				unsigned int tx, unsigned int len)
{
	unsigned int val = getreg32(base + I2C_AUTO_CONF);

	if (stop) {
		val |= (1 << 17);
	} else {
		val &= ~(1 << 17);
	}
	if (tx) {
		val &= ~(1 << 16);
	} else {
		val |= (1 << 16);
	}

	val &= ~0xFFFF;
	val |= len;
	putreg32(val, base + I2C_AUTO_CONF);
}

static void hsi2c_set_trans_mode(unsigned int base, unsigned int master,
				unsigned int tx)
{
	unsigned int val = getreg32(base + CTL);

	val |= (1 << 0); /* ctrl 0 bit write 1 */

	if (master) {
		val |= (1 << 3);
	} else {
		val &= ~(1 << 3);
	}

	val &= ~(3 << 6);
	if (tx) {
		val |= (1 << 7);
	} else {
		val |= (1 << 6);
	}

	putreg32(val, base + CTL);
}

static void hsi2c_set_hwacg_mode(unsigned int base, unsigned int slave)
{
	unsigned int val = getreg32(base + CTL);

	val &= ~(0x3 << 24);

	if (slave) {
		val |= (0x1 << 24);
	} else {
		val &= ~(0x1 << 24);
	}

	putreg32(val, base + CTL);
}

static int hsi2c_master_handler(void *args)
{
	struct s5j_i2c_priv_s *priv = args;
	unsigned int int_status;
	int off = priv->master_test_data->cur_msg, buf_off = priv->master_test_data->buf_count;
	int trans_count = priv->master_test_data->num;
	struct i2c_msg_s *msg = &priv->master_test_data->msg[off];
	unsigned int base = priv->config->base;

	int_status = hsi2c_read_int_status(base);
	if (int_status & HSI2C_INT_TX_ALMOST_EMPTY) {
		if (buf_off < msg->length) {
			putreg32(msg->buffer[buf_off], base + TXDATA);
			priv->master_test_data->buf_count++;
		} else {
			hsi2c_disable_int(base, HSI2C_INT_TX_ALMOST_EMPTY);
		}
		hsi2c_clear_int(base, HSI2C_INT_TX_ALMOST_EMPTY);
	}
	if (int_status & HSI2C_INT_RX_ALMOST_FULL) {
		msg->buffer[buf_off] = (u8) getreg32(base + RXDATA);
		priv->master_test_data->buf_count++;
		hsi2c_clear_int(base, HSI2C_INT_RX_ALMOST_FULL);
	}
	if (int_status & HSI2C_INT_TRANSFER_DONE_AUTO) {
		if ((off + 1) == trans_count) {
			/* complete(&priv->master_test_data->done); ?? */
		} else {
			off = ++priv->master_test_data->cur_msg;
			priv->master_test_data->buf_count = 0;
			msg++;

			if ((off + 1) == trans_count) {
				if (msg->flags & I2C_M_READ) {
					hsi2c_set_auto_config(base, 1, 0, msg->length);
				} else {
					hsi2c_set_auto_config(base, 1, 1, msg->length);
				}
			} else {
				if (msg->flags & I2C_M_READ) {
					hsi2c_set_auto_config(base, 0, 0, msg->length);
				} else {
					hsi2c_set_auto_config(base, 0, 1, msg->length);
				}
			}

			if (msg->flags & I2C_M_READ) {
				hsi2c_set_trans_mode(base, 1, 0);
			} else {
				hsi2c_set_trans_mode(base, 1, 1);
			}

			hsi2c_run_auto_mode(base, 1);

		}
		hsi2c_clear_int(base, HSI2C_INT_TRANSFER_DONE_AUTO);
	}

	return 0;
}

static int hsi2c_slave_handler(void *args)
{
	struct s5j_i2c_priv_s *priv = args;
	unsigned int int_status;
	unsigned int val;
	unsigned int base = priv->config->base;

	int_status = hsi2c_read_int_status(base);
	if (int_status & HSI2C_INT_SLAVE_ADDR_MATCH) {
		val = getreg32(base + I2C_TRANS_STATUS);
		if (val & SLAVE_TX_MODE) {
			priv->slave_test_data->status = SLAVE_SET_DATA;
			hsi2c_enable_int(base, HSI2C_INT_TX_ALMOST_EMPTY);
		} else {
			priv->slave_test_data->status = SLAVE_GET_REG;
		}

		hsi2c_clear_int(base, HSI2C_INT_SLAVE_ADDR_MATCH);
	} else if (int_status & HSI2C_INT_RX_ALMOST_FULL) {
		if (priv->slave_test_data->status == SLAVE_GET_REG) {
			priv->slave_test_data->current_reg = getreg32(base + RXDATA);
			priv->slave_test_data->status = SLAVE_GET_DATA;
		} else if (priv->slave_test_data->status == SLAVE_GET_DATA) {
			priv->slave_test_data->data[priv->slave_test_data->current_reg] = getreg32(base + RXDATA);
			priv->slave_test_data->current_reg++;
		}
		hsi2c_clear_int(base, HSI2C_INT_RX_ALMOST_FULL);
	} else if (int_status & HSI2C_INT_TX_ALMOST_EMPTY) {
		if (getreg32(base + I2C_TRANS_STATUS) & STOP_COND) {
			hsi2c_disable_int(base, HSI2C_INT_TX_ALMOST_EMPTY);
		} else {
			if (priv->slave_test_data->status == SLAVE_SET_DATA) {
				putreg32(priv->slave_test_data->data[priv->slave_test_data->current_reg], base + TXDATA);
				priv->slave_test_data->current_reg++;
			}
		}
		hsi2c_clear_int(base, HSI2C_INT_TX_ALMOST_EMPTY);
	}
	if (int_status & HSI2C_INT_RX_OVERRUN) {
		/* for Rx_overrun test */
		hsi2c_clear_int(base, HSI2C_INT_RX_OVERRUN);
	}
	if (int_status & HSI2C_INT_TX_UNDERRUN) {
		/* for Tx_underrun test */
		hsi2c_clear_int(base, HSI2C_INT_RX_OVERRUN);
	}

	return 0;
}

static void hsi2c_set_auto_mode(unsigned int base)
{
	unsigned int val;

	val = getreg32(base + I2C_CONF);
	val |= (1 << 31);
	putreg32(val, base + I2C_CONF);
}

static void hsi2c_set_fifo_level(unsigned int base)
{
	putreg32(0x10013, base + FIFO_CTL);
}

static void hsi2c_master_setup(struct s5j_i2c_priv_s *priv, unsigned int mode,
				unsigned int speed, unsigned int slave_addr)
{
	if (priv->mode == I2C_POLLING) {
		priv->xfer_speed = speed;
		hsi2c_calculate_timing(priv->config->base, priv->clock, speed);
	}
#ifdef CONFIG_S5J_I2C_INTERRUPT_MODE
	else if (priv->mode == I2C_INTERRUPT) {
		priv->master_test_data = (struct master_data *)malloc(sizeof(struct master_data));
		/* complete_init(&priv->master_test_data->done); */
		hsi2c_set_trans_mode(priv->config->base, 1, 1);	/* set master mode */
		hsi2c_conf(priv->config->base, speed);
		hsi2c_calculate_timing(priv->config->base, priv->clock, speed);
		hsi2c_set_slave_addr(priv->config->base, slave_addr, 1);
		hsi2c_set_auto_mode(priv->config->base);
		hsi2c_set_fifo_level(priv->config->base);
	}
#endif
}

static void hsi2c_slave_setup(struct s5j_i2c_priv_s *priv, unsigned int mode,
				unsigned int speed, unsigned int slave_addr)
{
	priv->slave_test_data = (struct slave_data *)malloc(sizeof(struct slave_data));

	/* slave mode is only support slave mode */
	hsi2c_set_trans_mode(priv->config->base, 0, 0);	/* set slave mode */

	/*set hwacg for slave mode */
	hsi2c_set_hwacg_mode(priv->config->base, 1);

	hsi2c_conf(priv->config->base, speed);
	hsi2c_calculate_timing(priv->config->base, priv->clock, speed);
	hsi2c_set_slave_addr(priv->config->base, slave_addr, 0);

	hsi2c_disable_int(priv->config->base, HSI2C_INT_ALL);
	hsi2c_enable_int(priv->config->base, HSI2C_INT_SLAVE_ADDR_MATCH | HSI2C_INT_RX_ALMOST_FULL);

	hsi2c_set_fifo_level(priv->config->base);
	hsi2c_set_auto_config(priv->config->base, 0, 0, 0);

	hsi2c_set_trans_mode(priv->config->base, 0, 0);

#ifdef CONFIG_S5J_I2C_INTERRUPT_MODE
	if ((priv->master == I2C_SLAVE_MODE) || (priv->mode == I2C_INTERRUPT)) {
		irq_attach(priv->config->irq, priv->config->isr, NULL);
	}
#endif
}

static void hsi2c_master_cleanup(struct s5j_i2c_priv_s *priv)
{
#ifdef CONFIG_S5J_I2C_INTERRUPT_MODE
	if (priv->mode == I2C_INTERRUPT) {
		free(priv->master_test_data);
		priv->master_test_data = NULL;
		irq_detach(priv->config->irq);
	}
#endif
}

static void hsi2c_slave_cleanup(struct s5j_i2c_priv_s *priv)
{
	/*  FreeHwMemP(priv->slave_test_data); */
	free(priv->slave_test_data);
	priv->slave_test_data = NULL;
#ifdef CONFIG_S5J_I2C_INTERRUPT_MODE
	if ((priv->master == I2C_SLAVE_MODE) || (priv->mode = I2C_INTERRUPT)) {
		irq_detach(priv->config->irq);
	}
#endif
}

static int hsi2c_setup(struct s5j_i2c_priv_s *priv, unsigned int master,
				unsigned int mode, unsigned int speed, unsigned int slave_addr)
{
	priv->master = master;
	priv->mode = mode;
	priv->xfer_speed = speed;
	priv->slave_addr = slave_addr;

	hsi2c_manual_fast_init(priv);

	if (master == I2C_MASTER) {
		hsi2c_master_setup(priv, mode, speed, slave_addr);
	} else if (master == I2C_SLAVE_MODE) {
		hsi2c_slave_setup(priv, mode, speed, slave_addr);
	}

	return 0;
}

static int hsi2c_cleanup(struct s5j_i2c_priv_s *priv)
{
	if (priv->master == I2C_MASTER) {
		hsi2c_master_cleanup(priv);
	} else if (priv->master == I2C_SLAVE_MODE) {
		hsi2c_slave_cleanup(priv);
	}

	priv->master = I2C_MASTER;
	priv->mode = I2C_POLLING;
	priv->xfer_speed = I2C_SPEED_400KHZ;

	putreg32(0x80000000, priv->config->base + CTL);
	up_udelay(10);

	putreg32(0x0, priv->config->base + CTL);
	up_udelay(100);

	return 0;
}

static inline void s5j_i2c_sem_wait(struct s5j_i2c_priv_s *priv)
{
	while (sem_wait(&priv->exclsem) != 0) {
		ASSERT(errno == EINTR);
	}
}

static inline void s5j_i2c_sem_post(struct s5j_i2c_priv_s *priv)
{
	sem_post(&priv->exclsem);
}

static inline void s5j_i2c_sem_init(struct s5j_i2c_priv_s *priv)
{
	sem_init(&priv->exclsem, 0, 1);
#ifndef CONFIG_I2C_POLLED
	sem_init(&priv->waitsem, 0, 0);
#endif
}

static inline void s5j_i2c_sem_destroy(struct s5j_i2c_priv_s *priv)
{
	sem_destroy(&priv->exclsem);
#ifndef CONFIG_I2C_POLLED
	sem_destroy(&priv->waitsem);
#endif
}

static int s5j_i2c0_interrupt(int irq, void *context, void *arg)
{
	struct s5j_i2c_priv_s *priv;

	/* Read the masked interrupt status */
	priv = &s5j_i2c0_priv;

	/* Let the common interrupt handler do the rest of the work */
	return hsi2c_master_handler(priv);
}

static int s5j_i2c1_interrupt(int irq, void *context, void *arg)
{
	struct s5j_i2c_priv_s *priv;

	/* Read the masked interrupt status */
	priv = &s5j_i2c1_priv;

	/* Let the common interrupt handler do the rest of the work */
	return hsi2c_master_handler(priv);
}

static int s5j_i2c2_interrupt(int irq, void *context, void *arg)
{
	struct s5j_i2c_priv_s *priv;

	/* Read the masked interrupt status */
	priv = &s5j_i2c2_priv;

	/* Let the common interrupt handler do the rest of the work */
	return hsi2c_master_handler(priv);
}

static int s5j_i2c3_interrupt(int irq, void *context, void *arg)
{
	struct s5j_i2c_priv_s *priv;

	/* Read the masked interrupt status */
	priv = &s5j_i2c3_priv;

	/* Let the common interrupt handler do the rest of the work */
	return hsi2c_master_handler(priv);
}

static int s5j_i2c_initialize(struct s5j_i2c_priv_s *priv, unsigned int frequency)
{
	const struct s5j_i2c_config_s *config = priv->config;
	int ret;

	ret = s5j_configgpio(config->scl_pin);
	if (ret < 0) {
		lldbg("I2C%d: s5j_configgpio(%08x) failed: %d\n", config->scl_pin, ret);
		return ret;
	}

	ret = s5j_configgpio(config->sda_pin);
	if (ret < 0) {
		lldbg("I2C%d: s5j_configgpio(%08x) failed: %d\n", config->sda_pin, ret);
		return ret;
	}

	/* Enable the I2C master block */
	/* Configure the the initial I2C clock frequency. */
	priv->xfer_speed = frequency;
	hsi2c_setup(priv, priv->master, priv->mode, priv->xfer_speed, priv->slave_addr);

	/*
	 * Attach interrupt handlers and enable interrupts at the NVIC (still
	 * disabled at the source).
	 */
#ifdef CONFIG_S5J_I2C_INTERRUPT_MODE
	if ((priv->master == I2C_SLAVE_MODE) || (priv->mode == I2C_INTERRUPT)) {
		irq_attach(config->irq, config->isr, NULL);
		up_enable_irq(config->irq);
	}
#endif

	return OK;
}

static int s5j_i2c_uninitialize(struct s5j_i2c_priv_s *priv)
{
	lldbg("I2C%d: refs=%d\n", priv->config->devno, priv->refs);

	/* Disable I2C */
	hsi2c_cleanup(priv);

	/* Unconfigure GPIO pins */
	s5j_unconfiggpio(priv->config->scl_pin);
	s5j_unconfiggpio(priv->config->sda_pin);

	/* Disable and detach interrupts */
#ifdef CONFIG_S5J_I2C_INTERRUPT_MODE
	if ((priv->master == I2C_SLAVE_MODE) || (priv->mode == I2C_INTERRUPT)) {
		up_disable_irq(priv->config->irq);
		irq_detach(priv->config->irq);
	}
#endif

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/**
 * @brief    i2c clock setting
 * @param    struct i2c_dev_s *dev : pointer to i2c_dev_s
 * @param    unsigned int frequency : ex ) 100000 == 100khz
 * @return   void
 * @note
 */
unsigned int s5j_i2c_setclock(FAR struct i2c_dev_s *dev, unsigned int frequency)
{
	struct s5j_i2c_priv_s *priv = (struct s5j_i2c_priv_s *)dev;

	/* Has the I2C bus frequency changed? */
	if (frequency != priv->xfer_speed) {
		/*
		 * Calculate the clock divider that results in the highest frequency
		 * that is than or equal to the desired speed.
		 */
		if (priv->mode != I2C_POLLING) {
			hsi2c_conf(priv->config->base, frequency);
		}

		hsi2c_calculate_timing(priv->config->base, priv->clock, frequency);
	}

	/* Save the new I2C frequency */
	priv->xfer_speed = frequency;
	return 0;
}

/**
 * @brief    Setting Slave Address for slave mode
 * @param    struct i2c_dev_s *dev : pointer to i2c_dev_s
 * @param    int addr : Slave address
 * @param     int nbits : 0==7bit
 *          1==10bit
 * @return    = 0
 * @note
 */
int s5j_i2c_setownaddress(FAR struct i2c_dev_s *dev, int addr, int nbits)
{
	struct s5j_i2c_priv_s *priv = (struct s5j_i2c_priv_s *)dev;

	priv->slave_addr = addr;
	if (nbits == 1) {
		priv->msgv->flags |= I2C_M_TEN;
	}
	hsi2c_set_slave_addr(priv->config->base, addr, 0);
	return 0;
}

/**
 * @brief    Generic I2C transfer function
 * @param    struct i2c_master_s *dev : structure visible to the I2C client
 * @param    struct i2c_msg_s *msgv : I2C transaction segment
 * @param    int msgc :  length
 * @return   int : ==0 :OK
 * @note
 */
int s5j_i2c_transfer(struct i2c_dev_s *dev, struct i2c_msg_s *msgv, int msgc)
{
	struct s5j_i2c_priv_s *priv = (struct s5j_i2c_priv_s *)dev;
	struct i2c_msg_s *pmsg;
	int ret = OK;
	int i;
	int nak_ok;
	int start = 1;
	int stop = 1;
	unsigned int base = priv->config->base;

	/* Ensure that address or flags don't change meanwhile */
	s5j_i2c_sem_wait(priv);

	/*
	 * register address write for read, write with count 1,
	 * i2c_read is expected from writing with count '1',
	 * no stop for next transfer
	 */
	if (((msgv->flags & I2C_M_READ) != 1) && (msgv->length == 1) && (msgc == 1)) {
		stop = 0;
	}

	if ((msgv->flags & I2C_M_READ) && (msgc == 1)) {
		start = 0;
	}

	priv->mcnt = 0;
	priv->mptr = NULL;
	priv->msgv = msgv;
	priv->msgc = msgc;

	/* TODO: initialization */
	if (start) {
		hsi2c_start(priv);
	}

	if (priv->xfer_speed > I2C_SPEED_400KHZ) {
		hsi2c_conf(base, I2C_SPEED_400KHZ);
		putreg32((0xF << 24 | I2C_SEND_DATA), base + I2C_MANUAL_CMD);
		hsi2c_wait_xfer_noack(priv);

		hsi2c_conf(base, priv->xfer_speed);
		hsi2c_repstart(priv);
		hsi2c_wait_xfer_noack(priv);
	}

	for (i = 0; i < msgc; i++) {
		pmsg = &msgv[i];
		nak_ok = pmsg->flags & I2C_M_IGNORE_NAK;
		if (!(pmsg->flags & I2C_M_NOSTART)) {
			if ((i > 0) || (start == 0)) {
				hsi2c_repstart(priv);
				hsi2c_wait_xfer_done(priv);
			}
			ret = do_address(priv, pmsg);
			if ((ret != 0) && !nak_ok) {
				goto fail;
			}
		}
		if (pmsg->flags & I2C_M_READ) {
			/* read bytes into buffer */
			ret = readbytes(priv, pmsg);
			if (ret < pmsg->length) {
				if (ret >= 0) {
					return -EIO;
				}
				goto fail;
			}
		} else {
			/* write bytes from buffer */
			ret = sendbytes(priv, pmsg);
			if (ret < pmsg->length) {
				if (ret >= 0) {
					ret = -EIO;
				}
				goto fail;
			}
		}
	}
	ret = i;

fail:
	priv->mcnt = 0;
	priv->mptr = NULL;
	if (stop) {
		hsi2c_stop(priv);
	}

	/* Ensure that address or flags don't change meanwhile */
	s5j_i2c_sem_post(priv);

	return ret;
}

int s5j_i2c_read(FAR struct i2c_dev_s *dev, FAR uint8_t *buffer, int buflen)
{
	struct s5j_i2c_priv_s *priv = (struct s5j_i2c_priv_s *)dev;
	struct i2c_msg_s msg;
	unsigned int flags;

	/* 7- or 10-bit? */
	flags = (priv->addrlen == 10) ? I2C_M_TEN : 0;

	/* Setup for the transfer */
	msg.addr = priv->slave_addr, msg.flags = (flags | I2C_M_READ);
	msg.buffer = (FAR uint8_t *) buffer;
	msg.length = buflen;

	/*
	 * Then perform the transfer
	 *
	 * REVISIT:  The following two operations must become atomic in order to
	 * assure thread safety.
	 */

	return s5j_i2c_transfer(dev, &msg, 1);
}

int s5j_i2c_write(FAR struct i2c_dev_s *dev, FAR const uint8_t *buffer,
				int buflen)
{
	struct s5j_i2c_priv_s *priv = (struct s5j_i2c_priv_s *)dev;
	struct i2c_msg_s msg;

	/* Setup for the transfer */
	msg.addr = priv->slave_addr;
	msg.flags = (priv->addrlen == 10) ? I2C_M_TEN : 0;
	msg.buffer = (FAR uint8_t *) buffer; /* Override const */
	msg.length = buflen;

	/*
	 * Then perform the transfer
	 *
	 * REVISIT:  The following two operations must become atomic in order to
	 * assure thread safety.
	 */
	return s5j_i2c_transfer(dev, &msg, 1);
}

/**
 * @brief   Initialize one I2C bus
 * @param   int port :
 * @return  struct i2c_master_s : device structure
 * @note
 */
struct i2c_dev_s *up_i2cinitialize(int port)
{
	struct s5j_i2c_priv_s *priv = NULL;
	const struct s5j_i2c_config_s *config;
	int flags;

	/* Get I2C private structure */
	if (g_s5j_i2c_priv[port] != NULL) {
		return (FAR struct i2c_dev_s *)g_s5j_i2c_priv[port];
	}

	switch (port) {
	case 0:
		priv = &s5j_i2c0_priv;
		config = &s5j_i2c0_config;
		cal_clk_enable(gate_hsi2c0);
		priv->clock = cal_clk_getrate(gate_hsi2c0);
		break;

	case 1:
		priv = &s5j_i2c1_priv;
		config = &s5j_i2c1_config;
		cal_clk_enable(gate_hsi2c1);
		priv->clock = cal_clk_getrate(gate_hsi2c1);
		break;

	case 2:
		priv = &s5j_i2c2_priv;
		config = &s5j_i2c2_config;
		cal_clk_enable(gate_hsi2c2);
		priv->clock = cal_clk_getrate(gate_hsi2c2);
		break;

	case 3:
		priv = &s5j_i2c3_priv;
		config = &s5j_i2c3_config;
		cal_clk_enable(gate_hsi2c3);
		priv->clock = cal_clk_getrate(gate_hsi2c3);
		break;

	default:
		lldbg("I2C%d: ERROR: Not supported\n", port);
		return NULL;
	}

	/* Initialize private device structure */
	priv->ops = &s5j_i2c_ops;

	/*
	 * Initialize private data for the first time, increment reference count,
	 * power-up hardware and configure GPIOs.
	 */
	flags = irqsave();;

	priv->refs++;
	if (priv->refs == 1) {
		/* Initialize the device structure */
		priv->config = config;
		s5j_i2c_sem_init(priv);

		/* Initialize the I2C hardware */
		s5j_i2c_initialize(priv, priv->xfer_speed);
	}

	g_s5j_i2c_priv[port] = priv;
	irqrestore(flags);

	return (FAR struct i2c_dev_s *)priv;
}

/**
 * @brief    Unitialize one I2C bus
 * @param    struct i2c_master_s *dev :
 * @return   ==0 : OK
 * @note
 */

int s5j_i2cbus_uninitialize(struct i2c_dev_s *dev)
{
	struct s5j_i2c_priv_s *priv = (struct s5j_i2c_priv_s *)dev;
	int flags;

	DEBUGASSERT(priv && priv->config && priv->refs > 0);

	lldbg("I2C%d: Uninitialize\n", priv->config->devno);

	/* Decrement reference count and check for underflow */
	flags = irqsave();

	/* Check if the reference count will decrement to zero */
	if (priv->refs < 2) {
		/* Yes.. Disable power and other HW resource (GPIO's) */
		s5j_i2c_uninitialize(priv);
		priv->refs = 0;

		/* Release unused resources */
		s5j_i2c_sem_destroy(priv);
	} else {
		/* No.. just decrement the number of references to the device */
		priv->refs--;
	}

	irqrestore(flags);
	return OK;
}

/**
 * @brief   Unitialize one I2C bus for the I2C character driver
 * @param   struct i2c_master_s *dev :
 * @return  ==0 : OK
 * @note
 */
void s5j_i2c_register(int bus)
{
	FAR struct i2c_dev_s *i2c;
	char path[16];
	int ret;

	lldbg("\ns5j_i2c_register start : %d ", bus);
	i2c = up_i2cinitialize(bus);
	if (i2c == NULL) {
		lldbg("ERROR: Failed to get I2C%d interface\n", bus);
	} else {
		snprintf(path, 16, "/dev/i2c-%d", bus);
		ret = i2c_uioregister(path, i2c);
		if (ret < 0) {
			lldbg("ERROR: Failed to register I2C%d driver: %d\n", bus, ret);
			s5j_i2cbus_uninitialize(i2c);
		} else {
			lldbg("register I2C%d driver: %d\n", bus, ret);
		}
	}
}
