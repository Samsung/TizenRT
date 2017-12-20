/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/// @file itc_libc_stdlib.c
/// @brief Test Case Example for Libc Stdlib API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdlib.h>
#include "tc_internal.h"

#define BUFF_SIZE 20

/**
 * @fn                   :itc_libc_stdlib_atoi_p_diff_combination
 * @brief                :convert a string to an integer
 * @Scenario             :convert a string to an integer
 * API's covered         :atoi
 * Preconditions         :None
 * Postconditions        :None
 */
static void itc_libc_stdlib_atoi_p_diff_combination(void)
{
	/* random string used for conversion */
	char str1[] = "2017";
	char str2[] = "-2017";
	char str3[] = "20.17";
	int ret_chk;

	ret_chk = atoi(str1);
	TC_ASSERT_EQ("atoi", ret_chk, 2017);

	ret_chk = atoi(str2);
	TC_ASSERT_EQ("atoi", ret_chk, -2017);

	ret_chk = atoi(str3);
	TC_ASSERT_EQ("atoi", ret_chk, 20);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :itc_libc_stdlib_atol_p_diff_combination
 * @brief                :convert a string to long integer
 * @Scenario             :convert a string to long integer
 * API's covered         :atol
 * Preconditions         :None
 * Postconditions        :None
 */
static void itc_libc_stdlib_atol_p_diff_combination(void)
{
	/* random string used for conversion */
	char str1[] = "201712121";
	char str2[] = "-201712121";
	char str3[] = "2017.12121";
	long int ret_chk;

	ret_chk = atol(str1);
	TC_ASSERT_EQ("atol", ret_chk, 201712121);

	ret_chk = atol(str2);
	TC_ASSERT_EQ("atol", ret_chk, -201712121);

	ret_chk = atol(str3);
	TC_ASSERT_EQ("atol", ret_chk, 2017);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :itc_libc_stdlib_atoll_p_diff_combination
 * @brief                :convert a string to long long integer
 * @Scenario             :convert a string to long long integer
 * API's covered         :atoll
 * Preconditions         :None
 * Postconditions        :None
 */
static void itc_libc_stdlib_atoll_p_diff_combination(void)
{
	/* random string used for conversion */
	char str1[] = "201712121988";
	char str2[] = "-201712121988";
	char str3[] = "2017.12121988";
	long long int ret_chk;

	ret_chk = atoll(str1);
	TC_ASSERT_EQ("atoll", ret_chk, 201712121988);

	ret_chk = atoll(str2);
	TC_ASSERT_EQ("atoll", ret_chk, -201712121988);

	ret_chk = atoll(str3);
	TC_ASSERT_EQ("atoll", ret_chk, 2017);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :itc_libc_stdlib_atof_p_diff_combination
 * @brief                :Convert a string to a double
 * @Scenario             :Convert a string to a double
 * API's covered         :atof
 * Preconditions         :None
 * Postconditions        :None
 */
static void itc_libc_stdlib_atof_p_diff_combination(void)
{
	char src1[BUFF_SIZE] = "1234.56abcd";
	char src2[BUFF_SIZE] = "abcd1234.56";
	char src3[BUFF_SIZE] = "-12.32abcd1234.56";
	double ret_chk = 0;

	ret_chk = atof(src1);
	TC_ASSERT_EQ("atof", ret_chk, 1234.56);

	ret_chk = atof(src2);
	TC_ASSERT_EQ("atof", ret_chk, 0.0);

	ret_chk = atof(src3);
	TC_ASSERT_EQ("atof", ret_chk, -12.32);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :itc_libc_stdlib_div_p_negative_num
 * @brief                :Compute quotient and remainder of an integer division
 * @Scenario             :Compute quotient and remainder of an negative integer division
 * API's covered         :div, ldiv, lldiv
 * Preconditions         :None
 * Postconditions        :None
 */
static void itc_libc_stdlib_div_p_negative_num(void)
{
	int num = 10;
	int denom = 3;
	int ret_quot = 3;
	int ret_rem = 1;
	div_t ret_val;
	ldiv_t lret_val;
	lldiv_t llret_val;

	ret_val = div(-num, denom);
	TC_ASSERT_EQ("div", ret_val.quot, -ret_quot);
	TC_ASSERT_EQ("div", ret_val.rem, -ret_rem);

	ret_val = div(num, -denom);
	TC_ASSERT_EQ("div", ret_val.quot, -ret_quot);
	TC_ASSERT_EQ("div", ret_val.rem, ret_rem);

	ret_val = div(-num, -denom);
	TC_ASSERT_EQ("div", ret_val.quot, ret_quot);
	TC_ASSERT_EQ("div", ret_val.rem, -ret_rem);

	lret_val = ldiv(-num, denom);
	TC_ASSERT_EQ("ldiv", lret_val.quot, -ret_quot);
	TC_ASSERT_EQ("ldiv", lret_val.rem, -ret_rem);

	lret_val = ldiv(num, -denom);
	TC_ASSERT_EQ("ldiv", lret_val.quot, -ret_quot);
	TC_ASSERT_EQ("ldiv", lret_val.rem, ret_rem);

	lret_val = ldiv(-num, -denom);
	TC_ASSERT_EQ("ldiv", lret_val.quot, ret_quot);
	TC_ASSERT_EQ("ldiv", lret_val.rem, -ret_rem);

	llret_val = lldiv(-num, denom);
	TC_ASSERT_EQ("lldiv", llret_val.quot, -ret_quot);
	TC_ASSERT_EQ("lldiv", llret_val.rem, -ret_rem);

	llret_val = lldiv(num, -denom);
	TC_ASSERT_EQ("lldiv", llret_val.quot, -ret_quot);
	TC_ASSERT_EQ("lldiv", llret_val.rem, ret_rem);

	llret_val = lldiv(-num, -denom);
	TC_ASSERT_EQ("lldiv", llret_val.quot, ret_quot);
	TC_ASSERT_EQ("lldiv", llret_val.rem, -ret_rem);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :itc_libc_stdlib_div_p_zero_num
 * @brief                :Compute quotient and remainder of an integer division
 * @Scenario             :Compute quotient and remainder of an zero integer division
 * API's covered         :div, ldiv, lldiv
 * Preconditions         :None
 * Postconditions        :None
 */
static void itc_libc_stdlib_div_p_zero_num(void)
{
	int num = 0;
	int denom = 3;
	int ret_quot = 0;
	int ret_rem = 0;
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

int itc_libc_stdlib_main(void)
{
	itc_libc_stdlib_atoi_p_diff_combination();
	itc_libc_stdlib_atol_p_diff_combination();
	itc_libc_stdlib_atoll_p_diff_combination();
	itc_libc_stdlib_atof_p_diff_combination();
	itc_libc_stdlib_div_p_zero_num();
	itc_libc_stdlib_div_p_negative_num();

	return 0;
}
