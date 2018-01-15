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

#define S5J_I2C_DEFAULT_XFER_CLOCK	(100 * 1000)	/* 100Khz */
#define S5J_I2C_DEFAULT_SLAVE_ADDR	0x22
#define S5J_I2C_DEFAULT_TIMEOUT		10000
#define S5J_I2C_DEFAULT_RETRY		3
#define S5J_I2C_DEFAULT_TYPE		I2C_MASTER_TYPE
#define S5J_I2C_DEFAULT_MODE		I2C_POLLING_MODE
#define S5J_I2C_DEFAULT_HS_CLOCK	400000			/* 400Khz */

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* I2C Device Hardware Configuration */
struct s5j_i2c_config_s {
	uintptr_t base;						/* I2C base address */
	uint8_t port;
	unsigned int freqid;
	s32 scl_pin;				/* GPIO configuration for SCL as SCL */
	s32 sda_pin;				/* GPIO configuration for SDA as SDA */
#ifndef CONFIG_I2C_POLLED
	uint32_t irq;				/* IRQ number */
#endif
};

/* I2C Device Private Data */
struct s5j_i2c_priv_s {
	const struct i2c_ops_s *ops;	/* Standard I2C operations */
	const struct s5j_i2c_config_s *config;	/* Port Configuration */

	sem_t sem_excl;					/* Mutual exclusion semaphore */
#ifndef CONFIG_I2C_POLLED
	sem_t sem_isr;					/* Interrupt wait semaphore */
#endif
	uint8_t refs;					/* Reference count */

	int xfer_speed;
	u32 master;
	u32 mode;
	unsigned int slave_addr;
	unsigned int timeout;

	unsigned int retries;
	/*  master data  */
	uint8_t msgc;					/* Message count */
	struct i2c_msg_s *msgv;			/* Message list */
	uint8_t *ptr;					/* Current message buffer */
	int mcnt;						/* Current message length */
	uint16_t mflags;				/* Current message flags */

