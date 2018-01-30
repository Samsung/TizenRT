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
 * arch/arm/src/s5j/chip/s5j200_efuse.h
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

#ifndef __ARCH_ARM_SRC_S5J_CHIP_S5JT200_EFUSE_H
#define __ARCH_ARM_SRC_S5J_CHIP_S5JT200_EFUSE_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Register Offset **********************************************************/
#define S5J_EFUSE_PRODUCT_ID			0x0000
#define S5J_EFUSE_SFR_APB				0x0100
#define S5J_EFUSE_CHIPID_0				0x0204
#define S5J_EFUSE_CHIPID_1				0x0208
#define S5J_EFUSE_CHIPID_2				0x020C
#define S5J_EFUSE_CHIPID_3				0x0210
#define S5J_EFUSE_PRODUCT_VER			0x0220
#define S5J_EFUSE_N_SECU_RD_DATA0		0x0318
#define S5J_EFUSE_N_SECU_RD_DATA1		0x031C
#define S5J_EFUSE_N_SECU_RD_DATA2		0x0320
#define S5J_EFUSE_N_SECU_RD_DATA3		0x0324
#define S5J_EFUSE_CONTROL				0x0328
#define S5J_EFUSE_CONFIG				0x032C
#define S5J_EFUSE_PROG_DATA0			0x0334
#define S5J_EFUSE_PROG_DATA1			0x0338
#define S5J_EFUSE_PROG_DATA2			0x033C
#define S5J_EFUSE_PROG_DATA3			0x0340
#define S5J_EFUSE_PROG_DEBUG			0x0370
#define S5J_EFUSE_SENSING_DEBUG			0x0374
#define S5J_EFUSE_INT					0x03FC
#define S5J_EFUSE_INT_STATUS			0x0400
#define S5J_EFUSE_INT_EN				0x0404
#define S5J_EFUSE_TIME_PARA0			0x0408
#define S5J_EFUSE_TIME_PARA1			0x040C
#define S5J_EFUSE_STATUS				0x0428
#define S5J_EFUSE_SECU_SPARE0			0x042C
#define S5J_EFUSE_N_SECU_SPARE0			0x0434
#define S5J_EFUSE_SECKEY0				0x0500
#define S5J_EFUSE_SECKEY1				0x0504
#define S5J_EFUSE_SECKEY2				0x0508
#define S5J_EFUSE_SECKEY3				0x050C
#define S5J_EFUSE_SECKEY4				0x0510
#define S5J_EFUSE_SECKEY5				0x0514
#define S5J_EFUSE_SECKEY6				0x0518
#define S5J_EFUSE_SECKEY7				0x051C

/* Register Bitfield Definitions ********************************************/
/* CONTROL ******************************************************************/
#define EFUSE_CONTROL_ADDRESS_SHIFT			0
#define EFUSE_CONTROL_ADDRESS_MASK			(0x7FFF << EFUSE_CONTROL_ADDRESS_SHIFT)
#define EFUSE_CONTROL_ADDRESS_EN			(0x1 << EFUSE_CONTROL_ADDRESS_SHIFT)

#define EFUSE_CONTROL_SW_PRE_READING_SHIFT	7
#define EFUSE_CONTROL_SW_PRE_READING_MASK	(0x1 << EFUSE_CONTROL_SW_PRE_READING_SHIFT)

#define EFUSE_CONTROL_PGM_SIZE_SHIFT		18
#define EFUSE_CONTROL_PGM_SIZE_MASK			(0x3 << EFUSE_CONTROL_PGM_SIZE_SHIFT)

#define EFUSE_CONTROL_READ_SIZE_SHIFT		20
#define EFUSE_CONTROL_READ_SIZE_MASK		(0x3 << EFUSE_CONTROL_READ_SIZE_SHIFT)

/* INT_STATUS ***************************************************************/
#define EFUSE_INT_STATUS_READ_DONE_SHIFT			0
#define EFUSE_INT_STATUS_READ_DONE_MASK				(0x1 << EFUSE_INT_STATUS_READ_DONE_SHIFT)

#define EFUSE_INT_STATUS_PROG_DONE_SHIFT			1
#define EFUSE_INT_STATUS_PROG_DONE_MASK				(0x1 << EFUSE_INT_STATUS_PROG_DONE_SHIFT)

#define EFUSE_INT_STATUS_PROG_VERIFY_FAIL_SHIFT		2
#define EFUSE_INT_STATUS_PROG_VERIFY_FAIL_MASK		(0x1 << EFUSE_INT_STATUS_PROG_VERIFY_FAIL_SHIFT)

#define EFUSE_INT_STATUS_PROG_VERIFY_DONE_SHIFT		3
#define EFUSE_INT_STATUS_PROG_VERIFY_DONE_MASK		(0x1 << EFUSE_INT_STATUS_PROG_VERIFY_DONE_SHIFT)

#endif							/* __ARCH_ARM_SRC_S5J_CHIP_S5JT200_EFUSE_H */
