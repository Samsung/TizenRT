//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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

// result_type operator()();

#include <random>
#include <cassert>
#include "libcxx_tc_common.h"
#include <system_error>

#include "test_macros.h"

int tc_numerics_rand_device_eval(void)
{
    std::random_device r;
    std::random_device::result_type e;
    e = r();
    UNUSED(e);

    TC_SUCCESS_RESULT();
    return 0;
}
