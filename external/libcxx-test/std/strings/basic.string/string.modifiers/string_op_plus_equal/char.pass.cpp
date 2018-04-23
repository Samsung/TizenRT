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

// basic_string<charT,traits,Allocator>& operator+=(charT c);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::value_type str, S expected)
{
    s += str;
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

int tc_libcxx_strings_string_op_plus_equal_char(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), 'a', S("a"))));
    TC_ASSERT_FUNC((test(S("12345"), 'a', S("12345a"))));
    TC_ASSERT_FUNC((test(S("1234567890"), 'a', S("1234567890a"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), 'a', S("12345678901234567890a"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
