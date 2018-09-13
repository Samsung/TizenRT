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

// static int pop_back();

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, S expected)
{
    s.pop_back();
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s[s.size()] == typename S::value_type());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

int tc_libcxx_strings_string_erase_pop_back(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S("abcde"), S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), S("abcdefghijklmnopqrs"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
