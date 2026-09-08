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

// static constexpr bool eq_int_type(int_type c1, int_type c2);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_strings_char_traits_specializations_char32_t_eq_int_type(void)
{
#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
#if TEST_STD_VER >= 11
    TC_ASSERT_EXPR( std::char_traits<char32_t>::eq_int_type(U'a', U'a'));
    TC_ASSERT_EXPR(!std::char_traits<char32_t>::eq_int_type(U'a', U'A'));
    TC_ASSERT_EXPR(!std::char_traits<char32_t>::eq_int_type(std::char_traits<char32_t>::eof(), U'A'));
#endif
    TC_ASSERT_EXPR( std::char_traits<char32_t>::eq_int_type(std::char_traits<char32_t>::eof(),
                                                    std::char_traits<char32_t>::eof()));
#endif  // _LIBCPP_HAS_NO_UNICODE_CHARS
    TC_SUCCESS_RESULT();
    return 0;
}
