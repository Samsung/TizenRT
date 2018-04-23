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
//   assign(const charT* s, size_type n);

#include <string>
#include <stdexcept>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, const typename S::value_type* str, typename S::size_type n, S expected)
{
    s.assign(str, n);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

int tc_libcxx_strings_string_assign_pointer_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), "", 0, S())));
    TC_ASSERT_FUNC((test(S(), "12345", 3, S("123"))));
    TC_ASSERT_FUNC((test(S(), "12345", 4, S("1234"))));
    TC_ASSERT_FUNC((test(S(), "12345678901234567890", 0, S())));
    TC_ASSERT_FUNC((test(S(), "12345678901234567890", 1, S("1"))));
    TC_ASSERT_FUNC((test(S(), "12345678901234567890", 3, S("123"))));
    TC_ASSERT_FUNC((test(S(), "12345678901234567890", 20, S("12345678901234567890"))));

    TC_ASSERT_FUNC((test(S("12345"), "", 0, S())));
    TC_ASSERT_FUNC((test(S("12345"), "12345", 5, S("12345"))));
    TC_ASSERT_FUNC((test(S("12345"), "1234567890", 10, S("1234567890"))));

    TC_ASSERT_FUNC((test(S("12345678901234567890"), "", 0, S())));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), "12345", 5, S("12345"))));
    test(S("12345678901234567890"), "12345678901234567890", 20,
         S("12345678901234567890"));
    }
    { // test assign to self
    typedef std::string S;
    S s_short = "123/";
    S s_long  = "Lorem ipsum dolor sit amet, consectetur/";

    s_short.assign(s_short.data(), s_short.size());
    TC_ASSERT_EXPR(s_short == "123/");
    s_short.assign(s_short.data() + 2, s_short.size() - 2);
    TC_ASSERT_EXPR(s_short == "3/");

    s_long.assign(s_long.data(), s_long.size());
    TC_ASSERT_EXPR(s_long == "Lorem ipsum dolor sit amet, consectetur/");

    s_long.assign(s_long.data() + 2, 8 );
    TC_ASSERT_EXPR(s_long == "rem ipsu");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
