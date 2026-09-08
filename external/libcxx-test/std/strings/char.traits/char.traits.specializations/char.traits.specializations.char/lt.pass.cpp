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

// static constexpr bool lt(char_type c1, char_type c2);

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_strings_char_traits_specializations_char_lt(void)
{
    TC_ASSERT_EXPR( std::char_traits<char>::lt('\0', 'A'));
    TC_ASSERT_EXPR(!std::char_traits<char>::lt('A', '\0'));

    TC_ASSERT_EXPR(!std::char_traits<char>::lt('a', 'a'));
    TC_ASSERT_EXPR( std::char_traits<char>::lt('A', 'a'));
    TC_ASSERT_EXPR(!std::char_traits<char>::lt('a', 'A'));

    TC_ASSERT_EXPR( std::char_traits<char>::lt('a', 'z'));
    TC_ASSERT_EXPR( std::char_traits<char>::lt('A', 'Z'));

    TC_ASSERT_EXPR( std::char_traits<char>::lt(' ', 'A'));
    TC_ASSERT_EXPR( std::char_traits<char>::lt('A', '~'));
    TC_SUCCESS_RESULT();
    return 0;
}
