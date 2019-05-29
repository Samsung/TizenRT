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
/************************************************************************************
 *  os/arch/arm/src/imxrt/imxrt_mtd_flash.c
 *
 *   Copyright (C) 2009-2011, 2013 Gregory Nutt. All rights reserved.
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
 ************************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/spi/spi.h>
#include <tinyara/fs/mtd.h>

#include "imxrt_flash.h"
#include "imxrt_log.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
/* Various manufacturers may have produced the parts.  0x20 is the manufacturer ID
 * for the STMicro MP25x serial FLASH.  If, for example, you are using the a Macronix
 * International MX25 serial FLASH, the correct manufacturer ID would be 0xc2.
 */

#ifndef CONFIG_IMXRT_MANUFACTURER
//#error  "Manufacturer Id not selected for the flash device"
#endif

#ifndef CONFIG_IMXRT_MEMORY_TYPE
//#error  "Memory Type not defined no selected for the flash device"
#endif

#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
/*  NorFlash capacity is 8,388,608 bytes:
 *  (2048 sectors) * (4096 bytes per sector)
 *  (32768 pages) * (256 bytes per page)
 */
#define IMXRT_1020EVK_NORFLASH_SECTOR_SHIFT    12	/* Sector size 1 << 12 = 4096 */
#define IMXRT_1020EVK_NORFLASH_NSECTORS        2048
#define IMXRT_1020EVK_NORFLASH_PAGE_SHIFT      8	/* Page size 1 << 8 = 256 */
#define IMXRT_1020EVK_NORFLASH_NPAGES          32768
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
/*  HyperFlash capacity is 67,108,864 bytes:
 *  (256 sectors) * (262,144 bytes per sector)
 *  (131072 pages) * (512 bytes per page)
 */
#define IMXRT_1050EVK_HYPERFLASH_SECTOR_SHIFT    18	/* Sector size 1 << 18 = 262144 */
#define IMXRT_1050EVK_HYPERFLASH_NSECTORS        256
#define IMXRT_1050EVK_HYPERFLASH_PAGE_SHIFT      9	/* Page size 1 << 9 = 512 */
#define IMXRT_1050EVK_HYPERFLASH_NPAGES          131072
#else
#endif

/************************************************************************************
 * Private Types
 ************************************************************************************/

/* This type represents the state of the MTD device.  The struct mtd_dev_s
 * must appear at the beginning of the definition so that you can freely
 * cast between pointers to struct mtd_dev_s and struct IMXRT_dev_s.
 */

struct imxrt_dev_s {
	struct mtd_dev_s mtd;	
	uint8_t sectorshift;		
	uint8_t pageshift;			
	uint16_t nsectors;			
	uint32_t npages;			
#ifdef CONFIG_IMXRT_SUBSECTOR_ERASE
	uint8_t subsectorshift;		
#endif
};

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/

/* Helpers */
static inline int imxrt_readid(struct imxrt_dev_s *priv);
static void imxrt_waitwritecomplete(struct imxrt_dev_s *priv);
static inline void imxrt_sectorerase(struct imxrt_dev_s *priv, off_t offset);
static inline int imxrt_bulkerase(struct imxrt_dev_s *priv, unsigned long arg);
static inline void imxrt_pagewrite(struct imxrt_dev_s *priv, FAR const uint8_t *buffer, off_t offset);

/* MTD driver methods */
static int imxrt_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks);
static ssize_t imxrt_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buf);
static ssize_t imxrt_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buf);
static ssize_t imxrt_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer);
#ifdef CONFIG_MTD_BYTE_WRITE
static ssize_t imxrt_write(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR const uint8_t *buffer);
#endif
static int imxrt_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg);

/************************************************************************************
 * Private Data
 ************************************************************************************/

/************************************************************************************
 * Private Functions
 ************************************************************************************/
/************************************************************************************
 * Name: imxrt_readid
 ************************************************************************************/

