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
//   replace(const_iterator i1, const_iterator i2, size_type n, charT c);

#include <string>
#include <algorithm>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::size_type pos1, typename S::size_type n1, typename S::size_type n2,
     typename S::value_type c, S expected)
{
    typename S::size_type old_size = s.size();
    typename S::const_iterator first = s.begin() + pos1;
    typename S::const_iterator last = s.begin() + pos1 + n1;
    typename S::size_type xlen = last - first;
    s.replace(first, last, n2, c);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    typename S::size_type rlen = n2;
    TC_ASSERT_EXPR(s.size() == old_size - xlen + rlen);
    return 0;
}

template <class S>
static int test0()
{
    TC_ASSERT_FUNC((test(S(""), 0, 0, 0, '3', S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, 5, '3', S("33333"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, 10, '3', S("3333333333"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, 20, '3', S("33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, 0, '3', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, 5, '3', S("33333abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, 10, '3', S("3333333333abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, 20, '3', S("33333333333333333333abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, 0, '3', S("bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, 5, '3', S("33333bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, 10, '3', S("3333333333bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, 20, '3', S("33333333333333333333bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, 0, '3', S("cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, 5, '3', S("33333cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, 10, '3', S("3333333333cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, 20, '3', S("33333333333333333333cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, 0, '3', S("e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, 5, '3', S("33333e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, 10, '3', S("3333333333e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, 20, '3', S("33333333333333333333e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, 0, '3', S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, 5, '3', S("33333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, 10, '3', S("3333333333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, 20, '3', S("33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, 0, '3', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, 5, '3', S("a33333bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, 10, '3', S("a3333333333bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, 20, '3', S("a33333333333333333333bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, 0, '3', S("acde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, 5, '3', S("a33333cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, 10, '3', S("a3333333333cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, 20, '3', S("a33333333333333333333cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, 0, '3', S("ade"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, 5, '3', S("a33333de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, 10, '3', S("a3333333333de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, 20, '3', S("a33333333333333333333de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, 0, '3', S("ae"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, 5, '3', S("a33333e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, 10, '3', S("a3333333333e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, 20, '3', S("a33333333333333333333e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, 0, '3', S("a"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, 5, '3', S("a33333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, 10, '3', S("a3333333333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, 20, '3', S("a33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, 0, '3', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, 5, '3', S("ab33333cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, 10, '3', S("ab3333333333cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, 20, '3', S("ab33333333333333333333cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, 0, '3', S("abde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, 5, '3', S("ab33333de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, 10, '3', S("ab3333333333de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, 20, '3', S("ab33333333333333333333de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, 0, '3', S("abe"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, 5, '3', S("ab33333e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, 10, '3', S("ab3333333333e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, 20, '3', S("ab33333333333333333333e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, 0, '3', S("ab"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, 5, '3', S("ab33333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, 10, '3', S("ab3333333333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, 20, '3', S("ab33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, 0, '3', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, 5, '3', S("abcd33333e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, 10, '3', S("abcd3333333333e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, 20, '3', S("abcd33333333333333333333e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, 0, '3', S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, 5, '3', S("abcd33333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, 10, '3', S("abcd3333333333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, 20, '3', S("abcd33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, 0, '3', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, 5, '3', S("abcde33333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, 10, '3', S("abcde3333333333"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, 20, '3', S("abcde33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, 0, '3', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, 5, '3', S("33333abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, 10, '3', S("3333333333abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, 20, '3', S("33333333333333333333abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, 0, '3', S("bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, 5, '3', S("33333bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, 10, '3', S("3333333333bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, 20, '3', S("33333333333333333333bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, 0, '3', S("fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, 5, '3', S("33333fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, 10, '3', S("3333333333fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, 20, '3', S("33333333333333333333fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, 0, '3', S("j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, 5, '3', S("33333j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, 10, '3', S("3333333333j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, 20, '3', S("33333333333333333333j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, 0, '3', S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, 5, '3', S("33333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, 10, '3', S("3333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, 20, '3', S("33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, 0, '3', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, 5, '3', S("a33333bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, 10, '3', S("a3333333333bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, 20, '3', S("a33333333333333333333bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, 0, '3', S("acdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, 5, '3', S("a33333cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, 10, '3', S("a3333333333cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, 20, '3', S("a33333333333333333333cdefghij"))));
    return 0;
}

template <class S>
static int test1()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, 0, '3', S("afghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, 5, '3', S("a33333fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, 10, '3', S("a3333333333fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, 20, '3', S("a33333333333333333333fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, 0, '3', S("aj"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, 5, '3', S("a33333j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, 10, '3', S("a3333333333j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, 20, '3', S("a33333333333333333333j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, 0, '3', S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, 5, '3', S("a33333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, 10, '3', S("a3333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, 20, '3', S("a33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, 0, '3', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, 5, '3', S("abcde33333fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, 10, '3', S("abcde3333333333fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, 20, '3', S("abcde33333333333333333333fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, 0, '3', S("abcdeghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, 5, '3', S("abcde33333ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, 10, '3', S("abcde3333333333ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, 20, '3', S("abcde33333333333333333333ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, 0, '3', S("abcdehij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, 5, '3', S("abcde33333hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, 10, '3', S("abcde3333333333hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, 20, '3', S("abcde33333333333333333333hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, 0, '3', S("abcdej"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, 5, '3', S("abcde33333j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, 10, '3', S("abcde3333333333j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, 20, '3', S("abcde33333333333333333333j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, 0, '3', S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, 5, '3', S("abcde33333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, 10, '3', S("abcde3333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, 20, '3', S("abcde33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, 0, '3', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, 5, '3', S("abcdefghi33333j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, 10, '3', S("abcdefghi3333333333j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, 20, '3', S("abcdefghi33333333333333333333j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, 0, '3', S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, 5, '3', S("abcdefghi33333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, 10, '3', S("abcdefghi3333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, 20, '3', S("abcdefghi33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, 0, '3', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, 5, '3', S("abcdefghij33333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, 10, '3', S("abcdefghij3333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, 20, '3', S("abcdefghij33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, 0, '3', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, 5, '3', S("33333abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, 10, '3', S("3333333333abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, 20, '3', S("33333333333333333333abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, 0, '3', S("bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, 5, '3', S("33333bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, 10, '3', S("3333333333bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, 20, '3', S("33333333333333333333bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, 0, '3', S("klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, 5, '3', S("33333klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, 10, '3', S("3333333333klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, 20, '3', S("33333333333333333333klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, 0, '3', S("t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, 5, '3', S("33333t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, 10, '3', S("3333333333t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, 20, '3', S("33333333333333333333t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, 0, '3', S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, 5, '3', S("33333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, 10, '3', S("3333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, 20, '3', S("33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, 0, '3', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, 5, '3', S("a33333bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, 10, '3', S("a3333333333bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, 20, '3', S("a33333333333333333333bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, 0, '3', S("acdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, 5, '3', S("a33333cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, 10, '3', S("a3333333333cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, 20, '3', S("a33333333333333333333cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, 0, '3', S("aklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, 5, '3', S("a33333klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, 10, '3', S("a3333333333klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, 20, '3', S("a33333333333333333333klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, 0, '3', S("at"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, 5, '3', S("a33333t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, 10, '3', S("a3333333333t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, 20, '3', S("a33333333333333333333t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, 0, '3', S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, 5, '3', S("a33333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, 10, '3', S("a3333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, 20, '3', S("a33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, 0, '3', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, 5, '3', S("abcdefghij33333klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, 10, '3', S("abcdefghij3333333333klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, 20, '3', S("abcdefghij33333333333333333333klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, 0, '3', S("abcdefghijlmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, 5, '3', S("abcdefghij33333lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, 10, '3', S("abcdefghij3333333333lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, 20, '3', S("abcdefghij33333333333333333333lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, 0, '3', S("abcdefghijpqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, 5, '3', S("abcdefghij33333pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, 10, '3', S("abcdefghij3333333333pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, 20, '3', S("abcdefghij33333333333333333333pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, 0, '3', S("abcdefghijt"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, 5, '3', S("abcdefghij33333t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, 10, '3', S("abcdefghij3333333333t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, 20, '3', S("abcdefghij33333333333333333333t"))));
    return 0;
}

template <class S>
static int test2()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, 0, '3', S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, 5, '3', S("abcdefghij33333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, 10, '3', S("abcdefghij3333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, 20, '3', S("abcdefghij33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, 0, '3', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, 5, '3', S("abcdefghijklmnopqrs33333t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, 10, '3', S("abcdefghijklmnopqrs3333333333t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, 20, '3', S("abcdefghijklmnopqrs33333333333333333333t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, 0, '3', S("abcdefghijklmnopqrs"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, 5, '3', S("abcdefghijklmnopqrs33333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, 10, '3', S("abcdefghijklmnopqrs3333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, 20, '3', S("abcdefghijklmnopqrs33333333333333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, 0, '3', S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, 5, '3', S("abcdefghijklmnopqrst33333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, 10, '3', S("abcdefghijklmnopqrst3333333333"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, 20, '3', S("abcdefghijklmnopqrst33333333333333333333"))));
    return 0;
}

int tc_libcxx_strings_string_replace_iter_iter_size_char(void)
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
