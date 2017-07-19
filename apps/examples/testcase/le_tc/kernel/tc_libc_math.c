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
 * @fn                  :tc_libc_math_acos
 * @brief               :Returns the principal value of the arc cosine of x
 * @scenario            :Returns the principal value of the arc cosine of x
 * @API's covered       :acos
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_acos(void)
{
	const double in_val[5] = { 0.64, -0.4, ZERO, -1, 1 };
	const double sol_val[5] = { 0.87629806116834, 1.9823131728624, 1.5707963267949, 3.1415926535898, 0 };
	double ret_val[5];
	int acos_idx;

	/* Returns the principal value of the arc cosine of x, expressed in radians */

	for (acos_idx = 0; acos_idx < 5; acos_idx++) {
		ret_val[acos_idx] = acos(in_val[acos_idx]);
		TC_ASSERT_LEQ("acos", fabs(sol_val[acos_idx] - ret_val[acos_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_acosf
 * @brief               :Returns the principal value of the arc cosine of x
 * @scenario            :Returns the principal value of the arc cosine of x
 * @API's covered       :acosf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_acosf(void)
{
	const float in_val[5] = { 0.64, -0.4, 0, -1, 1 };
	const double sol_val[5] = { 0.8762981295586, 1.9823131561279, 1.5707963705063, 3.1412079334259, 0.0004112283350 };
	float ret_val[5];
	int acosf_idx;

	/* Returns the nonnegative area hyperbolic cosine of x */

	for (acosf_idx = 0; acosf_idx < 5; acosf_idx++) {
		ret_val[acosf_idx] = acosf(in_val[acosf_idx]);
		TC_ASSERT_LEQ("acosf", fabs(sol_val[acosf_idx] - ret_val[acosf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_acosl
 * @brief               :Returns the principal value of the arc cosine of x
 * @scenario            :Returns the principal value of the arc cosine of x
 * @API's covered       :acosl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_acosl(void)
{
	const double in_val[5] = { 0.64, -0.4, ZERO, -1, 1 };
	const double sol_val[5] = { 0.87629806116834, 1.9823131728624, 1.5707963267949, 3.1415926535898, 0 };
	double ret_val[5];
	int acosl_idx;

	/* Returns the nonnegative area hyperbolic cosine of x */

	for (acosl_idx = 0; acosl_idx < 5; acosl_idx++) {
		ret_val[acosl_idx] = acosl(in_val[acosl_idx]);
		TC_ASSERT_LEQ("acosl", fabs(sol_val[acosl_idx] - ret_val[acosl_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_acosh
 * @brief               :Returns the nonnegative area hyperbolic cosine of x
 * @scenario            :Returns the nonnegative area hyperbolic cosine of x
 * @API's covered       :acosh
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_acosh(void)
{
	const double in_val[5] = { 0.64, -0.4, ZERO, -1, 1 };
	const double sol_val[5] = { 0.87629806116834, 1.9823131728624, 1.5707963267949, 3.1415926535898, 0 };
	double ret_val[5];
	int acosh_idx;

	/* Returns the nonnegative area hyperbolic cosine of x */

	for (acosh_idx = 0; acosh_idx < 5; acosh_idx++) {
		ret_val[acosh_idx] = acosh(in_val[acosh_idx]);
		TC_ASSERT_LEQ("acosh", fabs(sol_val[acosh_idx] - ret_val[acosh_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_acoshf
 * @brief               :Returns the principal value of the arc cosine of x
 * @scenario            :Returns the principal value of the arc cosine of x
 * @API's covered       :acoshf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_acoshf(void)
{
	const double in_val[5] = { 0.64, -0.4, ZERO, -1, 1 };
	const double sol_val[5] = { 0.87629806116834, 1.9823131728624, 1.5707963267949, 3.1415926535898, 0 };
	double ret_val[5];
	int acoshf_idx;

	/* Returns the nonnegative area hyperbolic cosine of x */

	for (acoshf_idx = 0; acoshf_idx < 5; acoshf_idx++) {
		ret_val[acoshf_idx] = acosh(in_val[acoshf_idx]);
		TC_ASSERT_LEQ("acoshf", fabs(sol_val[acoshf_idx] - ret_val[acoshf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/* Implementation to be checked */
/**
 * @fn                  :tc_libc_math_acoshl
 * @brief               :Returns the principal value of the arc cosine of x
 * @scenario            :Returns the principal value of the arc cosine of x
 * @API's covered       :acoshl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_acoshl(void)
{
	const double in_val[3] = { 7, 56, 2.45 };
	const double sol_val[3] = { 2.6339157938496, 4.7184191423729, 1.5447131178707 };
	double ret_val[3];
	int acoshl_idx;

	/* Returns the nonnegative area hyperbolic cosine of x */

	for (acoshl_idx = 0; acoshl_idx < 3; acoshl_idx++) {
		ret_val[acoshl_idx] = acoshl(in_val[acoshl_idx]);
		TC_ASSERT_LEQ("acoshl", fabs(sol_val[acoshl_idx] - ret_val[acoshl_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_asin
 * @brief               :Returns the principal value of the arc sine of x
 * @scenario            :Returns the principal value of the arc sine of x
 * @API's covered       :asin
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_asin(void)
{
	const double in_val[5] = { 0.64, -0.4, ZERO, -1, 1 };
	const double sol_val[5] = { 0.69449826562656, -0.41151684606749, 0, -1.5707963267949, 1.5707963267949 };
	double ret_val[5];
	int asin_idx;

	/* Returns the principal value of the arc sine of x, expressed in radians */

	for (asin_idx = 0; asin_idx < 5; asin_idx++) {
		ret_val[asin_idx] = asin(in_val[asin_idx]);
		TC_ASSERT_LEQ("asin", fabs(sol_val[asin_idx] - ret_val[asin_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_asinf
 * @brief               :Returns the principal value of the arc sine of x
 * @scenario            :Returns the principal value of the arc sine of x
 * @API's covered       :asinf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_asinf(void)
{
	float in_val[5] = { 0.64, -0.4, ZERO, -1, 1 };
	float sol_val[5] = { 0.6944981813431, -0.4115168154240, 0.0000000000000, -1.5704115629196, 1.5703850984573 };
	float ret_val[5];
	int asinf_idx;

	/* Returns the principal value of the arc sine of x, expressed in radians */

	for (asinf_idx = 0; asinf_idx < 5; asinf_idx++) {
		ret_val[asinf_idx] = asinf(in_val[asinf_idx]);
		TC_ASSERT_LEQ("asinf", fabs(sol_val[asinf_idx] - ret_val[asinf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_asinl
 * @brief               :Returns the principal value of the arc sine of x
 * @scenario            :Returns the principal value of the arc sine of x
 * @API's covered       :asinl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_asinl(void)
{
	const double in_val[5] = { 0.64, -0.4, ZERO, -1, 1 };
	const double sol_val[5] = { 0.69449826562656, -0.41151684606749, 0, -1.5707963267949, 1.5707963267949 };
	double ret_val[5];
	int asinl_idx;

	/* Returns the principal value of the arc sine of x, expressed in radians */

	for (asinl_idx = 0; asinl_idx < 5; asinl_idx++) {
		ret_val[asinl_idx] = asinl(in_val[asinl_idx]);
		TC_ASSERT_LEQ("asinl", fabs(sol_val[asinl_idx] - ret_val[asinl_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_asinh
 * @brief               :Returns the area hyperbolic sine of x
 * @scenario            :Returns the area hyperbolic sine of x
 * @API's covered       :asinh
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_asinh(void)
{
	const double in_val[3] = { 7, 56, 2.45 };
	const double sol_val[3] = { 2.6441207610586, 4.7185785811518, 1.6284998192842 };
	double ret_val[3];
	int asinh_idx;

	/* Returns the area hyperbolic sine of x */

	for (asinh_idx = 0; asinh_idx < 3; asinh_idx++) {
		ret_val[asinh_idx] = asinh(in_val[asinh_idx]);
		TC_ASSERT_LEQ("asinh", fabs(sol_val[asinh_idx] - ret_val[asinh_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_asinhf
 * @brief               :Returns the principal value of the arc cosine of x
 * @scenario            :Returns the principal value of the arc cosine of x
 * @API's covered       :asinhf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_asinhf(void)
{
	const double in_val[3] = { 7, 56, 2.45 };
	const double sol_val[3] = { 2.6441207610586, 4.7185785811518, 1.6284998192842 };
	double ret_val[3];
	int asinhf_idx;

	/* Returns the area hyperbolic sine of x */

	for (asinhf_idx = 0; asinhf_idx < 3; asinhf_idx++) {
		ret_val[asinhf_idx] = asinhf(in_val[asinhf_idx]);
		TC_ASSERT_LEQ("asinhf", fabs(sol_val[asinhf_idx] - ret_val[asinhf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_asinhl
 * @brief               :Returns the principal value of the arc sine of x
 * @scenario            :Returns the principal value of the arc sine of x
 * @API's covered       :asinhl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_asinhl(void)
{
	const double in_val[5] = { 7, 56, 2.45 };
	const double sol_val[5] = { 2.6441207610586, 4.7185785811518, 1.6284998192842 };
	double ret_val[5];
	int asinhl_idx;

	/* Returns the principal value of the arc sine of x, expressed in radians */

	for (asinhl_idx = 0; asinhl_idx < 5; asinhl_idx++) {
		ret_val[asinhl_idx] = asinhl(in_val[asinhl_idx]);
		TC_ASSERT_LEQ("asinhl", fabs(sol_val[asinhl_idx] - ret_val[asinhl_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atan
 * @brief               :Returns the principal value of the arc tangent of x
 * @scenario            :Returns the principal value of the arc tangent of x
 * @API's covered       :atan
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atan(void)
{
	const double in_val[6] = { 0.50, -0.50, 5, -5, 100, -100 };
	const double sol_val[6] = { 0.46364760900081, -0.46364760900081, 1.373400766945, -1.373400766945, 1.5607966601082, -1.5607966601082 };
	double ret_val[6];
	int atan_idx;

	/* Returns the principal value of the arc tangent of x, expressed in radians */

	for (atan_idx = 0; atan_idx < 6; atan_idx++) {
		ret_val[atan_idx] = atan(in_val[atan_idx]);
		TC_ASSERT_LEQ("atan", fabs(sol_val[atan_idx] - ret_val[atan_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atanf
 * @brief               :Returns the principal value of the arc tangent of x
 * @scenario            :Returns the principal value of the arc tangent of x
 * @API's covered       :atanf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atanf(void)
{
	const double in_val[6] = { 0.50, -0.50, 5, -5, 100, -100 };
	const double sol_val[6] = { 0.4636476039886, -0.4636476039886, 1.3734011650085, -1.3734012842178, 1.5608066320419, -1.5607856512070 };
	double ret_val[6];
	int atanf_idx;

	/* Returns the principal value of the arc tangent of x, expressed in radians */

	for (atanf_idx = 0; atanf_idx < 6; atanf_idx++) {
		ret_val[atanf_idx] = atanf(in_val[atanf_idx]);
		TC_ASSERT_LEQ("atanf", fabs(sol_val[atanf_idx] - ret_val[atanf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atanl
 * @brief               :Returns the principal value of the arc tangent of x
 * @scenario            :Returns the principal value of the arc tangent of x
 * @API's covered       :atanl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atanl(void)
{
	const double in_val[6] = { 0.50, -0.50, 5, -5, 100, -100 };
	const double sol_val[6] = { 0.46364760900081, -0.46364760900081, 1.373400766945, -1.373400766945, 1.5607966601082, -1.5607966601082 };
	double ret_val[6];
	int atanl_idx;

	/* Returns the principal value of the arc tangent of x, expressed in radians */

	for (atanl_idx = 0; atanl_idx < 6; atanl_idx++) {
		ret_val[atanl_idx] = atanl(in_val[atanl_idx]);
		TC_ASSERT_LEQ("atanl", fabs(sol_val[atanl_idx] - ret_val[atanl_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atanh
 * @brief               :Returns the principal value of the arc tangent of x
 * @scenario            :Returns the principal value of the arc tangent of x
 * @API's covered       :atanh
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atanh(void)
{
	const double in_val[5] = { M_PI_4, 0.50, -0.50, 1, -1 };
	const double sol_val[5] = { 1.0593061708232, 0.54930614433405, -0.54930614433405, INFINITY, -INFINITY };
	double ret_val[5];
	int atanh_idx;

	/* Returns the principal value of the arc tangent of x, expressed in radians */

	for (atanh_idx = 0; atanh_idx < 5; atanh_idx++) {
		ret_val[atanh_idx] = atanh(in_val[atanh_idx]);
		TC_ASSERT_LEQ("atanh", fabs(sol_val[atanh_idx] - ret_val[atanh_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atanhf
 * @brief               :Returns the principal value of the arc tangent of x
 * @scenario            :Returns the principal value of the arc tangent of x
 * @API's covered       :atanhf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atanhf(void)
{
	const double in_val[5] = { M_PI_4, 0.50, -0.50, 1, -1 };
	const double sol_val[5] = { 1.0593061708232, 0.54930614433405, -0.54930614433405, INFINITY, -INFINITY };
	double ret_val[5];
	int atanhf_idx;

	/* Returns the principal value of the arc tangent of x, expressed in radians */

	for (atanhf_idx = 0; atanhf_idx < 5; atanhf_idx++) {
		ret_val[atanhf_idx] = atanhf(in_val[atanhf_idx]);
		TC_ASSERT_LEQ("atanhf", fabs(sol_val[atanhf_idx] - ret_val[atanhf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/* Implementation to be checked */
/**
 * @fn                  :tc_libc_math_atanhl
 * @brief               :Returns the principal value of the arc tangent of x
 * @scenario            :Returns the principal value of the arc tangent of x
 * @API's covered       :atanhl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atanhl(void)
{
	const double in_val[5] = { M_PI_4, 0.50, -0.50, 1, -1 };
	const double sol_val[5] = { 1.0593061708232, 0.54930614433405, -0.54930614433405, INFINITY, INFINITY };
	double ret_val[5];
	int atanhl_idx;

	/* Returns the principal value of the arc tangent of x, expressed in radians */

	for (atanhl_idx = 0; atanhl_idx < 5; atanhl_idx++) {
		ret_val[atanhl_idx] = atanhl(in_val[atanhl_idx]);
		TC_ASSERT_LEQ("atanhl", fabs(sol_val[atanhl_idx] - ret_val[atanhl_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atan2
 * @brief               :Returns the principal value of the arc tangent of y/x
 * @scenario            :Returns the principal value of the arc tangent of y/x
 * @API's covered       :atan2
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atan2(void)
{
	const double in_val1[6] = { 0.50, -0.50, 5, 10, -5, -10 };
	const double in_val2[6] = { 0.50, -0.50, 5, 20, -5, 10 };
	const double sol_val[6] = { 0.78539816339745, -2.3561944901923, 0.78539816339745, 0.46364760900081, -2.3561944901923, -0.78539816339745 };
	double ret_val[6];
	int atan2_idx;

	/* Returns the principal value of the arc tangent of y/x, expressed in radians */

	for (atan2_idx = 0; atan2_idx < 6; atan2_idx++) {
		ret_val[atan2_idx] = atan2(in_val1[atan2_idx], in_val2[atan2_idx]);
		TC_ASSERT_LEQ("atan2", fabs(sol_val[atan2_idx] - ret_val[atan2_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atan2f
 * @brief               :Returns the principal value of the arc tangent of x
 * @scenario            :Returns the principal value of the arc tangent of x
 * @API's covered       :atan2f
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atan2f(void)
{
	const double in_val1[6] = { 0.50, -0.50, 5, 10, -5, -10 };
	const double in_val2[6] = { 0.50, -0.50, 5, 20, -5, 10 };
	const double sol_val[6] = { 0.78539816339745, -2.3561944901923, 0.78539816339745, 0.46364760900081, -2.3561944901923, -0.78539816339745 };
	double ret_val[6];
	int atan2f_idx;

	/* Returns the principal value of the arc tangent of x, expressed in radians */

	for (atan2f_idx = 0; atan2f_idx < 6; atan2f_idx++) {
		ret_val[atan2f_idx] = atan2f(in_val1[atan2f_idx], in_val2[atan2f_idx]);
		TC_ASSERT_LEQ("atan2f", fabs(sol_val[atan2f_idx] - ret_val[atan2f_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atan2l
 * @brief               :Returns the principal value of the arc tangent of x
 * @scenario            :Returns the principal value of the arc tangent of x
 * @API's covered       :atan2l
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atan2l(void)
{
	const double in_val1[6] = { 0.50, -0.50, 5, 10, -5, -10 };
	const double in_val2[6] = { 0.50, -0.50, 5, 20, -5, 10 };
	const double sol_val[6] = { 0.78539816339745, -2.3561944901923, 0.78539816339745, 0.46364760900081, -2.3561944901923, -0.78539816339745 };
	double ret_val[6];
	int atan2l_idx;

	/* Returns the principal value of the arc tangent of x, expressed in radians */

	for (atan2l_idx = 0; atan2l_idx < 6; atan2l_idx++) {
		ret_val[atan2l_idx] = atan2l(in_val1[atan2l_idx], in_val2[atan2l_idx]);
		TC_ASSERT_LEQ("atan2l", fabs(sol_val[atan2l_idx] - ret_val[atan2l_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_copysign
 * @brief                :Returns a value with the magnitude of x and the sign of y
 * @Scenario             :Returns a value with the magnitude of x and the sign of y
 * API's covered         :copysign
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_copysign(void)
{
	const double in_val1[3] = { 10.0, -10.0, -10.0 };
	const double in_val2[3] = { -1.0, -1.0, 1.0 };
	const double sol_val[3] = { -10.0, -10.0, 10.0 };

	double ret_val[3];
	int copysign_idx;

	/* Returns a value with the magnitude of x and the sign of y */

	for (copysign_idx = 0; copysign_idx < 3; copysign_idx++) {
		ret_val[copysign_idx] = copysign(in_val1[copysign_idx], in_val2[copysign_idx]);
		TC_ASSERT_LEQ("copysign", fabs(sol_val[copysign_idx] - ret_val[copysign_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_copysignf
 * @brief                :Returns a value with the magnitude of x and the sign of y
 * @Scenario             :Returns a value with the magnitude of x and the sign of y
 * API's covered         :copysignf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_copysignf(void)
{
	const double in_val1[3] = { 10.0, -10.0, -10.0 };
	const double in_val2[3] = { -1.0, -1.0, 1.0 };
	const double sol_val[3] = { -10.0, -10.0, 10.0 };

	double ret_val[3];
	int copysignf_idx;

	/* Returns a value with the magnitude of x and the sign of y */

	for (copysignf_idx = 0; copysignf_idx < 3; copysignf_idx++) {
		ret_val[copysignf_idx] = copysignf(in_val1[copysignf_idx], in_val2[copysignf_idx]);
		TC_ASSERT_LEQ("copysignf", fabs(sol_val[copysignf_idx] - ret_val[copysignf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_copysignl
 * @brief                :Returns a value with the magnitude of x and the sign of y
 * @Scenario             :Returns a value with the magnitude of x and the sign of y
 * API's covered         :copysignl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_copysignl(void)
{
	const double in_val1[3] = { 10.0, -10.0, -10.0 };
	const double in_val2[3] = { -1.0, -1.0, 1.0 };
	const double sol_val[3] = { -10.0, -10.0, 10.0 };

	double ret_val[3];
	int copysignl_idx;

	/* Returns a value with the magnitude of x and the sign of y */

	for (copysignl_idx = 0; copysignl_idx < 3; copysignl_idx++) {
		ret_val[copysignl_idx] = copysignl(in_val1[copysignl_idx], in_val2[copysignl_idx]);
		TC_ASSERT_LEQ("copysignl", fabs(sol_val[copysignl_idx] - ret_val[copysignl_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_cos
 * @brief               :Returns the cosine of an angle of x radians
 * @scenario            :Returns the cosine of an angle of x radians
 * @API's covered       :cos
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_cos(void)
{
	const double in_val[5] = { 3, -3, 0, M_PI, (2 * M_PI) };
	const double sol_val[5] = { -0.98999249660045, -0.98999249660045, 1, -1, 1 };
	double ret_val[5];
	int cos_idx;

	/* Returns the cosine of an angle of x radians */

	for (cos_idx = 0; cos_idx < 5; cos_idx++) {
		ret_val[cos_idx] = cos(in_val[cos_idx]);
		TC_ASSERT_LEQ("cos", fabs(sol_val[cos_idx] - ret_val[cos_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_cosf
 * @brief               :Returns the cosine of an angle of x radians
 * @scenario            :Returns the cosine of an angle of x radians
 * @API's covered       :cosf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_cosf(void)
{
	const double in_val[5] = { 3, -3, 0, M_PI, (2 * M_PI) };
	const double sol_val[5] = { -0.98999249660045, -0.98999249660045, 1, -1, 1 };
	double ret_val[5];
	int cosf_idx;

	/* Returns the cosfine of an angle of x radians */

	for (cosf_idx = 0; cosf_idx < 5; cosf_idx++) {
		ret_val[cosf_idx] = cosf(in_val[cosf_idx]);
		TC_ASSERT_LEQ("cosf", fabs(sol_val[cosf_idx] - ret_val[cosf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_cosh
 * @brief               :Returns the hyperbolic cosine of x
 * @scenario            :Returns the hyperbolic cosine of x
 * @API's covered       :cosh
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_cosh(void)
{
	const double in_val[5] = { 3, -3, 0, M_PI, (2 * M_PI) };
	const double sol_val[5] = { 10.067661995778, 10.067661995778, 1, 11.591953275522, 267.74676148375 };
	double ret_val[5];
	int cosh_idx;

	/* Returns the hyperbolic cosine of x */

	for (cosh_idx = 0; cosh_idx < 5; cosh_idx++) {
		ret_val[cosh_idx] = cosh(in_val[cosh_idx]);
		TC_ASSERT_LEQ("cosh", fabs(sol_val[cosh_idx] - ret_val[cosh_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_coshf
 * @brief               :Returns the hyperbolic cosine of x
 * @scenario            :Returns the hyperbolic cosine of x
 * @API's covered       :coshf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_coshf(void)
{
	const double in_val[5] = { 3, -3, 0, M_PI, (2 * M_PI) };
	const double sol_val[5] = { 10.0676622390747, 10.0676622390747, 1.0000000000000, 11.5919551849365, 267.7468261718750 };
	double ret_val[5];
	int coshf_idx;

	/* Returns the hyperbolic cosine of x */

	for (coshf_idx = 0; coshf_idx < 5; coshf_idx++) {
		ret_val[coshf_idx] = coshf(in_val[coshf_idx]);
		TC_ASSERT_LEQ("coshf", fabs(sol_val[coshf_idx] - ret_val[coshf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_coshl
 * @brief               :Returns the hyperbolic cosine of x
 * @scenario            :Returns the hyperbolic cosine of x
 * @API's covered       :coshl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_coshl(void)
{
	const double in_val[5] = { 3, -3, 0, M_PI, (2 * M_PI) };
	const double sol_val[5] = { 10.067661995778, 10.067661995778, 1, 11.591953275522, 267.74676148375 };
	double ret_val[5];
	int coshl_idx;

	/* Returns the hyperbolic cosine of x */

	for (coshl_idx = 0; coshl_idx < 5; coshl_idx++) {
		ret_val[coshl_idx] = coshl(in_val[coshl_idx]);
		TC_ASSERT_LEQ("coshl", fabs(sol_val[coshl_idx] - ret_val[coshl_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_cosl
 * @brief               :Returns the cosine of an angle of x radians
 * @scenario            :Returns the cosine of an angle of x radians
 * @API's covered       :cosl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_cosl(void)
{
	const double in_val[5] = { 3, -3, 0, M_PI, (2 * M_PI) };
	const double sol_val[5] = { -0.98999249660045, -0.98999249660045, 1, -1, 1 };
	double ret_val[5];
	int cosl_idx;

	/* Returns the cosline of an angle of x radians */

	for (cosl_idx = 0; cosl_idx < 5; cosl_idx++) {
		ret_val[cosl_idx] = cosl(in_val[cosl_idx]);
		TC_ASSERT_LEQ("cosl", fabs(sol_val[cosl_idx] - ret_val[cosl_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_erf
 * @brief                :Returns the error function value for x
 * @Scenario             :Returns the error function value for x
 * API's covered         :erf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_erf(void)
{
	const double in_val[5] = { 1, -1, ZERO, 0.5, -0.5 };
	const double sol_val[5] = { 0.8427006858353, -217.6481312007792, 0, 0.52049987781, -132.8284539391262 };
	double ret_val[5];
	int erf_idx;

	/* Returns the error function value for x */

	for (erf_idx = 0; erf_idx < 5; erf_idx++) {
		ret_val[erf_idx] = erf(in_val[erf_idx]);
		TC_ASSERT_LEQ("erf", fabs(sol_val[erf_idx] - ret_val[erf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_erff
 * @brief                :Returns the error function value for x
 * @Scenario             :Returns the error function value for x
 * API's covered         :erff
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_erff(void)
{
	const double in_val[5] = { 1, -1, ZERO, 0.5, -0.5 };
	const double sol_val[5] = { 0.8427006858353, -217.6480102539062, 0, 0.52049987781, -132.8285064697266 };
	double ret_val[5];
	int erff_idx;

	/* Returns the error function value for x */

	for (erff_idx = 0; erff_idx < 5; erff_idx++) {
		ret_val[erff_idx] = erff(in_val[erff_idx]);
		TC_ASSERT_LEQ("erff", fabs(sol_val[erff_idx] - ret_val[erff_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_erfl
 * @brief                :Returns the error function value for x
 * @Scenario             :Returns the error function value for x
 * API's covered         :erfl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_erfl(void)
{
	const double in_val[5] = { 1, -1, ZERO, 0.5, -0.5 };
	const double sol_val[5] = { 0.8427006858353, -217.6481312007792, 0, 0.52049987781, -132.8284539391262 };
	double ret_val[5];
	int erfl_idx;

	/* Returns the error function value for x */

	for (erfl_idx = 0; erfl_idx < 5; erfl_idx++) {
		ret_val[erfl_idx] = erfl(in_val[erfl_idx]);
		TC_ASSERT_LEQ("erfl", fabs(sol_val[erfl_idx] - ret_val[erfl_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

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
 * @fn                   :tc_libc_math_ldexp
 * @brief                :Returns the result of multiplying x (the significand) by 2 raised to the power of exp (the exponent)
 * @Scenario             :Returns the result of multiplying x (the significand) by 2 raised to the power of exp (the exponent)
 * API's covered         :ldexp
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_ldexp(void)
{
	const double in_val[3] = { 0.95, 0.0, -10.0 };
	const double sol_val[3] = { 15.2, 0, -10240 };
	int n[3] = { 4, -2, 10 };

	double ret_val[3];
	int ldexp_idx;

	/* Breaks the floating point number x into its binary significand and an integral exponent for 2 */

	for (ldexp_idx = 0; ldexp_idx < 3; ldexp_idx++) {
		ret_val[ldexp_idx] = ldexp(in_val[ldexp_idx], n[ldexp_idx]);
		TC_ASSERT_LEQ("ldexp", fabs(sol_val[ldexp_idx] - ret_val[ldexp_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_ldexpf
 * @brief                :Returns the result of multiplying x (the significand) by 2 raised to the power of exp (the exponent)
 * @Scenario             :Returns the result of multiplying x (the significand) by 2 raised to the power of exp (the exponent)
 * API's covered         :ldexpf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_ldexpf(void)
{
	float in_val[3] = { 0.95, 0, -10 };
	float sol_val[3] = { 15.1999998092651, 0, -10239.9990234375000 };
	int n[3] = { 4, -2, 10 };

	float ret_val[3];
	int ldexpf_idx;

	/* Breaks the floating point number x into its binary significand and an integral exponent for 2 */

	for (ldexpf_idx = 0; ldexpf_idx < 3; ldexpf_idx++) {
		ret_val[ldexpf_idx] = ldexpf(in_val[ldexpf_idx], n[ldexpf_idx]);
		TC_ASSERT_LEQ("ldexpf", fabs(sol_val[ldexpf_idx] - ret_val[ldexpf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_ldexpl
 * @brief                :Returns the result of multiplying x (the significand) by 2 raised to the power of exp (the exponent)
 * @Scenario             :Returns the result of multiplying x (the significand) by 2 raised to the power of exp (the exponent)
 * API's covered         :ldexpl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_ldexpl(void)
{
	const double in_val[3] = { 0.95, 0.0, -10.0 };
	const double sol_val[3] = { 15.2, 0, -10240 };
	double ret_val[3];
	int n[3] = { 4, -2, 10 };
	int ldexpl_idx;

	/* Breaks the floating point number x into its binary significand and an integral exponent for 2 */

	for (ldexpl_idx = 0; ldexpl_idx < 3; ldexpl_idx++) {
		ret_val[ldexpl_idx] = ldexpl(in_val[ldexpl_idx], n[ldexpl_idx]);
		TC_ASSERT_LEQ("ldexpl", fabs(sol_val[ldexpl_idx] - ret_val[ldexpl_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_log2
 * @brief                :Returns the binary (base-2) logarithm of x
 * @Scenario             :Returns the binary (base-2) logarithm of x
 * API's covered         :log2
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_log2(void)
{
	const double in_val[3] = { 1024, 5.5, -10.0 };
	const double sol_val[3] = { 10.0, 2.4594316186373, NAN };
	double ret_val[3];
	int log2_idx;

	/* Returns the binary (base-2) logarithm of x */

	for (log2_idx = 0; log2_idx < 3; log2_idx++) {
		ret_val[log2_idx] = log2(in_val[log2_idx]);
		TC_ASSERT_LEQ("log2", fabs(sol_val[log2_idx] - ret_val[log2_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_log2f
 * @brief                :Returns the binary (base-2) logarithm of x
 * @Scenario             :Returns the binary (base-2) logarithm of x
 * API's covered         :log2f
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_log2f(void)
{
	const double in_val[3] = { 1024, 5.5, -10.0 };
	const double sol_val[3] = { 10.0, 2.4594316186373, NAN };
	double ret_val[3];
	int log2f_idx;

	/* Returns the binary (base-2) logarithm of x */

	for (log2f_idx = 0; log2f_idx < 3; log2f_idx++) {
		ret_val[log2f_idx] = log2f(in_val[log2f_idx]);
		TC_ASSERT_LEQ("log2f", fabs(sol_val[log2f_idx] - ret_val[log2f_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_log2l
 * @brief                :Returns the binary (base-2) logarithm of x
 * @Scenario             :Returns the binary (base-2) logarithm of x
 * API's covered         :log2l
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_log2l(void)
{
	const double in_val[3] = { 1024, 5.5, -10.0 };
	const double sol_val[3] = { 10.0, 2.4594316186373, NAN };
	double ret_val[3];
	int log2l_idx;

	/* Returns the binary (base-2) logarithm of x */

	for (log2l_idx = 0; log2l_idx < 3; log2l_idx++) {
		ret_val[log2l_idx] = log2l(in_val[log2l_idx]);
		TC_ASSERT_LEQ("log2l", fabs(sol_val[log2l_idx] - ret_val[log2l_idx]), FLT_EPSILON);
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
	const double in_val1[7] = { VAL1, VAL1, ZERO, -VAL1, -VAL2, VAL1, NAN };
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
	const double in_val1[7] = { VAL1, VAL1, ZERO, -VAL1, -VAL2, VAL1, NAN };
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
	const double in_val[13][2] = { {2, 2}, {2, 0.5}, {2, -2}, {2, -0.5}, {1, NAN}, {ZERO, 0.5}, {ZERO, -2}, { -ZERO, -1}, { -ZERO, -2}, { -2, -1}, { -2, -2}, {0.5, INFINITY}, {2, INFINITY} };
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
	int sol_quo[10] = { 0, 0, -1, -1, -2, -2, 0, 0, 0, 0 };
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
	const double sol_val[8][6] = { {ZERO, ZERO, ZERO, ZERO, ZERO, ZERO}, {9007199254740994.0, 17592186044416.003906250, ZERO, INFINITY, INFINITY, ZERO}, { -9007199254740994.0, -17592186044416.003906250, -ZERO, -INFINITY, -INFINITY, -ZERO}, {18014398509481984.0, 35184372088832.0, ZERO, INFINITY, INFINITY, ZERO}, { -18014398509481984.0, -35184372088832.0, -ZERO, -INFINITY, -INFINITY, -ZERO}, {INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY}, { -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY}, {NAN, NAN, NAN, NAN, NAN, NAN} };
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
 * @fn                  :tc_libc_math_sin
 * @brief               :Returns the sine of an angle of x radians
 * @scenario            :Returns the sine of an angle of x radians
 * @API's covered       :sin
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_sin(void)
{
	const double in_val[5] = { 3, -3, 0, M_PI, M_PI_2 };
	const double sol_val[5] = { 0.14112000805987, -0.14112000805987, 0, 1.2246467991474e-16, 1 };
	double ret_val[5];
	int sin_idx;

	/* Returns the sine of an angle of x radians */

	for (sin_idx = 0; sin_idx < 5; sin_idx++) {
		ret_val[sin_idx] = sin(in_val[sin_idx]);
		TC_ASSERT_LEQ("sin", fabs(sol_val[sin_idx] - ret_val[sin_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_sinf
 * @brief               :Returns the sine of an angle of x radians
 * @scenario            :Returns the sine of an angle of x radians
 * @API's covered       :sinf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_sinf(void)
{
	const double in_val[5] = { 3, -3, 0, M_PI_2 };
	const double sol_val[5] = { 0.141120, -0.141120, 0.000000, 1 };
	double ret_val[5];
	int sinf_idx;

	/* Returns the sine of an angle of x radians */

	for (sinf_idx = 0; sinf_idx < 5; sinf_idx++) {
		ret_val[sinf_idx] = sinf(in_val[sinf_idx]);
		TC_ASSERT_LEQ("sinf", fabs(sol_val[sinf_idx] - ret_val[sinf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_sinh
 * @brief               :Returns the sine of an angle of x radians
 * @scenario            :Returns the sine of an angle of x radians
 * @API's covered       :sinh
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_sinh(void)
{
	const double in_val[5] = { 3, -3, 0, M_PI, M_PI_2 };
	const double sol_val[5] = { 10.01787492741, -10.01787492741, 0, 11.548739357258, 2.3012989023073 };
	double ret_val[5];
	int sinh_idx;

	/* Returns the sine of an angle of x radians */

	for (sinh_idx = 0; sinh_idx < 5; sinh_idx++) {
		ret_val[sinh_idx] = sinh(in_val[sinh_idx]);
		TC_ASSERT_LEQ("sinh", fabs(sol_val[sinh_idx] - ret_val[sinh_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_sinhf
 * @brief               :Returns the sine of an angle of x radians
 * @scenario            :Returns the sine of an angle of x radians
 * @API's covered       :sinhf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_sinhf(void)
{
	const double in_val[5] = { 3, -3, 0, M_PI, M_PI_2 };
	const double sol_val[5] = { 10.0178747177124, -10.0178756713867, 0.0000000000000, 11.5487403869629, 2.3012990951538 };
	double ret_val[5];
	int sinhf_idx;

	/* Returns the sine of an angle of x radians */

	for (sinhf_idx = 0; sinhf_idx < 5; sinhf_idx++) {
		ret_val[sinhf_idx] = sinhf(in_val[sinhf_idx]);
		TC_ASSERT_LEQ("sinhf", fabs(sol_val[sinhf_idx] - ret_val[sinhf_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_sinhl
 * @brief               :Returns the sine of an angle of x radians
 * @scenario            :Returns the sine of an angle of x radians
 * @API's covered       :sinhl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_sinhl(void)
{
	const double in_val[5] = { 3, -3, 0, M_PI, M_PI_2 };
	const double sol_val[5] = { 10.01787492741, -10.01787492741, 0, 11.548739357258, 2.3012989023073 };
	double ret_val[5];
	int sinhl_idx;

	/* Returns the sine of an angle of x radians */

	for (sinhl_idx = 0; sinhl_idx < 5; sinhl_idx++) {
		ret_val[sinhl_idx] = sinhl(in_val[sinhl_idx]);
		TC_ASSERT_LEQ("sinhl", fabs(sol_val[sinhl_idx] - ret_val[sinhl_idx]), FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_sinl
 * @brief               :Returns the sine of an angle of x radians
 * @scenario            :Returns the sine of an angle of x radians
 * @API's covered       :sinl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_sinl(void)
{
	const double in_val[5] = { 3, -3, 0, M_PI, M_PI_2 };
	const double sol_val[5] = { 0.14112000805987, -0.14112000805987, 0, 1.2246467991474e-16, 1 };
	double ret_val[5];
	int sinl_idx;

	/* Returns the sine of an angle of x radians */

	for (sinl_idx = 0; sinl_idx < 5; sinl_idx++) {
		ret_val[sinl_idx] = sinl(in_val[sinl_idx]);
		TC_ASSERT_LEQ("sinl", fabs(sol_val[sinl_idx] - ret_val[sinl_idx]), FLT_EPSILON);
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
	tc_libc_math_acos();
	tc_libc_math_acosf();
	tc_libc_math_acosl();
	tc_libc_math_acosh();
	tc_libc_math_acoshf();
	tc_libc_math_acoshl();
	tc_libc_math_asin();
	tc_libc_math_asinf();
	tc_libc_math_asinl();
	tc_libc_math_asinh();
	tc_libc_math_asinhf();
	tc_libc_math_asinhl();
	tc_libc_math_atan();
	tc_libc_math_atanf();
	tc_libc_math_atanl();
	tc_libc_math_atanh();
	tc_libc_math_atanhf();
	tc_libc_math_atanhl();
	tc_libc_math_atan2();
	tc_libc_math_atan2f();
	tc_libc_math_atan2l();
	tc_libc_math_copysign();
	tc_libc_math_copysignf();
	tc_libc_math_copysignl();
	tc_libc_math_cos();
	tc_libc_math_cosf();
	tc_libc_math_cosh();
	tc_libc_math_coshf();
	tc_libc_math_coshl();
	tc_libc_math_cosl();
	tc_libc_math_cbrt();
	tc_libc_math_exp2();
	tc_libc_math_erf();
	tc_libc_math_erff();
	tc_libc_math_erfl();
	tc_libc_math_fabs();
	tc_libc_math_fdim();
	tc_libc_math_floor();
	tc_libc_math_fmax();
	tc_libc_math_fmin();
	tc_libc_math_hypot();
	tc_libc_math_j0();
	tc_libc_math_j1();
	tc_libc_math_jn();
	tc_libc_math_ldexp();
	tc_libc_math_ldexpf();
	tc_libc_math_ldexpl();
	tc_libc_math_log2();
	tc_libc_math_log2f();
	tc_libc_math_log2l();
	tc_libc_math_nextafter();
	tc_libc_math_nexttoward();
	tc_libc_math_pow();
	tc_libc_math_remainder();
	tc_libc_math_remquo();
	tc_libc_math_scalbn();
	tc_libc_math_sin();
	tc_libc_math_sinf();
	tc_libc_math_sinh();
	tc_libc_math_sinhf();
	tc_libc_math_sinhl();
	tc_libc_math_sinl();
	tc_libc_math_y0();
	tc_libc_math_y1();
	tc_libc_math_yn();
#else
	printf("tc_libc_math will not be executed. CONFIG_HAVE_DOUBLE is not set.\n");
#endif
	return 0;
}
