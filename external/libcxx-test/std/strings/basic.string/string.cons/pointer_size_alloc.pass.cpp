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

// basic_string(const charT* s, size_type n, const Allocator& a = Allocator());

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"

template <class charT>
void
test(const charT* s, unsigned n)
{
    typedef std::basic_string<charT, std::char_traits<charT>, test_allocator<charT> > S;
    typedef typename S::traits_type T;
    typedef typename S::allocator_type A;
    S s2(s, n);
    LIBCPP_ASSERT(s2.__invariants());
    assert(s2.size() == n);
    assert(T::compare(s2.data(), s, n) == 0);
    assert(s2.get_allocator() == A());
    assert(s2.capacity() >= s2.size());
}

template <class charT, class A>
void
test(const charT* s, unsigned n, const A& a)
{
    typedef std::basic_string<charT, std::char_traits<charT>, A> S;
    typedef typename S::traits_type T;
    S s2(s, n, a);
    LIBCPP_ASSERT(s2.__invariants());
    assert(s2.size() == n);
    assert(T::compare(s2.data(), s, n) == 0);
    assert(s2.get_allocator() == a);
    assert(s2.capacity() >= s2.size());
}

int main()
{
    {
    typedef test_allocator<char> A;

    test("", 0);
    test("", 0, A(2));

    test("1", 1);
    test("1", 1, A(2));

    test("1234567980", 10);
    test("1234567980", 10, A(2));

    test("123456798012345679801234567980123456798012345679801234567980", 60);
    test("123456798012345679801234567980123456798012345679801234567980", 60, A(2));
    }
}
