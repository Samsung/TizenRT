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

// template<class charT, class traits, class Allocator>
//   static int swap(basic_string<charT,traits,Allocator>& lhs,
//             basic_string<charT,traits,Allocator>& rhs);

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s1, S s2)
{
    S s1_ = s1;
    S s2_ = s2;
    swap(s1, s2);
    LIBCPP_ASSERT(s1.__invariants());
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s1 == s2_);
    TC_ASSERT_EXPR(s2 == s1_);
    return 0;
}

int tc_libcxx_strings_string_special_swap(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), S(""))));
    TC_ASSERT_FUNC((test(S(""), S("12345"))));
    TC_ASSERT_FUNC((test(S(""), S("1234567890"))));
    TC_ASSERT_FUNC((test(S(""), S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), S("12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), S("1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), S("12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), S("1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), S("12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), S("1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), S("12345678901234567890"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