	struct i2c_msg_s *msg;
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

static void i2c_modifyreg32(struct s5j_i2c_priv_s *priv, int offset,
				uint32_t clearbits, uint32_t setbits)
{
	modifyreg32(priv->config->base + offset, clearbits, setbits);
}

static void hsi2c_set_hs_timing(struct s5j_i2c_priv_s *priv, unsigned int nClkDiv,
				unsigned int tSTART_SU, unsigned int tSTART_HD,
				unsigned int tSTOP_SU, unsigned int tDATA_SU,
				unsigned int tDATA_HD, unsigned int tSCL_L,
				unsigned int tSCL_H, unsigned int tSR_RELEASE)
{
	i2c_putreg32(priv, S5J_I2C_TIMING_HS1_OFFSET,
				I2C_TIMING_TSTART_SU(tSTART_SU) |
				I2C_TIMING_TSTART_HD(tSTART_HD) |
				I2C_TIMING_TSTOP_SU(tSTOP_SU));

	i2c_putreg32(priv, S5J_I2C_TIMING_HS2_OFFSET,
				I2C_TIMING_TDATA_SU(tDATA_SU) |
				I2C_TIMING_TSCL_L(tSCL_L) |
				I2C_TIMING_TSCL_H(tSCL_H));

	i2c_putreg32(priv, S5J_I2C_TIMING_HS3_OFFSET,
				I2C_TIMING_CLK_DIV(nClkDiv) |
				I2C_TIMING_TSR_RELEASE(tSR_RELEASE));

	i2c_putreg32(priv, S5J_I2C_TIMING_SLA_OFFSET,
				I2C_TIMING_TDATA_HD_SLA(tDATA_HD));
}

static void hsi2c_set_fs_timing(struct s5j_i2c_priv_s *priv, unsigned int nClkDiv,
				unsigned int tSTART_SU, unsigned int tSTART_HD,
				unsigned int tSTOP_SU, unsigned int tDATA_SU,
				unsigned int tDATA_HD, unsigned int tSCL_L,
				unsigned int tSCL_H, unsigned int tSR_RELEASE)
{
	i2c_putreg32(priv, S5J_I2C_TIMING_FS1_OFFSET,
				I2C_TIMING_TSTART_SU(tSTART_SU) |
				I2C_TIMING_TSTART_HD(tSTART_HD) |
				I2C_TIMING_TSTOP_SU(tSTOP_SU));

	i2c_putreg32(priv, S5J_I2C_TIMING_FS2_OFFSET,
				I2C_TIMING_TDATA_SU(tDATA_SU) |
				I2C_TIMING_TSCL_L(tSCL_L) |
				I2C_TIMING_TSCL_H(tSCL_H));

	i2c_putreg32(priv, S5J_I2C_TIMING_FS3_OFFSET,
				I2C_TIMING_CLK_DIV(nClkDiv) |
				I2C_TIMING_TSR_RELEASE(tSR_RELEASE));

	i2c_putreg32(priv, S5J_I2C_TIMING_SLA_OFFSET,
				I2C_TIMING_TDATA_HD_SLA(tDATA_HD));
}

static void hsi2c_calculate_timing(struct s5j_i2c_priv_s *priv, unsigned int nOpClk)
{
	unsigned int nClkDiv;
	unsigned int tFTL_CYCLE_SCL;
	s32 i;
	float uTemp0;
	s32 uTemp1;
	s32 uTemp2 = 0;

	/* FILTER_EN_SDA disable */
	i2c_modifyreg32(priv, S5J_I2C_CONF_OFFSET,
					I2C_CONF_FLT_CYCLE_SDA_MASK,
					I2C_CONF_FLT_CYCLE_SDA(0));

	/* FILTER_EN_SCL disable */
	i2c_modifyreg32(priv, S5J_I2C_CONF_OFFSET,
					I2C_CONF_FLT_CYCLE_SCL_MASK,
					I2C_CONF_FLT_CYCLE_SCL(0));

	tFTL_CYCLE_SCL = ((i2c_getreg32(priv, S5J_I2C_CONF_OFFSET) >>
					I2C_CONF_FLT_CYCLE_SCL_SHIFT) & I2C_CONF_FLT_CYCLE_SCL_BITS);

	uTemp0 = (s5j_clk_get_rate(priv->config->freqid) / nOpClk) - (tFTL_CYCLE_SCL + 3) * 2;

	for (i = 0; i < 256; i++) {
		uTemp1 = ((int)uTemp0 + ((tFTL_CYCLE_SCL + 3) % (i + 1)) * 2) / (i + 1);
		if (uTemp1 < 512) {		/*  TSCL_L/H max is 512 / 2 */
			uTemp2 = uTemp1 - 2;
			break;
		}
	}

	unsigned int tSCL_H;
	nClkDiv = i;
	if (nOpClk > S5J_I2C_DEFAULT_HS_CLOCK) {
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

	if (nOpClk > S5J_I2C_DEFAULT_HS_CLOCK) {
		/* 400Khz setting for Extended ID */
		/* High Speed Mode */
		hsi2c_set_fs_timing(priv, 1, 38, 38, 38, 19, 19, 38, 38, 76);
		hsi2c_set_hs_timing(priv, nClkDiv, tSTART_SU, tSTART_HD, tSTOP_SU,
					tDATA_SU, tDATA_HD, tSCL_L, tSCL_H, tSR_RELEASE);
	} else {
		/* Fast Speed Mode */
		hsi2c_set_fs_timing(priv, nClkDiv, tSTART_SU, tSTART_HD, tSTOP_SU,
					tDATA_SU, tDATA_HD, tSCL_L, tSCL_H, tSR_RELEASE);
	}
}

static void hsi2c_conf(struct s5j_i2c_priv_s *priv, unsigned int nOpClk)
{
	i2c_modifyreg32(priv, S5J_I2C_CONF_OFFSET,
					I2C_CONF_ADDRMODE_MASK |
					I2C_CONF_AUTO_MODE_MASK,
					0x0);

	if (nOpClk > S5J_I2C_DEFAULT_HS_CLOCK) {
		/* High Speed Mode */
		i2c_modifyreg32(priv, S5J_I2C_CONF_OFFSET,
						I2C_CONF_HS_MODE_MASK,
						I2C_CONF_HS_MODE_HIGH);
	} else {
		/* Fast Speed Mode */
		i2c_modifyreg32(priv, S5J_I2C_CONF_OFFSET,
						I2C_CONF_HS_MODE_MASK,
						I2C_CONF_HS_MODE_FAST);
	}
}

static void hsi2c_set_slave_addr(struct s5j_i2c_priv_s *priv, u16 addr, u32 is_master)
{
	if (!is_master) {
		/* Slave Address for I2C Slave */
		i2c_modifyreg32(priv, S5J_I2C_ADDR_OFFSET,
						I2C_ADDR_SLAVE_ADDR_SLA_MASK,
						I2C_ADDR_SLAVE_ADDR_SLA(addr));
	} else {
		/* Slave Address for I2C Master */
		i2c_modifyreg32(priv, S5J_I2C_ADDR_OFFSET,
						I2C_ADDR_SLAVE_ADDR_MAS_MASK,
						I2C_ADDR_SLAVE_ADDR_MAS(addr));
	}
}

static int hsi2c_manual_fast_init(struct s5j_i2c_priv_s *priv)
{
	hsi2c_conf(priv, priv->xfer_speed);
	hsi2c_calculate_timing(priv, priv->xfer_speed);

	i2c_modifyreg32(priv, S5J_I2C_INT_EN_OFFSET,
					I2C_INT_TRANSFER_DONE_MANUAL_MASK |
					I2C_INT_TRANSFER_DONE_NOACK_MANUAL_MASK,
					I2C_INT_TRANSFER_DONE_MANUAL_ON |
					I2C_INT_TRANSFER_DONE_NOACK_MANUAL_ON);

	/* Manual Control */
	i2c_modifyreg32(priv, S5J_I2C_CONF_OFFSET,
					I2C_CONF_AUTO_MODE_MASK,
					I2C_CONF_AUTO_MODE_MANUAL);

	/* Fast Speed Mode */
	i2c_modifyreg32(priv, S5J_I2C_CONF_OFFSET,
					I2C_CONF_HS_MODE_MASK,
					I2C_CONF_HS_MODE_FAST);

	i2c_modifyreg32(priv, S5J_I2C_INT_EN_OFFSET,
					I2C_INT_TRANSFER_DONE_MANUAL_MASK |
					I2C_INT_TRANSFER_DONE_NOACK_MANUAL_MASK,
					I2C_INT_TRANSFER_DONE_MANUAL_ON |
					I2C_INT_TRANSFER_DONE_NOACK_MANUAL_ON);

	return OK;
}

static int hsi2c_wait_xfer_done(struct s5j_i2c_priv_s *priv)
{
	int val;
	int timeout = priv->timeout;

	while (timeout-- > 0) {
		val = i2c_getreg32(priv, S5J_I2C_INT_STAT_OFFSET) & I2C_INT_TRANSFER_DONE_MANUAL_ON;
		if (val) {
			i2c_putreg32(priv, S5J_I2C_INT_STAT_OFFSET, val);
			return (val == I2C_INT_TRANSFER_DONE_MANUAL_ON);
		}
	}

	return -ETIMEDOUT;
}

static int hsi2c_wait_xfer_noack(struct s5j_i2c_priv_s *priv)
{
	int val;
	int timeout = priv->timeout;

	while (timeout-- > 0) {
		val = i2c_getreg32(priv, S5J_I2C_INT_STAT_OFFSET) & I2C_INT_TRANSFER_DONE_NOACK_MANUAL_ON;
		if (val) {
			i2c_putreg32(priv, S5J_I2C_INT_STAT_OFFSET, val);
			return (val == I2C_INT_TRANSFER_DONE_NOACK_MANUAL_ON);
		}
	}

	return -ETIMEDOUT;
}

static void hsi2c_start(struct s5j_i2c_priv_s *priv)
{
	/* Enables the current source for SCL signal to support high-speed operation */
	i2c_modifyreg32(priv, S5J_I2C_CTL_OFFSET,
					I2C_CTL_CS_ENB_MASK,
					I2C_CTL_CS_ENB_EN);

	/* HSI2C is master */
	i2c_modifyreg32(priv, S5J_I2C_CTL_OFFSET,
					I2C_CTL_MASTER_MASK,
					I2C_CTL_MASTER);

	/*
	 * TX channel enable signal
	 * TXCHON must be set after all the other configurations are completed.
	 * That is, the configuration should not be changed illegally after
	 * TX channels start working.
	 */
	i2c_modifyreg32(priv, S5J_I2C_CTL_OFFSET,
					I2C_CTL_TXCHON_MASK,
					I2C_CTL_TXCHON_ON);

	/* it forces an I2C master to send a start condition. */
	i2c_modifyreg32(priv, S5J_I2C_MANUAL_CMD_OFFSET,
					I2C_MANUAL_CMD_SEND_START_MASK,
					I2C_MANUAL_CMD_SEND_START_ON);
	hsi2c_wait_xfer_done(priv);
}

static void hsi2c_stop(struct s5j_i2c_priv_s *priv)
{
	/* it forces an I2C master to send a stop condition. */
	i2c_modifyreg32(priv, S5J_I2C_MANUAL_CMD_OFFSET,
					I2C_MANUAL_CMD_SEND_STOP_MASK,
					I2C_MANUAL_CMD_SEND_STOP_ON);
	hsi2c_wait_xfer_done(priv);
}

static void hsi2c_restart(struct s5j_i2c_priv_s *priv)
{
	/* it forces an I2C master to send a restart condition. */
	i2c_modifyreg32(priv, S5J_I2C_MANUAL_CMD_OFFSET,
					I2C_MANUAL_CMD_SEND_RESTART_MASK,
					I2C_MANUAL_CMD_SEND_RESTART_ON);
}

static int hsi2c_outb(struct s5j_i2c_priv_s *priv, u8 data)
{
	i2c_modifyreg32(priv, S5J_I2C_MANUAL_CMD_OFFSET,
					I2C_MANUAL_CMD_TX_DATA_MASK |
					I2C_MANUAL_CMD_SEND_DATA_MASK,
					I2C_MANUAL_CMD_TX_DATA(data) |
					I2C_MANUAL_CMD_SEND_DATA_ON);

	return hsi2c_wait_xfer_done(priv);
}

static int hsi2c_inb(struct s5j_i2c_priv_s *priv, bool is_ack)
{
	/* Looks awkward, but if I2C_MANUAL_CMD_RX_ACK_ON is set, ACK is NOT generated */
	if (!is_ack) {
		i2c_putreg32(priv, S5J_I2C_MANUAL_CMD_OFFSET,
					I2C_MANUAL_CMD_READ_DATA_ON |
					I2C_MANUAL_CMD_RX_ACK_ON);
	} else {
		i2c_putreg32(priv, S5J_I2C_MANUAL_CMD_OFFSET,
					I2C_MANUAL_CMD_READ_DATA_ON);
	}

	int ret = hsi2c_wait_xfer_done(priv);
	if (ret < 0) {
		return ret; /* timeout */
	}

	/* the received data */
	return ((i2c_getreg32(priv, S5J_I2C_MANUAL_CMD_OFFSET) >>
			I2C_MANUAL_CMD_RX_DATA_SHIFT) & I2C_MANUAL_CMD_RX_DATA_MASK);
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
		addr = I2C_ADDR10H(msg->addr);

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
			hsi2c_restart(priv);
			hsi2c_wait_xfer_done(priv);

			addr = I2C_READADDR10H(msg->addr);
			ret = try_address(priv, addr, retries);
			if ((ret != 1) && !nak_ok) {
				return -EIO;
			}
		}
	} else {
		/* 7-bit address */
		addr = I2C_ADDR8(msg->addr);
		if (flags & I2C_M_READ) {
			addr = I2C_READADDR8(msg->addr);
		}

		ret = try_address(priv, addr, retries);
		if ((ret != 1) && !nak_ok) {
			return -ENXIO;
		}
	}

