/*******************************************************************
*
* Copyright 2023 Samsung Electronics All Rights Reserved.
*
* File Name : uart_test_windows.c
* Description: test receive and send of data using UART
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <windows.h>
#include <winnt.h>
#include <minwindef.h>

/****************************************************************************
 * Preprocessor definitions
 ****************************************************************************/
#define UART_PATH_STR		"\\\\.\\COM%s"  /* for COM port greater than equal to 10 */
#define UART_ALT_PATH_STR	"COM%s"         /* for COM port less than 10 */
#define TTYPATH_LEN		25
#define MAX_PORTNO_LENGTH 	10

#define UART_TEST_COUNT		100

#define UART_TEST_START_VAL	0xaa
#define UART_TEST_VAL		0x55
#define UART_TEST_STOP_VAL	0xbb

#define UART_MAX_DELAY_MS	80        /* milliseconds */
#define UART_POLL_TIMEOUT_MS	2000  /* milliseconds */

#define HANDSHAKE_INTERVAL	100	/* milliseconds */
#define HANDSHAKE_CNT_MAX	10
/****************************************************************************
 * Private Variables
 ****************************************************************************/
static int pass, fail;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void configure_handle(HANDLE handle, int baud)
{
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    //Get the current serial port configuration
    if (!GetCommState(handle, &dcbSerialParams)) {
        //Handle the error condition
        printf("Failed to get serial port configuration\n");
        return;
    }

    //set the desired configuration parameters
    dcbSerialParams.BaudRate = baud; 
    dcbSerialParams.ByteSize = 8; 
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.StopBits = ONESTOPBIT;

    //apply the new configuration
    if (!SetCommState(handle, &dcbSerialParams)) {
        //Handle the error condition
        printf("Failed to set serial port configuration\n");
        return;
    }

    //Set the timeouts
    COMMTIMEOUTS timeouts = { 0 }; 
    timeouts.ReadIntervalTimeout = MAXDWORD; 
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    //apply the new timeouts
    if (!SetCommTimeouts(handle, &timeouts)) {
        //handle the error condition
        printf("Failed to set serial port timeouts\n");
        return;
    }

    FlushFileBuffers(handle);

    DWORD mode;
	GetConsoleMode(handle, &mode);
	mode |= 0x0001;
	SetConsoleMode(handle, mode);
}

static int poll_read(HANDLE handle, void *buf, int len, int timeout)
{
	DWORD bytesRead = 0;
	COMMTIMEOUTS timeouts;
	DWORD startTime = GetTickCount();

	// Set the timeout values for non-blocking read
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = timeout;
	SetCommTimeouts(handle, &timeouts);

	BOOL result = ReadFile(handle, buf, len, &bytesRead, NULL);

	if (result) {
		return bytesRead;
	}

	return -1;

}

/* UART Rx loss check test
 * 1. The target prints log on the console continuously
 * 2. The ubuntu app sends data on the micom uart
 * 3. The target verify rx data for data loss
 */
static void test_uart_rx(HANDLE handle, int baud)
{
	volatile int i = 0;
	volatile unsigned char ch;

	ch = UART_TEST_START_VAL;
	WriteFile(handle, (void *)&ch, 1, NULL, NULL);

	/* Send the test bytes for cnt times */
	for (int x = 0; x < UART_TEST_COUNT; x++) {
		ch = UART_TEST_VAL;
		WriteFile(handle, (void *)&ch, 1, NULL, NULL);
		ch = 0;
		poll_read(handle, (void *)&ch, 1, HANDSHAKE_INTERVAL);
		if (ch == UART_TEST_VAL) {
			i++;
		}
	}

	ch = UART_TEST_STOP_VAL;
	WriteFile(handle, (void *)&ch, 1, NULL, NULL);
	ReadFile(handle, (void *)&ch, 1, NULL, NULL);


	if (UART_TEST_COUNT - i == 0) {
		printf("[BAUD %6d Rx] PASS\n", baud);
		pass++;
	} else {
		printf("[BAUD %6d Rx] FAIL Bytes lost = %d / %d\n", baud, UART_TEST_COUNT - i, UART_TEST_COUNT);
		fail++;
	}

	PurgeComm(handle, PURGE_RXCLEAR);
}

