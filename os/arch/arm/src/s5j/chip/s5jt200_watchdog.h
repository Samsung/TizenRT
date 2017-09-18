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
 * arch/arm/src/s5j/chip/s5j200_watchdog.h
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

#ifndef __ARCH_ARM_SRC_S5J_CHIP_S5JT200_WATCHDOG_H
#define __ARCH_ARM_SRC_S5J_CHIP_S5JT200_WATCHDOG_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Address *********************************************************/
#define S5J_WDT_WTCON							0x80030000
#define S5J_WDT_WTDAT							0x80030004
#define S5J_WDT_WTCNT							0x80030008
#define S5J_WDT_WTCLRINT						0x8003000C

#define S5J_WDT_OSC							(26*1000*1000)

/* Register Bitfield Definitions ********************************************/

/* WTCON register */
#define WTCON_PRESCALER(a)						((a & 0xF) << 8)
#define WTCON_WATCHDOG_EN						(1 << 5)
#define WTCON_WATCHDOG_DIS						(0 << 5)
#define WTCON_CLOCK(a)							((a & 0x3) << 3)
#define WTCON_IRQ_EN							(1 << 2)
#define WTCON_IRQ_DIS							(0 << 2)
#define WTCON_RESET_EN							(1 << 0)
#define WTCON_RESET_DIS							(0 << 0)

#endif							/* __ARCH_ARM_SRC_S5J_CHIP_S5JT200_WATCHDOG_H */
