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

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register definition for RTC */
#define INTP        0x0030
#define RTCCON      0x0040
#define TICCNT      0x0044
#define RTCALM      0x0050
#define ALMSEC      0x0054
#define ALMMIN      0x0058
#define ALMHOUR     0x005C
#define ALMDAY      0x0060
#define ALMMON      0x0064
#define ALMYEAR     0x0068
#define BCDSEC      0x0070
#define BCDMIN      0x0074
#define BCDHOUR     0x0078
#define BCDDAY      0x007C
#define BCDDAYWEEK  0x0080
#define BCDMON      0x0084
#define BCDYEAR     0x0088
#define CURTICCNT   0x0090

#define RTC_GLB_ALM_EN		(0x1 << 6)
#define RTC_YEAR_ALM_EN		(0x1 << 5)
#define RTC_MONTH_ALM_EN	(0x1 << 4)
#define RTC_DAY_ALM_EN		(0x1 << 3)
#define RTC_HOUR_ALM_EN		(0x1 << 2)
#define RTC_MIN_ALM_EN		(0x1 << 1)
#define RTC_SEC_ALM_EN		(0x1 << 0)


#define RTCCON_CLKOUTEN 	(1 << 9)
#define RTCCON_TICEN 		(1 << 8)
#define RTCCON_TICCKSEL(x) 	((x & 0xf) << 4)
#define RTCCON_CLKSTOP 		(1 << 3)
#define RTCCON_CTLEN 		(1 << 0)

#define RTC_CLK_FREQ 		32768

typedef enum t_ticcksel {
	clk_32768HZ = 0,
	clk_16384HZ,
	clk_8192HZ,
	clk_4096HZ,
	clk_2048HZ,
	clk_1024HZ,
	clk_512HZ,
	clk_256HZ,
	clk_128HZ,
	clk_64HZ,
	clk_32HZ,
	clk_16HZ,
	clk_8HZ,
	clk_4HZ,
	clk_2HZ,
	clk_1HZ,
} d_ticcksel;


#define RTC_INTP_TIC_MASK 	(1 << 0)
#define RTC_INTP_ALM_MASK 	(1 << 1)

/****************************************************************************
 * Public Functions
 ****************************************************************************/


#ifdef __cplusplus
}
#endif
#endif							/* __ARCH_ARM_SRC_S5J_S5J_RTC_H */