static inline int imxrt_readid(struct imxrt_dev_s *priv)
{
	status_t status;

	fvdbg("priv: %p\n", priv);

	#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
	uint8_t vendorID = 0;

	/* Get vendor ID. */
	status = imxrt_flexspi_nor_get_vendor_id(IMXRT_FLEXSPI, &vendorID);
	if (status != kStatus_Success) {
		return status;
	}
	fvdbg("Vendor ID: 0x%x\r\n", vendorID);

	/* Save the FLASH geometry */

	priv->sectorshift = IMXRT_1020EVK_NORFLASH_SECTOR_SHIFT;
	priv->nsectors = IMXRT_1020EVK_NORFLASH_NSECTORS;
	priv->pageshift = IMXRT_1020EVK_NORFLASH_PAGE_SHIFT;
	priv->npages = IMXRT_1020EVK_NORFLASH_NPAGES;
	#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
	status = imxrt_flexspi_nor_hyperflash_cfi(IMXRT_FLEXSPI);
	/* Get vendor ID. */
	if (status != kStatus_Success) {
		return -ENODEV;
	} else {
		fvdbg("Found the HyperFlash by CFI\r\n");
	}

	/* Save the FLASH geometry */

	priv->sectorshift = IMXRT_1050EVK_HYPERFLASH_SECTOR_SHIFT;
	priv->nsectors = IMXRT_1050EVK_HYPERFLASH_NSECTORS;
	priv->pageshift = IMXRT_1050EVK_HYPERFLASH_PAGE_SHIFT;
	priv->npages = IMXRT_1050EVK_HYPERFLASH_NPAGES;
	#endif
	return OK;
}

/************************************************************************************
 * Name: imxrt_waitwritecomplete
 ************************************************************************************/

static void imxrt_waitwritecomplete(struct imxrt_dev_s *priv)
{
	status_t status;

	status = imxrt_flexspi_nor_wait_bus_busy(IMXRT_FLEXSPI);

	if (status != kStatus_Success) {
		fdbg("imxrt_flexspi_nor_wait_bus_busy: error[%d]\n", status);
	}
}

/************************************************************************************
 * Name:  imxrt_sectorerase
 ************************************************************************************/

static void imxrt_sectorerase(struct imxrt_dev_s *priv, off_t sector)
{
	status_t status;
	off_t offset;

	#ifdef CONFIG_IMXRT_SUBSECTOR_ERASE
	if (priv->subsectorshift > 0) {
		offset = sector << priv->subsectorshift;
	} else
	#endif
	{
		offset = sector << priv->sectorshift;
	}

	fvdbg("priv: 0x%x,  IMXRT_FLEXSPI: 0x%x,  offset: 0x%x\n", priv, IMXRT_FLEXSPI, (long)offset);

	status = imxrt_flexspi_nor_flash_erase_sector(IMXRT_FLEXSPI, offset);
	if (status != kStatus_Success) {
		fdbg("Erased fail!\n");
		return;
	}

	fvdbg("Erased\n");
}

/************************************************************************************
 * Name:  imxrt_bulkerase
 ************************************************************************************/

static inline int imxrt_bulkerase(struct imxrt_dev_s *priv, unsigned long arg)
{
	fvdbg("priv: %p\n", priv);

    #ifdef CONSIDER_IMPLEMENT
	/* Wait for any preceding write to complete.  We could simplify things by
	 * perform this wait at the end of each write operation (rather than at
	 * the beginning of ALL operations), but have the wait first will slightly
	 * improve performance.
	 */

	imxrt_waitwritecomplete(priv);

	/* Send write enable instruction */

	imxrt_writeenable(priv);

	/* Select this FLASH part */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, true);

	/* Send the "Bulk Erase (BE)" instruction */

	(void)SPI_SEND(priv->dev, IMXRT_BE);

	/* Deselect the FLASH */

	SPI_SELECT(priv->dev, SPIDEV_FLASH, false);
	
	size_t sectors_erased = 0;
	size_t startsector = 8;
//	fdbg("startsector: %d nsectors: %d\n", (long)startsector, (int)priv->nsectors);
	fdbg("startsector: %d nsectors: %d\n", (long)startsector, 2);

	/* Lock access to the SPI bus until we complete the erase */

