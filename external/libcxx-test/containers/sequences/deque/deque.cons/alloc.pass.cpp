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

// explicit deque(const allocator_type& a);

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "../../../NotConstructible.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class T, class Allocator>
void
test(const Allocator& a)
{
    std::deque<T, Allocator> d(a);
    TC_ASSERT_EXPR(d.size() == 0);
    TC_ASSERT_EXPR(d.get_allocator() == a);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
}

int tc_containers_sequences_deque_deque_cons_alloc(void) {
    test<int>(std::allocator<int>());
    test<NotConstructible>(test_allocator<NotConstructible>(3));
#if TEST_STD_VER >= 11
    test<int>(min_allocator<int>());
    test<int>(safe_allocator<int>());
    test<NotConstructible>(min_allocator<NotConstructible>{});
    test<NotConstructible>(safe_allocator<NotConstructible>{});
    test<int>(explicit_allocator<int>());
    test<NotConstructible>(explicit_allocator<NotConstructible>{});
#endif

  return 0;
}
