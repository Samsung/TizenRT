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

/// @file itc_fs.c
/// @brief Scenarios for Filesystem

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include "tc_common.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

#ifdef CONFIG_FS_SMARTFS
#if defined(CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS)
#define TMP_MOUNT_DEV_DIR CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME
#elif defined(CONFIG_ARTIK05X_AUTOMOUNT_USERFS)
#define TMP_MOUNT_DEV_DIR CONFIG_ARTIK05X_AUTOMOUNT_USERFS_DEVNAME
#elif defined(CONFIG_ARCH_BOARD_LM3S6965EK)
#define TMP_MOUNT_DEV_DIR "/dev/smart0p0"
#else
#define TMP_MOUNT_DEV_DIR "/dev/smart1"
#endif

#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
#define MOUNT_DEV_DIR TMP_MOUNT_DEV_DIR"d1"
#else
#define MOUNT_DEV_DIR TMP_MOUNT_DEV_DIR
#endif

#define FS_TYPE "smartfs"
#else
#define MOUNT_DEV_DIR "/dev/fs1"
#define FS_TYPE "unknwon"
#endif

#define BUFFER_LEN 30
#define MOUNT_DIR CONFIG_MOUNT_POINT
#define VFS_FILE_PATH MOUNT_DIR"vfs"
#define VFS_FOLDER_PATH MOUNT_DIR"folder"
#define VFS_LOOP_COUNT 5
#define VFS_TEST_CONTENTS_1 "THIS IS VFS TEST 1"
#define VFS_TEST_CONTENTS_2 "THIS IS VFS TEST 2"

static int g_flag;
/**
* @fn               add_dirs
* @description      add directory VFS_FOLDER_PATH, and count of inner directories
* @return           void
*/
static void add_dirs(int count)
{
	char folder_name[BUFFER_LEN];
	int len;
	int i;

	if (mkdir(VFS_FOLDER_PATH, 0777) != OK) {
		g_flag = ERROR;
	}

	strcpy(folder_name, VFS_FOLDER_PATH);
	strcat(folder_name, "/");
	len = strlen(folder_name);

	for (i = 0; i < count; i++) {
		itoa(i, folder_name + len, 10);
		if (mkdir(folder_name, 0777) != OK) {
			g_flag = ERROR;
		}
	}
}

/**
* @fn               remove_dirs
* @description      remove directory VFS_FOLDER_PATH, and count of inner directories
* @return           void
*/
static void remove_dirs(int count)
{
	char folder_name[BUFFER_LEN];
	int len;
	int i;

	strcpy(folder_name, VFS_FOLDER_PATH);
	strcat(folder_name, "/");
	len = strlen(folder_name);

	for (i = 0; i < count; i++) {
		itoa(i, folder_name + len, 10);
		if (rmdir(folder_name) != OK) {
			g_flag = ERROR;
		}
	}

	if (rmdir(VFS_FOLDER_PATH) != OK) {
		g_flag = ERROR;
	}
}

