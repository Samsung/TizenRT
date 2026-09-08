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

// template<> struct char_traits<char32_t>

// static size_t length(const char_type* s);
// constexpr in C++17

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

#if TEST_STD_VER > 14
constexpr bool test_constexpr()
{
    return std::char_traits<char32_t>::length(U"") == 0
        && std::char_traits<char32_t>::length(U"abcd") == 4;
    return 0;
}
#endif

int tc_libcxx_strings_char_traits_specializations_char32_t_length(void)
{
#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
#if TEST_STD_VER >= 11
    TC_ASSERT_EXPR(std::char_traits<char32_t>::length(U"") == 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::length(U"a") == 1);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::length(U"aa") == 2);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::length(U"aaa") == 3);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::length(U"aaaa") == 4);
#endif

#if TEST_STD_VER > 14
    static_assert(test_constexpr(), "" );
#endif
#endif  // _LIBCPP_HAS_NO_UNICODE_CHARS
    TC_SUCCESS_RESULT();
    return 0;
}
