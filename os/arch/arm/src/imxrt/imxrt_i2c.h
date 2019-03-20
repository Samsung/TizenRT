/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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
 * os/arch/arm/src/imxrt/imxrt_lpi2c.h
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author: Ivan Ucherdzhiev <ivanucherdjiev@gmail.com>
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

#ifndef __ARCH_ARM_SRC_IMXRT_IMX_LPI2C_H
#define __ARCH_ARM_SRC_IMXRT_IMX_LPI2C_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/i2c.h>

#include "chip.h"
#include "chip/imxrt_lpi2c.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define CCM_CSCDR2_LPI2C_CLK_SEL_OSC_CLK       (1 << 18)	/*  derive clock from ock_clk */
#define LPI2C_MSR_ERROR_MASK                (LPI2C_MSR_NDF(1U) | LPI2C_MSR_ALF(1U) | \
                                             LPI2C_MSR_BBF(1U) | LPI2C_MSR_FEF(1U))

#define LPI2C_MTDR_CMD_TXD                (0 << LPI2C_MTDR_CMD_SHIFT)
#define LPI2C_MTDR_CMD_RXD                (1 << LPI2C_MTDR_CMD_SHIFT)
#define LPI2C_MTDR_CMD_STOP               (2 << LPI2C_MTDR_CMD_SHIFT)
#define LPI2C_MTDR_CMD_RXD_DISC           (3 << LPI2C_MTDR_CMD_SHIFT)
#define LPI2C_MTDR_CMD_START              (4 << LPI2C_MTDR_CMD_SHIFT)
#define LPI2C_MTDR_CMD_START_NACK         (5 << LPI2C_MTDR_CMD_SHIFT)
#define LPI2C_MTDR_CMD_START_HI           (6 << LPI2C_MTDR_CMD_SHIFT)
#define LPI2C_MTDR_CMD_START_HI_NACK      (7 << LPI2C_MTDR_CMD_SHIFT)

/* LPI2C Master Config Register 0  */

#define LPI2C_MCFG0_HREN                    (1 << 0)	/* Host Request Enable Bit */
#define LPI2C_MCFG0_HRPOL                   (1 << 1)	/* Host Request Polarity Bit */
#define LPI2C_MCFG0_HRSEL                   (1 << 2)	/* Host Request Select Bit */
/* Bits 7-3 Reserved */
#define LPI2C_MCFG0_CIRFIFO                 (1 << 8)	/* Circular FIFO Enable Bit */
#define LPI2C_MCFG0_RDMO                    (1 << 9)	/* Receive Data Match Only Bit */
/* Bits 31-10 Reserved */

#define LPI2C_MCFG2_BUSIDLE_MASK            (0xfff << 0)	/* Bus Idle Timeout Period in Clock Cycles */
#define LPI2C_MCFG2_BUSIDLE_DISABLE         (0)
#define LPI2C_MCFG2_BUSIDLE(n)              (n & LPI2C_MCFG2_BUSIDLE_MASK)

#define LPI2C_MCFG2_FILTSCL_SHIFT           (16)
#define LPI2C_MCFG2_FILTSCL_MASK            (15 << LPI2C_MCFG2_FILTSCL_SHIFT)	/* Glitch Filter SCL */
#define LPI2C_MCFG2_FILTSCL_DISABLE         (0 << LPI2C_MCFG2_FILTSCL_SHIFT)
#define LPI2C_MCFG2_FILTSCL_CYCLES(n)       ((n << LPI2C_MCFG2_FILTSCL_SHIFT) & LPI2C_MCFG2_FILTSCL_MASK)

#define LPI2C_MCFG2_FILTSDA_SHIFT           (24)
#define LPI2C_MCFG2_FILTSDA_MASK            (15 << LPI2C_MCFG2_FILTSDA_SHIFT)	/* Glitch Filter SDA */
#define LPI2C_MCFG2_FILTSDA_DISABLE         (0 << LPI2C_MCFG2_FILTSDA_SHIFT)
#define LPI2C_MCFG2_FILTSDA_CYCLES(n)       ((n << LPI2C_MCFG2_FILTSDA_SHIFT) & LPI2C_MCFG2_FILTSDA_MASK)

#define LPI2C_MCFG3_PINLOW_SHIFT            (8)
#define LPI2C_MCFG3_PINLOW_MASK             (0xfff << LPI2C_MCFG3_PINLOW_SHIFT)	/* Configure The Pin Low Timeout in Clock Cycles */
#define LPI2C_MCFG3_PINLOW_CYCLES(n)        ((n << LPI2C_MCFG3_PINLOW_SHIFT) & LPI2C_MCFG3_PINLOW_MASK)

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/****************************************************************************
 * Name: imxrt_i2cbus_initialize
 *
 * Description:
 *   Initialize the selected I2C port. And return a unique instance of struct
 *   struct i2c_master_s.  This function may be called to obtain multiple
 *   instances of the interface, each of which may be set up with a
 *   different frequency and slave address.
 *
 * Input Parameters:
 *   Port number (for hardware that has multiple I2C interfaces)
 *
 * Returned Value:
 *   Valid I2C device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/

FAR struct i2c_master_s *imxrt_i2cbus_initialize(int port);

/****************************************************************************
 * Name: imxrt_i2cbus_uninitialize
 *
 * Description:
 *   De-initialize the selected I2C port, and power down the device.
 *
 * Input Parameters:
 *   Device structure as returned by the imxrt_i2cbus_initialize()
 *
 * Returned Value:
 *   OK on success, ERROR when internal reference count mismatch or dev
 *   points to invalid hardware device.
 *
 ****************************************************************************/

int imxrt_i2cbus_uninitialize(FAR struct i2c_master_s *dev);

#endif							/* __ARCH_ARM_SRC_IMXRT_IMX_LPI2C_H */
