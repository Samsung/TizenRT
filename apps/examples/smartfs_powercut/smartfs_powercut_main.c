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

/* File and Directory paths for testing */
/* Proper text files */
#define TEST_SMALL_T            "/mnt/small_text.txt"
#define TEST_SMALL_T_BACKUP     "/mnt/backup_small_text/small_text.txt"
#define TEST_LARGE_T            "/mnt/large_text.txt"
#define TEST_LARGE_T_BACKUP     "/mnt/backup_large_text/large_text.txt"

/* Random generated character files */
#define TEST_SMALL_R            "/mnt/small_random"
#define TEST_SMALL_R_BACKUP     "/mnt/backup_small_random/small_random"
#define TEST_LARGE_R            "/mnt/large_random"
#define TEST_LARGE_R_BACKUP     "/mnt/backup_large_random/large_random"

/* Sample Text file for testing, to be flashed to the board by the user */
#define SAMPLE_TXT_FILE         "/mnt/sample_file.txt"

/* Backup directoires for test files */
#define TEST_SMALL_R_BACKUP_DIR   "/mnt/backup_small_random"
#define TEST_LARGE_R_BACKUP_DIR   "/mnt/backup_large_random"
#define TEST_SMALL_T_BACKUP_DIR   "/mnt/backup_small_text"
#define TEST_LARGE_T_BACKUP_DIR   "/mnt/backup_large_text"

/* 2 buffer sizes for testing */
#define TEST_LARGE_BUFSIZE      8192
int TEST_SMALL_BUFSIZE;

/* No. of times file is written */
#define TEST_WRITECOUNT         10

/* File does not exist, verification & recovery not needed, we can rollback to backup file */
#define FILE_CHUNK_NO_INDEX     -1

struct buffer_data_s {
	uint16_t crc;
	char data[TEST_LARGE_BUFSIZE];
};
static struct buffer_data_s g_buf;

enum file_type_e {
	FILE_TYPE_RANDOM = 0,
	FILE_TYPE_TEXT = 1
};
typedef enum file_type_e file_type_t;

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

	for (int i = 0; i < TEST_WRITECOUNT; i++) {
		memset(g_buf.data, 0xff, data_len);
		make_random_chars(g_buf.data, data_len);

		g_buf.crc = crc16((const uint8_t *)g_buf.data, data_len);

		ret = write(fd, (void *)&g_buf, bufsize);
		if (ret != bufsize) {
			printf("write_random_data_file error ret : %d errno : %d\n", ret, errno);
			return ERROR;
		}
	}

	return OK;
}

static int write_text_file(int fd, size_t bufsize)
{
	int ret;
	int fd_sample;

	fd_sample = open(SAMPLE_TXT_FILE, O_RDOK);
	if (fd_sample < 0) {
		printf("Unable to open file: %s, errno: %d\n", SAMPLE_TXT_FILE, errno);
		return ERROR;
	}

	/* Read text from sample text file and write to text file for testing */
	for (int i = 0; i < TEST_WRITECOUNT; i++) {
		ret = read(fd_sample, (void *)&g_buf.data, bufsize);
		if (ret != bufsize) {
			printf("Unable to read file %s, ret : %d, errno : %d\n", SAMPLE_TXT_FILE, ret, errno);
			ret = ERROR;
			goto error_with_fd_sample;
		}

		ret = write(fd, (void *)&g_buf.data, bufsize);
		if (ret != bufsize) {
			printf("Unable to write to text file for testing, ret : %d, errno : %d\n", ret, errno);
			ret = ERROR;
			goto error_with_fd_sample;
		}
	}

	ret = OK;
error_with_fd_sample:
	close(fd_sample);
	return ret;
}

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
		printf("create_file %s error errno : %d\n", path, errno);
		return ERROR;
	}

	/* write data based on type */
	if (type == FILE_TYPE_RANDOM) {
		ret = write_random_data_file(fd, bufsize);
	} else {
		ret = write_text_file(fd, bufsize);
	}
		
	if (ret != OK) {
		printf("write failed path : %s ret : %d bufsize : %d\n", path, ret, bufsize);
		goto error_with_fd;
	}
	ret = OK;
	
error_with_fd:
	close(fd);
	printf("create file path : %s ret : %d\n", path, ret);
	return ret;
}

