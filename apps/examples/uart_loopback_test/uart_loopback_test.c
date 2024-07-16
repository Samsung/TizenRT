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
#include <sys/types.h>
#include <sys/stat.h>
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifndef CONFIG_UART_LOOPBACK_PORT
#define CONFIG_UART_LOOPBACK_PORT 2
#endif

#define UART_DEV_PATH		"/dev/ttyS%d"

#define TEST_STR		"1234567890abcdefghijklmnopqrstuvwxyz"
#define TEST_STR_LEN	37

static int uart_tx_loop(void)
{
	int fd = 0;
	char port[20] = {'\0'};
	sprintf(port, UART_DEV_PATH, CONFIG_UART_LOOPBACK_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open Tx UART port : %s\n", port);
		return -1;
	}

	usleep(300);

	char test_data[TEST_STR_LEN] = TEST_STR;
	write(fd, (void *)test_data, TEST_STR_LEN);
	printf("SEND SIZE:   %d\n", TEST_STR_LEN);
	printf("SEND UART:   %s\n", TEST_STR);

	close(fd);
	return 0;
}

static int uart_rx_loop(void)
{
	int fd = 0;
	char port[20] = {'\0'};
	volatile uint8_t ch;
	ssize_t ret_size;
	int remain_size;

	sprintf(port, UART_DEV_PATH, CONFIG_UART_LOOPBACK_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open Tx UART port : %s\n", port);
		return -1;
	}

	char test_data[TEST_STR_LEN] = TEST_STR;
	char read_buf[TEST_STR_LEN];
	char *read_ptr = &read_buf;
	int read_total_size = 0;

	remain_size = TEST_STR_LEN;

	while (0 < remain_size) {
		ret_size = read(fd, (void *)read_ptr, remain_size);
		remain_size -= ret_size;
		read_ptr += ret_size;
		read_total_size += ret_size;
	}
	printf("RECIVE SIZE: %d\n", read_total_size);
	printf("RECIVE UART: %s\n", read_buf);


	if (strncmp(read_buf, TEST_STR, TEST_STR_LEN) == 0) {
		printf("UART LOOPBACK TEST: PASSED\n");
	} else {
		printf("UART LOOPBACK TEST: FAILED (It does not match)\n");
	}

	close(fd);
	return 0;
}

/****************************************************************************
 * Public functions
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int uart_loopback_test_main(int argc, char **argv)
#endif
{
	pthread_t tx_tid, rx_tid;
	pthread_addr_t retval;
	printf("######################### UART loopback test START #########################\n");

	if (pthread_create(&rx_tid, NULL, (pthread_startroutine_t)uart_rx_loop, NULL) < 0) {
		printf("Failed to create rx pthread for running logs\n");
		return -1;
	}

	if (pthread_create(&tx_tid, NULL, (pthread_startroutine_t)uart_tx_loop, NULL) < 0) {
		printf("Failed to create tx pthread for running logs\n");
		pthread_cancel(rx_tid);
		return -1;
	}

	pthread_join(tx_tid, &retval);
	if (retval < 0) {
		printf("uart tx thread exit with error %d\n", retval);
		pthread_cancel(rx_tid);
	}

	pthread_join(rx_tid, NULL);

	printf("######################### UART loopback test END ###########################\n");

	return 0;
}
