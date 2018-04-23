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

// basic_string(const charT* s, const Allocator& a = Allocator());

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>

#include "test_macros.h"
#include "test_allocator.h"

template <class charT>
static int
test(const charT* s)
{
    typedef std::basic_string<charT, std::char_traits<charT>, test_allocator<charT> > S;
    typedef typename S::traits_type T;
    typedef typename S::allocator_type A;
    std::size_t n = T::length(s);
    S s2(s);
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s2.size() == n);
    TC_ASSERT_EXPR(T::compare(s2.data(), s, n) == 0);
    TC_ASSERT_EXPR(s2.get_allocator() == A());
    TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    return 0;
}

template <class charT, class A>
static int
test(const charT* s, const A& a)
{
    typedef std::basic_string<charT, std::char_traits<charT>, A> S;
    typedef typename S::traits_type T;
    std::size_t n = T::length(s);
    S s2(s, a);
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s2.size() == n);
    TC_ASSERT_EXPR(T::compare(s2.data(), s, n) == 0);
    TC_ASSERT_EXPR(s2.get_allocator() == a);
    TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    return 0;
}

int tc_libcxx_strings_string_cons_pointer_alloc(void)
{
    {
    typedef test_allocator<char> A;

    TC_ASSERT_FUNC((test("")));
    TC_ASSERT_FUNC((test("", A(2))));

    TC_ASSERT_FUNC((test("1")));
    TC_ASSERT_FUNC((test("1", A(2))));

    TC_ASSERT_FUNC((test("1234567980")));
    TC_ASSERT_FUNC((test("1234567980", A(2))));

    TC_ASSERT_FUNC((test("123456798012345679801234567980123456798012345679801234567980")));
    TC_ASSERT_FUNC((test("123456798012345679801234567980123456798012345679801234567980", A(2))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
