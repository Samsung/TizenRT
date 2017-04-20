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
 * arch/arm/src/s5j/s5j_spi.h
 *
 *   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
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

#ifndef __ARCH_ARM_SRC_S5J_S5J_SPIDRV_H
#define __ARCH_ARM_SRC_S5J_S5J_SPIDRV_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <arch/chip/irq.h>
#include <chip.h>
#include <chip/s5j_gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

typedef struct _SPI_SFR {
	u32 CH_CFG;				/*  0x00 spi configuration register */
	u32 CLK_CFG;			/*  0x04 deleted */
	u32 MODE_CFG;			/*  0x08 spi fifo control register */
	u32 CS_REG;				/*  0x0C slave selection signal */
	u32 SPI_INT_EN;			/*  0x10 spi interrupt enable register */
	u32 SPI_STATUS;			/*  0x14 spi status register */
	u32 SPI_TX_DATA;		/*  0x18 spi tx data register */
	u32 SPI_RX_DATA;		/*  0x1C spi rx data register */
	u32 PACKET_CNT_REG;		/*  0x20 count how many data master gets. */
	u32 PENDING_CLR_REG;	/*  0x24 pending clear register */
	u32 SWAP_CFG;			/*  0x28 swap config register */
	u32 FB_CLK_SEL;			/*  0x2C feedback clock config register */
} SPI_SFR;

typedef enum {
	SPI_PORT0 = 0,
	SPI_PORT1,
	SPI_PORT2,
	SPI_PORT3,
	SPI_PORT_MAX,
} SPI_PORT;

#ifdef __cplusplus
}
#endif
#endif /* __ARCH_ARM_SRC_S5J_S5J_SPIDRV_H */
