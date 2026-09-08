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
// vector<bool>

// static int swap(vector& x);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "test_allocator.h"

int tc_libcxx_containers_vector_bool_swap(void)
{
    {
        std::vector<bool> v1(100);
        std::vector<bool> v2(200);
        v1.swap(v2);
        TC_ASSERT_EXPR(v1.size() == 200);
        TC_ASSERT_EXPR(v1.capacity() >= 200);
        TC_ASSERT_EXPR(v2.size() == 100);
        TC_ASSERT_EXPR(v2.capacity() >= 100);
    }
    {
        typedef test_allocator<bool> A;
        std::vector<bool, A> v1(100, true, A(1, 1));
        std::vector<bool, A> v2(200, false, A(1, 2));
        swap(v1, v2);
        TC_ASSERT_EXPR(v1.size() == 200);
        TC_ASSERT_EXPR(v1.capacity() >= 200);
        TC_ASSERT_EXPR(v2.size() == 100);
        TC_ASSERT_EXPR(v2.capacity() >= 100);
        TC_ASSERT_EXPR(v1.get_allocator().get_id() == 1);
        TC_ASSERT_EXPR(v2.get_allocator().get_id() == 2);
    }
    {
        typedef other_allocator<bool> A;
        std::vector<bool, A> v1(100, true, A(1));
        std::vector<bool, A> v2(200, false, A(2));
        swap(v1, v2);
        TC_ASSERT_EXPR(v1.size() == 200);
        TC_ASSERT_EXPR(v1.capacity() >= 200);
        TC_ASSERT_EXPR(v2.size() == 100);
        TC_ASSERT_EXPR(v2.capacity() >= 100);
        TC_ASSERT_EXPR(v1.get_allocator() == A(2));
        TC_ASSERT_EXPR(v2.get_allocator() == A(1));
    }
    {
        std::vector<bool> v(2);
        std::vector<bool>::reference r1 = v[0];
        std::vector<bool>::reference r2 = v[1];
        r1 = true;
        using std::swap;
        swap(r1, r2);
        TC_ASSERT_EXPR(v[0] == false);
        TC_ASSERT_EXPR(v[1] == true);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
