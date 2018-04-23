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

// basic_string(size_type n, charT c, const Allocator& a = Allocator());

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
test(unsigned n, charT c)
{
    typedef std::basic_string<charT, std::char_traits<charT>, test_allocator<charT> > S;
    typedef typename S::allocator_type A;
    S s2(n, c);
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s2.size() == n);
    for (unsigned i = 0; i < n; ++i)
        TC_ASSERT_EXPR(s2[i] == c);
    TC_ASSERT_EXPR(s2.get_allocator() == A());
    TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    return 0;
}

template <class charT, class A>
static int
test(unsigned n, charT c, const A& a)
{
    typedef std::basic_string<charT, std::char_traits<charT>, A> S;
    S s2(n, c, a);
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s2.size() == n);
    for (unsigned i = 0; i < n; ++i)
        TC_ASSERT_EXPR(s2[i] == c);
    TC_ASSERT_EXPR(s2.get_allocator() == a);
    TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    return 0;
}

template <class Tp>
static int
test(Tp n, Tp c)
{
    typedef char charT;
    typedef std::basic_string<charT, std::char_traits<charT>, test_allocator<charT> > S;
    typedef typename S::allocator_type A;
    S s2(n, c);
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s2.size() == static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i)
        TC_ASSERT_EXPR(s2[i] == c);
    TC_ASSERT_EXPR(s2.get_allocator() == A());
    TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    return 0;
}

template <class Tp, class A>
static int
test(Tp n, Tp c, const A& a)
{
    typedef char charT;
    typedef std::basic_string<charT, std::char_traits<charT>, A> S;
    S s2(n, c, a);
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s2.size() == static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i)
        TC_ASSERT_EXPR(s2[i] == c);
    TC_ASSERT_EXPR(s2.get_allocator() == a);
    TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    return 0;
}

int tc_libcxx_strings_string_cons_size_char_alloc(void)
{
    {
    typedef test_allocator<char> A;

    TC_ASSERT_FUNC((test(0, 'a')));
    TC_ASSERT_FUNC((test(0, 'a', A(2))));

    TC_ASSERT_FUNC((test(1, 'a')));
    TC_ASSERT_FUNC((test(1, 'a', A(2))));

    TC_ASSERT_FUNC((test(10, 'a')));
    TC_ASSERT_FUNC((test(10, 'a', A(2))));

    TC_ASSERT_FUNC((test(100, 'a')));
    TC_ASSERT_FUNC((test(100, 'a', A(2))));

    TC_ASSERT_FUNC((test(static_cast<char>(100), static_cast<char>(65))));
    TC_ASSERT_FUNC((test(static_cast<char>(100), static_cast<char>(65), A(3))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
