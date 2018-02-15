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
//===-------------- support/xlocale/__strtonum_fallback.h -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// These are reimplementations of some extended locale functions ( *_l ) that
// aren't part of POSIX.  They are widely available though (GLIBC, BSD, maybe
// others).  The unifying aspect in this case is that all of these functions
// convert strings to some numeric type.
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_SUPPORT_XLOCALE_STRTONUM_FALLBACK_H
#define _LIBCPP_SUPPORT_XLOCALE_STRTONUM_FALLBACK_H

#ifdef __cplusplus
extern "C" {
#endif

inline _LIBCPP_ALWAYS_INLINE float strtof_l(const char *nptr,
                                            char **endptr, locale_t) {
  return ::strtof(nptr, endptr);
}

inline _LIBCPP_ALWAYS_INLINE double strtod_l(const char *nptr,
                                                   char **endptr, locale_t) {
  return ::strtod(nptr, endptr);
}

inline _LIBCPP_ALWAYS_INLINE long double strtold_l(const char *nptr,
                                                   char **endptr, locale_t) {
  return ::strtold(nptr, endptr);
}

inline _LIBCPP_ALWAYS_INLINE long long
strtoll_l(const char *nptr, char **endptr, int base, locale_t) {
  return ::strtoll(nptr, endptr, base);
}

inline _LIBCPP_ALWAYS_INLINE unsigned long long
strtoull_l(const char *nptr, char **endptr, int base, locale_t) {
  return ::strtoull(nptr, endptr, base);
}

inline _LIBCPP_ALWAYS_INLINE long long
wcstoll_l(const wchar_t *nptr, wchar_t **endptr, int base, locale_t) {
  return ::wcstoll(nptr, endptr, base);
}

inline _LIBCPP_ALWAYS_INLINE unsigned long long
wcstoull_l(const wchar_t *nptr, wchar_t **endptr, int base, locale_t) {
  return ::wcstoull(nptr, endptr, base);
}

inline _LIBCPP_ALWAYS_INLINE long double wcstold_l(const wchar_t *nptr,
                                                   wchar_t **endptr, locale_t) {
  return ::wcstold(nptr, endptr);
}

#ifdef __cplusplus
}
#endif

#endif // _LIBCPP_SUPPORT_XLOCALE_STRTONUM_FALLBACK_H