static int create_sample_text_file(void)
{
	int ret;
	int fd_sample;
	struct stat st;

	/* Check if the sample text file already exists */
	if (stat(SAMPLE_TXT_FILE, &st) == OK) {
		if (st.st_size == (TEST_WRITECOUNT * TEST_LARGE_BUFSIZE)) {
			printf("Sample text file already exists\n");
			return OK;
		}
		ret = unlink(SAMPLE_TXT_FILE);
		if (ret != OK) {
			printf("Failed to unlink old sample file %s, ret : %d, errno : %d\n", SAMPLE_TXT_FILE, ret, errno);
			return ERROR;
		}
	}	

	fd_sample = open(SAMPLE_TXT_FILE, O_CREAT | O_WROK);
	if (fd_sample < 0) {
		printf("Unable to create sample file: %s, errno: %d\n", SAMPLE_TXT_FILE, errno);
		ret = ERROR;
		goto error_with_fd_sample;
	}

	for (int i = 0; i < TEST_WRITECOUNT; i++) {
		memset(g_buf.data, 0xff, TEST_LARGE_BUFSIZE);
		make_random_chars(g_buf.data, TEST_LARGE_BUFSIZE);

		ret = write(fd_sample, (void *)g_buf.data, TEST_LARGE_BUFSIZE);
		if (ret != TEST_LARGE_BUFSIZE) {
			printf("Unable to write to file: %s, ret: %d\n", SAMPLE_TXT_FILE, ret);
			ret = ERROR;
			goto error_with_fd_sample;
		}
	}

	ret = OK;
error_with_fd_sample:
	close(fd_sample);
	return ret;
}

static int verify_file(char *path, int bufsize, int *index, file_type_t type)
{
	int fd;
	int fd_sample = 0;
	int ret;
	off_t size;
	int i;
	struct stat st;
	uint16_t crc1;
	uint16_t crc2;
	
	fd = open(path, O_RDWR);
	if (fd < 0) {
		if (errno == ENOENT) {
			printf("File to be verified does not exist, no need to verify!! path : %s\n", path);
			*index = FILE_CHUNK_NO_INDEX;
			return OK;
		}
		
		printf("Unable to open file : %s for verifying, errno : %d\n", path, errno);
		return ERROR;
	}

	if (type == FILE_TYPE_TEXT) {
		fd_sample = open(SAMPLE_TXT_FILE, O_RDOK);
		if (fd_sample < 0) {
			printf("Unable to open Sample Text File : %s, errno : %d\n", SAMPLE_TXT_FILE, errno);
			ret = ERROR;
			goto error_with_fd;
		}
	}

	for (i = 0; i < TEST_WRITECOUNT; i++) {
		if (type == FILE_TYPE_TEXT) {
			ret = read(fd, (void *)g_buf.data, bufsize);
			if (ret != bufsize) {
				printf("Unable to read file: %s, ret: %d, i: %d\n", path, ret, i);
				break;
			}
			crc1 = crc16((const uint8_t *)g_buf.data, bufsize);

			ret = read(fd_sample, (void *)g_buf.data, bufsize);
			if (ret != bufsize) {
				printf("Failed to read sample file: %s, ret: %d, i: %d\n", path, ret, i);
				ret = ERROR;
				goto error_with_fd_sample;
			}
			crc2 = crc16((const uint8_t *)g_buf.data, bufsize);
		}  else {
			ret = read(fd, (void *)&g_buf, bufsize);
			if (ret != bufsize) {
				printf("Unable to read file: %s, ret: %d, i: %d\n", path, ret, i);
				break;
			}
			crc1 = crc16((const uint8_t *)g_buf.data, (bufsize - sizeof(g_buf.crc)));
			crc2 = g_buf.crc;
		}

		if (crc1 != crc2) {
			printf("Verify file : %s failed at index %d, crc1 : %d, crc2 : %d\n", path, i, crc1, crc2);
			break;
		}
	}

	if (type == FILE_TYPE_TEXT) {
		close(fd_sample);
	}
	*index = i;
	/* Shrink file to last valid index here, it is necessary to bury invalid data chunk */
	if ((i > 0) && (i < TEST_WRITECOUNT)) {
		/* truncate current file to include valid data only */
		size = i * bufsize;
		
		ret = lseek(fd, 0, SEEK_SET);
		if (ret != 0) {
			printf("Verify_file : %s, seek to 0 failed, ret : %d, errno : %d\n", path, ret, errno);
			ret = ERROR;
			goto error_with_fd;
		}

		ret = ftruncate(fd, size);
		if (ret != OK) {
			printf("Shrink failed, filename : %s, target size : %d, errno : %d\n", path, size, errno);
			ret = ERROR;
			goto error_with_fd;
		}

		ret = stat(path, &st);
		if (ret == OK) {
			if (st.st_size != size) {
				printf("Shrunk file size is wrong. filename : %s, target size : %d, file size : %d\n", path, size, st.st_size);
				ret = ERROR;
				goto error_with_fd;
			}
			printf("Shrink works properly, filename : %s, target size : %d, current size : %d\n", path, size, st.st_size);
		} else {
			printf("Stat error, path : %d, errno : %d\n", path, errno);
			ret = ERROR;
			goto error_with_fd;
		}
	}

	ret = OK;

error_with_fd_sample:
	if (fd_sample > 0) {
		close(fd_sample);
	}
error_with_fd:
	close(fd);
	return ret;
}

