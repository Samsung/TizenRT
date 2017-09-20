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

#include <tinyara/config.h>
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
#include <sys/types.h>

#include <tinyara/streams.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/fs_utils.h>
#include <apps/shell/tash.h>
#include <time.h>
#include "tc_common.h"
#include "tc_internal.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define BUFLEN 64

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

#define VFS_INVALID_FILE_PATH MOUNT_DIR"noexistfile"

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

#define INV_FD -3

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

	/* Nagative case with invalid argument, not existing pathname. It will return ERROR */
	fd = open(VFS_INVALID_FILE_PATH, O_WROK);
	TC_ASSERT_LT_CLEANUP("open", fd, 0, close(fd));

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

	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC);
	TC_ASSERT_GEQ("open", fd, 0);
	ret = write(fd, buf, strlen(buf));
	close(fd);
	TC_ASSERT_EQ("write", ret, strlen(buf));

	/* Nagative case with invalid argument, no write access. It will return ERROR */
	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);
	ret = write(fd, buf, sizeof(buf));
	TC_ASSERT_LT_CLEANUP("write", ret, 0, close(fd));
	close(fd);

	/* Nagative case with invalid argument, fd. It will return ERROR */
	ret = write(CONFIG_NFILE_DESCRIPTORS, buf, sizeof(buf));
	TC_ASSERT_EQ("write", ret, ERROR);

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
	TC_ASSERT_GEQ("open", fd, 0);
	memset(buf, 0, sizeof(buf));
	ret = read(fd, buf, sizeof(buf));
	close(fd);
	TC_ASSERT_GEQ("read", ret, 0);
	TC_ASSERT_EQ("read", strcmp(buf, VFS_TEST_CONTENTS_1), 0);

	/* Nagative case with invalid argument, no read access. It will return ERROR */
	fd = open(VFS_FILE_PATH, O_WRONLY);
	TC_ASSERT_GEQ("open", fd, 0);
	memset(buf, 0, sizeof(buf));
	ret = read(fd, buf, sizeof(buf));
	close(fd);
	TC_ASSERT_LT("read", ret, 0);

	/* Nagative case with invalid argument, fd. It will return ERROR */
	ret = read(CONFIG_NFILE_DESCRIPTORS, buf, sizeof(buf));
	TC_ASSERT_EQ("read", ret, ERROR);

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
	TC_ASSERT_GEQ("open", fd, 0);
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
	TC_ASSERT_GEQ("open", fd1, 0);

	len = strlen(str1);
	ret = write(fd1, str1, len);
	TC_ASSERT_EQ_CLEANUP("write", ret, len, close(fd1));

	fd2 = dup(fd1);
	close(fd1);
	TC_ASSERT_GEQ("dup", fd2, 0);

	len = strlen(str2);
	ret = write(fd2, str2, strlen(str2));
	close(fd2);
	TC_ASSERT_EQ("write", ret, len);

	fd1 = open(filename, O_RDOK);
	TC_ASSERT_GEQ("open", fd1, 0);

	len = strlen(str1);
	ret = read(fd1, buf, len);

	TC_ASSERT_GT_CLEANUP("read", ret, 0, close(fd1));
	TC_ASSERT_EQ_CLEANUP("read", strcmp(buf, VFS_TEST_CONTENTS_1), 0, close(fd1));

	memset(buf, 0, sizeof(buf));
	len = strlen(str2);
	ret = read(fd1, buf, len);
	close(fd1);

	TC_ASSERT_GT("read", ret, 0);
	TC_ASSERT_EQ("read", strcmp(buf, VFS_TEST_CONTENTS_2), 0);

	/* Nagative case with invalid argument, invalid fd. It will return ERROR */
#if CONFIG_NFILE_DESCRIPTORS > 0
	fd1 = dup(CONFIG_NFILE_DESCRIPTORS);
	TC_ASSERT_LT_CLEANUP("dup", fd1, 0, close(fd1));
#endif

#if defined(CONFIG_NET) && CONFIG_NSOCKET_DESCRIPTORS > 0
	fd1 = dup(CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS);
	TC_ASSERT_LT_CLEANUP("dup", fd1, 0, close(fd1));
