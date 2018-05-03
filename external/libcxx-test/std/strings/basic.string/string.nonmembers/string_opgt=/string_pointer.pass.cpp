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
//   bool operator>=(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& lhs, const typename S::value_type* rhs, bool x)
{
    TC_ASSERT_EXPR((lhs >= rhs) == x);
    return 0;
}

int tc_libcxx_strings_string_opgt__string_pointer(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), "", true)));
    TC_ASSERT_FUNC((test(S(""), "abcde", false)));
    TC_ASSERT_FUNC((test(S(""), "abcdefghij", false)));
    TC_ASSERT_FUNC((test(S(""), "abcdefghijklmnopqrst", false)));
    TC_ASSERT_FUNC((test(S("abcde"), "", true)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcde", true)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdefghij", false)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdefghijklmnopqrst", false)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), "", true)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), "abcde", true)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), "abcdefghij", true)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), "abcdefghijklmnopqrst", false)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), "", true)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), "abcde", true)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), "abcdefghij", true)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), "abcdefghijklmnopqrst", true)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
