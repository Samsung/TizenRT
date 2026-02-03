/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#include "armino_flash.h"
#include <driver/flash.h>
#include <os/os.h>
#include "flash_hal.h"
#include "armstar.h"
/****************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
#define PAGE_SHIFT (12)
#define FLASH_FS_START CONFIG_BK_FLASH_BASE
#define BK_NSECTORS (CONFIG_BK_FLASH_CAPACITY / CONFIG_BK_FLASH_SECTOR_SIZE)
#define BK_START_SECOTR (FLASH_FS_START / CONFIG_BK_FLASH_SECTOR_SIZE)

/************************************************************************************
 * Private Types
 ************************************************************************************/

/* This type represents the state of the MTD device.  The struct mtd_dev_s must
 * appear at the beginning of the definition so that you can freely cast between
 * pointers to struct mtd_dev_s and struct bk_dev_s.
 */
struct bk_dev_s {
	struct mtd_dev_s mtd;		/* MTD interface */
	int nsectors;				/* number of erase sectors */
};

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/

/* MTD driver methods */
static ssize_t bk_erase_page(size_t page);
static int bk_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks);
static ssize_t bk_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buf);
static ssize_t bk_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buf);
static ssize_t bk_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer);
static int bk_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg);

#if defined(CONFIG_MTD_BYTE_WRITE)
static ssize_t bk_write(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR const uint8_t *buffer);
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static bool s_ota_test_flag = false;
static bool get_ota_test_flag(int arg)
{	
	s_ota_test_flag = arg;
	return BK_OK;
}

/************************************************************************************
 * Name: bk_erase
 ************************************************************************************/
static ssize_t bk_erase_page(size_t page)
{
	uint32_t addr = (page << PAGE_SHIFT);
	ssize_t ret;

	//printf("func :%s, line :%d, addr :%x\n", __func__, __LINE__, addr);
	#if (!CONFIG_SPE)
	if (bk_addr_is_kernel(addr)) {
		ret = bk_security_flash_erase_sector(addr);
	} else
	#endif
	{
		ret = bk_flash_erase_sector(addr);
	}

	if (ret != OK) {
		ret = -EIO;
	}
	return ret;
}

static int bk_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks)
{
	ssize_t result;
	startblock += BK_START_SECOTR;

	/* Erase the specified blocks and return status (OK or a negated errno) */
	while (nblocks > 0) {
		result = bk_erase_page(startblock);
		if (result < 0) {
			return (int)result;
		}
		startblock++;
		nblocks--;
	}
	return OK;
}

static ssize_t bk_flash_write(size_t addr, const void *buf, size_t length)
{
	#if (!CONFIG_SPE)
	if (bk_addr_is_kernel(addr)) {
		bk_security_flash_write_bytes(addr, (uint8_t *)buf, length);
	} else
	#endif
	{
		bk_flash_write_bytes(addr, (uint8_t *)buf, length);
	}

	return BK_OK;
}

ssize_t bk_flash_read(size_t addr, void *buf, size_t length)
{
	// printf("func :%s, line :%d, addr :%x, length :%d\n", __func__, __LINE__, addr, length);
	#if (!CONFIG_SPE)
	if (bk_addr_is_kernel(addr)) {
		bk_security_flash_read_bytes(addr, (uint8_t *)buf, length);
		SCB_CleanInvalidateDCache();
	}
	#if CONFIG_BUILD_PROTECTED
	#if CONFIG_FLASH_ENCRYPT_ENABLE
	else if (bk_addr_is_app_or_common(addr)) {
		if(s_ota_test_flag) {
			bk_data_read_app_or_common(addr, (uint8_t *)buf, length);
		} else {
			bk_instruction_read_app_or_common(addr, (uint8_t *)buf, length);
		}
	}
	#endif
	#endif
	else
	#endif
	{
		bk_flash_read_bytes(addr, (uint8_t *)buf, length);
	}

	return BK_OK;
}


/************************************************************************************
 * Name: bk_bread
 ************************************************************************************/
static ssize_t bk_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buffer)
{
	ssize_t result;
	// size_t off = (startblock << PAGE_SHIFT); // Unused variable
	result = bk_flash_read(CONFIG_BK_FLASH_BASE + (startblock << PAGE_SHIFT), buffer, nblocks << PAGE_SHIFT);
	return result < 0 ? result : nblocks;
}


/************************************************************************************
 * Name: bk_bwrite
 ************************************************************************************/
static ssize_t bk_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buffer)
{
	ssize_t result;
	result = bk_flash_write(CONFIG_BK_FLASH_BASE + (startblock << PAGE_SHIFT), buffer, nblocks << PAGE_SHIFT);
	return result < 0 ? result : nblocks;
}


/************************************************************************************
 * Name: bk_read
 ************************************************************************************/

static ssize_t bk_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer)
{
	ssize_t result;
	result = bk_flash_read(CONFIG_BK_FLASH_BASE + offset, buffer, nbytes);
	return result < 0 ? result : nbytes;
}


/************************************************************************************
 * Name: bk_write
 ************************************************************************************/

#ifdef CONFIG_MTD_BYTE_WRITE
static ssize_t bk_write(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR const uint8_t *buffer)
{
	size_t addr;
	ssize_t result;
	addr = CONFIG_BK_FLASH_BASE + offset;
	result = bk_flash_write(addr, buffer, nbytes);
	return result < 0 ? result : nbytes;
}
#endif


/************************************************************************************
 * Name: bk_ioctl
 ************************************************************************************/

static int bk_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg)
{
	int ret = -EINVAL;			/* Assume good command with bad parameters */
	FAR struct bk_dev_s *priv = (FAR struct bk_dev_s *)dev;

	switch (cmd) {
	case MTDIOC_GEOMETRY: {
		FAR struct mtd_geometry_s *geo = (FAR struct mtd_geometry_s *)((uintptr_t) arg);
		if (geo) {
			geo->blocksize = CONFIG_BK_FLASH_SECTOR_SIZE;
			geo->erasesize = CONFIG_BK_FLASH_SECTOR_SIZE;
			geo->neraseblocks = priv->nsectors;
			ret = OK;
		}
	}
	break;

	case MTDIOC_BULKERASE: {
		/* Erase the entire device */
		ret = bk_erase(dev, 0, priv->nsectors);
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
 * Name: bk_initialize
 *
 * Description:
 *   Create an initialize MTD device instance.  MTD devices are not registered
 *   in the file system, but are created as instances that can be bound to
 *   other functions (such as a block or character driver front end).
 *
 ************************************************************************************/

FAR struct mtd_dev_s *up_flashinitialize(void)
{

	FAR struct bk_dev_s *priv;

	bk_flash_driver_init();

	priv = (FAR struct bk_dev_s *)kmm_zalloc(sizeof(struct bk_dev_s));

	if (priv) {
		/* Initialize the allocated structure (unsupported methods were
		 * nullified by kmm_zalloc).
		 */
		priv->mtd.erase = bk_erase;
		priv->mtd.bread = bk_bread;
		priv->mtd.bwrite = bk_bwrite;
		priv->mtd.read = bk_read;
		priv->mtd.ioctl = bk_ioctl;
#if defined(CONFIG_MTD_BYTE_WRITE)
		priv->mtd.write = bk_write;
#endif
		priv->nsectors = BK_NSECTORS;
#ifdef CONFIG_MTD_REGISTRATION
		priv->mtd.name = "bk_flash";
#endif

		printf("up_flashinitialize OK!\r\n");

		return (FAR struct mtd_dev_s *)priv;
	}
	return NULL;
}

