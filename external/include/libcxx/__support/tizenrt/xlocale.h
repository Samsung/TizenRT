//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// TizenRT locale support - provides stubs for missing POSIX extended locale functions

#ifndef _LIBCPP___SUPPORT_TIZENRT_XLOCALE_H
#define _LIBCPP___SUPPORT_TIZENRT_XLOCALE_H

#include <__config>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// locale_t - TizenRT defines this in os/include/locale.h
// Define if not already available
#ifndef _LOCALE_T_DEFINED
typedef void* locale_t;
#define _LOCALE_T_DEFINED
#endif

// LC_*_MASK definitions (not provided by TizenRT's C library)
#ifndef LC_COLLATE_MASK
#define LC_COLLATE_MASK  (1 << 0)
#endif
#ifndef LC_CTYPE_MASK
#define LC_CTYPE_MASK    (1 << 1)
#endif
#ifndef LC_MONETARY_MASK
#define LC_MONETARY_MASK (1 << 2)
#endif
#ifndef LC_NUMERIC_MASK
#define LC_NUMERIC_MASK  (1 << 3)
#endif
#ifndef LC_TIME_MASK
#define LC_TIME_MASK     (1 << 4)
#endif
#ifndef LC_MESSAGES_MASK
#define LC_MESSAGES_MASK (1 << 5)
#endif
#ifndef LC_ALL_MASK
#define LC_ALL_MASK                                                                      \
  (LC_COLLATE_MASK | LC_CTYPE_MASK | LC_MONETARY_MASK | LC_NUMERIC_MASK | LC_TIME_MASK | \
   LC_MESSAGES_MASK)
#endif

// Locale management stubs - TizenRT only supports "C" locale
inline _LIBCPP_HIDE_FROM_ABI_C locale_t newlocale(int, const char*, locale_t) { return nullptr; }
inline _LIBCPP_HIDE_FROM_ABI_C void freelocale(locale_t) {}
inline _LIBCPP_HIDE_FROM_ABI_C locale_t uselocale(locale_t) { return nullptr; }
inline _LIBCPP_HIDE_FROM_ABI_C locale_t duplocale(locale_t) { return nullptr; }

// Character classification *_l functions
inline _LIBCPP_HIDE_FROM_ABI_C int isalnum_l(int __c, locale_t) { return isalnum(__c); }
inline _LIBCPP_HIDE_FROM_ABI_C int isalpha_l(int __c, locale_t) { return isalpha(__c); }
inline _LIBCPP_HIDE_FROM_ABI_C int isblank_l(int __c, locale_t) { return isblank(__c); }
inline _LIBCPP_HIDE_FROM_ABI_C int iscntrl_l(int __c, locale_t) { return iscntrl(__c); }
inline _LIBCPP_HIDE_FROM_ABI_C int isdigit_l(int __c, locale_t) { return isdigit(__c); }
inline _LIBCPP_HIDE_FROM_ABI_C int isgraph_l(int __c, locale_t) { return isgraph(__c); }
inline _LIBCPP_HIDE_FROM_ABI_C int islower_l(int __c, locale_t) { return islower(__c); }
inline _LIBCPP_HIDE_FROM_ABI_C int isprint_l(int __c, locale_t) { return isprint(__c); }
inline _LIBCPP_HIDE_FROM_ABI_C int ispunct_l(int __c, locale_t) { return ispunct(__c); }
inline _LIBCPP_HIDE_FROM_ABI_C int isspace_l(int __c, locale_t) { return isspace(__c); }
inline _LIBCPP_HIDE_FROM_ABI_C int isupper_l(int __c, locale_t) { return isupper(__c); }
inline _LIBCPP_HIDE_FROM_ABI_C int isxdigit_l(int __c, locale_t) { return isxdigit(__c); }

// Character conversion *_l functions
inline _LIBCPP_HIDE_FROM_ABI_C int tolower_l(int __c, locale_t) { return tolower(__c); }
inline _LIBCPP_HIDE_FROM_ABI_C int toupper_l(int __c, locale_t) { return toupper(__c); }

// String conversion *_l functions
inline _LIBCPP_HIDE_FROM_ABI_C float strtof_l(const char* __nptr, char** __endptr, locale_t) {
  return strtof(__nptr, __endptr);
}

inline _LIBCPP_HIDE_FROM_ABI_C double strtod_l(const char* __nptr, char** __endptr, locale_t) {
  return strtod(__nptr, __endptr);
}

inline _LIBCPP_HIDE_FROM_ABI_C long double strtold_l(const char* __nptr, char** __endptr, locale_t) {
  return strtold(__nptr, __endptr);
}

inline _LIBCPP_HIDE_FROM_ABI_C long long strtoll_l(const char* __nptr, char** __endptr, int __base, locale_t) {
  return strtoll(__nptr, __endptr, __base);
}

inline _LIBCPP_HIDE_FROM_ABI_C unsigned long long
strtoull_l(const char* __nptr, char** __endptr, int __base, locale_t) {
  return strtoull(__nptr, __endptr, __base);
}

// String collation *_l functions
inline _LIBCPP_HIDE_FROM_ABI_C int strcoll_l(const char* __s1, const char* __s2, locale_t) {
  return strcoll(__s1, __s2);
}

inline _LIBCPP_HIDE_FROM_ABI_C size_t strxfrm_l(char* __dest, const char* __src, size_t __n, locale_t) {
  return strxfrm(__dest, __src, __n);
}

// Time formatting *_l function
inline _LIBCPP_HIDE_FROM_ABI_C size_t
strftime_l(char* __s, size_t __max, const char* __format, const struct tm* __tm, locale_t) {
  return strftime(__s, __max, __format, __tm);
}

#ifdef __cplusplus
}
#endif

#endif // _LIBCPP___SUPPORT_TIZENRT_XLOCALE_H