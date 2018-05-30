/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

/// @file tc_libc_inttypes.c

/// @brief Test Case Example for Libc Stdlib API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <inttypes.h>
#include "tc_internal.h"

#define DECIMAL 10
#define HEXADECIMAL 16
#define BINARY 2
#define NVAL1 1000
#define NVAL2 12

/**
 * @fn                   :tc_libc_inttypes_imaxabs
 * @brief                :Calculate the absolute value of the argument of the appropriate integer type
 * @Scenario             :Compute the absolute value of the argument of the appropriate integer type for the function
 * API's covered         :imaxabs
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_inttypes_imaxabs(void)
{
	intmax_t val = -NVAL1;
	intmax_t ret_chk;

	ret_chk = imaxabs(val);
	TC_ASSERT_EQ("imaxabs", ret_chk, NVAL1);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_inttypes_imaxdiv
 * @brief                :Compute quotient and remainder of an integer division
 * @Scenario             :Compute quotient and remainder of an integer division
 * API's covered         :imaxdiv
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_inttypes_imaxdiv(void)
{
	intmax_t num = 3005;
	intmax_t denom = 2018;
	imaxdiv_t sol_val = { 1, 987 };
	imaxdiv_t ret_val;

	ret_val = imaxdiv(num, denom);
	TC_ASSERT_EQ("imaxdiv", ret_val.quot, sol_val.quot);
	TC_ASSERT_EQ("imaxdiv", ret_val.rem, sol_val.rem);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_inttypes_strtoimax
 * @brief                :converts the string to a integer value
 * @Scenario             :The strtoimax() function converts the initial part of the string in nptr to a integer value
 *                        according to the given base, which must be between 2 and 36 inclusive, or be the special value 0.
 * API's covered         :strtoimax
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_inttypes_strtoimax(void)
{
	/* random string used for conversion */
	const char str_lnum[] = "+2001 60c0c0 -1101110100110100100000 0x6fffff";
	char *end_ptr;
	intmax_t ret_chk;

	/* ret_chk = 2001 for string 2001 in decimal */

	ret_chk = strtoimax(str_lnum, &end_ptr, DECIMAL);
	TC_ASSERT_EQ("strtoimax", ret_chk, (intmax_t)2001);

	/* ret_chk = 6340800 for string 60c0c0 in hexadecimal */

	ret_chk = strtoimax(end_ptr, &end_ptr, HEXADECIMAL);
	TC_ASSERT_EQ("strtoimax", ret_chk, (intmax_t)6340800);

	/* ret_chk = -3624224 for string -1101110100110100100000 in binary */

	ret_chk = strtoimax(end_ptr, &end_ptr, BINARY);
	TC_ASSERT_EQ("strtoimax", ret_chk, (intmax_t)-3624224);

	/* ret_chk = 7340031 for string 0x6fffff with base value 0 */

	ret_chk = strtoimax(end_ptr, NULL, 0);
	TC_ASSERT_EQ("strtoimax", ret_chk, (intmax_t)7340031);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_inttypes_strtoumax
 * @brief                :converts the string to a unsigned integer value
 * @Scenario             :The strtoumax() function converts the initial part of the string in nptr to an unsigned int value
 *                        according to the given base, which must be between 2 and 36 inclusive, or be the special value 0.
 * API's covered         :strtoumax
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_inttypes_strtoumax(void)
{
	const char str_ulnum[] = "201 0x60 1100 0x6f";
	char *end_ptr;
	uintmax_t ret_chk;

	/* ret_chk = 201 for string 201 in decimal */

	ret_chk = strtoumax(str_ulnum, &end_ptr, DECIMAL);
	TC_ASSERT_EQ("strtoumax", ret_chk, (uintmax_t)201);

	/* ret_chk = 96 for string 60 in hexadecimal */

	ret_chk = strtoumax(end_ptr, &end_ptr, HEXADECIMAL);
	TC_ASSERT_EQ("strtoumax", ret_chk, (uintmax_t)96);

	/* ret_chk = 12 for string 1100 in binary */

	ret_chk = strtoumax(end_ptr, &end_ptr, BINARY);
	TC_ASSERT_EQ("strtoumax", ret_chk, (uintmax_t)12);

	/* ret_chk = 111 for string 0x6f with base 0 */

	ret_chk = strtoumax(end_ptr, NULL, 0);
	TC_ASSERT_EQ("strtoumax", ret_chk, (uintmax_t)111);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_inttypes
 ****************************************************************************/
int libc_inttypes_main(void)
{
	tc_libc_inttypes_imaxabs();
	tc_libc_inttypes_imaxdiv();
	tc_libc_inttypes_strtoimax();
	tc_libc_inttypes_strtoumax();

	return 0;
}
