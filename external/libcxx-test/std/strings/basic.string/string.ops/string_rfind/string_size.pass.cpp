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

// size_type rfind(const basic_string& str, size_type pos = npos) const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s, const S& str, typename S::size_type pos, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.rfind(str, pos) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(x <= pos && x + str.size() <= s.size());
    return 0;
}

template <class S>
static int
test(const S& s, const S& str, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.rfind(str) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(0 <= x && x + str.size() <= s.size());
    return 0;
}

template <class S>
static int test0()
{
    TC_ASSERT_FUNC((test(S(""), S(""), 0, 0)));
    TC_ASSERT_FUNC((test(S(""), S("abcde"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("abcdeabcde"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("abcdeabcdeabcdeabcde"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), S(""), 1, 0)));
    TC_ASSERT_FUNC((test(S(""), S("abcde"), 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("abcdeabcde"), 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("abcdeabcdeabcdeabcde"), 1, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S(""), 0, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcde"), 0, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcde"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcdeabcdeabcde"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S(""), 1, 1)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcde"), 1, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcde"), 1, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcdeabcdeabcde"), 1, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S(""), 2, 2)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcde"), 2, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcde"), 2, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcdeabcdeabcde"), 2, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S(""), 4, 4)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcde"), 4, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcde"), 4, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcdeabcdeabcde"), 4, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S(""), 5, 5)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcde"), 5, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcde"), 5, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcdeabcdeabcde"), 5, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S(""), 6, 5)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcde"), 6, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcde"), 6, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcdeabcdeabcde"), 6, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S(""), 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcde"), 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcde"), 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcdeabcdeabcde"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S(""), 1, 1)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcde"), 1, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcde"), 1, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcdeabcdeabcde"), 1, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S(""), 5, 5)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcde"), 5, 5)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcde"), 5, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcdeabcdeabcde"), 5, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S(""), 9, 9)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcde"), 9, 5)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcde"), 9, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcdeabcdeabcde"), 9, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S(""), 10, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcde"), 10, 5)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcde"), 10, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcdeabcdeabcde"), 10, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S(""), 11, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcde"), 11, 5)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcde"), 11, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcdeabcdeabcde"), 11, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S(""), 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcde"), 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcde"), 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcdeabcdeabcde"), 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S(""), 1, 1)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcde"), 1, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcde"), 1, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcdeabcdeabcde"), 1, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S(""), 10, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcde"), 10, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcde"), 10, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcdeabcdeabcde"), 10, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S(""), 19, 19)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcde"), 19, 15)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcde"), 19, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcdeabcdeabcde"), 19, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S(""), 20, 20)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcde"), 20, 15)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcde"), 20, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcdeabcdeabcde"), 20, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S(""), 21, 20)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcde"), 21, 15)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcde"), 21, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcdeabcdeabcde"), 21, 0)));
    return 0;
}

template <class S>
static int test1()
{
    TC_ASSERT_FUNC((test(S(""), S(""), 0)));
    TC_ASSERT_FUNC((test(S(""), S("abcde"), S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("abcdeabcde"), S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("abcdeabcdeabcdeabcde"), S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S(""), 5)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcde"), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcde"), S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), S("abcdeabcdeabcdeabcde"), S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S(""), 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcde"), 5)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcde"), 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), S("abcdeabcdeabcdeabcde"), S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S(""), 20)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcde"), 15)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcde"), 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), S("abcdeabcdeabcdeabcde"), 0)));
    return 0;
}

int tc_libcxx_strings_string_rfind_string_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test0<S>()));
    TC_ASSERT_FUNC((test1<S>()));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
