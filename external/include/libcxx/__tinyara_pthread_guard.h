// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This header is used to prevent circular dependency issues in TizenRT (__TINYARA__)
// when including standard C headers that need pthread types but we're still in
// the process of defining basic types.
//
// This header should be included BEFORE any libc++ headers that might indirectly
// include pthread.h through the libc++ configuration.

#ifndef _LIBCPP___TINYARA_PTHREAD_GUARD_H
#define _LIBCPP___TINYARA_PTHREAD_GUARD_H

// For TizenRT, prevent pthread.h from being included during initial type definition phase
#ifdef __TINYARA__
#define _LIBCPP_NO_PTHREAD_INCLUDE
#endif

#endif // _LIBCPP___TINYARA_PTHREAD_GUARD_H