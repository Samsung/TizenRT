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
 * os/arch/arm/src/imrt/imxrt_clockconfig.h
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author:  Gregory Nutt <gnutt@nuttx.org>
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

#ifndef __ARCH_ARM_SRC_IMXRT_IMX_CLOCKCONFIG_H
#define __ARCH_ARM_SRC_IMXRT_IMX_CLOCKCONFIG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CCM_CBCDR_PERIPH_CLK_SEL_PRE_PERIPH    (0)
#define CCM_CBCDR_PERIPH_CLK_SEL_PERIPH_CLK2   (1)
/* Bits 0-3:   Reserved */
#define CCM_CBCMR_LPSPI_CLK_SEL_PLL3_PFD1      ((uint32_t)(0) << CCM_CBCMR_LPSPI_CLK_SEL_SHIFT)
#define CCM_CBCMR_LPSPI_CLK_SEL_PLL3_PFD0      ((uint32_t)(1) << CCM_CBCMR_LPSPI_CLK_SEL_SHIFT)
#define CCM_CBCMR_LPSPI_CLK_SEL_PLL2           ((uint32_t)(2) << CCM_CBCMR_LPSPI_CLK_SEL_SHIFT)
#define CCM_CBCMR_LPSPI_CLK_SEL_PLL2_PFD2      ((uint32_t)(3) << CCM_CBCMR_LPSPI_CLK_SEL_SHIFT)
/* Bits 6-11:  Reserved */
#define CCM_CBCMR_PERIPH_CLK2_SEL_PLL3_SW      ((uint32_t)(0) << CCM_CBCMR_PERIPH_CLK2_SEL_SHIFT)
#define CCM_CBCMR_PERIPH_CLK2_SEL_OSC_CLK      ((uint32_t)(1) << CCM_CBCMR_PERIPH_CLK2_SEL_SHIFT)
#define CCM_CBCMR_PERIPH_CLK2_SEL_PLL2_BP      ((uint32_t)(2) << CCM_CBCMR_PERIPH_CLK2_SEL_SHIFT)
#define CCM_CBCMR_TRACE_CLK_SEL_PLL2           ((uint32_t)(0) << CCM_CBCMR_TRACE_CLK_SEL_SHIFT)
#define CCM_CBCMR_TRACE_CLK_SEL_PLL2_PFD2      ((uint32_t)(1) << CCM_CBCMR_TRACE_CLK_SEL_SHIFT)
#define CCM_CBCMR_TRACE_CLK_SEL_PLL2_PFD0      ((uint32_t)(2) << CCM_CBCMR_TRACE_CLK_SEL_SHIFT)
#define CCM_CBCMR_TRACE_CLK_SEL_PLL2_PFD1      ((uint32_t)(3) << CCM_CBCMR_TRACE_CLK_SEL_SHIFT)
/* Bits 16-17: Reserved */
#define CCM_CBCMR_PRE_PERIPH_CLK_SEL_PLL2      (0)
#define CCM_CBCMR_PRE_PERIPH_CLK_SEL_PLL2_PFD2 (1)
#define CCM_CBCMR_PRE_PERIPH_CLK_SEL_PLL2_PFD0 (2)
#define CCM_CBCMR_PRE_PERIPH_CLK_SEL_PLL1      (3)

#define CCM_ANALOG_PLL_SYS_DIV_SELECT_20              (0)
#define CCM_ANALOG_PLL_SYS_DIV_SELECT_22              (1)

#define CCM_CSCMR1_PERCLK_CLK_SEL_IPG_CLK_ROOT (0)
#define CCM_CSCMR1_PERCLK_CLK_SEL_OSC_CLK      (1)

#define CCM_CSCDR1_UART_CLK_SEL_PLL3_80        (0 << 6)	/* derive clock from pll3_80m */
#define CCM_CSCDR1_UART_CLK_SEL_OSC_CLK        (1 << 6)	/* derive clock from osc_clk */

#define CCM_CSCDR2_LPI2C_CLK_SEL_PLL3_60M      (0 << 18)	/*  derive clock from pll3_60m */
#define CCM_CSCDR2_LPI2C_CLK_SEL_OSC_CLK       (1 << 18)	/*  derive clock from ock_clk */

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: imxrt_clockconfig
 *
 * Description:
 *   Called to initialize the i.MXRT.  This does whatever setup is needed to
 *   put the SoC in a usable state.  This includes the initialization of
 *   clocking using the settings in board.h.
 *
 ****************************************************************************/

void imxrt_clockconfig(void);

#endif							/* __ARCH_ARM_SRC_IMXRT_IMX_CLOCKCONFIG_H */
