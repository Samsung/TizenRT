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

// template<> struct char_traits<char>

// static const char_type* find(const char_type* s, size_t n, const char_type& a);
// constexpr in C++17

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

#if TEST_STD_VER > 14
constexpr bool test_constexpr()
{
    constexpr const char *p = "123";
    return std::char_traits<char>::find(p, 3, '1') == p
        && std::char_traits<char>::find(p, 3, '2') == p + 1
        && std::char_traits<char>::find(p, 3, '3') == p + 2
        && std::char_traits<char>::find(p, 3, '4') == nullptr;
    return 0;
}
#endif

int tc_libcxx_strings_char_traits_specializations_char_find(void)
{
    char s1[] = {1, 2, 3};
    TC_ASSERT_EXPR(std::char_traits<char>::find(s1, 3, char(1)) == s1);
    TC_ASSERT_EXPR(std::char_traits<char>::find(s1, 3, char(2)) == s1+1);
    TC_ASSERT_EXPR(std::char_traits<char>::find(s1, 3, char(3)) == s1+2);
    TC_ASSERT_EXPR(std::char_traits<char>::find(s1, 3, char(4)) == 0);
    TC_ASSERT_EXPR(std::char_traits<char>::find(s1, 3, char(0)) == 0);
    TC_ASSERT_EXPR(std::char_traits<char>::find(NULL, 0, char(0)) == 0);

#if TEST_STD_VER > 14
    static_assert(test_constexpr(), "" );
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
