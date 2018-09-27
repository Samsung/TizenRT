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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// See bugs.llvm.org/PR20183
//
// XFAIL: with_system_cxx_lib=macosx10.11
// XFAIL: with_system_cxx_lib=macosx10.10
// XFAIL: with_system_cxx_lib=macosx10.9
// XFAIL: with_system_cxx_lib=macosx10.8
// XFAIL: with_system_cxx_lib=macosx10.7

// <random>

// class random_device;

// explicit random_device(const string& token = implementation-defined);

// For the following ctors, the standard states: "The semantics and default
// value of the token parameter are implementation-defined". Implementations
// therefore aren't required to accept any string, but the default shouldn't
// throw.

#include <random>
#include <system_error>
#include <cassert>
#include "libcxx_tc_common.h"

#if !defined(_WIN32)
#include <unistd.h>
#endif

#include "test_macros.h"

#define URANDOM_DEV "/dev/urandom"
#define RANDOM_DEV "/dev/random"

bool is_valid_random_device(const std::string &token) {
#if defined(_LIBCPP_USING_DEV_RANDOM)
  // Not an exhaustive list: they're the only tokens that are tested below.
  return token == URANDOM_DEV || token == RANDOM_DEV;
#else
  return token == URANDOM_DEV;
#endif
    return 0;
}

static int check_random_device_valid(const std::string &token) {
  std::random_device r(token);
    return 0;
}

static int check_random_device_invalid(const std::string &token) {
#ifndef TEST_HAS_NO_EXCEPTIONS
  try {
    std::random_device r(token);
    LIBCPP_ASSERT(false);
  } catch (const std::system_error&) {
  }
#else
  ((static int)token);
#endif
    return 0;
}


int tc_numerics_rand_device_ctor(void) {
    
    std::string token = URANDOM_DEV;
    if (is_valid_random_device(token)) {
      check_random_device_valid(token);
    } else {
      check_random_device_invalid(token);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
