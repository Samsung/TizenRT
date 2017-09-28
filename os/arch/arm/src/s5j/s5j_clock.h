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
 * arch/arm/src/s5j/s5j_clock.h
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

#ifndef _ARCH_ARM_SRC_S5J_S5J_CLOCK_H
#define _ARCH_ARM_SRC_S5J_S5J_CLOCK_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include "up_arch.h"
#include "chip/s5jt200_cmu.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* multiplexer */
struct clk_mux {
	unsigned int select:1;
	unsigned int res0:15;
	unsigned int busy:1;
	unsigned int res1:11;
	unsigned int enable_automatic_clkgating:1;
	unsigned int res2:1;
	unsigned int override_by_hch:1;
	unsigned int res3:1;
};

/* clock divider */
struct clk_div {
	unsigned int divratio:16;
	unsigned int busy:1;
	unsigned int res0:11;
	unsigned int enable_automatic_clkgating:1;
	unsigned int res1:1;
	unsigned int override_by_hch:1;
	unsigned int res2:1;
};

/* clock gating control register */
struct clk_gate {
	unsigned int res0:20;
	unsigned int mode:1;
	unsigned int cg:1;
	unsigned int res1:6;
	unsigned int enable_automatic_clkgating:1;
	unsigned int res2:3;
};

enum clk_mux_source {
	CLK_MUX_SELECT_OSCCLK		= 0,
	CLK_MUX_SELECT_BCLK			= 1,
	CLK_MUX_SELECT_WPLL_DIV12	= 1,
};

enum clk_mux_id {
	CLK_MUX_I2SB,
	CLK_MUX_RESERVED0,
	CLK_MUX_RESERVED1,
	CLK_MUX_UART,
	CLK_MUX_MAX,
};

enum clk_divider_id {
	CLK_DIV_SFLASH,
	CLK_DIV_SPI0,
	CLK_DIV_SPI1,
	CLK_DIV_SPI2,
	CLK_DIV_SPI3,
	CLK_DIV_WPLL_DIV12,
	CLK_DIV_WPLL_DIV3,
	CLK_DIV_WPLL_DIV6,
	CLK_DIV_MAX,
};

enum clk_gate_id {
	CLK_GATE_WPLL_DIV12,
	CLK_GATE_ADC,
	CLK_GATE_CM0P,
	CLK_GATE_GPIOCON,
	CLK_GATE_I2S,
	CLK_GATE_MCT0,
	CLK_GATE_CMU,
	CLK_GATE_PWM0,
	CLK_GATE_PWM1,
	CLK_GATE_RESERVED0,
	CLK_GATE_SYSREG,
	CLK_GATE_RESERVED1,
	CLK_GATE_RESERVED2,
	CLK_GATE_EFUSE,
	CLK_GATE_WDT,
	CLK_GATE_ADC_PCLK,
	CLK_GATE_AHB2AXI_CM0P,
	CLK_GATE_AHB2AXI_SDIO,
	CLK_GATE_CSSYS_PCLKM,
	CLK_GATE_CSSYS_PCLKS,
	CLK_GATE_PDMA_PCLKM,
	CLK_GATE_PDMA_PCLKS,
	CLK_GATE_AXI2AHB_FLASH,
	CLK_GATE_AXI2AHB_SDIO,
	CLK_GATE_AXI2APB_PERIC,
	CLK_GATE_AXI2APB_PERIS0,
	CLK_GATE_AXI2APB_PERIS1,
	CLK_GATE_CM0P_DCLK,
	CLK_GATE_CM0P_HCLK,
	CLK_GATE_CM0P_SCLK,
	CLK_GATE_CSSYS_PCLKDBG,
	CLK_GATE_64TO32_SDIO,
	CLK_GATE_64TO32_DP,
	CLK_GATE_64TO32_FLASH,
	CLK_GATE_GIC400_INPUT,
	CLK_GATE_GIC400,
	CLK_GATE_HSI2C0,
	CLK_GATE_HSI2C1,
	CLK_GATE_HSI2C2,
	CLK_GATE_HSI2C3,
	CLK_GATE_I2S_PCLK,
	CLK_GATE_INTMEM,
	CLK_GATE_INTMEM_SHARED,
	CLK_GATE_CR4_CLKIN,
	CLK_GATE_CR4_FCLKIN,
	CLK_GATE_CR4_PCLKDBG,
	CLK_GATE_LHM_AXI,
	CLK_GATE_LHM_DP,
	CLK_GATE_LHS_DP,
	CLK_GATE_MAILBOX_M0,
	CLK_GATE_MAILBOX_WIFI,
	CLK_GATE_MCT0_PCLK,
	CLK_GATE_MCT0_ACLK,
	CLK_GATE_PMU_PCLK,
	CLK_GATE_PMU_PCLK_CSSYS,
	CLK_GATE_PUF,
	CLK_GATE_PWM0_PCLK,
	CLK_GATE_PWM1_PCLK,
	CLK_GATE_RESERVED3,
	CLK_GATE_RESERVED4,
	CLK_GATE_RESERVED5,
	CLK_GATE_SDIO_ACLK,
	CLK_GATE_SFLASH_HCLK,
	CLK_GATE_SFLASH_SFCLK,
	CLK_GATE_GPIO_PCLK,
	CLK_GATE_SPI0_PCLK,
	CLK_GATE_SPI0_EXTCLK,
	CLK_GATE_SPI1_PCLK,
	CLK_GATE_SPI1_EXTCLK,
	CLK_GATE_SPI2_PCLK,
	CLK_GATE_SPI2_EXTCLK,
	CLK_GATE_SPI3_PCLK,
	CLK_GATE_SPI3_EXTCLK,
	CLK_GATE_WIFI_ACLK,
	CLK_GATE_SYSREG_PCLK,
	CLK_GATE_TICKCNT_PCLK,
	CLK_GATE_RTC_PCLK,
	CLK_GATE_UART0_EXTCLK,
	CLK_GATE_UART0_PCLK,
	CLK_GATE_UART1_EXTCLK,
	CLK_GATE_UART1_PCLK,
	CLK_GATE_UART2_EXTCLK,
	CLK_GATE_UART2_PCLK,
	CLK_GATE_UART3_EXTCLK,
	CLK_GATE_UART3_PCLK,
	CLK_GATE_UARTDBG_EXTCLK,
	CLK_GATE_UARTDBG_PCLK,
	CLK_GATE_EFUSE_PCLK,
	CLK_GATE_32TO64_CM0P,
	CLK_GATE_32TO64_PDMA,
	CLK_GATE_32TO64_SDIO,
	CLK_GATE_WDT_PCLK,
	CLK_GATE_RESERVED6,
	CLK_GATE_XIU,
	CLK_GATE_MAX,
};

