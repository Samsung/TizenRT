/*
 * Copyright (c) 2021 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_VASPRINTF_H
#define AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_VASPRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

int OsalVasprintf(char **ptr, const char *format, va_list ap, const char *name, int32_t ext);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_VASPRINTF_H
