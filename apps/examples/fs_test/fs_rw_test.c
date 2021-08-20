/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define USAGE                                                           \
	"\n FS R/W Performance Test\n"                                      \
	"\n usage :     fs_rw_test Path Size Iteration Operation Detail\n"  \
	"\n Path :      Test File Path\n"                                   \
	" Size :        Write/Read Data size\n"                             \
	" Iteration :   Number of repetitions\n"                            \
	" Operation :   1.Read/Write / 2.Read / 3.Write\n"                  \
	" Detail :      Enable(y) or Disable(n) Printing Details Option\n"  \
	"               (Printing is affects at R/W Performance)\n"         \
	"\n ex) fs_rw_test /mnt/test 128 30 1 y\n"

#define FS_RW_ITERATION_MAX 10000
#define FS_RW_SIZE_MAX 2048
#define TEST_OPT_WRITEREAD 1
#define TEST_OPT_WRITE 2
#define TEST_OPT_READ 3
/****************************************************************************
 * global value
 ****************************************************************************/

static int g_total_time = 0;
static int g_iteration = 0;
static int g_size = 0;
static int g_operation = 0;
static char g_printopt = 0;
static char *g_filepath = NULL;

/****************************************************************************
 * fs_rw_test
 ****************************************************************************/

static inline long get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

static void rw_print_detail(const char *format, ...)
{
	va_list ap;

	if (g_printopt == 'y' || g_printopt == 'Y') {
		va_start(ap, format);
		vfprintf(stdout, format, ap);
		va_end(ap);
	}
}

static int _rwt_open(char mode)
{
	int oflag = 0;

	if (mode == 'r') {
		oflag = O_RDONLY;
	} else if (mode == 'w') {
		oflag = O_WRONLY | O_CREAT | O_TRUNC;
	} else {
		printf("%s", USAGE);
		exit(EXIT_FAILURE);
	}

	long t = get_time();
	int fd = open(g_filepath, oflag);
	if (fd == -1) {
		printf("%s errno: %d\n", __FUNCTION__, errno);
		exit(EXIT_FAILURE);
	}
	long dt = get_time() - t;
	g_total_time += dt;
	rw_print_detail("open(%c):%dms, ", mode, dt);

	return fd;
}

static void _rwt_close(int fd)
{
	long t = get_time();
	int ret = close(fd);
	if (ret == -1) {
		printf("%s errno: %d\n", __FUNCTION__, errno);
		exit(EXIT_FAILURE);
	}
	long dt = get_time() - t;
	g_total_time += dt;
	rw_print_detail("close:%dms\n", dt);
}

static void _rwt_write(int fd, char *buf, size_t nbytes)
{
	int ret = 0;
	int total_write = 0;

	long t = get_time();
	while (total_write < nbytes) {
		ret = write(fd, buf + total_write, nbytes - total_write);
		if (ret == 0 || ret == -1) {
			close(fd);
			exit(EXIT_FAILURE);
		}
		total_write += ret;
	}
	long dt = get_time() - t;

	g_total_time += dt;
	rw_print_detail("write:%dms, ", dt);
}

static void _rwt_read(int fd, char *buf, size_t nbytes)
{
	int ret = 0;
	int total_read = 0;

	long t = get_time();
	while (total_read < nbytes) {
		ret = read(fd, buf + total_read, nbytes - total_read);
		if (ret == 0 || ret == -1) {
			close(fd);
			exit(EXIT_FAILURE);
		}
		total_read += ret;
	}
	long dt = get_time() - t;

	g_total_time += dt;
	rw_print_detail("read:%dms, ", dt);
}

static void wr_test(void *buf, size_t nbytes)
{
	int i = 0;
	int fd = -1;
	for (; i < g_iteration; i++) {
		fd = _rwt_open('w');
		_rwt_write(fd, buf, nbytes);
		_rwt_close(fd);

		fd = _rwt_open('r');
		_rwt_read(fd, buf, nbytes);
		_rwt_close(fd);
	}
}

static void w_test(void *buf, size_t nbytes)
{
	int i = 0;
	int fd = -1;
	for (; i < g_iteration; i++) {
		fd = _rwt_open('w');
		_rwt_write(fd, buf, nbytes);
		_rwt_close(fd);
	}
}

static void r_test(char *buf, size_t nbytes)
{
	int fd = -1;
	int total_write = 0;
	int ret = 0;
	fd = open(g_filepath, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd == -1) {
		printf("%s errno: %d\n", __FUNCTION__, errno);
		exit(EXIT_FAILURE);
	}
	while (total_write < nbytes) {
		ret = write(fd, buf + total_write, nbytes - total_write);
		if (ret == 0 || ret == -1) {
			close(fd);
			printf("Write Fail In %s(%d)\n", __FUNCTION__, errno);
			exit(EXIT_FAILURE);
		}
		total_write += ret;
	}
	close(fd);

	int i = 0;
	for (; i < g_iteration; i++) {
		fd = _rwt_open('r');
		_rwt_read(fd, buf, nbytes);
		_rwt_close(fd);
	}
}

void f_rw_performance(void)
{
	char buf[g_size];

	switch (g_operation) {
	case TEST_OPT_WRITEREAD:
		wr_test(buf, sizeof(buf));
		break;
	case TEST_OPT_WRITE:
		r_test(buf, sizeof(buf));
		break;
	case TEST_OPT_READ:
		w_test(buf, sizeof(buf));
		break;
	default:
		return;
	}
	printf("run_time: %d ms\n", g_total_time);
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int fs_rw_test_main(int argc, char *argv[])
#endif
{
	printf("FS R/W Performance Test!\n");
	g_total_time = 0;
	if (argc <= 5) {
		printf("%s", USAGE);
		return 0;
	}

	g_filepath = argv[1];
	g_size = atoi(argv[2]);
	g_iteration = atoi(argv[3]);
	g_operation = atoi(argv[4]);
	g_printopt = argv[5][0];

	if (g_operation < 0 || g_operation > 3 ||
		g_size > FS_RW_SIZE_MAX || g_size < 0 ||
		g_iteration < 1 || g_iteration > FS_RW_ITERATION_MAX) {
		printf("%s", USAGE);
		return -1;
	}

	if (strlen(argv[5]) != 1 ||
		(g_printopt != 'y' && g_printopt != 'n')) {
		printf("%s", USAGE);
		return -1;
	}

	f_rw_performance();

	return 0;
}
