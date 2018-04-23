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

// basic_string<charT,traits,Allocator>& operator=(charT c);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s1, typename S::value_type s2)
{
    typedef typename S::traits_type T;
    s1 = s2;
    LIBCPP_ASSERT(s1.__invariants());
    TC_ASSERT_EXPR(s1.size() == 1);
    TC_ASSERT_EXPR(T::eq(s1[0], s2));
    TC_ASSERT_EXPR(s1.capacity() >= s1.size());
    return 0;
}

int tc_libcxx_strings_string_cons_char_assignment(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), 'a')));
    TC_ASSERT_FUNC((test(S("1"), 'a')));
    TC_ASSERT_FUNC((test(S("123456789"), 'a')));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890"), 'a')));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
