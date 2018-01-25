/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
// -*- C++ -*-
//===------------------- support/musl/xlocale.h ------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This adds support for the extended locale functions that are currently
// missing from the Musl C library.
//
// This only works when the specified locale is "C" or "POSIX", but that's
// about as good as we can do without implementing full xlocale support
// in Musl.
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_SUPPORT_MUSL_XLOCALE_H
#define _LIBCPP_SUPPORT_MUSL_XLOCALE_H

#include <cstdlib>
#include <cwchar>

#ifdef __cplusplus
extern "C" {
#endif

static inline long long strtoll_l(const char *nptr, char **endptr, int base,
                                  locale_t) {
  return strtoll(nptr, endptr, base);
}

static inline unsigned long long strtoull_l(const char *nptr, char **endptr,
                                            int base, locale_t) {
  return strtoull(nptr, endptr, base);
}

static inline long long wcstoll_l(const wchar_t *nptr, wchar_t **endptr,
                                  int base, locale_t) {
  return wcstoll(nptr, endptr, base);
}

static inline unsigned long long wcstoull_l(const wchar_t *nptr,
                                            wchar_t **endptr, int base,
                                            locale_t) {
  return wcstoull(nptr, endptr, base);
}

static inline long double wcstold_l(const wchar_t *nptr, wchar_t **endptr,
                                    locale_t) {
  return wcstold(nptr, endptr);
}

#ifdef __cplusplus
}
#endif

#endif // _LIBCPP_SUPPORT_MUSL_XLOCALE_H
