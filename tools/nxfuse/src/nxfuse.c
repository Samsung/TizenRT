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
 * nxfuse - A FUSE filesystem for mounting NuttX FS natively under Linux.
 *
 * tools/nxfuse/nxfuse.c
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
#include <unistd.h>
#include <errno.h>
#include <debug.h>
#include <utime.h>
#include <sys/statfs.h>

#define FUSE_USE_VERSION 26

#include <fuse.h>

#ifdef HAVE_SYS_XATTR_H
#include <sys/xattr.h>
#endif

#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/dirent.h>
#include "nxfuse.h"

/****************************************************************************
 * Name: nxfuse_getattr
 *
 *      FUSE callback to get file attribute
 *
 ****************************************************************************/

static int nxfuse_getattr(const char *path, struct stat *fs)
{
	int ret = 0;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate stat is implemented by VFS */

	if (pdata->pinode->u.i_mops->stat == NULL) {
		return -ENOSYS;
	}

	/* Remove leading '/' from path */

	if (*path == '/') {
		path++;
	}

	/* Call the FS stat function */

	if (*path == '\0') {
		/* For the root dir stat, just set the values */

		fs->st_mode = S_IFDIR | 0777;
		fs->st_nlink = 3;
	} else {
		/* Stat the file on the filesystem */

		ret = pdata->pinode->u.i_mops->stat(pdata->pinode, path, fs);
		fs->st_mode &= 0770777;
	}

	/* Set the UID and GID to the current user (not defined in NuttX) */

	fs->st_uid = getuid();
	fs->st_gid = getgid();

	return ret;
}

/****************************************************************************
 * Name: nxfuse_mkdir
 *
 *      FUSE callback to create a new directory
 *
 ****************************************************************************/

static int nxfuse_mkdir(const char *path, mode_t mode)
{
	int ret;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate open is implemented by VFS */

	if (pdata->pinode->u.i_mops->mkdir == NULL) {
		return -ENOSYS;
	}

	/* Remove leading '/' from path */

	if (*path == '/') {
		path++;
	}

	/* Perform the mkdir */

	ret = pdata->pinode->u.i_mops->mkdir(pdata->pinode, path, mode);
	return ret;
}

/****************************************************************************
 * Name: nxfuse_unlink
 *
 *      FUSE callback to delete a regular file
 *
 ****************************************************************************/

static int nxfuse_unlink(const char *path)
{
	int ret;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate opendir is implemented by VFS */

	if (pdata->pinode->u.i_mops->unlink == NULL) {
		return -ENOSYS;
	}

	/* Remove leading '/' from path */

	if (*path == '/') {
		path++;
	}

	/* Perform the rename operation */

	ret = pdata->pinode->u.i_mops->unlink(pdata->pinode, path);
	return ret;
}

/****************************************************************************
 * Name: nxfuse_rmdir
 *
 *      FUSE callback to delete a directory
 *
 ****************************************************************************/

static int nxfuse_rmdir(const char *path)
{
	int ret;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate open is implemented by VFS */

	if (pdata->pinode->u.i_mops->rmdir == NULL) {
		return -ENOSYS;
	}

	/* Remove leading '/' from path */

	if (*path == '/') {
		path++;
	}

	/* Perform the mkdir */

	ret = pdata->pinode->u.i_mops->rmdir(pdata->pinode, path);
	return ret;
}

/****************************************************************************
 * Name: nxfuse_rename
 *
 *      FUSE callback to rename a file
 *
 ****************************************************************************/

static int nxfuse_rename(const char *path, const char *newpath)
{
	int ret;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate opendir is implemented by VFS */

	if (pdata->pinode->u.i_mops->rename == NULL) {
		return -ENOSYS;
	}

	/* Remove leading '/' from path and newpath */

	if (*path == '/') {
		path++;
	}
	if (*newpath == '/') {
		newpath++;
	}

	/* Perform the rename operation */

	ret = pdata->pinode->u.i_mops->rename(pdata->pinode, path, newpath);
	return ret;
}

/****************************************************************************
 * Name: nxfuse_chmod
 *
 *      FUSE callback to update the file mode bits
 *
 ****************************************************************************/

