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
 * arch/arm/src/s5j/s5j_clock.c
 *
 *   Copyright (C) 2009, 2011 Gregory Nutt. All rights reserved.
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
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include <assert.h>

#include <chip.h>

#include "s5j_clock.h"

/****************************************************************************
 * Pre-processor definitions
 ****************************************************************************/
#define FIXED_RATE_OSCCLK_MCU		26000000
#define FIXED_RATE_WPLL_CLK240M		240000000
#define FIXED_RATE_WPLL_CLK480M		480000000
#define FIXED_RATE_WPLL_CLK960M		960000000

/****************************************************************************
 * Public Functions
 ****************************************************************************/
static struct clk_div *get_clk_div(enum clk_divider_id id)
{
	struct mcu_clk *cmu = (struct mcu_clk *)S5J_CMU_BASE;

	return &cmu->dividers[id];
}

static int clk_div_get_ratio(enum clk_divider_id id)
{
	struct clk_div *clkdiv = get_clk_div(id);

	return clkdiv->divratio + 1;
}

static void clk_div_set_ratio(enum clk_divider_id id, unsigned long divratio)
{
	struct clk_div *clkdiv = get_clk_div(id);

	clkdiv->divratio = divratio - 1;
}

static struct clk_mux *get_clk_mux(enum clk_mux_id id)
{
	struct mcu_clk *cmu = (struct mcu_clk *)S5J_CMU_BASE;

	return &cmu->muxes[id];
}

static unsigned long clk_wpll_get_rate(enum clk_id id)
{
	unsigned long sclk, div;
	struct mcu_clk *cmu = (struct mcu_clk *)S5J_CMU_BASE;

	if (cmu->pll_con0.mux_sel) {
		sclk = FIXED_RATE_WPLL_CLK960M;
	} else {
		sclk = FIXED_RATE_OSCCLK_MCU;
	}

	div = 1;

	switch (id) {
	case CLK_WPLL_DIV12:
		div = clk_div_get_ratio(CLK_DIV_WPLL_DIV12);
	/* fall through */
	case CLK_WPLL_DIV6:
		div = div * clk_div_get_ratio(CLK_DIV_WPLL_DIV6);
	/* fall through */
	case CLK_WPLL_DIV3:
		div = div * clk_div_get_ratio(CLK_DIV_WPLL_DIV3);
		break;
	default:
		break;
	}

	return sclk / div;
}

static unsigned long clk_sflash_get_rate(void)
{
	unsigned long sclk, div;

	sclk = clk_wpll_get_rate(CLK_WPLL_DIV3);
	div  = clk_div_get_ratio(CLK_DIV_SFLASH);

	return sclk / div;
}

static unsigned long clk_uart_get_rate(void)
{
	struct clk_mux *mux = get_clk_mux(CLK_MUX_UART);

	if (mux->select == CLK_MUX_SELECT_OSCCLK) {
		return FIXED_RATE_OSCCLK_MCU;
	}

	return clk_wpll_get_rate(CLK_WPLL_DIV12);
}

static unsigned long clk_spi_get_rate(unsigned int id)
{
	unsigned long sclk, div;

	sclk = clk_wpll_get_rate(CLK_WPLL_DIV12);

	switch (id) {
	case CLK_SPL_SPI0:
		div = clk_div_get_ratio(CLK_DIV_SPI0);
		break;
	case CLK_SPL_SPI1:
		div = clk_div_get_ratio(CLK_DIV_SPI1);
		break;
	case CLK_SPL_SPI2:
		div = clk_div_get_ratio(CLK_DIV_SPI2);
		break;
	case CLK_SPL_SPI3:
		div = clk_div_get_ratio(CLK_DIV_SPI3);
		break;
	}

	return sclk / div;
}

static unsigned long clk_i2sb_get_rate(void)
{
	struct clk_mux *mux = get_clk_mux(CLK_MUX_I2SB);

	ASSERT(mux->select == CLK_MUX_SELECT_OSCCLK);

	return FIXED_RATE_OSCCLK_MCU;
}

unsigned long s5j_clk_get_rate(unsigned int id);

static void clk_sflash_set_rate(unsigned long rate)
{
	unsigned long div = s5j_clk_get_rate(CLK_WPLL_DIV3) / rate;

	clk_div_set_ratio(CLK_DIV_SFLASH, div);
}

