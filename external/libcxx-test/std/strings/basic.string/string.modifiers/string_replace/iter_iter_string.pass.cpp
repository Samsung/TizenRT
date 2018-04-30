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
//   replace(const_iterator i1, const_iterator i2, const basic_string& str);

#include <string>
#include <algorithm>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::size_type pos1, typename S::size_type n1, S str, S expected)
{
    typename S::size_type old_size = s.size();
    typename S::const_iterator first = s.begin() + pos1;
    typename S::const_iterator last = s.begin() + pos1 + n1;
    typename S::size_type xlen = last - first;
    s.replace(first, last, str);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    typename S::size_type rlen = str.size();
    TC_ASSERT_EXPR(s.size() == old_size - xlen + rlen);
    return 0;
}

template <class S>
static int test0()
{
    TC_ASSERT_FUNC((test(S(""), 0, 0, S(""), S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, S("12345"), S("12345"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, S("1234567890"), S("1234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, S("12345678901234567890"), S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, S(""), S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, S("12345"), S("12345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, S("1234567890"), S("1234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, S("12345678901234567890"), S("12345678901234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, S(""), S("bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, S("12345"), S("12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, S("1234567890"), S("1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, S("12345678901234567890"), S("12345678901234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, S(""), S("cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, S("12345"), S("12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, S("1234567890"), S("1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, S("12345678901234567890"), S("12345678901234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, S(""), S("e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, S("12345"), S("12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, S("1234567890"), S("1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, S("12345678901234567890"), S("12345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, S(""), S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, S("12345"), S("12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, S("1234567890"), S("1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, S("12345678901234567890"), S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, S(""), S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, S("12345"), S("a12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, S("1234567890"), S("a1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, S("12345678901234567890"), S("a12345678901234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, S(""), S("acde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, S("12345"), S("a12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, S("1234567890"), S("a1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, S("12345678901234567890"), S("a12345678901234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, S(""), S("ade"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, S("12345"), S("a12345de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, S("1234567890"), S("a1234567890de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, S("12345678901234567890"), S("a12345678901234567890de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, S(""), S("ae"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, S("12345"), S("a12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, S("1234567890"), S("a1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, S("12345678901234567890"), S("a12345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, S(""), S("a"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, S("12345"), S("a12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, S("1234567890"), S("a1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, S("12345678901234567890"), S("a12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, S(""), S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, S("12345"), S("ab12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, S("1234567890"), S("ab1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, S("12345678901234567890"), S("ab12345678901234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, S(""), S("abde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, S("12345"), S("ab12345de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, S("1234567890"), S("ab1234567890de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, S("12345678901234567890"), S("ab12345678901234567890de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, S(""), S("abe"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, S("12345"), S("ab12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, S("1234567890"), S("ab1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, S("12345678901234567890"), S("ab12345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, S(""), S("ab"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, S("12345"), S("ab12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, S("1234567890"), S("ab1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, S("12345678901234567890"), S("ab12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, S(""), S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, S("12345"), S("abcd12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, S("1234567890"), S("abcd1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, S("12345678901234567890"), S("abcd12345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, S(""), S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, S("12345"), S("abcd12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, S("1234567890"), S("abcd1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, S("12345678901234567890"), S("abcd12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, S(""), S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, S("12345"), S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, S("1234567890"), S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, S("12345678901234567890"), S("abcde12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, S(""), S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, S("12345"), S("12345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, S("1234567890"), S("1234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, S("12345678901234567890"), S("12345678901234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, S(""), S("bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, S("12345"), S("12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, S("1234567890"), S("1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, S("12345678901234567890"), S("12345678901234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, S(""), S("fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, S("12345"), S("12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, S("1234567890"), S("1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, S("12345678901234567890"), S("12345678901234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, S(""), S("j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, S("12345"), S("12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, S("1234567890"), S("1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, S("12345678901234567890"), S("12345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, S(""), S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, S("12345"), S("12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, S("1234567890"), S("1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, S("12345678901234567890"), S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, S(""), S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, S("12345"), S("a12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, S("1234567890"), S("a1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, S("12345678901234567890"), S("a12345678901234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, S(""), S("acdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, S("12345"), S("a12345cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, S("1234567890"), S("a1234567890cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, S("12345678901234567890"), S("a12345678901234567890cdefghij"))));
    return 0;
}

template <class S>
static int test1()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, S(""), S("afghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, S("12345"), S("a12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, S("1234567890"), S("a1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, S("12345678901234567890"), S("a12345678901234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, S(""), S("aj"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, S("12345"), S("a12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, S("1234567890"), S("a1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, S("12345678901234567890"), S("a12345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, S(""), S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, S("12345"), S("a12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, S("1234567890"), S("a1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, S("12345678901234567890"), S("a12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, S(""), S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, S("12345"), S("abcde12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, S("1234567890"), S("abcde1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, S("12345678901234567890"), S("abcde12345678901234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, S(""), S("abcdeghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, S("12345"), S("abcde12345ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, S("1234567890"), S("abcde1234567890ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, S("12345678901234567890"), S("abcde12345678901234567890ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, S(""), S("abcdehij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, S("12345"), S("abcde12345hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, S("1234567890"), S("abcde1234567890hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, S("12345678901234567890"), S("abcde12345678901234567890hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, S(""), S("abcdej"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, S("12345"), S("abcde12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, S("1234567890"), S("abcde1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, S("12345678901234567890"), S("abcde12345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, S(""), S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, S("12345"), S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, S("1234567890"), S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, S("12345678901234567890"), S("abcde12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, S(""), S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, S("12345"), S("abcdefghi12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, S("1234567890"), S("abcdefghi1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, S("12345678901234567890"), S("abcdefghi12345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, S(""), S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, S("12345"), S("abcdefghi12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, S("1234567890"), S("abcdefghi1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, S("12345678901234567890"), S("abcdefghi12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, S(""), S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, S("12345"), S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, S("1234567890"), S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, S("12345678901234567890"), S("abcdefghij12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, S(""), S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, S("12345"), S("12345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, S("1234567890"), S("1234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, S("12345678901234567890"), S("12345678901234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, S(""), S("bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, S("12345"), S("12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, S("1234567890"), S("1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, S("12345678901234567890"), S("12345678901234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, S(""), S("klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, S("12345"), S("12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, S("1234567890"), S("1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, S("12345678901234567890"), S("12345678901234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, S(""), S("t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, S("12345"), S("12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, S("1234567890"), S("1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, S("12345678901234567890"), S("12345678901234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, S(""), S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, S("12345"), S("12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, S("1234567890"), S("1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, S("12345678901234567890"), S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, S(""), S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, S("12345"), S("a12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, S("1234567890"), S("a1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, S("12345678901234567890"), S("a12345678901234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, S(""), S("acdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, S("12345"), S("a12345cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, S("1234567890"), S("a1234567890cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, S("12345678901234567890"), S("a12345678901234567890cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, S(""), S("aklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, S("12345"), S("a12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, S("1234567890"), S("a1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, S("12345678901234567890"), S("a12345678901234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, S(""), S("at"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, S("12345"), S("a12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, S("1234567890"), S("a1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, S("12345678901234567890"), S("a12345678901234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, S(""), S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, S("12345"), S("a12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, S("1234567890"), S("a1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, S("12345678901234567890"), S("a12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, S(""), S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, S("12345"), S("abcdefghij12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, S("1234567890"), S("abcdefghij1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, S("12345678901234567890"), S("abcdefghij12345678901234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, S(""), S("abcdefghijlmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, S("12345"), S("abcdefghij12345lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, S("1234567890"), S("abcdefghij1234567890lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, S("12345678901234567890"), S("abcdefghij12345678901234567890lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, S(""), S("abcdefghijpqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, S("12345"), S("abcdefghij12345pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, S("1234567890"), S("abcdefghij1234567890pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, S("12345678901234567890"), S("abcdefghij12345678901234567890pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, S(""), S("abcdefghijt"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, S("12345"), S("abcdefghij12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, S("1234567890"), S("abcdefghij1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, S("12345678901234567890"), S("abcdefghij12345678901234567890t"))));
    return 0;
}

template <class S>
static int test2()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, S(""), S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, S("12345"), S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, S("1234567890"), S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, S("12345678901234567890"), S("abcdefghij12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, S(""), S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, S("12345"), S("abcdefghijklmnopqrs12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, S("1234567890"), S("abcdefghijklmnopqrs1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, S("12345678901234567890"), S("abcdefghijklmnopqrs12345678901234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, S(""), S("abcdefghijklmnopqrs"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, S("12345"), S("abcdefghijklmnopqrs12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, S("1234567890"), S("abcdefghijklmnopqrs1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, S("12345678901234567890"), S("abcdefghijklmnopqrs12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, S(""), S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, S("12345"), S("abcdefghijklmnopqrst12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, S("1234567890"), S("abcdefghijklmnopqrst1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, S("12345678901234567890"), S("abcdefghijklmnopqrst12345678901234567890"))));
    return 0;
}

int tc_libcxx_strings_string_replace_iter_iter_string(void)
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
