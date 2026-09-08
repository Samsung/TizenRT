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

// <string>

// template<> struct char_traits<char>

// static size_t length(const char_type* s);
// constexpr in C++17

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

#if TEST_STD_VER > 14
constexpr bool test_constexpr()
{
    return std::char_traits<char>::length("") == 0
        && std::char_traits<char>::length("abcd") == 4;
    return 0;
}
#endif


int tc_libcxx_strings_char_traits_specializations_char_length(void)
{
    TC_ASSERT_EXPR(std::char_traits<char>::length("") == 0);
    TC_ASSERT_EXPR(std::char_traits<char>::length("a") == 1);
    TC_ASSERT_EXPR(std::char_traits<char>::length("aa") == 2);
    TC_ASSERT_EXPR(std::char_traits<char>::length("aaa") == 3);
    TC_ASSERT_EXPR(std::char_traits<char>::length("aaaa") == 4);

#if TEST_STD_VER > 14
    static_assert(test_constexpr(), "" );
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
