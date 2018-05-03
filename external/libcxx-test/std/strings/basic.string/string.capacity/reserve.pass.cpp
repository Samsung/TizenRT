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

// static int reserve(size_type res_arg=0);

#include <string>
#include <stdexcept>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s)
{
    typename S::size_type old_cap = s.capacity();
    S s0 = s;
    s.reserve();
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == s0);
    TC_ASSERT_EXPR(s.capacity() <= old_cap);
    TC_ASSERT_EXPR(s.capacity() >= s.size());
    return 0;
}

template <class S>
static int
test(S s, typename S::size_type res_arg)
{
    typename S::size_type old_cap = s.capacity();
    ((void)old_cap); // Prevent unused warning
    S s0 = s;
    if (res_arg <= s.max_size())
    {
        s.reserve(res_arg);
        TC_ASSERT_EXPR(s == s0);
        TC_ASSERT_EXPR(s.capacity() >= res_arg);
        TC_ASSERT_EXPR(s.capacity() >= s.size());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            s.reserve(res_arg);
            TC_ASSERT_EXPR(false);
        }
        catch (std::length_error&)
        {
            TC_ASSERT_EXPR(res_arg > s.max_size());
        }
    }
#endif
    return 0;
}

int tc_libcxx_strings_string_capacity_reserve(void)
{
    {
    typedef std::string S;
    {
    S s;
    TC_ASSERT_FUNC((test(s)));

    s.assign(10, 'a');
    s.erase(5);
    TC_ASSERT_FUNC((test(s)));

    s.assign(100, 'a');
    s.erase(50);
    TC_ASSERT_FUNC((test(s)));
    }
    {
    S s;
    TC_ASSERT_FUNC((test(s, 5)));
    TC_ASSERT_FUNC((test(s, 10)));
    TC_ASSERT_FUNC((test(s, 50)));
    }
    {
    S s(100, 'a');
    s.erase(50);
    TC_ASSERT_FUNC((test(s, 5)));
    TC_ASSERT_FUNC((test(s, 10)));
    TC_ASSERT_FUNC((test(s, 50)));
    TC_ASSERT_FUNC((test(s, 100)));
    TC_ASSERT_FUNC((test(s, S::npos)));
    }
    }
    TC_SUCCESS_RESULT();
    return 0;
}
