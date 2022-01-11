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
 * fs/vfs/fs_unlink.c
 *
 *   Copyright (C) 2007-2009 Gregory Nutt. All rights reserved.
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

#include <unistd.h>
#include <errno.h>
#include <tinyara/fs/fs.h>

#include "inode/inode.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#undef FS_HAVE_WRITABLE_MOUNTPOINT
#if !defined(CONFIG_DISABLE_MOUNTPOINT) && defined(CONFIG_FS_WRITABLE) && \
	CONFIG_NFILE_STREAMS > 0
#define FS_HAVE_WRITABLE_MOUNTPOINT 1
#endif

#undef FS_HAVE_PSEUDOFS_OPERATIONS
#if !defined(CONFIG_DISABLE_PSEUDOFS_OPERATIONS) && CONFIG_NFILE_STREAMS > 0
#define FS_HAVE_PSEUDOFS_OPERATIONS 1
#endif

#undef FS_HAVE_UNLINK
#if defined(FS_HAVE_WRITABLE_MOUNTPOINT) || defined(FS_HAVE_PSEUDOFS_OPERATIONS)
#define FS_HAVE_UNLINK 1
#endif

#ifdef FS_HAVE_UNLINK

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: unlink
 *
 * Description:  Remove a file managed a mountpoint
 *
 ****************************************************************************/

int unlink(FAR const char *pathname)
{
	FAR struct inode *inode;
	const char *relpath = NULL;
	int errcode;
	int ret;

	/* Get an inode for this file */

	inode = inode_find(pathname, &relpath);
	if (!inode) {
		/* There is no inode that includes in this path */

		errcode = ENOENT;
		goto errout;
	}
#ifndef CONFIG_DISABLE_MOUNTPOINT
	/* Check if the inode is a valid mountpoint. */

	if (INODE_IS_MOUNTPT(inode) && inode->u.i_mops) {
		/* Perform the unlink operation using the relative path at the
		 * mountpoint.
		 */

		if (inode->u.i_mops->unlink) {
			ret = inode->u.i_mops->unlink(inode, relpath);
			if (ret < 0) {
				errcode = -ret;
				goto errout_with_inode;
			}
		} else {
			errcode = ENOSYS;
			goto errout_with_inode;
		}
	} else
#endif

#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
		/* If this is a "dangling" pseudo-file node (i.e., it has operations) then rm
		 * should remove the node.
		 */

		if (!INODE_IS_SPECIAL(inode) && inode->u.i_ops) {
			/* If this is a pseudo-file node (i.e., it has no operations)
			 * then rmdir should remove the node.
			 */

			inode_semtake();

			/* Refuse to unlink the inode if it has children.  I.e., if it is
			 * functioning as a directory and the directory is not empty.
			 */

			if (inode->i_child != NULL) {
				errcode = ENOTEMPTY;
				inode_semgive();
				goto errout_with_inode;
			}

			/* Notify the driver that it has been unlinked.  If there are no
			 * open references to the driver instance, then the driver should
			 * release all resources because it is no longer accessible.
			 */

			if (INODE_IS_DRIVER(inode) && inode->u.i_ops->unlink) {
				/* Notify the character driver that it has been unlinked */

				ret = inode->u.i_ops->unlink(inode);
				if (ret < 0) {
					errcode = -ret;
					goto errout_with_inode;
				}
			}
#ifndef CONFIG_DISABLE_MOUNTPOINT
			else if (INODE_IS_BLOCK(inode) && inode->u.i_bops->unlink) {
				/* Notify the block driver that it has been unlinked */

				ret = inode->u.i_bops->unlink(inode);
				if (ret < 0) {
					errcode = -ret;
					goto errout_with_inode;
				}
			}
#endif

			/* Remove the old inode.  Because we hold a reference count on the
			 * inode, it will not be deleted now.  It will be deleted when all
			 * of the references to the inode have been released (perhaps
			 * when inode_release() is called below).  inode_remove() will
			 * return -EBUSY to indicate that the inode was not deleted now.
			 */

			ret = inode_remove(pathname);
			inode_semgive();

			if (ret < 0 && ret != -EBUSY) {
				errcode = -ret;
				goto errout_with_inode;
			}
		} else
#endif
		{
			errcode = ENXIO;
			goto errout_with_inode;
		}

	/* Successfully unlinked */

	inode_release(inode);
	return OK;

errout_with_inode:
	inode_release(inode);
errout:
	set_errno(errcode);
	return ERROR;
}

#endif							/* FS_HAVE_UNLINK */
