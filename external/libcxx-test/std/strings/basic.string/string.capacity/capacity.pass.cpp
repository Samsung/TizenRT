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

// size_type capacity() const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_allocator.h"

#include "test_macros.h"

template <class S>
static int
test(S s)
{
    S::allocator_type::throw_after = 0;
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
#endif
    {
        while (s.size() < s.capacity())
            s.push_back(typename S::value_type());
        TC_ASSERT_EXPR(s.size() == s.capacity());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    catch (...)
    {
        TC_ASSERT_EXPR(false);
    }
#endif
    S::allocator_type::throw_after = INT_MAX;
    return 0;
}

int tc_libcxx_strings_string_capacity_capacity(void)
{
    {
    typedef std::basic_string<char, std::char_traits<char>, test_allocator<char> > S;
    S s;
    TC_ASSERT_FUNC((test(s)));
    s.assign(10, 'a');
    s.erase(5);
    TC_ASSERT_FUNC((test(s)));
    s.assign(100, 'a');
    s.erase(50);
    TC_ASSERT_FUNC((test(s)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
