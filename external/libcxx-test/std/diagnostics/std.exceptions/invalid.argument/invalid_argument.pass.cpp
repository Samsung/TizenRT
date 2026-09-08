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

// test invalid_argument

#include <stdexcept>
#include <type_traits>
#include <cstring>
#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_diagnostics_stdexcept_invalid_argument(void)
{
    static_assert((std::is_base_of<std::logic_error, std::invalid_argument>::value),
                 "std::is_base_of<std::logic_error, std::invalid_argument>::value");
    static_assert(std::is_polymorphic<std::invalid_argument>::value,
                 "std::is_polymorphic<std::invalid_argument>::value");
    {
    const char* msg = "invalid_argument message";
    std::invalid_argument e(msg);
    TC_ASSERT_EXPR(std::strcmp(e.what(), msg) == 0);
    std::invalid_argument e2(e);
    TC_ASSERT_EXPR(std::strcmp(e2.what(), msg) == 0);
    e2 = e;
    TC_ASSERT_EXPR(std::strcmp(e2.what(), msg) == 0);
    }
    {
    std::string msg("another invalid_argument message");
    std::invalid_argument e(msg);
    TC_ASSERT_EXPR(e.what() == msg);
    std::invalid_argument e2(e);
    TC_ASSERT_EXPR(e2.what() == msg);
    e2 = e;
    TC_ASSERT_EXPR(e2.what() == msg);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
