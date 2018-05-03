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

// basic_string<charT,traits,Allocator>&
//   insert(size_type pos1, const basic_string<charT,traits,Allocator>& str,
//          size_type pos2, size_type n=npos);
// the "=npos" was added in C++14

#include <string>
#include <stdexcept>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::size_type pos1, S str, typename S::size_type pos2,
     typename S::size_type n, S expected)
{
    const typename S::size_type old_size = s.size();
    S s0 = s;
    if (pos1 <= old_size && pos2 <= str.size())
    {
        s.insert(pos1, str, pos2, n);
        LIBCPP_ASSERT(s.__invariants());
        TC_ASSERT_EXPR(s == expected);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            s.insert(pos1, str, pos2, n);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
            TC_ASSERT_EXPR(pos1 > old_size || pos2 > str.size());
            TC_ASSERT_EXPR(s == s0);
        }
    }
#endif
    return 0;
}

template <class S>
static int
test_npos(S s, typename S::size_type pos1, S str, typename S::size_type pos2, S expected)
{
    const typename S::size_type old_size = s.size();
    S s0 = s;
    if (pos1 <= old_size && pos2 <= str.size())
    {
        s.insert(pos1, str, pos2);
        LIBCPP_ASSERT(s.__invariants());
        TC_ASSERT_EXPR(s == expected);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            s.insert(pos1, str, pos2);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
            TC_ASSERT_EXPR(pos1 > old_size || pos2 > str.size());
            TC_ASSERT_EXPR(s == s0);
        }
    }
#endif
    return 0;
}


