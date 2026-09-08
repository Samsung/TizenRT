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

// static char_type* move(char_type* s1, const char_type* s2, size_t n);

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_strings_char_traits_specializations_char_move(void)
{
    char s1[] = {1, 2, 3};
    TC_ASSERT_EXPR(std::char_traits<char>::move(s1, s1+1, 2) == s1);
    TC_ASSERT_EXPR(s1[0] == char(2));
    TC_ASSERT_EXPR(s1[1] == char(3));
    TC_ASSERT_EXPR(s1[2] == char(3));
    s1[2] = char(0);
    TC_ASSERT_EXPR(std::char_traits<char>::move(s1+1, s1, 2) == s1+1);
    TC_ASSERT_EXPR(s1[0] == char(2));
    TC_ASSERT_EXPR(s1[1] == char(2));
    TC_ASSERT_EXPR(s1[2] == char(3));
    TC_ASSERT_EXPR(std::char_traits<char>::move(NULL, s1, 0) == NULL);
    TC_ASSERT_EXPR(std::char_traits<char>::move(s1, NULL, 0) == s1);
    TC_SUCCESS_RESULT();
    return 0;
}
