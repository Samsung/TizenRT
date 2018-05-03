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
//   basic_string& assign(InputIterator first, InputIterator last);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_iterators.h"

template <class S, class It>
static int
test(S s, It first, It last, S expected)
{
    s.assign(first, last);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

#ifndef TEST_HAS_NO_EXCEPTIONS
template <class S, class It>
static int
test_exceptions(S s, It first, It last)
{
    S aCopy = s;
    try {
        s.assign(first, last);
        TC_ASSERT_EXPR(false);
    }
    catch (...) {}
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == aCopy);
    return 0;
}
#endif

int tc_libcxx_strings_string_assign_iterator(void)
{
    {
    typedef std::string S;
    const char* s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    TC_ASSERT_FUNC((test(S(), s, s, S())));
    TC_ASSERT_FUNC((test(S(), s, s+1, S("A"))));
    TC_ASSERT_FUNC((test(S(), s, s+10, S("ABCDEFGHIJ"))));
    TC_ASSERT_FUNC((test(S(), s, s+52, S("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"))));

    TC_ASSERT_FUNC((test(S("12345"), s, s, S())));
    TC_ASSERT_FUNC((test(S("12345"), s, s+1, S("A"))));
    TC_ASSERT_FUNC((test(S("12345"), s, s+10, S("ABCDEFGHIJ"))));
    TC_ASSERT_FUNC((test(S("12345"), s, s+52, S("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"))));

    TC_ASSERT_FUNC((test(S("1234567890"), s, s, S())));
    TC_ASSERT_FUNC((test(S("1234567890"), s, s+1, S("A"))));
    TC_ASSERT_FUNC((test(S("1234567890"), s, s+10, S("ABCDEFGHIJ"))));
    TC_ASSERT_FUNC((test(S("1234567890"), s, s+52, S("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"))));

    TC_ASSERT_FUNC((test(S("12345678901234567890"), s, s, S())));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), s, s+1, S("A"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), s, s+10, S("ABCDEFGHIJ"))));
    test(S("12345678901234567890"), s, s+52,
         S("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    TC_ASSERT_FUNC((test(S(), input_iterator<const char*>(s), input_iterator<const char*>(s), S())));
    TC_ASSERT_FUNC((test(S(), input_iterator<const char*>(s), input_iterator<const char*>(s+1), S("A"))));
    test(S(), input_iterator<const char*>(s), input_iterator<const char*>(s+10),
         S("ABCDEFGHIJ"));
    test(S(), input_iterator<const char*>(s), input_iterator<const char*>(s+52),
         S("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S("12345"), input_iterator<const char*>(s), input_iterator<const char*>(s),
         S());
    test(S("12345"), input_iterator<const char*>(s), input_iterator<const char*>(s+1),
         S("A"));
    test(S("12345"), input_iterator<const char*>(s), input_iterator<const char*>(s+10),
         S("ABCDEFGHIJ"));
    test(S("12345"), input_iterator<const char*>(s), input_iterator<const char*>(s+52),
         S("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S("1234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s),
         S());
    test(S("1234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s+1),
         S("A"));
    test(S("1234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s+10),
         S("ABCDEFGHIJ"));
    test(S("1234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s+52),
         S("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S("12345678901234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s),
         S());
    test(S("12345678901234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s+1),
         S("A"));
    test(S("12345678901234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s+10),
         S("ABCDEFGHIJ"));
    test(S("12345678901234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s+52),
         S("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    { // test iterator operations that throw
    typedef std::string S;
    typedef ThrowingIterator<char> TIter;
    typedef input_iterator<TIter> IIter;
    const char* s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    TC_ASSERT_FUNC((test_exceptions(S(), IIter(TIter(s, s+10, 4, TIter::TAIncrement)), IIter())));
    TC_ASSERT_FUNC((test_exceptions(S(), IIter(TIter(s, s+10, 5, TIter::TADereference)), IIter())));
    TC_ASSERT_FUNC((test_exceptions(S(), IIter(TIter(s, s+10, 6, TIter::TAComparison)), IIter())));

    TC_ASSERT_FUNC((test_exceptions(S(), TIter(s, s+10, 4, TIter::TAIncrement), TIter())));
    TC_ASSERT_FUNC((test_exceptions(S(), TIter(s, s+10, 5, TIter::TADereference), TIter())));
    TC_ASSERT_FUNC((test_exceptions(S(), TIter(s, s+10, 6, TIter::TAComparison), TIter())));
    }
#endif

    { // test assigning to self
    typedef std::string S;
    S s_short = "123/";
    S s_long  = "Lorem ipsum dolor sit amet, consectetur/";

    s_short.assign(s_short.begin(), s_short.end());
    TC_ASSERT_EXPR(s_short == "123/");
    s_short.assign(s_short.begin() + 2, s_short.end());
    TC_ASSERT_EXPR(s_short == "3/");

    s_long.assign(s_long.begin(), s_long.end());
    TC_ASSERT_EXPR(s_long == "Lorem ipsum dolor sit amet, consectetur/");

    s_long.assign(s_long.begin() + 30, s_long.end());
    TC_ASSERT_EXPR(s_long == "nsectetur/");
    }

    { // test assigning a different type
    typedef std::string S;
    const uint8_t p[] = "ABCD";

    S s;
    s.assign(p, p + 4);
    TC_ASSERT_EXPR(s == "ABCD");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
