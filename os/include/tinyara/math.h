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
/****************************************************************************
 *
 *   Copyright (C) 2009, 2012, 2014-2015 Gregory Nutt. All rights reserved.
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
/**
 * @defgroup MATH_LIBC MATH
 * @ingroup KERNEL
 * @{
 */

///@file tinyara/math.h
///@brief Math APIs
#ifndef __INCLUDE_MATH_H
#define __INCLUDE_MATH_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

/* If CONFIG_ARCH_MATH_H is defined, then the top-level Makefile will copy
 * this header file to include/math.h where it will become the system math.h
 * header file.  In this case, the architecture specific code must provide
 * an arch/<architecture>/include/math.h file which will be included below:
 */

#ifdef CONFIG_ARCH_MATH_H
#include <arch/math.h>

/* If CONFIG_LIBM is enabled, then the math library at lib/math will be
 * built.  This library was taken from the math library developed for the
 * Rhombus OS by Nick Johnson (https://github.com/nickbjohnson4224/rhombus).
 * The port or the Rhombus math library was contributed by Darcy Gong.
 */

#elif defined(CONFIG_LIBM)

/****************************************************************************
 * Copyright (C) 2009-2011 Nick Johnson <nickbjohnson4224 at gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* General Constants ********************************************************/

#define INFINITY    (1.0/0.0)
#define NAN         (0.0/0.0)
#define HUGE_VAL    INFINITY

#define isnan(x)    ((x) != (x))
#define isinf(x)    (((x) == INFINITY) || ((x) == -INFINITY))
#define isfinite(x) (!(isinf(x)) && (x != NAN))

/* Exponential and Logarithmic constants ************************************/

#define M_E        2.7182818284590452353602874713526625
#define M_SQRT2    1.4142135623730950488016887242096981
#define M_SQRT1_2  0.7071067811865475244008443621048490
#define M_LOG2E    1.4426950408889634073599246810018921
#define M_LOG10E   0.4342944819032518276511289189166051
#define M_LN2      0.6931471805599453094172321214581765
#define M_LN10     2.3025850929940456840179914546843642

/* Trigonometric Constants **************************************************/

#define M_PI       3.1415926535897932384626433832795029
#define M_PI_2     1.5707963267948966192313216916397514
#define M_PI_4     0.7853981633974483096156608458198757
#define M_1_PI     0.3183098861837906715377675267450287
#define M_2_PI     0.6366197723675813430755350534900574
#define M_2_SQRTPI 1.1283791670955125738961589031215452

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/* General Functions ******************************************************* */
/**
 * @cond
 * @internal
 */
