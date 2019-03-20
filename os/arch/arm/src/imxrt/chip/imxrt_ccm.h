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
/************************************************************************************************************
 * os/arch/arm/src/imxrt/chip/imxrt_ccm.h
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author:  Janne Rosberg <janne@offcode.fi>
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
 ************************************************************************************************************/

#ifndef __ARCH_ARM_SRC_IMXRT_CHIP_IMXRT_CCM_H
#define __ARCH_ARM_SRC_IMXRT_CHIP_IMXRT_CCM_H

/************************************************************************************************************
 * Included Files
 ************************************************************************************************************/

#include <tinyara/config.h>
#include "chip/imxrt_memorymap.h"
#include "chip/imxrt105x_config.h"

/************************************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************************************/

/* Register offsets *****************************************************************************************/

#define IMXRT_CCM_CCR_OFFSET                     0x0000	/* CCM Control Register */
/* 0x0004  Reserved */
#define IMXRT_CCM_CSR_OFFSET                     0x0008	/* CCM Status Register */
#define IMXRT_CCM_CCSR_OFFSET                    0x000c	/* CCM Clock Switcher Register */
#define IMXRT_CCM_CACRR_OFFSET                   0x0010	/* CCM Arm Clock Root Register */
#define IMXRT_CCM_CBCDR_OFFSET                   0x0014	/* CCM Bus Clock Divider Register */
#define IMXRT_CCM_CBCMR_OFFSET                   0x0018	/* CCM Bus Clock Multiplexer Register */
#define IMXRT_CCM_CSCMR1_OFFSET                  0x001c	/* CCM Serial Clock Multiplexer Register 1 */
#define IMXRT_CCM_CSCMR2_OFFSET                  0x0020	/* CCM Serial Clock Multiplexer Register 2 */
#define IMXRT_CCM_CSCDR1_OFFSET                  0x0024	/* CCM Serial Clock Divider Register 1 */
#define IMXRT_CCM_CS1CDR_OFFSET                  0x0028	/* CCM Clock Divider Register */
#define IMXRT_CCM_CS2CDR_OFFSET                  0x002c	/* CCM Clock Divider Register */
#define IMXRT_CCM_CDCDR_OFFSET                   0x0030	/* CCM D1 Clock Divider Register */
/* 0x0034  Reserved */
#define IMXRT_CCM_CSCDR2_OFFSET                  0x0038	/* CCM Serial Clock Divider Register 2 */
#define IMXRT_CCM_CSCDR3_OFFSET                  0x003c	/* CCM Serial Clock Divider Register 3 */
/* 0x0040  Reserved */
/* 0x0044  Reserved */
#define IMXRT_CCM_CDHIPR_OFFSET                  0x0048	/* CCM Divider Handshake In-Process Register */
/* 0x004c  Reserved */
/* 0x0050  Reserved */
#define IMXRT_CCM_CLPCR_OFFSET                   0x0054	/* CCM Low Power Control Register */

#define IMXRT_CCM_CISR_OFFSET                    0x0058	/* CCM Interrupt Status Register */
#define IMXRT_CCM_CIMR_OFFSET                    0x005c	/* CCM Interrupt Mask Register */
#define IMXRT_CCM_CCOSR_OFFSET                   0x0060	/* CCM Clock Output Source Register */
#define IMXRT_CCM_CGPR_OFFSET                    0x0064	/* CCM General Purpose Register */
#define IMXRT_CCM_CCGR0_OFFSET                   0x0068	/* CCM Clock Gating Register 0 */
#define IMXRT_CCM_CCGR1_OFFSET                   0x006c	/* CCM Clock Gating Register 1 */
#define IMXRT_CCM_CCGR2_OFFSET                   0x0070	/* CCM Clock Gating Register 2 */
#define IMXRT_CCM_CCGR3_OFFSET                   0x0074	/* CCM Clock Gating Register 3 */
#define IMXRT_CCM_CCGR4_OFFSET                   0x0078	/* CCM Clock Gating Register 4 */
#define IMXRT_CCM_CCGR5_OFFSET                   0x007c	/* CCM Clock Gating Register 5 */
#define IMXRT_CCM_CCGR6_OFFSET                   0x0080	/* CCM Clock Gating Register 6 */
/* 0x0084  Reserved */
#define IMXRT_CCM_CMEOR_OFFSET                   0x0088	/* CCM Module Enable Overide Register */

