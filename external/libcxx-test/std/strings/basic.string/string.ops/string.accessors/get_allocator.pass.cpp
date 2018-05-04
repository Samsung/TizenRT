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

// allocator_type get_allocator() const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_allocator.h"

template <class S>
static int
test(const S& s, const typename S::allocator_type& a)
{
    TC_ASSERT_EXPR(s.get_allocator() == a);
    return 0;
}

int tc_libcxx_strings_string_accessors_get_allocator(void)
{
    {
    typedef test_allocator<char> A;
    typedef std::basic_string<char, std::char_traits<char>, A> S;
    TC_ASSERT_FUNC((test(S(""), A())));
    TC_ASSERT_FUNC((test(S("abcde", A(1)), A(1))));
    TC_ASSERT_FUNC((test(S("abcdefghij", A(2)), A(2))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst", A(3)), A(3))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
