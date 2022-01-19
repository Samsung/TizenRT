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
#include <tinyara/fs/ramdisk.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#ifndef CONFIG_DISABLE_POLL
#include <poll.h>
#endif
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/statfs.h>
#include <sys/select.h>
#include <sys/types.h>

#include <tinyara/streams.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/fs_utils.h>
#include <tinyara/configdata.h>
#include <time.h>
#include "tc_common.h"
#include "tc_internal.h"

#ifdef CONFIG_AUTOMOUNT_USERFS
#include <fcntl.h>
#include <sys/ioctl.h>
#include <tinyara/os_api_test_drv.h>
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define STDIO_BUFLEN		64
#define VFS_CONTENTS_LEN	20
#define SEEK_DEF		3
#define SEEK_OFFSET		6
#define MOUNT_DIR "/fsmnt/"
#define FS_PATH_MAX 15

#ifdef CONFIG_AUTOMOUNT_USERFS
static char *TMP_MOUNT_DEV_DIR;
#else
#define TMP_MOUNT_DEV_DIR "/dev/smart1"
#endif

#ifdef CONFIG_FS_SMARTFS
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
#define MOUNT_DEV_DIR TMP_MOUNT_DEV_DIR"d1"
#else
#define MOUNT_DEV_DIR TMP_MOUNT_DEV_DIR
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

#define VFS_UNLINK_FILE_PATH MOUNT_DIR"unlink"

#define VFS_FOLDER_PATH MOUNT_DIR"folder"

#define VFS_STAT_PATH VFS_FOLDER_PATH"/1"

#define VFS_FILE1_PATH MOUNT_DIR"file1.txt"

#define DEV_ZERO_PATH "/dev/zero"

#define DEV_CONSOLE_PATH "/dev/console"

#define DEV_NULL_PATH "/dev/null"

#define DEV_NEW_NULL_PATH "/dev/NULL"

#define VFS_LOOP_COUNT 5

#define LONG_FILE_PATH MOUNT_DIR"long"

#define STREAM_TEST_CONTENTS "THIS IS STREAM TEST"

#define VFS_TEST_CONTENTS_1 "THIS IS VFS TEST 1"

#define VFS_TEST_CONTENTS_2 "THIS IS VFS TEST 2"

#define VFS_TEST_CONTENTS_3 "THIS IS VFS TEST 3"

#define LONG_FILE_CONTENTS "Yesterday all my trouble seemed so far away. Now it looks as though they're here to stay. Oh, I believe in yesterday."

#define DEV_RAMDISK_PATH "/dev/ram2"

#define LONG_FILE_LOOP_COUNT 24

#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
#define FIFO_FILE_PATH "/dev/fifo_test"

#define FIFO_DATA "FIFO DATA"
#endif

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

#define INV_FD -3
#define INVALID_DESCRIPTOR -1

#define MTD_CONFIG_PATH "/dev/config"
#define DEV_PATH "/dev"
#define DEV_INVALID_DIR "/dev/invalid"
#define DEV_EMPTY_FOLDER_PATH "/dev/folder"
#define DEV_EMPTY_UNLINK_FILE_PATH "/dev/unlink"
#define VFS_INVALID_PATH "/mnt/nofolder"
#define INVALID_PATH "/empty"

#define ROOT_PATH "/"

#define NONEFS_TYPE     "None FS"
#define SMARTFS_TYPE    "smartfs"
#define PROCFS_TYPE     "procfs"
#define ROMFS_TYPE      "romfs"
#define TMPFS_TYPE      "tmpfs"

/****************************************************************************
 * Global Variables
 ****************************************************************************/
#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
static int g_thread_result;
#endif

/****************************************************************************
 * Init Functions
 ****************************************************************************/
static void vfs_mount(void);
static void vfs_unmount(void);
static void vfs_mkdir(const char *dirpath);
static void vfs_rmdir(const char *dirpath);

#ifndef CONFIG_DISABLE_ENVIRON
extern int mount_show(void);
#endif

