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

// static int resize(size_type n);

#include <string>
#include <stdexcept>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::size_type n, S expected)
{
    if (n <= s.max_size())
    {
        s.resize(n);
        LIBCPP_ASSERT(s.__invariants());
        TC_ASSERT_EXPR(s == expected);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            s.resize(n);
            TC_ASSERT_EXPR(false);
        }
        catch (std::length_error&)
        {
            TC_ASSERT_EXPR(n > s.max_size());
        }
    }
#endif
    return 0;
}

int tc_libcxx_strings_string_capacity_resize_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), 0, S())));
    TC_ASSERT_FUNC((test(S(), 1, S(1, '\0'))));
    TC_ASSERT_FUNC((test(S(), 10, S(10, '\0'))));
    TC_ASSERT_FUNC((test(S(), 100, S(100, '\0'))));
    TC_ASSERT_FUNC((test(S("12345"), 0, S())));
    TC_ASSERT_FUNC((test(S("12345"), 2, S("12"))));
    TC_ASSERT_FUNC((test(S("12345"), 5, S("12345"))));
    TC_ASSERT_FUNC((test(S("12345"), 15, S("12345\0\0\0\0\0\0\0\0\0\0", 15))));
    TC_ASSERT_FUNC((test(S("12345678901234567890123456789012345678901234567890"), 0, S())));
    test(S("12345678901234567890123456789012345678901234567890"), 10,
         S("1234567890"));
    test(S("12345678901234567890123456789012345678901234567890"), 50,
         S("12345678901234567890123456789012345678901234567890"));
    test(S("12345678901234567890123456789012345678901234567890"), 60,
         S("12345678901234567890123456789012345678901234567890\0\0\0\0\0\0\0\0\0\0", 60));
    TC_ASSERT_FUNC((test(S(), S::npos, S("not going to happen"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
