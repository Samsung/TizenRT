/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

#include <math.h>
#include <errno.h>

/************************************************************************
 * Public Functions
 ************************************************************************/

long int lrintf(float x) 
{
	long int ret;
	float intpart;
	float fracpart;

	/* If x is NaN, infinite, -infinite, too large or too small, domain error is returned */
	if (isnan(x) || isinf(x) || x > (float)LONG_MAX || x < (float)LONG_MIN) {
		set_errno(EDOM);
		return NAN;
	}

	/* If the current rounding mode rounds toward negative
	 * infinity, lrintf() is identical to floorf() with type conversion 
	 * to long. If the current rounding mode rounds toward 
	 * positive infinity, rintf() is identical to ceilf()
	 * with type conversion to long.
	 */
#if defined(CONFIG_FP_ROUND_POSITIVE) && CONFIG_FP_ROUNDING_POSITIVE != 0
	ret = (long int)ceilf(x);
#elif defined(CONFIG_FP_ROUND_NEGATIVE) && CONFIG_FP_ROUNDING_NEGATIVE != 0
	ret = (long int)floorf(x);
#else
	fracpart = modff(x, &intpart);	/* Separate the integer and fractional parts */
	if (x >= 0.0) {
		if (fracpart >= 0.5) {
			ret = (long int)intpart + 1;	/* Round up */
		} else {
			ret = (long int)intpart;	/* Round down */
		}
	} else {
		if (fracpart >= -0.5) {
			ret = (long int)intpart;	/* Round up */
		} else {
			ret = (long int)intpart - 1;	/* Round down */
		}
	}
#endif
	return ret;
}
