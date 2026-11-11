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
 * fs/fs_initialize.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/reboot_notifier.h>
#include <tinyara/trace.h>
#include <syslog.h>
#include <tinyara/fs/fs.h>

#include "rpmsgfs/rpmsgfs.h"
#include "inode/inode.h"
#include "aio/aio.h"
#include "vfs/vfs.h"
#include "fs_heap.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sync_reboot_handler
 ****************************************************************************/

static int sync_reboot_handler(FAR struct notifier_block *nb,
                               unsigned long action, FAR void *data)
{
	if (action == SYS_POWER_OFF || action == SYS_RESTART) {
		sync();
	}

	return 0;
}

#if defined(CONFIG_FS_AUTOMOUNT_PROCFS) || defined(CONFIG_FS_AUTOMOUNT_TMPFS)
/****************************************************************************
 * Name: fs_auto_mount
 *
 * Description:
 *   Mount the file systems that the configuration requests to be always
 *   mounted.  The TizenRT os_start() caller of fs_auto_mount() was
 *   replaced by the NuttX boot sequence with the 12.13.0 sync;
 *   fs_initialize() runs in the same boot phase.  The TizenRT system
 *   commands (ps, heapinfo, cpuload, irqinfo, kill) compile out their
 *   private mount fallback when CONFIG_FS_AUTOMOUNT_PROCFS is set, so
 *   /proc has to exist before the first command runs.
 *   CONFIG_FS_AUTOMOUNT_TMPFS adds the TizenRT convention of a
 *   mounted /tmp (ostest AIO scratch files and friends) to the same
 *   table.
 *
 ****************************************************************************/

static void fs_auto_mount(void)
{
	static const struct
	{
		FAR const char *fs_type;
		FAR const char *fs_mountpoint;
	} fs_automount[] =
	{
#ifdef CONFIG_FS_AUTOMOUNT_PROCFS
		{ "procfs", "/proc" },
#endif
#ifdef CONFIG_FS_AUTOMOUNT_TMPFS
		{ "tmpfs",  "/tmp"  },
#endif
		{ NULL,     NULL    }
	};

	FAR const char *fstype;
	FAR const char *mountpoint;
	int mnt_idx;
	int mnt_ret;

	for (mnt_idx = 0; ; mnt_idx++) {
		fstype     = fs_automount[mnt_idx].fs_type;
		mountpoint = fs_automount[mnt_idx].fs_mountpoint;

		if (fstype == NULL) {
			break;
		}

		mnt_ret = nx_mount(NULL, mountpoint, fstype, 0, NULL);
		if (mnt_ret < 0) {
			syslog(LOG_ERR, "ERROR: Failed to mount %s at %s: %d\n",
			       fstype, mountpoint, mnt_ret);
		}
	}
}
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Name: fs_sync_reboot_nb
 ****************************************************************************/

static struct notifier_block g_sync_nb =
{
  sync_reboot_handler
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fs_initialize
 *
 * Description:
 *   This is called from the OS initialization logic to configure the file
 *   system.
 *
 ****************************************************************************/

void fs_initialize(void)
{
	fs_trace_begin();

	fs_heap_initialize();

	/* Initial inode, file, and VFS data structures */

	inode_initialize();

	file_initlk();

#ifdef CONFIG_FS_AIO
	/* Initialize for asynchronous I/O */

	aio_initialize();

#endif

#ifdef CONFIG_FS_RPMSGFS_SERVER
	rpmsgfs_server_init();
#endif

#ifdef CONFIG_FS_NOTIFY
	notify_initialize();
#endif

	register_reboot_notifier(&g_sync_nb);

#if defined(CONFIG_FS_AUTOMOUNT_PROCFS) || defined(CONFIG_FS_AUTOMOUNT_TMPFS)
	/* Mount the file systems that the configuration requests to be
	 * always mounted, the way the TizenRT boot used to do.
	 */

	fs_auto_mount();
#endif
	fs_trace_end();
}
