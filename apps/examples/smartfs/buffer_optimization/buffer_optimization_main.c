/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
 *
 * apps/examples/smartfs/buffer_optimization/buffer_optimization_main.c
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/stat.h>
#include <sys/statfs.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include <tinyara/timer.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define SAMPLE_FILE "/mnt/sample_file"
#define TIMER_DEVNAME "/dev/timer%d"
#define READ_TEST_ITR 100	//No. of times file will be read for testing
#define DEFAULT_BUF_SIZE_S 512	//Smaller default buffer size used for demonstration testng
#define DEFAULT_BUF_SIZE_L 1024	//Larger default buffer size used for demonstration testing

/****************************************************************************
 * Private data
 ****************************************************************************/
char g_buf[DEFAULT_BUF_SIZE_L + 1];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: print_test_result
 *
 * Description: Helper function to print the test results.
 *
 ****************************************************************************/

void print_test_result(const char *name, int file_size, int itr, int buf_size, uint32_t time)
{
	printf("\n**************************************************************************************\n");
	printf("Throughput while %s %dKb file %d time(s) with %dbyte buffer size = %.02fKbps\n",\
			name, file_size, itr, buf_size, ((float)file_size * itr) / ((float)time / 1000000));
	printf("\n**************************************************************************************\n");
	return;
}

/****************************************************************************
 * Name: create_test_file
 *
 * Description: Creates the test file with test data that we will use
 *              to conduct the test.
 *
 ****************************************************************************/

static int create_test_file(int sample_file_size)
{
	int fd;
	int ret = OK;
	int buflen;
	memset(g_buf, DEFAULT_BUF_SIZE_L, '1');

	fd = open(SAMPLE_FILE, O_WROK | O_CREAT);
	if (fd < 0) {
		printf("Unable to open Sample File: %s for testing, Error: %d\n", SAMPLE_FILE, errno);
		return ERROR;
	}

	//Use larger default buffer size to create the sample file
	while (sample_file_size > 0) {
		if (sample_file_size >= DEFAULT_BUF_SIZE_L) {
			buflen = DEFAULT_BUF_SIZE_L;
		} else {
			buflen = sample_file_size;
		}
		ret = write(fd, g_buf, buflen);
		if (ret < 0) {
			printf("Unable to write to Sample File %s, Error: %d\n", SAMPLE_FILE, errno);
			ret = errno;
			goto error_with_fd;
		}
		sample_file_size -= ret;
	}
	ret = OK;

error_with_fd:
	close(fd);
	return ret;
}

/****************************************************************************
 * Name: read_test_util
 *
 * Description: Perform a back-to-back read of the entire file's contents,
 *              multiple times.
 *
 ****************************************************************************/

static int read_test_util(int buf_size)
{
	int fd;
	int ret;
	int itr = 0;

	while (itr < READ_TEST_ITR) {
		fd = open(SAMPLE_FILE, O_RDOK);
		if (fd < 0) {
			printf("Unable to open Sample File %s for reading, Errno = %d\n", SAMPLE_FILE, errno);
			return ERROR;
		}

		do {
			ret = read(fd, g_buf, buf_size);
			if (ret < 0) {
				printf("Unable to read File %s, Error: %d\n", SAMPLE_FILE, errno);
				ret = errno;
				goto errout_with_fd;
			}
		} while (ret == buf_size);

		close(fd);
		itr += 1;
	}

	ret = OK;
errout_with_fd:
	close(fd);
	return ret;
}

/****************************************************************************
 * Name: read_test
 *
 * Description: Set up the timer and run the read test with different
 *              buffer sizes.
 *
 ****************************************************************************/

