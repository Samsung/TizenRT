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

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define STDIO_BUFLEN		64
#define VFS_CONTENTS_LEN	20
#define SEEK_DEF		3
#define SEEK_OFFSET		6

#define MOUNT_DIR CONFIG_MOUNT_POINT

#ifdef CONFIG_FS_SMARTFS
#if defined(CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS)
#define TMP_MOUNT_DEV_DIR CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME
#elif defined(CONFIG_ARTIK05X_AUTOMOUNT_USERFS)
#define TMP_MOUNT_DEV_DIR CONFIG_ARTIK05X_AUTOMOUNT_USERFS_DEVNAME
#elif defined(CONFIG_ESP32_AUTOMOUNT_USERFS_DEVNAME)
#define TMP_MOUNT_DEV_DIR CONFIG_ESP32_AUTOMOUNT_USERFS_DEVNAME
#elif defined(CONFIG_ARCH_BOARD_LM3S6965EK)
#define TMP_MOUNT_DEV_DIR "/dev/smart0p0"
#elif defined(CONFIG_IMXRT_AUTOMOUNT_USERFS)
#define TMP_MOUNT_DEV_DIR CONFIG_IMXRT_AUTOMOUNT_USERFS_DEVNAME
#else
#define TMP_MOUNT_DEV_DIR "/dev/smart1"
#endif

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

#define VFS_FOLDER_PATH MOUNT_DIR"folder"

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

#define MTD_CONFIG_PATH "/dev/config"
#define MTD_FTL_PATH "/dev/mtdblock1"
#define BUF_SIZE 4096

#define DEV_PATH "/dev"
#define DEV_INVALID_DIR "/dev/invalid"
#define DEV_EMPTY_FOLDER_PATH "/dev/folder"
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

