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
 *
 * apps/examples/smartfs/fs_performance/fs_performance_main.c
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <tinyara/timer.h>

/****************************************************************************
 * Pre Processor Definitions
 ****************************************************************************/

#define TEST_FILE			CONFIG_MOUNT_POINT"test_file"
#define TEST_FLAG_FILE_CREATION		1
#define TEST_FLAG_OVERWRITE		2
#define TEST_FLAG_READ			3
#define TEST_FLAG_GC_EFFECT		4
#define TEST_FLAG_APPEND		5
#define TEST_FILE_NAME_LEN_MAX		15
#define ITR_OVE				5
#define ITR_APP				5
#define ITR_READ			100
#define ITR_GC				10
#define N_MULTIPLE_FILES		2
#define TIMER_DEVNAME			"/dev/timer%d"

/* Maximum buffer size used to read a file is 32kb */
#define MAX_RD_BUF_SIZE			32768

/* Maximum file size used for TCs is 64kb file */
#define MAX_BUF_SIZE			65536
#define MAX_FILE_SIZE			65536

/****************************************************************************
 * Public Variables
 ****************************************************************************/

static char g_file[N_MULTIPLE_FILES][TEST_FILE_NAME_LEN_MAX];
static char g_rw_buf[MAX_BUF_SIZE];
static char g_read_buf[MAX_RD_BUF_SIZE];
static char g_timer_path[_POSIX_PATH_MAX];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int do_test(int option, int fsz, int bsz)
{
	int ret = OK;
	int i;
	int j;
	int fd;
	unsigned long tot_time = 0;
	unsigned long itr_time = 0;
	struct timer_status_s before;
	struct timer_status_s after;
	int frt_fd = -1;

	memset(g_rw_buf, '0', MAX_BUF_SIZE);
	frt_fd = open(g_timer_path, O_RDONLY);
	if (frt_fd < 0) {
		printf("Unable to open file: %s errno: %d\n", g_timer_path, errno);
		return ERROR;
	}

	ret = ioctl(frt_fd, TCIOC_SETMODE, MODE_FREERUN);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Failed to set Free Run Timer: %d\n", errno);
		ret = -1;
		goto close_with_frt;
	}

	ret = ioctl(frt_fd, TCIOC_START, TRUE);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Failed to start Free Run Timer: %d\n", errno);
		ret = -1;
		goto close_with_frt;
	}

	switch (option) {

	case TEST_FLAG_FILE_CREATION :
		printf("\n****************************************************************************************\n");
		printf("********************************  TEST FILE CREATION  **********************************\n\n");
		printf("Testing jSmartFs File creation with File size: %d bytes\n", fsz);
		printf("The test creates a file with buffer size equal to file size.\n\n");

		ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&before);
		if (ret < 0) {
			goto error_with_frt_status;
		}

		fd = open(g_file[0], O_WROK | O_CREAT);
		if (fd < 0) {
			printf("Unable to open file: %s, fd: %d\n", g_file[0], fd);
			ret = ERROR;
			goto close_with_frt;
		}

		ret = write(fd, g_rw_buf, fsz);
		if (ret != (fsz)) {
			printf("Unable to write in file: %s, size: %d, ret: %d\n", g_file[0], fsz, ret);
			ret = ERROR;
			goto close_with_fd;
		}

		close(fd);

		ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&after);
		if (ret < 0) {
			fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
			ret = -1;
			goto error_with_frt_status;
		}

		tot_time  = after.timeleft - before.timeleft;
		printf("Time taken is: %lu us\n", tot_time);

		ret = unlink(g_file[0]);
		if (ret != OK) {
			printf("Failed to unlink file %s, errno : %d\n", g_file[0], errno);
			ret = ERROR;
			goto close_with_frt;
		}
		printf("\n****************************************************************************************\n");
		break;

	case TEST_FLAG_OVERWRITE :
		printf("\n****************************************************************************************\n");
		printf("**********************************  TEST OVERWRITE  ************************************\n\n");
		printf("Testing jSmartFs overwrite with File size: %d bytes, Buffer size: %d bytes\n", fsz, bsz);
		printf("The test overwrites a file from start, one buffer size at a time.\n\n");

		fd = open(g_file[0], O_WROK | O_CREAT);
		if (fd < 0) {
			printf("Unable to open file: %s, fd: %d\n", g_file[0], fd);
			ret = ERROR;
			goto close_with_frt;
		}

		for (i = 0; i < (fsz / bsz); i++) {
			ret = write(fd, g_rw_buf, bsz);
			if (ret != (bsz)) {
				printf("Unable to write in file: %s, ret: %d\n", g_file[0], ret);
				ret = ERROR;
				goto close_with_fd;
			}
		}
		close(fd);

		for (i = 1; i <= ITR_OVE; i++) {
			ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&before);
			if (ret < 0) {
				goto error_with_frt_status;
			}

			fd = open(g_file[0], O_WROK);
			if (fd < 0) {
				printf("Unable to open file : %s, fd: %d\n", g_file[0], fd);
				ret = ERROR;
				goto close_with_frt;
			}

			ret = lseek(fd, 0, SEEK_SET);
			if (ret != 0) {
				printf("Unable to move file pointer to start of the file\n");
				ret = ERROR;
				goto close_with_fd;
			}

			for (j = 1; j <= (fsz / bsz); j++) {
				ret = write(fd, g_rw_buf, bsz);
				if (ret != (bsz)) {
					printf("Unable to write in file: %s, iter= %d ,ret = %d\n", g_file[0], j, ret);
					ret = ERROR;
					goto close_with_fd;
				}
			}

			close(fd);

			ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&after);
			if (ret < 0) {
				goto error_with_frt_status;
			}

			itr_time = after.timeleft - before.timeleft;
			tot_time += itr_time;
			printf("iteration: %d, time: %lu us\n", i, itr_time);
		}
		printf("\nTotal time: %lu us\n", tot_time);

		ret = unlink(g_file[0]);
		if (ret != OK) {
			printf("Failed to unlink file %s, errno: %d\n", g_file[0], errno);
			ret = ERROR;
			goto close_with_frt;
		}
		printf("\n****************************************************************************************\n");
		break;

	case TEST_FLAG_APPEND :
		printf("\n****************************************************************************************\n");
		printf("************************************  TEST APPEND  *************************************\n\n");
		printf("Testing jSmartFs Append with file size: %d bytes, buffer size: %d bytes\n", fsz, bsz);
		printf("The test append 64kb data to the end of the file given.\n\n");
		for (i = 1; i <= ITR_APP; i++) {
			fd = open(g_file[0], O_WROK | O_CREAT);
			if (fd < 0) {
				printf("Unable to open file: %s, fd: %d\n", g_file[0], fd);
				ret = ERROR;
				goto close_with_frt;
			}

			/*create a test file of size fsz */
			ret = write(fd, g_rw_buf, fsz);
			if (ret != (fsz)) {
				printf("Unable to write in file: %s, ret: %d\n", g_file[0], ret);
				ret = ERROR;
				goto close_with_fd;
			}
			close(fd);

			ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&before);
			if (ret < 0) {
				goto error_with_frt_status;
			}

			fd = open(g_file[0], O_WROK | O_APPEND);
			if (fd < 0) {
				printf("Unable to open file : %s, fd: %d\n", g_file[0], fd);
				ret = ERROR;
				goto close_with_frt;
			}

			/*append 64 kb data*/
			for (j = 0; j < (MAX_FILE_SIZE / bsz); j++ ) {
				ret = write(fd, g_rw_buf, bsz);
				if (ret != (bsz)) {
					printf("Unable to append to file: %s, ret: %d\n", g_file[0], ret);
					ret = ERROR;
					goto close_with_fd;
				}
			}
			close(fd);

			ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&after);
			if (ret < 0) {
				goto error_with_frt_status;
			}

			itr_time = after.timeleft - before.timeleft;
			tot_time += itr_time;
			printf("iteration: %d, time: %lu us\n", i, itr_time);

			ret = unlink(g_file[0]);
			if (ret != OK) {
				printf("Failed to unlink file %s, errno: %d\n", g_file[0], errno);
				ret = ERROR;
				goto close_with_frt;
			}
		}
		printf("\nTotal time: %lu us\n", tot_time);
		printf("\n****************************************************************************************\n");
		break;

	case TEST_FLAG_READ :
		printf("\n****************************************************************************************\n");
		printf("************************************  TEST READ  ***************************************\n\n");
		printf("Testing jSmartFs Read with file size: %d bytes, buffer size: %d bytes\n", fsz, bsz);
		printf("The test reads a given file size with the given buffer size %d times.\n\n", ITR_READ);
		memset(g_read_buf, '1', MAX_RD_BUF_SIZE);

		fd = open(g_file[0], O_WROK | O_CREAT);
		if (fd < 0) {
			printf("Unable to open file: %s, errno: %d\n", g_file[0], errno);
			ret = ERROR;
			goto close_with_frt;
		}

		ret = write(fd, g_rw_buf, fsz);
		if (ret != (fsz)) {
			printf("Unable to write in file: %s, ret: %d\n", g_file[0], ret);
			ret = ERROR;
			goto close_with_fd;
		}
		close(fd);

		ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&before);
		if (ret < 0) {
			goto error_with_frt_status;
		}

		for (i = 1; i <= ITR_READ; i++) {
			fd = open(g_file[0], O_RDOK);
			if (fd < 0) {
				printf("Unable to open file: %s to read, fd: %d\n", g_file[0], fd);
				ret = ERROR;
				goto close_with_frt;
			}

			do {
				ret = read(fd, g_read_buf, bsz);
			} while (ret == bsz);

			close(fd);
		}

		ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&after);
		if (ret < 0) {
			goto error_with_frt_status;
		}

		tot_time += (after.timeleft - before.timeleft);
		printf("Time: %lu us\n", tot_time);

		ret = unlink(g_file[0]);
		if (ret < 0) {
			printf("Failed to unlink file %s, errno: %d\n", g_file[0], errno);
			ret = ERROR;
			goto close_with_frt;
		}
		printf("\n****************************************************************************************\n");
		break;

	case TEST_FLAG_GC_EFFECT :
		printf("\n****************************************************************************************\n");
		printf("**************************  TEST GARBAGE COLLECTION EFFECT  ****************************\n\n");
		printf("Testing jSmartFs Garbage collection with file size: %d bytes, buffer size: %d bytes\n", fsz, bsz);
		printf("The test triggers Garbage Collection by repeatedly creating and unlinking a file.\n\n");
		for (i = 1; i <= ITR_GC; i++) {
			ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&before);
			if (ret < 0) {
				goto error_with_frt_status;
			}

			fd = open(g_file[0], O_WROK | O_CREAT);
			if (fd < 0) {
				printf("Unable to open file: %s, fd: %d\n", g_file[0], fd);
				ret = ERROR;
				goto close_with_frt;
			}

			ret = write(fd, g_rw_buf, fsz);
			if (ret != (fsz)) {
				 printf("Unable to write to file: %s, ret: %d\n", g_file[0], ret);
				 ret = ERROR;
				 goto close_with_fd;
			}
			close(fd);

			ret = unlink(g_file[0]);
			if (ret != OK) {
				printf("Failed to unlink file: %s, errno: %d\n", g_file[0], errno);
				ret = ERROR;
				goto close_with_frt;
			}

			ret = ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&after);
			if (ret < 0) {
				goto error_with_frt_status;
			}

			itr_time = after.timeleft - before.timeleft;
			printf("iteration %d, time: %lu us\n", i, itr_time);
			tot_time += itr_time;
		}
		printf("\nTotal time: %lu us\n", tot_time);
		printf("\n****************************************************************************************\n");
		break;

	default:
		break;
	}

	ret = ioctl(frt_fd, TCIOC_STOP, 0);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Failed to stop the Timer : %d\n", errno);
		ret = -1;
		close(frt_fd);
	}

	close(frt_fd);
	return ret;

