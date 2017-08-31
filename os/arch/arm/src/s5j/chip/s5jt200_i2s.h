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
/************************************************************************************
 * arch/arm/src/s5j//chip/s5jt200_i2s.h
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
 ************************************************************************************/

#ifndef __ARCH_ARM_SRC_S5JT200_CHIP_S5J_I2S_H
#define __ARCH_ARM_SRC_S5JT200_CHIP_S5J_I2S_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>
#include "chip/s5j_memorymap.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#define S5J_I2S_MAXPERCLK         26000000  /* Maximum peripheral clock frequency */

/* I2S Register Offsets *************************************************************/

#define S5J_I2S_CON     0x0000  /* I2S interface control */
#define S5J_I2S_MOD     0x0004  /* I2S interface mode */
#define S5J_I2S_FIC     0x0008  /* I2S interface TxFIFO and Rx FIFO control */
#define S5J_I2S_PSR     0x000C  /* I2S interface clock divider control */
#define S5J_I2S_TXD     0x0010  /* I2S interface transmit sound data */
#define S5J_I2S_RXD     0x0014  /* I2S interface receive data */
#define S5J_I2S_FICS    0x0018  /* I2S interface secondary TxFIFO_S Control */
#define S5J_I2S_TXDS    0x001C  /* I2S interface transmit secondary data */
#define S5J_I2S_VER     0x0044  /* I2S version */

/* I2S Register Bit Definitions *****************************************************/

#define I2S_CR_SW_RST_SHIFT     (31)
#define I2S_CR_SW_RST_MASK      (1 << I2S_CR_SW_RST_SHIFT)
#define I2S_CR_SW_RST_RELEASE   (1 << I2S_CR_SW_RST_SHIFT)

#define I2S_CR_FRXOFSTATUS  (1 << 26)
#define I2S_CR_FRXOFINTEN   (1 << 25)
#define I2S_CR_FTXSURSTATUS (1 << 24)
#define I2S_CR_FTXSURINTEN  (1 << 23)
#define I2S_CR_FTXSEMPT     (1 << 22)
#define I2S_CR_FTXSFULL     (1 << 21)
#define I2S_CR_TXSDMAPAUSE  (1 << 20)
#define I2S_CR_TXSDMACTIVE  (1 << 18)
#define I2S_CR_FTXURSTATUS  (1 << 17)
#define I2S_CR_FTXURINTEN   (1 << 16)
#define I2S_CR_LRI          (1 << 11)
#define I2S_CR_FTX0EMPT     (1 << 10)
#define I2S_CR_FRXEMPT      (1 << 9)
#define I2S_CR_FTX0FULL     (1 << 8)
#define I2S_CR_FRXFULL      (1 << 7)
#define I2S_CR_TXDMAPAUSE   (1 << 6)
#define I2S_CR_RXDMAPAUSE   (1 << 5)
#define I2S_CR_TXCHPAUSE    (1 << 4)
#define I2S_CR_RXCHPAUSE    (1 << 3)
#define I2S_CR_TXDMACTIVE   (1 << 2)
#define I2S_CR_RXDMACTIVE   (1 << 1)
#define I2S_CR_I2SACTIVE    (1 << 0)

/* MOD register */
#define I2S_MOD_OP_CLK_MASK (3 << 30)
#define I2S_MOD_OP_CLK_PCLK (3 << 30)

#define BLC_8BIT    (1)
#define BLC_16BIT   (0)
#define BLC_24BIT   (2)

#define I2S_MOD_BLC_S_MASK  (3 << 26)
#define I2S_MOD_BLC_S(x)    ((x & 3) << 26)
#define I2S_MOD_BLC_P_MASK  (3 << 24)
#define I2S_MOD_BLC_P(x)    ((x & 3) << 24)
#define I2S_MOD_LRP         (1 << 15)
#define I2S_MOD_BLC_MASK    (3 << 13)
#define I2S_MOD_BLC(x)      ((x & 3) << 13)
#define I2S_MOD_CDCLKCON_IN (1 << 12)
#define I2S_MOD_MSS_SLAVE   (1 << 11)
#define I2S_MOD_RCLKSRC     (1 << 10)

#define TXR_TX              0
#define TXR_RX              1
#define TXR_TXRX            2
#define I2S_MOD_TXR_MASK    (3 << 8)
#define I2S_MOD_TXR(x)      ((x & 3) << 8)

#define I2S_MOD_SDF_MASK        (3 << 6)
#define I2S_MOD_SDF_I2S         (0 << 6)
#define I2S_MOD_SDF_MSB_Jstf    (1 << 6)
#define I2S_MOD_SDF_LSB_Jstf    (2 << 6)

#define RFS_256     (0x0)
#define RFS_192     (0x7)

#define BFS_64      (0x4)

#define I2S_MOD_RFS_MASK    (7 << 3)
#define I2S_MOD_RFS(x)      ((x & 7) << 3)
#define I2S_MOD_BFS_MASK    (7 << 0)
#define I2S_MOD_BFS(x)      ((x & 7) << 0)

/* FIC register */
#define I2S_FIC_TFLUSH      (1 << 15)
#define I2S_FIC_FTX0CNT     (0x7F << 8)
#define I2S_FIC_RFLUSH      (1 << 7)
#define I2S_FIC_FRXCNT      (0x7F << 0)

#define I2S_PSR_PSREN       (1 << 15)
#define I2S_PSR_PSVAL       (0x3F << 8)

#define I2S_FICS_TXDMA_TH_EN    (1 << 31)
#define I2S_FICS_TXDMA_TH       (0x3F << 24)
#define I2S_FICS_RXDMA_TH_EN    (1 << 23)
#define I2S_FICS_RXDMA_TH       (0x3F << 16)
#define I2S_FICS_TFLUSHS        (1 << 15)
#define I2S_FICS_FTXSCNT        (0x7F << 8)

#endif							/* __ARCH_ARM_SRC_S5JT200_CHIP_S5J_I2S_H */