static int nxfuse_chmod(const char *path, mode_t mode)
{
	int ret;
	struct file *filep;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate opendir is implemented by VFS */

	if (pdata->pinode->u.i_mops->open == NULL || pdata->pinode->u.i_mops->ioctl == NULL) {
		return -ENOSYS;
	}

	/* Remove leading '/' from path and newpath */

	if (*path == '/') {
		path++;
	}

	filep = (struct file *)malloc(sizeof(struct file));
	filep->f_seekpos = 0;
	filep->f_inode = pdata->pinode;
	filep->f_priv = NULL;
	filep->f_oflags = O_RDOK | O_WROK;

	/* First try to open the file */

	ret = pdata->pinode->u.i_mops->open(filep, path, filep->f_oflags, mode);
	if (ret != OK) {
		free(filep);
		return ret;
	}

	/* Now issue the FIOUTIME ioctl */

	ret = pdata->pinode->u.i_mops->ioctl(filep, FIOCHMOD, mode);
	ret = pdata->pinode->u.i_mops->close(filep);
	free(filep);

	return ret;
}

/****************************************************************************
 * Name: nxfuse_chown
 *
 *      FUSE callback to update the file ownership
 *
 ****************************************************************************/

static int nxfuse_chown(const char *path, uid_t uid, gid_t gid)
{
	return OK;
}

/****************************************************************************
 * Name: nxfuse_truncate
 *
 *      FUSE callback to tuncate a file to a given length
 *
 ****************************************************************************/

static int nxfuse_truncate(const char *path, off_t newsize)
{
	return OK;
}

/****************************************************************************
 * Name: nxfuse_utime
 *
 *      FUSE callback to update the modified time of a file
 *
 ****************************************************************************/

static int nxfuse_utime(const char *path, struct utimbuf *ubuf)
{
	int mode;
	int ret;
	struct file *filep;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate opendir is implemented by VFS */

	if (pdata->pinode->u.i_mops->open == NULL || pdata->pinode->u.i_mops->ioctl == NULL) {
		return -ENOSYS;
	}

	/* Remove leading '/' from path and newpath */

	if (*path == '/') {
		path++;
	}

	filep = (struct file *)malloc(sizeof(struct file));
	filep->f_seekpos = 0;
	filep->f_inode = pdata->pinode;
	filep->f_priv = NULL;
	filep->f_oflags = O_RDOK | O_WROK;
	mode = 0666;

	/* First try to open the file */

	ret = pdata->pinode->u.i_mops->open(filep, path, filep->f_oflags, mode);
	if (ret != OK) {
		free(filep);
		return ret;
	}

	/* Now issue the FIOUTIME ioctl */

	ret = pdata->pinode->u.i_mops->ioctl(filep, FIOUTIME, ubuf->modtime);
	ret = pdata->pinode->u.i_mops->close(filep);
	free(filep);

	return ret;
}

/****************************************************************************
 * Name: nxfuse_statfs
 *
 *      FUSE callback to stat the filesystem
 *
 ****************************************************************************/

static int nxfuse_statfs(const char *path, struct statvfs *vfs)
{
	struct statfs buf;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate statfs is implemented by VFS */

	if (pdata->pinode->u.i_mops->statfs == NULL) {
		return -ENOSYS;
	}

	/* Stat the filesystem */

	pdata->pinode->u.i_mops->statfs(pdata->pinode, &buf);

	/* Return the FS values */

	vfs->f_bsize = buf.f_bsize;
	vfs->f_ffree = buf.f_ffree;
	vfs->f_bavail = buf.f_bavail;
	vfs->f_blocks = buf.f_blocks;
	vfs->f_files = buf.f_files;
	vfs->f_bfree = buf.f_bfree;
	vfs->f_namemax = buf.f_namelen;

	return OK;
}

/****************************************************************************
 * Name: nxfuse_flush
 *
 *      FUSE callback to flush updates to a file
 *
 ****************************************************************************/

