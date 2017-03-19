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
 * arch/arm/src/s5j/s5j_qspi.c
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
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/arch.h>

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "chip.h"
#include "up_arch.h"

/****************************************************************************
 * Private Functions Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Static Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void s5j_qspi_set_gpio(void)
{
	int gpio_sf_clk;
	int gpio_sf_cs, gpio_sf_si, gpio_sf_so, gpio_sf_wp, gpio_sf_hld;

	gpio_sf_clk = s5j_gpio(GPP1, 0);
	gpio_sf_cs  = s5j_gpio(GPP1, 1);
	gpio_sf_si  = s5j_gpio(GPP1, 2);
	gpio_sf_so  = s5j_gpio(GPP1, 3);
	gpio_sf_wp  = s5j_gpio(GPP1, 4);
	gpio_sf_hld = s5j_gpio(GPP1, 5);

	gpio_cfg_pin(gpio_sf_clk, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_cs, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_si, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_so, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_wp, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_hld, GPIO_FUNC(2));

	gpio_set_pull(gpio_sf_clk, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_cs, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_si, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_so, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_wp, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_hld, GPIO_PULL_UP);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/**
 * @brief	disable write protection of FLASH
 * @param	void
 * @return	void
 * @note	writable when wp is disabled
*/
void s5j_qspi_disable_wp(void)
{
	unsigned int reg;

	/* someone has been disabled wp, we should wait until it's released */
	do {
		reg = (HW_REG32(0x80310000, 0x04) & ~(0x1 << 31)) >> 31;
	} while (reg);

	HW_REG32(0x80310000, 0x04) &= ~(0x1 << 31);
	HW_REG32(0x80310000, 0x04) |= (0x1 << 31);
}

/**
 * @brief	enable write protection of FLASH
 * @param	void
 * @return	void
 * @note
 */
void s5j_qspi_enable_wp(void)
{
	HW_REG32(0x80310000, 0x04) &= ~(0x1 << 31);
}

/**
 * @brief	initialize FLASH for QUAD IO in 80Mhz
 * @param	void
 * @return	void
 * @note
 */
void s5j_qspi_init(void)
{
	/* Set mix i/o to be FLASH signal, CLK/CS/SI/SO/WP/HOLD */
	s5j_qspi_set_gpio();

	HW_REG32(0x80310000, 0x04) = 0x8010001A;	/* disable WP */
	HW_REG32(0x80310000, 0x78) = 0x8;		/* FLASH_IO_MODE */
	HW_REG32(0x80310000, 0x74) = 0x4;		/* QUAD */

	/* Check FLASH has Quad Enabled */
	while (!(HW_REG8(0x80310000, 0xDC) & (0x1 << 6)));
	lldbg("FLASH Quad Enabled\n");

	HW_REG32(0x80310000, 0x04) = 0x0010001A;	/* Enable WP */

	/* Set FLASH clk 80Mhz for Max performance */
	cal_clk_setrate(d1_serialflash, 80000000);
}
