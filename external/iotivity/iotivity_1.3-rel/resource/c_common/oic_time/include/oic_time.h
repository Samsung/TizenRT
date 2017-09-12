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

#ifndef OIC_TIME_H_
#define OIC_TIME_H_

#include <stdint.h>

#define MS_PER_SEC  (1000)
#define US_PER_SEC  (1000000)
#define US_PER_MS   (1000)
#define NS_PER_US   (1000)
#define NS_PER_MS   (1000000)
#define HNS_PER_US  (10)


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef enum
{
    TIME_IN_MS = 0,
    TIME_IN_US,
}OICTimePrecision;

/*
 * If monotonic coarse/monotonic clock supported then gets current time as monotonic time
 * in milliseconds or microseconds as the elapsed time since some unspecified starting point
 * else gets current time in milliseconds or microseconds as the elapsed time since the epoch.
 *
 * For Arduino gets current time in milliseconds or microseconds since Arduino board begin
 * running this program.
 *
 * @param     precision   based on this parameter, current time is returned in milliseconds or
 *                        microseconds
 *
 * @warning   This function may be sensitive to system time changes on some platforms.
 *
 * @note
 *            On Arduino platform:
 *            if the time precision is in milliseconds then the function will overflow
 *            (go back to 0) after approximately 50 days.
 *            if the time precision is in microseconds then the function will overflow
 *            (go back to 0) after approximately 70minutes.
 *
 * @return
 *         returns current time in milliseconds or microseconds.
 */
uint64_t OICGetCurrentTime(OICTimePrecision precision);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // OIC_TIME_H_
