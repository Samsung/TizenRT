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

// <random>

// class random_device;

// double entropy() const;

#include <random>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_numerics_rand_device_entropy(void)
{
    std::random_device r;
    double e;

    e = r.entropy();
    UNUSED(e);
    TC_SUCCESS_RESULT();
    return 0;
}
