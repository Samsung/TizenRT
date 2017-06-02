/* *****************************************************************
*
* Copyright 2016 Intel Corporation
*
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

#include <assert.h>
#include <windows.h>
#include <errno.h>
#include "win_sleep.h"

#include "logger.h"
#define TAG  "WIN_SLEEP"

#define NS_PER_MS  (1000000)
#define US_PER_MS  (1000)

#define ASSERT_AND_FAIL(EXPRESSION) {    \
    assert((EXPRESSION));                \
    if (!(EXPRESSION))                   \
    {                                    \
        errno = EINVAL;                  \
        return -1;                       \
    }                                    \
}

int usleep(unsigned int usec)
{
    assert((usec % US_PER_MS) == 0); // granularity check
    Sleep((usec + US_PER_MS - 1) / US_PER_MS);
    return 0;
}

int nanosleep(const struct timespec *req, struct timespec *rem)
{
    if (req == NULL)
    {
        OIC_LOG(DEBUG, TAG, "nanosleep: req param should not be null");
    }
    ASSERT_AND_FAIL(req != NULL);
    ASSERT_AND_FAIL(rem == NULL);
    ASSERT_AND_FAIL(req->tv_sec == 0);
    assert((req->tv_nsec % NS_PER_MS) == 0); // granularity check

    Sleep((req->tv_nsec + NS_PER_MS - 1) / NS_PER_MS);

    return 0;
}

