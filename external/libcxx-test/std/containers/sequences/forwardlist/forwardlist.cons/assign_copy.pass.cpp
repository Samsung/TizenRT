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

// <forward_list>

// forward_list& operator=(const forward_list& x);

#include <forward_list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include <iterator>

#include "test_allocator.h"

int tc_libcxx_containers_forwardlist_cons_assign_copy(void)
{
    {
        typedef int T;
        typedef test_allocator<int> A;
        typedef std::forward_list<T, A> C;
        const T t0[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        const T t1[] = {10, 11, 12, 13};
        C c0(std::begin(t0), std::end(t0), A(10));
        C c1(std::begin(t1), std::end(t1), A(10));
        c1 = c0;
        TC_ASSERT_EXPR(c1 == c0);
        TC_ASSERT_EXPR(c1.get_allocator() == A(10));
    }
    {
        typedef int T;
        typedef test_allocator<int> A;
        typedef std::forward_list<T, A> C;
        const T t0[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        const T t1[] = {10, 11, 12, 13};
        C c0(std::begin(t0), std::end(t0), A(10));
        C c1(std::begin(t1), std::end(t1), A(11));
        c1 = c0;
        TC_ASSERT_EXPR(c1 == c0);
        TC_ASSERT_EXPR(c1.get_allocator() == A(11));
    }
    {
        typedef int T;
        typedef test_allocator<int> A;
        typedef std::forward_list<T, A> C;
        const T t0[] = {10, 11, 12, 13};
        const T t1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c0(std::begin(t0), std::end(t0), A(10));
        C c1(std::begin(t1), std::end(t1), A(10));
        c1 = c0;
        TC_ASSERT_EXPR(c1 == c0);
        TC_ASSERT_EXPR(c1.get_allocator() == A(10));
    }
    {
        typedef int T;
        typedef test_allocator<int> A;
        typedef std::forward_list<T, A> C;
        const T t0[] = {10, 11, 12, 13};
        const T t1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c0(std::begin(t0), std::end(t0), A(10));
        C c1(std::begin(t1), std::end(t1), A(11));
        c1 = c0;
        TC_ASSERT_EXPR(c1 == c0);
        TC_ASSERT_EXPR(c1.get_allocator() == A(11));
    }

    {
        typedef int T;
        typedef other_allocator<int> A;
        typedef std::forward_list<T, A> C;
        const T t0[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        const T t1[] = {10, 11, 12, 13};
        C c0(std::begin(t0), std::end(t0), A(10));
        C c1(std::begin(t1), std::end(t1), A(10));
        c1 = c0;
        TC_ASSERT_EXPR(c1 == c0);
        TC_ASSERT_EXPR(c1.get_allocator() == A(10));
    }
    {
        typedef int T;
        typedef other_allocator<int> A;
        typedef std::forward_list<T, A> C;
        const T t0[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        const T t1[] = {10, 11, 12, 13};
        C c0(std::begin(t0), std::end(t0), A(10));
        C c1(std::begin(t1), std::end(t1), A(11));
        c1 = c0;
        TC_ASSERT_EXPR(c1 == c0);
        TC_ASSERT_EXPR(c1.get_allocator() == A(10));
    }
    {
        typedef int T;
        typedef other_allocator<int> A;
        typedef std::forward_list<T, A> C;
        const T t0[] = {10, 11, 12, 13};
        const T t1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c0(std::begin(t0), std::end(t0), A(10));
        C c1(std::begin(t1), std::end(t1), A(10));
        c1 = c0;
        TC_ASSERT_EXPR(c1 == c0);
        TC_ASSERT_EXPR(c1.get_allocator() == A(10));
    }
    {
        typedef int T;
        typedef other_allocator<int> A;
        typedef std::forward_list<T, A> C;
        const T t0[] = {10, 11, 12, 13};
        const T t1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c0(std::begin(t0), std::end(t0), A(10));
        C c1(std::begin(t1), std::end(t1), A(11));
        c1 = c0;
        TC_ASSERT_EXPR(c1 == c0);
        TC_ASSERT_EXPR(c1.get_allocator() == A(10));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
