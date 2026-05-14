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
// UNSUPPORTED: c++03

// <deque>

// deque& operator=(deque&& c);

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_deque_deque_cons_move_assign(void) {
    {
        int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
        int* an = ab + sizeof(ab)/sizeof(ab[0]);
        typedef test_allocator<MoveOnly> A;
        std::deque<MoveOnly, A> c1(A(5));
        for (int* p = ab; p < an; ++p)
            c1.push_back(MoveOnly(*p));
        std::deque<MoveOnly, A> c2(A(5));
        for (int* p = ab; p < an; ++p)
            c2.push_back(MoveOnly(*p));
        std::deque<MoveOnly, A> c3(A(5));
        c3 = std::move(c1);
        TC_ASSERT_EXPR(c2 == c3);
        TC_ASSERT_EXPR(c1.size() == 0);
        TC_ASSERT_EXPR(c3.get_allocator() == A(5));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c1));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c2));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c3));
    }
    {
        int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
        int* an = ab + sizeof(ab)/sizeof(ab[0]);
        typedef test_allocator<MoveOnly> A;
        std::deque<MoveOnly, A> c1(A(5));
        for (int* p = ab; p < an; ++p)
            c1.push_back(MoveOnly(*p));
        std::deque<MoveOnly, A> c2(A(5));
        for (int* p = ab; p < an; ++p)
            c2.push_back(MoveOnly(*p));
        std::deque<MoveOnly, A> c3(A(6));
        c3 = std::move(c1);
        TC_ASSERT_EXPR(c2 == c3);
        TC_ASSERT_EXPR(c1.size() != 0);
        TC_ASSERT_EXPR(c3.get_allocator() == A(6));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c1));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c2));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c3));
    }
    {
        int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
        int* an = ab + sizeof(ab)/sizeof(ab[0]);
        typedef other_allocator<MoveOnly> A;
        std::deque<MoveOnly, A> c1(A(5));
        for (int* p = ab; p < an; ++p)
            c1.push_back(MoveOnly(*p));
        std::deque<MoveOnly, A> c2(A(5));
        for (int* p = ab; p < an; ++p)
            c2.push_back(MoveOnly(*p));
        std::deque<MoveOnly, A> c3(A(6));
        c3 = std::move(c1);
        TC_ASSERT_EXPR(c2 == c3);
        TC_ASSERT_EXPR(c1.size() == 0);
        TC_ASSERT_EXPR(c3.get_allocator() == A(5));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c1));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c2));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c3));
    }
    {
        int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
        int* an = ab + sizeof(ab)/sizeof(ab[0]);
        typedef min_allocator<MoveOnly> A;
        std::deque<MoveOnly, A> c1(A{});
        for (int* p = ab; p < an; ++p)
            c1.push_back(MoveOnly(*p));
        std::deque<MoveOnly, A> c2(A{});
        for (int* p = ab; p < an; ++p)
            c2.push_back(MoveOnly(*p));
        std::deque<MoveOnly, A> c3(A{});
        c3 = std::move(c1);
        TC_ASSERT_EXPR(c2 == c3);
        TC_ASSERT_EXPR(c1.size() == 0);
        TC_ASSERT_EXPR(c3.get_allocator() == A());
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c1));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c2));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c3));
    }

  return 0;
}
