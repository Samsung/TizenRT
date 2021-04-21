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

#include <tinyara/config.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#include <tinyara/fs/fs.h>

#include "utils_proc.h"

#define BUFFER_LEN 128

static bool is_numeric_name(const char *name)
{
	int name_index;

	if (name == NULL) {
		return false;
	}

	for (name_index = 0; name_index < CONFIG_NAME_MAX && name[name_index] != '\0'; name_index++) {
		if (!isdigit(name[name_index])) {
			/* Name contains something other than a numeric character */
			return false;
		}
	}

	return true;
}

int utils_proc_pid_foreach(procentry_handler_t handler, void *arg)
{
	DIR *dirp;
	struct dirent *entryp;
	int ret;

	/* Open "/proc" directory */
	dirp = opendir(PROCFS_MOUNT_POINT);

	if (!dirp) {
		/* Failed to open the directory */
		printf("Failed to open procfs, errno : %d\n", errno);
		return ERROR;
	}

	ret = OK;

	/* Read each directory entry */

	for (;;) {
		
		entryp = readdir(dirp);
		if (!entryp) {
			/* Finished with this directory */
			break;
		}

		if (!DIRENT_ISDIRECTORY(entryp->d_type) || !is_numeric_name(entryp->d_name)) {
			/* Not a directory or numeric name. It means this is not task/pthread entry */
			continue;
		}

		/* Call the handler with this directory entry */
		if (handler(entryp, arg) < 0) {
			ret = ERROR;
			break;
		}
	}
	closedir(dirp);

	return ret;
}

int utils_readfile(FAR const char *filepath, char *buf, int buflen, utils_handler_t handler, void *arg)
{
	int fd;
	ssize_t nread;

	if (filepath == NULL || buf == NULL || buflen <= 0) {
		printf("Invalid Parameter\n");
		return ERROR;
	}

	fd = open(filepath, O_RDONLY);
	if (fd < 0) {
		printf("Failed to open %s\n", filepath);
		return ERROR;
	}

	nread = 0;

	do {
		nread = read(fd, buf, buflen - 1);
		if (nread < 0) {
			/* Read error */
			printf("Failed to read %s, %d\n", filepath, errno);
			close(fd);
			return ERROR;
		}
		buf[nread] = '\0';
		if (handler != NULL) {
			handler(buf, arg);
		} else {
			printf("%s", buf);
		}
	} while (nread == buflen - 1);

	close(fd);

	return nread;
}
