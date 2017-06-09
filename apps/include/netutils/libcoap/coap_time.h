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
 * Copyright (c) 2010--2011, Olaf Bergmann
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  o Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *  o Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 ****************************************************************************/
/* coap_time.h -- Clock Handling
 *
 * Copyright (C) 2010--2013 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

/**
 * @file coap_time.h
 * @brief Clock Handling
 */

#ifndef _COAP_TIME_H_
#define _COAP_TIME_H_

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

/**
 * @defgroup clock Clock Handling
 * Default implementation of internal clock. You should redefine this if
 * you do not have time() and gettimeofday().
 * @{
 */

#ifdef WITH_LWIP

#include <stdint.h>
#include <net/lwip/sys.h>

/* lwIP provides ms in sys_now */
#define COAP_TICKS_PER_SECOND 1000

typedef uint32_t coap_tick_t;

static inline void coap_ticks_impl(coap_tick_t *t)
{
	*t = sys_now();
}
static inline void coap_clock_init_impl(void)
{
}

#define coap_clock_init coap_clock_init_impl

#define coap_ticks coap_ticks_impl

#endif
#ifdef WITH_CONTIKI
#include "clock.h"

typedef clock_time_t coap_tick_t;

/**
 * This data type is used to represent the difference between two
 * clock_tick_t values. This data type must have the same size in
 * memory as coap_tick_t to allow wrapping.
 */
typedef int coap_tick_diff_t;

#define COAP_TICKS_PER_SECOND CLOCK_SECOND

/** Set at startup to initialize the internal clock (time in seconds). */
extern clock_time_t clock_offset;

static inline void
contiki_clock_init_impl(void)
{
	clock_init();
	clock_offset = clock_time();
}

#define coap_clock_init contiki_clock_init_impl

static inline void
contiki_ticks_impl(coap_tick_t *t)
{
	*t = clock_time();
}

#define coap_ticks contiki_ticks_impl

#endif							/* WITH_CONTIKI */
#ifdef WITH_POSIX
typedef unsigned int coap_tick_t;

/**
 * This data type is used to represent the difference between two
 * clock_tick_t values. This data type must have the same size in
 * memory as coap_tick_t to allow wrapping.
 */
typedef int coap_tick_diff_t;

#define COAP_TICKS_PER_SECOND 1024

/** Set at startup to initialize the internal clock (time in seconds). */
extern time_t clock_offset;
#endif							/* WITH_POSIX */

#ifndef coap_clock_init
static inline void
coap_clock_init_impl(void)
{
#ifdef HAVE_TIME_H
	clock_offset = time(NULL);
#else
#ifdef __GNUC__
	/* Issue a warning when using gcc. Other prepropressors do
	 *  not seem to have a similar feature. */
#warning "cannot initialize clock"
#endif
	clock_offset = 0;
#endif
}
#define coap_clock_init coap_clock_init_impl
#endif							/* coap_clock_init */

#ifndef coap_ticks
static inline void
coap_ticks_impl(coap_tick_t *t)
{
#ifdef HAVE_SYS_TIME_H
	struct timeval tv;
	gettimeofday(&tv, NULL);
	*t = (tv.tv_sec - clock_offset) * COAP_TICKS_PER_SECOND + (tv.tv_usec * COAP_TICKS_PER_SECOND / 1000000);
#else
#error "clock not implemented"
#endif
}
#define coap_ticks coap_ticks_impl
#endif							/* coap_ticks */

/**
 * Returns @c 1 if and only if @p a is less than @p b where less is
 * defined on a signed data type.
 */
static inline int coap_time_lt(coap_tick_t a, coap_tick_t b)
{
	return ((coap_tick_diff_t)(a - b)) < 0;
}

/**
 * Returns @c 1 if and only if @p a is less than or equal @p b where
 * less is defined on a signed data type.
 */
static inline int coap_time_le(coap_tick_t a, coap_tick_t b)
{
	return a == b || coap_time_lt(a, b);
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif							/* _COAP_TIME_H_ */
