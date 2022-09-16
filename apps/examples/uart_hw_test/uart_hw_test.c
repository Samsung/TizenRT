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
/**
 * @file uart_hw_test.c
 * @brief This is test application for uart hw.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <sched.h>
#ifndef CONFIG_DISABLE_POLL
#include <poll.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define UART_DEV_PATH		"/dev/ttyS%d"

#define UART_TEST_COUNT		100

#define UART_TEST_START_VAL	0xaa
#define UART_TEST_VAL		0x55
#define UART_TEST_STOP_VAL	0xbb

#define UART_POLL_TIMEOUT_MS	2000

#define TEST_FILE_PATH		"/mnt/uart_test_file.txt"
#define TEST_FILE_LEN		500
#define TEST_FILE_STR		"abcdefghijklmnopqrstuvwxyz"
#define TEST_FILE_STR_LEN	26

#define HANDSHAKE_INTERVAL	100		/* msecs */

int iperf_main(int argc, char **argv);

struct pthread_arg {
	int argc;
	char **argv;
};

/****************************************************************************
 * Private functions
 ****************************************************************************/
static int poll_read(int fd, void *buf, int len, int timeout)
{
#ifndef CONFIG_DISABLE_POLL
	struct pollfd fds[1];
	fds[0].fd = fd;
	fds[0].events = POLLIN;
	poll(fds, 1, timeout);
	if (fds[0].revents & POLLIN) {
		return read(fd, buf, len);
	} else {
		return -1;
	}
#else
	return read(fd, buf, len);
#endif
}

static void uart_hw_rx_test(int fd)
{
	volatile uint8_t ch;

	printf("test_uart_rx: Test START\n");

	/* Loop until we receive the start value */
	do {
		read(fd, (void *)&ch, 1);
	} while (ch != UART_TEST_START_VAL);

	do {
		ch = 0;
		read(fd, (void *)&ch, 1);
		write(fd, (void *)&ch, 1);
	} while (ch != UART_TEST_STOP_VAL);

	printf("test_uart_rx: Test COMPLETE\n");
}

static void uart_hw_tx_test(int fd)
{
	volatile char ch;

	printf("test_uart_tx: Test START\n");

	/* Send test start value */
	ch = UART_TEST_START_VAL;
	write(fd, (void *)&ch, 1);

	/* Send test value for "cnt" times */
	ch = UART_TEST_VAL;
	for (int i = 0; i < UART_TEST_COUNT; i++) {
		write(fd, (void *)&ch, 1);
	}

	/* Send stop value to indicate end of test */
	ch = UART_TEST_STOP_VAL;
	write(fd, (void *)&ch, 1);

	printf("test_uart_tx: Test COMPLETE\n");
}

static void configure_tty(int fd, int baud)
{
	struct termios ttyio;
	memset(&ttyio, 0, sizeof(ttyio));

	tcgetattr(fd, &ttyio);

	/* set baudrate */
	cfsetispeed(&ttyio, baud);
	cfsetospeed(&ttyio, baud);

	ttyio.c_cflag &= ~CSIZE;
	ttyio.c_cflag |= CS8;
	ttyio.c_cflag &= ~CSTOPB;
	ttyio.c_cflag &= ~(PARENB | PARODD);

	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &ttyio);
}

static void run_background_console_logs(void)
{
	while (1) {
		char ch = 'a';
		while (ch <= 'z') {
			putchar(ch++);
			usleep(10);
		}
	}
}

static void run_background_fs_ops(void)
{
	struct stat st;
	int cnt, total;
	char buf[TEST_FILE_STR_LEN];

	while (1) {

		if (stat(TEST_FILE_PATH, &st) == 0) {
			unlink(TEST_FILE_PATH);
		}
		usleep(10);

		int fd = open(TEST_FILE_PATH, O_RDWR | O_CREAT);

		if (fd < 0){
			printf("FAIL to create test file %s\n", TEST_FILE_PATH);
			continue;
		}
		usleep(10);

		cnt = total = 0;
		while (total < TEST_FILE_LEN) {
			cnt = write(fd, TEST_FILE_STR, TEST_FILE_STR_LEN);
			if (cnt < 0) {
				printf("FAIL to write %d bytes to test file %s\n", TEST_FILE_STR_LEN, TEST_FILE_PATH);
				break;
			}
			total += cnt;
		}
		usleep(10);

		lseek(fd, 0, SEEK_SET);
		usleep(10);

		while (total > 0) {
			cnt = read(fd, buf, TEST_FILE_STR_LEN);
			if (cnt < 0) {
				printf("FAIL to read %d bytes from test file %s\n", TEST_FILE_STR_LEN, TEST_FILE_PATH);
				break;
			}
			total -= cnt;
		}
		usleep(10);

		close(fd);
		usleep(10);
	}
}

