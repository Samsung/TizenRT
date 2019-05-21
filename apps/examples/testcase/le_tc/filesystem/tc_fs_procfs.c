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
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tc_common.h"
#include <stdbool.h>
#include <tinyara/fs/mksmartfs.h>

#define PROCFS_TEST_MOUNTPOINT "/proc_test"
#define MTD_PROCFS_PATH PROCFS_TEST_MOUNTPOINT"/mtd"
#define PROC_BUFFER_LEN 128
#define PROC_FILEPATH_LEN CONFIG_PATH_MAX

#define LOOP_COUNT 5
#define PROC_UPTIME_PATH PROCFS_TEST_MOUNTPOINT"/uptime"
#define PROC_VERSION_PATH PROCFS_TEST_MOUNTPOINT"/version"
#define PROC_INVALID_PATH PROCFS_TEST_MOUNTPOINT"/nofile"
#define INVALID_PATH PROCFS_TEST_MOUNTPOINT"/fs/invalid"
#if defined(CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS)
#define SMARTFS_DEV_PATH CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME
#elif defined(CONFIG_ARTIK05X_AUTOMOUNT_USERFS)
#define SMARTFS_DEV_PATH CONFIG_ARTIK05X_AUTOMOUNT_USERFS_DEVNAME
#elif defined(CONFIG_ESP32_AUTOMOUNT_USERFS_DEVNAME)
#define SMARTFS_DEV_PATH CONFIG_ESP32_AUTOMOUNT_USERFS_DEVNAME
#elif defined(CONFIG_ARCH_BOARD_LM3S6965EK)
#define SMARTFS_DEV_PATH "/dev/smart0p0"
#elif defined(CONFIG_IMXRT_AUTOMOUNT_USERFS)
#define SMARTFS_DEV_PATH CONFIG_IMXRT_AUTOMOUNT_USERFS_DEVNAME
#else
#define SMARTFS_DEV_PATH "/dev/smart1"
#endif

#define PROC_SMARTFS_PATH PROCFS_TEST_MOUNTPOINT"/fs/smartfs"
#define PROC_SMARTFS_FILE_PATH PROCFS_TEST_MOUNTPOINT"/fs/smartfs/file.txt"

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
#ifndef CONFIG_FS_PROCFS_EXCLUDE_UPTIME
static int procfs_uptime_ops(char *dirpath)
{
	int ret;
	int fd;
	int fd1 = 0;
	struct stat st;
	fd = open(PROC_UPTIME_PATH, O_RDONLY);
	if (fd < 0) {
		printf("Failed to open \n");
		return ERROR;
	}

	ret = stat(PROC_UPTIME_PATH, &st);
	if (ret != OK) {
		printf("failed to stat \n");
		close(fd);
		return ERROR;
	}
	ret = dup2(fd, fd1);
	if (ret != OK) {
		printf("failed to duplicate the file data \n");
		close(fd);
		return ERROR;
	}

	ret = close(fd);
	if (ret != OK) {
		printf("failed to close \n");
		return ERROR;
	}

	return OK;
}
#endif

static int procfs_version_ops(char *dirpath)
{
	int ret;
	int fd;
	int fd1 = 0;
	struct stat st;
	fd = open(PROC_VERSION_PATH, O_RDONLY);
	if (fd < 0) {
		printf("Failed to open \n");
		return ERROR;
	}

	ret = stat(PROC_VERSION_PATH, &st);
	if (ret != OK) {
		printf("failed to stat \n");
		close(fd);
		return ERROR;
	}
	ret = dup2(fd, fd1);
	if (ret != OK) {
		printf("failed to duplicate the file data \n");
		close(fd);
		return ERROR;
	}

	ret = close(fd);
	if (ret != OK) {
		printf("failed to close \n");
		return ERROR;
	}

		return OK;
}

