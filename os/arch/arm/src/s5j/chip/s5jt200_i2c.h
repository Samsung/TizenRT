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
#define S5J_I2C_CTL_OFFSET				0x0000		/* HSI2C Control Register */
#define S5J_I2C_FIFO_CTL_OFFSET			0x0004		/* HSI2C FIFO Control Register */
#define S5J_I2C_TRAILING_CTL_OFFSET		0x0008		/* HSI2C Trailing Control Register */
#define S5J_I2C_INT_EN_OFFSET			0x0020		/* HSI2C Interrupt Enable Register */
#define S5J_I2C_INT_STAT_OFFSET			0x0024		/* HSI2C Interrupt Register */
#define S5J_I2C_FIFO_STAT_OFFSET		0x0030		/* HSI2C FIFO Status Register */
#define S5J_I2C_TXDATA_OFFSET			0x0034		/* HSI2C Transmitted Data Buffer Register */
#define S5J_I2C_RXDATA_OFFSET			0x0038		/* HSI2C Received Data Buffer Register */
#define S5J_I2C_CONF_OFFSET				0x0040		/* I2C Configuration */
#define S5J_I2C_AUTO_CONF_OFFSET		0x0044		/* I2C Configuration For Auto Mode */
#define S5J_I2C_TIMEOUT_OFFSET			0x0048		/* Time Out for I2C Master */
#define S5J_I2C_MANUAL_CMD_OFFSET		0x004C		/* Command Register for Manual Mode */
#define S5J_I2C_TRANS_STATUS_OFFSET		0x0050		/* HSI2C Transfer Status Register */
#define S5J_I2C_TIMING_HS1_OFFSET		0x0054		/* Timing Parameter for High-Speed Mode */
#define S5J_I2C_TIMING_HS2_OFFSET		0x0058		/* Timing Parameter for High-Speed Mode */
#define S5J_I2C_TIMING_HS3_OFFSET		0x005C		/* Timing Parameter for High-Speed Mode */
#define S5J_I2C_TIMING_FS1_OFFSET		0x0060		/* Timing Parameter for Fast-Speed Mode */
#define S5J_I2C_TIMING_FS2_OFFSET		0x0064		/* Timing Parameter for Fast-Speed Mode */
#define S5J_I2C_TIMING_FS3_OFFSET		0x0068		/* Timing Parameter for Fast-Speed Mode */
#define S5J_I2C_TIMING_SLA_OFFSET		0x006C		/* Timing Parameter for I2C Slave */
#define S5J_I2C_ADDR_OFFSET				0x0070		/* Address Register */


/* Register Bitfield Definitions ********************************************/
/* CTL **********************************************************************/
#define I2C_CTL_RESET					0x0

#define I2C_CTL_CS_ENB_SHIFT			0
#define I2C_CTL_CS_ENB_MASK				(0x1 << I2C_CTL_CS_ENB_SHIFT)
#define I2C_CTL_CS_ENB_EN				(0x0 << I2C_CTL_CS_ENB_SHIFT)
#define I2C_CTL_CS_ENB_SAMEDRIVE		(0x1 << I2C_CTL_CS_ENB_SHIFT)

#define I2C_CTL_MASTER_SHIFT			3
#define I2C_CTL_MASTER_MASK				(0x1 << I2C_CTL_MASTER_SHIFT)
#define I2C_CTL_SLAVE					(0x0 << I2C_CTL_MASTER_SHIFT)
#define I2C_CTL_MASTER					(0x1 << I2C_CTL_MASTER_SHIFT)

#define I2C_CTL_RXCHON_SHIFT			6
#define I2C_CTL_RXCHON_MASK				(0x1 << I2C_CTL_RXCHON_SHIFT)
#define I2C_CTL_RXCHON_OFF				(0x0 << I2C_CTL_RXCHON_SHIFT)
#define I2C_CTL_RXCHON_ON				(0x1 << I2C_CTL_RXCHON_SHIFT)

