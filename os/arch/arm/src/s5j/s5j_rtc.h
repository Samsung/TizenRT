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
 * arch/arm/src/s5j/s5j_rtc.h
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

#ifndef __ARCH_ARM_SRC_S5J_S5J_RTC_H
#define __ARCH_ARM_SRC_S5J_S5J_RTC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define S5J_RTC_INTP			0x80100030
#define S5J_RTC_RTCCON			0x80100040
#define S5J_RTC_TICCNT0			0x80100044
#define S5J_RTC_RTCALM			0x80100050
#define S5J_RTC_ALMSEC			0x80100054
#define S5J_RTC_ALMMIN			0x80100058
#define S5J_RTC_ALMHOUR			0x8010005C
#define S5J_RTC_ALMDAY			0x80100060
#define S5J_RTC_ALMMON			0x80100064
#define S5J_RTC_ALMYEAR			0x80100068
#define S5J_RTC_BCDSEC			0x80100070
#define S5J_RTC_BCDMIN			0x80100074
#define S5J_RTC_BCDHOUR			0x80100078
#define S5J_RTC_BCDDAY			0x8010007C
#define S5J_RTC_BCDDAYWEEK		0x80100080
#define S5J_RTC_BCDMON			0x80100084
#define S5J_RTC_BCDYEAR			0x80100088
#define S5J_RTC_CURTICCNT0		0x80100090

/* S5J_RTC_INTP */
#define RTC_INTP_ALARM			(0x1 << 1)
#define RTC_INTP_TIMETIC0		(0x1 << 0)

/* S5J_RTC_RTCCON */
#define RTC_RTCCON_CLKOUTEN		(0x1 << 9)
#define RTC_RTCCON_TICKEN0		(0x1 << 8)
#define RTC_RTCCON_TICCKSEL0_MASK	(0xf << 4)
#define RTC_RTCCON_TICCKSEL0_32768HZ	(0x0 << 4)
#define RTC_RTCCON_TICCKSEL0_16384HZ	(0x1 << 4)
#define RTC_RTCCON_TICCKSEL0_8192HZ	(0x2 << 4)
#define RTC_RTCCON_TICCKSEL0_4096HZ	(0x3 << 4)
#define RTC_RTCCON_TICCKSEL0_2048HZ	(0x4 << 4)
#define RTC_RTCCON_TICCKSEL0_1024HZ	(0x5 << 4)
#define RTC_RTCCON_TICCKSEL0_512HZ	(0x6 << 4)
#define RTC_RTCCON_TICCKSEL0_256HZ	(0x7 << 4)
#define RTC_RTCCON_TICCKSEL0_128HZ	(0x8 << 4)
#define RTC_RTCCON_TICCKSEL0_64HZ	(0x9 << 4)
#define RTC_RTCCON_TICCKSEL0_32HZ	(0xa << 4)
#define RTC_RTCCON_TICCKSEL0_16HZ	(0xb << 4)
#define RTC_RTCCON_TICCKSEL0_8HZ	(0xc << 4)
#define RTC_RTCCON_TICCKSEL0_4HZ	(0xd << 4)
#define RTC_RTCCON_TICCKSEL0_2HZ	(0xe << 4)
#define RTC_RTCCON_TICCKSEL0_1HZ	(0xf << 4)
#define RTC_RTCCON_CLKRST_MASK		(0x1 << 3)
#define RTC_RTCCON_CLKRST_ENABLE	(0x0 << 3)
#define RTC_RTCCON_CLKRST_DISABLE	(0x1 << 3)
#define RTC_RTCCON_CNTSEL_MASK		(0x1 << 2)
#define RTC_RTCCON_CNTSEL_MERGE_BCDCNT	(0x0 << 2)
#define RTC_RTCCON_CNTSEL_RESERVED	(0x1 << 2)
#define RTC_RTCCON_CLKSEL_MASK		(0x1 << 1)
#define RTC_RTCCON_CLKSEL_DIV32768	(0x0 << 1)
#define RTC_RTCCON_CLKSEL_RESERVED	(0x1 << 1)
#define RTC_RTCCON_CTLEN_MASK		(0x1 << 0)
#define RTC_RTCCON_CTLEN_DISABLE	(0x0 << 0)
#define RTC_RTCCON_CTLEN_ENABLE		(0x1 << 0)

/* S5J_RTC_RTCALM */
#define RTC_RTCALM_ALMEN_MASK		(0x1 << 6)
#define RTC_RTCALM_ALMEN_DISABLE	(0x0 << 6)
#define RTC_RTCALM_ALMEN_ENABLE		(0x1 << 6)
#define RTC_RTCALM_YEAREN_MASK		(0x1 << 5)
#define RTC_RTCALM_YEAREN_DISABLE	(0x0 << 5)
#define RTC_RTCALM_YEAREN_ENABLE	(0x1 << 5)
#define RTC_RTCALM_MONEN_MASK		(0x1 << 4)
#define RTC_RTCALM_MONEN_DISABLE	(0x0 << 4)
#define RTC_RTCALM_MONEN_ENABLE		(0x1 << 4)
#define RTC_RTCALM_DAYEN_MASK		(0x1 << 3)
#define RTC_RTCALM_DAYEN_DISABLE	(0x0 << 3)
#define RTC_RTCALM_DAYEN_ENABLE		(0x1 << 3)
#define RTC_RTCALM_HOUREN_MASK		(0x1 << 2)
#define RTC_RTCALM_HOUREN_DISABLE	(0x0 << 2)
#define RTC_RTCALM_HOUREN_ENABLE	(0x1 << 2)
#define RTC_RTCALM_MINEN_MASK		(0x1 << 1)
#define RTC_RTCALM_MINEN_DISABLE	(0x0 << 1)
#define RTC_RTCALM_MINEN_ENABLE		(0x1 << 1)
#define RTC_RTCALM_SECEN_MASK		(0x1 << 0)
#define RTC_RTCALM_SECEN_DISABLE	(0x0 << 0)
#define RTC_RTCALM_SECEN_ENABLE		(0x1 << 0)

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* The form of an alarm callback */
typedef void (*alarmcb_t)(void);

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: s5j_rtc_lowerhalf
 *
 * Description:
 *   Instantiate the RTC lower half driver for the S5J.
 *   General usage:
 *
 *     #include <tinyara/rtc.h>
 *     #include "s5j_rtc.h"
 *
 *     struct rtc_lowerhalf_s *lower;
 *     lower = s5j_rtc_lowerhalf();
 *     rtc_initialize(0, lower);
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   On success, a non-NULL RTC lower interface is returned. NULL is
 *   returned on any failure.
 *
 ****************************************************************************/
#ifdef CONFIG_RTC_DRIVER
FAR struct rtc_lowerhalf_s *s5j_rtc_lowerhalf(void);
#endif /* CONFIG_RTC_DRIVER */

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_S5J_S5J_RTC_H */
