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

// size_type rfind(const charT* s, size_type pos = npos) const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s, const typename S::value_type* str, typename S::size_type pos,
     typename S::size_type x)
{
    TC_ASSERT_EXPR(s.rfind(str, pos) == x);
    if (x != S::npos)
    {
        typename S::size_type n = S::traits_type::length(str);
        TC_ASSERT_EXPR(x <= pos && x + n <= s.size());
    }
    return 0;
}

template <class S>
static int
test(const S& s, const typename S::value_type* str, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.rfind(str) == x);
    if (x != S::npos)
    {
        typename S::size_type pos = s.size();
        typename S::size_type n = S::traits_type::length(str);
        TC_ASSERT_EXPR(x <= pos && x + n <= s.size());
    }
    return 0;
}

template <class S>
static int test0()
{
    TC_ASSERT_FUNC((test(S(""), "", 0, 0)));
    TC_ASSERT_FUNC((test(S(""), "abcde", 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "abcdeabcde", 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "abcdeabcdeabcdeabcde", 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "", 1, 0)));
    TC_ASSERT_FUNC((test(S(""), "abcde", 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "abcdeabcde", 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "abcdeabcdeabcdeabcde", 1, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "", 0, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcde", 0, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcde", 0, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcdeabcdeabcde", 0, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "", 1, 1)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcde", 1, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcde", 1, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcdeabcdeabcde", 1, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "", 2, 2)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcde", 2, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcde", 2, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcdeabcdeabcde", 2, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "", 4, 4)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcde", 4, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcde", 4, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcdeabcdeabcde", 4, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "", 5, 5)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcde", 5, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcde", 5, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcdeabcdeabcde", 5, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "", 6, 5)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcde", 6, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcde", 6, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcdeabcdeabcde", 6, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "", 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcde", 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcde", 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", 0, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "", 1, 1)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcde", 1, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcde", 1, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", 1, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "", 5, 5)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcde", 5, 5)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcde", 5, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", 5, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "", 9, 9)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcde", 9, 5)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcde", 9, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", 9, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "", 10, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcde", 10, 5)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcde", 10, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", 10, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "", 11, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcde", 11, 5)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcde", 11, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", 11, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "", 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcde", 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "", 1, 1)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcde", 1, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 1, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 1, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "", 10, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcde", 10, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 10, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 10, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "", 19, 19)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcde", 19, 15)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 19, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 19, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "", 20, 20)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcde", 20, 15)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 20, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 20, 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "", 21, 20)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcde", 21, 15)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 21, 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 21, 0)));
    return 0;
}

template <class S>
static int test1()
{
    TC_ASSERT_FUNC((test(S(""), "", 0)));
    TC_ASSERT_FUNC((test(S(""), "abcde", S::npos)));
    TC_ASSERT_FUNC((test(S(""), "abcdeabcde", S::npos)));
    TC_ASSERT_FUNC((test(S(""), "abcdeabcdeabcdeabcde", S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "", 5)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcde", 0)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcde", S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdeabcdeabcdeabcde", S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "", 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcde", 5)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcde", 0)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "", 20)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcde", 15)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 10)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 0)));
    return 0;
}

int tc_libcxx_strings_string_rfind_pointer_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test0<S>()));
    TC_ASSERT_FUNC((test1<S>()));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
