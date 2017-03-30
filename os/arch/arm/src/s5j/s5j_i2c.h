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
 * arch/arm/src/s5j/s5j_i2c.h
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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
 ************************************************************************************/

#ifndef __ARCH_ARM_SRC_S5J_I2C_H
#define __ARCH_ARM_SRC_S5J_I2C_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>
#include <tinyara/i2c.h>

#include "chip.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* I2C Register Offsets *************************************************************/
#define HSI2C_MASTER_TX_SLAVE_RX    1
#define HSI2C_SLAVE_TX_MASTER_RX    2
#define HSI2C_MASTER_TX_MASTER_RX   4

#define HSI2C_MASTER_BUFFER_BASE    0x44000000
#define HSI2C_SLAVE_BUFFER_BASE     0x44800000

#define HSI2C_MASTER_ID             0x9	/*  Master ID 00001xxx */
#define HSI2C_SLAVE_ADDRESS         0x50

/* Register Map*************************************************************** */

#define CTL               0x0000
#define FIFO_CTL          0x0004
#define TRAILING_CTL      0x0008
#define INT_EN            0x0020
#define INT_STAT          0x0024
#define FIFO_STAT         0x0030
#define TXDATA            0x0034
#define RXDATA            0x0038
#define I2C_CONF          0x0040
#define I2C_AUTO_CONF     0x0044
#define I2C_TIMEOUT       0x0048
#define I2C_MANUAL_CMD    0x004C
#define I2C_TRANS_STATUS  0x0050
#define I2C_TIMING_HS1    0x0054
#define I2C_TIMING_HS2    0x0058
#define I2C_TIMING_HS3    0x005C
#define I2C_TIMING_FS1    0x0060
#define I2C_TIMING_FS2    0x0064
#define I2C_TIMING_FS3    0x0068
#define I2C_TIMING_SLA    0x006C
#define I2C_ADDR          0x0070

#define I2C_START         (1 << 0)
#define I2C_RESTART       (1 << 1)
#define I2C_STOP          (1 << 2)
#define I2C_SEND_DATA     (1 << 3)
#define I2C_READ_DATA     (1 << 4)
#define I2C_RX_ACK        (1 << 5)
#define I2C_TX_MASK       (0xFF << 24)
#define I2C_RX_MASK       (0xFF << 16)

#define I2C_SPEED_400KHZ  400000

#define HSI2C_INT_SLAVE_ADDR_MATCH        (1 << 15)
#define HSI2C_INT_XFER_DONE_MANUAL        (1 << 13)
#define HSI2C_INT_XFER_DONE_NOACK_MANUAL  (1 << 12)
#define HSI2C_INT_NO_DEV_AUTO             (1 << 10)
#define HSI2C_INT_NO_DEV_ACK_AUTO         (1 << 9)
#define HSI2C_INT_TRANS_ABORT_AUTO        (1 << 8)
#define HSI2C_INT_TRANSFER_DONE_AUTO      (1 << 7)
#define HSI2C_INT_RX_OVERRUN              (1 << 5)
#define HSI2C_INT_TX_UNDERRUN             (1 << 2)
#define HSI2C_INT_RX_ALMOST_FULL          (1 << 1)
#define HSI2C_INT_TX_ALMOST_EMPTY         (1 << 0)

#define HSI2C_INT_ALL  0xFFFF

#define SLAVE_TX_MODE  (1 << 19)

#define STOP_COND      (1 << 18)

#define I2C_MASTER      0
#define I2C_SLAVE_MODE  1
#define I2C_POLLING     0
/* #define I2C_INTERRUPT   1 */

enum slave_status {
	SLAVE_IDLE,
	SLAVE_GET_REG,
	SLAVE_GET_DATA,
	SLAVE_SET_DATA,
};

struct slave_data {
	enum slave_status status;
	u32 current_reg;
	u8 data[20];
};

struct master_data {
	struct i2c_msg_s *msg;
	int num;
	int cur_msg;
	int buf_count;
	/*  struct completion done; */
};

/* I2C Device hardware configuration */
struct s5j_i2c_config_s {
	uintptr_t base;				/* I2C base address */
	unsigned int scl_pin;			/* GPIO configuration for SCL as SCL */
	unsigned int sda_pin;			/* GPIO configuration for SDA as SDA */
	int (*isr)(int, void *, void *);	/* Interrupt handler */
	unsigned int irq;				/* IRQ number */
	uint8_t devno;				/* I2Cn where n = devno */
};

/* I2C Device Private Data */
struct s5j_i2c_priv_s {
	const struct i2c_ops_s *ops;	/* Standard I2C operations */
	const struct s5j_i2c_config_s *config;	/* Port configuration */
	sem_t exclsem;				/* Mutual exclusion semaphore */
#ifndef CONFIG_I2C_POLLED
	sem_t waitsem;				/* Interrupt wait semaphore */
#endif
	uint8_t refs;				/* Reference count */
	volatile uint8_t intstate;	/* Interrupt handshake (see enum s5j_intstate_e) */

	int state;
	int clock;
	int xfer_speed;
	unsigned int master;
	unsigned int mode;
	unsigned int slave_addr;
	unsigned int addrlen;
	unsigned int timeout;
	char name[16];

