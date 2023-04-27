/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef __PLATFORM_STDLIB_H__
#define __PLATFORM_STDLIB_H__

#if defined (CONFIG_PLATFORM_8721D)
#include "platform_stdlib_rtl8721d.h"
#elif defined(CONFIG_PLATFORM_8735B)
#include "platform_stdlib_rtl8735b.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> /* va_list */
#include "ameba.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_PLATFORM_AMEBA_X) && (CONFIG_PLATFORM_AMEBA_X == 0)
#include "basic_types.h"
#endif


#ifdef __cplusplus
}
#endif

#endif //__PLATFORM_STDLIB_H__

