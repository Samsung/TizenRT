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

// int compare(size_type pos1, size_type n1, const basic_string& str) const;

#include <string>
#include <stdexcept>
#include <cassert>
#include "libcxx_tc_common.h"


#include "test_macros.h"

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
test(const S& s, typename S::size_type pos1, typename S::size_type n1,
     const S& str, int x)
{
    if (pos1 <= s.size())
    {
        TC_ASSERT_EXPR(sign(s.compare(pos1, n1, str)) == sign(x));
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            TEST_IGNORE_NODISCARD s.compare(pos1, n1, str);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
            TC_ASSERT_EXPR(pos1 > s.size());
        }
    }
#endif
    return 0;
}

template <class S>
static int test0()
{
    TC_ASSERT_FUNC((test(S(""), 0, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S(""), 0, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S(""), 0, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S(""), 0, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S(""), 0, 1, S(""), 0)));
    TC_ASSERT_FUNC((test(S(""), 0, 1, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S(""), 0, 1, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S(""), 0, 1, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S(""), 1, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S(""), 1, 0, S("abcde"), 0)));
    TC_ASSERT_FUNC((test(S(""), 1, 0, S("abcdefghij"), 0)));
    TC_ASSERT_FUNC((test(S(""), 1, 0, S("abcdefghijklmnopqrst"), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, S("abcde"), -4)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, S("abcdefghij"), -9)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, S("abcdefghijklmnopqrst"), -19)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, S(""), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, S("abcde"), -3)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, S("abcdefghij"), -8)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, S("abcdefghijklmnopqrst"), -18)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, S(""), 4)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, S("abcde"), -1)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, S("abcdefghij"), -6)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, S("abcdefghijklmnopqrst"), -16)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, S(""), 5)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, S("abcde"), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, S("abcdefghij"), -5)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, S("abcdefghijklmnopqrst"), -15)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 6, S(""), 5)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 6, S("abcde"), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 6, S("abcdefghij"), -5)));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 6, S("abcdefghijklmnopqrst"), -15)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, S(""), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, S(""), 3)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, S(""), 4)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 5, S(""), 4)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 5, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 5, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 5, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, S("abcde"), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, S("abcdefghij"), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, S("abcdefghijklmnopqrst"), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, S(""), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, S("abcde"), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, S("abcdefghij"), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, S("abcdefghijklmnopqrst"), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, S(""), 3)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, S("abcde"), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, S("abcdefghij"), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, S("abcdefghijklmnopqrst"), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 4, S(""), 3)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 4, S("abcde"), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 4, S("abcdefghij"), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 4, S("abcdefghijklmnopqrst"), 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, S("abcde"), 4)));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, S("abcdefghij"), 4)));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, S("abcdefghijklmnopqrst"), 4)));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 2, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 2, S("abcde"), 4)));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 2, S("abcdefghij"), 4)));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 2, S("abcdefghijklmnopqrst"), 4)));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 1, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 1, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 1, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 1, S("abcdefghijklmnopqrst"), -20)));
    return 0;
}

template <class S>
static int test1()
{
    TC_ASSERT_FUNC((test(S("abcde"), 6, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), 6, 0, S("abcde"), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), 6, 0, S("abcdefghij"), 0)));
    TC_ASSERT_FUNC((test(S("abcde"), 6, 0, S("abcdefghijklmnopqrst"), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, S("abcde"), -4)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, S("abcdefghij"), -9)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, S("abcdefghijklmnopqrst"), -19)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, S(""), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, S("abcde"), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, S("abcdefghij"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, S("abcdefghijklmnopqrst"), -15)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, S(""), 9)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, S("abcde"), 4)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, S("abcdefghij"), -1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, S("abcdefghijklmnopqrst"), -11)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, S(""), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, S("abcde"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, S("abcdefghij"), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, S("abcdefghijklmnopqrst"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 11, S(""), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 11, S("abcde"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 11, S("abcdefghij"), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 11, S("abcdefghijklmnopqrst"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, S(""), 4)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, S(""), 8)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, S(""), 9)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 10, S(""), 9)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 10, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 10, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 10, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, S("abcde"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, S("abcdefghij"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, S("abcdefghijklmnopqrst"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, S(""), 2)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, S("abcde"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, S("abcdefghij"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, S("abcdefghijklmnopqrst"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, S(""), 4)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, S("abcde"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, S("abcdefghij"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, S("abcdefghijklmnopqrst"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, S(""), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, S("abcde"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, S("abcdefghij"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, S("abcdefghijklmnopqrst"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 6, S(""), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 6, S("abcde"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 6, S("abcdefghij"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 6, S("abcdefghijklmnopqrst"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, S("abcde"), 9)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, S("abcdefghij"), 9)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, S("abcdefghijklmnopqrst"), 9)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 2, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 2, S("abcde"), 9)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 2, S("abcdefghij"), 9)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 2, S("abcdefghijklmnopqrst"), 9)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 1, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 1, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 1, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 1, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 0, S("abcde"), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 0, S("abcdefghij"), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 0, S("abcdefghijklmnopqrst"), 0)));
    return 0;
}

template <class S>
static int test2()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, S("abcde"), -4)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, S("abcdefghij"), -9)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, S("abcdefghijklmnopqrst"), -19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, S(""), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, S("abcde"), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, S("abcdefghij"), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, S("abcdefghijklmnopqrst"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, S(""), 19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, S("abcde"), 14)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, S("abcdefghij"), 9)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, S("abcdefghijklmnopqrst"), -1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, S(""), 20)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, S("abcde"), 15)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, S("abcdefghij"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, S("abcdefghijklmnopqrst"), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 21, S(""), 20)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 21, S("abcde"), 15)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 21, S("abcdefghij"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 21, S("abcdefghijklmnopqrst"), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, S(""), 9)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, S(""), 18)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, S(""), 19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 20, S(""), 19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 20, S("abcde"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 20, S("abcdefghij"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 20, S("abcdefghijklmnopqrst"), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, S("abcde"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, S("abcdefghij"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, S("abcdefghijklmnopqrst"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, S(""), 5)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, S("abcde"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, S("abcdefghij"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, S("abcdefghijklmnopqrst"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, S(""), 9)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, S("abcde"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, S("abcdefghij"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, S("abcdefghijklmnopqrst"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, S(""), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, S("abcde"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, S("abcdefghij"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, S("abcdefghijklmnopqrst"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 11, S(""), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 11, S("abcde"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 11, S("abcdefghij"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 11, S("abcdefghijklmnopqrst"), 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, S("abcde"), 19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, S("abcdefghij"), 19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, S("abcdefghijklmnopqrst"), 19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 2, S(""), 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 2, S("abcde"), 19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 2, S("abcdefghij"), 19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 2, S("abcdefghijklmnopqrst"), 19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 1, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 1, S("abcde"), -5)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 1, S("abcdefghij"), -10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 1, S("abcdefghijklmnopqrst"), -20)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 0, S(""), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 0, S("abcde"), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 0, S("abcdefghij"), 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 0, S("abcdefghijklmnopqrst"), 0)));
    return 0;
}

int tc_libcxx_strings_string_compare_size_size_string(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test0<S>()));
    TC_ASSERT_FUNC((test1<S>()));
    TC_ASSERT_FUNC((test2<S>()));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