static int procfs_rewind_tc(const char *dirpath)
{
	int count;
	DIR *dir;
	struct dirent *dirent;

	dir = opendir(dirpath);
	if (!dir) {
		printf("Failed to open directory %s\n", dirpath);
		return ERROR;
	}

	count = 0;
	do {
		dirent = readdir(dir);
		count++;
	} while (dirent != NULL);

	rewinddir(dir);
	do {
		dirent = readdir(dir);
		count--;
	} while (dirent != NULL);

	if (count != 0) {
		printf("rewind operation failed %s \n", dirpath);
		closedir(dir);
		return ERROR;
	}
	closedir(dir);

	return OK;
}
#ifndef CONFIG_SMARTFS_MULTI_ROOT_DIRS
void tc_fs_smartfs_mksmartfs(void)
{
	int ret;
	ret = mksmartfs(SMARTFS_DEV_PATH, 1);
	TC_ASSERT_EQ("mksmartfs", ret, OK);
	ret = mksmartfs(INVALID_PATH, 1);
	TC_ASSERT_NEQ("mksmartfs", ret, OK);
	ret = mksmartfs(SMARTFS_DEV_PATH, 0);
	TC_ASSERT_EQ("mksmartfs", ret, OK);

	TC_SUCCESS_RESULT();
}
#endif

#ifndef CONFIG_FS_PROCFS_EXCLUDE_SMARTFS
void tc_fs_smartfs_procfs_main(void)
{
	int fd;
	int ret;
	struct stat st;

	fd = open(PROC_SMARTFS_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, OK);

	/* entry not found condition */

	fd = open(PROC_SMARTFS_FILE_PATH, O_RDWR | O_CREAT);
	TC_ASSERT_EQ("open", fd, ERROR);

	fd = open(INVALID_PATH, O_RDONLY);
	TC_ASSERT_EQ("open", fd, ERROR);

	ret = read_dir_entries(PROC_SMARTFS_PATH);
	TC_ASSERT_EQ("read_dir_entries", ret, OK);

	ret = stat(PROC_SMARTFS_PATH, &st);
	TC_ASSERT_EQ("stat", ret, OK);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @testcase         tc_fs_driver_mtd_procfs_ops
* @brief            mtd procfs ops
* @scenario         opens /test_proc/mtd and performs operations
* @apicovered       mtdprocfs_operations (mtd_open, mtd_dup, mtd_stat, mtd_close)
* @precondition     NA
* @postcondition    NA
*/
#ifndef CONFIG_FS_PROCFS_EXCLUDE_MTD
static void tc_driver_mtd_procfs_ops(void)
{
	int fd;
	int ret;
	struct stat st;

	fd = open(MTD_PROCFS_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = stat(MTD_PROCFS_PATH, &st);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, OK);

	TC_SUCCESS_RESULT();
}
#endif

void tc_fs_procfs_main(void)
{
	int ret;
	struct stat st;
	bool procfs_mount_exist = false;

	ret = mount(NULL, PROCFS_TEST_MOUNTPOINT, "procfs", 0, NULL);
	if (ret < 0) {
		TC_ASSERT_EQ("mount", errno, EEXIST);
		procfs_mount_exist = true;
	}

	ret = read_dir_entries(PROCFS_TEST_MOUNTPOINT);
	TC_ASSERT_EQ("read_dir_entries", ret, OK);

	ret = procfs_rewind_tc(PROCFS_TEST_MOUNTPOINT);
	TC_ASSERT_EQ("procfs_rewind_tc", ret, OK);

#ifndef CONFIG_FS_PROCFS_EXCLUDE_UPTIME
	ret = procfs_uptime_ops(PROC_UPTIME_PATH);
	TC_ASSERT_EQ("procfs_uptime_ops", ret, OK);
#endif

	ret = stat(PROCFS_TEST_MOUNTPOINT, &st);
	TC_ASSERT_EQ("stat", ret, OK);

	ret = stat(PROC_INVALID_PATH, &st);
	TC_ASSERT_EQ("stat", ret, ERROR);

	ret = procfs_version_ops(PROC_UPTIME_PATH);
	TC_ASSERT_EQ("procfs_version_ops", ret, OK);
#ifndef CONFIG_FS_PROCFS_EXCLUDE_SMARTFS
	tc_fs_smartfs_procfs_main();
#endif
#ifndef CONFIG_FS_PROCFS_EXCLUDE_MTD
	tc_driver_mtd_procfs_ops();
#endif

	if (false == procfs_mount_exist) {
		ret = umount(PROCFS_TEST_MOUNTPOINT);
		TC_ASSERT_EQ("umount", ret, OK);
	}

	TC_SUCCESS_RESULT();
}
