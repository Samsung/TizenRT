/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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

/// @file libc_math.c
/// @brief Test Case Example for Libc Math API

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/compiler.h>
#include <stdio.h>
#include <tinyara/math.h>
#include <tinyara/float.h>
#include "tc_internal.h"

#ifdef CONFIG_HAVE_DOUBLE

/* dobule has 52 bit fraction(IEEE754). 4503599627370496 == 0x10000000000000 */

#define ZERO 0.0
#define VAL1 2251799813685248.5
#define VAL2 4503599627370496.5

/**
 * @fn                   :tc_libc_math_cbrt
 * @brief                :Returns the cube root of parameter
 * @Scenario             :Returns the cube root of parameter
 * API's covered         :cbrt
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_cbrt(void)
{
	const double in_val[6] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN };
	const double sol_val[6] = { ZERO, 131072.0, -131072.0, INFINITY, -INFINITY, NAN };
	double ret_val[6];
	int cbrt_idx;

	for (cbrt_idx = 0; cbrt_idx < 6; cbrt_idx++) {
		ret_val[cbrt_idx] = cbrt(in_val[cbrt_idx]);
		if (!(isnan(sol_val[cbrt_idx]) && isnan(ret_val[cbrt_idx]))) {
			TC_ASSERT_LEQ("cbrt", fabs(sol_val[cbrt_idx] - ret_val[cbrt_idx]), FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_fabs
 * @brief                :Returns the absolute value of parameter
 * @Scenario             :Returns the absolute value of parameter
 * API's covered         :fabs
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_fabs(void)
{
	const double in_val[5] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY };
	const double sol_val[5] = { ZERO, 2251799813685248.5, 2251799813685248.5, INFINITY, INFINITY };
	double ret_val[5];
	int fabs_idx;

	for (fabs_idx = 0; fabs_idx < 5; fabs_idx++) {
		ret_val[fabs_idx] = fabs(in_val[fabs_idx]);
		TC_ASSERT_EQ("fabs", sol_val[fabs_idx], ret_val[fabs_idx]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_fdim
 * @brief                :Returns the positive  difference max(x-y,0),  between thier arguments
 * @Scenario             :Returns the positive  difference max(x-y,0),  between their arguments
 * API's covered         :fdim
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_fdim(void)
{
	const double in_val1[9] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const double in_val2[9] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, NAN };
	const double sol_val[9] = { INFINITY, ZERO, 6755399441055744.0, ZERO, 6755399441055744.0, ZERO, INFINITY, ZERO, NAN };
	double ret_val[9];
	int fdim_idx;

	for (fdim_idx = 0; fdim_idx < 9; fdim_idx++) {
		ret_val[fdim_idx] = fdim(in_val1[fdim_idx], in_val2[fdim_idx]);
		if (!(isnan(sol_val[fdim_idx]) && isnan(ret_val[fdim_idx]))) {
			TC_ASSERT_LEQ("fdim", fabs(sol_val[fdim_idx] - ret_val[fdim_idx]), FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_floor
 * @brief                :Returns the floor value of parameter
 * @Scenario             :Returns the floor value of parameter
 * API's covered         :floor
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_floor(void)
{
	const double in_val[8] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY };
	const double sol_val[8] = { ZERO, -ZERO, 2251799813685248.0, -2251799813685249.0, 4503599627370496.0, -4503599627370497.0, INFINITY, -INFINITY };
	double ret_val[8];
	int floor_idx;

	for (floor_idx = 0; floor_idx < 8; floor_idx++) {
		ret_val[floor_idx] = floor(in_val[floor_idx]);
		TC_ASSERT_EQ("floor", sol_val[floor_idx], ret_val[floor_idx]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_fmax
 * @brief                :Returns the maximum value of argument1 and argument2
 * @Scenario             :Returns the maximum value of argument1 and argument2
 * API's covered         :fmax
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_fmax(void)
{
	const double in_val1[7] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN, VAL1 };
	const double in_val2[7] = { -INFINITY, INFINITY, -VAL1, VAL1, ZERO, NAN, NAN };
	const double sol_val[7] = { ZERO, INFINITY, -VAL1, INFINITY, ZERO, NAN, VAL1 };
	double ret_val[7];
	int fmax_idx;

	for (fmax_idx = 0; fmax_idx < 7; fmax_idx++) {
		ret_val[fmax_idx] = fmax(in_val1[fmax_idx], in_val2[fmax_idx]);
		if (!(isnan(sol_val[fmax_idx]) && isnan(ret_val[fmax_idx]))) {
			TC_ASSERT_LEQ("fmax", fabs(sol_val[fmax_idx] - ret_val[fmax_idx]), FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_fmin
 * @brief                :Returns the minimum value of argument1 and argument2
 * @Scenario             :Returns the minimum value of argument1 and argument2
 * API's covered         :fmin
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_fmin(void)
{
	const double in_val1[7] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN, VAL1 };
	const double in_val2[7] = { -INFINITY, INFINITY, -VAL1, VAL1, ZERO, NAN, NAN };
	const double sol_val[7] = { -INFINITY, VAL1, -VAL1, VAL1, -INFINITY, NAN, VAL1 };
	double ret_val[7];
	int fmin_idx;

	for (fmin_idx = 0; fmin_idx < 7; fmin_idx++) {
		ret_val[fmin_idx] = fmin(in_val1[fmin_idx], in_val2[fmin_idx]);
		if (!(isnan(sol_val[fmin_idx]) && isnan(ret_val[fmin_idx]))) {
			TC_ASSERT_LEQ("fmin", fabs(sol_val[fmin_idx] - ret_val[fmin_idx]), FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_hypot
 * @brief                :Returns the length of a right-angled  triangle with sides of length parameter1 and parameter2
 * @Scenario             :Returns the length of a right-angled  triangle with sides of length parameter1 and parameter2
 * API's covered         :hypot
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_hypot(void)
{
	const double in_val1[10] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, VAL1 };
	const double in_val2[10] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, INFINITY, NAN };
	const double sol_val[10] = { INFINITY, INFINITY, 5035177455121576.0, 5035177455121576.0, 5035177455121576.0, 5035177455121576.0, INFINITY, INFINITY, INFINITY, NAN };
	double ret_val[10];
	int hypot_idx;

	for (hypot_idx = 0; hypot_idx < 10; hypot_idx++) {
		ret_val[hypot_idx] = hypot(in_val1[hypot_idx], in_val2[hypot_idx]);
		if (!(isnan(sol_val[hypot_idx]) && isnan(ret_val[hypot_idx]))) {
			TC_ASSERT_LEQ("hypot", fabs(sol_val[hypot_idx] - ret_val[hypot_idx]), FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_pow
 * @brief                :Returns the power value of parameter
 * @Scenario             :Returns the power value of parameter
 * API's covered         :pow
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */

