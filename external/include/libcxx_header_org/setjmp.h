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
//===--------------------------- setjmp.h ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_SETJMP_H
#define _LIBCPP_SETJMP_H

/*
    setjmp.h synopsis

Macros:

    setjmp

Types:

    jmp_buf

void longjmp(jmp_buf env, int val);

*/

#include <__config>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

#include_next <setjmp.h>

#ifdef __cplusplus

#ifndef setjmp
#define setjmp(env) setjmp(env)
#endif

#endif // __cplusplus

#endif  // _LIBCPP_SETJMP_H