//	while (sectors_erased < priv->nsectors) {
	while (sectors_erased < 2) {
		/* Not using sub-sector erase.  Erase each full sector */

		imxrt_sectorerase(priv, startsector + sectors_erased);
		sectors_erased++;
	}

	fdbg("Return = erased sectors = %d\n", sectors_erased);
	return sectors_erased;
	#endif

	fdbg("Return: OK\n");
	return OK;
}

/************************************************************************************
 * Name:  imxrt_pagewrite
 ************************************************************************************/

static inline void imxrt_pagewrite(struct imxrt_dev_s *priv, FAR const uint8_t *buffer, off_t page)
{
	status_t status;
	off_t offset = page << priv->pageshift;

	fvdbg("priv:0x%x, page: %08lx offset: %08lx,  buf = 0x%x\n", priv, (long)page, (long)offset, buffer);

	/* Wait for any preceding write to complete.  We could simplify things by
	* perform this wait at the end of each write operation (rather than at
	* the beginning of ALL operations), but have the wait first will slightly
	* improve performance.
	*/

	status = imxrt_flexspi_nor_flash_page_program(IMXRT_FLEXSPI, offset,
											(void *)buffer);
	if (status != kStatus_Success) {
		fdbg("Page program failure !\r\n");
		return;
	}
	fvdbg("Written\n");
}

/************************************************************************************
 * Name:  imxrt_bytewrite
 ************************************************************************************/

#ifdef CONFIG_MTD_BYTE_WRITE
static inline void imxrt_bytewrite(struct imxrt_dev_s *priv, FAR const uint8_t *buffer, off_t offset, uint16_t count)
{
	status_t status;
	fvdbg("offset: %08lx  count:%d\n", (long)offset, count);

	#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
	#warning "Need to check Configuration!!"
	#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
	status = imxrt_flexspi_nor_hyperbus_write(IMXRT_FLEXSPI, offset, (uint32_t *)buffer, count);
	if (status != kStatus_Success) {
		fdbg("Page program failure !\r\n");
		return;
	}
	#endif

	fvdbg("Written\n");
}
#endif

/************************************************************************************
 * Name: imxrt_erase
 ************************************************************************************/

static int imxrt_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks)
{
	FAR struct imxrt_dev_s *priv = (FAR struct imxrt_dev_s *)dev;
	size_t blocks_erased = 0;

	fvdbg("priv: %p, startblock: %d, nblocks: %d\n", priv, startblock, nblocks);

	/* Lock access to the SPI bus until we complete the erase */

	while (blocks_erased < nblocks) {
#ifdef CONFIG_IMXRT_SUBSECTOR_ERASE
		size_t sectorboundry;
		size_t blkper;

		/* If we have a smaller erase size, then we will find as many full
		 * sector erase blocks as possible to speed up the process instead of
		 * erasing everything in smaller chunks.
		 */

		if (priv->subsectorshift > 0) {
			blkper = 1 << (priv->sectorshift - priv->subsectorshift);
			sectorboundry = (startblock + blkper - 1) / blkper;
			sectorboundry *= blkper;

			/* If we are on a sector boundry and have at least a full sector
			 * of blocks left to erase, then we can do a full sector erase.
			 */

			if (startblock == sectorboundry && blocksleft >= blkper) {
				/* Do a full sector erase */

				imxrt_sectorerase(priv, startblock);
				startblock += blkper;
				blocksleft -= blkper;
				continue;
			} else {
				/* Just do a sub-sector erase */

				imxrt_sectorerase(priv, startblock);
				startblock++;
				blocksleft--;
				continue;
			}
		}
#endif

		/* Not using sub-sector erase.  Erase each full sector */

		imxrt_sectorerase(priv, startblock + blocks_erased);
		blocks_erased++;
	}
	fvdbg("erased blocks = %d\n", blocks_erased);

	return blocks_erased == nblocks ? nblocks : EIO;
}

/************************************************************************************
 * Name: imxrt_bread
 ************************************************************************************/

