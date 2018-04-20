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

#include "test_macros.h"
#include "test_allocator.h"

template <class S>
void
test(S str, unsigned pos)
{
    typedef typename S::traits_type T;
    typedef typename S::allocator_type A;

    if (pos <= str.size())
    {
        S s2(str, pos);
        LIBCPP_ASSERT(s2.__invariants());
        typename S::size_type rlen = str.size() - pos;
        assert(s2.size() == rlen);
        assert(T::compare(s2.data(), str.data() + pos, rlen) == 0);
        assert(s2.get_allocator() == A());
        assert(s2.capacity() >= s2.size());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            S s2(str, pos);
            assert(false);
        }
        catch (std::out_of_range&)
        {
            assert(pos > str.size());
        }
    }
#endif
}

template <class S>
void
test(S str, unsigned pos, unsigned n)
{
    typedef typename S::traits_type T;
    typedef typename S::allocator_type A;
    if (pos <= str.size())
    {
        S s2(str, pos, n);
        LIBCPP_ASSERT(s2.__invariants());
        typename S::size_type rlen = std::min<typename S::size_type>(str.size() - pos, n);
        assert(s2.size() == rlen);
        assert(T::compare(s2.data(), str.data() + pos, rlen) == 0);
        assert(s2.get_allocator() == A());
        assert(s2.capacity() >= s2.size());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            S s2(str, pos, n);
            assert(false);
        }
        catch (std::out_of_range&)
        {
            assert(pos > str.size());
        }
    }
#endif
}

template <class S>
void
test(S str, unsigned pos, unsigned n, const typename S::allocator_type& a)
{
    typedef typename S::traits_type T;

    if (pos <= str.size())
    {
        S s2(str, pos, n, a);
        LIBCPP_ASSERT(s2.__invariants());
        typename S::size_type rlen = std::min<typename S::size_type>(str.size() - pos, n);
        assert(s2.size() == rlen);
        assert(T::compare(s2.data(), str.data() + pos, rlen) == 0);
        assert(s2.get_allocator() == a);
        assert(s2.capacity() >= s2.size());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            S s2(str, pos, n, a);
            assert(false);
        }
        catch (std::out_of_range&)
        {
            assert(pos > str.size());
        }
    }
#endif
}

int main()
{
    {
    typedef test_allocator<char> A;
    typedef std::basic_string<char, std::char_traits<char>, A> S;

    test(S(A(3)), 0);
    test(S(A(3)), 1);
    test(S("1", A(5)), 0);
    test(S("1", A(5)), 1);
    test(S("1", A(5)), 2);
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 0);
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 5);
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50);
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 500);

    test(S(A(3)), 0, 0);
    test(S(A(3)), 0, 1);
    test(S(A(3)), 1, 0);
    test(S(A(3)), 1, 1);
    test(S(A(3)), 1, 2);
    test(S("1", A(5)), 0, 0);
    test(S("1", A(5)), 0, 1);
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 0);
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 1);
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 10);
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 100);

    test(S(A(3)), 0, 0, A(4));
    test(S(A(3)), 0, 1, A(4));
    test(S(A(3)), 1, 0, A(4));
    test(S(A(3)), 1, 1, A(4));
    test(S(A(3)), 1, 2, A(4));
    test(S("1", A(5)), 0, 0, A(6));
    test(S("1", A(5)), 0, 1, A(6));
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 0, A(8));
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 1, A(8));
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 10, A(8));
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)), 50, 100, A(8));
    }
}
