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
 * drivers/bch/bchdev_unregister.c
 *
 *   Copyright (C) 2008-2009, 2012, 2016 Gregory Nutt. All rights reserved.
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

#include <sys/stat.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#include "bch.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: bchdev_unregister
 *
 * Description:
 *   Unregister character driver access to a block device that was created
 *   by a previous call to bchdev_register().
 *
 ****************************************************************************/
int bchdev_unregister(FAR const char *chardev)
{
	FAR struct bchlib_s *bch;
	int fd;
	int ret;

	/* Sanity check */
#ifdef CONFIG_DEBUG_FEATURES
	if (!chardev) {
		return -EINVAL;
	}
#endif

	/* Open the character driver associated with chardev */
	fd = open(chardev, O_RDONLY);
	if (fd < 0) {
		fdbg("ERROR: Failed to open %s: %d\n", chardev, errno);
		return -errno;
	}

	/*
	 * Get a reference to the internal data structure.  On success, we
	 * will hold a reference count on the state structure.
	 */
	ret = ioctl(fd, DIOC_GETPRIV, (unsigned long)((uintptr_t)&bch));
	(void)close(fd);

	if (ret < 0) {
		fdbg("ERROR: ioctl failed: %d\n", errno);
		return -errno;
	}

	/*
	 * Lock out context switches.  If there are no other references
	 * and no context switches, then we can assume that we can safely
	 * teardown the driver.
	 */
	sched_lock();

	/* Check if the internal structure is non-busy (we hold one reference). */
	if (bch->refs > 1) {
		ret = -EBUSY;
		goto errout_with_lock;
	}

	/*
	 * Unregister the driver (this cannot suspend or we lose our non-preemptive
	 * state!).  Once the driver is successfully unregistered, we can assume
	 * we have exclusive access to the state instance.
	 */
	ret = unregister_driver(chardev);
	if (ret < 0) {
		goto errout_with_lock;
	}

	sched_unlock();

	/* Release the internal structure */
	bch->refs = 0;
	return bchlib_teardown(bch);

errout_with_lock:
	bch->refs--;
	sched_unlock();
	return ret;
}