/* UART Tx byte gap time test
 * 1. The target prints log on the console continuously
 * 2. The target sends data on the micom data uart
 * 3. The ubuntu app checks tx packet gap using time api
 */
static void test_uart_tx(HANDLE handle, int baud)
{
    volatile unsigned char ch;
	double tm_diff, max_diff = 0;
	DWORD bytesRead = 0;
	DWORD bytesWritten = 0;
	LARGE_INTEGER freq, start, end;
	int cnt = 0;
	int numfail = 0;

	// Get the performance counter frequency
	QueryPerformanceFrequency(&freq);


	int ret = 0;
	do {
		ret = poll_read(handle, (void *)&ch, 1, 100) ;
		if (ret <= 0) {
			printf("flag 1 , %d\n", ret);
			numfail = UART_TEST_COUNT;
			goto result;
		}
	} while (ch != UART_TEST_START_VAL);

	/* Read bytes till we receive end of test value */
	do {
		QueryPerformanceCounter(&start);
		if (poll_read(handle, (void *)&ch, 1, UART_POLL_TIMEOUT_MS) <= 0) {
			printf("flag 2");
			numfail += UART_TEST_COUNT - cnt;
			goto result;
		}
		if (ch == UART_TEST_VAL) {
			QueryPerformanceCounter(&end);
			cnt++;

			tm_diff = ((end.QuadPart - start.QuadPart) * 1000.0) / freq.QuadPart  ;
			if (tm_diff > UART_MAX_DELAY_MS) {
				printf("flag 3");
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

	PurgeComm(handle, PURGE_RXCLEAR);
}

static void handshake(HANDLE handle, char c)
{
	volatile char ch;
	int ret;
	do {
		ch = 0;
        WriteFile(handle, (void *)&c, 1, NULL, NULL);
        ret = poll_read(handle, (void *)&ch, 1, HANDSHAKE_INTERVAL);
		Sleep(300);
	} while(ch != c);
}

static void perform_uart_tests(HANDLE handle)
{
	int bauds[] = {CBR_115200, CBR_38400, CBR_19200, CBR_9600, CBR_4800};
	for (int i = 0; i < sizeof(bauds) / sizeof(int); i++) {
		configure_handle(handle, bauds[i]);
		handshake(handle, 't');
		test_uart_tx(handle, bauds[i]);
		handshake(handle, 'r');
		test_uart_rx(handle, bauds[i]);
		handshake(handle, 'x');
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int main(int argc, char *argv[])
{
	char p[MAX_PORTNO_LENGTH];
	char portPath[TTYPATH_LEN] = {0, };

	printf("Enter UART port number : ");
	gets(p);
	if (strlen(p) < 2) 
		snprintf(portPath, TTYPATH_LEN, UART_ALT_PATH_STR, p);
	else 
		snprintf(portPath, TTYPATH_LEN, UART_PATH_STR, p);

	HANDLE handle;
    handle  = CreateFile(portPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (handle == INVALID_HANDLE_VALUE) {
        printf("%s open failed!!\nPlease enter correct device port\n", portPath);
		goto dev_open_err;
    }

	printf("Opened UART port : %s\n", portPath);

	pass = 0;
	fail = 0;

	printf("######################## UART Test Start #############################\n");
	printf("######################################################################\n");
	printf("Test Scenario 0: UART test only\n");
	printf("######################################################################\n");
	perform_uart_tests(handle);
	printf("\n######################################################################\n");
	printf("Test Scenario 1: UART test with background FS ops\n");
	printf("######################################################################\n");
	perform_uart_tests(handle);
	printf("\n######################################################################\n");
	printf("Test Scenario 2: UART test with background console logs\n");
	printf("######################################################################\n");
	perform_uart_tests(handle);
	printf("\n######################################################################\n");
	printf("Test Scenario 3: UART test with background iperf\n");
	printf("######################################################################\n");
	perform_uart_tests(handle);
	printf("######################################################################\n");
	printf("################## UART Test End [PASS: %d, FAIL: %d] #################\n", pass, fail);

	CloseHandle(handle);
dev_open_err:
	return 0;
}
