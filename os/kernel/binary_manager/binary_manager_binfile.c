/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <debug.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <tinyara/binary_manager.h>

#include "binary_manager/binary_manager.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_scan_ubin
 *
 * Description:
 *	 This function scans user binary files.
 *
 ****************************************************************************/
void binary_manager_scan_ubin(void)
{
	int ret;
	DIR *dirp;
	binary_header_t header_data;
	char filename[NAME_MAX];
	char filepath[CONFIG_PATH_MAX];
	char new_filepath[CONFIG_PATH_MAX];

	/* Open a directory for user binaries, BINARY_DIR_PATH */
	dirp = opendir(BINARY_DIR_PATH);
	if (dirp) {
		/* Read each directory entry */
		for (;;) {
			struct dirent *entryp = readdir(dirp);
			if (!entryp) {
				/* Finished with this directory */
				break;
			}
			/* Remove binary file which is not running */
			if (DIRENT_ISFILE(entryp->d_type)) {
				snprintf(filepath, CONFIG_PATH_MAX, "%s/%s", BINARY_DIR_PATH, entryp->d_name);
				ret = binary_manager_read_header(filepath, &header_data);
				if (ret < 0) {
					continue;
				}
				/* If binary is not registered, register it */
				(void)binary_manager_register_ubin(header_data.bin_name);
			}
		}
		closedir(dirp);
	} else if (errno != ENOENT) {
		bmdbg("Failed to open a directory, %s\n", BINARY_DIR_PATH);
	}
}