#define I2C_CTL_TXCHON_SHIFT			7
#define I2C_CTL_TXCHON_MASK				(0x1 << I2C_CTL_TXCHON_SHIFT)
#define I2C_CTL_TXCHON_OFF				(0x0 << I2C_CTL_TXCHON_SHIFT)
#define I2C_CTL_TXCHON_ON				(0x1 << I2C_CTL_TXCHON_SHIFT)

#define I2C_CTL_SW_RST_SHIFT			31
#define I2C_CTL_SW_RST_MASK				(0x1 << I2C_CTL_SW_RST_SHIFT)
#define I2C_CTL_SW_RST_OFF				(0x0 << I2C_CTL_SW_RST_SHIFT)
#define I2C_CTL_SW_RST_ON				(0x1 << I2C_CTL_SW_RST_SHIFT)

/* INT_EN & INT_STAT ********************************************************/
#define I2C_INT_TRANSFER_DONE_NOACK_MANUAL_SHIFT	12
#define I2C_INT_TRANSFER_DONE_NOACK_MANUAL_MASK		(0x1 << I2C_INT_TRANSFER_DONE_NOACK_MANUAL_SHIFT)
#define I2C_INT_TRANSFER_DONE_NOACK_MANUAL_OFF		(0x0 << I2C_INT_TRANSFER_DONE_NOACK_MANUAL_SHIFT)
#define I2C_INT_TRANSFER_DONE_NOACK_MANUAL_ON		(0x1 << I2C_INT_TRANSFER_DONE_NOACK_MANUAL_SHIFT)

#define I2C_INT_TRANSFER_DONE_MANUAL_SHIFT	13
#define I2C_INT_TRANSFER_DONE_MANUAL_MASK	(0x1 << I2C_INT_TRANSFER_DONE_MANUAL_SHIFT)
#define I2C_INT_TRANSFER_DONE_MANUAL_OFF	(0x0 << I2C_INT_TRANSFER_DONE_MANUAL_SHIFT)
#define I2C_INT_TRANSFER_DONE_MANUAL_ON		(0x1 << I2C_INT_TRANSFER_DONE_MANUAL_SHIFT)

/* CONF *********************************************************************/
#define I2C_CONF_FLT_CYCLE_SDA_SHIFT	13
#define I2C_CONF_FLT_CYCLE_SDA_MASK		(0x7 << I2C_CONF_FLT_CYCLE_SDA_SHIFT)
#define I2C_CONF_FLT_CYCLE_SDA(c)		((c) << I2C_CONF_FLT_CYCLE_SDA_SHIFT)

#define I2C_CONF_FLT_CYCLE_SCL_SHIFT	16
#define I2C_CONF_FLT_CYCLE_SCL_BITS		0x7
#define I2C_CONF_FLT_CYCLE_SCL_MASK		(0x7 << I2C_CONF_FLT_CYCLE_SCL_SHIFT)
#define I2C_CONF_FLT_CYCLE_SCL(c)		((c) << I2C_CONF_FLT_CYCLE_SCL_SHIFT)

#define I2C_CONF_HS_MODE_SHIFT			29
#define I2C_CONF_HS_MODE_MASK			(0x1 << I2C_CONF_HS_MODE_SHIFT)
#define I2C_CONF_HS_MODE_FAST			(0x0 << I2C_CONF_HS_MODE_SHIFT)
#define I2C_CONF_HS_MODE_HIGH			(0x1 << I2C_CONF_HS_MODE_SHIFT)

#define I2C_CONF_ADDRMODE_SHIFT			30
#define I2C_CONF_ADDRMODE_MASK			(0x1 << I2C_CONF_ADDRMODE_SHIFT)
#define I2C_CONF_ADDRMODE_10BIT			(0x1 << I2C_CONF_ADDRMODE_SHIFT)
#define I2C_CONF_ADDRMODE_7BIT			(0x0 << I2C_CONF_ADDRMODE_SHIFT)