/* Analog */

#define IMXRT_CCM_ANALOG_PLL_ARM_OFFSET          0x0000	/* Analog ARM PLL control Register */
#define IMXRT_CCM_ANALOG_PLL_USB1_OFFSET         0x0010	/* Analog USB1 480MHz PLL Control Register */
#define IMXRT_CCM_ANALOG_PLL_USB2_OFFSET         0x0020	/* Analog USB2 480MHz PLL Control Register */
#define IMXRT_CCM_ANALOG_PLL_SYS_OFFSET          0x0030	/* Analog System PLL Control Register */
#define IMXRT_CCM_ANALOG_PLL_SYS_SS_OFFSET       0x0040	/* 528MHz System PLL Spread Spectrum Register */
#define IMXRT_CCM_ANALOG_PLL_SYS_NUM_OFFSET      0x0050	/* Numerator of 528MHz System PLL Fractional Loop Divider */
#define IMXRT_CCM_ANALOG_PLL_SYS_DENOM_OFFSET    0x0060	/* Denominator of 528MHz System PLL Fractional Loop Divider */
#define IMXRT_CCM_ANALOG_PLL_AUDIO_OFFSET        0x0070	/* Analog Audio PLL control Register */
#define IMXRT_CCM_ANALOG_PLL_AUDIO_NUM_OFFSET    0x0080	/* Numerator of Audio PLL Fractional Loop Divider */
#define IMXRT_CCM_ANALOG_PLL_AUDIO_DENOM_OFFSET  0x0090	/* Denominator of Audio PLL Fractional Loop Divider */
#define IMXRT_CCM_ANALOG_PLL_VIDEO_OFFSET        0x00a0	/* Analog Video PLL control Register */
#define IMXRT_CCM_ANALOG_PLL_VIDEO_NUM_OFFSET    0x00b0	/* Numerator of Video PLL Fractional Loop Divider */
#define IMXRT_CCM_ANALOG_PLL_VIDEO_DENOM_OFFSET  0x00c0	/* Denominator of Video PLL Fractional Loop Divider */
#define IMXRT_CCM_ANALOG_PLL_ENET_OFFSET         0x00e0	/* Analog ENET PLL Control Register */
#define IMXRT_CCM_ANALOG_PFD_480_OFFSET          0x00f0	/* 480MHz Clock (PLL3) Phase Fractional Divider Control */
#define IMXRT_CCM_ANALOG_PFD_528_OFFSET          0x0100	/* 528MHz Clock (PLL2) Phase Fractional Divider Control */
#define IMXRT_CCM_ANALOG_MISC0_OFFSET            0x0150	/* Miscellaneous Register 0 */
#define IMXRT_CCM_ANALOG_MISC1_OFFSET            0x0160	/* Miscellaneous Register 1 */
#define IMXRT_CCM_ANALOG_MISC2_OFFSET            0x0170	/* Miscellaneous Register 2 */

/* Register addresses ***************************************************************************************/

