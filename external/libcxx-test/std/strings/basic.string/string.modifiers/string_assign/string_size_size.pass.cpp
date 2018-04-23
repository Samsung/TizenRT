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
//   assign(const basic_string<charT,traits>& str, size_type pos, size_type n=npos);
// the =npos was added for C++14

#include <string>
#include <stdexcept>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, S str, typename S::size_type pos, typename S::size_type n, S expected)
{
    if (pos <= str.size())
    {
        s.assign(str, pos, n);
        LIBCPP_ASSERT(s.__invariants());
        TC_ASSERT_EXPR(s == expected);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            s.assign(str, pos, n);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
            TC_ASSERT_EXPR(pos > str.size());
        }
    }
#endif
    return 0;
}

template <class S>
static int
test_npos(S s, S str, typename S::size_type pos, S expected)
{
    if (pos <= str.size())
    {
        s.assign(str, pos);
        LIBCPP_ASSERT(s.__invariants());
        TC_ASSERT_EXPR(s == expected);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            s.assign(str, pos);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
            TC_ASSERT_EXPR(pos > str.size());
        }
    }
#endif
    return 0;
}

int tc_libcxx_strings_string_assign_string_size_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), S(), 0, 0, S())));
    TC_ASSERT_FUNC((test(S(), S(), 1, 0, S())));
    TC_ASSERT_FUNC((test(S(), S("12345"), 0, 3, S("123"))));
    TC_ASSERT_FUNC((test(S(), S("12345"), 1, 4, S("2345"))));
    TC_ASSERT_FUNC((test(S(), S("12345"), 3, 15, S("45"))));
    TC_ASSERT_FUNC((test(S(), S("12345"), 5, 15, S(""))));
    TC_ASSERT_FUNC((test(S(), S("12345"), 6, 15, S("not happening"))));
    TC_ASSERT_FUNC((test(S(), S("12345678901234567890"), 0, 0, S())));
    TC_ASSERT_FUNC((test(S(), S("12345678901234567890"), 1, 1, S("2"))));
    TC_ASSERT_FUNC((test(S(), S("12345678901234567890"), 2, 3, S("345"))));
    TC_ASSERT_FUNC((test(S(), S("12345678901234567890"), 12, 13, S("34567890"))));
    TC_ASSERT_FUNC((test(S(), S("12345678901234567890"), 21, 13, S("not happening"))));

    TC_ASSERT_FUNC((test(S("12345"), S(), 0, 0, S())));
    TC_ASSERT_FUNC((test(S("12345"), S("12345"), 2, 2, S("34"))));
    TC_ASSERT_FUNC((test(S("12345"), S("1234567890"), 0, 100, S("1234567890"))));

    TC_ASSERT_FUNC((test(S("12345678901234567890"), S(), 0, 0, S())));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), S("12345"), 1, 3, S("234"))));
    test(S("12345678901234567890"), S("12345678901234567890"), 5, 10,
         S("6789012345"));
    }
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test_npos(S(), S(), 0, S())));
    TC_ASSERT_FUNC((test_npos(S(), S(), 1, S())));
    TC_ASSERT_FUNC((test_npos(S(), S("12345"), 0, S("12345"))));
    TC_ASSERT_FUNC((test_npos(S(), S("12345"), 1, S("2345"))));
    TC_ASSERT_FUNC((test_npos(S(), S("12345"), 3, S("45"))));
    TC_ASSERT_FUNC((test_npos(S(), S("12345"), 5, S(""))));
    TC_ASSERT_FUNC((test_npos(S(), S("12345"), 6, S("not happening"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
