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

/// @file tc_libc_stdlib.c
/// @brief Test Case Example for Libc Stdlib API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
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
#define QSORT_SMALL_ARRSIZE 6
#define QSORT_BIG_ARRSIZE 45
#define BSEARCH_ARRSIZE 10

/**
 *@fn					:thread_func
 *@description			:Function for tc_libc_stdlib_abort
 *@return				:void*
 */
static void *thread_func(void *arg)
{
	abort();
	return NULL;
}

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
 * @fn                   :tc_libc_stdlib_itoa
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

	itoa(-val, buffer, DECIMAL);
	TC_ASSERT_EQ("itoa", strcmp(buffer, "-12"), 0);

	/* conversion in hexadecimal */

	itoa(val, buffer, HEXADECIMAL);
	TC_ASSERT_EQ("itoa", strcmp(buffer, "c"), 0);

	/* conversion in binary */

	itoa(val, buffer, BINARY);
	TC_ASSERT_EQ("itoa", strcmp(buffer, "1100"), 0);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_stdlib_qsort
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
	/* qsort checks that the number of data is greater than 7 or not.
	  So tc checks with 7 data and 40 data for magic numbers.  */
	int qsort_smalldata[QSORT_SMALL_ARRSIZE] = { 40, 10, 100, 90, 20, 25 };
	int  qsort_smalldata1[QSORT_SMALL_ARRSIZE + 1] = { 1, 2, 3 };
	long lqsort_smalldata[QSORT_SMALL_ARRSIZE + 1] = { 10, 40, 10, 100, 90, 20, 25 };
	long lqsort_smalldata1[QSORT_SMALL_ARRSIZE + 1] = { 7, 6, 5, 4, 3, 2, 1 };
	int qsort_bigdata[QSORT_BIG_ARRSIZE] = { 16, 10, 27, 49, 18, 82, 27, 31, 11, 13, 101, 2, 99, 32, 51,
				72, 182, 939, 1, 61, 83, 5, 60, 131, 52, 39, 33, 127, 29, 19,
				12, 81, 281, 8, 931, 17, 111, 356, 14, 93, 20, 40, 30, 37, 73 };
	int data_idx;

	/* check that the number of data is smaller than 7 */
	qsort(qsort_smalldata, QSORT_SMALL_ARRSIZE, sizeof(int), compare);
	for (data_idx = 0; data_idx < QSORT_SMALL_ARRSIZE - 1; data_idx++) {
		TC_ASSERT_LEQ("qsort", qsort_smalldata[data_idx], qsort_smalldata[data_idx + 1]);
	}

	/* check that the number of data is equal to 7 */
	qsort(qsort_smalldata1, QSORT_SMALL_ARRSIZE + 1, sizeof(int), compare);
	for (data_idx = 0; data_idx < QSORT_SMALL_ARRSIZE; data_idx++) {
		TC_ASSERT_LEQ("qsort", qsort_smalldata1[data_idx], qsort_smalldata1[data_idx + 1]);
	}

	qsort(lqsort_smalldata, QSORT_SMALL_ARRSIZE + 1, sizeof(long), compare);
	for (data_idx = 0; data_idx < QSORT_SMALL_ARRSIZE; data_idx++) {
		TC_ASSERT_LEQ("qsort", lqsort_smalldata[data_idx], lqsort_smalldata[data_idx + 1]);
	}

	qsort(lqsort_smalldata1, QSORT_SMALL_ARRSIZE + 1, sizeof(long), compare);
	for (data_idx = 0; data_idx < QSORT_SMALL_ARRSIZE; data_idx++) {
		TC_ASSERT_LEQ("qsort", lqsort_smalldata1[data_idx], lqsort_smalldata1[data_idx + 1]);
	}

	/* check that the number of data is bigger than 40 */
	qsort(qsort_bigdata, QSORT_BIG_ARRSIZE, sizeof(int), compare);
	for (data_idx = 0; data_idx < QSORT_BIG_ARRSIZE - 1; data_idx++) {
		TC_ASSERT_LEQ("qsort", qsort_bigdata[data_idx], qsort_bigdata[data_idx + 1]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_stdlib_rand
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
	TC_ASSERT_LT("rand", ret_chk, RAND_MAX);

	ret_chk = rand();
	TC_ASSERT_GEQ("rand", ret_chk, 0);
	TC_ASSERT_LT("rand", ret_chk, RAND_MAX);

	ret_chk = rand();
	TC_ASSERT_GEQ("rand", ret_chk, 0);
	TC_ASSERT_LT("rand", ret_chk, RAND_MAX);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_stdlib_random
 * @brief                :random number generator
 * @Scenario             :Returns a pseudo-random long integer number in the range between 0 and LONG_MAX.
 * API's covered         :random
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_stdlib_random(void)
{
	long int ret_chk = ERROR;

	ret_chk = random();
	TC_ASSERT_GEQ("random", ret_chk, 0L);
	TC_ASSERT_LT("random", ret_chk, LONG_MAX);

	ret_chk = random();
	TC_ASSERT_GEQ("random", ret_chk, 0L);
	TC_ASSERT_LT("random", ret_chk, LONG_MAX);

	ret_chk = random();
	TC_ASSERT_GEQ("random", ret_chk, 0L);
	TC_ASSERT_LT("random", ret_chk, LONG_MAX);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_stdlib_strtol
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
	char str_lnum[] = "+2001 60c0c0 -1101110100110100100000 0x6fffff";
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
 * @fn                   :tc_libc_stdlib_strtoll
 * @brief                :converts the string to a long long integer value
 * @Scenario             :The strtoll() function works just like the strtol() function but returns a long long integer value.
 * API's covered         :strtoll
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_stdlib_strtoll(void)
{
	char str_llnum[] = "+1856892505 17b00a12b -01100011010110000010001101100 0x6fffff";
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
 * @fn                   :tc_libc_stdlib_strtoul
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
	char str_ulnum[] = "201 0x60 1100 0x6f";
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
 * @fn                   :tc_libc_stdlib_strtoull
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

/**
 * @fn                   :tc_libc_stdlib_strtod
 * @brief                :convert ASCII string to floating-point number
 * @Scenario             :convert ASCII string to floating-point number
 * API's covered         :strtod
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_stdlib_strtod(void)
{
	char *pos = NULL;
	double ret_chk = 0;
	const double inf = (1.0/0.0);

	ret_chk = strtod("1234.56abcd", &pos);
	TC_ASSERT_EQ("strtod", ret_chk, 1234.56);

	ret_chk = strtod("+1234.56abcd", &pos);
	TC_ASSERT_EQ("strtod", ret_chk, 1234.56);

	ret_chk = strtod("-1234.56abcd", &pos);
	TC_ASSERT_EQ("strtod", ret_chk, -1234.56);

	ret_chk = strtod("1.1E4abcd", &pos);
	TC_ASSERT_EQ("strtod", ret_chk, 1.1E4);

	ret_chk = strtod("+1.1E4abcd", &pos);
	TC_ASSERT_EQ("strtod", ret_chk, 1.1E4);

	ret_chk = strtod("-1.1E4abcd", &pos);
	TC_ASSERT_EQ("strtod", ret_chk, -1.1E4);

	ret_chk = strtod("   ", &pos);
	TC_ASSERT_EQ("strtod", ret_chk, 0.0);

	ret_chk = strtod("+1.1e+1040", &pos);
	TC_ASSERT_EQ("strtod", ret_chk, inf);

	ret_chk = strtod("+1.1e+1024", &pos);
	TC_ASSERT_EQ("strtod", ret_chk, inf);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_stdlib_atoi
 * @brief                :convert a string to an integer
 * @Scenario             :convert a string to an integer
 * API's covered         :atoi
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
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

/**
 * @fn                   :tc_libc_stdlib_atol
 * @brief                :convert a string to an long integer
 * @Scenario             :convert a string to an long integer
 * API's covered         :atol
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_stdlib_atol(void)
{
	/* random string used for conversion */
	char str_lnum[] = "20162015";
	long int ret_chk;

	ret_chk = atol(str_lnum);
	TC_ASSERT_EQ("atol", ret_chk, 20162015);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_stdlib_atoll
 * @brief                :convert a string to an long long integer
 * @Scenario             :convert a string to an long long integer
 * API's covered         :atoll
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_stdlib_atoll(void)
{
	/* random string used for conversion */
	char str_lnum[] = "201620152014";
	long long int ret_chk;

	ret_chk = atoll(str_lnum);
	TC_ASSERT_EQ("atoll", ret_chk, 201620152014);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_stdlib_srand
 * @brief                :pseudo-random number generator
 * @Scenario             :pseudo-random number generator
 * API's covered         :srand
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
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
/**
 * @fn                   :tc_libc_stdlib_sranddom
 * @brief                :pseudo-random number generator
 * @Scenario             :pseudo-random number generator
 * API's covered         :srandom
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_stdlib_srandom(void)
{
	unsigned int test_num = 1234;
	int first_result[3];
	int second_result[3];
	int result_iter;

	srandom(test_num);
	for (result_iter = 0; result_iter < 3; result_iter++) {
		first_result[result_iter] = random();
	}

	srandom(test_num);
	for (result_iter = 0; result_iter < 3; result_iter++) {
		second_result[result_iter] = random();
	}

	for (result_iter = 0; result_iter < 3; result_iter++) {
		TC_ASSERT_EQ("srandom", first_result[result_iter], second_result[result_iter]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_stdlib_atof
 * @brief                :Convert a string to a double
 * @Scenario             :Convert a string to a double
 * API's covered         :atof
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_stdlib_atof(void)
{
	char target[100] = "1234.56abcd";
	double ret_chk = 0;

	ret_chk = atof(target);
	TC_ASSERT_EQ("atof", ret_chk, 1234.56);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_stdlib_bsearch
 * @brief                :Binary search of a sorted array
 * @Scenario             :Binary search of a sorted array
 * API's covered         :bsearch
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_stdlib_bsearch(void)
{
	/* bsearch can find the result when data is ordered. So now using ordered data. */
	int test_data[BSEARCH_ARRSIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int key;
	int *search_result;

	/* check with valid key */
	key = 6;
	search_result = (int *)bsearch(&key, test_data, BSEARCH_ARRSIZE, sizeof(test_data[0]), compare);
	TC_ASSERT_EQ("bsearch", (*search_result) - (*test_data), 5);

	/* check with invalid key */
	key = 11;
	search_result = (int *)bsearch(&key, test_data, BSEARCH_ARRSIZE, sizeof(test_data[0]), compare);
	TC_ASSERT_EQ("bsearch", search_result, NULL);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_stdlib_abort
 * @brief                :Cause abnormal process termination
 * @Scenario             :Cause abnormal process termination
 * API's covered         :abort
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_stdlib_abort(void)
{
	pthread_t th_id;
	int ret_chk;

	ret_chk = pthread_create(&th_id, NULL, thread_func, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	ret_chk = pthread_join(th_id, NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_stdlib_div
 * @brief                :Compute quotient and remainder of an integer division
 * @Scenario             :Compute quotient and remainder of an integer division
 * API's covered         :div, ldiv, lldiv
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_stdlib_div(void)
{
	int num = 2147483647;
	int denom = 28672;
	int ret_quot = 74898;
	int ret_rem = 8191;
	div_t ret_val;
	ldiv_t lret_val;
	lldiv_t llret_val;

	ret_val = div(num, denom);
	TC_ASSERT_EQ("div", ret_val.quot, ret_quot);
	TC_ASSERT_EQ("div", ret_val.rem, ret_rem);

	lret_val = ldiv(num, denom);
	TC_ASSERT_EQ("ldiv", lret_val.quot, ret_quot);
	TC_ASSERT_EQ("ldiv", lret_val.rem, ret_rem);

	llret_val = lldiv(num, denom);
	TC_ASSERT_EQ("lldiv", llret_val.quot, ret_quot);
	TC_ASSERT_EQ("lldiv", llret_val.rem, ret_rem);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_stdlib
 ****************************************************************************/
int libc_stdlib_main(void)
{
	tc_libc_stdlib_abort();
	tc_libc_stdlib_abs_labs_llabs();
	tc_libc_stdlib_atof();
	tc_libc_stdlib_atoi();
	tc_libc_stdlib_atol();
	tc_libc_stdlib_atoll();
	tc_libc_stdlib_bsearch();
	tc_libc_stdlib_div();
	tc_libc_stdlib_itoa();
	tc_libc_stdlib_qsort();
	tc_libc_stdlib_rand();
	tc_libc_stdlib_random();
	tc_libc_stdlib_strtol();
	tc_libc_stdlib_strtoll();
	tc_libc_stdlib_strtoull();
	tc_libc_stdlib_strtoul();
	tc_libc_stdlib_strtod();
	tc_libc_stdlib_srand();
	tc_libc_stdlib_srandom();

	return 0;
}