	return OK;
}

static int hsi2c_setup(struct s5j_i2c_priv_s *priv, u32 master, u32 mode, u32 speed, u32 slave_addr)
{
	priv->master = master;
	priv->mode = mode;
	priv->xfer_speed = speed;
	priv->slave_addr = slave_addr;

	hsi2c_manual_fast_init(priv);

	if (master == I2C_MASTER_TYPE) {
		if (mode == I2C_POLLING_MODE) {
			hsi2c_calculate_timing(priv, priv->xfer_speed);
		} else if (mode == I2C_INTERRUPT_MODE) {
			/* TODO */
		}
	} else if (master == I2C_SLAVE_TYPE) {
		/* TODO */
	}

	return OK;
}

static int hsi2c_cleanup(struct s5j_i2c_priv_s *priv)
{
	if (priv->master == I2C_MASTER_TYPE) {
		if (priv->mode == I2C_INTERRUPT_MODE) {
			/* TODO */
		}
	} else if (priv->master == I2C_SLAVE_TYPE) {
		/* TODO */
	}

	/* Set default value */
	priv->master		= S5J_I2C_DEFAULT_TYPE;
	priv->mode			= S5J_I2C_DEFAULT_MODE;
	priv->xfer_speed	= S5J_I2C_DEFAULT_HS_CLOCK;

	/* Software Reset Signal */
	i2c_modifyreg32(priv, S5J_I2C_CTL_OFFSET,
					I2C_CTL_SW_RST_MASK,
					I2C_CTL_SW_RST_ON);
	up_udelay(10);

	i2c_putreg32(priv, S5J_I2C_CTL_OFFSET, I2C_CTL_RESET);
	up_udelay(100);

	return OK;
}

