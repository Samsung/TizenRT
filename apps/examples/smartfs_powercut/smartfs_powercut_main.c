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

/* TODO More names for text based file */
#define TEST_SMALL              "/mnt/test_small"
#define TEST_SMALL_BACKUP       "/mnt/backup_small/test_small"
#define TEST_SMALL_BACKUP_DIR   "/mnt/backup_small"

#define TEST_LARGE              "/mnt/test_large"
#define TEST_LARGE_BACKUP       "/mnt/backup_large/test_large"
#define TEST_LARGE_BACKUP_DIR   "/mnt/backup_large"


#define TEST_LARGE_BUFSIZE      8192
int TEST_SMALL_BUFSIZE;

/* Write Time for each file */
#define TEST_WRITECOUNT         10

/* File is not exist, so no verify & recovery needed, we can rollback backup file */
#define FILE_CHUNK_NO_INDEX     -1

struct buffer_data_s {
	uint16_t crc;
	char data[TEST_LARGE_BUFSIZE];
};

enum file_type_e {
	FILE_TYPE_RANDOM = 0,
	FILE_TYPE_TEXT = 1
};
typedef enum file_type_e file_type_t;

static struct buffer_data_s g_buf;

void make_random_chars(char *buf, size_t len)
{
	for (int i = 0; i < len - 1; i++) {
		buf[i] = rand() % len;
	}
	buf[len - 1] = '\0';
}

static int write_random_data_file(int fd, size_t bufsize)
{
	int ret;
	int data_len = bufsize - sizeof(g_buf.crc);

	memset(g_buf.data, 0xff, data_len);
	make_random_chars(g_buf.data, data_len);

	g_buf.crc = crc16((const uint8_t *)g_buf.data, data_len);

	ret = write(fd, (void *)&g_buf, bufsize);
	if (ret != bufsize) {
		printf("write_random_data_file error ret : %d errno : %d\n", ret, errno);
		return ERROR;
	}

	return OK;
}

/* small file uses buffer size same as stat.st_blksize */
static int create_file(char *path, ssize_t bufsize, file_type_t type)
{
	int fd;
	int ret;
	struct stat st;

	/* If file is already exist and written fully, just return OK */
	if (stat(path, &st) == OK) {
		printf("File already exist path : %s size : %d\n", path, st.st_size);
		return OK;

	}
	
	fd = open(path, O_CREAT | O_WROK);
	if (fd < 0) {
		printf("create file %s error errno : %d\n", path, errno);
		return ERROR;
	}
	
	for (int i = 0; i < TEST_WRITECOUNT; i++) {
		/* write data based on type */
		if (type == FILE_TYPE_RANDOM) {
			ret = write_random_data_file(fd, bufsize);
		} else {
			/* TODO proper text will be added here */
		}
		
		if (ret != OK) {
			printf("write failed path : %s ret : %d bufsize : %d\n", path, ret, bufsize);
			goto error_with_fd;
		}
	}

	ret = OK;
	
error_with_fd:
	close(fd);
	printf("create file path : %s ret : %d\n", path, ret);
	return ret;
}

