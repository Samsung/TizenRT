/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * SmartFUSE - A FUSE filesystem for mounting NuttX FS natively under Linux.
 *
 * tools/nxfuse/tinyara_services.c:
 *
 *   Contains wrapper functions and virtual mount services to emulate
 *   to emulate NuttX operations.
 *
 *   Copyright (C) 2016 Ken Pettit. All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/config.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/mtd.h>
#include <tinyara/fs/smart.h>
#include <tinyara/fs/ioctl.h>
#include <limits.h>

#include "nxfuse.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct fs_ops_s {
	const char *fs_name;
	void *(*vmount)(const char *datasource, const char *mount_point, int erasesize, int sectsize, int pagesize, char *generic);
	int (*mkfs)(const char *datasource, uint32_t erasesize, uint16_t sectsize, int pagesize, char *, int confirm);
	const struct mountpt_operations *pops;
};

/****************************************************************************
 * Extern referenced data
 ****************************************************************************/

#ifdef CONFIG_FS_SMARTFS
extern const struct mountpt_operations smartfs_operations;
#endif
extern void filemtd_teardown(FAR struct mtd_dev_s *dev);
extern FAR struct mtd_dev_s *filemtd_initialize(FAR const char *path, size_t offset, int16_t sectsize, int32_t erasesize);

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

#ifdef CONFIG_FS_SMARTFS
static void *smartfs_vmount(const char *datasource, const char *mount_point, uint32_t erasesize, uint16_t sectsize, int pagesize, char *generic);
static int smartfs_mkfs(const char *datasource, uint32_t erasesize, uint16_t sectsize, int pagesize, char *generic, int confirm);
#endif

/****************************************************************************
 * Private Variables
 ****************************************************************************/

static struct inode *g_inodes[10];
static int g_inode_count = 0;

/* Table of known NuttX FS types we can mount */

static struct fs_ops_s g_fs_ops[] = {
#ifdef CONFIG_FS_SMARTFS
	{"smartfs", smartfs_vmount, smartfs_mkfs, &smartfs_operations},
#endif

	{"", NULL, NULL}
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: get_errno
 ****************************************************************************/

int get_errno(void)
{
	return errno;
}

/****************************************************************************
 * Name: mtd_register
 *
 *  Dummy stub function
 *
 ****************************************************************************/

int mtd_register(FAR struct mtd_dev_s *mtd, FAR const char *name)
{
	return 0;
}

/****************************************************************************
 * Name: mtd_unregister
 *
 *  Dummy stub function
 *
 ****************************************************************************/

int mtd_unregister(FAR struct mtd_dev_s *mtd)
{
	return 0;
}

/****************************************************************************
 * Name: register_driver
 *
 *  Dummy stub function
 *
 ****************************************************************************/

int register_driver(FAR const char *path, FAR const struct file_operations *fops, mode_t mode, FAR void *priv)
{
	return 0;
}

/****************************************************************************
 * Name: register_blockdriver
 *
 *  Simple registration support
 *
 ****************************************************************************/

int register_blockdriver(FAR const char *path, FAR const struct block_operations *bops, mode_t mode, FAR void *priv)
{
	struct inode *node;
	int ret;

	node = calloc(sizeof(struct inode) + PATH_MAX, 1);
	if (node == NULL) {
		return -1;
	}

	strncpy(node->i_name, path, PATH_MAX);
	node->i_crefs = 0;
	node->i_peer = NULL;
	node->i_child = NULL;
	node->u.i_bops = bops;
#ifdef CONFIG_FILE_MODE
	node->i_mode = mode;
#endif
	node->i_private = priv;
	ret = OK;

	g_inodes[g_inode_count++] = node;

	return ret;
}

/****************************************************************************
 * Name: open_blockdriver
 *
 *  Simple find / open support for block drivers
 *
 ****************************************************************************/

int open_blockdriver(FAR const char *pathname, int mountflags, FAR struct inode **ppinode)
{
	int x;

	for (x = 0; x < g_inode_count; x++) {
		if (strcmp(pathname, g_inodes[x]->i_name) == 0) {
			*ppinode = g_inodes[x];
			g_inodes[x]->i_crefs++;
			return 0;
		}
	}

	*ppinode = NULL;
	return -1;
}

/****************************************************************************
 * Name: close_blockdriver
 *
 *  Simple close support for block drivers
 *
 ****************************************************************************/

int close_blockdriver(FAR struct inode *pinode)
{
	pinode->i_crefs--;
	return OK;
}

/****************************************************************************
 * Name: unregister_blockdriver
 *
 *  Simple unregister support for block drivers
 *
 ****************************************************************************/

int unregister_blockdriver(FAR const char *pathname)
{
	int x;
	struct inode *pinode;

	for (x = 0; x < g_inode_count; x++) {
		if (strcmp(pathname, g_inodes[x]->i_name) == 0) {
			pinode = g_inodes[x];
			g_inodes[x] = g_inodes[--g_inode_count];
			free(pinode);
		}
	}

	return OK;
}

/****************************************************************************
 * Name: dbg
 *
 *  A dbg implementation that maps to printf
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG
int dbg(const char *fmt, ...)
{
	va_list args;
	int ret;

	va_start(args, fmt);
	ret = vprintf(fmt, args);
	va_end(args);
	return ret;
}
#endif

/****************************************************************************
 * Name: vdbg
 *
 *  A dbg implementation that maps to printf
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_VERBOSE
int vdbg(const char *fmt, ...)
{
	va_list args;
	int ret;

	va_start(args, fmt);
	ret = vprintf(fmt, args);
	va_end(args);
	return ret;
}
#endif

/****************************************************************************
 * Name: vmount
 *
 *  A virtual mount implemention for registered NuttX filesystems.  Mount
 *  is performed based on the provided fs_type.
 *
 ****************************************************************************/

struct inode *vmount(const char *datasource, const char *mount_point, const char *fs_type, int erasesize, int sectsize, int pagesize, char *generic)
{
	int x;
	void *fs_handle;
	struct inode *pinode;

