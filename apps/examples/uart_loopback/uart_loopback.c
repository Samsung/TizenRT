/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#ifndef CONFIG_DISABLE_POLL
#include <poll.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifndef CONFIG_EXAMPLES_UART_LOOPBACK_PORT
#define CONFIG_EXAMPLES_UART_LOOPBACK_PORT 2
#endif

#define UART_DEV_PATH		"/dev/ttyS%d"

#define UART_POLL_TIMEOUT_MS	10000

#define TEST_STR		"1234567890abcdefghijklmnopqrstuvwxyz"
#define TEST_STR_LEN	37

static bool is_running;

static int uart_tx_loop(void)
{
	printf("UART TX THREAD START\n");
	int fd = 0;
	int tx_test_count = 0;
	char port[20] = {'\0'};
	sprintf(port, UART_DEV_PATH, CONFIG_EXAMPLES_UART_LOOPBACK_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open Tx UART(%d):\n", get_errno());
		return -1;
	}

	while (is_running) {
		tx_test_count++;
		usleep(200*1000);

		char test_data[TEST_STR_LEN] = TEST_STR;
		write(fd, (void *)test_data, TEST_STR_LEN);
		printf("SEND UART(%d): %s\n", tx_test_count, TEST_STR, TEST_STR_LEN);
	}

	close(fd);
	return 0;
}

static int uart_rx_loop(void)
{
	int fd = 0;
	char port[20] = {'\0'};
	ssize_t ret_size;
	int remain_size;
	char *read_ptr;
	int rx_test_count = 0;
	char read_buf[TEST_STR_LEN];

	printf("UART RX THREAD START\n");

	sprintf(port, UART_DEV_PATH, CONFIG_EXAMPLES_UART_LOOPBACK_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open Rx UART(%d):\n", get_errno());
		return -1;
	}

	while (is_running) {
		rx_test_count++;
		read_ptr = &read_buf;
		remain_size = TEST_STR_LEN;

		while (0 < remain_size) {

#ifndef CONFIG_DISABLE_POLL
			struct pollfd fds[1];
			fds[0].fd = fd;
			fds[0].events = POLLIN;
			if (poll(fds, 1, UART_POLL_TIMEOUT_MS) < 0) {
				printf("Fail to poll(%d):\n", get_errno());
				close(fd);
				return -1;
			}
			if (!(fds[0].revents & POLLIN)) {
				printf("RESULT(%d): FAILED (Timeout)\n");
				continue;
			}
#endif
			ret_size = read(fd, (void *)read_ptr, remain_size);
			remain_size -= ret_size;
			read_ptr += ret_size;
		}
		printf("RECEIVE(%d): %s\n", rx_test_count, read_buf);
		if (strncmp(read_buf, TEST_STR, TEST_STR_LEN) == 0) {
			printf("RESULT(%d): PASSED\n", rx_test_count);
		} else {
			printf("RESULT(%d): FAILED (It does not match)\n", rx_test_count);
		}
	}

	close(fd);
	return 0;
}

static void help_func(void)
{
	printf("usage: uart_loopback start/stop \n\n");
	printf("This test is to check if the same data is read as it was written through uart.\n");
	printf("For this test, you need to short-circuit the UART TX PIN and the UART RX PIN.\n");
	printf("This default uart port is /dev/ttyS2, you can change CONFIG_EXAMPLES_UART_LOOPBACK_PORT.\n\n");
	printf("  start\t\t Start uart loopback test\n");
	printf("  stop\t\t stop uart loopback test\n");	
}

/****************************************************************************
 * Public functions
 ****************************************************************************/
static int uart_loopback_task(int argc, char *argv[])
{
	int ret;
	pthread_t tx_tid;
	pthread_t rx_tid;

	printf("######################### UART loopback test START #########################\n");

	if (pthread_create(&rx_tid, NULL, (pthread_startroutine_t)uart_rx_loop, NULL) < 0) {
		printf("Fail to create rx pthread(%d):\n", get_errno());
		return -1;
	}
	(void)pthread_setname_np(rx_tid, "uart_loopback_rx");

	if (pthread_create(&tx_tid, NULL, (pthread_startroutine_t)uart_tx_loop, NULL) < 0) {
		printf("Fail to create tx pthread(%d):\n", get_errno());
		pthread_cancel(rx_tid);
		return -1;
	}
	(void)pthread_setname_np(tx_tid, "uart_loopback_tx");

	ret = pthread_join(rx_tid, NULL);
	if (ret != 0) {
		printf("Fail to join rx thread(%d):\n", ret);
	}
	ret = pthread_join(tx_tid, NULL);
	if (ret != 0) {
		printf("Fail to join tx thread(%d):\n", ret);
	}

	printf("######################### UART loopback test END ###########################\n");
}


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int uart_loopback_main(int argc, char **argv)
#endif
{
	int ret;
	int pid;

	if (argc != 2 || strncmp(argv[1], "help", 5) == 0) {
		help_func();
		return 0;
	}

	if (strncmp(argv[1], "start", 6) == 0) {
		if (is_running) {
			printf("uart_loopback is already running\n");
			return 0;
		}

		is_running = true;

		pid = task_create("uart_loopback_task", 100, 1024, uart_loopback_task, NULL);
		if (pid < 0) {
			printf("Fail to create uart_loopback_task task(errno %d)\n", get_errno());
			is_running = false;
			return -1;
		}

	} else if (strncmp(argv[1], "stop", 5) == 0) {
		if (!is_running) {
			printf("uart_loopback is not running\n");
			return 0;
		}

		is_running = false;

	} else {
		help_func();
	}

	return 0;
}
