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

#include <tinyara/kmalloc.h>

#include <arch/chip/irq.h>
#include <tinyara/watchdog.h>
#include <arch/chip/chip_types.h>

#include "chip.h"
#include "up_arch.h"
#include "cache.h"
#include "mpu.h"

#include <tinyara/clock.h>
#include <clock/clock.h>
#include <pthread.h>
#include <semaphore.h>
#include <up_internal.h>

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

static sem_t count_sem;
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/****************************************************************************
 * Private Types
 ****************************************************************************/
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void s5j_qspi_copy_4byte(unsigned int target_addr, unsigned int source_addr, unsigned int sizebyte)
{
	unsigned int loopt;
	unsigned int roundup;
	int i;

	roundup = sizebyte % 4;
	if (roundup) {
		sizebyte += (4 - roundup);
	}

	loopt = sizebyte / 4;

	for (i = 0; i < loopt; i++) {
		Outp32(target_addr + (4 * i), Inp32(source_addr + (4 * i)));
	}

	return;
}

static void s5j_qspi_copy_1byte(unsigned int target_addr, unsigned int source_addr, unsigned int sizebyte)
{
	unsigned int loopt;
	int i;
	loopt = sizebyte;

	for (i = 0; i < loopt; i++) {
		Outp8(target_addr + (i), Inp8(source_addr + (i)));
	}

	return;

}

static void s5j_qspi_set_gpio(void)
{
	signed int gpio_sf_clk, gpio_sf_cs, gpio_sf_si, gpio_sf_so, gpio_sf_wp, gpio_sf_hld;

	gpio_sf_clk = s5j_gpio(GPP1, 0);
	gpio_sf_cs = s5j_gpio(GPP1, 1);
	gpio_sf_si = s5j_gpio(GPP1, 2);
	gpio_sf_so = s5j_gpio(GPP1, 3);
	gpio_sf_wp = s5j_gpio(GPP1, 4);
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

	return;
}

/**
*
* @brief read status register from FLASH
* @param void
* @return void
* @note
*/
static flash_status_register s5j_qspi_get_status_register(void)
{
	flash_status_register reg = { 0 };
	reg.rdsr = Inp8(rRDSR);

	lldbg("0x%x\n", reg.rdsr);

	return reg;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/**
*
* @brief disable write protection of FLASH
* @param void
* @return void
* @note
	writable when wp is disabled
*/
void s5j_qspi_disable_wp(void)
{
	unsigned int reg;

	while (sem_wait(&count_sem) != OK) {
		ASSERT(errno == EINTR);
	}

	/* someone has been disabled wp, we should wait until it's released */

	do {
		reg = (HW_REG32(0x80310000, 0x04) & ~(0x1 << 31)) >> 31;
	} while (reg);

	HW_REG32(0x80310000, 0x04) &= ~(0x1 << 31);
	HW_REG32(0x80310000, 0x04) |= (0x1 << 31);
}

/**
*
* @brief enable write protection of FLASH
* @param void
* @return void
* @note
*/
void s5j_qspi_enable_wp(void)
{
	HW_REG32(0x80310000, 0x04) &= ~(0x1 << 31);
	sem_post(&count_sem);
}

/**
*
* @brief semaphore for FLASH access transaction
* @param void
* @return void
* @note
*/
void s5j_qspi_take_sem(void)
{
	while (sem_wait(&count_sem) != OK) {
		ASSERT(errno == EINTR);
	}
}

/**
*
* @brief semaphore for FLASH access transaction
* @param void
* @return void
* @note
*/
void s5j_qspi_release_sem(void)
{
	sem_post(&count_sem);
}

/**
*
* @brief initialize FLASH for QUAD IO in 80Mhz
* @param void
* @return void
* @note
*/
void s5j_qspi_init(void)
{
	/* Set mix i/o to be FLASH signal, CLK/CS/SI/SO/WP/HOLD */
	s5j_qspi_set_gpio();

	HW_REG32(0x80310000, 0x04) = 0x8010001A;	/* disable WP */
	HW_REG32(0x80310000, 0x78) = 0x8;	/* FLASH_IO_MODE */
	HW_REG32(0x80310000, 0x74) = 0x4;	/* QUAD */

	while (!(HW_REG8(0x80310000, 0xDC) & (0x1 << 6))) ;	/* Check FLASH has Quad Enabled */
	lldbg("FLASH Quad Enabled\n");

	s5j_qspi_get_status_register();
	HW_REG32(0x80310000, 0x04) = 0x0010001A;	/* Enable WP */

	/* Set FLASH clk 80Mhz for Max performance */
	cal_clk_setrate(d1_serialflash, 80000000);

	sem_init(&count_sem, 0, 1);
}
