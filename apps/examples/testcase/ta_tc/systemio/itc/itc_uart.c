/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file iotbus_uart.h

/// @brief Test Case Iotbus APIs for UART

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <termios.h>

#include <iotbus_uart.h>
#include "itc_internal.h"
#include "iotbus_error.h"

#define MICROSECOND 1000000
#ifdef CONFIG_ARCH_CHIP_STM32
#define DEVPATH "/dev/ttyS1"
#else                            // artik
#define DEVPATH "/dev/ttyS2"
#endif

/**
* @testcase         itc_iotbus_uart_init_stop_p
* @brief            initializes and closes uart_context
* @scenario         initializes and closes uart_context
* @apicovered       iotbus_uart_init, iotbus_uart_stop
* @precondition     none
* @postcondition    none
*/
void itc_iotbus_uart_init_stop_p(void)
{
    int ret = IOTBUS_ERROR_NONE;
    iotbus_uart_context_h h_uart = iotbus_uart_init(DEVPATH);
    TC_ASSERT_NOT_NULL("iotbus_uart_init", h_uart);

    ret = iotbus_uart_stop(h_uart);
    TC_ASSERT_EQ("iotbus_uart_stop", ret, IOTBUS_ERROR_NONE);

    TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_iotbus_uart_set_baudrate_p
* @brief            sets uart baud rate
* @scenario         sets uart baud rate
* @apicovered       iotbus_uart_set_baudrate
* @precondition     initializes uart_context
* @postcondition    closes uart_context
*/
void itc_iotbus_uart_set_baudrate_p(void)
{
    int i_baudrate = 115200;
    int ret = IOTBUS_ERROR_NONE;
    iotbus_uart_context_h h_uart = iotbus_uart_init(DEVPATH);
    TC_ASSERT_NOT_NULL("iotbus_uart_init", h_uart);

	ret = iotbus_uart_set_baudrate(h_uart, i_baudrate);
	TC_ASSERT_EQ_CLEANUP("iotbus_uart_set_baudrate", ret, IOTBUS_ERROR_NONE, iotbus_uart_stop(h_uart));

    ret = iotbus_uart_stop(h_uart);
    TC_ASSERT_EQ("iotbus_uart_stop", ret, IOTBUS_ERROR_NONE);

    TC_SUCCESS_RESULT();;
}

/**
* @testcase         itc_iotbus_uart_set_mode_p
* @brief            sets byte size, parity bit and stop bits
* @scenario         sets byte size, parity bit and stop bits
* @apicovered       iotbus_uart_set_mode
* @precondition     initializes uart_context
* @postcondition    closes uart_context
*/
void itc_iotbus_uart_set_mode_p(void)
{
	int i_bytesize = 8;
	int i_stop_bits = 1;
	int ret = IOTBUS_ERROR_NONE;
	int mode[] = { IOTBUS_UART_PARITY_NONE, IOTBUS_UART_PARITY_EVEN, IOTBUS_UART_PARITY_ODD };
	int i_modes = sizeof(mode) / sizeof(int);
	int index = 0;
	iotbus_uart_context_h h_uart = iotbus_uart_init(DEVPATH);
	TC_ASSERT_NOT_NULL("iotbus_uart_init", h_uart);

	for (index = 0; index < i_modes; index++) {
		ret = iotbus_uart_set_mode(h_uart, i_bytesize, mode[index], i_stop_bits);
		TC_ASSERT_EQ_CLEANUP("iotbus_uart_set_mode", ret, IOTBUS_ERROR_NONE, iotbus_uart_stop(h_uart));
	}

	ret = iotbus_uart_stop(h_uart);
	TC_ASSERT_EQ("iotbus_uart_stop", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_iotbus_uart_set_flowcontrol_p
* @brief            set flow control settings
* @scenario         set flow control settings
* @apicovered       iotbus_uart_set_flowcontrol
* @precondition     initializes uart_context
* @postcondition    closes uart_context
*/
void itc_iotbus_uart_set_flowcontrol_p(void)
{
	int i_size = 4;
	int ret = IOTBUS_ERROR_NONE;
	int rtscts[4][2] = { {1, 0}, {0, 1}, {1, 1}, {0, 0} };
	int index = 0;
	iotbus_uart_context_h h_uart = iotbus_uart_init(DEVPATH);
	TC_ASSERT_NOT_NULL("iotbus_uart_init", h_uart);

	for (index = 0; index < i_size; index++) {
		ret = iotbus_uart_set_flowcontrol(h_uart, rtscts[index][0], rtscts[index][1]);
		TC_ASSERT_EQ_CLEANUP("iotbus_uart_set_flowcontrol", ret, IOTBUS_ERROR_NONE, iotbus_uart_stop(h_uart));
	}

	ret = iotbus_uart_stop(h_uart);
	TC_ASSERT_EQ("iotbus_uart_stop", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_iotbus_uart_write_read_p
* @brief            write and read data over uart bus
* @scenario         write and read data over uart bus
* @apicovered       iotbus_uart_write, iotbus_uart_read
* @precondition     initializes uart_context
* @postcondition    closes uart_context
*/
void itc_iotbus_uart_write_read_p(void)
{
    int ret = IOTBUS_ERROR_NONE;
    char szInputText[32] = "UART READ/WRITE ITC TESTING!";
    char szOutputText[32];
    iotbus_uart_context_h h_uart = iotbus_uart_init(DEVPATH);
    TC_ASSERT_NOT_NULL("iotbus_uart_init", h_uart);

	ret = iotbus_uart_write(h_uart, szInputText, sizeof(szInputText));
	TC_ASSERT_EQ_CLEANUP("iotbus_uart_write", ret < 0, false, iotbus_uart_stop(h_uart));

    usleep(MICROSECOND);

	ret = iotbus_uart_read(h_uart, szOutputText, sizeof(szOutputText));
	TC_ASSERT_EQ_CLEANUP("iotbus_uart_read", ret < 0, false, iotbus_uart_stop(h_uart));

	TC_ASSERT_EQ_CLEANUP("iotbus_uart_read", strcmp(szInputText, szOutputText), 0, iotbus_uart_stop(h_uart));

    ret = iotbus_uart_stop(h_uart);
    TC_ASSERT_EQ("iotbus_uart_stop", ret, IOTBUS_ERROR_NONE);

    TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_iotbus_uart_flush_p
* @brief            flushes uart buffer
* @scenario         flushes uart buffer
* @apicovered       iotbus_uart_flush
* @precondition     initializes uart_context
* @postcondition    closes uart_context
*/
void itc_iotbus_uart_flush_p(void)
{
    int ret = IOTBUS_ERROR_NONE;
    iotbus_uart_context_h h_uart = iotbus_uart_init(DEVPATH);
    TC_ASSERT_NOT_NULL("iotbus_uart_init", h_uart);

	ret = iotbus_uart_flush(h_uart);
	TC_ASSERT_EQ_CLEANUP("iotbus_uart_flush", ret, IOTBUS_ERROR_NONE, iotbus_uart_stop(h_uart));

    ret = iotbus_uart_stop(h_uart);
    TC_ASSERT_EQ("iotbus_uart_stop", ret, IOTBUS_ERROR_NONE);

    TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: environ
 ****************************************************************************/

int itc_uart_main(void)
{
#ifndef CONFIG_SERIAL_TERMIOS
    SYSIO_ITC_UART_PRINT("IOTBUS Not Supported\n");
#else
    itc_iotbus_uart_init_stop_p();
    itc_iotbus_uart_set_baudrate_p();
    itc_iotbus_uart_set_mode_p();
    itc_iotbus_uart_set_flowcontrol_p();
    itc_iotbus_uart_write_read_p();
    itc_iotbus_uart_flush_p();

#endif

    return 0;
}
