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

#include <stdint.h>


#define SRC_MAX_RATIO   ((float)3)
#define SRC_MIN_RATIO   ((float)1 / SRC_MAX_RATIO)

#define MAX(a,b)        (((a) > (b)) ? (a) : (b))
#define MIN(a,b)        (((a) < (b)) ? (a) : (b))

// at least remain one frame (one sample for each channel)
#define OVERLAP_DEFAULT                 (1)

// calculate new sample data
#define CALC_NEW_SAMPLE(s1, s2, part)   ((s1) + ((((s2) - (s1)) * (int32_t)(part)) >> 16))

// fraction part bits
#define FRACBITS                        (16)

// convert to 16.16 fixed point value
#define TO_16_16_FIXED(x)               ((uint32_t)((float)(x) * (float)(1 << FRACBITS) + 0.5f))

// int part value: 16.0 fixed point
#define INTPART_VALUE(x)                ((x) >> FRACBITS)

// fraction part value: 0.16 fixed point
#define FRACPART_VALUE(x)               ((x) & 0xffff)

// convert sample width in bytes
#define BYTES_PER_SAMPLE(bits_per_sample)   ((bits_per_sample) >> 3)

// Round the given positive float number to the nearest integer
#define LRINTPF(pf)                     ((long)((float)(pf) + 0.5f))

// Max channel num supported for SRC
#define SRC_MAX_CH                      (2)

#endif	/* COMMON_H_INCLUDED */

