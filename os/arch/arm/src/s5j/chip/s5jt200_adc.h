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
 * arch/arm/src/s5j/chip/s5j200_adc.h
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

#ifndef __ARCH_ARM_SRC_S5J_CHIP_S5JT200_ADC_H
#define __ARCH_ARM_SRC_S5J_CHIP_S5JT200_ADC_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Address *********************************************************/
#define S5J_ADC_CON1			0x802D0000
#define S5J_ADC_CON2			0x802D0004
#define S5J_ADC_DAT				0x802D0008
#define S5J_ADC_SUMDAT			0x802D000C
#define S5J_ADC_INT				0x802D0010
#define S5J_ADC_INT_STATUS		0x802D0014
#define S5J_ADC_DEBUG_DATA		0x802D001C
#define S5J_ADC_VERSION			0x802D0028

/* Register Bitfield Definitions ********************************************/

/* ADC control 1 register */
#define ADC_CON1_SOFTRESET_SHIFT	1
#define ADC_CON1_SOFTRESET_MASK		(0x3 << ADC_CON1_SOFTRESET_SHIFT)
#define ADC_CON1_SOFTRESET_RESET	(0x2 << ADC_CON1_SOFTRESET_SHIFT)
#define ADC_CON1_SOFTRESET_NONRESET	(0x1 << ADC_CON1_SOFTRESET_SHIFT)

#define ADC_CON1_STCEN_SHIFT		0
#define ADC_CON1_STCEN_MASK			(0x1 << ADC_CON1_STCEN_SHIFT)
#define ADC_CON1_STCEN_DISABLE		(0x0 << ADC_CON1_STCEN_SHIFT)
#define ADC_CON1_STCEN_ENABLE		(0x1 << ADC_CON1_STCEN_SHIFT)

/* ADC control 2 register */
#define ADC_CON2_CTIME_SHIFT		4
#define ADC_CON2_CTIME_MASK			(0x7 << ADC_CON2_CTIME_SHIFT)
#define ADC_CON2_CTIME_1			(0x0 << ADC_CON2_CTIME_SHIFT)
#define ADC_CON2_CTIME_2			(0x1 << ADC_CON2_CTIME_SHIFT)
#define ADC_CON2_CTIME_4			(0x2 << ADC_CON2_CTIME_SHIFT)
#define ADC_CON2_CTIME_8			(0x3 << ADC_CON2_CTIME_SHIFT)
#define ADC_CON2_CTIME_16			(0x4 << ADC_CON2_CTIME_SHIFT)
#define ADC_CON2_CTIME_32			(0x5 << ADC_CON2_CTIME_SHIFT)
#define ADC_CON2_CTIME_64			(0x6 << ADC_CON2_CTIME_SHIFT)
#define ADC_CON2_CTIME_RESERVED		(0x7 << ADC_CON2_CTIME_SHIFT)

#define ADC_CON2_ACHSEL_SHIFT		0
#define ADC_CON2_ACHSEL_MASK		(0xf << ADC_CON2_ACHSEL_SHIFT)
#define ADC_CON2_ACHSEL_CHANNEL0	(0x0 << ADC_CON2_ACHSEL_SHIFT)
#define ADC_CON2_ACHSEL_CHANNEL1	(0x1 << ADC_CON2_ACHSEL_SHIFT)
#define ADC_CON2_ACHSEL_CHANNEL2	(0x2 << ADC_CON2_ACHSEL_SHIFT)
#define ADC_CON2_ACHSEL_CHANNEL3	(0x3 << ADC_CON2_ACHSEL_SHIFT)

/* ADC average data and flag register */
#define ADC_DAT_FLAG_SHIFT			31
#define ADC_DAT_FLAG_MASK			(0x1 << ADC_DAT_FLAG_SHIFT)
#define ADC_DAT_FLAG_INPROGRESS		(0x0 << ADC_DAT_FLAG_SHIFT)
#define ADC_DAT_FLAG_END			(0x1 << ADC_DAT_FLAG_SHIFT)

#define ADC_DAT_ADCDAT_SHIFT		0
#define ADC_DAT_ADCDAT_MASK			(0xfff << ADC_DAT_ADCDAT_SHIFT)

/* ADC sum data and flag register */
#define ADC_SUMDAT_FLAG_SHIFT		31
#define ADC_SUMDAT_FLAG_MASK		(0x1 << ADC_SUM_DAT_FLAG_SHIFT)
#define ADC_SUMDAT_FLAG_INPROGRESS	(0x0 << ADC_SUM_DAT_FLAG_SHIFT)
#define ADC_SUMDAT_FLAG_END			(0x1 << ADC_SUM_DAT_FLAG_SHIFT)

#define ADC_SUMDAT_ADCDAT_SHIFT		0
#define ADC_SUMDAT_ADCDAT_MASK		(0xfff << ADC_SUM_DAT_ADCDAT_SHIFT)

/* ADC interrupt enable/disable register */
#define ADC_INT_DISABLE				0
#define ADC_INT_ENABLE				1

/* ADC interrupt status register */
#define ADC_INT_STATUS_PENDING		1

#endif							/* __ARCH_ARM_SRC_S5J_CHIP_S5JT200_ADC_H */