#define IMXRT_CCM_CCR                            (IMXRT_CCM_BASE + IMXRT_CCM_CCR_OFFSET)
#define IMXRT_CCM_CSR                            (IMXRT_CCM_BASE + IMXRT_CCM_CSR_OFFSET)
#define IMXRT_CCM_CCSR                           (IMXRT_CCM_BASE + IMXRT_CCM_CCSR_OFFSET)
#define IMXRT_CCM_CACRR                          (IMXRT_CCM_BASE + IMXRT_CCM_CACRR_OFFSET)
#define IMXRT_CCM_CBCDR                          (IMXRT_CCM_BASE + IMXRT_CCM_CBCDR_OFFSET)
#define IMXRT_CCM_CBCMR                          (IMXRT_CCM_BASE + IMXRT_CCM_CBCMR_OFFSET)
#define IMXRT_CCM_CSCMR1                         (IMXRT_CCM_BASE + IMXRT_CCM_CSCMR1_OFFSET)
#define IMXRT_CCM_CSCMR2                         (IMXRT_CCM_BASE + IMXRT_CCM_CSCMR2_OFFSET)
#define IMXRT_CCM_CSCDR1                         (IMXRT_CCM_BASE + IMXRT_CCM_CSCDR1_OFFSET)
#define IMXRT_CCM_CS1CDR                         (IMXRT_CCM_BASE + IMXRT_CCM_CS1CDR_OFFSET)
#define IMXRT_CCM_CS2CDR                         (IMXRT_CCM_BASE + IMXRT_CCM_CS2CDR_OFFSET)
#define IMXRT_CCM_CDCDR                          (IMXRT_CCM_BASE + IMXRT_CCM_CDCDR_OFFSET)
#define IMXRT_CCM_CSCDR2                         (IMXRT_CCM_BASE + IMXRT_CCM_CSCDR2_OFFSET)
#define IMXRT_CCM_CSCDR3                         (IMXRT_CCM_BASE + IMXRT_CCM_CSCDR3_OFFSET)
#define IMXRT_CCM_CDHIPR                         (IMXRT_CCM_BASE + IMXRT_CCM_CDHIPR_OFFSET)
#define IMXRT_CCM_CLPCR                          (IMXRT_CCM_BASE + IMXRT_CCM_CLPCR_OFFSET)
#define IMXRT_CCM_CISR                           (IMXRT_CCM_BASE + IMXRT_CCM_CISR_OFFSET)
#define IMXRT_CCM_CIMR                           (IMXRT_CCM_BASE + IMXRT_CCM_CIMR_OFFSET)
#define IMXRT_CCM_CCOSR                          (IMXRT_CCM_BASE + IMXRT_CCM_CCOSR_OFFSET)
#define IMXRT_CCM_CGPR                           (IMXRT_CCM_BASE + IMXRT_CCM_CGPR_OFFSET)
#define IMXRT_CCM_CCGR0                          (IMXRT_CCM_BASE + IMXRT_CCM_CCGR0_OFFSET)
#define IMXRT_CCM_CCGR1                          (IMXRT_CCM_BASE + IMXRT_CCM_CCGR1_OFFSET)
#define IMXRT_CCM_CCGR2                          (IMXRT_CCM_BASE + IMXRT_CCM_CCGR2_OFFSET)
#define IMXRT_CCM_CCGR3                          (IMXRT_CCM_BASE + IMXRT_CCM_CCGR3_OFFSET)
#define IMXRT_CCM_CCGR4                          (IMXRT_CCM_BASE + IMXRT_CCM_CCGR4_OFFSET)
#define IMXRT_CCM_CCGR5                          (IMXRT_CCM_BASE + IMXRT_CCM_CCGR5_OFFSET)
#define IMXRT_CCM_CCGR6                          (IMXRT_CCM_BASE + IMXRT_CCM_CCGR6_OFFSET)
#define IMXRT_CCM_CMEOR                          (IMXRT_CCM_BASE + IMXRT_CCM_CMEOR_OFFSET)

#define IMXRT_CCM_ANALOG_PLL_ARM                 (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_ARM_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_USB1                (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_USB1_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_USB2                (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_USB2_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_SYS                 (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_SYS_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_SYS_SS              (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_SYS_SS_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_SYS_NUM             (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_SYS_NUM_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_SYS_DENOM           (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_SYS_DENOM_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_AUDIO               (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_AUDIO_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_AUDIO_NUM           (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_AUDIO_NUM_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_AUDIO_DENOM         (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_AUDIO_DENOM_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_VIDEO               (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_VIDEO_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_VIDEO_NUM           (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_VIDEO_NUM_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_VIDEO_DENOM         (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_VIDEO_DENOM_OFFSET)
#define IMXRT_CCM_ANALOG_PLL_ENET                (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PLL_ENET_OFFSET)
#define IMXRT_CCM_ANALOG_PFD_480                 (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PFD_480_OFFSET)
#define IMXRT_CCM_ANALOG_PFD_528                 (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_PFD_528_OFFSET)
#define IMXRT_CCM_ANALOG_MISC0                   (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_MISC0_OFFSET)
#define IMXRT_CCM_ANALOG_MISC1                   (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_MISC1_OFFSET)
#define IMXRT_CCM_ANALOG_MISC2                   (IMXRT_ANATOP_BASE + IMXRT_CCM_ANALOG_MISC2_OFFSET)

