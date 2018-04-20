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
#include <cstddef>

#include "test_macros.h"
#include "test_allocator.h"

template <class charT>
void
test(unsigned n, charT c)
{
    typedef std::basic_string<charT, std::char_traits<charT>, test_allocator<charT> > S;
    typedef typename S::allocator_type A;
    S s2(n, c);
    LIBCPP_ASSERT(s2.__invariants());
    assert(s2.size() == n);
    for (unsigned i = 0; i < n; ++i)
        assert(s2[i] == c);
    assert(s2.get_allocator() == A());
    assert(s2.capacity() >= s2.size());
}

template <class charT, class A>
void
test(unsigned n, charT c, const A& a)
{
    typedef std::basic_string<charT, std::char_traits<charT>, A> S;
    S s2(n, c, a);
    LIBCPP_ASSERT(s2.__invariants());
    assert(s2.size() == n);
    for (unsigned i = 0; i < n; ++i)
        assert(s2[i] == c);
    assert(s2.get_allocator() == a);
    assert(s2.capacity() >= s2.size());
}

template <class Tp>
void
test(Tp n, Tp c)
{
    typedef char charT;
    typedef std::basic_string<charT, std::char_traits<charT>, test_allocator<charT> > S;
    typedef typename S::allocator_type A;
    S s2(n, c);
    LIBCPP_ASSERT(s2.__invariants());
    assert(s2.size() == static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i)
        assert(s2[i] == c);
    assert(s2.get_allocator() == A());
    assert(s2.capacity() >= s2.size());
}

template <class Tp, class A>
void
test(Tp n, Tp c, const A& a)
{
    typedef char charT;
    typedef std::basic_string<charT, std::char_traits<charT>, A> S;
    S s2(n, c, a);
    LIBCPP_ASSERT(s2.__invariants());
    assert(s2.size() == static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i)
        assert(s2[i] == c);
    assert(s2.get_allocator() == a);
    assert(s2.capacity() >= s2.size());
}

int main()
{
    {
    typedef test_allocator<char> A;

    test(0, 'a');
    test(0, 'a', A(2));

    test(1, 'a');
    test(1, 'a', A(2));

    test(10, 'a');
    test(10, 'a', A(2));

    test(100, 'a');
    test(100, 'a', A(2));

    test(static_cast<char>(100), static_cast<char>(65));
    test(static_cast<char>(100), static_cast<char>(65), A(3));
    }
}
