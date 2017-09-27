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
#include <semaphore.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>
#include <tinyara/arch.h>
#include <tinyara/i2c.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "s5j_gpio.h"
#include "s5j_i2c.h"
#include "s5j_clock.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define S5J_DEFAULT_I2CXFER_CLOCK	(100 * 1000)	/* 100Khz */
#define S5J_DEFAULT_I2CSLAVE_ADDR	0x22
#define S5J_DEFAULT_I2C_TIMEOUT		10000
#define S5J_DEFAULT_HS_CLOCK		400000	/* 400Khz */
#define S5J_DEFAULT_HS_CLOCK		400000	/* 400Khz */

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct s5j_i2c_priv_s *g_s5j_i2c_priv[4] = { NULL };

/* I2C Device hardware configuration */
struct s5j_i2c_config_s {
	uintptr_t base;				/* I2C base address */
	unsigned int scl_pin;		/* GPIO configuration for SCL as SCL */
	unsigned int sda_pin;		/* GPIO configuration for SDA as SDA */
};

/* I2C Device Private Data */
struct s5j_i2c_priv_s {
	const struct i2c_ops_s *ops;	/* Standard I2C operations */
	const struct s5j_i2c_config_s *config;	/* Port configuration */
	sem_t sem_excl;				/* Mutual exclusion semaphore */
#ifndef CONFIG_I2C_POLLED
	sem_t sem_isr;				/* Interrupt wait semaphore */
#endif
	uint8_t refs;				/* Reference count */

	int clock;
	int xfer_speed;
	unsigned int slave_addr;
	unsigned int timeout;

	unsigned int initialized;
	unsigned int retries;
	/*  master data  */
	uint8_t msgc;				/* Message count */
	struct i2c_msg_s *msgv;		/* Message list */
	uint8_t *ptr;				/* Current message buffer */
	int mcnt;					/* Current message length */
	uint16_t mflags;			/* Current message flags */