#endif

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
	TC_ASSERT_GEQ("open", fd1, 0);

	fd2 = open(filename2, O_RDWR | O_CREAT);
	close(fd1);
	TC_ASSERT_GEQ("open", fd2, 0);

	/* now fd1 points fd2 */
	ret = dup2(fd2, fd1);
	close(fd2);
	TC_ASSERT_NEQ("dup2", ret, ERROR);
	TC_ASSERT_GEQ("dup2", fd1, 0);

	len = strlen(VFS_TEST_CONTENTS_3);
	ret = write(fd1, str, len);
	close(fd1);
	TC_ASSERT_EQ("write", ret, len);

	fd2 = open(filename2, O_RDONLY);
	TC_ASSERT_GEQ("open", fd2, 0);

	ret = read(fd2, buf, len);
	close(fd2);
	TC_ASSERT_GT("read", ret, 0);
	TC_ASSERT_EQ("read", strcmp(buf, VFS_TEST_CONTENTS_3), 0);

	/* Nagative case with invalid argument, invalid fd. It will return ERROR */
	fd1 = -1;
	ret = dup2(CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS, fd1);
	close(fd1);
	TC_ASSERT_LT("dup2", fd1, 0);
	TC_ASSERT_EQ("dup2", ret, ERROR);

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
	TC_ASSERT_GEQ("open", fd, 0);

	len = strlen(str);
	ret = write(fd, str, len);
	TC_ASSERT_EQ_CLEANUP("write", ret, len, close(fd));

	ret = fsync(fd);
	close(fd);
	TC_ASSERT_GEQ("fsync", ret, 0);

	/* Nagative case with invalid argument, no write access. It will return ERROR */
	fd = open(filename, O_RDOK);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = fsync(fd);
	close(fd);
	TC_ASSERT_EQ("fsync", ret, ERROR);

	/* Nagative case with invalid argument, fd. It will return ERROR */
	ret = fsync(CONFIG_NFILE_DESCRIPTORS);
	TC_ASSERT_EQ("fsync", ret, ERROR);

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
	TC_ASSERT_GEQ("open", fd, 0);

	ret = lseek(fd, 5, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("lseek", ret, 5, close(fd));

	memset(buf, 0, sizeof(buf));
	ret = read(fd, buf, sizeof(buf));
	close(fd);

	TC_ASSERT_GT("read", ret, 0);
	TC_ASSERT_EQ("read", strcmp(buf, "IS VFS TEST 2"), 0);

	/* Nagative case with invalid argument, fd. It will return ERROR */
	ret = lseek(CONFIG_NFILE_DESCRIPTORS, 5, SEEK_SET);
	TC_ASSERT_EQ("lseek", ret, ERROR);

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
	TC_ASSERT_GEQ("open", fd, 0);

	ret = pwrite(fd, str, strlen(str), 10);
	close(fd);
	TC_ASSERT_NEQ("pwrite", ret, ERROR);

	/* Nagative case with invalid argument, fd. It will return ERROR */
	ret = pwrite(CONFIG_NFILE_DESCRIPTORS, str, strlen(str), 10);
	TC_ASSERT_EQ("pwrite", ret, ERROR);

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
	TC_ASSERT_GEQ("open", fd, 0);

	memset(buf, 0, sizeof(buf));
	ret = pread(fd, buf, 20, 10);
	close(fd);
	TC_ASSERT_GT("pread", ret, 0);
	TC_ASSERT_EQ("pread", strcmp(buf, str), 0);

	/* Nagative case with invalid argument, fd. It will return ERROR */
	ret = pread(CONFIG_NFILE_DESCRIPTORS, buf, 20, 10);
	TC_ASSERT_EQ("pread", ret, ERROR);

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
	TC_ASSERT_NEQ("opendir", dirp, NULL);
	closedir(dirp);
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
	TC_ASSERT_NEQ("opendir", dirp, NULL);

	count = 0;
	while (1) {
		dirent = readdir(dirp);
		if (dirent == NULL) {
			break;
		}
		count++;
	}
	ret = closedir(dirp);
	TC_ASSERT_EQ("closedir", ret, OK);
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
	TC_ASSERT_NEQ("opendir", dirp, NULL);

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
	TC_ASSERT_EQ("closedir", ret, OK);
	TC_ASSERT_EQ("rewinddir", count, VFS_LOOP_COUNT * 2);
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
	TC_ASSERT_NEQ("opendir", dirp, NULL);

	offset = 2;
	seekdir(dirp, offset);
	TC_ASSERT_NEQ_CLEANUP("seekdir", dirp, NULL, closedir(dirp));
	dirent = readdir(dirp);
	TC_ASSERT_NEQ_CLEANUP("readdir", dirent, NULL, closedir(dirp));
	TC_ASSERT_EQ_CLEANUP("readdir", dirent->d_type, DTYPE_DIRECTORY, closedir(dirp));

	ret = closedir(dirp);
	TC_ASSERT_EQ("closedir", ret, OK);

	itoa((int)offset, filename, 10);
	TC_ASSERT_EQ("readdir", strncmp(dirent->d_name, filename, 1), 0);

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
	TC_ASSERT_NEQ("opendir", dirp, NULL);

	count = 0;
	while (1) {
		ret = readdir_r(dirp, &entry, &result);
		if (ret != 0 || result == NULL) {
			break;
		}
		count++;
	}
	ret = closedir(dirp);
	TC_ASSERT_EQ("closedir", ret, OK);
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
	TC_ASSERT_NEQ("opendir", dirp, NULL);

	offset = 2;
	seekdir(dirp, offset);
	TC_ASSERT_NEQ_CLEANUP("seekdir", dirp, NULL, closedir(dirp));
	res = telldir(dirp);
	TC_ASSERT_EQ_CLEANUP("telldir", res, offset, closedir(dirp));
	ret = closedir(dirp);
	TC_ASSERT_EQ("closedir", ret, OK);

	/* Nagative case with invalid argument, NULL stream. It will return (off_t)-1 */
	res = telldir(NULL);
	TC_ASSERT_EQ("telldir", res, (off_t)-1);

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
	TC_ASSERT_NEQ("opendir", dirp, NULL);

	ret = closedir(dirp);
	TC_ASSERT_EQ("closedir", ret, OK);

	dirp = opendir("nodir");
	TC_ASSERT_EQ_CLEANUP("opendir", dirp, NULL, closedir(dirp));

	ret = closedir(NULL);
	TC_ASSERT_EQ("closedir", ret, ERROR);

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

	/* Nagative case with invalid argument, NULL pathname. It will return ERROR */
	ret = rmdir(NULL);
	TC_ASSERT_EQ("rmdir", ret, ERROR);

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

	/* Nagative case with invalid argument, NULL pathname. It will return ERROR */
	ret = unlink(NULL);
	TC_ASSERT_EQ("unlink", ret, ERROR);

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

	/* stat root directory */
	ret = stat("/", &st);
	TC_ASSERT_EQ("stat", ret, OK);

	/* Nagative case with invalid argument, NULL pathname. It will return ERROR */
	ret = stat(NULL, &st);
	TC_ASSERT_EQ("stat", ret, ERROR);

	/* Nagative case with invalid argument, not existing pathname. It will return ERROR */
	ret = stat(VFS_INVALID_FILE_PATH, &st);
	TC_ASSERT_EQ("stat", ret, ERROR);

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

	/* Nagative case with invalid argument, NULL pathname. It will return ERROR */
	ret = statfs(NULL, &fs);
	TC_ASSERT_EQ("statfs", ret, ERROR);

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
	g_thread_result = true;

	ret = mkfifo(FIFO_FILE_PATH, 0666);
	if (ret < 0) {
		TC_ASSERT_EQ("mkfifo", ret, -EEXIST);
	}

	fd = open(FIFO_FILE_PATH, O_WRONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = pthread_create(&tid, NULL, (pthread_startroutine_t)mkfifo_test_listener, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_create", ret, 0, close(fd));

	size = strlen(FIFO_DATA) + 2;
	count = 1;
	while (count < 4) {
		if (!g_thread_result) {
			break;
		}
		snprintf(buf, size, "%s%d\0", FIFO_DATA, count);
		ret = write(fd, buf, size);
		TC_ASSERT_GT_CLEANUP("write", ret, 0, goto errout);
		count++;
		sleep(5);
	}
	close(fd);
	pthread_kill(tid, SIGUSR1);
	TC_ASSERT_EQ("mkfifo", g_thread_result, true);
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
	char dest_file[16];
	struct stat st;
	int fd1, fd2, ret;
	off_t size;
	off_t offset;

	snprintf(dest_file, sizeof(dest_file), "%s_dest", src_file);

	fd1 = open(src_file, O_RDONLY);
	TC_ASSERT_GEQ("open", fd1, 0);

	ret = stat(src_file, &st);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, close(fd1));

	size = st.st_size;
	/* case-1: offset = 0 */
	fd2 = open(dest_file, O_WRONLY | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd2, 0, close(fd1));

	ret = sendfile(fd2, fd1, 0, size);
	close(fd2);
	TC_ASSERT_EQ_CLEANUP("sendfile", ret, size, close(fd1));

	ret = stat(dest_file, &st);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, close(fd1));
	TC_ASSERT_EQ_CLEANUP("stat", st.st_size, size, close(fd1));

	/* case-2: offset = 1 */
	fd2 = open(dest_file, O_WRONLY | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd2, 0, close(fd1));

	offset = 1;
	ret = sendfile(fd2, fd1, &offset, size - 1);
	close(fd2);
	TC_ASSERT_EQ_CLEANUP("sendfile", ret, size - 1, close(fd1));

	ret = stat(dest_file, &st);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, close(fd1));
	TC_ASSERT_EQ_CLEANUP("stat", st.st_size, size - 1, close(fd1));

	/* case-3: offset = 1, invalid input fd, returns ERROR */
	fd2 = open(dest_file, O_WRONLY | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd2, 0, close(fd1));

	offset = 1;
	ret = sendfile(fd2, INV_FD, &offset, size - 1);
	close(fd2);
	TC_ASSERT_EQ_CLEANUP("sendfile", ret, ERROR, close(fd1));

	/* case-4: invalid input fd, returns ERROR */
	fd2 = open(dest_file, O_WRONLY | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd2, 0, close(fd1));

	ret = sendfile(fd2, INV_FD, NULL, size);
	close(fd2);
	TC_ASSERT_EQ_CLEANUP("sendfile", ret, ERROR, close(fd1));

	/* case-5: offset = 0, invalid output fd, returns ERROR */
	offset = 0;
	ret = sendfile(INV_FD, fd1, &offset, size);
	TC_ASSERT_EQ_CLEANUP("sendfile", ret, ERROR, close(fd1));

	/* case-6: current offset of input file is EOF, returns ERROR */
	fd2 = open(dest_file, O_WRONLY | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd2, 0, close(fd1));

	ret = lseek(fd1, 0, SEEK_END);
	TC_ASSERT_EQ_CLEANUP("lseek", ret, size, close(fd1); close(fd2));

	ret = sendfile(fd2, fd1, NULL, size);
	close(fd2);
	TC_ASSERT_EQ_CLEANUP("sendfile", ret, 0, close(fd1));

	ret = stat(dest_file, &st);
	close(fd1);
	TC_ASSERT_EQ("stat", ret, OK);
	TC_ASSERT_EQ("stat", st.st_size, 0);

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
	TC_ASSERT_GEQ("open", fd, 0);

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
	TC_ASSERT_NEQ_CLEANUP("select", errcnt, VFS_LOOP_COUNT, FD_CLR(STDIN_FILENO, &readfds));
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
	int fd;
	int ret;
	char *old_file = VFS_FILE_PATH;
	char new_file[12];

	snprintf(new_file, 12, "%s_re", old_file);
	unlink(new_file);

	ret = rename(old_file, new_file);
	TC_ASSERT_EQ("rename", ret, OK);

	/* Nagative case with invalid argument, not existing old pathname. It will return ERROR */
	ret = rename(old_file, new_file);
	TC_ASSERT_EQ("rename", ret, ERROR);

	old_file = new_file;

	/* Nagative case with invalid argument, already existing new pathname. It will return ERROR */
	fd = open(VFS_FILE_PATH, O_WROK | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);
	close(fd);
	ret = rename(old_file, VFS_FILE_PATH);
	TC_ASSERT_EQ("rename", ret, ERROR);

	/* Nagative case with invalid argument, NULL filepath. It will return ERROR */
	ret = rename(old_file, NULL);
	TC_ASSERT_EQ("rename", ret, ERROR);

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
	TC_ASSERT_GEQ("open", fd, 0);
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
	TC_ASSERT_GEQ("open", fd, 0);

	fp = fdopen(fd, "r");
	close(fd);
	TC_ASSERT_NEQ("fdopen", fp, NULL);
	TC_ASSERT_EQ_CLEANUP("fdopen", fp->fs_oflags, O_RDONLY, fclose(fp));
	fclose(fp);

	/* Nagative case with invalid argument, negative fd value. It will return NULL */
	fp = fdopen(-1, "r");
	TC_ASSERT_EQ_CLEANUP("fdopen", fp, NULL, fclose(fp));

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
	TC_ASSERT_NEQ("fopen", fp, NULL);
	fclose(fp);

	fp = fopen(filename, "r+");
	TC_ASSERT_NEQ("fopen", fp, NULL);
	fclose(fp);

	fp = fopen(filename, "rb");
	TC_ASSERT_NEQ("fopen", fp, NULL);
	fclose(fp);

	fp = fopen(filename, "rx");
	TC_ASSERT_NEQ("fopen", fp, NULL);
	fclose(fp);

	/* Nagative cases with invalid mode. It will return NULL */
	fp = fopen(filename, "b");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp));

	fp = fopen(filename, "x");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp));

	fp = fopen(filename, "z");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp));

	fp = fopen(filename, "+");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp));

	fp = fopen(filename, "rw");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp));

	fp = fopen(filename, "wr");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp));

	fp = fopen(filename, "wa");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp));

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
	TC_ASSERT_NEQ("fopen", fp, NULL);
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
	TC_ASSERT_NEQ("fopen", fp, NULL);
	TC_ASSERT_EQ_CLEANUP("fputs", fputs(str, fp), strlen(str), fclose(fp));

	/* Nagative case with invalid argument, NULL stream. It will return EOF */
	TC_ASSERT_EQ_CLEANUP("fputs", fputs(NULL, fp), EOF, fclose(fp));

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
	TC_ASSERT_NEQ("fopen", fp, NULL);

	memset(buf, 0, sizeof(buf));
	TC_ASSERT_NEQ_CLEANUP("fgets", fgets(buf, 20, fp), NULL, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fgets", strcmp(buf, VFS_TEST_CONTENTS_1), 0, fclose(fp));

	/* Nagative case with invalid argument, negative buffer size. It will return NULL */
	TC_ASSERT_EQ_CLEANUP("fgets", fgets(buf, -1, fp), NULL, fclose(fp));
	fclose(fp);
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
	TC_ASSERT_NEQ("fopen", fp, NULL);

	ret = fseek(fp, 5, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("fseek", ret, OK, fclose(fp));

	memset(buf, 0, sizeof(buf));
	TC_ASSERT_NEQ_CLEANUP("fgets", fgets(buf, 20, fp), NULL, fclose(fp));
	fclose(fp);
	TC_ASSERT_EQ("fgets", strcmp(buf, "IS VFS TEST 1"), 0);

	/* Nagative case with invalid argument, NULL stream. It will return ERROR */
	ret = fseek(NULL, 5, SEEK_SET);
	TC_ASSERT_EQ("fseek", ret, ERROR);

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
	TC_ASSERT_NEQ("fopen", fp, NULL);

	ret = fseek(fp, 5, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("fseek", ret, OK, fclose(fp));

	ret = ftell(fp);
	fclose(fp);
	TC_ASSERT_EQ("ftell", ret, 5);

	/* Nagative case with invalid argument, NULL stream. It will return ERROR */
	ret = ftell(NULL);
	TC_ASSERT_EQ("ftell", ret, ERROR);

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

	TC_ASSERT_NEQ("make_long_file", make_long_file(), ERROR);

	fp = fopen(filename, "r");
	TC_ASSERT_NEQ("fopen", fp, NULL);

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
	TC_ASSERT_NEQ("fopen", fp, NULL);

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
	TC_ASSERT_NEQ("fopen", fp, NULL);

	ret = fsetpos(fp, &pos);
	TC_ASSERT_EQ_CLEANUP("fsetpos", ret, OK, fclose(fp));

	ch = fgetc(fp);

	TC_ASSERT_NEQ_CLEANUP("fgetc", ch, EOF, fclose(fp));
	/* 'S' is 4th position of "THIS IS VFS TEST 2" */
	TC_ASSERT_EQ_CLEANUP("fgetc", ch, 'S', fclose(fp));

	/* Nagative case with invalid arguments, NULL position. It will return ERROR */
	ret = fsetpos(fp, NULL);
	fclose(fp);
	TC_ASSERT_EQ("fsetpos", ret, ERROR);

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
	TC_ASSERT_NEQ("fopen", fp, NULL);

	ret = fsetpos(fp, &pos);
	TC_ASSERT_EQ_CLEANUP("fsetpos", ret, OK, fclose(fp));

	ret = fgetpos(fp, &pos);
	fclose(fp);
	TC_ASSERT_EQ("fgetpos", ret, OK);

	/* Nagative case with invalid arguments. It will return ERROR */
	ret = fgetpos(NULL, NULL);
	TC_ASSERT_EQ("fgetpos", ret, ERROR);

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
	TC_ASSERT_NEQ("fopen", fp, NULL);

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
	TC_ASSERT_NEQ("fopen", fp, NULL);

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
	TC_ASSERT_NEQ("fopen", fp, NULL);

	len = strlen(VFS_TEST_CONTENTS_1);
	ret = fwrite(VFS_TEST_CONTENTS_1, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fwrite", ret, len, fclose(fp));

	len = strlen(VFS_TEST_CONTENTS_2);
	ret = fwrite(VFS_TEST_CONTENTS_2, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fwrite", ret, len, fclose(fp));

	len = strlen(VFS_TEST_CONTENTS_3);
	ret = fwrite(VFS_TEST_CONTENTS_3, 1, len, fp);
	fclose(fp);
	TC_ASSERT_EQ("fwrite", ret, len);

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
	TC_ASSERT_NEQ("fopen", fp, NULL);

	len = strlen(VFS_TEST_CONTENTS_1);
	memset(buf, 0, sizeof(buf));
	ret = fread(buf, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fread", ret, len, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fread", strcmp(buf, VFS_TEST_CONTENTS_1), 0, fclose(fp));

	len = strlen(VFS_TEST_CONTENTS_2);
	memset(buf, 0, sizeof(buf));
	ret = fread(buf, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fread", ret, len, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fread", strcmp(buf, VFS_TEST_CONTENTS_2), 0, fclose(fp));

	len = strlen(VFS_TEST_CONTENTS_3);
	memset(buf, 0, sizeof(buf));
	ret = fread(buf, 1, len, fp);
	fclose(fp);
	TC_ASSERT_EQ("fread", ret, len);
	TC_ASSERT_EQ("fread", strcmp(buf, VFS_TEST_CONTENTS_3), 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_freopen_tc
* @brief            Open file by freopen
* @scenario         Open file
* @apicovered       freopen
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_freopen_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	fp = freopen(filename, "w", NULL);
	TC_ASSERT_NEQ("freopen", fp, NULL);

	/* A case with non-NULL filename and valid stream */
	fp = freopen(filename, "w", fp);
	TC_ASSERT_NEQ("freopen", fp, NULL);

	/* A case with NULL filename, valid stream and mode flag */
	fp = freopen(NULL, "r", fp);
	fclose(fp);
	TC_ASSERT_NEQ("freopen", fp, NULL);

	/* A negative case with NULL path, valid stream and invalid mode. It will return NULL */
	fp = freopen(NULL, "z", fp);
	TC_ASSERT_EQ_CLEANUP("freopen", fp, NULL, fclose(fp));

	/* A negative case with NULL path and stream. It will return NULL */
	fp = freopen(NULL, "w", NULL);
	TC_ASSERT_EQ_CLEANUP("freopen", fp, NULL, fclose(fp));

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
	TC_ASSERT_NEQ("fopen", fp, NULL);

	ret = fputc(32, fp);
	TC_ASSERT_EQ_CLEANUP("fputc", ret, EOF, fclose(fp));

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
	TC_ASSERT_NEQ("fopen", fp, NULL);

	ret = fputc(32, fp);
	TC_ASSERT_EQ_CLEANUP("fputc", ret, EOF, fclose(fp));

	ret = ferror(fp);
	TC_ASSERT_NEQ_CLEANUP("ferror", ret, OK, fclose(fp));

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
	TC_ASSERT_NEQ("gets", gets(input_str), NULL);
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
	TC_ASSERT_NEQ("gets_s", gets_s(input_str, sizeof(input_str)), NULL);
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
	TC_ASSERT_NEQ("fopen", fp, NULL);

	fd = fileno(fp);
	fclose(fp);
	TC_ASSERT_GEQ("fileno", fd, 0);

	/* Nagative case with invalid argument, NULL stream. It will return ERROR */
	fd = fileno(NULL);
	TC_ASSERT_EQ("fileno", fd, ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_remove_tc
* @brief            Deletes the file whose name is specified in filename.
* @scenario         Open file
* @apicovered       remove
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_remove_tc(void)
{
	char *filename = VFS_FILE_PATH;
	int ret = NULL;

	ret = remove(filename);
	TC_ASSERT_EQ("remove", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_setbuf_tc
* @brief            Set stream buffer
* @scenario         Open file
* @apicovered       setbuf, setvbuf
* @precondition     NA
* @postcondition    NA
*/
#if CONFIG_STDIO_BUFFER_SIZE > 0
static void libc_stdio_setbuf_tc(void)
{
	FILE *fp;
	char buffer[64];
	char *filename = VFS_FILE_PATH;

	/* setbuf_test: DEFAULT buffering */

	fp = fopen(filename, "w");
	TC_ASSERT_NEQ("fopen", fp, NULL);

	/* setbuf_test: NO buffering */

	setbuf(fp, NULL);
	TC_ASSERT_EQ_CLEANUP("setbuf", fp->fs_bufstart, NULL, fclose(fp));

	/* setbuf_test: pre-allocated buffer */

	setbuf(fp, buffer);
	TC_ASSERT_EQ_CLEANUP("setbuf", fp->fs_bufstart, (FAR unsigned char *)buffer, fclose(fp));
	setbuf(fp, NULL);
	TC_ASSERT_EQ_CLEANUP("setbuf", fp->fs_bufstart, NULL, fclose(fp));

	fclose(fp);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @testcase         libc_stdio_setvbuf_tc
* @brief            Change stream buffering
* @scenario         Open file
* @apicovered       setbuf, setvbuf
* @precondition     NA
* @postcondition    NA
*/
#if CONFIG_STDIO_BUFFER_SIZE > 0
static void libc_stdio_setvbuf_tc(void)
{
	FILE *fp;
	char buffer[64];
	char *filename = VFS_FILE_PATH;
	int ret = NULL;

	/* setvbuf_test: DEFAULT buffering */

	fp = fopen(filename, "w");
	TC_ASSERT_NEQ("fopen", fp, NULL);

	/* setvbuf_test: NO buffering */

	ret = setvbuf(fp, NULL, _IONBF, 0);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp));

	/* setvbuf_test: FULL buffering */

	ret = setvbuf(fp, NULL, _IOFBF, 0);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp));
	ret = setvbuf(fp, NULL, _IONBF, 0);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp));

	/* setvbuf_test: LINE buffering */

	ret = setvbuf(fp, NULL, _IOLBF, 64);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp));
	ret = setvbuf(fp, NULL, _IONBF, 0);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp));

	/* setvbuf_test: FULL buffering, pre-allocated buffer */

	ret = setvbuf(fp, buffer, _IOFBF, 64);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp));
	ret = setvbuf(fp, NULL, _IONBF, 0);
	fclose(fp);
	TC_ASSERT_LEQ("setvbuf", ret, 0);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @testcase         libc_stdio_meminstream_tc