static int verify_random_file(char *path, int bufsize, int *index)
{
	int fd;
	int ret;
	ssize_t len;
	off_t size;
	int i;
	uint16_t crc;
	struct stat st;
	int data_len = bufsize - sizeof(g_buf.crc);
	
	fd = open(path, O_RDWR);
	if (fd < 0) {
		if (errno == ENOENT) {
			printf("Verify_file File is not exist, no need to verify!! path : %s\n", path);
			*index = FILE_CHUNK_NO_INDEX;
			return OK;
		}
		
		printf("verify_file %s open error... errno : %d\n", path, errno);
		return ERROR;
	}

	for (i = 0; i < TEST_WRITECOUNT; i++) {
		len = read(fd, (void *)&g_buf, bufsize);
		if (len != bufsize) {
			/* If write is not finished properly */
			printf("verify_file datachunk is not written fully..  path : %s ret : %d index : %d\n", path, len, i);
			break;
		} else {
			/* Maybe this won't be happened but check crc for double check */
			crc = crc16((const uint8_t *)g_buf.data, data_len);
			if (crc != g_buf.crc) {
				printf("verify_file crc mismatch!! path : %s calulcated crc : %d written crc : %d index : %d\n", path, crc, g_buf.crc, i);
				break;
			}
		}
	}

	*index = i;

	/* shrink to last valid index here, it is necessary to bury invalid data chunk */
	if ((i > 0) && (i < TEST_WRITECOUNT)) {
		/* truncate current file to include valid data only */
		size = i * bufsize;
		
		ret = lseek(fd, 0, SEEK_SET);
		if (ret != OK) {
			printf("verify_file, seek to 0 failed\n");
			ret = ERROR;
			goto error_with_fd;
		}

		ret = ftruncate(fd, size);
		if (ret != OK) {
			printf("Shrink failed filename : %s target size : %d errno : %d\n", path, size, errno);
			ret = ERROR;
			goto error_with_fd;
		}

		ret = stat(path, &st);
		if (ret == OK) {
			if (st.st_size != size) {
				printf("Shrunk size is wrong. filename : %s target size : %d file size : %d\n", path, size, st.st_size);
				ret = ERROR;
				goto error_with_fd;
			}
			printf("shrink works properly, target size : %d current size : %d\n", size, st.st_size);
		} else {
			printf("stat error. path : %d errno : %d\n", path, errno);
			ret = ERROR;
			goto error_with_fd;
		}
	}

	ret = OK;

error_with_fd:
	close(fd);
	return ret;
}

static int verify_file(char *path, int bufsize, int *idx, file_type_t type)
{
	if (type == FILE_TYPE_RANDOM) {
		return verify_random_file(path, bufsize, idx);
	}
	/* TODO verify proper text type */
	return OK;
}

static int recovery_random_file(char *src, char* backup, char* backup_dir, int bufsize, int idx)
{
	int fd1;
	int fd2;
	int ret;
	struct stat st;
	off_t offset;
	off_t start_offset;
	int filesize;
	
	printf("recovery_file src : %s idx : %d\n", src, idx);
	/* If index is FILE_CHUNK_NO_INDEX, There is no tested file, so just rollback backup file
	 * If index is 0, we should recovery whole of data from back up file, so just rollback it.
	 * If idnex is same as _TEST_WRITECOUNT, totally written, so no recovery necessary.
	 */
	if (idx == FILE_CHUNK_NO_INDEX || idx == 0 || idx == TEST_WRITECOUNT) {
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

		/* To test append, do not extend as a MAX size of file */
		if (idx < (TEST_WRITECOUNT - 2)) {
			filesize = (TEST_WRITECOUNT - 2) * bufsize;
		} else {
			filesize = idx * bufsize;
		}
		
		/* Extend last file, it's not common usage but add it to test ftruncate(extend) */
		ret = ftruncate(fd1, filesize);
		if (ret != OK) {
			printf("ftruncate(extend)error filename : %s target size : %d errno : %d\n", src, filesize, errno);
			ret = ERROR;
			goto error_with_fd1;
		}
		
		if (stat(src, &st) == OK) {
			if (st.st_size != filesize) {
				printf("size is wrong. filename : %s target size : %d file size : %d\n", src, filesize, st.st_size);
				ret = ERROR;
				goto error_with_fd1;
			}
			printf("extend works properly, target size : %d current size : %d\n", filesize, st.st_size);
		} else {
			printf("stat error. path : %d errno : %d\n", src, errno);
			ret = ERROR;
			goto error_with_fd1;
		}

		fd2 = open(backup, O_RDOK);
		if (fd2 < 0) {
			printf("%s open error... errno : %d\n", backup, errno);
			ret = ERROR;
			goto error_with_fd1;
		}
		
		start_offset = idx * bufsize;
		offset = lseek(fd1, start_offset, SEEK_SET);
		if (offset != start_offset) {
			printf("lseek error file : %s offset : %d errno : %d\n", src, offset, errno);
			ret = ERROR;
			goto error_with_fd2;
		}

		offset = lseek(fd2, start_offset, SEEK_SET);
		if (offset != start_offset) {
			printf("lseek error file : %s offset : %d errno : %d\n", backup, offset, errno);
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
		ret = unlink(backup);
		if (ret != OK) {
			printf("unlink %s failed errno : %d\n", backup, errno);
			return ret;
		
		}
	}
	
	ret = rmdir(backup_dir);
	if (ret != OK) {
		printf("rmdir %s failed errno : %d\n", backup_dir, errno);
		return ret;
	}

	return OK;
		
error_with_fd2:
	close(fd2);
error_with_fd1:
	close(fd1);
	return ret;
}

static int recovery_file(char *src, char* backup, char* backup_dir, int bufsize, int idx, file_type_t type)
{
	if (type == FILE_TYPE_RANDOM) {
		return recovery_random_file(src, backup, backup_dir, bufsize, idx);
	}
	/* TODO recovery proper text type..maybe it will memcpy from text. */
	return OK;
}

int init_test_file(void)
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

	ret = create_file(TEST_SMALL, TEST_SMALL_BUFSIZE, FILE_TYPE_RANDOM);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	ret = create_file(TEST_SMALL_BACKUP, TEST_SMALL_BUFSIZE, FILE_TYPE_RANDOM);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	ret = mkdir(TEST_LARGE_BACKUP_DIR, 0777);
	if (ret != OK && errno != EEXIST) {
		printf("init_test_file create backup dir for large file failed ret : %d errno : %d\n", ret, errno);
		return ERROR;
	}

	ret = create_file(TEST_LARGE, TEST_LARGE_BUFSIZE, FILE_TYPE_RANDOM);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	ret = create_file(TEST_LARGE_BACKUP, TEST_LARGE_BUFSIZE, FILE_TYPE_RANDOM);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}
	printf("create test file success!!\n");
	return OK;
}

