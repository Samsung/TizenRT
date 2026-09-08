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

// static char_type* assign(char_type* s, size_t n, char_type a);

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_strings_char_traits_specializations_char32_t_assign3(void)
{
#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
    char32_t s2[3] = {0};
    TC_ASSERT_EXPR(std::char_traits<char32_t>::assign(s2, 3, char32_t(5)) == s2);
    TC_ASSERT_EXPR(s2[0] == char32_t(5));
    TC_ASSERT_EXPR(s2[1] == char32_t(5));
    TC_ASSERT_EXPR(s2[2] == char32_t(5));
    TC_ASSERT_EXPR(std::char_traits<char32_t>::assign(NULL, 0, char32_t(5)) == NULL);
#endif  // _LIBCPP_HAS_NO_UNICODE_CHARS
    TC_SUCCESS_RESULT();
    return 0;
}
