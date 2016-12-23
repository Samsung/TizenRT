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

/// @file libc_math.c
/// @brief Test Case Example for Libc Math API

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/compiler.h>
#include <stdio.h>
#include <tinyara/math.h>
#include "tc_internal.h"

#ifdef CONFIG_HAVE_DOUBLE

/* dobule has 52 bit fraction(IEEE754). 4503599627370496 == 0x10000000000000 */

#define ZERO 0.0
#define VAL1 2251799813685248.5
#define VAL2 4503599627370496.5

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
		if (sol_val[fabs_idx] != ret_val[fabs_idx]) {
			printf("tc_libc_math_fabs FAIL\n");
			total_fail++;
			RETURN_ERR;
		}
	}

	printf("tc_libc_math_fabs PASS\n");
	total_pass++;
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
		if (sol_val[floor_idx] != ret_val[floor_idx]) {
			printf("tc_libc_math_floor FAIL\n");
			total_fail++;
			RETURN_ERR;
		}
	}

	printf("tc_libc_math_floor PASS\n");
	total_pass++;
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
			if (sol_val[pow_idx] != ret_val[pow_idx]) {
				printf("tc_libc_math_pow FAIL\n");
				total_fail++;
				RETURN_ERR;
			}
		} else {
			if (fabs(sol_val[pow_idx] - ret_val[pow_idx]) > 0.00000000000001) {
				printf("tc_libc_math_pow FAIL\n");
				total_fail++;
				RETURN_ERR;
			}
		}
	}

	printf("tc_libc_math_pow PASS\n");
	total_pass++;
}
#endif

/****************************************************************************
 * Name: libc_math
 ****************************************************************************/
int libc_math_main(void)
{
#ifdef CONFIG_HAVE_DOUBLE
	tc_libc_math_fabs();
	tc_libc_math_floor();
	tc_libc_math_pow();
#else
	printf("tc_libc_math will not be executed. CONFIG_HAVE_DOUBLE is not set.\n");
#endif
	return 0;
}
