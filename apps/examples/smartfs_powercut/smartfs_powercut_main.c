/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * examples/smartfs_powercut/smartfs_powercut_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <crc16.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <tinyara/fs/ioctl.h>
#include <tinyara/config.h>

/* Below paths are for test file which use small size of buffer(same as st_blksize) of stat */
#define TEST_SMALL              "/mnt/test_small"
#define TEST_SMALL_BACKUP       "/mnt/backup_small/test_small"
#define TEST_SMALL_BACKUP_DIR   "/mnt/backup_small"

#define TEST_LARGE              "/mnt/test_large"
#define TEST_LARGE_BACKUP       "/mnt/backup_large/test_large"
#define TEST_LARGE_BACKUP_DIR   "/mnt/backup_large"


#define TEST_LARGE_BUFSIZE      8192
int TEST_SMALL_BUFSIZE;

/* Write Time for each file */
#define TEST_WRITECOUNT         30

struct buffer_data_s {
	uint16_t crc;
	char data[TEST_LARGE_BUFSIZE];
};

static struct buffer_data_s g_buf;

void make_random_chars(char *buf, size_t len)
{
	for (int i = 0; i < len - 1; i++) {
		buf[i] = rand() % len;
	}
	buf[len - 1] = '\0';
}

static int write_file(int fd, size_t bufsize)
{
	int ret;
	int data_len = bufsize - sizeof(g_buf.crc);

	memset(g_buf.data, 0xff, data_len);
	make_random_chars(g_buf.data, data_len);

	g_buf.crc = crc16((const uint8_t *)g_buf.data, data_len);

	ret = write(fd, (void *)&g_buf, bufsize);
	if (ret != bufsize) {
		printf("write_file error ret : %d errno : %d\n", ret, errno);
		return ERROR;
	}

	return ret;
}

/* small file uses buffer size same as stat.st_blksize */
int create_file(char *path, ssize_t bufsize)
{
	int fd;
	int len;
	int total_len = 0;
	struct stat st;
	if (stat(path, &st) == OK) {
		printf("path %s already exist size : %d\n", path, st.st_size);
		return OK;
	}
			
	fd = open(path, O_CREAT | O_EXCL | O_WROK);
	if (fd < 0) {
		printf("create file %s error errno : %d\n", path, errno);
		return ERROR;
	}
	
	for (int i = 0; i < TEST_WRITECOUNT; i++) {
		len = write_file(fd, bufsize);
		if (len != bufsize) {
			printf("write failed len : %d bufsize : %d\n", len, bufsize);
			break;
		}
		total_len += len;
	}
	
	close(fd);

	if (total_len == bufsize * TEST_WRITECOUNT) {
		return OK;
	}
	
	printf("create file %s failed... total_len : %d\n", path, total_len);
	return ERROR;
}

int verify_file(char *path, int bufsize)
{
	int fd;
	int ret;
	int i;
	uint16_t crc;
	int data_len = bufsize - sizeof(g_buf.crc);
	
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		printf("verify_file %s open error... errno : %d\n", path, errno);
		return ERROR;
	}

	for (i = 0; i < TEST_WRITECOUNT; i++) {
		ret = read(fd, (void *)&g_buf, bufsize);
		if (ret != bufsize) {
			printf("verify_file %s data is not written properly.. ret : %d current read count : %d\n", path, ret, i);
			break;
		} else {
			crc = crc16((const uint8_t *)g_buf.data, data_len);
			if (crc != g_buf.crc) {
				printf("verify_file %s crc mismatch!! calulcated crc : %d written crc : %d read count : %d\n", path, crc, g_buf.crc, i);
				break;
			}
		}
	}
	close(fd);

	printf("%s last valid write count : %d\n", path, i);
	return i;
}

int recovery_file(char *src, char* backup, char* backup_dir, int bufsize, int idx)
{
	int fd1;
	int fd2;
	int ret;
	off_t offset;
	off_t start_offset;
	
	/* If idx is ERROR, it means it didn't create src file so bring backup file */
	printf("recovery_file src : %s idx : %d\n", src, idx);
	if (idx == ERROR || idx == 0 || idx == TEST_WRITECOUNT) {
		if (idx == 0 || idx == TEST_WRITECOUNT) {
			/* This means that file has created but there's nothing valid data or finished properly..
			 * so unlink src and then rename 
			 */
			ret = unlink(src);
			if (ret != OK) {
				printf("unlink %s failed errno : %d\n", src, errno);
				return ERROR;
			}
		}
		ret = rename(backup, src);
		if (ret != OK) {
			printf("rename failed origin : %s target : %s errno : %d idx : %d\n", backup, src, errno, idx);
			return ERROR;
		}

	} else {
		fd1 = open(src, O_WROK);
		if (fd1 < 0) {
			printf("%s open error... errno : %d\n", src, errno);
			return ERROR;
		}

		fd2 = open(backup, O_RDOK);
		if (fd2 < 0) {
			printf("%s open error... errno : %d\n", backup, errno);
			ret = ERROR;
			goto error_with_fd1;
		}
		start_offset = idx * bufsize;
		offset = lseek(fd1, start_offset, SEEK_SET);
		printf("recovery, start offset : %d\n", start_offset);
		if (offset != start_offset) {
			printf("lseek file %s error offset : %d errno : %d\n", src, offset, errno);
			ret = ERROR;
			goto error_with_fd2;
		}
		
		offset = lseek(fd2, start_offset, SEEK_SET);
		if (offset != start_offset) {
			printf("lseek file %s error offset : %d errno : %d\n", backup, offset, errno);
			ret = ERROR;
			goto error_with_fd2;
		}

		for (int i = idx; i < TEST_WRITECOUNT; i++) {
			ret = read(fd2, (void *)&g_buf, bufsize);
			if (ret != bufsize) {
				printf("recovery_file read failed ret : %d errno : %d\n", ret, errno);
				ret = ERROR;
				goto error_with_fd2;
			}

			ret = write(fd1, (void *)&g_buf, bufsize);
			if (ret != bufsize) {
				printf("recovery_file write failed ret : %d errno : %d\n", ret, errno);
				ret = ERROR;
				goto error_with_fd2;
			}
		}
		close(fd1);
		close(fd2);
		
		/* Now unlink & remove dir of backup */
		printf("unlink backup : %s\n", backup);
		ret = unlink(backup);
		if (ret != OK) {
			printf("unlink %s failed errno : %d\n", backup, errno);
			ret = ERROR;
		
		}
	}
	
	printf("remove backup dir : %s\n", backup_dir); 
	ret = rmdir(backup_dir);
	if (ret != OK) {
		printf("rmdir %s failed errno : %d\n", backup_dir, errno);
		ret = ERROR;
	}

	return OK;
		
error_with_fd2:
	close(fd2);
	
error_with_fd1:
	close(fd1);
	return ret;
}