static int recovery_file(char *src, char* backup, char* backup_dir, int bufsize, int idx)
{
	int fd1;
	int fd2;
	int ret;
	struct stat st;
	off_t offset;
	off_t start_offset;
	int filesize;
	
	printf("Recovering file src : %s idx : %d\n", src, idx);
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
				printf("Unlink %s failed, errno : %d\n", src, errno);
				return ERROR;
			}
		}
		ret = rename(backup, src);
		if (ret != OK) {
			printf("Rename failed, original filename : %s, target : %s, errno : %d, idx : %d\n", backup, src, errno, idx);
			return ERROR;
		}

	} else {
		fd1 = open(src, O_WROK);
		if (fd1 < 0) {
			printf("%s open error, errno : %d\n", src, errno);
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
			printf("ftruncate(extend) error, filename : %s, target size : %d, errno : %d\n", src, filesize, errno);
			ret = ERROR;
			goto error_with_fd1;
		}
		
		if (stat(src, &st) == OK) {
			if (st.st_size != filesize) {
				printf("Extended size is wrong, filename : %s, target size : %d, file size : %d\n", src, filesize, st.st_size);
				ret = ERROR;
				goto error_with_fd1;
			}
			printf("extendfile works properly, filename : %s, target size : %d current size : %d\n", src, filesize, st.st_size);
		} else {
			printf("Stat error, path : %d errno : %d\n", src, errno);
			ret = ERROR;
			goto error_with_fd1;
		}

		fd2 = open(backup, O_RDOK);
		if (fd2 < 0) {
			printf("%s open error, errno : %d\n", backup, errno);
			ret = ERROR;
			goto error_with_fd1;
		}
		
		start_offset = idx * bufsize;
		offset = lseek(fd1, start_offset, SEEK_SET);
		if (offset != start_offset) {
			printf("lseek error, file : %s, offset : %d, errno : %d\n", src, offset, errno);
			ret = ERROR;
			goto error_with_fd2;
		}

		offset = lseek(fd2, start_offset, SEEK_SET);
		if (offset != start_offset) {
			printf("lseek error, file : %s, offset : %d, errno : %d\n", backup, offset, errno);
			ret = ERROR;
			goto error_with_fd2;
		}

		for (int i = idx; i < TEST_WRITECOUNT; i++) {
			ret = read(fd2, (void *)&g_buf, bufsize);
			if (ret != bufsize) {
				printf("recovery_file, read failed, file : %s, ret : %d, index : %d, errno : %d\n", backup, ret, i, errno);
				ret = ERROR;
				goto error_with_fd2;
			}

			ret = write(fd1, (void *)&g_buf, bufsize);
			if (ret != bufsize) {
				printf("recovery_file, write failed, file : %s, ret : %d, index : %d,  errno : %d\n", src, ret, i, errno);
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

int init_test_file(void)
{
	struct stat st;
	int ret;

	ret = stat("/mnt", &st);
	if (ret != OK) {
		printf("stat error... errno : %d\n", errno);
		return ERROR;
	}

	/* Small test files use buffer size same as stat.st_blksize */
	TEST_SMALL_BUFSIZE = st.st_blksize;

	/* Create the sample text file for testing */
	ret = create_sample_text_file();
	if (ret != OK) {
		printf("Unable to create sample teext file for testing, ret : %d\n", ret);
		return ERROR;
	}

	/* Create small random file backup dir */
	ret = mkdir(TEST_SMALL_R_BACKUP_DIR, 0777);
	if (ret != OK && errno != EEXIST) {
		printf("init_test_file create backup dir for small file failed errno : %d\n", errno);
		return ERROR;
	}

	/* Create small random character files */
	ret = create_file(TEST_SMALL_R, TEST_SMALL_BUFSIZE, FILE_TYPE_RANDOM);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	ret = create_file(TEST_SMALL_R_BACKUP, TEST_SMALL_BUFSIZE, FILE_TYPE_RANDOM);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	/* Create small text file backup dir */
	ret = mkdir(TEST_SMALL_T_BACKUP_DIR, 0777);
	if (ret != OK && errno != EEXIST) {
		printf("init_test_file create backup dir for small file failed errno : %d\n", errno);
		return ERROR;
	}

	/* Create small text file and its backup */
	ret = create_file(TEST_SMALL_T, TEST_SMALL_BUFSIZE, FILE_TYPE_TEXT);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	ret = create_file(TEST_SMALL_T_BACKUP, TEST_SMALL_BUFSIZE, FILE_TYPE_TEXT);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	/* Create large random file backup dir */
	ret = mkdir(TEST_LARGE_R_BACKUP_DIR, 0777);
	if (ret != OK && errno != EEXIST) {
		printf("init_test_file create backup dir for large file failed ret : %d errno : %d\n", ret, errno);
		return ERROR;
	}

	/* Create large random character files */
	ret = create_file(TEST_LARGE_R, TEST_LARGE_BUFSIZE, FILE_TYPE_RANDOM);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	ret = create_file(TEST_LARGE_R_BACKUP, TEST_LARGE_BUFSIZE, FILE_TYPE_RANDOM);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	/* Create large text file backup dir */
	ret = mkdir(TEST_LARGE_T_BACKUP_DIR, 0777);
	if (ret != OK && errno != EEXIST) {
		printf("init_test_file create backup dir for large file failed ret : %d errno : %d\n", ret, errno);
		return ERROR;
	}

	/* Create large text file and its backup */
	ret = create_file(TEST_LARGE_T, TEST_LARGE_BUFSIZE, FILE_TYPE_TEXT);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	ret = create_file(TEST_LARGE_T_BACKUP, TEST_LARGE_BUFSIZE, FILE_TYPE_TEXT);
	if (ret != OK && errno != EEXIST) {
		return ERROR;
	}

	printf("Initializing test files successful!!\n");
	return OK;
}

static int do_test(char *src, char *backup, char *backupdir, int bufsize, file_type_t type)
{
	int ret;
	int index = TEST_WRITECOUNT;
	struct stat st;
	
	ret = verify_file(src, bufsize, &index, type);
	if (ret == ERROR) {
		printf("Verification failed for file : %s\n", src);
		return ret;
	}
	printf("Verification of file %s completed\n", src);

	ret = recovery_file(src, backup, backupdir, bufsize, index);
	if (ret != OK) {
		printf("recovery_file failed for file : %s\n", src);
		return ERROR;
	}
	printf("Recovery of file %s completed successfully\n", src);

	printf("Creating create backupdir : %s\n", backupdir);
	ret = mkdir(backupdir, 0777);
	if (ret != OK) {
		printf("Failed to create backup dir : %s errno : %d\n", backupdir, errno);
		return ERROR;
	}

	if (stat(src, &st) != OK) {
		printf("Failed to stat file : %s, errno : %d\n", src, errno);
		return ERROR;
	}

	printf("Rename original file : %s size : %d to backup file : %s\n", src, st.st_size, backup);
	ret = rename(src, backup);
	if (ret != OK) {
		printf("make back up file failed src : %s backup : %s errno : %d\n", src, backup, errno);
		return ERROR;
	}

	printf("Rename original to backup complete, create new file : %s\n", src);
	ret = create_file(src, bufsize, type);
	if (ret != OK) {
		printf("Unable to re-create original file : %s\n", src);
		return ERROR;
	}
	printf("Test on file :%s successful!!\n", src);
	return OK;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int smartfs_powercut_main(int argc, char *argv[])
#endif
{
	int ret;

	printf("Initializing test files\n");
	ret = init_test_file();

	if (ret != OK) {
		printf("init_test_file failed\n");
		return ERROR;
	}

	printf("################### Init Finished, Test will start after 3sec ###################\n");
	sleep(3);
	while (1) {
		/* Small test use same as size of data area of mtd sector, so there should not be error
		 * during the verify for small file test because it protected by journal.
		 */
		printf("do_test on %s\n", TEST_SMALL_R);
		ret = do_test(TEST_SMALL_R, TEST_SMALL_R_BACKUP, TEST_SMALL_R_BACKUP_DIR, TEST_SMALL_BUFSIZE, FILE_TYPE_RANDOM);
		if (ret != OK) {
			printf("Powercut test on small random file failed\n");
			return ret;
		}

		printf("do_test on %s\n", TEST_LARGE_R);
		ret = do_test(TEST_LARGE_R, TEST_LARGE_R_BACKUP, TEST_LARGE_R_BACKUP_DIR, TEST_LARGE_BUFSIZE, FILE_TYPE_RANDOM);
		if (ret != OK) {
			printf("Powercut test on large random file failed\n");
			return ret;
		}

		printf("do_test on %s\n", TEST_SMALL_T);
		ret = do_test(TEST_SMALL_T, TEST_SMALL_T_BACKUP, TEST_SMALL_T_BACKUP_DIR, TEST_SMALL_BUFSIZE, FILE_TYPE_TEXT);
		if (ret != OK) {
			printf("Powercut test on small text file failed\n");
			return ret;
		}

		printf("do_test on %s\n", TEST_LARGE_T);
		ret = do_test(TEST_LARGE_T, TEST_LARGE_T_BACKUP, TEST_LARGE_T_BACKUP_DIR, TEST_LARGE_BUFSIZE, FILE_TYPE_TEXT);
		if (ret != OK) {
			printf("Powercut test on large text file failed\n");
			return ret;
		}
	}
	
	return OK;
}
