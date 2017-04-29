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
 * arch/arm/src/s5j/s5j200_sflash.h
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

#ifndef __ARCH_ARM_SRC_S5J_CHIP_S5JT200_SFLASH_H
#define __ARCH_ARM_SRC_S5J_CHIP_S5JT200_SFLASH_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Address *********************************************************/
#define S5J_SFLASH_SFCON				0x80310004
#define S5J_SFLASH_ERASE_ADDRESS		0x80310010
#define S5J_SFLASH_USER_COMMAND			0x80310018
#define S5J_SFLASH_COMMAND1				0x8031001C
#define S5J_SFLASH_COMMAND2				0x80310020
#define S5J_SFLASH_COMMAND3				0x80310024
#define S5J_SFLASH_COMMAND4				0x80310028
#define S5J_SFLASH_COMMAND5				0x8031002C
#define S5J_SFLASH_USER_INSTRUCTION		0x80310059
#define S5J_SFLASH_SE					0x8031005E
#define S5J_SFLASH_IO_MODE				0x80310074
#define S5J_SFLASH_PERF_MODE			0x80310078
#define S5J_SFLASH_RDID					0x803100AC
#define S5J_SFLASH_BE					0x803100BE
#define S5J_SFLASH_CE					0x803100CE
#define S5J_SFLASH_RDSR					0x803100DC
#define S5J_SFLASH_WRDI					0x803100DD
#define S5J_SFLASH_WRSR					0x803100DE
#define S5J_SFLASH_WREN					0x803100EE

/* Register Bitfield Definitions ********************************************/

/* Control Register */
#define SFLASH_SFCON_WP_SHIFT			31
#define SFLASH_SFCON_WP_MASK			(0x1 << SFLASH_SFCON_WP_SHIFT)
#define SFLASH_SFCON_WP_ENABLE			(0x0 << SFLASH_SFCON_WP_SHIFT)
#define SFLASH_SFCON_WP_DISABLE			(0x1 << SFLASH_SFCON_WP_SHIFT)

#define SFLASH_SFCON_MEMORY_VENDOR_SHIFT	16
#define SFLASH_SFCON_MEMORY_VENDOR_MASK		(0x3f << SFLASH_SFCON_MEMORY_VENDOR_SHIFT)
#define SFLASH_SFCON_MEMORY_VENDOR_WINBOND	(0x20 << SFLASH_SFCON_MEMORY_VENDOR_SHIFT)
#define SFLASH_SFCON_MEMORY_VENDOR_MACRONIX	(0x10 << SFLASH_SFCON_MEMORY_VENDOR_SHIFT)
#define SFLASH_SFCON_MEMORY_VENDOR_ATMEL	(0x08 << SFLASH_SFCON_MEMORY_VENDOR_SHIFT)
#define SFLASH_SFCON_MEMORY_VENDOR_AMD		(0x04 << SFLASH_SFCON_MEMORY_VENDOR_SHIFT)
#define SFLASH_SFCON_MEMORY_VENDOR_STMICRO	(0x02 << SFLASH_SFCON_MEMORY_VENDOR_SHIFT)
#define SFLASH_SFCON_MEMORY_VENDOR_SST		(0x01 << SFLASH_SFCON_MEMORY_VENDOR_SHIFT)

#define SFLASH_SFCON_PAGE_EN_SHIFT		15
#define SFLASH_SFCON_PAGE_EN_MASK		(0x1 << SFLASH_SFCON_PAGE_EN_SHIFT)
#define SFLASH_SFCON_PAGE_EN_BYTEPROG	(0x0 << SFLASH_SFCON_PAGE_EN_SHIFT)
#define SFLASH_SFCON_PAGE_EN_PAGEPROG	(0x1 << SFLASH_SFCON_PAGE_EN_SHIFT)

#define SFLASH_SFCON_PAGE_SHIFT			8
#define SFLASH_SFCON_PAGE_MASK			(0xf << SFLASH_SFCON_PAGE_SHIFT)
#define SFLASH_SFCON_PAGE_4BYTES		(0x0 << SFLASH_SFCON_PAGE_SHIFT)
#define SFLASH_SFCON_PAGE_8BYTES		(0x1 << SFLASH_SFCON_PAGE_SHIFT)
#define SFLASH_SFCON_PAGE_16BYTES		(0x2 << SFLASH_SFCON_PAGE_SHIFT)
#define SFLASH_SFCON_PAGE_32BYTES		(0x3 << SFLASH_SFCON_PAGE_SHIFT)
#define SFLASH_SFCON_PAGE_64BYTES		(0x4 << SFLASH_SFCON_PAGE_SHIFT)
#define SFLASH_SFCON_PAGE_128BYTES		(0x5 << SFLASH_SFCON_PAGE_SHIFT)
#define SFLASH_SFCON_PAGE_256BYTES		(0x6 << SFLASH_SFCON_PAGE_SHIFT)
#define SFLASH_SFCON_PAGE_RESERVED		(0x7 << SFLASH_SFCON_PAGE_SHIFT)

#define SFLASH_SFCON_HALF_DELAY_SHIFT	7
#define SFLASH_SFCON_HALF_DELAY_MASK	(0x1 << SFLASH_SFCON_HALF_DELAY_SHIFT)
#define SFLASH_SFCON_HALF_DELAY_OFF		(0x0 << SFLASH_SFCON_HALF_DELAY_SHIFT)
#define SFLASH_SFCON_HALF_DELAY_ON		(0x1 << SFLASH_SFCON_HALF_DELAY_SHIFT)

#define SFLASH_SFCON_ERASE_WAIT_SHIFT	4
#define SFLASH_SFCON_ERASE_WAIT_MASK	(0x1 << SFLASH_SFCON_ERASE_WAIT_SHIFT)
#define SFLASH_SFCON_ERASE_WAIT_OFF		(0x0 << SFLASH_SFCON_ERASE_WAIT_SHIFT)
#define SFLASH_SFCON_ERASE_WAIT_ON		(0x1 << SFLASH_SFCON_ERASE_WAIT_SHIFT)

#define SFLASH_SFCON_PRE_CHARGE_SHIFT	0
#define SFLASH_SFCON_PRE_CHARGE_MASK	(0xf << SFLASH_SFCON_PRE_CHARGE_SHIFT)

/* Flash I/O Mode */
#define SFLASH_IO_MODE_MASK				0xf
#define SFLASH_IO_MODE_READ				0x0
#define SFLASH_IO_MODE_FAST_READ		0x1
#define SFLASH_IO_MODE_DUAL_FAST_READ	0x2
#define SFLASH_IO_MODE_QUAD_FAST_READ	0x4

/* Flash Performance Mode */
#define SFLASH_PERF_MODE_MASK			0x8
#define SFLASH_PERF_MODE_NORMAL			0x0
#define SFLASH_PERF_MODE_DUAL_QUAD		0x8

#endif /* __ARCH_ARM_SRC_S5J_CHIP_S5JT200_SFLASH_H */
