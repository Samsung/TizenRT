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
 * fs/driver/block/fs_openblockdriver.c
 *
 *   Copyright (C) 2008 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in pathname and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of pathname code must retain the above copyright
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
#include <sys/mount.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/fs/fs.h>
#include <tinyara/common_logs/common_logs.h>
#include "inode/inode.h"
#include "driver.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: find_blockdriver
 *
 * Description:
 *   Return the inode of the block driver specified by 'pathname'
 *
 * Inputs:
 *   pathname - the full path to the block driver to be located
 *   mountflags - if MS_RDONLY is not set, then driver must support write
 *     operations (see include/sys/mount.h)
 *   ppinode - address of the location to return the inode reference
 *
 * Return:
 *   Returns zero on success or a negated errno on failure:
 *
 *   EINVAL  - pathname or pinode is NULL
 *   ENOENT  - No block driver of this name is registered
 *   ENOTBLK - The inode associated with the pathname is not a block driver
 *   EACCESS - The MS_RDONLY option was not set but this driver does not
 *     support write access
 *
 ****************************************************************************/

int find_blockdriver(FAR const char *pathname, int mountflags, FAR struct inode **ppinode)
{
	FAR struct inode *inode;
	int ret = 0;				/* Assume success */

	/* Sanity checks */

#ifdef CONFIG_DEBUG
	if (!pathname || !ppinode) {
		ret = -EINVAL;
		goto errout;
	}
#endif

	/* Find the inode registered with this pathname */

	inode = inode_find(pathname, NULL);
	if (!inode) {
		fdbg("%s %s\n", clog_message_str[CMN_LOG_FAILED_OP], pathname);
		ret = -ENOENT;
		goto errout;
	}

	/* Verify that the inode is a block driver. */

	if (!INODE_IS_BLOCK(inode)) {
		fdbg("%s %s\n", clog_message_str[CMN_LOG_INVALID_VAL], pathname);
		ret = -ENOTBLK;
		goto errout_with_inode;
	}

	/* Make sure that the inode supports the requested access */

	if (!inode->u.i_bops || !inode->u.i_bops->read || (!inode->u.i_bops->write && (mountflags & MS_RDONLY) == 0)) {
		fdbg("%s %s\n", clog_message_str[CMN_LOG_INVALID_VAL], pathname);
		ret = -EACCES;
		goto errout_with_inode;
	}

	*ppinode = inode;
	return OK;

errout_with_inode:
	inode_release(inode);
errout:
	return ret;
}
