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
// UNSUPPORTED: c++03 && !stdlib=libc++

// <vector>

// vector& operator=(vector&& c);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests() {
    {
        std::vector<MoveOnly, test_allocator<MoveOnly> > l(test_allocator<MoveOnly>(5));
        std::vector<MoveOnly, test_allocator<MoveOnly> > lo(test_allocator<MoveOnly>(5));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        std::vector<MoveOnly, test_allocator<MoveOnly> > l2(test_allocator<MoveOnly>(5));
        l2 = std::move(l);
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == lo.get_allocator());
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
        std::vector<MoveOnly, test_allocator<MoveOnly> > l2(test_allocator<MoveOnly>(6));
        l2 = std::move(l);
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(!l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == test_allocator<MoveOnly>(6));
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
        std::vector<MoveOnly, other_allocator<MoveOnly> > l2(other_allocator<MoveOnly>(6));
        l2 = std::move(l);
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == lo.get_allocator());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l2));
    }
    {
        std::vector<MoveOnly, min_allocator<MoveOnly> > l((min_allocator<MoveOnly>()));
        std::vector<MoveOnly, min_allocator<MoveOnly> > lo((min_allocator<MoveOnly>()));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        std::vector<MoveOnly, min_allocator<MoveOnly> > l2((min_allocator<MoveOnly>()));
        l2 = std::move(l);
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == lo.get_allocator());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l2));
    }
    {
      std::vector<MoveOnly, safe_allocator<MoveOnly> > l((safe_allocator<MoveOnly>()));
      std::vector<MoveOnly, safe_allocator<MoveOnly> > lo((safe_allocator<MoveOnly>()));
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
      for (int i = 1; i <= 3; ++i) {
        l.push_back(i);
        lo.push_back(i);
      }
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
      std::vector<MoveOnly, safe_allocator<MoveOnly> > l2((safe_allocator<MoveOnly>()));
      l2 = std::move(l);
      TC_ASSERT_EXPR(l2 == lo);
      TC_ASSERT_EXPR(l.empty());
      TC_ASSERT_EXPR(l2.get_allocator() == lo.get_allocator());
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l2));
    }

    return true;
}

int tc_containers_sequences_vector_vector_cons_assign_move(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