static int do_test(char *src, char *backup, char *backupdir, int bufsize, file_type_t type)
{
	int ret;
	int index;
	struct stat st;
	
	ret = verify_file(src, bufsize, &index, type);
	if (ret == ERROR) {
		return ret;
	}

	ret = recovery_file(src, backup, backupdir, bufsize, index, type);
	if (ret != OK) {
		printf("recovery_file failed file : %s\n", src);
		return ERROR;
	} 

	printf("do_test create backupdir : %s\n", backupdir);
	ret = mkdir(backupdir, 0777);
	if (ret != OK) {
		printf("Test loop, create backup dir for file failed dir : %s errno : %d\n", backupdir, errno);
		return ERROR;
	}

	stat(src, &st);
	printf("make backup.. origin file : %s size : %d\n", src, st.st_size);
	ret = rename(src, backup);
	if (ret != OK) {
		printf("make back up file failed src : %s backup : %s errno : %d\n", src, backup, errno);
		return ERROR;
	}
	
	printf("make backup ended, crate new file : %s\n", src);
	ret = create_file(src, bufsize, type);
	if (ret != OK) {
		return ERROR;
	}
	printf("do_test Success!! file : %s\n", src);
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
		 * during the verify for small file test because it protected by journal.
		 */
		ret = do_test(TEST_SMALL, TEST_SMALL_BACKUP, TEST_SMALL_BACKUP_DIR, TEST_SMALL_BUFSIZE, FILE_TYPE_RANDOM);
		if (ret != OK) {
			printf("test small file failed\n");
			return ret;
		}

		ret = do_test(TEST_LARGE, TEST_LARGE_BACKUP, TEST_LARGE_BACKUP_DIR, TEST_LARGE_BUFSIZE, FILE_TYPE_RANDOM);
		if (ret != OK) {
			printf("test large file failed\n");
			return ret;
		}
	}
	
	return OK;
}
