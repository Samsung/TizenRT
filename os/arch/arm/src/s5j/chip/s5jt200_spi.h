/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/chip/s5j200_spi.h
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

#ifndef __ARCH_ARM_SRC_S5J_CHIP_S5JT200_SPI_H
#define __ARCH_ARM_SRC_S5J_CHIP_S5JT200_SPI_H

/****************************************************************************
 * Include
 ****************************************************************************/
#include "s5j_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* SPI Register Offsets *****************************************************/
#define S5J_SPI_CH_CFG_OFFSET			0x0000		/* SPI Configuration */
#define S5J_SPI_MODE_CFG_OFFSET			0x0008		/* FIFO Control */
#define S5J_SPI_CS_REG_OFFSET			0x000C		/* Slave Selection Control */
#define S5J_SPI_INT_EN_OFFSET			0x0010		/* Interrupt Enable */
#define S5J_SPI_STATUS_OFFSET			0x0014		/* SPI Status */
#define S5J_SPI_TX_DATA_OFFSET			0x0018		/* Tx Data */
#define S5J_SPI_RX_DATA_OFFSET			0x001C		/* Rx Data */
#define S5J_SPI_PACKET_CNT_REG_OFFSET	0x0020		/* Packet Count */
#define S5J_SPI_PENDING_CLR_REG_OFFSET	0x0024		/* Interrupt Pending Clear */
#define S5J_SPI_SWAP_CFG_OFFSET			0x0028		/* Swap Configuration */
#define S5J_SPI_FB_CLK_SEL_OFFSET		0x002C		/* Feedback Clock Selection */

/* Register Bitfield Definitions ********************************************/
/* CH_CFGn ******************************************************************/
#define SPI_CH_CFG_TX_CH_SHIFT			0
#define SPI_CH_CFG_TX_CH_MASK			(0x1 << SPI_CH_CFG_TX_CH_SHIFT)
#define SPI_CH_CFG_TX_CH_ON				(0x1 << SPI_CH_CFG_TX_CH_SHIFT)
#define SPI_CH_CFG_TX_CH_OFF			(0x0 << SPI_CH_CFG_TX_CH_SHIFT)

#define SPI_CH_CFG_RX_CH_SHIFT			1
#define SPI_CH_CFG_RX_CH_MASK			(0x1 << SPI_CH_CFG_RX_CH_SHIFT)
#define SPI_CH_CFG_RX_CH_ON				(0x1 << SPI_CH_CFG_RX_CH_SHIFT)
#define SPI_CH_CFG_RX_CH_OFF			(0x0 << SPI_CH_CFG_RX_CH_SHIFT)

#define SPI_CH_CFG_CPHA_SHIFT			2
#define SPI_CH_CFG_CPHA_MASK			(0x1 << SPI_CH_CFG_CPHA_SHIFT)
#define SPI_CH_CFG_CPHA_FORMATA			(0x0 << SPI_CH_CFG_CPHA_SHIFT)
#define SPI_CH_CFG_CPHA_FORMATB			(0x1 << SPI_CH_CFG_CPHA_SHIFT)

#define SPI_CH_CFG_CPOL_SHIFT			3
#define SPI_CH_CFG_CPOL_MASK			(0x1 << SPI_CH_CFG_CPOL_SHIFT)
#define SPI_CH_CFG_CPOL_HIGH			(0x0 << SPI_CH_CFG_CPOL_SHIFT)
#define SPI_CH_CFG_CPOL_LOW				(0x1 << SPI_CH_CFG_CPOL_SHIFT)

#define SPI_CH_CFG_SLAVE_SHIFT			4
#define SPI_CH_CFG_SLAVE_MASK			(0x1 << SPI_CH_CFG_SLAVE_SHIFT)
#define SPI_CH_CFG_MASTER				(0x0 << SPI_CH_CFG_SLAVE_SHIFT)
#define SPI_CH_CFG_SLAVE				(0x1 << SPI_CH_CFG_SLAVE_SHIFT)

#define SPI_CH_CFG_FIFO_FLUSH_SHIFT		5
#define SPI_CH_CFG_FIFO_FLUSH_MASK		(0x1 << SPI_CH_CFG_FIFO_FLUSH_SHIFT)
#define SPI_CH_CFG_FIFO_FLUSH_OFF		(0x0 << SPI_CH_CFG_FIFO_FLUSH_SHIFT)
#define SPI_CH_CFG_FIFO_FLUSH_ON		(0x1 << SPI_CH_CFG_FIFO_FLUSH_SHIFT)

