/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
 *
 *   Copyright (C) 2014-2016 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * The Samsung sample code has a BSD compatible license that requires this
 * copyright notice:
 *
 *   Copyright (c) 2025 Samsung Electronics, Inc.
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


#include <arch/board/board.h>
#include "chip.h"
#include "up_arch.h"
#include "up_internal.h"
#include "tinyara/kmalloc.h"
#include "i2c_hal.h"
#include "hal_i2c_types.h"
#include "i2c.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define I2C_NUM_MAX         2

#define I2C_ADDRESS_TENBITS             (10)
#define I2C_IO_INIT_LEVEL               (1)
#define armino_DEFAULT_I2CXFER_CLOCK     (100 * 1000)	/* 100Khz */
#define armino_DEFAULT_I2C_TIMEOUT		(500)


#define I2C_MASTER_TOUT_CNUM_DEFAULT    (8)

#define I2C_armino_BUFFER_SIZE           (32)

#define armino_I2C0_DEFAULT_SCLK_PIN         4
#define armino_I2C0_DEFAULT_SCLK_PULLUP_EN   0
#define armino_I2C0_DEFAULT_SDA_PIN          5
#define armino_I2C0_DEFAULT_SDA_PULLUP_EN    0

#define armino_I2C1_DEFAULT_SCLK_PIN         18
#define armino_I2C1_DEFAULT_SCLK_PULLUP_EN   0
#define armino_I2C1_DEFAULT_SDA_PIN          19
#define armino_I2C1_DEFAULT_SDA_PULLUP_EN    0

#define armino_I2C_FREQUENCY_STD (100 * 1000)
#define armino_I2C_FREQUENCY_FAST (400 * 1000)

#define armino_I2C_MAX_DELAY_LOOP (10000000000)
#define armino_BUS_BUSY 1

#define armino_I2C_READ_ERROR (-1)

#ifndef min
#define min(a, b)       (((a) < (b)) ? (a) : (b))
#endif

#define BIT_MASK(n)     (BIT(n) - 1)

#define WAIT_ISR        1

#undef i2cinfo
#define i2cinfo(format, ...)


/****************************************************************************
 * Private Types
 ****************************************************************************/
/* I2C Device Private Data */

struct armino_i2c_priv_s {
	 const struct i2c_ops_s *ops;	/* Standard I2C operations */
	 i2c_id_t i2c_num;
	 const i2c_config_t *config;	/* Port configuration */

	 sem_t sem_excl;				/* Mutual exclusion semaphore */
	 sem_t sem_isr;

	 uint8_t refs;				/* Reference count */

	 uint8_t cpuint;				/* CPU interrupt */

	 int xfer_speed;
	 uint32_t slave_addr;
	 uint8_t slave_addr_bits;
	 uint32_t timeout;

	 uint32_t initialized;
	 uint32_t retries;

	 uint8_t status;
	 struct i2c_msg_s *transfer_msg;
	 uint32_t data_count;
	 uint8_t last_data_count;
};


typedef enum {
	 I2C_STATUS_READ,				/*!< read status for current master command */
	 I2C_STATUS_WRITE,				/*!< write status for current master command */
	 I2C_STATUS_IDLE,				/*!< idle status for current master command */
	 I2C_STATUS_ACK_ERROR,			/*!< ack error status for current master command */
	 I2C_STATUS_ARBITRATION_LOST,	/*!< arbitration lost */
	 I2C_STATUS_DONE,				/*!< I2C command done */
	 I2C_STATUS_TIMEOUT,				/*!< I2C bus status error, and operation timeout */
} i2c_status_t;

enum i2c_armino_opcodes {
	 I2C_armino_OP_RSTART = 0,
	 I2C_armino_OP_WRITE,
	 I2C_armino_OP_READ,
	 I2C_armino_OP_STOP,
	 I2C_armino_OP_END
};

