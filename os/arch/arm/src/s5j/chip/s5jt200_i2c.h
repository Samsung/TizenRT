/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/chip/s5j200_i2c.h
 *
 *   Copyright (C) 2009-2010, 2014-2015 Gregory Nutt. All rights reserved.
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

#ifndef __ARCH_ARM_SRC_S5J_CHIP_S5JT200_I2C_H
#define __ARCH_ARM_SRC_S5J_CHIP_S5JT200_I2C_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Register Offset **********************************************************/
#define S5J_I2C_CTL				0x0000
#define S5J_I2C_TRAILING_CTL			0x0008
#define S5J_I2C_INT_EN				0x0020
#define S5J_I2C_INT_STAT			0x0024
#define S5J_I2C_FIFO_STAT			0x0030
#define S5J_I2C_TXDATA				0x0034
#define S5J_I2C_RXDATA				0x0038
#define S5J_I2C_CONF				0x0040
#define S5J_I2C_AUTO_CONF			0x0044
#define S5J_I2C_TIMEOUT				0x0048
#define S5J_I2C_MANUAL_CMD			0x004C
#define S5J_I2C_TRANS_STATUS			0x0050
#define S5J_I2C_TIMING_HS1			0x0054
#define S5J_I2C_TIMING_HS2			0x0058
#define S5J_I2C_TIMING_HS3			0x005C
#define S5J_I2C_TIMING_FS1			0x0060
#define S5J_I2C_TIMING_FS2			0x0064
#define S5J_I2C_TIMING_FS3			0x0068
#define S5J_I2C_TIMING_SLA			0x006C
#define S5J_I2C_ADDR				0x0070

/* Register Bitfield Definitions ********************************************/

/* I2C_CTL register */
#define I2C_CTL_RESET_VALUE			0x0
#define I2C_CTL_CS_ENB				(1 << 0)
#define I2C_CTL_MASTER				(1 << 3)
#define I2C_CTL_RXCHON				(1 << 6)
#define I2C_CTL_TXCHON				(1 << 7)
#define I2C_CTL_SW_RST				(1 << 31)

/* I2C_INT_EN & I2C_INT_STAT register */
#define I2C_INT_TX_ALMOSTEMPTY_EN		(1 << 0)
#define I2C_INT_RX_ALMOSTFULL_EN		(1 << 1)
#define I2C_INT_TX_UNDERRUN_EN			(1 << 2)
#define I2C_INT_RX_OVERRUN_EN			(1 << 5)
#define I2C_INT_TRANSFER_DONE_AUTO_EN		(1 << 7)
#define I2C_INT_TRANSFER_DONE_NOACK_MANUAL_EN	(1 << 12)
#define I2C_INT_TRANSFER_DONE_MANUAL_EN		(1 << 13)
#define I2C_INT_SLAVE_ADDR_MATCH_EN		(1 << 15)
#define I2C_INT_ALL				0xFFFF

/* I2C_CONF register */
#define I2C_CONF_HS_MODE			(1 << 29)
#define I2C_CONF_ADDRMODE			(1 << 30)
#define I2C_CONF_AUTO_MODE			(1 << 31)

/* I2C_AUTO_CONF register */
#define I2C_AUTO_CONF_TRANS_LEN_ALL				0xFFFF
#define I2C_AUTO_CONF_READ_WRITE				(1 << 16)
#define I2C_AUTO_CONF_STOP_AFTER_TRANS				(1 << 17)
#define I2C_AUTO_CONF_MASTER_RUN				(1 << 31)

/* I2C_MANUAL_CMD register */
#define I2C_MANUAL_CMD_SEND_START				(1 << 0)
#define I2C_MANUAL_CMD_SEND_RESTART				(1 << 1)
#define I2C_MANUAL_CMD_SEND_STOP				(1 << 2)
#define I2C_MANUAL_CMD_SEND_DATA				(1 << 3)
#define I2C_MANUAL_CMD_READ_DATA				(1 << 4)
#define I2C_MANUAL_CMD_RX_ACK					(1 << 5)
#define I2C_MANUAL_CMD_TX_DATA(x)				((x & 0xFF) << 24)

/* I2C_TIMING_HS & I2C_TIMING_FS & I2C_TIMING_SLA register */
#define I2C_TIMING_TSTOP_SU(x)					((x & 0xFF) << 8)
#define I2C_TIMING_TSTART_HD(x)					((x & 0xFF) << 16)
#define I2C_TIMING_TSTART_SU(x)					((x & 0xFF) << 24)
#define I2C_TIMING_TSCL_H(x)					((x & 0xFF) << 0)
#define I2C_TIMING_TSCL_L(x)					((x & 0xFF) << 8)
#define I2C_TIMING_TDATA_SU(x)					((x & 0xFF) << 24)
#define I2C_TIMING_TSR_RELEASE(x)				((x & 0xFF) << 0)
#define I2C_TIMING_CLK_DIV(x)					((x & 0xFF) << 16)
#define I2C_TIMING_TDATA_HD_SLA(x)				((x & 0xFFFF) << 0)

/* I2C_ADDR register */
#define I2C_ADDR_SLAVE_ADDR_SLA(x)				((x & 0x3FF) << 0)

#endif							/* __ARCH_ARM_SRC_S5J_CHIP_S5JT200_I2C_H */
