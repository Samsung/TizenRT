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

/// @file tc_fs_procfs.c

/// @brief Test Case Example for proc file system

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include "tc_common.h"

#define PROC_MOUNTPOINT "/proc"
#define PROC_BUFFER_LEN 128
#define PROC_FILEPATH_LEN CONFIG_PATH_MAX

/* Read all files in the directory */
static int read_dir_entries(const char *dirpath)
{
	int fd;
	int ret;
	DIR *dirp;
	ssize_t nread;
	struct dirent *entryp;
	char path[PROC_FILEPATH_LEN];
	char buf[PROC_BUFFER_LEN];

	dirp = opendir(dirpath);
	if (!dirp) {
		printf("Failed to open directory %s\n", dirpath);
		return ERROR;
	}

	while ((entryp = readdir(dirp)) != NULL) {
		snprintf(path, PROC_FILEPATH_LEN, "%s/%s", dirpath, entryp->d_name);
		if (!DIRENT_ISDIRECTORY(entryp->d_type)) {
			/* If this entry is a file, open and read it. */
			printf("%s: \n", path);
			fd = open(path, O_RDONLY);
			if (fd < 0) {
				printf("Failed to open file %s\n", path);
				goto error;
			}
			nread = 0;
			do {
				nread = read(fd, buf, PROC_BUFFER_LEN - 1);
				if (nread < 0) {
					/* Read error */
					printf("Failed to read : %d\n", errno);
					goto error_with_fd;
				}
				buf[nread] = '\0';
				printf("%s", buf);
			} while (nread == PROC_BUFFER_LEN - 1);
			printf("\n");
			close(fd);
		} else {
			ret = read_dir_entries(path);
			if (ret != OK) {
				goto error;
			}
		}
	}
	closedir(dirp);

	return OK;
error_with_fd:
	close(fd);
error:
	closedir(dirp);

	return ERROR;
}

void tc_fs_procfs_main(void)
{
	int ret;

	ret = mount(NULL, PROC_MOUNTPOINT, "procfs", 0, NULL);
	TC_ASSERT("mount", ret == OK || errno == EEXIST);

	ret = read_dir_entries(PROC_MOUNTPOINT);
	TC_ASSERT_EQ("read_dir_entries", ret, OK);

	TC_SUCCESS_RESULT();
}
