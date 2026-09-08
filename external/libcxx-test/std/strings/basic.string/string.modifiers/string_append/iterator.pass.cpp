//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
//   basic_string& append(InputIterator first, InputIterator last);

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "test_iterators.h"

template <class S, class It>
static int
test(S s, It first, It last, S expected)
{
    s.append(first, last);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

int tc_libcxx_strings_string_append_iterator(void)
{
    {
    typedef std::string S;
    const char* s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    TC_ASSERT_FUNC((test(S(), s, s, S())));
    TC_ASSERT_FUNC((test(S(), s, s+1, S("A"))));
    TC_ASSERT_FUNC((test(S(), s, s+10, S("ABCDEFGHIJ"))));
    TC_ASSERT_FUNC((test(S(), s, s+52, S("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"))));

    TC_ASSERT_FUNC((test(S("12345"), s, s, S("12345"))));
    TC_ASSERT_FUNC((test(S("12345"), s, s+1, S("12345A"))));
    TC_ASSERT_FUNC((test(S("12345"), s, s+10, S("12345ABCDEFGHIJ"))));
    TC_ASSERT_FUNC((test(S("12345"), s, s+52, S("12345ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"))));

    TC_ASSERT_FUNC((test(S("1234567890"), s, s, S("1234567890"))));
    TC_ASSERT_FUNC((test(S("1234567890"), s, s+1, S("1234567890A"))));
    TC_ASSERT_FUNC((test(S("1234567890"), s, s+10, S("1234567890ABCDEFGHIJ"))));
    TC_ASSERT_FUNC((test(S("1234567890"), s, s+52, S("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"))));

    TC_ASSERT_FUNC((test(S("12345678901234567890"), s, s, S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), s, s+1, S("12345678901234567890""A"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), s, s+10, S("12345678901234567890""ABCDEFGHIJ"))));
    test(S("12345678901234567890"), s, s+52,
         S("12345678901234567890""ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    TC_ASSERT_FUNC((test(S(), input_iterator<const char*>(s), input_iterator<const char*>(s), S())));
    TC_ASSERT_FUNC((test(S(), input_iterator<const char*>(s), input_iterator<const char*>(s+1), S("A"))));
    test(S(), input_iterator<const char*>(s), input_iterator<const char*>(s+10),
         S("ABCDEFGHIJ"));
    test(S(), input_iterator<const char*>(s), input_iterator<const char*>(s+52),
         S("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S("12345"), input_iterator<const char*>(s), input_iterator<const char*>(s),
         S("12345"));
    test(S("12345"), input_iterator<const char*>(s), input_iterator<const char*>(s+1),
         S("12345A"));
    test(S("12345"), input_iterator<const char*>(s), input_iterator<const char*>(s+10),
         S("12345ABCDEFGHIJ"));
    test(S("12345"), input_iterator<const char*>(s), input_iterator<const char*>(s+52),
         S("12345ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S("1234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s),
         S("1234567890"));
    test(S("1234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s+1),
         S("1234567890A"));
    test(S("1234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s+10),
         S("1234567890ABCDEFGHIJ"));
    test(S("1234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s+52),
         S("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S("12345678901234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s),
         S("12345678901234567890"));
    test(S("12345678901234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s+1),
         S("12345678901234567890""A"));
    test(S("12345678901234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s+10),
         S("12345678901234567890""ABCDEFGHIJ"));
    test(S("12345678901234567890"), input_iterator<const char*>(s), input_iterator<const char*>(s+52),
         S("12345678901234567890""ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
