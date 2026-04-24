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

// vector(vector&& c);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    test_allocator_statistics alloc_stats;
    {
        std::vector<MoveOnly, test_allocator<MoveOnly> > l(test_allocator<MoveOnly>(5, &alloc_stats));
        std::vector<MoveOnly, test_allocator<MoveOnly> > lo(test_allocator<MoveOnly>(5, &alloc_stats));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        std::vector<MoveOnly, test_allocator<MoveOnly> > l2 = std::move(l);
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == lo.get_allocator());
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
        std::vector<MoveOnly, other_allocator<MoveOnly> > l2 = std::move(l);
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == lo.get_allocator());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l2));
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        std::vector<int> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        std::vector<int>::const_iterator i = c1.begin();
        std::vector<int> c2 = std::move(c1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
        std::vector<int>::iterator j = c2.erase(i);
        TC_ASSERT_EXPR(*j == 3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
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
        std::vector<MoveOnly, min_allocator<MoveOnly> > l2 = std::move(l);
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == lo.get_allocator());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l2));
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        std::vector<int, min_allocator<int> > c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        std::vector<int, min_allocator<int> >::const_iterator i = c1.begin();
        std::vector<int, min_allocator<int> > c2 = std::move(c1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
        std::vector<int, min_allocator<int> >::iterator j = c2.erase(i);
        TC_ASSERT_EXPR(*j == 3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
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
      std::vector<MoveOnly, safe_allocator<MoveOnly> > l2 = std::move(l);
      TC_ASSERT_EXPR(l2 == lo);
      TC_ASSERT_EXPR(l.empty());
      TC_ASSERT_EXPR(l2.get_allocator() == lo.get_allocator());
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l2));
    }
    {
      int a1[] = {1, 3, 7, 9, 10};
      std::vector<int, safe_allocator<int> > c1(a1, a1 + sizeof(a1) / sizeof(a1[0]));
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
      std::vector<int, safe_allocator<int> >::const_iterator i = c1.begin();
      std::vector<int, safe_allocator<int> > c2                = std::move(c1);
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
      std::vector<int, safe_allocator<int> >::iterator j = c2.erase(i);
      TC_ASSERT_EXPR(*j == 3);
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
    }
    {
      alloc_stats.clear();
      using Vect = std::vector<int, test_allocator<int> >;
      Vect v(test_allocator<int>(42, 101, &alloc_stats));
      TC_ASSERT_EXPR(alloc_stats.count == 1);
      TC_ASSERT_EXPR(alloc_stats.copied == 1);
      TC_ASSERT_EXPR(alloc_stats.moved == 0);
      {
        const test_allocator<int>& a = v.get_allocator();
        TC_ASSERT_EXPR(a.get_data() == 42);
        TC_ASSERT_EXPR(a.get_id() == 101);
      }
      TC_ASSERT_EXPR(alloc_stats.count == 1);
      alloc_stats.clear_ctor_counters();

      Vect v2 = std::move(v);
      TC_ASSERT_EXPR(alloc_stats.count == 2);
      TC_ASSERT_EXPR(alloc_stats.copied == 0);
      TC_ASSERT_EXPR(alloc_stats.moved == 1);
      {
        const test_allocator<int>& a = v.get_allocator();
        TC_ASSERT_EXPR(a.get_id() == test_alloc_base::moved_value);
        TC_ASSERT_EXPR(a.get_data() == test_alloc_base::moved_value);
      }
      {
        const test_allocator<int>& a = v2.get_allocator();
        TC_ASSERT_EXPR(a.get_id() == 101);
        TC_ASSERT_EXPR(a.get_data() == 42);
      }
    }

    return true;
}

int tc_containers_sequences_vector_vector_cons_move(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