#ifndef CONFIG_I2C_POLLED
static int s5j_i2c_isr_process(void *args)
{
	struct s5j_i2c_priv_s *priv = args;

	/* TODO */

	return OK;
}

static int s5j_i2c_isr(int irq, void *context, FAR void *arg)
{
	struct s5j_i2c_priv_s *priv = (struct s5j_i2c_priv_s *)arg;

	DEBUGASSERT(priv != NULL);
	return s5j_i2c_isr_process(priv);
}
#endif

static void s5j_i2c_initialize(struct s5j_i2c_priv_s *priv)
{
	/* Configure GPIO pins */
	VERIFY(s5j_configgpio(priv->config->scl_pin) == OK);
	VERIFY(s5j_configgpio(priv->config->sda_pin) == OK);

#ifndef CONFIG_I2C_POLLED
	/* Attach ISRs */
	irq_attach(priv->config->irq, s5j_i2c_isr, priv);
	up_enable_irq(priv->config->irq);
#endif

	/* Enable I2C */
	hsi2c_setup(priv, priv->master, priv->mode, priv->xfer_speed, priv->slave_addr);
}

static void s5j_i2c_uninitialize(struct s5j_i2c_priv_s *priv)
{
	/* Disable I2C */
	hsi2c_cleanup(priv);

	/* Unconfigure GPIO pins */
	VERIFY(s5j_unconfiggpio(priv->config->scl_pin) == OK);
	VERIFY(s5j_unconfiggpio(priv->config->sda_pin) == OK);

#ifndef CONFIG_I2C_POLLED
	/* Disable and detach interrupts */
	up_disable_irq(priv->config->irq);
	irq_detach(priv->config->irq);
#endif
}

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* I2C Interface */
static const struct i2c_ops_s s5j_i2c_ops = {
	.setfrequency		= s5j_i2c_setclock,
	.setaddress			= s5j_i2c_setaddress,
	.write				= s5j_i2c_write,
	.read				= s5j_i2c_read,
#ifdef CONFIG_I2C_TRANSFER
	.transfer			= s5j_i2c_transfer,
#endif
#ifdef CONFIG_I2C_SLAVE
	.setownaddress		= NULL,
	.registercallback	= NULL,
#endif
};

