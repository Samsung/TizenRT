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

// template<> struct char_traits<wchar_t>

// static constexpr int_type to_int_type(char_type c);

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_strings_char_traits_specializations_wchar_t_to_int_type(void)
{
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    TC_ASSERT_EXPR(std::char_traits<wchar_t>::to_int_type(L'a') == L'a');
    TC_ASSERT_EXPR(std::char_traits<wchar_t>::to_int_type(L'A') == L'A');
    TC_ASSERT_EXPR(std::char_traits<wchar_t>::to_int_type(0) == 0);
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
