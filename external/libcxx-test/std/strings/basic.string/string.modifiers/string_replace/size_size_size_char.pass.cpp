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
//   replace(size_type pos, size_type n1, size_type n2, charT c);

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::size_type pos, typename S::size_type n1,
     typename S::size_type n2, typename S::value_type c,
     S expected)
{
    const typename S::size_type old_size = s.size();
    S s0 = s;
    if (pos <= old_size)
    {
        s.replace(pos, n1, n2, c);
        LIBCPP_ASSERT(s.__invariants());
        TC_ASSERT_EXPR(s == expected);
        typename S::size_type xlen = std::min(n1, old_size - pos);
        typename S::size_type rlen = n2;
        TC_ASSERT_EXPR(s.size() == old_size - xlen + rlen);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            s.replace(pos, n1, n2, c);
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
static int test0()
{
    TC_ASSERT_FUNC((test(S(""), 0, 0, 0, '2', S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, 5, '2', S("22222"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, 10, '2', S("2222222222"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, 20, '2', S("22222222222222222222"))));
    TC_ASSERT_FUNC((test(S(""), 0, 1, 0, '2', S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, 1, 5, '2', S("22222"))));
    TC_ASSERT_FUNC((test(S(""), 0, 1, 10, '2', S("2222222222"))));
    TC_ASSERT_FUNC((test(S(""), 0, 1, 20, '2', S("22222222222222222222"))));
    TC_ASSERT_FUNC((test(S(""), 1, 0, 0, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, 0, 5, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, 0, 10, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S(""), 1, 0, 20, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, 0, '2', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, 5, '2', S("22222abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, 10, '2', S("2222222222abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, 20, '2', S("22222222222222222222abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, 0, '2', S("bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, 5, '2', S("22222bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, 10, '2', S("2222222222bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, 20, '2', S("22222222222222222222bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, 0, '2', S("cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, 5, '2', S("22222cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, 10, '2', S("2222222222cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, 20, '2', S("22222222222222222222cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, 0, '2', S("e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, 5, '2', S("22222e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, 10, '2', S("2222222222e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, 20, '2', S("22222222222222222222e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, 0, '2', S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, 5, '2', S("22222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, 10, '2', S("2222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, 20, '2', S("22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 6, 0, '2', S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 6, 5, '2', S("22222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 6, 10, '2', S("2222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 6, 20, '2', S("22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, 0, '2', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, 5, '2', S("a22222bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, 10, '2', S("a2222222222bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, 20, '2', S("a22222222222222222222bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, 0, '2', S("acde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, 5, '2', S("a22222cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, 10, '2', S("a2222222222cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, 20, '2', S("a22222222222222222222cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, 0, '2', S("ade"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, 5, '2', S("a22222de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, 10, '2', S("a2222222222de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, 20, '2', S("a22222222222222222222de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, 0, '2', S("ae"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, 5, '2', S("a22222e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, 10, '2', S("a2222222222e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, 20, '2', S("a22222222222222222222e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, 0, '2', S("a"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, 5, '2', S("a22222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, 10, '2', S("a2222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, 20, '2', S("a22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 5, 0, '2', S("a"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 5, 5, '2', S("a22222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 5, 10, '2', S("a2222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 5, 20, '2', S("a22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, 0, '2', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, 5, '2', S("ab22222cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, 10, '2', S("ab2222222222cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, 20, '2', S("ab22222222222222222222cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, 0, '2', S("abde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, 5, '2', S("ab22222de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, 10, '2', S("ab2222222222de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, 20, '2', S("ab22222222222222222222de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, 0, '2', S("abe"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, 5, '2', S("ab22222e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, 10, '2', S("ab2222222222e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, 20, '2', S("ab22222222222222222222e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, 0, '2', S("ab"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, 5, '2', S("ab22222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, 10, '2', S("ab2222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, 20, '2', S("ab22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 4, 0, '2', S("ab"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 4, 5, '2', S("ab22222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 4, 10, '2', S("ab2222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 4, 20, '2', S("ab22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, 0, '2', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, 5, '2', S("abcd22222e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, 10, '2', S("abcd2222222222e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, 20, '2', S("abcd22222222222222222222e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, 0, '2', S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, 5, '2', S("abcd22222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, 10, '2', S("abcd2222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, 20, '2', S("abcd22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 2, 0, '2', S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 2, 5, '2', S("abcd22222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 2, 10, '2', S("abcd2222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 2, 20, '2', S("abcd22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, 0, '2', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, 5, '2', S("abcde22222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, 10, '2', S("abcde2222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, 20, '2', S("abcde22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 1, 0, '2', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 1, 5, '2', S("abcde22222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 1, 10, '2', S("abcde2222222222"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 1, 20, '2', S("abcde22222222222222222222"))));
    return 0;
}

template <class S>
static int test1()
{
    TC_ASSERT_FUNC((test(S("abcde"), 6, 0, 0, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, 0, 5, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, 0, 10, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcde"), 6, 0, 20, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, 0, '2', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, 5, '2', S("22222abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, 10, '2', S("2222222222abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, 20, '2', S("22222222222222222222abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, 0, '2', S("bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, 5, '2', S("22222bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, 10, '2', S("2222222222bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, 20, '2', S("22222222222222222222bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, 0, '2', S("fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, 5, '2', S("22222fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, 10, '2', S("2222222222fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, 20, '2', S("22222222222222222222fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, 0, '2', S("j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, 5, '2', S("22222j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, 10, '2', S("2222222222j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, 20, '2', S("22222222222222222222j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, 0, '2', S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, 5, '2', S("22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, 10, '2', S("2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, 20, '2', S("22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 11, 0, '2', S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 11, 5, '2', S("22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 11, 10, '2', S("2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 11, 20, '2', S("22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, 0, '2', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, 5, '2', S("a22222bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, 10, '2', S("a2222222222bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, 20, '2', S("a22222222222222222222bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, 0, '2', S("acdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, 5, '2', S("a22222cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, 10, '2', S("a2222222222cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, 20, '2', S("a22222222222222222222cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, 0, '2', S("afghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, 5, '2', S("a22222fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, 10, '2', S("a2222222222fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, 20, '2', S("a22222222222222222222fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, 0, '2', S("aj"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, 5, '2', S("a22222j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, 10, '2', S("a2222222222j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, 20, '2', S("a22222222222222222222j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, 0, '2', S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, 5, '2', S("a22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, 10, '2', S("a2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, 20, '2', S("a22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 10, 0, '2', S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 10, 5, '2', S("a22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 10, 10, '2', S("a2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 10, 20, '2', S("a22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, 0, '2', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, 5, '2', S("abcde22222fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, 10, '2', S("abcde2222222222fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, 20, '2', S("abcde22222222222222222222fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, 0, '2', S("abcdeghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, 5, '2', S("abcde22222ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, 10, '2', S("abcde2222222222ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, 20, '2', S("abcde22222222222222222222ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, 0, '2', S("abcdehij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, 5, '2', S("abcde22222hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, 10, '2', S("abcde2222222222hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, 20, '2', S("abcde22222222222222222222hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, 0, '2', S("abcdej"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, 5, '2', S("abcde22222j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, 10, '2', S("abcde2222222222j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, 20, '2', S("abcde22222222222222222222j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, 0, '2', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, 5, '2', S("abcde22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, 10, '2', S("abcde2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, 20, '2', S("abcde22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 6, 0, '2', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 6, 5, '2', S("abcde22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 6, 10, '2', S("abcde2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 6, 20, '2', S("abcde22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, 0, '2', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, 5, '2', S("abcdefghi22222j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, 10, '2', S("abcdefghi2222222222j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, 20, '2', S("abcdefghi22222222222222222222j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, 0, '2', S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, 5, '2', S("abcdefghi22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, 10, '2', S("abcdefghi2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, 20, '2', S("abcdefghi22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 2, 0, '2', S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 2, 5, '2', S("abcdefghi22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 2, 10, '2', S("abcdefghi2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 2, 20, '2', S("abcdefghi22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, 0, '2', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, 5, '2', S("abcdefghij22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, 10, '2', S("abcdefghij2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, 20, '2', S("abcdefghij22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 1, 0, '2', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 1, 5, '2', S("abcdefghij22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 1, 10, '2', S("abcdefghij2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 1, 20, '2', S("abcdefghij22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 0, 0, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 0, 5, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 0, 10, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 11, 0, 20, '2', S("can't happen"))));
    return 0;
}

template <class S>
static int test2()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, 0, '2', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, 5, '2', S("22222abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, 10, '2', S("2222222222abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, 20, '2', S("22222222222222222222abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, 0, '2', S("bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, 5, '2', S("22222bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, 10, '2', S("2222222222bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, 20, '2', S("22222222222222222222bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, 0, '2', S("klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, 5, '2', S("22222klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, 10, '2', S("2222222222klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, 20, '2', S("22222222222222222222klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, 0, '2', S("t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, 5, '2', S("22222t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, 10, '2', S("2222222222t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, 20, '2', S("22222222222222222222t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, 0, '2', S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, 5, '2', S("22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, 10, '2', S("2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, 20, '2', S("22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 21, 0, '2', S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 21, 5, '2', S("22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 21, 10, '2', S("2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 21, 20, '2', S("22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, 0, '2', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, 5, '2', S("a22222bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, 10, '2', S("a2222222222bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, 20, '2', S("a22222222222222222222bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, 0, '2', S("acdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, 5, '2', S("a22222cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, 10, '2', S("a2222222222cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, 20, '2', S("a22222222222222222222cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, 0, '2', S("aklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, 5, '2', S("a22222klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, 10, '2', S("a2222222222klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, 20, '2', S("a22222222222222222222klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, 0, '2', S("at"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, 5, '2', S("a22222t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, 10, '2', S("a2222222222t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, 20, '2', S("a22222222222222222222t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, 0, '2', S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, 5, '2', S("a22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, 10, '2', S("a2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, 20, '2', S("a22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 20, 0, '2', S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 20, 5, '2', S("a22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 20, 10, '2', S("a2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 20, 20, '2', S("a22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, 0, '2', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, 5, '2', S("abcdefghij22222klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, 10, '2', S("abcdefghij2222222222klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, 20, '2', S("abcdefghij22222222222222222222klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, 0, '2', S("abcdefghijlmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, 5, '2', S("abcdefghij22222lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, 10, '2', S("abcdefghij2222222222lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, 20, '2', S("abcdefghij22222222222222222222lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, 0, '2', S("abcdefghijpqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, 5, '2', S("abcdefghij22222pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, 10, '2', S("abcdefghij2222222222pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, 20, '2', S("abcdefghij22222222222222222222pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, 0, '2', S("abcdefghijt"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, 5, '2', S("abcdefghij22222t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, 10, '2', S("abcdefghij2222222222t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, 20, '2', S("abcdefghij22222222222222222222t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, 0, '2', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, 5, '2', S("abcdefghij22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, 10, '2', S("abcdefghij2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, 20, '2', S("abcdefghij22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 11, 0, '2', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 11, 5, '2', S("abcdefghij22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 11, 10, '2', S("abcdefghij2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 11, 20, '2', S("abcdefghij22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, 0, '2', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, 5, '2', S("abcdefghijklmnopqrs22222t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, 10, '2', S("abcdefghijklmnopqrs2222222222t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, 20, '2', S("abcdefghijklmnopqrs22222222222222222222t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, 0, '2', S("abcdefghijklmnopqrs"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, 5, '2', S("abcdefghijklmnopqrs22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, 10, '2', S("abcdefghijklmnopqrs2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, 20, '2', S("abcdefghijklmnopqrs22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 2, 0, '2', S("abcdefghijklmnopqrs"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 2, 5, '2', S("abcdefghijklmnopqrs22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 2, 10, '2', S("abcdefghijklmnopqrs2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 2, 20, '2', S("abcdefghijklmnopqrs22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, 0, '2', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, 5, '2', S("abcdefghijklmnopqrst22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, 10, '2', S("abcdefghijklmnopqrst2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, 20, '2', S("abcdefghijklmnopqrst22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 1, 0, '2', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 1, 5, '2', S("abcdefghijklmnopqrst22222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 1, 10, '2', S("abcdefghijklmnopqrst2222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 1, 20, '2', S("abcdefghijklmnopqrst22222222222222222222"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 0, 0, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 0, 5, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 0, 10, '2', S("can't happen"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 21, 0, 20, '2', S("can't happen"))));
    return 0;
}

int tc_libcxx_strings_string_replace_size_size_size_char(void)
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