template <class S>
static int test0()
{
    TC_ASSERT_FUNC((test(S(""), 0, S(""), 0, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S(""), 0, 1, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 0, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 0, 1, S("1"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 0, 2, S("12"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 0, 4, S("1234"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 0, 5, S("12345"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 0, 6, S("12345"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 1, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 1, 1, S("2"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 1, 2, S("23"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 1, 3, S("234"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 1, 4, S("2345"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 1, 5, S("2345"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 2, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 2, 1, S("3"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 2, 2, S("34"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 2, 3, S("345"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 2, 4, S("345"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 4, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 4, 1, S("5"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 4, 2, S("5"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 5, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 5, 1, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 0, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 0, 1, S("1"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 0, 5, S("12345"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 0, 9, S("123456789"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 0, 10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 0, 11, S("1234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 1, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 1, 1, S("2"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 1, 4, S("2345"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 1, 8, S("23456789"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 1, 9, S("234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 1, 10, S("234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 5, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 5, 1, S("6"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 5, 2, S("67"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 5, 4, S("6789"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 5, 5, S("67890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 5, 6, S("67890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 9, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 9, 1, S("0"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 9, 2, S("0"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 10, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 10, 1, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("1234567890"), 11, 0, S("can't happen"))));
    return 0;
}

template <class S>
static int test1()
{
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 0, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 0, 1, S("1"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 0, 10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 0, 19, S("1234567890123456789"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 0, 20, S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 0, 21, S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 1, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 1, 1, S("2"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 1, 9, S("234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 1, 18, S("234567890123456789"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 1, 19, S("2345678901234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 1, 20, S("2345678901234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 10, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 10, 1, S("1"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 10, 5, S("12345"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 10, 9, S("123456789"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 10, 10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 10, 11, S("1234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 19, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 19, 1, S("0"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 19, 2, S("0"))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 20, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 20, 1, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S(""), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S(""), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 0, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 0, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 0, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 0, 6, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 1, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 1, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 1, 3, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 1, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 1, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 2, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 2, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 2, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 2, 3, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 2, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 4, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 4, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 4, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 5, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 5, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345"), 6, 0, S("can't happen"))));
    return 0;
}

template <class S>
static int test2()
{
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 0, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 0, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 0, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 0, 11, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 1, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 1, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 1, 8, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 1, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 1, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 5, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 5, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 5, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 5, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 5, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 5, 6, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 9, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 9, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 9, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 10, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 10, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 0, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 0, 19, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 0, 20, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 0, 21, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 1, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 1, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 1, 18, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 1, 19, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 1, 20, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 10, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 10, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 10, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 10, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 10, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 10, 11, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 19, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 19, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 19, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 20, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 20, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S(""), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S(""), 0, 1, S("abcde"))));
    return 0;
}

template <class S>
static int test3()
{
    TC_ASSERT_FUNC((test(S("abcde"), 0, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 0, 1, S("1abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 0, 2, S("12abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 0, 4, S("1234abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 0, 5, S("12345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 0, 6, S("12345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 1, 1, S("2abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 1, 2, S("23abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 1, 3, S("234abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 1, 4, S("2345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 1, 5, S("2345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 2, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 2, 1, S("3abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 2, 2, S("34abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 2, 3, S("345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 2, 4, S("345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 4, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 4, 1, S("5abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 4, 2, S("5abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 5, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 5, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 0, 1, S("1abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 0, 5, S("12345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 0, 9, S("123456789abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 0, 10, S("1234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 0, 11, S("1234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 1, 1, S("2abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 1, 4, S("2345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 1, 8, S("23456789abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 1, 9, S("234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 1, 10, S("234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 5, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 5, 1, S("6abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 5, 2, S("67abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 5, 4, S("6789abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 5, 5, S("67890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 5, 6, S("67890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 9, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 9, 1, S("0abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 9, 2, S("0abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 10, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 10, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 0, 1, S("1abcde"))));
    return 0;
}

template <class S>
static int test4()
{
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 0, 10, S("1234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 0, 19, S("1234567890123456789abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 0, 20, S("12345678901234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 0, 21, S("12345678901234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 1, 1, S("2abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 1, 9, S("234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 1, 18, S("234567890123456789abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 1, 19, S("2345678901234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 1, 20, S("2345678901234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 10, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 10, 1, S("1abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 10, 5, S("12345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 10, 9, S("123456789abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 10, 10, S("1234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 10, 11, S("1234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 19, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 19, 1, S("0abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 19, 2, S("0abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 20, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 20, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S(""), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S(""), 0, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 0, 1, S("a1bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 0, 2, S("a12bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 0, 4, S("a1234bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 0, 5, S("a12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 0, 6, S("a12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 1, 1, S("a2bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 1, 2, S("a23bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 1, 3, S("a234bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 1, 4, S("a2345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 1, 5, S("a2345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 2, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 2, 1, S("a3bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 2, 2, S("a34bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 2, 3, S("a345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 2, 4, S("a345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 4, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 4, 1, S("a5bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 4, 2, S("a5bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 5, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 5, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 0, 1, S("a1bcde"))));
    return 0;
}

template <class S>
static int test5()
{
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 0, 5, S("a12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 0, 9, S("a123456789bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 0, 10, S("a1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 0, 11, S("a1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 1, 1, S("a2bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 1, 4, S("a2345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 1, 8, S("a23456789bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 1, 9, S("a234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 1, 10, S("a234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 5, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 5, 1, S("a6bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 5, 2, S("a67bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 5, 4, S("a6789bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 5, 5, S("a67890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 5, 6, S("a67890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 9, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 9, 1, S("a0bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 9, 2, S("a0bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 10, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 10, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 0, 1, S("a1bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 0, 10, S("a1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 0, 19, S("a1234567890123456789bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 0, 20, S("a12345678901234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 0, 21, S("a12345678901234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 1, 1, S("a2bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 1, 9, S("a234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 1, 18, S("a234567890123456789bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 1, 19, S("a2345678901234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 1, 20, S("a2345678901234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 10, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 10, 1, S("a1bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 10, 5, S("a12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 10, 9, S("a123456789bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 10, 10, S("a1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 10, 11, S("a1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 19, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 19, 1, S("a0bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 19, 2, S("a0bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 20, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 20, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S(""), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S(""), 0, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 0, 0, S("abcde"))));
    return 0;
}

template <class S>
static int test6()
{
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 0, 1, S("ab1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 0, 2, S("ab12cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 0, 4, S("ab1234cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 0, 5, S("ab12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 0, 6, S("ab12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 1, 1, S("ab2cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 1, 2, S("ab23cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 1, 3, S("ab234cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 1, 4, S("ab2345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 1, 5, S("ab2345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 2, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 2, 1, S("ab3cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 2, 2, S("ab34cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 2, 3, S("ab345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 2, 4, S("ab345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 4, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 4, 1, S("ab5cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 4, 2, S("ab5cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 5, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 5, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 0, 1, S("ab1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 0, 5, S("ab12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 0, 9, S("ab123456789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 0, 10, S("ab1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 0, 11, S("ab1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 1, 1, S("ab2cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 1, 4, S("ab2345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 1, 8, S("ab23456789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 1, 9, S("ab234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 1, 10, S("ab234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 5, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 5, 1, S("ab6cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 5, 2, S("ab67cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 5, 4, S("ab6789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 5, 5, S("ab67890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 5, 6, S("ab67890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 9, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 9, 1, S("ab0cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 9, 2, S("ab0cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 10, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 10, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 0, 1, S("ab1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 0, 10, S("ab1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 0, 19, S("ab1234567890123456789cde"))));
    return 0;
}

template <class S>
static int test7()
{
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 0, 20, S("ab12345678901234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 0, 21, S("ab12345678901234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 1, 1, S("ab2cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 1, 9, S("ab234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 1, 18, S("ab234567890123456789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 1, 19, S("ab2345678901234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 1, 20, S("ab2345678901234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 10, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 10, 1, S("ab1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 10, 5, S("ab12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 10, 9, S("ab123456789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 10, 10, S("ab1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 10, 11, S("ab1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 19, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 19, 1, S("ab0cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 19, 2, S("ab0cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 20, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 20, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S(""), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S(""), 0, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 0, 1, S("abcd1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 0, 2, S("abcd12e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 0, 4, S("abcd1234e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 0, 5, S("abcd12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 0, 6, S("abcd12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 1, 1, S("abcd2e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 1, 2, S("abcd23e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 1, 3, S("abcd234e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 1, 4, S("abcd2345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 1, 5, S("abcd2345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 2, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 2, 1, S("abcd3e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 2, 2, S("abcd34e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 2, 3, S("abcd345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 2, 4, S("abcd345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 4, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 4, 1, S("abcd5e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 4, 2, S("abcd5e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 5, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 5, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 0, 1, S("abcd1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 0, 5, S("abcd12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 0, 9, S("abcd123456789e"))));
    return 0;
}

template <class S>
static int test8()
{
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 0, 10, S("abcd1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 0, 11, S("abcd1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 1, 1, S("abcd2e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 1, 4, S("abcd2345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 1, 8, S("abcd23456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 1, 9, S("abcd234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 1, 10, S("abcd234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 5, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 5, 1, S("abcd6e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 5, 2, S("abcd67e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 5, 4, S("abcd6789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 5, 5, S("abcd67890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 5, 6, S("abcd67890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 9, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 9, 1, S("abcd0e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 9, 2, S("abcd0e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 10, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 10, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 0, 1, S("abcd1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 0, 10, S("abcd1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 0, 19, S("abcd1234567890123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 0, 20, S("abcd12345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 0, 21, S("abcd12345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 1, 1, S("abcd2e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 1, 9, S("abcd234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 1, 18, S("abcd234567890123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 1, 19, S("abcd2345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 1, 20, S("abcd2345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 10, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 10, 1, S("abcd1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 10, 5, S("abcd12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 10, 9, S("abcd123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 10, 10, S("abcd1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 10, 11, S("abcd1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 19, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 19, 1, S("abcd0e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 19, 2, S("abcd0e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 20, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 20, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S(""), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S(""), 0, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 0, 1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 0, 2, S("abcde12"))));
    return 0;
}

template <class S>
static int test9()
{
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 0, 4, S("abcde1234"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 0, 5, S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 0, 6, S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 1, 1, S("abcde2"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 1, 2, S("abcde23"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 1, 3, S("abcde234"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 1, 4, S("abcde2345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 1, 5, S("abcde2345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 2, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 2, 1, S("abcde3"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 2, 2, S("abcde34"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 2, 3, S("abcde345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 2, 4, S("abcde345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 4, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 4, 1, S("abcde5"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 4, 2, S("abcde5"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 5, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 5, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 0, 1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 0, 5, S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 0, 9, S("abcde123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 0, 10, S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 0, 11, S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 1, 1, S("abcde2"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 1, 4, S("abcde2345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 1, 8, S("abcde23456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 1, 9, S("abcde234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 1, 10, S("abcde234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 5, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 5, 1, S("abcde6"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 5, 2, S("abcde67"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 5, 4, S("abcde6789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 5, 5, S("abcde67890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 5, 6, S("abcde67890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 9, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 9, 1, S("abcde0"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 9, 2, S("abcde0"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 10, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 10, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 0, 1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 0, 10, S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 0, 19, S("abcde1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 0, 20, S("abcde12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 0, 21, S("abcde12345678901234567890"))));
    return 0;
}

template <class S>
static int test10()
{
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 1, 1, S("abcde2"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 1, 9, S("abcde234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 1, 18, S("abcde234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 1, 19, S("abcde2345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 1, 20, S("abcde2345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 10, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 10, 1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 10, 5, S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 10, 9, S("abcde123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 10, 10, S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 10, 11, S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 19, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 19, 1, S("abcde0"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 19, 2, S("abcde0"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 20, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 20, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S(""), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S(""), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 0, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 0, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 0, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 0, 6, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 1, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 1, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 1, 3, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 1, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 1, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 2, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 2, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 2, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 2, 3, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 2, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 4, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 4, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 4, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 5, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 5, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 0, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 0, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 0, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 0, 11, S("can't happen"))));
    return 0;
}

template <class S>
static int test11()
{
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 1, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 1, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 1, 8, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 1, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 1, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 5, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 5, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 5, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 5, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 5, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 5, 6, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 9, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 9, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 9, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 10, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 10, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 0, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 0, 19, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 0, 20, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 0, 21, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 1, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 1, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 1, 18, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 1, 19, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 1, 20, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 10, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 10, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 10, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 10, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 10, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 10, 11, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 19, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 19, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 19, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 20, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 20, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S(""), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S(""), 0, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 0, 1, S("1abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 0, 2, S("12abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 0, 4, S("1234abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 0, 5, S("12345abcdefghij"))));
    return 0;
}

template <class S>
static int test12()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 0, 6, S("12345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 1, 1, S("2abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 1, 2, S("23abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 1, 3, S("234abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 1, 4, S("2345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 1, 5, S("2345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 2, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 2, 1, S("3abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 2, 2, S("34abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 2, 3, S("345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 2, 4, S("345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 4, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 4, 1, S("5abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 4, 2, S("5abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 5, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 5, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 0, 1, S("1abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 0, 5, S("12345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 0, 9, S("123456789abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 0, 10, S("1234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 0, 11, S("1234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 1, 1, S("2abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 1, 4, S("2345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 1, 8, S("23456789abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 1, 9, S("234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 1, 10, S("234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 5, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 5, 1, S("6abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 5, 2, S("67abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 5, 4, S("6789abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 5, 5, S("67890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 5, 6, S("67890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 9, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 9, 1, S("0abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 9, 2, S("0abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 10, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 10, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 0, 1, S("1abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 0, 10, S("1234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 0, 19, S("1234567890123456789abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 0, 20, S("12345678901234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 0, 21, S("12345678901234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 1, 1, S("2abcdefghij"))));
    return 0;
}

template <class S>
static int test13()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 1, 9, S("234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 1, 18, S("234567890123456789abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 1, 19, S("2345678901234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 1, 20, S("2345678901234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 10, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 10, 1, S("1abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 10, 5, S("12345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 10, 9, S("123456789abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 10, 10, S("1234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 10, 11, S("1234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 19, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 19, 1, S("0abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 19, 2, S("0abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 20, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 20, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S(""), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S(""), 0, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 0, 1, S("a1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 0, 2, S("a12bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 0, 4, S("a1234bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 0, 5, S("a12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 0, 6, S("a12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 1, 1, S("a2bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 1, 2, S("a23bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 1, 3, S("a234bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 1, 4, S("a2345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 1, 5, S("a2345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 2, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 2, 1, S("a3bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 2, 2, S("a34bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 2, 3, S("a345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 2, 4, S("a345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 4, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 4, 1, S("a5bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 4, 2, S("a5bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 5, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 5, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 0, 1, S("a1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 0, 5, S("a12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 0, 9, S("a123456789bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 0, 10, S("a1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 0, 11, S("a1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 1, 1, S("a2bcdefghij"))));
    return 0;
}

template <class S>
static int test14()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 1, 4, S("a2345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 1, 8, S("a23456789bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 1, 9, S("a234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 1, 10, S("a234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 5, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 5, 1, S("a6bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 5, 2, S("a67bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 5, 4, S("a6789bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 5, 5, S("a67890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 5, 6, S("a67890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 9, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 9, 1, S("a0bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 9, 2, S("a0bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 10, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 10, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 0, 1, S("a1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 0, 10, S("a1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 0, 19, S("a1234567890123456789bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 0, 20, S("a12345678901234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 0, 21, S("a12345678901234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 1, 1, S("a2bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 1, 9, S("a234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 1, 18, S("a234567890123456789bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 1, 19, S("a2345678901234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 1, 20, S("a2345678901234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 10, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 10, 1, S("a1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 10, 5, S("a12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 10, 9, S("a123456789bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 10, 10, S("a1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 10, 11, S("a1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 19, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 19, 1, S("a0bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 19, 2, S("a0bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 20, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 20, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S(""), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S(""), 0, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 0, 1, S("abcde1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 0, 2, S("abcde12fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 0, 4, S("abcde1234fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 0, 5, S("abcde12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 0, 6, S("abcde12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 1, 0, S("abcdefghij"))));
    return 0;
}

template <class S>
static int test15()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 1, 1, S("abcde2fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 1, 2, S("abcde23fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 1, 3, S("abcde234fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 1, 4, S("abcde2345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 1, 5, S("abcde2345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 2, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 2, 1, S("abcde3fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 2, 2, S("abcde34fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 2, 3, S("abcde345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 2, 4, S("abcde345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 4, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 4, 1, S("abcde5fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 4, 2, S("abcde5fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 5, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 5, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 0, 1, S("abcde1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 0, 5, S("abcde12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 0, 9, S("abcde123456789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 0, 10, S("abcde1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 0, 11, S("abcde1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 1, 1, S("abcde2fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 1, 4, S("abcde2345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 1, 8, S("abcde23456789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 1, 9, S("abcde234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 1, 10, S("abcde234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 5, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 5, 1, S("abcde6fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 5, 2, S("abcde67fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 5, 4, S("abcde6789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 5, 5, S("abcde67890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 5, 6, S("abcde67890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 9, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 9, 1, S("abcde0fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 9, 2, S("abcde0fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 10, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 10, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 0, 1, S("abcde1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 0, 10, S("abcde1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 0, 19, S("abcde1234567890123456789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 0, 20, S("abcde12345678901234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 0, 21, S("abcde12345678901234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 1, 1, S("abcde2fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 1, 9, S("abcde234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 1, 18, S("abcde234567890123456789fghij"))));
    return 0;
}

template <class S>
static int test16()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 1, 19, S("abcde2345678901234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 1, 20, S("abcde2345678901234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 10, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 10, 1, S("abcde1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 10, 5, S("abcde12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 10, 9, S("abcde123456789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 10, 10, S("abcde1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 10, 11, S("abcde1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 19, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 19, 1, S("abcde0fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 19, 2, S("abcde0fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 20, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 20, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S(""), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S(""), 0, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 0, 1, S("abcdefghi1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 0, 2, S("abcdefghi12j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 0, 4, S("abcdefghi1234j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 0, 5, S("abcdefghi12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 0, 6, S("abcdefghi12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 1, 1, S("abcdefghi2j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 1, 2, S("abcdefghi23j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 1, 3, S("abcdefghi234j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 1, 4, S("abcdefghi2345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 1, 5, S("abcdefghi2345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 2, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 2, 1, S("abcdefghi3j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 2, 2, S("abcdefghi34j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 2, 3, S("abcdefghi345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 2, 4, S("abcdefghi345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 4, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 4, 1, S("abcdefghi5j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 4, 2, S("abcdefghi5j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 5, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 5, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 0, 1, S("abcdefghi1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 0, 5, S("abcdefghi12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 0, 9, S("abcdefghi123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 0, 10, S("abcdefghi1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 0, 11, S("abcdefghi1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 1, 1, S("abcdefghi2j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 1, 4, S("abcdefghi2345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 1, 8, S("abcdefghi23456789j"))));
    return 0;
}

template <class S>
static int test17()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 1, 9, S("abcdefghi234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 1, 10, S("abcdefghi234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 5, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 5, 1, S("abcdefghi6j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 5, 2, S("abcdefghi67j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 5, 4, S("abcdefghi6789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 5, 5, S("abcdefghi67890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 5, 6, S("abcdefghi67890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 9, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 9, 1, S("abcdefghi0j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 9, 2, S("abcdefghi0j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 10, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 10, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 0, 1, S("abcdefghi1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 0, 10, S("abcdefghi1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 0, 19, S("abcdefghi1234567890123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 0, 20, S("abcdefghi12345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 0, 21, S("abcdefghi12345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 1, 1, S("abcdefghi2j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 1, 9, S("abcdefghi234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 1, 18, S("abcdefghi234567890123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 1, 19, S("abcdefghi2345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 1, 20, S("abcdefghi2345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 10, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 10, 1, S("abcdefghi1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 10, 5, S("abcdefghi12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 10, 9, S("abcdefghi123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 10, 10, S("abcdefghi1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 10, 11, S("abcdefghi1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 19, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 19, 1, S("abcdefghi0j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 19, 2, S("abcdefghi0j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 20, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 20, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S(""), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S(""), 0, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 0, 1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 0, 2, S("abcdefghij12"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 0, 4, S("abcdefghij1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 0, 5, S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 0, 6, S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 1, 1, S("abcdefghij2"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 1, 2, S("abcdefghij23"))));
    return 0;
}

template <class S>
static int test18()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 1, 3, S("abcdefghij234"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 1, 4, S("abcdefghij2345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 1, 5, S("abcdefghij2345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 2, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 2, 1, S("abcdefghij3"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 2, 2, S("abcdefghij34"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 2, 3, S("abcdefghij345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 2, 4, S("abcdefghij345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 4, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 4, 1, S("abcdefghij5"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 4, 2, S("abcdefghij5"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 5, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 5, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 0, 1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 0, 5, S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 0, 9, S("abcdefghij123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 0, 10, S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 0, 11, S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 1, 1, S("abcdefghij2"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 1, 4, S("abcdefghij2345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 1, 8, S("abcdefghij23456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 1, 9, S("abcdefghij234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 1, 10, S("abcdefghij234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 5, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 5, 1, S("abcdefghij6"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 5, 2, S("abcdefghij67"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 5, 4, S("abcdefghij6789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 5, 5, S("abcdefghij67890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 5, 6, S("abcdefghij67890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 9, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 9, 1, S("abcdefghij0"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 9, 2, S("abcdefghij0"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 10, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 10, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 0, 1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 0, 10, S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 0, 19, S("abcdefghij1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 0, 20, S("abcdefghij12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 0, 21, S("abcdefghij12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 1, 1, S("abcdefghij2"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 1, 9, S("abcdefghij234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 1, 18, S("abcdefghij234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 1, 19, S("abcdefghij2345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 1, 20, S("abcdefghij2345678901234567890"))));
    return 0;
}

template <class S>
static int test19()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 10, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 10, 1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 10, 5, S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 10, 9, S("abcdefghij123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 10, 10, S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 10, 11, S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 19, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 19, 1, S("abcdefghij0"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 19, 2, S("abcdefghij0"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 20, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 20, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S(""), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S(""), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 0, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 0, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 0, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 0, 6, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 1, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 1, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 1, 3, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 1, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 1, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 2, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 2, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 2, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 2, 3, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 2, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 4, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 4, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 4, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 5, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 5, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 0, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 0, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 0, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 0, 11, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 1, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 1, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 1, 8, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 1, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 1, 10, S("can't happen"))));
    return 0;
}

template <class S>
static int test20()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 5, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 5, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 5, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 5, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 5, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 5, 6, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 9, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 9, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 9, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 10, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 10, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 0, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 0, 19, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 0, 20, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 0, 21, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 1, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 1, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 1, 18, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 1, 19, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 1, 20, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 10, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 10, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 10, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 10, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 10, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 10, 11, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 19, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 19, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 19, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 20, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 20, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S(""), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S(""), 0, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 0, 1, S("1abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 0, 2, S("12abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 0, 4, S("1234abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 0, 5, S("12345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 0, 6, S("12345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 1, 1, S("2abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 1, 2, S("23abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 1, 3, S("234abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 1, 4, S("2345abcdefghijklmnopqrst"))));
    return 0;
}

template <class S>
static int test21()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 1, 5, S("2345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 2, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 2, 1, S("3abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 2, 2, S("34abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 2, 3, S("345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 2, 4, S("345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 4, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 4, 1, S("5abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 4, 2, S("5abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 5, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 5, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 0, 1, S("1abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 0, 5, S("12345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 0, 9, S("123456789abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 0, 10, S("1234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 0, 11, S("1234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 1, 1, S("2abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 1, 4, S("2345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 1, 8, S("23456789abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 1, 9, S("234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 1, 10, S("234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 5, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 5, 1, S("6abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 5, 2, S("67abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 5, 4, S("6789abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 5, 5, S("67890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 5, 6, S("67890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 9, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 9, 1, S("0abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 9, 2, S("0abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 10, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 10, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 0, 1, S("1abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 0, 10, S("1234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 0, 19, S("1234567890123456789abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 0, 20, S("12345678901234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 0, 21, S("12345678901234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 1, 1, S("2abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 1, 9, S("234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 1, 18, S("234567890123456789abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 1, 19, S("2345678901234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 1, 20, S("2345678901234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 10, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 10, 1, S("1abcdefghijklmnopqrst"))));
    return 0;
}

template <class S>
static int test22()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 10, 5, S("12345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 10, 9, S("123456789abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 10, 10, S("1234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 10, 11, S("1234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 19, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 19, 1, S("0abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 19, 2, S("0abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 20, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 20, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S(""), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S(""), 0, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 0, 1, S("a1bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 0, 2, S("a12bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 0, 4, S("a1234bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 0, 5, S("a12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 0, 6, S("a12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 1, 1, S("a2bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 1, 2, S("a23bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 1, 3, S("a234bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 1, 4, S("a2345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 1, 5, S("a2345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 2, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 2, 1, S("a3bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 2, 2, S("a34bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 2, 3, S("a345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 2, 4, S("a345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 4, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 4, 1, S("a5bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 4, 2, S("a5bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 5, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 5, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 0, 1, S("a1bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 0, 5, S("a12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 0, 9, S("a123456789bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 0, 10, S("a1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 0, 11, S("a1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 1, 1, S("a2bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 1, 4, S("a2345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 1, 8, S("a23456789bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 1, 9, S("a234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 1, 10, S("a234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 5, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 5, 1, S("a6bcdefghijklmnopqrst"))));
    return 0;
}

template <class S>
static int test23()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 5, 2, S("a67bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 5, 4, S("a6789bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 5, 5, S("a67890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 5, 6, S("a67890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 9, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 9, 1, S("a0bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 9, 2, S("a0bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 10, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 10, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 0, 1, S("a1bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 0, 10, S("a1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 0, 19, S("a1234567890123456789bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 0, 20, S("a12345678901234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 0, 21, S("a12345678901234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 1, 1, S("a2bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 1, 9, S("a234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 1, 18, S("a234567890123456789bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 1, 19, S("a2345678901234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 1, 20, S("a2345678901234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 10, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 10, 1, S("a1bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 10, 5, S("a12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 10, 9, S("a123456789bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 10, 10, S("a1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 10, 11, S("a1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 19, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 19, 1, S("a0bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 19, 2, S("a0bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 20, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 20, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S(""), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S(""), 0, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 0, 1, S("abcdefghij1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 0, 2, S("abcdefghij12klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 0, 4, S("abcdefghij1234klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 0, 5, S("abcdefghij12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 0, 6, S("abcdefghij12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 1, 1, S("abcdefghij2klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 1, 2, S("abcdefghij23klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 1, 3, S("abcdefghij234klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 1, 4, S("abcdefghij2345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 1, 5, S("abcdefghij2345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 2, 0, S("abcdefghijklmnopqrst"))));
    return 0;
}

template <class S>
static int test24()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 2, 1, S("abcdefghij3klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 2, 2, S("abcdefghij34klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 2, 3, S("abcdefghij345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 2, 4, S("abcdefghij345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 4, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 4, 1, S("abcdefghij5klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 4, 2, S("abcdefghij5klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 5, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 5, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 0, 1, S("abcdefghij1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 0, 5, S("abcdefghij12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 0, 9, S("abcdefghij123456789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 0, 10, S("abcdefghij1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 0, 11, S("abcdefghij1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 1, 1, S("abcdefghij2klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 1, 4, S("abcdefghij2345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 1, 8, S("abcdefghij23456789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 1, 9, S("abcdefghij234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 1, 10, S("abcdefghij234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 5, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 5, 1, S("abcdefghij6klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 5, 2, S("abcdefghij67klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 5, 4, S("abcdefghij6789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 5, 5, S("abcdefghij67890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 5, 6, S("abcdefghij67890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 9, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 9, 1, S("abcdefghij0klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 9, 2, S("abcdefghij0klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 10, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 10, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 0, 1, S("abcdefghij1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 0, 10, S("abcdefghij1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 0, 19, S("abcdefghij1234567890123456789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 0, 20, S("abcdefghij12345678901234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 0, 21, S("abcdefghij12345678901234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 1, 1, S("abcdefghij2klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 1, 9, S("abcdefghij234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 1, 18, S("abcdefghij234567890123456789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 1, 19, S("abcdefghij2345678901234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 1, 20, S("abcdefghij2345678901234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 10, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 10, 1, S("abcdefghij1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 10, 5, S("abcdefghij12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 10, 9, S("abcdefghij123456789klmnopqrst"))));
    return 0;
}

template <class S>
static int test25()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 10, 10, S("abcdefghij1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 10, 11, S("abcdefghij1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 19, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 19, 1, S("abcdefghij0klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 19, 2, S("abcdefghij0klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 20, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 20, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S(""), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S(""), 0, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 0, 1, S("abcdefghijklmnopqrs1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 0, 2, S("abcdefghijklmnopqrs12t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 0, 4, S("abcdefghijklmnopqrs1234t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 0, 5, S("abcdefghijklmnopqrs12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 0, 6, S("abcdefghijklmnopqrs12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 1, 1, S("abcdefghijklmnopqrs2t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 1, 2, S("abcdefghijklmnopqrs23t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 1, 3, S("abcdefghijklmnopqrs234t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 1, 4, S("abcdefghijklmnopqrs2345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 1, 5, S("abcdefghijklmnopqrs2345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 2, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 2, 1, S("abcdefghijklmnopqrs3t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 2, 2, S("abcdefghijklmnopqrs34t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 2, 3, S("abcdefghijklmnopqrs345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 2, 4, S("abcdefghijklmnopqrs345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 4, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 4, 1, S("abcdefghijklmnopqrs5t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 4, 2, S("abcdefghijklmnopqrs5t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 5, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 5, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 0, 1, S("abcdefghijklmnopqrs1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 0, 5, S("abcdefghijklmnopqrs12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 0, 9, S("abcdefghijklmnopqrs123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 0, 10, S("abcdefghijklmnopqrs1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 0, 11, S("abcdefghijklmnopqrs1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 1, 1, S("abcdefghijklmnopqrs2t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 1, 4, S("abcdefghijklmnopqrs2345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 1, 8, S("abcdefghijklmnopqrs23456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 1, 9, S("abcdefghijklmnopqrs234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 1, 10, S("abcdefghijklmnopqrs234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 5, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 5, 1, S("abcdefghijklmnopqrs6t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 5, 2, S("abcdefghijklmnopqrs67t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 5, 4, S("abcdefghijklmnopqrs6789t"))));
    return 0;
}

template <class S>
static int test26()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 5, 5, S("abcdefghijklmnopqrs67890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 5, 6, S("abcdefghijklmnopqrs67890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 9, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 9, 1, S("abcdefghijklmnopqrs0t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 9, 2, S("abcdefghijklmnopqrs0t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 10, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 10, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 0, 1, S("abcdefghijklmnopqrs1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 0, 10, S("abcdefghijklmnopqrs1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 0, 19, S("abcdefghijklmnopqrs1234567890123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 0, 20, S("abcdefghijklmnopqrs12345678901234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 0, 21, S("abcdefghijklmnopqrs12345678901234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 1, 1, S("abcdefghijklmnopqrs2t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 1, 9, S("abcdefghijklmnopqrs234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 1, 18, S("abcdefghijklmnopqrs234567890123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 1, 19, S("abcdefghijklmnopqrs2345678901234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 1, 20, S("abcdefghijklmnopqrs2345678901234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 10, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 10, 1, S("abcdefghijklmnopqrs1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 10, 5, S("abcdefghijklmnopqrs12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 10, 9, S("abcdefghijklmnopqrs123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 10, 10, S("abcdefghijklmnopqrs1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 10, 11, S("abcdefghijklmnopqrs1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 19, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 19, 1, S("abcdefghijklmnopqrs0t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 19, 2, S("abcdefghijklmnopqrs0t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 20, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 20, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S(""), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S(""), 0, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 0, 1, S("abcdefghijklmnopqrst1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 0, 2, S("abcdefghijklmnopqrst12"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 0, 4, S("abcdefghijklmnopqrst1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 0, 5, S("abcdefghijklmnopqrst12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 0, 6, S("abcdefghijklmnopqrst12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 1, 1, S("abcdefghijklmnopqrst2"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 1, 2, S("abcdefghijklmnopqrst23"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 1, 3, S("abcdefghijklmnopqrst234"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 1, 4, S("abcdefghijklmnopqrst2345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 1, 5, S("abcdefghijklmnopqrst2345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 2, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 2, 1, S("abcdefghijklmnopqrst3"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 2, 2, S("abcdefghijklmnopqrst34"))));
    return 0;
}

template <class S>
static int test27()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 2, 3, S("abcdefghijklmnopqrst345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 2, 4, S("abcdefghijklmnopqrst345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 4, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 4, 1, S("abcdefghijklmnopqrst5"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 4, 2, S("abcdefghijklmnopqrst5"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 5, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 5, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 0, 1, S("abcdefghijklmnopqrst1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 0, 5, S("abcdefghijklmnopqrst12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 0, 9, S("abcdefghijklmnopqrst123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 0, 10, S("abcdefghijklmnopqrst1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 0, 11, S("abcdefghijklmnopqrst1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 1, 1, S("abcdefghijklmnopqrst2"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 1, 4, S("abcdefghijklmnopqrst2345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 1, 8, S("abcdefghijklmnopqrst23456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 1, 9, S("abcdefghijklmnopqrst234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 1, 10, S("abcdefghijklmnopqrst234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 5, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 5, 1, S("abcdefghijklmnopqrst6"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 5, 2, S("abcdefghijklmnopqrst67"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 5, 4, S("abcdefghijklmnopqrst6789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 5, 5, S("abcdefghijklmnopqrst67890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 5, 6, S("abcdefghijklmnopqrst67890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 9, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 9, 1, S("abcdefghijklmnopqrst0"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 9, 2, S("abcdefghijklmnopqrst0"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 10, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 10, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 0, 1, S("abcdefghijklmnopqrst1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 0, 10, S("abcdefghijklmnopqrst1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 0, 19, S("abcdefghijklmnopqrst1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 0, 20, S("abcdefghijklmnopqrst12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 0, 21, S("abcdefghijklmnopqrst12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 1, 1, S("abcdefghijklmnopqrst2"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 1, 9, S("abcdefghijklmnopqrst234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 1, 18, S("abcdefghijklmnopqrst234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 1, 19, S("abcdefghijklmnopqrst2345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 1, 20, S("abcdefghijklmnopqrst2345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 10, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 10, 1, S("abcdefghijklmnopqrst1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 10, 5, S("abcdefghijklmnopqrst12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 10, 9, S("abcdefghijklmnopqrst123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 10, 10, S("abcdefghijklmnopqrst1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 10, 11, S("abcdefghijklmnopqrst1234567890"))));
    return 0;
}

template <class S>
static int test28()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 19, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 19, 1, S("abcdefghijklmnopqrst0"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 19, 2, S("abcdefghijklmnopqrst0"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 20, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 20, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("12345678901234567890"), 21, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S(""), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S(""), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 0, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 0, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 0, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 0, 6, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 1, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 1, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 1, 3, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 1, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 1, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 2, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 2, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 2, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 2, 3, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 2, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 4, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 4, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 4, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 5, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 5, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 0, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 0, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 0, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 0, 11, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 1, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 1, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 1, 8, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 1, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 1, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 5, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 5, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 5, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 5, 4, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 5, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 5, 6, S("can't happen"))));
    return 0;
}

template <class S>
static int test29()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 9, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 9, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 9, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 10, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 10, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("1234567890"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 0, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 0, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 0, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 0, 19, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 0, 20, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 0, 21, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 1, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 1, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 1, 18, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 1, 19, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 1, 20, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 10, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 10, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 10, 5, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 10, 9, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 10, 10, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 10, 11, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 19, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 19, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 19, 2, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 20, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 20, 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("12345678901234567890"), 21, 0, S("can't happen"))));
    return 0;
}

template <class S>
static int test30()
{
    TC_ASSERT_FUNC((test_npos(S(""), 0, S("12345678901234567890"),  0, S("12345678901234567890"))));
    TC_ASSERT_FUNC((test_npos(S(""), 0, S("12345678901234567890"),  1, S( "2345678901234567890"))));
    TC_ASSERT_FUNC((test_npos(S(""), 0, S("12345678901234567890"),  2, S(  "345678901234567890"))));
    TC_ASSERT_FUNC((test_npos(S(""), 0, S("12345678901234567890"),  3, S(   "45678901234567890"))));
    TC_ASSERT_FUNC((test_npos(S(""), 0, S("12345678901234567890"),  5, S(     "678901234567890"))));
    TC_ASSERT_FUNC((test_npos(S(""), 0, S("12345678901234567890"), 10, S(          "1234567890"))));
    TC_ASSERT_FUNC((test_npos(S(""), 0, S("12345678901234567890"), 21, S("can't happen"))));
    TC_ASSERT_FUNC((test_npos(S("abcdefghijklmnopqrst"), 10, S("12345"), 0, S("abcdefghij12345klmnopqrst"))));
    TC_ASSERT_FUNC((test_npos(S("abcdefghijklmnopqrst"), 10, S("12345"), 1, S("abcdefghij2345klmnopqrst"))));
    TC_ASSERT_FUNC((test_npos(S("abcdefghijklmnopqrst"), 10, S("12345"), 3, S("abcdefghij45klmnopqrst"))));
    TC_ASSERT_FUNC((test_npos(S("abcdefghijklmnopqrst"), 10, S("12345"), 5, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test_npos(S("abcdefghijklmnopqrst"), 10, S("12345"), 6, S("can't happen"))));
    return 0;
}

int tc_libcxx_strings_string_insert_size_string_size_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test0<S>()));
    TC_ASSERT_FUNC((test1<S>()));
    TC_ASSERT_FUNC((test2<S>()));
    TC_ASSERT_FUNC((test3<S>()));
    TC_ASSERT_FUNC((test4<S>()));
    TC_ASSERT_FUNC((test5<S>()));
    TC_ASSERT_FUNC((test6<S>()));
    TC_ASSERT_FUNC((test7<S>()));
    TC_ASSERT_FUNC((test8<S>()));
    TC_ASSERT_FUNC((test9<S>()));
    TC_ASSERT_FUNC((test10<S>()));
    TC_ASSERT_FUNC((test11<S>()));
    TC_ASSERT_FUNC((test12<S>()));
    TC_ASSERT_FUNC((test13<S>()));
    TC_ASSERT_FUNC((test14<S>()));
    TC_ASSERT_FUNC((test15<S>()));
    TC_ASSERT_FUNC((test16<S>()));
    TC_ASSERT_FUNC((test17<S>()));
    TC_ASSERT_FUNC((test18<S>()));
    TC_ASSERT_FUNC((test19<S>()));
    TC_ASSERT_FUNC((test20<S>()));
    TC_ASSERT_FUNC((test21<S>()));
    TC_ASSERT_FUNC((test22<S>()));
    TC_ASSERT_FUNC((test23<S>()));
    TC_ASSERT_FUNC((test24<S>()));
    TC_ASSERT_FUNC((test25<S>()));
    TC_ASSERT_FUNC((test26<S>()));
    TC_ASSERT_FUNC((test27<S>()));
    TC_ASSERT_FUNC((test28<S>()));
    TC_ASSERT_FUNC((test29<S>()));
    TC_ASSERT_FUNC((test30<S>()));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
