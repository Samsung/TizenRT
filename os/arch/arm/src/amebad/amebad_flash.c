/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 *
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
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
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/kmalloc.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/mtd.h>
#include "up_arch.h"
#include "chip.h"
#include "flash_api.h"

/****************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
#define PAGE_SHIFT (8)
#define FLASH_FS_START CONFIG_AMEBAD_FLASH_BASE
#define AMEBAD_NSECTORS (CONFIG_AMEBAD_FLASH_CAPACITY / CONFIG_AMEBAD_FLASH_BLOCK_SIZE)
#define AMEBAD_START_SECOTR (FLASH_FS_START / CONFIG_AMEBAD_FLASH_BLOCK_SIZE)

/************************************************************************************
 * Private Types
 ************************************************************************************/

/* This type represents the state of the MTD device.  The struct mtd_dev_s must
 * appear at the beginning of the definition so that you can freely cast between
 * pointers to struct mtd_dev_s and struct amebad_dev_s.
 */
struct amebad_dev_s {
	struct mtd_dev_s mtd;		/* MTD interface */
	int nsectors;				/* number of erase sectors */
};

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/

/* MTD driver methods */
static int amebad_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks);
static ssize_t amebad_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buf);
static ssize_t amebad_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buf);
static ssize_t amebad_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer);
static int amebad_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg);

#if defined(CONFIG_MTD_BYTE_WRITE)
static ssize_t amebad_write(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR const uint8_t *buffer);
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/************************************************************************************
 * Name: amebad_erase
 ************************************************************************************/
static ssize_t amebad_erase_page(size_t page)
{
	uint32_t address;
	irqstate_t irqs;

	if (page > (AMEBAD_START_SECOTR + AMEBAD_NSECTORS)) {
		printf("Invalid page number\n");
		return -EFAULT;
	}

	/* Disable IRQs while erasing sector */
	irqs = irqsave();

	/* do erase */
	address = page * CONFIG_AMEBAD_FLASH_BLOCK_SIZE;
	flash_erase_sector(NULL, address);

	/* Restore IRQs */
	irqrestore(irqs);

	return OK;
}

static int amebad_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks)
{
	ssize_t result;
	startblock += AMEBAD_START_SECOTR;

	/* Erase the specified blocks and return status (OK or a negated errno) */
	while (nblocks > 0) {
		result = amebad_erase_page(startblock);
		if (result < 0) {
			return (int)result;
		}
		startblock++;
		nblocks--;
	}
	return OK;
}

static ssize_t amebad_flash_write(size_t addr, const void *buf, size_t length)
{
	int32_t result = 0;
	irqstate_t irqs;

	if (buf == NULL) {
		return -EINVAL;
	}

	if (addr > CONFIG_AMEBAD_FLASH_BASE + CONFIG_AMEBAD_FLASH_CAPACITY) {
		return -EFAULT;
	}

	/* Disable IRQs while erasing sector */
	irqs = irqsave();
	result = flash_stream_write(NULL, addr, length, buf);

	/* Restore IRQs */
	irqrestore(irqs);

	if (result < 0) {
		return -EIO;
	} else {
		return result;
	}
}

ssize_t amebad_flash_read(size_t addr, void *buf, size_t length)
{
	int32_t result = 0;
	ssize_t ret = 0;
	irqstate_t irqs;

	if (buf == NULL) {
		return -EINVAL;
	}

	if (addr > CONFIG_AMEBAD_FLASH_BASE + CONFIG_AMEBAD_FLASH_CAPACITY) {
		return -EFAULT;
	}

	/* Disable IRQs while erasing sector */
	irqs = irqsave();
	if ((addr & 0x3) == 0) {
		//! if addr is 4 bytes aligned
		result = flash_stream_read(NULL, addr, length, buf);
		if (result < 0) {
			ret = -EIO;
		} else {
			ret = result;
		}
	} else {
		//! if addr is not 4 bytes aligned
		uint32_t offset = addr & 0x3;
		int8_t *aligned_read_buf = (int8_t *)kmm_malloc(length + offset);
		if (aligned_read_buf == NULL) {
			ret = -EPERM;
		} else {
			result = flash_stream_read(NULL, (addr & 0xfffffffc), length + offset, (char *)aligned_read_buf);

			if (result < 0) {
				ret = -EIO;
			} else {
				ret = result;
			}
			memcpy(buf, aligned_read_buf + offset, length);
			kmm_free(aligned_read_buf);
			aligned_read_buf = NULL;
		}
	}

	/* Restore IRQs */
	irqrestore(irqs);
	return ret;
}


