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

// basic_string(const basic_string<charT,traits,Allocator>& str,
//              size_type pos, size_type n,
//              const Allocator& a = Allocator());
//
// basic_string(const basic_string<charT,traits,Allocator>& str,
//              size_type pos,
//              const Allocator& a = Allocator());

#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <scoped_allocator>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"

template <class S>
static int
test(S str, unsigned pos)
{
    typedef typename S::traits_type T;
    typedef typename S::allocator_type A;

    if (pos <= str.size())
    {
        S s2(str, pos);
        LIBCPP_ASSERT(s2.__invariants());
        typename S::size_type rlen = str.size() - pos;
        TC_ASSERT_EXPR(s2.size() == rlen);
        TC_ASSERT_EXPR(T::compare(s2.data(), str.data() + pos, rlen) == 0);
        TC_ASSERT_EXPR(s2.get_allocator() == A());
        TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            S s2(str, pos);
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
test(S str, unsigned pos, unsigned n)
{
    typedef typename S::traits_type T;
    typedef typename S::allocator_type A;
    if (pos <= str.size())
    {
        S s2(str, pos, n);
        LIBCPP_ASSERT(s2.__invariants());
        typename S::size_type rlen = std::min<typename S::size_type>(str.size() - pos, n);
        TC_ASSERT_EXPR(s2.size() == rlen);
        TC_ASSERT_EXPR(T::compare(s2.data(), str.data() + pos, rlen) == 0);
        TC_ASSERT_EXPR(s2.get_allocator() == A());
        TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            S s2(str, pos, n);
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
test(S str, unsigned pos, unsigned n, const typename S::allocator_type& a)
{
    typedef typename S::traits_type T;

    if (pos <= str.size())
    {
        S s2(str, pos, n, a);
        LIBCPP_ASSERT(s2.__invariants());
        typename S::size_type rlen = std::min<typename S::size_type>(str.size() - pos, n);
        TC_ASSERT_EXPR(s2.size() == rlen);
        TC_ASSERT_EXPR(T::compare(s2.data(), str.data() + pos, rlen) == 0);
        TC_ASSERT_EXPR(s2.get_allocator() == a);
        TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            S s2(str, pos, n, a);
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

int tc_libcxx_strings_string_cons_substr(void)
{
    {
    typedef test_allocator<char> A;
    typedef std::basic_string<char, std::char_traits<char>, A> S;

    TC_ASSERT_FUNC((test(S(A(3)), 0)));
    TC_ASSERT_FUNC((test(S(A(3)), 1)));
    TC_ASSERT_FUNC((test(S("1", A(5)), 0)));
    TC_ASSERT_FUNC((test(S("1", A(5)), 1)));
    TC_ASSERT_FUNC((test(S("1", A(5)), 2)));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 0)));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 5)));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50)));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 500)));

    TC_ASSERT_FUNC((test(S(A(3)), 0, 0)));
    TC_ASSERT_FUNC((test(S(A(3)), 0, 1)));
    TC_ASSERT_FUNC((test(S(A(3)), 1, 0)));
    TC_ASSERT_FUNC((test(S(A(3)), 1, 1)));
    TC_ASSERT_FUNC((test(S(A(3)), 1, 2)));
    TC_ASSERT_FUNC((test(S("1", A(5)), 0, 0)));
    TC_ASSERT_FUNC((test(S("1", A(5)), 0, 1)));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 0)));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 1)));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 10)));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 100)));

    TC_ASSERT_FUNC((test(S(A(3)), 0, 0, A(4))));
    TC_ASSERT_FUNC((test(S(A(3)), 0, 1, A(4))));
    TC_ASSERT_FUNC((test(S(A(3)), 1, 0, A(4))));
    TC_ASSERT_FUNC((test(S(A(3)), 1, 1, A(4))));
    TC_ASSERT_FUNC((test(S(A(3)), 1, 2, A(4))));
    TC_ASSERT_FUNC((test(S("1", A(5)), 0, 0, A(6))));
    TC_ASSERT_FUNC((test(S("1", A(5)), 0, 1, A(6))));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 0, A(8))));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 1, A(8))));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 10, A(8))));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 100, A(8))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
