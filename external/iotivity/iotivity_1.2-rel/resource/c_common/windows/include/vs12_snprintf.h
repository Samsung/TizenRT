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

#ifndef VS12_SNPRINTF_H__
#define VS12_SNPRINTF_H__

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

int vs12_snprintf(char *buffer, size_t count, const char *format, ...);

#if defined(_MSC_VER) && (_MSC_VER < 1900)
# define snprintf vs12_snprintf
#endif

#ifdef __cplusplus
}
#endif

#endif

