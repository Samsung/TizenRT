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
/****************************************************************************
 * origin: FreeBSD /usr/src/lib/msun/src/s_cbrtf.c
 *
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 * Debugged and optimized by Bruce D. Evans.
 *
 *
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 * cbrtf(x)
 * Return cube root of x
 ***************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/compiler.h>

#include <math.h>
#include <stdint.h>

/************************************************************************
 * Private Data
 ************************************************************************/

static const unsigned
B1 = 709958130,				/* B1 = (127-127.0/3-0.03306235651)*2**23 */
B2 = 642849266;				/* B2 = (127-127.0/3-24/3-0.03306235651)*2**23 */

/************************************************************************
 * Public Functions
 ************************************************************************/

float cbrtf(float x)
{
	double r;
	double T;
	union {
		float f;
		uint32_t i;
	} u = {
		x
	};
	uint32_t hx = u.i & 0x7fffffff;

	if (hx >= 0x7f800000) {		/* cbrt(NaN,INF) is itself */
		return x + x;
	}

	/* rough cbrt to 5 bits */
	if (hx < 0x00800000) {		/* zero or subnormal? */
		if (hx == 0) {
			return x;			/* cbrt(+-0) is itself */
		}
		u.f = x * 0x1p24f;
		hx = u.i & 0x7fffffff;
		hx = hx / 3 + B2;
	} else {
		hx = hx / 3 + B1;
	}
	u.i &= 0x80000000;
	u.i |= hx;

	/*
	 * First step Newton iteration (solving t*t-x/t == 0) to 16 bits.  In
	 * double precision so that its terms can be arranged for efficiency
	 * without causing overflow or underflow.
	 */
	T = u.f;
	r = T * T * T;
	T = T * ((double)x + x + r) / (x + r + r);

	/*
	 * Second step Newton iteration to 47 bits.  In double precision for
	 * efficiency and accuracy.
	 */
	r = T * T * T;
	T = T * ((double)x + x + r) / (x + r + r);

	/* rounding to 24 bits is perfect in round-to-nearest mode */
	return T;
}