static void clk_spi_set_rate(unsigned int id, unsigned long rate)
{
	unsigned long div = s5j_clk_get_rate(CLK_WPLL_DIV12) / rate;

	switch (id) {
	case CLK_SPL_SPI0:
		clk_div_set_ratio(CLK_DIV_SPI0, div);
		break;
	case CLK_SPL_SPI1:
		clk_div_set_ratio(CLK_DIV_SPI1, div);
		break;
	case CLK_SPL_SPI2:
		clk_div_set_ratio(CLK_DIV_SPI2, div);
		break;
	case CLK_SPL_SPI3:
		clk_div_set_ratio(CLK_DIV_SPI3, div);
		break;
	}
}

void s5j_clk_set_rate(unsigned int id, unsigned long rate)
{
	switch (id) {
	case CLK_SPL_SFLASH:
		clk_sflash_set_rate(rate);
		break;

	case CLK_SPL_SPI0:
	case CLK_SPL_SPI1:
	case CLK_SPL_SPI2:
	case CLK_SPL_SPI3:
		/* S5J provides an SPI with various kinds of clock sources.
		 * SPI uses the SCLK_SPI clock from an external Clock Controller Module (CMU).
		 * You can also select an SCLK_SPI from various clock sources.
		 * An SPI contains an internal 1/2 clock divider.
		 * Configure the SCLK_SPI value to the double of an SPI operating clock frequency.
		 */
		clk_spi_set_rate(id, rate * 2);
		break;

	default:
		/* Not supported */
		ASSERT(0);
		break;
	}
}

unsigned long s5j_clk_get_rate(unsigned int id)
{
	unsigned long rate = 0;

	switch (id) {
	case CLK_DFT_OSCCLK:
	case CLK_DFT_OSCCLK_SSS:
		rate = FIXED_RATE_OSCCLK_MCU;
		break;

	case CLK_WPLL_DIV3:
	case CLK_WPLL_DIV6:
	case CLK_WPLL_DIV12:
		rate = clk_wpll_get_rate(id);
		break;

	case CLK_SPL_SFLASH:
		rate = clk_sflash_get_rate();
		break;

	case CLK_SPL_UART:
		rate = clk_uart_get_rate();
		break;

	case CLK_SPL_SPI0:
	case CLK_SPL_SPI1:
	case CLK_SPL_SPI2:
	case CLK_SPL_SPI3:
		rate = clk_spi_get_rate(id);
		break;

	case CLK_SPL_I2SB:
		rate = clk_i2sb_get_rate();
		break;

	case CLK_SPL_BUS_D0:
	case CLK_SPL_BUS_D0_SSS:
		rate = clk_wpll_get_rate(CLK_WPLL_DIV3);
		break;

	case CLK_SPL_BUS_P0:
	case CLK_SPL_BUS_P0_SSS:
		rate = clk_wpll_get_rate(CLK_WPLL_DIV6);
		break;
	}

	return rate;
}

int s5j_clk_mux_select(enum clk_mux_id mux_id, enum clk_mux_source src)
{
	struct clk_mux *mux = get_clk_mux(mux_id);

	mux->select = src;

	return src;
}

void s5j_clk_enable(enum clk_gate_id id)
{
	struct mcu_clk *cmu = (struct mcu_clk *)S5J_CMU_BASE;

	cmu->gates[id].cg = 1;
	cmu->gates[id].mode = 1;
}

void s5j_clk_disable(enum clk_gate_id id)
{
	struct mcu_clk *cmu = (struct mcu_clk *)S5J_CMU_BASE;

	cmu->gates[id].cg = 0;
	cmu->gates[id].mode = 1;
}

void s5j_clk_pll_select_mux(bool clk960m)
{
	struct mcu_clk *cmu = (struct mcu_clk *)S5J_CMU_BASE;

	cmu->pll_con0.mux_sel = clk960m;
}

int s5j_clkinit(void)
{
	/* Clock gating on CM0 */
	s5j_clk_disable(CLK_GATE_CM0P);
	s5j_clk_disable(CLK_GATE_AHB2AXI_CM0P);
	s5j_clk_disable(CLK_GATE_CM0P_DCLK);
	s5j_clk_disable(CLK_GATE_CM0P_HCLK);
	s5j_clk_disable(CLK_GATE_CM0P_SCLK);

	/* actually, we never use SDIO and it is not working. */
	s5j_clk_disable(CLK_GATE_AHB2AXI_SDIO);
	s5j_clk_disable(CLK_GATE_AXI2AHB_SDIO);
	s5j_clk_disable(CLK_GATE_64TO32_SDIO);
	s5j_clk_disable(CLK_GATE_SDIO_ACLK);
	s5j_clk_disable(CLK_GATE_32TO64_SDIO);

	/* we do not use the tickcounter also. */
	s5j_clk_disable(CLK_GATE_TICKCNT_PCLK);

	return 0;
}
