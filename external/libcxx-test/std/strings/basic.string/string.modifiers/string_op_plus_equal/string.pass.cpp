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

// basic_string<charT,traits,Allocator>&
//   operator+=(const basic_string<charT,traits,Allocator>& str);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, S str, S expected)
{
    s += str;
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

int tc_libcxx_strings_string_op_plus_equal_string(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), S(), S())));
    TC_ASSERT_FUNC((test(S(), S("12345"), S("12345"))));
    TC_ASSERT_FUNC((test(S(), S("1234567890"), S("1234567890"))));
    TC_ASSERT_FUNC((test(S(), S("12345678901234567890"), S("12345678901234567890"))));

    TC_ASSERT_FUNC((test(S("12345"), S(), S("12345"))));
    TC_ASSERT_FUNC((test(S("12345"), S("12345"), S("1234512345"))));
    TC_ASSERT_FUNC((test(S("12345"), S("1234567890"), S("123451234567890"))));
    TC_ASSERT_FUNC((test(S("12345"), S("12345678901234567890"), S("1234512345678901234567890"))));

    TC_ASSERT_FUNC((test(S("1234567890"), S(), S("1234567890"))));
    TC_ASSERT_FUNC((test(S("1234567890"), S("12345"), S("123456789012345"))));
    TC_ASSERT_FUNC((test(S("1234567890"), S("1234567890"), S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("1234567890"), S("12345678901234567890"), S("123456789012345678901234567890"))));

    TC_ASSERT_FUNC((test(S("12345678901234567890"), S(), S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), S("12345"), S("1234567890123456789012345"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), S("1234567890"), S("123456789012345678901234567890"))));
    test(S("12345678901234567890"), S("12345678901234567890"),
         S("1234567890123456789012345678901234567890"));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
