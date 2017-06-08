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

/// @file libc_stdio.c

/// @brief Test Case Example for Libc Stdio API

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/types.h>
#include "tc_internal.h"

#define BUFF_SIZE 128

const char *printable_chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
* @fn                   :tc_libc_stdio_flush
* @brief                :The fflush() function flushes the output buffer
* @scenario             :The fflush() function flushes the output buffer of a stream.
* API's covered         :fflush
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_flush(void)
{
	int ret_chk;

	/* Without fflush, stream output will printed when buffer meet '\n'.
	 * So, stream output will printed after 3 sec */

	printf("fflush test : without fflush\n");
	fprintf(stdout, "%s", "You can see this line after 3 sec");
	sleep(3);
	printf("\n");

	/* Using fflush, stream output will printed when fflush is called.
	 * So, stream output will printed right now */

	printf("fflush test : using fflush\n");
	fprintf(stdout, "%s", "You can see this line right now");
	ret_chk = fflush(stdout);
	TC_ASSERT_EQ("fflush", ret_chk, OK);
	sleep(3);
	printf("\n");

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_stdio_avsprintf
* @brief                :write formatted output to a dynamically allocated string
* @scenario             :function shall write formatted output to a dynamically allocated string
* API's covered         :avsprintf
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_avsprintf(const char *format, ...)
{
	int ret_chk;
	char *buffer;
	va_list args;
	va_start(args, format);
	ret_chk = avsprintf(&buffer, format, args);
	va_end(args);
	TC_ASSERT_NOT_NULL("avsprintf", buffer);
	TC_ASSERT_EQ_CLEANUP("avsprintf",
						 ret_chk, strlen(printable_chars),
						 get_errno(),
						 TC_FREE_MEMORY(buffer));
	TC_ASSERT_EQ_CLEANUP("avsprintf",
						 strcmp(printable_chars, buffer), 0,
						 get_errno(),
						 TC_FREE_MEMORY(buffer));

	TC_FREE_MEMORY(buffer);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_stdio_snprintf
* @brief                :Write formatted output to a character array, up to a given maximum
* @scenario             :Write formatted output to a character array, up to a given maximum
* API's covered         :snprintf
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_snprintf(void)
{
	int ret_chk;
	char buffer[BUFF_SIZE];

	ret_chk = snprintf(buffer, BUFF_SIZE, "%s", printable_chars);
	TC_ASSERT_EQ("snprintf", ret_chk, strlen(printable_chars));
	TC_ASSERT_EQ("snprintf", strncmp(printable_chars, buffer, strlen(printable_chars)), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                  :tc_libc_stdio_sscanf_sprintf
* @brief               :Reads data from s and stores them according to parameter format. Composes a string     \
*                       with the same text that would be printed if format was used on printf
* @scenario            :Reads data from s and stores them according to parameter format into the locations     \
*                       given by the additional arguments, as if scanf was used, but reading from s instead    \
*                       of the standard input. Composes a string with the same text that would be printed if format \
*                       was used on printf, but instead of being printed, the content is stored as a C string          \
*                       in the buffer pointed by str
* API's covered        :sscanf, sprintf
* Preconditions        :none
* Postconditions       :none
* @return              :void
*/
static void tc_libc_stdio_sscanf_sprintf(void)
{
	int ret_chk;
	char char_arr[BUFF_SIZE], sz_pptr[BUFF_SIZE];

	ret_chk = sscanf(printable_chars, "%s", char_arr);
	TC_ASSERT_EQ("sscanf", ret_chk, 1);
	TC_ASSERT_EQ("sscanf", strncmp(printable_chars, char_arr, strlen(printable_chars)), -1);

	ret_chk = sscanf(printable_chars + 1, "%s", char_arr);
	TC_ASSERT_EQ("sscanf", ret_chk, 1);
	TC_ASSERT_EQ("sscanf", strncmp(printable_chars + 1, char_arr, strlen(printable_chars + 1)), 0);

	ret_chk = sprintf(sz_pptr, "%s", printable_chars);
	TC_ASSERT_EQ("sprintf", ret_chk, strlen(printable_chars));
	TC_ASSERT_EQ("sprintf", strncmp(printable_chars, sz_pptr, strlen(printable_chars)), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :vsscanf_func
* @description          :function for tc_libc_stdio_vsscanf_vsprintf
* @return               :NA
*/
static void vsscanf_func(const char *str, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsscanf(str, format, args);
	va_end(args);
}

/**
* @fn                   :tc_libc_stdio_vsscanf_vsprintf
* @brief                :Read formatted data from string into variable argument list
*                        Write formatted data from variable argument list to string
* @scenario             :Reads data from s and stores them according to parameter format into the
*                        locations pointed by the elements in the variable argument list identified by arg.
*                        Composes a string with the same text that would be printed if format was used on printf,
*                        but using the elements in the variable argument list identified by arg instead of
*                        additional function arguments and storing the resulting content as a C string in the
*                        buffer pointed by s.
* API's covered         :vsscanf, vsprintf
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_vsscanf_vsprintf(const char *format, ...)
{
	int ret_chk;
	char buffer[BUFF_SIZE];
	va_list args;

	vsscanf_func(printable_chars, "%s", buffer);
	TC_ASSERT_EQ("vsscanf", strncmp(printable_chars, buffer, strlen(printable_chars)), -1);

	vsscanf_func(printable_chars + 1, "%s", buffer);
	TC_ASSERT_EQ("vsscanf", strncmp(printable_chars + 1, buffer, strlen(printable_chars + 1)), 0);

	va_start(args, format);
	ret_chk = vsprintf(buffer, format, args);
	va_end(args);
	TC_ASSERT_EQ("vsprintf", ret_chk, strlen(printable_chars));
	TC_ASSERT_EQ("vsprintf", strncmp(printable_chars, buffer, strlen(printable_chars)), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_stdio_vprintf
* @brief                :Print formatted data from variable argument list to stdout.
* @scenario             :Writes the C string pointed by format to the standard output (stdout), replacing any
*                        format specifier in the same way as printf does, but using the elements in the variable
*                        argument list identified by arg instead of additional function arguments
* API's covered         :vprintf
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_vprintf(const char *format, ...)
{
	int ret_chk;
	va_list args;

	va_start(args, format);
	ret_chk = vprintf(format, args);
	va_end(args);
	TC_ASSERT_EQ("vprintf", ret_chk, strlen(printable_chars));

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_stdio_vsnprintf
* @brief                :Write formatted data from variable argument list to sized buffer
* @scenario             :Composes a string with the same text that would be printed if format was used on printf,
*                        but using the elements in the variable argument list identified by arg instead of
*                        additional function arguments and storing the resulting content as a C string in the
*                        buffer pointed by s (taking n as the maximum buffer capacity to fill).
* API's covered         :vsnprintf
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_vsnprintf(const char *format, ...)
{
	int ret_chk;
	char buffer[BUFF_SIZE];
	va_list args;

	va_start(args, format);
	ret_chk = vsnprintf(buffer, BUFF_SIZE, format, args);
	va_end(args);
	TC_ASSERT_EQ("vsnprintf", ret_chk, strlen(printable_chars));

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_stdio_puts
* @brief                :Writes the C string pointed by str to the standard output (stdout) and
*                        appends a newline character ('\n').
* @scenario             :The function begins copying from the address specified (str) until it reaches the      \
*                        terminating null character ('\0'). This terminating null-character is not copied to    \
*                        the stream.
* API's covered         :puts
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_puts(void)
{
	int ret_chk;
	int ret_length;

	/* appends a newline character ('\n'). */

	ret_length = strlen(printable_chars) + 1;

	/* scenario testcase is covered by checking the return value,
	 * which must be equal to characters written to standard output */

	ret_chk = puts(printable_chars);
	TC_ASSERT_EQ("puts", ret_chk, ret_length);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_stdio_perror
* @brief                :test perror prints errno well
* @scenario             :set errno to 61(ENODATA) and check that perror can print it
* API's covered         :perror
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_perror(void)
{
	char *result_msg = "No data available";
	const char *test_msg;

	set_errno(ENODATA);
#ifdef CONFIG_LIBC_STRERROR
	test_msg = strerror(get_errno());
	TC_ASSERT_EQ("strerror", strncmp(test_msg, result_msg, strlen(result_msg)), 0);
#endif
	perror("Perror Test : Err Msg - No data available");

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_stdio_printf
* @scenario             :check the return value which indicates the number of puts
* API's covered         :printf
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_printf(void)
{
	int ret_chk;
	ret_chk = printf("%s", printable_chars);
	TC_ASSERT_EQ("printf", ret_chk, strlen(printable_chars));

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_stdio_vfprintf
* @scenario             :check the return value which indicates the number of puts
* API's covered         :vfprintf
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_vfprintf(const char *format, ...)
{
	int ret_chk;
	va_list args;

	va_start(args, format);
	ret_chk = vfprintf(stdout, format, args);
	va_end(args);
	TC_ASSERT_EQ("vfprintf", ret_chk, strlen(printable_chars));

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_stdio_asprintf
* @brief                :write formatted output to a dynamically allocated string
* @scenario             :function shall write formatted output to a dynamically allocated string
* API's covered         :asprintf
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_asprintf(void)
{
	int ret_chk;
	char *buffer;

	ret_chk = asprintf(&buffer, "%s", printable_chars);

	TC_ASSERT_NOT_NULL("asprintf", buffer);
	TC_ASSERT_EQ_CLEANUP("asprintf",
						 ret_chk, strlen(printable_chars),
						 get_errno(),
						 TC_FREE_MEMORY(buffer));
	TC_ASSERT_EQ_CLEANUP("asprintf",
						 strcmp(printable_chars, buffer), 0,
						 get_errno(),
						 TC_FREE_MEMORY(buffer));

	TC_FREE_MEMORY(buffer);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_stdio_putchar
* @brief                :write formatted output to a dynamically allocated string
* @scenario             :function shall write formatted output to a dynamically allocated string
* API's covered         :putchar
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_putchar(void)
{
	int ret_chk;
	int c;

	/* 32( ) to 126(~) is printable ASCII code */

	for (c = 32; c <= 126; c++) {
		ret_chk = putchar((char)c);
		TC_ASSERT_EQ("putchar", ret_chk, c);
	}

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_stdio
 ****************************************************************************/

int libc_stdio_main(void)
{
	tc_libc_stdio_flush();
	tc_libc_stdio_avsprintf("%s", printable_chars);
	tc_libc_stdio_snprintf();
	tc_libc_stdio_sscanf_sprintf();
	tc_libc_stdio_vsscanf_vsprintf("%s", printable_chars);
	tc_libc_stdio_puts();
	tc_libc_stdio_vprintf("%s", printable_chars);
	tc_libc_stdio_vsnprintf("%s", printable_chars);
	tc_libc_stdio_perror();
	tc_libc_stdio_printf();
	tc_libc_stdio_vfprintf("%s", printable_chars);
	tc_libc_stdio_asprintf();
	tc_libc_stdio_putchar();

	return 0;
}