/**
* @testcase         itc_fs_vfs_umount_n_twice
* @brief            Unmount file system
* @scenario         Unmount file system twice.
* @apicovered       umount
* @precondition     File system should be mounted.
* @postcondition    NA
*/
static void itc_fs_vfs_umount_n_twice(void)
{
	TC_ASSERT_EQ("umount", umount(MOUNT_DIR), OK);
	TC_ASSERT_NEQ("umount", umount(MOUNT_DIR), OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_mount_p_read_mode
* @brief            Mount file system
* @scenario         Mount file system in read mode, and try to create directory.
* @apicovered       mount
* @precondition     File system should be initialized and not mounted.
* @postcondition    NA
*/
static void itc_fs_vfs_mount_p_read_mode(void)
{
	int ret;

	ret = mount(MOUNT_DEV_DIR, MOUNT_DIR, FS_TYPE, MS_RDONLY, NULL);
	TC_ASSERT_EQ("mount", ret, OK);

	ret = mkdir(VFS_FOLDER_PATH, 0777);
	TC_ASSERT_EQ_CLEANUP("mkdir", ret, OK, umount(MOUNT_DIR));

	ret = rmdir(VFS_FOLDER_PATH);
	TC_ASSERT_EQ("rmdir", ret, OK);

	ret = umount(MOUNT_DIR);
	TC_ASSERT_EQ("umount", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_mount_n_twice
* @brief            Mount file system
* @scenario         Mount file system twice.
* @apicovered       mount
* @precondition     File system should be initialized and not mounted.
* @postcondition    File system should be mounted
*/
static void itc_fs_vfs_mount_n_twice(void)
{
	int ret;

	ret = mount(MOUNT_DEV_DIR, MOUNT_DIR, FS_TYPE, 0, NULL);
	TC_ASSERT_EQ("mount", ret, OK);

	ret = mount(MOUNT_DEV_DIR, MOUNT_DIR, FS_TYPE, 0, NULL);
	TC_ASSERT_NEQ("mount", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_open_n
* @brief            Open file
* @scenario         Open with 1)empty string, 2)directory, 3)file with parent directory does not exists and 4)wrong path
* @apicovered       open
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_open_n(void)
{
	int fd;
	int ret;
	char filename[BUFFER_LEN];

	fd = open("", O_WRONLY | O_CREAT);
	TC_ASSERT_EQ_CLEANUP("open", fd, ERROR, close(fd));

	ret = mkdir(VFS_FOLDER_PATH, 0777);
	TC_ASSERT_EQ("mkdir", ret, OK);

	fd = open(VFS_FOLDER_PATH, O_WRONLY | O_CREAT);
	TC_ASSERT_EQ_CLEANUP("open", fd, ERROR, close(fd); rmdir(VFS_FOLDER_PATH));

	ret = rmdir(VFS_FOLDER_PATH);
	TC_ASSERT_EQ("rmdir", ret, OK);

	strcpy(filename, VFS_FOLDER_PATH);
	strcat(filename, "/file\0");

	fd = open(filename, O_WRONLY | O_CREAT);
	TC_ASSERT_EQ_CLEANUP("open", fd, ERROR, close(fd));

	strcpy(filename, VFS_FILE_PATH);
	strcat(filename, "/file\0");

	fd = open(filename, O_WRONLY | O_CREAT);
	TC_ASSERT_EQ_CLEANUP("open", fd, ERROR, close(fd));

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_close_n
* @brief            Close file
* @scenario         close twice, read after close
* @apicovered       close, read
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_close_n(void)
{
	int fd;
	int ret;
	char read_buf[BUFFER_LEN];

	fd = -1;
	ret = close(fd);
	TC_ASSERT_EQ("close", ret, ERROR);

	fd = open(VFS_FILE_PATH, O_WRONLY | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = write(fd, VFS_TEST_CONTENTS_1, 10);
	TC_ASSERT_EQ_CLEANUP("write", ret, 10, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, OK);

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, OK);

	memset(read_buf, 0, BUFFER_LEN);
	ret = read(fd, read_buf, 10);
	TC_ASSERT_EQ("read", ret, ERROR);

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_read_p_empty_file
* @brief            Read file
* @scenario         Read an empty content file
* @apicovered       read
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_read_p_empty_file(void)
{
	int fd;
	int ret;
	char read_buf[BUFFER_LEN];

	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);
	ret = close(fd);
	TC_ASSERT_EQ("close", ret, OK);

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	memset(read_buf, 0, BUFFER_LEN);
	ret = read(fd, read_buf, BUFFER_LEN);
	TC_ASSERT_EQ_CLEANUP("read", ret, 0, close(fd));
	ret = close(fd);
	TC_ASSERT_EQ("close", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_write_read_p
* @brief            Write file, Read file
* @scenario         Write text to the file, and read it.
* @apicovered       write, read
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_write_read_p(void)
{
	int fd;
	int ret;
	char *write_buf = VFS_TEST_CONTENTS_1;
	char read_buf[BUFFER_LEN];

	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = write(fd, write_buf, 0);
	TC_ASSERT_EQ_CLEANUP("write", ret, 0, close(fd));

	ret = write(fd, write_buf, 5);
	TC_ASSERT_EQ_CLEANUP("write", ret, 5, close(fd));

	ret = write(fd, write_buf + 5, 10);
	TC_ASSERT_EQ_CLEANUP("write", ret, 10, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, OK);

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	memset(read_buf, 0, BUFFER_LEN);
	ret = read(fd, read_buf, 0);
	TC_ASSERT_EQ_CLEANUP("read", ret, 0, close(fd));

	memset(read_buf, 0, BUFFER_LEN);
	ret = read(fd, read_buf, 10);
	TC_ASSERT_EQ_CLEANUP("read", ret, 10, close(fd));
	TC_ASSERT_EQ_CLEANUP("read", strncmp(read_buf, write_buf, ret), 0, close(fd));

	memset(read_buf, 0, BUFFER_LEN);
	ret = read(fd, read_buf, 10);
	TC_ASSERT_EQ_CLEANUP("read", ret, 5, close(fd));
	TC_ASSERT_EQ_CLEANUP("read", strncmp(read_buf, write_buf + 10, ret), 0, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_fsync_p
* @brief            Syncs the file in buffer with the disk.
* @scenario         Open same file with two file descriptors in read and write mode, and checks fsync.
* @apicovered       fsync, write, read
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_fsync_p(void)
{
	int fd1;
	int fd2;
	int ret;
	int len;
	char read_buf[BUFFER_LEN];
	len = strlen(VFS_TEST_CONTENTS_1);

	fd1 = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC | O_CREAT);
	TC_ASSERT_GEQ("open", fd1, 0);
	fd2 = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ_CLEANUP("open", fd2, 0, close(fd1));

	ret = write(fd1, VFS_TEST_CONTENTS_1, len);
	TC_ASSERT_EQ_CLEANUP("write", ret, len, close(fd1); close(fd2));

	TC_ASSERT_GEQ_CLEANUP("fsync", fsync(fd1), 0, close(fd1); close(fd2));

	memset(read_buf, 0, BUFFER_LEN);
	ret = read(fd2, read_buf, BUFFER_LEN);
	TC_ASSERT_EQ_CLEANUP("read", ret, len, close(fd1); close(fd2));
	TC_ASSERT_EQ_CLEANUP("read", strncmp(read_buf, VFS_TEST_CONTENTS_1, ret), 0, close(fd1); close(fd2));

	ret = close(fd1);
	TC_ASSERT_EQ_CLEANUP("close", ret, OK, close(fd2));
	ret = close(fd2);
	TC_ASSERT_EQ("close", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_lseek_n
* @brief            Move current file position to specific position
* @scenario         Move file position with invalid file descriptor(fd), invalid whence.
* @apicovered       lseek
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_lseek_n(void)
{
	int fd;

	TC_ASSERT_EQ("lseek", lseek(-1, 0, SEEK_SET), (off_t)ERROR);

	fd = open(VFS_FILE_PATH, O_RDWR | O_TRUNC | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);
	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, 0, -1), (off_t)ERROR, close(fd));
	TC_ASSERT_EQ("close", close(fd), OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_lseek_n_closed_fd
* @brief            Move current file position to specific position
* @scenario         Move file position with file descriptor(fd) that is closed.
* @apicovered       lseek
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_lseek_n_closed_fd(void)
{
	int fd;

	fd = open(VFS_FILE_PATH, O_RDWR | O_TRUNC | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);
	TC_ASSERT_EQ("close", close(fd), OK);

	TC_ASSERT_EQ("lseek", lseek(fd, 0, SEEK_SET), (off_t)ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_lseek_p
* @brief            Move current file position to specific position
* @scenario         Move file position with diffrent offsets and read and write.
* @apicovered       lseek, write, read
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_lseek_p(void)
{
	int fd;
	int ret;
	char read_buf[BUFFER_LEN];

	fd = open(VFS_FILE_PATH, O_RDWR | O_TRUNC | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);

	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, 0, SEEK_SET), (off_t)0, close(fd));
	TC_ASSERT_EQ_CLEANUP("write", write(fd, VFS_TEST_CONTENTS_1, 15), 15, close(fd));
	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, 0, SEEK_CUR), (off_t)15, close(fd));
	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, 0, SEEK_END), (off_t)15, close(fd));
	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, -5, SEEK_END), (off_t)10, close(fd));
	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, -10, SEEK_CUR), (off_t)0, close(fd));
	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, -15, SEEK_SET), (off_t)0, close(fd));
	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, 20, SEEK_CUR), (off_t)15, close(fd));
	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, 0, SEEK_SET), (off_t)0, close(fd));

	memset(read_buf, 0, BUFFER_LEN);
	ret = read(fd, read_buf, BUFFER_LEN);
	TC_ASSERT_EQ_CLEANUP("read", ret, 15, close(fd));
	TC_ASSERT_EQ_CLEANUP("read", strncmp(read_buf, VFS_TEST_CONTENTS_1, ret), 0, close(fd));

	TC_ASSERT_EQ("close", close(fd), OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_pwrite_pread_p
* @brief            Read/Write data at specific location of the file
* @scenario         1)write, read with invalid file descriptor(fd).
*                   2)Write data at different locations of the file and read it at different locations of the file.
*                   3)After each write/read checking file position should be same as before calling.
* @apicovered       pwrite, pread
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_pwrite_pread_p(void)
{
	int fd;
	int ret;
	char *write_buf = VFS_TEST_CONTENTS_1;
	char read_buf[BUFFER_LEN];

	TC_ASSERT_EQ("pwrite", pwrite(-1, write_buf, 10, 0), ERROR);
	TC_ASSERT_EQ("pread", pread(-1, read_buf, 10, 0), ERROR);

	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);

	TC_ASSERT_EQ_CLEANUP("pwrite", pwrite(fd, write_buf, 0, 0), 0, close(fd));
	TC_ASSERT_EQ_CLEANUP("pwrite", pwrite(fd, write_buf, 10, 0), 10, close(fd));

	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, 4, SEEK_SET), (off_t)4, close(fd));
	TC_ASSERT_EQ_CLEANUP("pwrite", pwrite(fd, write_buf + 7, 8, 7), 8, close(fd));
	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, 0, SEEK_CUR), (off_t)4, close(fd));

	TC_ASSERT_EQ("close", close(fd), OK);

	fd = open(VFS_FILE_PATH, O_RDONLY);
	TC_ASSERT_GEQ("open", fd, 0);

	memset(read_buf, 0, BUFFER_LEN);
	TC_ASSERT_EQ_CLEANUP("pread", pread(fd, read_buf, 0, 0), 0, close(fd));

	memset(read_buf, 0, BUFFER_LEN);
	ret = pread(fd, read_buf, 10, 0);
	TC_ASSERT_EQ_CLEANUP("pread", ret, 10, close(fd));
	TC_ASSERT_EQ_CLEANUP("pread", strncmp(read_buf, write_buf, ret), 0, close(fd));

	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, 4, SEEK_SET), (off_t)4, close(fd));

	memset(read_buf, 0, BUFFER_LEN);
	ret = pread(fd, read_buf, 10, 9);
	TC_ASSERT_EQ_CLEANUP("pread", ret, 6, close(fd));
	TC_ASSERT_EQ_CLEANUP("pread", strncmp(read_buf, write_buf + 9, ret), 0, close(fd));

	TC_ASSERT_EQ_CLEANUP("lseek", lseek(fd, 0, SEEK_CUR), (off_t)4, close(fd));

	TC_ASSERT_EQ("close", close(fd), OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_mkdir_n
* @brief            Create Folders
* @scenario         1)create folder with file location.
*                   2)create folder with empty string.
*                   3)create folder in read mode, and try to create a folder inside it.
* @apicovered       mkdir
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_mkdir_n(void)
{
	char folder_name_1[BUFFER_LEN];
	char folder_name_2[BUFFER_LEN];
	int ret;
	int fd;

	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, OK);

	ret = mkdir(VFS_FILE_PATH, 0777);
	TC_ASSERT_EQ("mkdir", ret, ERROR);

	ret = mkdir("", 0777);
	TC_ASSERT_EQ("mkdir", ret, ERROR);

	strcpy(folder_name_1, VFS_FOLDER_PATH);
	strcpy(folder_name_2, VFS_FOLDER_PATH);
	strcat(folder_name_2, "/1\0");

	ret = mkdir(folder_name_1, S_IRUSR | S_IRGRP | S_IROTH);
	TC_ASSERT_EQ("mkdir", ret, OK);

	ret = mkdir(folder_name_2, 0777);
	TC_ASSERT_NEQ_CLEANUP("mkdir", ret, OK, rmdir(folder_name_2); rmdir(folder_name_1));

	ret = rmdir(folder_name_1);
	TC_ASSERT_EQ("rmdir", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_rmdir_n
* @brief            Remove Folders
* @scenario         remove with file path, remove with empty string
* @apicovered       rmdir
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_rmdir_n(void)
{
	int ret;
	int fd;

	fd = open(VFS_FILE_PATH, O_WRONLY | O_TRUNC | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);
	TC_ASSERT_EQ("close", close(fd), OK);

	ret = rmdir(VFS_FILE_PATH);
	TC_ASSERT_EQ("rmdir", ret, ERROR);

	ret = rmdir("");
	TC_ASSERT_EQ("rmdir", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_mkdir_rmdir_n
* @brief            Create Folders, Remove Folders
* @scenario         create folder whose parent directory does not exists, create folder which already exists,
*                   remove folder which is not empty, remove folder which is not present.
* @apicovered       mkdir, rmdir
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_mkdir_rmdir_n(void)
{
	char folder_name_1[BUFFER_LEN];
	char folder_name_2[BUFFER_LEN];
	int ret;

	strcpy(folder_name_1, VFS_FOLDER_PATH);
	strcpy(folder_name_2, VFS_FOLDER_PATH);
	strcat(folder_name_2, "/1\0");

	//mkdir
	ret = mkdir(folder_name_2, 0777);
	TC_ASSERT_EQ("mkdir", ret, ERROR);

	ret = mkdir(folder_name_1, 0777);
	TC_ASSERT_EQ("mkdir", ret, OK);

	ret = mkdir(folder_name_2, 0777);
	TC_ASSERT_EQ("mkdir", ret, OK);

	ret = mkdir(folder_name_2, 0777);
	TC_ASSERT_EQ("mkdir", ret, ERROR);

	//rmdir
	ret = rmdir(folder_name_1);
	TC_ASSERT_EQ("rmdir", ret, ERROR);

	ret = rmdir(folder_name_2);
	TC_ASSERT_EQ("rmdir", ret, OK);

	ret = rmdir(folder_name_1);
	TC_ASSERT_EQ("rmdir", ret, OK);

	ret = rmdir(folder_name_1);
	TC_ASSERT_EQ("rmdir", ret, ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_opendir_n_invalid_path
* @brief            Open a directory
* @scenario         open directory with invalid path, open directory with empty string.
* @apicovered       opendir
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_opendir_n_invalid_path(void)
{
	DIR *dirp;

	rmdir(VFS_FOLDER_PATH);
	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_EQ_CLEANUP("opendir", dirp, NULL, closedir(dirp));

	dirp = opendir("");
	TC_ASSERT_EQ_CLEANUP("opendir", dirp, NULL, closedir(dirp));

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_opendir_n_file_path
* @brief            Open a directory
* @scenario         open directory with a file path.
* @apicovered       opendir
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_opendir_n_file_path(void)
{
	DIR *dirp;

	dirp = opendir(VFS_FILE_PATH); //Open with file path not with directory
	TC_ASSERT_EQ_CLEANUP("opendir", dirp, NULL, closedir(dirp));

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_readdir_rewinddir_p
* @brief            Reads contents of directory sequentially, and Resets the position of the contents in directory.
* @scenario         Adding and removing directory dynamically into a parent directory and reading and rewinding the parent directory
* @apicovered       readdir, rewinddir
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_readdir_rewinddir_p(void)
{
	int ret;
	int len;
	int count = 3;
	int dir_count = 0;
	DIR *dirp;
	struct dirent *st_dirent;
	char folder_name_0[BUFFER_LEN];
	char folder_name_1[BUFFER_LEN];
	char folder_name_2[BUFFER_LEN];

	strcpy(folder_name_0, VFS_FOLDER_PATH);
	strcat(folder_name_0, "/0\0");
	len = strlen(folder_name_0);
	strcpy(folder_name_1, folder_name_0);
	strcpy(folder_name_2, folder_name_0);
	folder_name_1[len - 1] = '1';
	folder_name_2[len - 1] = '2';

	st_dirent = readdir(NULL);
	TC_ASSERT_EQ("readdir", st_dirent, NULL);

	ret = mkdir(VFS_FOLDER_PATH, 0777);
	TC_ASSERT_EQ("mkdir", ret, OK);

	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dirp, NULL, rmdir(VFS_FOLDER_PATH));

	st_dirent = readdir(dirp);
	TC_ASSERT_EQ_CLEANUP("readdir", st_dirent, NULL, closedir(dirp); rmdir(VFS_FOLDER_PATH));

	rewinddir(dirp);

	st_dirent = readdir(dirp);
	TC_ASSERT_EQ_CLEANUP("readdir", st_dirent, NULL, closedir(dirp); rmdir(VFS_FOLDER_PATH));

	ret = closedir(dirp);
	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, rmdir(VFS_FOLDER_PATH));

	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dirp, NULL, rmdir(VFS_FOLDER_PATH));

	ret = mkdir(folder_name_0, 0777);
	TC_ASSERT_EQ_CLEANUP("mkdir", ret, OK, closedir(dirp); rmdir(VFS_FOLDER_PATH));

	st_dirent = readdir(dirp);
	TC_ASSERT_NEQ_CLEANUP("readdir", st_dirent, NULL, closedir(dirp); remove_dirs(count));

	ret = mkdir(folder_name_1, 0777);
	TC_ASSERT_EQ_CLEANUP("mkdir", ret, OK, closedir(dirp); remove_dirs(count));

	ret = mkdir(folder_name_2, 0777);
	TC_ASSERT_EQ_CLEANUP("mkdir", ret, OK, closedir(dirp); remove_dirs(count));

	rewinddir(dirp);

	ret = rmdir(folder_name_0);
	TC_ASSERT_EQ_CLEANUP("rmdir", ret, OK, closedir(dirp); remove_dirs(count));

	while (readdir(dirp) != NULL) {
		dir_count++;
	}

	TC_ASSERT_EQ_CLEANUP("readdir", dir_count, 2, closedir(dirp); remove_dirs(count));

	ret = closedir(dirp);
	TC_ASSERT_EQ_CLEANUP("closedir", ret, OK, remove_dirs(count));

	ret = rmdir(folder_name_1);
	TC_ASSERT_EQ_CLEANUP("rmdir", ret, OK, remove_dirs(count));

	ret = rmdir(folder_name_2);
	TC_ASSERT_EQ_CLEANUP("rmdir", ret, OK, remove_dirs(count));

	ret = rmdir(VFS_FOLDER_PATH);
	TC_ASSERT_EQ("rmdir", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_seekdir_telldir_p
* @brief            Move position of contents in directory, Get position of contents in directory.
* @scenario         move to different locations, and get the location.
* @apicovered       seekdir, telldir
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_seekdir_telldir_p(void)
{
	DIR *dirp;
	struct dirent *st_dirent;
	off_t offset;
	int count;
	char buffer[BUFFER_LEN];

	g_flag = OK;
	add_dirs(VFS_LOOP_COUNT);
	TC_ASSERT_EQ("mkdir", g_flag, OK);

	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dirp, NULL, remove_dirs(VFS_LOOP_COUNT));

	TC_ASSERT_EQ_CLEANUP("telldir", telldir(dirp), (off_t)0, closedir(dirp); remove_dirs(VFS_LOOP_COUNT));

	count = 0;
	while (1) {
		offset = count;
		seekdir(dirp, offset);
		TC_ASSERT_NEQ_CLEANUP("seekdir", dirp, NULL, closedir(dirp); remove_dirs(VFS_LOOP_COUNT));

		if (count < 0) {
			TC_ASSERT_EQ_CLEANUP("telldir", telldir(dirp), (off_t)0, closedir(dirp); remove_dirs(VFS_LOOP_COUNT));
			st_dirent = readdir(dirp);
			TC_ASSERT_NEQ_CLEANUP("readdir", st_dirent, NULL, closedir(dirp); remove_dirs(VFS_LOOP_COUNT));
			TC_ASSERT_EQ_CLEANUP("readdir", st_dirent->d_name[0], '0', closedir(dirp); remove_dirs(VFS_LOOP_COUNT));
			TC_ASSERT_EQ_CLEANUP("telldir", telldir(dirp), (off_t)1, closedir(dirp); remove_dirs(VFS_LOOP_COUNT));
		} else if (count >= 0 && count < 5) {
			TC_ASSERT_EQ_CLEANUP("telldir", telldir(dirp), (off_t)count, closedir(dirp); remove_dirs(VFS_LOOP_COUNT));
			st_dirent = readdir(dirp);
			TC_ASSERT_NEQ_CLEANUP("readdir", st_dirent, NULL, closedir(dirp); remove_dirs(VFS_LOOP_COUNT));
			itoa(count, buffer, 10);
			TC_ASSERT_EQ_CLEANUP("readdir", st_dirent->d_name[0], buffer[0], closedir(dirp); remove_dirs(VFS_LOOP_COUNT));
			TC_ASSERT_EQ_CLEANUP("telldir", telldir(dirp), (off_t)(count + 1), closedir(dirp); remove_dirs(VFS_LOOP_COUNT));
		} else {
			TC_ASSERT_EQ_CLEANUP("telldir", telldir(dirp), (off_t)5, closedir(dirp); remove_dirs(VFS_LOOP_COUNT));
			st_dirent = readdir(dirp);
			TC_ASSERT_EQ_CLEANUP("readdir", st_dirent, NULL, closedir(dirp); remove_dirs(VFS_LOOP_COUNT));
			TC_ASSERT_EQ_CLEANUP("telldir", telldir(dirp), (off_t)5, closedir(dirp); remove_dirs(VFS_LOOP_COUNT));
		}

		if (count == -5) {
			break;
		} else if (count == 10) {
			count = -count;
		} else {
			count++;
		}
	}

	TC_ASSERT_EQ_CLEANUP("closedir", closedir(dirp), OK, remove_dirs(VFS_LOOP_COUNT));

	g_flag = OK;
	remove_dirs(VFS_LOOP_COUNT);
	TC_ASSERT_EQ("rmdir", g_flag, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_fs_vfs_closedir_n
* @brief            Close opened directory
* @scenario         Read directory after closing.
* @apicovered       closedir, readdir
* @precondition     NA
* @postcondition    NA
*/
static void itc_fs_vfs_closedir_n(void)
{
	DIR *dirp;
	struct dirent *st_dirent;
	int dir_count = 1;

	g_flag = OK;
	add_dirs(dir_count);
	TC_ASSERT_EQ("mkdir", g_flag, OK);

	dirp = opendir(VFS_FOLDER_PATH);
	TC_ASSERT_NEQ_CLEANUP("opendir", dirp, NULL, remove_dirs(dir_count));

	TC_ASSERT_EQ_CLEANUP("closedir", closedir(dirp), OK, remove_dirs(dir_count));

	st_dirent = readdir(dirp);
	TC_ASSERT_EQ_CLEANUP("readdir", st_dirent, NULL, remove_dirs(dir_count));

	g_flag = OK;
	remove_dirs(dir_count);
	TC_ASSERT_EQ("rmdir", g_flag, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_libc_stdio_fdopen_p
* @brief            Open file with file descriptor(fd)
* @scenario         file descriptor(fd) in read mode, and opening it in write mode.
* @apicovered       fdopen
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_fdopen_p(void)
{
	int fd;
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	fd = open(filename, O_RDONLY | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);

	fp = fdopen(fd, "w");

	TC_ASSERT_NEQ("fdopen", fp, NULL);
	TC_ASSERT_EQ_CLEANUP("fdopen", fp->fs_oflags, O_WRONLY | O_CREAT | O_TRUNC, fclose(fp));

	TC_ASSERT_EQ("fclose", fclose(fp), OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_libc_stdio_fdopen_n
* @brief            Open file with file descriptor(fd)
* @scenario         Opening with invalid fd.
* @apicovered       fdopen
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_fdopen_n(void)
{
	int fd;
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	fd = open(filename, O_RDONLY | O_CREAT);
	TC_ASSERT_GEQ("open", fd, 0);
	TC_ASSERT_EQ("close", close(fd), OK);

	fp = fdopen(fd, "r");
	TC_ASSERT_EQ_CLEANUP("fdopen", fp, NULL, fclose(fp));

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_libc_stdio_fopen_n_invalid_path
* @brief            Open file with path, permissions
* @scenario         Opening with folder location, empty location.
* @apicovered       fopen
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_fopen_n_invalid_path(void)
{
	FILE *fp;
	char *foldername = VFS_FOLDER_PATH;

	fp = fopen("", "r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp));

	g_flag = OK;
	add_dirs(0);
	TC_ASSERT_EQ("mkdir", g_flag, OK);

	fp = fopen(foldername, "r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp); remove_dirs(0));

	g_flag = OK;
	remove_dirs(0);
	TC_ASSERT_EQ("rmdir", g_flag, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_libc_stdio_fopen_n_invalid_mode
* @brief            Open file with path, mode(permissions)
* @scenario         opening with invalid mode(permissions).
* @apicovered       fopen
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_fopen_n_invalid_mode(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	fp = fopen(filename, "rr");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp));

	fp = fopen(filename, "r+r");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp));

	fp = fopen(filename, "rr+");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp));

	fp = fopen(filename, "");
	TC_ASSERT_EQ_CLEANUP("fopen", fp, NULL, fclose(fp));

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_libc_stdio_fclose_n_null
* @brief            Close file
* @scenario         Closing file with null.
* @apicovered       fclose
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_fclose_n_null(void)
{
	TC_ASSERT_EQ("fclose", fclose(NULL), ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_libc_stdio_fputs_n
* @brief            write contents in the file through fputs
* @scenario         write to the file that was opened in read mode, with invalid file pointer(fp)
* @apicovered       fputs
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_fputs_n(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_1;

	fp = fopen(filename, "r");
	TC_ASSERT_NEQ("fopen", fp, NULL);
	TC_ASSERT_EQ_CLEANUP("fputs", fputs(str, fp), EOF, fclose(fp));
	TC_ASSERT_EQ("fclose", fclose(fp), OK);

	TC_ASSERT_EQ("fputs", fputs(str, NULL), EOF);

	fp = fopen(filename, "w");
	TC_ASSERT_NEQ("fopen", fp, NULL);
	TC_ASSERT_EQ("fclose", fclose(fp), OK);

	TC_ASSERT_EQ("fputs", fputs(str, fp), EOF);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_libc_stdio_fgets_n
* @brief            get contents in the file through fgets
* @scenario         fgets with invalid file pointer(fp)
* @apicovered       fgets
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_fgets_n(void)
{
	char buf[BUFFER_LEN];

	TC_ASSERT_EQ("fgets", fgets(buf, BUFFER_LEN, NULL), NULL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_libc_stdio_fputs_fgets_p
* @brief            write contents in the file through fputs, get contents in the file through fgets
* @scenario         write content in file with fputs and checking it with fgets
* @apicovered       fputs, fgets
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_fputs_fgets_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char *str1 = VFS_TEST_CONTENTS_1;
	char *str2 = VFS_TEST_CONTENTS_2;
	char read_buf[BUFFER_LEN];

	fp = fopen(filename, "w");
	TC_ASSERT_NEQ("fopen", fp, NULL);
	TC_ASSERT_NEQ_CLEANUP("fputs", fputs(str1, fp), EOF, fclose(fp));
	TC_ASSERT_EQ("fclose", fclose(fp), OK);

	fp = fopen(filename, "r+");
	TC_ASSERT_NEQ("fopen", fp, NULL);
	TC_ASSERT_NEQ_CLEANUP("fputs", fputs(str2, fp), EOF, fclose(fp));
	TC_ASSERT_EQ("fclose", fclose(fp), OK);

	fp = fopen(filename, "a");
	TC_ASSERT_NEQ("fopen", fp, NULL);
	TC_ASSERT_NEQ_CLEANUP("fputs", fputs("\n", fp), EOF, fclose(fp));
	TC_ASSERT_EQ("fclose", fclose(fp), OK);

	fp = fopen(filename, "a+");
	TC_ASSERT_NEQ("fopen", fp, NULL);

	TC_ASSERT_NEQ_CLEANUP("fputs", fputs(str1, fp), EOF, fclose(fp));

	TC_ASSERT_EQ("fclose", fclose(fp), OK);

	TC_ASSERT_EQ("fgets", fgets(read_buf, BUFFER_LEN, fp), NULL);

	fp = fopen(filename, "a");
	TC_ASSERT_NEQ("fopen", fp, NULL);
	TC_ASSERT_EQ_CLEANUP("fgets", fgets(read_buf, BUFFER_LEN, fp), NULL, fclose(fp));
	TC_ASSERT_EQ("fclose", fclose(fp), OK);

	fp = fopen(filename, "r");
	TC_ASSERT_NEQ("fopen", fp, NULL);

	memset(read_buf, 0, BUFFER_LEN);
	TC_ASSERT_NEQ_CLEANUP("fgets", fgets(read_buf, BUFFER_LEN, fp), NULL, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fgets", strlen(read_buf), 19, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fgets", strncmp(read_buf, str2, 18), 0, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fgets", read_buf[18], '\n', fclose(fp));

	memset(read_buf, 0, BUFFER_LEN);
	TC_ASSERT_NEQ_CLEANUP("fgets", fgets(read_buf, BUFFER_LEN, fp), NULL, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fgets", strlen(read_buf), 18, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fgets", strcmp(read_buf, str1), 0, fclose(fp));

	memset(read_buf, 0, BUFFER_LEN);
	TC_ASSERT_EQ_CLEANUP("fgets", fgets(read_buf, BUFFER_LEN, fp), NULL, fclose(fp));

	TC_ASSERT_EQ("fclose", fclose(fp), OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_libc_stdio_fseek_n
* @brief            Move file position to specific position
* @scenario         Move file position with invalid parameters
* @apicovered       fseek
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_fseek_n(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;

	ret = fseek(NULL, 0, SEEK_SET);
	TC_ASSERT_NEQ("fseek", ret, OK);

	fp = fopen(filename, "w");
	TC_ASSERT_NEQ("fopen", fp, NULL);

	ret = fseek(fp, 0, -1);
	TC_ASSERT_NEQ_CLEANUP("fseek", ret, OK, fclose(fp));

	ret = fclose(fp);
	TC_ASSERT_EQ("fclose", ret, OK);

	TC_SUCCESS_RESULT();
}

#if 0
/**
* @testcase         itc_libc_stdio_fseek_n_invalid_fp
* @brief            Move file position to specific position
* @scenario         Move file position with invalid fp
* @apicovered       fseek
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_fseek_n_invalid_fp(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;
	ret = 0;

	fp = fopen(filename, "w");
	TC_ASSERT_NEQ("fopen", fp, NULL);
	ret = fclose(fp);
	TC_ASSERT_EQ("fclose", ret, OK);

	ret = fseek(fp, 0, SEEK_SET);
	TC_ASSERT_NEQ("fseek", ret, OK);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @testcase         itc_libc_stdio_ftell_n
* @brief            Get current file position
* @scenario         Get file position with invalid fp
* @apicovered       ftell
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_ftell_n(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	TC_ASSERT_EQ("ftell", ftell(NULL), ERROR);

	fp = fopen(filename, "w");
	TC_ASSERT_NEQ("fopen", fp, NULL);
	TC_ASSERT_EQ("fclose", fclose(fp), OK);

	TC_ASSERT_EQ("ftell", ftell(fp), (long)ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_libc_stdio_fseek_ftell_p
* @brief            Move file position to specific position, Get current position of fp
* @scenario         Move fp to specific position, and check current position.
* @apicovered       fseek, ftell
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_fseek_ftell_p(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char write_buf[BUFFER_LEN];
	char read_buf[BUFFER_LEN];
	int null_index = 15;

	fp = fopen(filename, "w+");
	TC_ASSERT_NEQ("fopen", fp, NULL);

	TC_ASSERT_EQ_CLEANUP("fseek", fseek(fp, 0, SEEK_SET), OK, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("ftell", ftell(fp), (long)0, fclose(fp));

	strncpy(write_buf, VFS_TEST_CONTENTS_1, strlen(VFS_TEST_CONTENTS_1));

	write_buf[null_index] = '\0';
	TC_ASSERT_NEQ_CLEANUP("fputs", fputs(write_buf, fp), EOF, fclose(fp));

	TC_ASSERT_EQ_CLEANUP("fseek", fseek(fp, 0, SEEK_CUR), OK, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("ftell", ftell(fp), (long)15, fclose(fp));

	TC_ASSERT_EQ_CLEANUP("fseek", fseek(fp, 0, SEEK_END), OK, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("ftell", ftell(fp), (long)15, fclose(fp));

	TC_ASSERT_EQ_CLEANUP("fseek", fseek(fp, -5, SEEK_END), OK, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("ftell", ftell(fp), (long)10, fclose(fp));

	TC_ASSERT_EQ_CLEANUP("fseek", fseek(fp, -10, SEEK_CUR), OK, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("ftell", ftell(fp), (long)0, fclose(fp));

	TC_ASSERT_EQ_CLEANUP("fseek", fseek(fp, -15, SEEK_SET), OK, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("ftell", ftell(fp), (long)0, fclose(fp));

	TC_ASSERT_EQ_CLEANUP("fseek", fseek(fp, 20, SEEK_CUR), OK, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("ftell", ftell(fp), (long)15, fclose(fp));

	TC_ASSERT_EQ_CLEANUP("fseek", fseek(fp, 0, SEEK_SET), OK, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("ftell", ftell(fp), (long)0, fclose(fp));

	memset(read_buf, 0, BUFFER_LEN);
	TC_ASSERT_NEQ_CLEANUP("fgets", fgets(read_buf, BUFFER_LEN, fp), NULL, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fgets", strlen(read_buf), 15, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fgets", strncmp(read_buf, write_buf, 15), 0, fclose(fp));

	TC_ASSERT_EQ("fclose", fclose(fp), OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_libc_stdio_fwrite_fread_p
* @brief            Write contents in the file, Read contents of the file
* @scenario         Write contents and then check it by reading the file
* @apicovered       fwrite, fread
* @precondition     NA
* @postcondition    NA
*/
static void itc_libc_stdio_fwrite_fread_p(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;
	char *write_buf = VFS_TEST_CONTENTS_1;
	char read_buf[BUFFER_LEN];

	fp = fopen(filename, "w");
	TC_ASSERT_NEQ("fopen", fp, NULL);

	ret = fwrite(write_buf, 0, 0, fp);
	TC_ASSERT_EQ_CLEANUP("fwrite", ret, 0, fclose(fp));

	ret = fwrite(write_buf, 0, 5, fp);
	TC_ASSERT_EQ_CLEANUP("fwrite", ret, 0, fclose(fp));

	ret = fwrite(write_buf, 1, 5, fp);
	TC_ASSERT_EQ_CLEANUP("fwrite", ret, 5, fclose(fp));

	ret = fwrite(write_buf + 5, 2, 5, fp);
	TC_ASSERT_EQ_CLEANUP("fwrite", ret, 5, fclose(fp));

	ret = fclose(fp);
	TC_ASSERT_EQ("fclose", ret, OK);

	fp = fopen(filename, "r");
	TC_ASSERT_NEQ("fopen", fp, NULL);

	memset(read_buf, 0, BUFFER_LEN);
	ret = fread(read_buf, 0, 0, fp);
	TC_ASSERT_EQ_CLEANUP("fread", ret, 0, fclose(fp));

	memset(read_buf, 0, BUFFER_LEN);
	ret = fread(read_buf, 0, 5, fp);
	TC_ASSERT_EQ_CLEANUP("fread", ret, 0, fclose(fp));

	memset(read_buf, 0, BUFFER_LEN);
	ret = fread(read_buf, 2, 5, fp);
	TC_ASSERT_EQ_CLEANUP("fread", ret, 5, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fread", strncmp(read_buf, write_buf, 10), 0, fclose(fp));

	memset(read_buf, 0, BUFFER_LEN);
	ret = fread(read_buf, 1, 10, fp);
	TC_ASSERT_EQ_CLEANUP("fread", ret, 5, fclose(fp));
	TC_ASSERT_EQ_CLEANUP("fread", strncmp(read_buf, write_buf + 10, 5), 0, fclose(fp));

	ret = fclose(fp);
	TC_ASSERT_EQ("fclose", ret, OK);

	TC_SUCCESS_RESULT();
}

void itc_fs_main(void)
{
	itc_fs_vfs_umount_n_twice();
	itc_fs_vfs_mount_p_read_mode();
	itc_fs_vfs_mount_n_twice();
	itc_fs_vfs_open_n();
	itc_fs_vfs_close_n();
	itc_fs_vfs_read_p_empty_file();
	itc_fs_vfs_write_read_p();
	itc_fs_vfs_fsync_p();
	itc_fs_vfs_lseek_n();
	itc_fs_vfs_lseek_n_closed_fd();
	itc_fs_vfs_lseek_p();
	itc_fs_vfs_pwrite_pread_p();
	itc_fs_vfs_mkdir_n();
	itc_fs_vfs_rmdir_n();
	itc_fs_vfs_mkdir_rmdir_n();
	itc_fs_vfs_opendir_n_invalid_path();
	itc_fs_vfs_opendir_n_file_path();
	itc_fs_vfs_readdir_rewinddir_p();
	itc_fs_vfs_seekdir_telldir_p();
	itc_fs_vfs_closedir_n();

	itc_libc_stdio_fdopen_p();
	itc_libc_stdio_fdopen_n();
	itc_libc_stdio_fopen_n_invalid_path();
	itc_libc_stdio_fopen_n_invalid_mode();
	itc_libc_stdio_fclose_n_null();
	itc_libc_stdio_fputs_n();
	itc_libc_stdio_fgets_n();
	itc_libc_stdio_fputs_fgets_p();
	itc_libc_stdio_fseek_n();
	//itc_libc_stdio_fseek_n_invalid_fp();  //System hangs-up on this TC being called, program not terminating
	itc_libc_stdio_ftell_n();
	itc_libc_stdio_fseek_ftell_p();
	itc_libc_stdio_fwrite_fread_p();

	return;
}