static const struct s5j_i2c_config_s s5j_i2c0_config = {
	.base			= S5J_HSI2C0_BASE,
	.freqid			= CLK_SPL_BUS_P0,
	.scl_pin		= GPIO_I2C0_SCL,
	.sda_pin		= GPIO_I2C0_SDA,
#ifndef CONFIG_I2C_POLLED
	.irq			= IRQ_HSI2C_0,
#endif
};

static struct s5j_i2c_priv_s s5j_i2c0_priv = {
	.retries		= S5J_I2C_DEFAULT_RETRY,
	.master			= S5J_I2C_DEFAULT_TYPE,
	.mode			= S5J_I2C_DEFAULT_MODE,
	.xfer_speed		= S5J_I2C_DEFAULT_XFER_CLOCK,
	.slave_addr		= S5J_I2C_DEFAULT_SLAVE_ADDR,
	.timeout		= S5J_I2C_DEFAULT_TIMEOUT,
};

static const struct s5j_i2c_config_s s5j_i2c1_config = {
	.base			= S5J_HSI2C1_BASE,
	.freqid			= CLK_SPL_BUS_P0,
	.scl_pin		= GPIO_I2C1_SCL,
	.sda_pin		= GPIO_I2C1_SDA,
#ifndef CONFIG_I2C_POLLED
	.irq			= IRQ_HSI2C_1,
#endif
};