static ssize_t imxrt_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buffer)
{
	FAR struct imxrt_dev_s *priv = (FAR struct imxrt_dev_s *)dev;
	ssize_t nbytes = nblocks << priv->pageshift;

	fvdbg("priv: 0x%x , IMXRT_FLEXSPI: %x, startblock: %d   nblocks: %d   offset: 0x%x %08lx   nbytes: %d\n",
		priv, IMXRT_FLEXSPI, (long)startblock, (int)nblocks, (long)startblock << priv->pageshift, (long)startblock << priv->pageshift, (int)nblocks << priv->pageshift);

	memcpy(buffer, (void*)(IMXRT_FLASH_BASE + (startblock << priv->pageshift)), nblocks << priv->pageshift);
	fvdbg("nbytes:%d, buffer value: [%x] [%x] [%x] [%x] [%x] [%x]\n", nbytes, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);

	return (int)nbytes >> priv->pageshift;
}

/************************************************************************************
 * Name: imxrt_bwrite
 ************************************************************************************/

static ssize_t imxrt_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buffer)
{
	FAR struct imxrt_dev_s *priv = (FAR struct imxrt_dev_s *)dev;
	off_t blockswritten = 0;
	size_t pagesize = 1 << priv->pageshift;
	status_t status;
	ssize_t nbytes;

	fvdbg("priv: %x  ,  IMXRT_FLEXSPI: %x,  startblock: %08lx nblocks: %d, pagesize = %d, pageshift = %d,  offset = 0x%x  %08lx\n",
		priv, IMXRT_FLEXSPI, (long)startblock, (int)nblocks, pagesize, priv->pageshift, startblock << priv->pageshift, startblock << priv->pageshift);

	/* Lock the SPI bus and write each page to FLASH */

	fvdbg("buffer value: [%x] [%x] [%x] [%x] [%x] [%x]\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);

	while (blockswritten < nblocks) {
		imxrt_pagewrite(priv, buffer + pagesize * blockswritten, startblock + blockswritten);
		blockswritten++;
	}

	return nblocks;
}

/************************************************************************************
 * Name: imxrt_read
 ************************************************************************************/

static ssize_t imxrt_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer)
{
	status_t status;

	fvdbg("offset: %08lx nbytes: %d\n", (long)offset, (int)nbytes);

	memcpy(buffer, (void*)(IMXRT_FLASH_BASE + offset), nbytes);

	fvdbg("return nbytes: %d\n", (int)nbytes);
	return nbytes;
}

/************************************************************************************
 * Name: imxrt_write
 ************************************************************************************/

#ifdef CONFIG_MTD_BYTE_WRITE
static ssize_t imxrt_write(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR const uint8_t *buffer)
{
	FAR struct imxrt_dev_s *priv = (FAR struct imxrt_dev_s *)dev;
	int startpage;
	int endpage;
	int count;
	int index;
	int pagesize;
	int bytestowrite;

	fvdbg("offset: %08lx nbytes: %d\n", (long)offset, (int)nbytes);

	/* We must test if the offset + count crosses one or more pages
	 * and perform individual writes.  The devices can only write in
	 * page increments.
	 */

	startpage = offset / (1 << priv->pageshift);
	endpage = (offset + nbytes) / (1 << priv->pageshift);

	if (startpage == endpage) {
		/* All bytes within one programmable page.  Just do the write. */

		imxrt_bytewrite(priv, buffer, offset, nbytes);
	} else {
		/* Write the 1st partial-page */

		count = nbytes;
		pagesize = (1 << priv->pageshift);
		bytestowrite = pagesize - (offset & (pagesize - 1));
		imxrt_bytewrite(priv, buffer, offset, bytestowrite);

		/* Update offset and count */

		offset += bytestowrite;
		count -= bytestowrite;
		index = bytestowrite;

		/* Write full pages */

		while (count >= pagesize) {
			imxrt_bytewrite(priv, &buffer[index], offset, pagesize);

			/* Update offset and count */

			offset += pagesize;
			count -= pagesize;
			index += pagesize;
		}

		/* Now write any partial page at the end */

		if (count > 0) {
			imxrt_bytewrite(priv, &buffer[index], offset, count);
		}
	}

	return nbytes;
}
#endif							/* CONFIG_MTD_BYTE_WRITE */

