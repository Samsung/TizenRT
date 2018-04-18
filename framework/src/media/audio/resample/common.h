/******************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/*
** Copyright (c) 2002-2016, Erik de Castro Lopo <erikd@mega-nerd.com>
** All rights reserved.
**
** This code is released under 2-clause BSD license. Please see the
** file at : https://github.com/erikd/libsamplerate/blob/master/COPYING
*/

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#if 0
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif (SIZEOF_INT == 4)
typedef	int		int32_t ;
#elif (SIZEOF_LONG == 4)
typedef	long	int32_t ;
#endif
#endif
#define	SRC_MAX_RATIO			16
#define	SRC_MAX_RATIO_STR		"256"

#define	SRC_MIN_RATIO_DIFF		(1e-20)

#define	MAX(a,b)	(((a) > (b)) ? (a) : (b))
#define	MIN(a,b)	(((a) < (b)) ? (a) : (b))

#define	ARRAY_LEN(x)			((int) (sizeof (x) / sizeof ((x) [0])))
#define OFFSETOF(type,member)	((int) (&((type*) 0)->member))

#define	MAKE_MAGIC(a,b,c,d,e,f)	((a) + ((b) << 4) + ((c) << 8) + ((d) << 12) + ((e) << 16) + ((f) << 20))

/*
** Inspiration : http://sourcefrog.net/weblog/software/languages/C/unused.html
*/
#ifdef UNUSED
#elif defined (__GNUC__)
#	define UNUSED(x) UNUSED_ ## x __attribute__ ((unused))
#elif defined (__LCLINT__)
#	define UNUSED(x) /*@unused@*/ x
#else
#	define UNUSED(x) x
#endif

#ifdef __GNUC__
#	define WARN_UNUSED	__attribute__ ((warn_unused_result))
#else
#	define WARN_UNUSED
#endif

#include "samplerate.h"

enum {
	SRC_FALSE	= 0,
	SRC_TRUE	= 1,

	SRC_MODE_PROCESS	= 555,
	SRC_MODE_CALLBACK	= 556
} ;

enum {
	SRC_ERR_NO_ERROR = 0,

	SRC_ERR_MALLOC_FAILED,
	SRC_ERR_BAD_STATE,
	SRC_ERR_BAD_DATA,
	SRC_ERR_BAD_DATA_PTR,
	SRC_ERR_NO_PRIVATE,
	SRC_ERR_BAD_SRC_RATIO,
	SRC_ERR_BAD_PROC_PTR,
	SRC_ERR_SHIFT_BITS,
	SRC_ERR_FILTER_LEN,
	SRC_ERR_BAD_CONVERTER,
	SRC_ERR_BAD_CHANNEL_COUNT,
	SRC_ERR_SINC_BAD_BUFFER_LEN,
	SRC_ERR_SIZE_INCOMPATIBILITY,
	SRC_ERR_BAD_PRIV_PTR,
	SRC_ERR_BAD_SINC_STATE,
	SRC_ERR_DATA_OVERLAP,
	SRC_ERR_BAD_CALLBACK,
	SRC_ERR_BAD_MODE,
	SRC_ERR_NULL_CALLBACK,
	SRC_ERR_NO_VARIABLE_RATIO,
	SRC_ERR_SINC_PREPARE_DATA_BAD_LEN,
	SRC_ERR_BAD_INTERNAL_STATE,

	/* This must be the last error number. */
	SRC_ERR_MAX_ERROR
} ;
enum {
	SRC_TYPE_INVALID = 0,
	SRC_TYPE_NO_RESAMPLE,
	SRC_TYPE_DOWN_INT,
	SRC_TYPE_DOWN_FRAC,
	SRC_TYPE_UP_INT,
	SRC_TYPE_UP_FRAC,
} ;

static const int32_t filter_16khz_coeff[] = {
	2057290, -2973608, 1880478, 4362037,
	-14639744, 18523609, -1609189, -38502470,
	78073125, -68353935, -59103896, 617555440,
	617555440, -59103896, -68353935, 78073125,
	-38502470, -1609189, 18523609, -14639744,
	4362037, 1880478, -2973608, 2057290,
};
static const int32_t filter_22khz_coeff[] = {
	2089257, 2898328, -5820678, -10484531,
	19038724, 30542725, -50469415, -81505260,
	152544464, 478517512, 478517512, 152544464,
	-81505260, -50469415, 30542725, 19038724,
	-10484531, -5820678, 2898328, 2089257,
};

#define NUM_COEFF_22KHZ (sizeof(filter_22khz_coeff) / sizeof(filter_22khz_coeff[0]))
#define OVERLAP_22KHZ (NUM_COEFF_22KHZ - 2)


#define NUM_COEFF_16KHZ (sizeof(filter_16khz_coeff) / sizeof(filter_16khz_coeff[0]))
#define OVERLAP_16KHZ (NUM_COEFF_16KHZ - 1)

#endif	/* COMMON_H_INCLUDED */