/* Register bit definitions *********************************************************************************/

#define CCM_PODF_FROM_DIVISOR(n)                   ((n)-1)	/* PODF Values are divisor-1 */

/* Clock Gating Register 0-6 */

#define CCM_CG_OFF                               (0)	/* Clock is off during all modes */
#define CCM_CG_RUN                               (1)	/* Clock is on in run mode, but off in WAIT and STOP modes */
#define CCM_CG_ALL                               (3)	/* Clock is on during all modes, except STOP mode. */

#define CCM_CCGRX_CG_SHIFT(r)                    ((r) << 1)
#define CCM_CCGRX_CG_MASK(r)                     (0x3 << CCM_CCGRX_CG_SHIFT(r))
#define CCM_CCGRX_CG(r,v)                      ((uint32_t)(v) << CCM_CCGRX_CG_SHIFT(r))

#define CCM_CCGRX_CG0_SHIFT                      (0)
#define CCM_CCGRX_CG0_MASK                       (0x3 << CCM_CCGRX_CG0_SHIFT)
#define CCM_CCGRX_CG0(n)                       ((uint32_t)(n) << CCM_CCGRX_CG0_SHIFT)
#define CCM_CCGRX_CG1_SHIFT                      (2)
#define CCM_CCGRX_CG1_MASK                       (0x3 << CCM_CCGRX_CG1_SHIFT)
#define CCM_CCGRX_CG1(n)                       ((uint32_t)(n) << CCM_CCGRX_CG1_SHIFT)
#define CCM_CCGRX_CG2_SHIFT                      (4)
#define CCM_CCGRX_CG2_MASK                       (0x3 << CCM_CCGRX_CG2_SHIFT)
#define CCM_CCGRX_CG2(n)                       ((uint32_t)(n) << CCM_CCGRX_CG2_SHIFT)
#define CCM_CCGRX_CG3_SHIFT                      (6)
#define CCM_CCGRX_CG3_MASK                       (0x3 << CCM_CCGRX_CG3_SHIFT)
#define CCM_CCGRX_CG3(n)                       ((uint32_t)(n) << CCM_CCGRX_CG3_SHIFT)
#define CCM_CCGRX_CG4_SHIFT                      (8)
#define CCM_CCGRX_CG4_MASK                       (0x3 << CCM_CCGRX_CG4_SHIFT)
#define CCM_CCGRX_CG4(n)                       ((uint32_t)(n) << CCM_CCGRX_CG4_SHIFT)
#define CCM_CCGRX_CG5_SHIFT                      (10)
#define CCM_CCGRX_CG5_MASK                       (0x3 << CCM_CCGRX_CG5_SHIFT)
#define CCM_CCGRX_CG5(n)                       ((uint32_t)(n) << CCM_CCGRX_CG5_SHIFT)
#define CCM_CCGRX_CG6_SHIFT                      (12)
#define CCM_CCGRX_CG6_MASK                       (0x3 << CCM_CCGRX_CG6_SHIFT)
#define CCM_CCGRX_CG6(n)                       ((uint32_t)(n) << CCM_CCGRX_CG6_SHIFT)
#define CCM_CCGRX_CG7_SHIFT                      (14)
#define CCM_CCGRX_CG7_MASK                       (0x3 << CCM_CCGRX_CG7_SHIFT)
#define CCM_CCGRX_CG7(n)                       ((uint32_t)(n) << CCM_CCGRX_CG7_SHIFT)
#define CCM_CCGRX_CG8_SHIFT                      (16)
#define CCM_CCGRX_CG8_MASK                       (0x3 << CCM_CCGRX_CG8_SHIFT)
#define CCM_CCGRX_CG8(n)                       ((uint32_t)(n) << CCM_CCGRX_CG8_SHIFT)
#define CCM_CCGRX_CG9_SHIFT                      (18)
#define CCM_CCGRX_CG9_MASK                       (0x3 << CCM_CCGRX_CG9_SHIFT)
#define CCM_CCGRX_CG9(n)                       ((uint32_t)(n) << CCM_CCGRX_CG9_SHIFT)
#define CCM_CCGRX_CG10_SHIFT                     (20)
#define CCM_CCGRX_CG10_MASK                      (0x3 << CCM_CCGRX_CG10_SHIFT)
#define CCM_CCGRX_CG10(n)                      ((uint32_t)(n) << CCM_CCGRX_CG10_SHIFT)
#define CCM_CCGRX_CG11_SHIFT                     (22)
#define CCM_CCGRX_CG11_MASK                      (0x3 << CCM_CCGRX_CG11_SHIFT)
#define CCM_CCGRX_CG11(n)                      ((uint32_t)(n) << CCM_CCGRX_CG11_SHIFT)
#define CCM_CCGRX_CG12_SHIFT                     (24)
#define CCM_CCGRX_CG12_MASK                      (0x3 << CCM_CCGRX_CG12_SHIFT)
#define CCM_CCGRX_CG12(n)                      ((uint32_t)(n) << CCM_CCGRX_CG12_SHIFT)
#define CCM_CCGRX_CG13_SHIFT                     (26)
#define CCM_CCGRX_CG13_MASK                      (0x3 << CCM_CCGRX_CG13_SHIFT)
#define CCM_CCGRX_CG13(n)                      ((uint32_t)(n) << CCM_CCGRX_CG13_SHIFT)
#define CCM_CCGRX_CG14_SHIFT                     (28)
#define CCM_CCGRX_CG14_MASK                      (0x3 << CCM_CCGRX_CG14_SHIFT)
#define CCM_CCGRX_CG14(n)                      ((uint32_t)(n) << CCM_CCGRX_CG14_SHIFT)
#define CCM_CCGRX_CG15_SHIFT                     (30)
#define CCM_CCGRX_CG15_MASK                      (0x3 << CCM_CCGRX_CG15_SHIFT)
#define CCM_CCGRX_CG15(n)                      ((uint32_t)(n) << CCM_CCGRX_CG15_SHIFT)