static void handshake(int fd, char c)
{
	volatile char ch;
	do {
		ch = 0;
		poll_read(fd, (void *)&ch, 1, HANDSHAKE_INTERVAL);
	} while (ch != c);
	usleep(1000);
	write(fd, (void *)&ch, 1);
	usleep(1000);
}

static void perform_uart_tests(int fd)
{
	int bauds[] = {115200, 38400, 19200, 9600, 4800};
	for (int i = 0; i < sizeof(bauds) / sizeof(int); i++) {
		configure_tty(fd, bauds[i]);
		printf("BAUD = %d\n\r", bauds[i]);
		handshake(fd, 't');
		uart_hw_tx_test(fd);
		handshake(fd, 'r');
		uart_hw_rx_test(fd);
		handshake(fd, 'x');
	}
}

/****************************************************************************
 * Public functions
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int uart_hw_test_main(int argc, char **argv)
#endif
{
	int fd = 0;
	int p = 0;
	char port[20] = {'\0'};
	pthread_t tid;
	pid_t t_sr, t_cl;
	char *args[5] = {NULL, NULL, NULL, NULL, NULL};

	printf("Enter UART port number\n");
	p = getchar();
	p = atoi((const char *)&p);

	sprintf(port, UART_DEV_PATH, p);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open UART port : %s\n", port);
		return -1;
	}

	printf("Scenario 0: Running UART test only\n\r");
	perform_uart_tests(fd);

	printf("Scenario 1: Running UART test with background FS ops\n\r");
	if (pthread_create(&tid, NULL, (pthread_startroutine_t)run_background_fs_ops, NULL) < 0) {
		printf("Failed to create pthread for running logs\n\r");
		goto fail_after_open;
	}
	if (pthread_detach(tid) < 0) {
		printf("Failed to detach pthread for running logs\n\r");
		pthread_cancel(tid);
		goto fail_after_open;
	}
	perform_uart_tests(fd);
	pthread_cancel(tid);

	printf("Scenario 2: Running UART test with background logs\n\r");
	if (pthread_create(&tid, NULL, (pthread_startroutine_t) run_background_console_logs, NULL) < 0) {
		printf("Failed to create pthread for running logs\n\r");
		goto fail_after_open;
	}
	if (pthread_detach(tid) < 0) {
		printf("Failed to detach pthread for running logs\n\r");
		pthread_cancel(tid);
		goto fail_after_open;
	}
	perform_uart_tests(fd);
	pthread_cancel(tid);

	printf("Scenario 3: Running UART test with background iperf test\n\r");

	/* Start iperf server */
	args[0] = "-s";
	t_sr = task_create("iperf server", 125, 4096, iperf_main, args);
	if (t_sr < 0) {
		printf("Failed to create iperf server thread\n\r");
		goto fail_after_open;
	}

	/* Start iperf client and transmit data to localhost for 60 secs */
	args[0] = "-c";
	args[1] = "127.0.0.1";
        args[2] = "-t";
        args[3] = "60"; 
	t_cl = task_create("iperf client", 125, 4096, iperf_main, args);
	if (t_cl < 0) {
		printf("Failed to create iperf client thread\n\r");
		task_delete(t_sr);
		goto fail_after_open;
	}
	perform_uart_tests(fd);
	kill(t_sr, SIGKILL);
	kill(t_cl, SIGKILL);

	printf("######################### UART HW test COMPLETE #########################\n");
fail_after_open:
	close(fd);
	unlink(TEST_FILE_PATH);
	return 0;
}