static void tc_libc_math_pow(void)
{
	const double in_val[13][2] = { { 2, 2 }, { 2, 0.5 }, { 2, -2 }, { 2, -0.5 }, { 1, NAN }, { ZERO, 0.5 }, { ZERO, -2 }, { -ZERO, -1 }, { -ZERO, -2 }, { -2, -1 }, { -2, -2 }, { 0.5, INFINITY }, { 2, INFINITY } };
	const double sol_val[13] = { 4.0, M_SQRT2, 0.25, M_SQRT1_2, 1, ZERO, INFINITY, -INFINITY, INFINITY, -0.5, 0.25, ZERO, INFINITY };
	double ret_val[13];
	int pow_idx;

	for (pow_idx = 0; pow_idx < 13; pow_idx++) {
		ret_val[pow_idx] = pow(in_val[pow_idx][0], in_val[pow_idx][1]);
		if (isinf(sol_val[pow_idx])) {
			TC_ASSERT_EQ("pow", sol_val[pow_idx], ret_val[pow_idx]);
		} else {
			TC_ASSERT_LT("pow", fabs(sol_val[pow_idx] - ret_val[pow_idx]), 0.00000000000001);
		}
	}

	TC_SUCCESS_RESULT();
}
#endif

/****************************************************************************
 * Name: libc_math
 ****************************************************************************/
int libc_math_main(void)
{
#ifdef CONFIG_HAVE_DOUBLE
	tc_libc_math_cbrt();
	tc_libc_math_fabs();
	tc_libc_math_fdim();
	tc_libc_math_floor();
	tc_libc_math_fmax();
	tc_libc_math_fmin();
	tc_libc_math_hypot();
	tc_libc_math_pow();
#else
	printf("tc_libc_math will not be executed. CONFIG_HAVE_DOUBLE is not set.\n");
#endif
	return 0;
}