static struct s5j_i2c_priv_s s5j_i2c1_priv = {
	.retries		= S5J_I2C_DEFAULT_RETRY,
	.master			= S5J_I2C_DEFAULT_TYPE,
	.mode			= S5J_I2C_DEFAULT_MODE,
	.xfer_speed		= S5J_I2C_DEFAULT_XFER_CLOCK,
	.slave_addr		= S5J_I2C_DEFAULT_SLAVE_ADDR,
	.timeout		= S5J_I2C_DEFAULT_TIMEOUT,
};

static const struct s5j_i2c_config_s s5j_i2c2_config = {
	.base			= S5J_HSI2C2_BASE,
	.freqid			= CLK_SPL_BUS_P0,
	.scl_pin		= GPIO_I2C2_SCL,
	.sda_pin		= GPIO_I2C2_SDA,
#ifndef CONFIG_I2C_POLLED
	.irq			= IRQ_HSI2C_2,
#endif
};

static struct s5j_i2c_priv_s s5j_i2c2_priv = {
	.retries		= S5J_I2C_DEFAULT_RETRY,
	.master			= S5J_I2C_DEFAULT_TYPE,
	.mode			= S5J_I2C_DEFAULT_MODE,
	.xfer_speed		= S5J_I2C_DEFAULT_XFER_CLOCK,
	.slave_addr		= S5J_I2C_DEFAULT_SLAVE_ADDR,
	.timeout		= S5J_I2C_DEFAULT_TIMEOUT,
};

static const struct s5j_i2c_config_s s5j_i2c3_config = {
	.base			= S5J_HSI2C3_BASE,
	.freqid			= CLK_SPL_BUS_P0,
	.scl_pin		= GPIO_I2C3_SCL,
	.sda_pin		= GPIO_I2C3_SDA,
#ifndef CONFIG_I2C_POLLED
	.irq			= IRQ_HSI2C_3,
#endif
};

