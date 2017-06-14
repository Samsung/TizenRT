//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Defining _POSIX_C_SOURCE macro with 199309L (or greater) as value
// causes header files to expose definitions
// corresponding to the POSIX.1b, Real-time extensions
// (IEEE Std 1003.1b-1993) specification
//
// For this specific file, see use of clock_gettime,
// Refer to http://pubs.opengroup.org/stage7tc1/functions/clock_gettime.html
// and to http://man7.org/linux/man-pages/man2/clock_gettime.2.html
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include "iotivity_config.h"
#include "oic_time.h"

#include <stddef.h>        // For NULL

#if defined(HAVE_WINDOWS_H)
# include <windows.h>
# define HAVE_QUERYPERFORMANCEFREQUENCY
#elif !defined(WITH_ARDUINO)
# if _POSIX_TIMERS > 0
#  include <time.h>        // For clock_gettime()
# else
#  include <sys/time.h>    // For gettimeofday()
# endif  // _POSIX_TIMERS > 0
#endif

#define TAG "OIC_TIME"

uint64_t OICGetCurrentTime(OICTimePrecision precision)
{
    uint64_t currentTime = 0;

#ifdef WITH_ARDUINO
    currentTime = (TIME_IN_MS == precision) ? millis() : micros();
#elif defined(HAVE_QUERYPERFORMANCEFREQUENCY)
    static LARGE_INTEGER frequency = {0};

    if (!frequency.QuadPart)
    {
        QueryPerformanceFrequency(&frequency);
    }

    LARGE_INTEGER count = {0};
    QueryPerformanceCounter(&count);

    currentTime =
    (TIME_IN_MS == precision)
        ? count.QuadPart / (frequency.QuadPart / MS_PER_SEC)
        : count.QuadPart / (frequency.QuadPart / US_PER_SEC);
#else
# if _POSIX_TIMERS > 0
#   if defined(CLOCK_MONOTONIC_COARSE)
    static const clockid_t clockId = CLOCK_MONOTONIC_COARSE;
#   elif _POSIX_MONOTONIC_CLOCK >= 0
    // Option _POSIX_MONOTONIC_CLOCK == 0 indicates that the option is
    // available at compile time but may not be supported at run
    // time.  Check if option _POSIX_MONOTONIC_CLOCK is supported at
    // run time.
#     if _POSIX_MONOTONIC_CLOCK == 0
    static const clockid_t clockId =
        sysconf(_SC_MONOTONIC_CLOCK) > 0 ? CLOCK_MONOTONIC : CLOCK_REALTIME;
#     else
    static const clockid_t clockId = CLOCK_MONOTONIC;
#     endif  // _POSIX_MONOTONIC_CLOCK == 0
#   else
    static const clockid_t clockId = CLOCK_REALTIME;
#   endif  // CLOCK_MONOTONIC_COARSE

    struct timespec current = { .tv_sec = 0, .tv_nsec = 0 };
    if (clock_gettime(clockId, &current) == 0)
    {
        currentTime =
            (TIME_IN_MS == precision)
            ? (((uint64_t) current.tv_sec * MS_PER_SEC) + (current.tv_nsec / NS_PER_MS))
            : (((uint64_t) current.tv_sec * US_PER_SEC) + (current.tv_nsec / NS_PER_US));
    }
# else
    struct timeval current = { .tv_sec = 0, .tv_usec = 0 };
    if (gettimeofday(&current, NULL) == 0)
    {
        currentTime =
            (TIME_IN_MS == precision)
            ? (((uint64_t) current.tv_sec * MS_PER_SEC) + (current.tv_usec / US_PER_MS))
            : (((uint64_t) current.tv_sec * US_PER_SEC) + (current.tv_usec));
    }
# endif  // _POSIX_TIMERS > 0
#endif  // WITH_ARDUINO

    return currentTime;
}