	/* Loop for all known NuttX FS types */

	for (x = 0; g_fs_ops[x].vmount != NULL; x++) {
		/* Test if this is the FS we need to mount */

		if (strcmp(fs_type, g_fs_ops[x].fs_name) == 0) {
			/* Mount this FS */

			fs_handle = g_fs_ops[x].vmount(datasource, mount_point, erasesize, sectsize, pagesize, generic);

			/* Test if mount failed */

			if (fs_handle == NULL) {
				return NULL;
			}

			/* Setup a new inode */

			pinode = calloc(sizeof(struct inode), 1);
			pinode->i_crefs = 1;
			pinode->i_private = fs_handle;
			pinode->u.i_mops = g_fs_ops[x].pops;

			return pinode;
		}
	}

	/* Unknown fs_type */

	dbg("Unknown filesystem type: %s\n", fs_type);

	return NULL;
}

/****************************************************************************
 * Name: smartfs_vmount
 *
 *  The mount implementation for SmartFS type FS.
 *
 *  This creates a file based MTD device using the given datasource and
 *  initialized smart MTD and SmartFS on that filemtd device.
 *
 ****************************************************************************/

#ifdef CONFIG_FS_SMARTFS
void *smartfs_vmount(const char *datasource, const char *mount_point, uint32_t erasesize, uint16_t sectsize, int pagesize, char *generic)
{
	int ret;
	int offset = 0;
	struct mtd_dev_s *mtd;
	void *fshandle;
	struct inode *blkdriver;
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	char blkname[20];
	int nrootdirs;
#endif

	/* Try to create a filemtd device using the filename provided */

	mtd = filemtd_initialize(datasource, offset, pagesize, erasesize);
	if (mtd == NULL) {
		printf("error %d opening %s\n", errno, datasource);
		return NULL;
	}

	/* Now initialize the SMART routines on the MTD */

	ret = smart_initialize(0, mtd, NULL);
	if (ret != OK) {
		printf("error initializing SmartFS on %s\n", datasource);
		filemtd_teardown(mtd);
		return NULL;
	}

	/* Setup the SmartFS */

#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS

	/* The generic parameter is the directory number to mount */

	nrootdirs = atoi(generic);
	if (nrootdirs == 0) {
		/* If the root directory number is not specified,
		 * then default to 1.
		 */

		nrootdirs++;
	}

	snprintf(blkname, sizeof(blkname), "/dev/smart0d%d", nrootdirs);
	ret = open_blockdriver(blkname, 0, &blkdriver);
#else
	ret = open_blockdriver("/dev/smart0", 0, &blkdriver);
#endif
	ret = smartfs_operations.bind(blkdriver, NULL, &fshandle);

	if (ret != OK) {
		return NULL;
	}

	return fshandle;
}
#endif							/* CONFIG_FS_SMARTFS */

/****************************************************************************
 * Name: mkfs
 *
 *  A virtual mount implemention for registered NuttX filesystems.  Mount
 *  is performed based on the provided fs_type.
 *
 ****************************************************************************/

int mkfs(const char *datasource, const char *fs_type, uint32_t erasesize, uint16_t sectsize, int pagesize, char *generic, int confirm)
{
	int x;
	int ret = -ENODEV;

	/* Loop for all known NuttX FS types */

	for (x = 0; g_fs_ops[x].vmount != NULL; x++) {
		/* Test if this is the FS we need to mount */

		if (strcmp(fs_type, g_fs_ops[x].fs_name) == 0) {
			/* Call this FS's mkfs routine */

			if (g_fs_ops[x].mkfs != NULL) {
				ret = g_fs_ops[x].mkfs(datasource, erasesize, sectsize, pagesize, generic, confirm);
			}

			return ret;
		}
	}

	/* Unknown fs_type */

	dbg("Unknown filesystem type: %s\n", fs_type);

	return ret;
}

/****************************************************************************
 * Name: smartfs_umount
 *
 *  Unmounts a previously mounted SmartFS
 *
 ****************************************************************************/

#ifdef CONFIG_FS_SMARTFS
static int smartfs_umount(struct inode *blkdriver, void *fshandle)
{
	int ret;
	struct mtd_dev_s *mtd;

	/* Get the block driver */

#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	ret = open_blockdriver("/dev/smart0d1", 0, &blkdriver);
#else
	ret = open_blockdriver("/dev/smart0", 0, &blkdriver);
#endif
	free(fshandle);

	/* The blkdriver private data is an MTD pointer */

	mtd = *((struct mtd_dev_s **)blkdriver->i_private);
	filemtd_teardown(mtd);

	free(blkdriver->i_private);
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	unregister_blockdriver("/dev/smart0d1");
#else
	unregister_blockdriver("/dev/smart0");
#endif

	return ret;
}

/****************************************************************************
 * Name: smartfs_mkfs
 *
 *  Creates a SmartfS filesystem on the datasource
 *
 ****************************************************************************/

static int smartfs_mkfs(const char *datasource, uint32_t erasesize, uint16_t sectsize, int pagesize, char *generic, int confirm)
{
	int ret;
	int x;
	void *fshandle;
	struct inode *blkdriver;
	uint8_t type;
	struct smart_read_write_s request;

	/* Try to mount the device to see if there is a format already */

	fshandle = smartfs_vmount(datasource, "/tmp", erasesize, sectsize, pagesize, "");
	if (fshandle != NULL) {
		/* Test if confirm was specified */

		if (!confirm) {
			printf("\nSheepishly refusing to reformat ... use -c to confirm\n\n");

			/* Unmount the datasource */

			smartfs_umount(blkdriver, fshandle);

			return -EACCES;
		}
	}

	/* Okay to format the device.  Get the block driver inode */

	if (fshandle == NULL) {
		printf("\nFormatting %s with smartfs format\n", datasource);
	} else {
		printf("\nReformatting %s with smartfs format\n", datasource);
	}
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	ret = open_blockdriver("/dev/smart0d1", 0, &blkdriver);
#else
	ret = open_blockdriver("/dev/smart0", 0, &blkdriver);
#endif

	/* Perform a low-level SMART format */

#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	ret = blkdriver->u.i_bops->ioctl(blkdriver, BIOC_LLFORMAT, ((uint64_t)sectsize << 16) | atoi(generic));
#else
	ret = blkdriver->u.i_bops->ioctl(blkdriver, BIOC_LLFORMAT, ((uint64_t)sectsize << 16));
#endif
	if (ret != OK) {
		close_blockdriver(blkdriver);
		printf("Error %d during low-level format\n", ret);
		return ret;
	}

	/* Unmount the datasource */

	smartfs_umount(blkdriver, fshandle);
	fshandle = smartfs_vmount(datasource, "/tmp", erasesize, sectsize, pagesize, "");
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	ret = open_blockdriver("/dev/smart0d1", 0, &blkdriver);
#else
	ret = open_blockdriver("/dev/smart0", 0, &blkdriver);
#endif

	/* Now Write the filesystem to media.  Loop for each root dir entry and
	 * allocate the reserved Root Dir Enty, then write a blank root dir for it.
	 */

	type = SMARTFS_SECTOR_TYPE_DIR;
	request.offset = 0;
	request.count = 1;
	request.buffer = &type;
	x = 0;
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	for (; x < atoi(generic); x++)
#endif
	{
		ret = blkdriver->u.i_bops->ioctl(blkdriver, BIOC_ALLOCSECT, SMARTFS_ROOT_DIR_SECTOR + x);
		if (ret != SMARTFS_ROOT_DIR_SECTOR + x) {
			ret = -EIO;
			printf("Error %d allocating sector\n", ret);
			return ret;
		}

		/* Mark this block as a directory entry */

		request.logsector = SMARTFS_ROOT_DIR_SECTOR + x;

		/* Issue a write to the sector, single byte */

		ret = blkdriver->u.i_bops->ioctl(blkdriver, BIOC_WRITESECT, (unsigned long)&request);
		if (ret != 0) {
			ret = -EIO;
			printf("Error %d writing rootdir sector\n", ret);
			return ret;
		}
	}

	printf("Format successful\n");

	return ret;
}
#endif							/* CONFIG_FS_SMARTFS */
