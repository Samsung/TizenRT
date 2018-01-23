/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics Co., LTD. All Rights Reserved.
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
 * arch/arm/src/s5j/s5j_sflash.c
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
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
 * 3. Neither the name tinyara nor the names of its contributors may be
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

#include <debug.h>
#include <errno.h>
#include <semaphore.h>
#include <tinyara/semaphore.h>
#include <tinyara/irq.h>
#include <tinyara/progmem.h>

#include "up_arch.h"
#include "cache.h"
#include "chip.h"

#include "s5j_clock.h"
#include "s5j_gpio.h"
#include "chip/s5jt200_sflash.h"

static sem_t sem_excl;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void s5j_sflash_disable_wp(void)
{
	/* someone has been disabled wp, we should wait until it's released */
	while (getreg32(S5J_SFLASH_SFCON) & SFLASH_SFCON_WP_DISABLE) ;

	modifyreg32(S5J_SFLASH_SFCON,
				SFLASH_SFCON_WP_ENABLE, SFLASH_SFCON_WP_DISABLE);
}

static void s5j_sflash_enable_wp(void)
{
	modifyreg32(S5J_SFLASH_SFCON,
				SFLASH_SFCON_WP_DISABLE, SFLASH_SFCON_WP_ENABLE);
}

static uint8_t s5j_sflash_read_status(void)
{
	return getreg8(S5J_SFLASH_RDSR);
}

static inline void s5j_sflash_lock(void)
{
	sem_post(&sem_excl);
}

static inline void s5j_sflash_lock_init(void)
{
	sem_init(&sem_excl, 0, 1);
#ifdef CONFIG_PRIORITY_INHERITANCE
	sem_setprotocol(&sem_excl, SEM_PRIO_NONE);
#endif
}

static inline void s5j_sflash_unlock(void)
{
	while (sem_wait(&sem_excl) != OK) {
		ASSERT(errno == EINTR);
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_MTD_PROGMEM
size_t up_progmem_getaddress(size_t page)
{
	return S5J_FLASH_PADDR + FLASH_PAGE_SIZE * page;
}

ssize_t up_progmem_getpage(size_t addr)
{
	return (addr - S5J_FLASH_PADDR) / FLASH_PAGE_SIZE;
}

ssize_t up_progmem_erasepage(size_t page)
{
	size_t addr;

	if (page >= FLASH_MAX_PAGE) {
		return -EFAULT;
	}

	addr = up_progmem_getaddress(page);

	/* lock */
	s5j_sflash_lock();

	s5j_sflash_disable_wp();

	/* Set sector address and then send erase command */
	putreg32(addr - S5J_FLASH_PADDR, S5J_SFLASH_ERASE_ADDRESS);
	putreg8(0xff, S5J_SFLASH_SE);

	/* Wait for the completion */
	while (s5j_sflash_read_status() & 0x1);

	s5j_sflash_enable_wp();

	/* Invalidate cache */
	arch_invalidate_dcache(addr, addr + FLASH_BLOCK_SIZE);

	/* unlock */
	s5j_sflash_unlock();

	return FLASH_BLOCK_SIZE;
}

ssize_t up_progmem_ispageerased(size_t page)
{
	size_t addr;
	size_t count;
	size_t bwritten;

	if (page >= FLASH_MAX_PAGE) {
		return -EFAULT;
	}

	bwritten = 0;
	addr = up_progmem_getaddress(page);
	for (count = FLASH_PAGE_SIZE; count; count--) {
		if (getreg32(addr) != 0xffffff) {
			bwritten++;
		}
		addr += sizeof(int);
	}

	return bwritten * sizeof(int);
}

ssize_t up_progmem_write(size_t addr, const void *buf, size_t count)
{
	int page;
	int pagesize;
	size_t remain = count;

	page = up_progmem_getpage(addr + count);
	if (page < 0) {
		return -EINVAL;
	}

	pagesize = FLASH_PAGE_SIZE;

	while (remain) {
		int tmp = remain;

		if (tmp > pagesize) {
			tmp = pagesize;
		}

		/* lock */
		s5j_sflash_lock();

		s5j_sflash_disable_wp();

		/* Load and write data */
		memcpy((void *)(addr-S5J_FLASH_PADDR + S5J_FLASH_MIRROR_PADDR), buf, tmp);

		/* Flush cache */
		arch_flush_dcache(addr, addr + tmp);

		s5j_sflash_enable_wp();

		/* unlock */
		s5j_sflash_unlock();

		buf    += tmp;
		addr   += tmp;
		remain -= tmp;
	}

	return count;
}
#endif /* CONFIG_MTD_PROGMEM */

/**
 * @brief	initialize FLASH for QUAD IO in 80Mhz
 * @param	void
 * @return	void
 * @note
 */
void s5j_sflash_init(void)
{
	s5j_sflash_disable_wp();

	modifyreg32(S5J_SFLASH_SFCON, 0, SFLASH_SFCON_ERASE_WAIT_ON);
	putreg32(SFLASH_PERF_MODE_DUAL_QUAD, S5J_SFLASH_PERF_MODE);
	putreg32(SFLASH_IO_MODE_QUAD_FAST_READ, S5J_SFLASH_IO_MODE);

	/* Check FLASH has Quad Enabled */
	while (!(s5j_sflash_read_status() & (0x1 << 6))) ;
	lldbg("FLASH Quad Enabled\n");

	s5j_sflash_enable_wp();

	s5j_sflash_lock_init();

	/* Set FLASH clk 80Mhz for Max performance */
	s5j_clk_set_rate(CLK_SPL_SFLASH, 80000000);
}

#if defined(CONFIG_BOARD_CRASHDUMP)
void s5j_direct_erase(size_t start_addr, size_t len)
{
	s5j_sflash_disable_wp();
	uint32_t address = start_addr;

	while (address < start_addr + len) {
		putreg32(address - S5J_FLASH_PADDR, S5J_SFLASH_ERASE_ADDRESS);
		putreg8(0xff, S5J_SFLASH_SE);

		while (s5j_sflash_read_status() & 0x1);

		arch_invalidate_dcache(address, address + CONFIG_ARTIK05X_FLASH_PAGE_SIZE);
		address += CONFIG_ARTIK05X_FLASH_PAGE_SIZE;
	}
	s5j_sflash_enable_wp();
}

void s5j_direct_write(size_t addr, const void *buf, size_t len)
{
	size_t remain = len;

	while (remain) {
		int w_size = remain;

		if (w_size > CONFIG_ARTIK05X_FLASH_PAGE_SIZE) {
			w_size = CONFIG_ARTIK05X_FLASH_PAGE_SIZE;
		}

		s5j_sflash_disable_wp();
		memcpy((void *)(addr - S5J_FLASH_PADDR + S5J_FLASH_MIRROR_PADDR), buf, w_size);
		arch_flush_dcache(addr, addr + w_size);
		s5j_sflash_enable_wp();
		buf    += w_size;
		addr   += w_size;
		remain -= w_size;
	}
}
#endif
