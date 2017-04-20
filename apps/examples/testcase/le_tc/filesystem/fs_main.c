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
/// @brief Cursor functions.

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

#ifdef CONFIG_FS_AIO
#include <aio.h>
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define MOUNT_DIR CONFIG_MOUNT_POINT

#ifdef CONFIG_FS_SMARTFS
#define MOUNT_DEV_DIR "/dev/smart1"
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

#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 0)
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
static int g_tc_count;
static int g_tc_fail_count;
#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 0)
static int g_thread_result;
#endif
extern sem_t tc_sem;
extern int working_tc;

#ifdef CONFIG_FS_AIO

#define AIO_RDBUFFER_SIZE  128
#define AIO_WRBUFFER1_SIZE sizeof(g_wrbuffer1)
#define AIO_WRBUFFER2_SIZE sizeof(g_wrbuffer2)

#define AIO_NCTRLBLKS      5
#define CONFIG_EXAMPLES_OSTEST_AIOPATH "/mnt/file.txt"
#define AIO_FILEPATH CONFIG_EXAMPLES_OSTEST_AIOPATH

static const char g_wrbuffer1[] = "This is write buffer #1\n";
static const char g_wrbuffer2[] = "This second write buffer is this line\n";
static char g_rdbuffer[AIO_RDBUFFER_SIZE];

/* AIO control blocks:  write, nop, write, NULL, read */

static struct aiocb g_aiocbs[AIO_NCTRLBLKS - 1];
static struct aiocb *g_aiocb[AIO_NCTRLBLKS];

static struct aiocb *const g_aiocb_init[AIO_NCTRLBLKS] = {
	&g_aiocbs[0], &g_aiocbs[1], &g_aiocbs[2], NULL, &g_aiocbs[3]
};

static FAR void *const g_buffers[AIO_NCTRLBLKS] = {
	(FAR void *)g_wrbuffer1,
	(FAR void *)NULL,
	(FAR void *)g_wrbuffer2,
	(FAR void *)NULL,
	(FAR void *)g_rdbuffer
};

static const FAR uint8_t g_offsets[AIO_NCTRLBLKS] = {
	0, 0, AIO_WRBUFFER1_SIZE, 0, 0
};

static const FAR uint8_t g_nbytes[AIO_NCTRLBLKS] = {
	AIO_WRBUFFER1_SIZE, 0, AIO_WRBUFFER2_SIZE, 0, AIO_RDBUFFER_SIZE
};

static const FAR uint8_t g_opcode[AIO_NCTRLBLKS] = {
	LIO_WRITE, LIO_NOP, LIO_WRITE, LIO_NOP, LIO_READ
};

static int g_fildes;

int aio_test();

static void init_aiocb(bool signal)
{
	FAR struct aiocb *aiocbp;
	int i;

	memset(g_aiocbs, 0xff, (AIO_NCTRLBLKS - 1)*sizeof(struct aiocb));
	memset(g_rdbuffer, 0xff, AIO_RDBUFFER_SIZE);

	for (i = 0; i < AIO_NCTRLBLKS; i++) {
		aiocbp     = g_aiocb_init[i];
		g_aiocb[i] = aiocbp;

		if (aiocbp) {
			aiocbp->aio_sigevent.sigev_notify = signal ? SIGEV_SIGNAL : SIGEV_NONE;
			aiocbp->aio_sigevent.sigev_signo  = SIGUSR1;

			aiocbp->aio_buf        = g_buffers[i];
			aiocbp->aio_offset     = (off_t)g_offsets[i];
			aiocbp->aio_nbytes     = (size_t)g_nbytes[i];
			aiocbp->aio_fildes     = g_fildes;
			aiocbp->aio_reqprio    = 0;
			aiocbp->aio_lio_opcode = g_opcode[i];
		}
	}
}

static int check_done(void)
{
	FAR struct aiocb *aiocbp;
	int i;

	/* Check each entry in the list.  Break out of the loop if any entry
	 * has not completed.
	 */

	for (i = 0; i < AIO_NCTRLBLKS; i++) {
		/* Skip over NULL entries */

		aiocbp = g_aiocb[i];
		if (aiocbp) {
			/* Check if the I/O has completed */

			printf("  list[%d]. result = %d\n", i, aiocbp->aio_result);
			if (aiocbp->aio_lio_opcode == LIO_NOP) {
				printf("     NO operation\n");
			} else if (aiocbp->aio_result == -EINPROGRESS) {
				/* No.. return -EINPROGRESS */

				printf("     NOT finished\n");
				return -EINPROGRESS;
			} else if (aiocbp->aio_result == -ECANCELED) {
				/* No.. return -EINPROGRESS */

				printf("     Cancelled\n");
			}

			/* Check for an I/O error */

			else if (aiocbp->aio_result < 0) {
				printf("     ERROR: Failed I/O transfer\n");
			}

			/* Successful completion r */

			else {
				printf("     Successful completion\n");
			}
		} else {
			printf("  list[%d]. NULL\n", i);
		}
	}

	return OK;
}