struct i2c_armino_cmd {
	 uint32_t num_bytes:8;
	 uint32_t ack_en:1;
	 uint32_t ack_exp:1;
	 uint32_t ack_val:1;
	 uint32_t opcode:3;
	 uint32_t reserved:17;
	 uint32_t done:1;
};
/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct armino_i2c_priv_s armino_i2c0_priv = {
	 .i2c_num = 0,
	 .xfer_speed = 0,
	 .timeout = armino_DEFAULT_I2C_TIMEOUT,
	 .initialized = 0,
	 .retries = 3,
};

static struct armino_i2c_priv_s armino_i2c1_priv = {
	 .i2c_num = 1,
	 .xfer_speed = 0,
	 .timeout = armino_DEFAULT_I2C_TIMEOUT,
	 .initialized = 0,
	 .retries = 3,
};

struct armino_i2c_priv_s *g_armino_i2c_priv[I2C_NUM_MAX] = { NULL, NULL };

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int hsi2c_setup(struct armino_i2c_priv_s *priv);
static int i2c_hw_enable(i2c_id_t i2c_num);
static int i2c_hw_disable(i2c_id_t i2c_num);
static void i2c_hw_setpins(const i2c_config_t *config);
static void i2c_start_transfer(i2c_id_t i2c_num);
static inline void i2c_armino_reset_txfifo(i2c_id_t i2c_num);
static void i2c_armino_clear_commands(i2c_id_t i2c_num);
static inline void i2c_armino_reset_interrupts(i2c_id_t i2c_num);
static int i2c_armino_send_write_cmd(struct armino_i2c_priv_s *priv, int addr_bytes, int cmd_start_index);
static int i2c_armino_send_read_cmd(struct armino_i2c_priv_s *priv, int addr_bytes, int cmd_start_index);
static void i2c_armino_get_i2c_data(struct armino_i2c_priv_s *priv);
uint32_t armino_i2c_setclock(FAR struct i2c_dev_s *dev, uint32_t frequency);

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int hsi2c_setup(struct armino_i2c_priv_s *priv)
{
	int i2c_num = priv->i2c_num;

	i2c_config_t i2c_cfg = {0};
	i2c_cfg.baud_rate = priv->xfer_speed;
	i2c_cfg.addr_mode = priv->slave_addr_bits;
	i2c_cfg.slave_addr = priv->slave_addr;
	bk_i2c_init(i2c_num, &i2c_cfg);

	 return OK;
}

static int armino_i2c_initialize(struct armino_i2c_priv_s *priv)
{
	 int ret = hsi2c_setup(priv);
	 if (ret != OK) {
		 return ERROR;
	 }

	 //end of initallize!!
	 priv->initialized = 1;

	 return OK;
}

