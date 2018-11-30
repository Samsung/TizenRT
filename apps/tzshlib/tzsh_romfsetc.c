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
 * apps/tzshlib/tzsh_romfsetc.c
 *
 *   Copyright (C) 2008-2012 Gregory Nutt. All rights reserved.
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

#include <sys/mount.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/drivers/ramdisk.h>

#include "tzsh.h"

#ifdef CONFIG_TZSH_ROMFSETC

/* Should we use the default ROMFS image?  Or a custom, board-specific
 * ROMFS image?
 */

#ifdef CONFIG_TZSH_ARCHROMFS
#include <arch/board/tzsh_romfsimg.h>
#elif defined(CONFIG_TZSH_CUSTOMROMFS)
#include CONFIG_TZSH_CUSTOMROMFS_HEADER
#else							/* if defined(CONFIG_TZSH_DEFAULTROMFS) */
#include "tzsh_romfsimg.h"
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_romfsetc
 ****************************************************************************/

int tzsh_romfsetc(void)
{
	int ret;

	/* Create a ROM disk for the /etc filesystem */

	ret = romdisk_register(CONFIG_TZSH_ROMFSDEVNO, romfs_img, NSECTORS(romfs_img_len), CONFIG_TZSH_ROMFSSECTSIZE);
	if (ret < 0) {
		ferr("ERROR: romdisk_register failed: %d\n", -ret);
		return ERROR;
	}

	/* Mount the file system */

	finfo("Mounting ROMFS filesystem at target=%s with source=%s\n", CONFIG_TZSH_ROMFSMOUNTPT, MOUNT_DEVNAME);

	ret = mount(MOUNT_DEVNAME, CONFIG_TZSH_ROMFSMOUNTPT, "romfs", MS_RDONLY, NULL);
	if (ret < 0) {
		ferr("ERROR: mount(%s,%s,romfs) failed: %d\n", MOUNT_DEVNAME, CONFIG_TZSH_ROMFSMOUNTPT, errno);
		return ERROR;
	}
	return OK;
}

#endif							/* CONFIG_TZSH_ROMFSETC */
