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

// static static int assign(char_type& c1, const char_type& c2);
// constexpr in C++17

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
#if TEST_STD_VER > 14
constexpr bool test_constexpr()
{
    wchar_t c = L'1';
    std::char_traits<wchar_t>::assign(c, L'a');
    return c == L'a';
}
#endif

int tc_libcxx_strings_char_traits_specializations_wchar_t_assign2(void)
{
    wchar_t c = L'\0';
    std::char_traits<wchar_t>::assign(c, L'a');
    TC_ASSERT_EXPR(c == L'a');

#if TEST_STD_VER > 14
    static_assert(test_constexpr(), "" );
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#else
int tc_libcxx_strings_char_traits_specializations_wchar_t_assign2(void)
{
    TC_SUCCESS_RESULT();
    return 0;
}
#endif
