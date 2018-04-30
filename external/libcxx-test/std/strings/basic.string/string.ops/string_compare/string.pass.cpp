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

// int compare(const basic_string& str) const

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


static int sign(int x)
{
    if (x == 0)
        return 0;
    if (x < 0)
        return -1;
    return 1;
}

template <class S>
static int
test(const S& s, const S& str, int x)
{
    TC_ASSERT_EXPR(sign(s.compare(str)) == sign(x));
    return 0;
}

int tc_libcxx_strings_string_compare_string(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), S(""), 0)));
    TC_ASSERT_FUNC((test(S(""), S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S(""), S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S(""), S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcde"), S(""), 5)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcde"), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdefghij"), -5)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdefghijklmnopqrst"), -15)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), S(""), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), S("abcde"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), S("abcdefghij"), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), S("abcdefghijklmnopqrst"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), S(""), 20)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), S("abcde"), 15)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), S("abcdefghij"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), S("abcdefghijklmnopqrst"), 0)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
