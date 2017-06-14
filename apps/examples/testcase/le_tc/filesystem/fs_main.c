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

/// @file fs_main.c
/// @brief Main Function for Filesystem TestCase Example

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#ifndef CONFIG_DISABLE_POLL
#include <poll.h>
#endif
#include <errno.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/statfs.h>
#include <sys/select.h>

#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/fs_utils.h>
#include <apps/shell/tash.h>
#include <time.h>
#include "tc_common.h"
#include "tc_internal.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define MOUNT_DIR CONFIG_MOUNT_POINT

#ifdef CONFIG_FS_SMARTFS

#if defined(CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS)
#define MOUNT_DEV_DIR CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME

#elif defined(CONFIG_ARTIK053_AUTOMOUNT_USERFS)
#define MOUNT_DEV_DIR CONFIG_ARTIK053_AUTOMOUNT_USERFS_DEVNAME

#else
#define MOUNT_DEV_DIR "/dev/smart1"
#endif

#define TARGET_FS_NAME "smartfs"
#else
#define MOUNT_DEV_DIR "/dev/fs1"
#define TARGET_FS_NAME "unknwon"
#endif

#define VFS_FILE_PATH MOUNT_DIR"vfs"

#define VFS_DUP_FILE_PATH MOUNT_DIR"dup"

#define VFS_DUP2_FILE_PATH MOUNT_DIR"dup2"

#define VFS_FOLDER_PATH MOUNT_DIR"folder"

#define VFS_LOOP_COUNT 5

#define LONG_FILE_PATH MOUNT_DIR"long"

#define VFS_TEST_CONTENTS_1 "THIS IS VFS TEST 1"

#define VFS_TEST_CONTENTS_2 "THIS IS VFS TEST 2"

#define VFS_TEST_CONTENTS_3 "THIS IS VFS TEST 3"

#define LONG_FILE_CONTENTS "Yesterday all my trouble seemed so far away. Now it looks as though they're here to stay. Oh, I believe in yesterday."

#define LONG_FILE_LOOP_COUNT 24

/* Error message for CLEANUP MACRO */
#define ERROR_MSG_BAD_FD "wrong fd value"

#define ERROR_MSG_BAD_SIZE "wrong size"

#define ERROR_MSG_DIFFRENT_CONTENTS "diffrent contents"

#define ERROR_MSG_DIFFRENT_FLAG "diffrent flag"

#define ERROR_MSG_OP_FAILED "operation failed"

#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
#define FIFO_FILE_PATH "/dev/fifo_test"

#define FIFO_DATA "FIFO DATA"
#endif

#ifndef STDIN_FILENO
#  define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#  define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#  define STDERR_FILENO 2
#endif

/****************************************************************************
 * Global Variables
 ****************************************************************************/
#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
static int g_thread_result;
#endif
extern sem_t tc_sem;
extern int working_tc;

#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
/**
* @fn               mkfifo_test_listener
* @description      listener thread to read data from fifo
* @return           int
*/
void mkfifo_test_listener(pthread_addr_t pvarg)
{
	int fd, count, ret;
	char buf[12];
	char data[12];
	size_t size;
	printf("mkfifo_test_writer : write data in %s\n", FIFO_FILE_PATH);
	fd = open(FIFO_FILE_PATH, O_RDONLY);
	if (fd < 0) {
		printf("open Failed : %s\n", FIFO_FILE_PATH);
		g_thread_result = false;
		return;
	}
	count = 1;
	size = strlen(FIFO_DATA) + 2;
	while (count < 4) {
		ret = read(fd, buf, size);
		if (ret <= 0) {
			printf("Read data from fifo failed\n");
			g_thread_result = false;
			break;
		}
		snprintf(data, size, "%s%d\0", FIFO_DATA, count);
		printf("write data : %s read data : %s\n", data, buf);

		if (strcmp(data, buf) != 0) {
			printf("mkfifo failed compare Read data with written data\n");
			g_thread_result = false;
			break;
		}
		count++;
	}
	close(fd);
}
#endif

/**
* @fn               make_long_file
* @description      Make long size file to execute tc properly.
* @return           int
*/
static int make_long_file(void)
{
	FILE *fp;
	char *filename = LONG_FILE_PATH;
	char *str = LONG_FILE_CONTENTS;
	int i, ret;
	ret = 0;
	printf("Make long size file....please wait \n");
	fp = fopen(filename, "a+");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		return ERROR;
	}
	for (i = 0; i < LONG_FILE_LOOP_COUNT; i++) {
		ret += fputs(str, fp);
	}
	fputc('\0', fp);
	fclose(fp);
	printf("finished!\n");
	return ret;
}

