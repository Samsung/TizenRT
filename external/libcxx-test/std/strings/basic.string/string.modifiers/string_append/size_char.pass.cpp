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
//   append(size_type n, charT c);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::size_type n, typename S::value_type c, S expected)
{
    s.append(n, c);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

int tc_libcxx_strings_string_append_size_char(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), 0, 'a', S())));
    TC_ASSERT_FUNC((test(S(), 1, 'a', S(1, 'a'))));
    TC_ASSERT_FUNC((test(S(), 10, 'a', S(10, 'a'))));
    TC_ASSERT_FUNC((test(S(), 100, 'a', S(100, 'a'))));

    TC_ASSERT_FUNC((test(S("12345"), 0, 'a', S("12345"))));
    TC_ASSERT_FUNC((test(S("12345"), 1, 'a', S("12345a"))));
    TC_ASSERT_FUNC((test(S("12345"), 10, 'a', S("12345aaaaaaaaaa"))));

    TC_ASSERT_FUNC((test(S("12345678901234567890"), 0, 'a', S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), 1, 'a', S("12345678901234567890a"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), 10, 'a', S("12345678901234567890aaaaaaaaaa"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
