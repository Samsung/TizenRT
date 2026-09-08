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

// typedef char32_t       char_type;
// typedef uint_least32_t int_type;
// typedef streamoff      off_type;
// typedef u32streampos   pos_type;
// typedef mbstate_t      state_type;

#include <string>
#include <type_traits>
#include <cstdint>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_strings_char_traits_specializations_char32_t_types(void)
{
#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
    static_assert((std::is_same<std::char_traits<char32_t>::char_type, char32_t>::value), "");
    static_assert((std::is_same<std::char_traits<char32_t>::int_type, std::uint_least32_t>::value), "");
    static_assert((std::is_same<std::char_traits<char32_t>::off_type, std::streamoff>::value), "");
    static_assert((std::is_same<std::char_traits<char32_t>::pos_type, std::u32streampos>::value), "");
    static_assert((std::is_same<std::char_traits<char32_t>::state_type, std::mbstate_t>::value), "");
#endif  // _LIBCPP_HAS_NO_UNICODE_CHARS
    TC_SUCCESS_RESULT();
    return 0;
}
