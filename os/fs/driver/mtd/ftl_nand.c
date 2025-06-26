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
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 *
 *   Copyright (C) 2009, 2011-2012, 2016 Gregory Nutt. All rights reserved.
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
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/mtd.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct ftl_nand_s {
	FAR struct mtd_dev_s *mtd;     /* Contained MTD interface */
	struct mtd_geometry_s geo;     /* Device geometry */
	uint16_t              blkper;  /* R/W blocks per erase block */
#ifdef CONFIG_FS_WRITABLE
	FAR uint8_t          *eblock;  /* One, in-memory erase block */
#endif
	int                  *block_map; /* Logical to physical mapping of blocks by skipping bad blocks */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int     ftl_open(FAR struct inode *inode);
static int     ftl_close(FAR struct inode *inode);
static ssize_t ftl_reload(FAR void *priv, FAR uint8_t *buffer, off_t startblock, size_t nblocks);
static ssize_t ftl_read(FAR struct inode *inode, unsigned char *buffer, size_t start_sector, unsigned int nsectors);
#ifdef CONFIG_FS_WRITABLE
static ssize_t ftl_flush(FAR void *priv, FAR const uint8_t *buffer, off_t startblock, size_t nblocks);
static ssize_t ftl_write(FAR struct inode *inode, const unsigned char *buffer, size_t start_sector, unsigned int nsectors);
#endif
static int     ftl_geometry(FAR struct inode *inode, struct geometry *geometry);
static int     ftl_ioctl(FAR struct inode *inode, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct block_operations g_bops = {
	ftl_open,     /* open     */
	ftl_close,    /* close    */
	ftl_read,     /* read     */
#ifdef CONFIG_FS_WRITABLE
	ftl_write,    /* write    */
#else
	NULL,         /* write    */
#endif
	ftl_geometry, /* geometry */
	ftl_ioctl     /* ioctl    */
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
	, 0           /* unlink   */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ftl_open
 *
 * Description: Open the block device
 *
 ****************************************************************************/

static int ftl_open(FAR struct inode *inode)
{
	fvdbg("Entry\n");
	return OK;
}

/****************************************************************************
 * Name: ftl_close
 *
 * Description: close the block device
 *
 ****************************************************************************/

static int ftl_close(FAR struct inode *inode)
{
	fvdbg("Entry\n");
	return OK;
}

/****************************************************************************
 * Name: ftl_reload
 *
 * Description:  Read the specified number of sectors
 *
 ****************************************************************************/

static ssize_t ftl_reload(FAR void *priv, FAR uint8_t *buffer, off_t startblock, size_t nblocks)
{
	struct ftl_nand_s *dev = (struct ftl_nand_s *)priv;
	ssize_t nread = 0;

	/* loop over the blocks and map them to the physical block */

	for (int i = 0; i < nblocks; i++) {
		off_t curr_block = startblock + i;
		off_t erase_block = curr_block / dev->blkper;
		off_t offset = curr_block % dev->blkper;

		int physical_eblock = dev->block_map[erase_block];
		if (physical_eblock < 0) {
			return -ENXIO;
		}

		ssize_t read = MTD_BREAD(dev->mtd, physical_eblock * dev->blkper + offset, 1, buffer);

		if (read != 1) {
			dbg("ERROR: Read block %d failed: %d\n", curr_block, read);
			return read;
		}

		buffer += dev->geo.blocksize;
		nread++;
	}

	return nread;
}

/****************************************************************************
 * Name: ftl_read
 *
 * Description:  Read the specified number of sectors
 *
 ****************************************************************************/

static ssize_t ftl_read(FAR struct inode *inode, unsigned char *buffer,
						size_t start_sector, unsigned int nsectors)
{
	FAR struct ftl_nand_s *dev;

	fvdbg("sector: %d nsectors: %d\n", start_sector, nsectors);

	DEBUGASSERT(inode && inode->i_private);

	dev = (FAR struct ftl_nand_s *)inode->i_private;
	return ftl_reload(dev, buffer, start_sector, nsectors);
}

/****************************************************************************
 * Name: ftl_flush
 *
 * Description: Write the specified number of sectors
 *
 ****************************************************************************/

#ifdef CONFIG_FS_WRITABLE
static ssize_t ftl_flush(FAR void *priv, FAR const uint8_t *buffer, off_t startblock, size_t nblocks)
{
	struct ftl_nand_s *dev = (struct ftl_nand_s *)priv;
	off_t  alignedblock;
	off_t  mask;
	off_t  rwblock;
	off_t  eraseblock;
	off_t  offset;
	size_t remaining;
	size_t nxfrd;
	int    nbytes;
	int    ret;
	int    physical_eblock;

	/* Get the aligned block.  Here is is assumed: (1) The number of R/W blocks
	 * per erase block is a power of 2, and (2) the erase begins with that same
	 * alignment.
	 */

	mask         = dev->blkper - 1;
	alignedblock = (startblock + mask) & ~mask;

	/* Handle partial erase blocks before the first unaligned block */

	remaining = nblocks;
	if (alignedblock > startblock) {
		/* Check if the write is shorter than to the end of the erase block */

		bool short_write = (remaining < (alignedblock - startblock));

		/* Read the full erase block into the buffer */

		rwblock = startblock & ~mask;

		/* Get the logical block */

		physical_eblock = dev->block_map[ rwblock / dev->blkper];
		
		if (physical_eblock < 0) {
			return -ENXIO;
		}

		nxfrd   = MTD_BREAD(dev->mtd, physical_eblock * dev->blkper, dev->blkper, dev->eblock);
		if (nxfrd != dev->blkper) {
			dbg("ERROR: Read erase block %d failed: %d\n", rwblock, nxfrd);
			return -EIO;
		}

		/* Then erase the erase block */

		ret        = MTD_ERASE(dev->mtd, physical_eblock, 1);
		if (ret < 0) {
			dbg("ERROR: Erase block=%d failed: %d\n", eraseblock, ret);
			return ret;
		}

		/* Copy the user data at the end of the buffered erase block */

		offset = (startblock & mask) * dev->geo.blocksize;

		if (short_write) {
			nbytes = remaining * dev->geo.blocksize;
		} else {
			nbytes = dev->geo.erasesize - offset;
		}

		fvdbg("Copy %d bytes into erase block=%d at offset=%d\n", nbytes, eraseblock, offset);

		memcpy(dev->eblock + offset, buffer, nbytes);

		/* And write the erase block back to flash */

		nxfrd = MTD_BWRITE(dev->mtd, physical_eblock * dev->blkper, dev->blkper, dev->eblock);
		if (nxfrd != dev->blkper) {
			dbg("ERROR: Write erase block %d failed: %d\n", rwblock, nxfrd);
			return -EIO;
		}

		/* Then update for amount written */

		if (short_write) {
			remaining = 0;
		} else {
			remaining -= dev->blkper - (startblock & mask);
		}

		buffer += nbytes;
	}

	/* How handle full erase pages in the middle */

	while (remaining >= dev->blkper) {
		/* Erase the erase block */

		eraseblock = alignedblock / dev->blkper;

		physical_eblock = dev->block_map[eraseblock];

		if (physical_eblock < 0) {
			return -ENXIO;
		}

		ret = MTD_ERASE(dev->mtd, physical_eblock, 1);
		if (ret < 0) {
			dbg("ERROR: Erase block=%d failed: %d\n", eraseblock, ret);
			return ret;
		}

		/* Write a full erase back to flash */

		fvdbg("Write %d bytes into erase block=%d at offset=0\n",
			  dev->geo.erasesize, alignedblock);

		nxfrd = MTD_BWRITE(dev->mtd, physical_eblock * dev->blkper, dev->blkper, buffer);
		if (nxfrd != dev->blkper) {
			dbg("ERROR: Write erase block %d failed: %d\n", alignedblock, nxfrd);
			return -EIO;
		}

		/* Then update for amount written */

		alignedblock += dev->blkper;
		remaining    -= dev->blkper;
		buffer       += dev->geo.erasesize;
	}

	/* Finally, handle any partial blocks after the last full erase block */

	if (remaining > 0) {
		/* Read the full erase block into the buffer */

		eraseblock = alignedblock / dev->blkper;

		physical_eblock = dev->block_map[eraseblock];

                if (physical_eblock < 0) {
                        return -ENXIO;
                }

		nxfrd = MTD_BREAD(dev->mtd, physical_eblock * dev->blkper, dev->blkper, dev->eblock);
		if (nxfrd != dev->blkper) {
			dbg("ERROR: Read erase block %d failed: %d\n", alignedblock, nxfrd);
			return -EIO;
		}

		/* Then erase the erase block */

		ret = MTD_ERASE(dev->mtd, physical_eblock, 1);
		if (ret < 0) {
			dbg("ERROR: Erase block=%d failed: %d\n", eraseblock, ret);
			return ret;
		}

		/* Copy the user data at the beginning the buffered erase block */

		nbytes = remaining * dev->geo.blocksize;
		fvdbg("Copy %d bytes into erase block=%d at offset=0\n", nbytes, alignedblock);
		memcpy(dev->eblock, buffer, nbytes);

		/* And write the erase back to flash */

		nxfrd = MTD_BWRITE(dev->mtd, physical_eblock * dev->blkper, dev->blkper, dev->eblock);
		if (nxfrd != dev->blkper) {
			dbg("ERROR: Write erase block %d failed: %d\n", alignedblock, nxfrd);
			return -EIO;
		}
	}

	return nblocks;
}
#endif

/****************************************************************************
 * Name: ftl_write
 *
 * Description: Write (or buffer) the specified number of sectors
 *
 ****************************************************************************/

#ifdef CONFIG_FS_WRITABLE
static ssize_t ftl_write(FAR struct inode *inode, const unsigned char *buffer, size_t start_sector, unsigned int nsectors)
{
	struct ftl_nand_s *dev;

	fvdbg("sector: %d nsectors: %d\n", start_sector, nsectors);

	DEBUGASSERT(inode && inode->i_private);
	dev = (struct ftl_nand_s *)inode->i_private;
	return ftl_flush(dev, buffer, start_sector, nsectors);
}
#endif

/****************************************************************************
 * Name: ftl_geometry
 *
 * Description: Return device geometry
 *
 ****************************************************************************/

static int ftl_geometry(FAR struct inode *inode, struct geometry *geometry)
{
	struct ftl_nand_s *dev;

	fvdbg("Entry\n");

	DEBUGASSERT(inode);
	if (geometry) {
		dev = (struct ftl_nand_s *)inode->i_private;
		geometry->geo_available     = true;
		geometry->geo_mediachanged  = false;
#ifdef CONFIG_FS_WRITABLE
		geometry->geo_writeenabled  = true;
#else
		geometry->geo_writeenabled  = false;
#endif
		geometry->geo_nsectors      = dev->geo.neraseblocks * dev->blkper;
		geometry->geo_sectorsize    = dev->geo.blocksize;

		fvdbg("available: true mediachanged: false writeenabled: %s\n",
			  geometry->geo_writeenabled ? "true" : "false");
		fvdbg("nsectors: %d sectorsize: %d\n",
			  geometry->geo_nsectors, geometry->geo_sectorsize);

		return OK;
	}

	return -EINVAL;
}

/****************************************************************************
 * Name: ftl_ioctl
 *
 * Description: Return device geometry
 *
 ****************************************************************************/

static int ftl_ioctl(FAR struct inode *inode, int cmd, unsigned long arg)
{
	struct ftl_nand_s *dev ;
	int ret;

	fvdbg("Entry\n");
	DEBUGASSERT(inode && inode->i_private);

	/* Only one block driver ioctl command is supported by this driver (and
	 * that command is just passed on to the MTD driver in a slightly
	 * different form).
	 */

	if (cmd == BIOC_XIPBASE) {
		/* The argument accompanying the BIOC_XIPBASE should be non-NULL.  If
		 * DEBUG is enabled, we will catch it here instead of in the MTD
		 * driver.
		 */

		if (arg == 0) {
			dbg("ERROR: BIOC_XIPBASE argument is NULL\n");
			return -EINVAL;
		}

		/* Just change the BIOC_XIPBASE command to the MTDIOC_XIPBASE command. */

		cmd = MTDIOC_XIPBASE;
	}

	/* No other block driver ioctl commmands are not recognized by this
	 * driver.  Other possible MTD driver ioctl commands are passed through
	 * to the MTD driver (unchanged).
	 */

	dev = (struct ftl_nand_s *)inode->i_private;
	ret = MTD_IOCTL(dev->mtd, cmd, arg);
	if (ret < 0) {
		dbg("ERROR: MTD ioctl(%04x) failed: %d\n", cmd, ret);
	}

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ftl_initialize
 *
 * Description:
 *   Initialize to provide a block driver wrapper around an MTD interface
 *
 * Input Parameters:
 *   minor - The minor device number.  The MTD block device will be
 *      registered as as /dev/mtdblockN where N is the minor number.
 *   mtd - The MTD device that supports the FLASH interface.
 *
 ****************************************************************************/

int ftl_nand_initialize(int minor, FAR struct mtd_dev_s *mtd)
{
	struct ftl_nand_s *dev;
	char devname[16];
	int ret = -ENOMEM;

	/* Sanity check */

	if (minor < 0 || minor > 255 || !mtd) {
		return -EINVAL;
	}

	/* Allocate a FTL device structure */

	dev = (struct ftl_nand_s *)kmm_malloc(sizeof(struct ftl_nand_s));
	if (dev) {
		/* Initialize the FTL device structure */

		dev->mtd = mtd;

		/* Get the device geometry. (casting to uintptr_t first eliminates
		 * complaints on some architectures where the sizeof long is different
		 * from the size of a pointer).
		 */

		ret = MTD_IOCTL(mtd, MTDIOC_GEOMETRY, (unsigned long)((uintptr_t)&dev->geo));
		if (ret < 0) {
			dbg("ERROR: MTD ioctl(MTDIOC_GEOMETRY) failed: %d\n", ret);
			kmm_free(dev);
			return ret;
		}

		/* Fill the block mapping */
		
		dev->block_map = (int *)kmm_malloc(sizeof(int) * dev->geo.neraseblocks);
		if (!dev->block_map) {
			dbg("ERROR: Failed to allocate logical mapping of blocks\n");
			kmm_free(dev);
			return -ENOMEM;
		}

		int i = 0;
		int logical_block = 0;

		for (i = 0; i < dev->geo.neraseblocks; i++) {
			if (!MTD_ISBAD(mtd, i)) {
				dev->block_map[logical_block] = i;
				logical_block++;
			}
		}

		for (i = logical_block; i < dev->geo.neraseblocks; i++) {
			dev->block_map[i] = -1;
		}

		/* Allocate one, in-memory erase block buffer */

#ifdef CONFIG_FS_WRITABLE
		dev->eblock  = (FAR uint8_t *)kmm_malloc(dev->geo.erasesize);
		if (!dev->eblock) {
			dbg("ERROR: Failed to allocate an erase block buffer\n");
			kmm_free(dev);
			return -ENOMEM;
		}
#endif

		/* Get the number of R/W blocks per erase block */

		dev->blkper = dev->geo.erasesize / dev->geo.blocksize;
		DEBUGASSERT(dev->blkper * dev->geo.blocksize == dev->geo.erasesize);

		/* Create a MTD block device name */

		snprintf(devname, 16, "/dev/mtdblock%d", minor);

		/* Inode private data is a reference to the FTL device structure */

		ret = register_blockdriver(devname, &g_bops, 0, dev);
		if (ret < 0) {
			dbg("ERROR: register_blockdriver failed: %d\n", -ret);
			kmm_free(dev);
		}
	}

	return ret;
}
