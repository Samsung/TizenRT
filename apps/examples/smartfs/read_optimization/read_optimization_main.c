/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
 * apps/examples/smartfs/read_optimization/read_optimization_main.c
 *
 *   Copyright (C) 2013, 2015 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
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

#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include <tinyara/timer.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define SAMPLE_FILE	"/mnt/sample_file"
#define TIMER_DEVNAME	"/dev/timer%d"

/****************************************************************************
 * Private data
 ****************************************************************************/
char g_buf[1025];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: read_test
 *
 * Description: Perform a back-to-back read of the entire file's contents,
 *              multiple times.
 *
 ****************************************************************************/

static int read_test(int size)
{
	int fd;
	int ret;
	int itr = 0;

	while (itr < 100) {
		fd = open(SAMPLE_FILE, O_RDOK);
		if (fd < 0) {
			printf("Unable to open Sample File while testing, Errno = %d\n", errno);
			return ERROR;
		}

		do {
			ret = read(fd, g_buf, size);
		} while (ret == size);

		close(fd);
		itr += 1;
	}

	return OK;
}

/****************************************************************************
 * Name: create_test_file
 *
 * Description: Creates the test file with test data that we will use
 *              to conduct the test.
 *
 ****************************************************************************/

static int create_test_file(int f_size)
{
	int fd;
	int ret;
	memset(g_buf, '1', 1024);

	fd = open(SAMPLE_FILE, O_WROK | O_CREAT);
	if (fd < 0) {
		printf("Unable to open Sample File for testing, Error = %d\n", errno);
		return ERROR;
	}

	while (f_size >= 1024) {
		ret = write(fd, g_buf, 1024);
		if (ret != 1024) {
			printf("Unable to write to Sample File, Error = %d\n", errno);
			return ERROR;
		}
		f_size -= 1024;
	}
	if (f_size) {
		ret = write(fd, g_buf, f_size);
		if (ret != f_size) {
			printf("Unable to write to Sample File, Error = %d\n", errno);
			return ERROR;
		}
	}
	close(fd);

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, char *argv[])
#else
int read_optimization_main(int argc, char *argv[])
#endif
{
	int ret;
	int size = 8;
        int buf_size;
	int frt_fd = -1;
	int timer_devno = 0;
	char timer_path[_POSIX_PATH_MAX];
	struct timer_status_s time_1, time_2;
	uint32_t time_read;
	struct stat st;

	if (argc > 1) {
		timer_devno = atoi(argv[1]);
		if (argc == 3) {
			size = atoi(argv[2]);
		}
	} else {
		printf("Correct Test Command Usage: smartfs_read_opt <Timer Device Number> <file Size in Kb>\n");
		return -1;
	}

	snprintf(timer_path, _POSIX_PATH_MAX, TIMER_DEVNAME, timer_devno);
	printf("Timer path = %s\n", timer_path);

	ret = create_test_file(size * 1024);
	if (ret != OK) {
		printf("Unable to complete test\n");
		return -1;
	}

	frt_fd = open(timer_path, O_RDONLY);
	if (frt_fd < 0) {
		fprintf(stderr, "ERROR: Failed to open Free Run Timer: %d\n", errno);
		return -1;
	}

	if (ioctl(frt_fd, TCIOC_SETMODE, MODE_FREERUN) < 0) {
		fprintf(stderr, "ERROR: Failed to set Free Run Timer: %d\n", errno);
		return -1;
	}
	if (ioctl(frt_fd, TCIOC_START, TRUE) < 0) {
		fprintf(stderr, "ERROR: Failed to start Free Run Timer: %d\n", errno);
		return -1;
	}
	if (ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&time_1) < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		return -1;
	}

	read_test(512);

	if (ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&time_2) < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		return -1;
	}

	time_read = time_2.timeleft - time_1.timeleft;
	printf("\n****************************************************************************************\n");
	printf("Time taken to read %dKb file with 512 byte buffer, 100 times = %lu\n", size, time_read);
	printf("\n****************************************************************************************\n");

	if (ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&time_1) < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		return -1;
	}

	read_test(1024);

	if (ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&time_2) < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		return -1;
	}

	time_read = time_2.timeleft - time_1.timeleft;
	printf("\n****************************************************************************************\n");
	printf("Time taken to read %dKb file with 1024 byte buffer, 100 times = %lu\n", size, time_read);
	printf("\n****************************************************************************************\n");

	ret = stat("/mnt", &st);
	if (ret < 0) {
		printf("Stat to get optimal read buffer size failed, Errno = %d\n", errno);
		return -1;
	}
	buf_size = st.st_blksize;

	if (ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&time_1) < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		return -1;
	}

	read_test(buf_size);

	if (ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&time_2) < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		return -1;
	}

	time_read = time_2.timeleft - time_1.timeleft;
	printf("\n****************************************************************************************\n");
	printf("Time taken to read %dKb file with %d byte buffer, 100 times = %lu\n", size, buf_size, time_read);
	printf("\n****************************************************************************************\n");

	if (ioctl(frt_fd, TCIOC_STOP, 0) < 0) {
		fprintf(stderr, "ERROR: Failed to stop the Free Run Timer: %d\n", errno);
	}

	unlink(SAMPLE_FILE);
	printf("Test Example Complete\n\n");

	return ret;
}
