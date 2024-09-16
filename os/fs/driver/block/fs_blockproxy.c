/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * fs/driver/fs_blockproxy.c
 *
 *   Copyright (C) 2015-2016 Gregory Nutt. All rights reserved.
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
#include <sys/stat.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/common_logs/common_logs.h>

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && \
	!defined(CONFIG_DISABLE_PSEUDOFS_OPERATIONS)

/****************************************************************************
 * Private Data
 ****************************************************************************/

static uint32_t g_devno;
static sem_t g_devno_sem;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: unique_chardev
 *
 * Description:
 *   Create a unique temporary device name in the /dev/ directory of the
 *   psuedo-file system.  We cannot use mktemp for this because it will
 *   attempt to open() the file.
 *
 * Input parameters:
 *   None
 *
 * Returned Value:
 *   The allocated path to the device.  This must be released by the caller
 *   to prevent memory links.  NULL will be returned only the case where
 *   we fail to allocate memory.
 *
 ****************************************************************************/
static FAR char *unique_chardev(void)
{
	struct stat statbuf;
	char devbuf[16];
	uint32_t devno;
	int ret;

	/* Loop until we get a unique device name */
	for (; ; ) {
		/* Get the semaphore protecting the path number */
		while (sem_wait(&g_devno_sem) < 0) {
			DEBUGASSERT(errno == EINTR);
		}

		/* Get the next device number and release the semaphore */
		devno = ++g_devno;
		sem_post(&g_devno_sem);

		/* Construct the full device number */
		devno &= 0xffffff;
		snprintf(devbuf, 16, "/dev/tmp%06lx", (unsigned long)devno);

		/* Make sure that file name is not in use */
		ret = stat(devbuf, &statbuf);
		if (ret < 0) {
			DEBUGASSERT(errno == ENOENT);
			return strdup(devbuf);
		}

		/* It is in use, try again */
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: block_proxy
 *
 * Description:
 *   Create a temporary char driver using drivers/bch to mediate character
 *   oriented accessed to the block driver.
 *
 * Input parameters:
 *   blkdev - The path to the block driver
 *   oflags - Character driver open flags
 *
 * Returned Value:
 *   If positive, non-zero file descriptor is returned on success.  This
 *   is the file descriptor of the nameless character driver that mediates
 *   accesses to the block driver.
 *
 *   Errors that may be returned:
 *
 *     ENOMEM - Failed to create a temporay path name.
 *
 *   Plus:
 *
 *     - Errors reported from bchdev_register()
 *     - Errors reported from open() or unlink()
 *
 ****************************************************************************/
int block_proxy(FAR const char *blkdev, int oflags)
{
	FAR char *chardev;
	bool readonly;
	int ret;
	int fd;

	DEBUGASSERT(blkdev);
	DEBUGASSERT((oflags & (O_CREAT | O_EXCL | O_APPEND | O_TRUNC)) == 0);

	/* Create a unique temporary file name for the character device */
	chardev = unique_chardev();
	if (chardev == NULL) {
		fdbg("%s: unique_chardev()\n", clog_message_str[CMN_LOG_FAILED_OP]);
		return -ENOMEM;
	}

	/* Should this character driver be read-only? */
	readonly = ((oflags & O_WROK) == 0);

	/* Wrap the block driver with an instance of the BCH driver */
	ret = bchdev_register(blkdev, chardev, readonly);
	if (ret < 0) {
		fdbg("%s: bchdev_register(%s, %s): %d\n", clog_message_str[CMN_LOG_FAILED_OP],
				blkdev, chardev, ret);

		goto errout_with_chardev;
	}

	/* Open the newly created character driver */
	oflags &= ~(O_CREAT | O_EXCL | O_APPEND | O_TRUNC);
	fd = open(chardev, oflags);
	if (fd < 0) {
		ret = -errno;
		fdbg("%s: %s, %d\n", clog_message_str[CMN_LOG_FAILED_OP], chardev, ret);
		goto errout_with_bchdev;
	}

	/*
	 * Unlink the character device name.  The driver instance will persist,
	 * provided that CONFIG_DISABLE_PSEUDOFS_OPERATIONS=y (otherwise, we have
	 * a problem here!)
	 */
	ret = unlink(chardev);
	if (ret < 0) {
		ret = -errno;
		fdbg("%s to unlink %s: %d\n", clog_message_str[CMN_LOG_FAILED_OP], chardev, ret);
	}

	/*
	 * Free the allocate character driver name and return the open file
	 * descriptor.
	 */
	kmm_free(chardev);
	return fd;

errout_with_bchdev:
	(void)unlink(chardev);

errout_with_chardev:
	kmm_free(chardev);
	return ret;
}

/****************************************************************************
 * Name: unique_chardev_initialize
 *
 * Description:
 *   Initialize a semphore for unique character device
 *
 * Input parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void unique_chardev_initialize(void)
{
	/* Initialize a semphore for unique chardev */

	sem_init(&g_devno_sem, 0, 1);
}
#endif /* !CONFIG_DISABLE_MOUNTPOINT && !CONFIG_DISABLE_PSEUDOFS_OPERATIONS */
