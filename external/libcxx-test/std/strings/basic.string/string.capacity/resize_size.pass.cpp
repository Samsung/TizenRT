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
