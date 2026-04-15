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
//===--------------------------- limits.h ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_LIMITS_H
#define _LIBCPP_LIMITS_H

/*
    limits.h synopsis

Macros:

    CHAR_BIT
    SCHAR_MIN
    SCHAR_MAX
    UCHAR_MAX
    CHAR_MIN
    CHAR_MAX
    MB_LEN_MAX
    SHRT_MIN
    SHRT_MAX
    USHRT_MAX
    INT_MIN
    INT_MAX
    UINT_MAX
    LONG_MIN
    LONG_MAX
    ULONG_MAX
    LLONG_MIN   // C99
    LLONG_MAX   // C99
    ULLONG_MAX  // C99

*/

#include <__config>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

#ifndef __GNUC__
#include_next <limits.h>
#else
// GCC header limits.h recursively includes itself through another header called
// syslimits.h for some reason. This setup breaks down if we directly
// #include_next GCC's limits.h (reasons not entirely clear to me). Therefore,
// we manually re-create the necessary include sequence below:

// Get the system limits.h defines (force recurse into the next level)
#define _GCC_LIMITS_H_
#define _GCC_NEXT_LIMITS_H
#include_next <limits.h>

// Get the ISO C defines
#undef _GCC_LIMITS_H_
#include_next <limits.h>
#endif // __GNUC__

#endif  // _LIBCPP_LIMITS_H