error_with_frt_status:
	fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
	ret = -1;
	goto close_with_frt;
close_with_fd:
	close(fd);
close_with_frt:
	ret = ioctl(frt_fd, TCIOC_STOP, 0);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Failed to stop the Free Run Timer: %d\n", errno);
		ret = -1;
	}

	close(frt_fd);

	return ret;
}

static void fs_performance_show_usage(void) {
	printf("\n****************************************************************************************\n\n");
	printf("This test example executes one of the test cases and prints the time taken (for each iteration)\n");
	printf("\nUSAGE:  fs_performance [Timer device no.] [Test case no.] [File size(bytes)] [Buffer size(bytes)]\n\n");
	printf("	Timer Device no. :	FRT Timer device\n");
	printf("	Test Case        :	Test case to be executed. Range[1-5].\n");
	printf("      		1. File Creation Test : Creates a file of given file size in one go.\n");
	printf("			The buffer used in the test is equal to file size.\n");
	printf("      		2. Overwrite Test : Overwrite a file from start of given file with given buffer size.\n");
	printf("      		3. Append Test : Appends 64 kb data to a given file.\n");
	printf("      		4. Read Test : Read a file of given file size with given buffer size.\n");
	printf("      		5. Garbage Collection Effect Test : Creates a file and unlink it to trigger GC.\n");
	printf("			The buffer used in test is equal to file size.\n");
	printf("	File Size        :	File size(in bytes). Range[512-65536].\n");
	printf("	Buffer Size      :	Buffer size(in bytes). Range[512-File size].\n");
	printf("\n****************************************************************************************\n");
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int fs_performance_main(int argc, char *argv[])
#endif
{
	int option = -1;
	int ret = 0;
	int timer_devno = 0;
	int selected_opt;
	int fsz = MAX_FILE_SIZE;
	int bsz = MAX_RD_BUF_SIZE;

	if (argc == 2 && !strncmp(argv[1], "--help", strlen("--help") + 1)) {
		ret = OK;
		goto show_usage;
	}

	if (argc > 4) {
		timer_devno = atoi(argv[1]);
		option = atoi(argv[2]);
		if (option < 1 || option > 5) {
			goto show_usage;
		}

		fsz = atoi(argv[3]);
		if (fsz > MAX_FILE_SIZE) {
			goto show_usage;
		}

		if (argc == 5) {
			bsz = atoi(argv[4]);
			if (bsz > MAX_BUF_SIZE || bsz > fsz) {
				goto show_usage;
			}
		}
	} else {
		ret = -1;
		goto show_usage;
	}

	snprintf(g_timer_path, _POSIX_PATH_MAX, TIMER_DEVNAME, timer_devno);
	snprintf(g_file[0], TEST_FILE_NAME_LEN_MAX, "%s%d", TEST_FILE, 0);

	switch (option) {

	case 1: selected_opt = TEST_FLAG_FILE_CREATION;
		break;

	case 2: selected_opt = TEST_FLAG_OVERWRITE;
		break;

	case 3: selected_opt = TEST_FLAG_APPEND;
		break;

	case 4: selected_opt = TEST_FLAG_READ;
		break;

	case 5: selected_opt = TEST_FLAG_GC_EFFECT;
		break;

	default:
		goto show_usage;

	}

	ret = do_test(selected_opt, fsz, bsz);
	if (ret < 0) {
		printf("Unable to do performance test ret: %d\n", ret);
		ret = -1;
	}
	goto test_exist;

show_usage:
	fs_performance_show_usage();
test_exist:
	printf("Filesystem Performance Test Example Exits\n");
	return ret;
}
