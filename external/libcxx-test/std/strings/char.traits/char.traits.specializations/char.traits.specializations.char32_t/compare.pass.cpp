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

// <string>

// template<> struct char_traits<char32_t>

// static int compare(const char_type* s1, const char_type* s2, size_t n);
// constexpr in C++17

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

#if TEST_STD_VER > 14
constexpr bool test_constexpr()
{
    return std::char_traits<char32_t>::compare(U"123", U"223", 3) < 0
        && std::char_traits<char32_t>::compare(U"223", U"123", 3) > 0
        && std::char_traits<char32_t>::compare(U"123", U"123", 3) == 0;
    return 0;
}
#endif

int tc_libcxx_strings_char_traits_specializations_char32_t_compare(void)
{
#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
#if TEST_STD_VER >= 11
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"", U"", 0) == 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(NULL, NULL, 0) == 0);

    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"1", U"1", 1) == 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"1", U"2", 1) < 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"2", U"1", 1) > 0);

    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"12", U"12", 2) == 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"12", U"13", 2) < 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"12", U"22", 2) < 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"13", U"12", 2) > 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"22", U"12", 2) > 0);

    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"123", U"123", 3) == 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"123", U"223", 3) < 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"123", U"133", 3) < 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"123", U"124", 3) < 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"223", U"123", 3) > 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"133", U"123", 3) > 0);
    TC_ASSERT_EXPR(std::char_traits<char32_t>::compare(U"124", U"123", 3) > 0);
#endif

#if TEST_STD_VER > 14
    static_assert(test_constexpr(), "" );
#endif
#endif  // _LIBCPP_HAS_NO_UNICODE_CHARS
    TC_SUCCESS_RESULT();
    return 0;
}
