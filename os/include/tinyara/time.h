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
 *   Copyright (C) 2009, 2011 Gregory Nutt. All rights reserved.
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
///@file tinyara/time.h
///@brief Time APIs

#ifndef __INCLUDE_TINYARA_TIME_H
#define __INCLUDE_TINYARA_TIME_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdbool.h>
#include <time.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* If Gregorian time is not supported, then neither is Julian */

#ifndef CONFIG_GREGORIAN_TIME
#undef CONFIG_JULIAN_TIME
#else
#define JD_OF_EPOCH           2440588	/* Julian Date of noon, J1970 */

#ifdef CONFIG_JULIAN_TIME
#define GREG_DUTC           -141427	/* Default is October 15, 1582 */
#define GREG_YEAR            1582
#define GREG_MONTH           10
#define GREG_DAY             15
#endif							/* CONFIG_JULIAN_TIME */
#endif							/* !CONFIG_GREGORIAN_TIME */

#define EPOCH_YEAR           1970       /* EPOCH Base Year */

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/**
 * @ingroup TIME_KERNEL
 * @brief Return true if the specified year is a leap year
 * @details @b #include <tinyara/time.h>
 * @param[in] year year to check whether a leap year or not
 * @return if leap year, TRUE or FALSE
 * @since TizenRT v1.0
 */
EXTERN int clock_isleapyear(int year);

/**
 * @ingroup TIME_KERNEL
 * @brief Get the number of days that occurred before the beginning of the month.
 * @details @b #include <tinyara/time.h>
 * @param[in] month the beginning of the month
 * @param[in] leapyear leap year
 * @return the number of days
 * @since TizenRT v1.0
 */
EXTERN int clock_daysbeforemonth(int month, bool leapyear);

/**
 * @cond
 * @internal
 */
/**
 * @ingroup TIME_KERNEL
 * @brief Get the day of the week
 * @details @b #include <tinyara/time.h>
 * @param[in] mday The day of the month 1 - 31
 * @param[in] month The month of the year 1 - 12
 * @param[in] year the year including the 1900
 * @return Zero based day of the week 0-6, 0 = Sunday, 1 = Monday... 6 = Saturday
 * @since TizenRT v1.0
 */
#if defined(CONFIG_TIME_EXTENDED)
int clock_dayoftheweek(int mday, int month, int year);
#endif
/**
 * @endcond
 */

/**
 * @ingroup TIME_KERNEL
 * @brief Conversion Calendar/UTC
 * @details @b #include <tinyara/time.h> \n
 *    based on algorithms from p. 604
 *    of Seidelman, P. K. 1992.  Explanatory Supplement to
 *    the Astronomical Almanac.  University Science Books,
 *    Mill Valley.
 * @param[in] year year to change
 * @param[in] month month to change
 * @param[in] day day to change
 * @return the specific time
 * @since TizenRT v1.0
 */
EXTERN time_t clock_calendar2utc(int year, int month, int day);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_TINYARA_TIME_H */
