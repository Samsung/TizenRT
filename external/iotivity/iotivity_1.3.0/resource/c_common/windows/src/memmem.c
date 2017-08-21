/* *****************************************************************
*
* Copyright 2016 Microsoft
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

#include <stddef.h>
#include <memory.h>

void *memmem(const void *haystack, size_t haystackLen, const void *needle, size_t needleLen)
{
    if (needleLen == 0)
    {
        return (void *) haystack;
    }

    if (haystackLen >= needleLen)
    {
        for (size_t i = 0; i <= haystackLen - needleLen; i++)
        {
            if (memcmp(needle, ((char *) haystack) + i, needleLen) == 0)
            {
                return ((char *) haystack) + i;
            }
        }
    }
    return NULL;
}
