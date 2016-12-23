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
#define I2C_INTERRUPT   1

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

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_i2cbus_initialize
 *
 * Description:
 *   Initialize the selected I2C port. And return a unique instance of struct
 *   struct i2c_master_s.  This function may be called to obtain multiple
 *   instances of the interface, each of which may be set up with a
 *   different frequency and slave address.
 *
 * Input Parameter:
 *   Port number (for hardware that has multiple I2C interfaces)
 *
 * Returned Value:
 *   Valid I2C device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/

struct i2c_dev_s *up_i2cinitialize(int port);

/****************************************************************************
 * Name: s5j_i2cbus_uninitialize
 *
 * Description:
 *   De-initialize the selected I2C port, and power down the device.
 *
 * Input Parameter:
 *   Device structure as returned by the tiva_i2cbus_initialize()
 *
 * Returned Value:
 *   OK on success, ERROR when internal reference count mismatch or dev
 *   points to invalid hardware device.
 *
 ****************************************************************************/

int s5j_i2cbus_uninitialize(FAR struct i2c_dev_s *dev);

void s5j_i2c_register(int bus);

#endif							/* __ARCH_ARM_SRC_S5J_I2C_H */
