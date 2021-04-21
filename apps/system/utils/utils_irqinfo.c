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
#include <stdio.h>
#include <stdlib.h>
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
#include <errno.h>
#include <stdbool.h>
#include <sys/mount.h>
#endif
#include <tinyara/fs/fs.h>

#include "utils_proc.h"

#define IRQ_BUFLEN 64

int utils_irqinfo(int argc, char **args)
{
	int ret;
	char *filepath;
	char buf[IRQ_BUFLEN];
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	bool is_mounted;

	is_mounted = false;

	/* Mount Procfs to use */
	ret = mount(NULL, PROCFS_MOUNT_POINT, PROCFS_FSTYPE, 0, NULL);
	if (ret == ERROR) {
		if (errno == EEXIST) {
			is_mounted = true;
		} else {
			printf("Failed to mount procfs : %d\n", errno);
			return ERROR;
		}
	}
#endif

	asprintf(&filepath, "%s/%s", PROCFS_MOUNT_POINT, "irqs");
	ret = utils_readfile(filepath, buf, IRQ_BUFLEN, NULL, NULL);
	free(filepath);

#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	if (!is_mounted) {
		/* Detach mounted Procfs */
		(void)umount(PROCFS_MOUNT_POINT);
	}
#endif

	if (ret < 0) {
		printf("Failed to read %s\n", filepath);
		return ERROR;
	}
	printf("\n");

	return OK;
}
