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

/// @file libc_stdlib.c
/// @brief Test Case Example for Libc Stdlib API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include "tc_internal.h"

#define NVAL1 1000
#define NVAL2 12
#define ITOA_VAL 12
#define LVAL1 65537
#define LVAL2 100000
#define LLVAL1 65537123
#define LLVAL2 100000000
#define BUFF_SIZE 33
#define DECIMAL 10
#define HEXADECIMAL 16
#define BINARY 2

/**
* @fn                   :compare
* @description          :Function for tc_libc_stdlib_qsort
* @return               :int
*/
static int compare(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

/**
* @fn                   :tc_abs_labs_llabs
* @brief                :Returns the absolute value of parameter
* @Scenario             :Returns the absolute value of parameter
* API's covered         :abs, labs, llabs
* Preconditions         :None
* Postconditions        :None
* @return               :void
*/
static void tc_libc_stdlib_abs_labs_llabs(void)
{
	int val;
	int ret_chk;
	long int lval;
	long int lret_chk;
	long long int llval;
	long long int llret_chk;

	/* val is a positive number, ret_chk should be positive */

	val = NVAL1;
	ret_chk = abs(val);
	TC_ASSERT_EQ("abs", ret_chk, NVAL1);

	/* val is a negative number, ret_chk should be positive */

	val = -NVAL2;
	ret_chk = abs(val);
	TC_ASSERT_EQ("abs", ret_chk, NVAL2);

	/* lval is a positive number, lret_chk should be positive */

	lval = LVAL1;
	lret_chk = labs(lval);
	TC_ASSERT_EQ("labs", lret_chk, LVAL1);

	/* lval is a negative number, lret_chk should be positive */

	lval = -LVAL2;
	lret_chk = labs(lval);
	TC_ASSERT_EQ("labs", lret_chk, LVAL2);

	/* llVal is a positive number, llret_chk should be positive */

	llval = LLVAL1;
	llret_chk = llabs(llval);
	TC_ASSERT_EQ("llabs", llret_chk, LLVAL1);

	/* llVal is a negative number, llret_chk should be positive */

	llval = -LLVAL2;
	llret_chk = llabs(llval);
	TC_ASSERT_EQ("llabs", llret_chk, LLVAL2);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_imaxabs
* @brief                :Calculate the absolute value of the argument of the appropriate integer type
* @Scenario             :Compute the absolute value of the argument of the appropriate integer type for the function
* API's covered         :imaxabs
* Preconditions         :None
* Postconditions        :None
* @return               :void
*/
static void tc_libc_stdlib_imaxabs(void)
{
	intmax_t val = -NVAL1;
	intmax_t ret_chk = imaxabs(val);
	TC_ASSERT_EQ("imaxabs", ret_chk, NVAL1);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_itoa
* @brief                :Convert integer to string
* @Scenario             :Converts an integer value to a null-terminated string using the specified base and
*                        stores the result in the array given by str parameter.
* API's covered         :itoa
* Preconditions         :None
* Postconditions        :None
* @return               :void
*/
static void tc_libc_stdlib_itoa(void)
{
	/* ITOA_VAL = 12 */

	int val = ITOA_VAL;
	char buffer[BUFF_SIZE];

	/* conversion in decimal */

	itoa(val, buffer, DECIMAL);
	TC_ASSERT_EQ("itoa", strcmp(buffer, "12"), 0);

	/* conversion in hexadecimal */

	itoa(val, buffer, HEXADECIMAL);
	TC_ASSERT_EQ("itoa", strcmp(buffer, "c"), 0);

	/* conversion in binary */

	itoa(val, buffer, BINARY);
	TC_ASSERT_EQ("itoa", strcmp(buffer, "1100"), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_qsort
* @brief                :Sorts the elements of the array
* @Scenario             :Sorts the num elements of the array pointed to by base, each element size bytes long,
*                        using the compar function to determine the order.
* API's covered         :qsort
* Preconditions         :None
* Postconditions        :None
* @return               :void
*/
static void tc_libc_stdlib_qsort(void)
{
	/* random values filled in array for sorting */
	int value_arr[] = { 40, 10, 100, 90, 20, 25 };
	int data_idx = 0, arr_length;
	arr_length = (sizeof(value_arr) / sizeof(int));
	qsort(value_arr, arr_length, sizeof(int), compare);
	for (data_idx = 0; data_idx < arr_length; data_idx++) {
		if (data_idx != arr_length - 1) {
			TC_ASSERT_LEQ("qsort", value_arr[data_idx], value_arr[data_idx + 1]);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_rand
* @brief                :Returns a pseudo-random integral number
* @Scenario             :Returns a pseudo-random integral number in the range between 0 and RAND_MAX.
* API's covered         :rand, frand1, frand3, nrand, fgenerate1
* Preconditions         :None
* Postconditions        :None
* @return               :void
*/
static void tc_libc_stdlib_rand(void)
{
	int ret_chk = ERROR;

	ret_chk = rand();
	TC_ASSERT_GEQ("rand", ret_chk, 0);
	TC_ASSERT_LT("rand", ret_chk, MAX_RAND);

	ret_chk = rand();
	TC_ASSERT_GEQ("rand", ret_chk, 0);
	TC_ASSERT_LT("rand", ret_chk, MAX_RAND);

	ret_chk = rand();
	TC_ASSERT_GEQ("rand", ret_chk, 0);
	TC_ASSERT_LT("rand", ret_chk, MAX_RAND);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_strtol
* @brief                :converts the string to a long integer value
* @Scenario             :The strtol() function converts the initial part of the string in nptr to a long integer value
*                        according to the given base, which must be between 2 and 36 inclusive, or be the special value 0.
* API's covered         :strtol
* Preconditions         :None
* Postconditions        :None
* @return               :void
*/
static void tc_libc_stdlib_strtol(void)
{
	/* random string used for conversion */
	char str_lnum[] = "2001 60c0c0 -1101110100110100100000 0x6fffff";
	char *end_ptr;
	long int ret_chk;

	/* ret_chk = 2001 for string 2001 in decimal */

	ret_chk = strtol(str_lnum, &end_ptr, DECIMAL);
	TC_ASSERT_EQ("strtol", ret_chk, 2001);

	/* ret_chk = 6340800 for string 60c0c0 in hexadecimal */

	ret_chk = strtol(end_ptr, &end_ptr, HEXADECIMAL);
	TC_ASSERT_EQ("strtol", ret_chk, 6340800);

	/* ret_chk = -3624224 for string -1101110100110100100000 in binary */

	ret_chk = strtol(end_ptr, &end_ptr, BINARY);
	TC_ASSERT_EQ("strtol", ret_chk, -3624224);

	/* ret_chk = 7340031 for string 0x6fffff with base value 0 */

	ret_chk = strtol(end_ptr, NULL, 0);
	TC_ASSERT_EQ("strtol", ret_chk, 7340031);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_strtoll
* @brief                :converts the string to a long long integer value
* @Scenario             :The strtoll() function works just like the strtol() function but returns a long long integer value.
* API's covered         :strtoll
* Preconditions         :None
* Postconditions        :None
* @return               :void
*/
static void tc_libc_stdlib_strtoll(void)
{
	char str_llnum[] = "1856892505 17b00a12b -01100011010110000010001101100 0x6fffff";
	char *end_ptr;
	long long int ret_chk;

	/* ret_chk = 1856892505 for string 1856892505 in decimal */

	ret_chk = strtoll(str_llnum, &end_ptr, DECIMAL);
	TC_ASSERT_EQ("strtoll", ret_chk, 1856892505);

	/* ret_chk = 6358606123 for string 17b00a12b in hexadecimal */

	ret_chk = strtoll(end_ptr, &end_ptr, HEXADECIMAL);
	TC_ASSERT_EQ("strtoll", ret_chk, 6358606123);

	/* ret_chk = -208340076 for string -01100011010110000010001101100 in binary */

	ret_chk = strtoll(end_ptr, &end_ptr, BINARY);
	TC_ASSERT_EQ("strtoll", ret_chk, -208340076);

	/* ret_chk = 7340031 for string 0x6fffff with base 0 */

	ret_chk = strtoll(end_ptr, NULL, 0);
	TC_ASSERT_EQ("strtoll", ret_chk, 7340031);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_strtoul
* @brief                :converts the string to a unsigned long integer value
* @Scenario             :The strtoul() function converts the initial part of the string in nptr to an unsigned long int value
*                        according to the given base, which must be between 2 and 36 inclusive, or be the special value 0.
* API's covered         :strtoul
* Preconditions         :None
* Postconditions        :None
* @return               :void
*/
static void tc_libc_stdlib_strtoul(void)
{
	char str_ulnum[] = "201 60 1100 0x6f";
	char *end_ptr;
	unsigned long int ret_chk;

	/* ret_chk = 201 for string 201 in decimal */

	ret_chk = strtoul(str_ulnum, &end_ptr, DECIMAL);
	TC_ASSERT_EQ("strtoul", ret_chk, 201);

	/* ret_chk = 96 for string 60 in hexadecimal */

	ret_chk = strtoul(end_ptr, &end_ptr, HEXADECIMAL);
	TC_ASSERT_EQ("strtoul", ret_chk, 96);

	/* ret_chk = 12 for string 1100 in binary */

	ret_chk = strtoul(end_ptr, &end_ptr, BINARY);
	TC_ASSERT_EQ("strtoul", ret_chk, 12);

	/* ret_chk = 111 for string 0x6f with base 0 */

	ret_chk = strtoul(end_ptr, NULL, 0);
	TC_ASSERT_EQ("strtoul", ret_chk, 111);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_strtoull
* @brief                :converts the string to a unsigned long long integer value
* @Scenario             :The strtoull() function works just like the strtoul() function but returns an unsigned long long int value.
* API's covered         :strtoull
* Preconditions         :None
* Postconditions        :None
* @return               :void
*/
static void tc_libc_stdlib_strtoull(void)
{
	char str_ullnum[] = "250068492 7b06af00 1100011011110101010001100000 0x6fffff";
	char *end_ptr;
	unsigned long long int ret_chk;

	/* ret_chk = 250068492 for string 250068492 in decimal */

	ret_chk = strtoull(str_ullnum, &end_ptr, DECIMAL);
	TC_ASSERT_EQ("strtoull", ret_chk, 250068492);

	/* ret_chk = 2064035584 for string 7b06af00 in hexadecimal */

	ret_chk = strtoull(end_ptr, &end_ptr, HEXADECIMAL);
	TC_ASSERT_EQ("strtoull", ret_chk, 2064035584);

	/* ret_chk = 208622688 for string 1100011011110101010001100000 in binary */

	ret_chk = strtoull(end_ptr, &end_ptr, BINARY);
	TC_ASSERT_EQ("strtoull", ret_chk, 208622688);

	/* ret_chk = 7340031 for string 0x6fffff with base 0 */

	ret_chk = strtoull(end_ptr, NULL, 0);
	TC_ASSERT_EQ("strtoull", ret_chk, 7340031);

	TC_SUCCESS_RESULT();
}

static void tc_libc_stdlib_strtod(void)
{
	char target[100] = "1234.56abcd";
	char *pos = NULL;
	double ret_chk = 0;

	ret_chk = strtod(target, &pos);
	TC_ASSERT_EQ("atoi", ret_chk, 1234.56);

	TC_SUCCESS_RESULT();
}

static void tc_libc_stdlib_atoi(void)
{
	/* random string used for conversion */
	char str_lnum[] = "2016";
	int ret_chk;
	/* ret_chk = 2001 for string 2001 in decimal */
	ret_chk = atoi(str_lnum);
	TC_ASSERT_EQ("atoi", ret_chk, 2016);

	TC_SUCCESS_RESULT();
}

static void tc_libc_stdlib_atol(void)
{
	/* random string used for conversion */
	char str_lnum[] = "20162015";
	long int ret_chk;

	ret_chk = atol(str_lnum);
	TC_ASSERT_EQ("atol", ret_chk, 20162015);

	TC_SUCCESS_RESULT();
}

static void tc_libc_stdlib_atoll(void)
{
	/* random string used for conversion */
	char str_lnum[] = "201620152014";
	long long int ret_chk;

	ret_chk = atoll(str_lnum);
	TC_ASSERT_EQ("atoll", ret_chk, 201620152014);

	TC_SUCCESS_RESULT();
}

static void tc_libc_stdlib_srand(void)
{
	unsigned int test_num = 1234;
	int first_result[3];
	int second_result[3];
	int result_iter;

	srand(test_num);
	for (result_iter = 0; result_iter < 3; result_iter++) {
		first_result[result_iter] = rand();
	}

	srand(test_num);
	for (result_iter = 0; result_iter < 3; result_iter++) {
		second_result[result_iter] = rand();
	}

	for (result_iter = 0; result_iter < 3; result_iter++) {
		TC_ASSERT_EQ("srand", first_result[result_iter], second_result[result_iter]);
	}

	TC_SUCCESS_RESULT();
}

static void tc_libc_stdlib_atof(void)
{
	char target[100] = "1234.56abcd";
	double ret_chk = 0;

	ret_chk = atof(target);
	TC_ASSERT_EQ("atof", ret_chk, 1234.56);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_stdlib
 ****************************************************************************/
int libc_stdlib_main(void)
{
	tc_libc_stdlib_abs_labs_llabs();
	tc_libc_stdlib_imaxabs();
	tc_libc_stdlib_itoa();
	tc_libc_stdlib_qsort();
	tc_libc_stdlib_rand();
	tc_libc_stdlib_strtol();
	tc_libc_stdlib_strtoll();
	tc_libc_stdlib_strtoull();
	tc_libc_stdlib_strtoul();
	tc_libc_stdlib_strtod();
	tc_libc_stdlib_atoi();
	tc_libc_stdlib_atol();
	tc_libc_stdlib_atoll();
	tc_libc_stdlib_srand();
	tc_libc_stdlib_atof();

	return 0;
}
