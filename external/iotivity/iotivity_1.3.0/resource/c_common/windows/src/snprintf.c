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

#include "vs12_snprintf.h"
#include <stdio.h>
#include <stdarg.h>

int vs12_snprintf(char *buffer, size_t count, const char *format, ...)
{
    // The first three values are invalid when calling _vsnprintf.
    // Last check is to make sure the buffer is small enough for the
    // return value to make sense.
    if ((buffer == NULL) ||
        (count == 0) ||
        (format == NULL) ||
        (count > INT_MAX))
    {
        return -1;
    }

    // _vsnprintf behaves similarly to snprintf, but:
    //
    // - It doesn't always add a zero terminator to the output buffer
    // - When the output buffer is too small, it returns -1 instead of the
    //   required buffer size
    va_list args;
    va_start(args, format);
    int length = _vsnprintf(buffer, count, format, args);
    va_end(args);

    if ((length == -1) || (length == (int)count))
    {
        // Add the missing zero character terminator.
        buffer[count - 1] = '\0';

        // When length == -1, it would be good to compute the required output
        // buffer size, and return that computed value instead of -1. That would
        // bring the behavior of vs12_snprintf closer to the C99 version of
        // snprintf. However, IoTivity doesn't have an easy way to compute the
        // required output buffer size. Also, there are no known callers of
        // snprintf in IoTivity that are relying on a compliant return value,
        // when that compliant return value would be larger than the 'count'
        // input parameter.
        //
        // @todo Compute and return the output buffer size, instead of -1,
        //       when the output buffer was too small.
    }

    return length;
}
