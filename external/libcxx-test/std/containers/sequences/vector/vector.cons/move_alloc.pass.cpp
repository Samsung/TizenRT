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

// UNSUPPORTED: c++98, c++03

// <vector>

// vector(vector&& c, const allocator_type& a);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "MoveOnly.h"
#include "test_allocator.h"
#include "asan_testing.h"

int tc_libcxx_containers_vector_cons_move_alloc(void)
{
    {
        std::vector<MoveOnly, test_allocator<MoveOnly> > l(test_allocator<MoveOnly>(5));
        std::vector<MoveOnly, test_allocator<MoveOnly> > lo(test_allocator<MoveOnly>(5));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        std::vector<MoveOnly, test_allocator<MoveOnly> > l2(std::move(l), test_allocator<MoveOnly>(6));
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(!l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == test_allocator<MoveOnly>(6));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l2));
    }
    {
        std::vector<MoveOnly, test_allocator<MoveOnly> > l(test_allocator<MoveOnly>(5));
        std::vector<MoveOnly, test_allocator<MoveOnly> > lo(test_allocator<MoveOnly>(5));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        std::vector<MoveOnly, test_allocator<MoveOnly> > l2(std::move(l), test_allocator<MoveOnly>(5));
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == test_allocator<MoveOnly>(5));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l2));
    }
    {
        std::vector<MoveOnly, other_allocator<MoveOnly> > l(other_allocator<MoveOnly>(5));
        std::vector<MoveOnly, other_allocator<MoveOnly> > lo(other_allocator<MoveOnly>(5));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        std::vector<MoveOnly, other_allocator<MoveOnly> > l2(std::move(l), other_allocator<MoveOnly>(4));
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(!l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == other_allocator<MoveOnly>(4));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l2));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