static struct s5j_i2c_priv_s s5j_i2c3_priv = {
	.retries		= S5J_I2C_DEFAULT_RETRY,
	.master			= S5J_I2C_DEFAULT_TYPE,
	.mode			= S5J_I2C_DEFAULT_MODE,
	.xfer_speed		= S5J_I2C_DEFAULT_XFER_CLOCK,
	.slave_addr		= S5J_I2C_DEFAULT_SLAVE_ADDR,
	.timeout		= S5J_I2C_DEFAULT_TIMEOUT,
};

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
	hsi2c_set_slave_addr(priv, addr, 0);

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
	while (sem_wait(&priv->sem_excl) != 0) {
		ASSERT(errno == EINTR);
	}

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

	if (priv->xfer_speed > S5J_I2C_DEFAULT_HS_CLOCK) {
		hsi2c_conf(priv, S5J_I2C_DEFAULT_HS_CLOCK);
		i2c_modifyreg32(priv, S5J_I2C_MANUAL_CMD_OFFSET,
						I2C_MANUAL_CMD_TX_DATA_MASK |
						I2C_MANUAL_CMD_SEND_DATA_MASK,
						I2C_MANUAL_CMD_TX_DATA(0xF) |
						I2C_MANUAL_CMD_SEND_DATA_ON);
		hsi2c_wait_xfer_noack(priv);

		hsi2c_conf(priv, priv->xfer_speed);
		hsi2c_restart(priv);
		hsi2c_wait_xfer_noack(priv);
	}

	for (i = 0; i < msgc; i++) {
		pmsg = &msgv[i];
		nak_ok = pmsg->flags & I2C_M_IGNORE_NAK;
		if (!(pmsg->flags & I2C_M_NOSTART)) {
			if ((i > 0) || (start == 0)) {
				hsi2c_restart(priv);
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
	sem_post(&priv->sem_excl);

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

int s5j_i2c_write(FAR struct i2c_dev_s *dev, FAR const uint8_t *buffer, int buflen)
{
	struct s5j_i2c_priv_s *priv = (struct s5j_i2c_priv_s *)dev;
	struct i2c_msg_s msg;

	/* Setup for the transfer */
	msg.addr = priv->slave_addr;
	msg.flags = 0;
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
FAR struct i2c_dev_s *up_i2cinitialize(int port)
{
	FAR struct s5j_i2c_priv_s *priv = NULL;
	const struct s5j_i2c_config_s *config;
	irqstate_t flags;

	if (port >= I2C_PORT_MAX) {
		return NULL;
	}

	switch (port) {
	case I2C_PORT0:
		priv = &s5j_i2c0_priv;
		config = &s5j_i2c0_config;
		break;

	case I2C_PORT1:
		priv = &s5j_i2c1_priv;
		config = &s5j_i2c1_config;
		break;

	case I2C_PORT2:
		priv = &s5j_i2c2_priv;
		config = &s5j_i2c2_config;
		break;

	case I2C_PORT3:
		priv = &s5j_i2c3_priv;
		config = &s5j_i2c3_config;
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
	flags = irqsave();

	priv->refs++;
	if (priv->refs == 1) {
		/* Initialize the device structure */
		priv->config = config;

		sem_init(&priv->sem_excl, 0, 1);
#ifndef CONFIG_I2C_POLLED
		sem_init(&priv->sem_isr, 0, 0);
		sem_setprotocol(&priv->sem_isr, SEM_PRIO_NONE);
#endif

		/* Initialize the I2C hardware */
		s5j_i2c_initialize(priv);
	}

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
	irqstate_t flags;

	DEBUGASSERT(priv && priv->refs > 0);

	/* Decrement reference count and check for underflow */
	flags = irqsave();

	/* Check if the reference count will decrement to zero */
	if (priv->refs < 2) {
		/* Yes.. Disable power and other HW resource (GPIO's) */
		s5j_i2c_uninitialize(priv);
		priv->refs = 0;

		/* Release unused resources */
		sem_destroy(&priv->sem_excl);
#ifndef CONFIG_I2C_POLLED
		sem_destroy(&priv->sem_isr);
#endif
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