float ceilf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double ceil(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double ceill(long double x);
#endif
/**
 * @internal
 */
float floorf(float x);
#ifdef CONFIG_HAVE_DOUBLE
/**
 * @endcond
 */
/**
 * @ingroup MATH_LIBC
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
double floor(double x);
#endif
/**
 * @cond
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double floorl(long double x);
#endif
/**
 * @internal
 */
float roundf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double round(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double roundl(long double x);
#endif
/**
 * @internal
 */
float rintf(float x);		/* Not implemented */
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double rint(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double rintl(long double x);	/* Not implemented */
#endif
/**
 * @internal
 */
float fabsf(float x);
/**
 * @endcond
 */
#ifdef CONFIG_HAVE_DOUBLE
/**
 * @ingroup MATH_LIBC
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
double fabs(double x);
#endif
/**
 * @cond
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double fabsl(long double x);
#endif
/**
 * @internal
 */
float modff(float x, float *iptr);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double modf(double x, double *iptr);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double modfl(long double x, long double *iptr);
#endif
/**
 * @internal
 */
float fmodf(float x, float div);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double fmod(double x, double div);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double fmodl(long double x, long double div);
#endif

/* Exponential and Logarithmic Functions *********************************** */
/**
 * @internal
 */
float powf(float b, float e);
/**
 * @endcond
 */
#ifdef CONFIG_HAVE_DOUBLE
/**
 * @ingroup MATH_LIBC
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
double pow(double b, double e);
#endif
/**
 * @cond
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double powl(long double b, long double e);
#endif
/**
 * @internal
 */
float expf(float x);
/**
 * @internal
 */
#define expm1f(x) (expf(x) - 1.0)
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double exp(double x);
#define expm1(x) (exp(x) - 1.0)
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double expl(long double x);
#define expm1l(x) (expl(x) - 1.0)
#endif
/**
 * @internal
 */
float logf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double log(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double logl(long double x);
#endif
/**
 * @internal
 */
float log10f(float x);
/**
 * @internal
 */
#if CONFIG_HAVE_DOUBLE
double log10(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double log10l(long double x);
#endif
/**
 * @internal
 */
float log2f(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double log2(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double log2l(long double x);
#endif
/**
 * @internal
 */
float sqrtf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double sqrt(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double sqrtl(long double x);
#endif
/**
 * @internal
 */
float ldexpf(float x, int n);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double ldexp(double x, int n);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double ldexpl(long double x, int n);
#endif
/**
 * @internal
 */
float frexpf(float x, int *exp);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double frexp(double x, int *exp);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double frexpl(long double x, int *exp);
#endif

/* Trigonometric Functions ************************************************* */
/**
 * @internal
 */
float sinf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double sin(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double sinl(long double x);
#endif
/**
 * @internal
 */
float cosf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double cos(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double cosl(long double x);
#endif
/**
 * @internal
 */
float tanf(float x);
/**
 * @internal
 */
#if CONFIG_HAVE_DOUBLE
double tan(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double tanl(long double x);
#endif
/**
 * @internal
 */
float asinf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double asin(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double asinl(long double x);
#endif
/**
 * @internal
 */
float acosf(float x);
/**
 * @internal
 */
#if CONFIG_HAVE_DOUBLE
double acos(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double acosl(long double x);
#endif
/**
 * @internal
 */
float atanf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double atan(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double atanl(long double x);
#endif
/**
 * @internal
 */
float atan2f(float y, float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double atan2(double y, double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double atan2l(long double y, long double x);
#endif
/**
 * @internal
 */
float sinhf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double sinh(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double sinhl(long double x);
#endif
/**
 * @internal
 */
float coshf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double cosh(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double coshl(long double x);
#endif
/**
 * @internal
 */
float tanhf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double tanh(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double tanhl(long double x);
#endif
/**
 * @internal
 */
float asinhf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double asinh(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double asinhl(long double x);
#endif
/**
 * @internal
 */
float acoshf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double acosh(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double acoshl(long double x);
#endif
/**
 * @internal
 */
float atanhf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double atanh(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double atanhl(long double x);
#endif
/**
 * @internal
 */
float erff(float x);
/**
 * @internal
 */
#define     erfcf(x) (1 - erff(x))
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double erf(double x);
#define     erfc(x) (1 - erf(x))
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double erfl(long double x);
#define     erfcl(x) (1 - erfl(x))
#endif
/**
 * @internal
 */
float copysignf(float x, float y);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double copysign(double x, double y);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double copysignl(long double x, long double y);
#endif
/**
 * @internal
 */
float truncf(float x);
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
double trunc(double x);
#endif
/**
 * @internal
 */
#ifdef CONFIG_HAVE_LONG_DOUBLE
long double truncl(long double x);
#endif
/**
 * @internal
 */
#define nanf(x) ((float)(NAN))
/**
 * @internal
 */
#ifdef CONFIG_HAVE_DOUBLE
#define nan(x) ((double)(NAN))
#endif
#ifdef CONFIG_HAVE_LONG_DOUBLE
#define nanl(x) ((long double)(NAN))
#endif

/**
 * @endcond
 */
#if defined(__cplusplus)
}
#endif
#endif							/* CONFIG_LIBM */
#endif							/* __INCLUDE_MATH_H */
/**
 *@} */
