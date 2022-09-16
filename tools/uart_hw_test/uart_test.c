/*******************************************************************
*
* Copyright 2022 Samsung Electronics All Rights Reserved.
*
* File Name : trap.c
* Description: Receive RAM and/or Userfs dump using UART
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
******************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

//#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#include <poll.h>

/****************************************************************************
 * Preprocessor definitions
 ****************************************************************************/
#define UART_PATH_STR		"/dev/ttyUSB%c"
#define TTYPATH_LEN		25

#define UART_TEST_COUNT		100

#define UART_TEST_START_VAL	0xaa
#define UART_TEST_VAL		0x55
#define UART_TEST_STOP_VAL	0xbb

#define UART_MAX_DELAY_MS	80
#define UART_POLL_TIMEOUT_MS	2000

#define HANDSHAKE_INTERVAL	100	/* msecs */
#define HANDSHAKE_CNT_MAX	10
/****************************************************************************
 * Private Variables
 ****************************************************************************/
static int pass, fail;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void configure_tty(int fd, int baud)
{
	struct termios ttyio;
	memset(&ttyio, 0, sizeof(ttyio));

	ttyio.c_cflag       = baud | CS8 | CLOCAL | CREAD;
	ttyio.c_oflag       = 0;
	ttyio.c_lflag       = 0;
	ttyio.c_cc[VTIME]   = 0;
	ttyio.c_cc[VMIN]    = 1;

	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &ttyio);
	fcntl(fd, F_SETFL, FNDELAY);
}

static int poll_read(int fd, void *buf, int len, int timeout)
{
	struct pollfd fds[1];
	fds[0].fd = fd;
	fds[0].events = POLLIN;
	poll(fds, 1, timeout);
	if (fds[0].revents & POLLIN) {
		return read(fd, buf, len);
	} else {
		return -1;
	}
}

/* UART Rx loss check test
 * 1. The target prints log on the console continuously
 * 2. The ubuntu app sends data on the micom uart
 * 3. The target verify rx data for data loss
 */
static void test_uart_rx(int fd, int baud)
{
	volatile int i = 0;
	volatile u_int8_t ch;

	ch = UART_TEST_START_VAL;
	write(fd, (void *)&ch, 1);

	/* Send the test bytes for cnt times */
	for (int x = 0; x < UART_TEST_COUNT; x++) {
		ch = UART_TEST_VAL;
		write(fd, (void *)&ch, 1);
		ch = 0;
		poll_read(fd, (void *)&ch, 1, HANDSHAKE_INTERVAL);
		if (ch == UART_TEST_VAL) {
			i++;
		}
	}

	ch = UART_TEST_STOP_VAL;
	write(fd, (void *)&ch, 1);
	read(fd, (void *)&ch, 1);


	if (UART_TEST_COUNT - i == 0) {
		printf("[BAUD %6d Rx] PASS\n", baud);
		pass++;
	} else {
		printf("[BAUD %6d Rx] FAIL Bytes lost = %d / %d\n", baud, UART_TEST_COUNT - i, UART_TEST_COUNT);
		fail++;
	}

	do {
	} while (poll_read(fd, (void *)&ch, 1, 200) >= 0);
}

/* UART Tx byte gap time test
 * 1. The target prints log on the console continuously
 * 2. The target sends data on the micom data uart
 * 3. The ubuntu app checks tx packet gap using time api
 */
static void test_uart_tx(int fd, int baud)
{
	volatile u_int8_t ch;
	double tm_diff, max_diff = 0;
	struct timeval  tv1, tv2;
	int cnt = 0;
	int numfail = 0;

	do {
		if (poll_read(fd, (void *)&ch, 1, UART_POLL_TIMEOUT_MS) <= 0) {
			numfail = UART_TEST_COUNT;
			goto result;
		}
	} while (ch != UART_TEST_START_VAL);

	/* Read bytes till we receive end of test value */
	do {
		gettimeofday(&tv1, NULL);
		if (poll_read(fd, (void *)&ch, 1, UART_POLL_TIMEOUT_MS) <= 0) {
			numfail += UART_TEST_COUNT - cnt;
			goto result;
		}
		if (ch == UART_TEST_VAL) {
			gettimeofday(&tv2, NULL);
			cnt++;

			tm_diff = ((tv2.tv_sec - tv1.tv_sec) * 1000) + ((tv2.tv_usec - tv1.tv_usec) / 1000);
			if (tm_diff > UART_MAX_DELAY_MS) {
				numfail++;
				if (tm_diff > max_diff) {
					max_diff = tm_diff;
				}
			}
		}
	} while (ch != UART_TEST_STOP_VAL);

result:
	if (numfail) {
		fail++;
		printf("[BAUD %6d Tx] FAIL (Num out of spec = %d / %d Max delay = %fms)\n", baud, numfail, UART_TEST_COUNT, max_diff);
	} else {
		pass++;
		printf("[BAUD %6d Tx] PASS\n", baud);
	}

	do {
	} while (poll_read(fd, (void *)&ch, 1, 200) >= 0);
}

static void handshake(int fd, char c)
{
	volatile char ch;
	do {
		ch = 0;
		write(fd, (void *)&c, 1);
		poll_read(fd, (void *)&ch, 1, HANDSHAKE_INTERVAL);
		usleep(300000);
	} while(ch != c);
}

static void perform_uart_tests(int fd)
{
	int bauds[] = {B115200, B38400, B19200, B9600, B4800};
	int baudvals[] = {115200, 38400, 19200, 9600, 4800};
	for (int i = 0; i < sizeof(bauds) / sizeof(int); i++) {
		configure_tty(fd, bauds[i]);
		handshake(fd, 't');
		test_uart_tx(fd, baudvals[i]);
		handshake(fd, 'r');
		test_uart_rx(fd, baudvals[i]);
		handshake(fd, 'x');
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int main(int argc, char *argv[])
{
	int fd;
	char p = 0;
	char *dev_file = UART_PATH_STR;
	char tty_path[TTYPATH_LEN] = {0, };

	printf("Enter UART port number : ");
	p = getchar();
	snprintf(tty_path, TTYPATH_LEN, dev_file, p);
	if ((fd = open(tty_path, O_RDWR | O_SYNC)) == -1) {
		printf("%s open failed!!\nPlease enter correct device port\n", tty_path);
		goto dev_open_err;
	}
	printf("Opened UART port : %s\n", tty_path);

	pass = 0;
	fail = 0;

	printf("######################## UART Test Start #############################\n");
	printf("######################################################################\n");
	printf("Test Scenario 0: UART test only\n");
	printf("######################################################################\n");
	perform_uart_tests(fd);
	printf("\n######################################################################\n");
	printf("Test Scenario 1: UART test with background FS ops\n");
	printf("######################################################################\n");
	perform_uart_tests(fd);
	printf("\n######################################################################\n");
	printf("Test Scenario 2: UART test with background console logs\n");
	printf("######################################################################\n");
	perform_uart_tests(fd);
	printf("\n######################################################################\n");
	printf("Test Scenario 3: UART test with background iperf\n");
	printf("######################################################################\n");
	perform_uart_tests(fd);
	printf("######################################################################\n");
	printf("################## UART Test End [PASS: %d, FAIL: %d] #################\n", pass, fail);

	close(fd);
dev_open_err:
	return 0;
}