#define I2C_CONF_AUTO_MODE_SHIFT		31
#define I2C_CONF_AUTO_MODE_MASK			(0x1 << I2C_CONF_AUTO_MODE_SHIFT)
#define I2C_CONF_AUTO_MODE_MANUAL		(0x0 << I2C_CONF_AUTO_MODE_SHIFT)
#define I2C_CONF_AUTO_MODE_AUTO			(0x1 << I2C_CONF_AUTO_MODE_SHIFT)

/* MANUAL_CMD ***************************************************************/
#define I2C_MANUAL_CMD_SEND_START_SHIFT	0
#define I2C_MANUAL_CMD_SEND_START_MASK	(0x1 << I2C_MANUAL_CMD_SEND_START_SHIFT)
#define I2C_MANUAL_CMD_SEND_START_ON	(0x1 << I2C_MANUAL_CMD_SEND_START_SHIFT)

#define I2C_MANUAL_CMD_SEND_RESTART_SHIFT	1
#define I2C_MANUAL_CMD_SEND_RESTART_MASK	(0x1 << I2C_MANUAL_CMD_SEND_RESTART_SHIFT)
#define I2C_MANUAL_CMD_SEND_RESTART_ON		(0x1 << I2C_MANUAL_CMD_SEND_RESTART_SHIFT)

#define I2C_MANUAL_CMD_SEND_STOP_SHIFT	2
#define I2C_MANUAL_CMD_SEND_STOP_MASK	(0x1 << I2C_MANUAL_CMD_SEND_STOP_SHIFT)
#define I2C_MANUAL_CMD_SEND_STOP_ON		(0x1 << I2C_MANUAL_CMD_SEND_STOP_SHIFT)

#define I2C_MANUAL_CMD_SEND_DATA_SHIFT	3
#define I2C_MANUAL_CMD_SEND_DATA_MASK	(0x1 << I2C_MANUAL_CMD_SEND_DATA_SHIFT)
#define I2C_MANUAL_CMD_SEND_DATA_ON		(0x1 << I2C_MANUAL_CMD_SEND_DATA_SHIFT)

#define I2C_MANUAL_CMD_READ_DATA_SHIFT	4
#define I2C_MANUAL_CMD_READ_DATA_MASK	(0x1 << I2C_MANUAL_CMD_READ_DATA_SHIFT)
#define I2C_MANUAL_CMD_READ_DATA_ON		(0x1 << I2C_MANUAL_CMD_READ_DATA_SHIFT)

#define I2C_MANUAL_CMD_RX_ACK_SHIFT		5
#define I2C_MANUAL_CMD_RX_ACK_MASK		(0x1 << I2C_MANUAL_CMD_RX_ACK_SHIFT)
#define I2C_MANUAL_CMD_RX_ACK_ON		(0x1 << I2C_MANUAL_CMD_RX_ACK_SHIFT)

#define I2C_MANUAL_CMD_RX_DATA_SHIFT	16
#define I2C_MANUAL_CMD_RX_DATA_MASK		0xFF

#define I2C_MANUAL_CMD_TX_DATA_SHIFT	24
#define I2C_MANUAL_CMD_TX_DATA_MASK		(0xFF << I2C_MANUAL_CMD_TX_DATA_SHIFT)
#define I2C_MANUAL_CMD_TX_DATA(c)		((c) << I2C_MANUAL_CMD_TX_DATA_SHIFT)

/* I2C_TIMING_HS1 & I2C_TIMING_FS1 ******************************************/
#define I2C_TIMING_TSTOP_SU_SHIFT		8
#define I2C_TIMING_TSTOP_SU_BITS		0xFF
#define I2C_TIMING_TSTOP_SU(c)			(((c) & I2C_TIMING_TSTOP_SU_BITS) << I2C_TIMING_TSTOP_SU_SHIFT)

