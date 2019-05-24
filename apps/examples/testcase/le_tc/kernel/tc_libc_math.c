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

/// @file tc_libc_math.c
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

#define ONE 1.0

/* dobule has 52 bit fraction(IEEE754). 4503599627370496 == 0x10000000000000 */

#define ZERO 0.0
#define VAL1 2251799813685248.5
#define VAL2 4503599627370496.5
#define SIZE(var, type) (sizeof(var) / sizeof(type))

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
	const double in_val[] = { 0.64, -0.4, ZERO, -1, 1, 1.1, -2, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 0.876298061168341, 1.982313172862385, 1.5707963267949, 3.141592653589793, 0, NAN, NAN, NAN, NAN, NAN };
	double ret_val[SIZE(in_val, double)];
	int acos_idx;
	double compute_val;


	/* Returns the principal value of the arc cosine of x */

	for (acos_idx = 0; acos_idx < SIZE(in_val, double); acos_idx++) {
		ret_val[acos_idx] = acos(in_val[acos_idx]);
		compute_val = fabs(sol_val[acos_idx] - ret_val[acos_idx]);

		if ((isnan(sol_val[acos_idx]) ^ (isnan(ret_val[acos_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("acos", compute_val, FLT_EPSILON);
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
	const float in_val[] = { 0.64, -0.4, 0, -1, 1, 1.1, -2, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { 0.8762981, 1.9823132, 1.5707964, 3.14120793, 0, NAN, NAN, NAN, NAN, NAN };
	float ret_val[SIZE(in_val, float)];
	int acosf_idx;
	float compute_val;


	/* Returns the nonnegative area hyperbolic cosine of x */

	for (acosf_idx = 0; acosf_idx < SIZE(in_val, float); acosf_idx++) {
		ret_val[acosf_idx] = acosf(in_val[acosf_idx]);
		compute_val = fabsf(sol_val[acosf_idx] - ret_val[acosf_idx]);

		if ((isnan(sol_val[acosf_idx]) ^ (isnan(ret_val[acosf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("acosf", compute_val, FLT_EPSILON);
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
	const double in_val[] = { 0.64, ZERO, -1, 1, 1.1, 1024, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { -NAN, -NAN, -NAN, 0, 0.443568254385115, 7.624618747740734, INFINITY, -NAN, NAN };
	double ret_val[SIZE(in_val, double)];
	int acosh_idx;
	double compute_val;

	/* Returns the nonnegative area hyperbolic cosine of x */

	for (acosh_idx = 0; acosh_idx < SIZE(in_val, double); acosh_idx++) {
		ret_val[acosh_idx] = acosh(in_val[acosh_idx]);
		compute_val = fabs(sol_val[acosh_idx] - ret_val[acosh_idx]);

		if ((isnan(sol_val[acosh_idx]) ^ (isnan(ret_val[acosh_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("acosh", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_acoshf
 * @brief               :Returns the nonnegative area hyperbolic cosine of x
 * @scenario            :Returns the nonnegative area hyperbolic cosine of x
 * @API's covered       :acoshf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_acoshf(void)
{
	const float in_val[] = { 0.64, ZERO, -1, 1, 1.1, 7, 28, 2.45, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { NAN, NAN, NAN, 0, 0.4435682, 2.6339158, 4.0250325, 1.5447131, INFINITY, NAN, NAN };
	float ret_val[SIZE(in_val, float)];
	int acoshf_idx;
	float compute_val;

	/* Returns the nonnegative area hyperbolic cosine of x */

	for (acoshf_idx = 0; acoshf_idx < SIZE(in_val, float); acoshf_idx++) {
		ret_val[acoshf_idx] = acoshf(in_val[acoshf_idx]);
		compute_val = fabsf(sol_val[acoshf_idx] - ret_val[acoshf_idx]);

		if ((isnan(sol_val[acoshf_idx]) ^ (isnan(ret_val[acoshf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("acoshf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_acoshl
 * @brief               :Returns the nonnegative area hyperbolic cosine of x
 * @scenario            :Returns the nonnegative area hyperbolic cosine of x
 * @API's covered       :acoshl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_acoshl(void)
{
	const long double in_val[] = { 0.64, ZERO, -1, 1, 1.1, 1024, 7, 56, 2.45, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { -NAN, -NAN, -NAN, 0, 0.443568254385115, 7.624618747740734, 2.6339157938496, 4.7184191423729, 1.5447131178707, INFINITY, -NAN, NAN };
	long double ret_val[SIZE(in_val, long double)];
	int acoshl_idx;
	long double compute_val;

	/* Returns the nonnegative area hyperbolic cosine of x */

	for (acoshl_idx = 0; acoshl_idx < SIZE(in_val, long double); acoshl_idx++) {
		ret_val[acoshl_idx] = acoshl(in_val[acoshl_idx]);
		compute_val = fabsl(sol_val[acoshl_idx] - ret_val[acoshl_idx]);

		if ((isnan(sol_val[acoshl_idx]) ^ (isnan(ret_val[acoshl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("acoshl", compute_val, FLT_EPSILON);
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
	const long double in_val[] = { 0.64, -0.4, ZERO, -1, 1, 1.1, -2, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { 0.876298061168341, 1.982313172862385, 1.570796326794897, 3.141592653589793, 0, NAN, NAN, NAN, NAN, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int acosl_idx;
	long double compute_val;

	/* Returns the nonnegative area hyperbolic cosine of x */

	for (acosl_idx = 0; acosl_idx < SIZE(sol_val, long double); acosl_idx++) {
		ret_val[acosl_idx] = acosl(in_val[acosl_idx]);
		compute_val = fabsl(sol_val[acosl_idx] - ret_val[acosl_idx]);

		if ((isnan(sol_val[acosl_idx]) ^ (isnan(ret_val[acosl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("acosl", compute_val, FLT_EPSILON);
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
	const double in_val[] = { 0.64, -0.4, ZERO, -1, 1, 1.1, -2, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 0.694498265626556, -0.411516846067488, 0, -1.5707963267949, 1.5707963267949, NAN, NAN, NAN, NAN, NAN };
	double ret_val[SIZE(sol_val, double)];
	int asin_idx;
	double compute_val;

	/* Returns the principal value of the arc sine of x */

	for (asin_idx = 0; asin_idx < SIZE(sol_val, double); asin_idx++) {
		ret_val[asin_idx] = asin(in_val[asin_idx]);
		compute_val = fabs(sol_val[asin_idx] - ret_val[asin_idx]);

		if ((isnan(sol_val[asin_idx]) ^ (isnan(ret_val[asin_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("asin", compute_val, FLT_EPSILON);
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
	const float in_val[] = { 0.64, -0.4, ZERO, -1, 1, 1.1, -2, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { 0.694498240947723, -0.411516845226288, 0, -1.5704115629196, 1.5703850984573, NAN, NAN, NAN, NAN, NAN };
	float ret_val[SIZE(sol_val, float)];
	int asinf_idx;
	float compute_val;

	/* Returns the principal value of the arc sine of x */

	for (asinf_idx = 0; asinf_idx < SIZE(sol_val, float); asinf_idx++) {
		ret_val[asinf_idx] = asinf(in_val[asinf_idx]);
		compute_val = fabsf(sol_val[asinf_idx] - ret_val[asinf_idx]);

		if ((isnan(sol_val[asinf_idx]) ^ (isnan(ret_val[asinf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("asinf", compute_val, FLT_EPSILON);
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
	const double in_val[] = { 7, 56, 2.45, 0, -2, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 2.6441207610586, 4.7185785811518, 1.6284998192842, 0, -1.443635475178810, INFINITY, -INFINITY, NAN };
	double ret_val[SIZE(sol_val, double)];
	int asinh_idx;
	double compute_val;

	/* Returns the area hyperbolic sine of x */

	for (asinh_idx = 0; asinh_idx < SIZE(sol_val, double); asinh_idx++) {
		ret_val[asinh_idx] = asinh(in_val[asinh_idx]);
		compute_val = fabs(sol_val[asinh_idx] - ret_val[asinh_idx]);

		if ((isnan(sol_val[asinh_idx]) ^ (isnan(ret_val[asinh_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("asinh", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_asinhf
 * @brief               :Returns the area hyperbolic sine of x
 * @scenario            :Returns the area hyperbolic sine of x
 * @API's covered       :asinhf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_asinhf(void)
{
	const float in_val[] = { 7, 56, 2.45, 0, -2, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { 2.6441207610586, 4.7185785811518, 1.6284998192842, 0, -1.443635463714600, INFINITY, -INFINITY, NAN };
	float ret_val[SIZE(sol_val, float)];
	int asinhf_idx;
	float compute_val;

	/* Returns the area hyperbolic sine of x */

	for (asinhf_idx = 0; asinhf_idx < SIZE(sol_val, float); asinhf_idx++) {
		ret_val[asinhf_idx] = asinhf(in_val[asinhf_idx]);
		compute_val = fabsf(sol_val[asinhf_idx] - ret_val[asinhf_idx]);

		if ((isnan(sol_val[asinhf_idx]) ^ (isnan(ret_val[asinhf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("asinhf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_asinhl
 * @brief               :Returns the area hyperbolic sine of x
 * @scenario            :Returns the area hyperbolic sine of x
 * @API's covered       :asinhl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_asinhl(void)
{
	const long double in_val[] = { 7, 56, 2.45, 0, -2, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { 2.6441207610586, 4.7185785811518, 1.6284998192842, 0, -1.443635475178810, INFINITY, -INFINITY, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int asinhl_idx;
	long double compute_val;

	/* Returns the area hyperbolic sine of x */

	for (asinhl_idx = 0; asinhl_idx < SIZE(sol_val, long double); asinhl_idx++) {
		ret_val[asinhl_idx] = asinhl(in_val[asinhl_idx]);
		compute_val = fabsl(sol_val[asinhl_idx] - ret_val[asinhl_idx]);

		if ((isnan(sol_val[asinhl_idx]) ^ (isnan(ret_val[asinhl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("asinhl", compute_val, FLT_EPSILON);
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
	const long double in_val[] = { 0.64, -0.4, ZERO, -1, 1, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { 0.69449826562656, -0.41151684606749, 0, -1.5707963267949, 1.5707963267949, NAN, NAN, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int asinl_idx;
	long double compute_val;

	/* Returns the principal value of the arc sine of x */

	for (asinl_idx = 0; asinl_idx < SIZE(sol_val, long double); asinl_idx++) {
		ret_val[asinl_idx] = asinl(in_val[asinl_idx]);
		compute_val = fabsl(sol_val[asinl_idx] - ret_val[asinl_idx]);

		if ((isnan(sol_val[asinl_idx]) ^ (isnan(ret_val[asinl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("asinl", compute_val, FLT_EPSILON);
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
	const double in_val[] = { 0.50, -0.50, 5, -5, 100, -100, 0, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 0.46364760900081, -0.46364760900081, 1.373400766945, -1.373400766945, 1.5607966601082, -1.5607966601082, 0, (PI / 2), -(PI / 2), NAN };
	double ret_val[SIZE(sol_val, double)];
	int atan_idx;
	double compute_val;

	/* Returns the principal value of the arc tangent of x */

	for (atan_idx = 0; atan_idx < SIZE(sol_val, double); atan_idx++) {
		ret_val[atan_idx] = atan(in_val[atan_idx]);
		compute_val = fabs(sol_val[atan_idx] - ret_val[atan_idx]);

		if ((isnan(sol_val[atan_idx]) ^ (isnan(ret_val[atan_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("atan", compute_val, FLT_EPSILON);
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
	const float in_val[] = { 0.50, -0.50, 5, -5, 100, -100, 0, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { 0.4636476039886, -0.4636476039886, 1.3734011650085, -1.3734012842178, 1.5608066320419, -1.5607856512070, 0, (PI / 2), -(PI / 2), NAN };
	float ret_val[SIZE(sol_val, float)];
	int atanf_idx;
	float compute_val;

	/* Returns the principal value of the arc tangent of x */

	for (atanf_idx = 0; atanf_idx < SIZE(sol_val, float); atanf_idx++) {
		ret_val[atanf_idx] = atanf(in_val[atanf_idx]);
		compute_val = fabsf(sol_val[atanf_idx] - ret_val[atanf_idx]);

		if ((isnan(sol_val[atanf_idx]) ^ (isnan(ret_val[atanf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("atanf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atanh
 * @brief               :Returns the area hyperbolic tangent of x
 * @scenario            :Returns the area hyperbolic tangent of x
 * @API's covered       :atanh
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atanh(void)
{
	const double in_val[] = { M_PI_4, 0.50, -0.50, 1, -1, -5, 0, 3, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 1.0593061708232, 0.54930614433405, -0.54930614433405, INFINITY, -INFINITY, NAN, 0, NAN, NAN, NAN, NAN };
	double ret_val[SIZE(sol_val, double)];
	int atanh_idx;
	double compute_val;

	/* Returns the area hyperbolic tangent of x */

	for (atanh_idx = 0; atanh_idx < SIZE(sol_val, double); atanh_idx++) {
		ret_val[atanh_idx] = atanh(in_val[atanh_idx]);
		compute_val = fabs(sol_val[atanh_idx] - ret_val[atanh_idx]);

		if ((isnan(sol_val[atanh_idx]) ^ (isnan(ret_val[atanh_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("atanh", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atanhf
 * @brief               :Returns the area hyperbolic tangent of x
 * @scenario            :Returns the area hyperbolic tangent of x
 * @API's covered       :atanhf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atanhf(void)
{
	const float in_val[] = { M_PI_4, 0.50, -0.50, 1, -1, -5, 0, 3, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { 1.0593061708232, 0.54930614433405, -0.54930614433405, INFINITY, -INFINITY, NAN, 0, NAN, NAN, NAN, NAN };
	float ret_val[SIZE(sol_val, float)];
	int atanhf_idx;
	float compute_val;

	/* Returns the area hyperbolic tangent of x */

	for (atanhf_idx = 0; atanhf_idx < SIZE(sol_val, float); atanhf_idx++) {
		ret_val[atanhf_idx] = atanhf(in_val[atanhf_idx]);
		compute_val = fabsf(sol_val[atanhf_idx] - ret_val[atanhf_idx]);

		if ((isnan(sol_val[atanhf_idx]) ^ (isnan(ret_val[atanhf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("atanhf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atanhl
 * @brief               :Returns the area hyperbolic tangent of x
 * @scenario            :Returns the area hyperbolic tangent of x
 * @API's covered       :atanhl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atanhl(void)
{
	const long double in_val[] = { M_PI_4, 0.50, -0.50, 1, -1, -5, 0, 3, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { 1.0593061708232, 0.54930614433405, -0.54930614433405, INFINITY, -INFINITY, NAN, 0, NAN, NAN, NAN, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int atanhl_idx;
	long double compute_val;

	/* Returns the area hyperbolic tangent of x */

	for (atanhl_idx = 0; atanhl_idx < SIZE(sol_val, long double); atanhl_idx++) {
		ret_val[atanhl_idx] = atanhl(in_val[atanhl_idx]);
		compute_val = fabsl(sol_val[atanhl_idx] - ret_val[atanhl_idx]);

		if ((isnan(sol_val[atanhl_idx]) ^ (isnan(ret_val[atanhl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("atanhl", compute_val, FLT_EPSILON);
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
	const long double in_val[] = { 0.50, -0.50, 5, -5, 100, -100, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { 0.46364760900081, -0.46364760900081, 1.373400766945, -1.373400766945, 1.5607966601082, -1.5607966601082, (PI / 2), -(PI / 2), NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int atanl_idx;
	long double compute_val;

	/* Returns the principal value of the arc tangent of x */

	for (atanl_idx = 0; atanl_idx < SIZE(sol_val, long double); atanl_idx++) {
		ret_val[atanl_idx] = atanl(in_val[atanl_idx]);
		compute_val = fabsl(sol_val[atanl_idx] - ret_val[atanl_idx]);

		if ((isnan(sol_val[atanl_idx]) ^ (isnan(ret_val[atanl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("atanl", compute_val, FLT_EPSILON);
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
	const double in_val1[] = { -1, 2, 0, NAN, -0, -INFINITY, INFINITY, INFINITY, -INFINITY, INFINITY, -INFINITY, 0.50, -0.50, -0.50, 10, 5 };
	const double in_val2[] = { -0, 0, -1, 1, 0, 0, 0, INFINITY, INFINITY, -INFINITY, -INFINITY, 0.50, -0.50, 0, 0, -5 };
	const double sol_val[] = { -1.570796326794897, 1.570796326794897, 3.141592653589793, NAN, 0.000000000000000, -1.570796326794897, 1.570796326794897, 0.785398163397448, -0.785398163397448, 2.356194490192345, -2.356194490192345, 0.78539816339745, -2.3561944901923, -M_PI_2, M_PI_2, 2.3561944901923 };
	double ret_val[SIZE(sol_val, double)];
	int atan2_idx;
	double compute_val;

	/* Returns the principal value of the arc tangent of y/x */

	for (atan2_idx = 0; atan2_idx < SIZE(in_val1, double); atan2_idx++) {
		ret_val[atan2_idx] = atan2(in_val1[atan2_idx], in_val2[atan2_idx]);
		compute_val = fabs(sol_val[atan2_idx] - ret_val[atan2_idx]);

		if ((isnan(sol_val[atan2_idx]) ^ (isnan(ret_val[atan2_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("atan2", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atan2f
 * @brief               :Returns the principal value of the arc tangent of y/x
 * @scenario            :Returns the principal value of the arc tangent of y/x
 * @API's covered       :atan2f
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atan2f(void)
{
	const float in_val1[] = { -1, 2, 0, NAN, -0, -INFINITY, INFINITY, INFINITY, -INFINITY, INFINITY, -INFINITY, 0.50, -0.50, -0.50, 10, 5, -10 };
	const float in_val2[] = { -0, 0, -1, 1, 0, 0, 0, INFINITY, INFINITY, -INFINITY, -INFINITY, 0.50, -0.50, 0, 0, -5, 10 };
	const float sol_val[] = { -1.570796326794897, 1.570796326794897, 3.141592653589793, NAN, 0.000000000000000, -1.570796326794897, 1.570796326794897, 0.785398163397448, -0.785398163397448, 2.356194490192345, -2.356194490192345, 0.78539816339745, -2.3561944901923, -M_PI_2, M_PI_2, 2.3561944901923, -0.7853981633974 };
	float ret_val[SIZE(sol_val, float)];
	int atan2f_idx;
	float compute_val;

	/* Returns the principal value of the arc tangent of y/x */

	for (atan2f_idx = 0; atan2f_idx < SIZE(in_val1, float); atan2f_idx++) {
		ret_val[atan2f_idx] = atan2f(in_val1[atan2f_idx], in_val2[atan2f_idx]);
		compute_val = fabsf(sol_val[atan2f_idx] - ret_val[atan2f_idx]);

		if ((isnan(sol_val[atan2f_idx]) ^ (isnan(ret_val[atan2f_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("atan2f", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_atan2l
 * @brief               :Returns the principal value of the arc tangent of y/x
 * @scenario            :Returns the principal value of the arc tangent of y/x
 * @API's covered       :atan2l
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_atan2l(void)
{
	const long double in_val1[] = { -1, 2, 0, NAN, -0, -INFINITY, INFINITY, INFINITY, -INFINITY, INFINITY, -INFINITY, 0.50, -0.50, -0.50, 10, 5, -10 };
	const long double in_val2[] = { -0, 0, -1, 1, 0, 0, 0, INFINITY, INFINITY, -INFINITY, -INFINITY, 0.50, -0.50, 0, 0, -5, 10 };
	const long double sol_val[] = { -1.570796326794897, 1.570796326794897, 3.141592653589793, NAN, 0.000000000000000, -1.570796326794897, 1.570796326794897, 0.785398163397448, -0.785398163397448, 2.356194490192345, -2.356194490192345, 0.78539816339745, -2.3561944901923, -M_PI_2, M_PI_2, 2.3561944901923, -0.7853981633974 };
	long double ret_val[SIZE(sol_val, long double)];
	int atan2l_idx;
	long double compute_val;

	/* Returns the principal value of the arc tangent of y/x */

	for (atan2l_idx = 0; atan2l_idx < SIZE(in_val1, long double); atan2l_idx++) {
		ret_val[atan2l_idx] = atan2l(in_val1[atan2l_idx], in_val2[atan2l_idx]);
		compute_val = fabsl(sol_val[atan2l_idx] - ret_val[atan2l_idx]);

		if ((isnan(sol_val[atan2l_idx]) ^ (isnan(ret_val[atan2l_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("atan2l", compute_val, FLT_EPSILON);
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
	const double in_val[] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { ZERO, 131072.0, -131072.0, INFINITY, -INFINITY, NAN };
	double ret_val[SIZE(sol_val, double)];
	int cbrt_idx;
	double compute_val;

	for (cbrt_idx = 0; cbrt_idx < SIZE(sol_val, double); cbrt_idx++) {
		ret_val[cbrt_idx] = cbrt(in_val[cbrt_idx]);
		compute_val = fabs(sol_val[cbrt_idx] - ret_val[cbrt_idx]);

		if ((isnan(sol_val[cbrt_idx]) ^ (isnan(ret_val[cbrt_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("cbrt", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_cbrtf
 * @brief                :Returns the cube root of parameter
 * @Scenario             :Returns the cube root of parameter
 * API's covered         :cbrtf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_cbrtf(void)
{
	const float in_val[] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { ZERO, 131072.0, -131072.0, INFINITY, -INFINITY, NAN };
	float ret_val[SIZE(sol_val, float)];
	int cbrtf_idx;
	float compute_val;

	for (cbrtf_idx = 0; cbrtf_idx < SIZE(sol_val, float); cbrtf_idx++) {
		ret_val[cbrtf_idx] = cbrtf(in_val[cbrtf_idx]);
		compute_val = fabsf(sol_val[cbrtf_idx] - ret_val[cbrtf_idx]);

		if ((isnan(sol_val[cbrtf_idx]) ^ (isnan(ret_val[cbrtf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("cbrtf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_cbrtl
 * @brief                :Returns the cube root of parameter
 * @Scenario             :Returns the cube root of parameter
 * API's covered         :cbrtl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_cbrtl(void)
{
	const long double in_val[] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { ZERO, 131072.0, -131072.0, INFINITY, -INFINITY, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int cbrtl_idx;
	long double compute_val;

	for (cbrtl_idx = 0; cbrtl_idx < SIZE(sol_val, long double); cbrtl_idx++) {
		ret_val[cbrtl_idx] = cbrtl(in_val[cbrtl_idx]);
		compute_val = fabsl(sol_val[cbrtl_idx] - ret_val[cbrtl_idx]);

		if ((isnan(sol_val[cbrtl_idx]) ^ (isnan(ret_val[cbrtl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("cbrtl", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_ceil
 * @brief                :Rounds x upward, returning the smallest integral value that is not less than x
 * @Scenario             :Rounds x upward, returning the smallest integral value that is not less than x
 * API's covered         :ceil
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_ceil(void)
{
	const double in_val[] = { 2.3, 0.3, 1.0, -2.3, -8.0, 0, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 3.0, 1.0, 1.0, -2.0, -8.0, 0, INFINITY, -INFINITY, NAN };
	double ret_val[SIZE(sol_val, double)];
	int ceil_idx;
	double compute_val;

	/* Rounds x upward, returning the smallest integral value that is not less than x */

	for (ceil_idx = 0; ceil_idx < SIZE(sol_val, long double); ceil_idx++) {
		ret_val[ceil_idx] = ceil(in_val[ceil_idx]);
		compute_val = fabs(sol_val[ceil_idx] - ret_val[ceil_idx]);

		if ((isnan(sol_val[ceil_idx]) ^ (isnan(ret_val[ceil_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("ceil", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_ceilf
 * @brief                :Rounds x upward, returning the smallest integral value that is not less than x
 * @Scenario             :Rounds x upward, returning the smallest integral value that is not less than x
 * API's covered         :ceilf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_ceilf(void)
{
	const float in_val[] = { 2.3, 3.8, 1.0, -2.3, -8.0, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { 3.0, 4.0, 1.0, -2.0, -8.0, INFINITY, -INFINITY, NAN };
	float ret_val[SIZE(sol_val, float)];
	int ceilf_idx;
	float compute_val;

	/* Rounds x upward, returning the smallest integral value that is not less than x */

	for (ceilf_idx = 0; ceilf_idx < SIZE(sol_val, float); ceilf_idx++) {
		ret_val[ceilf_idx] = ceilf(in_val[ceilf_idx]);
		compute_val = fabsf(sol_val[ceilf_idx] - ret_val[ceilf_idx]);

		if ((isnan(sol_val[ceilf_idx]) ^ (isnan(ret_val[ceilf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("ceilf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_ceill
 * @brief                :Rounds x upward, returning the smallest integral value that is not less than x
 * @Scenario             :Rounds x upward, returning the smallest integral value that is not less than x
 * API's covered         :ceill
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_ceill(void)
{
	const long double in_val[] = { 2.3, 3.8, 1.0, -2.3, -8.0, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { 3.0, 4.0, 1.0, -2.0, -8.0, INFINITY, -INFINITY, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int ceill_idx;
	long double compute_val;

	/* Rounds x upward, returning the smallest integral value that is not less than x */

	for (ceill_idx = 0; ceill_idx < SIZE(sol_val, long double); ceill_idx++) {
		ret_val[ceill_idx] = ceill(in_val[ceill_idx]);
		compute_val = fabsl(sol_val[ceill_idx] - ret_val[ceill_idx]);

		if ((isnan(sol_val[ceill_idx]) ^ (isnan(ret_val[ceill_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("ceill", compute_val, FLT_EPSILON);
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
	const double in_val1[] = { 10.0, -10.0, -10.0, INFINITY, -INFINITY, NAN };
	const double in_val2[] = { -1.0, -1.0, 1.0, -1.0, 1.0, -1.0 };
	const double sol_val[] = { -10.0, -10.0, 10.0, -INFINITY, INFINITY, -NAN };
	double ret_val[SIZE(sol_val, double)];
	int copysign_idx;
	double compute_val;

	/* Returns a value with the magnitude of x and the sign of y */

	for (copysign_idx = 0; copysign_idx < SIZE(sol_val, double); copysign_idx++) {
		ret_val[copysign_idx] = copysign(in_val1[copysign_idx], in_val2[copysign_idx]);
		compute_val = fabs(sol_val[copysign_idx] - ret_val[copysign_idx]);

		if ((isnan(sol_val[copysign_idx]) ^ (isnan(ret_val[copysign_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("copysign", compute_val, FLT_EPSILON);
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
	const float in_val1[] = { 10.0, -10.0, -10.0, INFINITY, -INFINITY, NAN };
	const float in_val2[] = { -1.0, -1.0, 1.0, -1.0, 1.0, -1.0 };
	const float sol_val[] = { -10.0, -10.0, 10.0, -INFINITY, INFINITY, -NAN };
	float ret_val[SIZE(sol_val, float)];
	int copysignf_idx;
	float compute_val;

	/* Returns a value with the magnitude of x and the sign of y */

	for (copysignf_idx = 0; copysignf_idx < SIZE(sol_val, float); copysignf_idx++) {
		ret_val[copysignf_idx] = copysignf(in_val1[copysignf_idx], in_val2[copysignf_idx]);
		compute_val = fabsf(sol_val[copysignf_idx] - ret_val[copysignf_idx]);

		if ((isnan(sol_val[copysignf_idx]) ^ (isnan(ret_val[copysignf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("copysignf", compute_val, FLT_EPSILON);
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
	const long double in_val1[] = { 10.0, -10.0, -10.0, INFINITY, -INFINITY, NAN };
	const long double in_val2[] = { -1.0, -1.0, 1.0, -1.0, 1.0, -1.0 };
	const long double sol_val[] = { -10.0, -10.0, 10.0, -INFINITY, INFINITY, -NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int copysignl_idx;
	long double compute_val;

	/* Returns a value with the magnitude of x and the sign of y */

	for (copysignl_idx = 0; copysignl_idx < SIZE(sol_val, long double); copysignl_idx++) {
		ret_val[copysignl_idx] = copysignl(in_val1[copysignl_idx], in_val2[copysignl_idx]);
		compute_val = fabsl(sol_val[copysignl_idx] - ret_val[copysignl_idx]);

		if ((isnan(sol_val[copysignl_idx]) ^ (isnan(ret_val[copysignl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("copysignl", compute_val, FLT_EPSILON);
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
	const double in_val[] = { 3, -3, 0, M_PI, (2 * M_PI), INFINITY, -INFINITY, NAN };
	const double sol_val[] = { -0.98999249660045, -0.98999249660045, 1, -1, 1, NAN, NAN, NAN };
	double ret_val[SIZE(sol_val, double)];
	int cos_idx;
	double compute_val;

	/* Returns the cosine of an angle of x radians */

	for (cos_idx = 0; cos_idx < SIZE(sol_val, double); cos_idx++) {
		ret_val[cos_idx] = cos(in_val[cos_idx]);
		compute_val = fabs(sol_val[cos_idx] - ret_val[cos_idx]);

		if ((isnan(sol_val[cos_idx]) ^ (isnan(ret_val[cos_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("cos", compute_val, FLT_EPSILON);
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
	const float in_val[] = { 3, -3, 0, M_PI, (2 * M_PI), INFINITY, -INFINITY, NAN };
	const float sol_val[] = { -0.98999249660045, -0.98999249660045, 1, -1, 1, NAN, NAN, NAN };
	float ret_val[SIZE(sol_val, float)];
	int cosf_idx;
	float compute_val;

	/* Returns the cosine of an angle of x radians */

	for (cosf_idx = 0; cosf_idx < SIZE(sol_val, float); cosf_idx++) {
		ret_val[cosf_idx] = cosf(in_val[cosf_idx]);
		compute_val = fabsf(sol_val[cosf_idx] - ret_val[cosf_idx]);

		if ((isnan(sol_val[cosf_idx]) ^ (isnan(ret_val[cosf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("cosf", compute_val, FLT_EPSILON);
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
	const double in_val[] = { 3, -3, 0, M_PI, (2 * M_PI), 0, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 10.067661995778, 10.067661995778, 1, 11.591953275522, 267.74676148375, 1, INFINITY, INFINITY, NAN };
	double ret_val[SIZE(sol_val, double)];
	int cosh_idx;
	double compute_val;

	/* Returns the hyperbolic cosine of x */

	for (cosh_idx = 0; cosh_idx < SIZE(sol_val, double); cosh_idx++) {
		ret_val[cosh_idx] = cosh(in_val[cosh_idx]);
		compute_val = fabs(sol_val[cosh_idx] - ret_val[cosh_idx]);

		if ((isnan(sol_val[cosh_idx]) ^ (isnan(ret_val[cosh_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("cosh", compute_val, FLT_EPSILON);
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
	const float in_val[] = { 3, -3, 0, M_PI, (2 * M_PI), 0, -INFINITY, INFINITY, NAN };
	const float sol_val[] = { 10.0676622390747, 10.0676622390747, 1.0000000000000, 11.5919551849365, 267.7468261718750, 1, INFINITY, INFINITY, NAN };
	float ret_val[SIZE(sol_val, float)];
	int coshf_idx;
	float compute_val;

	/* Returns the hyperbolic cosine of x */

	for (coshf_idx = 0; coshf_idx < SIZE(sol_val, float); coshf_idx++) {
		ret_val[coshf_idx] = coshf(in_val[coshf_idx]);
		compute_val = fabsf(sol_val[coshf_idx] - ret_val[coshf_idx]);

		if ((isnan(sol_val[coshf_idx]) ^ (isnan(ret_val[coshf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("coshf", compute_val, FLT_EPSILON);
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
	const long double in_val[] = { 3, -3, 0, M_PI, (2 * M_PI), 0, -INFINITY, INFINITY, NAN };
	const long double sol_val[] = { 10.067661995778, 10.067661995778, 1, 11.591953275522, 267.74676148375, 1, INFINITY, INFINITY, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int coshl_idx;
	long double compute_val;

	/* Returns the hyperbolic cosine of x */

	for (coshl_idx = 0; coshl_idx < SIZE(sol_val, long double); coshl_idx++) {
		ret_val[coshl_idx] = coshl(in_val[coshl_idx]);
		compute_val = fabsl(sol_val[coshl_idx] - ret_val[coshl_idx]);

		if ((isnan(sol_val[coshl_idx]) ^ (isnan(ret_val[coshl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("coshl", compute_val, FLT_EPSILON);
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
	const long double in_val[] = { 3, -3, 0, M_PI, (2 * M_PI), INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { -0.98999249660045, -0.98999249660045, 1, -1, 1, NAN, NAN, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int cosl_idx;
	long double compute_val;

	/* Returns the cosine of an angle of x radians */

	for (cosl_idx = 0; cosl_idx < SIZE(sol_val, long double); cosl_idx++) {
		ret_val[cosl_idx] = cosl(in_val[cosl_idx]);
		compute_val = fabsl(sol_val[cosl_idx] - ret_val[cosl_idx]);

		if ((isnan(sol_val[cosl_idx]) ^ (isnan(ret_val[cosl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("cosl", compute_val, FLT_EPSILON);
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
	const double in_val[] = { 1, -1, ZERO, 0.5, -0.5, 0, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 0.842700689747, -0.842700689747, ZERO, 0.520500016304, -0.520500016304, 0, 1, -1, NAN };
	double ret_val[SIZE(sol_val, double)];
	int erf_idx;
	double compute_val;

	/* Returns the error function value for x */

	for (erf_idx = 0; erf_idx < SIZE(sol_val, double); erf_idx++) {
		ret_val[erf_idx] = erf(in_val[erf_idx]);
		compute_val = fabs(sol_val[erf_idx] - ret_val[erf_idx]);

		if ((isnan(sol_val[erf_idx]) ^ (isnan(ret_val[erf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("erf", compute_val, FLT_EPSILON);
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
	const float in_val[] = { 1, -1, ZERO, 0.5, -0.5, 0, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { 0.842700689747, -0.842700689747, ZERO, 0.520500016304, -0.520500016304, 0, 1, -1, NAN };
	float ret_val[SIZE(sol_val, float)];
	int erff_idx;
	float compute_val;

	/* Returns the error function value for x */

	for (erff_idx = 0; erff_idx < SIZE(sol_val, float); erff_idx++) {
		ret_val[erff_idx] = erff(in_val[erff_idx]);
		compute_val = fabsf(sol_val[erff_idx] - ret_val[erff_idx]);

		if ((isnan(sol_val[erff_idx]) ^ (isnan(ret_val[erff_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("erff", compute_val, FLT_EPSILON);
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
	const long double in_val[] = { 1, -1, ZERO, 0.5, -0.5, 0, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { 0.842700689747, -0.842700689747, ZERO, 0.520500016304, -0.520500016304, 0, 1, -1, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int erfl_idx;
	long double compute_val;

	/* Returns the error function value for x */

	for (erfl_idx = 0; erfl_idx < SIZE(sol_val, long double); erfl_idx++) {
		ret_val[erfl_idx] = erfl(in_val[erfl_idx]);
		compute_val = fabsl(sol_val[erfl_idx] - ret_val[erfl_idx]);

		if ((isnan(sol_val[erfl_idx]) ^ (isnan(ret_val[erfl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("erfl", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_exp
 * @brief                :Returns the base-e exponential value
 * @Scenario             :Returns the base-e exponential value
 * API's covered         :exp
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_exp(void)
{
	const double in_val[] = { ZERO, INFINITY, -INFINITY, NAN, VAL1, -VAL1, VAL2, -VAL2 };
	const double sol_val[] = { 1.0, INFINITY, ZERO, NAN, INFINITY, ZERO, INFINITY, ZERO };
	double ret_val[SIZE(sol_val, double)];
	int exp_idx;
	double compute_val;

	for (exp_idx = 0; exp_idx < SIZE(sol_val, double); exp_idx++) {
		ret_val[exp_idx] = exp(in_val[exp_idx]);
		compute_val = fabs(sol_val[exp_idx] - ret_val[exp_idx]);

		if ((isnan(sol_val[exp_idx]) ^ (isnan(ret_val[exp_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("exp", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_expf
 * @brief                :Returns the base-e exponential value
 * @Scenario             :Returns the base-e exponential value
 * API's covered         :expf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_expf(void)
{
	const float in_val[] = { ZERO, INFINITY, -INFINITY, NAN, VAL1, -VAL1, VAL2, -VAL2 };
	const float sol_val[] = { 1.0, INFINITY, ZERO, NAN, INFINITY, ZERO, INFINITY, ZERO };
	float ret_val[SIZE(sol_val, float)];
	int expf_idx;
	float compute_val;

	for (expf_idx = 0; expf_idx < SIZE(sol_val, float); expf_idx++) {
		ret_val[expf_idx] = expf(in_val[expf_idx]);
		compute_val = fabsf(sol_val[expf_idx] - ret_val[expf_idx]);

		if ((isnan(sol_val[expf_idx]) ^ (isnan(ret_val[expf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("expf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_expl
 * @brief                :Returns the base-e exponential value
 * @Scenario             :Returns the base-e exponential value
 * API's covered         :expl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_expl(void)
{
	const long double in_val[] = { ZERO, INFINITY, -INFINITY, NAN, VAL1, -VAL1, VAL2, -VAL2 };
	const long double sol_val[] = { 1.0, INFINITY, ZERO, NAN, INFINITY, ZERO, INFINITY, ZERO };
	long double ret_val[SIZE(sol_val, long double)];
	int expl_idx;
	long double compute_val;

	for (expl_idx = 0; expl_idx < SIZE(sol_val, long double); expl_idx++) {
		ret_val[expl_idx] = expl(in_val[expl_idx]);
		compute_val = fabsl(sol_val[expl_idx] - ret_val[expl_idx]);

		if ((isnan(sol_val[expl_idx]) ^ (isnan(ret_val[expl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("expl", compute_val, FLT_EPSILON);
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
	const double in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p-20 };
	const double sol_val[] = { 1.0, INFINITY, ZERO, INFINITY, ZERO, INFINITY, ZERO, NAN, 1.0000007 };
	double ret_val[SIZE(sol_val, double)];
	int exp2_idx;
	double compute_val;

	for (exp2_idx = 0; exp2_idx < SIZE(sol_val, double); exp2_idx++) {
		ret_val[exp2_idx] = exp2(in_val[exp2_idx]);
		compute_val = fabs(sol_val[exp2_idx] - ret_val[exp2_idx]);

		if ((isnan(sol_val[exp2_idx]) ^ (isnan(ret_val[exp2_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("exp2", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_exp2f
 * @brief                :Returns the base-2 exponential value
 * @Scenario             :Returns the base-2 exponential value
 * API's covered         :exp2f
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_exp2f(void)
{
	const float in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p-20 };
	const float sol_val[] = { 1.0, INFINITY, ZERO, INFINITY, ZERO, INFINITY, ZERO, NAN, 1.0000007 };
	float ret_val[SIZE(sol_val, float)];
	int exp2f_idx;
	float compute_val;

	for (exp2f_idx = 0; exp2f_idx < SIZE(sol_val, float); exp2f_idx++) {
		ret_val[exp2f_idx] = exp2f(in_val[exp2f_idx]);
		compute_val = fabsf(sol_val[exp2f_idx] - ret_val[exp2f_idx]);

		if ((isnan(sol_val[exp2f_idx]) ^ (isnan(ret_val[exp2f_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("exp2f", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_exp2l
 * @brief                :Returns the base-2 exponential value
 * @Scenario             :Returns the base-2 exponential value
 * API's covered         :exp2l
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_exp2l(void)
{
	const long double in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p-20 };
	const long double sol_val[] = { 1.0, INFINITY, ZERO, INFINITY, ZERO, INFINITY, ZERO, NAN, 1.0000007 };
	long double ret_val[SIZE(sol_val, long double)];
	int exp2l_idx;
	long double compute_val;

	for (exp2l_idx = 0; exp2l_idx < SIZE(sol_val, long double); exp2l_idx++) {
		ret_val[exp2l_idx] = exp2l(in_val[exp2l_idx]);
		compute_val = fabsl(sol_val[exp2l_idx] - ret_val[exp2l_idx]);

		if ((isnan(sol_val[exp2l_idx]) ^ (isnan(ret_val[exp2l_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("exp2l", compute_val, FLT_EPSILON);
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
	const double in_val[] = { ZERO, -ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { ZERO, ZERO, 2251799813685248.5, 2251799813685248.5, INFINITY, INFINITY, NAN };
	double ret_val[SIZE(sol_val, double)];
	int fabs_idx;
	double compute_val;

	for (fabs_idx = 0; fabs_idx < SIZE(sol_val, double); fabs_idx++) {
		ret_val[fabs_idx] = fabs(in_val[fabs_idx]);
		compute_val = fabs(sol_val[fabs_idx] - ret_val[fabs_idx]);

		if ((isnan(sol_val[fabs_idx]) ^ (isnan(ret_val[fabs_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("fabs", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_fabsf
 * @brief                :Returns the absolute value of parameter
 * @Scenario             :Returns the absolute value of parameter
 * API's covered         :fabsf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_fabsf(void)
{
	const float in_val[] = { ZERO, -ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { ZERO, ZERO, 2251799813685248.5, 2251799813685248.5, INFINITY, INFINITY, NAN };
	float ret_val[SIZE(sol_val, float)];
	int fabsf_idx;
	float compute_val;

	for (fabsf_idx = 0; fabsf_idx < SIZE(sol_val, float); fabsf_idx++) {
		ret_val[fabsf_idx] = fabsf(in_val[fabsf_idx]);
		compute_val = fabsf(sol_val[fabsf_idx] - ret_val[fabsf_idx]);

		if ((isnan(sol_val[fabsf_idx]) ^ (isnan(ret_val[fabsf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("fabsf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_fabsl
 * @brief                :Returns the absolute value of parameter
 * @Scenario             :Returns the absolute value of parameter
 * API's covered         :fabsl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_fabsl(void)
{
	const long double in_val[] = { ZERO, -ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { ZERO, ZERO, 2251799813685248.5, 2251799813685248.5, INFINITY, INFINITY, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int fabsl_idx;
	long double compute_val;

	for (fabsl_idx = 0; fabsl_idx < SIZE(sol_val, long double); fabsl_idx++) {
		ret_val[fabsl_idx] = fabsl(in_val[fabsl_idx]);
		compute_val = fabsl(sol_val[fabsl_idx] - ret_val[fabsl_idx]);

		if ((isnan(sol_val[fabsl_idx]) ^ (isnan(ret_val[fabsl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("fabsl", compute_val, FLT_EPSILON);
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
	const double in_val1[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, VAL1 };
	const double in_val2[] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, NAN, NAN };
	const double sol_val[] = { INFINITY, ZERO, 6755399441055744.0, ZERO, 6755399441055744.0, ZERO, INFINITY, ZERO, NAN, NAN };
	double ret_val[SIZE(sol_val, double)];
	int fdim_idx;
	double compute_val;

	for (fdim_idx = 0; fdim_idx < SIZE(sol_val, double); fdim_idx++) {
		ret_val[fdim_idx] = fdim(in_val1[fdim_idx], in_val2[fdim_idx]);
		compute_val = fabs(sol_val[fdim_idx] - ret_val[fdim_idx]);

		if ((isnan(sol_val[fdim_idx]) ^ (isnan(ret_val[fdim_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("fdim", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_fdimf
 * @brief                :Returns the positive  difference max(x-y,0),  between thier arguments
 * @Scenario             :Returns the positive  difference max(x-y,0),  between their arguments
 * API's covered         :fdimf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_fdimf(void)
{
	const float in_val1[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, VAL1 };
	const float in_val2[] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, NAN, NAN };
	const float sol_val[] = { INFINITY, ZERO, 6755399441055744.0, ZERO, 6755399441055744.0, ZERO, INFINITY, ZERO, NAN, NAN };
	float ret_val[SIZE(sol_val, float)];
	int fdimf_idx;
	float compute_val;

	for (fdimf_idx = 0; fdimf_idx < SIZE(sol_val, float); fdimf_idx++) {
		ret_val[fdimf_idx] = fdimf(in_val1[fdimf_idx], in_val2[fdimf_idx]);
		compute_val = fabsf(sol_val[fdimf_idx] - ret_val[fdimf_idx]);

		if ((isnan(sol_val[fdimf_idx]) ^ (isnan(ret_val[fdimf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("fdimf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_fdiml
 * @brief                :Returns the positive  difference max(x-y,0),  between thier arguments
 * @Scenario             :Returns the positive  difference max(x-y,0),  between their arguments
 * API's covered         :fdiml
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_fdiml(void)
{
	const long double in_val1[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, VAL1 };
	const long double in_val2[] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, NAN, NAN };
	const long double sol_val[] = { INFINITY, ZERO, 6755399441055744.0, ZERO, 6755399441055744.0, ZERO, INFINITY, ZERO, NAN, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int fdiml_idx;
	long double compute_val;

	for (fdiml_idx = 0; fdiml_idx < SIZE(sol_val, long double); fdiml_idx++) {
		ret_val[fdiml_idx] = fdiml(in_val1[fdiml_idx], in_val2[fdiml_idx]);
		compute_val = fabsl(sol_val[fdiml_idx] - ret_val[fdiml_idx]);

		if ((isnan(sol_val[fdiml_idx]) ^ (isnan(ret_val[fdiml_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("fdiml", compute_val, FLT_EPSILON);
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
	const double in_val[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { ZERO, -ZERO, 2251799813685248.0, -2251799813685249.0, 4503599627370496.0, -4503599627370497.0, INFINITY, -INFINITY, NAN };
	double ret_val[SIZE(sol_val, double)] = { ZERO };
	int floor_idx;
	double compute_val;

	for (floor_idx = 0; floor_idx < SIZE(sol_val, double); floor_idx++) {
		ret_val[floor_idx] = floor(in_val[floor_idx]);
		compute_val = fabs(sol_val[floor_idx] - ret_val[floor_idx]);

		if ((isnan(sol_val[floor_idx]) ^ (isnan(ret_val[floor_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("floor", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_floorf
 * @brief                :Returns the floor value of parameter
 * @Scenario             :Returns the floor value of parameter
 * API's covered         :floorf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_floorf(void)
{
	const float in_val[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { ZERO, -ZERO, 2251799813685248.0, -2251799813685249.0, 4503599627370496.0, -4503599627370497.0, INFINITY, -INFINITY, NAN };
	float ret_val[SIZE(sol_val, float)] = { ZERO };
	int floorf_idx;
	float compute_val;

	for (floorf_idx = 0; floorf_idx < SIZE(sol_val, float); floorf_idx++) {
		ret_val[floorf_idx] = floorf(in_val[floorf_idx]);
		compute_val = fabsf(sol_val[floorf_idx] - ret_val[floorf_idx]);

		if ((isnan(sol_val[floorf_idx]) ^ (isnan(ret_val[floorf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("floorf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_floorl
 * @brief                :Returns the floor value of parameter
 * @Scenario             :Returns the floor value of parameter
 * API's covered         :floorl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_floorl(void)
{
	const long double in_val[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { ZERO, -ZERO, 2251799813685248.0, -2251799813685249.0, 4503599627370496.0, -4503599627370497.0, INFINITY, -INFINITY, NAN };
	long double ret_val[SIZE(sol_val, long double)] = { ZERO };
	int floorl_idx;
	long double compute_val;

	for (floorl_idx = 0; floorl_idx < SIZE(sol_val, long double); floorl_idx++) {
		ret_val[floorl_idx] = floorl(in_val[floorl_idx]);
		compute_val = fabsl(sol_val[floorl_idx] - ret_val[floorl_idx]);

		if ((isnan(sol_val[floorl_idx]) ^ (isnan(ret_val[floorl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("floorl", compute_val, FLT_EPSILON);
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
	const double in_val1[] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN, VAL1 };
	const double in_val2[] = { -INFINITY, INFINITY, -VAL1, VAL1, ZERO, NAN, NAN };
	const double sol_val[] = { ZERO, INFINITY, -VAL1, INFINITY, ZERO, NAN, VAL1 };
	double ret_val[SIZE(sol_val, double)];
	int fmax_idx;
	double compute_val;

	for (fmax_idx = 0; fmax_idx < SIZE(sol_val, double); fmax_idx++) {
		ret_val[fmax_idx] = fmax(in_val1[fmax_idx], in_val2[fmax_idx]);
		compute_val = fabs(sol_val[fmax_idx] - ret_val[fmax_idx]);

		if ((isnan(sol_val[fmax_idx]) ^ (isnan(ret_val[fmax_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("fmax", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_fmaxf
 * @brief                :Returns the maximum value of argument1 and argument2
 * @Scenario             :Returns the maximum value of argument1 and argument2
 * API's covered         :fmaxf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_fmaxf(void)
{
	const float in_val1[] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN, VAL1 };
	const float in_val2[] = { -INFINITY, INFINITY, -VAL1, VAL1, ZERO, NAN, NAN };
	const float sol_val[] = { ZERO, INFINITY, -VAL1, INFINITY, ZERO, NAN, VAL1 };
	float ret_val[SIZE(sol_val, float)];
	int fmaxf_idx;
	float compute_val;

	for (fmaxf_idx = 0; fmaxf_idx < SIZE(sol_val, float); fmaxf_idx++) {
		ret_val[fmaxf_idx] = fmaxf(in_val1[fmaxf_idx], in_val2[fmaxf_idx]);
		compute_val = fabsf(sol_val[fmaxf_idx] - ret_val[fmaxf_idx]);

		if ((isnan(sol_val[fmaxf_idx]) ^ (isnan(ret_val[fmaxf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("fmaxf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_fmaxl
 * @brief                :Returns the maximum value of argument1 and argument2
 * @Scenario             :Returns the maximum value of argument1 and argument2
 * API's covered         :fmaxl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_fmaxl(void)
{
	const long double in_val1[] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN, VAL1 };
	const long double in_val2[] = { -INFINITY, INFINITY, -VAL1, VAL1, ZERO, NAN, NAN };
	const long double sol_val[] = { ZERO, INFINITY, -VAL1, INFINITY, ZERO, NAN, VAL1 };
	long double ret_val[SIZE(sol_val, double)];
	int fmaxl_idx;
	long double compute_val;

	for (fmaxl_idx = 0; fmaxl_idx < SIZE(sol_val, long double); fmaxl_idx++) {
		ret_val[fmaxl_idx] = fmaxl(in_val1[fmaxl_idx], in_val2[fmaxl_idx]);
		compute_val = fabsl(sol_val[fmaxl_idx] - ret_val[fmaxl_idx]);

		if ((isnan(sol_val[fmaxl_idx]) ^ (isnan(ret_val[fmaxl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("fmaxl", compute_val, FLT_EPSILON);
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
	const double in_val1[] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN, VAL1 };
	const double in_val2[] = { -INFINITY, INFINITY, -VAL1, VAL1, ZERO, NAN, NAN };
	const double sol_val[] = { -INFINITY, VAL1, -VAL1, VAL1, -INFINITY, NAN, VAL1 };
	double ret_val[SIZE(sol_val, double)];
	int fmin_idx;
	double compute_val;

	for (fmin_idx = 0; fmin_idx < SIZE(sol_val, double); fmin_idx++) {
		ret_val[fmin_idx] = fmin(in_val1[fmin_idx], in_val2[fmin_idx]);
		compute_val = fabs(sol_val[fmin_idx] - ret_val[fmin_idx]);

		if ((isnan(sol_val[fmin_idx]) ^ (isnan(ret_val[fmin_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("fmin", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_fminf
 * @brief                :Returns the minimum value of argument1 and argument2
 * @Scenario             :Returns the minimum value of argument1 and argument2
 * API's covered         :fminf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_fminf(void)
{
	const float in_val1[] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN, VAL1 };
	const float in_val2[] = { -INFINITY, INFINITY, -VAL1, VAL1, ZERO, NAN, NAN };
	const float sol_val[] = { -INFINITY, VAL1, -VAL1, VAL1, -INFINITY, NAN, VAL1 };
	float ret_val[SIZE(sol_val, float)];
	int fminf_idx;
	float compute_val;

	for (fminf_idx = 0; fminf_idx < SIZE(sol_val, float); fminf_idx++) {
		ret_val[fminf_idx] = fminf(in_val1[fminf_idx], in_val2[fminf_idx]);
		compute_val = fabsf(sol_val[fminf_idx] - ret_val[fminf_idx]);

		if ((isnan(sol_val[fminf_idx]) ^ (isnan(ret_val[fminf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("fminf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_fminl
 * @brief                :Returns the minimum value of argument1 and argument2
 * @Scenario             :Returns the minimum value of argument1 and argument2
 * API's covered         :fminl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_fminl(void)
{
	const long double in_val1[] = { ZERO, VAL1, -VAL1, INFINITY, -INFINITY, NAN, VAL1 };
	const long double in_val2[] = { -INFINITY, INFINITY, -VAL1, VAL1, ZERO, NAN, NAN };
	const long double sol_val[] = { -INFINITY, VAL1, -VAL1, VAL1, -INFINITY, NAN, VAL1 };
	long double ret_val[SIZE(sol_val, long double)];
	int fminl_idx;
	long double compute_val;

	for (fminl_idx = 0; fminl_idx < SIZE(sol_val, long double); fminl_idx++) {
		ret_val[fminl_idx] = fminl(in_val1[fminl_idx], in_val2[fminl_idx]);
		compute_val = fabsl(sol_val[fminl_idx] - ret_val[fminl_idx]);

		if ((isnan(sol_val[fminl_idx]) ^ (isnan(ret_val[fminl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("fminl", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_frexp
 * @brief                :Breaks floating point x into its binary significand (between 0.5(included) and 1.0(excluded)) and an integral exponent for 2
 * @Scenario             :If x is 0, both parts (significand and exponent) are 0. If x is negative, significand returned by this function is negative
 * API's covered         :frexp
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_frexp(void)
{
	const double in_val[] = { 8.0, 2.3, -10.0, 0.0, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 0.5, 0.575, -10, 0, INFINITY, -INFINITY, NAN };
	const int n_val[] = { 4, 2, 0, 0, 0, 0, 0 };
	double ret_val[SIZE(sol_val, double)];
	int n[SIZE(sol_val, double)];
	int frexp_idx;
	double compute_val;
	double ref_val = FLT_EPSILON;

	/* Breaks the floating point number x into its binary significand and an integral exponent for 2 */

	for (frexp_idx = 0; frexp_idx < SIZE(sol_val, double); frexp_idx++) {
		ret_val[frexp_idx] = frexp(in_val[frexp_idx], &n[frexp_idx]);
		compute_val = fabs(sol_val[frexp_idx] - ret_val[frexp_idx]);

		if ((isnan(sol_val[frexp_idx]) ^ (isnan(ret_val[frexp_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("frexp", compute_val, ref_val);

		compute_val = fabs(n[frexp_idx]);
		ref_val = fabs(n[frexp_idx]);

		if ((isnan(n[frexp_idx]) ^ (isnan(n_val[frexp_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_EQ("frexp", compute_val, ref_val);
	}


	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_frexpf
 * @brief                :Breaks the floating point number x into its binary significand (between 0.5(included) and 1.0(excluded)) and an integral exponent for 2
 * @Scenario             :If x is zero, both parts (significand and exponent) are zero. If x is negative, the significand returned by this function is negative.
 * API's covered         :frexpf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_frexpf(void)
{
	const float in_val[] = { 8.0, 2.3, -10.0, 0.0, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { 0.99999976158142, 0.574999988079071, -10, 0, INFINITY, -INFINITY, NAN };
	const int n_val[] = { 3, 2, 0, 0, 0, 0, 0 };
	float ret_val[SIZE(sol_val, float)];
	int n[SIZE(sol_val, float)];
	int frexpf_idx;
	float compute_val;
	float ref_val = FLT_EPSILON;

	/* Breaks the floating point number x into its binary significand and an integral exponent for 2 */

	for (frexpf_idx = 0; frexpf_idx < SIZE(sol_val, float); frexpf_idx++) {
		ret_val[frexpf_idx] = frexpf(in_val[frexpf_idx], &n[frexpf_idx]);
		compute_val = fabsf(sol_val[frexpf_idx] - ret_val[frexpf_idx]);

		if ((isnan(sol_val[frexpf_idx]) ^ (isnan(ret_val[frexpf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("frexpf", compute_val, ref_val);

		compute_val = fabsf(n[frexpf_idx]);
		ref_val = fabsf(n[frexpf_idx]);

		if ((isnan(n[frexpf_idx]) ^ (isnan(n_val[frexpf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_EQ("frexpf", compute_val, ref_val);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_frexpl
 * @brief                :Breaks the floating point number x into its binary significand (between 0.5(included) and 1.0(excluded)) and an integral exponent for 2
 * @Scenario             :If x is zero, both parts (significand and exponent) are zero. If x is negative, the significand returned by this function is negative.
 * API's covered         :frexpl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_frexpl(void)
{
	const long double in_val[] = { 8.0, 2.3, -10.0, 0.0, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { 0.5, 0.575, -10, 0, INFINITY, -INFINITY, NAN };
	const int n_val[] = { 4, 2, 0, 0, 0, 0, 0 };
	long double ret_val[SIZE(sol_val, long double)];
	int n[SIZE(sol_val, long double)];
	int frexpl_idx;
	long double compute_val;
	long double ref_val = FLT_EPSILON;

	/* Breaks the floating point number x into its binary significand and an integral exponent for 2 */

	for (frexpl_idx = 0; frexpl_idx < SIZE(sol_val, long double); frexpl_idx++) {
		ret_val[frexpl_idx] = frexpl(in_val[frexpl_idx], &n[frexpl_idx]);
		compute_val = fabsl(sol_val[frexpl_idx] - ret_val[frexpl_idx]);

		if ((isnan(sol_val[frexpl_idx]) ^ (isnan(ret_val[frexpl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("frexpl", compute_val, ref_val);

		compute_val = fabsl(n[frexpl_idx]);
		ref_val = fabsl(n[frexpl_idx]);

		if ((isnan(n[frexpl_idx]) ^ (isnan(n_val[frexpl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_EQ("frexpl", compute_val, ref_val);
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
	const double in_val1[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, VAL1 };
	const double in_val2[] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, INFINITY, NAN };
	const double sol_val[] = { INFINITY, INFINITY, 5035177455121576.0, 5035177455121576.0, 5035177455121576.0, 5035177455121576.0, INFINITY, INFINITY, INFINITY, NAN };
	double ret_val[SIZE(sol_val, double)];
	int hypot_idx;
	double compute_val;

	for (hypot_idx = 0; hypot_idx < SIZE(sol_val, double); hypot_idx++) {
		ret_val[hypot_idx] = hypot(in_val1[hypot_idx], in_val2[hypot_idx]);
		compute_val = fabs(sol_val[hypot_idx] - ret_val[hypot_idx]);

		if ((isnan(sol_val[hypot_idx]) ^ (isnan(ret_val[hypot_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("hypot", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_hypotf
 * @brief                :Returns the length of a right-angled  triangle
 *						  with sides of length parameter1 and parameter2
 * @Scenario             :Returns the length of a right-angled  triangle
 *                        with sides of length parameter1 and parameter2
 * API's covered         :hypotf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_hypotf(void)
{
	const float in_val1[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, VAL1, 0x5fe };
	const float in_val2[] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, INFINITY, NAN, 0x1p-700 };
	const float sol_val[] = { INFINITY, INFINITY, 5035177455121576.0, 5035177455121576.0, 5035177455121576.0, 5035177455121576.0, INFINITY, INFINITY, INFINITY, NAN, 0x5fe };
	float ret_val[SIZE(sol_val, float)];
	int hypotf_idx;
	float compute_val;

	for (hypotf_idx = 0; hypotf_idx < SIZE(sol_val, float); hypotf_idx++) {
		ret_val[hypotf_idx] = hypotf(in_val1[hypotf_idx], in_val2[hypotf_idx]);
		compute_val = fabsf(sol_val[hypotf_idx] - ret_val[hypotf_idx]);

		if ((isnan(sol_val[hypotf_idx]) ^ (isnan(ret_val[hypotf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("hypotf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_hypotl
 * @brief                :Returns the length of a right-angled  triangle
 *						  with sides of length parameter1 and parameter2
 * @Scenario             :Returns the length of a right-angled  triangle
 *                        with sides of length parameter1 and parameter2
 * API's covered         :hypotl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_hypotl(void)
{
	const long double in_val1[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, VAL1, 0x5fe };
	const long double in_val2[] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, INFINITY, NAN, 0x1p-700 };
	const long double sol_val[] = { INFINITY, INFINITY, 5035177455121576.0, 5035177455121576.0, 5035177455121576.0, 5035177455121576.0, INFINITY, INFINITY, INFINITY, NAN, 0x5fe };
	long double ret_val[SIZE(sol_val, long double)];
	int hypotl_idx;
	long double compute_val;

	for (hypotl_idx = 0; hypotl_idx < SIZE(sol_val, long double); hypotl_idx++) {
		ret_val[hypotl_idx] = hypotl(in_val1[hypotl_idx], in_val2[hypotl_idx]);
		compute_val = fabsl(sol_val[hypotl_idx] - ret_val[hypotl_idx]);

		if ((isnan(sol_val[hypotl_idx]) ^ (isnan(ret_val[hypotl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("hypotl", compute_val, FLT_EPSILON);
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
	const double in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p-13, 0x1p-127 };
	const double sol_val[] = { 1.0, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, NAN, 0.99999999627, 1.0 };
	double ret_val[SIZE(sol_val, double)] = { ZERO };
	int j0_idx;
	double compute_val;

	for (j0_idx = 0; j0_idx < SIZE(sol_val, double); j0_idx++) {
		ret_val[j0_idx] = j0(in_val[j0_idx]);
		compute_val = fabs(sol_val[j0_idx] - ret_val[j0_idx]);

		if ((isnan(sol_val[j0_idx]) ^ (isnan(ret_val[j0_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("j0", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_j0f
 * @brief                :Returns the bessel value of 1st kind of order 0
 * @Scenario             :Returns the bessel value of 1st kind of order 0
 * API's covered         :j0f
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_j0f(void)
{
	const float in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p-11, 2.0, 3.0, 8.0, 7.8 };
	const float sol_val[] = { 1.0, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, NAN, 0.999999940395, 0.223890766501427, -0.260051935911179, 0.171650826931000, 0.215407878160477 };
	float ret_val[SIZE(sol_val, float)] = { ZERO };
	int j0f_idx;
	float compute_val;

	for (j0f_idx = 0; j0f_idx < SIZE(sol_val, float); j0f_idx++) {
		ret_val[j0f_idx] = j0f(in_val[j0f_idx]);
		compute_val = fabsf(sol_val[j0f_idx] - ret_val[j0f_idx]);

		if ((isnan(sol_val[j0f_idx]) ^ (isnan(ret_val[j0f_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("j0f", compute_val, FLT_EPSILON);
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
	const double in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p-127 };
	const double sol_val[] = { ZERO, ZERO, -ZERO, ZERO, -ZERO, ZERO, -ZERO, NAN, ZERO };
	double ret_val[SIZE(sol_val, double)] = { ZERO };
	int j1_idx;
	double compute_val;

	for (j1_idx = 0; j1_idx < SIZE(sol_val, double); j1_idx++) {
		ret_val[j1_idx] = j1(in_val[j1_idx]);
		compute_val = fabs(sol_val[j1_idx] - ret_val[j1_idx]);

		if ((isnan(sol_val[j1_idx]) ^ (isnan(ret_val[j1_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("j1", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_j1f
 * @brief                :Returns the bessel value of 1st kind of order 1
 * @Scenario             :Returns the bessel value of 1st kind of order 1
 * API's covered         :j1f
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_j1f(void)
{
	const float in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p-27, -2.0, -3.0, -7.8, -8.0 };
	const float sol_val[] = { ZERO, ZERO, -ZERO, ZERO, -ZERO, ZERO, -ZERO, NAN, ZERO, -0.576724827289581, -0.339058995246887, -0.201356813311577, -0.234636336565018 };
	float ret_val[SIZE(sol_val, float)] = { ZERO };
	int j1f_idx;
	float compute_val;

	for (j1f_idx = 0; j1f_idx < SIZE(sol_val, float); j1f_idx++) {
		ret_val[j1f_idx] = j1f(in_val[j1f_idx]);
		compute_val = fabsf(sol_val[j1f_idx] - ret_val[j1f_idx]);

		if ((isnan(sol_val[j1f_idx]) ^ (isnan(ret_val[j1f_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("j1f", compute_val, FLT_EPSILON);
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
	const double in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p302, 0x1p-29, 0x1p-30, 0x1p-40 };
	const double sol_val[][7] = { {ZERO, 1.0, ZERO, ZERO, ZERO, ZERO, ZERO}, { -ZERO, ZERO, ZERO, -ZERO, -ZERO, ZERO, ZERO}, {ZERO, ZERO, -ZERO, -ZERO, ZERO, ZERO, -ZERO}, { -ZERO, ZERO, ZERO, -ZERO, -ZERO, ZERO, ZERO}, {ZERO, ZERO, -ZERO, -ZERO, ZERO, ZERO, -ZERO}, {ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO}, {ZERO, ZERO, -ZERO, ZERO, -ZERO, ZERO, -ZERO}, {NAN, NAN, NAN, NAN, NAN, NAN, NAN}, {ZERO, -ZERO, -ZERO, ZERO, ZERO, -ZERO, -ZERO}, { -ZERO, 1.0, ZERO, ZERO, ZERO, ZERO, ZERO}, { -ZERO, 1.0, ZERO, ZERO, ZERO, ZERO, ZERO}, { -ZERO, 1.0, ZERO, ZERO, ZERO, ZERO, ZERO} };
	double ret_val[SIZE(sol_val, double)][7];
	int jn_idx;
	int order_idx;

	for (jn_idx = 0; jn_idx < SIZE(in_val, double); jn_idx++) {
		for (order_idx = 0; order_idx < 7; order_idx++) {
			ret_val[jn_idx][order_idx] = jn(order_idx - 1, in_val[jn_idx]);
			if (!(isnan(sol_val[jn_idx][order_idx]) && isnan(ret_val[jn_idx][order_idx]))) {
				TC_ASSERT_LEQ("jn", fabs(sol_val[jn_idx][order_idx] - ret_val[jn_idx][order_idx]), FLT_EPSILON);
			}
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_jnf
 * @brief                :Returns the bessel value of 1st kind of order n
 * @Scenario             :Returns the bessel value of 1st kind of order n
 * API's covered         :jnf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_jnf(void)
{
	const float in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p-10, 0x1p-20, 0x2p-30 };
	int order[] = { -1, 0, 1, 2, 9 };
	const float sol_val[][SIZE(order, int)] = { { -ZERO, 1.0, ZERO, ZERO, ZERO}, { -ZERO, ZERO, ZERO, -ZERO, ZERO}, {ZERO, ZERO, -ZERO, -ZERO, -ZERO}, { -ZERO, ZERO, ZERO, -ZERO, ZERO}, {ZERO, ZERO, -ZERO, -ZERO, -ZERO}, { -ZERO, ZERO, ZERO, ZERO, ZERO}, {ZERO, ZERO, -ZERO, ZERO, -ZERO}, {NAN, NAN, NAN, NAN, NAN}, { -0.0004883, 0.9999998, 0.0004883, 0.0000001, ZERO}, { -0.0000005, 1.0, 0.0000005, ZERO, ZERO}, { -ZERO, 1.0, ZERO, ZERO, ZERO} };
	float ret_val[SIZE(sol_val, float)][SIZE(order, int)];
	int jnf_idx;
	int order_idx;

	for (jnf_idx = 0; jnf_idx < SIZE(in_val, float); jnf_idx++) {
		for (order_idx = 0; order_idx < 5; order_idx++) {
			ret_val[jnf_idx][order_idx] = jnf(order[order_idx], in_val[jnf_idx]);
			if (!(isnan(sol_val[jnf_idx][order_idx]) && isnan(ret_val[jnf_idx][order_idx]))) {
				TC_ASSERT_LEQ("jnf", fabsf(sol_val[jnf_idx][order_idx] - ret_val[jnf_idx][order_idx]), FLT_EPSILON);
			}
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
	const double in_val[] = { 0.95, 0.0, -10.0, 30.0, NAN, INFINITY, -INFINITY };
	const double sol_val[] = { 15.2, 0, -10240, 30.0, NAN, INFINITY, -INFINITY };
	double ret_val[SIZE(sol_val, double)];
	int n[] = { 4, -2, 10, 0, 2, 1, 3 };
	int ldexp_idx;
	double compute_val;

	/* Breaks the floating point number x into its binary significand and an integral exponent for 2 */

	for (ldexp_idx = 0; ldexp_idx < SIZE(sol_val, double); ldexp_idx++) {
		ret_val[ldexp_idx] = ldexp(in_val[ldexp_idx], n[ldexp_idx]);
		compute_val = fabs(sol_val[ldexp_idx] - ret_val[ldexp_idx]);

		if ((isnan(sol_val[ldexp_idx]) ^ (isnan(ret_val[ldexp_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("ldexp", compute_val, FLT_EPSILON);
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
	const float in_val[] = { 0.95, 0, -10, 30.0, NAN, INFINITY, -INFINITY };
	const float sol_val[] = { 15.200005531311, 0, -10240.003906250, 30.0, NAN, INFINITY, -INFINITY };
	float ret_val[SIZE(sol_val, float)];
	int n[] = { 4, -2, 10, 0, 2, 1, 3 };
	int ldexpf_idx;
	float compute_val;

	/* Breaks the floating point number x into its binary significand and an integral exponent for 2 */

	for (ldexpf_idx = 0; ldexpf_idx < SIZE(sol_val, float); ldexpf_idx++) {
		ret_val[ldexpf_idx] = ldexpf(in_val[ldexpf_idx], n[ldexpf_idx]);
		compute_val = fabsf(sol_val[ldexpf_idx] - ret_val[ldexpf_idx]);

		if ((isnan(sol_val[ldexpf_idx]) ^ (isnan(ret_val[ldexpf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("ldexpf", compute_val, FLT_EPSILON);
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
	const long double in_val[] = { 0.95, 0.0, -10.0, 30.0, NAN, INFINITY, -INFINITY };
	const long double sol_val[] = { 15.2, 0, -10240, 30.0, NAN, INFINITY, -INFINITY };
	long double ret_val[SIZE(sol_val, long double)];
	int n[] = { 4, -2, 10, 0, 2, 1, 3 };
	int ldexpl_idx;
	long double compute_val;

	/* Breaks the floating point number x into its binary significand and an integral exponent for 2 */

	for (ldexpl_idx = 0; ldexpl_idx < SIZE(sol_val, long double); ldexpl_idx++) {
		ret_val[ldexpl_idx] = ldexpl(in_val[ldexpl_idx], n[ldexpl_idx]);
		compute_val = fabsl(sol_val[ldexpl_idx] - ret_val[ldexpl_idx]);

		if ((isnan(sol_val[ldexpl_idx]) ^ (isnan(ret_val[ldexpl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("ldexpl", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_log
 * @brief                :Returns the natural (base-e) logarithm of x
 * @Scenario             :Returns the natural (base-e) logarithm of x
 * API's covered         :log
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_log(void)
{
	const double in_val[] = { 1024, 5.5, -10.0, 0.936584472656250, 0.0 };
	const double sol_val[] = { 6.931471805599453, 1.704748092238425, NAN, -0.065515560785398, -INFINITY };
	double ret_val[SIZE(sol_val, double)];
	int log_idx;
	double compute_val;

	/* Returns the natural (base-e) logarithm of x */

	for (log_idx = 0; log_idx < SIZE(sol_val, double); log_idx++) {
		ret_val[log_idx] = log(in_val[log_idx]);
		compute_val = fabs(sol_val[log_idx] - ret_val[log_idx]);

		if ((isnan(sol_val[log_idx]) ^ (isnan(ret_val[log_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("log", compute_val, FLT_EPSILON);
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
	const double in_val[] = { 1024, 5.5, -10.0, 0.95, 0, NAN, INFINITY, -INFINITY };
	const double sol_val[] = { 10.0, 2.4594316186373, NAN, -0.074000597000122, -INFINITY, NAN, INFINITY, NAN };
	double ret_val[SIZE(sol_val, double)];
	int log2_idx;
	double compute_val;

	/* Returns the binary (base-2) logarithm of x */

	for (log2_idx = 0; log2_idx < SIZE(sol_val, double); log2_idx++) {
		ret_val[log2_idx] = log2(in_val[log2_idx]);
		compute_val = fabs(sol_val[log2_idx] - ret_val[log2_idx]);

		if ((isnan(sol_val[log2_idx]) ^ (isnan(ret_val[log2_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("log2", compute_val, FLT_EPSILON);
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
	const float in_val[] = { 1024, 5.5, -10.0, 0.95, 0, NAN, INFINITY, -INFINITY };
	const float sol_val[] = { 10.0, 2.4594316186373, NAN, -0.074000597000122, -INFINITY, NAN, INFINITY, NAN };
	float ret_val[SIZE(sol_val, float)];
	int log2f_idx;
	float compute_val;

	/* Returns the binary (base-2) logarithm of x */

	for (log2f_idx = 0; log2f_idx < SIZE(sol_val, float); log2f_idx++) {
		ret_val[log2f_idx] = log2f(in_val[log2f_idx]);
		compute_val = fabsf(sol_val[log2f_idx] - ret_val[log2f_idx]);

		if ((isnan(sol_val[log2f_idx]) ^ (isnan(ret_val[log2f_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("log2f", compute_val, FLT_EPSILON);
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
	const long double in_val[] = { 1024, 5.5, -10.0, 0.95, 0, NAN, INFINITY, -INFINITY };
	const long double sol_val[] = { 10.0, 2.4594316186373, NAN, -0.074000597000122, -INFINITY, NAN, INFINITY, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int log2l_idx;
	long double compute_val;

	/* Returns the binary (base-2) logarithm of x */

	for (log2l_idx = 0; log2l_idx < SIZE(sol_val, long double); log2l_idx++) {
		ret_val[log2l_idx] = log2l(in_val[log2l_idx]);
		compute_val = fabsl(sol_val[log2l_idx] - ret_val[log2l_idx]);

		if ((isnan(sol_val[log2l_idx]) ^ (isnan(ret_val[log2l_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("log2l", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_log10
 * @brief                :Returns the binary (base-10) logarithm of x
 * @Scenario             :Returns the binary (base-10) logarithm of x
 * API's covered         :log10
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_log10(void)
{
	const double in_val[] = { 1.0, INFINITY, NAN, -1.0, 10.0 };
	const double sol_val[] = { ZERO, INFINITY, NAN, NAN, 1.0 };
	double ret_val[SIZE(sol_val, double)];
	int log10_idx;
	double compute_val;

	for (log10_idx = 0; log10_idx < SIZE(sol_val, double); log10_idx++) {
		ret_val[log10_idx] = log10(in_val[log10_idx]);
		compute_val = fabs(sol_val[log10_idx] - ret_val[log10_idx]);

		if ((isnan(sol_val[log10_idx]) ^ (isnan(ret_val[log10_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("log10", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_log10f
 * @brief                :Returns the binary (base-10) logarithm of x
 * @Scenario             :Returns the binary (base-10) logarithm of x
 * API's covered         :log10f
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_log10f(void)
{
	const float in_val[] = { 1.0, INFINITY, NAN, -1.0, 10.0 };
	const float sol_val[] = { ZERO, INFINITY, NAN, NAN, 1.0 };
	float ret_val[SIZE(sol_val, float)];
	int log10f_idx;
	float compute_val;

	for (log10f_idx = 0; log10f_idx < SIZE(sol_val, float); log10f_idx++) {
		ret_val[log10f_idx] = log10f(in_val[log10f_idx]);
		compute_val = fabsf(sol_val[log10f_idx] - ret_val[log10f_idx]);

		if ((isnan(sol_val[log10f_idx]) ^ (isnan(ret_val[log10f_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("log10f", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_log10l
 * @brief                :Returns the binary (base-10) logarithm of x
 * @Scenario             :Returns the binary (base-10) logarithm of x
 * API's covered         :log10l
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_log10l(void)
{
	const long double in_val[] = { 1.0, INFINITY, NAN, -1.0, 10.0 };
	const long double sol_val[] = { ZERO, INFINITY, NAN, NAN, 1.0 };
	long double ret_val[SIZE(sol_val, long double)];
	int log10l_idx;
	long double compute_val;

	for (log10l_idx = 0; log10l_idx < SIZE(sol_val, long double); log10l_idx++) {
		ret_val[log10l_idx] = log10l(in_val[log10l_idx]);
		compute_val = fabsl(sol_val[log10l_idx] - ret_val[log10l_idx]);

		if ((isnan(sol_val[log10l_idx]) ^ (isnan(ret_val[log10l_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("log10l", compute_val, FLT_EPSILON);
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
	const double in_val1[] = { VAL1, VAL1, ZERO, -VAL1, -VAL2, VAL1, NAN, ZERO };
	const double in_val2[] = { VAL1, VAL2, VAL1, -VAL2, ZERO, NAN, VAL2, ZERO };
	const double sol_val[] = { VAL1, 2251799813685249.0, ZERO, -2251799813685249.0, -4503599627370495.5, NAN, NAN, ZERO };
	double ret_val[SIZE(sol_val, double)];
	int nextafter_idx;
	double compute_val;

	for (nextafter_idx = 0; nextafter_idx < SIZE(sol_val, double); nextafter_idx++) {
		ret_val[nextafter_idx] = nextafter(in_val1[nextafter_idx], in_val2[nextafter_idx]);
		compute_val = fabs(sol_val[nextafter_idx] - ret_val[nextafter_idx]);

		if ((isnan(sol_val[nextafter_idx]) ^ (isnan(ret_val[nextafter_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("nextafter", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_nextafterf
 * @brief                :Returns the next representable floating-point
 *						  value following argument1 in the direction of argument2
 * @Scenario             :Returns the next representable floating-point
 *						  value following argument1 in the direction of argument2
 * API's covered         :nextafterf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_nextafterf(void)
{
	const float in_val1[] = { VAL1, VAL1, ZERO, -VAL1, -VAL2, VAL1, NAN, ZERO };
	const float in_val2[] = { VAL1, VAL2, VAL1, -VAL2, ZERO, NAN, VAL2, ZERO };
	const float sol_val[] = { 2251799813685248.0, 2251800082120704.0, ZERO, -2251800082120704.0, -4503599358935040.0, NAN, NAN, ZERO };
	float ret_val[SIZE(sol_val, float)];
	int nextafterf_idx;
	float compute_val;

	for (nextafterf_idx = 0; nextafterf_idx < SIZE(sol_val, float); nextafterf_idx++) {
		ret_val[nextafterf_idx] = nextafterf(in_val1[nextafterf_idx], in_val2[nextafterf_idx]);
		compute_val = fabsf(sol_val[nextafterf_idx] - ret_val[nextafterf_idx]);

		if ((isnan(sol_val[nextafterf_idx]) ^ (isnan(ret_val[nextafterf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("nextafterf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_nextafterl
 * @brief                :Returns the next representable floating-point
 *						  value following argument1 in the direction of argument2
 * @Scenario             :Returns the next representable floating-point
 *						  value following argument1 in the direction of argument2
 * API's covered         :nextafterl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_nextafterl(void)
{
	const long double in_val1[] = { VAL1, VAL1, ZERO, -VAL1, -VAL2, VAL1, NAN };
	const long double in_val2[] = { VAL1, VAL2, VAL1, -VAL2, ZERO, NAN, VAL2 };
	const long double sol_val[] = { VAL1, 2251799813685249.0, ZERO, -2251799813685249.0, -4503599627370495.5, NAN, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int nextafterl_idx;
	long double compute_val;

	for (nextafterl_idx = 0; nextafterl_idx < SIZE(sol_val, long double); nextafterl_idx++) {
		ret_val[nextafterl_idx] = nextafterl(in_val1[nextafterl_idx], in_val2[nextafterl_idx]);
		compute_val = fabsl(sol_val[nextafterl_idx] - ret_val[nextafterl_idx]);

		if ((isnan(sol_val[nextafterl_idx]) ^ (isnan(ret_val[nextafterl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("nextafterl", compute_val, FLT_EPSILON);
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
	const double in_val1[] = { VAL1, VAL1, ZERO, -VAL1, -VAL2, VAL1, NAN };
	const long double in_val2[] = { VAL1, VAL2, VAL1, -VAL2, ZERO, NAN, VAL2 };
	const double sol_val[] = { VAL1, 2251799813685249.0, ZERO, -2251799813685249.0, -4503599627370495.5, NAN, NAN };
	double ret_val[SIZE(sol_val, double)];
	int nexttoward_idx;
	double compute_val;

	for (nexttoward_idx = 0; nexttoward_idx < SIZE(sol_val, double); nexttoward_idx++) {
		ret_val[nexttoward_idx] = nexttoward(in_val1[nexttoward_idx], in_val2[nexttoward_idx]);
		compute_val = fabs(sol_val[nexttoward_idx] - ret_val[nexttoward_idx]);

		if ((isnan(sol_val[nexttoward_idx]) ^ (isnan(ret_val[nexttoward_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("nexttoward", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_nexttowardf
 * @brief                :Returns the next representable floating-point
 *						  value following argument1 in the direction of argument2
 * @Scenario             :Returns the next representable floating-point
 *						  value following argument1 in the direction of argument2
 * API's covered         :nexttowardf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_nexttowardf(void)
{
	const float in_val1[] = { VAL1, VAL1, ZERO, -VAL1, -VAL2, VAL1, NAN };
	const long double in_val2[] = { VAL1, VAL2, VAL1, -VAL2, ZERO, NAN, VAL2 };
	const float sol_val[] = { 2251800082120704.0, 2251800082120704.0, ZERO, -2251800082120704.0, -4503599358935040.0, NAN, NAN };
	float ret_val[SIZE(sol_val, float)];
	int nexttowardf_idx;
	float compute_val;

	for (nexttowardf_idx = 0; nexttowardf_idx < SIZE(sol_val, float); nexttowardf_idx++) {
		ret_val[nexttowardf_idx] = nexttowardf(in_val1[nexttowardf_idx], in_val2[nexttowardf_idx]);
		compute_val = fabsf(sol_val[nexttowardf_idx] - ret_val[nexttowardf_idx]);

		if ((isnan(sol_val[nexttowardf_idx]) ^ (isnan(ret_val[nexttowardf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("nexttowardf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_nexttowardl
 * @brief                :Returns the next representable floating-point
 *						  value following argument1 in the direction of argument2
 * @Scenario             :Returns the next representable floating-point
 *						  value following argument1 in the direction of argument2
 * API's covered         :nexttowardl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_nexttowardl(void)
{
	const long double in_val1[] = { VAL1, VAL1, ZERO, -VAL1, -VAL2, VAL1, NAN };
	const long double in_val2[] = { VAL1, VAL2, VAL1, -VAL2, ZERO, NAN, VAL2 };
	const long double sol_val[] = { VAL1, 2251799813685249.0, ZERO, -2251799813685249.0, -4503599627370495.5, NAN, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int nexttowardl_idx;
	long double compute_val;

	for (nexttowardl_idx = 0; nexttowardl_idx < SIZE(sol_val, long double); nexttowardl_idx++) {
		ret_val[nexttowardl_idx] = nexttowardl(in_val1[nexttowardl_idx], in_val2[nexttowardl_idx]);
		compute_val = fabsl(sol_val[nexttowardl_idx] - ret_val[nexttowardl_idx]);

		if ((isnan(sol_val[nexttowardl_idx]) ^ (isnan(ret_val[nexttowardl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("nexttowardl", compute_val, FLT_EPSILON);
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
	const double in_val[][2] = { {2, 2}, {2, 0.5}, {2, -2}, {2, -0.5}, {1, NAN}, {ZERO, 0.5}, {ZERO, -2}, { -ZERO, -1}, { -ZERO, -2}, { -2, -1}, { -2, -2}, {0.5, INFINITY}, {2, INFINITY}, { -0.936584472656250, 2} };
	const double sol_val[] = { 4.0, M_SQRT2, 0.25, M_SQRT1_2, 1, ZERO, INFINITY, -INFINITY, INFINITY, -0.5, 0.25, ZERO, INFINITY, 0.877190474420786 };
	double ret_val[SIZE(sol_val, double)];
	int pow_idx;

	for (pow_idx = 0; pow_idx < SIZE(sol_val, double); pow_idx++) {
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
	const double in_val1[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, INFINITY };
	const double in_val2[] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, NAN, VAL2 };
	const double sol_val[] = { ZERO, -ZERO, -2251799813685247.5, 2251799813685247.5, -1.0, 1.0, -NAN, -NAN, NAN, -NAN };
	double ret_val[SIZE(sol_val, double)];
	int remainder_idx;
	double compute_val;

	for (remainder_idx = 0; remainder_idx < SIZE(sol_val, double); remainder_idx++) {
		ret_val[remainder_idx] = remainder(in_val1[remainder_idx], in_val2[remainder_idx]);
		compute_val = fabs(sol_val[remainder_idx] - ret_val[remainder_idx]);

		if ((isnan(sol_val[remainder_idx]) ^ (isnan(ret_val[remainder_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("remainder", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_remainderf
 * @brief                :Returns the floating-point remainder of dividing argument1 by argument2
 * @Scenario             :Returns the floating-point remainder of dividing argument1 by argument2
 * API's covered         :remainderf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_remainderf(void)
{
	const float in_val1[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, INFINITY };
	const float in_val2[] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, NAN, VAL2 };
	const float sol_val[] = { ZERO, -ZERO, 2251799813685248.0, -2251799813685248.0, ZERO, -ZERO, -NAN, -NAN, NAN, -NAN };
	float ret_val[SIZE(sol_val, float)];
	int remainderf_idx;
	float compute_val;

	for (remainderf_idx = 0; remainderf_idx < SIZE(sol_val, float); remainderf_idx++) {
		ret_val[remainderf_idx] = remainderf(in_val1[remainderf_idx], in_val2[remainderf_idx]);
		compute_val = fabsf(sol_val[remainderf_idx] - ret_val[remainderf_idx]);

		if ((isnan(sol_val[remainderf_idx]) ^ (isnan(ret_val[remainderf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("remainderf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_remainderl
 * @brief                :Returns the floating-point remainder of dividing argument1 by argument2
 * @Scenario             :Returns the floating-point remainder of dividing argument1 by argument2
 * API's covered         :remainderl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_remainderl(void)
{
	const long double in_val1[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, INFINITY };
	const long double in_val2[] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, NAN, VAL2 };
	const long double sol_val[] = { ZERO, -ZERO, -2251799813685247.5, 2251799813685247.5, -1.0, 1.0, -NAN, -NAN, NAN, -NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int remainderl_idx;
	long double compute_val;

	for (remainderl_idx = 0; remainderl_idx < SIZE(sol_val, long double); remainderl_idx++) {
		ret_val[remainderl_idx] = remainderl(in_val1[remainderl_idx], in_val2[remainderl_idx]);
		compute_val = fabsl(sol_val[remainderl_idx] - ret_val[remainderl_idx]);

		if ((isnan(sol_val[remainderl_idx]) ^ (isnan(ret_val[remainderl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("remainderl", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_remquo
 * @brief                :Returns the floating-point remainder and stores part of quotient in argument/
 * @Scenario             :Returns the floating-point remainder and stores part of quotient in argument/
 * API's covered         :remquo
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_remquo(void)
{
	const double in_val1[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, INFINITY };
	const double in_val2[] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, NAN, VAL2 };
	const double sol_val[] = { ZERO, -ZERO, -2251799813685247.5, 2251799813685247.5, -1.0, 1.0, -NAN, -NAN, NAN, -NAN };
	const int sol_quo[] = { 0, 0, -1, -1, -2, -2, 0, 0, 0, 0 };
	double ret_val[SIZE(sol_val, double)];
	int ret_quo[SIZE(sol_quo, int)];
	int remquo_idx;
	double compute_val;

	for (remquo_idx = 0; remquo_idx < SIZE(sol_val, double); remquo_idx++) {
		ret_val[remquo_idx] = remquo(in_val1[remquo_idx], in_val2[remquo_idx], &ret_quo[remquo_idx]);
		compute_val = fabs(sol_val[remquo_idx] - ret_val[remquo_idx]);

		if ((isnan(sol_val[remquo_idx]) ^ (isnan(ret_val[remquo_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("remquo", compute_val, FLT_EPSILON);
		TC_ASSERT_EQ("remquo", sol_quo[remquo_idx], ret_quo[remquo_idx]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_remquof
 * @brief                :Returns the floating-point remainder and stores part of quotient in argument/
 * @Scenario             :Returns the floating-point remainder and stores part of quotient in argument/
 * API's covered         :remquof
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_remquof(void)
{
	const float in_val1[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, INFINITY };
	const float in_val2[] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, NAN, VAL2 };
	const float sol_val[] = { ZERO, -ZERO, 2251799813685248.0, -2251799813685248.0, ZERO, -ZERO, -NAN, -NAN, NAN, -NAN };
	const int sol_quo[] = { 0, 0, 0, 0, -2, -2, 0, 0, 0, 0 };
	float ret_val[SIZE(sol_val, float)];
	int ret_quo[SIZE(sol_quo, int)] = { 0 };
	int remquof_idx;
	float compute_val;

	for (remquof_idx = 0; remquof_idx < SIZE(sol_val, float); remquof_idx++) {
		ret_val[remquof_idx] = remquof(in_val1[remquof_idx], in_val2[remquof_idx], &ret_quo[remquof_idx]);
		compute_val = fabsf(sol_val[remquof_idx] - ret_val[remquof_idx]);

		if ((isnan(sol_val[remquof_idx]) ^ (isnan(ret_val[remquof_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("remquof", compute_val, FLT_EPSILON);
		TC_ASSERT_EQ("remquof", sol_quo[remquof_idx], ret_quo[remquof_idx]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_remquol
 * @brief                :Returns the floating-point remainder and stores part of quotient in argument/
 * @Scenario             :Returns the floating-point remainder and stores part of quotient in argument/
 * API's covered         :remquol
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_remquol(void)
{
	const long double in_val1[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, INFINITY };
	const long double in_val2[] = { -INFINITY, INFINITY, -VAL2, VAL2, -VAL1, VAL1, -ZERO, ZERO, NAN, VAL2 };
	const long double sol_val[] = { ZERO, -ZERO, -2251799813685247.5, 2251799813685247.5, -1.0, 1.0, -NAN, -NAN, NAN, -NAN };
	const int sol_quo[] = { 0, 0, -1, -1, -2, -2, 0, 0, 0, 0 };
	long double ret_val[SIZE(sol_val, long double)];
	int ret_quo[SIZE(sol_quo, int)] = { 0 };
	int remquol_idx;
	long double compute_val;

	for (remquol_idx = 0; remquol_idx < SIZE(sol_val, long double); remquol_idx++) {
		ret_val[remquol_idx] = remquol(in_val1[remquol_idx], in_val2[remquol_idx], &ret_quo[remquol_idx]);
		compute_val = fabsl(sol_val[remquol_idx] - ret_val[remquol_idx]);

		if ((isnan(sol_val[remquol_idx]) ^ (isnan(ret_val[remquol_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("remquol", compute_val, FLT_EPSILON);
		TC_ASSERT_EQ("remquol", sol_quo[remquol_idx], ret_quo[remquol_idx]);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_rint
 * @brief                :Returns the rounded integer value
 * @Scenario             :round their argument to an integer value in floating-point format
 * API's covered         :rint
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_rint(void)
{
	const double in_val[] = { ZERO, -ZERO, 1073741823.5, -1073741820.5, 2147483645.5, -2147483642.5, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { ZERO, -ZERO, 1073741824.0, -1073741820.0, 2147483646.0, -2147483642.0, INFINITY, -INFINITY, NAN };
	double ret_val[SIZE(sol_val, double)];
	int rint_idx;
	double compute_val;

	for (rint_idx = 0; rint_idx < SIZE(sol_val, double); rint_idx++) {
		ret_val[rint_idx] = rint(in_val[rint_idx]);
		compute_val = fabs(sol_val[rint_idx] - ret_val[rint_idx]);

		if ((isnan(sol_val[rint_idx]) ^ (isnan(ret_val[rint_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("rint", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_rintf
 * @brief                :Returns the rounded integer value
 * @Scenario             :round their argument to an integer value in floating-point format
 * API's covered         :rintf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_rintf(void)
{
	const float in_val[] = { ZERO, -ZERO, 1073741823.5, -1073741820.5, 2147483645.5, -2147483642.5, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { ZERO, -ZERO, 1073741824.0, -1073741820.0, 2147483646.0, -2147483642.0, INFINITY, -INFINITY, NAN };
	float ret_val[SIZE(sol_val, float)];
	int rintf_idx;
	float compute_val;

	for (rintf_idx = 0; rintf_idx < SIZE(sol_val, float); rintf_idx++) {
		ret_val[rintf_idx] = rintf(in_val[rintf_idx]);
		compute_val = fabsf(sol_val[rintf_idx] - ret_val[rintf_idx]);

		if ((isnan(sol_val[rintf_idx]) ^ (isnan(ret_val[rintf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("rintf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_rintl
 * @brief                :Returns the rounded integer value
 * @Scenario             :round their argument to an integer value in floating-point format
 * API's covered         :rintl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_rintl(void)
{
	const long double in_val[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { ZERO, -ZERO, 2251799813685248.0, -2251799813685248.0, 4503599627370496.0, -4503599627370496.0, INFINITY, -INFINITY, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int rintl_idx;
	long double compute_val;

	for (rintl_idx = 0; rintl_idx < SIZE(sol_val, long double); rintl_idx++) {
		ret_val[rintl_idx] = rintl(in_val[rintl_idx]);
		compute_val = fabsl(sol_val[rintl_idx] - ret_val[rintl_idx]);

		if ((isnan(sol_val[rintl_idx]) ^ (isnan(ret_val[rintl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("rintl", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_round
 * @brief                :Returns the rounded integer value
 * @Scenario             :round their argument to nearest integer, away from zero
 * API's covered         :round
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_round(void)
{
	const double in_val[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { ZERO, -ZERO, 2251799813685249.0, -2251799813685249.0, 4503599627370496.0, -4503599627370496.0, INFINITY, -INFINITY, NAN };
	double ret_val[SIZE(sol_val, double)];
	int round_idx;
	double compute_val;

	for (round_idx = 0; round_idx < SIZE(sol_val, double); round_idx++) {
		ret_val[round_idx] = round(in_val[round_idx]);
		compute_val = fabs(sol_val[round_idx] - ret_val[round_idx]);

		if ((isnan(sol_val[round_idx]) ^ (isnan(ret_val[round_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("round", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_roundf
 * @brief                :Returns the rounded integer value
 * @Scenario             :round their argument to nearest integer, away from zero
 * API's covered         :roundf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_roundf(void)
{
	const float in_val[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0.5f, -0.5f };
	const float sol_val[] = { ZERO, -ZERO, 2251799813685248.0, -2251799813685248.0, 4503599627370496.0, -4503599627370496.0, INFINITY, -INFINITY, NAN, 1.0f, -1.0f };
	float ret_val[SIZE(sol_val, float)];
	int roundf_idx;
	float compute_val;

	for (roundf_idx = 0; roundf_idx < SIZE(sol_val, float); roundf_idx++) {
		ret_val[roundf_idx] = roundf(in_val[roundf_idx]);
		compute_val = fabsf(sol_val[roundf_idx] - ret_val[roundf_idx]);

		if ((isnan(sol_val[roundf_idx]) ^ (isnan(ret_val[roundf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("roundf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_roundl
 * @brief                :Returns the rounded integer value
 * @Scenario             :round their argument to nearest integer, away from zero
 * API's covered         :roundl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_roundl(void)
{
	const long double in_val[] = { ZERO, -ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { ZERO, -ZERO, 2251799813685249.0, -2251799813685249.0, 4503599627370496.0, -4503599627370496.0, INFINITY, -INFINITY, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int roundl_idx;
	long double compute_val;

	for (roundl_idx = 0; roundl_idx < SIZE(sol_val, long double); roundl_idx++) {
		ret_val[roundl_idx] = roundl(in_val[roundl_idx]);
		compute_val = fabsl(sol_val[roundl_idx] - ret_val[roundl_idx]);

		if ((isnan(sol_val[roundl_idx]) ^ (isnan(ret_val[roundl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("roundl", compute_val, FLT_EPSILON);
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
	const double in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const int exp[] = { 2, -7, -1024, 1024, 3072, -3072 };
	const double sol_val[][SIZE(exp, int)] = { {ZERO, ZERO, ZERO, ZERO, ZERO, ZERO}, {9007199254740994.0, 17592186044416.003906250, ZERO, INFINITY, INFINITY, ZERO}, { -9007199254740994.0, -17592186044416.003906250, -ZERO, -INFINITY, -INFINITY, -ZERO}, {18014398509481984.0, 35184372088832.0, ZERO, INFINITY, INFINITY, ZERO}, { -18014398509481984.0, -35184372088832.0, -ZERO, -INFINITY, -INFINITY, -ZERO}, {INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY}, { -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY}, {NAN, NAN, NAN, NAN, NAN, NAN} };
	double ret_val[SIZE(in_val, double)][SIZE(exp, int)];
	int scalbn_idx;
	int exp_idx;
	double compute_val;

	for (scalbn_idx = 0; scalbn_idx < SIZE(in_val, double); scalbn_idx++) {
		for (exp_idx = 0; exp_idx < SIZE(exp, int); exp_idx++) {
			ret_val[scalbn_idx][exp_idx] = scalbn(in_val[scalbn_idx], exp[exp_idx]);
			compute_val = fabs(sol_val[scalbn_idx][exp_idx] - ret_val[scalbn_idx][exp_idx]);

			if ((isnan(sol_val[scalbn_idx][exp_idx]) ^ (isnan(ret_val[scalbn_idx][exp_idx])))) {
				compute_val = ONE;
			}

			TC_ASSERT_LEQ("scalbn", compute_val, FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_scalbnf
 * @brief                :Returns the multiply of argument1 by FLT_RADIX (probably 2) to the power of argument2
 * @Scenario             :Returns the multiply of argument1 by FLT_RADIX (probably 2) to the power of argument2
 * API's covered         :scalbnf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_scalbnf(void)
{
	const float in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const int exp[] = { 2, -7, -1024, 1024, 3072, -3072 };
	const float sol_val[][SIZE(exp, int)] = { {ZERO, ZERO, ZERO, ZERO, ZERO, ZERO}, {9007199254740994.0, 17592186044416.003906250, ZERO, INFINITY, INFINITY, ZERO}, { -9007199254740994.0, -17592186044416.003906250, -ZERO, -INFINITY, -INFINITY, -ZERO}, {18014398509481984.0, 35184372088832.0, ZERO, INFINITY, INFINITY, ZERO}, { -18014398509481984.0, -35184372088832.0, -ZERO, -INFINITY, -INFINITY, -ZERO}, {INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY}, { -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY}, {NAN, NAN, NAN, NAN, NAN, NAN} };
	float ret_val[SIZE(in_val, float)][SIZE(exp, int)];
	int scalbnf_idx;
	int exp_idx;
	float compute_val;

	for (scalbnf_idx = 0; scalbnf_idx < SIZE(in_val, float); scalbnf_idx++) {
		for (exp_idx = 0; exp_idx < SIZE(exp, int); exp_idx++) {
			ret_val[scalbnf_idx][exp_idx] = scalbnf(in_val[scalbnf_idx], exp[exp_idx]);
			compute_val = fabsf(sol_val[scalbnf_idx][exp_idx] - ret_val[scalbnf_idx][exp_idx]);

			if ((isnan(sol_val[scalbnf_idx][exp_idx]) ^ (isnan(ret_val[scalbnf_idx][exp_idx])))) {
				compute_val = ONE;
			}

			TC_ASSERT_LEQ("scalbnf", compute_val, FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_scalbnl
 * @brief                :Returns the multiply of argument1 by FLT_RADIX (probably 2) to the power of argument2
 * @Scenario             :Returns the multiply of argument1 by FLT_RADIX (probably 2) to the power of argument2
 * API's covered         :scalbnl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_scalbnl(void)
{
	const long double in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const int exp[] = { 2, -7, -1024, 1024, 3072, -3072 };
	const long double sol_val[][SIZE(exp, int)] = { {ZERO, ZERO, ZERO, ZERO, ZERO, ZERO}, {9007199254740994.0, 17592186044416.003906250, ZERO, INFINITY, INFINITY, ZERO}, { -9007199254740994.0, -17592186044416.003906250, -ZERO, -INFINITY, -INFINITY, -ZERO}, {18014398509481984.0, 35184372088832.0, ZERO, INFINITY, INFINITY, ZERO}, { -18014398509481984.0, -35184372088832.0, -ZERO, -INFINITY, -INFINITY, -ZERO}, {INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY}, { -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY}, {NAN, NAN, NAN, NAN, NAN, NAN} };
	long double ret_val[SIZE(in_val, long double)][SIZE(exp, int)];
	int scalbnl_idx;
	int exp_idx;
	long double compute_val;

	for (scalbnl_idx = 0; scalbnl_idx < SIZE(in_val, long double); scalbnl_idx++) {
		for (exp_idx = 0; exp_idx < SIZE(exp, int); exp_idx++) {
			ret_val[scalbnl_idx][exp_idx] = scalbnl(in_val[scalbnl_idx], exp[exp_idx]);
			compute_val = fabsl(sol_val[scalbnl_idx][exp_idx] - ret_val[scalbnl_idx][exp_idx]);

			if ((isnan(sol_val[scalbnl_idx][exp_idx]) ^ (isnan(ret_val[scalbnl_idx][exp_idx])))) {
				compute_val = ONE;
			}

			TC_ASSERT_LEQ("scalbnl", compute_val, FLT_EPSILON);
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
	const double in_val[] = { 3, -3, 0, M_PI, M_PI_2, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 0.14112000805987, -0.14112000805987, 0, 1.2246467991474e-16, 1, NAN, NAN, NAN };
	double ret_val[SIZE(sol_val, double)];
	int sin_idx;
	double compute_val;

	/* Returns the sine of an angle of x radians */

	for (sin_idx = 0; sin_idx < SIZE(sol_val, double); sin_idx++) {
		ret_val[sin_idx] = sin(in_val[sin_idx]);
		compute_val = fabs(sol_val[sin_idx] - ret_val[sin_idx]);

		if ((isnan(sol_val[sin_idx]) ^ (isnan(ret_val[sin_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("sin", compute_val, FLT_EPSILON);
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
	const float in_val[] = { 3, -3, 0, M_PI_2, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { 0.141120, -0.141120, 0.000000, 1, NAN, NAN, NAN };
	float ret_val[SIZE(sol_val, float)];
	int sinf_idx;
	float compute_val;

	/* Returns the sine of an angle of x radians */

	for (sinf_idx = 0; sinf_idx < SIZE(sol_val, float); sinf_idx++) {
		ret_val[sinf_idx] = sinf(in_val[sinf_idx]);
		compute_val = fabsf(sol_val[sinf_idx] - ret_val[sinf_idx]);

		if ((isnan(sol_val[sinf_idx]) ^ (isnan(ret_val[sinf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("sinf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_sinh
 * @brief               :Returns the hyperbolic sine of x
 * @scenario            :Returns the hyperbolic sine of x
 * @API's covered       :sinh
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_sinh(void)
{
	const double in_val[] = { 3, -3, 0, M_PI, M_PI_2, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 10.01787492741, -10.01787492741, 0, 11.548739357258, 2.3012989023073, INFINITY, -INFINITY, NAN };
	double ret_val[SIZE(sol_val, double)];
	int sinh_idx;
	double compute_val;

	/* Returns the hyperbolic sine of x */

	for (sinh_idx = 0; sinh_idx < SIZE(sol_val, double); sinh_idx++) {
		ret_val[sinh_idx] = sinh(in_val[sinh_idx]);
		compute_val = fabs(sol_val[sinh_idx] - ret_val[sinh_idx]);

		if ((isnan(sol_val[sinh_idx]) ^ (isnan(ret_val[sinh_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("sinh", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_sinhf
 * @brief               :Returns the hyperbolic sine of x
 * @scenario            :Returns the hyperbolic sine of x
 * @API's covered       :sinhf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_sinhf(void)
{
	const float in_val[] = { 3, -3, 0, M_PI, M_PI_2, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { 10.0178747177124, -10.0178756713867, 0.0000000000000, 11.5487403869629, 2.3012990951538, INFINITY, -INFINITY, NAN };
	float ret_val[SIZE(sol_val, float)];
	int sinhf_idx;
	float compute_val;

	/* Returns the hyperbolic sine of x */

	for (sinhf_idx = 0; sinhf_idx < SIZE(sol_val, float); sinhf_idx++) {
		ret_val[sinhf_idx] = sinhf(in_val[sinhf_idx]);
		compute_val = fabsf(sol_val[sinhf_idx] - ret_val[sinhf_idx]);

		if ((isnan(sol_val[sinhf_idx]) ^ (isnan(ret_val[sinhf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("sinhf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_sinhl
 * @brief               :Returns the hyperbolic sine of x
 * @scenario            :Returns the hyperbolic sine of x
 * @API's covered       :sinhl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_sinhl(void)
{
	const long double in_val[] = { 3, -3, 0, M_PI, M_PI_2, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { 10.01787492741, -10.01787492741, 0, 11.548739357258, 2.3012989023073, INFINITY, -INFINITY, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int sinhl_idx;
	long double compute_val;

	/* Returns the hyperbolic sine of x */

	for (sinhl_idx = 0; sinhl_idx < SIZE(sol_val, long double); sinhl_idx++) {
		ret_val[sinhl_idx] = sinhl(in_val[sinhl_idx]);
		compute_val = fabsl(sol_val[sinhl_idx] - ret_val[sinhl_idx]);

		if ((isnan(sol_val[sinhl_idx]) ^ (isnan(ret_val[sinhl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("sinhl", compute_val, FLT_EPSILON);
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
	const long double in_val[] = { 3, -3, 0, M_PI, M_PI_2, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { 0.14112000805987, -0.14112000805987, 0, 1.2246467991474e-16, 1, NAN, NAN, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int sinl_idx;
	long double compute_val;

	/* Returns the sine of an angle of x radians */

	for (sinl_idx = 0; sinl_idx < SIZE(sol_val, long double); sinl_idx++) {
		ret_val[sinl_idx] = sinl(in_val[sinl_idx]);
		compute_val = fabsl(sol_val[sinl_idx] - ret_val[sinl_idx]);

		if ((isnan(sol_val[sinl_idx]) ^ (isnan(ret_val[sinl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("sinl", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_sqrt
 * @brief                :Returns the square root of argument1
 * @Scenario             :Returns the square root of argument1
 * API's covered         :sqrt
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_sqrt(void)
{
	const double in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const double sol_val[] = { ZERO, 47453132.8121257797, -NAN, 67108864.0, -NAN, INFINITY, -NAN, NAN };
	double ret_val[SIZE(sol_val, double)];
	int sqrt_idx;
	double compute_val;

	for (sqrt_idx = 0; sqrt_idx < SIZE(sol_val, double); sqrt_idx++) {
		ret_val[sqrt_idx] = sqrt(in_val[sqrt_idx]);
		compute_val = fabs(sol_val[sqrt_idx] - ret_val[sqrt_idx]);

		if ((isnan(sol_val[sqrt_idx]) ^ (isnan(ret_val[sqrt_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("sqrt", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_sqrtf
 * @brief                :Returns the square root of argument1
 * @Scenario             :Returns the square root of argument1
 * API's covered         :sqrtf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_sqrtf(void)
{
	const float in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { ZERO, 47453132.8121257797, -NAN, 67108864.0, -NAN, INFINITY, -NAN, NAN };
	float ret_val[SIZE(sol_val, float)];
	int sqrtf_idx;
	float compute_val;

	for (sqrtf_idx = 0; sqrtf_idx < SIZE(sol_val, float); sqrtf_idx++) {
		ret_val[sqrtf_idx] = sqrtf(in_val[sqrtf_idx]);
		compute_val = fabsf(sol_val[sqrtf_idx] - ret_val[sqrtf_idx]);

		if ((isnan(sol_val[sqrtf_idx]) ^ (isnan(ret_val[sqrtf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("sqrtf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_sqrtl
 * @brief                :Returns the square root of argument1
 * @Scenario             :Returns the square root of argument1
 * API's covered         :sqrtl
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_sqrtl(void)
{
	const long double in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { ZERO, 47453132.8121257797, -NAN, 67108864.0, -NAN, INFINITY, -NAN, NAN };
	long double ret_val[SIZE(sol_val, long double)];
	int sqrtl_idx;
	long double compute_val;

	for (sqrtl_idx = 0; sqrtl_idx < SIZE(sol_val, long double); sqrtl_idx++) {
		ret_val[sqrtl_idx] = sqrtl(in_val[sqrtl_idx]);
		compute_val = fabsl(sol_val[sqrtl_idx] - ret_val[sqrtl_idx]);

		if ((isnan(sol_val[sqrtl_idx]) ^ (isnan(ret_val[sqrtl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("sqrtl", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_tan
 * @brief               :Returns the tangent of an angle of x radians
 * @scenario            :Returns the tangent of an angle of x radians
 * @API's covered       :tan
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_tan(void)
{
	const double in_val[] = { M_PI_4, 0.50, -0.50, 5, 10, -5, -10, ZERO,  INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 1, 0.54630248984379, -0.54630248984379, -3.3805150062466, 0.64836082745909, 3.3805150062466, -0.64836082745909, ZERO,  NAN, NAN, NAN };
	double ret_val[SIZE(in_val, double)];
	int tan_idx;
	double compute_val;

	/* Returns the tangent of an angle x */

	for (tan_idx = 0; tan_idx < SIZE(in_val, double); tan_idx++) {
		ret_val[tan_idx] = tan(in_val[tan_idx]);
		compute_val = fabs(sol_val[tan_idx] - ret_val[tan_idx]);

		if ((isnan(sol_val[tan_idx]) ^ (isnan(ret_val[tan_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("tan", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_tanf
 * @brief               :Returns the tangent of an angle of x radians
 * @scenario            :Returns the tangent of an angle of x radians
 * @API's covered       :tanf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_tanf(void)
{
	const float in_val[] = { M_PI_4, 0.50, -0.50, 5, 10, -5, -10, ZERO, INFINITY, -INFINITY, NAN };
	const float sol_val[] = { 1, 0.54630248984379, -0.54630248984379, -3.3805160522461, 0.6483603715897, 3.3805131912231, -0.6483607292175, ZERO, NAN, NAN, NAN };
	float ret_val[SIZE(in_val, double)];
	int tanf_idx;
	float compute_val;

	/* Returns the tangent of an angle x */

	for (tanf_idx = 0; tanf_idx < SIZE(in_val, double); tanf_idx++) {
		ret_val[tanf_idx] = tanf(in_val[tanf_idx]);
		compute_val = fabsf(sol_val[tanf_idx] - ret_val[tanf_idx]);

		if ((isnan(sol_val[tanf_idx]) ^ (isnan(ret_val[tanf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("tanf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_tanh
 * @brief               :Return the hyperbolic tangent of an angle of x radians
 * @scenario            :Return the hyperbolic tangent of an angle of x radians
 * @API's covered       :tanh
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_tanh(void)
{
	const double in_val[] = { M_PI_4, 0.50, -0.50, 5, 10, -5, -10, ZERO,  INFINITY, -INFINITY, NAN };
	const double sol_val[] = { 0.65579420263267, 0.46211715726001, -0.46211715726001, 0.9999092042626, 0.99999999587769, -0.9999092042626, -0.99999999587769, ZERO, 1.0, -1.0, NAN };
	double ret_val[SIZE(in_val, double)];
	int tanh_idx;
	double compute_val;

	/* Returns the hyperbolic tangent of an angle x */

	for (tanh_idx = 0; tanh_idx < SIZE(in_val, double); tanh_idx++) {
		ret_val[tanh_idx] = tanh(in_val[tanh_idx]);
		compute_val = fabs(sol_val[tanh_idx] - ret_val[tanh_idx]);

		if ((isnan(sol_val[tanh_idx]) ^ (isnan(ret_val[tanh_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("tanh", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_tanhf
 * @brief               :Return the hyperbolic tangent of an angle of x radians
 * @scenario            :Return the hyperbolic tangent of an angle of x radians
 * @API's covered       :tanhf
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_tanhf(void)
{
	const float in_val[] = { M_PI_4, 0.50, -0.50, 5, 10, -5, -10, ZERO,  INFINITY, -INFINITY, NAN };
	const float sol_val[] = { 0.65579420263267, 0.46211715726001, -0.46211715726001, 0.9999092042626, 0.99999999587769, -0.9999092042626, -0.99999999587769, ZERO, 1.0, -1.0, NAN };
	float ret_val[SIZE(in_val, double)];
	int tanhf_idx;
	float compute_val;

	/* Returns the hyperbolic tangent of an angle x */

	for (tanhf_idx = 0; tanhf_idx < SIZE(in_val, double); tanhf_idx++) {
		ret_val[tanhf_idx] = tanhf(in_val[tanhf_idx]);
		compute_val = fabsf(sol_val[tanhf_idx] - ret_val[tanhf_idx]);

		if ((isnan(sol_val[tanhf_idx]) ^ (isnan(ret_val[tanhf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("tanhf", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_tanhl
 * @brief               :Return the hyperbolic tangent of an angle of x radians
 * @scenario            :Return the hyperbolic tangent of an angle of x radians
 * @API's covered       :tanhl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_tanhl(void)
{
	const long double in_val[] = { M_PI_4, 0.50, -0.50, 5, 10, -5, -10, ZERO,  INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { 0.65579420263267, 0.46211715726001, -0.46211715726001, 0.9999092042626, 0.99999999587769, -0.9999092042626, -0.99999999587769, ZERO, 1.0, -1.0, NAN };
	long double ret_val[SIZE(in_val, double)];
	int tanhl_idx;
	long double compute_val;

	/* Returns the hyperbolic tangent of an angle x */

	for (tanhl_idx = 0; tanhl_idx < SIZE(in_val, double); tanhl_idx++) {
		ret_val[tanhl_idx] = tanhl(in_val[tanhl_idx]);
		compute_val = fabsl(sol_val[tanhl_idx] - ret_val[tanhl_idx]);

		if ((isnan(sol_val[tanhl_idx]) ^ (isnan(ret_val[tanhl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("tanhl", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_math_tanl
 * @brief               :Returns the tangent of an angle of x radians
 * @scenario            :Returns the tangent of an angle of x radians
 * @API's covered       :tanl
 * @Preconditions       :None
 * @Postconditions      :None
 * @Return              :void
 */
static void tc_libc_math_tanl(void)
{
	const long double in_val[] = { M_PI_4, 0.50, -0.50, 5, 10, -5, -10, ZERO, INFINITY, -INFINITY, NAN };
	const long double sol_val[] = { 1, 0.54630248984379, -0.54630248984379, -3.3805150062466, 0.64836082745909, 3.3805150062466, -0.64836082745909, ZERO, NAN, NAN, NAN };
	long double ret_val[SIZE(in_val, double)];
	int tanl_idx;
	long double compute_val;

	/* Returns the tangent of an angle x */

	for (tanl_idx = 0; tanl_idx < SIZE(in_val, double); tanl_idx++) {
		ret_val[tanl_idx] = tanl(in_val[tanl_idx]);
		compute_val = fabsl(sol_val[tanl_idx] - ret_val[tanl_idx]);

		if ((isnan(sol_val[tanl_idx]) ^ (isnan(ret_val[tanl_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("tanl", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_trunc
 * @brief                :Round to truncated integer value
 * @Scenario             :Round to truncated integer value
 * API's covered         :trunc
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_trunc(void)
{
	const double in_val[] = { 4.243, 0, -10.990, -INFINITY, INFINITY, NAN };
	const double sol_val[] = { 4.000, 0, -10.000, -INFINITY, INFINITY, NAN };
	double ret_val[SIZE(sol_val, double)];
	int trunc_idx;
	double compute_val;

	/* Round to truncated integer value */

	for (trunc_idx = 0; trunc_idx < SIZE(sol_val, double); trunc_idx++) {
		ret_val[trunc_idx] = trunc(in_val[trunc_idx]);
		compute_val = fabs(sol_val[trunc_idx] - ret_val[trunc_idx]);

		if ((isnan(sol_val[trunc_idx]) ^ (isnan(ret_val[trunc_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("trunc", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_truncf
 * @brief                :Round to truncated integer value
 * @Scenario             :Round to truncated integer value
 * API's covered         :truncf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_truncf(void)
{
	const float in_val[] = { 4.243, 0, -10.990, -INFINITY, INFINITY, NAN };
	const float sol_val[] = { 4.000, 0, -10.000, -INFINITY, INFINITY, NAN };
	float ret_val[SIZE(sol_val, float)];
	int truncf_idx;
	float compute_val;

	/* Round to truncated integer value */

	for (truncf_idx = 0; truncf_idx < SIZE(sol_val, float); truncf_idx++) {
		ret_val[truncf_idx] = truncf(in_val[truncf_idx]);
		compute_val = fabsf(sol_val[truncf_idx] - ret_val[truncf_idx]);

		if ((isnan(sol_val[truncf_idx]) ^ (isnan(ret_val[truncf_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("truncf", compute_val, FLT_EPSILON);
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
	const double in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p-27, 0x1p-30 };
	const double sol_val[] = { -INFINITY, ZERO, NAN, ZERO, NAN, ZERO, NAN, NAN, -11.9881267033518, -13.3119403042677 };
	double ret_val[SIZE(sol_val, double)];
	int y0_idx;
	double compute_val;

	for (y0_idx = 0; y0_idx < SIZE(sol_val, double); y0_idx++) {
		ret_val[y0_idx] = y0(in_val[y0_idx]);
		compute_val = fabs(sol_val[y0_idx] - ret_val[y0_idx]);

		if ((isnan(sol_val[y0_idx]) ^ (isnan(ret_val[y0_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("y0", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_y0f
 * @brief                :Returns the bessel value of 2nd kind of order 0
 * @Scenario             :Returns the bessel value of 2nd kind of order 0
 * API's covered         :y0f
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_y0f(void)
{
	const float in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p-13, 0x1p-15 };
	const float sol_val[] = { -INFINITY, ZERO, NAN, ZERO, NAN, ZERO, NAN, NAN, -5.810329437255859, -6.692872047424316 };
	float ret_val[SIZE(sol_val, float)];
	int y0f_idx;
	float compute_val;

	for (y0f_idx = 0; y0f_idx < SIZE(sol_val, float); y0f_idx++) {
		ret_val[y0f_idx] = y0f(in_val[y0f_idx]);
		compute_val = fabsf(sol_val[y0f_idx] - ret_val[y0f_idx]);

		if ((isnan(sol_val[y0f_idx]) ^ (isnan(ret_val[y0f_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("y0f", compute_val, FLT_EPSILON);
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
	const double in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p-60, 0x1p-54 };
	const double sol_val[] = { -INFINITY, -ZERO, NAN, -ZERO, NAN, ZERO, NAN, NAN, -733972625820500352.0, -11468322278445318.0 };
	double ret_val[SIZE(sol_val, double)];
	int y1_idx;
	double compute_val;

	for (y1_idx = 0; y1_idx < SIZE(sol_val, double); y1_idx++) {
		ret_val[y1_idx] = y1(in_val[y1_idx]);
		compute_val = fabs(sol_val[y1_idx] - ret_val[y1_idx]);

		if ((isnan(sol_val[y1_idx]) ^ (isnan(ret_val[y1_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("y1", compute_val, FLT_EPSILON);
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_y1f
 * @brief                :Returns the bessel value of 2nd kind of order 1
 * @Scenario             :Returns the bessel value of 2nd kind of order 1
 * API's covered         :y1f
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_y1f(void)
{
	const float in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p-30, 0x1p-27 };
	const float sol_val[] = { -INFINITY, -ZERO, NAN, -ZERO, NAN, ZERO, NAN, NAN, -683565248.0, -85445656.0 };
	float ret_val[SIZE(sol_val, float)];
	int y1f_idx;
	float compute_val;

	for (y1f_idx = 0; y1f_idx < SIZE(sol_val, float); y1f_idx++) {
		ret_val[y1f_idx] = y1f(in_val[y1f_idx]);
		compute_val = fabsf(sol_val[y1f_idx] - ret_val[y1f_idx]);

		if ((isnan(sol_val[y1f_idx]) ^ (isnan(ret_val[y1f_idx])))) {
			compute_val = ONE;
		}

		TC_ASSERT_LEQ("y1f", compute_val, FLT_EPSILON);
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
	const double in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN, 0x1p302 };
	const double sol_val[][7] = { {INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY}, {ZERO, ZERO, -ZERO, -ZERO, ZERO, ZERO, -ZERO}, {NAN, NAN, NAN, NAN, NAN, NAN, NAN}, {ZERO, ZERO, -ZERO, -ZERO, ZERO, ZERO, -ZERO}, {NAN, NAN, NAN, NAN, NAN, NAN, NAN}, { -ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO}, {NAN, NAN, NAN, NAN, NAN, NAN, NAN}, {NAN, NAN, NAN, NAN, NAN, NAN, NAN}, { -ZERO, -ZERO, ZERO, ZERO, -ZERO, -ZERO, ZERO} };
	double ret_val[SIZE(in_val, double)][7];
	int yn_idx;
	int order_idx;
	double compute_val;

	for (yn_idx = 0; yn_idx < SIZE(in_val, double); yn_idx++) {
		for (order_idx = 0; order_idx < 7; order_idx++) {
			ret_val[yn_idx][order_idx] = yn(order_idx - 1, in_val[yn_idx]);
			compute_val = fabs(sol_val[yn_idx][order_idx] - ret_val[yn_idx][order_idx]);

			if ((isnan(sol_val[yn_idx][order_idx]) ^ (isnan(ret_val[yn_idx][order_idx])))) {
				compute_val = ONE;
			}

			TC_ASSERT_LEQ("yn", compute_val, FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_ynf
 * @brief                :Returns the bessel value of 2nd kind of order n
 * @Scenario             :Returns the bessel value of 2nd kind of order n
 * API's covered         :ynf
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_ynf(void)
{
	const float in_val[] = { ZERO, VAL1, -VAL1, VAL2, -VAL2, INFINITY, -INFINITY, NAN };
	const float sol_val[][5] = { {INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY}, {ZERO, ZERO, -ZERO, -ZERO, ZERO}, {NAN, NAN, NAN, NAN, NAN}, {ZERO, ZERO, -ZERO, -ZERO, ZERO}, {NAN, NAN, NAN, NAN, NAN}, { -ZERO, ZERO, ZERO, ZERO, ZERO}, {NAN, NAN, NAN, NAN, NAN}, {NAN, NAN, NAN, NAN, NAN} };
	float ret_val[SIZE(in_val, float)][5];
	int ynf_idx;
	int order_idx;
	float compute_val;

	for (ynf_idx = 0; ynf_idx < SIZE(in_val, float); ynf_idx++) {
		for (order_idx = 0; order_idx < 5; order_idx++) {
			ret_val[ynf_idx][order_idx] = ynf(order_idx - 1, in_val[ynf_idx]);
			compute_val = fabsf(sol_val[ynf_idx][order_idx] - ret_val[ynf_idx][order_idx]);

			if ((isnan(sol_val[ynf_idx][order_idx]) ^ (isnan(ret_val[ynf_idx][order_idx])))) {
				compute_val = ONE;
			}

			TC_ASSERT_LEQ("ynf", compute_val, FLT_EPSILON);
		}
	}

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_gamma
 * @brief                :gamma(d) function returns (d-1)!
 * @Scenario             :put a value and check the return
 * API's covered         :gamma
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_gamma(void)
{
	double in_val[13] = { 2.0, 3.0, INFINITY, 256.5,
						1.0e-20, -257.5, -256.5, -1.0,
						-1.5, 8.1, 8.3, 8.8, 9.3 };
	double sol_val[13] = { 1, 2, INFINITY, 0x1p1023 * 256.5,
						1 / 1.0e-20, ZERO, -ZERO, NAN,
						2.363271801, 6169.5936974846, 9281.3925257465,
						26339.986354509, 77035.557963696 };
	int digit = 1e5;
	int gamma_idx;
	/**
	 * gamma value comparison
	 */
	for (gamma_idx = 0; gamma_idx < SIZE(in_val, double); gamma_idx++) {
		if (gamma_idx >= 8) {
			/**
			 * Compare to the fifth decimal place (digit)
			 */
			TC_ASSERT_EQ("gamma", floor(gamma(in_val[gamma_idx]) * digit), floor(sol_val[gamma_idx] * digit));
		} else if (gamma_idx == 7) {
			/**
			 * Compare to NAN
			 */
			TC_ASSERT_EQ("gamma", isnan(gamma(in_val[gamma_idx])), isnan(sol_val[gamma_idx]));
		} else {
			TC_ASSERT_EQ("gamma", gamma(in_val[gamma_idx]), sol_val[gamma_idx]);
		}
	}
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_math_lgamma
 * @brief                :lgamma(d) function returns the log of |(d-1)!|
 * @Scenario             :put a value and check the return
 * API's covered         :lgamma
 * Preconditions         :None
 * Postconditions        :None
 * @return               :void
 */
static void tc_libc_math_lgamma(void)
{
	double value;
	double result;

	value = 2.0;
	result = lgamma(value);
	TC_ASSERT_EQ("lgamma", result, 0);

	value = INFINITY;
	result = lgamma(value);
	TC_ASSERT_EQ("lgamma", result, (double)INFINITY);

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
	tc_libc_math_acosh();
	tc_libc_math_acoshf();
	tc_libc_math_acoshl();
	tc_libc_math_acosl();
	tc_libc_math_asin();
	tc_libc_math_asinf();
	tc_libc_math_asinh();
	tc_libc_math_asinhf();
	tc_libc_math_asinhl();
	tc_libc_math_asinl();
	tc_libc_math_atan();
	tc_libc_math_atanf();
	tc_libc_math_atanh();
	tc_libc_math_atanhf();
	tc_libc_math_atanhl();
	tc_libc_math_atanl();
	tc_libc_math_atan2();
	tc_libc_math_atan2f();
	tc_libc_math_atan2l();
	tc_libc_math_cbrt();
	tc_libc_math_cbrtf();
	tc_libc_math_cbrtl();
	tc_libc_math_ceil();
	tc_libc_math_ceilf();
	tc_libc_math_ceill();
	tc_libc_math_copysign();
	tc_libc_math_copysignf();
	tc_libc_math_copysignl();
	tc_libc_math_cos();
	tc_libc_math_cosf();
	tc_libc_math_cosh();
	tc_libc_math_coshf();
	tc_libc_math_coshl();
	tc_libc_math_cosl();
	tc_libc_math_erf();
	tc_libc_math_erff();
	tc_libc_math_erfl();
	tc_libc_math_exp();
	tc_libc_math_expf();
	tc_libc_math_expl();
	tc_libc_math_exp2();
	tc_libc_math_exp2f();
	tc_libc_math_exp2l();
	tc_libc_math_fabs();
	tc_libc_math_fabsf();
	tc_libc_math_fabsl();
	tc_libc_math_fdim();
	tc_libc_math_fdimf();
	tc_libc_math_fdiml();
	tc_libc_math_floor();
	tc_libc_math_floorf();
	tc_libc_math_floorl();
	tc_libc_math_fmax();
	tc_libc_math_fmaxf();
	tc_libc_math_fmaxl();
	tc_libc_math_fmin();
	tc_libc_math_fminf();
	tc_libc_math_fminl();
	tc_libc_math_frexp();
	tc_libc_math_frexpf();
	tc_libc_math_frexpl();
	tc_libc_math_gamma();
	tc_libc_math_hypot();
	tc_libc_math_hypotf();
	tc_libc_math_hypotl();
	tc_libc_math_j0();
	tc_libc_math_j0f();
	tc_libc_math_j1();
	tc_libc_math_j1f();
	tc_libc_math_jn();
	tc_libc_math_jnf();
	tc_libc_math_ldexp();
	tc_libc_math_ldexpf();
	tc_libc_math_ldexpl();
	tc_libc_math_lgamma();
	tc_libc_math_log();
	tc_libc_math_log2();
	tc_libc_math_log2f();
	tc_libc_math_log2l();
	tc_libc_math_log10();
	tc_libc_math_log10f();
	tc_libc_math_log10l();
	tc_libc_math_nextafter();
	tc_libc_math_nextafterf();
	tc_libc_math_nextafterl();
	tc_libc_math_nexttoward();
	tc_libc_math_nexttowardf();
	tc_libc_math_nexttowardl();
	tc_libc_math_pow();
	tc_libc_math_remainder();
	tc_libc_math_remainderf();
	tc_libc_math_remainderl();
	tc_libc_math_remquo();
	tc_libc_math_remquof();
	tc_libc_math_remquol();
	tc_libc_math_rint();
	tc_libc_math_rintf();
	tc_libc_math_rintl();
	tc_libc_math_round();
	tc_libc_math_roundf();
	tc_libc_math_roundl();
	tc_libc_math_scalbn();
	tc_libc_math_scalbnf();
	tc_libc_math_scalbnl();
	tc_libc_math_sin();
	tc_libc_math_sinf();
	tc_libc_math_sinh();
	tc_libc_math_sinhf();
	tc_libc_math_sinhl();
	tc_libc_math_sinl();
	tc_libc_math_sqrt();
	tc_libc_math_sqrtf();
	tc_libc_math_sqrtl();
	tc_libc_math_tan();
	tc_libc_math_tanf();
	tc_libc_math_tanh();
	tc_libc_math_tanhf();
	tc_libc_math_tanhl();
	tc_libc_math_tanl();
	tc_libc_math_trunc();
	tc_libc_math_truncf();
	tc_libc_math_y0();
	tc_libc_math_y0f();
	tc_libc_math_y1();
	tc_libc_math_y1f();
	tc_libc_math_yn();
	tc_libc_math_ynf();
#else
	printf("tc_libc_math will not be executed. CONFIG_HAVE_DOUBLE is not set.\n");
#endif
	return 0;
}