/************************************************************************************
 * Name: imxrt_ioctl
 ************************************************************************************/

static int imxrt_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg)
{
	FAR struct imxrt_dev_s *priv = (FAR struct imxrt_dev_s *)dev;
	int ret = -EINVAL;			/* Assume good command with bad parameters */

	fvdbg("cmd: %d \n", cmd);

	switch (cmd) {
	case MTDIOC_GEOMETRY: {
		FAR struct mtd_geometry_s *geo = (FAR struct mtd_geometry_s *)((uintptr_t)arg);
		if (geo) {
			/* Populate the geometry structure with information need to know
			 * the capacity and how to access the device.
			 *
			 * NOTE: that the device is treated as though it where just an array
			 * of fixed size blocks.  That is most likely not true, but the client
			 * will expect the device logic to do whatever is necessary to make it
			 * appear so.
			 */

			geo->blocksize = (1 << priv->pageshift);
#ifdef CONFIG_IMXRT_SUBSECTOR_ERASE
			if (priv->subsectorshift > 0) {
				geo->erasesize = (1 << priv->subsectorshift);
				geo->neraseblocks = priv->nsectors * (1 << (priv->sectorshift - priv->subsectorshift));
			} else
#endif
			{
				geo->erasesize = (1 << priv->sectorshift);
				geo->neraseblocks = priv->npages;
			}

			ret = OK;

			fvdbg("blocksize: %d erasesize: %d neraseblocks: %d\n", geo->blocksize, geo->erasesize, geo->neraseblocks);
		}
	}
	break;

	case MTDIOC_BULKERASE: {
		/* Erase the entire device */
		fdbg("[pid %d]: checkpoint line %d, arg = %d\n", getpid(), __LINE__, arg);
		ret = imxrt_bulkerase(priv, arg);
	}
	break;

	case MTDIOC_XIPBASE:
	default:
		ret = -ENOTTY;			/* Bad command */
		break;
	}

	fvdbg("return %d\n", ret);
	return ret;
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: imxrt_initialize
 *
 * Description:
 *   Create an initialize MTD device instance.  MTD devices are not registered
 *   in the file system, but are created as instances that can be bound to
 *   other functions (such as a block or character driver front end).
 *
 ************************************************************************************/

FAR struct mtd_dev_s *imxrt_mtd_initialize(void)
{
	FAR struct imxrt_dev_s *priv;
	int ret;

	/* Allocate a state structure (we allocate the structure instead of using
	 * a fixed, static allocation so that we can handle multiple FLASH devices.
	 * The current implementation would handle only one FLASH part per SPI
	 * device (only because of the SPIDEV_FLASH definition) and so would have
	 * to be extended to handle multiple FLASH parts on the same SPI bus.
	 */

	priv = (FAR struct imxrt_dev_s *)kmm_zalloc(sizeof(struct imxrt_dev_s));
	if (priv) {
		/* Initialize the allocated structure. (unsupported methods were
		 * nullified by kmm_zalloc).
		 */

		priv->mtd.erase = imxrt_erase;
		priv->mtd.bread = imxrt_bread;
		priv->mtd.bwrite = imxrt_bwrite;
		priv->mtd.read = imxrt_read;
#ifdef CONFIG_MTD_BYTE_WRITE
		priv->mtd.write = imxrt_write;
#endif
		priv->mtd.ioctl = imxrt_ioctl;

		/* Identify the FLASH chip and get its capacity */

		ret = imxrt_readid(priv);
		if (ret != OK) {
			/* Unrecognized! Discard all of that work we just did and return NULL */

			fdbg("Unrecognized\n");
			kmm_free(priv);
			priv = NULL;
		}
	}

	/* Return the implementation-specific state structure as the MTD device */

	fvdbg("Return %p\n", priv);
	return (FAR struct mtd_dev_s *)priv;
}