#ifdef CONFIG_AUTOMOUNT_USERFS
static char devname[CONFIG_SMARTFS_MAXNAMLEN];
#endif

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

	ret = read(fd, buf, 0);
	if (ret != 0) {
		g_thread_result = false;
		close(fd);
		return;
	}

	count = 1;
	size = strlen(FIFO_DATA) + 2;
	while (count < 6) {
		ret = read(fd, buf, size);
		if (ret == 0) {
			printf("Read data returned zero (EOF). Hence closing fifo\n");
			break;
		} else if (ret < 0) {
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

static void vfs_mount(void)
{
	int ret;
	struct statfs fs;

	if (0 > statfs(MOUNT_DIR, &fs)) {
		ret = mount(MOUNT_DEV_DIR, MOUNT_DIR, TARGET_FS_NAME, 0, NULL);
		TC_ASSERT_EQ("mount", ret, OK);
	}
}

static void vfs_unmount(void)
{
	int ret;
	struct statfs fs;

	if (!statfs(MOUNT_DIR, &fs)) {
		ret = umount(MOUNT_DIR);
		TC_ASSERT_EQ("umount", ret, OK);
	}
}

static void vfs_mkdir(const char *dirpath)
{
	char filename[50];
	struct stat st;
	int ret;
	int len;
	int i;

	if (0 > stat(dirpath, &st)) {
		ret = mkdir(dirpath, 0777);
		TC_ASSERT_EQ("mkdir", ret, OK);
	}

	len = strlen(dirpath) + 3;
	for (i = 0; i < VFS_LOOP_COUNT; i++) {
		memset(filename, 0, len);
		snprintf(filename, len, "%s/%d\0", dirpath, i);

		if (0 > stat(filename, &st)) {
			ret = mkdir(filename, 0777);
			TC_ASSERT_EQ("mkdir", ret, OK);
		}
	}
}

static void vfs_rmdir(const char *dirpath)
{
	char filename[50];
	struct stat st;
	int ret;
	int len;
	int i;

	len = strlen(dirpath) + 3;
	for (i = 0; i < VFS_LOOP_COUNT; i++) {
		memset(filename, 0, len);
		snprintf(filename, len, "%s/%d\0", dirpath, i);

		if (!stat(filename, &st)) {
			ret = rmdir(filename);
			TC_ASSERT_EQ("rmdir", ret, OK);
		}
	}

	if (!stat(dirpath, &st)) {
		ret = rmdir(dirpath);
		TC_ASSERT_EQ("rmdir", ret, OK);
	}
}

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
 * @testcase         tc_fs_vfs_mount_p
 * @brief            Mount file system
 * @scenario         Mount initialized file system
 * @apicovered       mount
 * @precondition     File system should be initialized. For smartfs, smart_initialize & mksmartfs should be excuted.
 * @postcondition    NA
 */
static void tc_fs_vfs_mount_p(void)
{
	int ret;

	/* Init */
	vfs_unmount();

	/* Testcase */
	ret = mount(MOUNT_DEV_DIR, MOUNT_DIR, TARGET_FS_NAME, 0, NULL);
	TC_ASSERT_EQ_CLEANUP("mount", ret, OK, vfs_unmount());

	/*search mountpoint operation*/
#if !defined(CONFIG_DISABLE_ENVIRON) && !defined(CONFIG_BUILD_PROTECTED)
	ret = mount_show();
	TC_ASSERT_EQ_CLEANUP("mount_show", ret, OK, vfs_unmount());
#endif

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_mount_exist_path_n
 * @brief            Mount file system
 * @scenario         Mount file system on exist path
 * @apicovered       mount
 * @precondition     File system should be initialized. For smartfs, smart_initialize & mksmartfs should be excuted.
 * @postcondition    NA
 */
static void tc_fs_vfs_mount_exist_path_n(void)
{
	int ret;

	/* Init */
	vfs_mount();

	/* Testcase */
	ret = mount(MOUNT_DEV_DIR, MOUNT_DIR, TARGET_FS_NAME, 0, NULL);
	TC_ASSERT_NEQ_CLEANUP("mount", ret, OK, vfs_unmount());

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_umount_p
 * @brief            Unmount file system
 * @scenario         Unmount mounted file system
 * @apicovered       umount
 * @precondition     File system should be mounted.
 * @postcondition    NA
 */
static void tc_fs_vfs_umount_p(void)
{
	int ret;

	/* Init */
	vfs_mount();

	/* Testcase */
	ret = umount(MOUNT_DIR);
	TC_ASSERT_EQ_CLEANUP("umount", ret, OK, vfs_unmount());

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_umount_noexist_path_n
 * @brief            Unmount file system
 * @scenario         Noexist path unmount file system
 * @apicovered       umount
 * @precondition     File system should be mounted.
 * @postcondition    NA
 */
static void tc_fs_vfs_umount_noexist_path_n(void)
{
	int ret;

	/* Init */
	vfs_unmount();

	/* Testcase */
	ret = umount(MOUNT_DIR);
	TC_ASSERT_NEQ("umount", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_open_p
 * @brief            Open file to do file operation
 * @scenario         Open specific file
 * @apicovered       open
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_open_p(void)
{
	int fd;

	/* Init */
	vfs_mount();

	/* Testcase */
	fd = open(VFS_FILE_PATH, O_WROK | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_open_invalid_path_n
 * @brief            Open file to do file operation
 * @scenario         Invalid path open file
 * @apicovered       open
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_open_invalid_path_n(void)
{
	int fd;
	/* Nagative case with invalid argument, not existing pathname. It will return ERROR */

	/* Testcase */
	fd = open(VFS_INVALID_FILE_PATH, O_WROK);
	TC_ASSERT_LT_CLEANUP("open", fd, 0, close(fd));

	/* Deinit */
	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_fdopen
* @brief            Open file to do file operation using file descriptor
* @scenario         Open specific file
* @apicovered       fs_fdopen
* @precondition     NA
* @postcondition    NA
*/
static void tc_fs_vfs_fdopen_p(void)
{
	int fd;
	struct file_struct *fp ;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_WROK | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);

	/* Testcase */
	fp = fs_fdopen(fd, O_WROK, NULL);
	TC_ASSERT_NEQ_CLEANUP("fs_fdopen", fp, NULL, close(fd); vfs_unmount());

	/* Deinit */
	fclose(fp);
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

static void tc_fs_vfs_fdopen_invalid_fp_n(void)
{
	int fd = -1;
	struct file_struct *fp ;

	/* Testcase */
	fp = fs_fdopen(fd, O_WROK, NULL);
	TC_ASSERT_EQ("fs_fdopen", fp, NULL);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_write_p
 * @brief            Write data into specific file
 * @scenario         Open file and then write data, if writing finished, close file
 * @apicovered       open, write
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_write_p(void)
{
	int fd, ret;
	char *buf = VFS_TEST_CONTENTS_1;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	ret = write(fd, buf, strlen(buf));
	TC_ASSERT_EQ_CLEANUP("write", ret, strlen(buf), close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_write_invalid_flags_n
 * @brief            Write data into specific file
 * @scenario         Open file with invalid flags then write data
 * @apicovered       write
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_write_invalid_flags_n(void)
{
	int fd, ret;
	char *buf = VFS_TEST_CONTENTS_1;
	/* Nagative case with invalid argument, no write access. It will return ERROR */

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	ret = write(fd, buf, sizeof(buf));
	TC_ASSERT_LT_CLEANUP("write", ret, 0, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_write_invalid_fd_n
 * @brief            Write data into specific file
 * @scenario         Write data to invalid file descriptor
 * @apicovered       write
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_write_invalid_fd_n(void)
{
	int ret;
	char *buf = VFS_TEST_CONTENTS_1;
	/* Nagative case with invalid argument, fd. It will return ERROR */

	/* Testcase */
	ret = write(INVALID_DESCRIPTOR, buf, strlen(buf));
	TC_ASSERT_EQ("write", ret, ERROR);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_ftruncate_p
 * @brief            Shrink or extend specific file
 * @scenario         Open file and change file size
 * @apicovered       open, ftruncate
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_ftruncate_p(void)
{
	int fd, ret;
	struct stat st;
	size_t len;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_RDWR);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());
	len = strlen(VFS_TEST_CONTENTS_1);

	ret = ftruncate(fd, (len + 5));
	TC_ASSERT_EQ_CLEANUP("ftruncate", ret, OK, close(fd); vfs_unmount());
	ret = stat(VFS_FILE_PATH, &st);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("stat", st.st_size, (len + 5), close(fd); vfs_unmount());	
	
	ret = ftruncate(fd, len);
	TC_ASSERT_EQ_CLEANUP("ftruncate", ret, OK, close(fd); vfs_unmount());
	ret = stat(VFS_FILE_PATH, &st);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("stat", st.st_size, len, close(fd); vfs_unmount());	

	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_ftruncate_invalid_fd_n
 * @brief            Shrink or extend specific file
 * @scenario         change length of file with invalid fd
 * @apicovered       open, ftruncate
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_ftruncate_invalid_fd_n(void)
{
	int ret;

	/* Init */
	vfs_mount();

	ret = ftruncate(INVALID_DESCRIPTOR, 0);
	TC_ASSERT_EQ_CLEANUP("ftruncate", ret, ERROR, vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("ftruncate", errno, EBADF, vfs_unmount());

	vfs_unmount();
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_ftruncate_invalid_length_n
 * @brief            Shrink or extend specific file
 * @scenario         Open file and change file size to negative length
 * @apicovered       open, ftruncate
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_ftruncate_invalid_length_n(void)
{
	int fd, ret;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_WROK);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	ret = ftruncate(fd, -1);
	TC_ASSERT_EQ_CLEANUP("ftruncate", ret, ERROR, close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("ftruncate", errno, EINVAL, close(fd); vfs_unmount());

	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_ftruncate_invalid_flags_n
 * @brief            Shrink or extend specific file
 * @scenario         Open file with read only permission and change file size
 * @apicovered       open, ftruncate
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_ftruncate_invalid_flags_n(void)
{
	int fd, ret;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	ret = ftruncate(fd, 0);
	TC_ASSERT_EQ_CLEANUP("ftruncate", ret, ERROR, close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("ftruncate", errno, EBADF, close(fd); vfs_unmount());

	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_read_p
 * @brief            Read data from specific file
 * @scenario         Open file and then read data, if reading finished, close file
 * @apicovered       open, read
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_read_p(void)
{
	int fd, ret;
	char buf[20];

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());
	memset(buf, 0, sizeof(buf));

	/* Testcase */
	ret = read(fd, buf, sizeof(buf));
	TC_ASSERT_GEQ_CLEANUP("read", ret, 0, close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("read", strcmp(buf, VFS_TEST_CONTENTS_1), 0, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_read_invalid_flags_n
 * @brief            Read after open the file for write only
 * @scenario         Open file and then read data, if reading finished, close file
 * @apicovered       open, read
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_read_invalid_flags_n(void)
{
	int fd, ret;
	char buf[20];
	/* Nagative case with invalid argument, no read access. It will return ERROR */

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_WRONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());
	memset(buf, 0, sizeof(buf));

	/* Testcase */
	ret = read(fd, buf, sizeof(buf));
	TC_ASSERT_LT_CLEANUP("read", ret, 0, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_read_invalid_fd_n
 * @brief            Read after open the file for write only
 * @scenario         Read data to invalid file descriptor
 * @apicovered       read
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_read_invalid_fd_n(void)
{
	int ret;
	char buf[20];
	/* Nagative case with invalid argument, fd. It will return ERROR */

	/* Testcase */
	ret = read(INVALID_DESCRIPTOR, buf, sizeof(buf));
	TC_ASSERT_EQ("read", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_close_p
 * @brief            Close file
 * @scenario         Open and close
 * @apicovered       open, close
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_close_p(void)
{
	int fd, ret;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	ret = close(fd);
	TC_ASSERT_GEQ_CLEANUP("close", ret, 0, vfs_unmount());

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_close_invalid_fd_n
 * @brief            Close file
 * @scenario         Close invalid file descriptor
 * @apicovered       close
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_close_invalid_fd_n(void)
{
	int ret;
	int fd = -1;

	/* Testcase */
	ret = close(fd);
	TC_ASSERT_EQ("close", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_dup_p
 * @brief            Clone a file descriptor to an arbitray descriptor number
 * @scenario         Open and write data. and then Clone descriptor to fd2, write data with fd2.
 *                   After write, check it writes properly or not.
 * @apicovered       open, write, dup, read
 * @precondition	 NA
 * @postcondition    NA
 */
static void tc_fs_vfs_dup_p(void)
{
	char *filename = VFS_DUP_FILE_PATH;
	char *str1 = VFS_TEST_CONTENTS_1;
	char *str2 = VFS_TEST_CONTENTS_2;
	char buf[20];
	ssize_t ret;
	int fd1, fd2, len;

	/* Init */
	vfs_mount();

	memset(buf, 0, sizeof(buf));
	fd1 = open(filename, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd1, 0, vfs_unmount());

	len = strlen(str1);
	ret = write(fd1, str1, len);
	TC_ASSERT_EQ_CLEANUP("write", ret, len, close(fd1); vfs_unmount());

	/* Testcase */
	fd2 = dup(fd1);
	close(fd1);
	TC_ASSERT_GEQ_CLEANUP("dup", fd2, 0, close(fd2); vfs_unmount());

	len = strlen(str2);
	ret = write(fd2, str2, strlen(str2));
	close(fd2);
	TC_ASSERT_EQ_CLEANUP("write", ret, len, vfs_unmount());

	fd1 = open(filename, O_RDOK);
	TC_ASSERT_GEQ_CLEANUP("open", fd1, 0, vfs_unmount());

	len = strlen(str1);
	ret = read(fd1, buf, len);

	TC_ASSERT_GT_CLEANUP("read", ret, 0, close(fd1); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("read", strcmp(buf, VFS_TEST_CONTENTS_1), 0, close(fd1); vfs_unmount());

	memset(buf, 0, sizeof(buf));
	len = strlen(str2);
	ret = read(fd1, buf, len);

	TC_ASSERT_GT_CLEANUP("read", ret, 0, close(fd1); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("read", strcmp(buf, VFS_TEST_CONTENTS_2), 0, close(fd1); vfs_unmount());

	/* Deinit */
	close(fd1);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_dup_invalid_fd_n
 * @brief            Clone a file descriptor to an arbitray descriptor number
 * @scenario         Clone invalid file descriptor
 * @apicovered       dup
 * @precondition	 NA
 * @postcondition    NA
 */
static void tc_fs_vfs_dup_invalid_fd_n(void)
{
	int fd;
	/* Nagative case with invalid argument, invalid fd. It will return ERROR */

	/* Testcase */
#if CONFIG_NFILE_DESCRIPTORS > 0
	fd = dup(INVALID_DESCRIPTOR);
	TC_ASSERT_LT_CLEANUP("dup", fd, 0, close(fd));
#endif

#if defined(CONFIG_NET) && CONFIG_NSOCKET_DESCRIPTORS > 0
	fd = dup(CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS);
	TC_ASSERT_LT_CLEANUP("dup", fd, 0, close(fd));
#endif

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_dup2_p
 * @brief            Clone a file descriptor  to a specific descriptor number
 * @scenario         Open and write data. and then Clone fd1 to fd2, write data with fd1.
 *                   After write, check it writes properly or not by read data with fd2.
 * @apicovered       open, write, dup2, read
 * @precondition	 NA
 * @postcondition    NA
 */
static void tc_fs_vfs_dup2_p(void)
{
	char *filename1 = VFS_DUP_FILE_PATH;
	char *filename2 = VFS_DUP2_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_3;
	char buf[20];
	ssize_t ret;
	int fd1, fd2, len;

	/* Init */
	vfs_mount();

	memset(buf, 0, sizeof(buf));

	fd1 = open(filename1, O_RDWR);
	TC_ASSERT_GEQ_CLEANUP("open", fd1, 0, vfs_unmount());

	fd2 = open(filename2, O_RDWR | O_CREAT);
	close(fd1);
	TC_ASSERT_GEQ_CLEANUP("open", fd2, 0, vfs_unmount());

	/* Testcase */
	/* now fd1 points fd2 */
	ret = dup2(fd2, fd1);
	close(fd2);
	TC_ASSERT_NEQ_CLEANUP("dup2", ret, ERROR, vfs_unmount());
	TC_ASSERT_GEQ_CLEANUP("dup2", fd1, 0, vfs_unmount());

	len = strlen(VFS_TEST_CONTENTS_3);
	ret = write(fd1, str, len);
	close(fd1);
	TC_ASSERT_EQ_CLEANUP("write", ret, len, vfs_unmount());

	fd2 = open(filename2, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd2, 0, vfs_unmount());

	ret = read(fd2, buf, len);

	TC_ASSERT_GT_CLEANUP("read", ret, 0, close(fd2));
	TC_ASSERT_EQ_CLEANUP("read", strcmp(buf, VFS_TEST_CONTENTS_3), 0, close(fd2));

	/* Deinit */
	close(fd2);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_dup2_invalid_fd_n
 * @brief            Clone a file descriptor to a specific descriptor number
 * @scenario         Clone invalid file descriptor
 * @apicovered       dup2
 * @precondition	 NA
 * @postcondition    NA
 */
static void tc_fs_vfs_dup2_invalid_fd_n(void)
{
	ssize_t ret;
	int fd;
	/* Nagative case with invalid argument, invalid fd. It will return ERROR */

	/* Testcase */
	fd = -1;
	ret = dup2(CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS, fd);
	close(fd);
	TC_ASSERT_LT("dup2", fd, 0);
	TC_ASSERT_EQ("dup2", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_fsync_p
 * @brief            Synchronize the file state on disk to match internal, in-memory state.
 * @scenario         Open and write data. and then check fsync works properly.
 * @apicovered       open, write, fsync, read
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_fsync_p(void)
{
	int ret, fd, len;
	char *filename = VFS_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_2;

	/* Init */
	vfs_mount();

	fd = open(filename, O_WROK | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	len = strlen(str);
	ret = write(fd, str, len);
	TC_ASSERT_EQ_CLEANUP("write", ret, len, close(fd); vfs_unmount());

	/* Testcase */
	ret = fsync(fd);
	TC_ASSERT_GEQ_CLEANUP("fsync", ret, 0, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_fsync_invalid_flags_n
 * @brief            Synchronize the file state on disk to match internal, in-memory state.
 * @scenario         Open the data read-only and make sure fsync is working properly.
 * @apicovered       write, fsync
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_fsync_invalid_flags_n(void)
{
	int ret, fd;
	char *filename = VFS_FILE_PATH;
	/* Nagative case with invalid argument, no write access. It will return ERROR */

	/* Init */
	vfs_mount();

	fd = open(filename, O_RDOK);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	ret = fsync(fd);
	TC_ASSERT_EQ_CLEANUP("fsync", ret, ERROR, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_fsync_invalid_fd_n
 * @brief            Synchronize the file state on disk to match internal, in-memory state.
 * @scenario         fsync to invalid file descriptor
 * @apicovered       fsync
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_fsync_invalid_fd_n(void)
{
	int ret;
	/* Nagative case with invalid argument, fd. It will return ERROR */

	/* Testcase */
	ret = fsync(INVALID_DESCRIPTOR);
	TC_ASSERT_EQ("fsync", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_lseek_p
 * @brief            Move current file position to specific position
 * @scenario         Open file and move position, and then read data to check lseek works properly or not.
 * @apicovered       open, lseek, read
 * @precondition     Data(VFS_TEST_CONTENTS_2) should be written in file(VFS_FILE_PATH)
 * @postcondition    NA
 */
static void tc_fs_vfs_lseek_p(void)
{
	int ret, fd;
	char *filename = VFS_FILE_PATH;
	char buf[20];

	/* Init */
	vfs_mount();

	fd = open(filename, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	ret = lseek(fd, 5, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("lseek", ret, 5, close(fd); vfs_unmount());

	memset(buf, 0, sizeof(buf));
	ret = read(fd, buf, sizeof(buf));
	close(fd);

	TC_ASSERT_GT_CLEANUP("read", ret, 0, vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("read", strcmp(buf, "IS VFS TEST 2"), 0, vfs_unmount());

	/* empty file seek*/
	fd = open(VFS_FILE1_PATH, O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	ret = lseek(fd, 10, SEEK_SET);
	TC_ASSERT_NEQ_CLEANUP("lseek", ret, 10, close(fd); vfs_unmount());
	close(fd);
#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
	ret = mkfifo(FIFO_FILE_PATH, 0666);
	if (ret < 0) {
		TC_ASSERT_EQ_CLEANUP("mkfifo", ret, -EEXIST, vfs_unmount());
	}
	fd = open(FIFO_FILE_PATH, O_WRONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	ret = lseek(fd, 10, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("lseek", ret, 10, close(fd); vfs_unmount());

	ret = lseek(fd, -10, SEEK_SET);
	TC_ASSERT_NEQ_CLEANUP("lseek", ret, 10, close(fd); vfs_unmount());

	ret = lseek(fd, 10, SEEK_CUR);
	TC_ASSERT_GEQ_CLEANUP("lseek", ret, 0, close(fd); vfs_unmount());

	ret = lseek(fd, 10, SEEK_END);
	TC_ASSERT_NEQ_CLEANUP("lseek", ret, 10, close(fd); vfs_unmount());

#endif

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_lseek_invalid_fd_n
 * @brief            Move current file position to specific position
 * @scenario         Move position to invalid file descriptor
 * @apicovered       lseek
 * @precondition     Data(VFS_TEST_CONTENTS_2) should be written in file(VFS_FILE_PATH)
 * @postcondition    NA
 */
static void tc_fs_vfs_lseek_invalid_fd_n(void)
{
	int ret;
	/* Nagative case with invalid argument, fd. It will return ERROR */

	/* Testcase */
	ret = lseek(INVALID_DESCRIPTOR, 5, SEEK_SET);
	TC_ASSERT_EQ("lseek", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_pwrite_p
 * @brief            Write data at specific position of file
 * @scenario         Open and write data at specific position. And then check file written on disk.
 * @apicovered       open, pwrite, read
 * @precondition     Data(VFS_TEST_CONTENTS_2) should be written in file(VFS_FILE_PATH)
 * @postcondition    NA
 */
static void tc_fs_vfs_pwrite_p(void)
{
	int ret, fd;
	char *filename = VFS_FILE_PATH;
	char *str = "pwrite test!";

	/* Init */
	vfs_mount();

	fd = open(filename, O_WRONLY | O_APPEND);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	ret = pwrite(fd, str, strlen(str), 10);
	TC_ASSERT_NEQ_CLEANUP("pwrite", ret, ERROR, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_pwrite_invalid_fd_n
 * @brief            Write data at specific position of file
 * @scenario         Write data at invalid file descriptor
 * @apicovered       pwrite
 * @precondition     Data(VFS_TEST_CONTENTS_2) should be written in file(VFS_FILE_PATH)
 * @postcondition    NA
 */
static void tc_fs_vfs_pwrite_invalid_fd_n(void)
{
	int ret;
	char *str = "pwrite test!";
	/* Nagative case with invalid argument, fd. It will return ERROR */

	/* Testcase */
	ret = pwrite(INVALID_DESCRIPTOR, str, strlen(str), 10);
	TC_ASSERT_EQ("pwrite", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_pread_p
 * @brief            Read data at specific position of file
 * @scenario         Open and read data from specific position.
 * @apicovered       open, pread
 * @precondition     tc_fs_vfs_pwrite should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_pread_p(void)
{
	int ret, fd;
	char *filename = VFS_FILE_PATH;
	char *str = "pwrite test!";
	char buf[20];

	/* Init */
	vfs_mount();

	fd = open(filename, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	memset(buf, 0, sizeof(buf));

	/* Testcase */
	ret = pread(fd, buf, 20, 10);
	TC_ASSERT_GT_CLEANUP("pread", ret, 0, close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("pread", strcmp(buf, str), 0, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_pread_invalid_fd_n
 * @brief            Read data at specific position of file
 * @scenario         read data from invalid file descriptor
 * @apicovered       open, pread
 * @precondition     tc_fs_vfs_pwrite should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_pread_invalid_fd_n(void)
{
	int ret;
	char buf[20];
	/* Nagative case with invalid argument, fd. It will return ERROR */

	/* Testcase */
	ret = pread(INVALID_DESCRIPTOR, buf, 20, 10);
	TC_ASSERT_EQ("pread", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_mkdir_p
 * @brief            Create folders
 * @scenario         Create folder(VFS_FOLDER_PATH) and create 5 sub-folders
 * @apicovered       mkdir
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_mkdir_p(void)
{
	int ret;

	/* Init */
	vfs_mount();
	vfs_rmdir(VFS_FOLDER_PATH);
	vfs_rmdir(DEV_EMPTY_FOLDER_PATH);

	/* Testcase */
	vfs_mkdir(VFS_FOLDER_PATH);

	/*creating an empty folder */
	ret = mkdir(DEV_EMPTY_FOLDER_PATH, 0777);
	TC_ASSERT_EQ_CLEANUP("mkdir", ret, OK, vfs_unmount());

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_mkdir_exist_path_n
 * @brief            Create folders
 * @scenario         Create folder that is the same path as already exists
 * @apicovered       mkdir
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_mkdir_exist_path_n(void)
{
	int ret;

	/* Init */
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);

	/* Testcase */
	ret = mkdir(VFS_FOLDER_PATH, 0777);
	TC_ASSERT_EQ_CLEANUP("mkdir", ret, ERROR, vfs_unmount());

	/* deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_mkdir_exceed_len_n
 * @brief            Create folders
 * @scenario         Create folder that exceed the maximum name length
 * @apicovered       mkdir
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_mkdir_exceed_len_n(void)
{
	int ret;
	char *filename;
	int i;

	/* Init */
	filename = (char *)malloc(sizeof(char) * (CONFIG_NAME_MAX + 1));
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);

	for (i = 0; i < CONFIG_NAME_MAX + 1; i++) {
		sprintf(filename, "%s%c", filename, 'a');
	}

	/* Testcase */
	ret = mkdir(filename, 0777);
	TC_ASSERT_EQ_CLEANUP("mkdir", ret, ERROR, free(filename); vfs_unmount());

	/* Deinit */
	free(filename);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_opendir_p
 * @brief            Open specific directory to use APIs defined in dirent.h
 * @scenario         Open specific directory
 * @apicovered       opendir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_opendir_p(void)
{
	DIR *dir;

	/* Init */
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);
	vfs_mkdir(DEV_EMPTY_FOLDER_PATH);

	/* Testcase */
	dir = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dir, NULL, vfs_unmount());
	closedir(dir);

	dir = opendir(DEV_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dir, NULL, vfs_unmount());
	closedir(dir);

	/* Opening an empty folder */
	dir = opendir(DEV_EMPTY_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dir, NULL, vfs_unmount());
	closedir(dir);

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_opendir_invalid_path_n
 * @brief            Open specific directory to use APIs defined in dirent.h
 * @scenario         Open invalid directory
 * @apicovered       opendir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_opendir_invalid_path_n(void)
{
	DIR *dir;

	/* Testcase */
	/*Path doesnot exist */
	dir = opendir(INVALID_PATH);
	TC_ASSERT_EQ("opendir", dir, NULL);

	/* Pseudo file system node covers error condition path is not a directory */
	dir = opendir(DEV_INVALID_DIR);
	TC_ASSERT_EQ("opendir", dir, NULL);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_readdir_p
 * @brief            Read contents in specific directory sequentially
 * @scenario         Read contents in specific folder(VFS_FOLDER_PATH)
 * @apicovered       opendir, readdir, closedir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_readdir_p(void)
{
	int ret;
	int count;
	DIR *dir;
	struct dirent *dirent;

	/* Init */
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);
	vfs_mkdir(DEV_EMPTY_FOLDER_PATH);

	dir = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dir, NULL, vfs_unmount());

	count = 0;

	/* Testcase */
	while (1) {
		dirent = readdir(dir);
		if (dirent == NULL) {
			break;
		}
		count++;
	}

	/* Deinit */
	ret = closedir(dir);
	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("readdir", count, VFS_LOOP_COUNT, vfs_unmount());

	/*reading empty folder */
	/* Init */
	dir = opendir(DEV_EMPTY_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dir, NULL, vfs_unmount());

	/* Testcase */
	do {
		dirent = readdir(dir);
	} while (dirent != NULL);

	/* Deinit */
	ret = closedir(dir);
	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, vfs_unmount());
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_readdir_invalid_path_n
 * @brief            Read contents in specific directory sequentially
 * @scenario         Read contents in invalid folder
 * @apicovered       opendir, readdir, closedir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_readdir_invalid_path_n(void)
{
	int ret;
	DIR *dir;
	struct dirent *dirent;

	/* Init */
	dir = opendir(VFS_INVALID_PATH);
	TC_ASSERT_EQ("opendir", dir, NULL);

	/* Testcase */
	/*reading invalid directory */
	dirent = readdir(dir);
	TC_ASSERT_EQ("readdir", dirent, NULL);

	/* Deinit */
	ret = closedir(dir);
	TC_ASSERT_NEQ("closedir", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_rewinddir_p
 * @brief            Reset current position of directory
 * @scenario         Read contents in specific folder(VFS_FOLDER_PATH), and it reachs end of contents, reset & read again
 * @apicovered       opendir, rewinddir, closedir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_rewinddir_p(void)
{
	int ret;
	int count;
	DIR *dir;
	struct dirent *dirent;

	/* Init */
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);

	dir = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dir, NULL, vfs_unmount());

	count = 0;

	/* Testcase */
	while (1) {
		dirent = readdir(dir);
		if (dirent == NULL) {
			if (count > VFS_LOOP_COUNT) {
				break;
			}
			rewinddir(dir);
			continue;
		}
		count++;
	}

	/* Deinit */
	ret = closedir(dir);

	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("rewinddir", count, VFS_LOOP_COUNT * 2, vfs_unmount());

	/*For Pseudo dir operations */
	/* Init */
	dir = opendir(ROOT_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dir, NULL, vfs_unmount());

	count = 0;
	while ((dirent = readdir(dir)) != NULL) {
		count++;
	}

	/* Testcase */
	rewinddir(dir);

	while ((dirent = readdir(dir)) != NULL) {
		count--;
	}

	/* Deinit */
	ret = closedir(dir);
	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("rewinddir", count, 0, vfs_unmount());
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_rewinddir_null_path_n
 * @brief            Reset current position of directory
 * @scenario         Reset & read again at null path
 * @apicovered       opendir, rewinddir, closedir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_rewinddir_null_path_n(void)
{
	DIR *dir = NULL;

	/* Testcase */
	rewinddir(dir);
	TC_ASSERT_EQ("rewinddir", dir, NULL);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_seekdir_p
 * @brief            Move current position of directory to specific position
 * @scenario         Change position of directory and read contents
 * @apicovered       opendir, seekdir, readdir, closedir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_seekdir_p(void)
{
	int ret;
	DIR *dir;
	struct dirent *dirent;
	off_t offset;
	char filename[1];

	/* Init */
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);

	dir = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dir, NULL, vfs_unmount());

	offset = 2;

	/* Testcase */
	seekdir(dir, offset);
	TC_ASSERT_NEQ_CLEANUP("seekdir", dir, NULL, closedir(dir); vfs_unmount());
	dirent = readdir(dir);
	TC_ASSERT_NEQ_CLEANUP("readdir", dirent, NULL, closedir(dir); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("readdir", dirent->d_type, DTYPE_DIRECTORY, closedir(dir); vfs_unmount());

	itoa((int)offset, filename, 10);
	TC_ASSERT_EQ_CLEANUP("readdir", strncmp(dirent->d_name, filename, 1), 0, closedir(dir); vfs_unmount());

	/* Deinit */
	ret = closedir(dir);
	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, vfs_unmount());

	/* for pseudo dir operations */
	/* Init */
	dir = opendir(ROOT_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dir, NULL, vfs_unmount());

	offset = 0;

	/* Testcase */
	seekdir(dir, offset);
	TC_ASSERT_NEQ_CLEANUP("seekdir", dir, NULL, closedir(dir); vfs_unmount());
	dirent = readdir(dir);
	TC_ASSERT_NEQ_CLEANUP("readdir", dirent, NULL, closedir(dir); vfs_unmount());

	/* Deinit */
	ret = closedir(dir);
	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, vfs_unmount());
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_seekdir_offset_n
 * @brief            Move current position of directory to specific position
 * @scenario         Change invalid position of directory and read contents
 * @apicovered       opendir, seekdir, readdir, closedir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_seekdir_offset_n(void)
{
	int ret;
	DIR *dir;
	struct dirent *dirent;
	off_t offset;
	/* For Negative offset in seekmountdir operations */

	/* Init */
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);

	dir = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dir, NULL, vfs_unmount());

	offset = -2;

	/* Testcase */
	seekdir(dir, offset);
	TC_ASSERT_NEQ_CLEANUP("seekdir", dir, NULL, closedir(dir); vfs_unmount());
	dirent = readdir(dir);
	TC_ASSERT_EQ_CLEANUP("readdir", dirent->d_type, DTYPE_DIRECTORY, closedir(dir); vfs_unmount());

	/* Deinit */
	ret = closedir(dir);
	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, vfs_unmount());
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_seekdir_position_n
 * @brief            Move current position of directory to specific position
 * @scenario         Change invalid position of directory and read contents
 * @apicovered       opendir, seekdir, readdir, closedir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_seekdir_position_n(void)
{
	int ret;
	DIR *dir;
	struct dirent *dirent;
	off_t offset;

	/* Init */
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);

	dir = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dir, NULL, vfs_unmount());

	offset = 5;

	/* Testcase */
	seekdir(dir, offset);
	TC_ASSERT_NEQ_CLEANUP("seekdir", dir, NULL, closedir(dir); vfs_unmount());
	dirent = readdir(dir);
	TC_ASSERT_EQ_CLEANUP("readdir", dirent, NULL, closedir(dir); vfs_unmount());

	/* Deinit */
	ret = closedir(dir);
	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, vfs_unmount());
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         fs_libc_dirent_readdir_r
 * @brief            Get position of next contents in specific directory
 * @scenario         Get next contents's position until it reachs end of contents, and check count of contents
 * @apicovered       opendir, readdir_r, closedir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_libc_dirent_readdir_r_p(void)
{
	int ret, count;
	DIR *dirp;
	struct dirent entry;
	struct dirent *result;

	/* Init */
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);
	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dirp, NULL, vfs_rmdir(VFS_FOLDER_PATH); vfs_unmount());

	/* Testcase */
	count = 0;
	while (1) {
		ret = readdir_r(dirp, &entry, &result);
		if (ret != 0 || result == NULL) {
			break;
		}
		count++;
	}
	TC_ASSERT_EQ_CLEANUP("readdir_r", count, VFS_LOOP_COUNT, vfs_rmdir(VFS_FOLDER_PATH); vfs_unmount());

	/* Deinit */
	ret = closedir(dirp);
	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, vfs_rmdir(VFS_FOLDER_PATH); vfs_unmount());
	vfs_rmdir(VFS_FOLDER_PATH);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_dirent_readdir_r_invalid_path_n
 * @brief            Get position of next contents in specific directory
 * @scenario         Directory with the invalid position is read
 * @apicovered       opendir, readdir_r, closedir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_libc_dirent_readdir_r_invalid_path_n(void)
{
	int ret;
	DIR *dirp;
	struct dirent entry;
	struct dirent *result;

	/* Init */
	dirp = opendir(VFS_INVALID_PATH);
	TC_ASSERT_EQ("opendir", dirp, NULL);

	/* Testcase */
	ret = readdir_r(dirp, &entry, &result);
	TC_ASSERT_NEQ("readdir_r", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         fs_libc_dirent_telldir_p
 * @brief            Get position of current contents in specific directory
 * @scenario         Get specific position by seekdir and check telldir returns position properly
 * @apicovered       opendir, seekdir, telldir, closedir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_libc_dirent_telldir_p(void)
{
	DIR *dirp;
	off_t offset, res;
	int ret;

	/* Init */
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);
	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dirp, NULL, vfs_mkdir(VFS_FOLDER_PATH); vfs_unmount());

	offset = 2;
	seekdir(dirp, offset);
	TC_ASSERT_NEQ_CLEANUP("seekdir", dirp, NULL, closedir(dirp); vfs_rmdir(VFS_FOLDER_PATH); vfs_unmount());
	res = telldir(dirp);
	TC_ASSERT_EQ_CLEANUP("telldir", res, offset, closedir(dirp); vfs_rmdir(VFS_FOLDER_PATH); vfs_unmount());
	ret = closedir(dirp);
	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, vfs_rmdir(VFS_FOLDER_PATH); vfs_unmount());

	/* Deinit */
	vfs_rmdir(VFS_FOLDER_PATH);
	vfs_unmount();

	/* Nagative case with invalid argument, NULL stream. It will return (off_t)-1 */
	res = telldir(NULL);
	TC_ASSERT_EQ("telldir", res, (off_t)-1);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_dirent_telldir_null_stream_n
 * @brief            Get position of current contents in specific directory
 * @scenario         Telldir to the NULL position
 * @apicovered       opendir, seekdir, telldir, closedir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_libc_dirent_telldir_null_stream_n(void)
{
	int ret;

	/* Nagative case with invalid argument, NULL stream. It will return (off_t)-1 */
	ret = telldir(NULL);
	TC_ASSERT_EQ("telldir", ret, (off_t)-1);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_closedir_p
 * @brief            Close opened directory
 * @scenario         Open and close directory
 * @apicovered       opendir, closedir
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_closedir_p(void)
{
	int ret;
	DIR *dirp;

	/* Init */
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);

	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dirp, NULL, vfs_unmount());

	/* Testcase */
	ret = closedir(dirp);
	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, vfs_unmount());

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_closedir_null_path_n
 * @brief            Close opened directory
 * @scenario         Close null directory
 * @apicovered       opendir, closedir
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_closedir_null_path_n(void)
{
	int ret;

	/* Testcase */
	ret = closedir(NULL);
	TC_ASSERT_EQ("closedir", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_rmdir_p
 * @brief            Remove each of directory
 * @scenario         Remove all exist directory
 * @apicovered       rmdir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_rmdir_p(void)
{
	/* Init */
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);
	vfs_mkdir(DEV_EMPTY_FOLDER_PATH);

	/* Testcase */
	vfs_rmdir(VFS_FOLDER_PATH);

	/*Removes the empty directory created*/
	vfs_rmdir(DEV_EMPTY_FOLDER_PATH);

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_rmdir_null_path_n
 * @brief            Remove each of directory
 * @scenario         Remove null directory
 * @apicovered       rmdir
 * @precondition     tc_fs_vfs_mkdir should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_rmdir_null_path_n(void)
{
	int ret;
	/* Nagative case with invalid argument, NULL pathname. It will return ERROR */
	/* Testcase */
	ret = rmdir(NULL);
	TC_ASSERT_EQ("rmdir", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_unlink_p
 * @brief            Unlink specific file
 * @scenario         Unlink specific file(VFS_DUP_FILE_PATH)
 * @apicovered       unlink
 * @precondition     tc_fs_vfs_dup should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_unlink_p(void)
{
	char *filepath = VFS_UNLINK_FILE_PATH;
#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
	char *impty_filepath = DEV_EMPTY_UNLINK_FILE_PATH;
#endif
	int ret;
	int fd;

	/* Init */
	vfs_mount();

	fd = open(filepath, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	close(fd);

	/* Testcase */
	ret = unlink(filepath);
	TC_ASSERT_EQ_CLEANUP("unlink", ret, OK, vfs_unmount());

#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
	/*Init */
	ret = mkfifo(impty_filepath, 0666);
	if (ret < 0) {
		TC_ASSERT_EQ_CLEANUP("mkfifo", ret, -EEXIST, vfs_unmount());
	}

	fd = open(impty_filepath, O_WRONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	close(fd);

	/* Testcase */
	ret = unlink(impty_filepath);
	TC_ASSERT_EQ_CLEANUP("unlink", ret, OK, vfs_unmount());
#endif

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_unlink_null_path_n
 * @brief            Unlink specific file
 * @scenario         Unlink null file
 * @apicovered       unlink
 * @precondition     tc_fs_vfs_dup should be passed
 * @postcondition    NA
 */
static void tc_fs_vfs_unlink_null_path_n(void)
{
	int ret;
	/* Nagative case with invalid argument, NULL pathname. It will return ERROR */
	/* Testcase */
	ret = unlink(NULL);
	TC_ASSERT_EQ("unlink", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_stat_p
 * @brief            Get status of specific file
 * @scenario         Get status of specific file(VFS_FILE_PATH) by stat
 * @apicovered       stat
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_stat_p(void)
{
	char *filename = VFS_STAT_PATH;
	struct stat st;
	int ret;

	/* Init */
	vfs_mount();
	vfs_mkdir(VFS_FOLDER_PATH);

	/* Testcase */
	ret = stat(filename, &st);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, vfs_unmount());

	/* stat root directory */
	ret = stat("/", &st);
	TC_ASSERT_EQ_CLEANUP("stat_CLEANUP", ret, OK, vfs_unmount());

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_stat_null_path_n
 * @brief            Get status of specific file
 * @scenario         Get status of null file path
 * @apicovered       stat
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_stat_null_path_n(void)
{
	int ret;
	struct stat st;
	/* Nagative case with invalid argument, NULL pathname. It will return ERROR */

	/* Testcase */
	ret = stat(NULL, &st);
	TC_ASSERT_EQ("stat", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_stat_invalid_path_n
 * @brief            Get status of specific file
 * @scenario         Get status of invalid file path
 * @apicovered       stat
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_stat_invalid_path_n(void)
{
	int ret;
	struct stat st;
	/* Nagative case with invalid argument, not existing pathname. It will return ERROR */

	/* Testcase */
	ret = stat(VFS_INVALID_FILE_PATH, &st);
	TC_ASSERT_EQ("stat", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_stat_empty_string_n
 * @brief            Get status of specific file
 * @scenario         Get status of empty string file path
 * @apicovered       stat
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_stat_empty_string_n(void)
{
	int ret;
	struct stat st;
	/*Negative testcase path is empty string */

	/* Testcase */
	ret = stat("", &st);
	TC_ASSERT_EQ("stat", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_statfs_p
 * @brief            Get status of mounted file system
 * @scenario         Get status of mounted file system by statfs and check type of file system
 * @apicovered       statfs
 * @precondition     File system should be mounted
 * @postcondition    NA
 */
static void tc_fs_vfs_statfs_p(void)
{
	struct statfs fs;
	int ret;

	/* Init */
	vfs_mount();

	/* Testcase */
	ret = statfs(MOUNT_DIR, &fs);
	TC_ASSERT_EQ_CLEANUP("statfs", ret, OK, vfs_unmount());
#ifdef CONFIG_FS_SMARTFS
	TC_ASSERT_EQ_CLEANUP("statfs", fs.f_type, SMARTFS_MAGIC, vfs_unmount());
#endif

	/*root pseudo file system */
	ret = statfs("/dev", &fs);
	TC_ASSERT_EQ_CLEANUP("statfs", ret, OK, vfs_unmount());

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_statfs_null_path_n
 * @brief            Get status of mounted file system
 * @scenario         Statfs and check type of null path
 * @apicovered       statfs
 * @precondition     File system should be mounted
 * @postcondition    NA
 */
static void tc_fs_vfs_statfs_null_path_n(void)
{
	struct statfs fs;
	int ret;
	/* Nagative case with invalid argument, NULL pathname. It will return ERROR */

	/* Testcase */
	ret = statfs(NULL, &fs);
	TC_ASSERT_EQ("statfs", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_fstat_p
 * @brief            Get status of specific file
 * @scenario         Get status of specific file(VFS_FILE_PATH) by stat
 * @apicovered       fstat
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_fstat_p(void)
{
	char *filename = VFS_FILE_PATH;
	struct stat st;
	int ret;
	int fd;

	/* Init */
	vfs_mount();

	ret = unlink(filename);
	TC_ASSERT_EQ_CLEANUP("unlink", ret, OK, vfs_unmount());

	fd = open(filename, O_RDWR | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	ret = fstat(fd, &st);
	TC_ASSERT_EQ_CLEANUP("fstat", ret, OK, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_fstat_invalid_fd_n
 * @brief            Get status of specific file
 * @scenario         Get status of invalid file descriptor
 * @apicovered       fstat
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_fstat_invalid_fd_n(void)
{
	struct stat st;
	int ret;
	int fd = -1;

	/* Testcase */
	ret = fstat(fd, &st);
	TC_ASSERT_NEQ_CLEANUP("fstat", ret, OK, close(fd));

	/* Deinit */
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_fstatfs_p
 * @brief            Get status of mounted file system
 * @scenario         Get status of mounted file system by statfs and check type of file system
 * @apicovered       fstatfs
 * @precondition     File system should be mounted
 * @postcondition    NA
 */
static void tc_fs_vfs_fstatfs_p(void)
{
	struct statfs fs;
	int ret;
	int fd;

	/* Init */
	vfs_mount();

	fd = open(VFS_FILE_PATH, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	ret = fstatfs(fd, &fs);
	TC_ASSERT_EQ_CLEANUP("fstatfs", ret, OK, close(fd); vfs_unmount());
#ifdef CONFIG_FS_SMARTFS
	TC_ASSERT_EQ_CLEANUP("fstatfs", fs.f_type, SMARTFS_MAGIC, close(fd); vfs_unmount());
#endif
	/* Deinit */
	close(fd);

#ifdef CONFIG_DEV_ZERO
	/* Init */
	fd = open(DEV_ZERO_PATH, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	/*root pseudo file system */
	ret = fstatfs(fd, &fs);
	TC_ASSERT_EQ_CLEANUP("fstatfs", ret, OK, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
#endif
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_fstatfs_invalid_fd_n
 * @brief            Get status of mounted file system
 * @scenario         Get status of invalid file descriptor
 * @apicovered       fstatfs
 * @precondition     File system should be mounted
 * @postcondition    NA
 */
static void tc_fs_vfs_fstatfs_invalid_fd_n(void)
{
	struct statfs fs;
	int ret;

	/* Testcase */
	ret = fstatfs(INVALID_DESCRIPTOR, &fs);
	TC_ASSERT_NEQ("fstatfs", ret, OK);

	TC_SUCCESS_RESULT();
}

#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
/**
 * @testcase         tc_fs_vfs_mkfifo_p
 * @brief            Get data thorugh the pipe which create by mkfifo
 * @scenario         Create fifo and check data between main task and its sub-thread
 * @apicovered       mkfifo, open, pthread_create, write, read
 * @precondition     CONFIG_PIPES should be enabled & CONFIG_DEV_PIPE_SIZE must greater than 11
 * @postcondition    NA
 */
static void tc_fs_vfs_mkfifo_p(void)
{
	int fd, ret;
	pthread_t tid;
	int count;
	char buf[12];
	size_t size;
	g_thread_result = true;

	/* Testcase */
	ret = mkfifo(FIFO_FILE_PATH, 0666);
	if (ret < 0) {
		TC_ASSERT_EQ("mkfifo", ret, -EEXIST);
	}

	ret = pthread_create(&tid, NULL, (pthread_startroutine_t) mkfifo_test_listener, NULL);
	TC_ASSERT_EQ("pthread_create", ret, 0);

	sleep(2);

	fd = open(FIFO_FILE_PATH, O_WRONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = write(fd, buf, 0);
	TC_ASSERT_EQ_CLEANUP("write", ret, 0, goto errout);

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
	sleep(1);
	pthread_kill(tid, SIGUSR1);
	TC_ASSERT_EQ("mkfifo", g_thread_result, true);

	fd = open(FIFO_FILE_PATH, O_WRONLY | O_NONBLOCK);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = ioctl(fd, PIPEIOC_POLICY, 0);
	TC_ASSERT_GEQ_CLEANUP("ioctl", ret, 0, goto errout);

	ret = ioctl(fd, PIPEIOC_POLICY, 1);
	TC_ASSERT_GEQ_CLEANUP("ioctl", ret, 0, goto errout);

	ret = ioctl(fd, -1, 0);
	TC_ASSERT_LT_CLEANUP("ioctl", ret, 0, goto errout);

	count = 0;
	snprintf(buf, size, "%s%d\0", FIFO_DATA, count);
	while (count <= CONFIG_DEV_PIPE_SIZE) {
		ret = write(fd, buf, size);
		TC_ASSERT_EQ_CLEANUP("write", (ret >= 0 || errno == EAGAIN), true, goto errout);
		if (ret < 0) {
			break;
		}
		count += size;
	}

	/* Deinit */
	close(fd);

	ret = unlink(FIFO_FILE_PATH);
	TC_ASSERT_GEQ("unlink", ret, 0);

	TC_SUCCESS_RESULT();
	return;
errout:
	pthread_kill(tid, SIGUSR1);
	close(fd);
}

/**
 * @testcase         tc_fs_vfs_mkfifo_exist_path_n
 * @brief            Get data thorugh the pipe which create by mkfifo
 * @scenario         Create fifo on exist path
 * @apicovered       mkfifo
 * @precondition     CONFIG_PIPES should be enabled & CONFIG_DEV_PIPE_SIZE must greater than 11
 * @postcondition    NA
 */
static void tc_fs_vfs_mkfifo_exist_path_n(void)
{
	int ret;
	struct stat st;

	/* Testcase */
	if (0 > stat(FIFO_FILE_PATH, &st)) {
		ret = mkfifo(FIFO_FILE_PATH, 0666);
		TC_ASSERT_EQ("mkfifo", ret, OK);

		ret = mkfifo(FIFO_FILE_PATH, 0666);
		TC_ASSERT_NEQ("mkfifo", ret, OK);
	} else {
		ret = mkfifo(FIFO_FILE_PATH, 0666);
		TC_ASSERT_NEQ("mkfifo", ret, OK);
	}

	TC_SUCCESS_RESULT();
}
#endif

/**
 * @testcase         tc_fs_vfs_sendfile_p
 * @brief            Send file data to specific descriptor from another descriptor
 * @scenario         Create new file and send data from exist file
 * @apicovered       open, stat, sendfile
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_sendfile_p(void)
{
	char *src_file = VFS_FILE_PATH;
	char dest_file[16];
	struct stat st;
	int fd1, fd2, ret;
	off_t size;
	off_t offset;
	char *str = "sendfile test";

	/* Init */
	vfs_mount();

	/* Testcase */
	snprintf(dest_file, sizeof(dest_file), "%s_dest", src_file);

	fd1 = open(src_file, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd1, 0, vfs_unmount());

	ret = pwrite(fd1, str, strlen(str), 0);
	TC_ASSERT_NEQ_CLEANUP("pwrite", ret, ERROR, close(fd1); vfs_unmount());

	ret = stat(src_file, &st);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, close(fd1); vfs_unmount());

	size = st.st_size;
	/* offset = 0 */
	fd2 = open(dest_file, O_WRONLY | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd2, 0, close(fd1); vfs_unmount());

	ret = sendfile(fd2, fd1, 0, size);
	close(fd2);
	TC_ASSERT_EQ_CLEANUP("sendfile", ret, size, close(fd1); vfs_unmount());

	ret = stat(dest_file, &st);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, close(fd1); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("stat", st.st_size, size, close(fd1); vfs_unmount());

	/* offset = 1 */
	fd2 = open(dest_file, O_WRONLY | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd2, 0, close(fd1); vfs_unmount());

	offset = 1;
	ret = sendfile(fd2, fd1, &offset, size - 1);
	close(fd2);
	TC_ASSERT_EQ_CLEANUP("sendfile", ret, size - 1, close(fd1); vfs_unmount());

	ret = stat(dest_file, &st);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, close(fd1); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("stat", st.st_size, size - 1, close(fd1); vfs_unmount());

	/* Deinit */
	close(fd1);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_sendfile_invalid_fd_n
 * @brief            Send file data to specific descriptor from another descriptor
 * @scenario         Create new file and send data from invalid file descriptor
 * @apicovered       open, stat, sendfile
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_sendfile_invalid_fd_n(void)
{
	char *src_file = VFS_FILE_PATH;
	char dest_file[16];
	int fd, ret;
	off_t size;
	off_t offset;
	struct stat st;
	char *str = "sendfile test";

	/* Init */
	vfs_mount();
	snprintf(dest_file, sizeof(dest_file), "%s_dest", src_file);

	/* Testcase */
	/* offset = 1, invalid input fd, returns ERROR */
	fd = open(dest_file, O_WRONLY | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	ret = pwrite(fd, str, strlen(str), 0);
	TC_ASSERT_NEQ_CLEANUP("pwrite", ret, ERROR, close(fd); vfs_unmount());

	ret = stat(dest_file, &st);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, close(fd); vfs_unmount());

	size = st.st_size;
	offset = 1;
	ret = sendfile(fd, INV_FD, &offset, size - 1);
	close(fd);
	TC_ASSERT_EQ_CLEANUP("sendfile", ret, ERROR, vfs_unmount());

	/* invalid input fd, returns ERROR */
	fd = open(dest_file, O_WRONLY | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	ret = sendfile(fd, INV_FD, NULL, size);
	TC_ASSERT_EQ_CLEANUP("sendfile", ret, ERROR, close(fd); vfs_unmount());

	/* offset = 0, invalid output fd, returns ERROR */
	offset = 0;
	ret = sendfile(INV_FD, fd, &offset, size);
	TC_ASSERT_EQ_CLEANUP("sendfile", ret, ERROR, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_sendfile_offset_eof_n
 * @brief            Send file data to specific descriptor from another descriptor
 * @scenario         Create new file and send data from end of file
 * @apicovered       open, stat, sendfile
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_sendfile_offset_eof_n(void)
{
	char *src_file = VFS_FILE_PATH;
	char dest_file[16];
	struct stat st;
	int fd1, fd2, ret;
	off_t size;
	char *str = "sendfile test";

	/* Init */
	vfs_mount();

	snprintf(dest_file, sizeof(dest_file), "%s_dest", src_file);

	fd1 = open(src_file, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd1, 0, vfs_unmount());

	ret = pwrite(fd1, str, strlen(str), 0);
	TC_ASSERT_NEQ_CLEANUP("pwrite", ret, ERROR, close(fd1); vfs_unmount());

	ret = stat(src_file, &st);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, close(fd1); vfs_unmount());

	size = st.st_size;

	/* current offset of input file is EOF, returns ERROR */
	fd2 = open(dest_file, O_WRONLY | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd2, 0, close(fd1); vfs_unmount());

	ret = lseek(fd1, 0, SEEK_END);
	TC_ASSERT_EQ_CLEANUP("lseek", ret, size, close(fd1); close(fd2); vfs_unmount());

	/* Testcase */
	ret = sendfile(fd2, fd1, NULL, size);
	close(fd2);
	TC_ASSERT_EQ_CLEANUP("sendfile", ret, 0, close(fd1); vfs_unmount());

	ret = stat(dest_file, &st);
	close(fd1);
	TC_ASSERT_EQ_CLEANUP("stat", ret, OK, vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("stat", st.st_size, 0, vfs_unmount());

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_fcntl_p
 * @brief            Access & control opened file with fcntl
 * @scenario         Open file with specific flag and get access mode with fcntl
 * @apicovered       open, fcntl
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_fcntl_p(void)
{
	int fd, mode;
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();

	fd = open(filename, O_WROK | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	mode = fcntl(fd, F_GETFL, 0) & O_ACCMODE;
	TC_ASSERT_EQ_CLEANUP("fcntl", mode, O_WROK, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_fcntl_invalid_fd_n
 * @brief            Access & control opened file with fcntl
 * @scenario         Invalid file descriptor with specific flag and get access mode with fcntl
 * @apicovered       fcntl
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_fcntl_invalid_fd_n(void)
{
	int ret;

	/* Testcase */
	ret = fcntl(INVALID_DESCRIPTOR, F_GETFL, 0);
	TC_ASSERT_EQ("fcntl", ret, ERROR);

	TC_SUCCESS_RESULT();
}

#ifndef CONFIG_DISABLE_POLL
/**
 * @testcase         tc_fs_vfs_poll_p
 * @brief            Polling for I/O
 * @scenario         Check poll works properly or not
 * @apicovered       poll
 * @precondition     CONFIG_DISABLE_POLL should be disabled
 * @postcondition    NA
 */
static void tc_fs_vfs_poll_p(void)
{
	struct pollfd pollfd;
	int ret;
	int fd;
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();

	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	pollfd.fd = fd;
	pollfd.events = POLLIN | POLLOUT;

	/* Poll regular file, it will always return positive value */
	ret = poll(&pollfd, 1, -1);

	TC_ASSERT_GT_CLEANUP("poll", ret, 0, close(fd); vfs_unmount());
	TC_ASSERT_CLEANUP("poll", pollfd.revents & POLLIN, close(fd); vfs_unmount());
	TC_ASSERT_CLEANUP("poll", pollfd.revents & POLLOUT, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
/**
 * @testcase         tc_fs_vfs_select_p
 * @brief            To monitor multiple I/O
 * @scenario         Input text within 5 sec and check select works properly(check change of STDIN)
 * @apicovered       select
 * @precondition     CONFIG_DISABLE_POLL should be disabled
 * @postcondition    NA
 */
static void tc_fs_vfs_select_p(void)
{
	struct timeval tv;
	fd_set readfds;
	int ret, errcnt;

	/* Init */
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	/* Testcase */
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
#endif

/**
 * @testcase         tc_fs_vfs_rename_p
 * @brief            Rename file to specific name
 * @scenario         Rename exist file to specific name
 * @apicovered       rename
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_rename_p(void)
{
	int fd;
	int ret;
	char *old_file = VFS_FILE_PATH;
	char new_file[12];

	/* Init */
	vfs_mount();

	fd = open(VFS_FILE_PATH, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());
	close(fd);

	snprintf(new_file, 12, "%s_re", old_file);
	unlink(new_file);

	/* Testcase */
	ret = rename(old_file, new_file);
	TC_ASSERT_EQ_CLEANUP("rename", ret, OK, vfs_unmount());

	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_rename_invalid_path_n
 * @brief            Rename file to specific name
 * @scenario         Rename invalid file path
 * @apicovered       rename
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_rename_invalid_path_n(void)
{
	int ret;
	char *invalid_file = VFS_INVALID_FILE_PATH;
	char new_file[20];
	/* Nagative case with invalid argument, not existing pathname. It will return ERROR */

	/* Init */
	snprintf(new_file, 12, "%s_re", invalid_file);

	/* Testcase */
	ret = rename(invalid_file, new_file);
	TC_ASSERT_EQ("rename", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_rename_exist_path_n
 * @brief            Rename file to specific name
 * @scenario         Rename old file to exist file
 * @apicovered       rename
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_rename_exist_path_n(void)
{
	int fd;
	int ret;
	char *exist_file = VFS_FILE_PATH;
	char old_file[12];

	/* Init */
	vfs_mount();

	fd = open(VFS_FILE_PATH, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());
	close(fd);

	snprintf(old_file, 12, "%s_re", exist_file);
	unlink(old_file);

	/* Testcase */
	ret = rename(exist_file, old_file);

	TC_ASSERT_EQ_CLEANUP("rename", ret, OK, vfs_unmount());

	/* Nagative case with invalid argument, already existing new pathname. It will return ERROR */
	fd = open(VFS_FILE_PATH, O_WROK | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());
	close(fd);
	ret = rename(old_file, exist_file);
	TC_ASSERT_EQ_CLEANUP("rename", ret, ERROR, vfs_unmount());

	/* Nagative case with invalid argument, NULL filepath. It will return ERROR */
	ret = rename(old_file, NULL);
	TC_ASSERT_EQ_CLEANUP("rename", ret, ERROR, vfs_unmount());

	/*Condition where rename is not possible*/
	ret = rename(DEV_NULL_PATH, DEV_NEW_NULL_PATH);
	TC_ASSERT_NEQ_CLEANUP("rename", ret, ERROR, vfs_unmount());

	ret = rename(DEV_NEW_NULL_PATH, DEV_NULL_PATH);
	TC_ASSERT_NEQ_CLEANUP("rename", ret, ERROR, vfs_unmount());

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_rename_null_path_n
 * @brief            Rename file to specific name
 * @scenario         Rename old file to exist file
 * @apicovered       rename
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_rename_null_path_n(void)
{
	int fd;
	int ret;
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();

	fd = open(VFS_FILE_PATH, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	/* Nagative case with invalid argument, NULL filepath. It will return ERROR */
	ret = rename(filename, NULL);
	TC_ASSERT_EQ_CLEANUP("rename", ret, ERROR, close(fd); vfs_unmount());

	/* Condition where rename is not possible */
	ret = rename(DEV_NULL_PATH, DEV_NEW_NULL_PATH);
	TC_ASSERT_NEQ_CLEANUP("rename", ret, ERROR, close(fd); vfs_unmount());

	ret = rename(DEV_NEW_NULL_PATH, DEV_NULL_PATH);
	TC_ASSERT_NEQ_CLEANUP("rename", ret, ERROR, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_rename_not_possible_n
 * @brief            Rename file to specific name
 * @scenario         Rename dev path
 * @apicovered       rename
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_fs_vfs_rename_not_possible_n(void)
{
	int ret;

	/* Testcase */
	/*Condition where rename is not possible*/
	ret = rename(DEV_NULL_PATH, DEV_NEW_NULL_PATH);
	TC_ASSERT_NEQ("rename", ret, ERROR);

	ret = rename(DEV_NEW_NULL_PATH, DEV_NULL_PATH);
	TC_ASSERT_NEQ("rename", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_ioctl_p
 * @brief            ioctl with opened file
 * @scenario         Get #byte of data from /dev/console by ioctl
 * @apicovered       open, ioctl
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_ioctl_p(void)
{
	int fd;
	int ret;
	long size;

	/* Init */
	fd = open(DEV_CONSOLE_PATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd, 0);

	/* Testcase */
	ret = ioctl(fd, FIONREAD, (unsigned long)&size);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd));

	/* Deinit */
	close(fd);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_ioctl_invalid_fd_n
 * @brief            ioctl with opened file
 * @scenario         Get #byte of data from invalid file descriptor by ioctl
 * @apicovered       ioctl
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_ioctl_invalid_fd_n(void)
{
	int ret;
	long size;

	/* Testcase */
	/*Negative case where invalid fd */
	ret = ioctl(INV_FD, FIONREAD, (unsigned long)&size);
	TC_ASSERT_EQ("ioctl", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_vfs_ioctl_invalid_cmd_n
 * @brief            ioctl with opened file
 * @scenario         Ioctl call with invalid cmd
 * @apicovered       open, ioctl
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_vfs_ioctl_invalid_cmd_n(void)
{
	int fd;
	int ret;
	long size;

	/*Negative cae where invalid cmd */

	/* Init */
	fd = open(DEV_CONSOLE_PATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd, 0);

	/* Testcase */
	ret = ioctl(fd, FIONREAD, (unsigned long)&size);
	TC_ASSERT_LEQ_CLEANUP("ioctl", ret, 0, close(fd));

	/* Deinit */
	close(fd);

	TC_SUCCESS_RESULT();
}

#if defined(CONFIG_MTD_CONFIG)
/**
 * @testcase         tc_driver_mtd_config_ops
 * @brief            mtd_config operations
 * @scenario         Set and get config from /dev/config
 * @apicovered       mtd_config ops
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_driver_mtd_config_ops(void)
{
	int fd;
	int ret;
	struct config_data_s config;
	char buf[8] = "test";

	fd = open(MTD_CONFIG_PATH, O_RDOK);
	TC_ASSERT_GEQ("open", fd, 0);

	config.id = 0xff;
	config.instance = 0;
	config.configdata = (unsigned char *)buf;
	config.len = 5;

	ret = ioctl(fd, CFGDIOC_SETCONFIG, &config);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd));

	ret = ioctl(fd, CFGDIOC_GETCONFIG, &config);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd));

	/*To cover negative condition */

	ret = ioctl(fd, CFGDIOC_SETCONFIG, NULL);
	TC_ASSERT_NEQ_CLEANUP("ioctl", ret, OK, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, OK);

	TC_SUCCESS_RESULT();
}
#endif

/**
 * @testcase         tc_libc_stdio_dprintf_p
 * @brief            Exact analogs of fprintf and vfprintf, except that they output to a file descriptor fd instead of to a stdio stream.
 * @scenario         Exact analogs of fprintf and vfprintf, except that they output to a file descriptor fd instead of to a stdio stream.
 * @apicovered       dprintf
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_dprintf_p(void)
{
	char *filename = VFS_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_1;
	int fd;
	int ret;

	/* Init */
	vfs_mount();
	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC);
	TC_ASSERT_GEQ("open", fd, 0);

	/* Testcase */
	ret = dprintf(fd, "%s", str);
	TC_ASSERT_EQ_CLEANUP("dprintf", ret, strnlen(str, VFS_CONTENTS_LEN), close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_dprintf_invalid_fd_n
 * @brief            Exact analogs of fprintf and vfprintf, except that they output to a file descriptor fd instead of to a stdio stream.
 * @scenario         Output string as invalid file descriptor
 * @apicovered       dprintf
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_dprintf_invalid_fd_n(void)
{
	char *str = VFS_TEST_CONTENTS_1;
	int ret;

	/* Testcase */
	ret = dprintf(INVALID_DESCRIPTOR, "%s", str);
	TC_ASSERT_EQ("dprintf", ret, ERROR);
	TC_ASSERT_EQ("dprintf", errno, EBADF);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fdopen_p
 * @brief            fdopen with available fd value
 * @scenario         Open file with specific flags, and then fdopen with different flag. Then check flag is changed properly
 * @apicovered       open, fdopen
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_libc_stdio_fdopen_p(void)
{
	int fd;
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();
	fd = open(filename, O_WROK | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);

	/* Testcase */
	fp = fdopen(fd, "r");
	TC_ASSERT_NEQ_CLEANUP("fdopen", fp, NULL, close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("fdopen", fp->fs_oflags, O_RDONLY, fclose(fp); close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fdopen_invalid_fd_n
 * @brief            fdopen with available fd value
 * @scenario         Open file with invalid file pointer
 * @apicovered       fdopen
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_libc_stdio_fdopen_invalid_fd_n(void)
{
	FILE *fp;

	/* Nagative case with invalid argument, negative fd value. It will return NULL */
	/* Testcase */
	fp = fdopen(-1, "r");
	TC_ASSERT_EQ_CLEANUP("fdopen", fp, NULL, fclose(fp));

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fopen_p
 * @brief            Open file by fopen
 * @scenario         Open file
 * @apicovered       fopen
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_fopen_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();

	/* Testcase */
	fp = fopen(filename, "w");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());
	fclose(fp);

	fp = fopen(filename, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());
	fclose(fp);

	fp = fopen(filename, "rb");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());
	fclose(fp);

	fp = fopen(filename, "rx");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());
	fclose(fp);

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fopen_invalid_mode_n
 * @brief            Open file by fopen
 * @scenario         Open file with invalid mode
 * @apicovered       fopen
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_fopen_invalid_mode_n(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	/* Nagative cases with invalid mode. It will return NULL */
	/* Testcase */
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
 * @testcase         tc_libc_stdio_fclose_p
 * @brief            Close file by fopen
 * @scenario         Open and Close file
 * @apicovered       fopen, fclose
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_fclose_p(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "w");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	ret = fclose(fp);
	TC_ASSERT_EQ_CLEANUP("fclose", ret, OK, vfs_unmount());

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fclose_invalid_fp_n
 * @brief            Close file by fopen
 * @scenario         Open and Close file
 * @apicovered       fclose
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_fclose_invalid_fp_n(void)
{
	FILE *fp = NULL;
	int ret;

	/* Testcase */
	ret = fclose(fp);
	TC_ASSERT_EQ("fclose", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fputs_p
 * @brief            Write contents through the fputs
 * @scenario         Write contents through the fputs, and check it works properly or not
 * @apicovered       fopen, fputs
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_libc_stdio_fputs_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_1;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	TC_ASSERT_EQ_CLEANUP("fputs", fputs(str, fp), strlen(str), fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fputs_null_stream_n
 * @brief            Write contents through the fputs
 * @scenario         Write contents through the fputs in null stream
 * @apicovered       fopen, fputs
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_libc_stdio_fputs_null_stream_n(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "w");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Nagative case with invalid argument, NULL stream. It will return EOF */
	/* Testcase */
	TC_ASSERT_EQ_CLEANUP("fputs", fputs(NULL, fp), EOF, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fgets_p
 * @brief            Read cntents through the fgets
 * @scenario         Read contents through the fgets, and check it is same contents as written by tc_libc_stdio_fgets
 * @apicovered       fopen, fgets
 * @precondition     tc_libc_stdio_fputs  should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_fgets_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char buf[20];
	char *str = VFS_TEST_CONTENTS_1;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("fputs", fputs(str, fp), strlen(str), fclose(fp); vfs_unmount());
	fclose(fp);

	fp = fopen(filename, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());
	memset(buf, 0, sizeof(buf));

	/* Testcase */
	TC_ASSERT_NEQ_CLEANUP("fgets", fgets(buf, 20, fp), NULL, fclose(fp); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("fgets", strcmp(buf, VFS_TEST_CONTENTS_1), 0, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fgets_buffer_size_n
 * @brief            Read cntents through the fgets
 * @scenario         Read contents of invalid buf size via fgets
 * @apicovered       fopen, fgets
 * @precondition     tc_libc_stdio_fputs  should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_fgets_buffer_size_n(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char buf[20];

	/* Init */
	vfs_mount();
	fp = fopen(filename, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	/* Nagative case with invalid argument, negative buffer size. It will return NULL */
	TC_ASSERT_EQ_CLEANUP("fgets", fgets(buf, -1, fp), NULL, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fseek_p
 * @brief            Move current file position to specific position
 * @scenario         Open file and move position, and then read data to check lseek works properly or not.
 * @apicovered       fopen, fseek, fgets
 * @precondition     tc_libc_stdio_fputs  should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_fseek_p(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;
	char buf[20];

	/* Init */
	vfs_mount();
	fp = fopen(filename, "r");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	ret = fseek(fp, 5, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("fseek", ret, OK, fclose(fp); vfs_unmount());

	memset(buf, 0, sizeof(buf));
	TC_ASSERT_NEQ_CLEANUP("fgets", fgets(buf, 20, fp), NULL, fclose(fp); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("fgets", strcmp(buf, "IS VFS TEST 1"), 0, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fseek_null_stream_n
 * @brief            Move current file position to specific position
 * @scenario         Try move position in null stream
 * @apicovered       fopen, fseek, fgets
 * @precondition     tc_libc_stdio_fputs  should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_fseek_null_stream_n(void)
{
	int ret;

	/* Nagative case with invalid argument, NULL stream. It will return ERROR */
	/* Testcase */
	ret = fseek(NULL, 5, SEEK_SET);
	TC_ASSERT_EQ("fseek", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_ftell_p
 * @brief            Get current file position
 * @scenario         Open file and move position, and get current position by ftell
 * @apicovered       fopen, fseek, ftell
 * @precondition     tc_libc_stdio_fputs  should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_ftell_p(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());
	ret = fseek(fp, 5, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("fseek", ret, OK, fclose(fp); vfs_unmount());

	/* Testcase */
	ret = ftell(fp);
	TC_ASSERT_EQ_CLEANUP("ftell", ret, 5, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_ftell_null_stream_n
 * @brief            Get current file position
 * @scenario         Try move position in null stream
 * @apicovered       ftell
 * @precondition     tc_libc_stdio_fputs  should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_ftell_null_stream_n(void)
{
	int ret;

	/* Nagative case with invalid argument, NULL stream. It will return ERROR */
	ret = ftell(NULL);
	TC_ASSERT_EQ("ftell", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_feof_p
 * @brief            Check file pointer is positioned at the end of file
 * @scenario         Make long size file and print contents until it reaches at the end of file
 * @apicovered       fopen, feof
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_feof_p(void)
{
	FILE *fp;
	char *filename = LONG_FILE_PATH;
	char buf[32];

	/* Init */
	vfs_mount();

	TC_ASSERT_NEQ("make_long_file", make_long_file(), ERROR);

	fp = fopen(filename, "r");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	/* Print contents of long file, if below became infinite loop, it means failed */
	while (!feof(fp)) {
		fgets(buf, 32, fp);
		printf("%s", buf);
		memset(buf, 0, 32);
	}
	printf("\n");

	/* Deinit */	
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fprintf_p
 * @brief            Write contents.
 * @scenario         Write contents through the fprintf.
 * @apicovered       fopen, fprintf
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_fprintf_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_2;
	int ret;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	ret = fprintf(fp, "%s", str);
	TC_ASSERT_EQ_CLEANUP("fprintf", ret, strlen(VFS_TEST_CONTENTS_2), fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fprintf_invalid_fp_n
 * @brief            Write contents.
 * @scenario         Write contents through the fprintf in invalid file point
 * @apicovered       fprintf
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_fprintf_invalid_fp_n(void)
{
	FILE *fp = NULL;
	char *str = VFS_TEST_CONTENTS_2;
	int ret;

	/* Testcase */
	ret = fprintf(fp, "%s", str);
	TC_ASSERT_EQ("fprintf", ret, EOF);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fsetpos_p
 * @brief            Set current file pointer to specific position
 * @scenario         Set new position of file pointer & check read contents to check it works properly
 * @apicovered       fopen, fsetpos, fgetc
 * @precondition     tc_libc_stdio_fprintf should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_fsetpos_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	fpos_t pos = 3;
	int ch, ret;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	ret = fsetpos(fp, &pos);
	TC_ASSERT_EQ_CLEANUP("fsetpos", ret, OK, fclose(fp); vfs_unmount());

	ch = fgetc(fp);

	TC_ASSERT_NEQ_CLEANUP("fgetc", ch, EOF, fclose(fp); vfs_unmount());
	/* 'S' is 4th position of "THIS IS VFS TEST 2" */
	TC_ASSERT_EQ_CLEANUP("fgetc", ch, 'S', fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fsetpos_null_position_n
 * @brief            Set current file pointer to specific position
 * @scenario         Set new position of null file pointer
 * @apicovered       fopen, fsetpos, fgetc
 * @precondition     tc_libc_stdio_fprintf should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_fsetpos_null_position_n(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ret;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	/* Nagative case with invalid arguments, NULL position. It will return ERROR */
	ret = fsetpos(fp, NULL);
	TC_ASSERT_EQ_CLEANUP("fsetpos", ret, ERROR, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fsetpos_invalid_fp_n
 * @brief            Set current file pointer to specific position
 * @scenario         Set new position of invalid file pointer
 * @apicovered       fopen, fsetpos, fgetc
 * @precondition     tc_libc_stdio_fprintf should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_fsetpos_invalid_fp_n(void)
{
	FILE *fp = NULL;
	fpos_t pos = 3;
	int ret;

	/* Testcase */
	ret = fsetpos(fp, &pos);
	TC_ASSERT_EQ("fsetpos", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fgetpos_p
 * @brief            Get current file pointer
 * @scenario         Set new position of file pointer & check current position by fgetpos
 * @apicovered       fopen, fsetpos, fgetpos
 * @precondition     tc_libc_stdio_fprintf should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_fgetpos_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	fpos_t pos = 2;
	int ret;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	ret = fsetpos(fp, &pos);
	TC_ASSERT_EQ_CLEANUP("fsetpos", ret, OK, fclose(fp); vfs_unmount());

	/* Testcase */
	ret = fgetpos(fp, &pos);
	TC_ASSERT_EQ_CLEANUP("fgetpos", ret, OK, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fgetpos_null_position_n
 * @brief            Get current file pointer
 * @scenario         Set new position of null position
 * @apicovered       fopen, fsetpos, fgetpos
 * @precondition     tc_libc_stdio_fprintf should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_fgetpos_null_position_n(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ret;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	/* Nagative case with null position. It will return ERROR */
	ret = fgetpos(fp, NULL);
	TC_ASSERT_EQ_CLEANUP("fgetpos", ret, ERROR, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fgetpos_invalid_fp_n
 * @brief            Get current file pointer
 * @scenario         Set new position of invalid file pointer
 * @apicovered       fopen, fsetpos, fgetpos
 * @precondition     tc_libc_stdio_fprintf should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_fgetpos_invalid_fp_n(void)
{
	fpos_t pos = 2;
	int ret;

	/* Nagative case with invalid fp. It will return ERROR */
	ret = fgetpos(NULL, &pos);
	TC_ASSERT_EQ("fgetpos", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fputc_p
 * @brief            Put character to file
 * @scenario         Put character 'S' to file
 * @apicovered       fopen, fputc
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_fputc_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ret;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	ret = fputc('S', fp);
	TC_ASSERT_GEQ_CLEANUP("fputc", ret, 0, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fputc_invalid_fp_n
 * @brief            Put character to file
 * @scenario         Put character 'S' to null file pointer
 * @apicovered       fopen, fputc
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_fputc_invalid_fp_n(void)
{
	FILE *fp = NULL;
	int ret;

	/* Testcase */
	ret = fputc('S', fp);
	TC_ASSERT_EQ("fputc", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fgetc_p
 * @brief            get character from file
 * @scenario         get character from file and check it is same as which it put in tc_libc_stdio_fputc
 * @apicovered       fopen, fgetc
 * @precondition     tc_libc_stdio_fputc should pass
 * @postcondition    NA
 */
static void tc_libc_stdio_fgetc_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ch;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	ch = fgetc(fp);
	TC_ASSERT_NEQ_CLEANUP("fgetc", ch, EOF, fclose(fp); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("fgetc", ch, 'S', fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fgetc_invalid_fp_n
 * @brief            get character from file
 * @scenario         get character from null file descriptor
 * @apicovered       fgetc
 * @precondition     tc_libc_stdio_fputc should pass
 * @postcondition    NA
 */
static void tc_libc_stdio_fgetc_invalid_fp_n(void)
{
	FILE *fp = NULL;
	int ch;

	/* Testcase */
	ch = fgetc(fp);
	TC_ASSERT_EQ("fgetc", ch, EOF);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fwrite_p
 * @brief            Write 1 line of contents
 * @scenario         Write 3 lines of contents and check its return value
 * @apicovered       fwrite
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_fwrite_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int len, ret;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	len = strlen(VFS_TEST_CONTENTS_1);
	ret = fwrite(VFS_TEST_CONTENTS_1, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fwrite", ret, len, fclose(fp); vfs_unmount());

	len = strlen(VFS_TEST_CONTENTS_2);
	ret = fwrite(VFS_TEST_CONTENTS_2, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fwrite", ret, len, fclose(fp); vfs_unmount());

	len = strlen(VFS_TEST_CONTENTS_3);
	ret = fwrite(VFS_TEST_CONTENTS_3, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fwrite", ret, len, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fwrite_invalid_fp_n
 * @brief            Write 1 line of contents
 * @scenario         Write line in invalid file descriptor
 * @apicovered       fwrite
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_fwrite_invalid_fp_n(void)
{
	FILE *fp = NULL;
	int len, ret;

	/* Testcase */
	len = strlen(VFS_TEST_CONTENTS_1);
	ret = fwrite(VFS_TEST_CONTENTS_1, 1, len, fp);
	TC_ASSERT_NEQ("fwrite", ret, len);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fread_p
 * @brief            Read 1 line of contents
 * @scenario         Read 3 lines of contents and check its same as contents which written in tc_libc_stdio_fwrite
 * @apicovered       fopen, fread
 * @precondition     tc_libc_stdio_fwrite should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_fread_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char buf[20];
	int len, ret;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	len = strlen(VFS_TEST_CONTENTS_1);
	memset(buf, 0, sizeof(buf));
	ret = fread(buf, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fread", ret, len, fclose(fp); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("fread", strcmp(buf, VFS_TEST_CONTENTS_1), 0, fclose(fp); vfs_unmount());

	len = strlen(VFS_TEST_CONTENTS_2);
	memset(buf, 0, sizeof(buf));
	ret = fread(buf, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fread", ret, len, fclose(fp); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("fread", strcmp(buf, VFS_TEST_CONTENTS_2), 0, fclose(fp); vfs_unmount());

	len = strlen(VFS_TEST_CONTENTS_3);
	memset(buf, 0, sizeof(buf));
	ret = fread(buf, 1, len, fp);
	TC_ASSERT_EQ_CLEANUP("fread", ret, len, fclose(fp); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("fread", strcmp(buf, VFS_TEST_CONTENTS_3), 0, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fread_invalid_fp_n
 * @brief            Read 1 line of contents
 * @scenario         Read line in null file descriptor
 * @apicovered       fopen, fread
 * @precondition     tc_libc_stdio_fwrite should be passed
 * @postcondition    NA
 */
static void tc_libc_stdio_fread_invalid_fp_n(void)
{
	FILE *fp = NULL;
	char buf[20];
	int len, ret;

	/* Testcase */
	len = strlen(VFS_TEST_CONTENTS_1);
	memset(buf, 0, sizeof(buf));
	ret = fread(buf, 1, len, fp);
	TC_ASSERT_NEQ("fread", ret, len);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_freopen_p
 * @brief            Open file by freopen
 * @scenario         Open file
 * @apicovered       freopen
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_freopen_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();

	/* Testcase */
	fp = freopen(filename, "w+", NULL);
	TC_ASSERT_NEQ_CLEANUP("freopen", fp, NULL, vfs_unmount());

	/* A case with non-NULL filename and valid stream */
	fp = freopen(filename, "w+", fp);
	TC_ASSERT_NEQ_CLEANUP("freopen", fp, NULL, vfs_unmount());

	/* A case with NULL filename, valid stream and mode flag */
	fp = freopen(NULL, "r+", fp);
	TC_ASSERT_NEQ_CLEANUP("freopen", fp, NULL, fclose(fp); vfs_unmount(););

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_freopen_invalid_mode_n
 * @brief            Open file by freopen
 * @scenario         Open file in null file descriptor and invalid mode
 * @apicovered       freopen
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_freopen_invalid_mode_n(void)
{
	FILE *fp = NULL;
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();

	/* Testcase */
	/* A negative case invalid mode. It will return NULL */
	fp = freopen(filename, "z", fp);
	TC_ASSERT_EQ_CLEANUP("freopen", fp, NULL, vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_freopen_null_path_n
 * @brief            Open file by freopen
 * @scenario         Open file null file descriptor and path
 * @apicovered       freopen
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_freopen_null_path_n(void)
{
	FILE *fp = NULL;;

	/* Init */
	vfs_mount();

	/* Testcase */
	/* A negative case with NULL path. It will return NULL */
	fp = freopen(NULL, "w+", fp);
	TC_ASSERT_EQ_CLEANUP("freopen", fp, NULL, vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_freopen_null_path_and_stream_n
 * @brief            Open file by freopen
 * @scenario         Open file null file null path and stream
 * @apicovered       freopen
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_freopen_null_path_and_stream_n(void)
{
	FILE *fp;

	/* Testcase */
	/* A negative case with NULL path and stream. It will return NULL */
	fp = freopen(NULL, "w+", NULL);
	TC_ASSERT_EQ("freopen", fp, NULL);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_ferror_p
 * @brief            Check error occured during operation
 * @scenario         Write data to file which opened with read only flag to make an error forcely and check ferror works properly
 * @apicovered       fopen, fputc, ferror
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_libc_stdio_ferror_p(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "r");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	ret = fputc(32, fp);
	TC_ASSERT_EQ_CLEANUP("fputc", ret, EOF, fclose(fp); vfs_unmount());

	/* Testcase */
	ret = ferror(fp);
	TC_ASSERT_NEQ_CLEANUP("ferror", ret, OK, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_clearerr_p
 * @brief            Check error cleared by clearerr after error occured during operation
 * @scenario         Write data to file which opened with read only flag to make an error forcely and
 *                   check ferror works properly and then check error disappeared by clearerr
 * @apicovered       fopen, fputc, ferror, clearerr
 * @precondition     File VFS_FILE_PATH should be existed
 * @postcondition    NA
 */
static void tc_libc_stdio_clearerr_p(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "r");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	ret = fputc(32, fp);
	TC_ASSERT_EQ_CLEANUP("fputc", ret, EOF, fclose(fp); vfs_unmount());

	ret = ferror(fp);
	TC_ASSERT_NEQ_CLEANUP("ferror", ret, OK, fclose(fp); vfs_unmount());

	/* Testcase */
	clearerr(fp);
	ret = ferror(fp);
	TC_ASSERT_EQ_CLEANUP("ferror", ret, OK, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
/**
 * @testcase         tc_libc_stdio_gets_p
 * @brief            get string by user input
 * @scenario         get string by user input and check it is NULL or not
 * @apicovered       gets
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_gets_p(void)
{
	char input_str[64];

	/* Testcase */
	printf("Enter text here : \n");
	fflush(stdout);
	TC_ASSERT_NEQ("gets", gets(input_str), NULL);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_gets_null_buf_n
 * @brief            get string by user input
 * @scenario         Check the result by null argument to gets
 * @apicovered       gets
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_gets_null_buf_n(void)
{
	/* Testcase */
	printf("Enter text here : \n");
	fflush(stdout);
	TC_ASSERT_EQ("gets", gets(NULL), NULL);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_gets_s_p
 * @brief            get string by user input
 * @scenario         get string by user input and check it is NULL or not
 * @apicovered       gets
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_gets_s_p(void)
{
	char input_str[64];

	/* Testcase */
	printf("Enter text here : \n");
	fflush(stdout);
	TC_ASSERT_NEQ("gets_s", gets_s(input_str, sizeof(input_str)), NULL);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_gets_s_buflen_n
 * @brief            get string by user input
 * @scenario         Get a string of the invalid length
 * @apicovered       gets
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_gets_s_buflen_n(void)
{
	char input_str[64];

	/* Testcase */
	printf("Enter text here : \n");
	fflush(stdout);
	TC_ASSERT_EQ("gets_s", gets_s(input_str, 1), input_str);

	TC_SUCCESS_RESULT();
}
#endif

/**
 * @testcase         tc_libc_stdio_fileno_p
 * @brief            Get fd value related to file stream
 * @scenario         Open file with fopen and get fd value through the fileno
 * @apicovered       fopen, fileno
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_fileno_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int fd;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	fd = fileno(fp);
	TC_ASSERT_GEQ_CLEANUP("fileno", fd, 0, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_fileno_null_stream_n
 * @brief            Get fd value related to file stream
 * @scenario         Call fileno with a null file pointer
 * @apicovered       fopen, fileno
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_fileno_null_stream_n(void)
{
	int fd;

	/* Testcase */
	/* Nagative case with invalid argument, NULL stream. It will return ERROR */
	fd = fileno(NULL);
	TC_ASSERT_EQ("fileno", fd, ERROR);

	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_STDIO_LINEBUFFER
/**
 * @testcase         tc_libc_stdio_lib_snoflush_p
 * @brief            provides a common, dummy flush method for seekable output streams that are not flushable.
 * @scenario         Only used if CONFIG_STDIO_LINEBUFFER is selected.
 * @apicovered       lib_snoflush
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_lib_snoflush_p(void)
{
	int ret;

	ret = lib_snoflush((FAR struct lib_sostream_s *)stdout);
	TC_ASSERT_EQ("lib_snoflush", ret, OK);

	TC_SUCCESS_RESULT();
}
#endif

/**
 * @testcase         tc_libc_stdio_lib_sprintf_p
 * @brief            Composes a string with the same text that would be printed if format was used on printf
 *                   But instead of being printed the content is stored as a C string in the buffer pointed by str
 * @scenario         A terminating null character is automatically appended after the content.
 * @apicovered       lib_sprintf
 * @precondition     The size of the buffer should be large enough to contain the entire resulting string
 * @postcondition    NA
 */
static void tc_libc_stdio_lib_sprintf_p(void)
{
	struct lib_memoutstream_s memoutstream;
	char *str = VFS_TEST_CONTENTS_1;
	char buf[STDIO_BUFLEN];
	int ret;

	lib_memoutstream((FAR struct lib_memoutstream_s *)&memoutstream, buf, STDIO_BUFLEN);
	TC_ASSERT_EQ("lib_memoutstream", memoutstream.buffer, (FAR char *)(buf));

	ret = lib_sprintf((FAR struct lib_outstream_s *)&memoutstream, "%s", str);
	TC_ASSERT_EQ("lib_sprintf", ret, strnlen(str, VFS_CONTENTS_LEN));

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_remove_p
 * @brief            Deletes the file whose name is specified in filename.
 * @scenario         Open file and remove
 * @apicovered       remove
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_remove_p(void)
{
	char *filename = VFS_FILE_PATH;
	int ret;
	int fd;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_WROK | O_CREAT);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());
	close(fd);

	/* Testcase */
	ret = remove(filename);
	TC_ASSERT_EQ("remove", ret, OK);

	/* Deinit */
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_remove_invalid_path_n
 * @brief            Deletes the file whose name is specified in filename.
 * @scenario         remove null path
 * @apicovered       remove
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_remove_invalid_path_n(void)
{
	int ret;

	ret = remove(NULL);
	TC_ASSERT_NEQ("remove", ret, OK);

	TC_SUCCESS_RESULT();
}

#if CONFIG_STDIO_BUFFER_SIZE > 0
/**
 * @testcase         tc_libc_stdio_setbuf_p
 * @brief            Set stream buffer
 * @scenario         Open file
 * @apicovered       setbuf, setvbuf
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_setbuf_p(void)
{
	FILE *fp;
	char buffer[64];
	char *filename = VFS_FILE_PATH;

	/* Init */
	vfs_mount();

	/* setbuf_test: DEFAULT buffering */
	fp = fopen(filename, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* Testcase */
	/* setbuf_test: NO buffering */
	setbuf(fp, NULL);
	TC_ASSERT_EQ_CLEANUP("setbuf", fp->fs_bufstart, NULL, fclose(fp); vfs_unmount());

	/* setbuf_test: pre-allocated buffer */
	setbuf(fp, buffer);
	TC_ASSERT_EQ_CLEANUP("setbuf", fp->fs_bufstart, (FAR unsigned char *)buffer, fclose(fp); vfs_unmount());
	setbuf(fp, NULL);
	TC_ASSERT_EQ_CLEANUP("setbuf", fp->fs_bufstart, NULL, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}
#endif

#if CONFIG_STDIO_BUFFER_SIZE > 0
/**
 * @testcase         tc_libc_stdio_setvbuf_p
 * @brief            Change stream buffering
 * @scenario         Open file
 * @apicovered       setbuf, setvbuf
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_setvbuf_p(void)
{
	FILE *fp;
	char buffer[64];
	char *filename = VFS_FILE_PATH;
	int ret;
	int fp_fd;

	/* Init */
	vfs_mount();

	/* setvbuf_test: DEFAULT buffering */
	fp = fopen(filename, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	/* setvbuf_test: fail to file open */
	fp_fd = fp->fs_fd;
	fp->fs_fd = ERROR;
	ret = setvbuf(fp, buffer, _IOFBF, 64);
	fp->fs_fd = fp_fd;
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("setvbuf", errno, EBADF, fclose(fp); vfs_unmount());

	/* setvbuf_test: Invalid mode check */
	ret = setvbuf(fp, NULL, ERROR, 0);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("setvbuf", errno, EINVAL, fclose(fp); vfs_unmount());

	/* setvbuf_test: EINVAL error check
	 * In case that if a buffer pointer is provided and the buffer size is zero
     */
	ret = setvbuf(fp, buffer, _IOFBF, 0);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("setvbuf", errno, EINVAL, fclose(fp); vfs_unmount());

	/* setvbuf_test: EBUSY test */
	fp->fs_bufpos++;
	ret = setvbuf(fp, NULL, _IOFBF, 64);
	fp->fs_bufpos = fp->fs_bufstart;
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("setvbuf", errno, EBUSY, fclose(fp); vfs_unmount());

	/* setvbuf_test: NO buffering */
	ret = setvbuf(fp, NULL, _IONBF, 0);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp); vfs_unmount());

	/* setvbuf_test: FULL buffering */
	ret = setvbuf(fp, NULL, _IOFBF, 0);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp); vfs_unmount());

	/* setvbuf_test: Reuse full buffering */
	ret = setvbuf(fp, NULL, _IOFBF, 0);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp); vfs_unmount());
	ret = setvbuf(fp, NULL, _IONBF, 0);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp); vfs_unmount());

	/* setvbuf_test: LINE buffering */
	ret = setvbuf(fp, NULL, _IOLBF, 64);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp); vfs_unmount());
	ret = setvbuf(fp, NULL, _IONBF, 0);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp); vfs_unmount());

	/* setvbuf_test: FULL buffering, pre-allocated buffer */
	ret = setvbuf(fp, buffer, _IOFBF, 64);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp); vfs_unmount());
	ret = setvbuf(fp, NULL, _IONBF, 0);
	TC_ASSERT_LEQ_CLEANUP("setvbuf", ret, 0, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}
#endif

/**
 * @testcase         tc_libc_stdio_meminstream_p
 * @brief            Initializes a stream for use with a fixed-size memory buffer
 * @scenario         Initializes a stream for use with a fixed-size memory buffer
 * @apicovered       lib_meminstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_meminstream_p(void)
{
	const char *str = STREAM_TEST_CONTENTS;
	int getch;

	struct lib_meminstream_s meminstream;

	/* Testcase */
	/* Check with 0 length */
	lib_meminstream((FAR struct lib_meminstream_s *)&meminstream, str, 0);
	TC_ASSERT_EQ("lib_meminstream", meminstream.buffer, (FAR char *)(str));

	getch = meminstream.public.get((FAR struct lib_instream_s *)&meminstream.public);
	TC_ASSERT_EQ("meminstream_getc", meminstream.public.nget, 0);
	TC_ASSERT_EQ("meminstream_getc", getch, EOF);

	/* Check with valid length */
	lib_meminstream((FAR struct lib_meminstream_s *)&meminstream, str, STDIO_BUFLEN);
	TC_ASSERT_EQ("lib_meminstream", meminstream.buffer, (FAR char *)(str));
	TC_ASSERT_EQ("lib_meminstream", meminstream.buflen, STDIO_BUFLEN);

	getch = meminstream.public.get((FAR struct lib_instream_s *)&meminstream.public);
	TC_ASSERT_EQ("meminstream_getc", meminstream.public.nget, 1);
	TC_ASSERT_EQ("meminstream_getc", getch, str[0]);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_memoutstream_p
 * @brief            Initializes a stream for use with a fixed-size memory buffer
 * @scenario         Initializes a stream for use with a fixed-size memory buffer
 * @apicovered       lib_memoutstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_memoutstream_p(void)
{
	char buffer[STDIO_BUFLEN];
	char *str = STREAM_TEST_CONTENTS;

	struct lib_memoutstream_s memoutstream;

	/* Check with valid length */

	lib_memoutstream((FAR struct lib_memoutstream_s *)&memoutstream, buffer, STDIO_BUFLEN);
	TC_ASSERT_EQ("lib_memoutstream", memoutstream.buffer, (FAR char *)(buffer));
	TC_ASSERT_EQ("lib_memoutstream", memoutstream.buflen, (STDIO_BUFLEN - 1));	/* Save space for null terminator, hence checking with (STDIO_BUFLEN-1)*/

	memoutstream.public.put((FAR struct lib_outstream_s *)&memoutstream.public, str[0]);
	TC_ASSERT_EQ("memoutstream_putc", memoutstream.public.nput, 1);
	TC_ASSERT_EQ("lib_memoutstream", memoutstream.buffer[0], str[0]);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_memsistream_p
 * @brief            Initializes a stream for use with a fixed-size memory buffer
 * @scenario         Initializes a stream for use with a fixed-size memory buffer
 * @apicovered       lib_memsistream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_memsistream_p(void)
{
	const char *str = STREAM_TEST_CONTENTS;
	int getch;
	off_t offset;
	struct lib_memsistream_s memsistream;

	/* Check with 0 length */

	lib_memsistream((FAR struct lib_memsistream_s *)&memsistream, str, 0);
	TC_ASSERT_EQ("lib_memsistream", memsistream.buffer, (FAR char *)(str));

	getch = memsistream.public.get((FAR struct lib_sistream_s *)&memsistream.public);
	TC_ASSERT_EQ("memsistream_getc", memsistream.public.nget, 0);
	TC_ASSERT_EQ("memsistream_getc", getch, EOF);

	/* Check with valid length */

	lib_memsistream((FAR struct lib_memsistream_s *)&memsistream, str, STDIO_BUFLEN);
	TC_ASSERT_EQ("lib_memsistream", memsistream.buffer, (FAR char *)(str));

	getch = memsistream.public.get((FAR struct lib_sistream_s *)&memsistream.public);
	TC_ASSERT_EQ("memsistream_getc", memsistream.public.nget, 1);
	TC_ASSERT_EQ("meminstream_getc", getch, str[0]);

	/* Check seek operation */

	offset = memsistream.public.seek((FAR struct lib_sistream_s *)&memsistream.public, SEEK_OFFSET, SEEK_SET);
/* Seek from the start of the file */
	TC_ASSERT_EQ("memsistream_seek", memsistream.offset, SEEK_OFFSET);
	TC_ASSERT_EQ("memsistream_seek", offset, (off_t)SEEK_OFFSET);

	offset = memsistream.public.seek((FAR struct lib_sistream_s *)&memsistream.public, SEEK_OFFSET, SEEK_CUR);	/* Seek from the current file offset */
	TC_ASSERT_EQ("memsistream_seek", memsistream.offset, SEEK_OFFSET * 2);
	TC_ASSERT_EQ("memsistream_seek", offset, (off_t)(SEEK_OFFSET + SEEK_OFFSET));

	offset = memsistream.public.seek((FAR struct lib_sistream_s *)&memsistream.public, SEEK_OFFSET, SEEK_END);	/* Seek from the end of the file */
	TC_ASSERT_EQ("memsistream_seek", memsistream.offset, SEEK_OFFSET * 2);
	TC_ASSERT_EQ("memsistream_seek", offset, (off_t)ERROR);

	offset = memsistream.public.seek((FAR struct lib_sistream_s *)&memsistream.public, SEEK_OFFSET, SEEK_DEF);	/* Seek none */
	TC_ASSERT_EQ("memsistream_seek", memsistream.offset, SEEK_OFFSET * 2);
	TC_ASSERT_EQ("memsistream_seek", offset, (off_t)ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_memsostream_p
 * @brief            Initializes a stream for use with a fixed-size memory buffer
 * @scenario         Initializes a stream for use with a fixed-size memory buffer
 * @apicovered       lib_memsostream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_memsostream_p(void)
{
	char buffer[STDIO_BUFLEN];
	char *str = STREAM_TEST_CONTENTS;
	off_t offset;

	struct lib_memsostream_s memsostream;

	/* Check with valid length */

	lib_memsostream((FAR struct lib_memsostream_s *)&memsostream, buffer, STDIO_BUFLEN);
	TC_ASSERT_EQ("lib_memsostream", memsostream.buffer, (FAR char *)(buffer));

	memsostream.public.put((FAR struct lib_sostream_s *)&memsostream.public, str[0]);
	TC_ASSERT_EQ("memsostream_putc", memsostream.public.nput, 1);
	TC_ASSERT_EQ("lib_memsostream", memsostream.buffer[0], str[0]);

	/* Check seek operation */

	offset = memsostream.public.seek((FAR struct lib_sostream_s *)&memsostream.public, SEEK_OFFSET, SEEK_SET);	/* Seek from the start of the file */
	TC_ASSERT_EQ("memsostream_seek", memsostream.offset, SEEK_OFFSET);
	TC_ASSERT_EQ("memsostream_seek", offset, (off_t)SEEK_OFFSET);

	offset = memsostream.public.seek((FAR struct lib_sostream_s *)&memsostream.public, SEEK_OFFSET, SEEK_CUR);	/* Seek from the current file offset */
	TC_ASSERT_EQ("memsostream_seek", memsostream.offset, SEEK_OFFSET * 2);
	TC_ASSERT_EQ("memsostream_seek", offset, (off_t)(SEEK_OFFSET + SEEK_OFFSET));

	offset = memsostream.public.seek((FAR struct lib_sostream_s *)&memsostream.public, SEEK_OFFSET, SEEK_END);	/* Seek from the end of the file */
	TC_ASSERT_EQ("memsostream_seek", memsostream.offset, SEEK_OFFSET * 2);
	TC_ASSERT_EQ("memsostream_seek", offset, (off_t)ERROR);

	offset = memsostream.public.seek((FAR struct lib_sostream_s *)&memsostream.public, SEEK_OFFSET, SEEK_DEF);	/* Seek none */
	TC_ASSERT_EQ("memsostream_seek", memsostream.offset, SEEK_OFFSET * 2);
	TC_ASSERT_EQ("memsostream_seek", offset, (off_t)ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_nullinstream_p
 * @brief            Initializes a NULL stream. The initialized stream will return only EOF
 * @scenario         Initializes a NULL stream. The initialized stream will return only EOF
 * @apicovered       lib_nullinstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_nullinstream_p(void)
{
	struct lib_instream_s nullinstream;
	int getch;

	lib_nullinstream((FAR struct lib_instream_s *)&nullinstream);
	TC_ASSERT_EQ("lib_nullinstream", nullinstream.nget, 0);

	getch = nullinstream.get((FAR struct lib_instream_s *)&nullinstream);
	TC_ASSERT_EQ("lib_nullinstream", getch, EOF);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_nulloutstream_p
 * @brief            Initializes a NULL stream. The initialized stream will write all data to the bit-bucket
 * @scenario         Initializes a NULL stream. The initialized stream will write all data to the bit-bucket
 * @apicovered       lib_nulloutstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_nulloutstream_p(void)
{
	struct lib_outstream_s nulloutstream;

	lib_nulloutstream((FAR struct lib_outstream_s *)&nulloutstream);
	TC_ASSERT_EQ("lib_nulloutstream", nulloutstream.nput, 0);

	nulloutstream.put((FAR struct lib_outstream_s *)&nulloutstream, 1);
	TC_ASSERT_EQ("nulloutstream_putc", nulloutstream.nput, 1);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_rawinstream_p
 * @brief            Initializes a stream for use with a file descriptor
 * @scenario         Initializes a stream for use with a file descriptor
 * @apicovered       lib_rawinstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_rawinstream_p(void)
{
	int fd;
	int ret;
	int getch;
	char *str = STREAM_TEST_CONTENTS;

	struct lib_rawinstream_s rawinstream;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());
	ret = write(fd, str, strlen(str));
	close(fd);
	TC_ASSERT_EQ_CLEANUP("write", ret, strlen(str), vfs_unmount());
	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	/* Positive case, one character is read from the rawinstream*/
	lib_rawinstream((FAR struct lib_rawinstream_s *)&rawinstream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawinstream", rawinstream.fd, fd, close(fd); vfs_unmount());

	getch = rawinstream.public.get((FAR struct lib_instream_s *)&rawinstream.public);
	TC_ASSERT_EQ_CLEANUP("rawinstream_getc", rawinstream.public.nget, 1, close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("rawinstream_getc", getch, str[0], close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_rawinstream_invalid_permission_n
 * @brief            Initializes a stream for use with a file descriptor
 * @scenario         Initialize the stream with invalid permissions
 * @apicovered       lib_rawinstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_rawinstream_invalid_permission_n(void)
{
	int fd;
	int getch;

	struct lib_rawinstream_s rawinstream;

	/* Init */
	vfs_mount();

	/* Testcase */
	/* Negative case, invalid file permission: no character is read from the rawinstream */
	fd = open(VFS_FILE_PATH, O_WRONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	lib_rawinstream((FAR struct lib_rawinstream_s *)&rawinstream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawinstream", rawinstream.fd, fd, close(fd); vfs_unmount());

	getch = rawinstream.public.get((FAR struct lib_instream_s *)&rawinstream.public);
	TC_ASSERT_EQ_CLEANUP("rawinstream_getc", rawinstream.public.nget, 0, close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("rawinstream_getc", getch, EOF, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_rawoutstream_p
 * @brief            Initializes a stream for use with a file descriptor
 * @scenario         Initializes a stream for use with a file descriptor
 * @apicovered       lib_rawoutstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_rawoutstream_p(void)
{
	int fd;
	char buffer[STDIO_BUFLEN];
	char *str = STREAM_TEST_CONTENTS;
	int ret;

	struct lib_rawoutstream_s rawoutstream;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	/* Positive case, one character is put to the rawoutstream */
	lib_rawoutstream((FAR struct lib_rawoutstream_s *)&rawoutstream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawoutstream", rawoutstream.fd, fd, close(fd); vfs_unmount());

	rawoutstream.public.put((FAR struct lib_outstream_s *)&rawoutstream.public, str[0]);
	close(fd);
	TC_ASSERT_EQ("rawoutstream_putc", rawoutstream.public.nput, 1);

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	memset(buffer, 0, sizeof(buffer));
	ret = read(fd, buffer, sizeof(buffer));
	TC_ASSERT_GEQ_CLEANUP("read", ret, 0, close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("read", buffer[0], str[0], close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_rawoutstream_invalid_permission_n
 * @brief            Initializes a stream for use with a file descriptor
 * @scenario         Initialize the stream with invalid permissions
 * @apicovered       lib_rawoutstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_rawoutstream_invalid_permission_n(void)
{
	int fd;

	struct lib_rawoutstream_s rawoutstream;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	/* Negative case, invalid file permission: no character is written to the rawoutstream */
	lib_rawoutstream((FAR struct lib_rawoutstream_s *)&rawoutstream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawoutstream", rawoutstream.fd, fd, close(fd); vfs_unmount());

	rawoutstream.public.put((FAR struct lib_outstream_s *)&rawoutstream.public, 1);
	TC_ASSERT_EQ_CLEANUP("rawoutstream_putc", rawoutstream.public.nput, 0, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_rawsistream_p
 * @brief            Initializes a stream for use with a file descriptor
 * @scenario         Initializes a stream for use with a file descriptor
 * @apicovered       lib_rawsistream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_rawsistream_p(void)
{
	int fd;
	int ret;
	int getch;
	off_t offset;
	char *str = STREAM_TEST_CONTENTS;

	struct lib_rawsistream_s rawsistream;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	ret = write(fd, str, strlen(str));
	close(fd);
	TC_ASSERT_EQ_CLEANUP("write", ret, strlen(str), vfs_unmount());

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	/* Positive case, one character is read from the rawsistream */
	lib_rawsistream((FAR struct lib_rawsistream_s *)&rawsistream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawsistream", rawsistream.fd, fd, close(fd); vfs_unmount());

	getch = rawsistream.public.get((FAR struct lib_sistream_s *)&rawsistream.public);
	TC_ASSERT_EQ_CLEANUP("rawsistream_getc", rawsistream.public.nget, 1, close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("rawsistream_getc", getch, str[0], close(fd); vfs_unmount());

	/* Check seek operation */
	offset = rawsistream.public.seek((FAR struct lib_sistream_s *)&rawsistream.public, SEEK_OFFSET, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("rawsistream_seek", offset, (off_t)SEEK_OFFSET, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_rawsistream_invalid_permission_n
 * @brief            Initializes a stream for use with a file descriptor
 * @scenario         Initialize the stream with invalid permissions
 * @apicovered       lib_rawsistream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_rawsistream_invalid_permission_n(void)
{
	int fd;
	int getch;
	struct lib_rawsistream_s rawsistream;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_WRONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount());

	/* Testcase */
	/* Negative case, invalid file permission: no character is read from the rawsistream */
	lib_rawsistream((FAR struct lib_rawsistream_s *)&rawsistream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawsistream", rawsistream.fd, fd, close(fd); vfs_unmount());

	getch = rawsistream.public.get((FAR struct lib_sistream_s *)&rawsistream.public);
	TC_ASSERT_EQ_CLEANUP("rawsistream_getc", rawsistream.public.nget, 0, close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("rawsistream_getc", getch, EOF, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_rawsostream_p
 * @brief            Initializes a stream for use with a file descriptor
 * @scenario         Initializes a stream for use with a file descriptor
 * @apicovered       lib_rawsostream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_rawsostream_p(void)
{
	int fd;
	off_t offset;
	char buffer[STDIO_BUFLEN];
	char *str = STREAM_TEST_CONTENTS;
	int ret;

	struct lib_rawsostream_s rawsostream;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC);
	TC_ASSERT_GEQ("open", fd, 0);

	/* Testcase */
	/* Positive case, two characters are put to the rawsostream */
	lib_rawsostream((FAR struct lib_rawsostream_s *)&rawsostream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawsostream", rawsostream.fd, fd, close(fd); vfs_unmount());

	rawsostream.public.put((FAR struct lib_sostream_s *)&rawsostream.public, str[0]);
	TC_ASSERT_EQ_CLEANUP("rawsostream_putc", rawsostream.public.nput, 1, close(fd); vfs_unmount());
	rawsostream.public.put((FAR struct lib_sostream_s *)&rawsostream.public, str[1]);
	close(fd);
	TC_ASSERT_EQ_CLEANUP("rawsostream_putc", rawsostream.public.nput, 2, vfs_unmount());

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	memset(buffer, 0, sizeof(buffer));
	ret = read(fd, buffer, sizeof(buffer));
	TC_ASSERT_GEQ_CLEANUP("read", ret, 0, close(fd));
	TC_ASSERT_EQ_CLEANUP("read", buffer[0], str[0], close(fd); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("read", buffer[1], str[1], close(fd); vfs_unmount());

	/* Check seek operation */
	offset = rawsostream.public.seek((FAR struct lib_sostream_s *)&rawsostream.public, SEEK_OFFSET, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("rawsostream_seek", offset, (off_t)2, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_rawsostream_invalid_permission_n
 * @brief            Initializes a stream for use with a file descriptor
 * @scenario         Initialize the stream with invalid permissions
 * @apicovered       lib_rawsostream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_rawsostream_invalid_permission_n(void)
{
	int fd;
	char *str = STREAM_TEST_CONTENTS;
	struct lib_rawsostream_s rawsostream;

	/* Init */
	vfs_mount();
	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, vfs_unmount(););

	/* Testcase */
	/* Negative case, invalid file permission: no character is written to the rawsostream */
	lib_rawsostream((FAR struct lib_rawsostream_s *)&rawsostream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawsostream", rawsostream.fd, fd, close(fd); vfs_unmount());

	rawsostream.public.put((FAR struct lib_sostream_s *)&rawsostream.public, str[0]);
	TC_ASSERT_EQ_CLEANUP("rawsostream_putc", rawsostream.public.nput, 0, close(fd); vfs_unmount());

	/* Deinit */
	close(fd);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_sprintf_p
 * @brief            Composes a string with the same text that would be printed if format was used on printf
 *                   But instead of being printed the content is stored as a C string in the buffer pointed by str
 * @scenario         A terminating null character is automatically appended after the content.
 * @apicovered       sprintf
 * @precondition     The size of the buffer should be large enough to contain the entire resulting string
 * @postcondition    NA
 */
static void tc_libc_stdio_sprintf_p(void)
{
	char buf[STDIO_BUFLEN];
	char *str = VFS_TEST_CONTENTS_1;
	int ret;

	/* Testcase */
	ret = sprintf((FAR char *)&buf, "%s", str);
	TC_ASSERT_EQ("sprintf", ret, strnlen(str, VFS_CONTENTS_LEN));

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_stdinstream_p
 * @brief            Initializes a stream for use with a FILE instance
 * @scenario         Initializes a stream for use with a FILE instance
 * @apicovered       lib_stdinstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_stdinstream_p(void)
{
	FILE *stream;
	int getch;
	char *str = STREAM_TEST_CONTENTS;

	struct lib_stdinstream_s stdinstream;

	/* Init */
	vfs_mount();
	stream = fopen(VFS_FILE_PATH, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", stream, NULL, vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("fputs", fputs(str, stream), strlen(str), fclose(stream); vfs_unmount());
	fclose(stream);

	stream = fopen(VFS_FILE_PATH, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", stream, NULL, vfs_unmount());

	/* Testcase */
	/* Positive case, one character is read from the stdinstream */
	lib_stdinstream((FAR struct lib_stdinstream_s *)&stdinstream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdinstream", stdinstream.stream, stream, fclose(stream); vfs_unmount());

	getch = stdinstream.public.get((FAR struct lib_instream_s *)&stdinstream.public);
	TC_ASSERT_EQ_CLEANUP("stdinstream_getc", stdinstream.public.nget, 1, fclose(stream); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("stdinstream_getc", getch, str[0], fclose(stream); vfs_unmount());

	/* Deinit */
	fclose(stream);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_stdinstream_invalid_permission_n
 * @brief            Initializes a stream for use with a FILE instance
 * @scenario         Initialize the stream with invalid permissions
 * @apicovered       lib_stdinstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_stdinstream_invalid_permission_n(void)
{
	FILE *stream;
	int getch;

	struct lib_stdinstream_s stdinstream;

	/* Init */
	vfs_mount();
	stream = fopen(VFS_FILE_PATH, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", stream, NULL, vfs_unmount());
	fclose(stream);

	/* Testcase */
	/* Negative case, file opened with inappropriate permission */
	stream = fopen(VFS_FILE_PATH, "w+");
	lib_stdinstream((FAR struct lib_stdinstream_s *)&stdinstream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdinstream", stdinstream.stream, stream, fclose(stream); vfs_unmount());

	getch = stdinstream.public.get((FAR struct lib_instream_s *)&stdinstream.public);
	TC_ASSERT_EQ_CLEANUP("stdinstream_getc", stdinstream.public.nget, 0, fclose(stream); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("stdinstream_getc", getch, EOF, fclose(stream); vfs_unmount());

	/* Deinit */
	fclose(stream);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_stdoutstream_p
 * @brief            Initializes a stream for use with a FILE instance
 * @scenario         Initializes a stream for use with a FILE instance
 * @apicovered       lib_stdoutstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_stdoutstream_p(void)
{
	FILE *stream;
	char buffer[STDIO_BUFLEN];
	char *str = STREAM_TEST_CONTENTS;
	int ret;

	struct lib_stdoutstream_s stdoutstream;

	/* Init */
	vfs_mount();
	stream = fopen(VFS_FILE_PATH, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", stream, NULL, vfs_unmount());

	/* Testcase */
	/* Positive case, one character is put to the stdoutstream */
	lib_stdoutstream((FAR struct lib_stdoutstream_s *)&stdoutstream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdoutstream", stdoutstream.stream, stream, fclose(stream); vfs_unmount());

	stdoutstream.public.put((FAR struct lib_outstream_s *)&stdoutstream.public, str[0]);
	TC_ASSERT_EQ_CLEANUP("stdoutstream_putc", stdoutstream.public.nput, 1, fclose(stream); vfs_unmount());

	/* Check flush operation */
#if defined(CONFIG_STDIO_LINEBUFFER) && CONFIG_STDIO_BUFFER_SIZE > 0
	stdoutstream.public.flush((FAR struct lib_outstream_s *)&stdoutstream.public);
#endif
	fclose(stream);

	stream = fopen(VFS_FILE_PATH, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", stream, NULL, vfs_unmount());

	memset(buffer, 0, sizeof(buffer));
	ret = fread(buffer, 1, 1, stream);
	TC_ASSERT_EQ_CLEANUP("fread", ret, 1, fclose(stream); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("fread", buffer[0], str[0], fclose(stream); vfs_unmount());

	/* Deinit */
	fclose(stream);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_stdoutstream_invalid_permission_n
 * @brief            Initializes a stream for use with a FILE instance
 * @scenario         Initialize the stream with invalid permissions
 * @apicovered       lib_stdoutstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_stdoutstream_invalid_permission_n(void)
{
	FILE *stream;
	char *str = STREAM_TEST_CONTENTS;

	struct lib_stdoutstream_s stdoutstream;

	/* Init */
	vfs_mount();
	stream = fopen(VFS_FILE_PATH, "r");
	TC_ASSERT_NEQ_CLEANUP("fopen", stream, NULL, vfs_unmount());

	/* Testcase */
	/* Negative case, file opened with inappropriate permission */
	lib_stdoutstream((FAR struct lib_stdoutstream_s *)&stdoutstream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdoutstream", stdoutstream.stream, stream, fclose(stream); vfs_unmount());

	stdoutstream.public.put((FAR struct lib_outstream_s *)&stdoutstream.public, str[0]);
	TC_ASSERT_EQ_CLEANUP("stdoutstream_putc", stdoutstream.public.nput, 0, fclose(stream); vfs_unmount());

	/* Deinit */
	fclose(stream);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_stdsistream_p
 * @brief            Initializes a stream for use with a FILE instance
 * @scenario         Initializes a stream for use with a FILE instance
 * @apicovered       lib_stdsistream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_stdsistream_p(void)
{
	FILE *stream;
	int getch;
	off_t offset;
	char *str = STREAM_TEST_CONTENTS;

	struct lib_stdsistream_s stdsistream;

	/* Init */
	vfs_mount();
	stream = fopen(VFS_FILE_PATH, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", stream, NULL, vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("fputs", fputs(str, stream), strlen(str), fclose(stream); vfs_unmount());

	fclose(stream);

	stream = fopen(VFS_FILE_PATH, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", stream, NULL, vfs_unmount());

	/* Testcase */
	/* Positive case, one character is read from the stdsistream */
	lib_stdsistream((FAR struct lib_stdsistream_s *)&stdsistream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdsistream", stdsistream.stream, stream, fclose(stream); vfs_unmount());

	getch = stdsistream.public.get((FAR struct lib_sistream_s *)&stdsistream.public);
	TC_ASSERT_EQ_CLEANUP("stdsistream_getc", stdsistream.public.nget, 1, fclose(stream); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("stdsistream_getc", getch, str[0], fclose(stream); vfs_unmount());

	/* Check seek operation */
	offset = stdsistream.public.seek((FAR struct lib_sistream_s *)&stdsistream.public, SEEK_OFFSET, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("stdsistream_seek", offset, (off_t)OK, fclose(stream); vfs_unmount());

	/* Deinit */
	fclose(stream);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_stdsistream_invalid_permission_n
 * @brief            Initializes a stream for use with a FILE instance
 * @scenario         Initialize the stream with invalid permissions
 * @apicovered       lib_stdsistream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_stdsistream_invalid_permission_n(void)
{
	FILE *stream;
	int getch;

	struct lib_stdsistream_s stdsistream;

	/* Init */
	vfs_mount();
	stream = fopen(VFS_FILE_PATH, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", stream, NULL, vfs_unmount());
	fclose(stream);

	/* Testcase */
	/* Negative case, file opened with inappropriate permission */
	stream = fopen(VFS_FILE_PATH, "w+");
	lib_stdsistream((FAR struct lib_stdsistream_s *)&stdsistream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdsistream", stdsistream.stream, stream, fclose(stream); vfs_unmount());

	getch = stdsistream.public.get((FAR struct lib_sistream_s *)&stdsistream.public);
	TC_ASSERT_EQ_CLEANUP("stdsistream_getc", stdsistream.public.nget, 0, fclose(stream); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("stdsistream_getc", getch, EOF, fclose(stream); vfs_unmount());

	/* Deinit */
	fclose(stream);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_stdsostream_p
 * @brief            Initializes a stream for use with a FILE instance
 * @scenario         Initializes a stream for use with a FILE instance
 * @apicovered       lib_stdsostream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_stdsostream_p(void)
{
	FILE *stream;
	off_t offset;
	char buffer[STDIO_BUFLEN];
	char *str = STREAM_TEST_CONTENTS;
	int ret;

	struct lib_stdsostream_s stdsostream;

	/* Init */
	vfs_mount();
	stream = fopen(VFS_FILE_PATH, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", stream, NULL, vfs_unmount());

	/* Testcase */
	/* Positive case, one character is put to the stdsostream */
	lib_stdsostream((FAR struct lib_stdsostream_s *)&stdsostream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdsostream", stdsostream.stream, stream, fclose(stream); vfs_unmount());

	stdsostream.public.put((FAR struct lib_sostream_s *)&stdsostream.public, str[0]);
	TC_ASSERT_EQ_CLEANUP("stdsostream_putc", stdsostream.public.nput, 1, fclose(stream); vfs_unmount());

	/* Check seek operation */
	offset = stdsostream.public.seek(&stdsostream.public, SEEK_OFFSET, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("stdsostream_seek", offset, (off_t)OK, fclose(stream); vfs_unmount());

	/* Check flush operation */
#if defined(CONFIG_STDIO_LINEBUFFER) && CONFIG_STDIO_BUFFER_SIZE > 0
	stdsostream.public.flush((FAR struct lib_sostream_s *)&stdsostream.public);
#endif
	fclose(stream);

	stream = fopen(VFS_FILE_PATH, "r+");
	TC_ASSERT_NEQ_CLEANUP("fopen", stream, NULL, vfs_unmount());

	memset(buffer, 0, sizeof(buffer));
	ret = fread(buffer, 1, 1, stream);
	TC_ASSERT_EQ_CLEANUP("fread", ret, 1, fclose(stream); vfs_unmount());
	TC_ASSERT_EQ_CLEANUP("fread", buffer[0], str[0], fclose(stream); vfs_unmount());

	/* Deinit */
	fclose(stream);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_stdsostream_invalid_permission_n
 * @brief            Initializes a stream for use with a FILE instance
 * @scenario         Initialize the stream with invalid permissions
 * @apicovered       lib_stdsostream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_stdsostream_invalid_permission_n(void)
{
	FILE *stream;
	char *str = STREAM_TEST_CONTENTS;

	struct lib_stdsostream_s stdsostream;

	/* Init */
	vfs_mount();
	stream = fopen(VFS_FILE_PATH, "r");
	TC_ASSERT_NEQ_CLEANUP("fopen", stream, NULL, vfs_unmount());

	/* Testcase */
	/* Negative case, file opened with inappropriate permission */
	lib_stdsostream((FAR struct lib_stdsostream_s *)&stdsostream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdsostream", stdsostream.stream, stream, fclose(stream); vfs_unmount());

	stdsostream.public.put((FAR struct lib_sostream_s *)&stdsostream.public, str[0]);
	TC_ASSERT_EQ_CLEANUP("stdsostream_putc", stdsostream.public.nput, 0, fclose(stream); vfs_unmount());

	/* Deinit */
	fclose(stream);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_mktemp_p
 * @brief            The mktemp() function generates a unique temporary filename from template.
 * @scenario         The mktemp() function generates a unique temporary filename from template.
 * @apicovered       mktemp()
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_mktemp_p(void)
{
	char test_template[16] = "test1XXXXXX";
	char *ret = NULL;
	FILE *fp = NULL;

#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	int ret_check = 0;
	bool tmpfs_mount_exist = false;

	/* Init */
#if defined(CONFIG_FS_TMPFS)
	ret_check = mount(NULL, CONFIG_LIBC_TMPDIR, "tmpfs", 0, NULL);
#elif defined(CONFIG_FS_SMARTFS)
	ret_check = mount(MOUNT_DEV_DIR, CONFIG_LIBC_TMPDIR, "smartfs", 0, NULL);
#endif
	if (ret_check < 0) {
		TC_ASSERT_EQ("mount", errno, EEXIST);
		tmpfs_mount_exist = true;
	}
#endif

	/* Test cases with valid input */
	ret = mktemp(test_template);
	TC_ASSERT_NEQ_CLEANUP("mktemp", strncmp(ret, "", 1), 0, goto errout);
	printf("\nmktemp: %s\n", ret);
	fp = fopen(ret, "r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, goto errout);
	TC_ASSERT_EQ_CLEANUP("fopen", errno, ENOENT, goto errout);
	fclose(fp);

	strncpy(test_template, "test12XXXXXX", strlen("test12XXXXXX"));
	ret = mktemp(test_template);
	TC_ASSERT_NEQ("mktemp", strncmp(ret, "", 1), 0);
	printf("mktemp: %s\n", ret);
	fp = fopen(ret, "r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, goto errout);
	TC_ASSERT_EQ_CLEANUP("fopen", errno, ENOENT, goto errout);

	/* Deinit */
	fclose(fp);

	TC_SUCCESS_RESULT();

errout:
#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	fclose(fp);
	if (false == tmpfs_mount_exist) {
		umount(CONFIG_LIBC_TMPDIR);
	}
#endif
	return;
}

/**
 * @testcase         tc_libc_stdio_mktemp_invalid_path_n
 * @brief            The mktemp() function generates a unique temporary filename from template.
 * @scenario         Generates a unique temporary filename from invalid path
 * @apicovered       mktemp()
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_mktemp_invalid_path_n(void)
{
	char test_template[16] = "test1XXXXXX";
	char *ret = NULL;

#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	int ret_check = 0;
	bool tmpfs_mount_exist = false;

	/* Init */
#if defined(CONFIG_FS_TMPFS)
	ret_check = mount(NULL, CONFIG_LIBC_TMPDIR, "tmpfs", 0, NULL);
#elif defined(CONFIG_FS_SMARTFS)
	ret_check = mount(MOUNT_DEV_DIR, CONFIG_LIBC_TMPDIR, "smartfs", 0, NULL);
#endif
	if (ret_check < 0) {
		TC_ASSERT_EQ("mount", errno, EEXIST);
		tmpfs_mount_exist = true;
	}
#endif

	/* Test cases with invalid input */
	strncpy(test_template, "test2JJOKbF", strlen("test2JJOKbF"));
	ret = mktemp(test_template);
	TC_ASSERT_EQ("mktemp", strncmp(ret, "", 1), 0);
	TC_ASSERT_EQ("mktemp", errno, EINVAL);

	strncpy(test_template, "test3XXXXX", strlen("test3XXXXX"));
	ret = mktemp(test_template);
	TC_ASSERT_EQ("mktemp", strncmp(ret, "", 1), 0);
	TC_ASSERT_EQ("mktemp", errno, EINVAL);

	ret = mktemp(NULL);
	TC_ASSERT_EQ("mktemp", ret, NULL);
	TC_ASSERT_EQ("mktemp", errno, EINVAL);

	/* Deinit */
#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	if (false == tmpfs_mount_exist) {
		umount(CONFIG_LIBC_TMPDIR);
	}
#endif

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_mkstemp_p
 * @brief            The  mkstemp() function generates a unique temporary filename from template, creates and opens the file, and returns an open file descriptor for the file.
 * @scenario         The mkstemp() function replaces the contents of the string pointed to by path_template by a unique filename, and returns a file descriptor for the file open for reading and writing.
 * @apicovered       mkstemp()
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_mkstemp_p(void)
{
	char test_template[32] = "/tmp/test1XXXXXX";
	int ret = 0;

#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	int ret_check = 0;
	bool tmpfs_mount_exist = false;

	/* Init */
#if defined(CONFIG_FS_TMPFS)
	ret_check = mount(NULL, CONFIG_LIBC_TMPDIR, "tmpfs", 0, NULL);
#elif defined(CONFIG_FS_SMARTFS)
	ret_check = mount(MOUNT_DEV_DIR, CONFIG_LIBC_TMPDIR, "smartfs", 0, NULL);
#endif
	if (ret_check < 0) {
		TC_ASSERT_EQ("mount", errno, EEXIST);
		tmpfs_mount_exist = true;
	}
#endif

	/* Test cases with valid input */
	ret = mkstemp(test_template);
	TC_ASSERT_GEQ_CLEANUP("mkstemp", ret, 0, goto errout);
	printf("\nmkstemp: %s\n", test_template);
	close(ret);
	unlink(test_template);

	strncpy(test_template, "/tmp/test12XXXXXX", strlen("/tmp/test12XXXXXX"));
	ret = mkstemp(test_template);
	TC_ASSERT_GEQ_CLEANUP("mkstemp", ret, 0, goto errout);
	printf("mkstemp: %s\n", test_template);
	close(ret);
	unlink(test_template);

	strncpy(test_template, "/tmp/test2mkstemp", strlen("/tmp/test2mkstemp"));
	ret = mkstemp(test_template);
	TC_ASSERT_GEQ_CLEANUP("mkstemp", ret, 0, goto errout);
	printf("mkstemp: %s\n", test_template);
	close(ret);
	unlink(test_template);

	strncpy(test_template, "/tmp/test3XXXXX", strlen("/tmp/test3XXXXX"));
	ret = mkstemp(test_template);
	TC_ASSERT_GEQ_CLEANUP("mkstemp", ret, 0, goto errout);
	printf("mkstemp: %s\n", test_template);
	close(ret);
	unlink(test_template);

	strncpy(test_template, "/tmp/test4XXXXXXXX", strlen("/tmp/test4XXXXXXXX"));
	ret = mkstemp(test_template);
	TC_ASSERT_GEQ_CLEANUP("mkstemp", ret, 0, goto errout);
	printf("mkstemp: %s\n", test_template);
	close(ret);
	unlink(test_template);

	TC_SUCCESS_RESULT();

errout:
#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	if (false == tmpfs_mount_exist) {
		umount(CONFIG_LIBC_TMPDIR);
	}
#endif
	return;
}

/**
 * @testcase         tc_libc_stdio_mkstemp_invalid_path_n
 * @brief            The  mkstemp() function generates a unique temporary filename from template, creates and opens the file, and returns an open file descriptor for the file.
 * @scenario         Call mkstemp with an invalid path argument
 * @apicovered       mkstemp()
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_mkstemp_invalid_path_n(void)
{
	int ret = 0;

	/* Test cases with invalid input */
	ret = mkstemp(NULL);
	TC_ASSERT_EQ("mkstemp", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_tempnam_p
 * @brief            Returns a pointer to a string that is a valid filename
 * @scenario         The tempnam() function returns a pointer to a unique temporary filename, or NULL if a unique name cannot be generated.
 * @apicovered       tempnam(), mkstemp(), mktemp()
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_tempnam_p(void)
{
	const char filename[] = "tempnam_tc_test";
	char *ret = NULL;
	FILE *fp = NULL;

#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	int ret_check = 0;
	bool tmpfs_mount_exist = false;

	/* Init */
#if defined(CONFIG_FS_TMPFS)
	ret_check = mount(NULL, CONFIG_LIBC_TMPDIR, "tmpfs", 0, NULL);
#elif defined(CONFIG_FS_SMARTFS)
	ret_check = mount(MOUNT_DEV_DIR, CONFIG_LIBC_TMPDIR, "smartfs", 0, NULL);
#endif
	if (ret_check < 0) {
		TC_ASSERT_EQ("mount", errno, EEXIST);
		tmpfs_mount_exist = true;
	}
#endif

	/* Testcase */
	/* with valid prefix */
	ret = tempnam(CONFIG_LIBC_TMPDIR, filename);
	TC_ASSERT_NEQ_CLEANUP("tempnam", ret, NULL, goto errout);
	printf("\ntempnam: %s\n", ret);

	fp = fopen(ret, "r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, goto errout);
	TC_ASSERT_EQ_CLEANUP("fopen", errno, ENOENT, goto errout);

	TC_SUCCESS_RESULT();

	/* Deinit */
errout:
#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	if (false == tmpfs_mount_exist) {
		umount(CONFIG_LIBC_TMPDIR);
	}
#endif
	return;
}

/**
 * @testcase         tc_libc_stdio_tempnam_null_arg_n
 * @brief            Returns a pointer to a string that is a valid filename
 * @scenario         Call tempnam with null argument
 * @apicovered       tempnam(), mkstemp(), mktemp()
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_tempnam_null_arg_n(void)
{
	char *ret = NULL;
	FILE *fp = NULL;

#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	int ret_check = 0;
	bool tmpfs_mount_exist = false;

	/* Init */
#if defined(CONFIG_FS_TMPFS)
	ret_check = mount(NULL, CONFIG_LIBC_TMPDIR, "tmpfs", 0, NULL);
#elif defined(CONFIG_FS_SMARTFS)
	ret_check = mount(MOUNT_DEV_DIR, CONFIG_LIBC_TMPDIR, "smartfs", 0, NULL);
#endif
	if (ret_check < 0) {
		TC_ASSERT_EQ("mount", errno, EEXIST);
		tmpfs_mount_exist = true;
	}
#endif

	/* Testcase */
	/* with NULL prefix */
	ret = tempnam(NULL, NULL);
	TC_ASSERT_NEQ_CLEANUP("tempnam", ret, NULL, goto errout);
	printf("tempnam: %s\n", ret);

	fp = fopen(ret, "r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, goto errout);
	TC_ASSERT_EQ_CLEANUP("fopen", errno, ENOENT, goto errout);

	TC_SUCCESS_RESULT();

	/* Deinit */
errout:
#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	if (false == tmpfs_mount_exist) {
		umount(CONFIG_LIBC_TMPDIR);
	}
#endif
	return;
}

/**
 * @testcase         tc_libc_stdio_tmpnam_p
 * @brief            Returns a pointer to a string that is a valid filename
 * @scenario         The tmpnam() function returns a pointer to a unique temporary filename, or NULL if a unique name cannot be generated.
 * @apicovered       tmpnam(), mkstemp(), mktemp()
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_tmpnam_p(void)
{
	char filename[16] = { '\0' };
	char *ret = NULL;
	FILE *fp = NULL;

#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	int ret_check = 0;
	bool tmpfs_mount_exist = false;

	/* Init */
#if defined(CONFIG_FS_TMPFS)
	ret_check = mount(NULL, CONFIG_LIBC_TMPDIR, "tmpfs", 0, NULL);
#elif defined(CONFIG_FS_SMARTFS)
	ret_check = mount(MOUNT_DEV_DIR, CONFIG_LIBC_TMPDIR, "smartfs", 0, NULL);
#endif
	if (ret_check < 0) {
		TC_ASSERT_EQ("mount", errno, EEXIST);
		tmpfs_mount_exist = true;
	}
#endif

	/* Testcase */
	/* with valid string */
	ret = tmpnam(filename);
	TC_ASSERT_NEQ_CLEANUP("tmpnam", ret, NULL, goto errout);
	printf("tmpnam: %s\n", ret);

	fp = fopen(ret, "r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, goto errout);
	TC_ASSERT_EQ_CLEANUP("fopen", errno, ENOENT, goto errout);

	TC_SUCCESS_RESULT();

	/* Deinit */
errout:
#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	if (false == tmpfs_mount_exist) {
		umount(CONFIG_LIBC_TMPDIR);
	}
#endif
	return;
}

/**
 * @testcase         tc_libc_stdio_tmpnam_null_string_n
 * @brief            Returns a pointer to a string that is a valid filename
 * @scenario         Call tmpnam with null string
 * @apicovered       tmpnam(), mkstemp(), mktemp()
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_tmpnam_null_string_n(void)
{
	char *ret = NULL;
	FILE *fp = NULL;

#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	int ret_check = 0;
	bool tmpfs_mount_exist = false;

	/* Init */
#if defined(CONFIG_FS_TMPFS)
	ret_check = mount(NULL, CONFIG_LIBC_TMPDIR, "tmpfs", 0, NULL);
#elif defined(CONFIG_FS_SMARTFS)
	ret_check = mount(MOUNT_DEV_DIR, CONFIG_LIBC_TMPDIR, "smartfs", 0, NULL);
#endif
	if (ret_check < 0) {
		TC_ASSERT_EQ("mount", errno, EEXIST);
		tmpfs_mount_exist = true;
	}
#endif

	/* Testcase */
	/* with NULL string */
	ret = tmpnam(NULL);
	TC_ASSERT_NEQ_CLEANUP("tmpnam", ret, NULL, goto errout);
	printf("\ntmpnam: %s\n", ret);

	fp = fopen(ret, "r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, goto errout);
	TC_ASSERT_EQ_CLEANUP("fopen", errno, ENOENT, goto errout);

	TC_SUCCESS_RESULT();

	/* Deinit */
errout:
#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	if (false == tmpfs_mount_exist) {
		umount(CONFIG_LIBC_TMPDIR);
	}
#endif
	return;
}

/**
 * @testcase         tc_libc_stdio_zeroinstream_p
 * @brief            Initializes a NULL stream.  The initialized stream will return an infinitely long stream of zeroes.
 * @scenario         Initializes a NULL stream.  The initialized stream will return an infinitely long stream of zeroes.
 * @apicovered       lib_zeroinstream
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_zeroinstream_p(void)
{
	struct lib_instream_s zeroinstream;
	int getch;

	lib_zeroinstream((FAR struct lib_instream_s *)&zeroinstream);
	TC_ASSERT_EQ("lib_zeroinstream", zeroinstream.nget, 0);

	getch = zeroinstream.get((FAR struct lib_instream_s *)&zeroinstream);
	TC_ASSERT_EQ("zeroinstream_getc", zeroinstream.nget, 1);
	TC_ASSERT_EQ("zeroinstream_getc", getch, 0);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_mqueue_ops_p
 * @brief            mqueue creation
 * @scenario         opens the mqueue
 * @apicovered       mq_open and mq_unlink
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_mqueue_ops_p(void)
{
	mqd_t mqd_fd;
	int ret;
	struct mq_attr attr;

	/* Init */
	attr.mq_maxmsg  = 20;
	attr.mq_msgsize = 10;
	attr.mq_flags   = 0;

	/* Testcase */
	mqd_fd = mq_open("test_mqueue", O_CREAT, 0666, &attr);
	TC_ASSERT_NEQ("mq_open", mqd_fd, (mqd_t)ERROR);

	ret = mq_unlink("test_mqueue");
	TC_ASSERT_EQ("mq_unlink", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_fs_mqueue_ops_invalid_param_n
 * @brief            mqueue creation
 * @scenario         opens the mqueue in invalid param
 * @apicovered       mq_open and mq_unlink
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_mqueue_ops_invalid_param_n(void)
{
	mqd_t mqd_fd;

	struct mq_attr attr;

	/* Init */
	attr.mq_maxmsg  = 20;
	attr.mq_msgsize = 10;
	attr.mq_flags   = 0;

	/* Testcase */
	/*Invalid param*/
	mqd_fd = mq_open(NULL, O_WRONLY | O_CREAT, 0666, &attr);
	TC_ASSERT_EQ("mq_open", mqd_fd, (mqd_t)ERROR);

	mqd_fd = mq_open(MOUNT_DIR, O_RDONLY, 0666, &attr);
	TC_ASSERT_EQ("mq_open", mqd_fd, (mqd_t)ERROR);

	/*Opening invalid mqueue*/
	mqd_fd = mq_open("Test_mqueue", O_RDONLY, 0666, &attr);
	TC_ASSERT_EQ("mq_open", mqd_fd, (mqd_t)ERROR);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_ungetc_p
 * @brief            Input character into file stream
 * @scenario         Get character by fgets and then input again with ungetc. after that compare both of characters
 * @apicovered       fopen, fputc, fgetc, ungetc
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_ungetc_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ret;
	int ch1, ch2;

	/* Init */
	vfs_mount();
	fp = fopen(filename, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	ret = fputc(32, fp);
	fclose(fp);
	TC_ASSERT_NEQ_CLEANUP("fputc", ret, EOF, vfs_unmount());

	fp = fopen(filename, "r");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	ch1 = fgetc(fp);
	TC_ASSERT_NEQ_CLEANUP("fgetc", ch1, EOF, fclose(fp); vfs_unmount());

	/* Testcase */
	ret = ungetc(STDIO_BUFLEN, fp);
	TC_ASSERT_NEQ_CLEANUP("ungetc", ret, EOF, fclose(fp); vfs_unmount());

	ch2 = fgetc(fp);
	TC_ASSERT_NEQ_CLEANUP("fgetc", ch2, EOF, fclose(fp); vfs_unmount());
	TC_ASSERT_NEQ_CLEANUP("fgetc", ch1, ch2, fclose(fp); vfs_unmount());

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         tc_libc_stdio_ungetc_invalid_arg_n
 * @brief            Input character into file stream
 * @scenario         Call ungetc with an invalid argument
 * @apicovered       fopen, fputc, fgetc, ungetc
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_libc_stdio_ungetc_invalid_arg_n(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ret;
	int ch1;
#if CONFIG_NUNGET_CHARS > 0
	int num;
#endif

	/* Init */
	vfs_mount();
	fp = fopen(filename, "w+");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	ret = fputc(32, fp);
	fclose(fp);
	TC_ASSERT_NEQ("fputc", ret, EOF);

	fp = fopen(filename, "r");
	TC_ASSERT_NEQ_CLEANUP("fopen", fp, NULL, vfs_unmount());

	ch1 = fgetc(fp);
	TC_ASSERT_NEQ_CLEANUP("fgetc", ch1, EOF, fclose(fp));

	/* Testcase */
	/* Negative case with invalid argument, NULL stream. It will return EOF */
	ret = ungetc(STDIO_BUFLEN, NULL);
	TC_ASSERT_EQ_CLEANUP("ungetc", ret, EOF, fclose(fp));

	/* Negative case with invalid argument. It will return EOF */
#if CONFIG_NUNGET_CHARS > 0
	num = fp->fs_nungotten;
	fp->fs_nungotten = 4;
	ret = ungetc(STDIO_BUFLEN, fp);
	fp->fs_nungotten = num;
	TC_ASSERT_EQ_CLEANUP("ungetc", ret, EOF, fclose(fp));
#endif

	/* Deinit */
	fclose(fp);
	vfs_unmount();

	TC_SUCCESS_RESULT();
}

#if defined(CONFIG_BCH) && !defined(CONFIG_BUILD_PROTECTED)
/**
 * @testcase         tc_fs_driver_ramdisk_ops_p
 * @brief            creating an ramddisk device
 * @scenario         Creates an /dev/ram2 and peforms operations on /dev/ram2
 * @apicovered       rd_open, rd_close, ramdisk_register, rd_read, rd_write
 * @precondition     NA
 * @postcondition    NA
 */
static void tc_fs_driver_ramdisk_ops_p(void)
{
	uint8_t *buffer;
	int sectsize = 512;
	uint32_t nsectors = 1;
	int minor = 2;
	int ret;
	int fd;
	char buf[20];
	long size;

	/* Allocate the memory backing up the ramdisk */
	buffer = (uint8_t *)malloc(sectsize * nsectors);
	if (!buffer) {
		printf("out of memory \n");
		return;
	}

	ret = ramdisk_register(minor, buffer, nsectors, sectsize, RDFLAG_WRENABLED | RDFLAG_FUNLINK);
	TC_ASSERT_EQ_CLEANUP("ramdisk_register", ret, OK, free(buffer));

	fd = open(DEV_RAMDISK_PATH, O_RDWR);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, free(buffer));

	ret = ioctl(fd, BIOC_XIPBASE, (unsigned long)&size);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, free(buffer); close(fd));

	ret = read(fd, buf, sizeof(buf));
	TC_ASSERT_NEQ_CLEANUP("read", ret, ERROR, free(buffer); close(fd));

#ifdef CONFIG_FS_WRITABLE
	ret = write(fd, buf, sizeof(buf));
	TC_ASSERT_NEQ_CLEANUP("write", ret, ERROR, free(buffer); close(fd));
#endif

	ret = close(fd);
	TC_ASSERT_EQ_CLEANUP("close", ret, OK, free(buffer));

	ret = unlink(DEV_RAMDISK_PATH);
	TC_ASSERT_EQ_CLEANUP("unlink", ret, OK, free(buffer));

	TC_SUCCESS_RESULT();
}
#endif

#ifdef CONFIG_AUTOMOUNT_USERFS
char *get_fs_mount_devname(void)
{
	return devname;
}

void get_userfs_devname(void)
{
	int partno;
	int fd;
	fd = open(OS_API_TEST_DRVPATH, O_WRONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	partno = ioctl(fd, TESTIOC_GET_FS_PARTNO, 0);
	TC_ASSERT_GEQ_CLEANUP("ioctl", partno, 0, close(fd));

	snprintf(devname, FS_PATH_MAX, "/dev/smart0p%d", partno);
	TMP_MOUNT_DEV_DIR = get_fs_mount_devname();

	close(fd);
}
#endif

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tc_filesystem_main(int argc, char *argv[])
#endif
{
	if (testcase_state_handler(TC_START, "FileSystem TC") == ERROR) {
		return ERROR;
	}

#ifdef CONFIG_AUTOMOUNT_USERFS
	get_userfs_devname();
#endif

	tc_fs_vfs_umount_p();
	tc_fs_vfs_umount_noexist_path_n();
	tc_fs_vfs_mount_p();
	tc_fs_vfs_mount_exist_path_n();
	tc_fs_vfs_open_p();
	tc_fs_vfs_open_invalid_path_n();
	tc_fs_vfs_write_p();
	tc_fs_vfs_write_invalid_flags_n();
	tc_fs_vfs_write_invalid_fd_n();
	tc_fs_vfs_ftruncate_p();
	tc_fs_vfs_ftruncate_invalid_fd_n();
	tc_fs_vfs_ftruncate_invalid_length_n();
	tc_fs_vfs_ftruncate_invalid_flags_n();
	tc_fs_vfs_read_p();
	tc_fs_vfs_read_invalid_flags_n();
	tc_fs_vfs_read_invalid_fd_n();
	tc_fs_vfs_close_p();
	tc_fs_vfs_close_invalid_fd_n();
	tc_fs_vfs_dup_p();
	tc_fs_vfs_dup_invalid_fd_n();
	tc_fs_vfs_dup2_p();
	tc_fs_vfs_dup2_invalid_fd_n();
	tc_fs_vfs_fsync_p();
	tc_fs_vfs_fsync_invalid_flags_n();
	tc_fs_vfs_fsync_invalid_fd_n();
	tc_fs_vfs_lseek_p();
	tc_fs_vfs_lseek_invalid_fd_n();
	tc_fs_vfs_pwrite_p();
	tc_fs_vfs_pwrite_invalid_fd_n();
	tc_fs_vfs_pread_p();
	tc_fs_vfs_pread_invalid_fd_n();
	tc_fs_vfs_mkdir_p();
	tc_fs_vfs_mkdir_exist_path_n();
	tc_fs_vfs_mkdir_exceed_len_n();
	tc_fs_vfs_opendir_p();
	tc_fs_vfs_opendir_invalid_path_n();
	tc_fs_vfs_readdir_p();
	tc_fs_vfs_readdir_invalid_path_n();
	tc_fs_vfs_rewinddir_p();
	tc_fs_vfs_rewinddir_null_path_n();
	tc_fs_vfs_seekdir_p();
	tc_fs_vfs_seekdir_offset_n();
	tc_fs_vfs_seekdir_position_n();
	tc_fs_vfs_closedir_p();
	tc_fs_vfs_closedir_null_path_n();
	tc_fs_vfs_rmdir_p();
	tc_fs_vfs_rmdir_null_path_n();
	tc_fs_vfs_unlink_p();
	tc_fs_vfs_unlink_null_path_n();
	tc_fs_vfs_stat_p();
	tc_fs_vfs_stat_null_path_n();
	tc_fs_vfs_stat_invalid_path_n();
	tc_fs_vfs_stat_empty_string_n();
	tc_fs_vfs_statfs_p();
	tc_fs_vfs_statfs_null_path_n();
	tc_fs_vfs_fstat_p();
	tc_fs_vfs_fstat_invalid_fd_n();
	tc_fs_vfs_fstatfs_p();
	tc_fs_vfs_fstatfs_invalid_fd_n();
#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
	tc_fs_vfs_mkfifo_p();
	tc_fs_vfs_mkfifo_exist_path_n();
#endif
	tc_fs_vfs_sendfile_p();
	tc_fs_vfs_sendfile_invalid_fd_n();
	tc_fs_vfs_sendfile_offset_eof_n();
	tc_fs_vfs_fcntl_p();
	tc_fs_vfs_fcntl_invalid_fd_n();
	tc_fs_vfs_fdopen_p();
	tc_fs_vfs_fdopen_invalid_fp_n();
#ifndef CONFIG_DISABLE_POLL
	tc_fs_vfs_poll_p();
#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
	tc_fs_vfs_select_p();
#endif
#endif
	tc_fs_vfs_rename_p();
	tc_fs_vfs_rename_invalid_path_n();
	tc_fs_vfs_rename_exist_path_n();
	tc_fs_vfs_rename_null_path_n();
	tc_fs_vfs_rename_not_possible_n();
	tc_fs_vfs_ioctl_p();
	tc_fs_vfs_ioctl_invalid_fd_n();
	tc_fs_vfs_ioctl_invalid_cmd_n();
#ifdef CONFIG_TC_FS_PROCFS
	tc_fs_procfs_main();
#endif
#ifdef CONFIG_TC_FS_MOPS
	tc_fs_mops_main();
#endif
#if defined(CONFIG_MTD_CONFIG)
	tc_driver_mtd_config_ops();
#endif
	tc_libc_dirent_readdir_r_p();
	tc_libc_dirent_readdir_r_invalid_path_n();
	tc_libc_dirent_telldir_p();
	tc_libc_dirent_telldir_null_stream_n();;
	tc_libc_stdio_dprintf_p();
	tc_libc_stdio_dprintf_invalid_fd_n();
	tc_libc_stdio_fdopen_p();
	tc_libc_stdio_fdopen_invalid_fd_n();
	tc_libc_stdio_fopen_p();
	tc_libc_stdio_fopen_invalid_mode_n();
	tc_libc_stdio_fclose_p();
	tc_libc_stdio_fclose_invalid_fp_n();
	tc_libc_stdio_fputs_p();
	tc_libc_stdio_fputs_null_stream_n();
	tc_libc_stdio_fgets_p();
	tc_libc_stdio_fgets_buffer_size_n();
	tc_libc_stdio_fseek_p();
	tc_libc_stdio_fseek_null_stream_n();
	tc_libc_stdio_ftell_p();
	tc_libc_stdio_ftell_null_stream_n();
	tc_libc_stdio_feof_p();
	tc_libc_stdio_fprintf_p();
	tc_libc_stdio_fprintf_invalid_fp_n();
	tc_libc_stdio_fsetpos_p();
	tc_libc_stdio_fsetpos_null_position_n();
	tc_libc_stdio_fsetpos_invalid_fp_n();
	tc_libc_stdio_fgetpos_p();
	tc_libc_stdio_fgetpos_null_position_n();
	tc_libc_stdio_fgetpos_invalid_fp_n();
	tc_libc_stdio_fputc_p();
	tc_libc_stdio_fputc_invalid_fp_n();
	tc_libc_stdio_fgetc_p();
	tc_libc_stdio_fgetc_invalid_fp_n();
	tc_libc_stdio_fwrite_p();
	tc_libc_stdio_fwrite_invalid_fp_n();
	tc_libc_stdio_fread_p();
	tc_libc_stdio_fread_invalid_fp_n();
	tc_libc_stdio_freopen_p();
	tc_libc_stdio_freopen_invalid_mode_n();
	tc_libc_stdio_freopen_null_path_n();
	tc_libc_stdio_freopen_null_path_and_stream_n();
	tc_libc_stdio_ferror_p();
	tc_libc_stdio_clearerr_p();
#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
	tc_libc_stdio_gets_p();
	tc_libc_stdio_gets_null_buf_n();
	tc_libc_stdio_gets_s_p();
	tc_libc_stdio_gets_s_buflen_n();
#endif
	tc_libc_stdio_fileno_p();
	tc_libc_stdio_fileno_null_stream_n();
#ifdef CONFIG_STDIO_LINEBUFFER
	tc_libc_stdio_lib_snoflush_p();
#endif
	tc_libc_stdio_lib_sprintf_p();
	tc_libc_stdio_mktemp_p();
	tc_libc_stdio_mktemp_invalid_path_n();
	tc_libc_stdio_mkstemp_p();
	tc_libc_stdio_mkstemp_invalid_path_n();
	tc_libc_stdio_remove_p();
	tc_libc_stdio_remove_invalid_path_n();
#if CONFIG_STDIO_BUFFER_SIZE > 0
	tc_libc_stdio_setbuf_p();
	tc_libc_stdio_setvbuf_p();
#endif
	tc_fs_mqueue_ops_p();
	tc_fs_mqueue_ops_invalid_param_n();
#if defined(CONFIG_BCH) && !defined(CONFIG_BUILD_PROTECTED)
	tc_fs_driver_ramdisk_ops_p();
#endif
	tc_libc_stdio_meminstream_p();
	tc_libc_stdio_memoutstream_p();
	tc_libc_stdio_memsistream_p();
	tc_libc_stdio_memsostream_p();
	tc_libc_stdio_nullinstream_p();
	tc_libc_stdio_nulloutstream_p();
	tc_libc_stdio_rawinstream_p();
	tc_libc_stdio_rawinstream_invalid_permission_n();
	tc_libc_stdio_rawoutstream_p();
	tc_libc_stdio_rawoutstream_invalid_permission_n();
	tc_libc_stdio_rawsistream_p();
	tc_libc_stdio_rawsistream_invalid_permission_n();
	tc_libc_stdio_rawsostream_p();
	tc_libc_stdio_rawsostream_invalid_permission_n();
	tc_libc_stdio_sprintf_p();
	tc_libc_stdio_stdinstream_p();
	tc_libc_stdio_stdinstream_invalid_permission_n();
	tc_libc_stdio_stdoutstream_p();
	tc_libc_stdio_stdoutstream_invalid_permission_n();
	tc_libc_stdio_stdsistream_p();
	tc_libc_stdio_stdsistream_invalid_permission_n();
	tc_libc_stdio_stdsostream_p();
	tc_libc_stdio_stdsostream_invalid_permission_n();
	tc_libc_stdio_tempnam_p();
	tc_libc_stdio_tempnam_null_arg_n();
	tc_libc_stdio_tmpnam_p();
	tc_libc_stdio_tmpnam_null_string_n();
	tc_libc_stdio_ungetc_p();
	tc_libc_stdio_ungetc_invalid_arg_n();
	tc_libc_stdio_zeroinstream_p();
#if defined(CONFIG_TC_FS_PROCFS) && defined(CONFIG_FS_SMARTFS) && !defined(CONFIG_SMARTFS_MULTI_ROOT_DIRS) && !defined(CONFIG_BUILD_PROTECTED)
	tc_fs_smartfs_mksmartfs_p();
	tc_fs_smartfs_mksmartfs_invalid_path_n();
#endif
#ifdef CONFIG_ITC_FS
	itc_fs_main();
#endif

	(void)testcase_state_handler(TC_END, "FileSystem TC");

	return 0;
}