int aio_test()
{
	int ret = -1;

	/* Case 1: Poll for transfer complete */

	printf("Test AIO: Poll for transfer complete\n");
	g_fildes = open(AIO_FILEPATH, O_RDWR | O_CREAT | O_TRUNC);
	if (g_fildes < 0) {
		printf("aio_test: ERROR: Failed to open %s: %d\n", AIO_FILEPATH, errno);
		goto errout;
	}

	init_aiocb(false);
	ret = lio_listio(LIO_NOWAIT, g_aiocb, AIO_NCTRLBLKS, NULL);
	if (ret < 0) {
		printf("aio_test: ERROR: lio_listio failed: %d\n", errno);
		goto errout;
	}

	do {
		usleep(500 * 1000);
		ret = check_done();
	} while (ret < 0);

	close(g_fildes);
	g_fildes = -1;

errout:

	close(g_fildes);
	g_fildes = -1;

	return ret;

}

#endif /* END OF CONFIG_FS_AIO */

#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 0)
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
			return;
		}
		snprintf(data, size, "%s%d\0", FIFO_DATA, count);
		printf("write data : %s read data : %s\n", data, buf);

		if (strcmp(data, buf) != 0) {
			printf("mkfifo failed compare Read data with written data\n");
			g_thread_result = false;
			return;
		}
		count++;
	}
	close(fd);
}
#endif
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


static int fs_vfs_mount_tc(void)
{
	int ret;

	printf("%d. mount Test started. \n", g_tc_count++);
	ret = mount(MOUNT_DEV_DIR, CONFIG_MOUNT_POINT, TARGET_FS_NAME, 0, NULL);
	if (ret < 0) {
		printf("mount Failed : %d\n", ret);
		g_tc_fail_count++;
		return ERROR;
	}
	printf("PASS!\n");
	return OK;
}

static int fs_vfs_umount_tc(void)
{
	int ret;

	printf("%d. umount Test started. \n", g_tc_count++);
	ret = umount(CONFIG_MOUNT_POINT);
	if (ret < 0) {
		printf("umount Failed : %d\n", ret);
		g_tc_fail_count++;
		return ERROR;
	}
	printf("PASS!\n");
	return OK;

}