#define SPI_CH_CFG_HIGH_SPEED_SHIFT		6
#define SPI_CH_CFG_HIGH_SPEED_MASK		(0x1 << SPI_CH_CFG_HIGH_SPEED_SHIFT)
#define SPI_CH_CFG_HIGH_SPEED_DIS		(0x0 << SPI_CH_CFG_HIGH_SPEED_SHIFT)
#define SPI_CH_CFG_HIGH_SPEED_EN		(0x1 << SPI_CH_CFG_HIGH_SPEED_SHIFT)

/* MODE_CFGn ****************************************************************/
#define SPI_MODE_CFG_DMA_TYPE_SHIFT		0
#define SPI_MODE_CFG_DMA_TYPE_MASK		(0x1 << SPI_MODE_CFG_DMA_TYPE_SHIFT)
#define SPI_MODE_CFG_DMA_SINGLE			(0x0 << SPI_MODE_CFG_DMA_TYPE_SHIFT)
#define SPI_MODE_CFG_DMA_4BURST			(0x1 << SPI_MODE_CFG_DMA_TYPE_SHIFT)

#define SPI_MODE_CFG_DMA_TX_SHIFT		1
#define SPI_MODE_CFG_DMA_TX_MASK		(0x1 << SPI_MODE_CFG_DMA_TX_SHIFT)
#define SPI_MODE_CFG_DMA_TX_OFF			(0x0 << SPI_MODE_CFG_DMA_TX_SHIFT)
#define SPI_MODE_CFG_DMA_TX_ON			(0x1 << SPI_MODE_CFG_DMA_TX_SHIFT)

#define SPI_MODE_CFG_DMA_RX_SHIFT		2
#define SPI_MODE_CFG_DMA_RX_MASK		(0x1 << SPI_MODE_CFG_DMA_RX_SHIFT)
#define SPI_MODE_CFG_DMA_RX_OFF			(0x0 << SPI_MODE_CFG_DMA_RX_SHIFT)
#define SPI_MODE_CFG_DMA_RX_ON			(0x1 << SPI_MODE_CFG_DMA_RX_SHIFT)

#define SPI_MODE_CFG_TX_RDY_SHIFT		5
#define SPI_MODE_CFG_TX_RDY_MASK		(0x3F << SPI_MODE_CFG_TX_RDY_SHIFT)
#define SPI_MODE_CFG_TX_RDY_LVL(c)		((c) << SPI_MODE_CFG_TX_RDY_SHIFT)

#define SPI_MODE_CFG_RX_RDY_SHIFT		11
#define SPI_MODE_CFG_RX_RDY_MASK		(0x3F << SPI_MODE_CFG_RX_RDY_SHIFT)
#define SPI_MODE_CFG_RX_RDY_LVL(c)		((c) << SPI_MODE_CFG_RX_RDY_SHIFT)

#define SPI_MODE_CFG_BUS_WIDTH_SHIFT	17
#define SPI_MODE_CFG_BUS_WIDTH_MASK		(0x3 << SPI_MODE_CFG_BUS_WIDTH_SHIFT)
#define SPI_MODE_CFG_BUS_WIDTH_8		(0x0 << SPI_MODE_CFG_BUS_WIDTH_SHIFT)
#define SPI_MODE_CFG_BUS_WIDTH_16		(0x1 << SPI_MODE_CFG_BUS_WIDTH_SHIFT)
#define SPI_MODE_CFG_BUS_WIDTH_32		(0x2 << SPI_MODE_CFG_BUS_WIDTH_SHIFT)

#define SPI_MODE_CFG_TRAILING_SHIFT		19
#define SPI_MODE_CFG_TRAILING_MASK		(0x3FF << SPI_MODE_CFG_TRAILING_SHIFT)
#define SPI_MODE_CFG_TRAILING_CNT(c)	((c) << SPI_MODE_CFG_TRAILING_SHIFT)

#define SPI_MODE_CFG_CH_WIDTH_SHIFT		29
#define SPI_MODE_CFG_CH_WIDTH_MASK		(0x3 << SPI_MODE_CFG_CH_WIDTH_SHIFT)
#define SPI_MODE_CFG_CH_WIDTH_8			(0x0 << SPI_MODE_CFG_CH_WIDTH_SHIFT)
#define SPI_MODE_CFG_CH_WIDTH_16		(0x1 << SPI_MODE_CFG_CH_WIDTH_SHIFT)
#define SPI_MODE_CFG_CH_WIDTH_32		(0x2 << SPI_MODE_CFG_CH_WIDTH_SHIFT)