static int armino_i2c_uninitialize(struct armino_i2c_priv_s *priv)
{
	/* Disable I2C */
	int i2c_num = priv->i2c_num;
	bk_i2c_deinit(i2c_num);
	//end of initallize!!
	priv->initialized = 0;

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @brief    Setting Slave Address for slave mode
 * @param    struct i2c_dev_s *dev : pointer to i2c_dev_s
 * @param    int addr : Slave address
 * @param    int nbits : 0==7bit
 *           1==10bit
 * @return    = 0
 * @note
 */
int armino_i2c_setaddress(FAR struct i2c_dev_s *dev, int addr, int nbits)
{
	 struct armino_i2c_priv_s *priv = (struct armino_i2c_priv_s *)dev;
	 if (priv == NULL) {
		 i2cinfo("i2c dev is NULL\n");
		 return ERROR;
	 }

	 sem_wait(&priv->sem_excl);
	 if (addr > 0) {
		 priv->slave_addr = addr;
	 }
	 if (nbits > 0) {
		 priv->slave_addr_bits = nbits;
	 }

	 bk_i2c_set_slave_address(priv->i2c_num, addr);

	 sem_post(&priv->sem_excl);
	 return OK;
}

/**
* @brief    i2c clock setting
* @param    struct i2c_dev_s *dev : pointer to i2c_dev_s
* @param    unsigned int frequency : ex ) 100000 == 100khz
* @return   void
* @note
*/
uint32_t armino_i2c_setclock(FAR struct i2c_dev_s *dev, uint32_t frequency)
{
	 struct armino_i2c_priv_s *priv = (struct armino_i2c_priv_s *)dev;

	 if ((armino_I2C_FREQUENCY_STD != frequency) && (armino_I2C_FREQUENCY_FAST != frequency)) {
		 return ERROR;
	 }

	 if (priv != NULL && priv->xfer_speed != frequency) {
		 sem_wait(&priv->sem_excl);
		 priv->xfer_speed = frequency;
		 bk_i2c_set_baud_rate(priv->i2c_num, frequency);
		 sem_post(&priv->sem_excl);
	 }
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
int armino_i2c_transfer(struct i2c_dev_s *dev, struct i2c_msg_s *msgs, int msgc)
{
	 struct armino_i2c_priv_s *priv = (struct armino_i2c_priv_s *)dev;
	 int ret = 0;
	 uint8_t i = 0;
	 uint16_t addr = msgs->addr;
	 uint32_t timeout_ms = 2000;

	 /* Ensure that address or flags don't change meanwhile */
	 sem_wait(&priv->sem_excl);

	 /* Mask out unused address bits, and make room for R/W bit */
	 i = 0;
	 addr = msgs[i].addr;
	 if ((msgs[i].flags & I2C_M_TEN) != I2C_M_TEN) {
		 addr &= BIT_MASK(7);
	 } else {
		 addr &= BIT_MASK(10);
	 }
	 //addr <<= 1;

	 for (i = 0; i < msgc; i++) {
		if ((msgs[i].flags & I2C_M_READ) != I2C_M_READ) {
			priv->status = I2C_STATUS_WRITE;
			bk_i2c_master_write(priv->i2c_num, addr, msgs[i].buffer, msgs[i].length, timeout_ms);
		} else {
			priv->last_data_count = 0;
			priv->status = I2C_STATUS_READ;
			bk_i2c_master_read(priv->i2c_num, addr, msgs[i].buffer, msgs[i].length, timeout_ms);
		}

		if (ret < 0) {
			break;
		}
	 }

	 /* Ensure that address or flags don't change meanwhile */
	 sem_post(&priv->sem_excl);
	 return ret;
}

int armino_i2c_read(FAR struct i2c_dev_s *dev, FAR uint8_t *buffer, int buflen)
{
	 struct armino_i2c_priv_s *priv = (struct armino_i2c_priv_s *)dev;
	 struct i2c_msg_s msg;
	 uint16_t flags = (priv->slave_addr_bits == I2C_ADDRESS_TENBITS) ? I2C_M_TEN : 0;
	 /* Setup for the transfer */
	 msg.addr = priv->slave_addr;
	 msg.flags = (flags | I2C_M_READ);
	 msg.buffer = (FAR uint8_t *)buffer;
	 msg.length = buflen;

	 /*
	  * Then perform the transfer
	  *
	  * REVISIT:  The following two operations must become atomic in order to
	  * assure thread safety.
	  */

	 return armino_i2c_transfer(dev, &msg, 1);
}

int armino_i2c_write(FAR struct i2c_dev_s *dev, FAR const uint8_t *buffer, int buflen)
{
	struct armino_i2c_priv_s *priv = (struct armino_i2c_priv_s *)dev;
	struct i2c_msg_s msg;

	uint16_t flags = (priv->slave_addr_bits == I2C_ADDRESS_TENBITS) ? I2C_M_TEN : 0;
	/* Setup for the transfer */
	msg.addr = priv->slave_addr;
	msg.flags = flags;
	msg.buffer = (FAR uint8_t *)buffer;	/* Override const */
	msg.length = buflen;

	/*
	 * Then perform the transfer
	 *
	 * REVISIT:  The following two operations must become atomic in order to
	 * assure thread safety.
	 */
	return armino_i2c_transfer(dev, &msg, 1);
}

/* I2C Interface */
static const struct i2c_ops_s armino_i2c_ops = {
	.setfrequency = armino_i2c_setclock,
	.setaddress = armino_i2c_setaddress,
	.write = armino_i2c_write,
	.read = armino_i2c_read,
	.transfer = armino_i2c_transfer,
};


/**
 * @brief   Initialize one I2C bus
 * @param   int port :
 * @return  struct i2c_master_s : device structure
 * @note
 */
struct i2c_dev_s *up_i2cinitialize(int port)
{
	struct armino_i2c_priv_s *priv = NULL;
	// const i2c_config_t *config; // Unused variable

	bk_i2c_driver_init();

	/* check if port is valid */
	if (port < 0 || port >= I2C_NUM_MAX) {
	 return NULL;
	}

	/* Get I2C private structure */
	if (g_armino_i2c_priv[port] != NULL) {
		priv = g_armino_i2c_priv[port];
		priv->refs++;
		i2cinfo("I2C port %d already initiallized: %d\n", port, priv->refs);
		return (FAR struct i2c_dev_s *)priv;
	}

	switch (port) {
	case 0:
	priv = &armino_i2c0_priv;
	break;

	case 1:
	priv = &armino_i2c1_priv;
	break;

	default:
	 i2cinfo("Unsupport I2C port %d\n");
	 return NULL;
	}

	priv->i2c_num = port;

	/* Initialize private device structure */
	priv->ops = &armino_i2c_ops;

	/*
	 * Initialize private data for the first time, increment reference count,
	 * power-up hardware and configure GPIOs.
	 */
	priv->refs++;

	if (priv->refs == 1) {
	 /* Initialize the device structure */
	 sem_init(&priv->sem_excl, 0, 1);
	 sem_init(&priv->sem_isr, 0, 0);
	 sem_setprotocol(&priv->sem_isr, SEM_PRIO_NONE);

	 /* Initialize the I2C hardware */
	 armino_i2c_initialize(priv);
	}

	priv->status = I2C_STATUS_IDLE;

	g_armino_i2c_priv[port] = priv;

	return (FAR struct i2c_dev_s *)priv;
}

/**
 * @brief    Unitialize one I2C bus
 * @param    struct i2c_master_s *dev :
 * @return   ==0 : OK
 * @note
 */
int up_i2cuninitialize(FAR struct i2c_dev_s *dev)
{
	struct armino_i2c_priv_s *priv = (struct armino_i2c_priv_s *)dev;
	int flags;

	DEBUGASSERT(priv && priv->config && priv->refs > 0);

	/* Decrement reference count and check for underflow */
	flags = irqsave();

	/* Check if the reference count will decrement to zero */
	if (priv->refs < 2) {
		/* Yes.. Disable power and other HW resource (GPIO's) */
		armino_i2c_uninitialize(priv);
		priv->refs = 0;

		/* Release unused resources */
		sem_destroy(&priv->sem_excl);

		sem_destroy(&priv->sem_isr);

	} else {
		/* No.. just decrement the number of references to the device */
		priv->refs--;
	}

	irqrestore(flags);

	bk_i2c_driver_deinit();
	return OK;
}

/**
 * @brief   Register I2C bus into file system as I2C character driver
 * @param   I2C bus number :
 * @return  none
 * @note
 */
void armino_i2c_register(int bus)
{
	FAR struct i2c_dev_s *i2c;
	char path[16];

	i2c = up_i2cinitialize(bus);
	if (i2c != NULL) {
#ifdef CONFIG_I2C_USERIO
		 snprintf(path, 16, "/dev/i2c-%d", bus);
		 if (i2c_uioregister(path, i2c) < 0) {
			 up_i2cuninitialize(i2c);
		 }
#endif
	}
}