	struct i2c_msg_s *msg;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static void hsi2c_set_hs_timing(struct s5j_i2c_priv_s *priv, unsigned int nClkDiv, unsigned int tSTART_SU, unsigned int tSTART_HD, unsigned int tSTOP_SU, unsigned int tDATA_SU, unsigned int tDATA_HD, unsigned int tSCL_L, unsigned int tSCL_H, unsigned int tSR_RELEASE);
static void hsi2c_set_fs_timing(struct s5j_i2c_priv_s *priv, unsigned int nClkDiv, unsigned int tSTART_SU, unsigned int tSTART_HD, unsigned int tSTOP_SU, unsigned int tDATA_SU, unsigned int tDATA_HD, unsigned int tSCL_L, unsigned int tSCL_H, unsigned int tSR_RELEASE);
static void hsi2c_calculate_timing(struct s5j_i2c_priv_s *priv, unsigned int nOpClk);
static void hsi2c_conf(struct s5j_i2c_priv_s *priv, unsigned int nOpClk);
static void hsi2c_enable_int(struct s5j_i2c_priv_s *priv, unsigned int bit);
static void hsi2c_set_slave_addr(struct s5j_i2c_priv_s *priv, u16 addr);
static int hsi2c_manual_fast_init(struct s5j_i2c_priv_s *priv);
static int hsi2c_wait_xfer_done(struct s5j_i2c_priv_s *priv);
static int hsi2c_wait_xfer_noack(struct s5j_i2c_priv_s *priv);
static void hsi2c_start(struct s5j_i2c_priv_s *priv);
static void hsi2c_stop(struct s5j_i2c_priv_s *priv);
static void hsi2c_repstart(struct s5j_i2c_priv_s *priv);
static int hsi2c_outb(struct s5j_i2c_priv_s *priv, u8 data);
static int hsi2c_inb(struct s5j_i2c_priv_s *priv, bool is_ack);
static int sendbytes(struct s5j_i2c_priv_s *priv, struct i2c_msg_s *msg);
static int readbytes(struct s5j_i2c_priv_s *priv, struct i2c_msg_s *msg);
static int try_address(struct s5j_i2c_priv_s *priv, u8 addr, int retries);
static int do_address(struct s5j_i2c_priv_s *priv, struct i2c_msg_s *msg);
static int hsi2c_setup(struct s5j_i2c_priv_s *priv);
static int hsi2c_cleanup(struct s5j_i2c_priv_s *priv);
static inline void s5j_i2c_sem_wait(struct s5j_i2c_priv_s *priv);
static inline void s5j_i2c_sem_post(struct s5j_i2c_priv_s *priv);
static inline void s5j_i2c_sem_init(struct s5j_i2c_priv_s *priv);
static inline void s5j_i2c_sem_destroy(struct s5j_i2c_priv_s *priv);
static int s5j_i2c_initialize(struct s5j_i2c_priv_s *priv);
static int s5j_i2c_uninitialize(struct s5j_i2c_priv_s *priv);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* I2C Interface */
static const struct i2c_ops_s s5j_i2c_ops = {
	.setfrequency = s5j_i2c_setclock,
	.setaddress = s5j_i2c_setaddress,
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
};

static struct s5j_i2c_priv_s s5j_i2c0_priv = {
	.xfer_speed = S5J_DEFAULT_I2CXFER_CLOCK,
	.slave_addr = S5J_DEFAULT_I2CSLAVE_ADDR,
	.timeout = S5J_DEFAULT_I2C_TIMEOUT,
	.initialized = 0,
	.retries = 3,
};

static const struct s5j_i2c_config_s s5j_i2c1_config = {
	.base = S5J_HSI2C1_BASE,
	.scl_pin = GPIO_I2C1_SCL,
	.sda_pin = GPIO_I2C1_SDA,
};

static struct s5j_i2c_priv_s s5j_i2c1_priv = {
	.xfer_speed = S5J_DEFAULT_I2CXFER_CLOCK,
	.slave_addr = S5J_DEFAULT_I2CSLAVE_ADDR,
	.timeout = S5J_DEFAULT_I2C_TIMEOUT,
	.initialized = 0,
	.retries = 3,
};

static const struct s5j_i2c_config_s s5j_i2c2_config = {
	.base = S5J_HSI2C2_BASE,
	.scl_pin = GPIO_I2C2_SCL,
	.sda_pin = GPIO_I2C2_SDA,
};

static struct s5j_i2c_priv_s s5j_i2c2_priv = {
	.xfer_speed = S5J_DEFAULT_I2CXFER_CLOCK,
	.slave_addr = S5J_DEFAULT_I2CSLAVE_ADDR,
	.timeout = S5J_DEFAULT_I2C_TIMEOUT,
	.initialized = 0,
	.retries = 3,
};

static const struct s5j_i2c_config_s s5j_i2c3_config = {
	.base = S5J_HSI2C3_BASE,
	.scl_pin = GPIO_I2C3_SCL,
	.sda_pin = GPIO_I2C3_SDA,
};

static struct s5j_i2c_priv_s s5j_i2c3_priv = {
	.xfer_speed = S5J_DEFAULT_I2CXFER_CLOCK,
	.slave_addr = S5J_DEFAULT_I2CSLAVE_ADDR,
	.timeout = S5J_DEFAULT_I2C_TIMEOUT,
	.initialized = 0,
	.retries = 3,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static uint32_t i2c_getreg32(struct s5j_i2c_priv_s *priv, int offset)
{
	return getreg32(priv->config->base + offset);
}

static void i2c_putreg32(struct s5j_i2c_priv_s *priv, int offset, uint32_t value)
{
	putreg32(value, priv->config->base + offset);
}

static void hsi2c_set_hs_timing(struct s5j_i2c_priv_s *priv, unsigned int nClkDiv, unsigned int tSTART_SU, unsigned int tSTART_HD, unsigned int tSTOP_SU, unsigned int tDATA_SU, unsigned int tDATA_HD, unsigned int tSCL_L, unsigned int tSCL_H, unsigned int tSR_RELEASE)
{
	i2c_putreg32(priv, S5J_I2C_TIMING_HS1, I2C_TIMING_TSTART_SU(tSTART_SU) | I2C_TIMING_TSTART_HD(tSTART_HD) | I2C_TIMING_TSTOP_SU(tSTOP_SU));

	i2c_putreg32(priv, S5J_I2C_TIMING_HS2, I2C_TIMING_TDATA_SU(tDATA_SU) | I2C_TIMING_TSCL_L(tSCL_L) | I2C_TIMING_TSCL_H(tSCL_H));

	i2c_putreg32(priv, S5J_I2C_TIMING_HS3, I2C_TIMING_CLK_DIV(nClkDiv) | I2C_TIMING_TSR_RELEASE(tSR_RELEASE));

	i2c_putreg32(priv, S5J_I2C_TIMING_SLA, I2C_TIMING_TDATA_HD_SLA(tDATA_HD));
}

static void hsi2c_set_fs_timing(struct s5j_i2c_priv_s *priv, unsigned int nClkDiv, unsigned int tSTART_SU, unsigned int tSTART_HD, unsigned int tSTOP_SU, unsigned int tDATA_SU, unsigned int tDATA_HD, unsigned int tSCL_L, unsigned int tSCL_H, unsigned int tSR_RELEASE)
{
	i2c_putreg32(priv, S5J_I2C_TIMING_FS1, I2C_TIMING_TSTART_SU(tSTART_SU) | I2C_TIMING_TSTART_HD(tSTART_HD) | I2C_TIMING_TSTOP_SU(tSTOP_SU));

	i2c_putreg32(priv, S5J_I2C_TIMING_FS2, I2C_TIMING_TDATA_SU(tDATA_SU) | I2C_TIMING_TSCL_L(tSCL_L) | I2C_TIMING_TSCL_H(tSCL_H));

	i2c_putreg32(priv, S5J_I2C_TIMING_FS3, I2C_TIMING_CLK_DIV(nClkDiv) | I2C_TIMING_TSR_RELEASE(tSR_RELEASE));

	i2c_putreg32(priv, S5J_I2C_TIMING_SLA, I2C_TIMING_TDATA_HD_SLA(tDATA_HD));
}

static void hsi2c_calculate_timing(struct s5j_i2c_priv_s *priv, unsigned int nOpClk)
{
	unsigned int reg;
	unsigned int nClkDiv;
	unsigned int tFTL_CYCLE_SCL;
	s32 i;
	float uTemp0;
	s32 uTemp1;
	s32 uTemp2 = 0;

	reg = i2c_getreg32(priv, S5J_I2C_CONF);
	reg &= ~(0x7 << 13);
	i2c_putreg32(priv, S5J_I2C_CONF, reg);

	reg = i2c_getreg32(priv, S5J_I2C_CONF);
	reg &= ~(0x7 << 16);
	i2c_putreg32(priv, S5J_I2C_CONF, reg);

	tFTL_CYCLE_SCL = (i2c_getreg32(priv, S5J_I2C_CONF) >> 16) & 0x7;

	uTemp0 = (priv->clock / nOpClk) - (tFTL_CYCLE_SCL + 3) * 2;

	for (i = 0; i < 256; i++) {
		uTemp1 = ((int)uTemp0 + ((tFTL_CYCLE_SCL + 3) % (i + 1)) * 2) / (i + 1);
		if (uTemp1 < 512) {		/*  TSCL_L/H max is 512 / 2 */
			uTemp2 = uTemp1 - 2;
			break;
		}
	}

	unsigned int tSCL_H;
	nClkDiv = i;
	if (nOpClk > S5J_DEFAULT_HS_CLOCK) {
		tSCL_H = ((uTemp2 + 10) / 3) - 5;
	} else {
		tSCL_H = uTemp2 / 2;
	}

	unsigned int tSCL_L = uTemp2 - tSCL_H;
	unsigned int tSTART_SU = tSCL_L;
	unsigned int tSTART_HD = tSCL_L;
	unsigned int tSTOP_SU = tSCL_L;
	unsigned int tDATA_SU = tSCL_L;
	unsigned int tDATA_HD = tSCL_L / 2;
	unsigned int tSR_RELEASE = uTemp2;

	if (nOpClk > S5J_DEFAULT_HS_CLOCK) {
		/* 400Khz setting for Extended ID */
		hsi2c_set_fs_timing(priv, 1, 38, 38, 38, 19, 19, 38, 38, 76);
		hsi2c_set_hs_timing(priv, nClkDiv, tSTART_SU, tSTART_HD, tSTOP_SU, tDATA_SU, tDATA_HD, tSCL_L, tSCL_H, tSR_RELEASE);
	} else {
		hsi2c_set_fs_timing(priv, nClkDiv, tSTART_SU, tSTART_HD, tSTOP_SU, tDATA_SU, tDATA_HD, tSCL_L, tSCL_H, tSR_RELEASE);
	}
}

static void hsi2c_conf(struct s5j_i2c_priv_s *priv, unsigned int nOpClk)
{
	unsigned int val;

	val = i2c_getreg32(priv, S5J_I2C_CONF);
	val &= ~(I2C_CONF_ADDRMODE | I2C_CONF_AUTO_MODE);

	if (nOpClk > S5J_DEFAULT_HS_CLOCK) {
		val |= I2C_CONF_HS_MODE;
	} else {
		val &= ~I2C_CONF_HS_MODE;
	}

	i2c_putreg32(priv, S5J_I2C_CONF, val);
}

static void hsi2c_enable_int(struct s5j_i2c_priv_s *priv, unsigned int bit)
{
	unsigned int val;

	val = i2c_getreg32(priv, S5J_I2C_INT_EN);
	val |= bit;

	i2c_putreg32(priv, S5J_I2C_INT_EN, val);
}

static void hsi2c_set_slave_addr(struct s5j_i2c_priv_s *priv, u16 addr)
{
	i2c_putreg32(priv, S5J_I2C_ADDR, I2C_ADDR_SLAVE_ADDR_SLA(addr));
}

static int hsi2c_manual_fast_init(struct s5j_i2c_priv_s *priv)
{
	unsigned int val;

	hsi2c_conf(priv, priv->xfer_speed);
	hsi2c_calculate_timing(priv, priv->xfer_speed);
	hsi2c_enable_int(priv, I2C_INT_TRANSFER_DONE_MANUAL_EN | I2C_INT_TRANSFER_DONE_NOACK_MANUAL_EN);

	priv->initialized = 1;

	val = i2c_getreg32(priv, S5J_I2C_CONF);
	val &= ~(I2C_CONF_AUTO_MODE | I2C_CONF_HS_MODE);
	i2c_putreg32(priv, S5J_I2C_CONF, val);

	hsi2c_enable_int(priv, I2C_INT_TRANSFER_DONE_NOACK_MANUAL_EN | I2C_INT_TRANSFER_DONE_MANUAL_EN);

	return OK;
}

static int hsi2c_wait_xfer_done(struct s5j_i2c_priv_s *priv)
{
	int val;
	int timeout = priv->timeout;

	while (timeout-- > 0) {
		val = i2c_getreg32(priv, S5J_I2C_INT_STAT) & I2C_INT_TRANSFER_DONE_MANUAL_EN;
		if (val) {
			i2c_putreg32(priv, S5J_I2C_INT_STAT, val);
			return (val == I2C_INT_TRANSFER_DONE_MANUAL_EN);
		}
	}

	return -ETIMEDOUT;
}

static int hsi2c_wait_xfer_noack(struct s5j_i2c_priv_s *priv)
{
	int val;
	int timeout = priv->timeout;

	while (timeout-- > 0) {
		val = i2c_getreg32(priv, S5J_I2C_INT_STAT) & I2C_INT_TRANSFER_DONE_NOACK_MANUAL_EN;
		if (val) {
			i2c_putreg32(priv, S5J_I2C_INT_STAT, val);
			return (val == I2C_INT_TRANSFER_DONE_NOACK_MANUAL_EN);
		}
	}

	return -ETIMEDOUT;
}

static void hsi2c_start(struct s5j_i2c_priv_s *priv)
{
	i2c_putreg32(priv, S5J_I2C_CTL, I2C_CTL_MASTER | I2C_CTL_TXCHON);	/* 0x88 */

	i2c_putreg32(priv, S5J_I2C_MANUAL_CMD, I2C_MANUAL_CMD_SEND_START);

	hsi2c_wait_xfer_done(priv);
}

static void hsi2c_stop(struct s5j_i2c_priv_s *priv)
{
	i2c_putreg32(priv, S5J_I2C_MANUAL_CMD, I2C_MANUAL_CMD_SEND_STOP);

	hsi2c_wait_xfer_done(priv);
}

static void hsi2c_repstart(struct s5j_i2c_priv_s *priv)
{
	i2c_putreg32(priv, S5J_I2C_MANUAL_CMD, I2C_MANUAL_CMD_SEND_RESTART);
}

static int hsi2c_outb(struct s5j_i2c_priv_s *priv, u8 data)
{
	unsigned int val;
	int ret;

	val = I2C_MANUAL_CMD_TX_DATA(data) | I2C_MANUAL_CMD_SEND_DATA;
	i2c_putreg32(priv, S5J_I2C_MANUAL_CMD, val);

	ret = hsi2c_wait_xfer_done(priv);

	return ret;
}

static int hsi2c_inb(struct s5j_i2c_priv_s *priv, bool is_ack)
{
	unsigned int val = I2C_MANUAL_CMD_READ_DATA;
	u8 data;
	int ret;

	/* Looks awkward, but if I2C_MANUAL_CMD_RX_ACK is set, ACK is NOT generated */
	if (!is_ack) {
		val |= I2C_MANUAL_CMD_RX_ACK;
	}
	i2c_putreg32(priv, S5J_I2C_MANUAL_CMD, val);

	ret = hsi2c_wait_xfer_done(priv);
	if (ret < 0) {
		return ret;				/* timeout */
	}

	data = (i2c_getreg32(priv, S5J_I2C_MANUAL_CMD) >> 16) & 0xFF;

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
			return -EIO;
		} else {
			/* Timeout */
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

		ret = try_address(priv, addr, retries);
		if ((ret != 1) && !nak_ok) {
			return -ENXIO;
		}

		/* the remaining 8 bit address */
		ret = hsi2c_outb(priv, msg->addr);
		if ((ret != 1) && !nak_ok) {
			return -ENXIO;
		}

		if (flags & I2C_M_READ) {
			hsi2c_repstart(priv);
			hsi2c_wait_xfer_done(priv);
			addr |= 0x1;
			ret = try_address(priv, addr, retries);
			if ((ret != 1) && !nak_ok) {
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

	return OK;
}

static int hsi2c_setup(struct s5j_i2c_priv_s *priv)
{
	hsi2c_manual_fast_init(priv);

	hsi2c_calculate_timing(priv, priv->xfer_speed);

	return OK;
}

static int hsi2c_cleanup(struct s5j_i2c_priv_s *priv)
{
	priv->xfer_speed = S5J_DEFAULT_HS_CLOCK;

	i2c_putreg32(priv, S5J_I2C_CTL, I2C_CTL_SW_RST);
	up_udelay(10);

	i2c_putreg32(priv, S5J_I2C_CTL, I2C_CTL_RESET_VALUE);
	up_udelay(100);

	return OK;
}

static inline void s5j_i2c_sem_wait(struct s5j_i2c_priv_s *priv)
{
	while (sem_wait(&priv->sem_excl) != 0) {
		ASSERT(errno == EINTR);
	}
}

static inline void s5j_i2c_sem_post(struct s5j_i2c_priv_s *priv)
{
	sem_post(&priv->sem_excl);
}

static inline void s5j_i2c_sem_init(struct s5j_i2c_priv_s *priv)
{
	sem_init(&priv->sem_excl, 0, 1);
#ifndef CONFIG_I2C_POLLED
	sem_init(&priv->sem_isr, 0, 0);
	sem_setprotocol(&priv->sem_isr, SEM_PRIO_NONE);
#endif
}

static inline void s5j_i2c_sem_destroy(struct s5j_i2c_priv_s *priv)
{
	sem_destroy(&priv->sem_excl);
#ifndef CONFIG_I2C_POLLED
	sem_destroy(&priv->sem_isr);
#endif
}

static int s5j_i2c_initialize(struct s5j_i2c_priv_s *priv)
{
	/* Configure GPIO pins */
	s5j_configgpio(priv->config->scl_pin);
	s5j_configgpio(priv->config->sda_pin);

	/* Enable I2C */
	hsi2c_setup(priv);

	return OK;
}

static int s5j_i2c_uninitialize(struct s5j_i2c_priv_s *priv)
{
	/* Disable I2C */
	hsi2c_cleanup(priv);

	/* Unconfigure GPIO pins */
	s5j_unconfiggpio(priv->config->scl_pin);
	s5j_unconfiggpio(priv->config->sda_pin);

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
		hsi2c_calculate_timing(priv, frequency);
	}

	/* Save the new I2C frequency */
	priv->xfer_speed = frequency;
	return OK;
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
int s5j_i2c_setaddress(FAR struct i2c_dev_s *dev, int addr, int nbits)
{
	struct s5j_i2c_priv_s *priv = (struct s5j_i2c_priv_s *)dev;

	priv->slave_addr = addr;
	if (nbits == 1) {
		priv->msgv->flags |= I2C_M_TEN;
	}
	hsi2c_set_slave_addr(priv, addr);
	return OK;
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
	priv->ptr = NULL;
	priv->msgv = msgv;
	priv->msgc = msgc;

	/* TODO: initialization */
	if (start) {
		hsi2c_start(priv);
	}

	if (priv->xfer_speed > S5J_DEFAULT_HS_CLOCK) {
		hsi2c_conf(priv, S5J_DEFAULT_HS_CLOCK);
		i2c_putreg32(priv, S5J_I2C_MANUAL_CMD, I2C_MANUAL_CMD_TX_DATA(0xF) | I2C_MANUAL_CMD_SEND_DATA);
		hsi2c_wait_xfer_noack(priv);

		hsi2c_conf(priv, priv->xfer_speed);
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
					ret = -EIO;
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
	priv->ptr = NULL;
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

	flags = 0;

	/* Setup for the transfer */
	msg.addr = priv->slave_addr, msg.flags = (flags | I2C_M_READ);
	msg.buffer = (FAR uint8_t *)buffer;
	msg.length = buflen;

	/*
	 * Then perform the transfer
	 *
	 * REVISIT:  The following two operations must become atomic in order to
	 * assure thread safety.
	 */

	return s5j_i2c_transfer(dev, &msg, 1);
}

int s5j_i2c_write(FAR struct i2c_dev_s *dev, FAR const uint8_t *buffer, int buflen)
{
	struct s5j_i2c_priv_s *priv = (struct s5j_i2c_priv_s *)dev;
	struct i2c_msg_s msg;

	/* Setup for the transfer */
	msg.addr = priv->slave_addr;
	msg.flags = 0;
	msg.buffer = (FAR uint8_t *)buffer;	/* Override const */
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
	int total_ports = sizeof(g_s5j_i2c_priv) / sizeof(g_s5j_i2c_priv[0]);

	/* check if port is valid */
	if (port < 0 || port >= total_ports) {
		return NULL;
	}

	/* Get I2C private structure */
	if (g_s5j_i2c_priv[port] != NULL) {
		priv = g_s5j_i2c_priv[port];
		priv->refs++;
		return (FAR struct i2c_dev_s *)priv;
	}

	switch (port) {
	case 0:
		priv = &s5j_i2c0_priv;
		config = &s5j_i2c0_config;
		priv->clock = s5j_clk_get_rate(CLK_SPL_BUS_P0);
		break;

	case 1:
		priv = &s5j_i2c1_priv;
		config = &s5j_i2c1_config;
		priv->clock = s5j_clk_get_rate(CLK_SPL_BUS_P0);
		break;

	case 2:
		priv = &s5j_i2c2_priv;
		config = &s5j_i2c2_config;
		priv->clock = s5j_clk_get_rate(CLK_SPL_BUS_P0);
		break;

	case 3:
		priv = &s5j_i2c3_priv;
		config = &s5j_i2c3_config;
		priv->clock = s5j_clk_get_rate(CLK_SPL_BUS_P0);
		break;

	default:
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
		s5j_i2c_initialize(priv);
	}

	g_s5j_i2c_priv[port] = priv;
	irqrestore(flags);

	return (FAR struct i2c_dev_s *)priv;
}

int up_i2cuninitialize(FAR struct i2c_dev_s *dev)
{
	return s5j_i2cbus_uninitialize(dev);
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

	i2c = up_i2cinitialize(bus);
	if (i2c != NULL) {
		snprintf(path, 16, "/dev/i2c-%d", bus);
		if (i2c_uioregister(path, i2c) < 0) {
			s5j_i2cbus_uninitialize(i2c);
		}
	}
}