static int nxfuse_flush(const char *path, struct fuse_file_info *fi)
{
	int ret = OK;
	struct file *filep;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate sync is implemented by VFS */

	if (pdata->pinode->u.i_mops->sync == NULL) {
		return OK;
	}

	/* Get our private file data from the FUSE file handle */

	filep = (struct file *)fi->fh;

	/* Perform the file sync */

	ret = pdata->pinode->u.i_mops->sync(filep);

	return ret;
}

/****************************************************************************
 * Name: nxfuse_opendir
 *
 *      FUSE callback to open a directory for reading
 *
 ****************************************************************************/

static int nxfuse_opendir(const char *path, struct fuse_file_info *fi)
{
	int ret;
	struct fs_dirent_s *de;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate opendir is implemented by VFS */

	if (pdata->pinode->u.i_mops->opendir == NULL) {
		return -ENOSYS;
	}

	/* Remove leading '/' from path */

	if (*path == '/') {
		path++;
	}

	/* Perform the opendir operation */

	de = malloc(sizeof(struct fs_dirent_s));
	if (de == NULL) {
		return -ENOMEM;
	}

	/* Perform the opendir operation */

	ret = pdata->pinode->u.i_mops->opendir(pdata->pinode, path, de);

	/* Save the opendir data in the file info */

	fi->fh = (intptr_t) de;
	return ret;
}

/****************************************************************************
 * Name: nxfuse_readdir
 *
 *      FUSE callback to read file content from a directory
 *
 ****************************************************************************/

static int nxfuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	int ret;
	struct stat fs;
	struct fs_dirent_s *de;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate readdir is implemented by VFS */

	if (pdata->pinode->u.i_mops->readdir == NULL) {
		return -ENOSYS;
	}

	/* Remove leading '/' from path */

	if (*path == '/') {
		path++;
	}

	/* Get pointer to the directory entry struct */

	de = (struct fs_dirent_s *)fi->fh;

	/* Read the first entry */

	ret = pdata->pinode->u.i_mops->readdir(pdata->pinode, de);
	if (ret != OK) {
		return ret;
	}

	/* Fill the buffer with all entries */

	do {
		/* Get the file stat info */

		pdata->pinode->u.i_mops->stat(pdata->pinode, path, &fs);

		/* Fill this entry's data using the FUSE filler function */

		if (filler(buf, de->fd_dir.d_name, &fs, 0) != 0) {
			return -ENOMEM;
		}

		/* Loop until no more directory entries */

	} while ((ret = pdata->pinode->u.i_mops->readdir(pdata->pinode, de)) == OK);

	return OK;
}

/****************************************************************************
 * Name: nxfuse_releasedir
 *
 *      FUSE callback to close an opendir
 *
 ****************************************************************************/

static int nxfuse_releasedir(const char *path, struct fuse_file_info *fi)
{
	struct fs_dirent_s *de;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Get pointer to the directory entry struct */

	de = (struct fs_dirent_s *)fi->fh;

	/* Call the closedir function */

	if (pdata->pinode->u.i_mops->closedir != NULL) {
		pdata->pinode->u.i_mops->closedir(pdata->pinode, de);
	}

	/* Free the de struct */

	free(de);
	return OK;
}

/****************************************************************************
 * Name: nxfuse_open
 *
 *      FUSE callback to open an existing file
 *
 ****************************************************************************/

static int nxfuse_open(const char *path, struct fuse_file_info *fi)
{
	int ret, mode;
	struct file *filep;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate open is implemented by VFS */

	if (pdata->pinode->u.i_mops->open == NULL) {
		return -ENOSYS;
	}

	/* Remove leading '/' from path */

	if (*path == '/') {
		path++;
	}

	/* Allocate a private file data struct to track things */

	filep = (struct file *)malloc(sizeof(struct file));
	filep->f_seekpos = 0;
	filep->f_pos = 0;
	filep->f_inode = pdata->pinode;
	filep->f_priv = NULL;
	filep->f_oflags = fi->flags;

	/* Map O_RDWR open flags to O_RDOK | O_WROK */

	if (fi->flags & O_RDWR) {
		filep->f_oflags |= O_RDOK | O_WROK;
	} else if (fi->flags & O_WRONLY) {
		filep->f_oflags |= O_WROK;
	} else {
		filep->f_oflags |= O_RDOK;
	}

	mode = 0666;

	/* Perform the open */

	if ((ret = pdata->pinode->u.i_mops->open(filep, path, filep->f_oflags, mode)) != OK) {
		/* Error opening file.  Free the filep struct and return */

		free(filep);
		return ret;
	}

	/* Save the pointer to our file data in the FUSE file info */

	fi->fh = (intptr_t) filep;
	return OK;
}

