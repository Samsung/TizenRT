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

// <vector>

// vector(const vector& v);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"
#include "asan_testing.h"

template <class C>
static int
test(const C& x)
{
    typename C::size_type s = x.size();
    C c(x);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.size() == s);
    TC_ASSERT_EXPR(c == x);
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
    return 0;
}

int tc_libcxx_containers_vector_cons_copy(void)
{
    {
        int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 1, 0};
        int* an = a + sizeof(a)/sizeof(a[0]);
        test(std::vector<int>(a, an));
    }
    {
        std::vector<int, test_allocator<int> > v(3, 2, test_allocator<int>(5));
        std::vector<int, test_allocator<int> > v2 = v;
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v2));
        TC_ASSERT_EXPR(v2 == v);
        TC_ASSERT_EXPR(v2.get_allocator() == v.get_allocator());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v2));
    }
#if TEST_STD_VER >= 11
    {
        std::vector<int, other_allocator<int> > v(3, 2, other_allocator<int>(5));
        std::vector<int, other_allocator<int> > v2 = v;
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v2));
        TC_ASSERT_EXPR(v2 == v);
        TC_ASSERT_EXPR(v2.get_allocator() == other_allocator<int>(-2));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v2));
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
