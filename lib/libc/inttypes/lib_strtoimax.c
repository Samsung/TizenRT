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
/****************************************************************************
 * lib/libc/inttypes/lib_strtoimax.c
 *
 *   Copyright (C) 2017 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdbool.h>
#include <inttypes.h>
#include <errno.h>

#include "lib_internal.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: strtoimax
 *
 * Description:
 *   The strtoimax() function  converts  the initial part of the string in
 *   nptr to a intmax_t integer value according to the given base, which
 *   must be between 2 and 36 inclusive, or be the special value 0.
 *
 * Returns:
 *   - The converted value, if the base and number are valid
 *   - 0 if an error occurs, and set errno to:
 *     * EINVAL if base < 2 or base > 36
 *   - INTMAX_MIN or INTMAX_MAX, of correct sign, if an overflow occurs,
 *     and set errno to:
 *     * ERANGE if the number cannot be represented using intmax_t
 *
 ****************************************************************************/

intmax_t strtoimax(FAR const char *nptr, FAR char **endptr, int base)
{
	uintmax_t accum = 0;
	bool negate = false;

	if (nptr) {
		/* Skip leading spaces */

		lib_skipspace(&nptr);

		/* Check for leading + or - */

		if (*nptr == '-') {
			negate = true;
			nptr++;
		} else if (*nptr == '+') {
			nptr++;
		}

		/* Get the unsigned value */

		accum = strtoull(nptr, endptr, base);

		/* Correct the sign of the result and check for overflow */

		if (negate) {
			const uintmax_t limit = ((uintmax_t) - (INTMAX_MIN + 1)) + 1;

			if (accum > limit) {
				set_errno(ERANGE);
				return INTMAX_MIN;
			}

			return (accum == limit) ? INTMAX_MIN : -(intmax_t) accum;
		}

		if (accum > INTMAX_MAX) {
			set_errno(ERANGE);
			return INTMAX_MAX;
		}
	}

	return (intmax_t) accum;
}
