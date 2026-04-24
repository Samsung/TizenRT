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
// <deque>

// deque(const deque&);

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class C>
void
test(const C& x)
{
    C c(x);
    TC_ASSERT_EXPR(c == x);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(x));
}

int tc_containers_sequences_deque_deque_cons_copy(void) {
    {
        int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
        int* an = ab + sizeof(ab)/sizeof(ab[0]);
        test(std::deque<int>(ab, an));
    }
    {
        std::deque<int, test_allocator<int> > v(3, 2, test_allocator<int>(5));
        std::deque<int, test_allocator<int> > v2 = v;
        TC_ASSERT_EXPR(v2 == v);
        TC_ASSERT_EXPR(v2.get_allocator() == v.get_allocator());
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(v));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(v2));
    }
#if TEST_STD_VER >= 11
    {
        std::deque<int, other_allocator<int> > v(3, 2, other_allocator<int>(5));
        std::deque<int, other_allocator<int> > v2 = v;
        TC_ASSERT_EXPR(v2 == v);
        TC_ASSERT_EXPR(v2.get_allocator() == other_allocator<int>(-2));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(v));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(v2));
    }
    {
        int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
        int* an = ab + sizeof(ab)/sizeof(ab[0]);
        test(std::deque<int, min_allocator<int>>(ab, an));
    }
    {
        std::deque<int, min_allocator<int> > v(3, 2, min_allocator<int>());
        std::deque<int, min_allocator<int> > v2 = v;
        TC_ASSERT_EXPR(v2 == v);
        TC_ASSERT_EXPR(v2.get_allocator() == v.get_allocator());
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(v));
        LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(v2));
    }
#endif

  return 0;
}