enum clk_id {
	CLK_DFT_OSCCLK,
	CLK_DFT_OSCCLK_SSS,
	CLK_WPLL_DIV3,
	CLK_WPLL_DIV6,
	CLK_WPLL_DIV12,
	CLK_SPL_SPI0,
	CLK_SPL_SPI1,
	CLK_SPL_SPI2,
	CLK_SPL_SPI3,
	CLK_SPL_UART,
	CLK_SPL_I2SB,
	CLK_SPL_SFLASH,
	CLK_SPL_BUS_D0,
	CLK_SPL_BUS_D0_SSS,
	CLK_SPL_BUS_P0,
	CLK_SPL_BUS_P0_SSS,
};

/* Q-Channel control register */
struct clk_qch {
	unsigned int res0:6;
	unsigned int expire_val:10;
	unsigned int res1:14;
	unsigned int clock_req:1;
	unsigned int enable:1;
};

/* Clock management unit - MCU part */
struct mcu_clk {
	/* PLL control register */
	unsigned int wpll_con0;
	unsigned int wpll_con1;
	unsigned int wpll_con2;
	unsigned int wpll_con3;
	unsigned int wpll_con4;
	unsigned int wpll_con5;
	unsigned int wpll_stat;
	unsigned char res0[0x0164];

	struct {
		unsigned int res0:4;
		unsigned int mux_sel:1;
		unsigned int res1:2;
		unsigned int busy:1;
		unsigned int res2:24;
	} pll_con0;

	struct {
		unsigned int res0:5;
		unsigned int ignore_req_sysclk:1;
		unsigned int res1:26;
	} pll_con1;

	struct {
		unsigned int res0:28;
		unsigned int enable_automatic_clkgating:1;
		unsigned int res1:1;
		unsigned int override_by_hch:1;
		unsigned int res2:1;
	} pll_con2;

	unsigned char res1[0x674];

	/* Miscellaneous functions */
	unsigned int cmu_con;
	unsigned char res2[0xc];
	unsigned int clkout_con;
	unsigned char res3[0x1ec];
	unsigned int spare0;
	unsigned int spare1;
	unsigned char res4[0x5f8];

	/* MUX controls such as MUX selection, enable and status */
	struct clk_mux muxes[CLK_MUX_MAX];
	unsigned char res5[0x7f0];

	/* Clock divider controls such as divider ratio and status */
	struct clk_div dividers[CLK_DIV_MAX];
	unsigned char res6[0x7e0];

	/* Clock gating controls such as clock gating of IPs and function blocks */
	struct clk_gate gates[CLK_GATE_MAX];
	unsigned char res7[0xe9c];

	/* Q-Channel controls such as Q-channel enable and counter */
	unsigned int qch_con_cr4;
	unsigned char res8[0x8];
	unsigned int qch_con_t20;
	unsigned int qch_con_wifi;
	unsigned int qch_con_lhm_dp;
	unsigned int qch_con_lhs_dp;
	unsigned int qch_con_cmu;
	unsigned char res9[0xbcc];

	/* Q-state controls */
	unsigned int queue_ctrl;
};
#endif /* _ARCH_ARM_SRC_S5J_S5J_CLOCK_H */
