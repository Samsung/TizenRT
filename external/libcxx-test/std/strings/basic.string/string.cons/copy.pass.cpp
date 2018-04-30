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

// basic_string(const basic_string<charT,traits,Allocator>& str);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"

template <class S>
static int
test(S s1)
{
    S s2 = s1;
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s2 == s1);
    TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    TC_ASSERT_EXPR(s2.get_allocator() == s1.get_allocator());
    return 0;
}

int tc_libcxx_strings_string_cons_copy(void)
{
    {
    typedef test_allocator<char> A;
    typedef std::basic_string<char, std::char_traits<char>, A> S;
    TC_ASSERT_FUNC((test(S(A(3)))));
    TC_ASSERT_FUNC((test(S("1", A(5)))));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
