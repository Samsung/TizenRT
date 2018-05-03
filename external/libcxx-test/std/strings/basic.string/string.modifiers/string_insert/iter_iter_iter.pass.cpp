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
//   iterator insert(const_iterator p, InputIterator first, InputIterator last);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_iterators.h"

template <class S, class It>
static int
test(S s, typename S::difference_type pos, It first, It last, S expected)
{
    typename S::const_iterator p = s.cbegin() + pos;
    typename S::iterator i = s.insert(p, first, last);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(i - s.begin() == pos);
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

int tc_libcxx_strings_string_insert_iter_iter_iter(void)
{
    {
    typedef std::string S;
    const char* s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    TC_ASSERT_FUNC((test(S(), 0, s, s, S())));
    TC_ASSERT_FUNC((test(S(), 0, s, s+1, S("A"))));
    TC_ASSERT_FUNC((test(S(), 0, s, s+10, S("ABCDEFGHIJ"))));
    TC_ASSERT_FUNC((test(S(), 0, s, s+52, S("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"))));

    TC_ASSERT_FUNC((test(S("12345"), 0, s, s, S("12345"))));
    TC_ASSERT_FUNC((test(S("12345"), 1, s, s+1, S("1A2345"))));
    TC_ASSERT_FUNC((test(S("12345"), 4, s, s+10, S("1234ABCDEFGHIJ5"))));
    TC_ASSERT_FUNC((test(S("12345"), 5, s, s+52, S("12345ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"))));

    TC_ASSERT_FUNC((test(S("1234567890"), 0, s, s, S("1234567890"))));
    TC_ASSERT_FUNC((test(S("1234567890"), 1, s, s+1, S("1A234567890"))));
    TC_ASSERT_FUNC((test(S("1234567890"), 10, s, s+10, S("1234567890ABCDEFGHIJ"))));
    TC_ASSERT_FUNC((test(S("1234567890"), 8, s, s+52, S("12345678ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz90"))));

    TC_ASSERT_FUNC((test(S("12345678901234567890"), 3, s, s, S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), 3, s, s+1, S("123A45678901234567890"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), 15, s, s+10, S("123456789012345ABCDEFGHIJ67890"))));
    test(S("12345678901234567890"), 20, s, s+52,
         S("12345678901234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    TC_ASSERT_FUNC((test(S(), 0, input_iterator<const char*>(s), input_iterator<const char*>(s), S())));
    TC_ASSERT_FUNC((test(S(), 0, input_iterator<const char*>(s), input_iterator<const char*>(s+1), S("A"))));
    TC_ASSERT_FUNC((test(S(), 0, input_iterator<const char*>(s), input_iterator<const char*>(s+10), S("ABCDEFGHIJ"))));
    TC_ASSERT_FUNC((test(S(), 0, input_iterator<const char*>(s), input_iterator<const char*>(s+52), S("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"))));

    TC_ASSERT_FUNC((test(S("12345"), 0, input_iterator<const char*>(s), input_iterator<const char*>(s), S("12345"))));
    TC_ASSERT_FUNC((test(S("12345"), 1, input_iterator<const char*>(s), input_iterator<const char*>(s+1), S("1A2345"))));
    TC_ASSERT_FUNC((test(S("12345"), 4, input_iterator<const char*>(s), input_iterator<const char*>(s+10), S("1234ABCDEFGHIJ5"))));
    TC_ASSERT_FUNC((test(S("12345"), 5, input_iterator<const char*>(s), input_iterator<const char*>(s+52), S("12345ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"))));

    TC_ASSERT_FUNC((test(S("1234567890"), 0, input_iterator<const char*>(s), input_iterator<const char*>(s), S("1234567890"))));
    TC_ASSERT_FUNC((test(S("1234567890"), 1, input_iterator<const char*>(s), input_iterator<const char*>(s+1), S("1A234567890"))));
    TC_ASSERT_FUNC((test(S("1234567890"), 10, input_iterator<const char*>(s), input_iterator<const char*>(s+10), S("1234567890ABCDEFGHIJ"))));
    TC_ASSERT_FUNC((test(S("1234567890"), 8, input_iterator<const char*>(s), input_iterator<const char*>(s+52), S("12345678ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz90"))));

    TC_ASSERT_FUNC((test(S("12345678901234567890"), 3, input_iterator<const char*>(s), input_iterator<const char*>(s), S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), 3, input_iterator<const char*>(s), input_iterator<const char*>(s+1), S("123A45678901234567890"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), 15, input_iterator<const char*>(s), input_iterator<const char*>(s+10), S("123456789012345ABCDEFGHIJ67890"))));
    test(S("12345678901234567890"), 20, input_iterator<const char*>(s), input_iterator<const char*>(s+52),
         S("12345678901234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