	unsigned int initialized;
	unsigned int retries;
	/*  master data  */
	uint8_t msgc;				/* Message count */
	struct i2c_msg_s *msgv;		/* Message list */
	uint8_t *mptr;				/* Current message buffer */
	int mcnt;					/* Current message length */
	uint16_t mflags;			/* Current message flags */

	struct i2c_msg_s *msg;

	/* interrupt */
	struct slave_data *slave_test_data;
	struct master_data *master_test_data;

};

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/
static void hsi2c_set_hs_timing(unsigned int base, unsigned int nClkDiv, unsigned int tSTART_SU, unsigned int tSTART_HD, unsigned int tSTOP_SU, unsigned int tSDA_SU, unsigned int tDATA_SU, unsigned int tDATA_HD, unsigned int tSCL_L, unsigned int tSCL_H, unsigned int tSR_RELEASE);
static void hsi2c_set_fs_timing(unsigned int base, unsigned int nClkDiv, unsigned int tSTART_SU, unsigned int tSTART_HD, unsigned int tSTOP_SU, unsigned int tDATA_SU, unsigned int tDATA_HD, unsigned int tSCL_L, unsigned int tSCL_H, unsigned int tSR_RELEASE);

static void hsi2c_calculate_timing(unsigned int base, unsigned int nPclk, unsigned int nOpClk);
static void hsi2c_conf(unsigned int base, unsigned int nOpClk);
static void hsi2c_enable_int(unsigned int base, unsigned int bit);
static void hsi2c_disable_int(unsigned int base, unsigned int bit);
static void hsi2c_clear_int(unsigned int base, unsigned int bit);
static unsigned int hsi2c_read_int_status(unsigned int base);
static void hsi2c_set_slave_addr(unsigned int base, u16 addr, unsigned int is_master);
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
static void hsi2c_run_auto_mode(unsigned int base, int on);
static void hsi2c_tx_fifo_reset(unsigned int base, int resetb);
static void hsi2c_set_auto_config(unsigned int base, unsigned int stop, unsigned int tx, unsigned int len);
static void hsi2c_set_trans_mode(unsigned int base, unsigned int master, unsigned int tx);
static void hsi2c_set_hwacg_mode(unsigned int base, unsigned int slave);
static int hsi2c_master_handler(void *args);
static int hsi2c_slave_handler(void *args);
static void hsi2c_set_auto_mode(unsigned int base);
static void hsi2c_set_fifo_level(unsigned int base);
static void hsi2c_master_setup(struct s5j_i2c_priv_s *priv, unsigned int mode, unsigned int speed, unsigned int slave_addr);
static void hsi2c_slave_setup(struct s5j_i2c_priv_s *priv, unsigned int mode, unsigned int speed, unsigned int slave_addr);
static void hsi2c_master_cleanup(struct s5j_i2c_priv_s *priv);
static void hsi2c_slave_cleanup(struct s5j_i2c_priv_s *priv);
static int hsi2c_setup(struct s5j_i2c_priv_s *priv, unsigned int master, unsigned int mode, unsigned int speed, unsigned int slave_addr);
static int hsi2c_cleanup(struct s5j_i2c_priv_s *priv);
static inline void s5j_i2c_sem_wait(struct s5j_i2c_priv_s *priv);
static inline void s5j_i2c_sem_post(struct s5j_i2c_priv_s *priv);
static inline void s5j_i2c_sem_init(struct s5j_i2c_priv_s *priv);
static inline void s5j_i2c_sem_destroy(struct s5j_i2c_priv_s *priv);
static int s5j_i2c_interrupt(struct s5j_i2c_priv_s *priv, unsigned int status);
static int s5j_i2c0_interrupt(int irq, void *context, void *arg);
static int s5j_i2c1_interrupt(int irq, void *context, void *arg);
static int s5j_i2c2_interrupt(int irq, void *context, void *arg);
static int s5j_i2c3_interrupt(int irq, void *context, void *arg);
static int s5j_i2c_initialize(struct s5j_i2c_priv_s *priv, unsigned int frequency);
static int s5j_i2c_uninitialize(struct s5j_i2c_priv_s *priv);


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
unsigned int s5j_i2c_setclock(struct i2c_dev_s *dev, unsigned int frequency);
int s5j_i2c_setownaddress(FAR struct i2c_dev_s *dev, int addr, int nbits);
int s5j_i2c_transfer(struct i2c_dev_s *dev, struct i2c_msg_s *msgv, int msgc);
int s5j_i2c_read(FAR struct i2c_dev_s *dev, FAR uint8_t *buffer, int buflen);
int s5j_i2c_write(FAR struct i2c_dev_s *dev, FAR const uint8_t *buffer, int buflen);
#ifdef CONFIG_I2C_RESET
int s5j_i2c_reset(FAR struct i2c_dev_s *dev);
#endif
struct i2c_dev_s *up_i2cinitialize(int port);
int s5j_i2cbus_uninitialize(FAR struct i2c_dev_s *dev);
void s5j_i2c_register(int bus);
#endif							/* __ARCH_ARM_SRC_S5J_I2C_H */
