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

/// @file tc_libc_fixedmath.c
/// @brief Test Case Example for Libc FixedMath API

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <math.h>
#include "tc_internal.h"
#include "fixedmath.h"

static void tc_fixedmath_b16mulb16(void)
{
	b16_t num;

	num = b16mulb16(b16TEN , b16TEN);
	TC_ASSERT_EQ("b16mulb16", num, b16HUNDRED);

	TC_SUCCESS_RESULT();
}

static void tc_fixedmath_ub16mulub16(void)
{
	ub16_t num;

	num = ub16mulub16(b16TEN , b16TEN);
	TC_ASSERT_EQ("ub16mulub16", num, b16HUNDRED);

	TC_SUCCESS_RESULT();
}

static void tc_fixedmath_b16sqr(void)
{
	b16_t num;

	num = b16sqr(b16TEN);
	TC_ASSERT_EQ("b16sqr", num, b16HUNDRED);

	TC_SUCCESS_RESULT();
}

static void tc_fixedmath_ub16sqr(void)
{
	ub16_t num;

	num = ub16sqr(b16TEN);
	TC_ASSERT_EQ("ub16sqr", num, b16HUNDRED);

	TC_SUCCESS_RESULT();
}

static void tc_fixedmath_b16divb16(void)
{
	b16_t num;

	num = b16divb16(b16HUNDRED , b16TEN);
	TC_ASSERT_EQ("b16divb16", num, b16TEN);

	TC_SUCCESS_RESULT();
}

static void tc_fixedmath_ub16divub16(void)
{
	ub16_t num;

	num = ub16divub16(b16HUNDRED , b16TEN);
	TC_ASSERT_EQ("ub16divub16", num, b16TEN);

	TC_SUCCESS_RESULT();
}

static void tc_fixedmath_b16sin(void)
{
	int degree;
	int radian;
	int output;
	b16_t b16radian;
	b16_t b16output;

	degree = -233;
	radian = (degree * M_PI) / 180;
	output = (int)sinf(radian);

	b16radian = itob16(radian);
	b16output = b16sin(b16radian);
	TC_ASSERT_EQ("b16sin", output, b16toi(b16output));

	degree = 0;
	radian = (degree * M_PI) / 180;
	output = (int)sinf(radian);

	b16radian = itob16(radian);
	b16output = b16sin(b16radian);
	TC_ASSERT_EQ("b16sin", output, b16toi(b16output));

	TC_SUCCESS_RESULT();
}

static void tc_fixedmath_b16cos(void)
{
	int degree = 0;
	int radian;
	int output;
	b16_t b16radian;
	b16_t b16output;

	radian = (degree * M_PI) / 180;
	output = (int)cosf(radian);
	b16radian = itob16(radian);
	b16output = b16cos(b16radian);

	TC_ASSERT_EQ("b16cos", output, OK);
	TC_ASSERT_EQ("b16cos", output, b16toi(b16output));

	TC_SUCCESS_RESULT();
}

static void tc_fixedmath_b16atan2(void)
{
	int x = -7;
	int y = 7;
	int radian;
	int output;
	b16_t b16_x;
	b16_t b16_y;
	b16_t b16output;

	radian = 180 / M_PI ;
	output = (int)atan2(y, x) * radian;

	b16_x = itob16(x);
	b16_y = itob16(y);
	b16output = b16atan2(b16_y, b16_x);
	TC_ASSERT_EQ("b16atan2", output, (b16toi(b16output) * radian));

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_fixedmath
 ****************************************************************************/
int libc_fixedmath_main(void)
{
	tc_fixedmath_b16atan2();
	tc_fixedmath_b16cos();
	tc_fixedmath_b16divb16();
	tc_fixedmath_b16mulb16();
	tc_fixedmath_b16sin();
	tc_fixedmath_b16sqr();
	tc_fixedmath_ub16divub16();
	tc_fixedmath_ub16mulub16();
	tc_fixedmath_ub16sqr();

	return 0;
}

