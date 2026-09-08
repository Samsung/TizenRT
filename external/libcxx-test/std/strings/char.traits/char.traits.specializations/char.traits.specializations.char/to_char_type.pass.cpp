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

// static constexpr char_type to_char_type(int_type c);

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_strings_char_traits_specializations_char_to_char_type(void)
{
    TC_ASSERT_EXPR(std::char_traits<char>::to_char_type('a') == 'a');
    TC_ASSERT_EXPR(std::char_traits<char>::to_char_type('A') == 'A');
    TC_ASSERT_EXPR(std::char_traits<char>::to_char_type(0) == 0);
    TC_SUCCESS_RESULT();
    return 0;
}
