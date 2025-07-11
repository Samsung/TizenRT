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

#define UART_DEV_PATH           "/dev/ttyS%d"
#define UART_DEV_PATH_MAX_LEN   12

#define UART_POLL_TIMEOUT_MS	10000

#define TEST_STR		"1234567890abcdefghijklmnopqrstuvwxyz"
#define TEST_STR_LEN	37

static bool is_running;

static int uart_tx_loop(pthread_addr_t *arg)
{
	int fd = 0;
	int tx_test_count = 0;
	char port[UART_DEV_PATH_MAX_LEN] = {'\0'};
	int port_num = (int)arg;

	snprintf(port, UART_DEV_PATH_MAX_LEN, UART_DEV_PATH, port_num);

	printf("UART TX THREAD START [Port: %s]\n", port);

	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open %s Tx UART(%d):\n", port, get_errno());
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

static int uart_rx_loop(pthread_addr_t *arg)
{
	int fd = 0;
	char port[UART_DEV_PATH_MAX_LEN] = {'\0'};
	ssize_t ret_size;
	int remain_size;
	char *read_ptr;
	int rx_test_count = 0;
	char read_buf[TEST_STR_LEN];
	int port_num = (int)arg;

	snprintf(port, UART_DEV_PATH_MAX_LEN, UART_DEV_PATH, port_num);

	printf("UART RX THREAD START [Port: %s]\n", port);

	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open %s Rx UART(%d):\n", port, get_errno());
		return -1;
	}

	while (is_running) {
		rx_test_count++;
		read_ptr = (char *)&read_buf;
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
				printf("RESULT(%d): FAILED (Timeout)\n", rx_test_count);
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
	printf("usage: uart_loopback start/stop [option]\n\n");
	printf("This test is to check if the same data is read as it was written through uart.\n");
	printf("For this test, you need to short-circuit the UART TX PIN and the UART RX PIN.\n");
	printf("This default uart port is /dev/ttyS2, you can change CONFIG_EXAMPLES_UART_LOOPBACK_PORT.\n\n");
	printf("  start\t\t Start uart loopback test\n");
	printf("  stop\t\t stop uart loopback test\n\n");
	printf("[option]\n");
	printf("  -p --port\t Set uart ttyS[n] port number (default: %d)\n", CONFIG_EXAMPLES_UART_LOOPBACK_PORT);
}

/****************************************************************************
 * Public functions
 ****************************************************************************/
static int uart_loopback_task(int argc, char *argv[])
{
	int ret;
	pthread_t tx_tid;
	pthread_t rx_tid;
	int port_num = CONFIG_EXAMPLES_UART_LOOPBACK_PORT;

	if (argc == 2) {
		port_num = atoi(argv[1]);
	}

	printf("######################### UART loopback test START #########################\n");

	if (pthread_create(&rx_tid, NULL, (pthread_startroutine_t)uart_rx_loop, (void *)port_num) < 0) {
		printf("Fail to create rx pthread(%d):\n", get_errno());
		return -1;
	}
	(void)pthread_setname_np(rx_tid, "uart_loopback_rx");

	if (pthread_create(&tx_tid, NULL, (pthread_startroutine_t)uart_tx_loop, (void *)port_num) < 0) {
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
	return ret;
}


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int uart_loopback_main(int argc, char **argv)
#endif
{
	int pid;
	char *parm[2] = {NULL, };

	if (argc < 2 || strncmp(argv[1], "help", 5) == 0) {
		help_func();
		return 0;
	}

	if (strncmp(argv[1], "start", 6) == 0) {
		if (is_running) {
			printf("uart_loopback is already running\n");
			return 0;
		}

		is_running = true;

		if (argc == 4 && (strncmp(argv[2], "-p", 3) == 0 || strncmp(argv[2], "--port", 7) == 0)) {
			parm[0] = argv[3];
			parm[1] = NULL;
		}

		pid = task_create("uart_loopback_task", 100, 1024, uart_loopback_task, (FAR char *const *)parm);
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
