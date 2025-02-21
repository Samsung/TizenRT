/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
 * fs/driver/mtd/little.c
 *
 * Sector Mapped Allocation for Little File System Flash block driver.
 *
 *   Copyright (C) 2013-2014 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
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
#include <stddef.h>
#include <string.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/mtd.h>

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int little_open(FAR struct inode *inode);
static int little_close(FAR struct inode *inode);
static ssize_t little_reload(struct mtd_dev_s *mtd, FAR uint8_t *buffer, off_t startblock, size_t nblocks);
static ssize_t little_read(FAR struct inode *inode, unsigned char *buffer, size_t start_sector, unsigned int nsectors);
#ifdef CONFIG_FS_WRITABLE
static ssize_t little_write(FAR struct inode *inode, const unsigned char *buffer, size_t start_sector, unsigned int nsectors);
#endif
static int little_geometry(FAR struct inode *inode, struct geometry *geometry);
static int little_ioctl(FAR struct inode *inode, int cmd, unsigned long arg);

int little_initialize(int minor, FAR struct mtd_dev_s *mtd, FAR const char *partname);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct block_operations g_bops = {
        little_open,                             /* open     */
        little_close,                            /* close    */
        little_read,                             /* read     */
#ifdef CONFIG_FS_WRITABLE
        little_write,                            /* write    */
#else
        NULL,                                    /* write    */
#endif
        little_geometry,                         /* geometry */
        little_ioctl                             /* ioctl    */
};

/****************************************************************************
 * Private variables
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int little_initialize(int minor, FAR struct mtd_dev_s *mtd, FAR const char *partname)
{
	int ret = -ENOMEM;
	char devname[18];

#ifdef CONFIG_DEBUG
	if (minor < 0 || minor > 255 || !mtd) {
		return -EINVAL;
	}
#endif
	if (partname != NULL) {
		snprintf(devname, 18, "/dev/little%d%s", minor, partname);
	} else {
		snprintf(devname, 18, "/dev/little%d", minor);
	}

	/* Inode private data is a reference to the SMART device structure. */

	ret = register_blockdriver(devname, &g_bops, 0, mtd);
	if (ret < 0) {
		fdbg("register_blockdriver failed: %d\n", -ret);
		return ret;
	}

	return OK;
}

static int little_open(FAR struct inode *inode)
{
	fvdbg("Entry\n");
	return OK;
}

static int little_close(FAR struct inode *inode)
{
	return OK;
}

static ssize_t little_reload(struct mtd_dev_s *mtd, FAR uint8_t *buffer, off_t startblock, size_t nblocks)
{
	fvdbg("Entry\n");
	return OK;
}

static ssize_t little_reload(struct mtd_dev_s *dev, FAR uint8_t *buffer, off_t startblock, size_t nblocks)
{

       ssize_t nread;
       ssize_t mtdBlocks, mtdStartBlock;
       uint16_t mtdBlksPerSector, size;
       struct mtd_geometry_s geo;
       int ret;

       size = 1024; // *TODO
       ret = MTD_IOCTL(dev, MTDIOC_GEOMETRY,(unsigned long)&geo);
       if (ret < 0) {
               fdbg("MTD ioctl(MTDIOC_GEOMETRY) failed: %d\n", ret);
               return ret;
        }
       mtdBlksPerSector = size / geo.blocksize;

       mtdBlocks = nblocks * mtdBlksPerSector;

       mtdStartBlock = startblock * mtdBlksPerSector;

       /* Read the full erase block into the buffer. */

        fvdbg("Read %d blocks starting at block %d\n", mtdBlocks, mtdStartBlock);
        nread = MTD_BREAD(dev, mtdStartBlock, mtdBlocks, buffer);
        if (nread != mtdBlocks) {
                fdbg("Read %d blocks starting at block %d failed: %d\n", nblocks, startblock, nread);
        }

	return nread / mtdBlksPerSector;
}

static ssize_t little_read(FAR struct inode *inode, unsigned char *buffer, size_t start_sector, unsigned int nsectors)
{
	fdbg("\n");
	//return little_reload((struct mtd_dev_s *)inode->i_private, buffer, start_sector, nsectors);
}

#ifdef CONFIG_FS_WRITABLE
static ssize_t little_write(FAR struct inode *inode, const unsigned char *buffer, size_t start_sector, unsigned int nsectors)
{
	fdbg("\n");
	return OK;
}
#endif

static int little_geometry(FAR struct inode *inode, struct geometry *geometry)
{
	fdbg("\n");
	return OK;
	/*FAR struct mtd_dev_s *mtd;
        uint32_t erasesize;
        uint16_t sectorsize;
        struct mtd_geometry_s geo;
        int ret;
        fvdbg("Entry\n");
        DEBUGASSERT(inode);
        sectorsize = 1024; // *TODO
        if (geometry) {
                mtd = (FAR struct mtd_dev_s *)inode->i_private;
                geometry->geo_available = true;
                geometry->geo_mediachanged = false;
#ifdef CONFIG_FS_WRITABLE
                geometry->geo_writeenabled = true;
#else
                geometry->geo_writeenabled = false;
#endif
                ret = MTD_IOCTL(mtd, MTDIOC_GEOMETRY,(unsigned long)&geo);
                if (ret < 0) {
                        fdbg("MTD ioctl(MTDIOC_GEOMETRY) failed: %d\n", ret);
                        return ret;
                }
                erasesize = geo.erasesize;
                geometry->geo_nsectors = geo.neraseblocks * erasesize / sectorsize;
                geometry->geo_sectorsize = sectorsize;

                fvdbg("available: true mediachanged: false writeenabled: %s\n", geometry->geo_writeenabled ? "true" : "false");
                fvdbg("nsectors: %d sectorsize: %d\n", geometry->geo_nsectors, geometry->geo_sectorsize);
                return OK;
        }
        return -EINVAL;
	*/
}

static int little_ioctl(FAR struct inode *inode, int cmd, unsigned long arg)
{
	fdbg("\n");
	return OK;
}

