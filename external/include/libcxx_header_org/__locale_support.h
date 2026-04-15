//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___LOCALE_SUPPORT_H
#define _LIBCPP___LOCALE_SUPPORT_H

#include <__config>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

// TizenRT compatibility: Provide stub implementations for missing locale functions
// These are needed for locale support but not provided by TizenRT's C library

#ifdef __cplusplus
extern "C" {
#endif

// Define locale_t as void* if not already defined
#ifndef _LOCALE_T_DEFINED
#define _LOCALE_T_DEFINED
typedef void* locale_t;
#endif

// Define LC_*_MASK constants if not already defined
#ifndef LC_COLLATE_MASK
#define LC_COLLATE_MASK (1<<0)
#endif
#ifndef LC_CTYPE_MASK
#define LC_CTYPE_MASK (1<<1)
#endif
#ifndef LC_MONETARY_MASK
#define LC_MONETARY_MASK (1<<2)
#endif
#ifndef LC_NUMERIC_MASK
#define LC_NUMERIC_MASK (1<<3)
#endif
#ifndef LC_TIME_MASK
#define LC_TIME_MASK (1<<4)
#endif
#ifndef LC_MESSAGES_MASK
#define LC_MESSAGES_MASK (1<<5)
#endif

// Stub implementation for uselocale - returns NULL (no previous locale)
// In embedded systems, we don't support thread-local locales
static inline locale_t uselocale(locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return (locale_t)0; // Return NULL - no previous locale to restore
}

// Stub implementations for locale-specific string conversion functions
// These call the standard versions and ignore the locale parameter
// In embedded systems, we use the global locale (which is the only locale)

static inline long long strtoll_l(const char* __nptr, char** __endptr, int __base, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return strtoll(__nptr, __endptr, __base);
}

static inline unsigned long long strtoull_l(const char* __nptr, char** __endptr, int __base, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return strtoull(__nptr, __endptr, __base);
}

static inline long double strtold_l(const char* __nptr, char** __endptr, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return strtold(__nptr, __endptr);
}

static inline float strtof_l(const char* __nptr, char** __endptr, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return strtof(__nptr, __endptr);
}

static inline double strtod_l(const char* __nptr, char** __endptr, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return strtod(__nptr, __endptr);
}

#ifndef _LIBCPP_HAS_NO_WIDE_CHARACTERS
static inline long long wcstoll_l(const wchar_t* __nptr, wchar_t** __endptr, int __base, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return wcstoll(__nptr, __endptr, __base);
}

static inline unsigned long long wcstoull_l(const wchar_t* __nptr, wchar_t** __endptr, int __base, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return wcstoull(__nptr, __endptr, __base);
}

static inline long double wcstold_l(const wchar_t* __nptr, wchar_t** __endptr, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return wcstold(__nptr, __endptr);
}

static inline float wcstof_l(const wchar_t* __nptr, wchar_t** __endptr, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return wcstof(__nptr, __endptr);
}

static inline double wcstod_l(const wchar_t* __nptr, wchar_t** __endptr, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return wcstod(__nptr, __endptr);
}
#endif // _LIBCPP_HAS_NO_WIDE_CHARACTERS

// Stub implementation for newlocale - returns NULL (global locale only)
static inline locale_t newlocale(int __category_mask, const char* __locale, locale_t __base) {
    (void)__category_mask; // Suppress unused parameter warnings
    (void)__locale;
    (void)__base;
    return (locale_t)0; // Return NULL - only global locale supported
}

// Stub implementation for freelocale - no-op
static inline void freelocale(locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    // No-op - nothing to free in embedded systems
}

// Stub implementation for duplocale - returns NULL
static inline locale_t duplocale(locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return (locale_t)0; // Return NULL - can't duplicate in embedded systems
}

// Stub implementations for locale-specific character classification functions
// These call the standard versions and ignore the locale parameter
static inline int isxdigit_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return isxdigit(__c);
}

static inline int isdigit_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return isdigit(__c);
}

static inline int isalpha_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return isalpha(__c);
}

static inline int isalnum_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return isalnum(__c);
}

static inline int isblank_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return isblank(__c);
}

static inline int iscntrl_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return iscntrl(__c);
}

static inline int isgraph_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return isgraph(__c);
}

static inline int islower_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return islower(__c);
}

static inline int isprint_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return isprint(__c);
}

static inline int ispunct_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return ispunct(__c);
}

static inline int isspace_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return isspace(__c);
}

static inline int isupper_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return isupper(__c);
}

// Stub implementations for locale-specific character conversion functions
static inline int tolower_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return tolower(__c);
}

static inline int toupper_l(int __c, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return toupper(__c);
}

// Add missing locale-specific string functions needed by locale.cpp
static inline int strcoll_l(const char* __s1, const char* __s2, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return strcoll(__s1, __s2);
}

static inline size_t strxfrm_l(char* __dest, const char* __src, size_t __n, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return strxfrm(__dest, __src, __n);
}

static inline size_t strftime_l(char* __s, size_t __max, const char* __format, const struct tm* __tm, locale_t __loc) {
    (void)__loc; // Suppress unused parameter warning
    return strftime(__s, __max, __format, __tm);
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _LIBCPP___LOCALE_SUPPORT_H
