//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"

template <class charT>
static int
test(const charT* s, unsigned n)
{
    typedef std::basic_string<charT, std::char_traits<charT>, test_allocator<charT> > S;
    typedef typename S::traits_type T;
    typedef typename S::allocator_type A;
    S s2(s, n);
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s2.size() == n);
    TC_ASSERT_EXPR(T::compare(s2.data(), s, n) == 0);
    TC_ASSERT_EXPR(s2.get_allocator() == A());
    TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    return 0;
}

template <class charT, class A>
static int
test(const charT* s, unsigned n, const A& a)
{
    typedef std::basic_string<charT, std::char_traits<charT>, A> S;
    typedef typename S::traits_type T;
    S s2(s, n, a);
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s2.size() == n);
    TC_ASSERT_EXPR(T::compare(s2.data(), s, n) == 0);
    TC_ASSERT_EXPR(s2.get_allocator() == a);
    TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    return 0;
}

int tc_libcxx_strings_string_cons_pointer_size_alloc(void)
{
    {
    typedef test_allocator<char> A;

    TC_ASSERT_FUNC((test("", 0)));
    TC_ASSERT_FUNC((test("", 0, A(2))));

    TC_ASSERT_FUNC((test("1", 1)));
    TC_ASSERT_FUNC((test("1", 1, A(2))));

    TC_ASSERT_FUNC((test("1234567980", 10)));
    TC_ASSERT_FUNC((test("1234567980", 10, A(2))));

    TC_ASSERT_FUNC((test("123456798012345679801234567980123456798012345679801234567980", 60)));
    TC_ASSERT_FUNC((test("123456798012345679801234567980123456798012345679801234567980", 60, A(2))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
