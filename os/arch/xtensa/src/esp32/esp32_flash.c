/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics Co., LTD. All Rights Reserved.
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
#include <tinyara/irq.h>
#include <tinyara/progmem.h>
#include <tinyara/kmalloc.h>
#include <stdio.h>
#include <string.h>

#include <esp_spi_flash.h>


#ifdef CONFIG_MTD_PROGMEM

#define PAGES_PER_SECTOR (CONFIG_ESP32_FLASH_BLOCK_SIZE/CONFIG_ESP32_FLASH_PAGE_SIZE)

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int32_t up_progmem_isaddressreadable(size_t address)
{
	return 0;
}

static int32_t up_progmem_isaddresswritable(size_t address)
{
	return 0;
}

/*number of erase blocks*/
static inline size_t start_page(void)
{
    return CONFIG_ESP_FLASH_BASE / CONFIG_ESP32_FLASH_BLOCK_SIZE;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

size_t up_progmem_npages(void)
{
    return CONFIG_ESP32_FLASH_CAPACITY / CONFIG_ESP32_FLASH_BLOCK_SIZE;
}

bool up_progmem_isuniform(void)
{
    return true;
}

size_t up_progmem_pagesize(size_t page)
{
    return CONFIG_ESP32_FLASH_PAGE_SIZE;
}

size_t up_progmem_blocksize(void)
{
    return CONFIG_ESP32_FLASH_BLOCK_SIZE;
}

/****************************************************************************
 * Name: up_progmem_getaddress
 *
 * Description:
 *   Page to address conversion
 *
 * Input Parameters:
 *   page - page index
 *
 * Returned Value:
 *   Base address of given page, SIZE_MAX if page index is not valid.
 *
 ****************************************************************************/

size_t up_progmem_getaddress(size_t page)
{
	return CONFIG_ESP_FLASH_BASE + up_progmem_pagesize(0) * page;
}

/****************************************************************************
 * Name: up_progmem_erasepage
 *
 * Description:
 *   Erase selected page.
 *
 * Input Parameters:
 *   page -
 *
 * Returned Value:
 *   Page size or negative value on error.  The following errors are reported
 *   (errno is not set!):
 *
 *     EFAULT: On invalid page
 *     EIO: On unsuccessful erase
 *     EROFS: On access to write protected area
 *     EACCES: Insufficient permissions (read/write protected)
 *     EPERM: If operation is not permitted due to some other constraints
 *        (i.e. some internal block is not running etc.)
 *
 ****************************************************************************/

ssize_t up_progmem_erasepage(size_t page)
{
	irqstate_t irqs;
	int32_t result = 0;
	result = result;

	if (page >= up_progmem_npages() + start_page()) {
		return -EFAULT;
	}
    /* Disable IRQs while erasing sector */
	irqs = irqsave();

	/* do erase */
	result = spi_flash_erase_sector(page);

	/* Restore IRQs */
	irqrestore(irqs);

	return result == 0 ? CONFIG_ESP32_FLASH_BLOCK_SIZE : -EIO;
}

/****************************************************************************
 * Name: up_progmem_write
 *
 * Description:
 *   Program data at given address
 *
 *   Note: this function is not limited to single page and nor it requires
 *   the address be aligned inside the page boundaries.
 *
 * Input Parameters:
 *   addr  - Address with or without flash offset (absolute or aligned to page0)
 *   buf   - Pointer to buffer
 *   count - Number of bytes to write *
 *
 * Returned Value:
 *   Bytes written or negative value on error.  The following errors are
 *   reported (errno is not set!)
 *
 *     EINVAL: if count is not aligned with the flash boundaries (i.e.
 *        some MCU's require per half-word or even word access)
 *     EFAULT: On invalid address
 *     EIO: On unsuccessful write
 *     EROFS: On access to write protected area
 *     EACCES: Insufficient permissions (read/write protected)
 *     EPERM: If operation is not permitted due to some other constraints
 *        (i.e. some internal block is not running etc.)
 *
 ****************************************************************************/

ssize_t up_progmem_write(size_t addr, const void *buf, size_t count)
{
	int32_t result = 0;
	ssize_t ret = 0;
	irqstate_t irqs;

	/* Disable IRQs while erasing sector */
	irqs = irqsave();

	if (0 != up_progmem_isaddresswritable(addr)) {
		ret = -EROFS;
	} else if ((addr & 0xff) != 0) {
		//! value should be 256 byte alignment
		ret = -EFAULT;
	} else if ((count & 0x3) != 0) {
		ret = -EINVAL;
	} else {
		char *adpt_buf = (char *)kmm_malloc(count);
		if (adpt_buf == NULL) {
			printf("fail to alloc memory\n");
			ret = -EPERM;
		} else {
			memcpy(adpt_buf, buf, count);
			result = spi_flash_write(addr, adpt_buf, count);
			if (result != 0) {
				ret = -EIO;
			} else {
				ret = count;
			}
		}
	}

	/* Restore IRQs */
	irqrestore(irqs);
	if (ret < 0) {
		printf("Errno: %d, addr %d, buf %p, count %d\n", ret, addr, buf, count);
	}
	return ret;
}

/****************************************************************************
 * Name: up_progmem_read
 *
 * Description:
 *   Read data at given address
 *
 *   Note: this function is not limited to single page and nor it requires
 *   the address be aligned inside the page boundaries.
 *
 * Input Parameters:
 *   addr  - Address with or without flash offset (absolute or aligned to page0)
 *   buf   - Pointer to buffer
 *   count - Number of bytes to read *
 *
 * Returned Value:
 *   Bytes read or negative value on error.  The following errors are
 *   reported (errno is not set!)
 *
 *     EINVAL: if count is not aligned with the flash boundaries (i.e.
 *        some MCU's require per half-word or even word access)
 *     EFAULT: On invalid address
 *     EIO: On unsuccessful write
 *     EROFS: On access to read protected area
 *     EACCES: Insufficient permissions (read/write protected)
 *     EPERM: If operation is not permitted due to some other constraints
 *        (i.e. some internal block is not running etc.)
 *
 ****************************************************************************/
ssize_t up_progmem_read(size_t addr, void *buf, size_t count)
{
	int32_t result = 0;
	ssize_t ret = 0;
	irqstate_t irqs;
	result = result;

	/* Disable IRQs while erasing sector */
	irqs = irqsave();
	if (0 != up_progmem_isaddressreadable(addr)) {
		ret = -EACCES;
	} else {
		if ((addr & 0x3) == 0) {
			//! if addr is 4 bytes aligned
			result = spi_flash_read(addr, buf, count);

			if (result != 0) {
				ret = -EIO;
			} else {
				ret = count;
			}
		} else {
			//! if addr is not 4 bytes aligned
			uint32_t offset = addr & 0x3;
			int8_t *aligned_read_buf = (int8_t *)kmm_malloc(count + offset);
			if (aligned_read_buf == NULL) {
				printf("fail to alloc memory\n");
				ret = -EPERM;
			} else {
				result = spi_flash_read((addr & 0xfffffffc), (char *)aligned_read_buf, count + offset);
				if (result != 0) {
					ret = -EIO;
				} else {
					ret = count;
				}

				memcpy(buf, aligned_read_buf + offset, count);
				kmm_free(aligned_read_buf);
				aligned_read_buf = NULL;
			}
		}
	}

	/* Restore IRQs */
	irqrestore(irqs);

	if (ret < 0) {
		printf("Errno: %d, addr %d, buf %p, count %d\n", ret, addr, buf, count);
	}
	return ret;
}
#endif							/* CONFIG_MTD_PROGMEM */

