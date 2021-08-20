/****************************************************************************
 *
 * Copyright 2018, 2021 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define USAGE                                                           \
	"\n FS R/W Performance Test\n"                                      \
	"\n Usage :     smartfs_test <operation> <path> <size> <iteration> <print_option>\n"  \
	"\nOperation :  1.Read/Write / 2.Read / 3.Write / 4. Write/Remove \n"                  \
	" Path :        Test File Path\n"                                   \
	" Size :        Write/Read Data size\n"                             \
	" Iteration :   Number of repetitions\n"                            \
	" Detail :      Enable(y) or Disable(n) Printing Details Option\n"  \
	"               (Printing affects at R/W Performance)\n"         \
	"\n ex) smartfs_test 1 /mnt/test 4096 30 y\n"

#define FS_RW_ITERATION_MAX 10000

enum TEST_OPTS {
	TEST_OPT_WRITEREAD = 1,
	TEST_OPT_READ = 2,
	TEST_OPT_WRITE = 3,
	TEST_OPT_WRITEREMOVE = 4,
	TEST_OPT_MAX,
};

/****************************************************************************
 * Global Variables
 ****************************************************************************/
static long g_total_time;
static char g_printopt;

/****************************************************************************
 * smartfs_test
 ****************************************************************************/

static inline long get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

static void print_log(const char *format, ...)
{
	va_list ap;

	if (g_printopt == 'y') {
		va_start(ap, format);
		vfprintf(stdout, format, ap);
		va_end(ap);
	}
}

static int fs_close(int fd)
{
	int ret;
	long time;
	long timediff;

	time = get_time();
	ret = close(fd);
	if (ret < 0) {
		printf("%s errno: %d\n", __FUNCTION__, errno);
		return ERROR;
	}
	timediff = get_time() - time;
	g_total_time += timediff;
	print_log("close:%ldms, ", timediff);

	return OK;
}

static int fs_write(char *filepath, char *buf, size_t nbytes)
{
	int fd;
	int ret;
	int total_write = 0;
	long time;
	long timediff;

	fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd < 0) {
		printf("%s errno: %d\n", __FUNCTION__, errno);
		return ERROR;
	}

	time = get_time();
	while (total_write < nbytes) {
		ret = write(fd, buf + total_write, nbytes - total_write);
		if (ret != nbytes - total_write) {
			printf("%s errno: %d\n", __FUNCTION__, errno);
			close(fd);
			return ERROR;
		}
		total_write += ret;
	}
	timediff = get_time() - time;
	g_total_time += timediff;
	print_log("write:%ldms, ", timediff);

	close(fd);

	return OK;
}

static int fs_read(char *filepath, char *buf, size_t nbytes)
{
	int fd;
	int ret;
	int total_read = 0;
	long time;
	long timediff;

	fd = open(filepath, O_RDONLY);
	if (fd < 0) {
		printf("%s errno: %d\n", __FUNCTION__, errno);
		return ERROR;
	}

	time = get_time();
	while (total_read < nbytes) {
		ret = read(fd, buf + total_read, nbytes - total_read);
		if (ret != nbytes - total_read) {
			printf("%s errno: %d\n", __FUNCTION__, errno);
			close(fd);
			return ERROR;
		}
		total_read += ret;
	}
	timediff = get_time() - time;
	g_total_time += timediff;
	print_log("read:%ldms, ", timediff);

	close(fd);

	return OK;
}

static int fs_remove(char *filepath, void *buf, int nbytes)
{
	int ret;
	long time;
	long timediff;

	time = get_time();
	ret = unlink(filepath);
	if (ret < 0) {
		printf("%s errno: %d\n", __FUNCTION__, errno);
		return ERROR;
	}
	timediff = get_time() - time;
	g_total_time += timediff;
	print_log("remove:%dms, ", timediff);

	return OK;
}

static int write_read_test(char *filepath, void *buf, int nbytes, int iteration)
{
	int i;
	int ret;

	for (i = 0; i < iteration; i++) {
		ret = fs_write(filepath, buf, nbytes);
		if (ret < 0) {
			return ERROR;
		}

		ret = fs_read(filepath, buf, nbytes);
		if (ret < 0) {
			return ERROR;
		}
	}

	return OK;
}

static int write_test(char *filepath, void *buf, int nbytes, int iteration)
{
	int i;
	int ret; 

	for (i = 0; i < iteration; i++) {
		ret = fs_write(filepath, buf, nbytes);
		if (ret < 0) {
			return ERROR;
		}
	}

	return OK;
}

static int read_test(char *filepath, void *buf, int nbytes, int iteration)
{
	int i;
	int ret;

	ret = fs_write(filepath, buf, nbytes);
	if (ret < 0) {
		return ERROR;
	}

	for (i = 0; i < iteration; i++) {
		ret = fs_read(filepath, buf, nbytes);
		if (ret < 0) {
			return ERROR;
		}
	}

	return OK;
}

static int write_remove_test(char *filepath, void *buf, int nbytes, int iteration)
{
	int i;
	int ret;

	for (i = 0; i < iteration; i++) {
		ret = fs_write(filepath, buf, nbytes);
		if (ret < 0) {
			return ERROR;
		}

		ret = fs_remove(filepath, buf, nbytes);
		if (ret < 0) {
			return ERROR;
		}
	}

	return OK;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int smartfs_test_main(int argc, char *argv[])
#endif
{
	int ret;
	int size;
	int operation;
	int iteration;
	char *buf;
	char *filepath;

	if (argc == 6) {
		printf("FS R/W Performance Test!\n");

		operation = atoi(argv[1]);
		filepath = argv[2];
		size = atoi(argv[3]);
		iteration = atoi(argv[4]);
		g_printopt = argv[5][0];

		if (operation < TEST_OPT_WRITEREAD || operation >= TEST_OPT_MAX ||
			size < 0 || iteration < 1 || iteration > FS_RW_ITERATION_MAX) {
			printf("%s", USAGE);
			return ERROR;
		}

		if (strlen(argv[5]) != 1 || (g_printopt != 'y' && g_printopt != 'n')) {
			printf("%s", USAGE);
			return ERROR;
		}

		buf = (char *)zalloc(size);
		if (!buf) {
			printf("%s: Memory allocation failed, %d\n", __FUNCTION__, size);
			return ERROR;
		}

		switch (operation) {
		case TEST_OPT_WRITEREAD:
			ret = write_read_test(filepath, buf, size, iteration);
			break;
		case TEST_OPT_WRITE:
			ret = read_test(filepath, buf, size, iteration);
			break;
		case TEST_OPT_READ:
			ret = write_test(filepath, buf, size, iteration);
			break;
		case TEST_OPT_WRITEREMOVE:
			ret = write_remove_test(filepath, buf, size, iteration);
			break;
		}
		free(buf);

		if (ret == OK) {
			/* Print measured run-time on success */
			printf("run_time: %ld ms\n", g_total_time);
			return OK;
		}
		printf("Test Failed. Please check above error logs\n");
		return ERROR;
	}

	printf("%s", USAGE);
	return ERROR;
}
