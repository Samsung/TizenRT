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

#include <math.h>
#include <stdint.h>
#include <tinyara/float.h>

#ifdef CONFIG_HAVE_DOUBLE

#if FLT_EVAL_METHOD > 1U && LDBL_MANT_DIG == 64
#define SPLIT (0x1p32 + 1)
#else
#define SPLIT (0x1p27 + 1)
#endif

/************************************************************************
 * Private Functions
 ************************************************************************/

static void sq(double *hi, double *lo, double x)
{
	double xh;
	double xl;
	double xc;

	xc = (double)x * SPLIT;
	xh = x - xc + xc;
	xl = x - xh;
	*hi = (double)x * x;
	*lo = xh * xh - *hi + 2 * xh * xl + xl * xl;
}

/************************************************************************
 * Public Functions
 ************************************************************************/

double hypot(double x, double y)
{
	union {
		double f;
		uint64_t i;
	} ux = {
		x
	}, uy = {
		y
	}, ut;
	int ex;
	int ey;
	double hx;
	double lx;
	double hy;
	double ly;
	double z;

	/* arrange |x| >= |y| */
	ux.i &= -1ULL >> 1;
	uy.i &= -1ULL >> 1;
	if (ux.i < uy.i) {
		ut = ux;
		ux = uy;
		uy = ut;
	}

	/* special cases */
	ex = ux.i >> 52;
	ey = uy.i >> 52;
	x = ux.f;
	y = uy.f;
	/* note: hypot(inf,nan) == inf */
	if (ey == 0x7ff) {
		return y;
	}
	if (ex == 0x7ff || uy.i == 0) {
		return x;
	}
	/* note: hypot(x,y) ~= x + y*y/x/2 with inexact for small y/x */
	/* 64 difference is enough for ld80 double */
	if (ex - ey > 64) {
		return x + y;
	}

	/* precise sqrt argument in nearest rounding mode without overflow */
	/* xh*xh must not overflow and xl*xl must not underflow in sq */
	z = 1;
	if (ex > 0x3ff + 510) {
		z = 0x1p700;
		x *= 0x1p-700;
		y *= 0x1p-700;
	} else if (ey < 0x3ff - 450) {
		z = 0x1p-700;
		x *= 0x1p700;
		y *= 0x1p700;
	}
	sq(&hx, &lx, x);
	sq(&hy, &ly, y);
	return z * sqrt(ly + lx + hy + hx);
}
#endif
