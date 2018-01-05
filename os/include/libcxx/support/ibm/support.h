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
//===----------------------- support/ibm/support.h ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_SUPPORT_IBM_SUPPORT_H
#define _LIBCPP_SUPPORT_IBM_SUPPORT_H

extern "builtin" int __popcnt4(unsigned int);
extern "builtin" int __popcnt8(unsigned long long);
extern "builtin" unsigned int __cnttz4(unsigned int);
extern "builtin" unsigned int __cnttz8(unsigned long long);
extern "builtin" unsigned int __cntlz4(unsigned int);
extern "builtin" unsigned int __cntlz8(unsigned long long);

// Builtin functions for counting population
#define __builtin_popcount(x) __popcnt4(x)
#define __builtin_popcountll(x) __popcnt8(x)
#if defined(__64BIT__)
#define __builtin_popcountl(x) __builtin_popcountll(x)
#else
#define __builtin_popcountl(x) __builtin_popcount(x)
#endif

// Builtin functions for counting trailing zeros
#define __builtin_ctz(x) __cnttz4(x)
#define __builtin_ctzll(x) __cnttz8(x)
#if defined(__64BIT__)
#define __builtin_ctzl(x) __builtin_ctzll(x)
#else
#define __builtin_ctzl(x) __builtin_ctz(x)
#endif

// Builtin functions for counting leading zeros
#define __builtin_clz(x) __cntlz4(x)
#define __builtin_clzll(x) __cntlz8(x)
#if defined(__64BIT__)
#define __builtin_clzl(x) __builtin_clzll(x)
#else
#define __builtin_clzl(x) __builtin_clz(x)
#endif

#if defined(__64BIT__)
#define __SIZE_WIDTH__ 64
#else
#define __SIZE_WIDTH__ 32
#endif

#endif // _LIBCPP_SUPPORT_IBM_SUPPORT_H