/* CS_REGn ******************************************************************/
#define SPI_CS_REG_NSSOUT_SHIFT			0
#define SPI_CS_REG_NSSOUT_MASK			(0x1 << SPI_CS_REG_NSSOUT_SHIFT)
#define SPI_CS_REG_NSSOUT_ON			(0x0 << SPI_CS_REG_NSSOUT_SHIFT)
#define SPI_CS_REG_NSSOUT_OFF			(0x1 << SPI_CS_REG_NSSOUT_SHIFT)

#define SPI_CS_REG_MODE_SHIFT			1
#define SPI_CS_REG_MODE_MASK			(0x1 << SPI_CS_REG_MODE_SHIFT)
#define SPI_CS_REG_MODE_MANUAL			(0x0 << SPI_CS_REG_MODE_SHIFT)
#define SPI_CS_REG_MODE_AUTO			(0x1 << SPI_CS_REG_MODE_SHIFT)

#define SPI_CS_REG_NCS_TIME_SHIFT		4
#define SPI_CS_REG_NCS_TIME_MASK		(0x3F << SPI_CS_REG_NCS_TIME_SHIFT)
#define SPI_CS_REG_NCS_TIME_COUNT(c)	((c) << SPI_CS_REG_NCS_TIME_SHIFT)

/* INT_ENn ******************************************************************/
#define SPI_INT_EN_TX_FIFO_SHIFT		0
#define SPI_INT_EN_TX_FIFO_MASK			(0x1 << SPI_INT_EN_TX_FIFO_SHIFT)
#define SPI_INT_EN_TX_FIFO_OFF			(0x0 << SPI_INT_EN_TX_FIFO_SHIFT)
#define SPI_INT_EN_TX_FIFO_ON			(0x1 << SPI_INT_EN_TX_FIFO_SHIFT)

#define SPI_INT_EN_RX_FIFO_SHIFT		1
#define SPI_INT_EN_RX_FIFO_MASK			(0x1 << SPI_INT_EN_RX_FIFO_SHIFT)
#define SPI_INT_EN_RX_FIFO_OFF			(0x0 << SPI_INT_EN_RX_FIFO_SHIFT)
#define SPI_INT_EN_RX_FIFO_ON			(0x1 << SPI_INT_EN_RX_FIFO_SHIFT)

#define SPI_INT_EN_TX_UNDERRUN_SHIFT	2
#define SPI_INT_EN_TX_UNDERRUN_MASK		(0x1 << SPI_INT_EN_TX_UNDERRUN_SHIFT)
#define SPI_INT_EN_TX_UNDERRUN_OFF		(0x0 << SPI_INT_EN_TX_UNDERRUN_SHIFT)
#define SPI_INT_EN_TX_UNDERRUN_ON		(0x1 << SPI_INT_EN_TX_UNDERRUN_SHIFT)

#define SPI_INT_EN_TX_OVERRUN_SHIFT		3
#define SPI_INT_EN_TX_OVERRUN_MASK		(0x1 << SPI_INT_EN_TX_OVERRUN_SHIFT)
#define SPI_INT_EN_TX_OVERRUN_OFF		(0x0 << SPI_INT_EN_TX_OVERRUN_SHIFT)
#define SPI_INT_EN_TX_OVERRUN_ON		(0x1 << SPI_INT_EN_TX_OVERRUN_SHIFT)

#define SPI_INT_EN_RX_UNDERRUN_SHIFT	4
#define SPI_INT_EN_RX_UNDERRUN_MASK		(0x1 << SPI_INT_EN_RX_UNDERRUN_SHIFT)
#define SPI_INT_EN_RX_UNDERRUN_OFF		(0x0 << SPI_INT_EN_RX_UNDERRUN_SHIFT)
#define SPI_INT_EN_RX_UNDERRUN_ON		(0x1 << SPI_INT_EN_RX_UNDERRUN_SHIFT)

