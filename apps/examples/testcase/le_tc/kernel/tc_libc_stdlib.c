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
	int val1 = NVAL1;
	int val2 = -NVAL2;
	int ret_chk1;
	int ret_chk2;
	long int lval1 = LVAL1;
	long int lval2 = -LVAL2;
	long int lret_chk1;
	long int lret_chk2;
	long long int llval1 = LLVAL1;
	long long int llval2 = -LLVAL2;
	long long int llret_chk1;
	long long int llret_chk2;

	/* val1 is a positive number, ret_chk1 should be positive */
	ret_chk1 = abs(val1);
	/* val2 is a negative number, ret_chk2 should be positive */
	ret_chk2 = abs(val2);
	if (ret_chk1 != NVAL1 || ret_chk2 != NVAL2) {
		printf("tc_libc_stdlib_abs FAIL Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	/* lval1 is a positive number, lret_chk1 should be positive */
	lret_chk1 = labs(lval1);
	/* lval2 is a negative number, lret_chk2 should be positive */
	lret_chk2 = labs(lval2);
	if (lret_chk1 != LVAL1 || lret_chk2 != LVAL2) {
		printf("tc_libc_stdlib_labs FAIL Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	/* llVal1 is a positive number, llret_chk1 should be positive */
	llret_chk1 = llabs(llval1);
	/* llVal2 is a negative number, llret_chk2 should be positive */
	llret_chk2 = llabs(llval2);
	if (llret_chk1 != LLVAL1 || llret_chk2 != LLVAL2) {
		printf("tc_libc_stdlib_llabs FAIL Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_stdlib_abs_labs_llabs PASS\n");
	total_pass++;
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
	if (ret_chk != NVAL1) {
		printf("tc_libc_stdlib_imaxabs FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_stdlib_imaxabs PASS\n");
	total_pass++;
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
	if (strcmp(buffer, "12") != OK) {
		printf("tc_libc_stdlib_itoa FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	/* conversion in hexadecimal */
	itoa(val, buffer, HEXADECIMAL);
	if (strcmp(buffer, "c") != OK) {
		printf("tc_libc_stdlib_itoa FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	/* conversion in binary */
	itoa(val, buffer, BINARY);
	if (strcmp(buffer, "1100") != OK) {
		printf("tc_libc_stdlib_itoa FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_stdlib_itoa PASS\n");
	total_pass++;
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
			if (value_arr[data_idx] > value_arr[data_idx + 1]) {
				printf("tc_libc_stdlib_qsort FAIL\n");
				total_fail++;
				RETURN_ERR;
			}
		}
	}
	printf("tc_libc_stdlib_qsort PASS\n");
	total_pass++;
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
	int ret_chk1 = ERROR;
	int ret_chk2 = ERROR;
	int ret_chk3 = ERROR;
	ret_chk1 = rand();
	ret_chk2 = rand();
	ret_chk3 = rand();
	if ((ret_chk1 < 0 || ret_chk1 > MAX_RAND) || (ret_chk2 < 0 || ret_chk2 > MAX_RAND) || (ret_chk3 < 0 || ret_chk3 > MAX_RAND)) {
		printf("tc_libc_stdlib_rand FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_stdlib_rand PASS\n");
	total_pass++;
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
	long int li1;
	long int li2;
	long int li3;
	long int li4;
	/* li1 = 2001 for string 2001 in decimal */
	li1 = strtol(str_lnum, &end_ptr, DECIMAL);
	/* li2 = 6340800 for string 60c0c0 in hexadecimal */
	li2 = strtol(end_ptr, &end_ptr, HEXADECIMAL);
	/* li3 = -3624224 for string -1101110100110100100000 in binary */
	li3 = strtol(end_ptr, &end_ptr, BINARY);
	/* li4 = 7340031 for string 0x6fffff with base value 0 */
	li4 = strtol(end_ptr, NULL, 0);
	if (li1 != 2001 || li2 != 6340800 || li3 != -3624224 || li4 != 7340031) {
		printf("tc_libc_stdlib_strtol FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_stdlib_strtol PASS\n");
	total_pass++;
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
	long long int lli1;
	long long int lli2;
	long long int lli3;
	long long int lli4;

	/* li1 = 1856892505 for string 1856892505 in decimal */
	lli1 = strtoll(str_llnum, &end_ptr, DECIMAL);
	/* lli2 = 6358606123 for string 17b00a12b in hexadecimal */
	lli2 = strtoll(end_ptr, &end_ptr, HEXADECIMAL);
	/* lli3 = -208340076 for string -01100011010110000010001101100 in binary */
	lli3 = strtoll(end_ptr, &end_ptr, BINARY);
	/* lli4 = 7340031 for string 0x6fffff with base 0 */
	lli4 = strtoll(end_ptr, NULL, 0);
	if (lli1 != 1856892505 || lli2 != 6358606123 || lli3 != -208340076 || lli4 != 7340031) {
		printf("tc_libc_stdlib_strtoll FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_stdlib_strtoll PASS\n");
	total_pass++;
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
	unsigned long int uli1;
	unsigned long int uli2;
	unsigned long int uli3;
	unsigned long int uli4;

	/* uli1 = 201 for string 201 in decimal */
	uli1 = strtoul(str_ulnum, &end_ptr, DECIMAL);
	/* uli2 = 96 for string 60 in hexadecimal */
	uli2 = strtoul(end_ptr, &end_ptr, HEXADECIMAL);
	/* uli3 = 12 for string 1100 in binary */
	uli3 = strtoul(end_ptr, &end_ptr, BINARY);
	/* uli4 = 111 for string 0x6f with base 0 */
	uli4 = strtoul(end_ptr, NULL, 0);
	if (uli1 != 201 || uli2 != 96 || uli3 != 12 || uli4 != 111) {
		printf("tc_libc_stdlib_strtoul FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_stdlib_strtoul PASS\n");
	total_pass++;
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
	unsigned long long int ulli1;
	unsigned long long int ulli2;
	unsigned long long int ulli3;
	unsigned long long int ulli4;

	/* ulli1 = 250068492 for string 250068492 in decimal */
	ulli1 = strtoull(str_ullnum, &end_ptr, DECIMAL);
	/* ulli2 = 2064035584 for string 7b06af00 in hexadecimal */
	ulli2 = strtoull(end_ptr, &end_ptr, HEXADECIMAL);
	/* ulli3 = 208622688 for string 1100011011110101010001100000 in binary */
	ulli3 = strtoull(end_ptr, &end_ptr, BINARY);
	/* ulli4 = 7340031 for string 0x6fffff with base 0 */
	ulli4 = strtoull(end_ptr, NULL, 0);
	if (ulli1 != 250068492 || ulli2 != 2064035584 || ulli3 != 208622688 || ulli4 != 7340031) {
		printf("tc_libc_stdlib_strtoull FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_stdlib_strtoull PASS\n");
	total_pass++;
}

static void tc_libc_stdlib_strtod(void)
{
	char target[100] = "1234.56abcd";
	char *pos = NULL;
	double value = 0;

	value = strtod(target, &pos);
	if (value != 1234.56) {
		total_fail++;
		printf("tc_libc_stdlib_strtod FAIL : not matched %d %s\n", value, pos);
		RETURN_ERR;
	}
	total_pass++;
	printf("tc_libc_stdlib_strtod PASS\n");
}

static void tc_libc_stdlib_atoi(void)
{
	/* random string used for conversion */
	char str_lnum[] = "2016";
	int target;
	/* target = 2001 for string 2001 in decimal */
	target = atoi(str_lnum);
	if (target != 2016) {
		total_fail++;
		printf("tc_libc_stdlib_atoi FAIL\n");
		RETURN_ERR;
	}

	printf("tc_libc_stdlib_atoi PASS\n");
	total_pass++;
}

static void tc_libc_stdlib_atol(void)
{
	/* random string used for conversion */
	char str_lnum[] = "20162015";
	long int target;

	target = atol(str_lnum);
	if (target != 20162015) {
		total_fail++;
		printf("tc_libc_stdlib_atol FAIL\n");
		RETURN_ERR;
	}

	printf("tc_libc_stdlib_atol PASS\n");
	total_pass++;
}

static void tc_libc_stdlib_atoll(void)
{
	/* random string used for conversion */
	char str_lnum[] = "201620152014";
	long long int target;

	target = atoll(str_lnum);
	if (target != 201620152014) {
		total_fail++;
		printf("tc_libc_stdlib_atoll FAIL\n");
		RETURN_ERR;
	}

	printf("tc_libc_stdlib_atoll PASS\n");
	total_pass++;
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
		if (first_result[result_iter] != second_result[result_iter]) {
			total_fail++;
			printf("tc_libc_stdlib_srand FAIL : not matched\n");
			RETURN_ERR;
		}
	}

	printf("tc_libc_stdlib_srand PASS\n");
	total_pass++;
}

static void tc_libc_stdlib_atof(void)
{
	char target[100] = "1234.56abcd";
	double value = 0;

	value = atof(target);
	if (value != 1234.56) {
		total_fail++;
		printf("tc_libc_stdlib_atof FAIL\n");
		RETURN_ERR;
	}
	total_pass++;
	printf("tc_libc_stdlib_atof PASS\n");
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