int init_test_file()
{
	struct stat st;
	int ret;
	ret = stat("/mnt", &st);
	if (ret != OK) {
		printf("stat error... errno : %d\n", errno);
		return ERROR;
	}
	
	TEST_SMALL_BUFSIZE = st.st_blksize;
	
	ret = mkdir(TEST_SMALL_BACKUP_DIR, 0777);
	if (ret != OK && errno != EEXIST) {
		printf("init_test_file create backup dir for small file failed errno : %d\n", errno);
		return ERROR;
	}

	ret = create_file(TEST_SMALL, TEST_SMALL_BUFSIZE);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	ret = create_file(TEST_SMALL_BACKUP, TEST_SMALL_BUFSIZE);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	ret = mkdir(TEST_LARGE_BACKUP_DIR, 0777);
	if (ret != OK && errno != EEXIST) {
		printf("init_test_file create backup dir for large file failed ret : %d errno : %d\n", ret, errno);
		return ERROR;
	}

	ret = create_file(TEST_LARGE, TEST_LARGE_BUFSIZE);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	ret = create_file(TEST_LARGE_BACKUP, TEST_LARGE_BUFSIZE);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}
	printf("create test file success!!\n");
	return OK;
}

static int do_test(char *src, char *backup, char *backupdir, int bufsize)
{
	int ret;
	
	ret = verify_file(src, bufsize);
	if (ret != TEST_WRITECOUNT) {
		ret = recovery_file(src, backup, backupdir, bufsize, ret);
		if (ret != OK) {
			printf("recovery_file %s failed\n", src);
			return ERROR;
		}
	} else {
		printf("verify file %s success!! clean backup files\n", src);
		ret = unlink(backup);
		if (ret != OK) {
			printf("unlink %s failed errno : %d\n", backup, errno);
			ret = ERROR;
		}

		ret = rmdir(backupdir);
		if (ret != OK) {
			printf("rmdir %s failed errno : %d\n", backupdir, errno);
			ret = ERROR;
		}
	}

	printf("do_test create backupdir : %s\n", backupdir);
	ret = mkdir(backupdir, 0777);
	if (ret != OK) {
		printf("Test loop, create backup dir for file %s failed errno : %d\n", src, errno);
		return ERROR;
	}
	struct stat st;
	stat(src, &st);
	printf("make backup.. origin file : %s size : %d\n", src, st.st_size);
	ret = rename(src, backup);
	if (ret != OK) {
		printf("make back up file %s failed errno : %d\n", src, errno);
		return ERROR;
	}
	printf("make backup ended, crate new file : %s\n", src);
	ret = create_file(src, bufsize);
	if (ret != OK) {
		return ERROR;
	}
	printf("do_test file : %s success!!\n", src);
	return OK;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int smartfs_powercut_main(int argc, char *argv[])
#endif
{
	int ret;
	printf("Init test to create test files\n");
	ret = init_test_file();

	if (ret != OK) {
		printf("init_test_file failed\n");
		return ERROR;
	}
	printf("################### Init Finished, Test will started after 3sec ###################\n");
	sleep(3);
	while (1) {
		/* Small test use same as size of data area of mtd sector, so there should not be error
		 * during the verify for small file test becuase it protected by journal.
		 */
		ret = do_test(TEST_SMALL, TEST_SMALL_BACKUP, TEST_SMALL_BACKUP_DIR, TEST_SMALL_BUFSIZE);
		if (ret != OK) {
			printf("test small file failed\n");
			break;
		}

		ret = do_test(TEST_LARGE, TEST_LARGE_BACKUP, TEST_LARGE_BACKUP_DIR, TEST_LARGE_BUFSIZE);
		if (ret != OK) {
			printf("test large file failed\n");
			break;
		}
	}
	
	return ret;
}
