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
 * arch/arm/src/s5j/chip/s5jt200_mct.h
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

#ifndef __ARCH_ARM_SRC_S5J_CHIP_S5JT200_MCT_H
#define __ARCH_ARM_SRC_S5J_CHIP_S5JT200_MCT_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Register Address *********************************************************/
#define S5J_MCT_CFG					S5J_MCT_BASE

#define S5J_MCT_TCNTB_OFFSET		0x0000
#define S5J_MCT_TCNTO_OFFSET		0x0004
#define S5J_MCT_ICNTB_OFFSET		0x0008
#define S5J_MCT_ICNTO_OFFSET		0x000C
#define S5J_MCT_FRCNTB_OFFSET		0x0010
#define S5J_MCT_FRCNTO_OFFSET		0x0014
#define S5J_MCT_TCON_OFFSET			0x0020
#define S5J_MCT_INT_CSTAT_OFFSET	0x0030
#define S5J_MCT_INT_ENB_OFFSET		0x0034
#define S5J_MCT_WSTAT_OFFSET		0x0040

/* Register Bitfield Definitions ********************************************/
/* MCT TCON register */
#define S5J_MCT_TCON_FRC_START		(1 << 3)
#define S5J_MCT_TCON_INTERVAL_MODE	(1 << 2)
#define S5J_MCT_TCON_INT_START		(1 << 1)
#define S5J_MCT_TCON_TIMER_START	(1 << 0)

/* MCT INTR register */
#define S5J_MCT_INTR_FRC			(1 << 1)
#define S5J_MCT_INTR_ICNT			(1 << 0)

/* MCT WSTAT register */
#define S5J_MCT_WSTAT_TCON			(1 << 3)
#define S5J_MCT_WSTAT_FRCCNTB		(1 << 2)
#define S5J_MCT_WSTAT_ICNTB			(1 << 1)
#define S5J_MCT_WSTAT_TCNTB			(1 << 0)

/* MCT CSTAT register */
#define S5J_MCT_INT_CSTAT_FRC		(1 << 1)
#define S5J_MCT_INT_CSTAT_ICNT		(1 << 0)

/* MCT INT register */
#define S5J_MCT_INT_ENB_FRC_MASK	(1 << 1)
#define S5J_MCT_INT_ENB_ICNT_MASK	(1 << 0)

#endif /* __ARCH_ARM_SRC_S5J_CHIP_S5JT200_MCT_H */