/**
* @testcase         fs_vfs_fs_mount_tc
* @brief            Mount file system
* @scenario         Mount initialized file system
* @apicovered       mount
* @precondition     File system should be initialized. For smartfs, smart_initialize & mksmartfs should be excuted.
* @postcondition    NA
*/
static void fs_vfs_mount_tc(void)
{
	int ret;
	ret = mount(MOUNT_DEV_DIR, CONFIG_MOUNT_POINT, TARGET_FS_NAME, 0, NULL);
	TC_ASSERT_EQ("mount", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_fs_umount_tc
* @brief            Unmount file system
* @scenario         Unmount mounted file system
* @apicovered       umount
* @precondition     File system should be mounted.
* @postcondition    NA
*/
static void fs_vfs_umount_tc(void)
{
	int ret;
	ret = umount(CONFIG_MOUNT_POINT);
	TC_ASSERT_EQ("umount", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_open_tc
* @brief            Open file to do file operation
* @scenario         Open specific file
* @apicovered       open
* @precondition     NA
* @postcondition    NA
*/
static void fs_vfs_open_tc(void)
{
	int fd;
	fd = open(VFS_FILE_PATH, O_WROK | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_write_tc
* @brief            Write data into specific file
* @scenario         Open file and then write data, if writing finished, close file
* @apicovered       open, write
* @precondition     NA
* @postcondition    NA
*/
static void fs_vfs_write_tc(void)
{
	int fd, ret;
	char *buf = VFS_TEST_CONTENTS_1;
	int len;
	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC);
	TC_ASSERT_GEQ("write", fd, 0);
	len = strlen(buf);
	ret = write(fd, buf, len);
	close(fd);
	TC_ASSERT_EQ("write", ret, len);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_read_tc
* @brief            Read data from specific file
* @scenario         Open file and then read data, if reading finished, close file
* @apicovered       open, read
* @precondition	    NA
* @postcondition    NA
*/
static void fs_vfs_read_tc(void)
{
	int fd, ret;
	char buf[20];
	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("read", fd, 0);
	memset(buf, 0, sizeof(buf));
	ret = read(fd, buf, sizeof(buf));
	close(fd);
	TC_ASSERT_GEQ("read", ret, 0);
	TC_ASSERT_EQ("read", strcmp(buf, VFS_TEST_CONTENTS_1), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_close_tc
* @brief            Close file
* @scenario         Open and close
* @apicovered       open, close
* @precondition     NA
* @postcondition    NA
*/
static void fs_vfs_close_tc(void)
{
	int fd, ret;
	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("close", fd, 0);
	ret = close(fd);
	TC_ASSERT_GEQ("close", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_dup_tc
* @brief            Clone a file descriptor to an arbitray descriptor number
* @scenario         Open and write data. and then Clone descriptor to fd2, write data with fd2.
*                   After write, check it writes properly or not.
* @apicovered       open, write, dup, read
* @precondition	    NA
* @postcondition    NA
*/
static void fs_vfs_dup_tc(void)
{
	char *filename = VFS_DUP_FILE_PATH;
	char *str1 = VFS_TEST_CONTENTS_1;
	char *str2 = VFS_TEST_CONTENTS_2;
	char buf[20];
	ssize_t ret;
	int fd1, fd2, len;

	memset(buf, 0, sizeof(buf));
	fd1 = open(filename, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ("dup", fd1, 0);

	len = strlen(str1);
	ret = write(fd1, str1, len);
	TC_ASSERT_EQ_CLEANUP("dup", ret, len, ERROR_MSG_BAD_SIZE, close(fd1));

	fd2 = dup(fd1);
	TC_ASSERT_GEQ("dup", fd2, 0);

	len = strlen(str2);
	ret = write(fd2, str2, strlen(str2));
	close(fd1);
	close(fd2);
	TC_ASSERT_EQ("dup", ret, len);

	fd1 = open(filename, O_RDOK);
	TC_ASSERT_GEQ("dup", fd1, 0);

	len = strlen(str1);
	ret = read(fd1, buf, len);

	TC_ASSERT_GT_CLEANUP("dup", ret, 0, ERROR_MSG_BAD_SIZE, close(fd1));
	TC_ASSERT_EQ_CLEANUP("dup", strcmp(buf, VFS_TEST_CONTENTS_1), 0, ERROR_MSG_DIFFRENT_CONTENTS, close(fd1));

	memset(buf, 0, sizeof(buf));
	len = strlen(str2);
	ret = read(fd1, buf, len);
	close(fd1);

	TC_ASSERT_GT("dup", ret, 0);
	TC_ASSERT_EQ("dup", strcmp(buf, VFS_TEST_CONTENTS_2), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_dup2_tc
* @brief            Clone a file descriptor  to a specific descriptor number
* @scenario         Open and write data. and then Clone fd1 to fd2, write data with fd1.
*                   After write, check it writes properly or not by read data with fd2.
* @apicovered       open, write, dup2, read
* @precondition	    NA
* @postcondition    NA
*/
static void fs_vfs_dup2_tc(void)
{
	char *filename1 = VFS_DUP_FILE_PATH;
	char *filename2 = VFS_DUP2_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_3;
	char buf[20];
	ssize_t ret;
	int fd1, fd2, len;

	memset(buf, 0, sizeof(buf));

	fd1 = open(filename1, O_RDWR);
	TC_ASSERT_GEQ("dup2", fd1, 0);

	fd2 = open(filename2, O_RDWR | O_CREAT);
	close(fd1);
	TC_ASSERT_GEQ("dup2", fd2, 0);

	/* now fd1 points fd2 */
	ret = dup2(fd2, fd1);
	TC_ASSERT_GEQ_CLEANUP("dup2", ret, 0, ERROR_MSG_BAD_FD, close(fd2));

	len = strlen(VFS_TEST_CONTENTS_3);
	ret = write(fd1, str, len);
	TC_ASSERT_EQ_CLEANUP("dup2", ret, len, ERROR_MSG_BAD_SIZE, close(fd2));
	close(fd1);
	close(fd2);

	fd2 = open(filename2, O_RDONLY);
	TC_ASSERT_GEQ("dup2", fd2, 0);

	ret = read(fd2, buf, len);
	close(fd2);

	TC_ASSERT_GT("dup2", ret, 0);
	TC_ASSERT_EQ("dup2", strcmp(buf, VFS_TEST_CONTENTS_3), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_fsync_tc
* @brief            Synchronize the file state on disk to match internal, in-memory state.
* @scenario         Open and write data. and then check fsync works properly.
* @apicovered       open, write, fsync, read
* @precondition     NA
* @postcondition    NA
*/
static void fs_vfs_fsync_tc(void)
{
	int ret, fd, len;
	char *filename = VFS_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_2;

	fd = open(filename, O_WROK | O_TRUNC);
	TC_ASSERT_GEQ("fsync", fd, 0);

	len = strlen(str);
	ret = write(fd, str, len);
	TC_ASSERT_EQ_CLEANUP("fsync", ret, len, ERROR_MSG_BAD_SIZE, close(fd));

	ret = fsync(fd);
	TC_ASSERT_GEQ_CLEANUP("fsync", ret, 0, ERROR_MSG_OP_FAILED, close(fd));
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_lseek_tc
* @brief            Move current file position to specific position
* @scenario         Open file and move position, and then read data to check lseek works properly or not.
* @apicovered       open, lseek, read
* @precondition     Data(VFS_TEST_CONTENTS_2) should be written in file(VFS_FILE_PATH)
* @postcondition    NA
*/
static void fs_vfs_lseek_tc(void)
{
	int ret, fd;
	char *filename = VFS_FILE_PATH;
	char buf[20];

	fd = open(filename, O_RDONLY);
	TC_ASSERT_GEQ("lseek", fd, 0);

	ret = lseek(fd, 5, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("lseek", ret, 5, ERROR_MSG_OP_FAILED, close(fd));

	memset(buf, 0, sizeof(buf));
	ret = read(fd, buf, sizeof(buf));
	close(fd);

	TC_ASSERT_GT("lseek", ret, 0);
	TC_ASSERT_EQ("lseek", strcmp(buf, "IS VFS TEST 2"), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_pwrite_tc
* @brief            Write data at specific position of file
* @scenario         Open and write data at specific position. And then check file written on disk.
* @apicovered       open, pwrite, read
* @precondition     Data(VFS_TEST_CONTENTS_2) should be written in file(VFS_FILE_PATH)
* @postcondition    NA
*/
static void fs_vfs_pwrite_tc(void)
{
	int ret, fd;
	char *filename = VFS_FILE_PATH;
	char *str = "pwrite test!";

	fd = open(filename, O_WRONLY | O_APPEND);
	TC_ASSERT_GEQ("pwrite", fd, 0);

	ret = pwrite(fd, str, strlen(str), 10);
	close(fd);
	TC_ASSERT_NEQ("pwrite", ret, ERROR);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_pread_tc
* @brief            Read data at specific position of file
* @scenario         Open and read data from specific position.
* @apicovered       open, pread
* @precondition     fs_vfs_pwrite_tc should be passed
* @postcondition    NA
*/
static void fs_vfs_pread_tc(void)
{
	int ret, fd;
	char *filename = VFS_FILE_PATH;
	char *str = "pwrite test!";
	char buf[20];

	fd = open(filename, O_RDONLY);
	TC_ASSERT_GEQ("pread", fd, 0);

	memset(buf, 0, sizeof(buf));
	ret = pread(fd, buf, 20, 10);
	close(fd);
	TC_ASSERT_GT("pread", ret, 0);
	TC_ASSERT_EQ("pread", strcmp(buf, str), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_mkdir_tc
* @brief            Create folders
* @scenario         Create folder(VFS_FOLDER_PATH) and create 5 sub-folders
* @apicovered       mkdir
* @precondition     NA
* @postcondition    NA
*/
static void fs_vfs_mkdir_tc(void)
{
	char filename[14];
	int i;
	int ret;
	size_t len;

	len = strlen(VFS_FOLDER_PATH) + 3;
	/** make parent folder first **/
	ret = mkdir(VFS_FOLDER_PATH, 0777);
	TC_ASSERT_EQ("mkdir", ret, OK);

	for (i = 0; i < VFS_LOOP_COUNT; i++) {
		memset(filename, 0, len);
		snprintf(filename, len, "%s/%d\0", VFS_FOLDER_PATH, i);
		ret = mkdir(filename, 0777);
		TC_ASSERT_EQ("mkdir", ret, OK);
	}
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_opendir_tc
* @brief            Open specific directory to use APIs defined in dirent.h
* @scenario         Open specific directory
* @apicovered       opendir
* @precondition     fs_vfs_mkdir_tc should be passed
* @postcondition    NA
*/
static void fs_vfs_opendir_tc(void)
{
	DIR *dirp;

	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT("opendir", dirp);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_readdir_tc
* @brief            Read contents in specific directory sequentially
* @scenario         Read contents in specific folder(VFS_FOLDER_PATH)
* @apicovered       opendir, readdir, closedir
* @precondition     fs_vfs_mkdir_tc should be passed
* @postcondition    NA
*/
static void fs_vfs_readdir_tc(void)
{
	int ret, count;
	DIR *dirp;
	struct dirent *dirent;

	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT("readdir", dirp);

	count = 0;
	while (1) {
		dirent = readdir(dirp);
		if (dirent == NULL) {
			break;
		}
		count++;
	}
	ret = closedir(dirp);
	TC_ASSERT_EQ("readdir", ret, OK);
	TC_ASSERT_EQ("readdir", count, VFS_LOOP_COUNT);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_rewinddir_tc
* @brief            Reset current position of directory
* @scenario         Read contents in specific folder(VFS_FOLDER_PATH), and it reachs end of contents, reset & read again
* @apicovered       opendir, rewinddir, closedir
* @precondition     fs_vfs_mkdir_tc should be passed
* @postcondition    NA
*/
static void fs_vfs_rewinddir_tc(void)
{
	int ret, count;
	DIR *dirp;
	struct dirent *dirent;

	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT("rewinddir", dirp);

	count = 0;
	while (1) {
		dirent = readdir(dirp);
		if (dirent == NULL) {
			if (count > VFS_LOOP_COUNT) {
				break;
			}
			rewinddir(dirp);
			continue;
		}
		count++;
	}
	ret = closedir(dirp);
	TC_ASSERT_EQ("readdir", ret, OK);
	TC_ASSERT_EQ("readdir", count, VFS_LOOP_COUNT * 2);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_seekdir_tc
* @brief            Move current position of directory to specific position
* @scenario         Change position of directory and read contents
* @apicovered       opendir, seekdir, readdir, closedir
* @precondition     fs_vfs_mkdir_tc should be passed
* @postcondition    NA
*/
static void fs_vfs_seekdir_tc(void)
{
	int ret;
	DIR *dirp;
	struct dirent *dirent;
	off_t offset;
	char filename[1];

	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT("seekdir", dirp);

	offset = 2;
	seekdir(dirp, offset);
	dirent = readdir(dirp);
	TC_ASSERT("seekdir", dirent);

	itoa((int)offset, filename, 10);
	TC_ASSERT_EQ("seekdir", dirent->d_type, DTYPE_DIRECTORY);
	TC_ASSERT_EQ("seekdir", strncmp(dirent->d_name, filename, 1), 0);

	ret = closedir(dirp);
	TC_ASSERT_EQ("readdir", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_libc_dirent_readdir_r_tc
* @brief            Get position of next contents in specific directory
* @scenario         Get next contents's position until it reachs end of contents, and check count of contents
* @apicovered       opendir, readdir_r, closedir
* @precondition     fs_vfs_mkdir_tc should be passed
* @postcondition    NA
*/
static void fs_libc_dirent_readdir_r_tc(void)
{
	int ret, count;
	DIR *dirp;
	struct dirent entry;
	struct dirent *result;

	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT("seekdir", dirp);

	count = 0;
	while (1) {
		ret = readdir_r(dirp, &entry, &result);
		if (ret != 0 || result == NULL) {
			break;
		}
		count++;
	}
	ret = closedir(dirp);
	TC_ASSERT_EQ("readdir_r", ret, OK);
	TC_ASSERT_EQ("readdir_r", count, VFS_LOOP_COUNT);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_libc_dirent_telldir_tc
* @brief            Get position of current contents in specific directory
* @scenario         Get specific position by seekdir and check telldir returns position properly
* @apicovered       opendir, seekdir, telldir, closedir
* @precondition     fs_vfs_mkdir_tc should be passed
* @postcondition    NA
*/
static void fs_libc_dirent_telldir_tc(void)
{
	DIR *dirp;
	off_t offset, res;
	int ret;

	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT("telldir", dirp);

	offset = 2;
	seekdir(dirp, offset);
	res = telldir(dirp);
	TC_ASSERT_EQ("telldir", res, offset);
	ret = closedir(dirp);
	TC_ASSERT_EQ("telldir", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_closedir_tc
* @brief            Close opened directory
* @scenario         Open and close directory
* @apicovered       opendir, closedir
* @precondition     NA
* @postcondition    NA
*/
static void fs_vfs_closedir_tc(void)
{
	int ret;
	DIR *dirp;

	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT("closedir", dirp);

	ret = closedir(dirp);
	TC_ASSERT_EQ("closedir", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_rmdir_tc
* @brief            Remove each of directory
* @scenario         Remove all exist directory
* @apicovered       rmdir
* @precondition     fs_vfs_mkdir_tc should be passed
* @postcondition    NA
*/
static void fs_vfs_rmdir_tc(void)
{
	char filename[14];
	size_t len;
	int i, ret;

	len = strlen(VFS_FOLDER_PATH) + 3;

	/** remove child folders first **/
	for (i = 0; i < VFS_LOOP_COUNT; i++) {
		memset(filename, 0, len);
		snprintf(filename, len, "%s/%d\0", VFS_FOLDER_PATH, i);
		ret = rmdir(filename);
		TC_ASSERT_EQ("rmdir", ret, OK);
	}

	/** now there is no entry, remove parent folder **/
	ret = rmdir(VFS_FOLDER_PATH);
	TC_ASSERT_EQ("rmdir", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_unlink_tc
* @brief            Unlink specific file
* @scenario         Unlink specific file(VFS_DUP_FILE_PATH)
* @apicovered       unlink
* @precondition     fs_vfs_dup_tc should be passed
* @postcondition    NA
*/
static void fs_vfs_unlink_tc(void)
{
	char *filename = VFS_DUP_FILE_PATH;
	int ret;

	ret = unlink(filename);
	TC_ASSERT_EQ("unlink", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_stat_tc
* @brief            Get status of specific file
* @scenario         Get status of specific file(VFS_FILE_PATH) by stat
* @apicovered       stat
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void fs_vfs_stat_tc(void)
{
	char *filename = VFS_FILE_PATH;
	struct stat st;
	int ret;

	ret = stat(filename, &st);
	TC_ASSERT_EQ("stat", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_statfs_tc
* @brief            Get status of mounted file system
* @scenario         Get status of mounted file system by statfs and check type of file system
* @apicovered       statfs
* @precondition     File system should be mounted
* @postcondition    NA
*/
static void fs_vfs_statfs_tc(void)
{
	struct statfs fs;
	int ret;

	ret = statfs(MOUNT_DIR, &fs);
	TC_ASSERT_EQ("statfs", ret, OK);
#ifdef CONFIG_FS_SMARTFS
	TC_ASSERT_EQ("statfs", fs.f_type, SMARTFS_MAGIC);
#endif
	TC_SUCCESS_RESULT();
}

#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
/**
* @testcase         fs_vfs_mkfifo_tc
* @brief            Get data thorugh the pipe which create by mkfifo
* @scenario         Create fifo and check data between main task and its sub-thread
* @apicovered       mkfifo, open, pthread_create, write, read
* @precondition     CONFIG_PIPES should be enabled & CONFIG_DEV_PIPE_SIZE must greater than 11
* @postcondition    NA
*/
static void fs_vfs_mkfifo_tc(void)
{
	int fd, ret;
	pthread_t tid;
	int count;
	char buf[12];
	size_t size;
	bool result = false;
	g_thread_result = true;

	ret = mkfifo(FIFO_FILE_PATH, 0666);
	result = (ret >= 0) || (ret == -EEXIST);
	TC_ASSERT("mkfifo", result);

	fd = open(FIFO_FILE_PATH, O_WRONLY);
	TC_ASSERT_GEQ("mkfifo", fd, 0);

	ret = pthread_create(&tid, NULL, (pthread_startroutine_t)mkfifo_test_listener, NULL);
	TC_ASSERT_EQ_CLEANUP("mkfifo", ret, 0, ERROR_MSG_OP_FAILED, close(fd));

	size = strlen(FIFO_DATA) + 2;
	count = 1;
	while (count < 4) {
		if (!g_thread_result) {
			break;
		}
		snprintf(buf, size, "%s%d\0", FIFO_DATA, count);
		ret = write(fd, buf, size);
		TC_ASSERT_GT_CLEANUP("mkfifo", ret, 0, ERROR_MSG_BAD_SIZE, goto errout);
		count++;
		sleep(5);
	}
	close(fd);
	pthread_kill(tid, SIGUSR1);
	TC_ASSERT("mkfifo", g_thread_result);
	TC_SUCCESS_RESULT();
errout:
	pthread_kill(tid, SIGUSR1);
	close(fd);
}
#endif

/**
* @testcase         fs_vfs_sendfile_tc
* @brief            Send file data to specific descriptor from another descriptor
* @scenario         Create new file and send data from exist file
* @apicovered       open, stat, sendfile
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void fs_vfs_sendfile_tc(void)
{
	char *src_file = VFS_FILE_PATH;
	char dest_file[12];
	struct stat st;
	int fd1, fd2, ret;
	off_t size;

	snprintf(dest_file, 12, "%s_dest", src_file);

	fd1 = open(src_file, O_RDONLY);
	TC_ASSERT_GEQ("sendfile", fd1, 0);

	ret = stat(src_file, &st);
	TC_ASSERT_EQ("sendfile", ret, OK);

	size = st.st_size;
	fd2 = open(dest_file, O_WRONLY | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("sendfile", fd2, 0, ERROR_MSG_BAD_FD, close(fd1));

	ret = sendfile(fd2, fd1, 0, size);
	close(fd1);
	close(fd2);
	TC_ASSERT_EQ("sendfile", ret, size);

	ret = stat(dest_file, &st);
	TC_ASSERT_EQ("sendfile", ret, OK);
	TC_ASSERT_EQ("sendfile", size, st.st_size);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_fcntl_tc
* @brief            Access & control opened file with fcntl
* @scenario         Open file with specific flag and get access mode with fcntl
* @apicovered       open, fcntl
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void fs_vfs_fcntl_tc(void)
{
	int fd, mode;
	char *filename = VFS_FILE_PATH;
	fd = open(filename, O_WROK);
	TC_ASSERT_GEQ("fcntl", fd, 0);

	mode = fcntl(fd, F_GETFL, 0) & O_ACCMODE;
	close(fd);
	TC_ASSERT_EQ("fcntl", mode, O_WROK);
	TC_SUCCESS_RESULT();
}

#ifndef CONFIG_DISABLE_POLL
/**
* @testcase         fs_vfs_poll_tc
* @brief            Polling for I/O
* @scenario         Check poll works properly or not(STDIN, STDOUT)
* @apicovered       poll
* @precondition     CONFIG_DISABLE_POLL should be disabled
* @postcondition    NA
*/
static void fs_vfs_poll_tc(void)
{
	struct pollfd fds[2];
	int ret;
	int timeout = 30;
	printf("input text : ");

	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;

	fds[1].fd = STDOUT_FILENO;
	fds[1].events = POLLOUT;

	fflush(stdout);

	ret = poll(fds, 2, timeout * 1000);
	TC_ASSERT_GEQ("poll", ret, 0);

	if (fds[0].revents & POLLIN) {
		ret = OK;
		printf("stdin is readable.\n");
	}

	if (fds[1].revents & POLLOUT) {
		ret = OK;
		printf("stdout is writable.\n");
	}
	TC_ASSERT_EQ("poll", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_select_tc
* @brief            To monitor multiple I/O
* @scenario         Input text within 5 sec and check select works properly(check change of STDIN)
* @apicovered       select
* @precondition     CONFIG_DISABLE_POLL should be disabled
* @postcondition    NA
*/
static void fs_vfs_select_tc(void)
{
	struct timeval tv;
	fd_set readfds;
	int ret, errcnt;

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	tv.tv_sec = 5;
	tv.tv_usec = 0;
	errcnt = 1;
	printf("Enter Text within %d second(remain_count : %d)\n", tv.tv_sec, VFS_LOOP_COUNT);
	while (1) {
		ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);

		if (FD_ISSET(STDIN_FILENO, &readfds)) {
			printf("Select Success!!");
			break;
		} else {
			printf("Select Failed enter text again (remain count : %d)\n", VFS_LOOP_COUNT - errcnt);
			FD_SET(STDIN_FILENO, &readfds);
			errcnt++;
		}
		if (ret < 0) {
			errcnt = VFS_LOOP_COUNT;
			break;
		}

		if (errcnt == VFS_LOOP_COUNT) {
			break;
		}
		tv.tv_sec = 5;
		tv.tv_usec = 0;
	}
	TC_ASSERT_NEQ_CLEANUP("select", errcnt, VFS_LOOP_COUNT, ERROR_MSG_OP_FAILED, FD_CLR(STDIN_FILENO, &readfds));
	TC_SUCCESS_RESULT();
}
#endif

/**
* @testcase         fs_vfs_rename_tc
* @brief            Rename file to specific name
* @scenario         Rename exist file to specific name
* @apicovered       rename
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void fs_vfs_rename_tc(void)
{
	int ret;
	char *filename = VFS_FILE_PATH;
	char dest_file[12];

	snprintf(dest_file, 12, "%s_re", filename);
	unlink(dest_file);

	ret = rename(filename, dest_file);
	TC_ASSERT_EQ("rename", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_vfs_ioctl_tc
* @brief            ioctl with opened file
* @scenario         Get #byte of data from /dev/console by ioctl
* @apicovered       open, ioctl
* @precondition     NA
* @postcondition    NA
*/
static void fs_vfs_ioctl_tc(void)
{
	int fd, ret;
	long size;

	fd = open("/dev/console", O_RDWR);
	TC_ASSERT_GEQ("ioctl", fd, 0);
	ret = ioctl(fd, FIONREAD, &size);
	close(fd);
	TC_ASSERT_EQ("ioctl", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fdopen_tc
* @brief            fdopen with available fd value
* @scenario         Open file with specific flags, and then fdopen with diffrent flag. Then check flag is changed properly
* @apicovered       open, fdopen
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void libc_stdio_fdopen_tc(void)
{
	int fd;
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	fd = open(filename, O_WROK | O_CREAT);
	TC_ASSERT_GEQ("fdopen", fd, 0);

	fp = fdopen(fd, "r");
	TC_ASSERT_CLEANUP("fdopen", fp, ERROR_MSG_OP_FAILED, close(fd));
	close(fd);
	TC_ASSERT_EQ_CLEANUP("fdopen", fp->fs_oflags, O_RDONLY, ERROR_MSG_DIFFRENT_FLAG, fclose(fp));
	fclose(fp);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fopen_tc
* @brief            Open file by fopen
* @scenario         Open file
* @apicovered       fopen
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_fopen_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	fp = fopen(filename, "w");
	TC_ASSERT("fopen", fp);
	fclose(fp);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fclose_tc
* @brief            Close file by fopen
* @scenario         Open and Close file
* @apicovered       fopen, fclose
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_fclose_tc(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;

	fp = fopen(filename, "w");
	TC_ASSERT("fclose", fp);
	ret = fclose(fp);
	TC_ASSERT_EQ("fclose", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fputs_tc
* @brief            Write contents through the fputs
* @scenario         Write contents through the fputs, and check it works properly or not
* @apicovered       fopen, fputs
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void libc_stdio_fputs_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_1;

	fp = fopen(filename, "w");
	TC_ASSERT("fputs", fp);
	TC_ASSERT_EQ_CLEANUP("fputs", fputs(str, fp), strlen(str), ERROR_MSG_BAD_SIZE, fclose(fp));
	fclose(fp);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fgets_tc
* @brief            Read cntents through the fgets
* @scenario         Read contents through the fgets, and check it is same contents as written by libc_stdio_fgets_tc
* @apicovered       fopen, fgets
* @precondition     libc_stdio_fputs_tc  should be passed
* @postcondition    NA
*/
static void libc_stdio_fgets_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char buf[20];

	fp = fopen(filename, "r");
	TC_ASSERT("fgets", fp);

	memset(buf, 0, sizeof(buf));
	TC_ASSERT_CLEANUP("fgets", fgets(buf, 20, fp), ERROR_MSG_BAD_SIZE, fclose(fp));

	fclose(fp);
	TC_ASSERT_EQ("fgets", strcmp(buf, VFS_TEST_CONTENTS_1), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fseek_tc
* @brief            Move current file position to specific position
* @scenario         Open file and move position, and then read data to check lseek works properly or not.
* @apicovered       fopen, fseek, fgets
* @precondition     libc_stdio_fputs_tc  should be passed
* @postcondition    NA
*/
static void libc_stdio_fseek_tc(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;
	char buf[20];

	fp = fopen(filename, "r");
	TC_ASSERT("fseek", fp);

	ret = fseek(fp, 5, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("fseek", ret, OK, ERROR_MSG_OP_FAILED, fclose(fp));

	memset(buf, 0, sizeof(buf));
	TC_ASSERT_CLEANUP("fseek", fgets(buf, 20, fp), ERROR_MSG_BAD_SIZE, fclose(fp));
	fclose(fp);
	TC_ASSERT_EQ("fseek", strcmp(buf, "IS VFS TEST 1"), 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_ftell_tc
* @brief            Get current file position
* @scenario         Open file and move position, and get current position by ftell
* @apicovered       fopen, fseek, ftell
* @precondition     libc_stdio_fputs_tc  should be passed
* @postcondition    NA
*/
static void libc_stdio_ftell_tc(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;

	fp = fopen(filename, "r");
	TC_ASSERT("ftell", fp);

	ret = fseek(fp, 5, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("ftell", ret, OK, ERROR_MSG_OP_FAILED, fclose(fp));

	ret = ftell(fp);
	fclose(fp);
	TC_ASSERT_EQ("ftell", ret, 5);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_feof_tc
* @brief            Check file pointer is positioned at the end of file
* @scenario         Make long size file and print contents until it reaches at the end of file
* @apicovered       fopen, feof
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_feof_tc(void)
{
	FILE *fp;
	char *filename = LONG_FILE_PATH;
	char buf[32];

	TC_ASSERT_NEQ("feof", make_long_file(), ERROR);

	fp = fopen(filename, "r");
	TC_ASSERT("feof", fp);

	/* Print contents of long file, if below became infinite loop, it means failed */
	while (!feof(fp)) {
		fgets(buf, 32, fp);
		printf("%s", buf);
		memset(buf, 0, 32);
	}
	printf("\n");
	fclose(fp);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fprintf_tc
* @brief            Write contents.
* @scenario         Write contents through the fprintf.
* @apicovered       fopen, fprintf
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_fprintf_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_2;
	int ret;

	fp = fopen(filename, "w+");
	TC_ASSERT("fprintf", fp);

	ret = fprintf(fp, "%s", str);
	fclose(fp);
	TC_ASSERT_EQ("fprintf", ret, strlen(VFS_TEST_CONTENTS_2));
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fsetpos_tc
* @brief            Set current file pointer to specific position
* @scenario         Set new position of file pointer & check read contents to check it works properly
* @apicovered       fopen, fsetpos, fgetc
* @precondition     libc_stdio_fprintf_tc should be passed
* @postcondition    NA
*/
static void libc_stdio_fsetpos_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	fpos_t pos = 3;
	int ch, ret;

	fp = fopen(filename, "r");
	TC_ASSERT("fsetpos", fp);

	ret = fsetpos(fp, &pos);
	TC_ASSERT_EQ_CLEANUP("fsetpos", ret, OK, ERROR_MSG_OP_FAILED, fclose(fp));

	ch = fgetc(fp);
	fclose(fp);
	TC_ASSERT_NEQ("fsetpos", ch, EOF);
	/* 'S' is 4th position of "THIS IS VFS TEST 2" */
	TC_ASSERT_EQ("fsetpos", ch, 'S');
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fgetpos_tc
* @brief            Get current file pointer
* @scenario         Set new position of file pointer & check current position by fgetpos
* @apicovered       fopen, fsetpos, fgetpos
* @precondition     libc_stdio_fprintf_tc should be passed
* @postcondition    NA
*/
static void libc_stdio_fgetpos_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	fpos_t pos = 2;
	int ret;

	fp = fopen(filename, "r");
	TC_ASSERT("fsetpos", fp);

	ret = fsetpos(fp, &pos);
	TC_ASSERT_EQ_CLEANUP("fgetpos", ret, OK, ERROR_MSG_OP_FAILED, fclose(fp));

	ret = fgetpos(fp, &pos);
	fclose(fp);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fputc_tc
* @brief            Put character to file
* @scenario         Put character 'S' to file
* @apicovered       fopen, fputc
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_fputc_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ret;

	fp = fopen(filename, "w+");
	TC_ASSERT("fputc", fp);

	ret = fputc('S', fp);
	fclose(fp);
	TC_ASSERT_GEQ("fputc", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fgetc_tc
* @brief            get character from file
* @scenario         get character from file and check it is same as which it put in libc_stdio_fputc_tc
* @apicovered       fopen, fgetc
* @precondition     libc_stdio_fputc_tc should pass
* @postcondition    NA
*/
static void libc_stdio_fgetc_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ch;

	fp = fopen(filename, "r");
	TC_ASSERT("fgetc", fp);

	ch = fgetc(fp);
	fclose(fp);
	TC_ASSERT_NEQ("fgetc", ch, EOF);
	TC_ASSERT_EQ("fgetc", ch, 'S');
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fwrite_tc
* @brief            Write 1 line of contents
* @scenario         Write 3 lines of contents and check its return value
* @apicovered       fopen, fprintf
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_fwrite_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int len, ret;

	fp = fopen(filename, "w+");
	TC_ASSERT("fwrite", fp);

	len = strlen(VFS_TEST_CONTENTS_1);
	ret = fwrite(VFS_TEST_CONTENTS_1, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fwrite", ret, len, ERROR_MSG_BAD_SIZE, fclose(fp));

	len = strlen(VFS_TEST_CONTENTS_2);
	ret = fwrite(VFS_TEST_CONTENTS_2, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fwrite", ret, len, ERROR_MSG_BAD_SIZE, fclose(fp));

	len = strlen(VFS_TEST_CONTENTS_3);
	ret = fwrite(VFS_TEST_CONTENTS_3, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fwrite", ret, len, ERROR_MSG_BAD_SIZE, fclose(fp));

	fclose(fp);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fread_tc
* @brief            Read 1 line of contents
* @scenario         Read 3 lines of contents and check its same as contents which written in libc_stdio_fwrite_tc
* @apicovered       fopen, fread
* @precondition     libc_stdio_fwrite_tc should be passed
* @postcondition    NA
*/
static void libc_stdio_fread_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char buf[20];
	int len, ret;

	fp = fopen(filename, "r");
	TC_ASSERT("fread", fp);

	len = strlen(VFS_TEST_CONTENTS_1);
	memset(buf, 0, sizeof(buf));
	ret = fread(buf, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fread", ret, len, ERROR_MSG_BAD_SIZE, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fread", strcmp(buf, VFS_TEST_CONTENTS_1), 0, ERROR_MSG_DIFFRENT_CONTENTS, fclose(fp));

	len = strlen(VFS_TEST_CONTENTS_2);
	memset(buf, 0, sizeof(buf));
	ret = fread(buf, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fread", ret, len, ERROR_MSG_BAD_SIZE, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fread", strcmp(buf, VFS_TEST_CONTENTS_2), 0, ERROR_MSG_DIFFRENT_CONTENTS, fclose(fp));

	len = strlen(VFS_TEST_CONTENTS_3);
	memset(buf, 0, sizeof(buf));
	ret = fread(buf, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fread", ret, len, ERROR_MSG_BAD_SIZE, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fread", strcmp(buf, VFS_TEST_CONTENTS_3), 0, ERROR_MSG_DIFFRENT_CONTENTS, fclose(fp));

	fclose(fp);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_ferror_tc
* @brief            Check error occured during operation
* @scenario         Write data to file which opened with read only flag to make an error forcely and check ferror works properly
* @apicovered       fopen, fputc, ferror
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void libc_stdio_ferror_tc(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;

	fp = fopen(filename, "r");
	TC_ASSERT("ferror", fp);

	ret = fputc(32, fp);
	TC_ASSERT_EQ_CLEANUP("ferror", ret, EOF, ERROR_MSG_OP_FAILED, fclose(fp));

	ret = ferror(fp);
	fclose(fp);
	TC_ASSERT_NEQ("ferror", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_clearerr_tc
* @brief            Check error cleared by clearerr after error occured during operation
* @scenario         Write data to file which opened with read only flag to make an error forcely and
*                   check ferror works properly and then check error disappeared by clearerr
* @apicovered       fopen, fputc, ferror, clearerr
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void libc_stdio_clearerr_tc(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;

	fp = fopen(filename, "r");
	TC_ASSERT("ferror", fp);

	ret = fputc(32, fp);
	TC_ASSERT_EQ_CLEANUP("ferror", ret, EOF, ERROR_MSG_OP_FAILED, fclose(fp));

	ret = ferror(fp);
	TC_ASSERT_NEQ_CLEANUP("ferror", ret, OK, ERROR_MSG_OP_FAILED, fclose(fp));

	clearerr(fp);

	ret = ferror(fp);
	fclose(fp);
	TC_ASSERT_EQ("ferror", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_gets_tc
* @brief            get string by user input
* @scenario         get string by user input and check it is NULL or not
* @apicovered       gets
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_gets_tc(void)
{
	char input_str[64];

	printf("Enter text here : \n");
	fflush(stdout);
	TC_ASSERT("gets", gets(input_str));
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_gets_s_tc
* @brief            get string by user input
* @scenario         get string by user input and check it is NULL or not
* @apicovered       gets
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_gets_s_tc(void)
{
	char input_str[64];

	printf("Enter text here : \n");
	fflush(stdout);
	TC_ASSERT("gets_s", gets_s(input_str, sizeof(input_str)));
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_fileno_tc
* @brief            Get fd value related to file stream
* @scenario         Open file with fopen and get fd value through the fileno
* @apicovered       fopen, fileno
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_fileno_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int fd;

	fp = fopen(filename, "w");
	TC_ASSERT("fileno", fp);

	fd = fileno(fp);
	fclose(fp);
	TC_ASSERT_GEQ("fileno", fd, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_ungetc_tc
* @brief            Input character into file stream
* @scenario         Get character by fgets and then input again with ungetc. after that compare both of characters
* @apicovered       fopen, fputc, fgetc, ungetc
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_ungetc_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ret;
	int ch1, ch2;

	fp = fopen(filename, "w+");
	TC_ASSERT("ungetc", fp);

	ret = fputc(32, fp);
	fclose(fp);
	TC_ASSERT_NEQ("ungetc", ret, EOF);


	fp = fopen(filename, "r");
	TC_ASSERT("ungetc", fp);

	ch1 = fgetc(fp);
	TC_ASSERT_NEQ_CLEANUP("ungetc", ch1, EOF, ERROR_MSG_DIFFRENT_CONTENTS, fclose(fp));

	ret = ungetc(64, fp);
	TC_ASSERT_NEQ_CLEANUP("ungetc", ret, EOF, ERROR_MSG_OP_FAILED, fclose(fp));

	ch2 = fgetc(fp);
	fclose(fp);
	TC_ASSERT_NEQ("ungetc", ch2, EOF);

	TC_ASSERT_NEQ("ungetc", ch1, ch2);
	TC_SUCCESS_RESULT();
}

static int fs_sample_launcher(int argc, char **args)
{
	total_pass = 0;
	total_fail = 0;

	fs_vfs_umount_tc();
	fs_vfs_mount_tc();
	fs_vfs_open_tc();
	fs_vfs_write_tc();
	fs_vfs_read_tc();
	fs_vfs_close_tc();
	fs_vfs_dup_tc();
	fs_vfs_dup2_tc();
	fs_vfs_fsync_tc();
	fs_vfs_lseek_tc();
	fs_vfs_pwrite_tc();
	fs_vfs_pread_tc();
	fs_vfs_mkdir_tc();
	fs_vfs_opendir_tc();
	fs_vfs_readdir_tc();
	fs_vfs_rewinddir_tc();
	fs_vfs_seekdir_tc();
	fs_vfs_closedir_tc();
	fs_libc_dirent_readdir_r_tc();
	fs_libc_dirent_telldir_tc();
	fs_vfs_rmdir_tc();
	fs_vfs_unlink_tc();
	fs_vfs_stat_tc();
	fs_vfs_statfs_tc();
#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
	fs_vfs_mkfifo_tc();
#endif
	fs_vfs_sendfile_tc();
	fs_vfs_fcntl_tc();
#ifndef CONFIG_DISABLE_POLL
	fs_vfs_poll_tc();
	fs_vfs_select_tc();
#endif
	fs_vfs_rename_tc();
	fs_vfs_ioctl_tc();
#ifdef CONFIG_TC_FS_PROCFS
	tc_fs_procfs_main();
#endif
	libc_stdio_fdopen_tc();
	libc_stdio_fopen_tc();
	libc_stdio_fclose_tc();
	libc_stdio_fputs_tc();
	libc_stdio_fgets_tc();
	libc_stdio_fseek_tc();
	libc_stdio_ftell_tc();
	libc_stdio_feof_tc();
	libc_stdio_fprintf_tc();
	libc_stdio_fsetpos_tc();
	libc_stdio_fgetpos_tc();
	libc_stdio_fputc_tc();
	libc_stdio_fgetc_tc();
	libc_stdio_fwrite_tc();
	libc_stdio_fread_tc();
	libc_stdio_ferror_tc();
	libc_stdio_clearerr_tc();
	libc_stdio_gets_tc();
	libc_stdio_gets_s_tc();
	libc_stdio_fileno_tc();
	libc_stdio_ungetc_tc();

	printf("#########################################\n");
	printf("           FS TC Result               \n");
	printf("           PASS : %d FAIL : %d        \n",
		   total_pass, total_fail);
	printf("#########################################\n");
	return total_pass;
}


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int fs_main(int argc, char *argv[])
#endif
{
	sem_wait(&tc_sem);
	working_tc++;

#ifdef CONFIG_TASH
	tash_cmd_install("fs_sample", fs_sample_launcher, TASH_EXECMD_SYNC);
#else
	fs_sample_launcher(argc, argv);
#endif

	working_tc--;
	sem_post(&tc_sem);

	return 0;
}