#define SPI_INT_EN_RX_OVERRUN_SHIFT		5
#define SPI_INT_EN_RX_OVERRUN_MASK		(0x1 << SPI_INT_EN_RX_OVERRUN_SHIFT)
#define SPI_INT_EN_RX_OVERRUN_OFF		(0x0 << SPI_INT_EN_RX_OVERRUN_SHIFT)
#define SPI_INT_EN_RX_OVERRUN_ON		(0x1 << SPI_INT_EN_RX_OVERRUN_SHIFT)

#define SPI_INT_EN_TAILING_SHIFT		6
#define SPI_INT_EN_TAILING_MASK			(0x1 << SPI_INT_EN_TAILING_SHIFT)
#define SPI_INT_EN_TAILING_OFF			(0x0 << SPI_INT_EN_TAILING_SHIFT)
#define SPI_INT_EN_TAILING_ON			(0x1 << SPI_INT_EN_TAILING_SHIFT)

/* STATUSn ******************************************************************/
#define SPI_STATUS_TX_FIFO_RDY_SHIFT	0
#define SPI_STATUS_TX_FIFO_RDY_MASK		(0x1 << SPI_STATUS_TX_FIFO_RDY_SHIFT)
#define SPI_STATUS_TX_FIFO_RDY_MORE		(0x0 << SPI_STATUS_TX_FIFO_RDY_SHIFT)
#define SPI_STATUS_TX_FIFO_RDY_LESS		(0x1 << SPI_STATUS_TX_FIFO_RDY_SHIFT)

#define SPI_STATUS_RX_FIFO_RDY_SHIFT	1
#define SPI_STATUS_RX_FIFO_RDY_MASK		(0x1 << SPI_STATUS_RX_FIFO_RDY_SHIFT)
#define SPI_STATUS_RX_FIFO_RDY_LESS		(0x0 << SPI_STATUS_RX_FIFO_RDY_SHIFT)
#define SPI_STATUS_RX_FIFO_RDY_MORE		(0x1 << SPI_STATUS_RX_FIFO_RDY_SHIFT)

#define SPI_STATUS_TX_UNDERRUN_SHIFT	2
#define SPI_STATUS_TX_UNDERRUN_MASK		(0x1 << SPI_STATUS_TX_UNDERRUN_SHIFT)
#define SPI_STATUS_TX_UNDERRUN_OK		(0x0 << SPI_STATUS_TX_UNDERRUN_SHIFT)
#define SPI_STATUS_TX_UNDERRUN_ERR		(0x1 << SPI_STATUS_TX_UNDERRUN_SHIFT)

#define SPI_STATUS_TX_OVERRUN_SHIFT		3
#define SPI_STATUS_TX_OVERRUN_MASK		(0x1 << SPI_STATUS_TX_OVERRUN_SHIFT)
#define SPI_STATUS_TX_OVERRUN_OK		(0x0 << SPI_STATUS_TX_OVERRUN_SHIFT)
#define SPI_STATUS_TX_OVERRUN_ERR		(0x1 << SPI_STATUS_TX_OVERRUN_SHIFT)

#define SPI_STATUS_RX_UNDERRUN_SHIFT	4
#define SPI_STATUS_RX_UNDERRUN_MASK		(0x1 << SPI_STATUS_RX_UNDERRUN_SHIFT)
#define SPI_STATUS_RX_UNDERRUN_OK		(0x0 << SPI_STATUS_RX_UNDERRUN_SHIFT)
#define SPI_STATUS_RX_UNDERRUN_ERR		(0x1 << SPI_STATUS_RX_UNDERRUN_SHIFT)

#define SPI_STATUS_RX_OVERRUN_SHIFT		5
#define SPI_STATUS_RX_OVERRUN_MASK		(0x1 << SPI_STATUS_RX_OVERRUN_SHIFT)
#define SPI_STATUS_RX_OVERRUN_OK		(0x0 << SPI_STATUS_RX_OVERRUN_SHIFT)
#define SPI_STATUS_RX_OVERRUN_ERR		(0x1 << SPI_STATUS_RX_OVERRUN_SHIFT)

#define SPI_STATUS_TX_FIFO_LVL_SHIFT	6
#define SPI_STATUS_TX_FIFO_LVL_MASK		(0x1FF)

#define SPI_STATUS_RX_FIFO_LVL_SHIFT	15
#define SPI_STATUS_RX_FIFO_LVL_MASK		(0x1FF)

#endif /* __ARCH_ARM_SRC_S5J_CHIP_S5JT200_SPI_H */
