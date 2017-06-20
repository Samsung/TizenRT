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
 * @fn                   :tc_libc_math_exp2
 * @brief                :Returns the base-2 exponential value
 * @Scenario             :Returns the base-2 exponential value
 * API's covered         :exp2
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_exp2(void)
{
	const double in_val[8] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const double sol_val[8] = { 1.0, INFINITY, ZERO, INFINITY, ZERO, INFINITY, ZERO, NAN };
	double ret_val[8];
	int exp2_idx;

	for (exp2_idx = 0; exp2_idx < 8; exp2_idx++) {
		ret_val[exp2_idx] = exp2(in_val[exp2_idx]);
		if (!(isnan(sol_val[exp2_idx]) && isnan(ret_val[exp2_idx]))) {
			TC_ASSERT_LEQ("exp2", fabs(sol_val[exp2_idx] - ret_val[exp2_idx]), FLT_EPSILON);
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
 * @fn                   :tc_libc_math_j0
 * @brief                :Returns the bessel value of 1st kind of order 0
 * @Scenario             :Returns the bessel value of 1st kind of order 0
 * API's covered         :j0
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_j0(void)
{
	const double in_val[8] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const double sol_val[8] = { 1.0, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, NAN };
	double ret_val[8] = { ZERO };
	int j0_idx;

	for (j0_idx = 0; j0_idx < 8; j0_idx++) {
		ret_val[j0_idx] = j0(in_val[j0_idx]);
		if (!(isnan(sol_val[j0_idx]) && isnan(ret_val[j0_idx]))) {
			TC_ASSERT_LEQ("j0", fabs(sol_val[j0_idx] - ret_val[j0_idx]), FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_j1
 * @brief                :Returns the bessel value of 1st kind of order 1
 * @Scenario             :Returns the bessel value of 1st kind of order 1
 * API's covered         :j1
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_j1(void)
{
	const double in_val[8] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const double sol_val[8] = { ZERO, ZERO, -ZERO, ZERO, -ZERO, ZERO, -ZERO, NAN };
	double ret_val[8] = { ZERO };
	int j1_idx;

	for (j1_idx = 0; j1_idx < 8; j1_idx++) {
		ret_val[j1_idx] = j1(in_val[j1_idx]);
		if (!(isnan(sol_val[j1_idx]) && isnan(ret_val[j1_idx]))) {
			TC_ASSERT_LEQ("j1", fabs(sol_val[j1_idx] - ret_val[j1_idx]), FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_jn
 * @brief                :Returns the bessel value of 1st kind of order n
 * @Scenario             :Returns the bessel value of 1st kind of order n
 * API's covered         :jn
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_jn(void)
{
	const double in_val[8] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const double sol_val[8] = { ZERO, -ZERO, -ZERO, -ZERO, -ZERO, ZERO, ZERO, NAN };
	double ret_val[8];
	int jn_idx;
	int order = 2;

	for (jn_idx = 0; jn_idx < 8; jn_idx++) {
		ret_val[jn_idx] = jn(order, in_val[jn_idx]);
		if (!(isnan(sol_val[jn_idx]) && isnan(ret_val[jn_idx]))) {
			TC_ASSERT_LEQ("jn", fabs(sol_val[jn_idx] - ret_val[jn_idx]), FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_nextafter
 * @brief                :Returns the next representable floating-point value following argument1 in the direction of argument2
 * @Scenario             :Returns the next representable floating-point value following argument1 in the direction of argument2
 * API's covered         :nextafter
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_nextafter(void)
{
	const double in_val1[7] = { VAL1, VAL1, ZERO, -VAL1, -VAL2 , VAL1, NAN };
	const double in_val2[7] = { VAL1, VAL2, VAL1, -VAL2, ZERO, NAN, VAL2 };
	const double sol_val[7] = { VAL1, 2251799813685249.0, ZERO, -2251799813685249.0, -4503599627370495.5, NAN, NAN };
	double ret_val[7];
	int nextafter_idx;

	for (nextafter_idx = 0; nextafter_idx < 7; nextafter_idx++) {
		ret_val[nextafter_idx] = nextafter(in_val1[nextafter_idx], in_val2[nextafter_idx]);
		if (!(isnan(sol_val[nextafter_idx]) && isnan(ret_val[nextafter_idx]))) {
			TC_ASSERT_LEQ("nextafter", fabs(sol_val[nextafter_idx] - ret_val[nextafter_idx]), FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_nexttoward
 * @brief                :Returns the next representable floating-point value following argument1 in the direction of argument2
 * @Scenario             :Returns the next representable floating-point value following argument1 in the direction of argument2
 * API's covered         :nexttoward
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_nexttoward(void)
{
	const double in_val1[7] = { VAL1, VAL1, ZERO, -VAL1, -VAL2 , VAL1, NAN };
	const long double in_val2[7] = { VAL1, VAL2, VAL1, -VAL2, ZERO, NAN, VAL2 };
	const double sol_val[7] = { VAL1, 2251799813685249.0, ZERO, -2251799813685249.0, -4503599627370495.5, NAN, NAN };
	double ret_val[7];
	int nexttoward_idx;

	for (nexttoward_idx = 0; nexttoward_idx < 7; nexttoward_idx++) {
		ret_val[nexttoward_idx] = nexttoward(in_val1[nexttoward_idx], in_val2[nexttoward_idx]);
		if (!(isnan(sol_val[nexttoward_idx]) && isnan(ret_val[nexttoward_idx]))) {
			TC_ASSERT_LEQ("nexttoward", fabs(sol_val[nexttoward_idx] - ret_val[nexttoward_idx]), FLT_EPSILON);
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

/**
 * @fn                   :tc_libc_math_remainder
 * @brief                :Returns the floating-point remainder of dividing argument1 by argument2
 * @Scenario             :Returns the floating-point remainder of dividing argument1 by argument2
 * API's covered         :remainder
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_remainder(void)
{
	const double in_val1[10] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, INFINITY };
	const double in_val2[10] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, NAN, VAL2 };
	const double sol_val[10] = { ZERO, -ZERO, -2251799813685247.5, 2251799813685247.5, -1.0, 1.0, -NAN, -NAN, NAN, -NAN };
	double ret_val[10];
	int remainder_idx;

	for (remainder_idx = 0; remainder_idx < 10; remainder_idx++) {
		ret_val[remainder_idx] = remainder(in_val1[remainder_idx], in_val2[remainder_idx]);
		if (!(isnan(sol_val[remainder_idx]) && isnan(ret_val[remainder_idx]))) {
			TC_ASSERT_LEQ("remainder", fabs(sol_val[remainder_idx] - ret_val[remainder_idx]), FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_remquo
 * @brief                :Returns the floating-point remainder and stores part of quotient in argument3
 * @Scenario             :Returns the floating-point remainder and stores part of quotient in argument3
 * API's covered         :remquo
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_remquo(void)
{
	const double in_val1[10] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, INFINITY };
	const double in_val2[10] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, NAN, VAL2 };
	const double sol_val[10] = { ZERO, -ZERO, -2251799813685247.5, 2251799813685247.5, -1.0, 1.0, -NAN, -NAN, NAN, -NAN };
	int sol_quo[10] =  { 0, 0, -1, -1, -2, -2, 0, 0, 0, 0 };
	double ret_val[10];
	int ret_quo[10];
	int remquo_idx;

	for (remquo_idx = 0; remquo_idx < 10; remquo_idx++) {
		ret_val[remquo_idx] = remquo(in_val1[remquo_idx], in_val2[remquo_idx], &ret_quo[remquo_idx]);
		if (!(isnan(sol_val[remquo_idx]) && isnan(ret_val[remquo_idx]))) {
			TC_ASSERT_LEQ("remquo", fabs(sol_val[remquo_idx] - ret_val[remquo_idx]), FLT_EPSILON);
			TC_ASSERT_EQ("remquo", sol_quo[remquo_idx], ret_quo[remquo_idx]);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_scalbn
 * @brief                :Returns the multiply of argument1 by FLT_RADIX (probably 2) to the power of argument2
 * @Scenario             :Returns the multiply of argument1 by FLT_RADIX (probably 2) to the power of argument2
 * API's covered         :scalbn
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_scalbn(void)
{
	const double in_val[8] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const double sol_val[8][6] = { { ZERO, ZERO, ZERO, ZERO, ZERO, ZERO }, { 9007199254740994.0, 17592186044416.003906250, ZERO, INFINITY, INFINITY, ZERO }, { -9007199254740994.0, -17592186044416.003906250, -ZERO, -INFINITY, -INFINITY, -ZERO }, { 18014398509481984.0, 35184372088832.0, ZERO, INFINITY, INFINITY, ZERO }, { -18014398509481984.0, -35184372088832.0, -ZERO, -INFINITY, -INFINITY, -ZERO }, { INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY }, { NAN, NAN, NAN, NAN, NAN, NAN } };
	double ret_val[8][6];
	int scalbn_idx;
	long int exp[6] = { 2, -7, -1024, 1024, 3072, -3072 };
	int exp_idx;

	for (scalbn_idx = 0; scalbn_idx < 8; scalbn_idx++) {
		for (exp_idx = 0; exp_idx < 6; exp_idx++) {
			ret_val[scalbn_idx][exp_idx] = scalbn(in_val[scalbn_idx], exp[exp_idx]);
			if (!(isnan(sol_val[scalbn_idx][exp_idx]) && isnan(ret_val[scalbn_idx][exp_idx]))) {
				TC_ASSERT_LEQ("scalbn", fabs(sol_val[scalbn_idx][exp_idx] - ret_val[scalbn_idx][exp_idx]), FLT_EPSILON);
			}
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_y0
 * @brief                :Returns the bessel value of 2nd kind of order 0
 * @Scenario             :Returns the bessel value of 2nd kind of order 0
 * API's covered         :y0
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_y0(void)
{
	const double in_val[8] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const double sol_val[8] = { -INFINITY, ZERO, NAN, ZERO, NAN, ZERO, NAN, NAN };
	double ret_val[8];
	int y0_idx;

	for (y0_idx = 0; y0_idx < 8; y0_idx++) {
		ret_val[y0_idx] = y0(in_val[y0_idx]);
		if (!(isnan(sol_val[y0_idx]) && isnan(ret_val[y0_idx]))) {
			TC_ASSERT_LEQ("y0", fabs(sol_val[y0_idx] - ret_val[y0_idx]), FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_y1
 * @brief                :Returns the bessel value of 2nd kind of order 1
 * @Scenario             :Returns the bessel value of 2nd kind of order 1
 * API's covered         :y1
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_y1(void)
{
	const double in_val[8] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const double sol_val[8] = { -INFINITY, -ZERO, NAN, -ZERO, NAN, ZERO, NAN, NAN };
	double ret_val[8];
	int y1_idx;

	for (y1_idx = 0; y1_idx < 8; y1_idx++) {
		ret_val[y1_idx] = y1(in_val[y1_idx]);
		if (!(isnan(sol_val[y1_idx]) && isnan(ret_val[y1_idx]))) {
			TC_ASSERT_LEQ("y1", fabs(sol_val[y1_idx] - ret_val[y1_idx]), FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_yn
 * @brief                :Returns the bessel value of 2nd kind of order n
 * @Scenario             :Returns the bessel value of 2nd kind of order n
 * API's covered         :yn
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_yn(void)
{
	const double in_val[8] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const double sol_val[8] = { -INFINITY, ZERO, NAN, ZERO, NAN, ZERO, NAN, NAN };
	double ret_val[8];
	int yn_idx;
	int order = 3;

	for (yn_idx = 0; yn_idx < 8; yn_idx++) {
		ret_val[yn_idx] = yn(order, in_val[yn_idx]);
		if (!(isnan(sol_val[yn_idx]) && isnan(ret_val[yn_idx]))) {
			TC_ASSERT_LEQ("yn", fabs(sol_val[yn_idx] - ret_val[yn_idx]), FLT_EPSILON);
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
	tc_libc_math_exp2();
	tc_libc_math_fabs();
	tc_libc_math_fdim();
	tc_libc_math_floor();
	tc_libc_math_fmax();
	tc_libc_math_fmin();
	tc_libc_math_hypot();
	tc_libc_math_j0();
	tc_libc_math_j1();
	tc_libc_math_jn();
	tc_libc_math_nextafter();
	tc_libc_math_nexttoward();
	tc_libc_math_pow();
	tc_libc_math_remainder();
	tc_libc_math_remquo();
	tc_libc_math_scalbn();
	tc_libc_math_y0();
	tc_libc_math_y1();
	tc_libc_math_yn();
#else
	printf("tc_libc_math will not be executed. CONFIG_HAVE_DOUBLE is not set.\n");
#endif
	return 0;
}
