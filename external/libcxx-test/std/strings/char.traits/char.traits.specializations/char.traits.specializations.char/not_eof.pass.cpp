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

// static constexpr int_type not_eof(int_type c);

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_strings_char_traits_specializations_char_not_eof(void)
{
    TC_ASSERT_EXPR(std::char_traits<char>::not_eof('a') == 'a');
    TC_ASSERT_EXPR(std::char_traits<char>::not_eof('A') == 'A');
    TC_ASSERT_EXPR(std::char_traits<char>::not_eof(0) == 0);
    TC_ASSERT_EXPR(std::char_traits<char>::not_eof(std::char_traits<char>::eof()) !=
           std::char_traits<char>::eof());
    TC_SUCCESS_RESULT();
    return 0;
}