/************************************************************************************
 * Name: amebad_bread
 ************************************************************************************/
static ssize_t amebad_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buffer)
{
	ssize_t result;
	result = amebad_flash_read(CONFIG_AMEBAD_FLASH_BASE + (startblock << PAGE_SHIFT), buffer, nblocks << PAGE_SHIFT);
	return result < 0 ? result : nblocks;
}


/************************************************************************************
 * Name: amebad_bwrite
 ************************************************************************************/
static ssize_t amebad_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buffer)
{
	ssize_t result;
	result = amebad_flash_write(CONFIG_AMEBAD_FLASH_BASE + (startblock << PAGE_SHIFT), buffer, nblocks << PAGE_SHIFT);
	return result < 0 ? result : nblocks;
}


/************************************************************************************
 * Name: amebad_read
 ************************************************************************************/

static ssize_t amebad_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer)
{
	ssize_t result;
	result = amebad_flash_read(CONFIG_AMEBAD_FLASH_BASE + offset, buffer, nbytes);
	return result < 0 ? result : nbytes;
}


/************************************************************************************
 * Name: amebad_write
 ************************************************************************************/

#ifdef CONFIG_MTD_BYTE_WRITE
static ssize_t amebad_write(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR const uint8_t *buffer)
{
	FAR struct amebad_dev_s *priv = (FAR struct mtd_dev_s *)dev;
	size_t addr;
	ssize_t result;
	addr = CONFIG_AMEBAD_FLASH_BASE + offset;
	result = amebad_flash_write(addr, buffer, nbytes);
	return result < 0 ? result : nbytes;
}
#endif


/************************************************************************************
 * Name: amebad_ioctl
 ************************************************************************************/

static int amebad_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg)
{
	int ret = -EINVAL;			/* Assume good command with bad parameters */
	FAR struct amebad_dev_s *priv = (FAR struct mtd_dev_s *)dev;

	switch (cmd) {
	case MTDIOC_GEOMETRY: {
		FAR struct mtd_geometry_s *geo = (FAR struct mtd_geometry_s *)((uintptr_t) arg);
		if (geo) {
			geo->blocksize = CONFIG_AMEBAD_FLASH_PAGE_SIZE;
			geo->erasesize = CONFIG_AMEBAD_FLASH_BLOCK_SIZE;
			geo->neraseblocks = priv->nsectors;
			ret = OK;
		}
	}
	break;

	case MTDIOC_BULKERASE: {
		/* Erase the entire device */
		ret = amebad_erase(dev, 0, priv->nsectors);
	}
	break;

	case MTDIOC_XIPBASE:
	default:
		ret = -ENOTTY;			/* Bad command */
		break;
	}
	return ret;
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: amebad_initialize
 *
 * Description:
 *   Create an initialize MTD device instance.  MTD devices are not registered
 *   in the file system, but are created as instances that can be bound to
 *   other functions (such as a block or character driver front end).
 *
 ************************************************************************************/

FAR struct mtd_dev_s *up_flashinitialize(void)
{
	FAR struct amebad_dev_s *priv;
	priv = (FAR struct amebad_dev_s *)kmm_zalloc(sizeof(struct amebad_dev_s));

	if (priv) {
		/* Initialize the allocated structure (unsupported methods were
		 * nullified by kmm_zalloc).
		 */
		priv->mtd.erase = amebad_erase;
		priv->mtd.bread = amebad_bread;
		priv->mtd.bwrite = amebad_bwrite;
		priv->mtd.read = amebad_read;
		priv->mtd.ioctl = amebad_ioctl;

#if defined(CONFIG_MTD_BYTE_WRITE)
		priv->mtd.write = amebad_write;
#endif
		priv->nsectors = AMEBAD_NSECTORS;
		return (FAR struct mtd_dev_s *)priv;
	}
	return NULL;
}

