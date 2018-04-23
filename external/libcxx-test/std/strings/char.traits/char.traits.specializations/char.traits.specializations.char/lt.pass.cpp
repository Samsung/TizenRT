/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
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