/* Macros used by imxrt_periphclks.h */

#define CCM_CCGR_GPIO2                           IMXRT_CCM_CCGR0, 15
#define CCM_CCGR_LPUART2                         IMXRT_CCM_CCGR0, 14
#define CCM_CCGR_GPT2_SERIAL                     IMXRT_CCM_CCGR0, 13
#define CCM_CCGR_GPT2_BUS                        IMXRT_CCM_CCGR0, 12
#define CCM_CCGR_TRACE                           IMXRT_CCM_CCGR0, 11
#define CCM_CCGR_CAN2_SERIAL                     IMXRT_CCM_CCGR0, 10
#define CCM_CCGR_CAN2                            IMXRT_CCM_CCGR0, 9
#define CCM_CCGR_CAN1_SERIAL                     IMXRT_CCM_CCGR0, 8
#define CCM_CCGR_CAN1                            IMXRT_CCM_CCGR0, 7
#define CCM_CCGR_LPUART3                         IMXRT_CCM_CCGR0, 6
#define CCM_CCGR_DCP                             IMXRT_CCM_CCGR0, 5
#define CCM_CCGR_MQS                             IMXRT_CCM_CCGR0, 2
#define CCM_CCGR_AIPS_TZ2                        IMXRT_CCM_CCGR0, 1
#define CCM_CCGR_AIPS_TZ1                        IMXRT_CCM_CCGR0, 0

