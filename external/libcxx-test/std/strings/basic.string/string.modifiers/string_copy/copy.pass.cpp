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

// size_type copy(charT* s, size_type n, size_type pos = 0) const;

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S str, typename S::value_type* s, typename S::size_type n,
     typename S::size_type pos)
{
    const S& cs = str;
    if (pos <= cs.size())
    {
        typename S::size_type r = cs.copy(s, n, pos);
        typename S::size_type rlen = std::min(n, cs.size() - pos);
        TC_ASSERT_EXPR(r == rlen);
        for (r = 0; r < rlen; ++r)
            TC_ASSERT_EXPR(S::traits_type::eq(cs[pos+r], s[r]));
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            typename S::size_type r = cs.copy(s, n, pos);
            ((void)r); // Prevent unused warning
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
            TC_ASSERT_EXPR(pos > str.size());
        }
    }
#endif
    return 0;
}

int tc_libcxx_strings_string_copy_copy(void)
{
    {
    typedef std::string S;
    char s[50];
    TC_ASSERT_FUNC((test(S(""), s, 0, 0)));
    TC_ASSERT_FUNC((test(S(""), s, 0, 1)));
    TC_ASSERT_FUNC((test(S(""), s, 1, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 0, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 0, 1)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 0, 2)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 0, 4)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 0, 5)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 0, 6)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 1, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 1, 1)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 1, 2)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 1, 4)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 1, 5)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 2, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 2, 1)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 2, 2)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 2, 4)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 4, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 4, 1)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 4, 2)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 5, 0)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 5, 1)));
    TC_ASSERT_FUNC((test(S("abcde"), s, 6, 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 0, 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 0, 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 0, 2)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 0, 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 0, 19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 0, 20)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 0, 21)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 1, 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 1, 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 1, 2)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 1, 9)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 1, 18)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 1, 19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 1, 20)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 2, 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 2, 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 2, 2)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 2, 9)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 2, 17)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 2, 18)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 2, 19)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 10, 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 10, 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 10, 2)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 10, 5)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 10, 9)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 10, 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 10, 11)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 19, 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 19, 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 19, 2)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 20, 0)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 20, 1)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), s, 21, 0)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
