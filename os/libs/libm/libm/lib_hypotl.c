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
 ***************************************************************************/
/****************************************************************************
 *
 * Copyright © 2005-2014 Rich Felker, et al.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/compiler.h>

#include "libm.h"

#ifdef CONFIG_HAVE_LONG_DOUBLE

/************************************************************************
 * Public Functions
 ************************************************************************/

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double hypotl(long double x, long double y)
{
	return hypot(x, y);
}
#elif (LDBL_MANT_DIG == 64 || LDBL_MANT_DIG == 113) && LDBL_MAX_EXP == 16384
#if LDBL_MANT_DIG == 64
#define SPLIT (0x1p32L+1)
#elif LDBL_MANT_DIG == 113
#define SPLIT (0x1p57L+1)
#endif

/************************************************************************
 * Private Functions
 ************************************************************************/

static void sq(long double *hi, long double *lo, long double x)
{
	long double xh;
	long double xl;
	long double xc;
	xc = x * SPLIT;
	xh = x - xc + xc;
	xl = x - xh;
	*hi = x * x;
	*lo = xh * xh - *hi + 2 * xh * xl + xl * xl;
}

/************************************************************************
 * Public Functions
 ************************************************************************/

long double hypotl(long double x, long double y)
{
	union ldshape ux = { x }, uy = {
		y
	};
	int ex;
	int ey;
	long double hx;
	long double lx;
	long double hy;
	long double ly;
	long double z;

	ux.i.se &= 0x7fff;
	uy.i.se &= 0x7fff;
	if (ux.i.se < uy.i.se) {
		ex = uy.i.se;
		ey = ux.i.se;
		x = uy.f;
		y = ux.f;
	} else {
		ex = ux.i.se;
		ey = uy.i.se;
		x = ux.f;
		y = uy.f;
	}

	if (ex == 0x7fff && isinf(y)) {
		return y;
	}
	if (ex == 0x7fff || y == 0) {
		return x;
	}
	if (ex - ey > LDBL_MANT_DIG) {
		return x + y;
	}

	z = 1;
	if (ex > 0x3fff + 8000) {
		z = 0x1p10000L;
		x *= 0x1p-10000L;
		y *= 0x1p-10000L;
	} else if (ey < 0x3fff - 8000) {
		z = 0x1p-10000L;
		x *= 0x1p10000L;
		y *= 0x1p10000L;
	}
	sq(&hx, &lx, x);
	sq(&hy, &ly, y);
	return z * sqrtl(ly + lx + hy + hx);
}
#endif
#endif
