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
 * origin: FreeBSD /usr/src/lib/msun/src/math_private.h
 *
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 ***************************************************************************/

#ifndef _LIBM_H
#define _LIBM_H

/****************************************************************************
 *	Included Files
 ***************************************************************************/

#include <stdint.h>
#include <float.h>
#include <math.h>
#include <complex.h>
#include <tinyara/endian.h>

/****************************************************************************
 *	Global Type Declarations
 ***************************************************************************/

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
#elif LDBL_MANT_DIG == 64 && LDBL_MAX_EXP == 16384 && __BYTE_ORDER == __LITTLE_ENDIAN
union ldshape {
	long double f;
	struct {
		uint64_t m;
		uint16_t se;
	} i;
};
#elif LDBL_MANT_DIG == 113 && LDBL_MAX_EXP == 16384 && __BYTE_ORDER == __LITTLE_ENDIAN
union ldshape {
	long double f;
	struct {
		uint64_t lo;
		uint32_t mid;
		uint16_t top;
		uint16_t se;
	} i;
	struct {
		uint64_t lo;
		uint64_t hi;
	} i2;
};
#elif LDBL_MANT_DIG == 113 && LDBL_MAX_EXP == 16384 && __BYTE_ORDER == __BIG_ENDIAN
union ldshape {
	long double f;
	struct {
		uint16_t se;
		uint16_t top;
		uint32_t mid;
		uint64_t lo;
	} i;
	struct {
		uint64_t hi;
		uint64_t lo;
	} i2;
};
#else
#error Unsupported long double representation
#endif

/****************************************************************************
 *	Pre-processor Definitions
 ***************************************************************************/

#define FORCE_EVAL(x)                             \
	do {                                          \
		if (sizeof(x) == sizeof(float)) {         \
			volatile float __x = 0;               \
			UNUSED(__x);                          \
			__x = (x);                            \
		} else if (sizeof(x) == sizeof(double)) { \
			volatile double __x = 0;              \
			UNUSED(__x);                          \
			__x = (x);                            \
		}                                         \
		else {                                    \
			volatile long double __x = 0;         \
			UNUSED(__x);                          \
			__x = (x);                            \
		}                                         \
	} while (0)


/* Get two 32 bit ints from a double.  */
#define EXTRACT_WORDS(hi, lo, d)    \
	do {                            \
		union {                     \
			double f;               \
			uint64_t i;             \
		} __u;                      \
		__u.f = (d);                \
		(hi) = __u.i >> 32;         \
		(lo) = (uint32_t)__u.i;     \
	} while (0)

/* Get the more significant 32 bit int from a double.  */
#define GET_HIGH_WORD(hi, d)        \
	do {                            \
		union {                     \
			double f;               \
			uint64_t i;             \
		} __u;                      \
		__u.f = (d);                \
		(hi) = __u.i >> 32;         \
	} while (0)

/* Get the less significant 32 bit int from a double.  */
#define GET_LOW_WORD(lo, d)         \
	do {                            \
		union {                     \
			double f;               \
			uint64_t i;             \
		} __u;                      \
		__u.f = (d);                \
		(lo) = (uint32_t)__u.i;     \
	} while (0)

/* Set a double from two 32 bit ints.  */
#define INSERT_WORDS(d, hi, lo)                         \
	do {                                                \
		union {                                         \
			double f;                                   \
			uint64_t i;                                 \
		} __u;                                          \
		__u.i = ((uint64_t)(hi) << 32) | (uint32_t)(lo);\
		(d) = __u.f;                                    \
	} while (0)

/* Set the more significant 32 bits of a double from an int.  */
#define SET_HIGH_WORD(d, hi)                    \
	do {                                        \
		union {                                 \
			double f;                           \
			uint64_t i;                         \
		} __u;                                  \
		__u.f = (d);                            \
		__u.i &= 0xffffffff;                    \
		__u.i |= (uint64_t)(hi) << 32;          \
		(d) = __u.f;                            \
	} while (0)

/* Set the less significant 32 bits of a double from an int.  */
#define SET_LOW_WORD(d, lo)                     \
	do {                                        \
		union {                                 \
			double f;                           \
			uint64_t i;                         \
		} __u;                                  \
		__u.f = (d);                            \
		__u.i &= 0xffffffff00000000ull;         \
		__u.i |= (uint32_t)(lo);                \
		(d) = __u.f;                            \
	} while (0)

/* Get a 32 bit int from a float.  */
#define GET_FLOAT_WORD(w, d)                    \
	do {                                        \
		union {                                 \
			float f;                            \
			uint32_t i;                         \
		} __u;                                  \
		__u.f = (d);                            \
		(w) = __u.i;                            \
	} while (0)

/* Set a float from a 32 bit int.  */
#define SET_FLOAT_WORD(d, w)                    \
	do {                                        \
		union {                                 \
			float f;                            \
			uint32_t i;                         \
		} __u;                                  \
		__u.i = (w);                            \
		(d) = __u.f;                            \
	} while (0)

#undef __CMPLX
#undef CMPLX
#undef CMPLXF
#undef CMPLXL

#define __CMPLX(x, y, t)        \
	((union {                   \
		_Complex t __z;         \
		t __xy[2];              \
	}){ .__xy = {(x), (y)} } .__z)

#define CMPLX(x, y)  __CMPLX(x, y, double)
#define CMPLXF(x, y) __CMPLX(x, y, float)
#define CMPLXL(x, y) __CMPLX(x, y, long double)

#endif