static int fs_vfs_open_tc(void)
{
	int fd;
	char *filename = VFS_FILE_PATH;
	printf("%d. open Test started. \n", g_tc_count++);
	fd = open(filename, O_WROK | O_CREAT);
	if (fd < 0) {
		printf("open Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd;
	}
	printf("PASS!\n");
	close(fd);
	return fd;

}

static int fs_vfs_write_tc(void)
{
	int fd, ret;
	char *filename = VFS_FILE_PATH;
	char *buf = VFS_TEST_CONTENTS_1;
	int len;

	printf("%d. write Test started. \n", g_tc_count++);
	fd = open(filename, O_WRONLY | O_TRUNC);
	if (fd < 0) {
		printf("Open Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd;
	}

	len = strlen(buf);
	ret = write(fd, buf, len);
	if (ret != len) {
		printf("Write Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd);
		return ret;
	}
	printf("PASS!\n");
	close(fd);
	return ret;
}

static int fs_vfs_read_tc(void)
{
	int fd, ret;
	char *filename = VFS_FILE_PATH;
	char buf[20];

	printf("%d. read Test started. \n", g_tc_count++);
	fd = open(VFS_FILE_PATH, O_RDONLY);
	if (fd < 0) {
		printf("Open Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd;
	}
	memset(buf, 0, sizeof(buf));
	ret = read(fd, buf, sizeof(buf));
	if (ret <= 0 || strcmp(buf, VFS_TEST_CONTENTS_1) != 0) {
		printf("Read Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd);
		return ret;
	}
	printf("PASS! : %s\n", buf);
	close(fd);
	return ret;
}

static int fs_vfs_close_tc(void)
{
	int fd, ret;
	char *filename = VFS_FILE_PATH;

	printf("%d. close Test started. \n", g_tc_count++);
	fd = open(VFS_FILE_PATH, O_RDONLY);
	if (fd < 0) {
		printf("open Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd;
	}

	ret = close(fd);
	if (ret < 0) {
		printf("close Failed : %d\n", ret);
		g_tc_fail_count++;
		return ret;
	}
	printf("PASS!\n");
	return ret;
}

static int fs_vfs_dup_tc(void)
{
	char *filename = VFS_DUP_FILE_PATH;
	char *str1 = VFS_TEST_CONTENTS_1;
	char *str2 = VFS_TEST_CONTENTS_2;
	char buf[20];
	int fd1, fd2, ret, len;

	memset(buf, 0, sizeof(buf));

	printf("%d. dup Test started. \n", g_tc_count++);
	fd1 = open(filename, O_RDWR | O_CREAT | O_TRUNC);
	if (fd1 < 0) {
		printf("open Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd1;
	}
	len = strlen(str1);
	ret = write(fd1, str1, len);
	if (ret != len) {
		printf("Write /w fd1 Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd1);
		return ret;
	}

	fd2 = dup(fd1);
	if (fd2 < 0) {
		close(fd1);
		printf("dup Failed : %d\n", fd2);
		g_tc_fail_count++;
		return ret;
	}
	len = strlen(str2);
	ret = write(fd2, str2, strlen(str2));
	if (ret != len) {
		printf("Write /w fd2 Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd1);
		close(fd2);
		return ret;
	}
	close(fd1);
	close(fd2);

	fd1 = open(filename, O_RDOK);
	if (fd1 < 0) {
		printf("open after dup Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd1;
	}

	len = strlen(str1);
	ret = read(fd1, buf, len);
	if (ret <= 0 || strcmp(buf, VFS_TEST_CONTENTS_1) != 0) {
		printf("Read Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd1);
		return ret;
	}
	printf("First Read : %s\n", buf);

	memset(buf, 0, sizeof(buf));
	len = strlen(str2);
	ret = read(fd1, buf, len);
	if (ret <= 0 || strcmp(buf, VFS_TEST_CONTENTS_2) != 0) {
		printf("Read Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd1);
		return ret;
	}
	printf("Second Read : %s\n", buf);
	printf("PASS!\n");
	close(fd1);
	return ret;
}

static int fs_vfs_dup2_tc(void)
{
	char *filename1 = VFS_DUP_FILE_PATH;
	char *filename2 = VFS_DUP2_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_3;
	char buf[20];
	int fd1, fd2, ret, len;

	memset(buf, 0, sizeof(buf));

	printf("%d. dup2 Test started. \n", g_tc_count++);
	fd1 = open(filename1, O_RDWR);
	if (fd1 < 0) {
		printf("open Failed : %s\n", filename1);
		g_tc_fail_count++;
		return fd1;
	}

	fd2 = open(filename2, O_RDWR | O_CREAT);
	if (fd2 < 0) {
		printf("open Failed : %s\n", filename2);
		g_tc_fail_count++;
		close(fd1);
		return fd1;
	}
	close(fd1);
	//now fd1 points fd2
	ret = dup2(fd2, fd1);
	if (ret < 0) {
		printf("dup2 Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd2);
		return ret;
	}

	len = strlen(VFS_TEST_CONTENTS_3);
	ret = write(fd1, str, len);
	if (ret != len) {
		printf("Write /w fd1 Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd2);
		return ret;
	}
	printf("fd1 = %d fd2 = %d\n", fd1, fd2);
	close(fd1);
	close(fd2);

	fd2 = open(filename2, O_RDONLY);
	if (fd2 < 0) {
		printf("open Failed : %s\n", filename2);
		g_tc_fail_count++;
		return fd2;
	}

	ret = read(fd2, buf, len);
	if (ret <= 0 || strcmp(buf, VFS_TEST_CONTENTS_3) != 0) {
		printf("Read Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd2);
		return ret;
	}
	printf("Read : %s\n", buf);
	close(fd2);
	printf("PASS!\n");
	return ret;
}

static int fs_vfs_fsync_tc(void)
{
	int ret, fd, len;
	char *filename = VFS_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_2;
	char buf[20];

	printf("%d. fsync Test started. \n", g_tc_count++);

	fd = open(filename, O_WROK | O_TRUNC);
	if (fd < 0) {
		printf("open Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd;
	}

	len = strlen(str);
	ret = write(fd, str, len);
	if (ret != len) {
		printf("Write Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd);
		return ret;
	}
	ret = fsync(fd);
	if (ret < 0) {
		printf("fsync Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd);
		return ret;
	}
	close(fd);

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		printf("open Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd;
	}
	memset(buf, 0, sizeof(buf));
	ret = read(fd, buf, len);
	if (ret <= 0 || strcmp(buf, VFS_TEST_CONTENTS_2) != 0) {
		printf("Read Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd);
		return ret;
	}
	printf("Read : %s\n", buf);
	close(fd);
	printf("PASS!\n");
	return ret;
}

static int fs_vfs_lseek_tc(void)
{
	int ret, fd;
	char *filename = VFS_FILE_PATH;
	char buf[20];

	printf("%d. lseek Test started. \n", g_tc_count++);

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		printf("open Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd;
	}
	ret = lseek(fd, 5, SEEK_SET);
	if (ret != 5) {
		printf("lseek Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd);
		return ret;
	}
	memset(buf, 0, sizeof(buf));
	ret = read(fd, buf, sizeof(buf));
	if (ret <= 0 || strcmp(buf, "IS VFS TEST 2") != 0) {
		printf("Read Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd);
		return ret;
	}
	printf("Read : %s\n", buf);
	close(fd);
	printf("PASS!\n");
	return ret;
}

static int fs_vfs_pwrite_tc(void)
{
	int ret, fd;
	char *filename = VFS_FILE_PATH;
	char *str = "pwrite test!";

	printf("%d. pwrite Test started. \n", g_tc_count++);

	fd = open(filename, O_WRONLY | O_APPEND);
	if (fd < 0) {
		printf("open Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd;
	}
	ret = pwrite(fd, str, strlen(str), 10);
	if (ret == ERROR) {
		printf("pwrite Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd);
		return ret;
	}
	close(fd);
	printf("PASS!\n");
	return ret;
}

static int fs_vfs_pread_tc(void)
{
	int ret, fd;
	char *filename = VFS_FILE_PATH;
	char *str = "pwrite test!";
	char buf[20];

	printf("%d. pread Test started. \n", g_tc_count++);

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		printf("open Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd;
	}
	memset(buf, 0, sizeof(buf));
	ret = pread(fd, buf, 20, 10);
	if (ret < 0 || strcmp(buf, str) != 0) {
		printf("pread Failed : %d\n", ret);
		g_tc_fail_count++;
		close(fd);
		return ret;
	}
	printf("Read : %s\n", buf);
	close(fd);
	printf("PASS!\n");
	return ret;
}

static int fs_vfs_mkdir_tc(void)
{
	char filename[14];
	int i;
	size_t len;
	printf("%d. mkdir Test started. \n", g_tc_count++);
	len = strlen(VFS_FOLDER_PATH) + 3;
	/** make parent folder first **/
	if (mkdir(VFS_FOLDER_PATH, 0777) != OK) {
		printf("mkdir Failed : %s\n", VFS_FOLDER_PATH);
		g_tc_fail_count++;
		return ERROR;
	}
	for (i = 0; i < VFS_LOOP_COUNT; i++) {
		memset(filename, 0, len);
		snprintf(filename, len, "%s/%d\0", VFS_FOLDER_PATH, i);
		if (mkdir(filename, 0777) != OK) {
			printf("mkdir Failed : %s\n", filename);
			g_tc_fail_count++;
			return ERROR;
		}
	}
	printf("PASS!\n");
	return OK;
}

static int fs_vfs_opendir_tc(void)
{
	DIR *dirp;

	printf("%d. opendir Test started. \n", g_tc_count++);
	dirp = opendir(VFS_FOLDER_PATH);

	if (!dirp) {
		/* Failed to open the directory */
		printf("opendir failed %s\n", VFS_FOLDER_PATH);
		g_tc_fail_count++;
		return ERROR;
	}
	printf("PASS!\n");
	closedir(dirp);
	return OK;
}

static int fs_vfs_readdir_tc(void)
{
	int ret, count;
	DIR *dirp;
	struct dirent *dirent;
	printf("%d. readdir Test started. \n", g_tc_count++);
	dirp = opendir(VFS_FOLDER_PATH);
	if (!dirp) {
		/* Failed to open the directory */
		printf("opendir %s failed\n", VFS_FOLDER_PATH);
		g_tc_fail_count++;
		return ERROR;
	}
	count = 0;
	while (1) {
		dirent = readdir(dirp);
		if (dirent == NULL) {
			printf("readdir finished!!\n");
			break;
		}
		printf("type : %s name : %s\n", dirent->d_type == DTYPE_FILE ? "file     " : "directory", dirent->d_name);
		count++;
	}
	ret = closedir(dirp);
	if (ret != OK) {
		printf("closedir failed!! %d\n", ret);
		g_tc_fail_count++;
		return ERROR;
	}
	if (count != VFS_LOOP_COUNT) {
		printf("readdir result is wrong : %d\n", count);
		g_tc_fail_count++;
		return ERROR;
	}
	printf("PASS!\n");
	return OK;
}

static int fs_vfs_rewinddir_tc(void)
{
	int ret, count;
	DIR *dirp;
	struct dirent *dirent;
	printf("%d. rewinddir Test started. \n", g_tc_count++);
	dirp = opendir(VFS_FOLDER_PATH);
	if (!dirp) {
		/* Failed to open the directory */
		printf("opendir %s failed\n", VFS_FOLDER_PATH);
		g_tc_fail_count++;
		return ERROR;
	}
	count = 0;
	while (1) {
		dirent = readdir(dirp);
		if (dirent == NULL) {
			if (count > VFS_LOOP_COUNT) {
				printf("readdir finished!!\n");
				break;
			}
			rewinddir(dirp);
			continue;
		}
		printf("type : %s name : %s\n", dirent->d_type == DTYPE_FILE ? "file     " : "directory", dirent->d_name);
		count++;
	}
	ret = closedir(dirp);
	if (ret != OK) {
		printf("closedir failed!! %d\n", ret);
		g_tc_fail_count++;
		return ERROR;
	}
	if (count != VFS_LOOP_COUNT * 2) {
		printf("rewinddir result is wrong : %d\n", count);
		g_tc_fail_count++;
		return ERROR;
	}
	printf("PASS!\n");
	return OK;
}

static int fs_vfs_seekdir_tc(void)
{
	int ret;
	DIR *dirp;
	struct dirent *dirent;
	off_t offset;
	char filename[1];
	printf("%d. seekdir Test started. \n", g_tc_count++);
	dirp = opendir(VFS_FOLDER_PATH);
	if (!dirp) {
		/* Failed to open the directory */
		printf("opendir %s failed\n", VFS_FOLDER_PATH);
		g_tc_fail_count++;
		return ERROR;
	}
	offset = 2;
	seekdir(dirp, offset);
	dirent = readdir(dirp);
	if (dirent == NULL) {
		printf("readdir failed!!\n");
		g_tc_fail_count++;
		return ERROR;
	}
	itoa((int)offset, filename, 10);
	printf("type : %s name : %s\n", dirent->d_type == DTYPE_FILE ? "file     " : "directory", dirent->d_name);
	if ((dirent->d_type == DTYPE_DIRECTORY) && (strncmp(dirent->d_name, filename, 1) != 0)) {
		printf("read wrongly!!\n");
		g_tc_fail_count++;
		return ERROR;
	}
	ret = closedir(dirp);
	if (ret != OK) {
		printf("closedir failed!! %d\n", ret);
		g_tc_fail_count++;
		return ERROR;
	}
	printf("PASS!\n");
	return OK;
}

static int fs_libc_dirent_readdir_r_tc(void)
{
	int ret, count;
	DIR *dirp;
	struct dirent entry;
	struct dirent *result;
	printf("%d. readdir_r Test started. \n", g_tc_count++);
	dirp = opendir(VFS_FOLDER_PATH);
	if (!dirp) {
		/* Failed to open the directory */
		printf("opendir %s failed\n", VFS_FOLDER_PATH);
		g_tc_fail_count++;
		return ERROR;
	}
	count = 0;
	while (1) {
		ret = readdir_r(dirp, &entry, &result);
		if (ret != 0 || result == NULL) {
			printf("readdir_r finished!!\n");
			break;
		}
		printf("type : %s name : %s\n", result->d_type == DTYPE_FILE ? "file     " : "directory", result->d_name);
		count++;
	}
	ret = closedir(dirp);
	if (ret != OK) {
		printf("closedir failed!! %d\n", ret);
		g_tc_fail_count++;
		return ERROR;
	}
	if (count != VFS_LOOP_COUNT) {
		printf("readdir result is wrong : %d\n", count);
		g_tc_fail_count++;
		return ERROR;
	}
	printf("PASS!\n");
	return OK;
}

static int fs_libc_dirent_telldir_tc(void)
{
	DIR *dirp;
	off_t offset, res;

	printf("%d. telldir Test started. \n", g_tc_count++);
	dirp = opendir(VFS_FOLDER_PATH);
	if (!dirp) {
		/* Failed to open the directory */
		printf("opendir %s failed\n", VFS_FOLDER_PATH);
		g_tc_fail_count++;
		return ERROR;
	}
	offset = 2;
	seekdir(dirp, offset);
	res = telldir(dirp);
	printf("telldir dir position : %d\n", res);
	if (res != offset) {
		printf("telldir error!!!!\n");
		g_tc_fail_count++;
		closedir(dirp);
		return ERROR;
	}
	closedir(dirp);
	printf("PASS!\n");
	return OK;
}

static int fs_vfs_closedir_tc(void)
{
	int ret;

	DIR *dirp;

	printf("%d. closedir Test started. \n", g_tc_count++);
	dirp = opendir(VFS_FOLDER_PATH);
	if (!dirp) {
		/* Failed to open the directory */
		printf("opendir %s failed\n", VFS_FOLDER_PATH);
		g_tc_fail_count++;
		return ERROR;
	}
	ret = closedir(dirp);
	if (ret != OK) {
		printf("closedir failed!! %d\n", ret);
	}
	printf("PASS!\n");
	return OK;
}

static int fs_vfs_rmdir_tc(void)
{
	char filename[14];
	size_t len;
	int i;

	printf("%d. rmdir Test started. \n", g_tc_count++);
	len = strlen(VFS_FOLDER_PATH) + 3;

	/** remove child folders first **/
	for (i = 0; i < VFS_LOOP_COUNT; i++) {
		memset(filename, 0, len);
		snprintf(filename, len, "%s/%d\0", VFS_FOLDER_PATH, i);
		if (rmdir(filename) != OK) {
			printf("rmdir Failed : %s\n", filename);
			g_tc_fail_count++;
			return ERROR;
		}
	}
	/** now there is no entry, remove parent folder **/
	if (rmdir(VFS_FOLDER_PATH) != OK) {
		printf("rmdir Failed : %s\n", VFS_FOLDER_PATH);
		g_tc_fail_count++;
		return ERROR;
	}

	printf("PASS!\n");
	return OK;
}

static int fs_vfs_unlink_tc(void)
{
	char *filename = VFS_DUP_FILE_PATH;

	printf("%d. unlink Test started. \n", g_tc_count++);
	if (unlink(filename) != OK) {
		printf("unlink Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}
	printf("PASS!\n");
	return OK;
}

static int fs_vfs_stat_tc(void)
{
	char *filename = VFS_FILE_PATH;
	struct stat st;
	int ret;

	printf("%d. stat Test started. \n", g_tc_count++);
	ret = stat(filename, &st);
	if (ret != OK) {
		printf("stat Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	printf("File Name : %s  Mode : %d\n", filename, st.st_mode);
	printf("File Size : %d  Allocated Block : %d\n", st.st_size, st.st_blocks);
	printf("PASS!\n");
	return OK;
}

static int fs_vfs_statfs_tc(void)
{
	struct statfs fs;
	printf("%d. statfs Test started. \n", g_tc_count++);
	if (statfs(MOUNT_DIR, &fs) != OK) {
		printf("statfs Failed\n");
		g_tc_fail_count++;
		return ERROR;
	}
	printf("FS Type : %s\n", fs.f_type == SMARTFS_MAGIC ? TARGET_FS_NAME : "unknown");
	printf("Block Size : %ld\n", fs.f_bsize);
	printf("Total Blocks : %ld\n", fs.f_blocks);
	printf("Free Blocks : %ld\n", fs.f_bfree);
	printf("Free Blocks for non-super user : %ld\n", fs.f_bavail);
	printf("Total file nodes : %ld\n", fs.f_files);
	printf("Free file nodes : %ld\n", fs.f_ffree);
	printf("Max Length of file name : %ld\n", fs.f_namelen);
	printf("PASS!\n");
	return OK;
}


#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 0)
static int fs_vfs_mkfifo_tc(void)
{
	int fd, ret;
	pthread_t tid;
	int count;
	char buf[12];
	size_t size;
	printf("%d. mkfifo Test started. \n", g_tc_count++);
	g_thread_result = true;
	ret = mkfifo(FIFO_FILE_PATH, 0666);
	if (ret < 0 && ret != -EEXIST) {
		printf("mkfifo Failed : %s ret : %d\n", FIFO_FILE_PATH, ret);
		g_tc_fail_count++;
		return ERROR;
	}
	fd = open(FIFO_FILE_PATH, O_WRONLY);
	if (fd < 0) {
		printf("open Failed : %s\n", FIFO_FILE_PATH);
		g_tc_fail_count++;
		return fd;
	}
	ret = pthread_create(&tid, NULL, (pthread_startroutine_t)mkfifo_test_listener, NULL);
	if (ret != 0) {
		printf("thread create Failed : %d\n", ret);
		g_tc_fail_count++;
		return fd;
	}
	size = strlen(FIFO_DATA) + 2;
	count = 1;
	while (count < 4) {
		snprintf(buf, size, "%s%d\0", FIFO_DATA, count);
		ret = write(fd, buf, size);
		if (ret <= 0) {
			printf("Write data to fifo failed\n");
			g_tc_fail_count++;
			pthread_kill(tid, SIGUSR1);
			close(fd);
			return ERROR;
		}
		count++;
		sleep(5);
	}
	close(fd);
	pthread_kill(tid, SIGUSR1);
	if (g_thread_result) {
		printf("PASS!\n");
		return OK;
	}
	printf("mkfifo test failed!!\n");
	g_tc_fail_count++;
	return ERROR;
}
#endif

static int fs_vfs_sendfile_tc(void)
{
	char *src_file = VFS_FILE_PATH;
	char dest_file[12];
	struct stat st;
	int fd1, fd2, ret;
	printf("%d. sendfile Test started. \n", g_tc_count++);

	snprintf(dest_file, 12, "%s_dest", src_file);
	printf("[%s]\n", dest_file);

	fd1 = open(src_file, O_RDONLY);
	if (fd1 < 0) {
		printf("open Failed : %s\n", src_file);
		g_tc_fail_count++;
		return fd1;
	}

	ret = stat(src_file, &st);
	if (ret != OK) {
		printf("stat Failed : %s\n", src_file);
		g_tc_fail_count++;
		close(fd1);
		return ERROR;
	}

	fd2 = open(dest_file, O_WRONLY | O_CREAT);
	if (fd2 < 0) {
		printf("open Failed : %s\n", src_file);
		g_tc_fail_count++;
		close(fd1);
		return fd2;
	}

	if (sendfile(fd2, fd1, 0, st.st_size) < 0) {
		printf("send Failed : from %s to %s\n", src_file, dest_file);
		g_tc_fail_count++;
		close(fd1);
		close(fd2);
		return ERROR;
	}

	close(fd1);
	close(fd2);
	printf("PASS!\n");
	return OK;
}

static int fs_vfs_fcntl_tc(void)
{
	int fd, mode;
	char *filename = VFS_FILE_PATH;
	printf("%d. fcntl Test started. \n", g_tc_count++);
	fd = open(filename, O_WROK);
	if (fd < 0) {
		printf("open Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd;
	}

	mode = fcntl(fd, F_GETFL, 0) & O_ACCMODE;
	close(fd);
	if (mode != O_WROK) {
		printf("fcntl Failed : %d\n", mode);
		g_tc_fail_count++;
		return ERROR;
	}
	printf("PASS!\n");
	return OK;
}

#ifndef CONFIG_DISABLE_POLL
static int fs_vfs_poll_tc(void)
{
	struct pollfd fds[2];
	int ret;
	int timeout = 30;

	printf("%d. poll Test started. \n", g_tc_count++);

	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;

	fds[1].fd = STDOUT_FILENO;
	fds[1].events = POLLOUT;
	printf("input text : ");

	fflush(stdout);

	ret = poll(fds, 2, timeout * 1000);

	if (ret == ERROR) {
		printf("poll timeout");
		g_tc_fail_count++;
		return ERROR;
	}
	if (ret == 0) {
		printf("%d seconds elapsed. \n", timeout);
		g_tc_fail_count++;
		return ERROR;
	}

	if (fds[0].revents & POLLIN) {
		ret = OK;
		printf("stdin is readable.\n");
	}

	if (fds[1].revents & POLLOUT) {
		ret = OK;
		printf("stdout is writable.\n");
	}
	printf("PASS!\n");
	return ret;
}

static int fs_vfs_select_tc(void)
{
	struct timeval tv;
	fd_set readfds;
	int ret, c, errcnt;

	printf("%d. select Test started. \n", g_tc_count++);

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
	if (errcnt == VFS_LOOP_COUNT) {
		printf("select test failed!\n");
		g_tc_fail_count++;
		FD_CLR(STDIN_FILENO, &readfds);
		return ERROR;
	}
	printf("PASS!\n");
	return OK;
}
#endif

static int fs_vfs_rename_tc(void)
{
	int ret;
	char *filename = VFS_FILE_PATH;
	char dest_file[12];

	printf("%d. rename Test started. \n", g_tc_count++);
	snprintf(dest_file, 12, "%s_re", filename);
	unlink(dest_file);

	ret = rename(filename, dest_file);
	if (ret != OK) {
		printf("rename Failed : Original Name : %s  Target Name : %s\n", filename,
			   dest_file);
		g_tc_fail_count++;
		return ret;
	}
	printf("PASS!\n");
	return ret;
}

static int fs_vfs_ioctl_tc(void)
{
	int fd, ret;
	long size;

	printf("%d. ioctl Test started. \n", g_tc_count++);
	fd = open("/dev/console", O_RDWR);

	ret = ioctl(fd, FIONREAD, &size);
	close(fd);
	printf("size = %ld ret = %d\n", size, ret);
	printf("PASS!\n");
	return ret;
}

static int libc_stdio_fdopen_tc(void)
{
	int fd;
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	printf("%d. fdopen Test started. \n", g_tc_count++);
	fd = open(filename, O_WROK | O_CREAT);
	if (fd < 0) {
		printf("open Failed : %s\n", filename);
		g_tc_fail_count++;
		return fd;
	}

	fp = fdopen(fd, "r");
	close(fd);
	if (fp == NULL) {
		printf("fdopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	printf("PASS!\n");
	fclose(fp);
	return OK;

}

static int libc_stdio_fopen_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	printf("%d. fopen Test started. \n", g_tc_count++);
	fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	printf("PASS!\n");
	fclose(fp);
	return OK;
}

static int libc_stdio_fclose_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;

	printf("%d. fclose Test started. \n", g_tc_count++);
	fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	if (fclose(fp) != OK) {
		printf("fclose Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_fputs_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_1;
	printf("%d. fputs Test started. \n", g_tc_count++);
	fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	if (fputs(str, fp) != strlen(str)) {
		printf("fputs Failed : %s\n", filename);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}
	printf("PASS!\n");
	fclose(fp);
	return OK;
}

static int libc_stdio_fgets_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char buf[20];

	printf("%d. fgets Test started. \n", g_tc_count++);
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	memset(buf, 0, sizeof(buf));
	if (fgets(buf, 20, fp) == NULL || strcmp(buf, VFS_TEST_CONTENTS_1) != 0) {
		printf("fgets Failed : %s\n", filename);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}
	fclose(fp);
	printf("Read : %s\n", buf);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_fseek_tc(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;
	char buf[20];

	printf("%d. fseek Test started. \n", g_tc_count++);

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	ret = fseek(fp, 5, SEEK_SET);
	if (ret < 0) {
		printf("fseek Failed : %d\n", ret);
		g_tc_fail_count++;
		fclose(fp);
		return ret;
	}
	memset(buf, 0, sizeof(buf));
	if (fgets(buf, 20, fp) == NULL || strcmp(buf, "IS VFS TEST 1") != 0) {
		printf("fgets Failed : %s\n", filename);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}
	fclose(fp);
	printf("Read : %s\n", buf);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_ftell_tc(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;

	printf("%d. ftell Test started. \n", g_tc_count++);

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	ret = fseek(fp, 5, SEEK_SET);
	if (ret < 0) {
		printf("fseek Failed : %d\n", ret);
		g_tc_fail_count++;
		fclose(fp);
		return ret;
	}
	ret = ftell(fp);
	fclose(fp);
	if (ret != 5) {
		printf("ftell Failed : %d\n", ret);
		g_tc_fail_count++;
		return ret;
	}
	printf("ftell : %d\n", ret);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_feof_tc(void)
{
	FILE *fp;
	char *filename = LONG_FILE_PATH;
	char buf[32];
	printf("%d. feof Test started. \n", g_tc_count++);
	if (make_long_file() == ERROR) {
		printf("make_long_file Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}
	while (!feof(fp)) {
		fgets(buf, 32, fp);
		printf("%s", buf);
		memset(buf, 0, 32);
	}
	printf("\n");
	fclose(fp);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_fprintf_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char *str = VFS_TEST_CONTENTS_2;
	printf("%d. fprintf Test started. \n", g_tc_count++);
	fp = fopen(filename, "w+");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	if (fprintf(fp, "%s", str) < 0) {
		printf("fprintf Failed : %s\n", filename);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}
	fclose(fp);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_fsetpos_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	fpos_t pos = 3;
	int ch;

	printf("%d. fsetpos Test started. \n", g_tc_count++);
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	if (fsetpos(fp, &pos) < 0) {
		printf("fsetpos Failed : %d\n", pos);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}
	ch = fgetc(fp);
	printf("ch = %c\n", ch);
	if (ch == EOF || ch != 'S') {
		printf("fgetc Failed : %s\n", filename);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}

	fclose(fp);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_fgetpos_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	fpos_t pos = 0;
	int ch;

	printf("%d. fgetpos Test started. \n", g_tc_count++);
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}
	ch = fgetc(fp);
	if (ch == EOF) {
		printf("fgetc Failed : %s\n", filename);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}

	if (fgetpos(fp, &pos) < 0 || pos != 1) {
		printf("fgetpos Failed : %d\n", pos);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}

	fclose(fp);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_fgetc_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ch;

	printf("%d. fgetc Test started. \n", g_tc_count++);
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}
	ch = fgetc(fp);
	if (ch == EOF || ch != 'T') {
		printf("fgetc Failed : %s\n", filename);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}

	fclose(fp);
	printf("fgetc : %c\n", ch);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_fputc_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ret;

	printf("%d. fputc Test started. \n", g_tc_count++);
	fp = fopen(filename, "w+");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}
	ret = fputc('T', fp);
	if (ret < 0) {
		printf("fputc Failed : %s\n", filename);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}

	fclose(fp);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_fwrite_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char *str1 = VFS_TEST_CONTENTS_1;
	char *str2 = VFS_TEST_CONTENTS_2;
	char *str3 = VFS_TEST_CONTENTS_3;
	int len;

	printf("%d. fwrite Test started. \n", g_tc_count++);
	len = strlen(str1);
	fp = fopen(filename, "w+");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	if (fwrite(str1, 1, len, fp) != len) {
		printf("fwrite Failed(Contents1) : %s\n", filename);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}
	if (fwrite(str2, 1, len, fp) != len) {
		printf("fwrite Failed(Contents2) : %s\n", filename);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}
	if (fwrite(str3, 1, len, fp) != len) {
		printf("fwrite Failed(Contents3) : %s\n", filename);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}

	fclose(fp);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_fread_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	char buf[20];
	int len, size;
	printf("%d. fread Test started. \n", g_tc_count++);
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	len = strlen(VFS_TEST_CONTENTS_1);
	size = sizeof(buf);
	memset(buf, 0, size);
	if (fread(buf, 1, len, fp) != len || strcmp(buf, VFS_TEST_CONTENTS_1)) {
		printf("fread #1 Failed : %s\n", buf);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}
	printf("1st Line = %s\n", buf);

	memset(buf, 0, size);
	if (fread(buf, 1, len, fp) != len || strcmp(buf, VFS_TEST_CONTENTS_2)) {
		printf("fread #2 Failed : %s\n", buf);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}
	printf("2nd Line = %s\n", buf);

	memset(buf, 0, size);
	if (fread(buf, 1, len, fp) != len || strcmp(buf, VFS_TEST_CONTENTS_3)) {
		printf("fread #3 Failed : %s\n", buf);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}
	printf("3rd Line = %s\n", buf);

	fclose(fp);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_ferror_tc(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;

	printf("%d. ferror Test started. \n", g_tc_count++);

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}
	ret = fputc(32, fp);

	if (ret != EOF) {
		printf("fputc Failed : %d\n", ret);
		g_tc_fail_count++;
		fclose(fp);
		return ret;
	}
	ret = ferror(fp);
	fclose(fp);
	if (ret == OK) {
		printf("ferror Failed : %d\n", ret);
		g_tc_fail_count++;
		return ret;
	}

	printf("ferror : %d\n", ret);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_clearerr_tc(void)
{
	FILE *fp;
	int ret;
	char *filename = VFS_FILE_PATH;

	printf("%d. clearerr Test started. \n", g_tc_count++);

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}
	ret = fputc(32, fp);

	if (ret != EOF) {
		printf("fputc Failed : %d\n", ret);
		g_tc_fail_count++;
		fclose(fp);
		return ret;
	}
	ret = ferror(fp);

	if (ret == OK) {
		printf("ferror Failed : %d\n", ret);
		g_tc_fail_count++;
		fclose(fp);
		return ret;
	}
	clearerr(fp);

	ret = ferror(fp);
	if (ret != OK) {
		printf("clearerr Failed : %d\n", ret);
		g_tc_fail_count++;
		fclose(fp);
		return ret;
	}

	fclose(fp);
	printf("clearerr : %d\n", ret);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_gets_tc(void)
{
	char input_str[64];
	printf("%d. gets Test started. \n", g_tc_count++);
	printf("Enter text here : ");
	fflush(stdout);
	if (gets(input_str) == NULL) {
		printf("gets Failed\n");
		g_tc_fail_count++;
		return ERROR;
	}
	printf("gets : %s\n", input_str);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_gets_s_tc(void)
{
	char input_str[64];
	printf("%d. gets_s Test started. \n", g_tc_count++);
	printf("Enter text here : ");
	fflush(stdout);
	if (gets_s(input_str, sizeof(input_str)) == NULL) {
		printf("gets_s Failed\n");
		g_tc_fail_count++;
		return ERROR;
	}

	printf("gets_s : %s\n", input_str);
	printf("PASS!\n");
	return OK;
}

static int libc_stdio_fileno_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int handle;

	printf("%d. fileno Test started. \n", g_tc_count++);

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}

	handle = fileno(fp);

	if (handle < 0) {
		printf("fileno Failed : %d\n", handle);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}

	printf("handle number: %d\n", handle);
	fclose(fp);
	printf("PASS!\n");
	return 0;
}

static int libc_stdio_ungetc_tc(void)
{
	FILE *fp;
	char *filename = VFS_FILE_PATH;
	int ret;
	int ch1, ch2;

	printf("%d. ungetc Test started. \n", g_tc_count++);

	fp = fopen(filename, "w+");
	if (fp == NULL) {
		printf("First fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;
	}
	ret = fputc(32, fp);
	if (ret == EOF) {
		printf("fputc Failed : %c\n", 32);
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}
	fclose(fp);

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Second fopen Failed : %s\n", filename);
		g_tc_fail_count++;
		return ERROR;

	}

	ch1 = fgetc(fp);
	if (ch1 == EOF) {
		printf("First fgetc Failed\n");
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}

	if (ungetc(64, fp) == EOF) {
		printf("ungetc Failed\n");
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}

	ch2 = fgetc(fp);
	if (ch2 == EOF) {
		printf("Second fgetc Failed\n");
		g_tc_fail_count++;
		fclose(fp);
		return ERROR;
	}
	fclose(fp);

	if (ch1 == ch2) {
		printf("ungetc Failed ch1 : %d  ch2 : %d\n", ch1, ch2);
		g_tc_fail_count++;
		return ERROR;
	}
	printf("ch1 : %d  ch2 : %d\n", ch1, ch2);
	printf("PASS!\n");
	return OK;
}

static int fs_sample_launcher(int argc, char **args)
{
	g_tc_count = 1;
	g_tc_fail_count = 0;

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
#if defined(CONFIG_PIPES) && (CONFIG_DEV_PIPE_SIZE > 0)
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
	printf("           Total TC : %d              \n", --g_tc_count);
	printf("           PASS : %d FAIL : %d        \n",
		   (g_tc_count - g_tc_fail_count), g_tc_fail_count);
	printf("#########################################\n");
	return g_tc_count;
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