#define CCM_CCGR_CSU                             IMXRT_CCM_CCGR1, 14
#define CCM_CCGR_GPIO1                           IMXRT_CCM_CCGR1, 13
#define CCM_CCGR_LPUART4                         IMXRT_CCM_CCGR1, 12
#define CCM_CCGR_GPT_SERIAL                      IMXRT_CCM_CCGR1, 11
#define CCM_CCGR_GPT_BUS                         IMXRT_CCM_CCGR1, 10
#define CCM_CCGR_ADC1                            IMXRT_CCM_CCGR1, 8
#define CCM_CCGR_AOI2                            IMXRT_CCM_CCGR1, 7
#define CCM_CCGR_PIT                             IMXRT_CCM_CCGR1, 6
#define CCM_CCGR_ENET                            IMXRT_CCM_CCGR1, 5
#define CCM_CCGR_ADC2                            IMXRT_CCM_CCGR1, 4
#define CCM_CCGR_LPSPI4                          IMXRT_CCM_CCGR1, 3
#define CCM_CCGR_LPSPI3                          IMXRT_CCM_CCGR1, 2
#define CCM_CCGR_LPSPI2                          IMXRT_CCM_CCGR1, 1
#define CCM_CCGR_LPSPI1                          IMXRT_CCM_CCGR1, 0

#define CCM_CCGR_PXP                             IMXRT_CCM_CCGR2, 15
#define CCM_CCGR_LCD                             IMXRT_CCM_CCGR2, 14
#define CCM_CCGR_GPIO3                           IMXRT_CCM_CCGR2, 13
#define CCM_CCGR_XBAR2                           IMXRT_CCM_CCGR2, 12
#define CCM_CCGR_XBAR1                           IMXRT_CCM_CCGR2, 11
#define CCM_CCGR_IPMUX3                          IMXRT_CCM_CCGR2, 10
#define CCM_CCGR_IPMUX2                          IMXRT_CCM_CCGR2, 9
#define CCM_CCGR_IPMUX1                          IMXRT_CCM_CCGR2, 8
#define CCM_CCGR_XBAR3                           IMXRT_CCM_CCGR2, 7
#define CCM_CCGR_OCOTP_CTRL                      IMXRT_CCM_CCGR2, 6
#define CCM_CCGR_LPI2C3                          IMXRT_CCM_CCGR2, 5
#define CCM_CCGR_LPI2C2                          IMXRT_CCM_CCGR2, 4
#define CCM_CCGR_LPI2C1                          IMXRT_CCM_CCGR2, 3
#define CCM_CCGR_IOMUXC_SNVS                     IMXRT_CCM_CCGR2, 2
#define CCM_CCGR_CSI                             IMXRT_CCM_CCGR2, 1

#define CCM_CCGR_IOMUXC_SNVS_GPR                 IMXRT_CCM_CCGR3, 15
#define CCM_CCGR_OCRAM                           IMXRT_CCM_CCGR3, 14
#define CCM_CCGR_ACMP4                           IMXRT_CCM_CCGR3, 13
#define CCM_CCGR_ACMP3                           IMXRT_CCM_CCGR3, 12
#define CCM_CCGR_ACMP2                           IMXRT_CCM_CCGR3, 11
#define CCM_CCGR_ACMP1                           IMXRT_CCM_CCGR3, 10
#define CCM_CCGR_FLEXRAM                         IMXRT_CCM_CCGR3, 9
#define CCM_CCGR_WDOG1                           IMXRT_CCM_CCGR3, 8
#define CCM_CCGR_EWM                             IMXRT_CCM_CCGR3, 7
#define CCM_CCGR_GPIO4                           IMXRT_CCM_CCGR3, 6
#define CCM_CCGR_LCDIF_PIX                       IMXRT_CCM_CCGR3, 5
#define CCM_CCGR_AOI1                            IMXRT_CCM_CCGR3, 4
#define CCM_CCGR_LPUART6                         IMXRT_CCM_CCGR3, 3
#define CCM_CCGR_SEMC                            IMXRT_CCM_CCGR3, 2
#define CCM_CCGR_LPUART5                         IMXRT_CCM_CCGR3, 1
#define CCM_CCGR_FLEXIO2                         IMXRT_CCM_CCGR3, 0

