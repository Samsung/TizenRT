// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___MBSTATE_T_H
#define _LIBCPP___MBSTATE_T_H

#include <__config>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

// The goal of this header is to provide mbstate_t without requiring all of
// <uchar.h> or <wchar.h>. It's also used by the libc++ versions of <uchar.h>
// and <wchar.h> to get mbstate_t when the C library doesn't provide <uchar.h>
// or <wchar.h>, hence the #include_next of those headers instead of #include.
// (e.g. if <wchar.h> isn't present in the C library, the libc++ <wchar.h>
// will include this header. This header needs to not turn around and cyclically
// include <wchar.h>, but fall through to <uchar.h>.)
//
// This does not define std::mbstate_t -- this only brings in the declaration
// in the global namespace.

// We define this here to support older versions of glibc <wchar.h> that do
// not define this for clang. This is also set in libc++'s <wchar.h> header,
// and we need to do so here too to avoid a different function signature given
// a different include order.
#ifdef __cplusplus
#  define __CORRECT_ISO_CPP_WCHAR_H_PROTO
#endif

#if defined(_LIBCPP_HAS_NO_WIDE_CHARACTERS) && defined(__TINYARA__)
# ifndef _WINT_T
#    define _WINT_T
     typedef int wint_t;
# endif
  typedef void* __gnuc_va_list;  
# ifndef ___MBSTATE_T_DECLARED
#  if __has_include_next(<wchar.h>)
#    include_next <wchar.h>  // System's wchar.h provides mbstate_t (kernel build only)
#  endif
# endif
#elif defined(_LIBCPP_HAS_NO_WIDE_CHARACTERS)
// mbstate_t already defined in __config_site for non-TizenRT platforms
#elif defined(_LIBCPP_HAS_MUSL_LIBC)
#   define __NEED_mbstate_t
#   include <bits/alltypes.h>
#   undef __NEED_mbstate_t
#elif __has_include(<bits/types/mbstate_t.h>)
#   include <bits/types/mbstate_t.h> // works on most Unixes
#elif __has_include(<sys/_types/_mbstate_t.h>)
#   include <sys/_types/_mbstate_t.h> // works on Darwin
#elif !defined(_LIBCPP_HAS_NO_WIDE_CHARACTERS) && defined(__TINYARA__)
  // TizenRT with wide-char: define mbstate_t directly to avoid including toolchain's wchar.h which conflicts with TizenRT's wchar.h
  // (different _mbstate_t struct, getwchar macro, FAR pointer conflicts)
# ifndef __machine_mbstate_t_defined
  typedef struct { int __fill[6]; } _mbstate_t;
#   define __machine_mbstate_t_defined
# endif
  typedef _mbstate_t mbstate_t;
# define _MBSTATE_T_DEFINED
# define ___MBSTATE_T_DECLARED
#elif !defined(_LIBCPP_HAS_NO_WIDE_CHARACTERS) && __has_include_next(<wchar.h>)
#   include_next <wchar.h> // fall back to the C standard provider of mbstate_t
#elif __has_include_next(<uchar.h>)
#   include_next <uchar.h> // <uchar.h> is also required to make mbstate_t visible
#else
#   error "We don't know how to get the definition of mbstate_t without <wchar.h> on your platform."
#endif

#endif // _LIBCPP___MBSTATE_T_H
