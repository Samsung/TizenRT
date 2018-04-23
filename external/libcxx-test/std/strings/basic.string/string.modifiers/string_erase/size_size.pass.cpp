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
//   erase(size_type pos = 0, size_type n = npos);

#include <string>
#include <stdexcept>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::size_type pos, typename S::size_type n, S expected)
{
    const typename S::size_type old_size = s.size();
    S s0 = s;
    if (pos <= old_size)
    {
        s.erase(pos, n);
        LIBCPP_ASSERT(s.__invariants());
        TC_ASSERT_EXPR(s[s.size()] == typename S::value_type());
        TC_ASSERT_EXPR(s == expected);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            s.erase(pos, n);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
            TC_ASSERT_EXPR(pos > old_size);
            TC_ASSERT_EXPR(s == s0);
        }
    }
#endif
    return 0;
}

template <class S>
static int
test(S s, typename S::size_type pos, S expected)
{
    const typename S::size_type old_size = s.size();
    S s0 = s;
    if (pos <= old_size)
    {
        s.erase(pos);
        LIBCPP_ASSERT(s.__invariants());
        TC_ASSERT_EXPR(s[s.size()] == typename S::value_type());
        TC_ASSERT_EXPR(s == expected);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            s.erase(pos);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
            TC_ASSERT_EXPR(pos > old_size);
            TC_ASSERT_EXPR(s == s0);
        }
    }
#endif
    return 0;
}

template <class S>
static int
test(S s, S expected)
{
    s.erase();
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s[s.size()] == typename S::value_type());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

int tc_libcxx_strings_string_erase_size_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), 0, 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, 1, S(""))));
    TC_ASSERT_FUNC((test(S(""), 1, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, S("bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, S("cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, S("e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 6, S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, S("acde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, S("ade"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, S("ae"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, S("a"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 5, S("a"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, S("abde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, S("abe"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, S("ab"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 4, S("ab"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 2, S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 1, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, S("bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, S("fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, S("j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 11, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, S("acdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, S("afghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, S("aj"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 10, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, S("abcdeghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, S("abcdehij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, S("abcdej"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 6, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 2, S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 1, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 0, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, S("bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, S("klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, S("t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 21, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, S("acdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, S("aklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, S("at"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 20, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, S("abcdefghijlmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, S("abcdefghijpqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, S("abcdefghijt"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 11, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, S("abcdefghijklmnopqrs"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 2, S("abcdefghijklmnopqrs"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 1, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 0, S("can't happen"))));

    TC_ASSERT_FUNC((test(S(""), 0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 1, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("a"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("ab"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("abcdefghijklmnopqrs"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, S("can't happen"))));

    TC_ASSERT_FUNC((test(S(""), S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), S(""))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