#define CCM_CCGR_ENC4                            IMXRT_CCM_CCGR4, 15
#define CCM_CCGR_ENC3                            IMXRT_CCM_CCGR4, 14
#define CCM_CCGR_ENC2                            IMXRT_CCM_CCGR4, 13
#define CCM_CCGR_ENC1                            IMXRT_CCM_CCGR4, 12
#define CCM_CCGR_PWM4                            IMXRT_CCM_CCGR4, 11
#define CCM_CCGR_PWM3                            IMXRT_CCM_CCGR4, 10
#define CCM_CCGR_PWM2                            IMXRT_CCM_CCGR4, 9
#define CCM_CCGR_PWM1                            IMXRT_CCM_CCGR4, 8
#define CCM_CCGR_SIM_EMS                         IMXRT_CCM_CCGR4, 7
#define CCM_CCGR_SIM_M                           IMXRT_CCM_CCGR4, 6
#define CCM_CCGR_TSC_DIG                         IMXRT_CCM_CCGR4, 5
#define CCM_CCGR_SIM_M7                          IMXRT_CCM_CCGR4, 4
#define CCM_CCGR_BEE                             IMXRT_CCM_CCGR4, 3
#define CCM_CCGR_IOMUXC_GPR                      IMXRT_CCM_CCGR4, 2
#define CCM_CCGR_IOMUXC                          IMXRT_CCM_CCGR4, 1

#define CCM_CCGR_SNVS_LP                         IMXRT_CCM_CCGR5, 15
#define CCM_CCGR_SNVS_HP                         IMXRT_CCM_CCGR5, 14
#define CCM_CCGR_LPUART7                         IMXRT_CCM_CCGR5, 13
#define CCM_CCGR_LPUART1                         IMXRT_CCM_CCGR5, 12
#define CCM_CCGR_SAI3                            IMXRT_CCM_CCGR5, 11
#define CCM_CCGR_SAI2                            IMXRT_CCM_CCGR5, 10
#define CCM_CCGR_SAI1                            IMXRT_CCM_CCGR5, 9
#define CCM_CCGR_SIM_MAIN                        IMXRT_CCM_CCGR5, 8
#define CCM_CCGR_SPDIF                           IMXRT_CCM_CCGR5, 7
#define CCM_CCGR_AIPSTZ4                         IMXRT_CCM_CCGR5, 6
#define CCM_CCGR_WDOG2                           IMXRT_CCM_CCGR5, 5
#define CCM_CCGR_KPP                             IMXRT_CCM_CCGR5, 4
#define CCM_CCGR_DMA                             IMXRT_CCM_CCGR5, 3
#define CCM_CCGR_WDOG3                           IMXRT_CCM_CCGR5, 2
#define CCM_CCGR_FLEXIO1                         IMXRT_CCM_CCGR5, 1
#define CCM_CCGR_ROM                             IMXRT_CCM_CCGR5, 0

#define CCM_CCGR_TIMER3                          IMXRT_CCM_CCGR6, 15
#define CCM_CCGR_TIMER2                          IMXRT_CCM_CCGR6, 14
#define CCM_CCGR_TIMER1                          IMXRT_CCM_CCGR6, 13
#define CCM_CCGR_LPI2C4_SERIAL                   IMXRT_CCM_CCGR6, 12
#define CCM_CCGR_ANADIG                          IMXRT_CCM_CCGR6, 11
#define CCM_CCGR_SIM_PER                         IMXRT_CCM_CCGR6, 10
#define CCM_CCGR_AIPS_TZ3                        IMXRT_CCM_CCGR6, 9
#define CCM_CCGR_TIMER4                          IMXRT_CCM_CCGR6, 8
#define CCM_CCGR_LPUART8                         IMXRT_CCM_CCGR6, 7
#define CCM_CCGR_TRNG                            IMXRT_CCM_CCGR6, 6
#define CCM_CCGR_FLEXSPI                         IMXRT_CCM_CCGR6, 5
#define CCM_CCGR_IPMUX4                          IMXRT_CCM_CCGR6, 4
#define CCM_CCGR_DCDC                            IMXRT_CCM_CCGR6, 3
#define CCM_CCGR_USDHC2                          IMXRT_CCM_CCGR6, 2
#define CCM_CCGR_USDHC1                          IMXRT_CCM_CCGR6, 1
#define CCM_CCGR_USBOH3                          IMXRT_CCM_CCGR6, 0

