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
 * fs/fs_utils.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/smart.h>
#include <tinyara/fs/mtd.h>
#include <tinyara/fs/mksmartfs.h>
#include <sys/mount.h>
#include "smartfs/smartfs.h"
#include "inode/inode.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#ifdef CONFIG_MTD
FAR static struct mtd_dev_s *mtd = NULL;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
FAR struct mtd_dev_s *get_fsmtd(void)
{
	struct mtd_dev_s *mtd_master = NULL;
	mtd_master = up_flashinitialize();
	if (!mtd_master) {
		return NULL;
	}
#ifdef CONFIG_MTD_PARTITION
	mtd = get_mtd_partition(MTD_FS);
#else
	mtd = mtd_master;
#endif
	return mtd;
}

#endif							/* END OF CONFIG_MTD */

/****************************************************************************
 * This is called from the User application to initiate the file system
 ****************************************************************************/
int fs_initiate(char *dev_name, char *mount_name)
{
	int ret;
	ret = ERROR;
	if (dev_name == NULL || mount_name == NULL) {
		goto error_out;
	}
#ifdef CONFIG_MTD
	if (mtd != NULL) {
		fdbg("mtd is not available, return mtd\n");
		return OK;
	}
	mtd = get_fsmtd();
	if (!mtd) {
		fdbg("ERROR: Failed to create RAM MTD instance\n");
		goto error_out;
	}
#ifdef CONFIG_MTD_SMART
	ret = smart_initialize(1, mtd, NULL);
	if (ret != OK) {
		fdbg("ERROR: SMART initialization failed: %d\n", -ret);
		goto error_out;
	}
#endif
#endif
#ifdef CONFIG_FS_SMARTFS
	ret = mksmartfs(dev_name, false);
	if (ret != OK) {
		fdbg("ERROR: MKSMARTFS failed: %d\n", -ret);
		goto error_out;
	}

	ret = mount(dev_name, CONFIG_MOUNT_POINT, mount_name, 0, NULL);
	if (ret != OK) {
		fdbg("ERROR: Mount failed: %d\n", -ret);
	}
#endif
error_out:
	return ret > 0 ? -ret : ret;
}

/****************************************************************************
 * This is called from the User applications to clean the file system
 ****************************************************************************/
int fs_clean(char *dev_name)
{
	struct inode *inode;
	int ret;
	ret = ERROR;
	if (dev_name == NULL) {
		goto error_out;
	}
#ifdef CONFIG_FS_SMARTFS
	ret = umount(CONFIG_MOUNT_POINT);
	if (ret != OK) {
		fdbg("umount failed : %d\n", ret);
		ret = -ret;
		goto error_out;
	}
#endif
	inode = inode_find(dev_name, NULL);
	if (inode == NULL) {
		return -ENOENT;
	}
#if defined(CONFIG_MTD) && defined(CONFIG_MTD_SMART)
	ret = smart_clean(inode);
	if (ret != OK) {
		fdbg("smart_clean failed : %d\n", ret);
		ret = -ret;
		goto error_out;
	}
	mtd = NULL;
#endif
	unregister_driver(dev_name);
	inode_release(inode);
	inode = NULL;

error_out:
	return ret > 0 ? -ret : ret;
}

/****************************************************************************
 * This is called from the User applications to erase flash & clean file
 * system
 ****************************************************************************/
int fs_erase(char *dev_name)
{
	int ret;
	ret = ERROR;
	if (dev_name == NULL) {
		goto error_out;
	}
	ret = fs_clean(dev_name);
	if (ret == -EBUSY) {
		fdbg("Error in cleaning flash, There are open files\n");
		goto error_out;
	}
#ifdef CONFIG_MTD
	if (mtd == NULL) {
		mtd = get_fsmtd();
	}
	if (mtd != NULL) {
		ret = MTD_IOCTL(mtd, MTDIOC_BULKERASE, 0xff);
		if (ret != OK) {
			fdbg("Error in erasing flash\n");
			goto error_out;
		}
	}
	mtd = NULL;
#endif
error_out:
	return ret > 0 ? -ret : ret;
}

/****************************************************************************
 * This is called from the User applications to recover lost sectors after a
 * power failure
 ****************************************************************************/
#if defined(CONFIG_FS_SMARTFS) && defined(CONFIG_SMARTFS_SECTOR_RECOVERY)
int fs_recover(void)
{
	int ret = -EINVAL;
	struct inode *inode;
	inode = inode_find(CONFIG_MOUNT_POINT, NULL);
	if (!inode) {
		fdbg("ERROR: Couldn't find inode: %d\n", -ret);
		ret = -EINVAL;
		goto error_out;
	}
#ifndef CONFIG_DISABLE_MOUNTPOINT
	if (INODE_IS_MOUNTPT(inode)) {
		ret = smartfs_recover(inode);
	} else {
		fdbg("inode isnt mount pt\n");
		ret = -EINVAL;
	}
#endif
error_out:
	inode_release(inode);
	return ret;
}
#endif
