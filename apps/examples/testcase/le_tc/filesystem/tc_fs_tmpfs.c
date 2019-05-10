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

/// @file tc_fs_tmpfs.c

/// @brief Test Case Example for tmp file system

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <tinyara/fs/ioctl.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <dirent.h>
#include "tc_common.h"

#define TMPFS_TEST_CONTENTS "THIS IS TMP FS"
#define TMPFS_TEST_MOUNTPOINT "/tmpfs_test"
#define INVALID_PATH "/empty"

void tc_fs_tmpfs_main(void)
{
	int ret;
	int fd1 = -1;
	int fd2 = -1;
	char *write_buf = TMPFS_TEST_CONTENTS;
	char read_buf[32];
	long size;
	struct stat st;
	struct statfs fs;
	struct dirent *dirent;
	DIR *dir;

	ret = mount(NULL, TMPFS_TEST_MOUNTPOINT, "tmpfs", 0, NULL);
	TC_ASSERT_EQ("mount", ret, OK);

	fd1 = open(TMPFS_TEST_MOUNTPOINT"/test", O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ("open", fd1, 0);

	ret = write(fd1, write_buf, strlen(write_buf));
	TC_ASSERT_EQ_CLEANUP("write", ret, strlen(write_buf), close(fd1));

	ret = lseek(fd1, 0, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("lseek", ret, OK, close(fd1));

	memset(read_buf, 0, sizeof(read_buf));
	ret = read(fd1, read_buf, sizeof(read_buf));
	printf("read:%s\n", read_buf);
	TC_ASSERT_EQ_CLEANUP("read", ret, strlen(write_buf), close(fd1));
	TC_ASSERT_EQ_CLEANUP("read", strcmp(read_buf, TMPFS_TEST_CONTENTS), 0, close(fd1));

	ret = ioctl(fd1, FIOC_MMAP, (unsigned long)&size);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd1));

	fd2 = dup(fd1);
	TC_ASSERT_GEQ_CLEANUP("dup", fd2, 0, close(fd1));

	ret = fstat(fd1, &st);
	TC_ASSERT_EQ_CLEANUP("fstat", ret, OK, close(fd1));

	close(fd1);
	close(fd2);

	dir = opendir(TMPFS_TEST_MOUNTPOINT);
	TC_ASSERT_NEQ("opendir", dir, NULL);

	dirent = readdir(dir);
	TC_ASSERT_NEQ_CLEANUP("opendir", dirent, NULL, closedir(dir));

	rewinddir(dir);

	ret = closedir(dir);
	TC_ASSERT_EQ("closedir", ret, OK);

	ret = mkdir(TMPFS_TEST_MOUNTPOINT"/dir", 0777);
	TC_ASSERT_EQ("mkdir", ret, OK);

	ret = rmdir(TMPFS_TEST_MOUNTPOINT"/dir");
	TC_ASSERT_EQ("rmdir", ret, OK);

	ret = stat(TMPFS_TEST_MOUNTPOINT"/test", &st);
	TC_ASSERT_EQ("stat", ret, OK);

	ret = rename(TMPFS_TEST_MOUNTPOINT"/test", TMPFS_TEST_MOUNTPOINT"/test_new");
	TC_ASSERT_EQ("rename", ret, OK);

	ret = statfs(TMPFS_TEST_MOUNTPOINT, &fs);
	TC_ASSERT_EQ("statfs", ret, OK);

	ret = unlink(TMPFS_TEST_MOUNTPOINT"/test_new");
	TC_ASSERT_EQ("unlink", ret, OK);

	ret = umount(TMPFS_TEST_MOUNTPOINT);
	TC_ASSERT_EQ("umount", ret, OK);

	TC_SUCCESS_RESULT();
}
