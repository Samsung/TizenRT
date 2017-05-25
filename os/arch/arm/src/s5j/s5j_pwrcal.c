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
 * arch/arm/src/s5j/s5j_pwrcal.c
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
#include <sys/types.h>
#include <string.h>

#include <tinyara/kmalloc.h>
#include <arch/chip/irq.h>
#include <chip.h>

#include "s5j_cmu.h"
#include "s5j_vclk.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/
unsigned int cal_clk_is_enabled(unsigned int id)
{
	return 0;
}

/*
 *  This definition of SPI_CLK should be replaced by function
 *  reruning actual CLK frequency
 */
#define SPI_CLK 40000000

/* This definitions should be moved into the right place */
#define S5J_CON_DIV_CLK_SPI0 (S5J_CMU_BASE + 0x1804)
#define S5J_CON_DIV_CLK_SPI1 (S5J_CMU_BASE + 0x1808)
#define S5J_CON_DIV_CLK_SPI2 (S5J_CMU_BASE + 0x180C)
#define S5J_CON_DIV_CLK_SPI3 (S5J_CMU_BASE + 0x1810)

int cal_clk_setrate(unsigned int id, unsigned long rate)
{
	unsigned long parents;
	unsigned int div;

	switch (id) {
	case d1_spi0:
		/* CLK_CON_DIV_DIV_CLK_SPI */
		parents = SPI_CLK;
		div = parents / rate;
		if (div == 0) {
			div = 1;
		}
		modifyreg32(S5J_CON_DIV_CLK_SPI0, 0x7ff, (div - 1));
		break;
	case d1_spi1:
		/* CLK_CON_DIV_DIV_CLK_SPI1 */
		parents = SPI_CLK;
		div = parents / rate;
		if (div == 0) {
			div = 1;
		}
		modifyreg32(S5J_CON_DIV_CLK_SPI1, 0x7FF, (div - 1));
		break;
	case d1_spi2:
		/* CLK_CON_DIV_DIV_CLK_SPI2 */
		parents = SPI_CLK;
		div = parents / rate;
		if (div == 0) {
			div = 1;
		}
		modifyreg32(S5J_CON_DIV_CLK_SPI2, 0x7ff, (div - 1));
		break;
	case d1_spi3:
		/* CLK_CON_DIV_DIV_CLK_SPI3 */
		parents = SPI_CLK;
		div = parents / rate;
		if (div == 0) {
			div = 1;
		}
		modifyreg32(S5J_CON_DIV_CLK_SPI3, 0x7ff, (div - 1));
		break;
	case gate_hsi2c0:
	case gate_hsi2c1:
	case gate_hsi2c2:
	case gate_hsi2c3:
		break;
	case d1_serialflash:
		/* CLK_CON_DIV_DIV_CLK_SERIALFLASH */
		parents = 320000000;
		div = parents / rate;
		modifyreg32(0x80081800, 0xf, (div - 1));
		break;
	default:
		break;
	}

	return -1;
}

unsigned long cal_clk_getrate(unsigned int id)
{
	unsigned long rate = 0;

	switch (id) {
	case d1_spi0:
		break;
	case d1_spi1:
		break;
	case d1_serialflash:
		break;
	case m1_clkcmu_uart:
		rate = S5J_DEFAULT_UART_CLOCK;
		break;
	case gate_hsi2c0:
	case gate_hsi2c1:
	case gate_hsi2c2:
	case gate_hsi2c3:
		rate = S5J_DEFAULT_I2C_CLOCK;
		break;
	default:
		break;
	}

	return rate;
}

int cal_clk_enable(unsigned int id)
{
	return 0;
}

int cal_clk_disable(unsigned int id)
{
	return 0;
}

int cal_init(void)
{
	return 0;
}