/****************************************************************************
 * Name: nxfuse_access
 *
 *      FUSE callback to test if filesystem access is granted
 *
 ****************************************************************************/

static int nxfuse_access(const char *path, int mask)
{
	return OK;
}

/****************************************************************************
 * Name: nxfuse_create
 *
 *      FUSE callback to create a new file in the filesystem
 *
 ****************************************************************************/

static int nxfuse_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	int ret;
	struct file *filep;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate open is implemented by VFS */

	if (pdata->pinode->u.i_mops->open == NULL) {
		return -ENOSYS;
	}

	/* Remove leading '/' from path */

	if (*path == '/') {
		path++;
	}

	/* Allocate a private file data struct to track things */

	filep = (struct file *)malloc(sizeof(struct file));
	filep->f_seekpos = 0;
	filep->f_inode = pdata->pinode;
	filep->f_priv = NULL;

	/* Append the O_CREAT and O_TRUNC flags to perform force create / recreate */

	filep->f_oflags = fi->flags | O_CREAT | O_TRUNC;	// | O_WROK;

	/* Perform the open */

	if ((ret = pdata->pinode->u.i_mops->open(filep, path, filep->f_oflags, mode)) != OK) {
		free(filep);
		return ret;
	}

	/* Save the pointer to our file data in the FUSE file info */

	fi->fh = (intptr_t) filep;
	return OK;
}

/****************************************************************************
 * Name: nxfuse_ftruncate
 *
 *      FUSE callback to truncate an already opened file.
 *
 ****************************************************************************/

static int nxfuse_ftruncate(const char *path, off_t offset, struct fuse_file_info *fi)
{
	return OK;
}

/****************************************************************************
 * Name: nxfuse_read
 *
 *      FUSE callback to read from an opened file
 *
 ****************************************************************************/

static int nxfuse_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int ret;
	struct file *filep;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate read is implemented by VFS */

	if (pdata->pinode->u.i_mops->read == NULL) {
		return -ENOSYS;
	}

	/* Get our private file data from the FUSE file handle */

	filep = (struct file *)fi->fh;

	/* Test if seek needed  */

	if (filep->f_seekpos != offset) {
		/* Seek to new location */

		if (pdata->pinode->u.i_mops->seek == NULL) {
			return -ENOSYS;
		}

		pdata->pinode->u.i_mops->seek(filep, offset, SEEK_SET);
		filep->f_seekpos = offset;
	}

	/* Perform the read */

	ret = pdata->pinode->u.i_mops->read(filep, buf, size);

	/* Keep track of the file position for seeking */

	if (ret > 0) {
		filep->f_seekpos += ret;
	}

	/* Return the number of bytes read */

	return ret;
}

/****************************************************************************
 * Name: nxfuse_write
 *
 *      FUSE callback to write to an opened file
 *
 ****************************************************************************/

static int nxfuse_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int ret;
	struct file *filep;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate write is implemented by VFS */

	if (pdata->pinode->u.i_mops->write == NULL) {
		return -ENOSYS;
	}

	/* Get our private file data from the FUSE file handle */

	filep = (struct file *)fi->fh;

	/* Test if seek needed  */

	if (filep->f_seekpos != offset) {
		/* Seek to new location */

		if (pdata->pinode->u.i_mops->seek == NULL) {
			return -ENOSYS;
		}

		pdata->pinode->u.i_mops->seek(filep, offset, SEEK_SET);
		filep->f_seekpos = offset;
	}

	/* Perform the write */

	ret = pdata->pinode->u.i_mops->write(filep, buf, size);

	/* Keep track of the file position for seeking */

	if (ret > 0) {
		filep->f_seekpos += ret;
	}

	/* Return the number of bytes read */

	return ret;
}

