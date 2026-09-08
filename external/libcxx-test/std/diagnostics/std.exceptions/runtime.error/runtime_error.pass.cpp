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

// test runtime_error

#include <stdexcept>
#include <type_traits>
#include <cstring>
#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_diagnostics_stdexcept_runtime_error(void)
{
    static_assert((std::is_base_of<std::exception, std::runtime_error>::value),
                 "std::is_base_of<std::exception, std::runtime_error>::value");
    static_assert(std::is_polymorphic<std::runtime_error>::value,
                 "std::is_polymorphic<std::runtime_error>::value");
    {
    const char* msg = "runtime_error message";
    std::runtime_error e(msg);
    TC_ASSERT_EXPR(std::strcmp(e.what(), msg) == 0);
    std::runtime_error e2(e);
    TC_ASSERT_EXPR(std::strcmp(e2.what(), msg) == 0);
    e2 = e;
    TC_ASSERT_EXPR(std::strcmp(e2.what(), msg) == 0);
    }
    {
    std::string msg("another runtime_error message");
    std::runtime_error e(msg);
    TC_ASSERT_EXPR(e.what() == msg);
    std::runtime_error e2(e);
    TC_ASSERT_EXPR(e2.what() == msg);
    e2 = e;
    TC_ASSERT_EXPR(e2.what() == msg);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
