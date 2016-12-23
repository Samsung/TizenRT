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

#define BUFF_SIZE 32

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
* @fn                   :asprintf_func
* @description          :Function for tc_libc_stdio_asprintf
* @return               :int
*/
static int asprintf_func(const char *format, ...)
{
	int ret_chk;
	char *buffer;
	const char *pname = "Example";
	va_list args;
	va_start(args, format);
	ret_chk = avsprintf(&buffer, format, args);
	va_end(args);

	if (buffer == NULL) {
		printf("tc_libc_stdio_avsprintf FAIL\n");
		return ERROR;
	}
	if (ret_chk != strlen(pname)) {
		printf("tc_libc_stdio_avsprintf FAIL\n");
		free(buffer);
		return ERROR;
	}
	if (strcmp(pname, buffer) != 0) {
		printf("tc_libc_stdio_avsprintf FAIL, Error No: %d\n", errno);
		free(buffer);
		return ERROR;
	}

	free(buffer);
	return OK;
}

/**
* @fn                   :vfprintf_func
* @description          :Function for tc_libc_stdio_vfprintf
* @return               :int
*/
static int vfprintf_func(const char *format, ...)
{
	int ret_chk;
	const char *pstr = "tc_libc_stdio_vfprintf";
	va_list args;
	va_start(args, format);
	ret_chk = vfprintf(stdout, format, args);
	va_end(args);

	if (ret_chk != strlen(pstr)) {
		printf(" FAIL\n");
		return ERROR;
	}

	return OK;
}