#define I2C_TIMING_TSTART_HD_SHIFT		16
#define I2C_TIMING_TSTART_HD_BITS		0xFF
#define I2C_TIMING_TSTART_HD(c)			(((c) & I2C_TIMING_TSTART_HD_BITS) << I2C_TIMING_TSTART_HD_SHIFT)

#define I2C_TIMING_TSTART_SU_SHIFT		24
#define I2C_TIMING_TSTART_SU_BITS		0xFF
#define I2C_TIMING_TSTART_SU(c)			(((c) & I2C_TIMING_TSTART_SU_BITS) << I2C_TIMING_TSTART_SU_SHIFT)

/* I2C_TIMING_HS2 & I2C_TIMING_FS2 ******************************************/
#define I2C_TIMING_TSCL_H_SHIFT			0
#define I2C_TIMING_TSCL_H_BITS			0xFF
#define I2C_TIMING_TSCL_H(c)			(((c) & I2C_TIMING_TSCL_H_BITS) << I2C_TIMING_TSCL_H_SHIFT)

#define I2C_TIMING_TSCL_L_SHIFT			8
#define I2C_TIMING_TSCL_L_BITS			0xFF
#define I2C_TIMING_TSCL_L(c)			(((c) & I2C_TIMING_TSCL_L_BITS) << I2C_TIMING_TSCL_L_SHIFT)

#define I2C_TIMING_TDATA_SU_SHIFT		24
#define I2C_TIMING_TDATA_SU_BITS		0xFF
#define I2C_TIMING_TDATA_SU(c)			(((c) & I2C_TIMING_TDATA_SU_BITS) << I2C_TIMING_TDATA_SU_SHIFT)

/* I2C_TIMING_HS3 & I2C_TIMING_FS3 ******************************************/
#define I2C_TIMING_TSR_RELEASE_SHIFT	0
#define I2C_TIMING_TSR_RELEASE_BITS		0xFF
#define I2C_TIMING_TSR_RELEASE(c)		(((c) & I2C_TIMING_TSR_RELEASE_BITS) << I2C_TIMING_TSR_RELEASE_SHIFT)

#define I2C_TIMING_CLK_DIV_SHIFT		16
#define I2C_TIMING_CLK_DIV_BITS			0xFF
#define I2C_TIMING_CLK_DIV(c)			(((c) & I2C_TIMING_CLK_DIV_BITS) << I2C_TIMING_CLK_DIV_SHIFT)

/* I2C_TIMING_SLA ***********************************************************/
#define I2C_TIMING_TDATA_HD_SLA_SHIFT	0
#define I2C_TIMING_TDATA_HD_SLA_BITS	0xFFFF
#define I2C_TIMING_TDATA_HD_SLA(c)		(((c) & I2C_TIMING_TDATA_HD_SLA_BITS) << I2C_TIMING_TDATA_HD_SLA_SHIFT)

/* I2C_ADDR *****************************************************************/
#define I2C_ADDR_SLAVE_ADDR_SLA_SHIFT	0
#define I2C_ADDR_SLAVE_ADDR_SLA_MASK	(0x3FF << I2C_ADDR_SLAVE_ADDR_SLA_SHIFT)
#define I2C_ADDR_SLAVE_ADDR_SLA(c)		((c) << I2C_ADDR_SLAVE_ADDR_SLA_SHIFT)

#define I2C_ADDR_SLAVE_ADDR_MAS_SHIFT	10
#define I2C_ADDR_SLAVE_ADDR_MAS_MASK	(0x3FF << I2C_ADDR_SLAVE_ADDR_MAS_SHIFT)
#define I2C_ADDR_SLAVE_ADDR_MAS(c)		((c) << I2C_ADDR_SLAVE_ADDR_MAS_SHIFT)

#endif							/* __ARCH_ARM_SRC_S5J_CHIP_S5JT200_I2C_H */