* @brief            Initializes a stream for use with a fixed-size memory buffer
* @scenario         Initializes a stream for use with a fixed-size memory buffer
* @apicovered       lib_meminstream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_meminstream_tc(void)
{
	FAR char buf[BUFLEN];

	struct lib_meminstream_s meminstream;

	lib_meminstream((FAR struct lib_meminstream_s *)&meminstream, buf, BUFLEN);
	TC_ASSERT_EQ("lib_meminstream", meminstream.buffer, (FAR char *)(buf));
	TC_ASSERT_EQ("lib_meminstream", meminstream.buflen, BUFLEN);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_memoutstream_tc
* @brief            Initializes a stream for use with a fixed-size memory buffer
* @scenario         Initializes a stream for use with a fixed-size memory buffer
* @apicovered       lib_memoutstream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_memoutstream_tc(void)
{
	FAR char buf[BUFLEN];

	struct lib_memoutstream_s memoutstream;

	lib_memoutstream((FAR struct lib_memoutstream_s *)&memoutstream, buf, BUFLEN);
	TC_ASSERT_EQ("lib_memoutstream", memoutstream.buffer, (FAR char *)(buf));
	TC_ASSERT_EQ("lib_memoutstream", memoutstream.buflen, (BUFLEN - 1));	/* Save space for null terminator, hence checing with (BUFLEN-1)*/

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_memsistream_tc
* @brief            Initializes a stream for use with a fixed-size memory buffer
* @scenario         Initializes a stream for use with a fixed-size memory buffer
* @apicovered       lib_memsistream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_memsistream_tc(void)
{
	FAR char buf[BUFLEN];

	struct lib_memsistream_s memsistream;

	lib_memsistream((FAR struct lib_memsistream_s *)&memsistream, buf, BUFLEN);
	TC_ASSERT_EQ("lib_memsistream", memsistream.buffer, (FAR char *)(buf));

	lib_memsistream((FAR struct lib_memsistream_s *)&memsistream, buf + 2, BUFLEN);
	TC_ASSERT_EQ("lib_memsistream", (memsistream.buffer - (FAR char *)(buf)), 2);
	TC_ASSERT_EQ("lib_memsistream", memsistream.buflen, BUFLEN);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_memsostream_tc
* @brief            Initializes a stream for use with a fixed-size memory buffer
* @scenario         Initializes a stream for use with a fixed-size memory buffer
* @apicovered       lib_memsostream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_memsostream_tc(void)
{
	FAR char buf[BUFLEN];

	struct lib_memsostream_s memsostream;

	lib_memsostream((FAR struct lib_memsostream_s *)&memsostream, buf, BUFLEN);
	TC_ASSERT_EQ("lib_memsostream", memsostream.buffer, (FAR char *)(buf));

	lib_memsostream((FAR struct lib_memsostream_s *)&memsostream, buf + 4, BUFLEN);
	TC_ASSERT_EQ("lib_memsostream", (memsostream.buffer - (FAR char *)(buf)), 4);
	TC_ASSERT_EQ("lib_memsostream", memsostream.buflen, (BUFLEN - 1));	/* Save space for null terminator, hence checing with (BUFLEN-1)*/

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_nullinstream_tc
* @brief            Initializes a NULL stream. The initialized stream will return only EOF
* @scenario         Initializes a NULL stream. The initialized stream will return only EOF
* @apicovered       lib_nullinstream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_nullinstream_tc(void)
{
	struct lib_instream_s nullinstream;

	lib_nullinstream((FAR struct lib_instream_s *)&nullinstream);
	TC_ASSERT_EQ("lib_nullinstream", nullinstream.nget, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_nulloutstream_tc
* @brief            Initializes a NULL stream. The initialized stream will write all data to the bit-bucket
* @scenario         Initializes a NULL stream. The initialized stream will write all data to the bit-bucket
* @apicovered       lib_nulloutstream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_nulloutstream_tc(void)
{
	struct lib_outstream_s nulloutstream;

	lib_nulloutstream((FAR struct lib_outstream_s *)&nulloutstream);
	TC_ASSERT_EQ("lib_nulloutstream", nulloutstream.nput, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_rawinstream_tc
* @brief            Initializes a stream for use with a file descriptor
* @scenario         Initializes a stream for use with a file descriptor
* @apicovered       lib_rawinstream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_rawinstream_tc(void)
{
	int fd;
	char *filename = VFS_FILE_PATH;

	struct lib_rawinstream_s rawinstream;

	fd = open(filename, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	lib_rawinstream((FAR struct lib_rawinstream_s *)&rawinstream, fd);
	close(fd);
	TC_ASSERT_EQ("lib_rawinstream", rawinstream.fd, fd);
	TC_ASSERT_EQ("lib_rawinstream", rawinstream.public.nget, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_rawoutstream_tc
* @brief            Initializes a stream for use with a file descriptor
* @scenario         Initializes a stream for use with a file descriptor
* @apicovered       lib_rawoutstream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_rawoutstream_tc(void)
{
	int fd;
	char *filename = VFS_FILE_PATH;

	struct lib_rawoutstream_s rawoutstream;

	fd = open(filename, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	lib_rawoutstream((FAR struct lib_rawoutstream_s *)&rawoutstream, fd);
	close(fd);
	TC_ASSERT_EQ("lib_rawoutstream", rawoutstream.fd, fd);
	TC_ASSERT_EQ("lib_rawoutstream", rawoutstream.public.nput, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_rawsistream_tc
* @brief            Initializes a stream for use with a file descriptor
* @scenario         Initializes a stream for use with a file descriptor
* @apicovered       lib_rawsistream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_rawsistream_tc(void)
{
	int fd;
	char *filename = VFS_FILE_PATH;

	struct lib_rawsistream_s rawsistream;

	fd = open(filename, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	lib_rawsistream((FAR struct lib_rawsistream_s *)&rawsistream, fd);
	close(fd);
	TC_ASSERT_EQ("lib_rawsistream", rawsistream.fd, fd);
	TC_ASSERT_EQ("lib_rawsistream", rawsistream.public.nget, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_rawsostream_tc
* @brief            Initializes a stream for use with a file descriptor
* @scenario         Initializes a stream for use with a file descriptor
* @apicovered       lib_rawsostream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_rawsostream_tc(void)
{
	int fd;
	char *filename = VFS_FILE_PATH;

	struct lib_rawsostream_s rawsostream;

	fd = open(filename, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	lib_rawsostream((FAR struct lib_rawsostream_s *)&rawsostream, fd);
	close(fd);
	TC_ASSERT_EQ("lib_rawsostream", rawsostream.fd, fd);
	TC_ASSERT_EQ("lib_rawsostream", rawsostream.public.nput, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_stdinstream_tc
* @brief            Initializes a stream for use with a FILE instance
* @scenario         Initializes a stream for use with a FILE instance
* @apicovered       lib_stdinstream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_stdinstream_tc(void)
{
	FILE *stream;
	char *filename = VFS_FILE_PATH;

	struct lib_stdinstream_s stdinstream;

	stream = fopen(filename, "w");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	lib_stdinstream((FAR struct lib_stdinstream_s *)&stdinstream, stream);
	fclose(stream);
	TC_ASSERT_EQ("lib_stdinstream", stdinstream.stream, stream);
	TC_ASSERT_EQ("lib_stdinstream", stdinstream.public.nget, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_stdoutstream_tc
* @brief            Initializes a stream for use with a FILE instance
* @scenario         Initializes a stream for use with a FILE instance
* @apicovered       lib_stdoutstream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_stdoutstream_tc(void)
{
	FILE *stream;
	char *filename = VFS_FILE_PATH;

	struct lib_stdoutstream_s stdoutstream;

	stream = fopen(filename, "w");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	lib_stdoutstream((FAR struct lib_stdoutstream_s *)&stdoutstream, stream);
	fclose(stream);
	TC_ASSERT_EQ("lib_stdoutstream", stdoutstream.stream, stream);
	TC_ASSERT_EQ("lib_stdoutstream", stdoutstream.public.nput, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_stdsistream_tc
* @brief            Initializes a stream for use with a FILE instance
* @scenario         Initializes a stream for use with a FILE instance
* @apicovered       lib_stdsistream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_stdsistream_tc(void)
{
	FILE *stream;
	char *filename = VFS_FILE_PATH;

	struct lib_stdsistream_s stdsistream;

	stream = fopen(filename, "w");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	lib_stdsistream((FAR struct lib_stdsistream_s *)&stdsistream, stream);
	fclose(stream);
	TC_ASSERT_EQ("lib_stdsistream", stdsistream.stream, stream);
	TC_ASSERT_EQ("lib_stdsistream", stdsistream.public.nget, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_stdsostream_tc
* @brief            Initializes a stream for use with a FILE instance
* @scenario         Initializes a stream for use with a FILE instance
* @apicovered       lib_stdsostream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_stdsostream_tc(void)
{
	FILE *stream;
	char *filename = VFS_FILE_PATH;

	struct lib_stdsostream_s stdsostream;

	stream = fopen(filename, "w");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	lib_stdsostream((FAR struct lib_stdsostream_s *)&stdsostream, stream);
	fclose(stream);
	TC_ASSERT_EQ("lib_stdsostream", stdsostream.stream, stream);
	TC_ASSERT_EQ("lib_stdsostream", stdsostream.public.nput, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         libc_stdio_zeroinstream_tc
* @brief            Initializes a NULL stream.  The initialized stream will return an infinitely long stream of zeroes.
* @scenario         Initializes a NULL stream.  The initialized stream will return an infinitely long stream of zeroes.
* @apicovered       lib_zeroinstream
* @precondition     NA
* @postcondition    NA
*/
static void libc_stdio_zeroinstream_tc(void)
{
	struct lib_instream_s zeroinstream;

	lib_zeroinstream((FAR struct lib_instream_s *)&zeroinstream);
	TC_ASSERT_EQ("lib_zeroinstream", zeroinstream.nget, 0);

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
	TC_ASSERT_NEQ("fopen", fp, NULL);

	ret = fputc(32, fp);
	fclose(fp);
	TC_ASSERT_NEQ("fputc", ret, EOF);

	fp = fopen(filename, "r");
	TC_ASSERT_NEQ("fopen", fp, NULL);

	ch1 = fgetc(fp);
	TC_ASSERT_NEQ_CLEANUP("fgetc", ch1, EOF, fclose(fp));

	ret = ungetc(64, fp);
	TC_ASSERT_NEQ_CLEANUP("ungetc", ret, EOF, fclose(fp));

	ch2 = fgetc(fp);
	fclose(fp);
	TC_ASSERT_NEQ("fgetc", ch2, EOF);

	TC_ASSERT_NEQ("fgetc", ch1, ch2);
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
	libc_stdio_freopen_tc();
	libc_stdio_ferror_tc();
	libc_stdio_clearerr_tc();
	libc_stdio_gets_tc();
	libc_stdio_gets_s_tc();
	libc_stdio_fileno_tc();
	libc_stdio_remove_tc();
#if CONFIG_STDIO_BUFFER_SIZE > 0
	libc_stdio_setbuf_tc();
	libc_stdio_setvbuf_tc();
#endif
	libc_stdio_meminstream_tc();
	libc_stdio_memoutstream_tc();
	libc_stdio_memsistream_tc();
	libc_stdio_memsostream_tc();
	libc_stdio_nullinstream_tc();
	libc_stdio_nulloutstream_tc();
	libc_stdio_rawinstream_tc();
	libc_stdio_rawoutstream_tc();
	libc_stdio_rawsistream_tc();
	libc_stdio_rawsostream_tc();
	libc_stdio_stdinstream_tc();
	libc_stdio_stdoutstream_tc();
	libc_stdio_stdsistream_tc();
	libc_stdio_stdsostream_tc();
	libc_stdio_ungetc_tc();
	libc_stdio_zeroinstream_tc();

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