/**
* @fn                   :avsprintf_func
* @description          :Function for tc_libc_stdio_avsprintf
* @return               :int
*/
static int avsprintf_func(const char *format, ...)
{
	int ret_chk;
	char *buffer;
	const char *pname = "Example";
	va_list args;
	va_start(args, format);
	ret_chk = avsprintf(&buffer, format, args);
	va_end(args);

	if (buffer == NULL) {
		printf("tc_libc_stdio_avsprintf FAIL\n");
		return ERROR;
	}
	if (ret_chk != strlen(pname)) {
		printf("tc_libc_stdio_avsprintf FAIL\n");
		free(buffer);
		return ERROR;
	}
	if (strcmp(pname, buffer) != 0) {
		printf("tc_libc_stdio_avsprintf FAIL, Error No: %d\n", errno);
		free(buffer);
		return ERROR;
	}

	free(buffer);
	return OK;
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
* @fn                   :vsprintf_func
* @description          :function for tc_libc_stdio_vsscanf_vsprintf
* @return               :int
*/
static int vsprintf_func(const char *format, ...)
{
	int ret_chk = ERROR;
	char buffer[BUFF_SIZE];
	char *pname = "test";
	va_list args;
	va_start(args, format);
	ret_chk = vsprintf(buffer, format, args);
	va_end(args);
	if (ret_chk != strlen(pname)) {
		printf("tc_libc_stdio_vsscanf_vsprintf vsprintf FAIL");
		return ERROR;
	}
	if (strncmp(pname, buffer, strlen(pname)) != OK) {
		printf("tc_libc_stdio_vsscanf_vsprintf sprintf FAIL");
		return ERROR;
	}
	return OK;
}

/**
* @fn                   :vprintf_func
* @description          :Function for tc_libc_stdio_vprintf
* @return               :int
*/
static int vprintf_func(const char *format, ...)
{
	int val;
	va_list args;
	va_start(args, format);
	val = vprintf(format, args);
	va_end(args);

	return val;
}

/**
* @fn                   :vsnprintf_func
* @description          :Function for tc_libc_stdio_vsnprintf
* @return               :int
*/
static int vsnprintf_func(const char *format, ...)
{
	char buffer[BUFF_SIZE];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, BUFF_SIZE, format, args);
	va_end(args);

	return strlen(buffer);
}

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
	int ret_chk = ERROR;
	char *str1 = "tc_libc_stdio_flush ";
	char *str2 = "PASS\n";

	fprintf(stdout, str1);
	fprintf(stdout, str2);
	ret_chk = fflush(stdout);
	/* Flushes the output buffer of stream, cannot be checked for scenario TC, as output is flushed on screen
	   flused output is checked manually on screen */
	if (ret_chk != OK) {
		printf("tc_libc_stdio_fflush FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	total_pass++;
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
static void tc_libc_stdio_avsprintf(void)
{
	int ret_chk = ERROR;
	const char *pname = "Example";
	ret_chk = avsprintf_func("%s", pname);

	if (ret_chk != OK) {
		printf("tc_libc_stdio_avsprintf FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_stdio_avsprintf PASS\n");
	total_pass++;
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
	int ret_chk = ERROR;
	char *pname = "Example";
	char buff[BUFF_SIZE];

	ret_chk = snprintf(buff, BUFF_SIZE, "%s", pname);
	if (ret_chk != strlen(pname)) {
		printf("tc_libc_stdio_snprintf FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	if (strncmp(pname, buff, strlen(pname)) != OK) {
		printf("tc_libc_stdio_snprintf FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_stdio_snprintf PASS\n");
	total_pass++;
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
	int ret_chk = ERROR;
	char *pname = "test";
	char char_arr[BUFF_SIZE], sz_pptr[BUFF_SIZE];

	ret_chk = sscanf(pname, "%s", char_arr);
	if (ret_chk == OK) {
		printf("tc_libc_stdio_sscanf_sprintf sscanf FAIL");
		total_fail++;
		RETURN_ERR;
	}
	if (strncmp(pname, char_arr, strlen(pname)) != OK) {
		printf("tc_libc_stdio_sscanf_sprintf sscanf FAIL");
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = sprintf(sz_pptr, "%s", pname);
	if (ret_chk != strlen(pname)) {
		printf("tc_libc_stdio_sscanf_sprintf sprintf FAIL");
		total_fail++;
		RETURN_ERR;
	}
	if (strncmp(pname, sz_pptr, strlen(pname)) != OK) {
		printf("tc_libc_stdio_sscanf_sprintf sprintf FAIL");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_stdio_sscanf_sprintf PASS\n");
	total_pass++;
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
static void tc_libc_stdio_vsscanf_vsprintf(void)
{
	int ret_chk = ERROR;
	char *pname = "test";
	char char_arr[BUFF_SIZE];

	vsscanf_func(pname, "%s", char_arr);
	if (strncmp(pname, char_arr, strlen(pname)) != OK) {
		printf("tc_libc_stdio_vsscanf_vsprintf vsscanf FAIL");
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = vsprintf_func("%s", pname);
	if (ret_chk != OK) {
		printf("tc_libc_stdio_vsscanf_vsprintf vsprintf FAIL");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_stdio_vsscanf_vsprintf PASS\n");
	total_pass++;
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
static void tc_libc_stdio_vprintf(void)
{
	int ret_chk = ERROR;
	char *pname = "tc_";

	ret_chk = vprintf_func("%s", pname);
	if (ret_chk != strlen(pname)) {
		printf("tc_libc_stdio_vprintf FAIL");
		total_fail++;
		RETURN_ERR;
	}

	printf("libc_stdio_vprintf PASS\n");
	total_pass++;
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
static void tc_libc_stdio_vsnprintf(void)
{
	int ret_chk = ERROR;
	char *pname = "Example";

	ret_chk = vsnprintf_func("%s", pname);
	if (ret_chk != strlen(pname)) {
		printf("tc_libc_stdio_vsnprintf FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_stdio_vsnprintf PASS\n");
	total_pass++;
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
	int ret_chk = ERROR;
	int ret_length;
	char *str_ptr = "tc_libc_stdio_puts PASS";
	ret_length = strlen(str_ptr) + 1;	/* appends a newline character ('\n'). */
	ret_chk = puts(str_ptr);
	/* scenario testcase is covered by checking the return value, which must be equal to characters written to standard output */
	if (ret_chk != ret_length) {
		printf("tc_libc_stdio_puts FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	total_pass++;
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
	if (strncmp(test_msg, result_msg, strlen(result_msg)) != 0) {
		total_fail++;
		printf("tc_libc_stdio_perror FAIL\n");
		RETURN_ERR;
	}
#endif
	perror("Perror Test : Err Msg - No data available");
	total_pass++;
	printf("tc_libc_stdio_perror PASS\n");
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
	const char *str = "tc_libc_stdio_printf";

	ret_chk = printf("%s", str);
	if (ret_chk != strlen(str)) {
		printf(" FAIL %d %d\n", strlen(str), ret_chk);
		total_fail++;
		RETURN_ERR;
	}
	printf(" PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_libc_stdio_vfprintf
* @scenario             :check the return value which indicates the number of puts
* API's covered         :vfprintf
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_stdio_vfprintf(void)
{
	int ret_chk = ERROR;
	const char *pstr = "tc_libc_stdio_vfprintf";

	ret_chk = vfprintf_func("%s", pstr);
	if (ret_chk != OK) {
		total_fail++;
		RETURN_ERR;
	}

	printf(" PASS\n");
	total_pass++;
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
	int ret_chk = ERROR;
	const char *pname = "Example";
	ret_chk = asprintf_func("%s", pname);

	if (ret_chk != OK) {
		printf("tc_libc_stdio_asprintf FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_stdio_asprintf PASS\n");
	total_pass++;
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
	int ret;

	ret = putchar('t');
	if (ret < 0) {
		printf("tc_libc_stdio_putchar FAIL%d\n");
		total_fail++;
		RETURN_ERR;
	}

	printf("c_libc_stdio_putchar PASS\n");
	total_pass++;
}

/****************************************************************************
 * Name: libc_stdio
 ****************************************************************************/

int libc_stdio_main(void)
{
	tc_libc_stdio_flush();
	tc_libc_stdio_avsprintf();
	tc_libc_stdio_snprintf();
	tc_libc_stdio_sscanf_sprintf();
	tc_libc_stdio_vsscanf_vsprintf();
	tc_libc_stdio_puts();
	tc_libc_stdio_vprintf();
	tc_libc_stdio_vsnprintf();
	tc_libc_stdio_perror();
	tc_libc_stdio_printf();
	tc_libc_stdio_vfprintf();
	tc_libc_stdio_asprintf();
	tc_libc_stdio_putchar();

	return 0;
}
