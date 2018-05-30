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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef SUPPORT_TEST_WORKAROUNDS_H
#define SUPPORT_TEST_WORKAROUNDS_H

#include "test_macros.h"

#if defined(TEST_COMPILER_EDG)
# define TEST_WORKAROUND_EDG_EXPLICIT_CONSTEXPR // VSO#424280
#endif

#if defined(TEST_COMPILER_C1XX)
# define TEST_WORKAROUND_C1XX_BROKEN_IS_TRIVIALLY_COPYABLE // VSO#117743
# ifndef _MSC_EXTENSIONS
#  define TEST_WORKAROUND_C1XX_BROKEN_ZA_CTOR_CHECK // VSO#119998
# endif
#endif

#endif // SUPPORT_TEST_WORKAROUNDS_H