static int read_test(int frt_fd, int file_size)
{
	int ret;
	int buf_size;
	struct timer_status_s time_1, time_2;
	uint32_t time_read;
	struct stat st;

	printf("================================== STARTING READ TESTS ==================================\n\n");

	ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&time_1);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		return ret;
	}

	ret = read_test_util(DEFAULT_BUF_SIZE_S);
	if (ret != OK) {
		printf("Reading with buffer size %d failed\n", DEFAULT_BUF_SIZE_S);
		return ret;
	}

	ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&time_2);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		return ret;
	}

	time_read = time_2.timeleft - time_1.timeleft;
	print_test_result("reading", file_size * 1024, READ_TEST_ITR, DEFAULT_BUF_SIZE_S, time_read);

	ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&time_1);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		return ret;
	}

	ret = read_test_util(DEFAULT_BUF_SIZE_L);
	if (ret != OK) {
		printf("Reading with buffer size %d failed\n", DEFAULT_BUF_SIZE_L);
		return ret;
	}

	ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&time_2);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		return ret;
	}

	time_read = time_2.timeleft - time_1.timeleft;
	print_test_result("reading", file_size * 1024, READ_TEST_ITR, DEFAULT_BUF_SIZE_L, time_read);

	ret = stat("/mnt", &st);
	if (ret < 0) {
		printf("Stat to get optimal read buffer size failed, Errno = %d\n", errno);
		return -1;
	}
	buf_size = st.st_blksize;

	ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&time_1);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		return ret;
	}

	ret = read_test_util(buf_size);
	if (ret != OK) {
		printf("Reading with buffer size %d failed\n", buf_size);
		return ret;
	}

	ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&time_2);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		return ret;
	}

	time_read = time_2.timeleft - time_1.timeleft;
	printf("TEST WITH OPTIMIZED BUFFER\n");
	print_test_result("reading", file_size * 1024, READ_TEST_ITR, buf_size, time_read);

	printf("================================== READ TESTS COMPLETE ==================================\n\n");

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, char *argv[])
#else
int buffer_optimization_main(int argc, char *argv[])
#endif
{
	int ret = OK;
	int file_size = 8;
	int frt_fd = -1;
	int timer_devno = 0;
	struct statfs buf;

	ret = statfs("/mnt", &buf);
	if (ret < 0) {
		printf("Statfs Failed!!! ret: %d\n", ret);
		return -1;
	}
	char timer_path[buf.f_namelen];

	if (argc > 1) {
		timer_devno = atoi(argv[1]);
		if (argc == 3) {
			file_size = atoi(argv[2]);
		}
	} else {
		printf("\nCorrect test command usage is: smartfs_buf_opt <Timer Device No.> <File Size in Kb>\n");
		printf("-> Read test example will read the file back to back, one buffer size at a time\n");
		return -1;
	}

	snprintf(timer_path, _POSIX_PATH_MAX, TIMER_DEVNAME, timer_devno);
	printf("Timer path = %s\nCreating Test File\n", timer_path);

	ret = create_test_file(file_size * 1024);
	if (ret != OK) {
		printf("Unable to create test file, ret: %d\n", ret);
		return -1;
	}
	printf("Test File Creation Successful\n");

	frt_fd = open(timer_path, O_RDONLY);
	if (frt_fd < 0) {
		fprintf(stderr, "ERROR: Failed to open Free Run Timer: %d\n", errno);
		return -1;
	}

	if (ioctl(frt_fd, TCIOC_SETMODE, MODE_FREERUN) < 0) {
		fprintf(stderr, "ERROR: Failed to set Free Run Timer: %d\n", errno);
		ret = -1;
		goto error_with_frt_fd;
	}
	if (ioctl(frt_fd, TCIOC_START, TRUE) < 0) {
		fprintf(stderr, "ERROR: Failed to start Free Run Timer: %d\n", errno);
		ret = -1;
		goto error_with_frt_fd;
	}

	ret = read_test(frt_fd, file_size);
	if (ret != OK) {
		printf("Read test failed on file: %s, ret: %d\n", SAMPLE_FILE, ret);
		ret = -1;
	}

	if (ioctl(frt_fd, TCIOC_STOP, 0) < 0) {
		fprintf(stderr, "ERROR: Failed to stop the Free Run Timer: %d\n", errno);
		ret = -1;
	}
	if (ret == OK) {
		printf("Test Example Completed Successfully\n");
	}

error_with_frt_fd:
	close(frt_fd);
	printf("SmartFs Optimized Buffer Test Example Exits\n\n");
	return ret;
}
