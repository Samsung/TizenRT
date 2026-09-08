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

// static char_type* assign(char_type* s, size_t n, char_type a);

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_strings_char_traits_specializations_char_assign3(void)
{
    char s2[3] = {0};
    TC_ASSERT_EXPR(std::char_traits<char>::assign(s2, 3, char(5)) == s2);
    TC_ASSERT_EXPR(s2[0] == char(5));
    TC_ASSERT_EXPR(s2[1] == char(5));
    TC_ASSERT_EXPR(s2[2] == char(5));
    TC_ASSERT_EXPR(std::char_traits<char>::assign(NULL, 0, char(5)) == NULL);
    TC_SUCCESS_RESULT();
    return 0;
}
