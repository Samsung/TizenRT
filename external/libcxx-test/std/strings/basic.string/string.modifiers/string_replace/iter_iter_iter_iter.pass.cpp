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

// template<class InputIterator>
//   basic_string&
//   replace(const_iterator i1, const_iterator i2, InputIterator j1, InputIterator j2);

#include <string>
#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_iterators.h"

template <class S, class It>
static int
test(S s, typename S::size_type pos1, typename S::size_type n1, It f, It l, S expected)
{
    typename S::size_type old_size = s.size();
    typename S::const_iterator first = s.begin() + pos1;
    typename S::const_iterator last = s.begin() + pos1 + n1;
    typename S::size_type xlen = last - first;
    s.replace(first, last, f, l);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    typename S::size_type rlen = std::distance(f, l);
    TC_ASSERT_EXPR(s.size() == old_size - xlen + rlen);
    return 0;
}

#ifndef TEST_HAS_NO_EXCEPTIONS
template <class S, class It>
static int
test_exceptions(S s, typename S::size_type pos1, typename S::size_type n1, It f, It l)
{
    typename S::const_iterator first = s.begin() + pos1;
    typename S::const_iterator last = s.begin() + pos1 + n1;
    S aCopy = s;
    try {
        s.replace(first, last, f, l);
        TC_ASSERT_EXPR(false);
        }
    catch (...) {}
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == aCopy);
    return 0;
}
#endif

const char* str = "12345678901234567890";