#ifndef CONFIG_DISABLE_ENVIRON
extern int mount_show(void);
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
* @testcase         tc_fs_vfs_mount
* @brief            Mount file system
* @scenario         Mount initialized file system
* @apicovered       mount
* @precondition     File system should be initialized. For smartfs, smart_initialize & mksmartfs should be excuted.
* @postcondition    NA
*/
static void tc_fs_vfs_mount(void)
{
	int ret;

	ret = mount(MOUNT_DEV_DIR, CONFIG_MOUNT_POINT, TARGET_FS_NAME, 0, NULL);
	TC_ASSERT_EQ("mount", ret, OK);

	/*For each mountpt operation*/

#ifndef CONFIG_DISABLE_ENVIRON
	ret = mount_show();
	TC_ASSERT_EQ("mount_show", ret, OK);
#endif
	TC_SUCCESS_RESULT();
}
/**
* @testcase         tc_fs_vfs_umount
* @brief            Unmount file system
* @scenario         Unmount mounted file system
* @apicovered       umount
* @precondition     File system should be mounted.
* @postcondition    NA
*/
static void tc_fs_vfs_umount(void)
{
	int ret;
	ret = umount(CONFIG_MOUNT_POINT);
	TC_ASSERT_EQ("umount", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_open
* @brief            Open file to do file operation
* @scenario         Open specific file
* @apicovered       open
* @precondition     NA
* @postcondition    NA
*/
static void tc_fs_vfs_open(void)
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
* @testcase         tc_fs_vfs_fdopen
* @brief            Open file to do file operation using file descriptor
* @scenario         Open specific file
* @apicovered       fs_fdopen
* @precondition     NA
* @postcondition    NA
*/
static void tc_fs_vfs_fdopen(void)
{
	int fd;
	struct file_struct *fp ;
	fd = open(VFS_FILE_PATH, O_WROK | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);

	fp = fs_fdopen(fd, O_WROK, NULL);
	TC_ASSERT_NEQ_CLEANUP("fs_fdopen", fp, NULL, close(fd));

	fclose(fp);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_write
* @brief            Write data into specific file
* @scenario         Open file and then write data, if writing finished, close file
* @apicovered       open, write
* @precondition     NA
* @postcondition    NA
*/
static void tc_fs_vfs_write(void)
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
	ret = write(CONFIG_NFILE_DESCRIPTORS, buf, strlen(buf));
	TC_ASSERT_EQ("write", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_read
* @brief            Read data from specific file
* @scenario         Open file and then read data, if reading finished, close file
* @apicovered       open, read
* @precondition	    NA
* @postcondition    NA
*/
static void tc_fs_vfs_read(void)
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
* @testcase         tc_fs_vfs_close
* @brief            Close file
* @scenario         Open and close
* @apicovered       open, close
* @precondition     NA
* @postcondition    NA
*/
static void tc_fs_vfs_close(void)
{
	int fd, ret;
	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);
	ret = close(fd);
	TC_ASSERT_GEQ("close", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_dup
* @brief            Clone a file descriptor to an arbitray descriptor number
* @scenario         Open and write data. and then Clone descriptor to fd2, write data with fd2.
*                   After write, check it writes properly or not.
* @apicovered       open, write, dup, read
* @precondition	    NA
* @postcondition    NA
*/
static void tc_fs_vfs_dup(void)
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
* @testcase         tc_fs_vfs_dup2
* @brief            Clone a file descriptor  to a specific descriptor number
* @scenario         Open and write data. and then Clone fd1 to fd2, write data with fd1.
*                   After write, check it writes properly or not by read data with fd2.
* @apicovered       open, write, dup2, read
* @precondition	    NA
* @postcondition    NA
*/
static void tc_fs_vfs_dup2(void)
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
* @testcase         tc_fs_vfs_fsync
* @brief            Synchronize the file state on disk to match internal, in-memory state.
* @scenario         Open and write data. and then check fsync works properly.
* @apicovered       open, write, fsync, read
* @precondition     NA
* @postcondition    NA
*/
static void tc_fs_vfs_fsync(void)
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
* @testcase         tc_fs_vfs_lseek
* @brief            Move current file position to specific position
* @scenario         Open file and move position, and then read data to check lseek works properly or not.
* @apicovered       open, lseek, read
* @precondition     Data(VFS_TEST_CONTENTS_2) should be written in file(VFS_FILE_PATH)
* @postcondition    NA
*/
static void tc_fs_vfs_lseek(void)
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

	/* empty file seek*/
	fd = open(VFS_FILE1_PATH, O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = lseek(fd, 10, SEEK_SET);
	TC_ASSERT_NEQ_CLEANUP("lseek", ret, 10, close(fd));
	close(fd);
#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
	ret = mkfifo(FIFO_FILE_PATH, 0666);
	if (ret < 0) {
		TC_ASSERT_EQ("mkfifo", ret, -EEXIST);
	}
	fd = open(FIFO_FILE_PATH, O_WRONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = lseek(fd, 10, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("lseek", ret, 10, close(fd));

	ret = lseek(fd, -10, SEEK_SET);
	TC_ASSERT_NEQ_CLEANUP("lseek", ret, 10, close(fd));

	ret = lseek(fd, 10, SEEK_CUR);
	TC_ASSERT_GEQ_CLEANUP("lseek", ret, 0, close(fd));

	ret = lseek(fd, 10, SEEK_END);
	TC_ASSERT_NEQ_CLEANUP("lseek", ret, 10, close(fd));

	close(fd);
#endif

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_pwrite
* @brief            Write data at specific position of file
* @scenario         Open and write data at specific position. And then check file written on disk.
* @apicovered       open, pwrite, read
* @precondition     Data(VFS_TEST_CONTENTS_2) should be written in file(VFS_FILE_PATH)
* @postcondition    NA
*/
static void tc_fs_vfs_pwrite(void)
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
* @testcase         tc_fs_vfs_pread
* @brief            Read data at specific position of file
* @scenario         Open and read data from specific position.
* @apicovered       open, pread
* @precondition     tc_fs_vfs_pwrite should be passed
* @postcondition    NA
*/
static void tc_fs_vfs_pread(void)
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
* @testcase         tc_fs_vfs_mkdir
* @brief            Create folders
* @scenario         Create folder(VFS_FOLDER_PATH) and create 5 sub-folders
* @apicovered       mkdir
* @precondition     NA
* @postcondition    NA
*/
static void tc_fs_vfs_mkdir(void)
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
	/*creating an empty folder */
	ret = mkdir(DEV_EMPTY_FOLDER_PATH, 0777);
	TC_ASSERT_EQ("mkdir", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_opendir
* @brief            Open specific directory to use APIs defined in dirent.h
* @scenario         Open specific directory
* @apicovered       opendir
* @precondition     tc_fs_vfs_mkdir should be passed
* @postcondition    NA
*/
static void tc_fs_vfs_opendir(void)
{
	DIR *dir;

	dir = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ("opendir", dir, NULL);
	closedir(dir);
	/*Path doesnot exist */
	dir = opendir(INVALID_PATH);
	TC_ASSERT_EQ("opendir", dir, NULL);

	dir = opendir(DEV_PATH);
	TC_ASSERT_NEQ("opendir", dir, NULL);
	closedir(dir);
	/* Pseudo file system node covers error condition path is not a directory */
	dir = opendir(DEV_INVALID_DIR);
	TC_ASSERT_EQ("opendir", dir, NULL);

	/* Opening an empty folder */
	dir = opendir(DEV_EMPTY_FOLDER_PATH);
	TC_ASSERT_NEQ("opendir", dir, NULL);
	closedir(dir);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_readdir
* @brief            Read contents in specific directory sequentially
* @scenario         Read contents in specific folder(VFS_FOLDER_PATH)
* @apicovered       opendir, readdir, closedir
* @precondition     tc_fs_vfs_mkdir should be passed
* @postcondition    NA
*/
static void tc_fs_vfs_readdir(void)
{
	int ret;
	int count;
	DIR *dir;
	struct dirent *dirent;

	dir = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ("opendir", dir, NULL);

	count = 0;
	while (1) {
		dirent = readdir(dir);
		if (dirent == NULL) {
			break;
		}
		count++;
	}
	ret = closedir(dir);
	TC_ASSERT_EQ("closedir", ret, OK);
	TC_ASSERT_EQ("readdir", count, VFS_LOOP_COUNT);

	/*reading invalid directory */

	dir = opendir(VFS_INVALID_PATH);
	TC_ASSERT_EQ("opendir", dir, NULL);

	do {
		dirent = readdir(dir);
	} while (dirent != NULL);

	ret = closedir(dir);
	TC_ASSERT_NEQ("closedir", ret, OK);

	/*reading empty folder */

	dir = opendir(DEV_EMPTY_FOLDER_PATH);
	TC_ASSERT_NEQ("opendir", dir, NULL);

	do {
		dirent = readdir(dir);
	} while (dirent != NULL);

	ret = closedir(dir);
	TC_ASSERT_EQ("closedir", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_rewinddir
* @brief            Reset current position of directory
* @scenario         Read contents in specific folder(VFS_FOLDER_PATH), and it reachs end of contents, reset & read again
* @apicovered       opendir, rewinddir, closedir
* @precondition     tc_fs_vfs_mkdir should be passed
* @postcondition    NA
*/
static void tc_fs_vfs_rewinddir(void)
{
	int ret;
	int count;
	DIR *dir;
	struct dirent *dirent;

	dir = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ("opendir", dir, NULL);

	count = 0;
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
	ret = closedir(dir);
	TC_ASSERT_EQ("closedir", ret, OK);
	TC_ASSERT_EQ("rewinddir", count, VFS_LOOP_COUNT * 2);

	/*For Pseudo dir operations */
	dir = opendir(ROOT_PATH);
	TC_ASSERT_NEQ("opendir", dir, NULL);

	count = 0;
	while ((dirent = readdir(dir)) != NULL) {
		count++;
	}
	rewinddir(dir);

	while ((dirent = readdir(dir)) != NULL) {
		count--;
	}

	ret = closedir(dir);
	TC_ASSERT_EQ("closedir", ret, OK);
	TC_ASSERT_EQ("rewinddir", count, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_seekdir
* @brief            Move current position of directory to specific position
* @scenario         Change position of directory and read contents
* @apicovered       opendir, seekdir, readdir, closedir
* @precondition     tc_fs_vfs_mkdir should be passed
* @postcondition    NA
*/
static void tc_fs_vfs_seekdir(void)
{
	int ret;
	DIR *dir;
	struct dirent *dirent;
	off_t offset;
	char filename[1];

	dir = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ("opendir", dir, NULL);

	offset = SEEK_END;
	seekdir(dir, offset);
	TC_ASSERT_NEQ_CLEANUP("seekdir", dir, NULL, closedir(dir));
	dirent = readdir(dir);
	TC_ASSERT_NEQ_CLEANUP("readdir", dirent, NULL, closedir(dir));
	TC_ASSERT_EQ_CLEANUP("readdir", dirent->d_type, DTYPE_DIRECTORY, closedir(dir));

	itoa((int)offset, filename, 10);
	TC_ASSERT_EQ_CLEANUP("readdir", strncmp(dirent->d_name, filename, 1), 0, closedir(dir));

	ret = closedir(dir);
	TC_ASSERT_EQ("closedir", ret, OK);

	/* For Negative offset in seekmountdir operations */
	dir = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ("opendir", dir, NULL);

	offset = -2;
	seekdir(dir, offset);
	TC_ASSERT_NEQ_CLEANUP("seekdir", dir, NULL, closedir(dir));

	ret = closedir(dir);
	TC_ASSERT_EQ("closedir", ret, OK);
	/* for pseudo dir operations */

	dir = opendir(ROOT_PATH);
	TC_ASSERT_NEQ("opendir", dir, NULL);

	offset = SEEK_END;
	seekdir(dir, offset);
	TC_ASSERT_NEQ_CLEANUP("seekdir", dir, NULL, closedir(dir));
	dirent = readdir(dir);
	TC_ASSERT_NEQ_CLEANUP("readdir", dirent, NULL, closedir(dir));

	ret = closedir(dir);
	TC_ASSERT_EQ("closedir", ret, OK);

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
static void tc_fs_libc_dirent_readdir_r(void)
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
	TC_ASSERT_EQ("readdir_r", count, VFS_LOOP_COUNT);
	ret = closedir(dirp);
	TC_ASSERT_EQ("closedir", ret, OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         fs_libc_dirent_telldir
* @brief            Get position of current contents in specific directory
* @scenario         Get specific position by seekdir and check telldir returns position properly
* @apicovered       opendir, seekdir, telldir, closedir
* @precondition     tc_fs_vfs_mkdir should be passed
* @postcondition    NA
*/
static void tc_fs_libc_dirent_telldir(void)
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
* @testcase         tc_fs_vfs_closedir
* @brief            Close opened directory
* @scenario         Open and close directory
* @apicovered       opendir, closedir
* @precondition     NA
* @postcondition    NA
*/
static void tc_fs_vfs_closedir(void)
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
* @testcase         tc_fs_vfs_rmdir
* @brief            Remove each of directory
* @scenario         Remove all exist directory
* @apicovered       rmdir
* @precondition     tc_fs_vfs_mkdir should be passed
* @postcondition    NA
*/
static void tc_fs_vfs_rmdir(void)
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

	/*Removes the empty directory created*/
	ret = rmdir(DEV_EMPTY_FOLDER_PATH);
	TC_ASSERT_EQ("rmdir", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_unlink
* @brief            Unlink specific file
* @scenario         Unlink specific file(VFS_DUP_FILE_PATH)
* @apicovered       unlink
* @precondition     tc_fs_vfs_dup should be passed
* @postcondition    NA
*/
static void tc_fs_vfs_unlink(void)
{
	char *filename = VFS_DUP_FILE_PATH;
	int ret;

	ret = unlink(filename);
	TC_ASSERT_EQ("unlink", ret, OK);

	/* Nagative case with invalid argument, NULL pathname. It will return ERROR */
	ret = unlink(NULL);
	TC_ASSERT_EQ("unlink", ret, ERROR);
#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
	ret = unlink(FIFO_FILE_PATH);
	TC_ASSERT_EQ("unlink", ret, OK);
#endif
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_stat
* @brief            Get status of specific file
* @scenario         Get status of specific file(VFS_FILE_PATH) by stat
* @apicovered       stat
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void tc_fs_vfs_stat(void)
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

	/*Negative testcase path is empty string */
	ret = stat("", &st);
	TC_ASSERT_EQ("stat", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_statfs
* @brief            Get status of mounted file system
* @scenario         Get status of mounted file system by statfs and check type of file system
* @apicovered       statfs
* @precondition     File system should be mounted
* @postcondition    NA
*/
static void tc_fs_vfs_statfs(void)
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

	/*root pseudo file system */
	ret = statfs("/dev", &fs);
	TC_ASSERT_EQ("statfs", ret, OK);


	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_fstat
* @brief            Get status of specific file
* @scenario         Get status of specific file(VFS_FILE_PATH) by stat
* @apicovered       fstat
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void tc_fs_vfs_fstat(void)
{
	char *filename = VFS_FILE_PATH;
	struct stat st;
	int ret;
	int fd;
	fd = open(filename, O_RDWR);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = fstat(fd, &st);
	TC_ASSERT_EQ_CLEANUP("fstat", ret, OK, close(fd));

	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_fstatfs
* @brief            Get status of mounted file system
* @scenario         Get status of mounted file system by statfs and check type of file system
* @apicovered       fstatfs
* @precondition     File system should be mounted
* @postcondition    NA
*/
static void tc_fs_vfs_fstatfs(void)
{
	struct statfs fs;
	int ret;
	int fd;

	fd = open(VFS_FILE_PATH, 0666);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = fstatfs(fd, &fs);
	TC_ASSERT_EQ_CLEANUP("fstatfs", ret, OK, close(fd));
#ifdef CONFIG_FS_SMARTFS
	TC_ASSERT_EQ_CLEANUP("fstatfs", fs.f_type, SMARTFS_MAGIC, close(fd));
#endif
	close(fd);

#ifdef CONFIG_DEV_ZERO
	fd = open(DEV_ZERO_PATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd, 0);
	/*root pseudo file system */
	ret = fstatfs(fd, &fs);
	TC_ASSERT_EQ_CLEANUP("fstatfs", ret, OK, close(fd));
	close(fd);
#endif

	TC_SUCCESS_RESULT();
}

#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
/**
* @testcase         tc_fs_vfs_mkfifo
* @brief            Get data thorugh the pipe which create by mkfifo
* @scenario         Create fifo and check data between main task and its sub-thread
* @apicovered       mkfifo, open, pthread_create, write, read
* @precondition     CONFIG_PIPES should be enabled & CONFIG_DEV_PIPE_SIZE must greater than 11
* @postcondition    NA
*/
static void tc_fs_vfs_mkfifo(void)
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

	close(fd);

	ret = unlink(FIFO_FILE_PATH);
	TC_ASSERT_GEQ("unlink", ret, 0);

	TC_SUCCESS_RESULT();
	return;
errout:
	pthread_kill(tid, SIGUSR1);
	close(fd);
}
#endif

/**
* @testcase         tc_fs_vfs_sendfile
* @brief            Send file data to specific descriptor from another descriptor
* @scenario         Create new file and send data from exist file
* @apicovered       open, stat, sendfile
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void tc_fs_vfs_sendfile(void)
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
* @testcase         tc_fs_vfs_fcntl
* @brief            Access & control opened file with fcntl
* @scenario         Open file with specific flag and get access mode with fcntl
* @apicovered       open, fcntl
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void tc_fs_vfs_fcntl(void)
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
* @testcase         tc_fs_vfs_poll
* @brief            Polling for I/O
* @scenario         Check poll works properly or not
* @apicovered       poll
* @precondition     CONFIG_DISABLE_POLL should be disabled
* @postcondition    NA
*/
static void tc_fs_vfs_poll(void)
{
	struct pollfd pollfd;
	int ret;
	int fd;
	char *filename = VFS_FILE_PATH;

	fd = open(filename, O_RDWR);
	TC_ASSERT_GEQ("open", fd, 0);

	pollfd.fd = fd;
	pollfd.events = POLLIN | POLLOUT;

	/* Poll regular file, it will always return positive value */
	ret = poll(&pollfd, 1, -1);

	TC_ASSERT_GT_CLEANUP("poll", ret, 0, close(fd));
	TC_ASSERT_CLEANUP("poll", pollfd.revents & POLLIN, close(fd));
	TC_ASSERT_CLEANUP("poll", pollfd.revents & POLLOUT, close(fd));

	close(fd);
	TC_SUCCESS_RESULT();
}

#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
/**
* @testcase         tc_fs_vfs_select
* @brief            To monitor multiple I/O
* @scenario         Input text within 5 sec and check select works properly(check change of STDIN)
* @apicovered       select
* @precondition     CONFIG_DISABLE_POLL should be disabled
* @postcondition    NA
*/
static void tc_fs_vfs_select(void)
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
#endif

/**
* @testcase         tc_fs_vfs_rename
* @brief            Rename file to specific name
* @scenario         Rename exist file to specific name
* @apicovered       rename
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void tc_fs_vfs_rename(void)
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

	/*Condition where rename is not possible*/
	ret = rename(DEV_NULL_PATH, DEV_NEW_NULL_PATH);
	TC_ASSERT_NEQ("rename", ret, ERROR);

	ret = rename(DEV_NEW_NULL_PATH, DEV_NULL_PATH);
	TC_ASSERT_NEQ("rename", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_vfs_ioctl
* @brief            ioctl with opened file
* @scenario         Get #byte of data from /dev/console by ioctl
* @apicovered       open, ioctl
* @precondition     NA
* @postcondition    NA
*/
static void tc_fs_vfs_ioctl(void)
{
	int fd1;
	int fd2;
	int ret;
	long size;

	fd1 = open(DEV_CONSOLE_PATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd1, 0);
	ret = ioctl(fd1, FIONREAD, (unsigned long)&size);
	close(fd1);
	TC_ASSERT_EQ("ioctl", ret, OK);

	/*Negative case where invalid fd */
	ret = ioctl(INV_FD, FIONREAD, (unsigned long)&size);
	TC_ASSERT_EQ("ioctl", ret, ERROR);

	/*Negative cae where invalid cmd */
	fd2 = open(DEV_CONSOLE_PATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd2, 0);

	ret = ioctl(fd2, FIONREAD, (unsigned long)&size);
	close(fd2);
	TC_ASSERT_LEQ("ioctl", ret, 0);

	TC_SUCCESS_RESULT();
}
/**
* @testcase         tc_driver_mtd_config_ops
* @brief            mtd_config operations
* @scenario         Set and get config from /dev/config
* @apicovered       mtd_config ops
* @precondition     NA
* @postcondition    NA
*/
#if defined(CONFIG_MTD_CONFIG)
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
* @testcase         tc_driver_mtd_ftl_ops
* @brief            ftl block operations
* @scenario         opens the ftl device and perforsm operations
* @apicovered       ftl block operations
* @precondition     NA
* @postcondition    NA
*/
#if defined(CONFIG_MTD_FTL) && defined(CONFIG_BCH)
static void tc_driver_mtd_ftl_ops(void)
{
	int fd;
	int ret;
	char *buf;

	buf = (char *)malloc(BUF_SIZE);
	if (!buf) {
		printf("Memory not allocated \n");
		return;
	}

	fd = open(MTD_FTL_PATH, O_RDWR);
	TC_ASSERT_GEQ_CLEANUP("open", fd, 0, free(buf));

	ret = read(fd, buf, BUF_SIZE);
	TC_ASSERT_EQ_CLEANUP("read", ret, BUF_SIZE, goto cleanup);
#ifdef CONFIG_FS_WRITABLE
	ret = write(fd, buf, BUF_SIZE);
	TC_ASSERT_EQ_CLEANUP("write", ret, BUF_SIZE, goto cleanup);
#endif
	free(buf);
	buf = NULL;
	ret = close(fd);
	TC_ASSERT_EQ("close", ret, OK);
	TC_SUCCESS_RESULT();
	return;
cleanup:
	free(buf);
	close(fd);
}
#endif
/**
* @testcase         tc_libc_stdio_dprintf
* @brief            Exact analogs of fprintf and vfprintf, except that they output to a file descriptor fd instead of to a stdio stream.
* @scenario         Exact analogs of fprintf and vfprintf, except that they output to a file descriptor fd instead of to a stdio stream.
* @apicovered       dprintf
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_dprintf(void)
{
	char *filename = VFS_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_1;
	int fd;
	int ret;

	fd = open(filename, O_RDWR);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = dprintf(fd, "%s", str);
	close(fd);
	TC_ASSERT_EQ("dprintf", ret, strnlen(str, VFS_CONTENTS_LEN));

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_fdopen
* @brief            fdopen with available fd value
* @scenario         Open file with specific flags, and then fdopen with diffrent flag. Then check flag is changed properly
* @apicovered       open, fdopen
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void tc_libc_stdio_fdopen(void)
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
* @testcase         tc_libc_stdio_fopen
* @brief            Open file by fopen
* @scenario         Open file
* @apicovered       fopen
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_fopen(void)
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
* @testcase         tc_libc_stdio_fclose
* @brief            Close file by fopen
* @scenario         Open and Close file
* @apicovered       fopen, fclose
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_fclose(void)
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
* @testcase         tc_libc_stdio_fputs
* @brief            Write contents through the fputs
* @scenario         Write contents through the fputs, and check it works properly or not
* @apicovered       fopen, fputs
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void tc_libc_stdio_fputs(void)
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
* @testcase         tc_libc_stdio_fgets
* @brief            Read cntents through the fgets
* @scenario         Read contents through the fgets, and check it is same contents as written by tc_libc_stdio_fgets
* @apicovered       fopen, fgets
* @precondition     tc_libc_stdio_fputs  should be passed
* @postcondition    NA
*/
static void tc_libc_stdio_fgets(void)
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
* @testcase         tc_libc_stdio_fseek
* @brief            Move current file position to specific position
* @scenario         Open file and move position, and then read data to check lseek works properly or not.
* @apicovered       fopen, fseek, fgets
* @precondition     tc_libc_stdio_fputs  should be passed
* @postcondition    NA
*/
static void tc_libc_stdio_fseek(void)
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
* @testcase         tc_libc_stdio_ftell
* @brief            Get current file position
* @scenario         Open file and move position, and get current position by ftell
* @apicovered       fopen, fseek, ftell
* @precondition     tc_libc_stdio_fputs  should be passed
* @postcondition    NA
*/
static void tc_libc_stdio_ftell(void)
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
* @testcase         tc_libc_stdio_feof
* @brief            Check file pointer is positioned at the end of file
* @scenario         Make long size file and print contents until it reaches at the end of file
* @apicovered       fopen, feof
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_feof(void)
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
* @testcase         tc_libc_stdio_fprintf
* @brief            Write contents.
* @scenario         Write contents through the fprintf.
* @apicovered       fopen, fprintf
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_fprintf(void)
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
* @testcase         tc_libc_stdio_fsetpos
* @brief            Set current file pointer to specific position
* @scenario         Set new position of file pointer & check read contents to check it works properly
* @apicovered       fopen, fsetpos, fgetc
* @precondition     tc_libc_stdio_fprintf should be passed
* @postcondition    NA
*/
static void tc_libc_stdio_fsetpos(void)
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
* @testcase         tc_libc_stdio_fgetpos
* @brief            Get current file pointer
* @scenario         Set new position of file pointer & check current position by fgetpos
* @apicovered       fopen, fsetpos, fgetpos
* @precondition     tc_libc_stdio_fprintf should be passed
* @postcondition    NA
*/
static void tc_libc_stdio_fgetpos(void)
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
* @testcase         tc_libc_stdio_fputc
* @brief            Put character to file
* @scenario         Put character 'S' to file
* @apicovered       fopen, fputc
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_fputc(void)
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
* @testcase         tc_libc_stdio_fgetc
* @brief            get character from file
* @scenario         get character from file and check it is same as which it put in tc_libc_stdio_fputc
* @apicovered       fopen, fgetc
* @precondition     tc_libc_stdio_fputc should pass
* @postcondition    NA
*/
static void tc_libc_stdio_fgetc(void)
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
* @testcase         tc_libc_stdio_fwrite
* @brief            Write 1 line of contents
* @scenario         Write 3 lines of contents and check its return value
* @apicovered       fopen, fprintf
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_fwrite(void)
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
* @testcase         tc_libc_stdio_fread
* @brief            Read 1 line of contents
* @scenario         Read 3 lines of contents and check its same as contents which written in tc_libc_stdio_fwrite
* @apicovered       fopen, fread
* @precondition     tc_libc_stdio_fwrite should be passed
* @postcondition    NA
*/
static void tc_libc_stdio_fread(void)
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
* @testcase         tc_libc_stdio_freopen
* @brief            Open file by freopen
* @scenario         Open file
* @apicovered       freopen
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_freopen(void)
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
* @testcase         tc_libc_stdio_ferror
* @brief            Check error occured during operation
* @scenario         Write data to file which opened with read only flag to make an error forcely and check ferror works properly
* @apicovered       fopen, fputc, ferror
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void tc_libc_stdio_ferror(void)
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
* @testcase         tc_libc_stdio_clearerr
* @brief            Check error cleared by clearerr after error occured during operation
* @scenario         Write data to file which opened with read only flag to make an error forcely and
*                   check ferror works properly and then check error disappeared by clearerr
* @apicovered       fopen, fputc, ferror, clearerr
* @precondition     File VFS_FILE_PATH should be existed
* @postcondition    NA
*/
static void tc_libc_stdio_clearerr(void)
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

#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
/**
* @testcase         tc_libc_stdio_gets
* @brief            get string by user input
* @scenario         get string by user input and check it is NULL or not
* @apicovered       gets
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_gets(void)
{
	char input_str[64];

	printf("Enter text here : \n");
	fflush(stdout);
	TC_ASSERT_NEQ("gets", gets(input_str), NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_gets_s
* @brief            get string by user input
* @scenario         get string by user input and check it is NULL or not
* @apicovered       gets
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_gets_s(void)
{
	char input_str[64];

	printf("Enter text here : \n");
	fflush(stdout);
	TC_ASSERT_NEQ("gets_s", gets_s(input_str, sizeof(input_str)), NULL);
	TC_SUCCESS_RESULT();
}
#endif

/**
* @testcase         tc_libc_stdio_fileno
* @brief            Get fd value related to file stream
* @scenario         Open file with fopen and get fd value through the fileno
* @apicovered       fopen, fileno
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_fileno(void)
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

#ifdef CONFIG_STDIO_LINEBUFFER
/**
* @testcase         tc_libc_stdio_lib_snoflush
* @brief            provides a common, dummy flush method for seekable output streams that are not flushable.
* @scenario         Only used if CONFIG_STDIO_LINEBUFFER is selected.
* @apicovered       lib_snoflush
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_lib_snoflush(void)
{
	int ret;

	ret = lib_snoflush((FAR struct lib_sostream_s *)stdout);
	TC_ASSERT_EQ("lib_snoflush", ret, OK);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @testcase         tc_libc_stdio_lib_sprintf
* @brief            Composes a string with the same text that would be printed if format was used on printf
*                   But instead of being printed the content is stored as a C string in the buffer pointed by str
* @scenario         A terminating null character is automatically appended after the content.
* @apicovered       lib_sprintf
* @precondition     The size of the buffer should be large enough to contain the entire resulting string
* @postcondition    NA
*/
static void tc_libc_stdio_lib_sprintf(void)
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
* @testcase         tc_libc_stdio_remove
* @brief            Deletes the file whose name is specified in filename.
* @scenario         Open file
* @apicovered       remove
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_remove(void)
{
	char *filename = VFS_FILE_PATH;
	int ret;

	ret = remove(filename);
	TC_ASSERT_EQ("remove", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_setbuf
* @brief            Set stream buffer
* @scenario         Open file
* @apicovered       setbuf, setvbuf
* @precondition     NA
* @postcondition    NA
*/
#if CONFIG_STDIO_BUFFER_SIZE > 0
static void tc_libc_stdio_setbuf(void)
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
* @testcase         tc_libc_stdio_setvbuf
* @brief            Change stream buffering
* @scenario         Open file
* @apicovered       setbuf, setvbuf
* @precondition     NA
* @postcondition    NA
*/
#if CONFIG_STDIO_BUFFER_SIZE > 0
static void tc_libc_stdio_setvbuf(void)
{
	FILE *fp;
	char buffer[64];
	char *filename = VFS_FILE_PATH;
	int ret;

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
* @testcase         tc_libc_stdio_meminstream
* @brief            Initializes a stream for use with a fixed-size memory buffer
* @scenario         Initializes a stream for use with a fixed-size memory buffer
* @apicovered       lib_meminstream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_meminstream(void)
{
	const char *str = STREAM_TEST_CONTENTS;
	int getch;

	struct lib_meminstream_s meminstream;

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
* @testcase         tc_libc_stdio_memoutstream
* @brief            Initializes a stream for use with a fixed-size memory buffer
* @scenario         Initializes a stream for use with a fixed-size memory buffer
* @apicovered       lib_memoutstream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_memoutstream(void)
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
* @testcase         tc_libc_stdio_memsistream
* @brief            Initializes a stream for use with a fixed-size memory buffer
* @scenario         Initializes a stream for use with a fixed-size memory buffer
* @apicovered       lib_memsistream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_memsistream(void)
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

	offset = memsistream.public.seek((FAR struct lib_sistream_s *)&memsistream.public, SEEK_OFFSET, SEEK_SET);	/* Seek from the start of the file */
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
* @testcase         tc_libc_stdio_memsostream
* @brief            Initializes a stream for use with a fixed-size memory buffer
* @scenario         Initializes a stream for use with a fixed-size memory buffer
* @apicovered       lib_memsostream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_memsostream(void)
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
* @testcase         tc_libc_stdio_nullinstream
* @brief            Initializes a NULL stream. The initialized stream will return only EOF
* @scenario         Initializes a NULL stream. The initialized stream will return only EOF
* @apicovered       lib_nullinstream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_nullinstream(void)
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
* @testcase         tc_libc_stdio_nulloutstream
* @brief            Initializes a NULL stream. The initialized stream will write all data to the bit-bucket
* @scenario         Initializes a NULL stream. The initialized stream will write all data to the bit-bucket
* @apicovered       lib_nulloutstream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_nulloutstream(void)
{
	struct lib_outstream_s nulloutstream;

	lib_nulloutstream((FAR struct lib_outstream_s *)&nulloutstream);
	TC_ASSERT_EQ("lib_nulloutstream", nulloutstream.nput, 0);

	nulloutstream.put((FAR struct lib_outstream_s *)&nulloutstream, 1);
	TC_ASSERT_EQ("nulloutstream_putc", nulloutstream.nput, 1);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_rawinstream
* @brief            Initializes a stream for use with a file descriptor
* @scenario         Initializes a stream for use with a file descriptor
* @apicovered       lib_rawinstream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_rawinstream(void)
{
	int fd;
	int ret;
	int getch;
	char *str = STREAM_TEST_CONTENTS;

	struct lib_rawinstream_s rawinstream;

	/* Negative case, invalid file permission: no character is read from the rawinstream */

	fd = open(VFS_FILE_PATH, O_WRONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	lib_rawinstream((FAR struct lib_rawinstream_s *)&rawinstream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawinstream", rawinstream.fd, fd, close(fd));

	getch = rawinstream.public.get((FAR struct lib_instream_s *)&rawinstream.public);
	close(fd);
	TC_ASSERT_EQ("rawinstream_getc", rawinstream.public.nget, 0);
	TC_ASSERT_EQ("rawinstream_getc", getch, EOF);

	/* Positive case, one character is read from the rawinstream*/

	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = write(fd, str, strlen(str));
	close(fd);
	TC_ASSERT_EQ("write", ret, strlen(str));

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	lib_rawinstream((FAR struct lib_rawinstream_s *)&rawinstream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawinstream", rawinstream.fd, fd, close(fd));

	getch = rawinstream.public.get((FAR struct lib_instream_s *)&rawinstream.public);
	close(fd);
	TC_ASSERT_EQ("rawinstream_getc", rawinstream.public.nget, 1);
	TC_ASSERT_EQ("rawinstream_getc", getch, str[0]);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_rawoutstream
* @brief            Initializes a stream for use with a file descriptor
* @scenario         Initializes a stream for use with a file descriptor
* @apicovered       lib_rawoutstream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_rawoutstream(void)
{
	int fd;
	char buffer[STDIO_BUFLEN];
	char *str = STREAM_TEST_CONTENTS;
	int ret;

	struct lib_rawoutstream_s rawoutstream;

	/* Negative case, invalid file permission: no character is written to the rawoutstream */

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	lib_rawoutstream((FAR struct lib_rawoutstream_s *)&rawoutstream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawoutstream", rawoutstream.fd, fd, close(fd));

	rawoutstream.public.put((FAR struct lib_outstream_s *)&rawoutstream.public, 1);
	close(fd);
	TC_ASSERT_EQ("rawoutstream_putc", rawoutstream.public.nput, 0);

	/* Positive case, one character is put to the rawoutstream */

	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC);
	TC_ASSERT_GEQ("open", fd, 0);

	lib_rawoutstream((FAR struct lib_rawoutstream_s *)&rawoutstream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawoutstream", rawoutstream.fd, fd, close(fd));

	rawoutstream.public.put((FAR struct lib_outstream_s *)&rawoutstream.public, str[0]);
	close(fd);
	TC_ASSERT_EQ("rawoutstream_putc", rawoutstream.public.nput, 1);

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	memset(buffer, 0, sizeof(buffer));
	ret = read(fd, buffer, sizeof(buffer));
	close(fd);
	TC_ASSERT_GEQ("read", ret, 0);
	TC_ASSERT_EQ("read", buffer[0], str[0]);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_rawsistream
* @brief            Initializes a stream for use with a file descriptor
* @scenario         Initializes a stream for use with a file descriptor
* @apicovered       lib_rawsistream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_rawsistream(void)
{
	int fd;
	int ret;
	int getch;
	off_t offset;
	char *str = STREAM_TEST_CONTENTS;

	struct lib_rawsistream_s rawsistream;

	/* Negative case, invalid file permission: no character is read from the rawsistream */

	fd = open(VFS_FILE_PATH, O_WRONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	lib_rawsistream((FAR struct lib_rawsistream_s *)&rawsistream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawsistream", rawsistream.fd, fd, close(fd));

	getch = rawsistream.public.get((FAR struct lib_sistream_s *)&rawsistream.public);
	close(fd);
	TC_ASSERT_EQ("rawsistream_getc", rawsistream.public.nget, 0);
	TC_ASSERT_EQ("rawsistream_getc", getch, EOF);

	/* Positive case, one character is read from the rawsistream */

	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = write(fd, str, strlen(str));
	close(fd);
	TC_ASSERT_EQ("write", ret, strlen(str));

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	lib_rawsistream((FAR struct lib_rawsistream_s *)&rawsistream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawsistream", rawsistream.fd, fd, close(fd));

	getch = rawsistream.public.get((FAR struct lib_sistream_s *)&rawsistream.public);
	TC_ASSERT_EQ_CLEANUP("rawsistream_getc", rawsistream.public.nget, 1, close(fd));
	TC_ASSERT_EQ_CLEANUP("rawsistream_getc", getch, str[0], close(fd));

	/* Check seek operation */

	offset = rawsistream.public.seek((FAR struct lib_sistream_s *)&rawsistream.public, SEEK_OFFSET, SEEK_SET);
	close(fd);
	TC_ASSERT_EQ("rawsistream_seek", offset, (off_t)SEEK_OFFSET);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_rawsostream
* @brief            Initializes a stream for use with a file descriptor
* @scenario         Initializes a stream for use with a file descriptor
* @apicovered       lib_rawsostream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_rawsostream(void)
{
	int fd;
	off_t offset;
	char buffer[STDIO_BUFLEN];
	char *str = STREAM_TEST_CONTENTS;
	int ret;

	struct lib_rawsostream_s rawsostream;

	/* Negative case, invalid file permission: no character is written to the rawsostream */

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	lib_rawsostream((FAR struct lib_rawsostream_s *)&rawsostream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawsostream", rawsostream.fd, fd, close(fd));

	rawsostream.public.put((FAR struct lib_sostream_s *)&rawsostream.public, str[0]);
	close(fd);
	TC_ASSERT_EQ("rawsostream_putc", rawsostream.public.nput, 0);

	/* Positive case, two characters are put to the rawsostream */

	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC);
	TC_ASSERT_GEQ("open", fd, 0);

	lib_rawsostream((FAR struct lib_rawsostream_s *)&rawsostream, fd);
	TC_ASSERT_EQ_CLEANUP("lib_rawsostream", rawsostream.fd, fd, close(fd));

	rawsostream.public.put((FAR struct lib_sostream_s *)&rawsostream.public, str[0]);
	TC_ASSERT_EQ_CLEANUP("rawsostream_putc", rawsostream.public.nput, 1, close(fd));
	rawsostream.public.put((FAR struct lib_sostream_s *)&rawsostream.public, str[1]);
	close(fd);
	TC_ASSERT_EQ("rawsostream_putc", rawsostream.public.nput, 2);

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	memset(buffer, 0, sizeof(buffer));
	ret = read(fd, buffer, sizeof(buffer));
	TC_ASSERT_GEQ_CLEANUP("read", ret, 0, close(fd));
	TC_ASSERT_EQ_CLEANUP("read", buffer[0], str[0], close(fd));
	TC_ASSERT_EQ_CLEANUP("read", buffer[1], str[1], close(fd));

	/* Check seek operation */

	offset = rawsostream.public.seek((FAR struct lib_sostream_s *)&rawsostream.public, SEEK_OFFSET, SEEK_SET);
	close(fd);
	TC_ASSERT_EQ("rawsostream_seek", offset, (off_t)2);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_sprintf
* @brief            Composes a string with the same text that would be printed if format was used on printf
*                   But instead of being printed the content is stored as a C string in the buffer pointed by str
* @scenario         A terminating null character is automatically appended after the content.
* @apicovered       sprintf
* @precondition     The size of the buffer should be large enough to contain the entire resulting string
* @postcondition    NA
*/
static void tc_libc_stdio_sprintf(void)
{
	char buf[STDIO_BUFLEN];
	char *str = VFS_TEST_CONTENTS_1;
	int ret;

	ret = sprintf((FAR char *)&buf, "%s", str);
	TC_ASSERT_EQ("sprintf", ret, strnlen(str, VFS_CONTENTS_LEN));

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_stdinstream
* @brief            Initializes a stream for use with a FILE instance
* @scenario         Initializes a stream for use with a FILE instance
* @apicovered       lib_stdinstream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_stdinstream(void)
{
	FILE *stream;
	int getch;
	char *str = STREAM_TEST_CONTENTS;

	struct lib_stdinstream_s stdinstream;

	/* Negative case, file opened with inappropriate permission */

	stream = fopen(VFS_FILE_PATH, "w");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	lib_stdinstream((FAR struct lib_stdinstream_s *)&stdinstream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdinstream", stdinstream.stream, stream, fclose(stream));

	getch = stdinstream.public.get((FAR struct lib_instream_s *)&stdinstream.public);
	fclose(stream);
	TC_ASSERT_EQ("stdinstream_getc", stdinstream.public.nget, 0);
	TC_ASSERT_EQ("stdinstream_getc", getch, EOF);

	/* Positive case, one character is read from the stdinstream */

	stream = fopen(VFS_FILE_PATH, "w+");
	TC_ASSERT_NEQ("fopen", stream, NULL);
	TC_ASSERT_EQ_CLEANUP("fputs", fputs(str, stream), strlen(str), fclose(stream));
	fclose(stream);

	stream = fopen(VFS_FILE_PATH, "r+");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	lib_stdinstream((FAR struct lib_stdinstream_s *)&stdinstream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdinstream", stdinstream.stream, stream, fclose(stream));

	getch = stdinstream.public.get((FAR struct lib_instream_s *)&stdinstream.public);
	fclose(stream);
	TC_ASSERT_EQ("stdinstream_getc", stdinstream.public.nget, 1);
	TC_ASSERT_EQ("stdinstream_getc", getch, str[0]);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_stdoutstream
* @brief            Initializes a stream for use with a FILE instance
* @scenario         Initializes a stream for use with a FILE instance
* @apicovered       lib_stdoutstream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_stdoutstream(void)
{
	FILE *stream;
	char buffer[STDIO_BUFLEN];
	char *str = STREAM_TEST_CONTENTS;
	int ret;

	struct lib_stdoutstream_s stdoutstream;

	/* Negative case, file opened with inappropriate permission */

	stream = fopen(VFS_FILE_PATH, "r");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	lib_stdoutstream((FAR struct lib_stdoutstream_s *)&stdoutstream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdoutstream", stdoutstream.stream, stream, fclose(stream));

	stdoutstream.public.put((FAR struct lib_outstream_s *)&stdoutstream.public, str[0]);
	fclose(stream);
	TC_ASSERT_EQ("stdoutstream_putc", stdoutstream.public.nput, 0);

	/* Positive case, one character is put to the stdoutstream */

	stream = fopen(VFS_FILE_PATH, "w+");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	lib_stdoutstream((FAR struct lib_stdoutstream_s *)&stdoutstream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdoutstream", stdoutstream.stream, stream, fclose(stream));

	stdoutstream.public.put((FAR struct lib_outstream_s *)&stdoutstream.public, str[0]);
	TC_ASSERT_EQ_CLEANUP("stdoutstream_putc", stdoutstream.public.nput, 1, fclose(stream));

	/* Check flush operation */

#if defined(CONFIG_STDIO_LINEBUFFER) && CONFIG_STDIO_BUFFER_SIZE > 0
	stdoutstream.public.flush((FAR struct lib_outstream_s *)&stdoutstream.public);
#endif
	fclose(stream);

	stream = fopen(VFS_FILE_PATH, "r+");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	memset(buffer, 0, sizeof(buffer));
	ret = fread(buffer, 1, 1, stream);
	fclose(stream);
	TC_ASSERT_EQ("fread", ret, 1);
	TC_ASSERT_EQ("fread", buffer[0], str[0]);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_stdsistream
* @brief            Initializes a stream for use with a FILE instance
* @scenario         Initializes a stream for use with a FILE instance
* @apicovered       lib_stdsistream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_stdsistream(void)
{
	FILE *stream;
	int getch;
	off_t offset;
	char *str = STREAM_TEST_CONTENTS;

	struct lib_stdsistream_s stdsistream;

	/* Negative case, file opened with inappropriate permission */

	stream = fopen(VFS_FILE_PATH, "w");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	lib_stdsistream((FAR struct lib_stdsistream_s *)&stdsistream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdsistream", stdsistream.stream, stream, fclose(stream));

	getch = stdsistream.public.get((FAR struct lib_sistream_s *)&stdsistream.public);
	fclose(stream);
	TC_ASSERT_EQ("stdsistream_getc", stdsistream.public.nget, 0);
	TC_ASSERT_EQ("stdsistream_getc", getch, EOF);

	/* Positive case, one character is read from the stdsistream */

	stream = fopen(VFS_FILE_PATH, "w+");
	TC_ASSERT_NEQ("fopen", stream, NULL);
	TC_ASSERT_EQ_CLEANUP("fputs", fputs(str, stream), strlen(str), fclose(stream));

	fclose(stream);

	stream = fopen(VFS_FILE_PATH, "r+");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	lib_stdsistream((FAR struct lib_stdsistream_s *)&stdsistream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdsistream", stdsistream.stream, stream, fclose(stream));

	getch = stdsistream.public.get((FAR struct lib_sistream_s *)&stdsistream.public);
	TC_ASSERT_EQ_CLEANUP("stdsistream_getc", stdsistream.public.nget, 1, fclose(stream));
	TC_ASSERT_EQ_CLEANUP("stdsistream_getc", getch, str[0], fclose(stream));

	/* Check seek operation */

	offset = stdsistream.public.seek((FAR struct lib_sistream_s *)&stdsistream.public, SEEK_OFFSET, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("stdsistream_seek", offset, (off_t)OK, fclose(stream));

	fclose(stream);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_stdsostream
* @brief            Initializes a stream for use with a FILE instance
* @scenario         Initializes a stream for use with a FILE instance
* @apicovered       lib_stdsostream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_stdsostream(void)
{
	FILE *stream;
	off_t offset;
	char buffer[STDIO_BUFLEN];
	char *str = STREAM_TEST_CONTENTS;
	int ret;

	struct lib_stdsostream_s stdsostream;

	/* Negative case, file opened with inappropriate permission */

	stream = fopen(VFS_FILE_PATH, "r");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	lib_stdsostream((FAR struct lib_stdsostream_s *)&stdsostream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdsostream", stdsostream.stream, stream, fclose(stream));

	stdsostream.public.put((FAR struct lib_sostream_s *)&stdsostream.public, str[0]);
	fclose(stream);
	TC_ASSERT_EQ("stdsostream_putc", stdsostream.public.nput, 0);

	/* Positive case, one character is put to the stdsostream */

	stream = fopen(VFS_FILE_PATH, "w+");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	lib_stdsostream((FAR struct lib_stdsostream_s *)&stdsostream, stream);
	TC_ASSERT_EQ_CLEANUP("lib_stdsostream", stdsostream.stream, stream, fclose(stream));

	stdsostream.public.put((FAR struct lib_sostream_s *)&stdsostream.public, str[0]);
	TC_ASSERT_EQ_CLEANUP("stdsostream_putc", stdsostream.public.nput, 1, fclose(stream));

	/* Check seek operation */

	offset = stdsostream.public.seek(&stdsostream.public, SEEK_OFFSET, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("stdsostream_seek", offset, (off_t)OK, fclose(stream));

	/* Check flush operation */

#if defined(CONFIG_STDIO_LINEBUFFER) && CONFIG_STDIO_BUFFER_SIZE > 0
	stdsostream.public.flush((FAR struct lib_sostream_s *)&stdsostream.public);
#endif
	fclose(stream);

	stream = fopen(VFS_FILE_PATH, "r+");
	TC_ASSERT_NEQ("fopen", stream, NULL);

	memset(buffer, 0, sizeof(buffer));
	ret = fread(buffer, 1, 1, stream);
	fclose(stream);
	TC_ASSERT_EQ("fread", ret, 1);
	TC_ASSERT_EQ("fread", buffer[0], str[0]);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_stdio_mktemp
* @brief            The mktemp() function generates a unique temporary filename from template.
* @scenario         The mktemp() function generates a unique temporary filename from template.
* @apicovered       mktemp()
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_mktemp(void)
{
	char test_template[16] = "test1XXXXXX";
	char *ret = NULL;
	FILE *fp = NULL;

#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	int ret_check = 0;
	bool tmpfs_mount_exist = false;
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
	fclose(fp);

	/* Test cases with invalid input */

	strncpy(test_template, "test2", strlen("test2"));
	ret = mktemp(test_template);
	TC_ASSERT_EQ_CLEANUP("mktemp", strncmp(ret, "", 1), 0, goto errout);
	TC_ASSERT_EQ_CLEANUP("mktemp", errno, EINVAL, goto errout);

	strncpy(test_template, "test3XXXXX", strlen("test3XXXXX"));
	ret = mktemp(test_template);
	TC_ASSERT_EQ_CLEANUP("mktemp", strncmp(ret, "", 1), 0, goto errout);
	TC_ASSERT_EQ_CLEANUP("mktemp", errno, EINVAL, goto errout);

	ret = mktemp(NULL);
	TC_ASSERT_EQ_CLEANUP("mktemp", ret, NULL, goto errout);
	TC_ASSERT_EQ_CLEANUP("mktemp", errno, EINVAL, goto errout);

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
* @testcase         tc_libc_stdio_mkstemp
* @brief            The  mkstemp() function generates a unique temporary filename from template, creates and opens the file, and returns an open file descriptor for the file.
* @scenario         The mkstemp() function replaces the contents of the string pointed to by path_template by a unique filename, and returns a file descriptor for the file open for reading and writing.
* @apicovered       mkstemp()
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_mkstemp(void)
{
	char test_template[32] = "/tmp/test1XXXXXX";
	int ret = 0;

#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	int ret_check = 0;
	bool tmpfs_mount_exist = false;
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

	/* Test cases with invalid input */

	ret = mkstemp(NULL);
	TC_ASSERT_EQ_CLEANUP("mkstemp", ret, ERROR, goto errout);

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
* @testcase         tc_libc_stdio_tempnam
* @brief            Returns a pointer to a string that is a valid filename
* @scenario         The tempnam() function returns a pointer to a unique temporary filename, or NULL if a unique name cannot be generated.
* @apicovered       tempnam(), mkstemp(), mktemp()
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_tempnam(void)
{
	const char filename[] = "tempnam_tc_test";
	char *ret = NULL;
	FILE *fp = NULL;

#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	int ret_check = 0;
	bool tmpfs_mount_exist = false;
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
	/* TC 1 with valid prefix */

	ret = tempnam(CONFIG_LIBC_TMPDIR, filename);
	TC_ASSERT_NEQ_CLEANUP("tempnam", ret, NULL, goto errout);
	printf("\ntempnam: %s\n", ret);

	fp = fopen(ret, "r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, goto errout);
	TC_ASSERT_EQ_CLEANUP("fopen", errno, ENOENT, goto errout);

	/* TC 2 with NULL prefix */

	ret = tempnam(NULL, NULL);
	TC_ASSERT_NEQ_CLEANUP("tempnam", ret, NULL, goto errout);
	printf("tempnam: %s\n", ret);

	fp = fopen(ret, "r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, goto errout);
	TC_ASSERT_EQ_CLEANUP("fopen", errno, ENOENT, goto errout);

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
* @testcase         tc_libc_stdio_tmpnam
* @brief            Returns a pointer to a string that is a valid filename
* @scenario         The tmpnam() function returns a pointer to a unique temporary filename, or NULL if a unique name cannot be generated.
* @apicovered       tmpnam(), mkstemp(), mktemp()
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_tmpnam(void)
{
	char filename[16] = { '\0' };
	char *ret = NULL;
	FILE *fp = NULL;

#if defined(CONFIG_FS_TMPFS) || defined(CONFIG_FS_SMARTFS)
	int ret_check = 0;
	bool tmpfs_mount_exist = false;
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
	/* TC 1 with NULL string */

	ret = tmpnam(NULL);
	TC_ASSERT_NEQ_CLEANUP("tmpnam", ret, NULL, goto errout);
	printf("\ntmpnam: %s\n", ret);

	fp = fopen(ret, "r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, goto errout);
	TC_ASSERT_EQ_CLEANUP("fopen", errno, ENOENT, goto errout);

	/* TC 2 with valid string */

	ret = tmpnam(filename);
	TC_ASSERT_NEQ_CLEANUP("tmpnam", ret, NULL, goto errout);
	printf("tmpnam: %s\n", ret);

	fp = fopen(ret, "r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, goto errout);
	TC_ASSERT_EQ_CLEANUP("fopen", errno, ENOENT, goto errout);

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
* @testcase         tc_libc_stdio_zeroinstream
* @brief            Initializes a NULL stream.  The initialized stream will return an infinitely long stream of zeroes.
* @scenario         Initializes a NULL stream.  The initialized stream will return an infinitely long stream of zeroes.
* @apicovered       lib_zeroinstream
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_zeroinstream(void)
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
* @testcase         tc_fs_mqueue_ops
* @brief            mqueue creation
* @scenario         opens the mqueue
* @apicovered       mq_open and mq_unlink
* @precondition     NA
* @postcondition    NA
*/
static void tc_fs_mqueue_ops(void)
{
	mqd_t mqd_fd;
	int ret;
	struct mq_attr attr;
	attr.mq_maxmsg  = 20;
	attr.mq_msgsize = 10;
	attr.mq_flags   = 0;

	/*Invalid param*/
	mqd_fd = mq_open(NULL, O_WRONLY | O_CREAT, 0666, &attr);
	TC_ASSERT_EQ("mq_open", mqd_fd, (mqd_t)ERROR);

	mqd_fd = mq_open(MOUNT_DIR, O_RDONLY, 0666, &attr);
	TC_ASSERT_EQ("mq_open", mqd_fd, (mqd_t)ERROR);

	mqd_fd = mq_open("test_mqueue", O_CREAT, 0666, &attr);
	TC_ASSERT_NEQ("mq_open", mqd_fd, (mqd_t)ERROR);

	/*Opening invalid mqueue*/
	mqd_fd = mq_open("Test_mqueue", O_RDONLY, 0666, &attr);
	TC_ASSERT_EQ("mq_open", mqd_fd, (mqd_t)ERROR);

	ret = mq_unlink("Test_mqueue");
	TC_ASSERT_EQ("mq_unlink", mqd_fd, (mqd_t)ERROR);

	ret = mq_unlink("test_mqueue");
	TC_ASSERT_EQ("mq_unlink", ret, OK);

	TC_SUCCESS_RESULT();
}
/**
* @testcase         tc_libc_stdio_ungetc
* @brief            Input character into file stream
* @scenario         Get character by fgets and then input again with ungetc. after that compare both of characters
* @apicovered       fopen, fputc, fgetc, ungetc
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_stdio_ungetc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ret;
	int ch1, ch2;
#if CONFIG_NUNGET_CHARS > 0
	int num;
#endif

	fp = fopen(filename, "w+");
	TC_ASSERT_NEQ("fopen", fp, NULL);

	ret = fputc(32, fp);
	fclose(fp);
	TC_ASSERT_NEQ("fputc", ret, EOF);

	fp = fopen(filename, "r");
	TC_ASSERT_NEQ("fopen", fp, NULL);

	ch1 = fgetc(fp);
	TC_ASSERT_NEQ_CLEANUP("fgetc", ch1, EOF, fclose(fp));

	/* Negative case with invalid argument, NULL stream. It will return EOF */

	ret = ungetc(STDIO_BUFLEN, NULL);
	TC_ASSERT_EQ_CLEANUP("ungetc", ret, EOF, fclose(fp));

	ret = ungetc(STDIO_BUFLEN, fp);
	TC_ASSERT_NEQ_CLEANUP("ungetc", ret, EOF, fclose(fp));

	/* Negative case with invalid argument. It will return EOF */

#if CONFIG_NUNGET_CHARS > 0
	num = fp->fs_nungotten;
	fp->fs_nungotten = 4;
	ret = ungetc(STDIO_BUFLEN, fp);
	fp->fs_nungotten = num;
	TC_ASSERT_EQ_CLEANUP("ungetc", ret, EOF, fclose(fp));
#endif
	ch2 = fgetc(fp);
	fclose(fp);
	TC_ASSERT_NEQ("fgetc", ch2, EOF);

	TC_ASSERT_NEQ("fgetc", ch1, ch2);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_fs_driver_ramdisk_ops
* @brief            creating an ramddisk device
* @scenario         Creates an /dev/ram2 and peforms operations on /dev/ram2
* @apicovered       rd_open, rd_close, ramdisk_register, rd_read, rd_write
* @precondition     NA
* @postcondition    NA
*/
#ifdef CONFIG_BCH
static void tc_fs_driver_ramdisk_ops(void)
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
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tc_filesystem_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "FileSystem TC") == ERROR) {
		return ERROR;
	}

	tc_fs_vfs_umount();
	tc_fs_vfs_mount();
	tc_fs_vfs_open();
	tc_fs_vfs_write();
	tc_fs_vfs_read();
	tc_fs_vfs_close();
	tc_fs_vfs_dup();
	tc_fs_vfs_dup2();
	tc_fs_vfs_fsync();
	tc_fs_vfs_lseek();
	tc_fs_vfs_pwrite();
	tc_fs_vfs_pread();
	tc_fs_vfs_mkdir();
	tc_fs_vfs_opendir();
	tc_fs_vfs_readdir();
	tc_fs_vfs_rewinddir();
	tc_fs_vfs_seekdir();
	tc_fs_vfs_closedir();
	tc_fs_libc_dirent_readdir_r();
	tc_fs_libc_dirent_telldir();
	tc_fs_vfs_rmdir();
	tc_fs_vfs_unlink();
	tc_fs_vfs_stat();
	tc_fs_vfs_statfs();
	tc_fs_vfs_fstat();
	tc_fs_vfs_fstatfs();
#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 11)
	tc_fs_vfs_mkfifo();
#endif
	tc_fs_vfs_sendfile();
	tc_fs_vfs_fcntl();
	tc_fs_vfs_fdopen();
#ifndef CONFIG_DISABLE_POLL
	tc_fs_vfs_poll();
#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
	tc_fs_vfs_select();
#endif
#endif

	tc_fs_vfs_rename();
	tc_fs_vfs_ioctl();
#ifdef CONFIG_TC_FS_PROCFS
	tc_fs_procfs_main();
#endif
#ifdef CONFIG_TC_FS_MOPS
	tc_fs_mops_main();
#endif
#if defined(CONFIG_MTD_CONFIG)
	tc_driver_mtd_config_ops();
#endif

#if defined(CONFIG_MTD_FTL) && defined(CONFIG_BCH)
	tc_driver_mtd_ftl_ops();
#endif

	tc_libc_stdio_dprintf();
	tc_libc_stdio_fdopen();
	tc_libc_stdio_fopen();
	tc_libc_stdio_fclose();
	tc_libc_stdio_fputs();
	tc_libc_stdio_fgets();
	tc_libc_stdio_fseek();
	tc_libc_stdio_ftell();
	tc_libc_stdio_feof();
	tc_libc_stdio_fprintf();
	tc_libc_stdio_fsetpos();
	tc_libc_stdio_fgetpos();
	tc_libc_stdio_fputc();
	tc_libc_stdio_fgetc();
	tc_libc_stdio_fwrite();
	tc_libc_stdio_fread();
	tc_libc_stdio_freopen();
	tc_libc_stdio_ferror();
	tc_libc_stdio_clearerr();
#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
	tc_libc_stdio_gets();
	tc_libc_stdio_gets_s();
#endif
	tc_libc_stdio_fileno();
#ifdef CONFIG_STDIO_LINEBUFFER
	tc_libc_stdio_lib_snoflush();
#endif
	tc_libc_stdio_lib_sprintf();
	tc_libc_stdio_mktemp();
	tc_libc_stdio_mkstemp();
	tc_libc_stdio_remove();
#if CONFIG_STDIO_BUFFER_SIZE > 0
	tc_libc_stdio_setbuf();
	tc_libc_stdio_setvbuf();
#endif
	tc_fs_mqueue_ops();
#ifdef CONFIG_BCH
	tc_fs_driver_ramdisk_ops();
#endif
	tc_libc_stdio_meminstream();
	tc_libc_stdio_memoutstream();
	tc_libc_stdio_memsistream();
	tc_libc_stdio_memsostream();
	tc_libc_stdio_nullinstream();
	tc_libc_stdio_nulloutstream();
	tc_libc_stdio_rawinstream();
	tc_libc_stdio_rawoutstream();
	tc_libc_stdio_rawsistream();
	tc_libc_stdio_rawsostream();
	tc_libc_stdio_sprintf();
	tc_libc_stdio_stdinstream();
	tc_libc_stdio_stdoutstream();
	tc_libc_stdio_stdsistream();
	tc_libc_stdio_stdsostream();
	tc_libc_stdio_tempnam();
	tc_libc_stdio_tmpnam();
	tc_libc_stdio_ungetc();
	tc_libc_stdio_zeroinstream();
#ifdef CONFIG_ITC_FS
	itc_fs_main();
#endif
#if defined(CONFIG_TC_FS_PROCFS) && !defined(CONFIG_SMARTFS_MULTI_ROOT_DIRS)
	tc_fs_smartfs_mksmartfs();
#endif
	(void)tc_handler(TC_END, "FileSystem TC");

	return 0;
}