/****************************************************************************
 * Name: nxfuse_release
 *
 *      FUSE callback to close an open file
 *
 ****************************************************************************/

static int nxfuse_release(const char *path, struct fuse_file_info *fi)
{
	int ret = OK;
	struct file *filep;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Get our private file data from the FUSE file handle */

	filep = (struct file *)fi->fh;

	/* Perform the close and free memory */

	if (pdata->pinode->u.i_mops->close != NULL) {
		ret = pdata->pinode->u.i_mops->close(filep);
	}

	free(filep);
	fi->fh = 0;

	return ret;
}

/****************************************************************************
 * Name: nxfuse_fsync
 *
 *      FUSE callback to sync a file
 *
 ****************************************************************************/

static int nxfuse_fsync(const char *path, int datasync, struct fuse_file_info *fi)
{
	int ret = OK;
	struct file *filep;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate sync is implemented by VFS */

	if (pdata->pinode->u.i_mops->sync == NULL) {
		return -ENOSYS;
	}

	/* Get our private file data from the FUSE file handle */

	filep = (struct file *)fi->fh;

	/* Perform the file sync */

	ret = pdata->pinode->u.i_mops->sync(filep);

	return ret;
}

/****************************************************************************
 * Name: nxfuse_init
 *
 *      FUSE callback to init the filesystem
 *
 ****************************************************************************/

static void *nxfuse_init(struct fuse_conn_info *conn)
{
	return fuse_get_context()->private_data;
}

/****************************************************************************
 * Name: nxfuse_fgetattr
 *
 *      FUSE callback to get attributes of an open file
 *
 ****************************************************************************/

static int nxfuse_fgetattr(const char *path, struct stat *fs, struct fuse_file_info *fi)
{
	int ret = 0;
	struct nxfuse_state *pdata = (struct nxfuse_state *)(fuse_get_context()->private_data);

	/* Validate stat is implemented by VFS */

	if (pdata->pinode->u.i_mops->stat == NULL) {
		return -ENOSYS;
	}

	/* Remove leading '/' from path */

	if (*path == '/') {
		path++;
	}

	/* Call the FS stat function */

	if (*path == '\0') {
		/* For the root dir stat, just set the values */

		fs->st_mode = S_IFDIR | 0777;
		fs->st_nlink = 3;
	} else {
		/* Stat the file on the filesystem */

		ret = pdata->pinode->u.i_mops->stat(pdata->pinode, path, fs);
		fs->st_mode &= 0770777;
	}

	return ret;
}

/****************************************************************************
 * Name: nxfuse_oper
 *
 *      FUSE operations structure
 *
 ****************************************************************************/

struct fuse_operations nxfuse_oper = {
	.getattr = nxfuse_getattr,
	.readlink = NULL,
	.getdir = NULL,
	.mknod = NULL,
	.mkdir = nxfuse_mkdir,
	.unlink = nxfuse_unlink,
	.rmdir = nxfuse_rmdir,
	.symlink = NULL,
	.rename = nxfuse_rename,
	.link = NULL,
	.chmod = nxfuse_chmod,
	.chown = nxfuse_chown,
	.truncate = nxfuse_truncate,
	.utime = nxfuse_utime,
	.open = nxfuse_open,
	.read = nxfuse_read,
	.write = nxfuse_write,
	.statfs = nxfuse_statfs,
	.flush = nxfuse_flush,
	.release = nxfuse_release,
	.fsync = nxfuse_fsync,

#ifdef HAVE_SYS_XATTR_H
	.setxattr = NULL,
	.getxattr = NULL,
	.listxattr = NULL,
	.removexattr = NULL,
#endif

	.opendir = nxfuse_opendir,
	.readdir = nxfuse_readdir,
	.releasedir = nxfuse_releasedir,
	.fsyncdir = NULL,
	.init = nxfuse_init,
	.destroy = NULL,
	.access = nxfuse_access,
	.create = nxfuse_create,
	.ftruncate = nxfuse_ftruncate,
	.fgetattr = nxfuse_fgetattr
};