template <class S>
static int test0()
{
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+1, S("1"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+2, S("12"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+4, S("1234"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+5, S("12345"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+1, S("1"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+5, S("12345"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+9, S("123456789"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+1, S("1"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+19, S("1234567890123456789"))));
    TC_ASSERT_FUNC((test(S(""), 0, 0, str, str+20, S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+1, S("1abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+2, S("12abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+4, S("1234abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+5, S("12345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+1, S("1abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+5, S("12345abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+9, S("123456789abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+10, S("1234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+1, S("1abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+10, S("1234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+19, S("1234567890123456789abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 0, str, str+20, S("12345678901234567890abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+0, S("bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+0, S("bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+1, S("1bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+2, S("12bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+4, S("1234bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+5, S("12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+0, S("bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+1, S("1bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+5, S("12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+9, S("123456789bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+10, S("1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+0, S("bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+1, S("1bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+10, S("1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+19, S("1234567890123456789bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 1, str, str+20, S("12345678901234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+0, S("cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+0, S("cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+1, S("1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+2, S("12cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+4, S("1234cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+5, S("12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+0, S("cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+1, S("1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+5, S("12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+9, S("123456789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+10, S("1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+0, S("cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+1, S("1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+10, S("1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+19, S("1234567890123456789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 2, str, str+20, S("12345678901234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+0, S("e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+0, S("e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+1, S("1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+2, S("12e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+4, S("1234e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+5, S("12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+0, S("e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+1, S("1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+5, S("12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+9, S("123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+10, S("1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+0, S("e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+1, S("1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+10, S("1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+19, S("1234567890123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 4, str, str+20, S("12345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+1, S("1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+2, S("12"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+4, S("1234"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+5, S("12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+1, S("1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+5, S("12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+9, S("123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+1, S("1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+19, S("1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 0, 5, str, str+20, S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+1, S("a1bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+2, S("a12bcde"))));
    return 0;
}

template <class S>
static int test1()
{
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+4, S("a1234bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+5, S("a12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+1, S("a1bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+5, S("a12345bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+9, S("a123456789bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+10, S("a1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+1, S("a1bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+10, S("a1234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+19, S("a1234567890123456789bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 0, str, str+20, S("a12345678901234567890bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+0, S("acde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+0, S("acde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+1, S("a1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+2, S("a12cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+4, S("a1234cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+5, S("a12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+0, S("acde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+1, S("a1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+5, S("a12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+9, S("a123456789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+10, S("a1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+0, S("acde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+1, S("a1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+10, S("a1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+19, S("a1234567890123456789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 1, str, str+20, S("a12345678901234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+0, S("ade"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+0, S("ade"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+1, S("a1de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+2, S("a12de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+4, S("a1234de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+5, S("a12345de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+0, S("ade"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+1, S("a1de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+5, S("a12345de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+9, S("a123456789de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+10, S("a1234567890de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+0, S("ade"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+1, S("a1de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+10, S("a1234567890de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+19, S("a1234567890123456789de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 2, str, str+20, S("a12345678901234567890de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+0, S("ae"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+0, S("ae"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+1, S("a1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+2, S("a12e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+4, S("a1234e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+5, S("a12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+0, S("ae"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+1, S("a1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+5, S("a12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+9, S("a123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+10, S("a1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+0, S("ae"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+1, S("a1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+10, S("a1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+19, S("a1234567890123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 3, str, str+20, S("a12345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+0, S("a"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+0, S("a"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+1, S("a1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+2, S("a12"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+4, S("a1234"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+5, S("a12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+0, S("a"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+1, S("a1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+5, S("a12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+9, S("a123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+10, S("a1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+0, S("a"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+1, S("a1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+10, S("a1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+19, S("a1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, 4, str, str+20, S("a12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+1, S("ab1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+2, S("ab12cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+4, S("ab1234cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+5, S("ab12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+1, S("ab1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+5, S("ab12345cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+9, S("ab123456789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+10, S("ab1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+1, S("ab1cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+10, S("ab1234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+19, S("ab1234567890123456789cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 0, str, str+20, S("ab12345678901234567890cde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+0, S("abde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+0, S("abde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+1, S("ab1de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+2, S("ab12de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+4, S("ab1234de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+5, S("ab12345de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+0, S("abde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+1, S("ab1de"))));
    return 0;
}

template <class S>
static int test2()
{
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+5, S("ab12345de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+9, S("ab123456789de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+10, S("ab1234567890de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+0, S("abde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+1, S("ab1de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+10, S("ab1234567890de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+19, S("ab1234567890123456789de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 1, str, str+20, S("ab12345678901234567890de"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+0, S("abe"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+0, S("abe"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+1, S("ab1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+2, S("ab12e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+4, S("ab1234e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+5, S("ab12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+0, S("abe"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+1, S("ab1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+5, S("ab12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+9, S("ab123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+10, S("ab1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+0, S("abe"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+1, S("ab1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+10, S("ab1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+19, S("ab1234567890123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 2, str, str+20, S("ab12345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+0, S("ab"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+0, S("ab"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+1, S("ab1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+2, S("ab12"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+4, S("ab1234"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+5, S("ab12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+0, S("ab"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+1, S("ab1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+5, S("ab12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+9, S("ab123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+10, S("ab1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+0, S("ab"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+1, S("ab1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+10, S("ab1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+19, S("ab1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, 3, str, str+20, S("ab12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+1, S("abcd1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+2, S("abcd12e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+4, S("abcd1234e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+5, S("abcd12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+1, S("abcd1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+5, S("abcd12345e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+9, S("abcd123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+10, S("abcd1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+1, S("abcd1e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+10, S("abcd1234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+19, S("abcd1234567890123456789e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 0, str, str+20, S("abcd12345678901234567890e"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+0, S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+0, S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+1, S("abcd1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+2, S("abcd12"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+4, S("abcd1234"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+5, S("abcd12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+0, S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+1, S("abcd1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+5, S("abcd12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+9, S("abcd123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+10, S("abcd1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+0, S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+1, S("abcd1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+10, S("abcd1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+19, S("abcd1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, 1, str, str+20, S("abcd12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+2, S("abcde12"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+4, S("abcde1234"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+5, S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+5, S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+9, S("abcde123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+10, S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+10, S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+19, S("abcde1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcde"), 5, 0, str, str+20, S("abcde12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+1, S("1abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+2, S("12abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+4, S("1234abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+5, S("12345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+1, S("1abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+5, S("12345abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+9, S("123456789abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+10, S("1234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+0, S("abcdefghij"))));
    return 0;
}

template <class S>
static int test3()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+1, S("1abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+10, S("1234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+19, S("1234567890123456789abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 0, str, str+20, S("12345678901234567890abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+0, S("bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+0, S("bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+1, S("1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+2, S("12bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+4, S("1234bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+5, S("12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+0, S("bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+1, S("1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+5, S("12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+9, S("123456789bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+10, S("1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+0, S("bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+1, S("1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+10, S("1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+19, S("1234567890123456789bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 1, str, str+20, S("12345678901234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+0, S("fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+0, S("fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+1, S("1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+2, S("12fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+4, S("1234fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+5, S("12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+0, S("fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+1, S("1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+5, S("12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+9, S("123456789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+10, S("1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+0, S("fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+1, S("1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+10, S("1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+19, S("1234567890123456789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 5, str, str+20, S("12345678901234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+0, S("j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+0, S("j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+1, S("1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+2, S("12j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+4, S("1234j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+5, S("12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+0, S("j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+1, S("1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+5, S("12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+9, S("123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+10, S("1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+0, S("j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+1, S("1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+10, S("1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+19, S("1234567890123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 9, str, str+20, S("12345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+1, S("1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+2, S("12"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+4, S("1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+5, S("12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+1, S("1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+5, S("12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+9, S("123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+1, S("1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+19, S("1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, 10, str, str+20, S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+1, S("a1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+2, S("a12bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+4, S("a1234bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+5, S("a12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+1, S("a1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+5, S("a12345bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+9, S("a123456789bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+10, S("a1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+1, S("a1bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+10, S("a1234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+19, S("a1234567890123456789bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 0, str, str+20, S("a12345678901234567890bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+0, S("acdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+0, S("acdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+1, S("a1cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+2, S("a12cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+4, S("a1234cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+5, S("a12345cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+0, S("acdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+1, S("a1cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+5, S("a12345cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+9, S("a123456789cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+10, S("a1234567890cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+0, S("acdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+1, S("a1cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+10, S("a1234567890cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+19, S("a1234567890123456789cdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 1, str, str+20, S("a12345678901234567890cdefghij"))));
    return 0;
}

template <class S>
static int test4()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+0, S("afghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+0, S("afghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+1, S("a1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+2, S("a12fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+4, S("a1234fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+5, S("a12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+0, S("afghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+1, S("a1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+5, S("a12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+9, S("a123456789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+10, S("a1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+0, S("afghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+1, S("a1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+10, S("a1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+19, S("a1234567890123456789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 4, str, str+20, S("a12345678901234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+0, S("aj"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+0, S("aj"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+1, S("a1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+2, S("a12j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+4, S("a1234j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+5, S("a12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+0, S("aj"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+1, S("a1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+5, S("a12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+9, S("a123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+10, S("a1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+0, S("aj"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+1, S("a1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+10, S("a1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+19, S("a1234567890123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 8, str, str+20, S("a12345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+0, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+0, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+1, S("a1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+2, S("a12"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+4, S("a1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+5, S("a12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+0, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+1, S("a1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+5, S("a12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+9, S("a123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+10, S("a1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+0, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+1, S("a1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+10, S("a1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+19, S("a1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, 9, str, str+20, S("a12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+1, S("abcde1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+2, S("abcde12fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+4, S("abcde1234fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+5, S("abcde12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+1, S("abcde1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+5, S("abcde12345fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+9, S("abcde123456789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+10, S("abcde1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+1, S("abcde1fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+10, S("abcde1234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+19, S("abcde1234567890123456789fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 0, str, str+20, S("abcde12345678901234567890fghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+0, S("abcdeghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+0, S("abcdeghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+1, S("abcde1ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+2, S("abcde12ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+4, S("abcde1234ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+5, S("abcde12345ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+0, S("abcdeghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+1, S("abcde1ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+5, S("abcde12345ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+9, S("abcde123456789ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+10, S("abcde1234567890ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+0, S("abcdeghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+1, S("abcde1ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+10, S("abcde1234567890ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+19, S("abcde1234567890123456789ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 1, str, str+20, S("abcde12345678901234567890ghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+0, S("abcdehij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+0, S("abcdehij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+1, S("abcde1hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+2, S("abcde12hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+4, S("abcde1234hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+5, S("abcde12345hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+0, S("abcdehij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+1, S("abcde1hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+5, S("abcde12345hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+9, S("abcde123456789hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+10, S("abcde1234567890hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+0, S("abcdehij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+1, S("abcde1hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+10, S("abcde1234567890hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+19, S("abcde1234567890123456789hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 2, str, str+20, S("abcde12345678901234567890hij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+0, S("abcdej"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+0, S("abcdej"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+1, S("abcde1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+2, S("abcde12j"))));
    return 0;
}

template <class S>
static int test5()
{
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+4, S("abcde1234j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+5, S("abcde12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+0, S("abcdej"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+1, S("abcde1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+5, S("abcde12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+9, S("abcde123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+10, S("abcde1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+0, S("abcdej"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+1, S("abcde1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+10, S("abcde1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+19, S("abcde1234567890123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 4, str, str+20, S("abcde12345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+2, S("abcde12"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+4, S("abcde1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+5, S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+5, S("abcde12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+9, S("abcde123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+10, S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+0, S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+1, S("abcde1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+10, S("abcde1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+19, S("abcde1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, 5, str, str+20, S("abcde12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+1, S("abcdefghi1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+2, S("abcdefghi12j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+4, S("abcdefghi1234j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+5, S("abcdefghi12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+1, S("abcdefghi1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+5, S("abcdefghi12345j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+9, S("abcdefghi123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+10, S("abcdefghi1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+1, S("abcdefghi1j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+10, S("abcdefghi1234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+19, S("abcdefghi1234567890123456789j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 0, str, str+20, S("abcdefghi12345678901234567890j"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+0, S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+0, S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+1, S("abcdefghi1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+2, S("abcdefghi12"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+4, S("abcdefghi1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+5, S("abcdefghi12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+0, S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+1, S("abcdefghi1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+5, S("abcdefghi12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+9, S("abcdefghi123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+10, S("abcdefghi1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+0, S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+1, S("abcdefghi1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+10, S("abcdefghi1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+19, S("abcdefghi1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, 1, str, str+20, S("abcdefghi12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+2, S("abcdefghij12"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+4, S("abcdefghij1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+5, S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+5, S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+9, S("abcdefghij123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+10, S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+10, S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+19, S("abcdefghij1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 10, 0, str, str+20, S("abcdefghij12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+1, S("1abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+2, S("12abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+4, S("1234abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+5, S("12345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+1, S("1abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+5, S("12345abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+9, S("123456789abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+10, S("1234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+1, S("1abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+10, S("1234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+19, S("1234567890123456789abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 0, str, str+20, S("12345678901234567890abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+0, S("bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+0, S("bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+1, S("1bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+2, S("12bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+4, S("1234bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+5, S("12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+0, S("bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+1, S("1bcdefghijklmnopqrst"))));
    return 0;
}

template <class S>
static int test6()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+5, S("12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+9, S("123456789bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+10, S("1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+0, S("bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+1, S("1bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+10, S("1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+19, S("1234567890123456789bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 1, str, str+20, S("12345678901234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+0, S("klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+0, S("klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+1, S("1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+2, S("12klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+4, S("1234klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+5, S("12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+0, S("klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+1, S("1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+5, S("12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+9, S("123456789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+10, S("1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+0, S("klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+1, S("1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+10, S("1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+19, S("1234567890123456789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 10, str, str+20, S("12345678901234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+0, S("t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+0, S("t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+1, S("1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+2, S("12t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+4, S("1234t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+5, S("12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+0, S("t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+1, S("1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+5, S("12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+9, S("123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+10, S("1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+0, S("t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+1, S("1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+10, S("1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+19, S("1234567890123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 19, str, str+20, S("12345678901234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+1, S("1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+2, S("12"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+4, S("1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+5, S("12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+1, S("1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+5, S("12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+9, S("123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+0, S(""))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+1, S("1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+10, S("1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+19, S("1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, 20, str, str+20, S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+1, S("a1bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+2, S("a12bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+4, S("a1234bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+5, S("a12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+1, S("a1bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+5, S("a12345bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+9, S("a123456789bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+10, S("a1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+1, S("a1bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+10, S("a1234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+19, S("a1234567890123456789bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 0, str, str+20, S("a12345678901234567890bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+0, S("acdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+0, S("acdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+1, S("a1cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+2, S("a12cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+4, S("a1234cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+5, S("a12345cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+0, S("acdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+1, S("a1cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+5, S("a12345cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+9, S("a123456789cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+10, S("a1234567890cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+0, S("acdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+1, S("a1cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+10, S("a1234567890cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+19, S("a1234567890123456789cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 1, str, str+20, S("a12345678901234567890cdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+0, S("aklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+0, S("aklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+1, S("a1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+2, S("a12klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+4, S("a1234klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+5, S("a12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+0, S("aklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+1, S("a1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+5, S("a12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+9, S("a123456789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+10, S("a1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+0, S("aklmnopqrst"))));
    return 0;
}

template <class S>
static int test7()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+1, S("a1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+10, S("a1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+19, S("a1234567890123456789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 9, str, str+20, S("a12345678901234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+0, S("at"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+0, S("at"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+1, S("a1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+2, S("a12t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+4, S("a1234t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+5, S("a12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+0, S("at"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+1, S("a1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+5, S("a12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+9, S("a123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+10, S("a1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+0, S("at"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+1, S("a1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+10, S("a1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+19, S("a1234567890123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 18, str, str+20, S("a12345678901234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+0, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+0, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+1, S("a1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+2, S("a12"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+4, S("a1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+5, S("a12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+0, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+1, S("a1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+5, S("a12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+9, S("a123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+10, S("a1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+0, S("a"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+1, S("a1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+10, S("a1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+19, S("a1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, 19, str, str+20, S("a12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+1, S("abcdefghij1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+2, S("abcdefghij12klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+4, S("abcdefghij1234klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+5, S("abcdefghij12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+1, S("abcdefghij1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+5, S("abcdefghij12345klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+9, S("abcdefghij123456789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+10, S("abcdefghij1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+1, S("abcdefghij1klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+10, S("abcdefghij1234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+19, S("abcdefghij1234567890123456789klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 0, str, str+20, S("abcdefghij12345678901234567890klmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+0, S("abcdefghijlmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+0, S("abcdefghijlmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+1, S("abcdefghij1lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+2, S("abcdefghij12lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+4, S("abcdefghij1234lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+5, S("abcdefghij12345lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+0, S("abcdefghijlmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+1, S("abcdefghij1lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+5, S("abcdefghij12345lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+9, S("abcdefghij123456789lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+10, S("abcdefghij1234567890lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+0, S("abcdefghijlmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+1, S("abcdefghij1lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+10, S("abcdefghij1234567890lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+19, S("abcdefghij1234567890123456789lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 1, str, str+20, S("abcdefghij12345678901234567890lmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+0, S("abcdefghijpqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+0, S("abcdefghijpqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+1, S("abcdefghij1pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+2, S("abcdefghij12pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+4, S("abcdefghij1234pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+5, S("abcdefghij12345pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+0, S("abcdefghijpqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+1, S("abcdefghij1pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+5, S("abcdefghij12345pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+9, S("abcdefghij123456789pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+10, S("abcdefghij1234567890pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+0, S("abcdefghijpqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+1, S("abcdefghij1pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+10, S("abcdefghij1234567890pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+19, S("abcdefghij1234567890123456789pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 5, str, str+20, S("abcdefghij12345678901234567890pqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+0, S("abcdefghijt"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+0, S("abcdefghijt"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+1, S("abcdefghij1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+2, S("abcdefghij12t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+4, S("abcdefghij1234t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+5, S("abcdefghij12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+0, S("abcdefghijt"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+1, S("abcdefghij1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+5, S("abcdefghij12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+9, S("abcdefghij123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+10, S("abcdefghij1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+0, S("abcdefghijt"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+1, S("abcdefghij1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+10, S("abcdefghij1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+19, S("abcdefghij1234567890123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 9, str, str+20, S("abcdefghij12345678901234567890t"))));
    return 0;
}

template <class S>
static int test8()
{
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+2, S("abcdefghij12"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+4, S("abcdefghij1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+5, S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+5, S("abcdefghij12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+9, S("abcdefghij123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+10, S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+0, S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+1, S("abcdefghij1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+10, S("abcdefghij1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+19, S("abcdefghij1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, 10, str, str+20, S("abcdefghij12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+1, S("abcdefghijklmnopqrs1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+2, S("abcdefghijklmnopqrs12t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+4, S("abcdefghijklmnopqrs1234t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+5, S("abcdefghijklmnopqrs12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+1, S("abcdefghijklmnopqrs1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+5, S("abcdefghijklmnopqrs12345t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+9, S("abcdefghijklmnopqrs123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+10, S("abcdefghijklmnopqrs1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+1, S("abcdefghijklmnopqrs1t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+10, S("abcdefghijklmnopqrs1234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+19, S("abcdefghijklmnopqrs1234567890123456789t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 0, str, str+20, S("abcdefghijklmnopqrs12345678901234567890t"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+0, S("abcdefghijklmnopqrs"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+0, S("abcdefghijklmnopqrs"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+1, S("abcdefghijklmnopqrs1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+2, S("abcdefghijklmnopqrs12"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+4, S("abcdefghijklmnopqrs1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+5, S("abcdefghijklmnopqrs12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+0, S("abcdefghijklmnopqrs"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+1, S("abcdefghijklmnopqrs1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+5, S("abcdefghijklmnopqrs12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+9, S("abcdefghijklmnopqrs123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+10, S("abcdefghijklmnopqrs1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+0, S("abcdefghijklmnopqrs"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+1, S("abcdefghijklmnopqrs1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+10, S("abcdefghijklmnopqrs1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+19, S("abcdefghijklmnopqrs1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, 1, str, str+20, S("abcdefghijklmnopqrs12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+1, S("abcdefghijklmnopqrst1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+2, S("abcdefghijklmnopqrst12"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+4, S("abcdefghijklmnopqrst1234"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+5, S("abcdefghijklmnopqrst12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+1, S("abcdefghijklmnopqrst1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+5, S("abcdefghijklmnopqrst12345"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+9, S("abcdefghijklmnopqrst123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+10, S("abcdefghijklmnopqrst1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+0, S("abcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+1, S("abcdefghijklmnopqrst1"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+10, S("abcdefghijklmnopqrst1234567890"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+19, S("abcdefghijklmnopqrst1234567890123456789"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 20, 0, str, str+20, S("abcdefghijklmnopqrst12345678901234567890"))));
    return 0;
}

int tc_libcxx_strings_string_replace_iter_iter_iter_iter(void)
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
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    { // test iterator operations that throw
    typedef std::string S;
    typedef ThrowingIterator<char> TIter;
    typedef input_iterator<TIter> IIter;
    const char* s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    TC_ASSERT_FUNC((test_exceptions(S("abcdefghijklmnopqrst"), 10, 5, IIter(TIter(s, s+10, 4, TIter::TAIncrement)), IIter())));
    TC_ASSERT_FUNC((test_exceptions(S("abcdefghijklmnopqrst"), 10, 5, IIter(TIter(s, s+10, 5, TIter::TADereference)), IIter())));
    TC_ASSERT_FUNC((test_exceptions(S("abcdefghijklmnopqrst"), 10, 5, IIter(TIter(s, s+10, 6, TIter::TAComparison)), IIter())));

    TC_ASSERT_FUNC((test_exceptions(S("abcdefghijklmnopqrst"), 10, 5, TIter(s, s+10, 4, TIter::TAIncrement), TIter())));
    TC_ASSERT_FUNC((test_exceptions(S("abcdefghijklmnopqrst"), 10, 5, TIter(s, s+10, 5, TIter::TADereference), TIter())));
    TC_ASSERT_FUNC((test_exceptions(S("abcdefghijklmnopqrst"), 10, 5, TIter(s, s+10, 6, TIter::TAComparison), TIter())));
    }
#endif

    { // test replacing into self
    typedef std::string S;
    S s_short = "123/";
    S s_long  = "Lorem ipsum dolor sit amet, consectetur/";

    s_short.replace(s_short.begin(), s_short.begin(), s_short.begin(), s_short.end());
    TC_ASSERT_EXPR(s_short == "123/123/");
    s_short.replace(s_short.begin(), s_short.begin(), s_short.begin(), s_short.end());
    TC_ASSERT_EXPR(s_short == "123/123/123/123/");
    s_short.replace(s_short.begin(), s_short.begin(), s_short.begin(), s_short.end());
    TC_ASSERT_EXPR(s_short == "123/123/123/123/123/123/123/123/");

    s_long.replace(s_long.begin(), s_long.begin(), s_long.begin(), s_long.end());
    TC_ASSERT_EXPR(s_long == "Lorem ipsum dolor sit amet, consectetur/Lorem ipsum dolor sit amet, consectetur/");
    }

    { // test assigning a different type
    typedef std::string S;
    const uint8_t pc[] = "ABCD";
    uint8_t        p[] = "EFGH";

    S s;
    s.replace(s.begin(), s.end(), pc, pc + 4);
    TC_ASSERT_EXPR(s == "ABCD");

    s.clear();
    s.replace(s.begin(), s.end(), p, p + 4);
    TC_ASSERT_EXPR(s == "EFGH");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