/* Analog ENET PLL Control Register */

#define CCM_ANALOG_PLL_ENET_ENET0_DIV_SELECT_SHIFT      (0)	/* Bits 0-1:    Controls the frequency of the ethernet0 reference clock */
#define CCM_ANALOG_PLL_ENET_ENET0_DIV_SELECT_MASK       (0x3 << CCM_ANALOG_PLL_ENET_ENET0_DIV_SELECT_SHIFT)
#define CCM_ANALOG_PLL_ENET_ENET0_DIV_SELECT_25MHZ    ((uint32_t)(0) << CCM_ANALOG_PLL_ENET_ENET0_DIV_SELECT_SHIFT)
#define CCM_ANALOG_PLL_ENET_ENET0_DIV_SELECT_50MHZ    ((uint32_t)(1) << CCM_ANALOG_PLL_ENET_ENET0_DIV_SELECT_SHIFT)
#define CCM_ANALOG_PLL_ENET_ENET0_DIV_SELECT_100MHZ   ((uint32_t)(2) << CCM_ANALOG_PLL_ENET_ENET0_DIV_SELECT_SHIFT)
#define CCM_ANALOG_PLL_ENET_ENET0_DIV_SELECT_125MHZ   ((uint32_t)(3) << CCM_ANALOG_PLL_ENET_ENET0_DIV_SELECT_SHIFT)
#define CCM_ANALOG_PLL_ENET_ENET1_DIV_SELECT_SHIFT      (2)	/* Bits 0-1:    Controls the frequency of the ethernet1 reference clock */
#define CCM_ANALOG_PLL_ENET_ENET1_DIV_SELECT_MASK       (0x3 << CCM_ANALOG_PLL_ENET_ENET1_DIV_SELECT_SHIFT)
#define CCM_ANALOG_PLL_ENET_ENET1_DIV_SELECT_25MHZ    ((uint32_t)(0) << CCM_ANALOG_PLL_ENET_ENET1_DIV_SELECT_SHIFT)
#define CCM_ANALOG_PLL_ENET_ENET1_DIV_SELECT_50MHZ    ((uint32_t)(1) << CCM_ANALOG_PLL_ENET_ENET1_DIV_SELECT_SHIFT)
#define CCM_ANALOG_PLL_ENET_ENET1_DIV_SELECT_100MHZ   ((uint32_t)(2) << CCM_ANALOG_PLL_ENET_ENET1_DIV_SELECT_SHIFT)
#define CCM_ANALOG_PLL_ENET_ENET1_DIV_SELECT_125MHZ   ((uint32_t)(3) << CCM_ANALOG_PLL_ENET_ENET1_DIV_SELECT_SHIFT)

#define CCM_ANALOG_PLL_ENET_BYPASS_CLK_SRC_REF_24M   ((uint32_t)(0) << CCM_ANALOG_PLL_ENET_BYPASS_CLK_SRC_SHIFT)	/* Select 24Mhz Osc as source */
#define CCM_ANALOG_PLL_ENET_BYPASS_CLK_SRC_CLK1      ((uint32_t)(1) << CCM_ANALOG_PLL_ENET_BYPASS_CLK_SRC_SHIFT)	/* Select the CLK1_N / CLK1_P as source */

#define CCM_ANALOG_PLL_ENET_ENABLE_125M                (1 << 19)	/* Bit 19:     */
#define CCM_ANALOG_PLL_ENET_ENET1_125M_EN               (1 << 13)	/* Bit 13:     Enable the PLL providing the ENET1 125 MHz reference clock */
#define CCM_ANALOG_PLL_ENET_ENET2_125M_EN              (1 << 20)	/* Bit 20:     Enable the PLL providing the ENET2 125 MHz reference clock */

#endif							/* __ARCH_ARM_SRC_IMXRT_CHIP_IMXRT_CCM_H */
