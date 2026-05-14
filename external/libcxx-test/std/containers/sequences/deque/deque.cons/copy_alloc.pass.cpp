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

// deque(const deque& c, const allocator_type& a);

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class C>
void
test(const C& x, const typename C::allocator_type& a)
{
    C c(x, a);
    TC_ASSERT_EXPR(c == x);
    TC_ASSERT_EXPR(c.get_allocator() == a);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
}

int tc_containers_sequences_deque_deque_cons_copy_alloc(void) {
    {
        int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
        int* an = ab + sizeof(ab)/sizeof(ab[0]);
        test(std::deque<int, test_allocator<int> >(ab, an, test_allocator<int>(3)),
                                                           test_allocator<int>(4));
    }
    {
        int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
        int* an = ab + sizeof(ab)/sizeof(ab[0]);
        test(std::deque<int, other_allocator<int> >(ab, an, other_allocator<int>(3)),
                                                            other_allocator<int>(4));
    }
#if TEST_STD_VER >= 11
    {
        int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
        int* an = ab + sizeof(ab)/sizeof(ab[0]);
        test(std::deque<int, min_allocator<int> >(ab, an, min_allocator<int>()),
                                                          min_allocator<int>());
    }
    {
        int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
        int* an = ab + sizeof(ab)/sizeof(ab[0]);
        test(std::deque<int, safe_allocator<int> >(ab, an, safe_allocator<int>()),
                                                          safe_allocator<int>());
    }
#endif

  return 0;
}
